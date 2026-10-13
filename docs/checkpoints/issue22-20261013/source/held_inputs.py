"""UNEXECUTED held input owner. No admission, launch or public proof API."""
from __future__ import annotations
import ctypes
import hashlib
import os
import stat
from dataclasses import dataclass
from pathlib import Path

MAX_INPUT_BYTES = 1024 * 1024 * 1024
MAX_PARSER_BYTES = 64 * 1024 * 1024
MAX_ENTRIES = 65536
MAX_HANDLES = 65536
MAX_OBSERVATIONS = 4096
READ_CHUNK = 65536

@dataclass
class Cell:
    path: str = ""
    fd: int | None = None
    native: int | None = None
    scanner: object | None = None
    close_attempted: bool = False
    closed: bool = False
    close_error: BaseException | None = None

@dataclass(frozen=True)
class FileObservation:
    ordinal: int
    path: str
    before: tuple
    after: tuple
    named: tuple
    size: int
    sha256: str

@dataclass(frozen=True)
class DirectoryObservation:
    ordinal: int
    path: str
    object: tuple
    members: tuple

@dataclass
class Failure:
    phase: str | None = None
    operation: str | None = None
    path: str | None = None
    original: BaseException | None = None

class AccountingExhausted(RuntimeError):
    pass

def identity(s):
    return (s.st_dev, s.st_ino, s.st_mode, s.st_nlink, s.st_uid, s.st_gid,
            s.st_size, s.st_mtime_ns, s.st_ctime_ns,
            getattr(s, "st_file_attributes", 0), getattr(s, "st_reparse_tag", 0))

def object_identity(s):
    return (s.st_dev, s.st_ino, stat.S_IFMT(s.st_mode),
            getattr(s, "st_file_attributes", 0), getattr(s, "st_reparse_tag", 0))

def require_object(s, directory):
    if stat.S_ISLNK(s.st_mode) or getattr(s, "st_file_attributes", 0) & 0x400:
        raise ValueError("link/reparse input rejected")
    if not (stat.S_ISDIR(s.st_mode) if directory else stat.S_ISREG(s.st_mode)):
        raise ValueError("unexpected object type")
    if not directory and s.st_nlink != 1:
        raise ValueError("multiple-link input rejected")

class HeldInputs:
    def __init__(self):
        # Preallocate capability/error cells BEFORE any native acquisition.
        # Actual whole Python/runtime memory fit still needs measured evidence.
        self._cells = [Cell() for _ in range(MAX_HANDLES)]
        self._used = 0
        self._files = [None] * MAX_OBSERVATIONS
        self._public_files = [None] * MAX_OBSERVATIONS
        self._file_count = 0
        self._directories = [None] * MAX_OBSERVATIONS
        self._directory_count = 0
        self._retained_members = 0
        self._failures = [Failure() for _ in range(MAX_HANDLES)]
        self._failure_count = 0
        self._emergency = None
        self._recording_error = None

    def _record(self, phase, operation, path, original):
        if self._failure_count == len(self._failures):
            if self._emergency is None:
                self._emergency = original
            return
        try:
            slot = self._failures[self._failure_count]
            # Original first, in an already allocated error cell. Logging must
            # not construct a Failure after a native effect or interrupt later
            # independent eligible closes if interpreter bookkeeping fails.
            slot.original = original
            slot.phase, slot.operation, slot.path = phase, operation, path
            self._failure_count += 1
        except BaseException as recording_error:
            if self._emergency is None:
                self._emergency = original
            if self._recording_error is None:
                self._recording_error = recording_error

    def _reserve(self, path):
        if self._emergency is not None or self._failure_count >= len(self._failures) - 1:
            raise RuntimeError("error accounting exhausted")
        if self._used == len(self._cells):
            raise RuntimeError("capability accounting exhausted")
        ordinal = self._used
        cell = self._cells[ordinal]
        cell.path = path
        self._used += 1
        return ordinal, cell

    def _open(self, path, directory, parent=None, leaf=None):
        before = os.lstat(path)
        require_object(before, directory)
        ordinal, cell = self._reserve(path)
        if os.name == "nt":
            import msvcrt
            kernel = ctypes.WinDLL("kernel32", use_last_error=True)
            create = kernel.CreateFileW
            create.argtypes = [ctypes.c_wchar_p, ctypes.c_uint32, ctypes.c_uint32,
                               ctypes.c_void_p, ctypes.c_uint32, ctypes.c_uint32,
                               ctypes.c_void_p]
            create.restype = ctypes.c_void_p
            handle = create(path, 0x80000000, 1, None, 3,
                            0x00200000 | (0x02000000 if directory else 0), None)
            if handle == ctypes.c_void_p(-1).value:
                raise ctypes.WinError(ctypes.get_last_error())
            cell.native = handle
            # Failed CRT conversion keeps original native capability owned here.
            cell.fd = msvcrt.open_osfhandle(handle, os.O_RDONLY | os.O_BINARY)
            cell.native = None
        else:
            flags = os.O_RDONLY | os.O_CLOEXEC | os.O_NOFOLLOW
            if directory:
                flags |= os.O_DIRECTORY
            cell.fd = os.open(leaf if parent is not None else path, flags, dir_fd=parent)
        held, named = os.fstat(cell.fd), os.lstat(path)
        require_object(held, directory)
        require_object(named, directory)
        if object_identity(before) != object_identity(held) or object_identity(named) != object_identity(held):
            raise ValueError("named/held object replacement")
        return ordinal

    def _parents(self, path):
        if type(path) is not str or len(path) > 32768 or "\0" in path:
            raise ValueError("bounded literal original path required")
        absolute = Path(path)
        if not absolute.is_absolute() or ".." in absolute.parts or not absolute.name:
            raise ValueError("literal absolute leaf required")
        previous = None
        for parent in reversed(absolute.parents):
            ordinal = self._open(str(parent), True, previous,
                                 parent.name if previous is not None else None)
            previous = self._cells[ordinal].fd
        if previous is None:
            raise ValueError("missing held parent")
        return previous, absolute.name

    def _read(self, cell, size, retain):
        if cell.fd is None or cell.close_attempted:
            raise ValueError("ineligible retained reader")
        os.lseek(cell.fd, 0, os.SEEK_SET)
        digest, remaining, offset = hashlib.sha256(), size, 0
        data = bytearray(size) if retain else None
        while remaining:
            chunk = os.read(cell.fd, min(remaining, READ_CHUNK))
            if not chunk:
                raise EOFError("short original read")
            digest.update(chunk)
            if data is not None:
                data[offset:offset + len(chunk)] = chunk
            remaining -= len(chunk)
            offset += len(chunk)
        if os.read(cell.fd, 1):
            raise ValueError("original input grew")
        # One parser bytearray and one returned immutable bytes copy coexist.
        # Both plus chunk/runtime/native storage must be measured, never guessed.
        return digest.hexdigest(), bytes(data) if data is not None else None

    def capture_file(self, path, maximum=MAX_INPUT_BYTES):
        try:
            if type(maximum) is not int or not 0 <= maximum <= MAX_INPUT_BYTES:
                raise ValueError("invalid input bound")
            if self._file_count == MAX_OBSERVATIONS:
                raise ValueError("file observation accounting exhausted")
            parent, leaf = self._parents(path)
            ordinal = self._open(path, False, parent, leaf)
            cell = self._cells[ordinal]
            before = os.fstat(cell.fd)
            if before.st_size > maximum:
                raise ValueError("input bound exceeded before payload read/allocation")
            digest, _ = self._read(cell, before.st_size, False)
            after, named = os.fstat(cell.fd), os.lstat(path)
            require_object(after, False)
            require_object(named, False)
            if identity(before) != identity(after) or identity(named) != identity(after):
                raise ValueError("original input changed")
            observation = FileObservation(ordinal, path, identity(before), identity(after),
                                          identity(named), before.st_size, digest)
            public = FileObservation(observation.ordinal, observation.path, observation.before,
                                     observation.after, observation.named, observation.size, observation.sha256)
            self._files[self._file_count] = observation
            self._public_files[self._file_count] = public
            self._file_count += 1
            return public
        except BaseException as original:
            self._record("primary", "capture_file", path, original)
            raise

    def read_captured(self, observation):
        ordinal = next((i for i in range(self._file_count) if self._public_files[i] is observation), None)
        if ordinal is None:
            raise ValueError("unknown immutable observation")
        # Caller-visible DTO fields never control native handle/path/length/hash
        # even if foreign code bypasses the frozen dataclass with object tricks.
        observation = self._files[ordinal]
        cell = self._cells[observation.ordinal]
        try:
            if observation.size > MAX_PARSER_BYTES:
                raise ValueError("parser bound exceeded before allocation")
            if identity(os.fstat(cell.fd)) != observation.after:
                raise ValueError("original held input changed")
            digest, body = self._read(cell, observation.size, True)
            if digest != observation.sha256 or identity(os.fstat(cell.fd)) != observation.after or identity(os.lstat(observation.path)) != observation.named:
                raise ValueError("captured body changed")
            return body
        except BaseException as original:
            self._record("capture", "read_captured", observation.path, original)
            raise

    def _membership(self, cell, maximum):
        result = []
        # Reserve durable scanner custody before acquisition. A scanner close
        # failure is independent of an iteration/stat primary failure; neither
        # original replaces the other and an uncertain close is never repeated.
        _, owner = self._reserve(cell.path)
        owner.scanner = os.scandir(cell.path if os.name == "nt" else cell.fd)
        primary = None
        try:
            scanner = owner.scanner
            for entry in scanner:
                if len(result) == maximum:
                    raise ValueError("membership accounting exhausted")
                name = entry.name
                if len(name) > 512 or name in (".", "..") or "/" in name or "\\" in name:
                    raise ValueError("invalid native directory entry")
                result.append((name, identity(entry.stat(follow_symlinks=False))))
        except BaseException as original:
            primary = original
        owner.close_attempted = True
        try:
            owner.scanner.close()
            owner.closed = True
        except BaseException as original:
            owner.close_error = original
            self._record("disposal", "scanner_close", cell.path, original)
        if primary is not None:
            raise primary
        if owner.close_error is not None:
            raise owner.close_error
        return tuple(sorted(result))

    def capture_membership(self, path):
        try:
            if self._directory_count == MAX_OBSERVATIONS:
                raise ValueError("directory observation accounting exhausted")
            parent, leaf = self._parents(path)
            ordinal = self._open(path, True, parent, leaf)
            cell = self._cells[ordinal]
            before = object_identity(os.fstat(cell.fd))
            remaining = MAX_ENTRIES - self._retained_members
            first, second = self._membership(cell, remaining), self._membership(cell, remaining)
            if first != second or object_identity(os.fstat(cell.fd)) != before or object_identity(os.lstat(path)) != before:
                raise ValueError("directory changed during capture")
            observation = DirectoryObservation(ordinal, path, before, first)
            self._directories[self._directory_count] = observation
            self._directory_count += 1
            self._retained_members += len(first)
            return DirectoryObservation(observation.ordinal, observation.path,
                                        observation.object, observation.members)
        except BaseException as original:
            self._record("primary", "capture_membership", path, original)
            raise

    def revalidate(self):
        if self._emergency is not None:
            raise AccountingExhausted("retained original error accounting exhausted")
        start = self._failure_count
        for i in range(self._used):
            if self._emergency is not None:
                break
            cell = self._cells[i]
            if cell.fd is None or cell.close_attempted:
                continue
            try:
                if object_identity(os.fstat(cell.fd)) != object_identity(os.lstat(cell.path)):
                    raise ValueError("named/held identity changed")
            except BaseException as original:
                self._record("capture", "revalidate_identity", cell.path, original)
        for i in range(self._file_count):
            if self._emergency is not None:
                break
            observation = self._files[i]
            cell = self._cells[observation.ordinal]
            try:
                digest, _ = self._read(cell, observation.size, False)
                if digest != observation.sha256 or identity(os.fstat(cell.fd)) != observation.after or identity(os.lstat(observation.path)) != observation.named:
                    raise ValueError("file bytes/identity changed")
            except BaseException as original:
                self._record("capture", "revalidate_file", observation.path, original)
        for i in range(self._directory_count):
            if self._emergency is not None:
                break
            observation = self._directories[i]
            cell = self._cells[observation.ordinal]
            try:
                if self._membership(cell, len(observation.members)) != observation.members or object_identity(os.fstat(cell.fd)) != observation.object or object_identity(os.lstat(observation.path)) != observation.object:
                    raise ValueError("membership changed")
            except BaseException as original:
                self._record("capture", "revalidate_membership", observation.path, original)
        if self._emergency is not None:
            raise AccountingExhausted("first overflow original remains privately retained")
        return tuple(self._failures[start:self._failure_count])

    def close(self):
        start = self._failure_count
        for i in reversed(range(self._used)):
            cell = self._cells[i]
            if cell.close_attempted or (cell.fd is None and cell.native is None and cell.scanner is None):
                continue
            cell.close_attempted = True
            try:
                if cell.scanner is not None:
                    cell.scanner.close()
                elif cell.fd is not None:
                    os.close(cell.fd)
                else:
                    kernel = ctypes.WinDLL("kernel32", use_last_error=True)
                    close = kernel.CloseHandle
                    close.argtypes, close.restype = [ctypes.c_void_p], ctypes.c_int
                    if not close(cell.native):
                        raise ctypes.WinError(ctypes.get_last_error())
                cell.closed = True
            except BaseException as original:
                cell.close_error = original
                self._record("disposal", "close", cell.path, original)
                # Never retry uncertain numeric descriptor/handle on later calls.
        # Each failed eligible close's original is independently owned by its
        # preallocated cell even when the common error log is exhausted.
        if self._emergency is not None:
            raise AccountingExhausted("eligible close originals retained; log exhausted")
        return tuple(self._failures[start:self._failure_count])

    def has_unresolved_originals(self):
        # Physical custody only: earlier capture errors remain qualification
        # failures even when every independently eligible original is closed.
        for i in range(self._used):
            cell = self._cells[i]
            if not cell.closed and (cell.fd is not None or cell.native is not None or cell.scanner is not None):
                return True
        return False
    def has_original_failures(self):
        # Durable source state check without allocating an outward failure tuple.
        # This is not native input/rights/ROOT admission.
        return self._failure_count != 0 or self._emergency is not None or self._recording_error is not None

    def private_failures(self):
        return tuple(self._failures[:self._failure_count])

    def private_overflow_original(self):
        return self._emergency

    def private_recording_original(self):
        return self._recording_error

    def private_close_outcomes(self):
        return tuple((i, cell.path, cell.fd, cell.native, cell.close_attempted,
                      cell.closed, cell.close_error)
                     for i, cell in enumerate(self._cells[:self._used]))

    def public_error_codes(self):
        # Exact built-in types only. Unknown foreign exception properties,
        # metaclasses, __str__, args and errno getters are never evaluated.
        known = ((OSError, "OSError"), (FileNotFoundError, "FileNotFoundError"),
                 (PermissionError, "PermissionError"), (IsADirectoryError, "IsADirectoryError"),
                 (NotADirectoryError, "NotADirectoryError"), (FileExistsError, "FileExistsError"),
                 (ValueError, "ValueError"), (RuntimeError, "RuntimeError"), (EOFError, "EOFError"))
        os_types = (OSError, FileNotFoundError, PermissionError, IsADirectoryError,
                    NotADirectoryError, FileExistsError)
        result = []
        for failure in self.private_failures():
            original = failure.original
            actual_type = type(original)
            is_os = any(actual_type is candidate for candidate in os_types)
            name = next((name for candidate, name in known if actual_type is candidate), "foreign_exception")
            errno = getattr(original, "errno", None) if is_os else None
            winerror = getattr(original, "winerror", None) if is_os else None
            result.append({"phase": failure.phase, "operation": failure.operation,
                           "type": name,
                           "errno": errno if type(errno) is int else None,
                           "winerror": winerror if type(winerror) is int else None})
        if self._emergency is not None:
            result.append({"phase": "capture", "operation": "accounting_exhausted",
                           "type": "retained_private_original", "errno": None, "winerror": None})
        return tuple(result)

"""UNEXECUTED concrete command input builder; no launch/admission API.

The reviewed driver must supply the complete original command selection, not an
arbitrary caller list that becomes true merely by hashing. Captured observations
are source/input evidence only; actual native owner/ACL/writer exclusion, loaded
memory, compiler/toolchain and root admission remain separate native results.
"""
from __future__ import annotations
from typing import NamedTuple
from dataclasses import dataclass
import hashlib
from pathlib import Path
from held_inputs import HeldInputs, FileObservation

class Requirement(NamedTuple):
    role: str
    path: str
    sha256: str
    size: int

@dataclass(frozen=True)
class NativeArchiveFoundationPrerequisite:
    native_extent_bytes: int
    archive_foundation: tuple[int, int, int]
    archive_readback: tuple[int, int, int]

@dataclass(frozen=True)
class NativeArchiveReadbackPrerequisite:
    native_extent_bytes: int
    archive_foundation: tuple[int, int, int]
    archive_readback: tuple[int, int, int]

class NativeArchivePrerequisiteObservation(NamedTuple):
    requirement: NativeArchiveFoundationPrerequisite | NativeArchiveReadbackPrerequisite
    original_geometry: tuple[int, int, int, int]

class Command(NamedTuple):
    image: str
    argv: tuple[str, ...]
    cwd: str
    environment: tuple[tuple[str, str], ...]
    original_deadline_ms: int | None
    source_origin: str

class CapturedCommand(NamedTuple):
    command: Command
    command_line: str
    environment_utf16le: bytes
    observations: tuple[FileObservation | NativeSubjectObservation | NativeArchivePrerequisiteObservation, ...]
    requirements: tuple[Requirement | NativeArchiveFoundationPrerequisite | NativeArchiveReadbackPrerequisite, ...]

class NativeSubjectObservation(NamedTuple):
    # A retained view of native startup observations, not a Python file capture
    # or authentic membership. Native BindInputs independently rejoins originals.
    requirement: Requirement
    original_geometry: tuple[int, int, int, int]

# These are reader roles, not six Boolean admissions. Every selected dependency
# needs actual observations; this module does not assert a list is complete.
ROLES = ("product", "script", "inline-source", "preload", "interpreter", "library",
         "compiler", "compiler-input", "git-index", "git-storage", "configuration",
         "workflow", "creator", "root-control")


def literal(value):
    if type(value) is not str or "\0" in value:
        raise ValueError("literal NUL-free original string required")
    return value


def utf16_units(value, maximum):
    literal(value)
    count = 0
    for character in value:
        scalar = ord(character)
        if 0xD800 <= scalar <= 0xDFFF:
            raise ValueError("unpaired UTF-16 surrogate in original string")
        count += 2 if scalar > 0xFFFF else 1
        if count > maximum:
            raise ValueError("original UTF-16 bound exceeded before copy")
    return count


def quoted_units(argument, maximum):
    units = utf16_units(argument, maximum)
    if argument and not any(c in argument for c in ' \t\n\v"'):
        return units
    count, slashes = 2, 0
    for character in argument:
        if character == "\\":
            slashes += 1
        else:
            count += slashes * (2 if character == '"' else 1)
            count += 2 if character == '"' or ord(character) > 0xFFFF else 1
            slashes = 0
        if count > maximum or slashes > maximum:
            raise ValueError("quoted UTF-16 bound exceeded before copy")
    count += slashes * 2
    if count > maximum:
        raise ValueError("quoted UTF-16 bound exceeded before copy")
    return count


def windows_quote(argument):
    # CommandLineToArgvW/CRT-style quoting for a selected direct executable.
    # This does NOT quote cmd.exe or PowerShell grammar; selected shell source
    # must be a separate original input with its own exact argument protocol.
    literal(argument)
    quoted_units(argument, 32766)
    if argument and not any(c in argument for c in ' \t\n\v"'):
        return argument
    result, slashes = ['"'], 0
    for character in argument:
        if character == "\\":
            slashes += 1
        elif character == '"':
            result.append("\\" * (slashes * 2 + 1))
            result.append('"')
            slashes = 0
        else:
            result.append("\\" * slashes)
            result.append(character)
            slashes = 0
    result.append("\\" * (slashes * 2))
    result.append('"')
    return "".join(result)


def environment_block(environment):
    if type(environment) is not tuple or len(environment) > 4096:
        raise ValueError("bounded literal environment tuple required")
    # Charge all original WCHARs, separators, equals and double NUL before
    # upper/sort/join/encoding allocations. 65536 is this observer's source
    # bound, not a misapplied ANSI Windows environment limit.
    charged = 1 if environment else 2
    for row in environment:
        if type(row) is not tuple or len(row) != 2:
            raise ValueError("literal environment pair required")
        charged += utf16_units(row[0], 65536 - charged)
        charged += utf16_units(row[1], 65536 - charged) + 2
        if charged > 65536:
            raise ValueError("environment source bound exceeded before copy")
    names, rows = set(), []
    for row in environment:
        if type(row) is not tuple or len(row) != 2:
            raise ValueError("literal environment pair required")
        name, value = literal(row[0]), literal(row[1])
        # No inherited hidden drive-current-directory entries or substitutions.
        if not name or "=" in name or any(ord(c) < 32 for c in name):
            raise ValueError("invalid environment name")
        key = name.upper()
        if key in names:
            raise ValueError("case-ambiguous Windows environment")
        names.add(key)
        rows.append((name, value))
    rows.sort(key=lambda row: row[0].upper())
    body = "\0".join(name + "=" + value for name, value in rows) + "\0\0"
    encoded = body.encode("utf-16le", "strict")
    if len(encoded) > 131072:
        raise ValueError("environment exceeds selected native bound")
    return encoded


class CommandInputs:
    def __init__(self, held):
        if type(held) is not HeldInputs:
            raise ValueError("concrete held owner required")
        self.held = held
        self._captured = None
        self._capture_attempted = False
        self._capture_original = None
        self._revalidation_original = None
        self._native_classes = None

    def capture(self, command, requirements, native_classes=None):
        if self._capture_attempted:
            raise ValueError("original command capture already attempted")
        self._capture_attempted = True
        self._native_classes = native_classes
        try:
            if self.held.has_original_failures():
                raise ValueError("earlier original held failure remains absorbing")
            result = self._capture(command, requirements, native_classes)
            if self.held.has_original_failures():
                raise ValueError("original capture contains held failures")
            self._captured = result
            return result
        except BaseException as original:
            self._capture_original = original
            raise

    def _capture(self, command, requirements, native_classes):
        if self._captured is not None:
            raise ValueError("command input owner already captured")
        if type(command) is not Command or type(requirements) is not tuple:
            raise ValueError("immutable original command selection required")
        if not Path(literal(command.image)).is_absolute() or not Path(literal(command.cwd)).is_absolute():
            raise ValueError("absolute original executable and cwd required")
        if type(command.argv) is not tuple or len(command.argv) > 4096:
            raise ValueError("bounded original argv required")
        if command.original_deadline_ms is not None and (
                type(command.original_deadline_ms) is not int or command.original_deadline_ms <= 0):
            raise ValueError("invalid original caller deadline")
        literal(command.source_origin)
        charged = 1 # Native terminating WCHAR NUL.
        for ordinal, value in enumerate((command.image,) + command.argv):
            if ordinal:
                charged += 1
            charged += quoted_units(value, 32767 - charged)
            if charged > 32767:
                raise ValueError("native command line exceeds original bound before copy")
        line = " ".join(windows_quote(value) for value in (command.image,) + command.argv)
        block = environment_block(command.environment)
        if not 0 < len(requirements) <= 4096:
            raise ValueError("bounded selected dependency requirements required")
        if native_classes is not None and (type(native_classes) is not tuple or len(native_classes) != len(requirements)):
            raise ValueError("complete original native class-view roster required")
        paths, observations = {}, []
        for index, requirement in enumerate(requirements):
            geometry = None if native_classes is None else native_classes[index]
            if type(requirement) in (NativeArchiveFoundationPrerequisite, NativeArchiveReadbackPrerequisite):
                kind = 5 if type(requirement) is NativeArchiveFoundationPrerequisite else 6
                if (type(requirement.native_extent_bytes) is not int or requirement.native_extent_bytes <= 0 or
                        type(geometry) is not tuple or geometry != (kind, requirement.native_extent_bytes, 0, 0) or
                        any(type(value) is not int for value in geometry)):
                    raise ValueError("same original native archive geometry required")
                for selector in (requirement.archive_foundation, requirement.archive_readback):
                    if type(selector) is not tuple or len(selector) != 3 or any(
                            type(value) is not int or not 0 <= value <= 0xffffffff for value in selector):
                        raise ValueError("exact native archive occurrence selector required")
                if (requirement.archive_foundation[1:] != (0, 0) or
                        requirement.archive_readback[1:] != (0, 1) or
                        requirement.archive_foundation[0] != requirement.archive_readback[0] or
                        index != (0 if kind == 5 else 1)):
                    raise ValueError("source-fixed native archive pair order required")
                if kind == 6 and (type(requirements[0]) is not NativeArchiveFoundationPrerequisite or
                        requirements[0].native_extent_bytes != requirement.native_extent_bytes or
                        requirements[0].archive_foundation != requirement.archive_foundation or
                        requirements[0].archive_readback != requirement.archive_readback):
                    raise ValueError("same source native archive pair required")
                observations.append(NativeArchivePrerequisiteObservation(requirement, geometry))
                continue
            if type(requirement) is not Requirement or type(requirement.role) is not str or requirement.role not in ROLES:
                raise ValueError("unknown dependency role")
            literal(requirement.path)
            if requirement.path in paths and (geometry is None or paths[requirement.path] is None):
                raise ValueError("duplicate original input path")
            # Distinct source-selected unit/range occurrences may retain the
            # SAME native unit path. Each remains a separate native acquisition.
            paths[requirement.path] = geometry
            if type(requirement.size) is not int or requirement.size < 0:
                raise ValueError("invalid exact source size")
            if type(requirement.sha256) is not str or len(requirement.sha256) != 64 or any(
                    character not in "0123456789abcdef" for character in requirement.sha256):
                raise ValueError("invalid original source digest")
            if geometry is not None:
                if type(geometry) is not tuple or len(geometry) != 4 or any(type(value) is not int for value in geometry):
                    raise ValueError("original native source geometry shape")
                reader_class, native_size, offset, extent = geometry
                if reader_class not in (3, 4) or native_size <= 0 or offset < 0 or extent < 0 or offset > native_size or extent > native_size - offset or extent != requirement.size:
                    raise ValueError("original native source geometry extent")
                if requirement.path == command.image:
                    raise ValueError("source unit cannot replace original executable")
                observed = NativeSubjectObservation(requirement, geometry)
            else:
                observed = self.held.capture_file(requirement.path, requirement.size)
                if observed.size != requirement.size or observed.sha256 != requirement.sha256:
                    raise ValueError("actual selected dependency bytes differ")
            observations.append(observed)
        if command.image not in paths:
            raise ValueError("actual executable missing from selected inputs")
        self.held.capture_membership(command.cwd)
        result = CapturedCommand(command, line, block, tuple(observations), requirements)
        return result

    def revalidate(self, captured):
        if self._captured is None or captured is not self._captured:
            raise ValueError("foreign command snapshot")
        try:
            if self._capture_original is not None or self._revalidation_original is not None or self.held.has_original_failures():
                raise ValueError("original capture failure remains absorbing")
            self.held.revalidate()
            if self.held.has_original_failures():
                raise ValueError("actual original held inputs changed")
        except BaseException as original:
            if self._revalidation_original is None:
                self._revalidation_original = original
            raise
        # Successful byte revalidation is only one gate predicate, never ROOT,
        # ACL/writer exclusion, loaded memory, compiler or native source authority.
        return captured

    def private_original_errors(self):
        return self._capture_original, self._revalidation_original

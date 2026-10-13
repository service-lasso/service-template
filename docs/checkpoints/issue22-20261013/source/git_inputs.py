"""Strict source-only Git object/index readers; no Git command or ref discovery.

Decode only an explicitly requested issue/develop candidate and its own tree.
No other branch, replacement, graft, filter, hook or ambient config resolution.
Loose storage is read directly. Packed storage needs a separately admitted
original Git acquisition route with actual retained pack/index/tool inputs and
native stream/terminal outcomes; framed_object validates the exact resulting
object frame only. No pure reader supplies that missing native route.
"""
from __future__ import annotations

import hashlib
import os
import re
import struct
import zlib
from pathlib import PurePosixPath

from held_inputs import HeldInputs, MAX_PARSER_BYTES


OID = re.compile(r"[0-9a-f]{40}\Z")
NORMAL_MODE = {b"100644", b"100755", b"40000"}


def windows_namespace(paths):
    """Selected Win32 lexical policy, not native alias/object proof.

    Native held file IDs/final paths, volume component policy and ordinal case
    comparisons still bind actual names; Unicode casefold is only conservative
    lexical collision rejection and never proves NTFS namespace uniqueness.
    """
    if type(paths) is not tuple or len(paths) > 65536:
        raise ValueError("bounded original path tuple required")
    devices = {"CON", "PRN", "AUX", "NUL"}
    devices.update(prefix + digit for prefix in ("COM", "LPT") for digit in "123456789¹²³")
    names = set()
    for path in paths:
        for component in path.split("/"):
            if component.endswith((".", " ")) or any(c in component for c in '<>"|?*') or \
                    component.casefold() == ".git" or component.split(".", 1)[0].upper() in devices:
                raise ValueError("unsupported Win32 path representation")
            if any(0xD800 <= ord(c) <= 0xDFFF for c in component):
                raise ValueError("invalid original Unicode path")
        key = path.casefold()
        if key in names:
            raise ValueError("case-ambiguous selected Windows path")
        names.add(key)
    return tuple(paths)


def path_name(raw: bytes) -> str:
    value = raw.decode("utf-8", errors="strict")
    path = PurePosixPath(value)
    if not value or value.startswith("/") or "\\" in value or ":" in value or \
            any(part in ("", ".", "..", ".git") for part in value.split("/")) or \
            any(ord(char) < 32 or ord(char) == 127 for char in value):
        raise ValueError("unsafe Git path")
    if str(path) != value:
        raise ValueError("noncanonical Git path")
    return value


def inflate_exact(compressed: bytes, limit: int) -> bytes:
    decoder = zlib.decompressobj()
    body = decoder.decompress(compressed, limit + 1)
    if len(body) > limit or decoder.unconsumed_tail or not decoder.eof or decoder.unused_data:
        raise ValueError("invalid, concatenated or overbound loose Git compression")
    return body


class GitInputs:
    def __init__(self, held: HeldInputs, object_directory: str):
        self.held, self.object_directory = held, object_directory
        self.observations: dict[str, dict] = {}

    def object(self, oid: str) -> tuple[str, bytes]:
        if not OID.fullmatch(oid):
            raise ValueError("full lowercase Git OID required")
        path = os.path.join(self.object_directory, oid[:2], oid[2:])
        # Missing loose object is an unavailable capability, not empty object or
        # permission to execute ambient Git, fetch or read another branch.
        observed = self.held.capture_file(path)
        compressed = self.held.read_captured(observed)
        frame = inflate_exact(compressed, MAX_PARSER_BYTES)
        if hashlib.sha1(frame).hexdigest() != oid:
            raise ValueError("genuine NUL-framed object SHA1 differs")
        header, separator, body = frame.partition(b"\0")
        kind, space, length = header.partition(b" ")
        if not separator or not space or kind not in (b"commit", b"tree", b"blob", b"tag") or \
                not re.fullmatch(rb"0|[1-9][0-9]*", length) or int(length) != len(body):
            raise ValueError("invalid Git type/length/NUL frame")
        self.observations[oid] = {"oid": oid, "type": kind.decode("ascii"),
            "payloadBytes": str(len(body)), "payloadSha256": hashlib.sha256(body).hexdigest(),
            "frameSha256": hashlib.sha256(frame).hexdigest(), "storage": observed}
        return kind.decode("ascii"), body

    def candidate_tree(self, commit: str) -> dict[str, dict]:
        kind, body = self.object(commit)
        if kind != "commit":
            raise ValueError("candidate is not commit")
        first, separator, _ = body.partition(b"\n")
        if not separator or not re.fullmatch(rb"tree [0-9a-f]{40}", first):
            raise ValueError("commit tree header malformed")
        result: dict[str, dict] = {}
        active: set[str] = set()

        def descend(tree: str, prefix: str, depth: int) -> None:
            if depth > 64 or tree in active:
                raise ValueError("tree depth/cycle bound")
            active.add(tree)
            actual, payload = self.object(tree)
            if actual != "tree":
                raise ValueError("tree references non-tree")
            offset, names = 0, set()
            while offset < len(payload):
                end = payload.find(b"\0", offset)
                if end < 0 or end + 21 > len(payload):
                    raise ValueError("truncated binary Git tree")
                metadata = payload[offset:end]
                mode, space, name = metadata.partition(b" ")
                if not space or mode not in NORMAL_MODE or b"/" in name:
                    raise ValueError("unsupported mode/unsafe tree leaf")
                leaf = path_name(name)
                if leaf in names:
                    raise ValueError("duplicate tree leaf")
                names.add(leaf)
                oid = payload[end + 1:end + 21].hex()
                path = f"{prefix}/{leaf}" if prefix else leaf
                if mode == b"40000":
                    descend(oid, path, depth + 1)
                else:
                    blob_kind, blob = self.object(oid)
                    if blob_kind != "blob" or path in result or len(result) >= 65536:
                        raise ValueError("tree blob/membership differs")
                    result[path] = {"oid": oid, "mode": mode.decode("ascii"),
                                    "bytes": str(len(blob)),
                                    "sha256": hashlib.sha256(blob).hexdigest()}
                offset = end + 21
            active.remove(tree)

        descend(first[5:].decode("ascii"), "", 0)
        if os.name == "nt":
            windows_namespace(tuple(result))
        return result


def index_v2_v3(raw: bytes) -> dict:
    if type(raw) is not bytes or not 32 <= len(raw) <= MAX_PARSER_BYTES or raw[:4] != b"DIRC" or \
            hashlib.sha1(raw[:-20]).digest() != raw[-20:]:
        raise ValueError("Git index header/checksum differs")
    version, count = struct.unpack_from(">II", raw, 4)
    if version not in (2, 3) or count > 65536:
        raise ValueError("unsupported index version/count")
    offset, entries, previous = 12, [], b""
    for _ in range(count):
        start = offset
        if offset + 62 > len(raw) - 20:
            raise ValueError("truncated index entry")
        mode = struct.unpack_from(">I", raw, offset + 24)[0]
        oid = raw[offset + 40:offset + 60].hex()
        flags = struct.unpack_from(">H", raw, offset + 60)[0]
        offset += 62
        # Stage0, no assume-valid, no skip-worktree/intent-to-add, no extended
        # hidden flags. Do not silently normalize a nonordinary candidate index.
        if flags & 0xF000 or mode not in (0o100644, 0o100755):
            raise ValueError("index mode/stage/flags unsupported")
        end = raw.find(b"\0", offset, len(raw) - 20)
        if end < 0:
            raise ValueError("unterminated index path")
        name = raw[offset:end]
        if not name or name <= previous or (flags & 0xFFF) != min(len(name), 0xFFF):
            raise ValueError("index path order/length differs")
        path = path_name(name)
        offset = start + ((end - start + 1 + 7) // 8) * 8
        if offset > len(raw) - 20 or any(raw[end:offset]):
            raise ValueError("index NUL padding differs")
        entries.append({"path": path, "mode": f"{mode:o}", "oid": oid})
        previous = name
    # Git's primary index format permits ignoring optional extensions whose
    # signature starts A..Z. Preserve every raw byte/hash as input; caches never
    # replace our independently checked stage0 entries or actual held source.
    # Lowercase required features (e.g. split index 'link') cannot be ignored.
    extensions = []
    while offset < len(raw) - 20:
        if offset + 8 > len(raw) - 20:
            raise ValueError("truncated index extension header")
        signature, size = raw[offset:offset + 4], struct.unpack_from(">I", raw, offset + 4)[0]
        start, end = offset, offset + 8 + size
        if end > len(raw) - 20 or not 65 <= signature[0] <= 90:
            raise ValueError("required/invalid index extension is unsupported")
        extensions.append({"signatureHex": signature.hex(), "offset": str(start),
                           "size": str(size), "rawSha256": hashlib.sha256(raw[start:end]).hexdigest()})
        offset = end
    if os.name == "nt":
        windows_namespace(tuple(entry["path"] for entry in entries))
    return {"version": version, "entries": entries, "extensions": extensions,
            "rawSize": str(len(raw)), "rawSha256": hashlib.sha256(raw).hexdigest(),
            "trailingSha1": raw[-20:].hex()}


def framed_object(oid: str, frame: bytes) -> tuple[str, bytes]:
    """Validate actual original admitted Git --batch output for packed storage.

    The native driver must retain original Git executable/libraries/config,
    physical pack/index inputs, exact requested OID and raw output/EOF/terminal.
    Supplying bytes to this pure function grants NONE of those authorities.
    Expected frame is genuine '<type> <length>\0<payload>', reconstructed only
    from the original bounded --batch header and exact original payload bytes.
    """
    if type(frame) is not bytes or not OID.fullmatch(oid) or len(frame) > MAX_PARSER_BYTES or hashlib.sha1(frame).hexdigest() != oid:
        raise ValueError("actual Git frame/OID differs")
    header, nul, body = frame.partition(b"\0")
    kind, space, length = header.partition(b" ")
    if not nul or not space or kind not in (b"commit", b"tree", b"blob", b"tag") or \
            not re.fullmatch(rb"0|[1-9][0-9]*", length) or int(length) != len(body):
        raise ValueError("actual Git frame type/length differs")
    return kind.decode("ascii"), body

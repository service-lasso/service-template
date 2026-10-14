"""UNRUN private native-constructor input encoder; no file/launch/admission API.

The returned bytes select original inputs, not authority. Every supplied object,
capacity, runtime and complete source association still requires actual admission.
"""
import struct

PYTHON_TEXT = ("home", "program", "executable", "base_executable", "prefix",
               "exec_prefix", "base_prefix", "base_exec_prefix")
HOST_SIZE = ("dispatch_rows", "host_closure_observations", "sink_bytes",
             "readback_bytes", "sink_calls", "collector_bytes", "collector_reads",
             "collector_observations", "final_bytes", "final_readback_bytes",
             "final_calls", "host_progress_observations", "final_collector_bytes",
              "final_collector_reads", "final_collector_observations", "pending_call_rows",
              "observer_threads", "observer_invocations", "observer_pointers",
              "observer_string_bytes", "observer_read_rows", "notice_stack_bytes",
              "receiver_stack_bytes", "notice_io_rows", "receiver_io_rows")
HOST_DWORD = ("final_collector_wait_ms", "collector_wait_ms")


def exact_mapping(value, keys):
    if type(value) is not dict or any(type(key) is not str for key in value):
        raise ValueError("closed private selection mapping required")
    if set(value) != set(keys):
        raise ValueError("missing or unexpected private selection field")
    return value


def exact_tuple(value):
    if type(value) is not tuple:
        raise ValueError("exact private selection sequence required")
    return value


class Encoder:
    def __init__(self, selected_bytes):
        if type(selected_bytes) is not int or not 0 < selected_bytes < (1 << 64) - 1:
            raise ValueError("selected native input byte capacity required")
        self.backing = bytearray(selected_bytes)
        self.used = 0

    def raw(self, original):
        if type(original) is not bytes or len(original) > len(self.backing) - self.used:
            raise ValueError("private selection body exceeds precharged backing")
        self.backing[self.used:self.used + len(original)] = original
        self.used += len(original)

    def unsigned(self, original, width=8):
        if type(width) is not int or width not in (4, 8) or type(original) is not int or not 0 <= original < 1 << (8 * width):
            raise ValueError("exact unsigned private selection value required")
        self.raw(struct.pack("<Q" if width == 8 else "<I", original))

    def text(self, original):
        if type(original) is not str:
            raise ValueError("exact original selection text required")
        units = 0
        for scalar in original:
            code = ord(scalar)
            if not code or 0xd800 <= code <= 0xdfff:
                raise ValueError("NUL or surrogate in private selection text")
            units += 2 if code > 0xffff else 1
            if units >= 32768:
                raise ValueError("native selection UTF16 representation exceeded")
        if 8 + units * 2 > len(self.backing) - self.used:
            raise ValueError("private text exceeds selected backing before encoding")
        self.unsigned(units)
        self.raw(original.encode("utf-16le"))

    def environment(self, original):
        rows = exact_tuple(original)
        self.unsigned(len(rows))
        for row in rows:
            if type(row) is not tuple or len(row) != 2:
                raise ValueError("original environment name/value pair required")
            self.text(row[0])
            self.text(row[1])

    def requirements(self, original):
        row = exact_mapping(original, ("files", "maximum_observations", "minimum_poll_ns"))
        files = exact_tuple(row["files"])
        if not 0 < len(files) <= 1024:
            raise ValueError("bounded original requirement roster required")
        self.unsigned(len(files))
        for original_file in files:
            item = exact_mapping(original_file, ("role", "path", "sha256", "size"))
            self.text(item["role"])
            self.text(item["path"])
            digest = item["sha256"]
            if type(digest) is not bytes or len(digest) != 32:
                raise ValueError("raw selected SHA256 bytes required")
            self.raw(digest)
            self.unsigned(item["size"])
        self.unsigned(row["maximum_observations"])
        self.unsigned(row["minimum_poll_ns"])


def encode_private_selection(original, selected_bytes):
    """Return exact private input bytes; never write a file or grant authority."""
    original = exact_mapping(original, ("phase", "python", "capabilities", "node", "root",
        "environment", "requirements", "bootstrap", "host", "main", "layouts", "ingress_body"))
    phase = original["phase"]
    if type(phase) is not str or phase not in ("generation", "final_qualification"):
        raise ValueError("actual protected route phase required")
    count = 1 if phase == "generation" else 2
    writer = Encoder(selected_bytes)
    writer.raw(b"TPL77IN4")
    writer.unsigned(4, 4)
    writer.unsigned(count - 1, 4)
    python = exact_mapping(original["python"], PYTHON_TEXT + ("module_paths",))
    for name in PYTHON_TEXT:
        writer.text(python[name])
    modules = exact_tuple(python["module_paths"])
    writer.unsigned(len(modules))
    for path in modules:
        writer.text(path)
    capabilities = exact_mapping(original["capabilities"], ("candidate_directory", "tool_file", "evidence_directory"))
    for name in ("candidate_directory", "tool_file", "evidence_directory"):
        writer.text(capabilities[name])
    writer.text(original["node"])
    writer.text(original["root"])
    writer.environment(original["environment"])
    requirements = exact_tuple(original["requirements"])
    if len(requirements) != count:
        raise ValueError("phase must retain every original protected command")
    for row in requirements:
        writer.requirements(row)
    bootstrap = exact_mapping(original["bootstrap"], ("image", "cwd", "environment", "requirements"))
    writer.text(bootstrap["image"])
    writer.text(bootstrap["cwd"])
    writer.environment(bootstrap["environment"])
    writer.requirements(bootstrap["requirements"])
    host = exact_mapping(original["host"], HOST_SIZE + HOST_DWORD)
    if type(host["pending_call_rows"]) is not int or host["pending_call_rows"] <= 0:
        raise ValueError("original pending call subdivision required")
    for name in ("observer_threads", "observer_invocations", "observer_string_bytes", "observer_read_rows", "notice_stack_bytes",
              "receiver_stack_bytes", "notice_io_rows", "receiver_io_rows"):
        if type(host[name]) is not int or host[name] <= 0:
            raise ValueError("complete original observer subdivisions required")
    for name in HOST_SIZE:
        writer.unsigned(host[name])
    for name in HOST_DWORD:
        writer.unsigned(host[name], 4)
    main = exact_mapping(original["main"], ("observations", "receipt_polls", "receipt_stack_bytes"))
    for name in ("observations", "receipt_polls", "receipt_stack_bytes"):
        writer.unsigned(main[name])
    layouts = exact_tuple(original["layouts"])
    if len(layouts) != count + 1:
        raise ValueError("actual bootstrap plus protected command layouts required")
    writer.unsigned(len(layouts))
    for original_layout in layouts:
        layout = exact_mapping(original_layout, ("caller_rows", "read_rows", "ntdll_ordinal", "images"))
        for name in ("caller_rows", "read_rows", "ntdll_ordinal"):
            writer.unsigned(layout[name])
        images = exact_tuple(layout["images"])
        writer.unsigned(len(images))
        for image in images:
            if type(image) is not tuple or len(image) != 2:
                raise ValueError("actual image byte/region geometry required")
            writer.unsigned(image[0])
            writer.unsigned(image[1])
    body = original["ingress_body"]
    if type(body) is not bytes or not 0 < len(body) <= 65536:
        raise ValueError("bounded private original ingress body required")
    writer.unsigned(len(body), 4)
    writer.raw(body)
    # Exact final bytes overlap the full precharged backing and original caller
    # selection; include all three and encoder temporaries in whole accounting.
    return bytes(memoryview(writer.backing)[:writer.used])


LINUX_HOST_SIZE = ("dispatch_rows", "host_observations", "sink_bytes", "sink_calls")
LINUX_PENDING_SIZE = ("clock_rows", "task_observations", "stat_bytes", "status_bytes",
    "proc_reads", "fdinfo_bytes", "supervisor_stack", "supervisor_joins",
    "receiver_stack", "receiver_waits", "receiver_joins", "maps_bytes", "maps_reads",
    "memory_regions", "memory_bytes", "memory_reads")
LINUX_CALLER_SIZE = ("caller_stack", "caller_joins", "sink_ordinal", "source_root_ordinal",
    "tool_ordinal", "evidence_root_ordinal")


class LinuxEncoder(Encoder):
    def text(self, original):
        if type(original) is not str:
            raise ValueError("exact original Linux selection text required")
        extent = 0
        for scalar in original:
            code = ord(scalar)
            if not code or 0xd800 <= code <= 0xdfff:
                raise ValueError("NUL or surrogate in private Linux text")
            extent += 1 if code < 0x80 else 2 if code < 0x800 else 3 if code < 0x10000 else 4
            if extent > len(self.backing) - self.used - 8:
                raise ValueError("private Linux text exceeds backing before encoding")
        self.unsigned(extent)
        self.raw(original.encode("utf-8"))

    def requirements(self, original, command, phase_ordinal):
        row = exact_mapping(original, ("files", "maximum_observations", "minimum_poll_ns", "read_rows"))
        files = exact_tuple(row["files"])
        reads = exact_tuple(row["read_rows"])
        if not files or len(reads) != len(files):
            raise ValueError("every Linux original input needs its read subdivision")
        if command == 0 and (len(files) < 2 or type(files[0]) is not dict or
                type(files[1]) is not dict or files[0].get("kind") != 5 or files[1].get("kind") != 6):
            raise ValueError("complete explicit Linux foundation/readback pair required")
        for requirement, (item, read_count) in enumerate(zip(files, reads)):
            if type(item) is not dict or type(item.get("kind")) is not int or type(read_count) is not int:
                raise ValueError("exact Linux requirement/read subdivision required")
            if item["kind"] in (5, 6):
                if (command != 0 or requirement != item["kind"] - 5 or read_count != 0 or
                        item.get("archive_foundation") != (phase_ordinal, 0, 0) or
                        item.get("archive_readback") != (phase_ordinal, 0, 1) or
                        type(item.get("native_extent_bytes")) is not int or item["native_extent_bytes"] <= 0 or
                        item["native_extent_bytes"] != files[0].get("native_extent_bytes")):
                    raise ValueError("exact native pair occurrence/extent and zero immutable reads required")
            elif item["kind"] != 1 or read_count <= 0:
                raise ValueError("positive immutable Linux read subdivision required")
        self.unsigned(len(files))
        for original_file in files:
            if type(original_file) is not dict or type(original_file.get("kind")) is not int:
                raise ValueError("exact typed Linux requirement required")
            kind = original_file["kind"]
            if kind == 1:
                item = exact_mapping(original_file, ("kind", "role", "path", "sha256", "size"))
                self.unsigned(kind, 4)
                self.text(item["role"])
                self.text(item["path"])
                if type(item["sha256"]) is not bytes or len(item["sha256"]) != 32:
                    raise ValueError("raw Linux original SHA256 bytes required")
                self.raw(item["sha256"])
                self.unsigned(item["size"])
            elif kind in (5, 6):
                item = exact_mapping(original_file, ("kind", "native_extent_bytes",
                    "archive_foundation", "archive_readback"))
                if type(item["native_extent_bytes"]) is not int or item["native_extent_bytes"] <= 0:
                    raise ValueError("positive original native archive extent required")
                self.unsigned(kind, 4)
                self.unsigned(item["native_extent_bytes"])
                for name in ("archive_foundation", "archive_readback"):
                    selector = exact_tuple(item[name])
                    if len(selector) != 3:
                        raise ValueError("exact archive occurrence selector required")
                    for value in selector:
                        self.unsigned(value, 4)
            else:
                raise ValueError("unsupported private Linux requirement kind")
        self.unsigned(row["maximum_observations"])
        self.unsigned(row["minimum_poll_ns"])
        self.unsigned(len(reads))
        for count in reads:
            self.unsigned(count)


def encode_linux_private_selection(original, selected_bytes):
    """Closed constructor bytes only; original FDs are separately supplied."""
    original = exact_mapping(original, ("phase", "python", "node", "root", "environment",
        "requirements", "host", "pending", "deadline", "caller", "main", "ingress_body"))
    phase = original["phase"]
    if type(phase) is not str or phase not in ("generation", "final_qualification"):
        raise ValueError("actual protected Linux phase required")
    count = 1 if phase == "generation" else 2
    writer = LinuxEncoder(selected_bytes)
    writer.raw(b"TPL77LN3")
    writer.unsigned(3, 4)
    writer.unsigned(count - 1, 4)
    python = exact_mapping(original["python"], PYTHON_TEXT + ("module_paths",))
    for name in PYTHON_TEXT:
        writer.text(python[name])
    modules = exact_tuple(python["module_paths"])
    writer.unsigned(len(modules))
    for path in modules:
        writer.text(path)
    writer.text(original["node"])
    writer.text(original["root"])
    writer.environment(original["environment"])
    requirements = exact_tuple(original["requirements"])
    if len(requirements) != count:
        raise ValueError("every unchanged Linux protected command required")
    for command, row in enumerate(requirements):
        writer.requirements(row, command, 0 if phase == "generation" else 1)
    host = exact_mapping(original["host"], LINUX_HOST_SIZE)
    for name in LINUX_HOST_SIZE:
        writer.unsigned(host[name])
    pending = exact_mapping(original["pending"], LINUX_PENDING_SIZE)
    for name in LINUX_PENDING_SIZE:
        writer.unsigned(pending[name])
    deadline = original["deadline"]
    if deadline is None:
        writer.raw(b"\0")
        writer.unsigned(0)
    else:
        if type(deadline) is not int or not 0 <= deadline < 1 << 64:
            raise ValueError("actual original absolute Linux deadline required")
        writer.raw(b"\1")
        writer.unsigned(deadline)
    caller = exact_mapping(original["caller"], LINUX_CALLER_SIZE)
    for name in LINUX_CALLER_SIZE:
        writer.unsigned(caller[name])
    main = exact_mapping(original["main"], ("sessions", "route_string_bytes", "observations"))
    for name in ("sessions", "route_string_bytes", "observations"):
        writer.unsigned(main[name])
    body = original["ingress_body"]
    if type(body) is not bytes or not body:
        raise ValueError("actual original Linux ingress body required")
    writer.unsigned(len(body))
    writer.raw(body)
    return bytes(memoryview(writer.backing)[:writer.used])
    # Whole backing/final bytes/caller objects/temporaries overlap and require
    # the measured full profile. No filesystem, interpreter or native effects.

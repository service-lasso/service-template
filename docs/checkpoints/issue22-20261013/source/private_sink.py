"""UNRUN private byte sink. Native original ownership/ACL admission is separate.

The caller transfers an original open read/write descriptor, never a path to
reopen. This owner keeps raw I/O outcomes and readback bytes. Durable bytes are
an encoding of observations, not a replacement for live native originals.
"""
import os
import stat

class Attempt:
    __slots__ = ('operation', 'offset', 'requested', 'result', 'original', 'body')
    def __init__(self):
        self.operation = None
        self.offset = self.requested = 0
        self.result = None
        self.original = None
        self.body = None

class PrivateByteSink:
    def __init__(self, maximum_bytes, maximum_attempts):
        if type(maximum_bytes) is not int or maximum_bytes <= 0:
            raise ValueError('selected sink byte capacity required')
        if type(maximum_attempts) is not int or maximum_attempts <= 0:
            raise ValueError('selected sink outcome capacity required')
        self.capacity = maximum_bytes
        self.attempts = tuple(Attempt() for _ in range(maximum_attempts))
        self.count = 0
        self.fd = None
        self.adoption_attempted = False
        self.initial_stat = None
        self.written = self.verified = 0
        self.failed = False
        self.first_original = None
        self.close_attempted = False
        self.close_result = None
        self.close_original = None
        self.closed = False

    def _fail(self, original):
        self.failed = True
        if self.first_original is None:
            self.first_original = original

    def _cell(self, operation, offset=0, requested=0):
        if self.count == len(self.attempts):
            original = RuntimeError('original sink outcome capacity exhausted')
            self._fail(original)
            raise original
        cell = self.attempts[self.count]
        self.count += 1
        cell.operation, cell.offset, cell.requested = operation, offset, requested
        return cell

    def adopt_transferred_original(self, fd):
        if self.adoption_attempted:
            raise ValueError('original sink adoption already attempted')
        if type(fd) is not int or fd < 0:
            raise ValueError('original transferred descriptor required')
        # Durable owner attachment precedes fstat/seek effects. Transfer and
        # caller association must already be captured by concrete native ingress.
        self.adoption_attempted = True
        self.fd = fd
        cell = self._cell('fstat')
        try:
            cell.result = os.fstat(fd)
            self.initial_stat = cell.result
            if not stat.S_ISREG(cell.result.st_mode) or cell.result.st_size != 0:
                raise ValueError('original sink must be an empty regular file')
        except BaseException as original:
            cell.original = original
            self._fail(original)
            raise
        cell = self._cell('seek-initial')
        try:
            cell.result = os.lseek(fd, 0, os.SEEK_SET)
            if cell.result != 0:
                raise ValueError('original sink initial offset mismatch')
        except BaseException as original:
            cell.original = original
            self._fail(original)
            raise

    def append_original_encoding(self, body):
        try:
            return self._append_original_encoding(body)
        except BaseException as original:
            self._fail(original)
            raise

    def _append_original_encoding(self, body):
        if self.failed or self.fd is None or self.close_attempted:
            raise ValueError('original sink unavailable')
        if type(body) is not bytes:
            raise ValueError('immutable original encoding required')
        if len(body) > self.capacity - self.written:
            original = ValueError('selected original sink byte capacity exceeded')
            self._fail(original)
            raise original
        # Retain exact supplied bytes before writes. Native original objects stay
        # separately owned; these bytes cannot manufacture native proof.
        position = 0
        view = memoryview(body)
        while position != len(body):
            cell = self._cell('write', self.written, len(body) - position)
            cell.body = body
            try:
                cell.result = os.write(self.fd, view[position:])
                if cell.result > 0:
                    position += cell.result
                    self.written += cell.result
                if cell.result <= 0 or position > len(body):
                    raise ValueError('original sink write extent invalid')
            except BaseException as original:
                cell.original = original
                self._fail(original)
                raise

    def flush_readback(self):
        try:
            return self._flush_readback()
        except BaseException as original:
            self._fail(original)
            raise

    def _flush_readback(self):
        if self.failed or self.fd is None or self.close_attempted:
            raise ValueError('original sink unavailable')
        cell = self._cell('fsync')
        try:
            cell.result = os.fsync(self.fd)
        except BaseException as original:
            cell.original = original
            self._fail(original)
            raise
        cell = self._cell('seek-readback')
        try:
            cell.result = os.lseek(self.fd, 0, os.SEEK_SET)
            if cell.result != 0:
                raise ValueError('original sink readback offset mismatch')
        except BaseException as original:
            cell.original = original
            self._fail(original)
            raise
        self.verified = 0
        # Compare exact successful write extents, including partial writes.
        original_attempt_count = self.count
        for original_ordinal in range(original_attempt_count):
            prior = self.attempts[original_ordinal]
            if prior.operation != 'write' or prior.result is None or prior.result <= 0:
                continue
            remaining = prior.result
            body_start = prior.requested
            body_start = len(prior.body) - body_start
            while remaining:
                cell = self._cell('readback', self.verified, min(65536, remaining))
                try:
                    cell.body = os.read(self.fd, cell.requested)
                    cell.result = len(cell.body)
                    if not cell.result or cell.body != prior.body[body_start:body_start + cell.result]:
                        raise ValueError('original sink readback differs')
                    body_start += cell.result
                    remaining -= cell.result
                    self.verified += cell.result
                except BaseException as original:
                    cell.original = original
                    self._fail(original)
                    raise
        cell = self._cell('readback-eof', self.verified, 1)
        try:
            cell.body = os.read(self.fd, 1)
            cell.result = len(cell.body)
            if cell.result or self.verified != self.written:
                raise ValueError('original sink readback extent differs')
        except BaseException as original:
            cell.original = original
            self._fail(original)
            raise
        # Preserve append offset after readback. No reopening by name.
        cell = self._cell('seek-append', self.written)
        try:
            cell.result = os.lseek(self.fd, self.written, os.SEEK_SET)
            if cell.result != self.written:
                raise ValueError('original sink append offset differs')
        except BaseException as original:
            cell.original = original
            self._fail(original)
            raise

    def close_once_eligible(self):
        # Caller establishes native producer completion before invoking this.
        # Unknown close never retries a possibly recycled numeric descriptor.
        if self.fd is None or self.close_attempted:
            return
        self.close_attempted = True
        try:
            self.close_result = os.close(self.fd)
            self.closed = True
        except BaseException as original:
            self.close_original = original
            self._fail(original)

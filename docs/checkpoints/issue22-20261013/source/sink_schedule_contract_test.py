"""UNRUN source regressions; never native ingress/authority acceptance."""
import os
import tempfile
import unittest
from unittest.mock import patch
from private_sink import PrivateByteSink
from observation_schedule import RosterCapacity, reserve_original_deadline, ScheduleClock

class SinkContract(unittest.TestCase):
    def make_sink(self, directory):
        fd = os.open(os.path.join(directory, 'owned-private-sink'),
                     os.O_CREAT | os.O_EXCL | os.O_RDWR, 0o600)
        sink = PrivateByteSink(65536, 128)
        sink.adopt_transferred_original(fd)
        return sink

    def test_append_readback_append_readback(self):
        with tempfile.TemporaryDirectory() as directory:
            sink = self.make_sink(directory)
            try:
                sink.append_original_encoding(b'first\x00original')
                sink.flush_readback()
                sink.append_original_encoding(b'second\xfforiginal')
                sink.flush_readback()
                self.assertEqual(sink.verified, sink.written)
                self.assertFalse(sink.failed)
                self.assertEqual(sum(x.operation == 'readback-eof' for x in sink.attempts), 2)
            finally:
                sink.close_once_eligible()

    def test_short_write_extents_survive_readback(self):
        with tempfile.TemporaryDirectory() as directory:
            sink = self.make_sink(directory)
            original_write = os.write
            def short_write(fd, body):
                return original_write(fd, body[:2])
            try:
                with patch('private_sink.os.write', side_effect=short_write):
                    sink.append_original_encoding(b'abcdefg')
                sink.flush_readback()
                writes = [x for x in sink.attempts if x.operation == 'write']
                self.assertEqual([x.result for x in writes], [2, 2, 2, 1])
                self.assertEqual([x.offset for x in writes], [0, 2, 4, 6])
                self.assertEqual(sink.verified, 7)
            finally:
                sink.close_once_eligible()

    def test_failed_write_keeps_original_and_prior_extent(self):
        with tempfile.TemporaryDirectory() as directory:
            sink = self.make_sink(directory)
            original_write = os.write
            failure = OSError(5, 'injected original')
            calls = 0
            def write_then_fail(fd, body):
                nonlocal calls
                calls += 1
                if calls == 1:
                    return original_write(fd, body[:2])
                raise failure
            try:
                with patch('private_sink.os.write', side_effect=write_then_fail):
                    with self.assertRaises(OSError):
                        sink.append_original_encoding(b'abcdefg')
                writes = [x for x in sink.attempts if x.operation == 'write']
                self.assertEqual(sink.written, 2)
                self.assertIs(writes[1].original, failure)
                self.assertIs(sink.first_original, failure)
                with self.assertRaises(ValueError):
                    sink.flush_readback()
            finally:
                sink.close_once_eligible()

    def test_unknown_close_never_retries(self):
        with tempfile.TemporaryDirectory() as directory:
            sink = self.make_sink(directory)
            original_fd = sink.fd
            failure = OSError(5, 'injected close')
            try:
                with patch('private_sink.os.close', side_effect=failure) as closing:
                    sink.close_once_eligible()
                    sink.close_once_eligible()
                self.assertEqual(closing.call_count, 1)
                self.assertIs(sink.close_original, failure)
                self.assertFalse(sink.closed)
            finally:
                # The injected function performed no real close; this is only
                # fixture disposal, not permission to retry uncertain native IO.
                os.close(original_fd)

class ScheduleContract(unittest.TestCase):
    def test_delayed_first_tick_does_not_renew_original_deadline(self):
        reserved = reserve_original_deadline(15000,
            (RosterCapacity('actual-native-roster', 100, 4, 4),))
        clock = ScheduleClock(reserved, 100)
        self.assertTrue(clock.observe_tick(15000000100))
        self.assertTrue(clock.deadline_observed)

    def test_deadline_seen_even_when_spacing_denies_step(self):
        reserved = reserve_original_deadline(15000,
            (RosterCapacity('actual-native-roster', 100, 4, 4),))
        clock = ScheduleClock(reserved, 100)
        self.assertTrue(clock.observe_tick(14999999100))
        self.assertFalse(clock.observe_tick(15000000100))
        self.assertTrue(clock.deadline_observed)

    def test_every_reserved_roster_fits(self):
        rows = (RosterCapacity('events', 65536, 8, 1),
                RosterCapacity('native-results', 65536, 16, 8))
        reserved = reserve_original_deadline(30000, rows)
        for source, actual in zip(rows, reserved.reserved_rows):
            self.assertLessEqual(actual[1], source.available_rows)
        self.assertGreaterEqual((reserved.maximum_steps - 1) * reserved.minimum_poll_ns,
                                reserved.original_deadline_ns)

if __name__ == '__main__':
    unittest.main()

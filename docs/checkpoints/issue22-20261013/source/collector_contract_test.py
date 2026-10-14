"""UNRUN meaningful owned-file regressions; not native/runtime qualification.

No fixture may run before whole source review and complete exact-command input
admission. All effects are in newly owned temporary fixture roots.
"""
import os
import tempfile
import unittest
from pathlib import Path
from unittest.mock import patch
from held_inputs import HeldInputs, Failure, AccountingExhausted
from command_inputs import CommandInputs, Command, Requirement, utf16_units, quoted_units, environment_block

class CollectorContract(unittest.TestCase):
    def test_outward_dto_cannot_redirect_original_read(self):
        with tempfile.TemporaryDirectory(prefix="template-creator-owned-") as root:
            path = Path(root, "original.txt")
            path.write_bytes(b"original selected bytes")
            owner = HeldInputs()
            try:
                visible = owner.capture_file(str(path))
                object.__setattr__(visible, "ordinal", 65535)
                object.__setattr__(visible, "path", str(Path(root, "foreign.txt")))
                object.__setattr__(visible, "size", 0)
                object.__setattr__(visible, "sha256", "0" * 64)
                self.assertEqual(owner.read_captured(visible), b"original selected bytes")
            finally:
                owner.close()

    def test_real_append_is_not_original_captured_body(self):
        with tempfile.TemporaryDirectory(prefix="template-creator-owned-") as root:
            path = Path(root, "original.txt")
            path.write_bytes(b"before")
            owner = HeldInputs()
            try:
                visible = owner.capture_file(str(path))
                # Windows share denial may prevent the writer, which is itself
                # a useful native outcome; POSIX must catch actual changed bytes.
                try:
                    with path.open("ab") as writer:
                        writer.write(b"after")
                except PermissionError:
                    self.assertEqual(os.name, "nt")
                else:
                    with self.assertRaises(ValueError):
                        owner.read_captured(visible)
            finally:
                owner.close()

    def test_uncertain_close_is_not_repeated_and_other_eligible_closes_run(self):
        with tempfile.TemporaryDirectory(prefix="template-creator-owned-") as root:
            path = Path(root, "original.txt")
            path.write_bytes(b"native close fixture")
            owner = HeldInputs()
            owner.capture_file(str(path))
            actual = os.close
            calls = []
            original = OSError(5, "owned injected close-result failure")
            def uncertain(fd):
                calls.append(fd)
                actual(fd)
                if len(calls) == 1:
                    raise original
            with patch("held_inputs.os.close", uncertain):
                failures = owner.close()
                first_count = len(calls)
                owner.close()
            self.assertGreater(first_count, 1)
            self.assertEqual(len(calls), first_count)
            self.assertEqual(len(set(calls)), first_count)
            self.assertTrue(any(row.original is original for row in failures))
            self.assertTrue(any(row[-1] is original for row in owner.private_close_outcomes()))
            self.assertTrue(owner.has_unresolved_originals())

    def test_physical_close_does_not_erase_prior_qualification_failure(self):
        with tempfile.TemporaryDirectory(prefix="template-creator-owned-") as root:
            path = Path(root, "original.txt")
            path.write_bytes(b"owned retained original")
            owner = HeldInputs()
            owner.capture_file(str(path))
            self.assertTrue(owner.has_unresolved_originals())
            original = ValueError("owned earlier qualification failure")
            owner._record("qualification", "owned regression", None, original)
            owner.close()
            self.assertFalse(owner.has_unresolved_originals())
            self.assertTrue(owner.has_original_failures())
            self.assertIs(owner.private_failures()[0].original, original)
    def test_first_overflow_original_is_absorbing(self):
        owner = HeldInputs()
        owner._failures = [Failure()]
        first, overflow, later = ValueError("first"), OSError(5, "overflow"), RuntimeError("later")
        owner._record("primary", "first", "private", first)
        owner._record("capture", "second", "private", overflow)
        owner._record("disposal", "third", "private", later)
        self.assertIs(owner.private_failures()[0].original, first)
        self.assertIs(owner.private_overflow_original(), overflow)
        with self.assertRaises(AccountingExhausted):
            owner.revalidate()
        with self.assertRaises(AccountingExhausted):
            owner.close()

    def test_foreign_exception_properties_are_not_projected(self):
        class Foreign(Exception):
            @property
            def errno(self):
                raise AssertionError("foreign errno getter executed")
            @property
            def winerror(self):
                raise AssertionError("foreign winerror getter executed")
            def __str__(self):
                raise AssertionError("foreign formatting executed")
        owner = HeldInputs()
        original = Foreign()
        owner._record("capture", "owned_fixture", "private", original)
        row = owner.public_error_codes()[0]
        self.assertEqual(row, {"phase":"capture", "operation":"owned_fixture",
                              "type":"foreign_exception", "errno":None, "winerror":None})
        self.assertIs(owner.private_failures()[0].original, original)
        owner.close()

    def test_scanner_primary_and_close_originals_survive_independently(self):
        with tempfile.TemporaryDirectory(prefix="template-creator-owned-") as root:
            Path(root, "entry").write_bytes(b"entry")
            owner = HeldInputs()
            actual_scandir = os.scandir
            primary, disposal = OSError(5, "owned iterator failure"), OSError(6, "owned scanner close failure")
            class Scanner:
                def __init__(self, path):
                    self.original = actual_scandir(path)
                def __iter__(self):
                    return self
                def __next__(self):
                    raise primary
                def close(self):
                    self.original.close()
                    raise disposal
            try:
                with patch("held_inputs.os.scandir", Scanner):
                    with self.assertRaises(OSError) as caught:
                        owner.capture_membership(root)
                self.assertIs(caught.exception, primary)
                self.assertTrue(any(row.original is primary for row in owner.private_failures()))
                self.assertTrue(any(row.original is disposal for row in owner.private_failures()))
                self.assertTrue(any(row[-1] is disposal for row in owner.private_close_outcomes()))
            finally:
                owner.close()

class CommandFailureContract(unittest.TestCase):
    def test_prior_held_failure_denies_capture_without_new_acquisition(self):
        held = HeldInputs()
        original = OSError(5, "owned prior capture failure")
        held._record("capture", "owned regression", None, original)
        owner = CommandInputs(held)
        try:
            with self.assertRaises(ValueError):
                owner.capture(None, ())
            self.assertEqual(held._used, 0)
            self.assertIs(held.private_failures()[0].original, original)
            self.assertIsNone(owner._captured)
        finally:
            held.close()

    def test_original_command_and_requirement_fields_are_tuple_immutable(self):
        command = Command("/owned/image", (), "/owned", (), None, "original source")
        requirement = Requirement("product", "/owned/image", "0" * 64, 0)
        with self.assertRaises(AttributeError):
            object.__setattr__(command, "image", "/other/image")
        with self.assertRaises(AttributeError):
            object.__setattr__(requirement, "sha256", "1" * 64)

    def test_no_snapshot_and_failed_capture_cannot_be_reused(self):
        held = HeldInputs()
        owner = CommandInputs(held)
        try:
            with self.assertRaises(ValueError):
                owner.revalidate(None)
            with self.assertRaises(ValueError) as first:
                owner.capture(None, ())
            self.assertIs(owner.private_original_errors()[0], first.exception)
            with self.assertRaises(ValueError):
                owner.capture(None, ())
            self.assertIs(owner.private_original_errors()[0], first.exception)
        finally:
            held.close()

    def test_revalidation_original_remains_absorbing_without_new_held_error(self):
        held = HeldInputs()
        owner = CommandInputs(held)
        # Inject only the already-successful snapshot boundary, avoiding native
        # launch/fixture claims. This exercises caller failure composition.
        snapshot = object()
        owner._captured = snapshot
        original = MemoryError("owned revalidation bookkeeping failure")
        try:
            with patch.object(held, "revalidate", side_effect=original):
                with self.assertRaises(MemoryError) as first:
                    owner.revalidate(snapshot)
            self.assertIs(first.exception, original)
            with patch.object(held, "revalidate") as later:
                with self.assertRaises(ValueError):
                    owner.revalidate(snapshot)
                later.assert_not_called()
            self.assertIs(owner.private_original_errors()[1], original)
        finally:
            held.close()

class LiteralCommandContract(unittest.TestCase):
    def test_supplementary_wchar_limit_and_quote_expansion(self):
        self.assertEqual(utf16_units("a\U0001f600", 3), 3)
        with self.assertRaises(ValueError):
            utf16_units("\U0001f600" * 16384, 32766)
        self.assertEqual(quoted_units('a"\\', 7), 7)
        with self.assertRaises(ValueError):
            quoted_units('a"\\', 6)
        with self.assertRaises(ValueError):
            environment_block((("OWNED", "\U0001f600" * 32768),))
        self.assertEqual(environment_block(()), b"\0\0\0\0")

if __name__ == "__main__":
    unittest.main()

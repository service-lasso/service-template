"""UNRUN injected sequencing regressions, never a native implementation adapter.

These test only creator outcome/eligible-input retention after the identified
ordering defect. No fake object below supplies ingress/native authority.
"""
import unittest
from unittest.mock import patch
from creator_entry import TemplateCreator, SelectedStep, RoutePhase
from command_inputs import Command, Requirement, CommandInputs, NativeSubjectObservation
from types import SimpleNamespace
from pathlib import Path

class HeldFixture:
    def __init__(self, fail_close=False):
        self.failed = False
        self.close_count = 0
        self.fail_close = fail_close
    def close(self):
        self.close_count += 1
        self.failed = self.fail_close
        return ('injected-returned-failure',) if self.failed else ()
    def has_original_failures(self):
        return self.failed

class InputsFixture:
    def __init__(self, held):
        self.held = held
    def capture(self, command, requirements, native_classes):
        if native_classes != tuple(None for _ in requirements):
            raise ValueError('injected class-view sequence mismatch')
        return command
    def revalidate(self, captured):
        return captured

class NativeSequenceFixture:
    def __init__(self, eligible=True, fail_begin=False):
        self.eligible = eligible
        self.fail_begin = fail_begin
        self.owner = object()
        self.final_capture = None
        self.dispose_count = 0
        self.final_error = None
        self.closure_results = []
        self.closure_calls = 0
    def adopt_original_ingress(self, *args): pass
    def precharge_complete_route(self, *args): pass
    def capture_input_classes(self, ordinal, step):
        return tuple(None for _ in step.requirements)
    def bind_initial_inputs(self, *args): pass
    def prepare_original_sink(self): pass
    def preowned_command(self, ordinal): return self.owner
    def begin_original(self, *args):
        if self.fail_begin:
            raise RuntimeError('injected original begin failure')
    def step_original(self, *args): pass
    def capture_to_original_sink(self, *args): pass
    def original_command_disposed(self, *args): return True
    def verify_original_command_result(self, *args): pass
    def flush_readback_original_sink(self): pass
    def verify_complete_native_closure(self):
        self.closure_calls += 1
        if self.final_error is not None:
            raise self.final_error
        if self.closure_results:
            return self.closure_results.pop(0)
    def dispose_independently_eligible(self, *args): self.dispose_count += 1
    def verify_input_disposal_eligible(self, *args):
        if not self.eligible:
            raise RuntimeError('injected unresolved original native custody')
    def capture_creator_originals(self, originals, failures):
        self.final_capture = (originals, failures)

class EntryOrderingContract(unittest.TestCase):
    def creator(self, held, native, observations=1):
        # Command bytes irrelevant to this sequencing-only fixture; no execution.
        command = Command('never-executed', ('sequence-fixture',), '.', (), None, 'injected-sequence-only')
        step = SelectedStep('sequence-fixture', command, (), observations, 1)
        with patch('creator_entry.HeldInputs', return_value=held), \
             patch('creator_entry.CommandInputs', side_effect=InputsFixture), \
             patch('creator_entry.NativeSession', NativeSequenceFixture):
            return TemplateCreator(object(), native, RoutePhase.FINAL_QUALIFICATION, (step,))

    def test_returned_input_close_failure_cannot_publish_success(self):
        held = HeldFixture(fail_close=True)
        native = NativeSequenceFixture()
        creator = self.creator(held, native)
        outcome = creator.run_private()
        self.assertEqual(held.close_count, 1)
        self.assertFalse(outcome.qualification_complete)
        self.assertTrue(outcome.retained_failure)
        failures = native.final_capture[1][0]
        self.assertTrue(any(cell.original is held for cell in failures))

    def test_unresolved_native_custody_keeps_original_inputs_open(self):
        held = HeldFixture()
        native = NativeSequenceFixture(eligible=False, fail_begin=True)
        creator = self.creator(held, native)
        outcome = creator.run_private()
        self.assertEqual(held.close_count, 0)
        self.assertFalse(outcome.qualification_complete)
        self.assertEqual(native.dispose_count, 1)
        self.assertIs(creator.private_originals()[1][0], held)

    def test_later_final_exception_remains_on_original_entry(self):
        held = HeldFixture()
        native = NativeSequenceFixture()
        original = RuntimeError('injected failure after original failure snapshot')
        native.final_error = original
        creator = self.creator(held,native)
        outcome = creator.run_private()
        self.assertFalse(outcome.qualification_complete)
        self.assertEqual(native.final_capture[1][0], ())
        self.assertTrue(any(cell.original is original for cell in creator.private_failures()[0]))
        # The actual native host must strongly retain THIS entry, not only the
        # earlier tuple. This sequencing fixture grants no native lifetime proof.

    def test_pending_closure_is_observed_again_before_outcome(self):
        native = NativeSequenceFixture()
        native.closure_results = [False, False, None]
        creator = self.creator(HeldFixture(), native, observations=3)
        outcome = creator.run_private()
        self.assertEqual(native.closure_calls, 3)
        self.assertTrue(outcome.qualification_complete)

    def test_pending_exhaustion_retains_failed_outcome(self):
        native = NativeSequenceFixture()
        native.closure_results = [False, False]
        creator = self.creator(HeldFixture(), native, observations=2)
        outcome = creator.run_private()
        self.assertEqual(native.closure_calls, 2)
        self.assertFalse(outcome.qualification_complete)
        self.assertTrue(any(cell.phase == 'final-custody-readback'
                            for cell in creator.private_failures()[0]))

    def test_truthy_or_falsey_surrogate_is_not_native_closure(self):
        for result in (True, 0, (), ''):
            with self.subTest(result=result):
                native = NativeSequenceFixture()
                native.closure_results = [result]
                creator = self.creator(HeldFixture(), native)
                self.assertFalse(creator.run_private().qualification_complete)

class NativeClassCaptureContract(unittest.TestCase):
    # Actual CommandInputs body with synthetic native geometry and no native IO.
    # These cases cannot qualify the native class producer or source membership.
    def inputs(self):
        owner = object.__new__(CommandInputs)
        owner._captured = None
        owner.held = SimpleNamespace(capture_file=lambda path, size: SimpleNamespace(size=size, sha256='a' * 64),
                                     capture_membership=lambda path: None)
        return owner

    def selection(self):
        image = str(Path('synthetic-selected-tool').absolute())
        unit = str(Path('synthetic-selected-unit').absolute())
        command = Command(image, (), str(Path('.').absolute()), (), None, 'synthetic-only')
        return command, (Requirement('product', image, 'a' * 64, 1),
                         Requirement('compiler-input', unit, 'b' * 64, 4))

    def test_native_range_is_distinct_and_does_not_read_unit_as_file(self):
        owner = self.inputs()
        command, requirements = self.selection()
        calls = []
        owner.held.capture_file = lambda path, size: (calls.append(path) or SimpleNamespace(size=size, sha256='a' * 64))
        geometry = (4, 100, 64, 4)
        result = owner._capture(command, requirements, (None, geometry))
        self.assertEqual(calls, [command.image])
        self.assertIs(type(result.observations[1]), NativeSubjectObservation)
        self.assertIs(result.observations[1].original_geometry, geometry)
        self.assertIs(result.observations[1].requirement, requirements[1])

    def test_malformed_or_wrong_range_is_refused(self):
        command, requirements = self.selection()
        for geometry in ((4, 67, 64, 4), (4, 100, 64, 3), (0, 100, 64, 4), (4, 100, True, 4)):
            with self.subTest(geometry=geometry), self.assertRaises(ValueError):
                self.inputs()._capture(command, requirements, (None, geometry))

    def test_unit_cannot_replace_native_executable(self):
        command, requirements = self.selection()
        with self.assertRaises(ValueError):
            self.inputs()._capture(command, requirements, ((3, 1, 0, 1), None))

    def test_distinct_native_ranges_preserve_same_unit_path_occurrences(self):
        command, requirements = self.selection()
        second = Requirement('configuration', requirements[1].path, 'c' * 64, 5)
        result = self.inputs()._capture(command, requirements + (second,),
                                       (None, (4, 100, 64, 4), (4, 100, 68, 5)))
        self.assertEqual(len(result.observations), 3)
        self.assertIs(result.observations[2].requirement, second)

if __name__ == '__main__':
    unittest.main()

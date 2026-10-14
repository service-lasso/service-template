"""UNRUN producer regression tests. No native custody/authority acceptance."""
import unittest
from selected_route import final_qualification, generation, ObservationSelection
from creator_entry import RoutePhase


class OriginalRouteTests(unittest.TestCase):
    def test_protected_suites_remain_one_original_invocation(self):
        # Empty requirements construct a source proposal only. They cannot grant
        # actual input completeness or pass the native launch associations.
        phase, steps = final_qualification("/original/node", "/original/template", (), (), (),
                                            ObservationSelection(12,1000000), ObservationSelection(3,1000000))
        self.assertIs(phase, RoutePhase.FINAL_QUALIFICATION)
        self.assertEqual(len(steps),2)
        self.assertEqual(steps[0].command.argv, (
            "--test","tests/template-contract.test.mjs",
            "tests/development-candidate-workflow.test.mjs",
            "tests/qualification-publication.test.mjs"))
        self.assertEqual(steps[1].command.argv, (
            "scripts/validate-development-candidate-workflow.mjs",
            ".github/workflows/development-candidate.yml"))
        self.assertIsNone(steps[0].command.original_deadline_ms)
        self.assertIsNone(steps[1].command.original_deadline_ms)

    def test_generation_requires_a_separate_phase(self):
        phase, steps = generation("/original/node", "/original/template", (), (), ObservationSelection(4,1000000))
        self.assertIs(phase, RoutePhase.GENERATION)
        self.assertEqual(tuple(step.command.argv for step in steps), (("scripts/generate-template-contract.mjs",),))

    def test_interval_representation_does_not_invent_100ms_policy(self):
        _, steps = generation("/original/node", "/original/template", (), (), ObservationSelection(4,1000000000))
        self.assertEqual(steps[0].minimum_poll_ns,1000000000)
        with self.assertRaises(ValueError):
            generation("/original/node", "/original/template", (), (), ObservationSelection(4,1 << 64))


if __name__ == "__main__":
    unittest.main()

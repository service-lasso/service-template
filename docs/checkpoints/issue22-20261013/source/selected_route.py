"""UNRUN source-owned original Template command route.

Requirement records and schedule selections are inputs to capture/precharge,
not evidence of complete input authority, actual memory fit or native custody.
Generation changes locked bytes and requires its own admission before the fresh
final-qualification input root. No publication/provider operation is started.
"""
from typing import NamedTuple
from command_inputs import Command, Requirement
from creator_entry import RoutePhase, SelectedStep


class ObservationSelection(NamedTuple):
    maximum_observations: int
    minimum_poll_ns: int


def _step(name, node, argv, root, environment, requirements, observation):
    if type(node) is not str or not node or type(root) is not str or not root:
        raise ValueError("actual selected interpreter and root required")
    if type(environment) is not tuple or any(type(pair) is not tuple or len(pair) != 2
            or type(pair[0]) is not str or type(pair[1]) is not str for pair in environment):
        raise ValueError("literal selected environment required")
    if type(requirements) is not tuple or any(type(row) is not Requirement for row in requirements):
        raise ValueError("original requirement observations required")
    if type(observation) is not ObservationSelection:
        raise ValueError("actual selected observation schedule required")
    if type(observation.maximum_observations) is not int or not 0 < observation.maximum_observations <= 65536:
        raise ValueError("selected finite observation capacity required")
    if type(observation.minimum_poll_ns) is not int or not 0 < observation.minimum_poll_ns < 1 << 64:
        raise ValueError("selected finite observation interval required")
    # The original protected sources impose 15000/30000ms on their descendants.
    # They do not impose either duration on this whole Node invocation.
    command = Command(node,argv,root,environment,None,"original-template-source-route:"+name)
    return SelectedStep(name,command,requirements,observation.maximum_observations,observation.minimum_poll_ns)


def generation(node, root, environment, requirements, observation):
    return RoutePhase.GENERATION, (
        _step("generate-template-contract",node,("scripts/generate-template-contract.mjs",),
              root,environment,requirements,observation),
    )


def final_qualification(node, root, environment, suite_requirements,
                        validator_requirements, suite_observation, validator_observation):
    return RoutePhase.FINAL_QUALIFICATION, (
        _step("unchanged-three-protected-suites",node,(
            "--test","tests/template-contract.test.mjs",
            "tests/development-candidate-workflow.test.mjs",
            "tests/qualification-publication.test.mjs"),root,environment,
            suite_requirements,suite_observation),
        _step("original-development-candidate-workflow-validator",node,(
            "scripts/validate-development-candidate-workflow.mjs",
            ".github/workflows/development-candidate.yml"),root,environment,
            validator_requirements,validator_observation),
    )


# Native-helper/compiler invocations must retain the actual selected compiler,
# toolchain argv/ENV, original generated source and installed/native role inputs.
# This module does not guess those unavailable objects or replace them with a
# portable shell compiler command. The native host supplies that admitted phase.

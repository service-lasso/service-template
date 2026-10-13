"""UNRUN private Template creator route; native adapter integration incomplete.

No public argv/ENV admission. The independently admitted native caller must
supply the actual original ingress and source-bound native session. Neither
object can be replaced by a JSON manifest, signature-shaped string or booleans.
"""
from enum import Enum
from typing import NamedTuple
import time
from command_inputs import Command, CommandInputs, Requirement
from held_inputs import HeldInputs
from native_session import NativeSession

class Role(Enum):
    CALLER = "original-caller"
    ROOT = "input-fixture-evidence-root"
    TOOLS = "original-tools-compiler-libraries"
    CAPACITY = "observer-native-capacity"

class RoutePhase(Enum):
    GENERATION = "separately-admitted-generation"
    FINAL_QUALIFICATION = "separately-admitted-final-qualification"
    NATIVE_HELPER = "separately-admitted-native-helper"

class Availability(Enum):
    UNSUPPLIED = "unsupplied"
    UNKNOWN = "unknown"
    OBSERVED = "observed"  # Observation is not authority/qualification.

class SelectedStep(NamedTuple):
    name: str
    command: Command
    requirements: tuple[Requirement, ...]
    maximum_observations: int
    minimum_poll_ns: int

class OriginalFailure:
    # Cells are allocated before acquisition; originals are references, never str().
    def __init__(self):
        self.phase = None
        self.original = None

class CreatorOutcome(NamedTuple):
    completed_steps: int
    qualification_complete: bool
    retained_failure: bool

class TemplateCreator:
    """Private connected sequencing owner; no automatic source/native grants.

    native is the same source-bound native session that owns the original ingress,
    concrete native gate, platform lifecycle, raw observation sink and capabilities.
    Its concrete C++/private handoff adapter is still being implemented. No fake
    native adapter is provided here and an application callback is not admission.
    """
    def __init__(self, original_ingress, native, phase, steps):
        if type(native) is not NativeSession:
            raise ValueError("source-bound original native session required")
        if type(phase) is not RoutePhase:
            raise ValueError("one independently admitted route phase required")
        self._phase = phase
        if type(steps) is not tuple or not steps or len(steps) > 65:
            raise ValueError("complete bounded original command roster required")
        for step in steps:
            if type(step) is not SelectedStep or type(step.name) is not str or not step.name:
                raise ValueError("exact selected step required")
            if type(step.command) is not Command or type(step.requirements) is not tuple or any(type(item) is not Requirement for item in step.requirements):
                raise ValueError("exact source-owned command and requirement tuple required")
            if type(step.maximum_observations) is not int or not 0 < step.maximum_observations <= 65536:
                raise ValueError("input-derived native observation capacity required")
            # 65536 is the current native stream/event roster representation,
            # not a suite duration. Full per-call precharge must be stricter when
            # one Step consumes multiple native rows or bootstrap observations.
            if type(step.minimum_poll_ns) is not int or not 0 < step.minimum_poll_ns < 1 << 64:
                raise ValueError("finite source-owned observation schedule required")
        self._ingress, self._native, self._steps = original_ingress, native, steps
        self._attempted = False
        self._completed = 0
        self._failures = [OriginalFailure() for _ in range(len(steps) * 4 + 8)]
        self._failure_count = 0
        self._overflow_original = None
        self._recording_original = None
        # Every command input/capture owner exists before any launch. Whole source
        # storage and interpreter/native allocations still require actual fit.
        self._held = tuple(HeldInputs() for _ in steps)
        self._inputs = tuple(CommandInputs(owner) for owner in self._held)
        self._snapshots = [None] * len(steps)
        self._native_owners = [None] * len(steps)
        self._role_states = {role: Availability.UNSUPPLIED for role in Role}

    def _retain(self, phase, original):
        # Failure recording must not interrupt other independently eligible closes.
        try:
            if self._failure_count == len(self._failures):
                if self._overflow_original is None:
                    self._overflow_original = original
                return
            cell = self._failures[self._failure_count]
            cell.original = original
            cell.phase = phase
            self._failure_count += 1
        except BaseException as recording_original:
            if self._overflow_original is None:
                self._overflow_original = original
            if self._recording_original is None:
                self._recording_original = recording_original

    def private_failures(self):
        return (tuple(self._failures[:self._failure_count]),
                self._overflow_original, self._recording_original)
    def private_originals(self):
        return self._ingress, self._held, self._snapshots, self._native_owners

    def _observe_final_closure(self):
        # Separate subdivision, preflighted by the native core before launch.
        # Reusing this selected cadence does not renew any original deadline.
        selection = self._steps[-1]
        next_tick = time.monotonic_ns()
        for observation in range(selection.maximum_observations):
            now = time.monotonic_ns()
            if now < next_tick:
                time.sleep((next_tick - now) / 1000000000)
            result = self._native.verify_complete_native_closure()
            if result is None:
                return
            if result is not False:
                raise RuntimeError("unexpected original native closure result")
            next_tick = time.monotonic_ns() + selection.minimum_poll_ns
        # Native owners remain resident. Exhaustion cannot imply worker exit.
        raise RuntimeError("selected final closure observation capacity exhausted")

    def run_private(self):
        if self._attempted:
            raise ValueError("private original creator entry already attempted")
        self._attempted = True
        try:
            # Concrete session must acquire/reobserve actual supplied originals,
            # hold raw outcomes and authenticate the private caller association.
            # This call is NOT implemented by a manifest/Boolean compatibility stub.
            self._native.adopt_original_ingress(self._ingress, self._phase, tuple(Role))
            for role in Role:
                self._role_states[role] = Availability.UNKNOWN
            self._native.precharge_complete_route(self._steps)
            for ordinal, step in enumerate(self._steps):
                native_classes = self._native.capture_input_classes(ordinal, step)
                if type(native_classes) is not tuple or len(native_classes) != len(step.requirements):
                    raise ValueError("complete original native class-view result required before input capture")
                snapshot = self._inputs[ordinal].capture(step.command, step.requirements, native_classes)
                self._snapshots[ordinal] = snapshot
            # This owner covers ONE admitted phase. Generation and final tests
            # cannot share a stale captured selection: changed generated/final
            # bytes need different whole review/new ROOT and a new entry owner.
            # All initial command inputs are captured before bootstrap. Actual
            # generated inputs are separately captured by the concrete native gate
            # at their stopped child route; original assertions remain unchanged.
            self._native.bind_initial_inputs(self._inputs, self._snapshots)
            self._native.prepare_original_sink()
            for ordinal, step in enumerate(self._steps):
                self._inputs[ordinal].revalidate(self._snapshots[ordinal])
                owner = self._native.preowned_command(ordinal)
                self._native_owners[ordinal] = owner  # Attach BEFORE Begin effects.
                self._native.begin_original(owner, self._snapshots[ordinal])
                observations = 0
                next_tick = time.monotonic_ns()
                while observations < step.maximum_observations:
                    now = time.monotonic_ns()
                    if now < next_tick:
                        time.sleep((next_tick - now) / 1000000000)
                    # Finite platform Step retains actual native outputs first.
                    self._native.step_original(owner)
                    observations += 1
                    next_tick = time.monotonic_ns() + step.minimum_poll_ns
                    self._native.capture_to_original_sink(owner)
                    if self._native.original_command_disposed(owner):
                        break
                else:
                    # Resource schedule exhaustion is retained failure, never a
                    # newly invented suite deadline, termination or EOF witness.
                    raise RuntimeError("selected observation capacity exhausted")
                self._native.verify_original_command_result(owner, step)
                self._completed += 1
            self._native.flush_readback_original_sink()
            # Outcome is published only after eligible input disposal/readback.
        except BaseException as original:
            self._retain("primary", original)
            # Explicit independently eligible cleanup only; native session retains
            # unknown active processes/IO/handles, never kills or fabricates closure.
            for owner in self._native_owners:
                if owner is not None:
                    try:
                        self._native.dispose_independently_eligible(owner)
                    except BaseException as disposal_original:
                        self._retain("native-disposal", disposal_original)
        # No unconditional finally-close: active/unknown native custody may still
        # depend on held inputs and writer exclusion. The concrete source-bound
        # session must establish eligibility from its retained original results.
        for ordinal, held in enumerate(self._held):
            try:
                self._native.verify_input_disposal_eligible(ordinal, held)
            except BaseException as eligibility_original:
                self._retain("input-retained", eligibility_original)
                continue
            try:
                held.close()
            except BaseException as close_original:
                self._retain("input-disposal", close_original)
            if held.has_original_failures():
                # Actual exceptions remain in held's private preowned cells.
                self._retain("input-original-failure", held)
        try:
            self._native.capture_creator_originals(self.private_originals(),
                                                  self.private_failures())
            self._native.flush_readback_original_sink()
            self._observe_final_closure()
        except BaseException as final_original:
            self._retain("final-custody-readback", final_original)
        failed = (self._failure_count != 0 or self._overflow_original is not None
                  or self._recording_original is not None)
        return CreatorOutcome(self._completed, not failed, failed)

# Source sequencing only: concrete native ingress/gate/sink binding remains
# incomplete and must be whole-reviewed before execution. No supplied callback
# or manifest substitutes for actual original custody and native observations.

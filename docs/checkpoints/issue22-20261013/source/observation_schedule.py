"""UNRUN finite observation reservation for an existing per-command deadline.

This does not add a suite deadline or permission to kill. Exhaustion/deadline
marks failed qualification and retains live ownership for eligible cleanup.
"""
from typing import NamedTuple

class RosterCapacity(NamedTuple):
    name: str
    available_rows: int
    fixed_rows: int
    worst_rows_per_step: int

class ReservedSchedule(NamedTuple):
    original_deadline_ns: int
    minimum_poll_ns: int
    maximum_steps: int
    reserved_rows: tuple[tuple[str, int], ...]


def reserve_original_deadline(deadline_ms, rosters):
    if type(deadline_ms) is not int or deadline_ms <= 0:
        raise ValueError('actual original per-command deadline required')
    if type(rosters) is not tuple or not rosters:
        raise ValueError('actual native roster capacities required')
    maximum_steps = None
    names = set()
    for roster in rosters:
        if type(roster) is not RosterCapacity or type(roster.name) is not str:
            raise ValueError('exact source-owned roster required')
        if not roster.name or roster.name in names:
            raise ValueError('unique native roster name required')
        names.add(roster.name)
        for number in roster[1:]:
            if type(number) is not int or number < 0:
                raise ValueError('finite native row reservation required')
        if roster.fixed_rows > roster.available_rows:
            raise ValueError('fixed native observations exceed selected capacity')
        if roster.worst_rows_per_step:
            steps = ((roster.available_rows - roster.fixed_rows)
                     // roster.worst_rows_per_step)
            maximum_steps = steps if maximum_steps is None else min(maximum_steps, steps)
    if maximum_steps is None or maximum_steps < 2:
        raise ValueError('native observation route has no finite usable capacity')
    # Reserve one extra final observation. All other steps are spaced over the
    # already selected command interval; caller checks its actual clock results.
    duration_ns = deadline_ms * 1000000
    interval = (duration_ns + maximum_steps - 2) // (maximum_steps - 1)
    reservations = tuple((r.name, r.fixed_rows + maximum_steps * r.worst_rows_per_step)
                         for r in rosters)
    return ReservedSchedule(duration_ns, interval, maximum_steps, reservations)

class ScheduleClock:
    def __init__(self, reserved, original_start_ns):
        if type(reserved) is not ReservedSchedule:
            raise ValueError('source-owned reserved schedule required')
        if type(original_start_ns) is not int or original_start_ns < 0:
            raise ValueError("original command start clock required")
        self.reserved = reserved
        self.start = original_start_ns
        self.last = None
        self.count = 0
        self.exhausted = False
        self.deadline_observed = False

    def observe_tick(self, original_monotonic_ns):
        if type(original_monotonic_ns) is not int or original_monotonic_ns < 0:
            raise ValueError('original monotonic clock return required')
        if self.exhausted:
            raise ValueError('original observation schedule exhausted')
        if original_monotonic_ns < self.start or (self.last is not None and original_monotonic_ns < self.last):
            self.exhausted = True
            raise ValueError('original monotonic clock regressed')
        self.deadline_observed |= (original_monotonic_ns - self.start
                                  >= self.reserved.original_deadline_ns)
        if self.last is not None and original_monotonic_ns - self.last < self.reserved.minimum_poll_ns:
            return False
        if self.count == self.reserved.maximum_steps:
            self.exhausted = True
            raise ValueError('precharged native observation capacity exhausted')
        self.last = original_monotonic_ns
        self.count += 1
        self.deadline_observed |= (original_monotonic_ns - self.start
                                  >= self.reserved.original_deadline_ns)
        return True

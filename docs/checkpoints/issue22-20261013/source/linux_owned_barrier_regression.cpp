// UNRUN validators for actual retained native scenarios, not an invoked harness.
#include "linux_driver.hpp"
namespace template_creator_linux::regression {
bool OriginalOwnedBarrierHeld(const State& state,size_t requestOrdinal) noexcept {
    if(requestOrdinal>=state.ownedInterruptCount) return false;
    const auto& request=state.ownedInterrupts[requestOrdinal];
    if(!request.gateAttempted||!request.gateReturned||!request.nativeAttempted||
       request.nativeResult!=0||request.observedStopOrdinal>=state.eventCount||
       request.traceeOrdinal>=state.traceeCount) return false;
    const auto& stop=state.events[request.observedStopOrdinal];
    const auto& tracee=state.tracees[request.traceeOrdinal];
    return stop.originalInterruptOrdinal==requestOrdinal && stop.waitAttempted &&
        stop.enrolledTraceeOrdinal==request.traceeOrdinal &&
        stop.requestedTid==tracee.tid && stop.returnedTid==tracee.tid &&
        WIFSTOPPED(stop.status) && OriginalRestartAdmissionShape(stop,RestartAdmissionCut::BeforeReservation) &&
        tracee.nativeStop==stop.nativeStop && stop.nativeStop!=NativeStopKind::NotObserved;
}
bool OriginalOwnedBarrierReleased(const State& state,size_t requestOrdinal) noexcept {
    if(requestOrdinal>=state.ownedInterruptCount) return false;
    const auto& request=state.ownedInterrupts[requestOrdinal];
    if(!request.gateAttempted||!request.gateReturned||!request.nativeAttempted||request.nativeResult!=0||
       request.observedStopOrdinal>=state.eventCount||request.traceeOrdinal>=state.traceeCount) return false;
    const auto& stop=state.events[request.observedStopOrdinal];
    if(stop.originalInterruptOrdinal!=requestOrdinal||stop.enrolledTraceeOrdinal!=request.traceeOrdinal||
       !stop.waitAttempted||!WIFSTOPPED(stop.status)||stop.requestedTid!=stop.returnedTid||
       !OriginalRestartSucceeded(stop)) return false;
    if(stop.nativeStop==NativeStopKind::GroupListening)
        return stop.originalResumeRequest==PTRACE_LISTEN && stop.deliveredSignal==0;
    if(stop.nativeStop==NativeStopKind::SignalDelivery)
        return stop.originalResumeRequest==PTRACE_SYSCALL && stop.deliveredSignal==WSTOPSIG(stop.status);
    return stop.originalResumeRequest==PTRACE_SYSCALL && stop.deliveredSignal==0;
}
}

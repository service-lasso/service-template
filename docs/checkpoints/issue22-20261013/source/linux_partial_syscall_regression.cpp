// UNRUN/UNCOMPILED predicates over actual retained native runs. These do not
// construct wait statuses, syscall outputs, owner associations or gate grants.
#include "linux_driver.hpp"
namespace template_creator_linux::regression {
bool OriginalPartialExit(const State& state,size_t ordinal,PartialSyscallOrigin expected) noexcept {
    if(ordinal>=state.eventCount || expected==PartialSyscallOrigin::None) return false;
    const auto& event=state.events[ordinal];
    if(!event.waitAttempted || event.returnedTid!=event.requestedTid ||
       !WIFSTOPPED(event.status) || WSTOPSIG(event.status)!=(SIGTRAP|0x80) ||
       !event.syscallInfoAttempted || !event.syscallInfoShape ||
       event.syscallInfo.op!=PTRACE_SYSCALL_INFO_EXIT ||
       event.partialSyscallOrigin!=expected || event.linkedEntryOrdinal!=EventLimit) return false;
    if(event.partialInitialStopOrdinal>=ordinal) return false;
    const auto& initial=state.events[event.partialInitialStopOrdinal];
    if(initial.nativeStop!=NativeStopKind::InitialSeized || !initial.waitAttempted ||
       initial.requestedTid!=event.requestedTid || initial.returnedTid!=event.returnedTid ||
       !initial.continued || initial.originalResumeRequest!=PTRACE_SYSCALL) return false;
    if(expected==PartialSyscallOrigin::OriginalSeizeBoundary)
        return state.seizeAttempted && state.seizeResult==0 && state.interruptAttempted &&
            state.interruptResult==0 && event.requestedTid==state.originalForkResult &&
            event.partialBirthEventOrdinal==EventLimit;
    if(event.partialBirthEventOrdinal>=ordinal) return false;
    const auto& birth=state.events[event.partialBirthEventOrdinal];
    const auto kind=static_cast<unsigned>(birth.status)>>16;
    return birth.waitAttempted && WIFSTOPPED(birth.status) && birth.messageResult==0 &&
        birth.message==static_cast<unsigned long>(event.requestedTid) &&
        (kind==PTRACE_EVENT_FORK||kind==PTRACE_EVENT_CLONE||kind==PTRACE_EVENT_VFORK);
}
// An ordinary unmatched EXIT must retain its actual output but cannot obtain
// a fabricated entry or a partial-origin grant. Its native gate is not called.
bool OriginalUnpairedExitDenied(const State& state,size_t ordinal) noexcept {
    if(ordinal>=state.eventCount) return false;
    const auto& event=state.events[ordinal];
    return state.failed && state.continuationDenied && state.firstOperation &&
        std::strcmp(state.firstOperation,"unpaired_native_syscall_exit")==0 &&
        event.syscallInfoAttempted &&
        event.syscallInfoResult>=0 && event.syscallInfo.op==PTRACE_SYSCALL_INFO_EXIT &&
        event.syscallInfoResult>=static_cast<long>(offsetof(struct __ptrace_syscall_info,exit.is_error)+sizeof(event.syscallInfo.exit.is_error)) &&
        event.syscallInfo.exit.is_error<=1 &&
        event.partialSyscallOrigin==PartialSyscallOrigin::None &&
        event.linkedEntryOrdinal==EventLimit && !event.syscallInfoShape && !event.continued;
}
}

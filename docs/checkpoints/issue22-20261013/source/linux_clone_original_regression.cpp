// UNRUN/UNCOMPILED native regression predicates for the ACTUAL retained Owner
// after separately admitted native runs. No fake Gate, status or State producer
// is supplied. These are custody checks, not Linux/source/issuer qualification.
#include "linux_driver.hpp"
namespace template_creator_linux::regression {
bool OriginalParentAndChildCloneResults(const State& original) noexcept {
    if (!original.forkAttempted || original.originalCloneResult<=0 ||
        original.originalCloneResult!=original.originalForkResult || original.rootPidfd>=original.fdCount ||
        original.originalClonePidfdOutput<0 || original.rootPidfdOutputUnknown || original.rootCreationUnknown ||
        !original.childOriginalCount || original.childOriginals[0].operation!=22 ||
        original.childOriginals[0].originalResult!=0 || !original.childOriginals[0].nativeReturned) return false;
    const auto& arguments=original.originalCloneArguments;
    return arguments.flags==CLONE_PIDFD && arguments.exit_signal==SIGCHLD &&
        arguments.pidfd==reinterpret_cast<uintptr_t>(&original.fds[original.rootPidfd].original) &&
        !arguments.stack && !arguments.stack_size && !arguments.parent_tid && !arguments.child_tid &&
        !arguments.tls && !arguments.set_tid && !arguments.set_tid_size && !arguments.cgroup;
}
bool OriginalSuccessfulExecChildLedger(const State& original) noexcept {
    constexpr int operations[]{22,7,8,9,10,11,1,2,23,24,25,5,16};
    if (!OriginalParentAndChildCloneResults(original) || original.childOriginalCount!=sizeof(operations)/sizeof(operations[0]) ||
        !original.setupValidated || !original.setupStream.eof || original.setupStream.used!=0 || !original.terminal ||
        !original.disposed || original.failed) return false;
    for (size_t i=0;i<original.childOriginalCount;++i)
        if (original.childOriginals[i].operation!=operations[i]) return false;
    const auto& attempt=original.childOriginals[12];
    // Success is no-return exec plus genuine native EXEC event and clean setup
    // EOF, never an invented successful returned value from execve.
    return !attempt.nativeReturned && original.traceeCount && original.tracees[0].execEvent &&
        !original.childOriginals[8].nativeReturned && original.childOriginals[9].nativeReturned &&
        original.childOriginals[9].originalResult==1 && !original.childOriginals[9].originalErrno &&
        original.childOriginals[10].nativeReturned && original.childOriginals[10].originalResult==0 &&
        original.seizeAttempted && original.seizeResult==0 && original.interruptAttempted &&
        original.interruptResult==0 && original.launchReleaseAttempted && original.launchReleased;
}
bool OriginalFailedExecChildLedger(const State& original,int expectedNativeExecError) noexcept {
    constexpr int operations[]{22,7,8,9,10,11,1,2,23,24,25,5,16,6,19,20,21};
    if (!OriginalParentAndChildCloneResults(original) || !expectedNativeExecError ||
        original.childOriginalCount!=sizeof(operations)/sizeof(operations[0]) || !original.failed ||
        !original.terminal || !original.disposed || !original.childFailureWriteAttempted ||
        original.childFailureRequested!=sizeof(ChildSetupFailure) || !original.setupStream.eof ||
        original.setupStream.used!=sizeof(ChildSetupFailure)) return false;
    for (size_t i=0;i<original.childOriginalCount;++i)
        if (original.childOriginals[i].operation!=operations[i]) return false;
    const auto& returned=original.childOriginals[13];
    const auto& write=original.childOriginals[15];
    return !original.childOriginals[12].nativeReturned && returned.nativeReturned && returned.originalResult==-1 &&
        returned.originalErrno==expectedNativeExecError && original.childFailureBody.operation==6 &&
        original.childFailureBody.originalErrno==expectedNativeExecError &&
        original.originalSetupFailure.originalErrno==expectedNativeExecError && write.nativeReturned &&
        write.originalResult==static_cast<long>(sizeof(ChildSetupFailure)) && !write.originalErrno &&
        !original.childOriginals[16].nativeReturned && WIFEXITED(original.tracees[0].terminalStatus) &&
        WEXITSTATUS(original.tracees[0].terminalStatus)==127;
}
// Short/failed failure-write cases require their actual raw return/EOF/native
// results and remain separate admitted native runs; no full-write inference.
}

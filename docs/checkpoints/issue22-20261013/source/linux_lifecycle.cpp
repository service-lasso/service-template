// UNCOMPILED selected fresh-owned seized/syscall lifecycle. No ambient attach, detach, EXITKILL,
// kill/terminate API or changes to another worker. Kernel capabilities, libc,
// compiler, input/root authority and actual installed outcomes remain unproved.
#define _GNU_SOURCE
#include <sys/ptrace.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <sys/syscall.h>
#include "linux_original_deadline.hpp"
#if defined(__x86_64__) && !defined(__ILP32__)
#include <sys/user.h>
#endif
#include <linux/sched.h>
#include <linux/audit.h>
#include <cstddef>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <errno.h>
#include <array>
#include <atomic>
#include <new>
#include <cstdint>
#include <memory>
#include <exception>
#include <cstring>
namespace template_creator_linux {
constexpr size_t TraceeLimit = 4096, EventLimit = 65536, FdLimit = 16384;
struct Fd {
    int original = -1;
    bool attempted = false, closed = false;
    int closeError = 0, closeResult = -1;
    void CloseOnce() noexcept {
        if (original < 0 || attempted) return;
        attempted = true;
        closeResult = close(original);
        if (closeResult == 0) closed = true;
        else closeError = errno; // EINTR is uncertain; never retry this number.
    }
};
enum class NativeStopKind { NotObserved, InitialSeized, OwnedBarrier, SyscallEntry, SyscallExit, Event, SignalDelivery, GroupListening, Terminal };
enum class PartialSyscallOrigin { None, OriginalSeizeBoundary, NativeBirthBoundary };
struct Tracee {
    pid_t tid = 0, originalCreatorEventTid = 0;
    // Exact retained causal rows, not a later search by reusable numeric TID.
    size_t creatorTraceeOrdinal = TraceeLimit, creatorEventOrdinal = EventLimit;
    unsigned long forkEvent = 0;
    bool initialStop = true, execEvent = false, exitEvent = false, terminal = false;
    int terminalStatus = 0;
    bool supersededByExec = false, waitIdentityPending = false;
    size_t pendingWaitEvent = EventLimit;
    size_t replacementTracee = TraceeLimit, execEventOrdinal = EventLimit;
    unsigned long exitEventStatus = 0;
    bool seizedOriginal=false;
    size_t pendingSyscallEntry=EventLimit;
    PartialSyscallOrigin firstSyscallBoundary=PartialSyscallOrigin::None;
    size_t initialSeizedStopOrdinal=EventLimit;
    size_t pendingOwnedInterrupt=EventLimit;
    NativeStopKind nativeStop=NativeStopKind::NotObserved;
};
enum class WaitAssociation { NotObserved, BoundOriginal, RetainedPending, Denied };
struct Event {
    bool deferredSetupObservation=false;
    bool setupPrefixCaptureReturned=false;size_t setupPrefixBefore=0,setupPrefixAfter=0;
    bool childPrefixCaptureAttempted=false,childPrefixCaptureReturned=false;
    size_t childPrefixBefore=0,childPrefixAfter=0;
    const void* childPrefixMapOriginal=nullptr;
    size_t childPrefixMapBytesOriginal=0;
    bool childPrefixMapLiveOriginal=false;
    bool childLaunchByteCaptured=false;unsigned char childLaunchByteOriginal=0;
    // Fixed owning output, never a pointer to transient register storage.
    bool registersAttempted=false,registersReturned=false,registersCaptured=false;
    long registersResult=-1;int registersError=0;
    OriginalClockBoundary registersBeforeClock{},registersAfterClock{};
#if defined(__x86_64__) && !defined(__ILP32__)
    struct user_regs_struct originalRegisters{};
#endif
    size_t enrolledTraceeOrdinal=TraceeLimit;
    std::exception_ptr beforeWaitException, beforeTerminalException, continuationException,syscallGateException;
    WaitAssociation beforeWaitAssociation = WaitAssociation::NotObserved;
    bool waitAttempted = false;
    pid_t requestedTid = 0, returnedTid = 0;
    int status = 0, waitError = 0;
    unsigned long message = 0;
    long messageResult = 0, optionsResult = 0, continueResult = 0;
    int messageError = 0, optionsError = 0, continueError = 0;
    int deliveredSignal = 0;
    siginfo_t signalInformation{};
    long signalInformationResult = 0;
    int signalInformationError = 0;
    bool continued = false, restartAttempted=false;
    bool restartAdmissionActive=false,continueNativeAttempted=false,continueNativeReturned=false;
    NativeStopKind nativeStop=NativeStopKind::NotObserved;
    bool syscallInfoAttempted=false,syscallInfoShape=false;
    struct __ptrace_syscall_info syscallInfo{};
    long syscallInfoResult=-1;int syscallInfoError=0;
    size_t linkedEntryOrdinal=EventLimit;
    PartialSyscallOrigin partialSyscallOrigin=PartialSyscallOrigin::None;
    size_t partialBirthEventOrdinal=EventLimit;
    size_t partialInitialStopOrdinal=EventLimit;
    size_t originalInterruptOrdinal=EventLimit;
    enum __ptrace_request originalResumeRequest=PTRACE_SYSCALL;
    size_t formerExecTracee = TraceeLimit, replacementExecTracee = TraceeLimit;
};
enum class RestartAdmissionCut { BeforeReservation, ReservedGate };
inline bool OriginalRestartAdmissionShape(const Event& event,RestartAdmissionCut cut) noexcept {
    if(event.continued||event.continueNativeAttempted||event.continueNativeReturned||event.continuationException)return false;
    switch(cut){
        case RestartAdmissionCut::BeforeReservation:return !event.restartAttempted&&!event.restartAdmissionActive;
        case RestartAdmissionCut::ReservedGate:return event.restartAttempted&&event.restartAdmissionActive;
    }
    return false;
}
inline bool OriginalRestartSucceeded(const Event& event) noexcept {
    return event.restartAttempted&&!event.restartAdmissionActive&&event.continueNativeAttempted&&
        event.continueNativeReturned&&event.continued&&event.continueResult==0&&!event.continueError&&
        !event.continuationException;
}
struct ChildSetupFailure { int operation, originalErrno; };
struct ChildNativeOutcome {
    int operation = 0;
    long originalResult = 0;
    int originalErrno = 0;
    bool nativeReturned = true;
};
struct SharedChildOutcomes {
    // clone return + six dup2 + fcntl/close_range + launch read attempt/return/close +
    // fchdir/cwd-close + exec attempt/return + failure write attempt/return/_exit =19.
    std::array<ChildNativeOutcome, 19> rows{};
    ChildSetupFailure failureBody{};
    int failureWriter = -1;
    size_t failureRequested = 0;
    unsigned char launchByte=0;
    std::atomic<unsigned> published{0};
};
static_assert(ATOMIC_INT_LOCK_FREE == 2, "child publication requires lock-free native unsigned atomics");
struct ReadResult {
    bool attempted = false;
    ssize_t original = 0;
    int originalErrno = 0;
    size_t destinationOffset = 0, requested = 0;
};
struct Reader {
    static constexpr size_t ByteLimit = 8 * 1024 * 1024;
    Fd* capability = nullptr;
    std::unique_ptr<unsigned char[]> bytes = std::make_unique<unsigned char[]>(ByteLimit);
    std::unique_ptr<ReadResult[]> results = std::make_unique<ReadResult[]>(EventLimit);
    size_t used = 0, resultCount = 0;
    unsigned char growthProbe = 0;
    std::unique_ptr<unsigned char[]> discard = std::make_unique<unsigned char[]>(ByteLimit);
    size_t discardRetained = 0;
    std::unique_ptr<ReadResult[]> discardResults = std::make_unique<ReadResult[]>(EventLimit);
    size_t discardResultCount = 0;
    uint64_t discardedBytes = 0;
    bool discardUsed = false, discardEof = false, discardFailed = false;
    bool eof = false, failed = false, exhausted = false;
    bool Poll() noexcept {
        if (!capability || capability->original < 0 || capability->attempted || eof || failed) return false;
        if (resultCount == EventLimit) { exhausted = failed = true; return false; }
        ReadResult& row = results[resultCount++];
        row.destinationOffset = used;
        bool probe = used == ByteLimit;
        row.requested = probe ? 1 : ByteLimit - used > 65536 ? 65536 : ByteLimit - used;
        row.attempted = true;
        row.original = read(capability->original, probe ? &growthProbe : bytes.get() + used, row.requested);
        if (row.original < 0) {
            row.originalErrno = errno;
            if (errno != EAGAIN && errno != EWOULDBLOCK) failed = true;
            return false;
        }
        if (!row.original) { eof = true; return false; }
        if (probe) { exhausted = failed = true; return false; }
        if (static_cast<size_t>(row.original) > row.requested) { failed = true; return false; }
        used += static_cast<size_t>(row.original);
        return true;
    }
    bool DisposeEligible(bool originalTraceesTerminal) noexcept {
        if (!originalTraceesTerminal || !(eof || discardEof) || !capability) return false;
        capability->CloseOnce();
        return capability->closed;
    }
    bool DrainPoll() noexcept {
        if (!failed || !capability || capability->attempted || discardEof || discardFailed) return false;
        discardUsed = true;
        if (discardResultCount == EventLimit) { discardFailed = true; return false; }
        ReadResult& row = discardResults[discardResultCount++];
        row.destinationOffset = discardRetained;
        if (discardRetained == ByteLimit) { discardFailed = true; return false; }
        row.requested = ByteLimit - discardRetained > 65536 ? 65536 : ByteLimit - discardRetained;
        row.attempted = true;
        row.original = read(capability->original, discard.get() + discardRetained, row.requested);
        if (row.original < 0) {
            row.originalErrno = errno;
            if (errno != EAGAIN && errno != EWOULDBLOCK) discardFailed = true;
            return false;
        }
        if (!row.original) { discardEof = true; return false; }
        if (static_cast<size_t>(row.original) > row.requested ||
                static_cast<uint64_t>(row.original) > UINT64_MAX - discardedBytes) {
            discardFailed = true; return false;
        }
        discardRetained += static_cast<size_t>(row.original);
        discardedBytes += static_cast<uint64_t>(row.original);
        return true;
    }
};
struct SetupOutcome {
    bool attempted = false;
    const char* operation = nullptr;
    long result = 0;
    int originalErrno = 0;
    std::array<int, 2> descriptors{{-1,-1}};
};
struct ThreadOriginal {
    long result = -1;
    int originalErrno = 0;
};
struct OwnedInterruptOriginal {
    size_t traceeOrdinal=TraceeLimit;
    bool gateAttempted=false,gateReturned=false,nativeAttempted=false;
    long nativeResult=-1;
    int nativeError=0;
    size_t observedStopOrdinal=EventLimit;
    size_t terminalBeforeStopOrdinal=EventLimit;
    std::exception_ptr gateException;
};
struct State {
    bool unlaunchedDisposalAttempted = false, unlaunchedDisposalResult = false;
    static constexpr size_t ArgumentLimit = 4097, CommandByteLimit = 1024 * 1024;
    std::array<char, 32768> originalImage{}, originalCwd{};
    std::array<char, CommandByteLimit> originalArgBytes{}, originalEnvBytes{};
    std::array<char*, ArgumentLimit + 1> originalArgv{}, originalEnvironment{};
    size_t imageBytes = 0, cwdBytes = 0, argumentBytes = 0, environmentBytes = 0;
    size_t argumentCount = 0, environmentCount = 0;
    Reader stdoutStream, stderrStream, setupStream;
    std::array<SetupOutcome, 16> setupOutcomes{};
    size_t setupOutcomeCount = 0;
    std::array<ThreadOriginal, EventLimit> threadOriginals{};
    ThreadOriginal originalCreatingThread{};
    bool creatingThreadAttempted = false, creatingThreadShapeValid = false;
    size_t threadOriginalCount = 0;
    std::array<Tracee, TraceeLimit> tracees{};
    std::array<Event, EventLimit> events{};
    std::array<OwnedInterruptOriginal,EventLimit> ownedInterrupts{};
    size_t ownedInterruptCount=0;
    std::array<Fd, FdLimit> fds{};
    size_t traceeCount = 0, eventCount = 0, fdCount = 0, nextTracee = 0;
    size_t currentRootTracee = 0;
    pid_t originalForkResult = -1;
    // Legacy caller field above now contains normalized clone3 child PID; the
    // genuine operation/ABI/raw result and atomic pidfd output stay separate.
    struct clone_args originalCloneArguments{},nativeCloneArguments{};
    long originalCloneResult = -1;
    int originalClonePidfdOutput = -1;
    bool rootCreationUnknown = false, rootPidfdOutputUnknown = false;
    bool forkAttempted = false;
    bool seizeGateAttempted=false,seizeGateReturned=false;
    std::exception_ptr seizeGateException;
    bool seizeAttempted=false,seized=false,interruptAttempted=false;
    long seizeResult=-1,interruptResult=-1;int seizeError=0,interruptError=0;
    unsigned long originalSeizeOptions=PTRACE_O_TRACESYSGOOD|PTRACE_O_TRACEFORK|PTRACE_O_TRACEVFORK|PTRACE_O_TRACECLONE|PTRACE_O_TRACEVFORKDONE|PTRACE_O_TRACEEXEC|PTRACE_O_TRACEEXIT;
    bool launchReleaseAttempted=false,launchReleased=false;ssize_t launchWriteResult=-1;int launchWriteError=0;
    unsigned char originalLaunchByte=1;
    bool initialGateAttempted = false,initialGateReturned = false;
    bool finalGateAttempted = false,finalGateReturned = false;
    std::array<int, 6> childSourceOriginals{{-1,-1,-1,-1,-1,-1}};
    std::array<int, 6> duplicateErrors{};
    std::array<size_t, 6> childSources{{FdLimit,FdLimit,FdLimit,FdLimit,FdLimit,FdLimit}};
    int forkError = 0, rootPidfdError = 0;
    size_t rootPidfd = FdLimit;
    bool failed = false, continuationDenied = false, terminal = false, disposed = false;
    const char* firstOperation = nullptr;
    int firstError = 0;
    std::exception_ptr originalGateException;
    std::exception_ptr initialGateException, finalGateException, failureDrainGateException;
    bool setupValidated = false;
    ChildSetupFailure originalSetupFailure{};
    void* originalChildMapResult = MAP_FAILED;
    bool pageSizeAttempted = false;
    long originalPageSize = -1;
    int pageSizeErrno = 0;
    size_t childMapBytes = 0;
    void* childMapAddressOriginal = nullptr;
    int childMapProtectionOriginal = 0, childMapFlagsOriginal = 0;
    int childMapFdOriginal = -1;
    off_t childMapOffsetOriginal = 0;
    int childMapErrno = 0;
    bool childMapAttempted = false, childUnmapAttempted = false, childUnmapped = false;
    int childUnmapResult = -1, childUnmapErrno = 0;
    SharedChildOutcomes* childShared = nullptr;
    std::array<ChildNativeOutcome, 19> childOriginals{};
    unsigned childOriginalCount = 0;
    ChildSetupFailure childFailureBody{};
    int childFailureWriter = -1;
    size_t childFailureRequested = 0;
    bool childFailureWriteAttempted = false;
    Event overflowEvent{};
    bool overflowPresent = false;
};
class Gate {
public:
    virtual ~Gate() = default;
    virtual bool BeforeFork(const char*, const char* const*, const char* const*, const char*, int, int) = 0;
    virtual bool BeforeOwnedSeize(const Tracee&,const State&)=0;
    virtual bool BeforeOwnedInterrupt(const Tracee&,const OwnedInterruptOriginal&,const State&)=0;
    virtual bool BeforeSyscallEntry(const Tracee&,Event&,const State&)=0;
    virtual bool BeforeSyscallExit(const Tracee&,Event&,const State&)=0;
    virtual WaitAssociation BeforeWait(const Tracee&, const Event&, const State&) = 0;
    virtual bool BeforeContinue(const Tracee&, const Event&, const State&) = 0;
    // Must derive same ORIGINAL native lifetime (including any ECHILD gap)
    // from retained native identity; numeric TID/status equality is insufficient.
    virtual bool BeforeTerminal(const Tracee&, const Event&, const State&) = 0;
    virtual bool BeforeFailureContinue(const Tracee&, const Event&, const State&) = 0;
    virtual bool BeforeFailureDrain(const State&) = 0;
    virtual bool ReservedSourceBytes(size_t&) const noexcept = 0;
    virtual bool DisposeOriginalTaskInputsEligible() noexcept = 0;
};
// Source causality is necessary but does not replace native task/pidfd identity.
// A concrete gate can consume this exact retained join without PID-only search.
inline bool RetainedBirthAssociation(const Tracee& candidate, const State& state) noexcept {
    size_t ordinal = TraceeLimit;
    for (size_t i=0; i<state.traceeCount; ++i)
        if (&state.tracees[i] == &candidate) { ordinal = i; break; }
    if (ordinal == TraceeLimit) return false;
    if (ordinal == 0)
        return state.forkAttempted && state.originalCloneResult > 0 &&
            !state.rootCreationUnknown && !state.rootPidfdOutputUnknown &&
            state.originalCloneResult == candidate.tid && state.rootPidfd < state.fdCount &&
            state.originalClonePidfdOutput >= 0 &&
            state.fds[state.rootPidfd].original == state.originalClonePidfdOutput;
    if (candidate.execEventOrdinal < state.eventCount) {
        const auto& transition = state.events[candidate.execEventOrdinal];
        return transition.waitAttempted && transition.returnedTid == candidate.tid &&
            WIFSTOPPED(transition.status) &&
            (static_cast<unsigned>(transition.status)>>16) == PTRACE_EVENT_EXEC &&
            transition.messageResult == 0 && transition.replacementExecTracee == ordinal &&
            transition.formerExecTracee < ordinal &&
            transition.message == static_cast<unsigned long>(state.tracees[transition.formerExecTracee].tid) &&
            state.tracees[transition.formerExecTracee].replacementTracee == ordinal;
    }
    if (candidate.creatorTraceeOrdinal >= ordinal || candidate.creatorEventOrdinal >= state.eventCount) return false;
    const auto& parent = state.tracees[candidate.creatorTraceeOrdinal];
    const auto& birth = state.events[candidate.creatorEventOrdinal];
    const unsigned kind = static_cast<unsigned>(birth.status)>>16;
    return birth.waitAttempted && birth.returnedTid == parent.tid &&
        birth.enrolledTraceeOrdinal==candidate.creatorTraceeOrdinal &&
        birth.requestedTid == parent.tid && WIFSTOPPED(birth.status) &&
        (kind == PTRACE_EVENT_FORK || kind == PTRACE_EVENT_VFORK || kind == PTRACE_EVENT_CLONE) &&
        birth.messageResult == 0 && birth.message == static_cast<unsigned long>(candidate.tid) &&
        candidate.originalCreatorEventTid == parent.tid && candidate.forkEvent == kind;
}
class Owner {
    std::unique_ptr<State> state_ = std::make_unique<State>();
    pid_t ownerTid_ = 0;
    size_t stdoutRead_ = FdLimit, stdoutWrite_ = FdLimit;
    size_t stderrRead_ = FdLimit, stderrWrite_ = FdLimit;
    size_t setupRead_ = FdLimit, setupWrite_ = FdLimit;
    size_t launchRead_=FdLimit,launchWrite_=FdLimit;
    void Fail(const char* op, int original, bool halt = true) noexcept {
        state_->failed = true; state_->continuationDenied |= halt;
        if (!state_->firstOperation) { state_->firstOperation = op; state_->firstError = original; }
    }
    bool Thread() noexcept {
        auto& s = *state_;
        if (s.threadOriginalCount == s.threadOriginals.size()) {
            Fail("thread_result_capacity_before_call", ENOMEM); return false;
        }
        auto& row = s.threadOriginals[s.threadOriginalCount++];
        row.result = syscall(SYS_gettid);
        if (row.result < 0) row.originalErrno = errno;
        if (s.creatingThreadShapeValid && row.result == ownerTid_) return true;
        Fail("same_tracer_thread", EINVAL); return false;
    }
    SetupOutcome* SetupRow(const char* operation) noexcept {
        auto& s = *state_;
        if (s.setupOutcomeCount == s.setupOutcomes.size()) {
            Fail("setup_result_capacity_before_call", ENOMEM); return nullptr;
        }
        auto& row = s.setupOutcomes[s.setupOutcomeCount++];
        row.operation = operation; return &row;
    }
    bool Pipe(size_t& reader, size_t& writer) noexcept {
        auto& s = *state_;
        if (s.fdCount > FdLimit - 2) { Fail("fd_capacity", ENOMEM); return false; }
        reader = s.fdCount++; writer = s.fdCount++;
        auto pipeRow = SetupRow("pipe2");
        if (!pipeRow) return false;
        pipeRow->attempted = true;
        pipeRow->result = pipe2(pipeRow->descriptors.data(), O_CLOEXEC);
        if (pipeRow->result != 0) {
            pipeRow->originalErrno = errno; Fail("pipe2", pipeRow->originalErrno); return false;
        }
        s.fds[reader].original = pipeRow->descriptors[0]; s.fds[writer].original = pipeRow->descriptors[1];
        auto flagsRow = SetupRow("read_nonblocking");
        if (!flagsRow) return false;
        flagsRow->descriptors[0] = pipeRow->descriptors[0];
        flagsRow->attempted = true;
        flagsRow->result = fcntl(flagsRow->descriptors[0], F_SETFL, O_NONBLOCK);
        if (flagsRow->result != 0) {
            flagsRow->originalErrno = errno; Fail("read_nonblocking", flagsRow->originalErrno); return false;
        }
        return true;
    }
    bool LaunchPipe() noexcept {
        auto& s=*state_;if(s.fdCount>FdLimit-2){Fail("launch_pipe_capacity",ENOMEM);return false;}
        launchRead_=s.fdCount++;launchWrite_=s.fdCount++;
        auto* row=SetupRow("private_launch_pipe2");if(!row)return false;row->attempted=true;
        row->result=pipe2(row->descriptors.data(),O_CLOEXEC);
        if(row->result<0){row->originalErrno=errno;Fail("private_launch_pipe2",row->originalErrno);return false;}
        s.fds[launchRead_].original=row->descriptors[0];s.fds[launchWrite_].original=row->descriptors[1];return true;
    }
    template<size_t Capacity>
    static bool CopyLiteral(const char* source, std::array<char, Capacity>& destination,
                            size_t& copied) noexcept {
        if (!source) return false;
        for (size_t i=0; i<Capacity; ++i) {
            destination[i] = source[i];
            if (!source[i]) { copied = i + 1; return true; }
        }
        return false;
    }
    static bool CopyVector(char* const* source, std::array<char, State::CommandByteLimit>& bytes,
                           std::array<char*, State::ArgumentLimit + 1>& pointers,
                           size_t& count, size_t& used) noexcept {
        if (!source) return false;
        for (size_t i=0; i<=State::ArgumentLimit; ++i) {
            if (!source[i]) { pointers[i] = nullptr; count = i; return true; }
            if (i == State::ArgumentLimit) return false;
            pointers[i] = bytes.data() + used;
            bool ended = false;
            const char* original = source[i];
            while (used < bytes.size()) {
                char value = *original++;
                bytes[used++] = value;
                if (!value) { ended = true; break; }
            }
            if (!ended) return false;
        }
        return false;
    }
    static void ChildObserve(SharedChildOutcomes* shared, int operation,
                             long result, int originalErrno, bool nativeReturned = true) noexcept {
        const unsigned ordinal = shared->published.load(std::memory_order_relaxed);
        // clone return + six dup2 + fcntl/close_range + launch read
        // attempt/return/close + fchdir/cwd-close + exec attempt/return + failure
        // attempt/return/_exit fit EXACTLY19 cells on the longest path.
        // No child allocation, lock, native retry or overwritten observation.
        if (ordinal >= shared->rows.size()) _exit(127);
        shared->rows[ordinal] = {operation, result, originalErrno, nativeReturned};
        shared->published.store(ordinal + 1, std::memory_order_release);
    }
    static void ChildFailure(SharedChildOutcomes* shared, int writer,
                             int operation, int originalErrno) noexcept {
        ChildSetupFailure value{operation, originalErrno};
        shared->failureBody = value;
        shared->failureWriter = writer;
        shared->failureRequested = sizeof(value);
        ChildObserve(shared, 19, 0, 0, false); // Raw failure-write inputs published before call.
        const ssize_t result = write(writer, &value, sizeof(value));
        const int writeError = result < 0 ? errno : 0;
        ChildObserve(shared, 20, result, writeError);
        ChildObserve(shared, 21, 0, 0, false); // _exit has no native return.
        _exit(127); // Original owned setup failure, never observer termination.
    }
    bool CaptureChildOutputs() noexcept {
        auto& s = *state_;
        if (!s.childShared || s.childUnmapped) return true;
        const unsigned count = s.childShared->published.load(std::memory_order_acquire);
        if (count > s.childOriginals.size()) { Fail("child_native_output_shape", 0); return false; }
        while (s.childOriginalCount < count) {
            const unsigned ordinal = s.childOriginalCount++;
            s.childOriginals[ordinal] = s.childShared->rows[ordinal];
            if (s.childOriginals[ordinal].operation == 19) {
                s.childFailureBody = s.childShared->failureBody;
                s.childFailureWriter = s.childShared->failureWriter;
                s.childFailureRequested = s.childShared->failureRequested;
                s.childFailureWriteAttempted = true;
            }
        }
        return true;
    }
    bool UnmapEligibleChildOutputs() noexcept {
        auto& s = *state_;
        if (!CaptureChildOutputs()) return false; // Preserve unknown raw map, continue other eligible disposal.
        const bool acquired=s.childMapAttempted && s.originalChildMapResult!=MAP_FAILED;
        if (acquired && !s.childUnmapAttempted) {
            s.childUnmapAttempted = true;
            s.childUnmapResult = munmap(s.originalChildMapResult, s.childMapBytes);
            if (s.childUnmapResult < 0) s.childUnmapErrno = errno;
            else s.childUnmapped = true;
        }
        return !acquired || s.childUnmapped;
    }
public:
    Owner() {
        auto& s = *state_;
        s.creatingThreadAttempted = true;
        s.originalCreatingThread.result = syscall(SYS_gettid);
        if (s.originalCreatingThread.result < 0) s.originalCreatingThread.originalErrno = errno;
        s.creatingThreadShapeValid = s.originalCreatingThread.result > 0 &&
            s.originalCreatingThread.result <= INT32_MAX;
        if (s.creatingThreadShapeValid) ownerTid_ = static_cast<pid_t>(s.originalCreatingThread.result);
        else Fail("original_creating_thread",s.originalCreatingThread.originalErrno);
    }
    const State& Current() const noexcept { return *state_; }
    // Source-only barrier producer. The concrete gate must join original live
    // ownership and the existing deadline before this native request. A returned
    // request is not a stopped-thread/map-stability proof; only its later exact
    // retained wait row can provide the causal observation.
    bool RequestOwnedInterrupt(size_t traceeOrdinal,Gate& gate) {
        auto& s=*state_;
        if(!Thread()||s.continuationDenied||traceeOrdinal>=s.traceeCount||
           s.ownedInterruptCount==s.ownedInterrupts.size()) return false;
        auto& tracee=s.tracees[traceeOrdinal];
        if(!tracee.seizedOriginal||tracee.initialStop||tracee.terminal||tracee.supersededByExec||
           tracee.pendingOwnedInterrupt!=EventLimit) return false;
        const size_t ordinal=s.ownedInterruptCount++;
        auto& row=s.ownedInterrupts[ordinal];row.traceeOrdinal=traceeOrdinal;
        row.gateAttempted=true;
        try { row.gateReturned=gate.BeforeOwnedInterrupt(tracee,row,s); }
        catch(...) { row.gateException=std::current_exception();if(!s.originalGateException)s.originalGateException=row.gateException;Fail("owned_interrupt_gate_exception",0);return false; }
        if(!row.gateReturned){Fail("owned_interrupt_gate",EACCES);return false;}
        row.nativeAttempted=true;
        row.nativeResult=ptrace(PTRACE_INTERRUPT,tracee.tid,nullptr,nullptr);
        if(row.nativeResult<0){row.nativeError=errno;Fail("owned_barrier_INTERRUPT",row.nativeError);return false;}
        tracee.pendingOwnedInterrupt=ordinal;return true;
    }
    bool ReleaseOwnedBarrier(size_t eventOrdinal,Gate& gate) {
        auto& s=*state_;
        if(!Thread()||s.continuationDenied||eventOrdinal>=s.eventCount) return false;
        auto& event=s.events[eventOrdinal];
        if(event.nativeStop==NativeStopKind::NotObserved||event.restartAttempted||
           event.originalInterruptOrdinal>=s.ownedInterruptCount) return false;
        const auto& request=s.ownedInterrupts[event.originalInterruptOrdinal];
        if(request.observedStopOrdinal!=eventOrdinal||request.traceeOrdinal>=s.traceeCount) return false;
        auto& tracee=s.tracees[request.traceeOrdinal];
        if(tracee.terminal||tracee.supersededByExec||tracee.nativeStop!=event.nativeStop) return false;
        // Once this exact stop's admission is attempted, do not retry a failed
        // callback/native restart. Independent natural terminal capture remains.
        event.restartAttempted=true;event.restartAdmissionActive=true;
        try {
            if(!(s.failed?gate.BeforeFailureContinue(tracee,event,s):gate.BeforeContinue(tracee,event,s))){event.restartAdmissionActive=false;Fail("owned_barrier_release_gate",EACCES);return false;}
        }catch(...){event.restartAdmissionActive=false;event.continuationException=std::current_exception();if(!s.originalGateException)s.originalGateException=event.continuationException;Fail("owned_barrier_release_exception",0);return false;}
        event.restartAdmissionActive=false;event.continueNativeAttempted=true;
        event.continueResult=ptrace(event.originalResumeRequest,tracee.tid,nullptr,
            reinterpret_cast<void*>(static_cast<intptr_t>(event.deliveredSignal)));
        event.continueNativeReturned=true;
        if(event.continueResult<0){event.continueError=errno;Fail("owned_barrier_release",event.continueError);return false;}
        event.continued=true;return true;
    }
    bool ReservedSourceBytes(size_t& bytes) const noexcept {
        bytes=sizeof(*this)+sizeof(State);
        if (!state_->pageSizeAttempted || state_->originalPageSize<=0 || !state_->childMapBytes) return false;
        const size_t dynamicReaders=3*(2*Reader::ByteLimit+2*EventLimit*sizeof(ReadResult));
        if (dynamicReaders>SIZE_MAX-bytes) return false;
        bytes+=dynamicReaders;
        if (state_->childMapBytes>SIZE_MAX-bytes) return false;
        bytes+=state_->childMapBytes;
        return true; // Actual rounded requested mapping, not native/runtime measured fit.
    }
    int Stdout() const noexcept { return stdoutRead_ == FdLimit ? -1 : state_->fds[stdoutRead_].original; }
    int Stderr() const noexcept { return stderrRead_ == FdLimit ? -1 : state_->fds[stderrRead_].original; }
    int Setup() const noexcept { return setupRead_ == FdLimit ? -1 : state_->fds[setupRead_].original; }
    bool Begin(const char* image, char* const* argv, char* const* environment,
               const char* cwd, int originalEmptyStdin, int originalDirectory, Gate& gate) {
        auto& s = *state_;
        if (!Thread() || s.originalForkResult != -1 || s.failed) return false;
        if (!CopyLiteral(image, s.originalImage, s.imageBytes) ||
                !CopyLiteral(cwd, s.originalCwd, s.cwdBytes) ||
                !CopyVector(argv, s.originalArgBytes, s.originalArgv, s.argumentCount, s.argumentBytes) ||
                !CopyVector(environment, s.originalEnvBytes, s.originalEnvironment, s.environmentCount, s.environmentBytes) ||
                !s.argumentCount || originalEmptyStdin<0 || originalDirectory<0 ||
                s.originalImage[0] != '/' || s.originalCwd[0] != '/') {
            Fail("literal_original_command_capacity_or_shape", EINVAL); return false;
        }
        image = s.originalImage.data(); cwd = s.originalCwd.data();
        argv = s.originalArgv.data(); environment = s.originalEnvironment.data();
        // Immutable storage/complete original selection and native rights are
        // concrete gate obligations; no inherited envp or ambient PATH search.
        try {
            s.initialGateAttempted=true;
            s.initialGateReturned=gate.BeforeFork(image, argv, environment, cwd, originalEmptyStdin, originalDirectory);
            if (!s.initialGateReturned) {
                Fail("complete_before_fork", EACCES); return false;
            }
        } catch (...) {
            s.initialGateException = std::current_exception();
            if (!s.originalGateException) s.originalGateException = s.initialGateException;
            Fail("BeforeFork_exception", 0); return false;
        }
        s.pageSizeAttempted = true;
        errno = 0;
        s.originalPageSize = sysconf(_SC_PAGESIZE);
        if (s.originalPageSize < 0) s.pageSizeErrno = errno;
        if (s.originalPageSize <= 0 || static_cast<uint64_t>(s.originalPageSize)>SIZE_MAX ||
            sizeof(SharedChildOutcomes)>SIZE_MAX-(static_cast<size_t>(s.originalPageSize)-1)) {
            Fail("original_child_map_page_geometry", s.pageSizeErrno); return false;
        }
        const size_t pageBytes=static_cast<size_t>(s.originalPageSize);
        s.childMapBytes=((sizeof(SharedChildOutcomes)+pageBytes-1)/pageBytes)*pageBytes;
        s.childMapAddressOriginal = nullptr;
        s.childMapProtectionOriginal = PROT_READ | PROT_WRITE;
        s.childMapFlagsOriginal = MAP_SHARED | MAP_ANONYMOUS;
        s.childMapFdOriginal = -1;
        s.childMapOffsetOriginal = 0;
        s.childMapAttempted = true;
        s.originalChildMapResult = mmap(s.childMapAddressOriginal, s.childMapBytes,
                                        s.childMapProtectionOriginal, s.childMapFlagsOriginal,
                                        s.childMapFdOriginal, s.childMapOffsetOriginal);
        if (s.originalChildMapResult == MAP_FAILED) {
            s.childMapErrno = errno; Fail("shared_child_original_map", s.childMapErrno); return false;
        }
        if (!s.originalChildMapResult) {
            // Address zero is a native mapping outcome, not MAP_FAILED. Retain
            // it for eligible munmap without constructing a C++ null object.
            Fail("shared_child_map_null_source_representation",0); return false;
        }
        s.childShared = new (s.originalChildMapResult) SharedChildOutcomes{};
        if (!Pipe(stdoutRead_, stdoutWrite_) || !Pipe(stderrRead_, stderrWrite_) || !Pipe(setupRead_, setupWrite_) || !LaunchPipe()) return false;
        s.stdoutStream.capability = &s.fds[stdoutRead_];
        s.stderrStream.capability = &s.fds[stderrRead_];
        s.setupStream.capability = &s.fds[setupRead_];
        // Source capabilities are duplicated above all child destinations BEFORE
        // clone3. Thus closed ambient 0/1/2 or reused pipe descriptor numbers can
        // never make sequential dup2 clobber a still-needed original source.
        s.childSourceOriginals = {originalEmptyStdin, s.fds[stdoutWrite_].original,
                                 s.fds[stderrWrite_].original, s.fds[setupWrite_].original,s.fds[launchRead_].original,originalDirectory};
        for (size_t i=0; i<s.childSources.size(); ++i) {
            if (s.fdCount == FdLimit) { Fail("child_mapping_capacity", ENOMEM); return false; }
            s.childSources[i] = s.fdCount++;
            auto duplicateRow = SetupRow("child_source_duplicate");
            if (!duplicateRow) return false;
            duplicateRow->descriptors[0] = s.childSourceOriginals[i];
            duplicateRow->attempted = true;
            duplicateRow->result = fcntl(s.childSourceOriginals[i], F_DUPFD_CLOEXEC, 6);
            s.fds[s.childSources[i]].original = static_cast<int>(duplicateRow->result);
            if (s.fds[s.childSources[i]].original < 0) {
                duplicateRow->originalErrno = errno;
                s.duplicateErrors[i] = duplicateRow->originalErrno; Fail("child_source_duplicate", s.duplicateErrors[i]); return false;
            }
        }
        if (s.fdCount == FdLimit) { Fail("pidfd_capacity", ENOMEM); return false; }
        s.rootPidfd = s.fdCount++; // Kernel output cell owned before creation.
        auto forkRow = SetupRow("clone3_CLONE_PIDFD");
        if (!forkRow) return false;
        s.originalCloneArguments.flags = CLONE_PIDFD;
        s.originalCloneArguments.pidfd = reinterpret_cast<uintptr_t>(&s.fds[s.rootPidfd].original);
        s.originalCloneArguments.exit_signal = SIGCHLD;
        // No CLONE_VM/THREAD/PARENT/namespace/credential/privilege changes. A
        // null stack uses the original copied-address-space stack, not shared VM.
        s.nativeCloneArguments = s.originalCloneArguments;
        // Revalidate SAME originals after actual map/page/pipe/capability setup
        // and BEFORE creation. Initial admission does not certify later capture
        // outputs or whole simultaneous storage merely because setup returned.
        try {
            s.finalGateAttempted=true;
            s.finalGateReturned=gate.BeforeFork(image,argv,environment,cwd,originalEmptyStdin,originalDirectory);
            if (!s.finalGateReturned) { Fail("complete_final_before_clone",EACCES); return false; }
        } catch (...) {
            s.finalGateException=std::current_exception();
            if (!s.originalGateException) s.originalGateException=s.finalGateException;
            Fail("final_BeforeFork_exception",0); return false;
        }
        s.forkAttempted = true; forkRow->attempted = true;
        s.originalCloneResult = syscall(SYS_clone3,&s.nativeCloneArguments,sizeof(s.nativeCloneArguments));
        forkRow->result = s.originalCloneResult;
        if (s.originalCloneResult < 0) { forkRow->originalErrno = errno; s.forkError = forkRow->originalErrno; }
        s.originalClonePidfdOutput = s.fds[s.rootPidfd].original;
        forkRow->descriptors[0] = s.originalClonePidfdOutput;
        if (s.originalCloneResult == 0) {
            // Child private State is NOT a parent observation. Publish its
            // genuine zero return before ANY later child native operation.
            ChildObserve(s.childShared, 22, s.originalCloneResult, 0);
            // Async-signal-safe child route, with exact inherited capability set.
            int setupWriter = s.fds[s.childSources[3]].original;
            for (size_t i=0; i<s.childSources.size(); ++i) {
                const int result = dup2(s.fds[s.childSources[i]].original, static_cast<int>(i));
                const int originalErrno = result < 0 ? errno : 0;
                ChildObserve(s.childShared, static_cast<int>(7+i), result, originalErrno);
                if (result < 0) ChildFailure(s.childShared, setupWriter, static_cast<int>(7+i), originalErrno);
            }
            long result = fcntl(3, F_SETFD, FD_CLOEXEC);
            int originalErrno = result < 0 ? errno : 0;
            ChildObserve(s.childShared, 1, result, originalErrno);
            if (result < 0) ChildFailure(s.childShared, 3, 1, originalErrno);
            result = syscall(SYS_close_range, 6u, ~0u, 0u);
            originalErrno = result < 0 ? errno : 0;
            ChildObserve(s.childShared, 2, result, originalErrno);
            if (result < 0) ChildFailure(s.childShared, 3, 2, originalErrno);
            // Source-owned trampoline waits for actual parent seized stop. This
            // private launch input is not a product/native admission Boolean.
            ChildObserve(s.childShared,23,1,0,false);
            result=read(4,&s.childShared->launchByte,1);
            originalErrno=result<0?errno:0;
            ChildObserve(s.childShared,24,result,originalErrno);
            if(result!=1||s.childShared->launchByte!=1)ChildFailure(s.childShared,3,24,originalErrno);
            result=close(4);originalErrno=result<0?errno:0;
            ChildObserve(s.childShared,25,result,originalErrno);
            if(result<0)ChildFailure(s.childShared,3,25,originalErrno);
            result = fchdir(5);
            originalErrno = result != 0 ? errno : 0;
            ChildObserve(s.childShared, 5, result, originalErrno);
            if (result != 0) ChildFailure(s.childShared, 3, 5, originalErrno);
            result=close(5);originalErrno=result<0?errno:0;
            ChildObserve(s.childShared,26,result,originalErrno);
            if(result<0)ChildFailure(s.childShared,3,26,originalErrno);
            ChildObserve(s.childShared, 16, 0, 0, false); // Original exec attempt, no fabricated return.
            result = execve(image, argv, environment);
            originalErrno = errno;
            ChildObserve(s.childShared, 6, result, originalErrno);
            ChildFailure(s.childShared, 3, 6, originalErrno);
        }
        if (s.originalCloneResult < 0) {
            s.rootPidfdOutputUnknown = s.originalClonePidfdOutput >= 0;
            Fail("clone3_CLONE_PIDFD", s.forkError); return false;
        }
        if (s.originalCloneResult > INT32_MAX) {
            s.rootCreationUnknown = true;
            Fail("clone3_original_child_pid_shape", 0); return false;
        }
        s.originalForkResult = static_cast<pid_t>(s.originalCloneResult);
        // Register the actual original child before any later native operation.
        s.tracees[s.traceeCount++].tid = s.originalForkResult;
        if (s.originalClonePidfdOutput < 0) {
            s.rootPidfdOutputUnknown = true;
            Fail("clone3_original_pidfd_output_shape", 0);
        }
        // These parent copies are independently eligible after original birth.
        // Close them before any gate/seize failure can return; launch writer
        // remains held separately until an actual admitted release or failure.
        s.fds[launchRead_].CloseOnce();
        if(!s.fds[launchRead_].closed)Fail("parent_launch_reader_close",s.fds[launchRead_].closeError);
        s.fds[stdoutWrite_].CloseOnce(); s.fds[stderrWrite_].CloseOnce(); s.fds[setupWrite_].CloseOnce();
        for (auto index : {stdoutWrite_, stderrWrite_, setupWrite_})
            if (!s.fds[index].closed) Fail("parent_writer_close", s.fds[index].closeError);
        for (auto index : s.childSources) {
            s.fds[index].CloseOnce();
            if (!s.fds[index].closed) Fail("parent_child_source_close", s.fds[index].closeError);
        }
        // Failure closes only this parent's original private writer. An
        // unseized child can observe actual EOF and take its recorded setup
        // failure route. A seized/stopped child remains retained; this close
        // is neither terminal proof nor permission to restart it.
        const auto failLaunch = [&]() noexcept {
            s.fds[launchWrite_].CloseOnce();
            if(!s.fds[launchWrite_].closed) Fail("failed_launch_writer_close",s.fds[launchWrite_].closeError);
        };
        if(s.failed) { failLaunch(); return false; }
        return !s.failed; // Owned pending setup; bounded Poll observes op23.
    }
    bool AdvanceDeferredSeize(Gate& gate) {
        auto& s=*state_;
        const auto failLaunch=[&]() noexcept {
            s.fds[launchWrite_].CloseOnce();
            if(!s.fds[launchWrite_].closed)Fail("failed_launch_writer_close",s.fds[launchWrite_].closeError);
        };
        if(!Thread()||s.failed||s.seizeGateAttempted||s.seizeAttempted||s.rootPidfdOutputUnknown)return false;
        size_t sourceReadAttempts=0;const ChildNativeOutcome* sourceReadAttempt=nullptr;
        for(size_t i=0;i<s.childOriginalCount;++i)if(s.childOriginals[i].operation==23){
            ++sourceReadAttempts;sourceReadAttempt=&s.childOriginals[i];
        }
        if(sourceReadAttempts!=1||!sourceReadAttempt||sourceReadAttempt->nativeReturned||
           sourceReadAttempt->originalResult!=1||sourceReadAttempt->originalErrno!=0)return false;
        if(!s.rootPidfdOutputUnknown) {
            try {
                s.seizeGateAttempted=true;s.seizeGateReturned=gate.BeforeOwnedSeize(s.tracees[0],s);
                if(!s.seizeGateReturned){Fail("original_owned_seize_gate",EACCES);failLaunch();return false;}
            }catch(...){s.seizeGateException=std::current_exception();if(!s.originalGateException)s.originalGateException=s.seizeGateException;Fail("owned_seize_gate_exception",0);failLaunch();return false;}
            s.seizeAttempted=true;
            s.seizeResult=ptrace(PTRACE_SEIZE,s.originalForkResult,nullptr,reinterpret_cast<void*>(s.originalSeizeOptions));
            if(s.seizeResult<0){s.seizeError=errno;Fail("owned_PTRACE_SEIZE",s.seizeError);failLaunch();return false;}
            s.seized=true;s.tracees[0].seizedOriginal=true;
            s.interruptAttempted=true;s.interruptResult=ptrace(PTRACE_INTERRUPT,s.originalForkResult,nullptr,nullptr);
            if(s.interruptResult<0){s.interruptError=errno;Fail("owned_initial_INTERRUPT",s.interruptError);failLaunch();return false;}
        }
        return !s.failed;
    }
    // Exactly one waitpid for one enrolled original tracee; never waitpid(-1)
    // or consume another child/worker's status. All actual raw statuses retained.
    bool Poll(Gate& gate) {
        auto& s = *state_;
        if (!Thread() || s.terminal || !s.traceeCount) return false;
        if (s.eventCount == EventLimit) { Fail("event_capacity_before_wait", ENOMEM); return false; }
        size_t visited = 0;
        while (visited++ < s.traceeCount && (s.tracees[s.nextTracee].terminal || s.tracees[s.nextTracee].supersededByExec))
            s.nextTracee = (s.nextTracee + 1) % s.traceeCount;
        size_t selectedTracee = s.nextTracee;
        Tracee& tracee = s.tracees[selectedTracee];
        Tracee* continuingTracee = &tracee;
        s.nextTracee = (s.nextTracee + 1) % s.traceeCount;
        if (tracee.terminal || tracee.supersededByExec) { s.terminal = true; return false; }
        // Pending ECHILD never suppresses a later genuine status for this TID;
        // bounded round-robin observations continue until native reconciliation.
        Event& e = s.events[s.eventCount++]; e.requestedTid = tracee.tid;
        e.enrolledTraceeOrdinal=selectedTracee;
        if(!s.seized&&!s.seizeAttempted&&!s.failed){
            e.deferredSetupObservation=true;e.setupPrefixBefore=s.childOriginalCount;
            e.setupPrefixCaptureReturned=CaptureChildOutputs();
            e.setupPrefixAfter=s.childOriginalCount;
            if(!e.setupPrefixCaptureReturned)return false;
            size_t attempts=0;const ChildNativeOutcome* attempt=nullptr;
            for(size_t i=0;i<e.setupPrefixAfter;++i)if(s.childOriginals[i].operation==23){
                ++attempts;attempt=&s.childOriginals[i];
            }
            if(attempts==1&&attempt&&!attempt->nativeReturned&&attempt->originalResult==1&&attempt->originalErrno==0)
                return AdvanceDeferredSeize(gate);
            if(attempts>1||s.childFailureWriteAttempted){
                Fail("original_child_preseize_setup_failure",0);
                s.fds[launchWrite_].CloseOnce();
                if(!s.fds[launchWrite_].closed)Fail("failed_launch_writer_close",s.fds[launchWrite_].closeError);
            }else return false; // No wait/resume, exactly one reserved observation.
        }
        try {
            e.beforeWaitAssociation = gate.BeforeWait(tracee, e, s);
        } catch (...) {
            e.beforeWaitException = std::current_exception();
            if (!s.originalGateException) s.originalGateException = e.beforeWaitException;
            Fail("BeforeWait_exception", 0); return false;
        }
        if (e.beforeWaitAssociation == WaitAssociation::RetainedPending) return false;
        if (e.beforeWaitAssociation != WaitAssociation::BoundOriginal) {
            Fail("original_native_before_wait_identity_gate", EACCES); return false;
        }
        e.waitAttempted = true;
        e.returnedTid = waitpid(tracee.tid, &e.status, WNOHANG | __WALL);
        if (e.returnedTid < 0) {
            e.waitError = errno;
            if (e.waitError == ECHILD) {
                // A nonleader exec can remove this enrolled TID before its
                // leader EXEC event is observed. Only that native event may
                // reconcile it; this row is NOT terminal or successful exit.
                if (!tracee.waitIdentityPending) tracee.pendingWaitEvent = s.eventCount - 1;
                tracee.waitIdentityPending = true;
            } else Fail("waitpid", e.waitError);
            return false;
        }
        if (!e.returnedTid) return false;
        if (e.returnedTid != tracee.tid) { Fail("wait_identity", ECHILD); return false; }
        // Source ledger publication is append-only. Acquire and retain this
        // exact stopped/terminal event's prefix before any consumer callback;
        // disposal-only capture cannot supply a prior syscall association.
        e.childPrefixBefore=s.childOriginalCount;
        e.childPrefixMapOriginal=s.childShared;
        e.childPrefixMapBytesOriginal=s.childMapBytes;
        e.childPrefixMapLiveOriginal=s.childMapAttempted&&s.childShared&&
            s.childShared==s.originalChildMapResult&&!s.childUnmapAttempted&&!s.childUnmapped;
        e.childPrefixCaptureAttempted=true;
        e.childPrefixCaptureReturned=CaptureChildOutputs();
        e.childPrefixAfter=s.childOriginalCount;
        if(!e.childPrefixCaptureReturned)return false;
        if(s.childShared&&!s.childUnmapped){
            e.childLaunchByteOriginal=s.childShared->launchByte;
            e.childLaunchByteCaptured=true;
        }
        if (WIFEXITED(e.status) || WIFSIGNALED(e.status)) {
            try {
                if (!gate.BeforeTerminal(tracee, e, s)) {
                    Fail("original_native_terminal_identity_gate", EACCES); return false;
                }
            } catch (...) {
                e.beforeTerminalException = std::current_exception();
                if (!s.originalGateException) s.originalGateException = e.beforeTerminalException;
                Fail("BeforeTerminal_exception", 0); return false;
            }
            tracee.waitIdentityPending = false;
            if(tracee.pendingOwnedInterrupt<s.ownedInterruptCount) {
                auto& request=s.ownedInterrupts[tracee.pendingOwnedInterrupt];
                request.terminalBeforeStopOrdinal=s.eventCount-1;
                tracee.pendingOwnedInterrupt=EventLimit;
                // Actual terminal can permit physical cleanup, but cannot
                // substitute for this requested stopped-task observation.
                Fail("owned_barrier_terminal_before_stop",0,false);
            }
            tracee.nativeStop=e.nativeStop=NativeStopKind::Terminal;
            tracee.terminalStatus = e.status; tracee.terminal = true;
            bool all = true;
            for (size_t i=0; i<s.traceeCount; ++i) all &= s.tracees[i].terminal || s.tracees[i].supersededByExec;
            s.terminal = all;
            return true;
        }
        // Failed continuation admission does not erase independently observed
        // natural terminal rows above. It never authorizes further stop-query
        // or restart effects below; the actual stop stays retained unchanged.
        if(s.continuationDenied) return false;
        if (!WIFSTOPPED(e.status)) { Fail("unknown_wait_status", EINVAL); return false; }
        if(tracee.pendingOwnedInterrupt<s.ownedInterruptCount) {
            auto& request=s.ownedInterrupts[tracee.pendingOwnedInterrupt];
            if(request.traceeOrdinal!=selectedTracee||!request.nativeAttempted||request.nativeResult!=0||
               request.observedStopOrdinal!=EventLimit){Fail("owned_barrier_association",EINVAL);return false;}
            // Preserve the actual stop tag below. INTERRUPT can coincide with
            // syscall/signal stops; no synthetic EVENT_STOP is inserted.
            request.observedStopOrdinal=s.eventCount-1;
            e.originalInterruptOrdinal=tracee.pendingOwnedInterrupt;
            tracee.pendingOwnedInterrupt=EventLimit;
        }
        unsigned event = static_cast<unsigned>(e.status) >> 16;
        int signal = WSTOPSIG(e.status);
        if (tracee.initialStop && event==PTRACE_EVENT_STOP && signal==SIGTRAP) {
            if(!s.seized||!tracee.seizedOriginal){Fail("initial_stop_without_original_seize",EINVAL);return false;}
            tracee.initialStop=false;tracee.nativeStop=e.nativeStop=NativeStopKind::InitialSeized;
            tracee.firstSyscallBoundary=selectedTracee==0?PartialSyscallOrigin::OriginalSeizeBoundary:PartialSyscallOrigin::NativeBirthBoundary;
            tracee.initialSeizedStopOrdinal=s.eventCount-1;
            e.deliveredSignal=0;
        } else if (!event && signal==(SIGTRAP|0x80)) {
            if(!tracee.seizedOriginal){Fail("syscall_stop_without_original_seize",EINVAL);return false;}
            e.syscallInfoAttempted=true;
            e.syscallInfoResult=ptrace(PTRACE_GET_SYSCALL_INFO,tracee.tid,
                reinterpret_cast<void*>(sizeof(e.syscallInfo)),&e.syscallInfo);
            if(e.syscallInfoResult<0){e.syscallInfoError=errno;Fail("PTRACE_GET_SYSCALL_INFO",e.syscallInfoError);return false;}
            const size_t common=offsetof(struct __ptrace_syscall_info,entry);
            size_t required=common;
            if(e.syscallInfoResult<static_cast<long>(common)||
               e.syscallInfoResult>static_cast<long>(sizeof(e.syscallInfo))){Fail("syscall_info_available_length",EINVAL);return false;}
#if defined(__x86_64__) && !defined(__ILP32__)
            const bool arch=e.syscallInfo.arch==AUDIT_ARCH_X86_64;
#elif defined(__aarch64__) && !defined(__ILP32__)
            const bool arch=e.syscallInfo.arch==AUDIT_ARCH_AARCH64;
#else
            const bool arch=false;
#endif
            if(!arch){Fail("unqualified_native_syscall_ABI",ENOTSUP);return false;}
            if(e.syscallInfo.op==PTRACE_SYSCALL_INFO_ENTRY) {
                required=offsetof(struct __ptrace_syscall_info,entry.args)+sizeof(e.syscallInfo.entry.args);
                if(e.syscallInfoResult<static_cast<long>(required)||tracee.pendingSyscallEntry!=EventLimit){Fail("unpaired_native_syscall_entry",EINVAL);return false;}
                tracee.firstSyscallBoundary=PartialSyscallOrigin::None;
                tracee.pendingSyscallEntry=s.eventCount-1;
                tracee.nativeStop=e.nativeStop=NativeStopKind::SyscallEntry;
            }else if(e.syscallInfo.op==PTRACE_SYSCALL_INFO_EXIT) {
                required=offsetof(struct __ptrace_syscall_info,exit.is_error)+sizeof(e.syscallInfo.exit.is_error);
                if(e.syscallInfoResult<static_cast<long>(required)||e.syscallInfo.exit.is_error>1){Fail("native_syscall_exit_shape",EINVAL);return false;}
                if(tracee.pendingSyscallEntry<s.eventCount-1) e.linkedEntryOrdinal=tracee.pendingSyscallEntry;
                else {
                    // Retain a partial call, never a fabricated preceding entry.
                    // BeforeSyscallExit must bind the actual trampoline/birth ABI;
                    // the causal boundary alone grants no input consumption.
                    if(tracee.firstSyscallBoundary==PartialSyscallOrigin::None){Fail("unpaired_native_syscall_exit",EINVAL);return false;}
                    e.partialSyscallOrigin=tracee.firstSyscallBoundary;
                    e.partialBirthEventOrdinal=tracee.creatorEventOrdinal;
                    e.partialInitialStopOrdinal=tracee.initialSeizedStopOrdinal;
                }
                tracee.firstSyscallBoundary=PartialSyscallOrigin::None;
                tracee.pendingSyscallEntry=EventLimit;
                tracee.nativeStop=e.nativeStop=NativeStopKind::SyscallExit;
            }else{Fail("unselected_syscall_info_tag",ENOTSUP);return false;}
            e.syscallInfoShape=true;e.deliveredSignal=0;
            try {
                const bool admitted=e.nativeStop==NativeStopKind::SyscallEntry?
                    gate.BeforeSyscallEntry(tracee,e,s):gate.BeforeSyscallExit(tracee,e,s);
                if(!admitted){Fail("original_syscall_input_output_gate",EACCES);return false;}
            }catch(...){e.syscallGateException=std::current_exception();if(!s.originalGateException)s.originalGateException=e.syscallGateException;Fail("syscall_gate_exception",0);return false;}
        } else if(event==PTRACE_EVENT_STOP) {
            if(signal==SIGTRAP&&e.originalInterruptOrdinal<s.ownedInterruptCount) {
                tracee.nativeStop=e.nativeStop=NativeStopKind::OwnedBarrier;e.deliveredSignal=0;
            } else if(signal==SIGSTOP||signal==SIGTSTP||signal==SIGTTIN||signal==SIGTTOU) {
                tracee.nativeStop=e.nativeStop=NativeStopKind::GroupListening;
                e.originalResumeRequest=PTRACE_LISTEN;e.deliveredSignal=0;
            }else{Fail("unassociated_seized_event_stop",EINVAL);return false;}
        } else if (event) {
            tracee.nativeStop=e.nativeStop=NativeStopKind::Event;
            e.messageResult = ptrace(PTRACE_GETEVENTMSG, tracee.tid, nullptr, &e.message);
            if (e.messageResult < 0) { e.messageError = errno; Fail("PTRACE_GETEVENTMSG", e.messageError); return false; }
            if (event == PTRACE_EVENT_FORK || event == PTRACE_EVENT_VFORK || event == PTRACE_EVENT_CLONE) {
                if (s.traceeCount == TraceeLimit || !e.message || e.message > INT32_MAX) {
                    s.overflowEvent = e; s.overflowPresent = true; Fail("tracee_capacity_identity", ENOMEM); return false;
                }
                Tracee& child = s.tracees[s.traceeCount++];
                child.tid = static_cast<pid_t>(e.message); child.originalCreatorEventTid = tracee.tid; child.forkEvent = event;
                child.creatorTraceeOrdinal = selectedTracee;
                child.creatorEventOrdinal = s.eventCount-1;child.seizedOriginal=tracee.seizedOriginal;
            } else if(event==PTRACE_EVENT_VFORK_DONE) {
                // Raw completion event/message stays distinct from child terminal.
            } else if (event == PTRACE_EVENT_EXIT) {
                tracee.exitEvent = true; tracee.exitEventStatus = e.message;
            } else if (event == PTRACE_EVENT_EXEC) {
                if (!e.message || e.message > INT32_MAX) {
                    Fail("original_exec_former_tid_shape", EINVAL); return false;
                }
                if (e.message == static_cast<unsigned long>(tracee.tid)) {
                    tracee.execEvent = true;
                } else {
                    size_t former = TraceeLimit;
                    for (size_t i=0; i<s.traceeCount; ++i)
                        if (s.tracees[i].tid == static_cast<pid_t>(e.message) &&
                                !s.tracees[i].terminal && !s.tracees[i].supersededByExec) {
                            if (former != TraceeLimit) { Fail("ambiguous_owned_exec_former_tid", EINVAL); return false; }
                            former = i;
                        }
                    if (former == TraceeLimit || former == selectedTracee || s.traceeCount == TraceeLimit) {
                        Fail("owned_exec_identity_or_capacity", EINVAL); return false;
                    }
                    // The actual EXEC event + GETEVENTMSG attests the former
                    // enrolled TID becoming this enrolled leader TID. Retain
                    // both old records unchanged as to exit/status; replacement
                    // is a distinct native transition, NEVER invented exit.
                    size_t replacement = s.traceeCount++;
                    Tracee& next = s.tracees[replacement];
                    next.tid = tracee.tid; next.originalCreatorEventTid = s.tracees[former].originalCreatorEventTid;
                    next.creatorTraceeOrdinal = s.tracees[former].creatorTraceeOrdinal;
                    next.creatorEventOrdinal = s.tracees[former].creatorEventOrdinal;
                    next.initialStop = false; next.execEvent = true;next.seizedOriginal=tracee.seizedOriginal;
                    next.nativeStop=e.nativeStop; // SAME genuine EXEC stop, not a new observation.
                    next.pendingSyscallEntry=s.tracees[former].pendingSyscallEntry;
                    next.execEventOrdinal = s.eventCount - 1;
                    e.formerExecTracee = former; e.replacementExecTracee = replacement;
                    tracee.supersededByExec = true; tracee.replacementTracee = replacement;
                    tracee.waitIdentityPending = false;
                    s.tracees[former].supersededByExec = true;
                    s.tracees[former].waitIdentityPending = false;
                    s.tracees[former].replacementTracee = replacement;
                    if (s.currentRootTracee == selectedTracee) s.currentRootTracee = replacement;
                    continuingTracee = &next;
                }
            } else { Fail("unselected_ptrace_event", ENOTSUP); return false; }
            e.deliveredSignal = 0;
        } else {
            e.signalInformationResult = ptrace(PTRACE_GETSIGINFO, tracee.tid, nullptr, &e.signalInformation);
            if (e.signalInformationResult < 0) {
                e.signalInformationError = errno;
                // Seized group-stops belong to PTRACE_EVENT_STOP above.
                // A conflicting signal-information failure is retained as
                // unknown; do not reinterpret it as a LISTEN/restart grant.
                if (errno == EINVAL && (signal == SIGSTOP || signal == SIGTSTP ||
                        signal == SIGTTIN || signal == SIGTTOU))
                    Fail("original_group_stop_retained", 0);
                else Fail("PTRACE_GETSIGINFO", e.signalInformationError);
                return false;
            }
            // Preserve actual application signal-delivery stop; unknown SIGTRAP
            // is not swallowed as a debugger breakpoint or synthetic success.
            tracee.nativeStop=e.nativeStop=NativeStopKind::SignalDelivery;
            e.deliveredSignal = signal;
        }
        if(e.originalInterruptOrdinal<s.ownedInterruptCount) return true; // Held until exact ReleaseOwnedBarrier.
        e.restartAttempted=true;e.restartAdmissionActive=true;
        try {
            if (!(s.failed ? gate.BeforeFailureContinue(*continuingTracee, e, s) : gate.BeforeContinue(*continuingTracee, e, s))) {
                e.restartAdmissionActive=false;
                Fail("precontinuation_input_gate", EACCES); return false;
            }
        } catch (...) {
            e.restartAdmissionActive=false;
            e.continuationException = std::current_exception();
            if (!s.originalGateException) s.originalGateException = e.continuationException;
            Fail("continuation_gate_exception", 0); return false;
        }
        e.restartAdmissionActive=false;
        if(e.nativeStop==NativeStopKind::InitialSeized&&selectedTracee==0&&!s.launchReleaseAttempted) {
            s.launchReleaseAttempted=true;auto* row=SetupRow("original_private_launch_release");if(!row)return false;
            row->attempted=true;row->result=write(s.fds[launchWrite_].original,&s.originalLaunchByte,1);
            s.launchWriteResult=row->result;if(row->result<0){row->originalErrno=errno;s.launchWriteError=row->originalErrno;}
            s.launchReleased=row->result==1;if(!s.launchReleased){Fail("original_private_launch_release",s.launchWriteError);return false;}
            s.fds[launchWrite_].CloseOnce();if(!s.fds[launchWrite_].closed){Fail("parent_launch_writer_close",s.fds[launchWrite_].closeError);return false;}
        }
        e.continueNativeAttempted=true;
        e.continueResult = ptrace(e.originalResumeRequest, continuingTracee->tid, nullptr,
                                 reinterpret_cast<void*>(static_cast<intptr_t>(e.deliveredSignal)));
        e.continueNativeReturned=true;
        if (e.continueResult < 0) { e.continueError = errno; Fail("selected_ptrace_restart", e.continueError); return false; }
        e.continued = true;
        return true;
    }
    // Stream owner must independently establish actual EOF/disposal eligibility.
    // Native process terminal permits its independent pidfd/writer closes even
    // if a stream operation failed. Failed qualification can never become PASS.
    bool DisposeEligible() noexcept {
        auto& s = *state_;
        if (!Thread() || !s.terminal) return false;
        // Attempt every independently eligible reader and descriptor; one
        // uncertain close never suppresses the others or gets retried.
        s.stdoutStream.DisposeEligible(s.terminal);
        s.stderrStream.DisposeEligible(s.terminal);
        s.setupStream.DisposeEligible(s.terminal);
        for (size_t i=0; i<s.fdCount; ++i) {
            if (i == stdoutRead_ || i == stderrRead_ || i == setupRead_) continue;
            if (i == s.rootPidfd && s.rootPidfdOutputUnknown) continue;
            s.fds[i].CloseOnce();
        }
        bool all = UnmapEligibleChildOutputs() && !s.rootPidfdOutputUnknown && !s.rootCreationUnknown;
        for (size_t i=0; i<s.fdCount; ++i) all &= s.fds[i].original < 0 || s.fds[i].closed;
        s.disposed = all;
        return s.disposed;
    }
    bool PollStreams() noexcept {
        if (!Thread() || state_->originalForkResult <= 0) return false;
        auto& s = *state_;
        CaptureChildOutputs();
        bool progress = s.stdoutStream.Poll();
        progress = s.stderrStream.Poll() || progress;
        progress = s.setupStream.Poll() || progress;
        if (s.stdoutStream.failed || s.stderrStream.failed || s.setupStream.failed)
            Fail("stream_capture_failed_predicate", 0, false);
        return progress;
    }
    bool PollFailureDrain(Gate& gate) {
        auto& s = *state_;
        if (!Thread() || !s.failed || s.originalGateException) return false;
        try {
            if (!gate.BeforeFailureDrain(s)) return false;
        } catch (...) {
            s.failureDrainGateException = std::current_exception();
            if (!s.originalGateException) s.originalGateException = s.failureDrainGateException;
            Fail("failure_drain_gate_exception", 0); return false;
        }
        bool progress = s.stdoutStream.DrainPoll();
        progress = s.stderrStream.DrainPoll() || progress;
        progress = s.setupStream.DrainPoll() || progress;
        return progress;
    }
    bool ValidateSetupEof() noexcept {
        auto& s = *state_;
        if (!Thread() || s.setupValidated || !s.setupStream.eof) return s.setupValidated;
        // Raw setup bytes and read outcomes remain in their original owner.
        // Empty EOF alone can also mean death before exec, so require the
        // actual root exec event rather than interpreting pipe EOF as launch.
        if (s.setupStream.used == sizeof(ChildSetupFailure)) {
            memcpy(&s.originalSetupFailure, s.setupStream.bytes.get(), sizeof(ChildSetupFailure));
            Fail("original_child_setup_failure", s.originalSetupFailure.originalErrno, false);
            return false;
        }
        if (s.setupStream.used != 0) {
            Fail("original_setup_channel_shape", 0, false); return false;
        }
        if (!s.traceeCount || s.currentRootTracee >= s.traceeCount ||
                !s.tracees[s.currentRootTracee].execEvent) {
            if (s.terminal) Fail("setup_eof_without_original_root_exec", 0, false);
            return false;
        }
        s.setupValidated = true;
        return true;
    }
    bool DisposeUnlaunched() noexcept {
        auto& s = *state_;
        if (!Thread() || s.originalForkResult > 0 || s.originalCloneResult > 0 || s.rootCreationUnknown) return false;
        if (s.unlaunchedDisposalAttempted) return s.unlaunchedDisposalResult;
        s.unlaunchedDisposalAttempted = true;
        bool all = UnmapEligibleChildOutputs() && !s.rootPidfdOutputUnknown;
        for (size_t i=0; i<s.fdCount; ++i) {
            if (i == s.rootPidfd && s.rootPidfdOutputUnknown) continue;
            s.fds[i].CloseOnce();
            all &= s.fds[i].original < 0 || s.fds[i].closed;
        }
        if (!all) s.failed = true;
        s.unlaunchedDisposalResult = all;
        // Actual pre-create/clone3-failure disposal only. No synthetic child
        // terminal state, no post-create cleanup eligibility, no recovered PASS.
        return all;
    }
};
} // namespace template_creator_linux

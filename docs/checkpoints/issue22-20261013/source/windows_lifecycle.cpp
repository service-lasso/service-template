// UNCOMPILED source-only lifecycle owner. No input admission or runtime proof.
// One instance stays on its creating native thread through bootstrap and commands.
// No attach, detach, terminate, kill-on-close Job or destructor-close operation.
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <array>
#include <cstdint>
#include <exception>
#include <memory>
#include <string>
#include <algorithm>
#include <stdexcept>
#include "windows_streams.hpp"

namespace template_creator {
constexpr SIZE_T MemberLimit = 4096, ResourceLimit = 65536, EventLimit = 65536;
constexpr SIZE_T FailureLimit = ResourceLimit + 32;
constexpr SIZE_T NativeResultLimit = 65536;
enum class Kind { CreationProcess, CreationThread, Process, Thread, Image, EventFile, Job };
struct Resource {
    HANDLE original = nullptr;
    Kind kind = Kind::Image;
    bool attempted = false, closed = false, acquired = false, unknownOutput = false;
    HANDLE duplicateSource = nullptr;
    HANDLE duplicateSourceProcessOperand = nullptr, duplicateTargetProcessOperand = nullptr;
    DWORD duplicateDesiredAccess = 0, duplicateOptions = 0;
    BOOL duplicateInherit = FALSE;
    bool duplicateAttempted = false;
    BOOL duplicateResult = FALSE;
    DWORD duplicateError = 0;
    DWORD closeError = ERROR_SUCCESS;
    BOOL closeResult = FALSE;
};
struct Failure {
    const char* phase = nullptr;
    const char* operation = nullptr;
    DWORD code = ERROR_SUCCESS;
    std::exception_ptr original; // Private original C++ callback exception.
};
struct Member {
    DWORD pid = 0;
    SIZE_T process = ResourceLimit;
    SIZE_T originalCreateEvent = EventLimit;
    FILETIME birth{}, exitTime{};
    DWORD code = STILL_ACTIVE;
    bool exitEvent = false, continuedExit = false, shutdown = false;
};
enum class SavedEventDecision { Deny, PreserveOriginalPolicy, ConsumeOwnedObserverException,
    ConsumeOwnedObserverCleanupException };
struct Event {
    SavedEventDecision requestedDecision = SavedEventDecision::Deny;
    bool decisionAttempted = false, decisionReturned = false, originalPolicyAttempted = false;
    DWORD originalPolicyDisposition = 0;
    std::exception_ptr decisionOriginal;
    SIZE_T createdThreadResource = ResourceLimit, createdThreadMemberOrdinal = MemberLimit;
    SIZE_T createdThreadProcessResource = ResourceLimit, createdThreadNativeResult = NativeResultLimit;
    SIZE_T exitingThreadCreateEvent = EventLimit;
    SIZE_T processLastThreadExitEvent = EventLimit;
    bool threadExitObserved = false, threadExitContinued = false;
    SIZE_T originalImageFileResource = ResourceLimit, retainedImageResource = ResourceLimit;
    SIZE_T createdMemberOrdinal = MemberLimit, createdProcessResource = ResourceLimit;
    SIZE_T exitingMemberOrdinal = MemberLimit, exitingProcessResource = ResourceLimit;
    DEBUG_EVENT original{}; // Exact native union, including original capabilities.
    DWORD disposition = 0;
    bool continued = false;
    BOOL continuationResult = FALSE;
    DWORD continuationError = 0;
};
struct NativeResult {
    bool threadTimesAttempted = false, threadIdAttempted = false, threadProcessIdAttempted = false;
    DWORD threadIdResult = 0, threadIdError = 0, threadProcessIdResult = 0, threadProcessIdError = 0;
    const char* operation = nullptr;
    HANDLE original = nullptr;
    DWORD waitResult = 0, waitError = 0;
    BOOL timesResult = FALSE, codeResult = FALSE, jobResult = FALSE;
    BOOL debugWaitResult = FALSE;
    DWORD debugWaitError = 0;
    DWORD currentThreadResult = 0;
    DEBUG_EVENT debugWaitOutput{};
    DWORD timesError = 0, codeError = 0, jobError = 0;
    FILETIME born{}, exited{}, kernel{}, user{};
    DWORD code = STILL_ACTIVE;
    JOBOBJECT_BASIC_ACCOUNTING_INFORMATION job{};
};
struct State {
    bool unlaunchedDisposalAttempted = false, unlaunchedDisposalResult = false;
    std::array<wchar_t, 32767> originalCommand{}, nativeMutableCommand{};
    std::array<wchar_t, 32768> originalImage{}, originalCwd{};
    std::array<wchar_t, 65536> originalEnvironment{};
    SIZE_T commandChars = 0, imageChars = 0, cwdChars = 0, environmentChars = 0;
    LaunchStreams streams;
    std::array<Resource, ResourceLimit> resources{};
    std::array<Member, MemberLimit> members{};
    std::array<Event, EventLimit> events{};
    std::array<Failure, FailureLimit> failures{};
    std::array<NativeResult, NativeResultLimit> nativeResults{};
    SIZE_T resourceCount = 0, memberCount = 0, eventCount = 0, failureCount = 0;
    SIZE_T nativeResultCount = 0;
    DWORD root = 0;
    SIZE_T creationThread = ResourceLimit;
    SIZE_T job = ResourceLimit;
    JOBOBJECT_EXTENDED_LIMIT_INFORMATION jobLimits{};
    JOBOBJECT_BASIC_ACCOUNTING_INFORMATION terminalJobAccounting{};
    bool jobAssigned = false, jobEmpty = false;
    bool jobCreateAttempted = false, createAttempted = false, creationOutputsUnknown = false;
    DWORD jobCreateError = 0, jobLimitsError = 0, createError = 0, jobAssignError = 0;
    BOOL jobLimitsResult = FALSE, createResult = FALSE, jobAssignResult = FALSE;
    PROCESS_INFORMATION originalCreationInformation{};
    LPSECURITY_ATTRIBUTES originalProcessSecurityOperand=nullptr,originalThreadSecurityOperand=nullptr;
    BOOL originalCreationInheritance=FALSE;
    DWORD originalCreationFlags=0;
    bool configurationAttempted = false, rootResumeAttempted = false;
    BOOL configurationResult = FALSE;
    DWORD configurationError = 0, rootResumeResult = 0, rootResumeError = 0;
    bool bootstrap = false, created = false, policyDisabled = false;
    bool terminal = false, disposed = false, rootResumed = false;
    bool stopped = false, continuationDenied = false, exhausted = false;
    DEBUG_EVENT overflowEvent{}; // First event that cannot be enrolled, retained.
    bool overflowEventPresent = false;
    Failure overflowFailure{}; // First overflow original, never overwritten.
    bool overflowFailurePresent = false;
};
static bool same_time(FILETIME a, FILETIME b) {
    return a.dwLowDateTime == b.dwLowDateTime && a.dwHighDateTime == b.dwHighDateTime;
}
static bool positive(FILETIME a) { return a.dwLowDateTime || a.dwHighDateTime; }

// Pure virtual deliberately: a Boolean implementation alone is not admission.
// Complete source-owned input/rights/loaded-memory gate and stream driver must
// compose this class before any reviewed/admitted command can execute.
class InputGate {
public:
    virtual ~InputGate() = default;
    virtual bool BeforeCreate(const State&, const wchar_t*, const wchar_t*, const wchar_t*,
                              const wchar_t*, SIZE_T) = 0;
    virtual bool BeforeContinue(const DEBUG_EVENT&, const State&) = 0;
    // Explicit source-owned retained-failure continuation policy. No default
    // waiver: a failed capture can never authorize a new command or success.
    virtual bool BeforeFailureContinue(const DEBUG_EVENT&, const State&) = 0;
    // Existing policies preserve their exact forwarding behavior. The concrete
    // owned observer overrides this seam only after its complete saved-event proof.
    virtual SavedEventDecision DecideSavedEvent(const Event& event, const State& state, bool failed) {
        return (failed ? BeforeFailureContinue(event.original, state) : BeforeContinue(event.original, state))
            ? SavedEventDecision::PreserveOriginalPolicy : SavedEventDecision::Deny;
    }
    // Closed by default until the concrete controller supplies the SAME retained
    // capture/effective-context row; requesting an enum alone grants nothing.
    virtual bool ValidateOwnedObserverConsumption(const Event&, const State&) { return false; }
    // Separate closed path: cleanup cannot qualify a useful observation. The
    // concrete owner must prove this exact owned cause plus fresh completed
    // restoration and independently settled native borrowers/outputs.
    virtual bool ValidateOwnedObserverCleanupConsumption(const Event&, const State&) { return false; }
    // Closed independently: terminal process state does not settle an observer's native borrowers.
    virtual bool BeforeNativeDisposal(const State&) { return false; }
    virtual bool BeforeFailureDrain(const State&) = 0;
    virtual bool IsInitialDebuggerBreakpoint(const DEBUG_EVENT&, const State&) = 0;
};

class OwnedDebugThread {
    const DWORD ownerThread_ = GetCurrentThreadId();
    bool configured_ = false, bootstrapDisposed_ = false;
    bool streamFailureReported_ = false;
    // All backing stores exist before CreateProcess; no event vector/map grows.
    // Actual entire owner/runtime memory fit remains a required measured result.
    std::unique_ptr<State[]> selectedStates_;
    State* state_ = nullptr;
    SIZE_T selectedCommandCount_ = 0, currentCommand_ = 0;

    void fail(const char* phase, const char* op, DWORD code,
              std::exception_ptr original = {}, bool haltContinuation = true) noexcept {
        auto& s = *state_;
        s.stopped = true;
        s.continuationDenied = s.continuationDenied || haltContinuation;
        if (s.failureCount < FailureLimit) {
            s.failures[s.failureCount++] = {phase, op, code, original};
        } else {
            s.exhausted = true;
            if (!s.overflowFailurePresent) {
                s.overflowFailure = {phase, op, code, original};
                s.overflowFailurePresent = true;
            }
        }
    }
    bool thread() noexcept {
        auto& s = *state_;
        if (s.nativeResultCount == NativeResultLimit) {
            fail("capture", "thread_result_capacity_before_call", ERROR_NOT_ENOUGH_MEMORY); return false;
        }
        auto& row = s.nativeResults[s.nativeResultCount++];
        row.operation = "GetCurrentThreadId";
        row.currentThreadResult = GetCurrentThreadId();
        if (ownerThread_ == row.currentThreadResult) return true;
        fail("primary", "same_event_thread", ERROR_INVALID_THREAD_ID);
        return false;
    }
    template<SIZE_T Capacity>
    static bool CopyLiteral(const wchar_t* source, std::array<wchar_t, Capacity>& destination,
                            SIZE_T& count) noexcept {
        if (!source) return false;
        SIZE_T index = 0;
        for (; index < Capacity; ++index) {
            destination[index] = source[index];
            if (!source[index]) { count = index; return index != 0; }
        }
        return false;
    }
    SIZE_T hold(HANDLE h, Kind kind) noexcept {
        auto& s = *state_;
        if (s.resourceCount == ResourceLimit) {
            fail("capture", "resource_capacity", ERROR_NOT_ENOUGH_MEMORY);
            s.exhausted = true;
            return ResourceLimit;
        }
        SIZE_T index = s.resourceCount++;
        s.resources[index].original = h;
        s.resources[index].acquired = h && h != INVALID_HANDLE_VALUE;
        s.resources[index].kind = kind;
        return index;
    }
    SIZE_T duplicate(HANDLE h, Kind kind) noexcept {
        // Reserve a durable destination cell BEFORE the native duplicate call.
        auto index = hold(nullptr, kind);
        if (index == ResourceLimit) return index;
        auto& target = state_->resources[index];
        if (!h || h == INVALID_HANDLE_VALUE) {
            fail("capture", "duplicate_invalid_original", ERROR_INVALID_HANDLE);
            return index;
        }
        target.duplicateSourceProcessOperand=GetCurrentProcess();
        target.duplicateTargetProcessOperand=GetCurrentProcess();
        target.duplicateSource=h;target.duplicateDesiredAccess=0;
        target.duplicateInherit=FALSE;target.duplicateOptions=DUPLICATE_SAME_ACCESS;
        target.duplicateAttempted = true;
        target.duplicateResult = DuplicateHandle(target.duplicateSourceProcessOperand,target.duplicateSource,
                             target.duplicateTargetProcessOperand,&target.original,target.duplicateDesiredAccess,
                             target.duplicateInherit,target.duplicateOptions);
        if (!target.duplicateResult) {
            target.duplicateError = GetLastError();
            target.unknownOutput = target.original != nullptr;
            fail("capture", "DuplicateHandle", target.duplicateError);
        } else {
            target.acquired = target.original && target.original != INVALID_HANDLE_VALUE;
            if (!target.acquired) {
                target.unknownOutput = true;
                fail("capture", "successful_duplicate_missing_valid_handle", ERROR_INVALID_DATA);
            }
        }
        return index;
    }
    Member* member(DWORD pid) noexcept {
        auto& s = *state_;
        for (SIZE_T i = 0; i < s.memberCount; ++i)
            if (s.members[i].pid == pid) return &s.members[i];
        return nullptr;
    }
    NativeResult* native_result(const char* operation, HANDLE original) noexcept {
        auto& s = *state_;
        if (s.nativeResultCount == NativeResultLimit) {
            s.exhausted = true;
            fail("capture", "native_result_capacity_before_call", ERROR_NOT_ENOUGH_MEMORY);
            return nullptr;
        }
        auto& row = s.nativeResults[s.nativeResultCount++];
        row.operation = operation; row.original = original;
        return &row;
    }
    void close(Resource& cell) noexcept {
        if (!cell.acquired || cell.attempted) return;
        cell.attempted = true; // Set BEFORE close; never retry uncertain values.
        cell.closeResult = CloseHandle(cell.original);
        if (cell.closeResult) cell.closed = true;
        else {
            cell.closeError = GetLastError(); // Independently retained per cell.
            fail("disposal", "CloseHandle", cell.closeError);
        }
    }
    void event_file(Event& event, HANDLE original) noexcept {
        if (!original) {
            fail("capture", "missing_native_image_file", ERROR_INVALID_HANDLE);
            return;
        }
        // Own original first, then duplicate; failure preserves both lifetimes.
        auto index = event.originalImageFileResource = hold(original, Kind::EventFile);
        event.retainedImageResource = duplicate(original, Kind::Image);
        if (index != ResourceLimit) close(state_->resources[index]);
    }
    void enroll(Event& saved) noexcept {
        auto& s = *state_;
        const auto& e=saved.original;
        if (member(e.dwProcessId)) {
            fail("capture", "duplicate_process_identity", ERROR_ALREADY_EXISTS);
            return;
        }
        if (s.memberCount == MemberLimit) {
            fail("capture", "member_capacity", ERROR_NOT_ENOUGH_MEMORY);
            s.exhausted = true;
            return;
        }
        saved.createdMemberOrdinal=s.memberCount;
        Member& m = s.members[s.memberCount++];
        m.originalCreateEvent = static_cast<SIZE_T>(&saved-s.events.data());
        m.pid = e.dwProcessId;
        m.process = saved.createdProcessResource = duplicate(e.u.CreateProcessInfo.hProcess, Kind::Process);
        if (m.process == ResourceLimit || !s.resources[m.process].acquired) return;
        auto observed = native_result("enroll_process_times", s.resources[m.process].original);
        if (!observed) return;
        observed->timesResult = GetProcessTimes(observed->original, &observed->born,
            &observed->exited, &observed->kernel, &observed->user);
        m.birth = observed->born; m.exitTime = observed->exited;
        if (!observed->timesResult) {
            observed->timesError = GetLastError();
            fail("capture", "birth", observed->timesError);
        } else if (!positive(m.birth))
            fail("capture", "positive_birth", ERROR_INVALID_DATA);
    }
    void enroll_thread(Event& saved, HANDLE original) noexcept {
        auto& s = *state_;
        saved.createdThreadResource = duplicate(original, Kind::Thread);
        auto m = member(saved.original.dwProcessId);
        if (!m) { fail("capture", "thread_process_occurrence", ERROR_NOT_FOUND); return; }
        saved.createdThreadMemberOrdinal = static_cast<SIZE_T>(m-s.members.data());
        saved.createdThreadProcessResource = m->process;
        if (saved.createdThreadResource == ResourceLimit ||
                !s.resources[saved.createdThreadResource].acquired) return;
        auto row = native_result("enroll_thread_occurrence", s.resources[saved.createdThreadResource].original);
        if (!row) return;
        saved.createdThreadNativeResult = static_cast<SIZE_T>(row-s.nativeResults.data());
        row->threadTimesAttempted = true;
        row->timesResult = GetThreadTimes(row->original, &row->born, &row->exited, &row->kernel, &row->user);
        if (!row->timesResult) row->timesError = GetLastError();
        row->threadIdAttempted = true;
        row->threadIdResult = GetThreadId(row->original);
        if (!row->threadIdResult) row->threadIdError = GetLastError();
        row->threadProcessIdAttempted = true;
        row->threadProcessIdResult = GetProcessIdOfThread(row->original);
        if (!row->threadProcessIdResult) row->threadProcessIdError = GetLastError();
        // exited is retained raw only: GetThreadTimes leaves it undefined for a live thread.
        if (!row->timesResult || !positive(row->born) ||
                row->threadIdResult != saved.original.dwThreadId ||
                row->threadProcessIdResult != saved.original.dwProcessId)
            fail("capture", "thread_occurrence_observation", ERROR_INVALID_DATA);
    }
    void observe_thread_exit(Event& saved) noexcept {
        auto& s = *state_;
        auto m = member(saved.original.dwProcessId);
        if (!m) { fail("capture", "thread_exit_process", ERROR_NOT_FOUND); return; }
        SIZE_T matched = EventLimit;
        for (SIZE_T i=0; i+1<s.eventCount; ++i) {
            auto& born = s.events[i];
            if (born.createdThreadResource == ResourceLimit || born.threadExitObserved ||
                    born.createdThreadMemberOrdinal != static_cast<SIZE_T>(m-s.members.data()) ||
                    born.createdThreadProcessResource != m->process ||
                    born.original.dwThreadId != saved.original.dwThreadId) continue;
            if (matched != EventLimit) { fail("capture", "ambiguous_thread_exit", ERROR_INVALID_DATA); return; }
            matched = i;
        }
        if (matched == EventLimit) { fail("capture", "unenrolled_thread_exit", ERROR_NOT_FOUND); return; }
        saved.exitingThreadCreateEvent = matched;
        s.events[matched].threadExitObserved = true;
    }
    void terminal(Member& m) noexcept {
        auto& s = *state_;
        if (m.shutdown || !m.continuedExit || m.process == ResourceLimit) return;
        HANDLE h = s.resources[m.process].original;
        if (!s.resources[m.process].acquired) return;
        auto observed = native_result("terminal_process", h);
        if (!observed) return;
        DWORD waited = observed->waitResult = WaitForSingleObject(h, 0);
        if (waited == WAIT_TIMEOUT) return;
        if (waited != WAIT_OBJECT_0) {
            if (waited == WAIT_FAILED) observed->waitError = GetLastError();
            fail("capture", "terminal_signal", waited == WAIT_FAILED ? observed->waitError : ERROR_INVALID_DATA, {}, false);
            return;
        }
        observed->timesResult = GetProcessTimes(h, &observed->born, &observed->exited,
                                               &observed->kernel, &observed->user);
        if (!observed->timesResult) {
            observed->timesError = GetLastError();
            fail("capture", "terminal_times", observed->timesError, {}, false); return;
        }
        observed->codeResult = GetExitCodeProcess(h, &observed->code);
        if (!observed->codeResult) {
            observed->codeError = GetLastError();
            fail("capture", "terminal_code", observed->codeError, {}, false); return;
        }
        // Literal exit259 is valid when same retained handle is signaled and
        // same birth/positive exit/native exit-event code all agree.
        if (!same_time(observed->born, m.birth) || !positive(observed->exited) || !m.exitEvent || observed->code != m.code) {
            fail("capture", "terminal_binding", ERROR_INVALID_DATA); return;
        }
        m.exitTime = observed->exited;
        m.shutdown = true;
    }
    void refresh_terminal() noexcept {
        auto& s = *state_;
        bool all = s.memberCount != 0;
        for (SIZE_T i = 0; i < s.memberCount; ++i) {
            terminal(s.members[i]);
            all = all && s.members[i].shutdown;
        }
        if (all && !s.continuationDenied && s.jobAssigned) {
            auto h = s.resources[s.job].original;
            auto observed = native_result("terminal_job", h);
            if (!observed) return;
            observed->jobResult = QueryInformationJobObject(h, JobObjectBasicAccountingInformation,
                    &observed->job, sizeof(observed->job), nullptr);
            if (!observed->jobResult) {
                observed->jobError = GetLastError();
                fail("capture", "terminal_job_membership", observed->jobError, {}, false);
            } else {
                s.terminalJobAccounting = observed->job;
                s.jobEmpty = observed->job.ActiveProcesses == 0;
            }
        }
        s.terminal = all && s.jobEmpty && !s.continuationDenied;
    }
public:
    explicit OwnedDebugThread(SIZE_T selectedCommandCount) {
        if (!selectedCommandCount || selectedCommandCount > 65)
            throw std::invalid_argument("complete selected command capacity");
        // Every selected command state and stream store is allocated before
        // bootstrap/native acquisition; future transitions allocate nothing.
        selectedStates_ = std::make_unique<State[]>(selectedCommandCount);
        selectedCommandCount_ = selectedCommandCount; state_ = &selectedStates_[0];
    }
    const State& Current() const noexcept { return *state_; }
    const State* PrivateSelectedStates() const noexcept { return selectedStates_.get(); }
    SIZE_T SelectedCommandCapacity() const noexcept { return selectedCommandCount_; }
    SIZE_T CurrentCommandOrdinal() const noexcept { return currentCommand_; }
    bool ReservedSourceBytes(SIZE_T& bytes) const noexcept {
        // Actual currently allocated source cells only; excludes allocator,
        // OS/kernel, tool/interpreter and loaded runtime memory.
        bytes = sizeof(*this);
        constexpr SIZE_T each = sizeof(State) + 4 * StreamLimit;
        if (selectedCommandCount_ > (SIZE_MAX-bytes)/each) return false;
        bytes += selectedCommandCount_*each;
        return true;
    }
    // No destructor closes/detaches/terminates. A live/stopped owner requires
    // independent retained-thread policy; C++ object destruction proves nothing.
    bool Begin(const wchar_t* image, std::wstring& command,
               const wchar_t* cwd, const wchar_t* environment,
               SIZE_T environmentChars, InputGate& gate, bool bootstrap) noexcept {
        auto& s = *state_;
        if (!thread()) return false;
        if (s.created || s.stopped || s.disposed ||
            (!bootstrap && (!configured_ || !bootstrapDisposed_)) ||
            (bootstrap && (configured_ || bootstrapDisposed_))) {
            fail("primary", "bootstrap_order", ERROR_INVALID_STATE); return false;
        }
        s.bootstrap = bootstrap;
        if (!environment || environmentChars < 2 || environmentChars > 65536 ||
            environment[environmentChars-1] || environment[environmentChars-2]) {
            fail("primary", "explicit_environment", ERROR_INVALID_DATA); return false;
        }
        for (SIZE_T i=0; i+1<environmentChars; ++i) {
            if (!environment[i] && !environment[i+1] && i != environmentChars-2) {
                fail("primary", "early_environment_terminator", ERROR_INVALID_DATA); return false;
            }
        }
        if (command.size() >= s.originalCommand.size() ||
                command.find(L'\0') != std::wstring::npos ||
                !CopyLiteral(image, s.originalImage, s.imageChars) ||
                !CopyLiteral(cwd, s.originalCwd, s.cwdChars)) {
            fail("primary", "literal_command_path_bound", ERROR_INVALID_DATA); return false;
        }
        s.commandChars = command.size(); s.environmentChars = environmentChars;
        std::copy(command.begin(), command.end(), s.originalCommand.begin());
        std::copy(command.begin(), command.end(), s.nativeMutableCommand.begin());
        std::copy_n(environment, environmentChars, s.originalEnvironment.begin());
        try {
            if (!gate.BeforeCreate(s, s.originalImage.data(), s.originalCommand.data(),
                    s.originalCwd.data(), s.originalEnvironment.data(), s.environmentChars)) {
                fail("primary", "complete_before_create", ERROR_ACCESS_DENIED); return false;
            }
        } catch (...) {
            fail("primary", "BeforeCreate_exception", ERROR_UNHANDLED_EXCEPTION, std::current_exception());
            return false;
        }
        if (!s.streams.Prepare()) {
            fail("primary", "prepare_explicit_streams", s.streams.primaryError); return false;
        }
        s.job = hold(nullptr, Kind::Job);
        if (s.job == ResourceLimit) return false;
        s.jobCreateAttempted = true;
        s.resources[s.job].original = CreateJobObjectW(nullptr, nullptr);
        s.resources[s.job].acquired = s.resources[s.job].original != nullptr;
        if (!s.resources[s.job].original) {
            s.jobCreateError = GetLastError();
            fail("primary", "CreateJobObjectW", s.jobCreateError); return false;
        }
        // Explicit zero limit flags: no kill-on-close or breakaway authority.
        s.jobLimitsResult = SetInformationJobObject(s.resources[s.job].original,
                JobObjectExtendedLimitInformation, &s.jobLimits, sizeof(s.jobLimits));
        if (!s.jobLimitsResult) {
            s.jobLimitsError = GetLastError();
            fail("primary", "SetInformationJobObject", s.jobLimitsError); return false;
        }
        // Two creation ownership cells reserved before acquisition.
        auto process = hold(nullptr, Kind::CreationProcess);
        auto threadCell = hold(nullptr, Kind::CreationThread);
        if (process == ResourceLimit || threadCell == ResourceLimit) return false;
        auto& pi = s.originalCreationInformation;
        s.originalProcessSecurityOperand=nullptr;s.originalThreadSecurityOperand=nullptr;
        s.originalCreationInheritance=TRUE;
        s.originalCreationFlags=DEBUG_PROCESS | CREATE_SUSPENDED | CREATE_UNICODE_ENVIRONMENT | EXTENDED_STARTUPINFO_PRESENT;
        s.createAttempted = true;
        s.createResult = CreateProcessW(s.originalImage.data(), s.nativeMutableCommand.data(),
                s.originalProcessSecurityOperand,s.originalThreadSecurityOperand,s.originalCreationInheritance,s.originalCreationFlags,
                s.originalEnvironment.data(), s.originalCwd.data(), &s.streams.startup.StartupInfo, &pi);
        if (!s.createResult) {
            s.createError = GetLastError();
            s.creationOutputsUnknown = pi.hProcess || pi.hThread;
            fail("primary", "CreateProcessW", s.createError); return false;
        }
        // No allocation or callback lies between acquisition and durable custody.
        s.resources[process].original = pi.hProcess;
        s.resources[threadCell].original = pi.hThread;
        s.resources[process].acquired = pi.hProcess && pi.hProcess != INVALID_HANDLE_VALUE;
        s.resources[threadCell].acquired = pi.hThread && pi.hThread != INVALID_HANDLE_VALUE;
        s.creationThread = threadCell;
        s.root = pi.dwProcessId;
        s.created = true;
        if (!s.resources[process].acquired || !s.resources[threadCell].acquired) {
            s.creationOutputsUnknown = true;
            fail("primary", "successful_create_missing_valid_handles", ERROR_INVALID_DATA); return false;
        }
        if (bootstrap) {
            // Exposed default-kill interval is explicit: successful creation to
            // SAME-thread successful FALSE. Failure keeps owned bootstrap stopped.
            s.configurationAttempted = true;
            s.configurationResult = DebugSetProcessKillOnExit(FALSE);
            if (!s.configurationResult) {
                s.configurationError = GetLastError();
                fail("primary", "DebugSetProcessKillOnExit_FALSE", s.configurationError); return false;
            }
            configured_ = true;
        }
        s.policyDisabled = configured_;
        s.jobAssignResult = AssignProcessToJobObject(s.resources[s.job].original, pi.hProcess);
        if (!s.jobAssignResult) {
            s.jobAssignError = GetLastError();
            fail("primary", "AssignProcessToJobObject", s.jobAssignError); return false;
        }
        s.jobAssigned = true;
        if (!s.streams.AfterCreate()) {
            fail("disposal", "parent_stream_writer_close_failed_predicate", ERROR_SUCCESS, {}, false); return false;
        }
        return true;
    }
    // One nonblocking native poll only. Original per-command deadlines belong
    // to the source-owned driver; timeout is never termination or EOF evidence.
    bool Poll(InputGate& gate) noexcept {
        auto& s = *state_;
        if (!thread() || !s.created || s.continuationDenied || s.terminal || s.disposed) return false;
        auto observedWait = native_result("WaitForDebugEventEx_zero", nullptr);
        if (!observedWait) return false;
        observedWait->debugWaitResult = WaitForDebugEventEx(&observedWait->debugWaitOutput, 0);
        if (!observedWait->debugWaitResult) {
            DWORD code = observedWait->debugWaitError = GetLastError();
            if (code != ERROR_SEM_TIMEOUT) fail("capture", "WaitForDebugEventEx", code);
            else refresh_terminal();
            return false;
        }
        const DEBUG_EVENT& e = observedWait->debugWaitOutput;
        // Charge worst event ownership before interpreting/duplicating handles.
        SIZE_T needed = e.dwDebugEventCode == CREATE_PROCESS_DEBUG_EVENT ? 4 :
                        e.dwDebugEventCode == LOAD_DLL_DEBUG_EVENT ? 2 :
                        e.dwDebugEventCode == CREATE_THREAD_DEBUG_EVENT ? 1 : 0;
        bool needsMember = e.dwDebugEventCode == CREATE_PROCESS_DEBUG_EVENT;
        if (s.eventCount == EventLimit || needed > ResourceLimit - s.resourceCount ||
                (needsMember && s.memberCount == MemberLimit)) {
            if (!s.overflowEventPresent) { s.overflowEvent = e; s.overflowEventPresent = true; }
            s.exhausted = true;
            fail("capture", "event_capacity", ERROR_NOT_ENOUGH_MEMORY);
            return false; // Original event remains stopped, never discarded.
        }
        Event& saved = s.events[s.eventCount++]; saved.original = e;
        if (e.dwDebugEventCode == CREATE_PROCESS_DEBUG_EVENT) {
            enroll(saved); enroll_thread(saved, e.u.CreateProcessInfo.hThread);
            event_file(saved, e.u.CreateProcessInfo.hFile);
        } else if (e.dwDebugEventCode == CREATE_THREAD_DEBUG_EVENT) {
            enroll_thread(saved, e.u.CreateThread.hThread);
        } else if (e.dwDebugEventCode == EXIT_THREAD_DEBUG_EVENT) {
            observe_thread_exit(saved);
        } else if (e.dwDebugEventCode == LOAD_DLL_DEBUG_EVENT) {
            event_file(saved, e.u.LoadDll.hFile);
        } else if (e.dwDebugEventCode == EXIT_PROCESS_DEBUG_EVENT) {
            auto m = member(e.dwProcessId);
            if (!m) fail("capture", "unenrolled_exit", ERROR_NOT_FOUND);
            else {
                // Attach this exact native occurrence to the already-enrolled
                // original before any gate or continuation. This is last-user
                // exit evidence only: shutdown/EOF/disposal remain independent.
                saved.exitingMemberOrdinal = static_cast<SIZE_T>(m-s.members.data());
                saved.exitingProcessResource = m->process;
                m->exitEvent = true; m->code = e.u.ExitProcess.dwExitCode;
                // Windows does not emit EXIT_THREAD for the last thread. Retain
                // the actual process last-thread event separately; never invent
                // an EXIT_THREAD status or infer kernel/handle settlement here.
                for (SIZE_T i=0; i+1<s.eventCount; ++i) {
                    auto& born=s.events[i];
                    if (born.createdThreadResource != ResourceLimit && !born.threadExitObserved &&
                            born.createdThreadMemberOrdinal == saved.exitingMemberOrdinal &&
                            born.createdThreadProcessResource == m->process)
                        born.processLastThreadExitEvent=static_cast<SIZE_T>(&saved-s.events.data());
                }
            }
        }
        if (s.continuationDenied) return false;
        DWORD disposition = DBG_CONTINUE;
        try {
            saved.decisionAttempted = true;
            saved.requestedDecision = gate.DecideSavedEvent(saved, s, s.stopped);
            saved.decisionReturned = true;
            if (saved.requestedDecision == SavedEventDecision::Deny) {
                fail("capture", "precontinuation_input_gate", ERROR_ACCESS_DENIED); return false;
            }
            if (saved.requestedDecision == SavedEventDecision::PreserveOriginalPolicy) {
                saved.originalPolicyAttempted = true;
                if (e.dwDebugEventCode == EXCEPTION_DEBUG_EVENT && !gate.IsInitialDebuggerBreakpoint(saved.original, s))
                    disposition = DBG_EXCEPTION_NOT_HANDLED;
                saved.originalPolicyDisposition = disposition;
            } else if (saved.requestedDecision == SavedEventDecision::ConsumeOwnedObserverException) {
                if(e.dwDebugEventCode != EXCEPTION_DEBUG_EVENT || s.stopped ||
                    !gate.ValidateOwnedObserverConsumption(saved, s)) {
                    fail("capture", "invalid_saved_event_decision", ERROR_INVALID_DATA); return false;
                }
            } else if(saved.requestedDecision == SavedEventDecision::ConsumeOwnedObserverCleanupException) {
                // Existing primary failure is retained. This sole real native
                // continuation consumes only the independently restored owned
                // exception; it admits no new slot, command or useful success.
                if(e.dwDebugEventCode != EXCEPTION_DEBUG_EVENT ||
                    !gate.ValidateOwnedObserverCleanupConsumption(saved,s)) {
                    fail("capture", "invalid_cleanup_event_decision", ERROR_INVALID_DATA); return false;
                }
            } else {
                fail("capture", "invalid_saved_event_decision", ERROR_INVALID_DATA); return false;
            }
        } catch (...) {
            saved.decisionOriginal = std::current_exception();
            fail("capture", "continuation_gate_exception", ERROR_UNHANDLED_EXCEPTION, saved.decisionOriginal);
            return false;
        }
        saved.disposition = disposition;
        saved.continuationResult = ContinueDebugEvent(e.dwProcessId, e.dwThreadId, disposition);
        if (!saved.continuationResult) {
            saved.continuationError = GetLastError();
            fail("capture", "ContinueDebugEvent", saved.continuationError); return false;
        }
        saved.continued = true;
        if (saved.exitingThreadCreateEvent != EventLimit)
            s.events[saved.exitingThreadCreateEvent].threadExitContinued = true;
        if (e.dwDebugEventCode == EXIT_PROCESS_DEBUG_EVENT) {
            auto m = member(e.dwProcessId);
            if (m) m->continuedExit = true;
        }
        if (e.dwDebugEventCode == CREATE_PROCESS_DEBUG_EVENT && e.dwProcessId == s.root && !s.rootResumed) {
            s.rootResumeAttempted = true;
            s.rootResumeResult = ResumeThread(s.resources[s.creationThread].original);
            if (s.rootResumeResult == static_cast<DWORD>(-1)) {
                s.rootResumeError = GetLastError();
                fail("primary", "ResumeThread", s.rootResumeError); return false;
            }
            s.rootResumed = true;
        }
        refresh_terminal();
        return true;
    }
    bool DisposeEligible(InputGate& gate) noexcept {
        auto& s = *state_;
        if (!thread() || !s.terminal) return false;
        try { if(!gate.BeforeNativeDisposal(s))return false; }
        catch(...) { fail("disposal","observer_native_borrower_gate",ERROR_INVALID_DATA,std::current_exception());return false; }
        bool streamsDisposed = s.streams.DisposeEligible(s.terminal);
        // Stream disposal failure never prevents later independently eligible
        // native process/thread/image/Job close attempts. Every original close
        // error remains in its own preowned stream cell; project exact codes.
        PipeHandle* streamCells[] = {&s.streams.inputRead, &s.streams.inputWrite,
            &s.streams.out.read, &s.streams.out.write, &s.streams.err.read, &s.streams.err.write};
        for (const auto cell : streamCells)
            if (cell->attempted && !cell->closed && !cell->closeReported) {
                cell->closeReported = true;
                fail("disposal", "stream_CloseHandle", cell->closeError);
            }
        for (SIZE_T i = 0; i < s.resourceCount; ++i) close(s.resources[i]);
        // Physical disposal is independent of the absorbing qualification result.
        bool all = streamsDisposed;
        for (SIZE_T i = 0; i < s.resourceCount; ++i)
            all = all && !s.resources[i].unknownOutput && (!s.resources[i].acquired || s.resources[i].closed);
        s.disposed = all;
        if (s.bootstrap && s.disposed) {
            auto root = member(s.root);
            if (!root || root->code != 0) {
                fail("primary", "bootstrap_nonzero", ERROR_PROCESS_ABORTED);
                // Keep the completed physical-disposal fact.
            } else if (!s.stopped && !s.continuationDenied) bootstrapDisposed_ = true;
        }
        return s.disposed;
    }
    bool PollStreams() noexcept {
        if (!thread() || !state_->created) return false;
        bool progressed = state_->streams.Poll();
        if ((state_->streams.out.failed || state_->streams.err.failed) && !streamFailureReported_) {
            streamFailureReported_ = true;
            // Aggregate predicate has no invented native error. Original peek,
            // read and overflow outcomes remain independently in both streams.
            fail("capture", "stream_failed_predicate", ERROR_SUCCESS, {}, false);
        }
        return progressed;
    }
    bool PollFailureDrain(InputGate& gate) noexcept {
        if (!thread() || !state_->created || !state_->stopped) return false;
        try {
            if (!gate.BeforeFailureDrain(*state_)) return false;
        } catch (...) {
            fail("capture", "failure_drain_gate_exception", ERROR_UNHANDLED_EXCEPTION, std::current_exception());
            return false;
        }
        bool progressed = state_->streams.out.DrainPoll();
        return state_->streams.err.DrainPoll() || progressed;
    }
    bool DisposeUnlaunched() noexcept {
        auto& s = *state_;
        if (!thread() || s.created) return false;
        if (s.unlaunchedDisposalAttempted) return s.unlaunchedDisposalResult;
        s.unlaunchedDisposalAttempted = true;
        bool streamsClosed = s.streams.DisposeUnlaunched();
        for (SIZE_T i = 0; i < s.resourceCount; ++i) close(s.resources[i]);
        bool all = streamsClosed && !s.creationOutputsUnknown;
        for (SIZE_T i = 0; i < s.resourceCount; ++i)
            all = all && !s.resources[i].unknownOutput && (!s.resources[i].acquired || s.resources[i].closed);
        s.unlaunchedDisposalResult = all;
        // Independent disposal outcome only: original setup failure remains
        // absorbing and cannot complete bootstrap or authorize another command.
        return all;
    }
    // Reuse only AFTER full bootstrap native resource and stream disposal.
    // The complete input/memory gate and command admission remain separate.
    bool NextCommand() {
        if (!thread() || !bootstrapDisposed_ || !state_->disposed ||
            state_->stopped || state_->continuationDenied) return false;
        if (currentCommand_ + 1 == selectedCommandCount_) {
            fail("primary", "command_history_capacity", ERROR_NOT_ENOUGH_MEMORY);
            return false;
        }
        // Previous originals remain in their preallocated selected state.
        // No allocation can unwind between commands after bootstrap custody.
        state_ = &selectedStates_[++currentCommand_];
        streamFailureReported_ = false;
        return true;
    }
};

// Pure association over already retained originals. This neither queries a
// numeric TID nor grants register access: the controller must use this exact
// typed duplicate while the SAME saved event remains stopped.
inline const Event* OriginalStoppedThreadOccurrenceFor(const State& state,const Event& stopped,const Event& born) noexcept {
    if (!state.eventCount || &stopped != &state.events[state.eventCount-1] || stopped.continued)
        return nullptr;
    bool enrolled=false;
    for(SIZE_T i=0;i<state.eventCount;++i)if(&state.events[i]==&born){enrolled=true;break;}
    if(!enrolled || (born.original.dwDebugEventCode!=CREATE_PROCESS_DEBUG_EVENT &&
        born.original.dwDebugEventCode!=CREATE_THREAD_DEBUG_EVENT) ||
        stopped.original.dwDebugEventCode==EXIT_PROCESS_DEBUG_EVENT ||
        born.original.dwProcessId!=stopped.original.dwProcessId ||
        born.createdThreadResource>=state.resourceCount || born.createdThreadNativeResult>=state.nativeResultCount ||
        born.createdThreadMemberOrdinal>=state.memberCount || born.threadExitObserved || born.threadExitContinued ||
        born.processLastThreadExitEvent!=EventLimit)return nullptr;
    const auto& resource=state.resources[born.createdThreadResource];
    const auto& native=state.nativeResults[born.createdThreadNativeResult];
    const auto& member=state.members[born.createdThreadMemberOrdinal];
    if(resource.kind!=Kind::Thread || !resource.acquired || resource.attempted || resource.closed ||
        resource.unknownOutput || native.original!=resource.original ||
        !native.threadTimesAttempted || !native.timesResult || !positive(native.born) ||
        !native.threadIdAttempted || native.threadIdResult!=born.original.dwThreadId ||
        !native.threadProcessIdAttempted || native.threadProcessIdResult!=born.original.dwProcessId ||
        member.process!=born.createdThreadProcessResource || member.pid!=born.original.dwProcessId)return nullptr;
    for(SIZE_T i=0;i<state.eventCount;++i) {
        const auto& other=state.events[i];
        if(&other==&born || other.createdThreadResource>=state.resourceCount ||
            other.threadExitObserved || other.threadExitContinued || other.processLastThreadExitEvent!=EventLimit)continue;
        if(other.original.dwProcessId==born.original.dwProcessId && other.original.dwThreadId==born.original.dwThreadId)
            return nullptr; // Conflicting unresolved lifetime, never choose by numeric ID.
    }
    return &born;
}
inline const Event* OriginalStoppedThreadOccurrence(const State& state, const Event& stopped) noexcept {
    const Event* found=nullptr;
    for (SIZE_T i=0; i<state.eventCount; ++i) {
        const auto& born=state.events[i];
        if(born.original.dwThreadId!=stopped.original.dwThreadId || !OriginalStoppedThreadOccurrenceFor(state,stopped,born))continue;
        if (found) return nullptr;
        found=&born;
    }
    return found;
}
} // namespace template_creator

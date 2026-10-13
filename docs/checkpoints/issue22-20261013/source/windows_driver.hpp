// UNCOMPILED finite Windows driver composition; no standalone launch entrypoint.
// Native input policy and actual ROOT/source admission are separate constructor
// dependencies. This driver never turns a supplied list/hash into those grants.
#pragma once
#include "windows_lifecycle.cpp"
namespace template_creator {
enum class DriverPhase { NotStarted, Observing, RetainedFailure, NativeDisposed };
struct DriverObservation {
    DriverPhase phase = DriverPhase::NotStarted;
    bool eventProgress = false, streamProgress = false, failureDrainProgress = false;
    bool nativeTerminal = false, nativeDisposed = false;
    // These are source state observations, never product/native acceptance.
};
class FiniteCommandDriver {
    OwnedDebugThread owner_;
    InputGate& concreteGate_;
    DriverObservation latest_{};
public:
    FiniteCommandDriver(InputGate& concreteGate, SIZE_T completeSelectedCommands)
        : owner_(completeSelectedCommands), concreteGate_(concreteGate) {}
    const State& PrivateOriginals() const noexcept { return owner_.Current(); }
    bool ReservedSourceBytes(SIZE_T& bytes) const noexcept {
        SIZE_T ownerBytes = 0;
        if (!owner_.ReservedSourceBytes(ownerBytes)) return false;
        bytes = sizeof(*this)-sizeof(owner_);
        if (ownerBytes > SIZE_MAX-bytes) return false;
        bytes += ownerBytes; return true;
    }
    bool Begin(const wchar_t* image, std::wstring& mutableOriginalCommand,
               const wchar_t* cwd, const wchar_t* literalEnvironment,
               SIZE_T environmentChars, bool bootstrap) noexcept {
        if (latest_.phase != DriverPhase::NotStarted) return false;
        bool began = owner_.Begin(image, mutableOriginalCommand, cwd,
                                 literalEnvironment, environmentChars, concreteGate_, bootstrap);
        latest_.phase = began ? DriverPhase::Observing : DriverPhase::RetainedFailure;
        if (!began && !owner_.Current().created) {
            latest_.nativeDisposed = owner_.DisposeUnlaunched();
            if (latest_.nativeDisposed) latest_.phase = DriverPhase::NativeDisposed;
            // Failed launch qualification remains absorbing; no child terminal is invented.
        }
        return began;
    }
    // No infinite loop/sleep/natural-wait retry. Caller schedules each finite
    // observation under its ACTUAL original command/caller deadlines. No new
    // whole-suite deadline, kill or abandonment semantics are invented here.
    DriverObservation Step() noexcept {
        if (latest_.phase == DriverPhase::NotStarted || latest_.phase == DriverPhase::NativeDisposed ||
                !owner_.Current().created) return latest_;
        latest_.streamProgress = owner_.PollStreams();
        latest_.eventProgress = owner_.Poll(concreteGate_);
        if (owner_.Current().stopped)
            latest_.failureDrainProgress = owner_.PollFailureDrain(concreteGate_);
        else latest_.failureDrainProgress = false;
        latest_.nativeTerminal = owner_.Current().terminal;
        if (latest_.nativeTerminal) owner_.DisposeEligible(concreteGate_);
        latest_.nativeDisposed = owner_.Current().disposed;
        latest_.phase = latest_.nativeDisposed ? DriverPhase::NativeDisposed :
            owner_.Current().stopped ? DriverPhase::RetainedFailure : DriverPhase::Observing;
        return latest_;
    }
    bool NextCommand() {
        if (latest_.phase != DriverPhase::NativeDisposed || !owner_.NextCommand()) return false;
        latest_ = {};
        return true;
    }
};
// NativeDisposed requires source policy, native member terminal/Job-empty,
// original stdout/stderr EOF and actual once-only resource disposal. It does
// not imply complete tool/compiler authority, protected assertion results,
// durable private/public evidence, publication or actual role admission.
} // namespace template_creator

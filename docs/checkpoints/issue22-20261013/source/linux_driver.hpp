// UNCOMPILED finite source composition. No complete input gate/entry admission.
#pragma once
#include "linux_lifecycle.cpp"
namespace template_creator_linux {
enum class DriverPhase { NotStarted, Observing, RetainedFailure, NativeDisposed };
struct DriverObservation {
    DriverPhase phase = DriverPhase::NotStarted;
    bool eventProgress = false, streamProgress = false, failureDrainProgress = false;
    bool nativeTerminal = false, setupValidated = false, nativeDisposed = false;
    int originalRootWaitStatus = 0;
    bool originalRootTerminal = false;
    size_t currentRootTraceeOrdinal = 0;
    int currentRootWaitStatus = 0;
    bool currentRootTerminal = false;
};
class FiniteCommandDriver {
    Owner owner_;
    Gate& concreteGate_;
    DriverObservation latest_{};
public:
    explicit FiniteCommandDriver(Gate& gate) : concreteGate_(gate) {}
    const State& PrivateOriginals() const noexcept { return owner_.Current(); }
    // Native session only: barrier ordinals refer to this SAME retained owner.
    // These do not add Python operations or accept a replacement callback.
    bool RequestOwnedInterrupt(size_t traceeOrdinal) {
        if(latest_.phase!=DriverPhase::Observing)return false;
        const bool requested=owner_.RequestOwnedInterrupt(traceeOrdinal,concreteGate_);
        if(owner_.Current().failed)latest_.phase=DriverPhase::RetainedFailure;
        return requested;
    }
    bool ReleaseOwnedBarrier(size_t originalEventOrdinal) {
        if(latest_.phase==DriverPhase::NotStarted||latest_.phase==DriverPhase::NativeDisposed)return false;
        const bool released=owner_.ReleaseOwnedBarrier(originalEventOrdinal,concreteGate_);
        if(owner_.Current().failed)latest_.phase=DriverPhase::RetainedFailure;
        return released;
    }
    bool ReservedSourceBytes(size_t& bytes) const noexcept {
        size_t owned = 0;
        if (!owner_.ReservedSourceBytes(owned)) return false;
        const size_t additional = sizeof(*this)-sizeof(Owner);
        if (owned > SIZE_MAX-additional) return false;
        bytes = owned+additional; return true;
    }
    // No callback or new wait/continuation after an uncertain outer driver
    // exception. Already-established native terminal/EOF can still dispose.
    bool DisposeAlreadyEligible() noexcept {
        const auto& original = owner_.Current();
        if (original.originalCloneResult <= 0 && !original.rootCreationUnknown)
            return owner_.DisposeUnlaunched();
        if (!original.terminal) return false;
        return owner_.DisposeEligible();
    }
    bool Begin(const char* image, char* const* argv, char* const* environment,
               const char* cwd, int originalEmptyStdin, int originalDirectory) {
        if (latest_.phase != DriverPhase::NotStarted) return false;
        bool began = owner_.Begin(image, argv, environment, cwd, originalEmptyStdin, originalDirectory, concreteGate_);
        latest_.phase = began ? DriverPhase::Observing : DriverPhase::RetainedFailure;
        if (!began && owner_.Current().originalForkResult <= 0) {
            latest_.nativeDisposed = owner_.DisposeUnlaunched();
            if (latest_.nativeDisposed) latest_.phase = DriverPhase::NativeDisposed;
            // Failed launch qualification remains absorbing; no child terminal is invented.
        }
        return began;
    }
    // Exactly one bounded observation step; scheduling and idle-row reservation
    // must derive from the actual source caller deadline before launch. Timeout
    // is failure/retention, never kill, terminal, stream EOF or disposal proof.
    DriverObservation Step() {
        const auto& before = owner_.Current();
        if (latest_.phase == DriverPhase::NotStarted || latest_.phase == DriverPhase::NativeDisposed ||
                before.originalForkResult <= 0) return latest_;
        latest_.streamProgress = owner_.PollStreams();
        latest_.eventProgress = owner_.Poll(concreteGate_);
        latest_.setupValidated = owner_.ValidateSetupEof();
        if (owner_.Current().failed)
            latest_.failureDrainProgress = owner_.PollFailureDrain(concreteGate_);
        else latest_.failureDrainProgress = false;
        latest_.nativeTerminal = owner_.Current().terminal;
        if (latest_.nativeTerminal) owner_.DisposeEligible();
        const auto& current = owner_.Current();
        latest_.nativeDisposed = current.disposed;
        if (current.traceeCount) {
            latest_.originalRootTerminal = current.tracees[0].terminal;
            latest_.originalRootWaitStatus = current.tracees[0].terminalStatus;
            latest_.currentRootTraceeOrdinal = current.currentRootTracee;
            latest_.currentRootWaitStatus = current.tracees[current.currentRootTracee].terminalStatus;
            latest_.currentRootTerminal = current.tracees[current.currentRootTracee].terminal;
        }
        latest_.phase = latest_.nativeDisposed ? DriverPhase::NativeDisposed :
            current.failed ? DriverPhase::RetainedFailure : DriverPhase::Observing;
        return latest_;
    }
};
// Raw nonzero/signaled command results remain caller evidence; this composition
// does not reinterpret expected negative test outcomes as success or enforce an
// invented zero-exit contract. Whole input/tool/native admission is unfinished.
} // namespace template_creator_linux

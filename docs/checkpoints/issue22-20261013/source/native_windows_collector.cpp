// UNCOMPILED retained platform ownership. This is not a standalone host or
// native admission receipt; the creating interpreter/thread must remain alive.
#include "native_windows_platform.hpp"
namespace template_creator_bridge {
bool NativePlatformState::PreownCollector(SIZE_T selectedBytes,SIZE_T selectedReads,SIZE_T selectedObservations,DWORD selectedWaitMilliseconds) noexcept {
    if (collectorPrechargeAttempted) return false;
    collectorPrechargeAttempted = true;
    // All allocation precedes command/bootstrap effects. The registry-owned
    // platform retains the collector even when later acquisition fails.
    if (!prechargeAttempted && !inputBindingAttempted && !collectorStartAttempted) {
        try {
            collector = new template_creator::OriginalWindowsSinkCollector(sink,selectedBytes,selectedReads,selectedObservations,selectedWaitMilliseconds);
            return true;
        } catch (...) { collectorPrechargeOriginal = std::current_exception(); }
    }
    qualificationFailed = true;
    return false;
}
bool NativePlatformState::StartFinalCollector() noexcept {
    if (collectorStartAttempted) return false;
    collectorStartAttempted = true;
    if (!collector || !commands.AllPhysicalCommandsDisposed() || !sink.CompletionKnown()) {
        qualificationFailed = true; return false;
    }
    // Failed qualification may still capture its physically completed prefix.
    // This call seals the source-owned copies before the worker sees them.
    const bool started = collector->Start();
    if (!started) qualificationFailed = true;
    return started;
}
bool NativePlatformState::PollFinalCollector() noexcept {
    if (!collectorStartAttempted || !collector) return false;
    return collector->Poll();
}
bool NativePlatformState::CollectorPhysicalDisposed() const noexcept {
    return !collector || collector->PhysicalDisposed();
}
bool NativePlatformState::CollectorQualified() const noexcept {
    return collectorStartAttempted && collector && collector->Qualified();
}
}

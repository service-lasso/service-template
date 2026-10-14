// UNCOMPILED finite failure cleanup; no new command, kill, detach or guessed EOF.
#ifdef _WIN32
#include "native_windows_platform.hpp"
#include "windows_standalone_host.hpp"
namespace template_creator_bridge {
PyObject* NativePlatformState::DisposeEligible(PyObject* owner) {
    qualificationFailed = true;
    return commands.DisposeIndependentlyEligible(owner);
}
bool NativePlatformState::DisposeHostEligibleOriginals() noexcept {
    if (hostDisposalAttempted) return hostDisposalResult;
    // No native child/collector or unknown sink operation may still depend on
    // input exclusion, expected copies, destination handles or their lifetimes.
    if (!commands.AllPhysicalCommandsDisposed() || !CollectorPhysicalDisposed() || !sink.CompletionKnown()) return false;
    if(sourceHostOriginal && !sourceHostOriginal->StopOriginalNoticeForPlatform(*this))return false;
    if(pendingCallsOriginal) {
        pendingCallsOriginal->CloseFutureEffects();
        if(!pendingCallsOriginal->EndpointsIndependentlyEligible())return false;
    }
    hostDisposalAttempted = true;
    bool all = true;
    for (SIZE_T i=0;i<route.SelectedCount();++i) {
        auto& input = inputOwners[i];
        if (input.files) {
            input.disposalAttempted = true;
            input.disposalResult = input.files->Dispose();
            if (!input.disposalResult) all = false;
        }
        if(input.directoryAcquired && !input.directoryUnknown && !input.directoryCloseAttempted) {
            input.directoryCloseAttempted=true;
            input.directoryCloseReturned=CloseHandle(input.directoryReturned);
            if(!input.directoryCloseReturned)input.directoryCloseError=GetLastError();
        }
        if(input.directoryUnknown || (input.directoryAcquired && !input.directoryCloseReturned))all=false;
    }
    // Every eligible file/crypto close above is attempted even after an earlier
    // independent close failed. Ingress likewise retains each once-close output.
    ingress.DisposeEligible();
    hostDisposalResult = all && AllPhysicalOriginalsDisposed();
    if (!hostDisposalResult) qualificationFailed = true;
    return hostDisposalResult; // Physical-only, never successful qualification.
}
}
#endif

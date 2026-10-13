// UNCOMPILED concrete final route/collector predicate. Native host lifetime and
// final original-close publication remain separate from this Python result.
#ifdef _WIN32
#include "native_windows_platform.hpp"
namespace template_creator_bridge {
bool NativePlatformState::AllPhysicalOriginalsDisposed() const noexcept {
    if (!commands.AllPhysicalCommandsDisposed() || !CollectorPhysicalDisposed() ||
        !sink.CompletionKnown() || !ingress.PhysicallyDisposed()) return false;
    for (SIZE_T i=0;i<route.SelectedCount();++i) {
        const auto& input=inputOwners[i];
        if ((input.files && !input.files->PhysicallyDisposed()) || input.directoryUnknown ||
            (input.directoryAcquired && !input.directoryCloseReturned)) return false;
    }
    return true; // Physical-only, including honestly failed qualification.
}
PyObject* NativePlatformState::VerifyClosure() {
    if(!creatorHeldInputs||!creatorHeldInputs->CurrentFieldIsOriginal()||creatorHeldInputs->replacementObserved)
        qualificationFailed=true;
    if(PyErr_Occurred())return nullptr;
    if(collectorStartAttempted && collector && !CollectorPhysicalDisposed()){
        PollFinalCollector();
        if(!CollectorPhysicalDisposed())Py_RETURN_FALSE;
    }
    bool inputsClean = inputCapturesClean;
    for (SIZE_T i=0;i<route.SelectedCount();++i) {
        const auto& owner = inputOwners[i];
        inputsClean = inputsClean && owner.captureAttempted && owner.files && !owner.files->Failed();
        if(i)inputsClean=inputsClean && owner.directoryAssociated;
        if (owner.files) {
            for (SIZE_T j=0;j<owner.files->FileCount();++j)
                inputsClean = inputsClean && owner.files->PrivateFiles()[j].valid;
        }
    }
    // No pending collector, merely copied bytes, unknown IO or physical-only
    // disposal may produce an affirmative final verification result.
    if (!creatorCaptureAttempted || !creatorRecordAppended || !creatorPrefixReadback ||
        !creatorCollectorStarted || !CollectorPhysicalDisposed() || !CollectorQualified() ||
        !commands.QualificationComplete() || !commands.AllPhysicalCommandsDisposed() ||
        !inputsClean || !sink.CompletionKnown() || !sink.Verified() || qualificationFailed ||
        !LaunchAssociationsEstablished()) {
        qualificationFailed = true;
        if (!PyErr_Occurred()) PyErr_SetString(PyExc_RuntimeError,"complete original native route closure not established");
        return nullptr;
    }
    Py_RETURN_NONE; // Host must separately retain/publish originals and close.
}
}
#endif

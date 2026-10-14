// UNCOMPILED actual standalone-host progression on the original Python/native
// thread. The native main must retain this host and interpreter while physical
// custody is unresolved. Exhausting selected observations is failed UNKNOWN,
// never permission to return from main, finalize Python or terminate a worker.
#ifdef _WIN32
#include "windows_standalone_host.hpp"
namespace template_creator_bridge {
bool OriginalWindowsStandaloneHost::StartConnectedCreator(PyObject* actualSelectedSteps) noexcept {
    if (creatorStartAttempted || !core || !platform || !wrapper ||
        !core->OnOriginalPythonThread() || !core->OnCreatingNativeThread()) return false;
    creatorStartAttempted=true;
    try {
        PyObject* constructed=ConstructCreatorEntry(actualSelectedSteps);
        creatorConstructionReturnOriginal=constructed;
        if (!constructed || PyErr_Occurred()) return false;
        // ConstructCreatorEntry returns an additional owned reference. Keep it
        // in the host rather than release any exception/entry graph at return.
        // entryOriginal already owns the same exact object independently.
        PyObject* invoked=InvokeCreator(entryOriginal);
        creatorReturnOriginal=invoked;
        // The core independently owns the real invocation result. Its exact
        // original, not a truthiness conversion, is the successful return.
        if (!invoked || PyErr_Occurred()) return false;
        return true;
    } catch (...) {
        if (!creatorStartOriginal) creatorStartOriginal=std::current_exception();
        // Preserve any already pending Python original. The native exception
        // is an independent original, not an excuse to replace its triple.
        return false;
    }
}
bool OriginalWindowsStandaloneHost::ObserveConnectedHost() noexcept {
    if (!core || !progress || !core->OnOriginalPythonThread()) return false;
    if (progressCount==selected.hostProgressObservations) {
        progressExhausted=true;
        return false;
    }
    auto& row=progress[progressCount++];
    row.attempted=true;

    // Before any native observation/cleanup effects, retain the current error
    // separately from earlier construction/dispatch/entry snapshot originals.
    if (PyErr_Occurred()) {
        PyErr_Fetch(&row.pendingType,&row.pendingValue,&row.pendingTraceback);
        Py_XINCREF(row.pendingType); Py_XINCREF(row.pendingValue); Py_XINCREF(row.pendingTraceback);
        PyErr_Restore(row.pendingType,row.pendingValue,row.pendingTraceback);
    }
    try {
        if (!core->OnCreatingNativeThread()) return false;
        row.hostSourceCountKnown=ReservedHostSourceBytes(row.reservedHostSourceBytes);
        // A failed construction before platform publication still owns its
        // source channel and final evidence output. It has no child command.
        if (platform) {
            if (platform->collectorStartAttempted && !platform->CollectorPhysicalDisposed()) {
                row.collectorPollAttempted=true;
                row.collectorPollReturned=platform->PollFinalCollector();
            }
            row.sessionCloseAttempted=true;
            row.sessionCloseReturned=ObserveHostClose();
            if (!row.sessionCloseReturned) return false;
        } else if (transferred || creatorInvoked) {
            return false; // Unknown attachment cannot become unlaunched disposal.
        }
        row.hostCloseAttempted=true;
        row.hostCloseReturned=CloseHostOwnedEligible();
        if (!row.hostCloseReturned) return false;
        if (platform && !finalPublicationAttempted) {
            row.publicationAttempted=true;
            row.publicationReturned=PublishPostCloseOriginals();
        }
        // The separate final sink is observed by its own preowned native reader
        // only AFTER its last host write. Expected bytes remain sealed/resident
        // on this same creating host while the native reader runs and joins.
        if (finalOutput && finalOutput->adopted && finalOutput->sink.CompletionKnown() &&
            finalCollector && !finalCollectorStartAttempted) {
            row.finalCollectorStartAttempted=true;
            finalCollectorStartAttempted=true;
            finalCollectorStartReturned=finalCollector->Start();
            row.finalCollectorStartReturned=finalCollectorStartReturned;
        }
        if (finalCollectorStartAttempted && finalCollector && !finalCollector->PhysicalDisposed()) {
            row.finalCollectorPollAttempted=true;
            row.finalCollectorPollReturned=finalCollector->Poll();
            if (!finalCollector->PhysicalDisposed()) return false;
        }
        // A failed known publication may independently close its own outputs.
        // Pending read/write owns the destination and makes this ineligible.
        if (finalOutput && finalOutput->sink.CompletionKnown() &&
            (!finalCollector || finalCollector->PhysicalDisposed()) && !finalCloseAttempted) {
            row.finalCloseAttempted=true;
            finalCloseAttempted=true;
            finalCloseReturned=finalOutput->DisposeEligible();
            row.finalCloseReturned=finalCloseReturned;
        }
        row.physicalDisposed=hostHandlesPhysicallyClosed && finalCloseAttempted && finalCloseReturned &&
            (!finalCollector || finalCollector->PhysicalDisposed()) &&
            (!platform || core->PrivateHostClosure().physicalDisposed);
        row.qualificationFailed=!noticeOwner || !noticeOwner->qualified_ || !row.hostSourceCountKnown || !platform || !creatorInvoked || core->PrivateHostClosure().qualificationFailed ||
            !finalPublicationVerified || !finalCollectorStartReturned || !finalCollector || !finalCollector->Qualified() ||
            !row.physicalDisposed || row.pendingType || connectOriginal || prechargeOriginal ||
            creatorStartOriginal || progressExhausted;
        return row.physicalDisposed;
    } catch (...) {
        row.nativeOriginal=std::current_exception();
        row.qualificationFailed=true;
        return false;
    }
}
}
#endif

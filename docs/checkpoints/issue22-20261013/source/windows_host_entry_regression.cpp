// UNRUN/UNCOMPILED regression functions. A separately admitted native test host
// supplies a fresh ACTUAL connected host per case, with no creator invocation.
// These cases exercise the real imports/constructor/cleanup, not a replacement
// module, native session or authority fixture. They launch no product command.
#ifdef _WIN32
#include "windows_standalone_host.hpp"
namespace template_creator_bridge::regression {
struct EntryFailureObservation {
    PyObject* selectedSteps=nullptr;
    PyObject* constructionResult=nullptr;
    PyObject* previousModule=nullptr;
    PyObject* errorType=nullptr;
    PyObject* errorValue=nullptr;
    PyObject* errorTraceback=nullptr;
    PyObject* restorationType=nullptr;
    PyObject* restorationValue=nullptr;
    PyObject* restorationTraceback=nullptr;
    int installResult=-1,restoreResult=-1;
    bool hadPreviousModule=false,actualFailure=false,originalRetained=false;
    bool closeReturned=false,hostCloseReturned=false,preEntry=false;
    bool noCreatorInvocation=false,physicalDisposed=false,qualificationFailed=false;
};
// Storage and the fresh host must remain alive in the native test custodian.
// sys.modules belongs to this admitted owned test interpreter. None is Python's
// documented import-denial sentinel; no fake class/module claims native rights.
bool OriginalImportFailure(OriginalWindowsStandaloneHost& host,EntryFailureObservation& row) {
    if (!host.wrapper || !host.platform || host.entryConstructionAttempted || host.creatorInvoked || PyErr_Occurred()) return false;
    PyObject* modules=PyImport_GetModuleDict();
    if (!modules) return false;
    PyObject* previous=PyDict_GetItemString(modules,"native_session");
    row.hadPreviousModule=previous!=nullptr;
    row.previousModule=Py_XNewRef(previous);
    row.selectedSteps=PyTuple_New(0);
    if (!row.selectedSteps) return false;
    row.installResult=PyDict_SetItemString(modules,"native_session",Py_None);
    if (row.installResult!=0) return false;
    row.constructionResult=host.ConstructCreatorEntry(row.selectedSteps);
    row.actualFailure=!row.constructionResult && PyErr_ExceptionMatches(PyExc_ModuleNotFoundError);
    // Keep this exact triple independently of module restoration and cleanup.
    PyErr_Fetch(&row.errorType,&row.errorValue,&row.errorTraceback);
    row.originalRetained=row.errorType==host.constructionType && row.errorValue==host.constructionValue &&
        row.errorTraceback==host.constructionTraceback;
    row.restoreResult=row.hadPreviousModule ? PyDict_SetItemString(modules,"native_session",row.previousModule) :
        PyDict_DelItemString(modules,"native_session");
    if (PyErr_Occurred()) PyErr_Fetch(&row.restorationType,&row.restorationValue,&row.restorationTraceback);
    Py_XINCREF(row.errorType); Py_XINCREF(row.errorValue); Py_XINCREF(row.errorTraceback);
    PyErr_Restore(row.errorType,row.errorValue,row.errorTraceback);
    row.closeReturned=host.ObserveHostClose();
    row.hostCloseReturned=host.CloseHostOwnedEligible();
    const auto& close=host.core->PrivateHostClosure();
    row.preEntry=close.preEntryDisposal;
    row.noCreatorInvocation=!host.creatorInvoked && !host.core->PrivateCreatorInvocation().attempted;
    row.physicalDisposed=close.physicalDisposed;
    row.qualificationFailed=close.qualificationFailed;
    return row.actualFailure && row.originalRetained && row.restoreResult==0 && !row.restorationType &&
        row.closeReturned && row.hostCloseReturned && row.preEntry && row.noCreatorInvocation &&
        row.physicalDisposed && row.qualificationFailed && close.pendingType==row.errorType && close.pendingValue==row.errorValue;
}
bool OriginalEntryConstructorFailure(OriginalWindowsStandaloneHost& host,EntryFailureObservation& row) {
    if (!host.wrapper || !host.platform || host.entryConstructionAttempted || host.creatorInvoked || PyErr_Occurred()) return false;
    row.selectedSteps=PyTuple_New(0);
    if (!row.selectedSteps) return false;
    row.constructionResult=host.ConstructCreatorEntry(row.selectedSteps);
    // Actual admitted NativeSession and actual TemplateCreator constructor must
    // have been reached. Empty exact roster fails before any command capture.
    row.actualFailure=!row.constructionResult && host.bridgeOriginal && host.creatorClass && host.phaseOriginal &&
        PyErr_ExceptionMatches(PyExc_ValueError);
    PyErr_Fetch(&row.errorType,&row.errorValue,&row.errorTraceback);
    row.originalRetained=row.errorType==host.constructionType && row.errorValue==host.constructionValue &&
        row.errorTraceback==host.constructionTraceback;
    Py_XINCREF(row.errorType); Py_XINCREF(row.errorValue); Py_XINCREF(row.errorTraceback);
    PyErr_Restore(row.errorType,row.errorValue,row.errorTraceback);
    row.closeReturned=host.ObserveHostClose();
    row.hostCloseReturned=host.CloseHostOwnedEligible();
    const auto& close=host.core->PrivateHostClosure();
    row.preEntry=close.preEntryDisposal;
    row.noCreatorInvocation=!host.creatorInvoked && !host.core->PrivateCreatorInvocation().attempted;
    row.physicalDisposed=close.physicalDisposed; row.qualificationFailed=close.qualificationFailed;
    return row.actualFailure && row.originalRetained && row.closeReturned && row.hostCloseReturned &&
        row.preEntry && row.noCreatorInvocation && row.physicalDisposed && row.qualificationFailed &&
        close.pendingType==row.errorType && close.pendingValue==row.errorValue;
}
// Wrapper allocation and factory exceptions before/after exact five-handle
// transfer still need their own admitted allocation-failure runs. Neither of
// the above failures is credited as exercising those different boundaries.
}
#endif

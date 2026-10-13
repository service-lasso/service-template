// UNCOMPILED native host post-entry closure. The standalone host must consume
// this retained row while keeping its original interpreter/thread alive. A
// false result never authorizes host exit, interpreter finalization or retry of
// an uncertain close. This is not an installed-host or runtime qualification.
#ifdef _WIN32
#include "native_windows_platform.hpp"
namespace template_creator_bridge {
bool NativeSessionCore::FinishWindowsHostOriginals() noexcept {
    auto& row = hostClosure_;
    if (row.eligible || row.nativeException) return row.physicalDisposed;
    if (!OnOriginalPythonThread() || !OnCreatingNativeThread()) return false;
    row.attempted = true;
    // Retain a pending original before native cleanup. In particular an error
    // after CaptureCreator's earlier failure snapshot remains both here and in
    // the strong entry/dispatch owners. Never replace it with a cleanup error.
    if (!row.pendingType && !row.pendingValue && !row.pendingTraceback && PyErr_Occurred()) {
        PyErr_Fetch(&row.pendingType,&row.pendingValue,&row.pendingTraceback);
        Py_XINCREF(row.pendingType); Py_XINCREF(row.pendingValue); Py_XINCREF(row.pendingTraceback);
        PyErr_Restore(row.pendingType,row.pendingValue,row.pendingTraceback);
    }
    try {
        if (!platform_) return false;
        row.preEntryDisposal=!creatorInvocation_.attempted;
        // Collector polling belongs to its precharged native schedule. Do not
        // start another collector or renew the original creator deadline here.
        row.eligible = platform_->commands.AllPhysicalCommandsDisposed() &&
            platform_->CollectorPhysicalDisposed() && platform_->sink.CompletionKnown();
        if (!row.eligible) return false;
        row.disposalReturned = platform_->DisposeHostEligibleOriginals();
        row.physicalDisposed = row.disposalReturned && platform_->AllPhysicalOriginalsDisposed();
        row.qualificationFailed = row.preEntryDisposal || platform_->qualificationFailed ||
            !platform_->commands.QualificationComplete() || !platform_->CollectorQualified() ||
            !platform_->sink.Verified() || !row.physicalDisposed ||
            creatorInvocation_.pythonType || creatorInvocation_.nativeException || row.pendingType;
    } catch (...) {
        row.nativeException = std::current_exception();
        row.qualificationFailed = true;
    }
    return row.physicalDisposed; // Never qualification, publication or exit authority.
}
}
#endif

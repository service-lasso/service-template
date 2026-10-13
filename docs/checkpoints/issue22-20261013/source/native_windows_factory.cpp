// UNCOMPILED native-only factory. The source-owned host/creating-thread lifetime
// still must be implemented; a resident registry pointer is not host proof.
#ifdef _WIN32
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include "native_session_registry.hpp"
#include "native_windows_platform.hpp"
namespace template_creator_bridge {
NativePlatformState* RetainedNativeRegistry::PreownWindowsPlatform(NativeSessionCore& original,
    OriginalPhase originalPhase,std::size_t protectedCommands,std::size_t sinkBytes,std::size_t readbackBytes,
    std::size_t sinkCalls,std::size_t collectorBytes,std::size_t collectorReads,
    std::size_t collectorObservations,unsigned long collectorWaitMilliseconds,const std::array<void*,5>& actualTransferredOriginals,
    OriginalWindowsPendingCalls& pendingCalls) {
    if (!Owns(&original) || original.platformAttachmentAttempted_) return nullptr;
    original.platformAttachmentAttempted_ = true;
    if (originalPhase != OriginalPhase::Generation && originalPhase != OriginalPhase::FinalQualification &&
        originalPhase != OriginalPhase::NativeHelper) return nullptr;
    if (!original.OnOriginalPythonThread() || !original.OnCreatingNativeThread()) return nullptr;
    std::size_t ordinal = 0;
    while (ordinal<count_ && sessions_[ordinal]!=&original) ++ordinal;
    if (ordinal==count_) return nullptr;
    NativePlatformState* state = nullptr;
    try {
        // Complete allocation precedes transfer. Caller retains originals if
        // this constructor fails; no CloseHandle or ingress query has occurred.
        state = new NativePlatformState(originalPhase,protectedCommands,sinkBytes,readbackBytes,sinkCalls);
    } catch (...) { original.platformAttachmentOriginal_ = std::current_exception(); throw; }
    platforms_[ordinal] = state; original.platform_ = state;
    if (!state->PreownCollector(collectorBytes,collectorReads,collectorObservations,collectorWaitMilliseconds)) {
        // Caller still owns every transfer handle. Retain the published source
        // owner/allocation original, without pretending ingress was acquired.
        if (!PyErr_Occurred()) PyErr_SetString(PyExc_RuntimeError,"independent collector precharge failed before ingress transfer");
        return state;
    }
    // Both registry references precede ALL acquisition/query/conversion effects.
    // The ingress attaches all five handles before its first native call.
    state->pendingCallsOriginal = &pendingCalls;
    state->transferredOriginals = actualTransferredOriginals;
    const bool captured = state->ingress.CaptureTransferredOriginals(state->transferredOriginals,pendingCalls);
    state->originalIngressIdentity = PyObject_CallNoArgs(reinterpret_cast<PyObject*>(&PyBaseObject_Type));
    if (!state->originalIngressIdentity) {
        PyErr_Fetch(&original.platformPythonType_,&original.platformPythonValue_,&original.platformPythonTraceback_);
        Py_XINCREF(original.platformPythonType_); Py_XINCREF(original.platformPythonValue_); Py_XINCREF(original.platformPythonTraceback_);
        PyErr_Restore(original.platformPythonType_,original.platformPythonValue_,original.platformPythonTraceback_);
        return state; // Actual failed acquisition/capabilities remain retained.
    }
    state->ingressBound = captured; // Native capture result; no root/tool/role authority grant.
    return state;
}
}
#endif

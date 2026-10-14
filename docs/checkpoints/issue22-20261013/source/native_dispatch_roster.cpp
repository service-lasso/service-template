// UNCOMPILED preowned native dispatch roster. Platform Invoke/gates remain in
// the connected platform source unit; this file contains no fallback outcomes.
#include "native_session_core.hpp"
#include <stdexcept>
#ifdef _WIN32
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#else
#include <sys/syscall.h>
#include <unistd.h>
#include <cerrno>
#endif
namespace template_creator_bridge {
bool NativeSessionCore::PreflightCreatorDispatches(PyObject* steps) noexcept {
    // Current Precharge dispatch is already reserved. This is remaining real
    // storage, not SIZE_MAX masquerading as physical reservation authority.
    if(!steps||!PyTuple_CheckExact(steps)||!PyTuple_GET_SIZE(steps))return false;
    std::size_t required=5,last=0;
    const auto add=[&](std::size_t n) noexcept {
        if(n>SIZE_MAX-required)return false;required+=n;return true;
    };
    for(Py_ssize_t i=0;i<PyTuple_GET_SIZE(steps);++i){
        auto* step=PyTuple_GET_ITEM(steps,i);
        if(!step||!PyTuple_Check(step)||PyTuple_GET_SIZE(step)!=5)return false;
        auto* count=PyTuple_GET_ITEM(step,3);
        if(!count||!PyLong_CheckExact(count))return false;
        const auto observations=PyLong_AsSize_t(count);
        if(PyErr_Occurred()||!observations||observations>65536||
           observations>(SIZE_MAX-6)/3||!add(6+3*observations))return false;
        last=observations;
    }
    return add(last)&&!dispatchExhausted_&&dispatchCount_<=dispatchCapacity_&&
        required<=dispatchCapacity_-dispatchCount_;
}
static void CaptureNativeThread(OriginalNativeThreadCheck& row) noexcept {
    row.attempted = true;
#ifdef _WIN32
    row.returned = GetCurrentThreadId();
#else
    row.returned = syscall(SYS_gettid);
    if (row.returned < 0) row.error = errno;
#endif
}
NativeSessionCore::NativeSessionCore(std::size_t completeSelectedDispatchRows,std::size_t completeSelectedHostObservations) {
    if (!Py_IsInitialized() || !PyGILState_Check())
        throw std::invalid_argument("original initialized interpreter/GIL required before native registry acquisition");
    originalPythonThread_ = PyThreadState_Get();
    originalInterpreter_ = PyThreadState_GetInterpreter(originalPythonThread_);
    constexpr auto maximum = std::numeric_limits<std::size_t>::max();
    if (!completeSelectedHostObservations || completeSelectedHostObservations>maximum-4)
        throw std::invalid_argument("complete original host observation capacity");
    const auto fixedChecks=completeSelectedHostObservations+4;
    if (fixedChecks>(maximum-sizeof(*this))/sizeof(OriginalNativeThreadCheck) ||
        !completeSelectedDispatchRows || completeSelectedDispatchRows > maximum-fixedChecks ||
        completeSelectedDispatchRows >
            (maximum-sizeof(*this)-fixedChecks*sizeof(OriginalNativeThreadCheck))/(sizeof(OriginalDispatch)+sizeof(OriginalNativeThreadCheck)))
        throw std::invalid_argument("complete original dispatch roster capacity");
    dispatches_ = std::make_unique<OriginalDispatch[]>(completeSelectedDispatchRows);
    // Exact source path: one platform factory, one Wrap, one bridge require,
    // one original host creator invocation, then one Require
    // per selected dispatch, plus explicitly precharged host closure observations.
    // Additional calls exhaust instead of overwriting; no deadline is renewed.
    nativeThreadCapacity_ = completeSelectedDispatchRows+fixedChecks;
    nativeThreadChecks_ = std::make_unique<OriginalNativeThreadCheck[]>(nativeThreadCapacity_);
    dispatchCapacity_ = completeSelectedDispatchRows;
    CaptureNativeThread(nativeThreadBirth_); // No later throwing acquisition.
}
bool NativeSessionCore::OnOriginalPythonThread() const noexcept {
    if (!Py_IsInitialized() || !PyGILState_Check()) return false;
    PyThreadState* current = PyThreadState_Get();
    return current == originalPythonThread_ &&
        PyThreadState_GetInterpreter(current) == originalInterpreter_;
}
std::size_t NativeSessionCore::ReservedDispatchSourceBytes() const noexcept {
    // Python argument/result/error referents and native platform stores are NOT
    // included by this source struct counter; actual simultaneous fit is separate.
    return sizeof(*this) + dispatchCapacity_*sizeof(OriginalDispatch) + nativeThreadCapacity_*sizeof(OriginalNativeThreadCheck);
}
bool NativeSessionCore::OnCreatingNativeThread() const noexcept {
    if (!OnOriginalPythonThread() || nativeThreadExhausted_) return false;
    if (nativeThreadCount_ == nativeThreadCapacity_) { nativeThreadExhausted_ = true; return false; }
    auto& row = nativeThreadChecks_[nativeThreadCount_++]; CaptureNativeThread(row);
    row.matched = nativeThreadBirth_.returned > 0 && row.returned == nativeThreadBirth_.returned;
    return row.matched;
}
OriginalDispatch* NativeSessionCore::ReserveDispatch(Operation operation, PyObject* originals) noexcept {
    if (!OnOriginalPythonThread() || !originals || !PyTuple_CheckExact(originals)) return nullptr;
    if (dispatchExhausted_ || dispatchCount_ == dispatchCapacity_) {
        dispatchExhausted_ = true;
        if (!overflowDispatch_.arguments) {
            overflowDispatch_.operation = operation;
            overflowDispatch_.arguments = originals;
            Py_INCREF(originals);
        }
        return nullptr;
    }
    OriginalDispatch& row = dispatches_[dispatchCount_++];
    row.operation = operation;
    row.arguments = originals;
    Py_INCREF(originals); // Non-allocating strong attachment before any Invoke effect.
    return &row;
}
bool NativeSessionCore::ClaimReservedInvocation(Operation operation,PyObject* originals) noexcept {
    if (!OnOriginalPythonThread() || !dispatchCount_ || dispatchExhausted_) return false;
    auto& row = dispatches_[dispatchCount_-1];
    if (!row.attempted || row.platformInvoked || row.returned || row.operation != operation || row.arguments != originals) return false;
    row.platformInvoked = true; return true;
}
// The retained native registry must not destroy this core while processes,
// streams, handles, original errors or unflushed sink custody are unresolved.
// Consequently no destructor decrefs original Python referents or closes native
// resources. Eligible release is an explicit native session operation, pending
// complete platform closure and durable original readback, never wrapper GC.
}

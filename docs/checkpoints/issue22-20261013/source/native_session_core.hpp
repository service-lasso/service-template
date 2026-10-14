// UNCOMPILED native bridge contract. Platform bodies are part of this source unit,
// not dynamically supplied application callbacks or an external issuer API.
#pragma once
#include <Python.h>
#include "native_creator_held_inputs.hpp"
#include <exception>
#include <cstddef>
#include <memory>
#include <limits>
namespace template_creator_bridge {
enum class Operation {
    AdoptIngress, PrechargeRoute, CaptureInputClasses, BindInputs, PrepareSink, PreownedCommand,
    Begin, Step, CaptureCommand, CommandDisposed, VerifyCommand,
    DisposeEligible, InputDisposalEligible, CaptureCreator, ReadbackSink, VerifyClosure
};
enum class OriginalPhase { Generation, FinalQualification, NativeHelper };
struct OriginalDispatch {
    Operation operation{};
    PyObject* arguments = nullptr; // Strong original ref, no serialized substitute.
    PyObject* result = nullptr;    // Strong original ref, including failed routes.
    std::exception_ptr nativeException;
    PyObject* pythonType = nullptr;
    PyObject* pythonValue = nullptr;
    PyObject* pythonTraceback = nullptr;
    bool attempted = false, platformInvoked = false, returned = false;
};
struct OriginalNativeThreadCheck {
    long long returned = -1;
    int error = 0;
    bool attempted = false, matched = false;
};
struct OriginalCreatorInvocation {
    OriginalCreatorHeldInputs heldInputs;
    PyObject* entry = nullptr;
    PyObject* method = nullptr;
    PyObject* result = nullptr;
    PyObject* pythonType = nullptr;
    PyObject* pythonValue = nullptr;
    PyObject* pythonTraceback = nullptr;
    std::exception_ptr nativeException;
    bool attempted = false, callAttempted = false, returned = false;
};
struct OriginalHostClosure {
    bool finalInputCutAttempted=false,finalInputCutReturned=false;
    std::exception_ptr finalInputCutOriginal;
    bool attempted = false, eligible = false, disposalReturned = false;
    bool preEntryDisposal = false;
    bool physicalDisposed = false, qualificationFailed = true;
    bool linuxFinalHostConsumed=false,linuxFinalHostNormal=false;
    PyObject* pendingType = nullptr;
    PyObject* pendingValue = nullptr;
    PyObject* pendingTraceback = nullptr;
    std::exception_ptr nativeException;
};
// Defined by the connected platform session implementation. No virtual methods,
// mutable function table, caller booleans or Python attribute dispatch.
struct NativePlatformState;
class RetainedNativeRegistry;
class NativeSessionCore {
    friend class RetainedNativeRegistry;
    ~NativeSessionCore() = default; // Registry-owned; never Python/stack lifetime.
    std::unique_ptr<OriginalDispatch[]> dispatches_;
    std::size_t dispatchCapacity_ = 0, dispatchCount_ = 0;
    OriginalDispatch overflowDispatch_{};
    bool dispatchExhausted_ = false;
    PyThreadState* originalPythonThread_ = nullptr;
    PyInterpreterState* originalInterpreter_ = nullptr;
    NativePlatformState* platform_ = nullptr; // Retained native registry owns lifetime.
    bool platformAttachmentAttempted_ = false;
    std::exception_ptr platformAttachmentOriginal_;
    PyObject* platformPythonType_ = nullptr;
    PyObject* platformPythonValue_ = nullptr;
    PyObject* platformPythonTraceback_ = nullptr;
    OriginalNativeThreadCheck nativeThreadBirth_{};
    mutable std::unique_ptr<OriginalNativeThreadCheck[]> nativeThreadChecks_;
    std::size_t nativeThreadCapacity_ = 0;
    mutable std::size_t nativeThreadCount_ = 0;
    mutable bool nativeThreadExhausted_ = false;
    bool ClaimReservedInvocation(Operation operation,PyObject* originals) noexcept;
    OriginalCreatorInvocation creatorInvocation_{};
    OriginalHostClosure hostClosure_{};
public:
    NativeSessionCore(std::size_t completeSelectedDispatchRows,std::size_t completeSelectedHostObservations);
    NativeSessionCore(const NativeSessionCore&) = delete;
    NativeSessionCore& operator=(const NativeSessionCore&) = delete;
    NativeSessionCore(NativeSessionCore&&) = delete;
    NativeSessionCore& operator=(NativeSessionCore&&) = delete;
    std::size_t ReservedDispatchSourceBytes() const noexcept;
    const OriginalDispatch* PrivateDispatches() const noexcept { return dispatches_.get(); }
    std::size_t PrivateDispatchCount() const noexcept { return dispatchCount_; }
    const OriginalDispatch& PrivateOverflowDispatch() const noexcept { return overflowDispatch_; }
    bool DispatchExhausted() const noexcept { return dispatchExhausted_; }
    bool PreflightCreatorDispatches(PyObject* steps) noexcept;
    const OriginalNativeThreadCheck& PrivateNativeThreadBirth() const noexcept { return nativeThreadBirth_; }
    const OriginalNativeThreadCheck* PrivateNativeThreadChecks() const noexcept { return nativeThreadChecks_.get(); }
    std::size_t PrivateNativeThreadCheckCount() const noexcept { return nativeThreadCount_; }
    const std::exception_ptr& PrivatePlatformAttachmentOriginal() const noexcept { return platformAttachmentOriginal_; }
    PyObject* PrivatePlatformPythonType() const noexcept { return platformPythonType_; }
    PyObject* PrivatePlatformPythonValue() const noexcept { return platformPythonValue_; }
    PyObject* PrivatePlatformPythonTraceback() const noexcept { return platformPythonTraceback_; }
    PyObject* Invoke(Operation operation, PyObject* originals);
    bool OnOriginalPythonThread() const noexcept;
    bool OnCreatingNativeThread() const noexcept;
    bool IsOriginalIngress(PyObject* original) const noexcept;
    // The owner retains raw outputs BEFORE conversion/exception propagation.
    OriginalDispatch* ReserveDispatch(Operation operation, PyObject* originals) noexcept;
    bool AllPhysicalCustodyDisposed() const noexcept;
    // Native host only. Retaining entry itself also retains failures appended
    // AFTER the final private_failures snapshot. No module method exposes this.
    PyObject* RunRetainedCreatorEntry(PyObject* originalEntry);
    const OriginalCreatorInvocation& PrivateCreatorInvocation() const noexcept { return creatorInvocation_; }
    // Native creating-thread caller only; never a Python operation. This keeps
    // the original entry/error graph resident and does not finalize Python.
    bool FinishWindowsHostOriginals() noexcept;
    bool FinishLinuxHostOriginals() noexcept;
    bool PrepareLinuxFinalPublicationInputs() noexcept;
    const OriginalHostClosure& PrivateHostClosure() const noexcept { return hostClosure_; }
    NativePlatformState* PrivatePlatformOriginal() const noexcept { return platform_; }
};
// Called directly inside the admitted embedded/native custodian while the core
// owner is retained. It is deliberately absent from Python module methods.
// Core must outlive the wrapper even when Python drops all references: retained
// process/IO/error custody cannot be disposed by Python object finalization.
PyObject* WrapRetainedOriginal(NativeSessionCore& original);
}

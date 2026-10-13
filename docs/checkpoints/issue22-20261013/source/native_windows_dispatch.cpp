// UNCOMPILED platform dispatch. Remaining concrete ingress/capture/host bodies
// are required before this translation unit can link; no fallback is provided.
#ifdef _WIN32
#include "native_windows_platform.hpp"
namespace template_creator_bridge {
bool NativeSessionCore::IsOriginalIngress(PyObject* original) const noexcept {
    return platform_ && platform_->ingressBound && original && original == platform_->originalIngressIdentity;
}
bool NativeSessionCore::AllPhysicalCustodyDisposed() const noexcept {
    return platform_ && platform_->AllPhysicalOriginalsDisposed();
}
PyObject* NativeSessionCore::Invoke(Operation operation,PyObject* originals) {
    if (!platform_ || !OnOriginalPythonThread() || !originals || !PyTuple_CheckExact(originals) ||
        !ClaimReservedInvocation(operation,originals)) {
        if (!PyErr_Occurred()) PyErr_SetString(PyExc_RuntimeError,"retained original native Windows platform required");
        return nullptr;
    }
    const auto count = PyTuple_GET_SIZE(originals);
    const auto at = [&](Py_ssize_t i) noexcept { return PyTuple_GET_ITEM(originals,i); };
    // Arity is checked again at the platform boundary, including native callers;
    // module dispatch has already retained this exact arguments tuple.
    switch (operation) {
    case Operation::AdoptIngress: if (count==3) return platform_->AdoptIngress(at(0),at(1),at(2)); break;
    case Operation::PrechargeRoute:
        if(count==1){
            if(!PreflightCreatorDispatches(at(0))){
                platform_->qualificationFailed=true;
                if(!PyErr_Occurred())PyErr_SetString(PyExc_RuntimeError,"complete creator dispatch subdivision unavailable");
                return nullptr;
            }
            return platform_->PrechargeRoute(at(0));
        }break;
    case Operation::CaptureInputClasses: if (count==2) return platform_->CaptureInputClasses(at(0),at(1)); break;
    case Operation::BindInputs: if (count==2) return platform_->BindInputs(at(0),at(1)); break;
    case Operation::PrepareSink: if (count==0) return platform_->PrepareSink(); break;
    case Operation::PreownedCommand: if (count==1) return platform_->commands.Preowned(at(0)); break;
    case Operation::Begin:
        if (count==2) {
            if (!platform_->LaunchAssociationsEstablished()) {
                if (!PyErr_Occurred()) PyErr_SetString(PyExc_RuntimeError,"complete original native launch associations unmet");
                return nullptr;
            }
            return platform_->commands.Begin(at(0),at(1));
        }
        break;
    case Operation::Step: if (count==1) return platform_->commands.Step(at(0)); break;
    case Operation::CaptureCommand: if (count==1) return platform_->CaptureCommand(at(0)); break;
    case Operation::CommandDisposed: if (count==1) return platform_->commands.Disposed(at(0)); break;
    case Operation::VerifyCommand: if (count==2) return platform_->VerifyCommand(at(0),at(1)); break;
    case Operation::DisposeEligible: if (count==1) return platform_->DisposeEligible(at(0)); break;
    case Operation::InputDisposalEligible: if (count==2) return platform_->InputDisposalEligible(at(0),at(1)); break;
    case Operation::CaptureCreator: if (count==2) return platform_->CaptureCreator(at(0),at(1)); break;
    case Operation::ReadbackSink:
        if (count==0) {
            if (!platform_->sink.FlushReadbackOriginal()) {
                if (!PyErr_Occurred()) PyErr_SetString(PyExc_RuntimeError,"original native sink prefix readback failed");
                return nullptr;
            }
            Py_RETURN_NONE;
        }
        break;
    case Operation::VerifyClosure: if (count==0) return platform_->VerifyClosure(); break;
    }
    if (!PyErr_Occurred()) PyErr_SetString(PyExc_ValueError,"original native operation arity");
    return nullptr;
}
}
#endif

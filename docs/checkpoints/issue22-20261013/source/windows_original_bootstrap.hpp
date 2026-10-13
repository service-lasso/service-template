// UNCOMPILED actual fixed no-argument bootstrap capture. The five-field native
// converter is reused only to construct image/cwd/environment/requirements;
// no generation producer or product command is invoked by this owner.
#pragma once
#include "windows_original_route_arguments.hpp"
#include "native_windows_platform.hpp"
namespace template_creator_bridge {
struct OriginalBootstrapCapture {
    OriginalWindowsRouteArguments* conversion=nullptr;
    PyObject *module=nullptr,*commandClass=nullptr,*inputsClass=nullptr,*argv=nullptr,*origin=nullptr;
    PyObject *heldModule=nullptr,*heldClass=nullptr,*held=nullptr;
    PyObject *command=nullptr,*inputs=nullptr,*captureMethod=nullptr,*snapshot=nullptr;
    PyObject *failureType=nullptr,*failureValue=nullptr,*failureTraceback=nullptr;
    bool attempted=false,returned=false,bindingAttempted=false,bound=false;
    bool disposalAttempted=false,disposalReturned=false,physicallyDisposed=false;
    PyObject *closeMethod=nullptr,*closeResult=nullptr,*failureMethod=nullptr,*failureResult=nullptr;
    PyObject *cleanupType=nullptr,*cleanupValue=nullptr,*cleanupTraceback=nullptr;
    PyObject *unresolvedMethod=nullptr,*unresolvedResult=nullptr;
    PyObject *revalidationMethod=nullptr,*revalidationResult=nullptr;
    PyObject *revalidationType=nullptr,*revalidationValue=nullptr,*revalidationTraceback=nullptr;
    bool revalidationAttempted=false,revalidationReturned=false;
private:
    ~OriginalBootstrapCapture()=default; // Actual input holds/exception refs resident.
    bool Clean(PyObject* value) noexcept {
        if(!value && !PyErr_Occurred())PyErr_SetString(PyExc_SystemError,"bootstrap capture missing actual return");
        if(!PyErr_Occurred())return value!=nullptr;
        if(!failureType&&!failureValue&&!failureTraceback) {
            PyErr_Fetch(&failureType,&failureValue,&failureTraceback);
            Py_XINCREF(failureType);Py_XINCREF(failureValue);Py_XINCREF(failureTraceback);
            PyErr_Restore(failureType,failureValue,failureTraceback);
        }
        return false;
    }
public:
    // Selection.node is actual bootstrap binary, not the product Node image.
    // Generation here names the existing five-field conversion shape only;
    // calling any selected_route producer on it is forbidden by this caller.
    explicit OriginalBootstrapCapture(const OriginalNativeRouteSelection& original,SIZE_T stringUnits) {
        if(original.phase!=OriginalPhase::Generation)throw std::invalid_argument("bootstrap single input conversion shape");
        conversion=new OriginalWindowsRouteArguments(original,stringUnits,false);
    }
    OriginalBootstrapCapture(const OriginalBootstrapCapture&)=delete;
    OriginalBootstrapCapture& operator=(const OriginalBootstrapCapture&)=delete;
    bool CaptureAndBind(NativePlatformState& platform) noexcept {
        if(attempted)return false;attempted=true;
        if(!conversion->BuildOriginalArguments())return false;
        module=PyImport_ImportModule("command_inputs");if(!Clean(module))return false;
        commandClass=PyObject_GetAttrString(module,"Command");if(!Clean(commandClass))return false;
        inputsClass=PyObject_GetAttrString(module,"CommandInputs");if(!Clean(inputsClass))return false;
        argv=PyTuple_New(0);if(!Clean(argv))return false;
        origin=PyUnicode_FromString("source-owned no-argument debugger bootstrap");if(!Clean(origin))return false;
        command=PyObject_CallFunctionObjArgs(commandClass,conversion->OriginalNode(),argv,conversion->OriginalRoot(),
            conversion->OriginalEnvironment(),Py_None,origin,nullptr);
        if(!Clean(command))return false;
        heldModule=PyImport_ImportModule("held_inputs");if(!Clean(heldModule))return false;
        heldClass=PyObject_GetAttrString(heldModule,"HeldInputs");if(!Clean(heldClass))return false;
        held=PyObject_CallNoArgs(heldClass);if(!Clean(held))return false;
        inputs=PyObject_CallOneArg(inputsClass,held);if(!Clean(inputs))return false;
        captureMethod=PyObject_GetAttrString(inputs,"capture");if(!Clean(captureMethod))return false;
        snapshot=PyObject_CallFunctionObjArgs(captureMethod,command,conversion->OriginalRequirements(0),nullptr);
        if(!Clean(snapshot))return false;
        returned=true;bindingAttempted=true;
        bound=platform.BindOriginalBootstrap(snapshot);
        if(PyErr_Occurred()){Clean(Py_None);return false;}
        return bound;
    }
    bool DisposeAfterPlatformClosure(NativePlatformState& platform) noexcept {
        if(disposalAttempted)return physicallyDisposed;
        if(!held)return true;
        if(!platform.AllPhysicalOriginalsDisposed())return false;
        disposalAttempted=true;
        // The real HeldInputs once-close cells preserve each original failure.
        // A pending Python original is retained before eligible cleanup.
        PyObject *pendingType=nullptr,*pendingValue=nullptr,*pendingTraceback=nullptr;
        PyErr_Fetch(&pendingType,&pendingValue,&pendingTraceback);
        if(pendingType && !failureType&&!failureValue&&!failureTraceback) {
            failureType=Py_XNewRef(pendingType);failureValue=Py_XNewRef(pendingValue);failureTraceback=Py_XNewRef(pendingTraceback);
        }
        if(returned && inputs && snapshot) {
            revalidationAttempted=true;
            revalidationMethod=PyObject_GetAttrString(inputs,"revalidate");
            if(revalidationMethod && !PyErr_Occurred())
                revalidationResult=PyObject_CallOneArg(revalidationMethod,snapshot);
            if(!revalidationResult && !PyErr_Occurred())
                PyErr_SetString(PyExc_SystemError,"bootstrap final revalidation missing actual return");
            revalidationReturned=revalidationResult==snapshot && !PyErr_Occurred();
            if(PyErr_Occurred())
                PyErr_Fetch(&revalidationType,&revalidationValue,&revalidationTraceback);
            // This independent original stays retained while clearing the error
            // indicator permits all already-eligible close attempts below.
        }
        closeMethod=PyObject_GetAttrString(held,"close");
        if(Clean(closeMethod)) {closeResult=PyObject_CallNoArgs(closeMethod);Clean(closeResult);}
        if(!PyErr_Occurred()) {
            failureMethod=PyObject_GetAttrString(held,"has_original_failures");
            if(Clean(failureMethod)) {failureResult=PyObject_CallNoArgs(failureMethod);Clean(failureResult);}
        }
        if(!PyErr_Occurred()) {
            unresolvedMethod=PyObject_GetAttrString(held,"has_unresolved_originals");
            if(Clean(unresolvedMethod)) {unresolvedResult=PyObject_CallNoArgs(unresolvedMethod);Clean(unresolvedResult);}
        }
        physicallyDisposed=closeResult && unresolvedResult==Py_False;
        disposalReturned=physicallyDisposed && revalidationReturned && failureResult==Py_False && !PyErr_Occurred();
        // Retain this independent cleanup triple even when capture already failed.
        if(PyErr_Occurred()) {
            PyErr_Fetch(&cleanupType,&cleanupValue,&cleanupTraceback);
            PyErr_Restore(Py_XNewRef(cleanupType),Py_XNewRef(cleanupValue),Py_XNewRef(cleanupTraceback));
        }
        if(pendingType || pendingValue || pendingTraceback)PyErr_Restore(pendingType,pendingValue,pendingTraceback);
        else if(revalidationType || revalidationValue || revalidationTraceback)
            PyErr_Restore(Py_XNewRef(revalidationType),Py_XNewRef(revalidationValue),Py_XNewRef(revalidationTraceback));
        return physicallyDisposed;
    }
    // None is the original bootstrap policy, not a new timer. Actual binary,
    // compiler/CRT/loader, input membership and native gate remain separately
    // admitted. This capture owns real held inputs; host must join their eligible
    // final revalidation/disposal instead of dropping this owner after binding.
};
}

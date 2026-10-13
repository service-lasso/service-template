// UNCOMPILED actual fixed source-route producer on the original Python thread.
// Original arguments select inputs/schedules; they are not native authority.
#pragma once
#include "native_session_core.hpp"
namespace template_creator_bridge {
struct OriginalWindowsRouteProducer {
    PyObject *arguments=nullptr,*module=nullptr,*function=nullptr,*result=nullptr;
    PyObject *phaseClass=nullptr,*phase=nullptr,*steps=nullptr;
    PyObject *failureType=nullptr,*failureValue=nullptr,*failureTraceback=nullptr;
    bool attempted=false,callAttempted=false,returned=false;
    std::exception_ptr nativeOriginal;
private:
    ~OriginalWindowsRouteProducer()=default;
    friend struct OriginalWindowsNativeMain;
    void RetainFailure() noexcept {
        if(PyErr_Occurred() && !failureType && !failureValue && !failureTraceback) {
            PyErr_Fetch(&failureType,&failureValue,&failureTraceback);
            Py_XINCREF(failureType);Py_XINCREF(failureValue);Py_XINCREF(failureTraceback);
            PyErr_Restore(failureType,failureValue,failureTraceback);
        }
    }
public:
    bool Produce(OriginalPhase selected,PyObject* exactOriginalArguments) noexcept {
        if(attempted)return false;
        attempted=true;arguments=Py_XNewRef(exactOriginalArguments);
        if(PyErr_Occurred()) {RetainFailure();return false;}
        const char* name=selected==OriginalPhase::Generation?"generation":
            selected==OriginalPhase::FinalQualification?"final_qualification":nullptr;
        const char* phaseName=selected==OriginalPhase::Generation?"GENERATION":
            selected==OriginalPhase::FinalQualification?"FINAL_QUALIFICATION":nullptr;
        const Py_ssize_t arity=selected==OriginalPhase::Generation?5:7;
        if(!name || !arguments || !PyTuple_CheckExact(arguments) || PyTuple_GET_SIZE(arguments)!=arity) {
            PyErr_SetString(PyExc_ValueError,"exact original protected route arguments required");RetainFailure();return false;
        }
        try {
            module=PyImport_ImportModule("selected_route");
            if(module&&!PyErr_Occurred())function=PyObject_GetAttrString(module,name);
            if(function&&!PyErr_Occurred()) {
                callAttempted=true;result=PyObject_CallObject(function,arguments);
            }
            // Attach every actual return before considering a pending error.
            if(result&&!PyErr_Occurred())phaseClass=PyObject_GetAttrString(module,"RoutePhase");
            if(phaseClass&&!PyErr_Occurred())phase=PyObject_GetAttrString(phaseClass,phaseName);
            if(!PyErr_Occurred() && (!result || !PyTuple_CheckExact(result) || PyTuple_GET_SIZE(result)!=2 ||
                PyTuple_GET_ITEM(result,0)!=phase || !PyTuple_CheckExact(PyTuple_GET_ITEM(result,1)) ||
                PyTuple_GET_SIZE(PyTuple_GET_ITEM(result,1))!=(selected==OriginalPhase::Generation?1:2)))
                PyErr_SetString(PyExc_ValueError,"source protected producer returned inconsistent original route");
            if(PyErr_Occurred()) {RetainFailure();return false;}
            steps=Py_NewRef(PyTuple_GET_ITEM(result,1));returned=true;return true;
        } catch(...) {
            nativeOriginal=std::current_exception();
            if(!PyErr_Occurred())PyErr_SetString(PyExc_RuntimeError,"original source route producer failed");
            RetainFailure();return false;
        }
    }
    // Native-helper source/compiler argv has its own selected producer; it is
    // never invented by this fixed generation/final-qualification producer.
};
}

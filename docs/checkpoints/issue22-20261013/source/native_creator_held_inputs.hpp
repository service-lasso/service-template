// UNCOMPILED retained source entry field identity; not input/native authority.
#pragma once
#include <Python.h>
#include <cstddef>
namespace template_creator_bridge {
struct OriginalCreatorHeldInputs {
    PyObject* entry=nullptr;
    PyObject* dictionary=nullptr;
    PyObject* tuple=nullptr;
    PyObject* inputs=nullptr;
    bool attempted=false,captured=false,inputsCaptured=false,replacementObserved=false;
    int originalLookupResult=0;
    bool lookupAttempted=false,lookupReturned=false;
    int inputsLookupResult=0;
    bool inputsLookupAttempted=false,inputsLookupReturned=false;
    bool Capture(PyObject* originalEntry) noexcept {
        if(attempted)return false;attempted=true;entry=Py_XNewRef(originalEntry);
        if(!entry){if(!PyErr_Occurred())PyErr_SetString(PyExc_ValueError,"original creator entry absent");return false;}
        dictionary=PyObject_GenericGetDict(entry,nullptr); // Retain actual return before classification.
        if(!dictionary||PyErr_Occurred())return false;
        if(!PyDict_CheckExact(dictionary)){
            PyErr_SetString(PyExc_TypeError,"original creator dictionary required");return false;
        }
        lookupAttempted=true;originalLookupResult=PyDict_GetItemStringRef(dictionary,"_held",&tuple);lookupReturned=true;
        if(originalLookupResult<0||PyErr_Occurred())return false;
        if(originalLookupResult!=1||!tuple||!PyTuple_CheckExact(tuple)){
            if(!PyErr_Occurred())PyErr_SetString(PyExc_TypeError,"original creator held tuple required");return false;
        }
        captured=true; // Independently retained held membership survives later failure.
        inputsLookupAttempted=true;
        inputsLookupResult=PyDict_GetItemStringRef(dictionary,"_inputs",&inputs);
        inputsLookupReturned=true;
        if(inputsLookupResult<0||PyErr_Occurred())return false;
        if(inputsLookupResult!=1||!inputs||!PyTuple_CheckExact(inputs)||
           PyTuple_GET_SIZE(inputs)!=PyTuple_GET_SIZE(tuple)){
            if(!PyErr_Occurred())PyErr_SetString(PyExc_TypeError,"original creator input tuple required");return false;
        }
        inputsCaptured=true;return true;
    }
    bool Contains(std::size_t ordinal,PyObject* original) const noexcept {
        return captured&&original&&ordinal<static_cast<std::size_t>(PyTuple_GET_SIZE(tuple))&&
            PyTuple_GET_ITEM(tuple,static_cast<Py_ssize_t>(ordinal))==original;
    }
    bool CurrentFieldIsOriginal() noexcept {
        if(!captured)return false;
        auto* current=PyObject_GenericGetDict(entry,nullptr);
        PyObject* currentTuple=nullptr;
        const int found=current&&!PyErr_Occurred()&&PyDict_CheckExact(current)?
            PyDict_GetItemStringRef(current,"_held",&currentTuple):0;
        const bool same=current&&!PyErr_Occurred()&&current==dictionary&&found==1&&currentTuple==tuple;
        Py_XDECREF(currentTuple);
        Py_XDECREF(current);replacementObserved|=!same;return same;
    }
    // Registry/core owns this cell and all strong references through actual
    // native custody. No destructor decrefs under an unknown interpreter state.
    // CPython >=3.13 strong/error-preserving dictionary API:
    // https://docs.python.org/3.14/c-api/dict.html#c.PyDict_GetItemStringRef
};
}

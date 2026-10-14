// UNCOMPILED original-thread host invocation. This does not initialize/finalize
// Python, authenticate source inputs, or authorize host exit on a failed run.
#include "native_session_core.hpp"
#ifdef _WIN32
#include "native_windows_platform.hpp"
#else
#include "native_linux_platform.hpp"
#endif
namespace template_creator_bridge {
PyObject* NativeSessionCore::RunRetainedCreatorEntry(PyObject* originalEntry) {
    auto& row = creatorInvocation_;
    if (row.attempted || !originalEntry || !OnOriginalPythonThread() || !OnCreatingNativeThread()) {
        if (!PyErr_Occurred()) PyErr_SetString(PyExc_RuntimeError,"same uninvoked retained creator host required");
        return nullptr;
    }
    row.attempted = true;
    row.entry = Py_NewRef(originalEntry); // Before lookup, execution or failure.
    if (platform_) {
        platform_->creatorEntryOriginal=Py_NewRef(row.entry);
        platform_->creatorHeldInputs=&row.heldInputs;
    }
    try {
        if(!row.heldInputs.Capture(row.entry)&&!PyErr_Occurred())
            PyErr_SetString(PyExc_SystemError,"original creator held capture failed without original error");
        if(!PyErr_Occurred())row.method = PyObject_GetAttrString(row.entry,"run_private");
        if (row.method && (!PyMethod_Check(row.method) || PyMethod_GET_SELF(row.method)!=row.entry)) {
            if (!PyErr_Occurred()) PyErr_SetString(PyExc_TypeError,"original bound creator entry method required");
        }
        if (row.method && !PyErr_Occurred()) {
            row.callAttempted = true;
            row.result = PyObject_CallNoArgs(row.method);
            row.returned = true;
            if (row.result && PyErr_Occurred()) {
                // Keep both the returned object and pending original triple.
            } else if (!row.result && !PyErr_Occurred()) {
                PyErr_SetString(PyExc_SystemError,"creator returned null without an original Python error");
            }
        }
    } catch (...) {
        row.nativeException = std::current_exception();
        if (!PyErr_Occurred()) PyErr_SetString(PyExc_RuntimeError,"native creator invocation exception");
    }
    if (PyErr_Occurred()) {
        PyErr_Fetch(&row.pythonType,&row.pythonValue,&row.pythonTraceback);
        Py_XINCREF(row.pythonType); Py_XINCREF(row.pythonValue); Py_XINCREF(row.pythonTraceback);
        PyErr_Restore(row.pythonType,row.pythonValue,row.pythonTraceback);
        return nullptr;
    }
    return Py_XNewRef(row.result);
    // Neither a Python outcome nor an exception permits interpreter/thread exit.
    // The native host must consume actual physical custody/collector outcomes.
}
}

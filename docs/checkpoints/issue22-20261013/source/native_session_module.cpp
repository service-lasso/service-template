// UNCOMPILED source-owned CPython type and closed native operation dispatch.
// Not a runnable module until native_session_core platform bodies are connected.
#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <cstring>
#include "native_session_core.hpp"
namespace template_creator_bridge {
struct SessionObject { PyObject_HEAD NativeSessionCore* retainedOriginal; };
static PyTypeObject SessionType = {PyVarObject_HEAD_INIT(nullptr, 0)};
static void DestroyWrapper(PyObject* self) noexcept {
    // No native close, kill, detach or owner deletion from Python finalization.
    reinterpret_cast<SessionObject*>(self)->retainedOriginal = nullptr;
    Py_TYPE(self)->tp_free(self);
}
static NativeSessionCore* Require(PyObject* value) {
    if (Py_TYPE(value) != &SessionType) {
        PyErr_SetString(PyExc_TypeError, "original source-owned native session required"); return nullptr;
    }
    auto* core = reinterpret_cast<SessionObject*>(value)->retainedOriginal;
    if (!core || !core->OnOriginalPythonThread() || !core->OnCreatingNativeThread()) {
        PyErr_SetString(PyExc_RuntimeError, "original native thread custody required"); return nullptr;
    }
    return core;
}
PyObject* WrapRetainedOriginal(NativeSessionCore& original) {
    if (PyErr_Occurred()) return nullptr; // Preserve a native factory's original pending triple.
    if (!(SessionType.tp_flags & Py_TPFLAGS_READY)) {
        PyErr_SetString(PyExc_RuntimeError, "native source module not initialized"); return nullptr;
    }
    if (!original.OnOriginalPythonThread() || !original.OnCreatingNativeThread()) {
        PyErr_SetString(PyExc_RuntimeError, "native factory thread mismatch"); return nullptr;
    }
    auto* value = PyObject_New(SessionObject, &SessionType);
    if (!value) return nullptr;
    value->retainedOriginal = &original;
    return reinterpret_cast<PyObject*>(value);
}
static PyObject* RequireOriginal(PyObject*, PyObject* value) {
    if (!Require(value)) return nullptr;
    Py_RETURN_NONE;
}
struct OperationName { const char* name; Operation operation; Py_ssize_t arity; };
static constexpr OperationName names[] = {
    {"adopt-original-ingress", Operation::AdoptIngress, 3},
    {"precharge-complete-route", Operation::PrechargeRoute, 1},
    {"capture-input-classes", Operation::CaptureInputClasses, 2},
    {"bind-initial-inputs", Operation::BindInputs, 2},
    {"prepare-original-sink", Operation::PrepareSink, 0},
    {"preowned-command", Operation::PreownedCommand, 1},
    {"begin-original", Operation::Begin, 2},
    {"step-original", Operation::Step, 1},
    {"capture-to-original-sink", Operation::CaptureCommand, 1},
    {"original-command-disposed", Operation::CommandDisposed, 1},
    {"verify-original-command-result", Operation::VerifyCommand, 2},
    {"dispose-independently-eligible", Operation::DisposeEligible, 1},
    {"verify-input-disposal-eligible", Operation::InputDisposalEligible, 2},
    {"capture-creator-originals", Operation::CaptureCreator, 2},
    {"flush-readback-original-sink", Operation::ReadbackSink, 0},
    {"verify-complete-native-closure", Operation::VerifyClosure, 0}
};
static PyObject* InvokeOriginal(PyObject*, PyObject* args) {
    PyObject *object, *name, *originals;
    if (!PyArg_UnpackTuple(args, "invoke_original", 3, 3, &object, &name, &originals)) return nullptr;
    NativeSessionCore* core = Require(object);
    if (!core) return nullptr;
    if (!PyUnicode_CheckExact(name) || !PyTuple_CheckExact(originals)) {
        PyErr_SetString(PyExc_TypeError, "closed operation and exact original tuple required"); return nullptr;
    }
    Py_ssize_t length = 0;
    const char* encoded = PyUnicode_AsUTF8AndSize(name, &length);
    if (!encoded) return nullptr;
    const OperationName* selected = nullptr;
    for (const auto& candidate : names)
        if (length == static_cast<Py_ssize_t>(strlen(candidate.name)) &&
            memcmp(encoded, candidate.name, static_cast<size_t>(length)) == 0) { selected = &candidate; break; }
    if (!selected || PyTuple_GET_SIZE(originals) != selected->arity) {
        PyErr_SetString(PyExc_ValueError, "unknown native operation or original arity"); return nullptr;
    }
    OriginalDispatch* row = core->ReserveDispatch(selected->operation, originals);
    if (!row) {
        PyErr_SetString(PyExc_RuntimeError, "native original dispatch capacity unavailable"); return nullptr;
    }
    // The row and strong argument references exist before platform effects.
    row->attempted = true;
    PyObject* result = nullptr;
    try { result = core->Invoke(selected->operation, originals); }
    catch (...) {
        row->nativeException = std::current_exception();
        // A platform body may have raised Python and C++ failures together.
        // Preserve its pending triple; synthesize only when no Python original exists.
        if (!PyErr_Occurred())
            PyErr_SetString(PyExc_RuntimeError, "retained original native dispatch failure");
    }
    if (result && PyErr_Occurred()) {
        // Preserve the invalid mixed native/Python outcome, never mask its error.
        row->returned = true; row->result = result; result = nullptr;
    }
    if (!result) {
        if (!PyErr_Occurred()) PyErr_SetString(PyExc_RuntimeError, "native dispatch returned no original result");
        PyErr_Fetch(&row->pythonType, &row->pythonValue, &row->pythonTraceback);
        // Row retains its own originals; restoring takes separate references.
        Py_XINCREF(row->pythonType); Py_XINCREF(row->pythonValue); Py_XINCREF(row->pythonTraceback);
        PyErr_Restore(row->pythonType, row->pythonValue, row->pythonTraceback);
        return nullptr;
    }
    row->returned = true;
    row->result = result; Py_INCREF(result);
    return result;
}
static PyMethodDef methods[] = {
    {"require_original_session", RequireOriginal, METH_O, nullptr},
    {"invoke_original", InvokeOriginal, METH_VARARGS, nullptr},
    {nullptr, nullptr, 0, nullptr}
};
static PyModuleDef definition = {PyModuleDef_HEAD_INIT, "template_creator_native", nullptr, -1, methods};
}
PyMODINIT_FUNC PyInit_template_creator_native() {
    using namespace template_creator_bridge;
    SessionType.tp_name = "template_creator_native.OriginalSession";
    SessionType.tp_basicsize = sizeof(SessionObject);
    SessionType.tp_flags = Py_TPFLAGS_DEFAULT; // No subclassing or public constructor.
    SessionType.tp_dealloc = DestroyWrapper;
    if (PyType_Ready(&SessionType) < 0) return nullptr;
    // Type is intentionally not exported; only the direct native factory wraps.
    return PyModule_Create(&definition);
}

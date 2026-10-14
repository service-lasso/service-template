// UNCOMPILED actual private identity/phase and original command-route binding.
// Successful binding acknowledges captured structure, not native role authority.
#ifdef _WIN32
#include "native_windows_platform.hpp"
namespace template_creator_bridge {
static bool Literal(PyObject* original,const char* expected) noexcept {
    if (!original || !PyUnicode_CheckExact(original)) return false;
    size_t length = 0; while (expected[length]) ++length;
    if (PyUnicode_GET_LENGTH(original) != static_cast<Py_ssize_t>(length)) return false;
    for (size_t i=0;i<length;++i) if (PyUnicode_READ_CHAR(original,static_cast<Py_ssize_t>(i)) != static_cast<unsigned char>(expected[i])) return false;
    return true;
}
static PyObject* BindingError(NativePlatformState& state,const char* message) noexcept {
    state.qualificationFailed = true;
    if (!PyErr_Occurred()) PyErr_SetString(PyExc_ValueError,message); return nullptr;
}
PyObject* NativePlatformState::AdoptIngress(PyObject* original,PyObject* phase,PyObject* roles) {
    if (adoptAttempted) return BindingError(*this,"original ingress adoption already attempted");
    adoptAttempted = true;
    phaseOriginal = Py_XNewRef(phase); rolesOriginal = Py_XNewRef(roles);
    if (!sourceObjectsAssociated || !sourceCapabilitiesOriginal || !sourceMainOriginal ||
        !ingressBound || !ingress.Captured() || !original || original != originalIngressIdentity)
        return BindingError(*this,"exact captured private ingress required");
    const char* selected = originalPhase == OriginalPhase::Generation ? "separately-admitted-generation" :
        originalPhase == OriginalPhase::FinalQualification ? "separately-admitted-final-qualification" :
        originalPhase == OriginalPhase::NativeHelper ? "separately-admitted-native-helper" : nullptr;
    if (!selected || !Literal(phase,selected) || !roles || !PyTuple_CheckExact(roles) || PyTuple_GET_SIZE(roles)!=4)
        return BindingError(*this,"original native phase/role roster mismatch");
    const char* expected[] = {"original-caller","input-fixture-evidence-root","original-tools-compiler-libraries","observer-native-capacity"};
    for (Py_ssize_t i=0;i<4;++i) if (!Literal(PyTuple_GET_ITEM(roles,i),expected[i]))
        return BindingError(*this,"original role relationship mismatch");
    adopted = true;
    // Role availability remains UNKNOWN in the frontend. Capture and phase
    // equality are not root/tool/capacity qualification or launch permission.
    Py_RETURN_NONE;
}
static bool Argv(PyObject* step,const char* const* expected,Py_ssize_t count) noexcept {
    if (!step || !PyTuple_Check(step) || PyTuple_GET_SIZE(step)!=5) return false;
    PyObject* command = PyTuple_GET_ITEM(step,1);
    if (!command || !PyTuple_Check(command) || PyTuple_GET_SIZE(command)!=6) return false;
    PyObject* argv = PyTuple_GET_ITEM(command,1);
    if (!argv || !PyTuple_CheckExact(argv) || PyTuple_GET_SIZE(argv)!=count || PyTuple_GET_ITEM(command,4)!=Py_None) return false;
    for (Py_ssize_t i=0;i<count;++i) if (!Literal(PyTuple_GET_ITEM(argv,i),expected[i])) return false;
    return true;
}
PyObject* NativePlatformState::PrechargeRoute(PyObject* steps) {
    if (prechargeAttempted) return BindingError(*this,"original route precharge already attempted");
    prechargeAttempted = true; stepsOriginal = Py_XNewRef(steps);
    if (!adopted || qualificationFailed || !steps || !PyTuple_CheckExact(steps) ||
        PyTuple_GET_SIZE(steps)!=static_cast<Py_ssize_t>(route.SelectedCount()-1))
        return BindingError(*this,"complete original selected route required");
    if (originalPhase == OriginalPhase::Generation) {
        const char* expected[] = {"scripts/generate-template-contract.mjs"};
        if (PyTuple_GET_SIZE(steps)!=1 || !Argv(PyTuple_GET_ITEM(steps,0),expected,1))
            return BindingError(*this,"original generation route mismatch");
    } else if (originalPhase == OriginalPhase::FinalQualification) {
        const char* suites[] = {"--test","tests/template-contract.test.mjs","tests/development-candidate-workflow.test.mjs","tests/qualification-publication.test.mjs"};
        const char* validator[] = {"scripts/validate-development-candidate-workflow.mjs",".github/workflows/development-candidate.yml"};
        if (PyTuple_GET_SIZE(steps)!=2 || !Argv(PyTuple_GET_ITEM(steps,0),suites,4) || !Argv(PyTuple_GET_ITEM(steps,1),validator,2))
            return BindingError(*this,"unchanged protected route mismatch");
    }
    // Native-helper argv must be bound to actual selected compiler/generated
    // source/tool inputs by BindInputs and LaunchAssociationsEstablished.
    stepsBound = true; Py_RETURN_NONE;
}
PyObject* NativePlatformState::VerifyCommand(PyObject* owner,PyObject* exactStep) {
    if (!stepsBound || qualificationFailed) return BindingError(*this,"original selected steps not bound");
    for (Py_ssize_t i=0;i<PyTuple_GET_SIZE(stepsOriginal);++i) {
        if (PyTuple_GET_ITEM(stepsOriginal,i)==exactStep && commands.PrivateOriginal(static_cast<SIZE_T>(i)).token==owner)
            return commands.VerifyZeroResult(owner);
    }
    return BindingError(*this,"exact original command/step association required");
}
PyObject* NativePlatformState::PrepareSink() {
    if (!adopted || qualificationFailed || !ingress.Captured()) return BindingError(*this,"captured original sink ingress required");
    const auto& capability = ingress.OriginalCapabilities()[4];
    if (capability.closeAttempted || !sink.AdoptBorrowedOriginal(capability.original))
        return BindingError(*this,"actual original native sink adoption failed");
    Py_RETURN_NONE;
}
}
#endif

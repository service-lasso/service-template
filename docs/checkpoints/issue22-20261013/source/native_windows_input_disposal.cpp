// UNCOMPILED concrete Python-held input eligibility. Membership is taken from
// the retained original entry's actual dictionary, without an application
// callback/property lookup or a caller-supplied disposal Boolean. This does not
// establish tool/root/source authority or successful qualification.
#ifdef _WIN32
#include "native_windows_platform.hpp"
namespace template_creator_bridge {
PyObject* NativePlatformState::InputDisposalEligible(PyObject* ordinal,PyObject* held) {
    if (!ordinal || !PyLong_CheckExact(ordinal) || !held || !creatorEntryOriginal || !creatorHeldInputs) {
        qualificationFailed=true;
        if (!PyErr_Occurred()) PyErr_SetString(PyExc_ValueError,"original retained input owner required");
        return nullptr;
    }
    const auto index=PyLong_AsSize_t(ordinal);
    if (PyErr_Occurred()) { qualificationFailed=true; return nullptr; }
    const bool associated=creatorHeldInputs->entry==creatorEntryOriginal&&creatorHeldInputs->Contains(index,held);
    // A changed current field fails qualification but cannot replace original
    // membership or suppress an independently eligible original member's close.
    if(!creatorHeldInputs->CurrentFieldIsOriginal())qualificationFailed=true;
    if(PyErr_Occurred())return nullptr;
    // Before input binding/launch, no command has acquired a child. After any
    // launch every acquired command must really be disposed. Collector reads
    // depend on its private sink/copies, but an active collector is conservatively
    // retained here; no input exclusion is released under unknown IO.
    const bool eligible=associated && commands.AllPhysicalCommandsDisposed() && sink.CompletionKnown() &&
        (!collectorStartAttempted || CollectorPhysicalDisposed());
    if (!eligible) {
        qualificationFailed=true;
        if (!PyErr_Occurred()) PyErr_SetString(PyExc_RuntimeError,"original input disposal not independently eligible");
        return nullptr;
    }
    Py_RETURN_NONE; // Allows this exact held owner to attempt its once-only closes.
}
}
#endif

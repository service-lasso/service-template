// UNCOMPILED connected original-thread Python entry construction. These imports
// and all their loader/preload/extension inputs need the same admitted ROOT;
// module/class names and retained references are not supply-chain authority.
#ifdef _WIN32
#include "windows_standalone_host.hpp"
namespace template_creator_bridge {
PyObject* OriginalWindowsStandaloneHost::ConstructCreatorEntry(PyObject* actualSelectedSteps) {
    if (entryConstructionAttempted || !wrapper || !core || !platform || !transferred ||
        !SourceChannelAssociationEstablished(*platform) ||
        !core->OnOriginalPythonThread() || !actualSelectedSteps || !PyTuple_CheckExact(actualSelectedSteps)) {
        if (!PyErr_Occurred()) PyErr_SetString(PyExc_RuntimeError,"original connected host entry selection required");
        return nullptr;
    }
    entryConstructionAttempted=true;
    stepsOriginal=Py_NewRef(actualSelectedSteps);
    // Every actual lookup/call output stays in this resident host before later
    // Python work; failure originals are retained below without stringification.
    bridgeModule=PyImport_ImportModule("native_session");
    if (bridgeModule && !PyErr_Occurred()) bridgeClass=PyObject_GetAttrString(bridgeModule,"NativeSession");
    if (bridgeClass && !PyErr_Occurred()) bridgeOriginal=PyObject_CallOneArg(bridgeClass,wrapper);
    if (bridgeOriginal && !PyErr_Occurred()) creatorModule=PyImport_ImportModule("creator_entry");
    if (creatorModule && !PyErr_Occurred()) creatorClass=PyObject_GetAttrString(creatorModule,"TemplateCreator");
    if (creatorClass && !PyErr_Occurred()) phaseClass=PyObject_GetAttrString(creatorModule,"RoutePhase");
    const char* selectedPhase=selected.phase==OriginalPhase::Generation ? "GENERATION" :
        selected.phase==OriginalPhase::FinalQualification ? "FINAL_QUALIFICATION" :
        selected.phase==OriginalPhase::NativeHelper ? "NATIVE_HELPER" : nullptr;
    if (phaseClass && !PyErr_Occurred() && selectedPhase) phaseOriginal=PyObject_GetAttrString(phaseClass,selectedPhase);
    if (phaseOriginal && !PyErr_Occurred()) entryOriginal=PyObject_CallFunctionObjArgs(creatorClass,
        platform->originalIngressIdentity,bridgeOriginal,phaseOriginal,stepsOriginal,nullptr);
    if (!entryOriginal && !PyErr_Occurred()) PyErr_SetString(PyExc_SystemError,"host entry construction returned no original");
    if (PyErr_Occurred()) {
        PyErr_Fetch(&constructionType,&constructionValue,&constructionTraceback);
        Py_XINCREF(constructionType); Py_XINCREF(constructionValue); Py_XINCREF(constructionTraceback);
        PyErr_Restore(constructionType,constructionValue,constructionTraceback);
        return nullptr;
    }
    return Py_NewRef(entryOriginal);
}
}
#endif

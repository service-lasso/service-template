// UNCOMPILED actual factory/wrapper/creator/closure caller. No main exit grant.
#ifndef _WIN32
#include "linux_standalone_host.hpp"
#include <cstdint>
namespace template_creator_bridge {
void OriginalLinuxStandaloneHost::RetainError(PyObject*& type,PyObject*& value,PyObject*& traceback) noexcept {
    if(type||value||traceback||!PyErr_Occurred())return;
    PyErr_Fetch(&type,&value,&traceback);Py_XINCREF(type);Py_XINCREF(value);Py_XINCREF(traceback);
    PyErr_Restore(type,value,traceback);
}
bool OriginalLinuxStandaloneHost::Connect(const OriginalLinuxHostSelection& original){
    if(connectAttempted)return false;connectAttempted=true;originalSelection=original;
    try{
        if((original.bodyBytes&&!original.body)||(original.capabilityCount&&!original.capabilities)||
           !original.inputReadRows||!original.inputCounts||!original.commands||
           original.bodyBytes>template_creator_linux::OriginalPrivateIngress::BodyCapacity||
           original.capabilityCount>template_creator_linux::OriginalPrivateIngress::FdCapacity||
           original.capabilityCount>PTRDIFF_MAX/sizeof(int)||original.commands>PTRDIFF_MAX/sizeof(std::size_t)||
           (original.phase==OriginalPhase::Generation&&original.commands!=1)||
           (original.phase==OriginalPhase::FinalQualification&&original.commands!=2)){
            if(!PyErr_Occurred())PyErr_SetString(PyExc_ValueError,"complete original Linux host selection required");
            RetainError(connectType,connectValue,connectTraceback);return false;
        }
        // Entire geometry/content copy precedes PreownSession's gettid and
        // platform native acquisition. Copied integers are borrowed caller
        // capabilities, not transferred ownership or native role permission.
        if(original.bodyBytes)bodyCopy_.assign(original.body,original.body+original.bodyBytes);
        if(original.capabilityCount)capabilityCopy_.assign(original.capabilities,original.capabilities+original.capabilityCount);
        readCopy_.resize(original.commands);readPointers_.resize(original.commands);countsCopy_.resize(original.commands);
        for(std::size_t i=0;i<original.commands;++i){
            const auto count=original.inputCounts[i];
            if(count>PTRDIFF_MAX/sizeof(std::size_t)||(count&&!original.inputReadRows[i]))throw std::invalid_argument("original Linux read geometry");
            countsCopy_[i]=count;
            if(count)readCopy_[i].assign(original.inputReadRows[i],original.inputReadRows[i]+count);
            readPointers_[i]=readCopy_[i].data();
        }
        selectionCopied=true;
        core=registry_.PreownSession(original.dispatchRows,original.hostObservations);
        if(!core)return false;
        platform=registry_.PreownLinuxPlatform(*core,original.phase,original.commands,original.sinkBytes,original.sinkCalls,
            original.geometry,original.hasDeadline,original.absoluteNanoseconds,original.callerStack,original.callerJoins,
            bodyCopy_.data(),bodyCopy_.size(),capabilityCopy_.data(),capabilityCopy_.size(),original.procRoot,
            original.sinkOrdinal,original.sourceRootOrdinal,original.toolOrdinal,original.evidenceRootOrdinal,
            readPointers_.data(),countsCopy_.data(),original.preInterpreterHost,original.selectionOriginal);
        if(!platform||PyErr_Occurred()||!platform->captureReturned||!platform->captureResult){
            RetainError(connectType,connectValue,connectTraceback);return false;
        }
        wrapper=WrapRetainedOriginal(*core); // Retain before classification.
        if(!wrapper||PyErr_Occurred()){RetainError(connectType,connectValue,connectTraceback);return false;}
        connected=true;return true;
    }catch(...){
        connectException=std::current_exception();
        if(core&&!platform)platform=core->PrivatePlatformOriginal(); // Published partial factory owner.
        RetainError(connectType,connectValue,connectTraceback);return false;
    }
}
PyObject* OriginalLinuxStandaloneHost::ConstructCreatorEntry(PyObject* steps){
    if(constructionAttempted||!connected||!core||!platform||!wrapper||
       !core->OnOriginalPythonThread()||!core->OnCreatingNativeThread()||!steps||!PyTuple_CheckExact(steps)){
        if(!PyErr_Occurred())PyErr_SetString(PyExc_RuntimeError,"same original Linux connected host required");return nullptr;
    }
    constructionAttempted=true;stepsOriginal=Py_NewRef(steps);
    bridgeModule=PyImport_ImportModule("native_session");
    if(bridgeModule&&!PyErr_Occurred())bridgeClass=PyObject_GetAttrString(bridgeModule,"NativeSession");
    if(bridgeClass&&!PyErr_Occurred())bridgeOriginal=PyObject_CallOneArg(bridgeClass,wrapper);
    if(bridgeOriginal&&!PyErr_Occurred())creatorModule=PyImport_ImportModule("creator_entry");
    if(creatorModule&&!PyErr_Occurred())creatorClass=PyObject_GetAttrString(creatorModule,"TemplateCreator");
    if(creatorClass&&!PyErr_Occurred())phaseClass=PyObject_GetAttrString(creatorModule,"RoutePhase");
    const char* phase=originalSelection.phase==OriginalPhase::Generation?"GENERATION":
        originalSelection.phase==OriginalPhase::FinalQualification?"FINAL_QUALIFICATION":
        originalSelection.phase==OriginalPhase::NativeHelper?"NATIVE_HELPER":nullptr;
    if(phaseClass&&!PyErr_Occurred()&&phase)phaseOriginal=PyObject_GetAttrString(phaseClass,phase);
    if(phaseOriginal&&!PyErr_Occurred())entryOriginal=PyObject_CallFunctionObjArgs(creatorClass,
        platform->originalIngressIdentity,bridgeOriginal,phaseOriginal,stepsOriginal,nullptr);
    if(!entryOriginal&&!PyErr_Occurred())PyErr_SetString(PyExc_SystemError,"Linux creator construction returned no original");
    if(PyErr_Occurred()){RetainError(constructionType,constructionValue,constructionTraceback);return nullptr;}
    return Py_NewRef(entryOriginal);
}
PyObject* OriginalLinuxStandaloneHost::InvokeCreator(){
    if(invokeAttempted||!entryOriginal||!core||PyErr_Occurred())return nullptr;
    invokeAttempted=true;invocationResult=core->RunRetainedCreatorEntry(entryOriginal);
    if(!invocationResult||PyErr_Occurred())return nullptr;
    return Py_NewRef(invocationResult);
}
}
#endif

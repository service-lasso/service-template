// UNCOMPILED native-only resident factory; no Python numeric capability input.
#ifndef _WIN32
#include "native_session_registry.hpp"
#include "native_linux_platform.hpp"
namespace template_creator_bridge {
NativePlatformState* RetainedNativeRegistry::PreownLinuxPlatform(NativeSessionCore& original,OriginalPhase phase,
    std::size_t selectedCommands,std::size_t sinkBytes,std::size_t sinkCalls,
    const template_creator_linux::PendingInputGeometry& geometry,bool hasOriginalDeadline,
    std::uint64_t originalAbsoluteNanoseconds,std::size_t callerStack,std::size_t callerJoins,
    const unsigned char* originalBody,std::size_t originalBodyBytes,const int* originalCapabilities,
    std::size_t originalCapabilityCount,int actualProcRoot,std::size_t actualSinkOrdinal,
    std::size_t actualRootOrdinal,std::size_t actualToolOrdinal,std::size_t actualEvidenceOrdinal,
    const std::size_t* const* inputReadRows,const std::size_t* inputCounts,
    template_creator_linux::OriginalLinuxPendingHost* adoptedHost,
    template_creator_linux::OriginalLinuxHeldFile* selectionOriginal) {
    if(!Owns(&original)||original.platformAttachmentAttempted_)return nullptr;
    original.platformAttachmentAttempted_=true;
    if((phase!=OriginalPhase::Generation&&phase!=OriginalPhase::FinalQualification&&phase!=OriginalPhase::NativeHelper)||
       !original.OnOriginalPythonThread()||!original.OnCreatingNativeThread())return nullptr;
    std::size_t ordinal=0;while(ordinal<count_&&sessions_[ordinal]!=&original)++ordinal;
    if(ordinal==count_)return nullptr;
    // HostChannel prepends its actual self pidfd at delivered ordinal0.
    if(originalCapabilityCount>=template_creator_linux::OriginalPrivateIngress::FdCapacity)return nullptr;
    if(bool(adoptedHost)!=bool(selectionOriginal))return nullptr;
    if(adoptedHost){
        auto& pending=adoptedHost->OriginalPendingOwner();
        if(!selectionOriginal->CapturedSourceBytes()||selectionOriginal->OriginalDeadlineOwner()!=&pending.OriginalDeadlineOwner()||
           selectionOriginal->OriginalSupervisorOwner()!=&pending.OriginalSupervisorOwner()||
           !adoptedHost->SameOriginalConstruction(geometry,hasOriginalDeadline,originalAbsoluteNanoseconds,callerStack,callerJoins)||
           !adoptedHost->SameOriginalPublication(originalBody,originalBodyBytes,originalCapabilities,originalCapabilityCount,actualProcRoot)||
           !adoptedHost->ReservePlatformAdoption())return nullptr;
        // Reservation precedes constructors/effects. If construction throws,
        // no platform owns this host: the enclosing startup owner retains it.
    }
    NativePlatformState* state=nullptr;
    try {
        // Default source host starts below; an adopted original is already
        // active. Never move/copy its native owners during platform allocation.
        state=new NativePlatformState(phase,selectedCommands,sinkBytes,sinkCalls,geometry,
            hasOriginalDeadline,originalAbsoluteNanoseconds,callerStack,callerJoins,adoptedHost);
    }catch(...){original.platformAttachmentOriginal_=std::current_exception();throw;}
    platforms_[ordinal]=state;original.platform_=state;
    if(adoptedHost){
        state->externalPendingHostOwned=adoptedHost->CompleteReservedPlatformAdoption(state);
        state->selectionOriginal=selectionOriginal;
        if(!state->externalPendingHostOwned){state->qualificationFailed=true;return state;}
    }
    if(!state->PreownStorageOriginal())return state;
    state->originalProcRoot=actualProcRoot;
    state->callerPidfdOrdinal=0;state->sinkOrdinal=actualSinkOrdinal;
    try {
        if(!state->initialObjects.SelectOriginalOrdinals(
            actualRootOrdinal,actualToolOrdinal,actualEvidenceOrdinal,actualSinkOrdinal,
            state->callerPidfdOrdinal,originalCapabilityCount+1)) {
            state->qualificationFailed=true;
            if(!PyErr_Occurred())PyErr_SetString(PyExc_ValueError,"original Linux object ordinals invalid");
            return state;
        }
        // Actual source host selects finite per-file read capacities, not trust
        // predicates. Copy ALL layouts before source channel/thread effects.
        if(!inputReadRows||!inputCounts){
            state->qualificationFailed=true;
            if(!PyErr_Occurred())PyErr_SetString(PyExc_ValueError,"original Linux input layouts absent");
            return state;
        }
        for(std::size_t i=0;i<selectedCommands;++i)
            if(!state->PreownInputLayout(i,inputReadRows[i],inputCounts[i]))return state;
        state->transferAttempted=true;
        state->captureResult=adoptedHost?
            adoptedHost->BeganOriginal()&&!adoptedHost->Failed():
            state->pendingHost.BeginOriginal(originalBody,originalBodyBytes,
                originalCapabilities,originalCapabilityCount,actualProcRoot);
        state->captureReturned=true;
    }catch(...){state->originalCaptureException=std::current_exception();state->qualificationFailed=true;throw;}
    if(!state->captureResult)state->qualificationFailed=true;
    // BeginOriginal retains actual startup rows in the SAME pending host. Do
    // not call StartOriginalObservers a second time or infer partial attempts
    // from this aggregate return. Read those native owners for final closure.
    state->originalIngressIdentity=PyObject_CallNoArgs(reinterpret_cast<PyObject*>(&PyBaseObject_Type));
    if(!state->originalIngressIdentity||PyErr_Occurred()){
        state->qualificationFailed=true;
        PyErr_Fetch(&original.platformPythonType_,&original.platformPythonValue_,&original.platformPythonTraceback_);
        if(!original.platformPythonType_) {
            PyErr_SetString(PyExc_RuntimeError,"original Linux ingress identity construction failed");
            PyErr_Fetch(&original.platformPythonType_,&original.platformPythonValue_,&original.platformPythonTraceback_);
        }
        Py_XINCREF(original.platformPythonType_);Py_XINCREF(original.platformPythonValue_);Py_XINCREF(original.platformPythonTraceback_);
        PyErr_Restore(original.platformPythonType_,original.platformPythonValue_,original.platformPythonTraceback_);
    }
    return state; // Captured credentials/fds never imply source/native admission.
}
}
#endif

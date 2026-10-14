// UNCOMPILED closed phase/route composition. Structure is not native authority.
#ifndef _WIN32
#include "native_linux_platform.hpp"
#include "linux_session_gate.hpp"
namespace template_creator_bridge {
static bool RetainedReceivedSubjects(const LinuxInputOwner& owner,OriginalPhase phase,size_t command) noexcept {
    if(!owner.initialProviderOriginal||!owner.receivedSubjectAssociations||!owner.files||
       !owner.snapshot||!PyTuple_CheckExact(owner.snapshot)||PyTuple_GET_SIZE(owner.snapshot)!=5)return false;
    auto* requirements=PyTuple_GET_ITEM(owner.snapshot,4);
    if(!PyTuple_CheckExact(requirements)||PyTuple_GET_SIZE(requirements)!=static_cast<Py_ssize_t>(owner.count))return false;
    const auto* archive=owner.initialProviderOriginal->OriginalArchiveOwner();
    if(!archive)return false;
    for(size_t i=0;i<owner.count;++i){
        const auto& row=owner.receivedSubjectAssociations[i];const auto* cut=row.cut;
        if(owner.selectedReadRows&&owner.selectedReadRows[i]==0){
            const auto* pair=owner.receiptFoundationPairOriginal;
            const auto* source=pair?pair->SourceOriginal():nullptr;
            if(command!=0||i>1||!pair||!source||pair->ReceiverOriginal()!=owner.initialProviderOriginal||
               !slcli_bq_provider_expectations_bootstrap_shape(source,static_cast<uint32_t>(phase))||
               !pair->NativeCorrespondenceObserved()||row.receiptFoundationPairOriginal!=pair||
               !row.attempted||row.result!=LinuxReceivedSubjectAssociationOriginal::Result::MatchedNativeArchiveCorrespondence||
               row.receiver!=owner.initialProviderOriginal||row.archive!=archive||cut||row.heldFile||row.sourceUnit||owner.files[i]||
               row.command!=command||row.requirement!=i||row.requirementOriginal!=PyTuple_GET_ITEM(requirements,i)||
               !row.nativeRequirementDictionaryOriginal||!row.finalClock.shapeValid||row.finalClock.expired||
               row.finalClock.nanoseconds<pair->FinalCorrespondenceClockOriginal().nanoseconds)return false;
            continue;
        }
        const bool kind=cut&&cut->unit?
            (row.result==LinuxReceivedSubjectAssociationOriginal::Result::MatchedNativeUnitIdentity&&
             row.sourceUnit==cut->unit&&row.heldFile&&row.heldFile->OriginalCaptureMode()==
                template_creator_linux::OriginalLinuxHeldFile::CaptureMode::SourceUnitIdentity):
            (row.result==LinuxReceivedSubjectAssociationOriginal::Result::Matched&&!row.sourceUnit);
        if(!row.attempted||!kind||
           row.receiver!=owner.initialProviderOriginal||row.archive!=archive||!cut||cut->owner!=archive||
           cut!=archive->OriginalCut(cut->ordinal)||!cut->offer||
           cut->offer!=owner.initialProviderOriginal->OriginalOffer(cut->ordinal)||
           cut->offer->sourceRoute.phase!=static_cast<uint32_t>(phase)||
           cut->offer->sourceRoute.command!=command||cut->offer->sourceRoute.requirement!=i||
           row.command!=command||row.requirement!=i||row.heldFile!=owner.files[i].get()||
           row.requirementOriginal!=PyTuple_GET_ITEM(requirements,i)||!row.finalClock.shapeValid||
           row.finalClock.expired||row.finalClock.nanoseconds<cut->finalClock.nanoseconds)return false;
    }
    return true; // Retained correspondence only; no current open-file or authority claim.
}
static bool LinuxLiteral(PyObject* original,const char* expected) noexcept {
    if(!original||!PyUnicode_CheckExact(original))return false;
    std::size_t n=0;while(expected[n])++n;
    if(PyUnicode_GET_LENGTH(original)!=static_cast<Py_ssize_t>(n))return false;
    for(std::size_t i=0;i<n;++i)
        if(PyUnicode_READ_CHAR(original,static_cast<Py_ssize_t>(i))!=static_cast<unsigned char>(expected[i]))return false;
    return true;
}
static PyObject* LinuxBindingError(NativePlatformState& owner,const char* message) noexcept {
    owner.qualificationFailed=true;if(!PyErr_Occurred())PyErr_SetString(PyExc_RuntimeError,message);return nullptr;
}
PyObject* NativePlatformState::AdoptIngress(PyObject* original,PyObject* phase,PyObject* roles){
    if(adoptAttempted)return LinuxBindingError(*this,"original Linux adoption already attempted");
    adoptAttempted=true;originalPhaseObject=Py_XNewRef(phase);originalRoles=Py_XNewRef(roles);
    if(!captureReturned||!captureResult||!original||original!=originalIngressIdentity||
       !pending.OriginalIngressOwner().Captured()||pendingHost.Failed())
        return LinuxBindingError(*this,"same captured Linux private ingress required");
    const char* selected=originalPhase==OriginalPhase::Generation?"separately-admitted-generation":
        originalPhase==OriginalPhase::FinalQualification?"separately-admitted-final-qualification":
        originalPhase==OriginalPhase::NativeHelper?"separately-admitted-native-helper":nullptr;
    if(!selected||!LinuxLiteral(phase,selected)||!roles||!PyTuple_CheckExact(roles)||PyTuple_GET_SIZE(roles)!=4)
        return LinuxBindingError(*this,"same original Linux phase/role roster required");
    const char* expected[]={"original-caller","input-fixture-evidence-root","original-tools-compiler-libraries","observer-native-capacity"};
    for(Py_ssize_t i=0;i<4;++i)if(!LinuxLiteral(PyTuple_GET_ITEM(roles,i),expected[i]))
        return LinuxBindingError(*this,"Linux original role relationship mismatch");
    adopted=true;Py_RETURN_NONE; // No role availability or launch grant.
}
static bool LinuxArgv(PyObject* step,const char* const* expected,Py_ssize_t count) noexcept {
    if(!step||!PyTuple_Check(step)||PyTuple_GET_SIZE(step)!=5)return false;
    auto* command=PyTuple_GET_ITEM(step,1);
    if(!command||!PyTuple_Check(command)||PyTuple_GET_SIZE(command)!=6)return false;
    auto* argv=PyTuple_GET_ITEM(command,1);
    if(!argv||!PyTuple_CheckExact(argv)||PyTuple_GET_SIZE(argv)!=count||PyTuple_GET_ITEM(command,4)!=Py_None)return false;
    for(Py_ssize_t i=0;i<count;++i)if(!LinuxLiteral(PyTuple_GET_ITEM(argv,i),expected[i]))return false;
    return true;
}
PyObject* NativePlatformState::PrechargeRoute(PyObject* steps){
    if(prechargeAttempted)return LinuxBindingError(*this,"original Linux route already selected");
    prechargeAttempted=true;originalSteps=Py_XNewRef(steps);
    if(!adopted||qualificationFailed||!steps||!PyTuple_CheckExact(steps)||
       PyTuple_GET_SIZE(steps)!=static_cast<Py_ssize_t>(route.CommandCount()))
        return LinuxBindingError(*this,"complete original Linux step roster required");
    if(originalPhase==OriginalPhase::Generation){
        const char* expected[]={"scripts/generate-template-contract.mjs"};
        if(PyTuple_GET_SIZE(steps)!=1||!LinuxArgv(PyTuple_GET_ITEM(steps,0),expected,1))
            return LinuxBindingError(*this,"unchanged Linux generation route required");
    }else if(originalPhase==OriginalPhase::FinalQualification){
        const char* suites[]={"--test","tests/template-contract.test.mjs","tests/development-candidate-workflow.test.mjs","tests/qualification-publication.test.mjs"};
        const char* validator[]={"scripts/validate-development-candidate-workflow.mjs",".github/workflows/development-candidate.yml"};
        if(PyTuple_GET_SIZE(steps)!=2||!LinuxArgv(PyTuple_GET_ITEM(steps,0),suites,4)||
           !LinuxArgv(PyTuple_GET_ITEM(steps,1),validator,2))
            return LinuxBindingError(*this,"unchanged Linux three-suite and validator route required");
    }
    stepsBound=true;Py_RETURN_NONE;
}
PyObject* NativePlatformState::PrepareSink(){
    if(sinkPrepareAttempted)return LinuxBindingError(*this,"original Linux sink already attempted");
    sinkPrepareAttempted=true;const auto& ingress=pending.OriginalIngressOwner();
    if(!adopted||!stepsBound||qualificationFailed||!captureReturned||!captureResult||pendingHost.Failed()||
       inputDisposalStarted||!inputCapturesClean||!PrelaunchOriginalAssociationsEstablished()||
       sinkOrdinal==callerPidfdOrdinal||sinkOrdinal>=ingress.OriginalDescriptorCount())
        return LinuxBindingError(*this,"same selected Linux private sink descriptor required");
    const auto& descriptor=ingress.OriginalDescriptors()[sinkOrdinal];
    if(descriptor.original<0||descriptor.statResult!=0||descriptor.closeAttempted)
        return LinuxBindingError(*this,"original Linux sink custody unavailable");
    if(!sink.AdoptOriginal(descriptor.original))return LinuxBindingError(*this,"original Linux sink adoption failed");
    for(std::size_t ordinal=0;ordinal<route.CommandCount();++ordinal){
        const auto& owner=inputOwners[ordinal];
        for(std::size_t i=0;i<owner.count;++i){
            if(owner.selectedReadRows&&owner.selectedReadRows[i]==0){
                if(!owner.receiptFoundationPairOriginal||
                   owner.receiptFoundationPairOriginal!=pendingHost.ReceiptFoundationPairOriginal()||
                   !sink.ExcludeNativeReceiptPairOriginal(*owner.receiptFoundationPairOriginal))
                    return LinuxBindingError(*this,"original Linux sink aliases or cannot exclude native archive pair");
                continue;
            }
            if(!owner.files[i]||!sink.ExcludeCapturedInputOriginal(*owner.files[i]))
                return LinuxBindingError(*this,"original Linux sink aliases or cannot exclude protected input");
        }
    }
    sinkPrepared=true;
    Py_RETURN_NONE;
}
PyObject* NativePlatformState::CaptureCommand(PyObject* token){
    if(!sinkPrepared||creatorCaptureAttempted)return LinuxBindingError(*this,"original Linux command capture phase closed");
    PyObject* result=commands.CaptureOriginalCommand(token,sink);
    if(!result||PyErr_Occurred())return result;
    for(std::size_t i=0;i<route.CommandCount();++i)if(commands.PrivateOriginal(i).originalToken==token){
        if(!inputOwners[i].gate||!static_cast<OriginalLinuxSessionGate*>(inputOwners[i].gate.get())->CaptureOriginals(sink)){
            Py_DECREF(result);return LinuxBindingError(*this,"original Linux gate capture failed");
        }
        return result;
    }
    Py_DECREF(result);return LinuxBindingError(*this,"same Linux gate command token required");
}
PyObject* NativePlatformState::VerifyCommand(PyObject* token,PyObject* step){
    if(!stepsBound||qualificationFailed||!originalSteps)return LinuxBindingError(*this,"original Linux route not bound");
    for(Py_ssize_t i=0;i<PyTuple_GET_SIZE(originalSteps);++i)
        if(PyTuple_GET_ITEM(originalSteps,i)==step&&commands.PrivateOriginal(static_cast<std::size_t>(i)).originalToken==token)
            return commands.VerifyZeroResult(token);
    return LinuxBindingError(*this,"same Linux original command/step pair required");
}
PyObject* NativePlatformState::DisposeEligible(PyObject* token){
    qualificationFailed=true;return commands.DisposeIndependentlyEligible(token);
}
bool NativePlatformState::RetainPrelaunchOriginalAssociations() noexcept {
    if(!inputCapturesClean||qualificationFailed||!captureReturned||!captureResult||!adopted||!stepsBound||
       !pendingHost.BeganOriginal()||pendingHost.Failed()||!initialObjects.Captured()||
       !protectedStdin.Qualified()||!originalSnapshots||!inputOwners||
       (externalPendingHost?(!externalPendingHostOwned||!pendingHost.IsOriginalPlatformOwner(this)):
           (&pendingHost!=&pendingHostStorage)))return false;
    for(size_t i=0;i<route.CommandCount();++i){
        auto& owner=inputOwners[i];auto& row=owner.prelaunchOriginal;
        if(row.attempted)return false;row.attempted=true;
        row.commandOriginal=&route.PrivateCommand(i);row.snapshotOriginal=owner.snapshot;
        row.callerHostOriginal=&pendingHost;row.rootOriginal=&initialObjects.PrivateOriginals()[0];
        row.evidenceOriginal=&initialObjects.PrivateOriginals()[2];
        row.toolOrdinal=owner.launchFileOrdinal;row.cwdOriginal=owner.cwd.get();
        if(row.toolOrdinal<owner.count&&owner.files)row.toolOriginal=owner.files[row.toolOrdinal].get();
        row.associated=RetainedReceivedSubjects(owner,originalPhase,i)&&
            owner.sourceAssociated&&owner.captureClean&&owner.launchFileAssociated&&
            owner.gate&&row.toolOriginal&&row.cwdOriginal&&row.cwdOriginal->Captured()&&
            initialObjects.SameCapturedTool(*row.toolOriginal)&&
            row.commandOriginal->plan.SameSnapshot(row.snapshotOriginal)&&
            commands.PrivateOriginal(i).originalSnapshot==row.snapshotOriginal&&
            commands.PrivateOriginal(i).originalToken;
        if(!row.associated)return false;
    }
    return true;
}
bool NativePlatformState::PrelaunchOriginalAssociationsEstablished() const noexcept {
    if(!inputCapturesClean||!captureReturned||!captureResult||!adopted||!stepsBound||
       !inputOwners||!route.CommandCount())return false;
    for(size_t i=0;i<route.CommandCount();++i){
        const auto& owner=inputOwners[i];const auto& row=owner.prelaunchOriginal;
        if(!RetainedReceivedSubjects(owner,originalPhase,i))return false;
        if(!row.attempted||!row.associated||row.commandOriginal!=&route.PrivateCommand(i)||
           row.callerHostOriginal!=&pendingHost||row.rootOriginal!=&initialObjects.PrivateOriginals()[0]||
           row.evidenceOriginal!=&initialObjects.PrivateOriginals()[2]||
           row.snapshotOriginal!=owner.snapshot||row.toolOrdinal!=owner.launchFileOrdinal||
           row.toolOrdinal>=owner.count||!owner.files||row.toolOriginal!=owner.files[row.toolOrdinal].get()||
           row.cwdOriginal!=owner.cwd.get()||commands.PrivateOriginal(i).originalSnapshot!=row.snapshotOriginal)
            return false;
    }
    return true; // Retained source/native association, not consumed execution.
}
bool NativePlatformState::ExecutedOriginalCommandsQualified() const noexcept {
    if(!PrelaunchOriginalAssociationsEstablished()||!commands.QualificationComplete()||
       !commands.AllPhysicalCommandsDisposed())return false;
    for(size_t i=0;i<route.CommandCount();++i){
        const auto& owner=inputOwners[i];const auto& command=route.PrivateCommand(i);
        if(!owner.gate||!command.driver||
           !static_cast<const OriginalLinuxSessionGate*>(owner.gate.get())->ActualExecutionQualificationComplete(
               command.driver->PrivateOriginals()))return false;
    }
    return true;
}
bool NativePlatformState::ReservedPlatformSourceBytes(std::size_t& bytes) const noexcept {
    bytes=sizeof(*this);std::size_t selected=0;
    const auto add=[&](std::size_t whole,std::size_t embedded) noexcept {
        if(whole<embedded||whole-embedded>SIZE_MAX-bytes)return false;
        bytes+=whole-embedded;return true;
    };
    if(allocatedInputOwnerCapacity> (SIZE_MAX-bytes)/sizeof(LinuxInputOwner))return false;
    bytes+=allocatedInputOwnerCapacity*sizeof(LinuxInputOwner);
    for(std::size_t ordinal=0;ordinal<allocatedInputOwnerCapacity;++ordinal){
        const auto& owner=inputOwners[ordinal];
        if(owner.kernelExeCandidate){
            const auto charge=owner.kernelExeCandidate->ReservedSourceBytes();
            if(charge>SIZE_MAX-bytes)return false;bytes+=charge;
        }
        if(owner.gate){
            std::size_t charge=0;if(!owner.gate->ReservedSourceBytes(charge)||charge>SIZE_MAX-bytes)return false;bytes+=charge;
        }
        if(owner.cwd){
            const auto charge=owner.cwd->ReservedSourceBytes();
            if(charge>SIZE_MAX-bytes)return false;bytes+=charge;
        }
        const std::size_t per=(owner.selectedReadRows?sizeof(std::size_t):0)+
            (owner.interpreterObservations?sizeof(LinuxInterpreterRequirementOriginal):0)+
            (owner.receivedSubjectAssociations?sizeof(LinuxReceivedSubjectAssociationOriginal):0)+
            (owner.files?sizeof(std::unique_ptr<template_creator_linux::OriginalLinuxHeldFile>):0);
        if(per&&owner.count>(SIZE_MAX-bytes)/per)return false;bytes+=owner.count*per;
        for(std::size_t i=0;i<owner.count;++i)if(owner.files&&owner.files[i]){
            const auto charge=owner.files[i]->ReservedSourceBytes();
            if(charge>SIZE_MAX-bytes)return false;bytes+=charge;
        }
    }
    if(!externalPendingHost&&(!pendingHostStorage.ReservedSourceBytes(selected)||!add(selected,sizeof(pendingHostStorage))))return false;
    // Adopted route has no fallback backing/native acquisitions; its inline
    // dormant operand storage is already included in sizeof(*this).
    if(externalPendingHostOwned&&(!pendingHost.ReservedSourceBytes(selected)||!add(selected,0)))return false;
    if(selectionOriginal&&externalPendingHostOwned){const auto count=selectionOriginal->ReservedSourceBytes();if(count>SIZE_MAX-bytes)return false;bytes+=count;}
    return
        route.ReservedSourceBytes(selected)&&add(selected,sizeof(route))&&
        add(commands.ReservedSourceBytes(),sizeof(commands))&&sink.ReservedSourceBytes(selected)&&add(selected,sizeof(sink))&&
        collector.ReservedSourceBytes(selected)&&add(selected,sizeof(collector))&&
        finalOutput.ReservedSourceBytes(selected)&&add(selected,sizeof(finalOutput));
    // SAME adopted host/selection are owned here after transfer; enclosing
    // startup totals must not charge them twice. Excludes Python referents,
    // allocator/TLS/kernel/loaded runtime; this is never a whole-fit certificate.
}
}
#endif

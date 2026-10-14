// UNCOMPILED connected physical disposition. Not final independent capture,
// interpreter exit authority, native admission or qualification success.
#ifndef _WIN32
#include "native_linux_platform.hpp"
#include "linux_initial_provider_archive.hpp"
#include "linux_private_selection_startup.hpp"
namespace template_creator_linux {
bool OriginalInitialProviderArchive::DisposeAfterUsefulInputCutOriginal(
    const template_creator_bridge::NativePlatformState& platform) noexcept {
    if(usefulInputCutOriginal_&&usefulInputCutOriginal_!=&platform)return false;
    if(usefulInputCutOriginal_==&platform&&NativeBorrowersSettledOriginal())return true;
    if(&platform.pending.OriginalDeadlineOwner()!=&deadline_||
       &platform.pending.OriginalSupervisorOwner()!=&supervisor_||
       !platform.inputDisposalStarted||!platform.nativeInputDisposalResult||
       !platform.commands.AllPhysicalCommandsDisposed()||!platform.sink.AllAttemptedOriginalCallsReturned()||
       supervisor_.HasPendingOriginalNativeCall()||!platform.protectedStdin.PhysicalDisposed())return false;
    if(!platform.route.CommandCount()||receiver_.count_!=capacity_||count_!=capacity_)return false;
    if(platform.allocatedInputOwnerCapacity!=platform.route.CommandCount())return false;
    for(size_t i=0;i<platform.allocatedInputOwnerCapacity;++i)
        if(platform.inputOwners[i].initialProviderOriginal!=&receiver_)return false;
    for(size_t i=0;i<count_;++i){
        const auto& cut=cuts_[i];
        if(cut.owner!=this||cut.offer!=receiver_.OriginalOffer(i)||!cut.prefixRetained||
           (i==0?!cut.process:(!cut.file&&!cut.unit))||(cut.file&&cut.unit))return false;
    }
    usefulInputCutOriginal_=&platform;usefulDisposalAttempted_=true;
    // All actual command/input FD borrowers have settled at the SAME platform
    // cut. Resident archive bytes remain borrowed by final publication.
    for(size_t i=1;i<count_;++i){
        if(cuts_[i].unitPayload&&!cuts_[i].unitPayload->ReleaseBeforeExposureOriginal())return false;
        if(cuts_[i].unit){if(!cuts_[i].unit->ReleaseBeforeExposureOriginal())return false;}
        else if(!cuts_[i].file->ReleaseBeforeSubjectExposureOriginal())return false;
    }
    if(!cuts_[0].process->
        DisposeBeforeSubjectExposureOriginal(supervisor_))return false;
    if(!receiver_.DisposeBeforeSubjectExposureOriginal())return false;
    return NativeBorrowersSettledOriginal();
}
}
namespace template_creator_bridge {
bool NativePlatformState::ObserveFailedPreownDispositionOriginal() noexcept {
    const auto frontier=FailedPreownSourceFrontierOriginal();
    if(frontier==FailedPreownHostFrontier::None)return false;
    if(failedPreownPhysicalDisposed)return failedPreownViewPublished;
    if(frontier==FailedPreownHostFrontier::AdoptedOriginalHost&&
       (!failedPreownStartupOriginal||!failedPreownStartupOriginal->FailedPreownBorrowersSettledOriginal(*this)))return false;
    if(!commands.AllPhysicalCommandsDisposed()||!sink.AllAttemptedOriginalCallsReturned()||
       pending.OriginalSupervisorOwner().HasPendingOriginalNativeCall())return false;
    failedPreownDispositionAttempted=true;inputDisposalStarted=true;hostDisposalAttempted=true;
    // Preown has never created these resources; no final-output/collector IO
    // or imaginary worker return is attempted. Adopted startup subjects have
    // independently settled before this actual supervisor shutdown frontier.
    if(!DisposeNativeInputOriginalsEligible()||!protectedStdin.DisposeAfterOriginalRouteEligible())return false;
    if(!hostObserverClosure){hostObserverClosure=pendingHost.ObserveEligibleObserverClosure();if(!hostObserverClosure)return false;}
    if(!ingressDisposalAttempted){ingressDisposalAttempted=true;ingressDisposalResult=pending.OriginalIngressOwner().DisposeEligible();}
    if(!ingressDisposalResult||pending.OriginalSupervisorOwner().HasPendingOriginalNativeCall())return false;
    RetainedPreownView view{};
    if(!ObserveRetainedPreownViewOriginal(view))return false;
    // Source view initializes the complete representation including padding.
    // This resident immutable cut is not an absent file/archive byte export.
    std::memcpy(&failedPreownRetainedView,&view,sizeof(view));failedPreownViewPublished=true;
    hostDisposalResult=true;failedPreownPhysicalDisposed=true;qualificationFailed=true;
    return true;
}
bool NativePlatformState::PrepareFinalPublicationInputCut() noexcept {
    inputDisposalStarted=true; // No future product command, even on refusal.
    if(finalPublicationInputCut)return true;
    if(externalPendingHost&&!externalPendingHostOwned)return false;
    if(!commands.AllPhysicalCommandsDisposed()||!sink.AllAttemptedOriginalCallsReturned()||
       pending.OriginalSupervisorOwner().HasPendingOriginalNativeCall())return false;
    if(collectorStartAttempted&&!collector.PollEligibleJoin())return false;
    const bool inputs=DisposeNativeInputOriginalsEligible();
    const bool stdinClosed=protectedStdin.DisposeAfterOriginalRouteEligible();
    if(!inputs||!stdinClosed||pending.OriginalSupervisorOwner().HasPendingOriginalNativeCall())return false;
    // Settle upstream FD borrowers before stopping the SAME supervisor. The
    // retained archive graph remains resident through final output readback.
    for(size_t i=0;i<allocatedInputOwnerCapacity;++i)if(inputOwners[i].initialProviderOriginal){
        auto* archive=inputOwners[i].initialProviderOriginal->archiveOriginal_;
        if(!archive||!archive->DisposeAfterUsefulInputCutOriginal(*this))return false;
    }
    finalPublicationInputCut=true;
    finalOutput.AdmitOwnedInputCut(); // Same actual owner and physical cut only.
    return true; // Original supervisor/caller/ingress still live.
}
bool NativePlatformState::ObservePhysicalHostClosureEligible() noexcept {
    if(FailedPreownSourceFrontierOriginal()!=FailedPreownHostFrontier::None)return ObserveFailedPreownDispositionOriginal();
    if(externalPendingHost&&!externalPendingHostOwned)return false;
    inputDisposalStarted=true;
    if(hostDisposalResult)return true;
    if(!commands.AllPhysicalCommandsDisposed()||!sink.AllAttemptedOriginalCallsReturned()||
       pending.OriginalSupervisorOwner().HasPendingOriginalNativeCall())return false;
    hostDisposalAttempted=true;
    if(!finalPublicationInputCut||!finalOutput.PhysicalDisposed())return false;
    if(collectorStartAttempted&&!collector.PollEligibleJoin())return false;
    // Each once-close original is retained by its actual owner. Failure in one
    // independently eligible input does not prevent later eligible closes.
    const bool inputsClosed=DisposeNativeInputOriginalsEligible();
    const bool stdinClosed=protectedStdin.DisposeAfterOriginalRouteEligible();
    if(!inputsClosed||!stdinClosed)return false; // Independent eligible closes all attempted.
    // Underlying source owners retain actual stop/publication/join native rows.
    // A false current progress result may be observed again; no uncertain
    // native close is retried by this aggregate source wrapper.
    if(!hostObserverClosure){
        hostObserverClosure=pendingHost.ObserveEligibleObserverClosure();
        if(!hostObserverClosure)return false;
    }
    // Receiver and caller no longer borrow ingress descriptors. This actual
    // final once-close can remain pending: retain entire resident platform and
    // interpreter/main; a prior notice is not a guaranteed second notice.
    if(!ingressDisposalAttempted){
        ingressDisposalAttempted=true;
        ingressDisposalResult=pending.OriginalIngressOwner().DisposeEligible();
    }
    if(!ingressDisposalResult)qualificationFailed=true;
    qualificationFailed|=pendingHost.Failed();
    hostDisposalResult=inputsClosed&&hostObserverClosure&&ingressDisposalResult;
    return hostDisposalResult;
}
bool NativePlatformState::ConsumeSettledHostOriginals() noexcept {
    if(!AllPhysicalOriginalsDisposed())return false;
    if(settledHostOutcomeObserved)return settledHostNormalOutcome;
    settledHostOutcomeObserved=true;
    settledHostNormalOutcome=(!externalPendingHost||
        (externalPendingHostOwned&&pendingHost.IsOriginalPlatformOwner(this)))&&
        pendingHost.SettledNormalOriginals()&&commands.QualificationComplete()&&ExecutedOriginalCommandsQualified()&&
        !qualificationFailed&&inputCapturesClean&&creatorCaptureAttempted&&
        creatorRecordAppended&&creatorPrefixReadback&&collectorStartReturned&&collector.Qualified()&&
        finalPublicationReturned&&!finalPublicationFailed&&finalOutput.Qualified();
    if(!settledHostNormalOutcome)qualificationFailed=true;
    return settledHostNormalOutcome;
}
bool NativePlatformState::AllPhysicalOriginalsDisposed() const noexcept {
    if(failedPreownDispositionAttempted)return failedPreownPhysicalDisposed&&failedPreownViewPublished&&
        FailedPreownSourceFrontierOriginal()!=FailedPreownHostFrontier::None&&hostDisposalAttempted&&hostDisposalResult&&
        nativeInputDisposalAttempted&&nativeInputDisposalResult&&hostObserverClosure&&ingressDisposalAttempted&&ingressDisposalResult&&
        commands.AllPhysicalCommandsDisposed()&&sink.AllAttemptedOriginalCallsReturned()&&protectedStdin.PhysicalDisposed()&&
        !pending.OriginalSupervisorOwner().HasPendingOriginalNativeCall();
    return hostDisposalAttempted&&hostDisposalResult&&nativeInputDisposalAttempted&&
        nativeInputDisposalResult&&hostObserverClosure&&ingressDisposalAttempted&&
        ingressDisposalResult&&finalPublicationInputCut&&finalOutput.PhysicalDisposed()&&commands.AllPhysicalCommandsDisposed()&&
        sink.AllAttemptedOriginalCallsReturned()&&collector.PhysicalDisposed()&&protectedStdin.PhysicalDisposed()&&
        !pending.OriginalSupervisorOwner().HasPendingOriginalNativeCall();
    // Physical-only. Genuine final private receiver/publication and original
    // Python-held owner settlement remain separate final host predicates.
}
}
#endif

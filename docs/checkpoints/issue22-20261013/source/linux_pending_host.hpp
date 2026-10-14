// UNCOMPILED connected native caller/creator response ownership. The complete
// creator platform, useful command route and input-role gate remain separate.
#pragma once
#include "linux_pending_input_owner.hpp"
#include "linux_caller_receipt.hpp"
#include "linux_host_wait_policy.hpp"
#include <cstring>
namespace template_creator_bridge {class OriginalLinuxSelectionStartup;}
namespace template_creator_linux {
class OriginalInitialProviderReceiver;
class OriginalReceiptFoundationPair;
class OriginalLinuxPrebirthRetentionPolicy;
class OriginalLinuxPendingHost {
    friend class template_creator_bridge::OriginalLinuxSelectionStartup;
    friend bool VerifySurvivingPendingHostPreownRegressionOriginal();
    OriginalInitialProviderReceiver* initialProviderOriginal_=nullptr;
    OriginalReceiptFoundationPair* receiptFoundationPairOriginal_=nullptr;
    OriginalLinuxPrebirthRetentionPolicy* retentionPolicyOriginal_=nullptr;
    bool BindInitialProviderBeforeBeginOriginal(OriginalInitialProviderReceiver* original) noexcept {
        if(!original||initialProviderOriginal_||beginAttempted_||dispositionRequested_)return false;
        initialProviderOriginal_=original;return true;
    }
    bool BindReceiptFoundationPairBeforeBeginOriginal(OriginalReceiptFoundationPair* original) noexcept {
        if(!original||!initialProviderOriginal_||receiptFoundationPairOriginal_||beginAttempted_||dispositionRequested_)return false;
        receiptFoundationPairOriginal_=original;return true;
    }
    bool BindRetentionPolicyBeforeBeginOriginal(OriginalLinuxPrebirthRetentionPolicy* original) noexcept {
        if(!original||!initialProviderOriginal_||!receiptFoundationPairOriginal_||retentionPolicyOriginal_||beginAttempted_||dispositionRequested_)return false;
        retentionPolicyOriginal_=original;return true;
    }
    OriginalLinuxPendingInputOwner pending_;
    OriginalHostWaitPolicy waitPolicy_;
    OriginalLinuxHostChannel channel_;
    OriginalLinuxCallerReceipt caller_;
    const PendingInputGeometry originalGeometry_;
    const bool originalHasDeadline_;
    const uint64_t originalAbsoluteNs_;
    const size_t originalCallerStack_,originalCallerJoins_;
    bool preownAttempted_=false,preownReturned_=false,preowned_=false;
    std::exception_ptr preownException_;
    bool adoptionReserved_=false;
    int originalProcRootArgument_=-1;
    template_creator_bridge::NativePlatformState* platformOwner_=nullptr;
    bool beginAttempted_=false,began_=false,failed_=false,observerClosure_=false;
    bool dispositionRequested_=false;
    ~OriginalLinuxPendingHost()=default;
public:
    OriginalLinuxPendingHost(DeferredOriginalPreownTag,const PendingInputGeometry& geometry,bool hasDeadline,uint64_t absoluteNs,
                            size_t callerStack,size_t callerJoins) noexcept
        :pending_(DeferredOriginalPreown,geometry,hasDeadline,absoluteNs),
         waitPolicy_(DeferredOriginalPreown,pending_.OriginalDeadlineOwner(),2),
         channel_(pending_.OriginalDeadlineOwner()),
         caller_(DeferredOriginalPreown,channel_,pending_.OriginalDeadlineOwner(),pending_.OriginalReceiverOwner(),callerStack,callerJoins),
         originalGeometry_(geometry),originalHasDeadline_(hasDeadline),originalAbsoluteNs_(absoluteNs),
         originalCallerStack_(callerStack),originalCallerJoins_(callerJoins) {}
    OriginalLinuxPendingHost(const PendingInputGeometry& geometry,bool hasDeadline,uint64_t absoluteNs,
                            size_t callerStack,size_t callerJoins)
        :OriginalLinuxPendingHost(DeferredOriginalPreown,geometry,hasDeadline,absoluteNs,callerStack,callerJoins){
        if(!PreownOriginal()&&preownException_)std::rethrow_exception(preownException_);
    }
    bool PreownOriginal() noexcept {
        if(preownAttempted_)return false;preownAttempted_=true;
        struct PublishReturn{bool& original;~PublishReturn(){original=true;}} publishReturn{preownReturned_};
        if(!pending_.PreownOriginal()){preownException_=pending_.PreownExceptionOriginal();failed_=true;return false;}
        if(!waitPolicy_.PreownOriginal()){preownException_=waitPolicy_.PreownExceptionOriginal();failed_=true;return false;}
        if(!caller_.PreownOriginal()){preownException_=caller_.PreownExceptionOriginal();failed_=true;return false;}
        preowned_=true;return true;
    }
    struct RetainedPreownBackingView {
        const OriginalLinuxPendingHost* hostOriginal=nullptr;
        const OriginalLinuxPendingInputOwner* pendingOriginal=nullptr;
        const OriginalDeadline* deadlineOriginal=nullptr;
        const HeldTaskIdentity* creatorTaskOriginal=nullptr;
        const OriginalDeadlineSupervisor* supervisorOriginal=nullptr;
        const OriginalDeadlineReceiver* receiverOriginal=nullptr;
        const OriginalLinuxCallerReceipt* callerOriginal=nullptr;
        const OriginalPidfdHandoffAssociation* handoffOriginal=nullptr;
        const std::exception_ptr* exceptionOriginal=nullptr;
        size_t clockCapacity=0,taskSlots=0,bornTasks=0,supervisorJoins=0,supervisorReturns=0;
        size_t callerTaskSlots=0,bornCallerTasks=0,fdinfoBytes=0,fdinfoReads=0;
        size_t receiverWaits=0,receiverJoins=0,receiverReturns=0,callerJoins=0,callerReturns=0,waitRows=0;
        bool attempted=false,returned=false,preowned=false;
    };
    bool ObserveRetainedPreownBackingOriginal(RetainedPreownBackingView& out) const noexcept {
        if(!preownAttempted_||!preownReturned_)return false;
        const auto destination=reinterpret_cast<uintptr_t>(&out),source=reinterpret_cast<uintptr_t>(this);
        if(destination>UINTPTR_MAX-sizeof(out)||source>UINTPTR_MAX-sizeof(*this)||
           (destination<source+sizeof(*this)&&source<destination+sizeof(out)))return false;
        std::memset(&out,0,sizeof(out));
        out.hostOriginal=this;out.pendingOriginal=&pending_;out.deadlineOriginal=&pending_.OriginalDeadlineOwner();
        out.creatorTaskOriginal=&pending_.creatorTask_;out.supervisorOriginal=&pending_.OriginalSupervisorOwner();
        out.receiverOriginal=&pending_.OriginalReceiverOwner();out.callerOriginal=&caller_;out.handoffOriginal=&pending_.caller_;out.exceptionOriginal=&preownException_;
        out.callerTaskSlots=pending_.caller_.OriginalCallerTask().AllocatedObservationCapacityOriginal();
        out.bornCallerTasks=pending_.caller_.OriginalCallerTask().BornObservationCountOriginal();
        const auto& fdinfo=pending_.caller_.OriginalFdinfo();
        out.fdinfoBytes=fdinfo.bytes?fdinfo.byteCapacity+1:0;out.fdinfoReads=fdinfo.readCapacity;
        out.clockCapacity=pending_.OriginalDeadlineOwner().OriginalCapacity();
        out.taskSlots=pending_.creatorTask_.AllocatedObservationCapacityOriginal();out.bornTasks=pending_.creatorTask_.BornObservationCountOriginal();
        out.supervisorJoins=pending_.OriginalSupervisorOwner().AllocatedJoinCapacityOriginal();
        out.supervisorReturns=pending_.OriginalSupervisorOwner().AllocatedJoinReturnCapacityOriginal();
        out.receiverWaits=pending_.OriginalReceiverOwner().AllocatedWaitCapacityOriginal();
        out.receiverJoins=pending_.OriginalReceiverOwner().AllocatedJoinCapacityOriginal();
        out.receiverReturns=pending_.OriginalReceiverOwner().AllocatedJoinReturnCapacityOriginal();
        out.callerJoins=caller_.AllocatedJoinCapacityOriginal();out.callerReturns=caller_.AllocatedJoinReturnCapacityOriginal();
        out.waitRows=waitPolicy_.AllocatedCapacityOriginal();out.attempted=preownAttempted_;out.returned=preownReturned_;out.preowned=preowned_;
        return out.bornTasks<=out.taskSlots&&out.bornCallerTasks<=out.callerTaskSlots;
    }
    bool PreownedOriginal() const noexcept{return preowned_;}
    const std::exception_ptr& PreownExceptionOriginal() const noexcept{return preownException_;}
    OriginalLinuxPendingHost(const OriginalLinuxPendingHost&)=delete;
    OriginalLinuxPendingHost& operator=(const OriginalLinuxPendingHost&)=delete;
    bool BeginOriginal(const unsigned char* selectedBody,size_t bodyBytes,
                       const int* originalCapabilities,size_t capabilityCount,int originalProcRoot) noexcept {
        if(!preowned_||dispositionRequested_||beginAttempted_)return false;beginAttempted_=true;
        originalProcRootArgument_=originalProcRoot;
        const auto* firstPolicy=waitPolicy_.ObserveOriginalMain();
        if(!firstPolicy||!waitPolicy_.SameObservedPolicy(*firstPolicy)){failed_=true;return false;}
        if(!channel_.ConstructOriginal()||
           !channel_.PublishOriginal(selectedBody,bodyBytes,originalCapabilities,capabilityCount)) {failed_=true;return false;}
        const int creatorSocket=channel_.TransferOriginalCreatorEndpoint();
        if(creatorSocket<0||!pending_.CaptureOriginal(creatorSocket,originalProcRoot,0)){failed_=true;return false;}
        // Original native caller receives on the paired endpoint before any
        // potentially blocking private-input call can start on creator thread.
        if(!caller_.StartOriginal()){failed_=true;return false;}
        if(!pending_.StartOriginalObservers()){failed_=true;return false;}
        const auto* lastPolicy=waitPolicy_.ObserveOriginalMain();
        if(!lastPolicy||!waitPolicy_.SameObservedPolicy(*lastPolicy)){failed_=true;return false;}
        began_=true;return true;
    }
    bool ObserveEligibleObserverClosure() noexcept {
        dispositionRequested_=true;
        // A pending original input syscall keeps all owners alive. Expiry still
        // travels independently supervisor -> receiver -> caller native thread.
        if(!pending_.StopOriginalObserversEligible())return false;
        if(!caller_.ObserveStopJoinEligible())return false;
        const bool callerDisposed=caller_.DisposeAfterOriginalReceiversEligible();
        const bool channelDisposed=channel_.DisposeUntransferredEligible();
        observerClosure_=callerDisposed&&channelDisposed;
        if(pending_.Failed()||caller_.Failed())failed_=true;
        return observerClosure_; // Physical-only; not input or command closure.
    }
    OriginalLinuxPendingInputOwner& OriginalPendingOwner() noexcept{return pending_;}
    const OriginalInitialProviderReceiver* InitialProviderOriginal() const noexcept{return initialProviderOriginal_;}
    const OriginalReceiptFoundationPair* ReceiptFoundationPairOriginal() const noexcept{return receiptFoundationPairOriginal_;}
    OriginalReceiptFoundationPair* ReceiptFoundationPairOriginal() noexcept{return receiptFoundationPairOriginal_;}
    const OriginalLinuxPrebirthRetentionPolicy* RetentionPolicyOriginal() const noexcept{return retentionPolicyOriginal_;}
    OriginalLinuxPrebirthRetentionPolicy* RetentionPolicyOriginal() noexcept{return retentionPolicyOriginal_;}
    const OriginalLinuxPendingInputOwner& OriginalPendingOwner() const noexcept{return pending_;}
    const OriginalLinuxCallerReceipt& OriginalCallerReceipt() const noexcept{return caller_;}
    bool OriginalDeadlineNoticeReceivedByCaller() const noexcept{return caller_.OriginalNoticeReceived();}
    bool Failed() const noexcept{return failed_||pending_.Failed()||caller_.Failed();}
    bool SettledNormalOriginals() const noexcept {
        // observerClosure_ is published only by the concrete actual join/close
        // route above. Returned worker bodies alone never establish settlement.
        return began_&&dispositionRequested_&&observerClosure_&&!Failed()&&
            pending_.ObserversPhysicalDisposed()&&
            pending_.OriginalSupervisorOwner().PhysicalDisposed()&&
            pending_.OriginalSupervisorOwner().WorkerOriginalsPublished()&&
            !pending_.OriginalSupervisorOwner().HasPendingOriginalNativeCall()&&
            pending_.OriginalReceiverOwner().PhysicalDisposed()&&
            pending_.OriginalReceiverOwner().OriginalReceiverReturned()&&
            pending_.OriginalReceiverOwner().OriginalNormalStopReceived()&&
            caller_.OriginalCallerOriginalsPublished()&&
            pending_.OriginalDeadlineOwner().QualificationClean();
    }
    bool SameOriginalConstruction(const PendingInputGeometry& g,bool deadline,uint64_t absolute,
                                 size_t stack,size_t joins) const noexcept {
        const auto& o=originalGeometry_;
        return g.clockRows==o.clockRows&&g.taskObservations==o.taskObservations&&g.statBytes==o.statBytes&&
            g.statusBytes==o.statusBytes&&g.procReads==o.procReads&&g.fdinfoBytes==o.fdinfoBytes&&
            g.supervisorStack==o.supervisorStack&&g.supervisorJoins==o.supervisorJoins&&
            g.receiverStack==o.receiverStack&&g.receiverWaits==o.receiverWaits&&g.receiverJoins==o.receiverJoins&&
            g.mapsBytes==o.mapsBytes&&g.mapsReads==o.mapsReads&&g.memoryRegions==o.memoryRegions&&
            g.memoryBytes==o.memoryBytes&&g.memoryReads==o.memoryReads&&
            deadline==originalHasDeadline_&&absolute==originalAbsoluteNs_&&stack==originalCallerStack_&&joins==originalCallerJoins_;
    }
    bool ReservePlatformAdoption() noexcept {
        if(dispositionRequested_||adoptionReserved_||platformOwner_||!began_||Failed())return false;
        adoptionReserved_=true;return true;
    }
    bool CompleteReservedPlatformAdoption(template_creator_bridge::NativePlatformState* owner) noexcept {
        if(!owner||!adoptionReserved_||platformOwner_)return false;
        platformOwner_=owner;return true;
    }
    bool IsOriginalPlatformOwner(const template_creator_bridge::NativePlatformState* owner) const noexcept {
        return owner&&platformOwner_==owner;
    }
    bool HasOriginalPlatformOwner() const noexcept{return platformOwner_!=nullptr;}
    bool BeginAttemptedOriginal() const noexcept{return beginAttempted_;}
    bool BeganOriginal() const noexcept{return began_;}
    bool SameOriginalPublication(const unsigned char* body,size_t bytes,const int* descriptors,
                                size_t count,int procRoot) const noexcept {
        if(!began_||procRoot!=originalProcRootArgument_||!body||bytes!=channel_.bodyBytes_||
           count==SIZE_MAX||count+1!=channel_.descriptorCount_||(count&&!descriptors)||
           std::memcmp(body,channel_.selectedBody_.data(),bytes))return false;
        for(size_t i=0;i<count;++i)if(descriptors[i]!=channel_.selectedDescriptors_[i+1])return false;
        return true; // SAME source originals, not native rights/role admission.
    }
    bool ReservedSourceBytes(size_t& bytes) const noexcept {
        bytes=sizeof(*this);size_t n=0;
        auto add=[&](size_t whole,size_t embedded){if(whole<embedded||whole-embedded>SIZE_MAX-bytes)return false;bytes+=whole-embedded;return true;};
        if(!pending_.ReservedSourceBytes(n)||!add(n,sizeof(pending_)))return false;
        if(!caller_.ReservedSourceBytes(n)||!add(n,sizeof(caller_)))return false;
        n=waitPolicy_.ReservedSourceBytes();if(!add(n,sizeof(waitPolicy_)))return false;
        return true; // Full creator/runtime/kernel/native inputs additional.
    }
    friend class OriginalLinuxPlatformState;
    friend struct template_creator_bridge::NativePlatformState;
    friend class template_creator_bridge::OriginalLinuxSelectionStartup;
};
}

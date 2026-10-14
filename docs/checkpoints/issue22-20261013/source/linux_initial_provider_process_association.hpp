// UNCOMPILED kernel process correspondence only; no initial principal/source grant.
#pragma once
#ifndef _WIN32
#include "linux_upstream_subject_receiver.hpp"
#include "linux_pidfd_identity.hpp"
namespace template_creator_bridge {class OriginalLinuxSelectionStartup;}
namespace template_creator_linux {
class OriginalInitialProviderProcessAssociation {
    friend class template_creator_bridge::OriginalLinuxSelectionStartup;
    friend class OriginalInitialProviderReceiver;
    friend class OriginalInitialProviderArchive;
    friend bool VerifySurvivingPendingHostPreownRegressionOriginal();
    const OriginalInitialProviderReceiver& receiver_;
    size_t sourceBytes_=0;
    size_t observations_,statBytes_,statusBytes_,reads_,fdinfoBytes_;
    bool preownAttempted_=false,preownReturned_=false,preowned_=false;
    std::exception_ptr preownException_;
    HeldTaskIdentity callerTask_;
    ProcOwnedFd fdinfoDirectory_;
    ProcFileObservation fdinfo_;
    PidfdPollOriginal before_,after_;
    OriginalClockBoundary finalCorrespondenceClock_{};
    const OriginalInitialProviderReceiver::OfferOriginal* offer_=nullptr;
    const HeldTaskIdentity* receiverTask_=nullptr;
    const ProcTaskObservation* receiverObservation_=nullptr;
    OriginalDeadline* deadline_=nullptr;
    OriginalDeadlineSupervisor* supervisor_=nullptr;
    int borrowedPidfd_=-1;pid_t messagePid_=0;
    long signalResult_=-1;int signalError_=0;
    bool captureStarted_=false,signalAttempted_=false,signalReturned_=false,associated_=false;
    bool disposalRequested_=false,disposed_=false;
    ~OriginalInitialProviderProcessAssociation()=default;
public:
    static size_t CheckedSourceBacking(size_t observations,size_t statBytes,size_t statusBytes,size_t reads,size_t fdinfoBytes){
        const size_t task=HeldTaskIdentity::CheckedSourceBacking(observations,statBytes,statusBytes,reads);
        if(!fdinfoBytes||fdinfoBytes==SIZE_MAX||!reads)throw std::invalid_argument("provider fdinfo geometry");
        size_t total=sizeof(OriginalInitialProviderProcessAssociation);
        const auto add=[&](size_t n,size_t width){if(width&&n>(SIZE_MAX-total)/width)
            throw std::invalid_argument("provider process aggregate backing");total+=n*width;};
        add(task-sizeof(HeldTaskIdentity),1);add(fdinfoBytes+1,1);add(reads,sizeof(ProcReadOriginal));return total;
    }
private:
    OriginalInitialProviderProcessAssociation(const OriginalInitialProviderReceiver& receiver,
        size_t observations,size_t statBytes,size_t statusBytes,size_t reads,size_t fdinfoBytes)
        :receiver_(receiver),observations_(observations),statBytes_(statBytes),statusBytes_(statusBytes),reads_(reads),fdinfoBytes_(fdinfoBytes),
         callerTask_(DeferredOriginalPreown,observations,statBytes,statusBytes,reads),
         fdinfo_(DeferredOriginalPreown,fdinfoBytes,reads){}
    bool PreownStorageOriginal() noexcept {
        if(preownAttempted_)return false;preownAttempted_=true;
        struct Returned {bool& value;~Returned(){value=true;}} returned{preownReturned_};
        try {
            sourceBytes_=CheckedSourceBacking(observations_,statBytes_,statusBytes_,reads_,fdinfoBytes_);
            if(!callerTask_.PreownOriginal()){preownException_=callerTask_.PreownExceptionOriginal();return false;}
            if(!fdinfo_.PreownOriginal()){preownException_=fdinfo_.preownException;return false;}
            preowned_=true;return true;
        }catch(...){if(!preownException_)preownException_=std::current_exception();return false;}
    }
    bool CaptureOriginal(HeldTaskIdentity& receiverTask,pid_t receiverTid,OriginalDeadline& deadline,
        OriginalDeadlineSupervisor& supervisor) noexcept {
        if(!preowned_||captureStarted_||disposalRequested_)return false;captureStarted_=true;deadline_=&deadline;
        if(supervisor.OriginalDeadlineOwner()!=&deadline)return false;
        supervisor_=&supervisor;
        offer_=receiver_.OriginalOffer(0);receiverTask_=&receiverTask;
        const auto* peer=receiver_.OriginalObservedEndpointPeer();
        if(!offer_||!peer||!offer_->rawOfferObserved||!offer_->returned||offer_->rightsCount!=1||
           offer_->decoded.sequence!=1||offer_->decoded.role!=SLCLI_BQ_SUBJECT_REFERENCE||
           offer_->decoded.member_bytes||offer_->decoded.command!=UINT32_MAX||offer_->decoded.requirement!=UINT32_MAX||
           offer_->credentials.pid!=peer->pid||offer_->credentials.uid!=peer->uid||offer_->credentials.gid!=peer->gid||
           peer->pid<=0||!receiverTask.ObservationCount())return false;
        const auto& descriptor=offer_->rights[0];
        if(descriptor.original<0||descriptor.closeAttempted||!descriptor.statReturned)return false;
        borrowedPidfd_=descriptor.original;messagePid_=offer_->credentials.pid;
        receiverObservation_=&receiverTask.OriginalObservation(receiverTask.ObservationCount()-1);
        auto* root=receiverTask.OriginalRootOwner();
        if(!root||!root->Available()||root->OriginalDeadlineOwner()!=&deadline||
           !receiverTask.SameOriginal(*receiverObservation_)||receiverObservation_->observedPid!=receiverTid||
           !OriginalPidfdHandoffAssociation::OneNamespacePid(*receiverObservation_,receiverTid))return false;
        if(!before_.Capture(borrowedPidfd_,deadline,&supervisor))return false;
        if(!deadline.Observe("initial-provider-pidfd-signal-zero",false))return false;
        if(!supervisor.BeginNativeCall())return false;
        signalAttempted_=true;signalResult_=syscall(SYS_pidfd_send_signal,borrowedPidfd_,0,nullptr,0);
        if(signalResult_<0)signalError_=errno;signalReturned_=true;
        supervisor.OriginalNativeCallReturned();
        const bool signalClock=deadline.Observe("initial-provider-pidfd-signal-zero",true);
        if(signalResult_!=0||!signalClock)return false;
        if(!deadline.Observe("initial-provider-fdinfo-directory-open",false))return false;
        if(!supervisor.BeginNativeCall())return false;
        fdinfoDirectory_.openAttempted=true;
        fdinfoDirectory_.openResult=openat(receiverTask.OriginalDirectory().original,"fdinfo",O_PATH|O_DIRECTORY|O_NOFOLLOW|O_CLOEXEC);
        fdinfoDirectory_.original=fdinfoDirectory_.openResult;
        if(fdinfoDirectory_.openResult<0)fdinfoDirectory_.openError=errno;
        fdinfoDirectory_.openReturned=true;supervisor.OriginalNativeCallReturned();
        const bool directoryClock=deadline.Observe("initial-provider-fdinfo-directory-open",true);
        if(fdinfoDirectory_.openResult<0||!directoryClock)return false;
        char literal[32]{};const auto converted=std::to_chars(literal,literal+sizeof(literal)-1,borrowedPidfd_);
        if(converted.ec!=std::errc{}||!fdinfo_.Capture(fdinfoDirectory_.original,literal,deadline,&supervisor)||
           !OriginalPidfdHandoffAssociation::FdinfoPidOriginal(fdinfo_,messagePid_))return false;
        if(!callerTask_.BindFromOriginalProcRoot(*root,messagePid_,&supervisor))return false;
        const auto& caller=callerTask_.OriginalObservation(callerTask_.ObservationCount()-1);
        if(!callerTask_.SameOriginal(caller)||caller.observedTgid!=messagePid_||!after_.Capture(borrowedPidfd_,deadline,&supervisor))return false;
        associated_=IngressNativeClock::Capture(finalCorrespondenceClock_,&deadline,
            "initial-provider-process-correspondence",true)&&finalCorrespondenceClock_.shapeValid&&
            !finalCorrespondenceClock_.expired&&after_.postClockOriginal&&after_.postClockOriginal->shapeValid&&
            !after_.postClockOriginal->expired&&finalCorrespondenceClock_.nanoseconds>=after_.postClockOriginal->nanoseconds;
        return associated_; // No admitted UID/image/entry/source/provider authority.
    }
    bool DisposeBeforeSubjectExposureOriginal(OriginalDeadlineSupervisor& supervisor) noexcept {
        disposalRequested_=true;
        if((supervisor_&&supervisor_!=&supervisor)||(deadline_&&supervisor.OriginalDeadlineOwner()!=deadline_))return false;
        if((signalAttempted_&&!signalReturned_)||(before_.attempted&&!before_.returnedObserved)||
           (after_.attempted&&!after_.returnedObserved)||(fdinfoDirectory_.openAttempted&&!fdinfoDirectory_.openReturned)||
           (fdinfo_.descriptor.openAttempted&&!fdinfo_.descriptor.openReturned)||supervisor.HasPendingOriginalNativeCall())return false;
        for(size_t i=0;i<fdinfo_.readCount;++i)if(fdinfo_.reads[i].attempted&&!fdinfo_.reads[i].returnedObserved)return false;
        fdinfo_.descriptor.CloseOnce(deadline_,&supervisor);fdinfoDirectory_.CloseOnce(deadline_,&supervisor);
        disposed_=callerTask_.DisposeEligible(&supervisor)&&
            (fdinfo_.descriptor.original<0||fdinfo_.descriptor.closed)&&
            (fdinfoDirectory_.original<0||fdinfoDirectory_.closed);
        return disposed_;
    }
public:
    struct RetainedPreownBackingView {
        const OriginalInitialProviderProcessAssociation* ownerOriginal;
        const HeldTaskIdentity* callerTaskOriginal;const ProcFileObservation* fdinfoOriginal;
        const std::exception_ptr* exceptionOriginal;
        size_t taskCapacity,bornTasks,fdinfoByteCapacity,fdinfoReadCapacity,allocatedBackingBytes;
        bool attempted,returned,preowned,disposed;
    };
    bool ObserveRetainedPreownBackingOriginal(RetainedPreownBackingView& out) const noexcept {
        const auto output=reinterpret_cast<uintptr_t>(&out),owner=reinterpret_cast<uintptr_t>(this);size_t bytes=0;
        if(!disposed_||(preownAttempted_&&!preownReturned_)||!AllocatedBackingBytesOriginal(bytes)||
           callerTask_.BornObservationCountOriginal()>callerTask_.AllocatedObservationCapacityOriginal()||
           output>UINTPTR_MAX-sizeof(out)||owner>UINTPTR_MAX-sizeof(*this)||
           (output<owner+sizeof(*this)&&owner<output+sizeof(out)))return false;
        if(!OriginalBackingStorageSeparate(&out,sizeof(out),callerTask_.AllocatedObservationSlotsOriginal(),callerTask_.AllocatedObservationCapacityOriginal(),sizeof(std::unique_ptr<ProcTaskObservation>))||
           !OriginalBackingStorageSeparate(&out,sizeof(out),fdinfo_.bytes.get(),fdinfo_.bytes?fdinfo_.byteCapacity+1:0)||
           !OriginalBackingStorageSeparate(&out,sizeof(out),fdinfo_.reads.get(),fdinfo_.readCapacity,sizeof(ProcReadOriginal)))return false;
        for(size_t i=0;i<callerTask_.BornObservationCountOriginal();++i){
            const auto* row=callerTask_.AllocatedObservationOriginal(i);if(!row||!OriginalBackingStorageSeparate(&out,sizeof(out),row,1,sizeof(*row)))return false;
            for(const auto* file:{&row->stat,&row->status})
                if(!OriginalBackingStorageSeparate(&out,sizeof(out),file->bytes.get(),file->bytes?file->byteCapacity+1:0)||
                   !OriginalBackingStorageSeparate(&out,sizeof(out),file->reads.get(),file->readCapacity,sizeof(ProcReadOriginal))||
                   file->used>file->byteCapacity||file->readCount>file->readCapacity||
                   (file->byteCapacity&&!file->bytes)||(file->readCapacity&&!file->reads))return false;
        }
        std::memset(&out,0,sizeof(out));out.ownerOriginal=this;out.callerTaskOriginal=&callerTask_;out.fdinfoOriginal=&fdinfo_;
        out.exceptionOriginal=&preownException_;out.taskCapacity=callerTask_.AllocatedObservationCapacityOriginal();
        out.bornTasks=callerTask_.BornObservationCountOriginal();out.fdinfoByteCapacity=fdinfo_.bytes?fdinfo_.byteCapacity+1:0;
        out.fdinfoReadCapacity=fdinfo_.readCapacity;out.allocatedBackingBytes=bytes;
        out.attempted=preownAttempted_;out.returned=preownReturned_;out.preowned=preowned_;out.disposed=disposed_;return true;
    }
    bool DisposedBeforeExposureOriginal() const noexcept{return disposalRequested_&&disposed_;}
    size_t SourceBackingBytes() const noexcept{return sourceBytes_;} // Requested geometry, never a fit grant.
    bool PreownedOriginal() const noexcept{return preowned_;}
    bool PreownReturnedOriginal() const noexcept{return preownReturned_;}
    const std::exception_ptr& PreownExceptionOriginal() const noexcept{return preownException_;}
    bool AllocatedBackingBytesOriginal(size_t& bytes) const noexcept {
        size_t task=0;if(!callerTask_.ReservedSourceBytes(task)||task<sizeof(callerTask_))return false;
        bytes=sizeof(*this);const auto add=[&](size_t n){if(n>SIZE_MAX-bytes)return false;bytes+=n;return true;};
        return add(task-sizeof(callerTask_))&&add(fdinfo_.bytes?fdinfo_.byteCapacity+1:0)&&
            fdinfo_.readCapacity<=SIZE_MAX/sizeof(ProcReadOriginal)&&add(fdinfo_.readCapacity*sizeof(ProcReadOriginal));
    }
    bool KernelCorrespondenceObserved() const noexcept{return associated_;}
    const OriginalInitialProviderReceiver& OriginalReceiverOwner() const noexcept{return receiver_;}
    const OriginalInitialProviderReceiver::OfferOriginal* OriginalReceivedOffer() const noexcept{return offer_;}
    const HeldTaskIdentity& OriginalCallerTask() const noexcept{return callerTask_;}
    const ProcFileObservation& OriginalFdinfo() const noexcept{return fdinfo_;}
    const PidfdPollOriginal& OriginalBeforePoll() const noexcept{return before_;}
    const PidfdPollOriginal& OriginalAfterPoll() const noexcept{return after_;}
    long OriginalSignalZeroResult() const noexcept{return signalResult_;}
    int OriginalSignalZeroError() const noexcept{return signalError_;}
    bool OriginalSignalZeroAttempted() const noexcept{return signalAttempted_;}
    bool OriginalSignalZeroReturned() const noexcept{return signalReturned_;}
};
inline bool OriginalInitialProviderReceiver::AcknowledgeProcessReferenceOriginal(
    const OriginalInitialProviderProcessAssociation& original) noexcept {
    if(&original.receiver_!=this||!original.associated_||original.disposalRequested_||
       original.deadline_!=deadline_||original.supervisor_!=supervisor_||
       original.offer_!=OriginalOffer(0)||!original.signalReturned_||original.signalResult_!=0||
       !original.before_.returnedObserved||original.before_.returned!=0||original.before_.output.revents||
       !original.after_.returnedObserved||original.after_.returned!=0||original.after_.output.revents||
       !original.offer_||original.offer_->rightsCount!=1||original.offer_->rights[0].closeAttempted||
       original.borrowedPidfd_!=original.offer_->rights[0].original)return false;
    return SendReadbackAckOriginal(0);
}
}
#endif

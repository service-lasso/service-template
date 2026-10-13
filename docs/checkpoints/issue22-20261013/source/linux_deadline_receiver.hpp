// UNCOMPILED live independent receiver of the SAME owned deadline response.
// No Python access, process signals, cancellation or caller-result fabrication.
#pragma once
#include "linux_deadline_supervisor.hpp"
#include "linux_pidfd_identity.hpp"
namespace template_creator_linux {
class OriginalDeadlineReceiver {
    OriginalDeadlineSupervisor& supervisor_;
    OriginalPrivateIngress& ingress_;
    OriginalPidfdHandoffAssociation& callerAssociation_;
    // A single native SOCK_SEQPACKET send is the complete notice attempt.
    // EAGAIN/short/error is retained, never retried or called delivered.
    SupervisorNativeOriginal callerSend_;
    std::array<unsigned char,sizeof(OriginalDeadlineSupervisor::PendingResponse)> callerBody_{};
    iovec callerIovec_{};msghdr callerMessage_{};
    int originalCallerSocket_=-1;
    std::atomic<bool> callerNoticeDelivered_{false};
    pthread_t originalThread_{},actualWorkerSelf_{};
    pthread_attr_t attributes_{};
    SupervisorNativeOriginal attrInit_,attrStack_,stackQuery_,guardQuery_,attrDestroy_,
        threadCreate_,workerSelfTid_;
    OriginalClockBoundary selfBefore_{},selfAfter_{};
    bool selfAttempted_=false,selfReturned_=false;
    std::unique_ptr<SupervisorNativeOriginal[]> waits_,joins_;
    std::unique_ptr<void*[]> joinReturns_;
    size_t waitCapacity_=0,waitCount_=0,joinCapacity_=0,joinCount_=0,
        selectedStack_=0,actualStack_=0,actualGuard_=0,selectedWaits_=0,selectedJoins_=0,joinReturnCapacity_=0;
    bool preownAttempted_=false,preowned_=false;
    std::exception_ptr preownException_;
    std::atomic<bool> returned_{false},responseReceived_{false},normalStopReceived_{false},failed_{false};
    bool startAttempted_=false,created_=false,attributesAcquired_=false,
        joined_=false,joinUnknown_=false,disposed_=false;
    void Run() noexcept {
        supervisor_.Before(workerSelfTid_,"deadline-receiver-gettid",true);
        supervisor_.Returned(workerSelfTid_,syscall(SYS_gettid));
        if(workerSelfTid_.returned<=0){failed_.store(true,std::memory_order_release);return;}
        CaptureSupervisorClock(selfBefore_,supervisor_.origin_,"deadline-receiver-pthread-self",false);
        selfAttempted_=true;actualWorkerSelf_=pthread_self();selfReturned_=true;
        CaptureSupervisorClock(selfAfter_,supervisor_.origin_,"deadline-receiver-pthread-self",true);
        while(supervisor_.nativePublication_.load(std::memory_order_acquire)!=1) {
            if(waitCount_==waitCapacity_){failed_.store(true,std::memory_order_release);return;}
            auto& row=waits_[waitCount_++];
            supervisor_.Before(row,"deadline-receiver-publication-wait",true);
            supervisor_.Returned(row,syscall(SYS_futex,
                reinterpret_cast<int*>(&supervisor_.nativePublication_),FUTEX_WAIT_PRIVATE,0,nullptr,nullptr,0));
            if(row.returned!=0 && !(row.returned==-1&&(row.error==EINTR||row.error==EAGAIN))) {
                failed_.store(true,std::memory_order_release);return;
            }
        }
        // Actual observer write -> SAME private pipe read -> byte equality.
        // This remains a deadline/failure notice, never the pending syscall's result.
        if(!supervisor_.responseWrite_.attempted) {
            uint64_t wakeValue=0;
            if(supervisor_.wakeRead_.returnedObserved&&supervisor_.wakeRead_.returned==8)
                std::memcpy(&wakeValue,supervisor_.wakeBytes_.data(),8);
            const bool normal=wakeValue==supervisor_.originalStopValue_&&!supervisor_.ExpiredOrFailed();
            normalStopReceived_.store(normal,std::memory_order_release);
            if(!normal)failed_.store(true,std::memory_order_release);return;
        }
        const bool prefixCaptured=supervisor_.ReadPendingResponseOriginal();
        const bool complete=prefixCaptured&&supervisor_.responseNoticeComplete_.load(std::memory_order_acquire);
        responseReceived_.store(complete,std::memory_order_release);
        if(!complete)failed_.store(true,std::memory_order_release);
        // Receiver is independent of the blocked creator thread. SAME received
        // ingress capability and SAME captured caller relation are necessary
        // transport associations; neither grants runtime/input role authority.
        if(!complete)return;
        const auto& socket=ingress_.OriginalSocket();
        if(!callerAssociation_.Associated()||
           &callerAssociation_.OriginalIngressOwner()!=&ingress_||
           socket.original!=originalCallerSocket_||socket.closeAttempted) {
            failed_.store(true,std::memory_order_release);return;
        }
        std::memcpy(callerBody_.data(),supervisor_.responseReadBytes_.data(),callerBody_.size());
        callerIovec_.iov_base=callerBody_.data();callerIovec_.iov_len=callerBody_.size();
        callerMessage_.msg_iov=&callerIovec_;callerMessage_.msg_iovlen=1;
        supervisor_.Before(callerSend_,"deadline-receiver-original-caller-send",true);
        callerSend_.requested=callerBody_.size();
        supervisor_.Returned(callerSend_,sendmsg(originalCallerSocket_,&callerMessage_,MSG_DONTWAIT|MSG_NOSIGNAL));
        const bool delivered=callerSend_.returnedObserved&&
            callerSend_.returned==static_cast<long>(callerBody_.size());
        callerNoticeDelivered_.store(delivered,std::memory_order_release);
        if(!delivered)failed_.store(true,std::memory_order_release);
    }
    static void* ThreadMain(void* opaque) noexcept {
        auto& owner=*static_cast<OriginalDeadlineReceiver*>(opaque);
        owner.Run();
        owner.supervisor_.receiverFinished_.store(true,std::memory_order_release);
        owner.returned_.store(true,std::memory_order_release);return nullptr;
    }
    ~OriginalDeadlineReceiver()=default; // Original host retains owner through join.
public:
    OriginalDeadlineReceiver(DeferredOriginalPreownTag,OriginalDeadlineSupervisor& actualSupervisor,
        OriginalPrivateIngress& originalIngress,OriginalPidfdHandoffAssociation& originalCallerAssociation,
        size_t selectedStack,size_t selectedWaits,size_t selectedJoins) noexcept
        :supervisor_(actualSupervisor),ingress_(originalIngress),callerAssociation_(originalCallerAssociation),
         selectedStack_(selectedStack),selectedWaits_(selectedWaits),selectedJoins_(selectedJoins) {}
    OriginalDeadlineReceiver(OriginalDeadlineSupervisor& actualSupervisor,
                             OriginalPrivateIngress& originalIngress,
                             OriginalPidfdHandoffAssociation& originalCallerAssociation,size_t selectedStack,
                             size_t selectedWaits,size_t selectedJoins)
        :OriginalDeadlineReceiver(DeferredOriginalPreown,actualSupervisor,originalIngress,originalCallerAssociation,
            selectedStack,selectedWaits,selectedJoins) {
        if(!PreownOriginal())std::rethrow_exception(preownException_);
    }
    bool PreownOriginal() noexcept {
        if(preownAttempted_)return false;preownAttempted_=true;
        try {
            size_t total=sizeof(*this);
            const auto add=[&](size_t count,size_t width){if(count>(SIZE_MAX-total)/width)return false;total+=count*width;return true;};
            if(!selectedStack_||!selectedWaits_||!selectedJoins_||
               !add(selectedWaits_,sizeof(SupervisorNativeOriginal))||!add(selectedJoins_,sizeof(SupervisorNativeOriginal))||
               !add(selectedJoins_,sizeof(void*)))throw std::invalid_argument("native deadline receiver geometry");
            if(!supervisor_.PreownedOriginal()||supervisor_.originalReceiver_||supervisor_.startAttempted_)
                throw std::invalid_argument("same preowned original receiver before supervisor effects");
            waits_=std::make_unique<SupervisorNativeOriginal[]>(selectedWaits_);waitCapacity_=selectedWaits_;
            joins_=std::make_unique<SupervisorNativeOriginal[]>(selectedJoins_);joinCapacity_=selectedJoins_;
            joinReturns_=std::make_unique<void*[]>(selectedJoins_);joinReturnCapacity_=selectedJoins_;
            supervisor_.originalReceiver_=this;preowned_=true;return true;
        }catch(...){preownException_=std::current_exception();failed_.store(true);return false;}
    }
    bool PreownedOriginal() const noexcept{return preowned_;}
    const std::exception_ptr& PreownExceptionOriginal() const noexcept{return preownException_;}
    size_t AllocatedWaitCapacityOriginal() const noexcept{return waits_?waitCapacity_:0;}
    size_t AllocatedJoinCapacityOriginal() const noexcept{return joins_?joinCapacity_:0;}
    size_t AllocatedJoinReturnCapacityOriginal() const noexcept{return joinReturns_?joinReturnCapacity_:0;}
    OriginalDeadlineReceiver(const OriginalDeadlineReceiver&)=delete;
    OriginalDeadlineReceiver& operator=(const OriginalDeadlineReceiver&)=delete;
    bool Start() noexcept {
        if(!preowned_)return false;
        if(startAttempted_||disposed_)return false;startAttempted_=true;
        if(!supervisor_.startAttempted_)return false;
        if(!supervisor_.origin_.HasOriginalDeadline())return supervisor_.started_;
        if(!supervisor_.created_||supervisor_.responseReader_.original<0)return false;
        if(!ingress_.Captured()||!callerAssociation_.Associated()||
           &callerAssociation_.OriginalIngressOwner()!=&ingress_||
           callerAssociation_.OriginalDeadlineOwner()!=&supervisor_.origin_||
           ingress_.OriginalSocket().original<0||ingress_.OriginalSocket().closeAttempted)return false;
        originalCallerSocket_=ingress_.OriginalSocket().original;
        // Receiver creation is independently eligible failure capture even if
        // the original deadline expired during supervisor construction.
        supervisor_.Before(attrInit_,"deadline-receiver-attr-init",true);
        supervisor_.Returned(attrInit_,pthread_attr_init(&attributes_),false);
        attributesAcquired_=attrInit_.returned==0;if(!attributesAcquired_)return false;
        supervisor_.Before(attrStack_,"deadline-receiver-attr-stack",true);
        supervisor_.Returned(attrStack_,pthread_attr_setstacksize(&attributes_,selectedStack_),false);
        supervisor_.Before(stackQuery_,"deadline-receiver-stack-query",true);
        supervisor_.Returned(stackQuery_,pthread_attr_getstacksize(&attributes_,&actualStack_),false);
        supervisor_.Before(guardQuery_,"deadline-receiver-guard-query",true);
        supervisor_.Returned(guardQuery_,pthread_attr_getguardsize(&attributes_,&actualGuard_),false);
        if(attrStack_.returned||stackQuery_.returned||guardQuery_.returned)return false;
        supervisor_.Before(threadCreate_,"deadline-receiver-thread-create",true);
        supervisor_.Returned(threadCreate_,pthread_create(&originalThread_,&attributes_,ThreadMain,this),false);
        created_=threadCreate_.returned==0;
        supervisor_.receiverCreated_.store(created_,std::memory_order_release);
        supervisor_.Before(attrDestroy_,"deadline-receiver-attr-destroy",true);
        supervisor_.Returned(attrDestroy_,pthread_attr_destroy(&attributes_),false);
        if(!created_||attrDestroy_.returned)failed_.store(true,std::memory_order_release);
        return created_&&attrDestroy_.returned==0;
    }
    bool ObserveEligibleJoinAndDisposal() noexcept {
        if(disposed_)return true;
        if(created_&&!joined_) {
            if(!returned_.load(std::memory_order_acquire)||joinUnknown_||joinCount_==joinCapacity_)return false;
            const size_t ordinal=joinCount_++;auto& row=joins_[ordinal];
            supervisor_.Before(row,"deadline-receiver-tryjoin",true);
            supervisor_.Returned(row,pthread_tryjoin_np(originalThread_,&joinReturns_[ordinal]),false);
            if(row.returned==0){joined_=true;supervisor_.receiverJoined_.store(true,std::memory_order_release);if(joinReturns_[ordinal])failed_.store(true,std::memory_order_release);}
            else if(row.returned!=EBUSY){joinUnknown_=true;failed_.store(true,std::memory_order_release);}
            if(!joined_)return false;
        }
        if(attributesAcquired_&&!attrDestroy_.attempted) {
            supervisor_.Before(attrDestroy_,"deadline-receiver-eligible-attr-destroy",true);
            supervisor_.Returned(attrDestroy_,pthread_attr_destroy(&attributes_),false);
        }
        disposed_=(!created_||joined_)&&(!attributesAcquired_||
            (attrDestroy_.returnedObserved&&attrDestroy_.returned==0));
        return disposed_;
    }
    bool OriginalNoticeReceived() const noexcept{return responseReceived_.load(std::memory_order_acquire);}
    bool OriginalCallerNoticeDelivered() const noexcept{return callerNoticeDelivered_.load(std::memory_order_acquire);}
    const SupervisorNativeOriginal& OriginalCallerSend() const noexcept{return callerSend_;}
    bool OriginalNormalStopReceived() const noexcept{return normalStopReceived_.load(std::memory_order_acquire);}
    bool OriginalReceiverReturned() const noexcept{return returned_.load(std::memory_order_acquire);}
    bool Failed() const noexcept{return failed_.load(std::memory_order_acquire);}
    bool PhysicalDisposed() const noexcept{return disposed_;}
    bool ReservedSourceBytes(size_t& bytes) const noexcept {
        bytes=sizeof(*this);auto add=[&](size_t n){if(n>SIZE_MAX-bytes)return false;bytes+=n;return true;};
        if(!add(waitCapacity_*sizeof(SupervisorNativeOriginal))||!add(joinCapacity_*sizeof(SupervisorNativeOriginal))||
            !add(joinReturnCapacity_*sizeof(void*)))return false;
        if(supervisor_.origin_.HasOriginalDeadline()) {
            if(!stackQuery_.returnedObserved||stackQuery_.returned||!guardQuery_.returnedObserved||guardQuery_.returned)return false;
            if(!add(actualStack_)||!add(actualGuard_))return false;
        }
        return true; // Actual kernel/TLS/libc/allocator/runtime accounting additional.
    }
    friend class OriginalLinuxPlatformState;
    friend class OriginalLinuxPendingInputOwner;
    friend class OriginalLinuxCallerReceipt;
};
}

// UNCOMPILED independent native original caller of the private source channel.
// Receives while creator's ioctl/pread may remain pending. No Python access,
// syscall cancellation, signal, process termination or command success fiction.
#pragma once
#include "linux_host_channel.hpp"
#include "linux_deadline_receiver.hpp"
namespace template_creator_linux {
class OriginalLinuxCallerReceipt {
    OriginalLinuxHostChannel& channel_;
    OriginalDeadline& deadline_;
    OriginalDeadlineReceiver& creatorReceiver_;
    OriginalPrivateIngress received_;
    IngressFd stop_{},callerSocket_{};
    int originalSocket_=-1;
    pthread_t originalThread_{};
    pthread_attr_t attributes_{};
    SupervisorNativeOriginal eventCreate_,attrInit_,attrStack_,stackQuery_,guardQuery_,attrDestroy_,
        threadCreate_,workerTid_,poll_,stopWrite_,stopRead_,socketClose_,stopClose_;
    std::array<unsigned char,sizeof(uint64_t)> stopBytes_{};
    std::array<pollfd,2> pollInput_{},pollOutput_{};
    uint64_t originalStopValue_=1;
    OriginalPendingDeadlineNotice notice_{};
    std::unique_ptr<SupervisorNativeOriginal[]> joins_;
    std::unique_ptr<void*[]> joinReturns_;
    size_t capacity_=0,count_=0,selectedStack_=0,actualStack_=0,actualGuard_=0,selectedJoins_=0,joinReturnCapacity_=0;
    bool preownAttempted_=false,preowned_=false;
    std::exception_ptr preownException_;
    bool startAttempted_=false,attributesAcquired_=false,created_=false,joined_=false,
        joinUnknown_=false,stopAttempted_=false,disposed_=false;
    std::atomic<bool> returned_{false},noticeReceived_{false},failed_{false};
    void Before(SupervisorNativeOriginal& row,const char* op) noexcept {
        CaptureSupervisorClock(row.beforeClock,deadline_,op,false);row.attempted=true;
    }
    void Returned(SupervisorNativeOriginal& row,long value,bool errnoStyle=true) noexcept {
        row.returned=value;if(errnoStyle&&value<0)row.error=errno;
        if(!errnoStyle&&value)row.error=static_cast<int>(value);
        row.returnedObserved=true;
        CaptureSupervisorClock(row.afterClock,deadline_,row.beforeClock.operation,true);
        if(!row.beforeClock.shapeValid||!row.afterClock.shapeValid||
           row.afterClock.nanoseconds<row.beforeClock.nanoseconds)failed_.store(true,std::memory_order_release);
    }
    void Run() noexcept {
        Before(workerTid_,"original-caller-receipt-gettid");Returned(workerTid_,syscall(SYS_gettid));
        if(workerTid_.returned<=0){failed_.store(true,std::memory_order_release);return;}
        pollInput_[0].fd=originalSocket_;pollInput_[0].events=POLLIN;
        pollInput_[1].fd=stop_.original;pollInput_[1].events=POLLIN;pollOutput_=pollInput_;
        Before(poll_,"original-caller-receipt-poll");Returned(poll_,poll(pollOutput_.data(),2,-1));
        // Interrupted/failed poll is retained, never renewed as a new wait.
        if(poll_.returned<=0){failed_.store(true,std::memory_order_release);return;}
        if(pollOutput_[0].revents&POLLIN) {
            // SAME endpoint transferred before thread creation; capture attaches
            // every delivered FD before rejecting unexpected ancillary data.
            const bool captured=received_.CaptureTransferredSocket(originalSocket_,&deadline_,true);
            const bool shape=captured&&received_.OriginalDescriptorCount()==0&&
                received_.OriginalReceiveResult()==static_cast<ssize_t>(sizeof(notice_))&&
                received_.MessageCredentialsPresent()&&
                received_.OriginalMessageCredentials().pid==channel_.selfPid_.returned;
            if(shape){
                std::memcpy(&notice_,received_.OriginalBody().data(),sizeof(notice_));
                const bool origin=deadline_.HasOriginalDeadline()&&
                    notice_.originalAbsoluteNs==deadline_.OriginalAbsoluteNanoseconds()&&
                    (notice_.deadlineObserved||notice_.observationFailed);
                noticeReceived_.store(origin,std::memory_order_release);
                if(!origin)failed_.store(true,std::memory_order_release);
            }else failed_.store(true,std::memory_order_release);
        } else if(pollOutput_[1].revents&POLLIN) {
            Before(stopRead_,"original-caller-stop-read");stopRead_.requested=stopBytes_.size();
            Returned(stopRead_,read(stop_.original,stopBytes_.data(),stopBytes_.size()));
            uint64_t actual=0;if(stopRead_.returned==sizeof(actual))std::memcpy(&actual,stopBytes_.data(),sizeof(actual));
            if(stopRead_.returned!=sizeof(actual)||actual!=originalStopValue_)failed_.store(true,std::memory_order_release);
        } else failed_.store(true,std::memory_order_release);
    }
    static void* ThreadMain(void* p) noexcept {
        auto& owner=*static_cast<OriginalLinuxCallerReceipt*>(p);owner.Run();
        owner.creatorReceiver_.supervisor_.callerFinished_.store(true,std::memory_order_release);
        owner.returned_.store(true,std::memory_order_release);return nullptr;
    }
    ~OriginalLinuxCallerReceipt()=default;
public:
    OriginalLinuxCallerReceipt(DeferredOriginalPreownTag,OriginalLinuxHostChannel& channel,OriginalDeadline& deadline,
        OriginalDeadlineReceiver& creatorReceiver,size_t stack,size_t joins) noexcept
        :channel_(channel),deadline_(deadline),creatorReceiver_(creatorReceiver),selectedStack_(stack),selectedJoins_(joins) {}
    OriginalLinuxCallerReceipt(OriginalLinuxHostChannel& channel,OriginalDeadline& deadline,
            OriginalDeadlineReceiver& creatorReceiver,size_t stack,size_t joins)
        :OriginalLinuxCallerReceipt(DeferredOriginalPreown,channel,deadline,creatorReceiver,stack,joins) {
        if(!PreownOriginal())std::rethrow_exception(preownException_);
    }
    bool PreownOriginal() noexcept {
        if(preownAttempted_)return false;preownAttempted_=true;
        try {
            if(!selectedStack_||!selectedJoins_||selectedJoins_>(SIZE_MAX-sizeof(*this))/sizeof(SupervisorNativeOriginal))
                throw std::invalid_argument("original caller receipt geometry");
            const auto rowBytes=selectedJoins_*sizeof(SupervisorNativeOriginal);
            if(selectedJoins_>(SIZE_MAX-sizeof(*this)-rowBytes)/sizeof(void*))
                throw std::invalid_argument("aggregate original caller receipt geometry");
            auto& supervisor=creatorReceiver_.supervisor_;
            if(!creatorReceiver_.PreownedOriginal()||supervisor.originalCaller_||supervisor.startAttempted_||&supervisor.origin_!=&deadline_)
                throw std::invalid_argument("same preowned original caller before supervisor effects");
            joins_=std::make_unique<SupervisorNativeOriginal[]>(selectedJoins_);capacity_=selectedJoins_;
            joinReturns_=std::make_unique<void*[]>(selectedJoins_);joinReturnCapacity_=selectedJoins_;
            supervisor.originalCaller_=this;preowned_=true;return true;
        }catch(...){preownException_=std::current_exception();failed_.store(true);return false;}
    }
    bool OutputBackingSeparateOriginal(const void* output,size_t bytes) const noexcept {
        return OriginalBackingStorageSeparate(output,bytes,this,1,sizeof(*this))&&
            OriginalBackingStorageSeparate(output,bytes,joins_.get(),AllocatedJoinCapacityOriginal(),sizeof(SupervisorNativeOriginal))&&
            OriginalBackingStorageSeparate(output,bytes,joinReturns_.get(),AllocatedJoinReturnCapacityOriginal(),sizeof(void*));
    }
    bool PreownedOriginal() const noexcept{return preowned_;}
    const std::exception_ptr& PreownExceptionOriginal() const noexcept{return preownException_;}
    size_t AllocatedJoinCapacityOriginal() const noexcept{return joins_?capacity_:0;}
    size_t AllocatedJoinReturnCapacityOriginal() const noexcept{return joinReturns_?joinReturnCapacity_:0;}
    OriginalLinuxCallerReceipt(const OriginalLinuxCallerReceipt&)=delete;
    OriginalLinuxCallerReceipt& operator=(const OriginalLinuxCallerReceipt&)=delete;
    bool StartOriginal() noexcept {
        if(!preowned_)return false;
        if(startAttempted_)return false;startAttempted_=true;
        if(&channel_.deadline_!=&deadline_||!channel_.constructed_)return false;
        originalSocket_=channel_.TransferOriginalCallerEndpoint();if(originalSocket_<0)return false;
        callerSocket_.original=originalSocket_; // Owner before any native effect.
        Before(eventCreate_,"original-caller-stop-event");Returned(eventCreate_,eventfd(0,EFD_CLOEXEC|EFD_NONBLOCK));
        stop_.original=eventCreate_.returned;
        if(stop_.original<0)return false;
        Before(attrInit_,"original-caller-attr-init");Returned(attrInit_,pthread_attr_init(&attributes_),false);
        attributesAcquired_=attrInit_.returned==0;if(!attributesAcquired_)return false;
        Before(attrStack_,"original-caller-attr-stack");Returned(attrStack_,pthread_attr_setstacksize(&attributes_,selectedStack_),false);
        Before(stackQuery_,"original-caller-stack-query");Returned(stackQuery_,pthread_attr_getstacksize(&attributes_,&actualStack_),false);
        Before(guardQuery_,"original-caller-guard-query");Returned(guardQuery_,pthread_attr_getguardsize(&attributes_,&actualGuard_),false);
        if(attrStack_.returned||stackQuery_.returned||guardQuery_.returned)return false;
        Before(threadCreate_,"original-caller-thread-create");Returned(threadCreate_,pthread_create(&originalThread_,&attributes_,ThreadMain,this),false);
        created_=threadCreate_.returned==0;
        creatorReceiver_.supervisor_.callerCreated_.store(created_,std::memory_order_release);
        Before(attrDestroy_,"original-caller-attr-destroy");Returned(attrDestroy_,pthread_attr_destroy(&attributes_),false);
        if(!created_||attrDestroy_.returned)failed_.store(true,std::memory_order_release);
        return created_&&attrDestroy_.returned==0;
    }
    bool ObserveStopJoinEligible() noexcept {
        // This stop is only the private caller's local wait, never the pending
        // creator syscall. Creator response owner must finish before FD closes.
        if(created_&&!returned_.load(std::memory_order_acquire)&&!stopAttempted_) {
            stopAttempted_=true;Before(stopWrite_,"original-caller-stop-write");
            Returned(stopWrite_,write(stop_.original,&originalStopValue_,sizeof(originalStopValue_)));
            if(stopWrite_.returned!=sizeof(originalStopValue_))failed_.store(true,std::memory_order_release);
        }
        if(created_&&!joined_) {
            if(!returned_.load(std::memory_order_acquire)||joinUnknown_||count_==capacity_)return false;
            const size_t ordinal=count_++;auto& row=joins_[ordinal];Before(row,"original-caller-tryjoin");
            Returned(row,pthread_tryjoin_np(originalThread_,&joinReturns_[ordinal]),false);
            if(row.returned==0){joined_=true;if(joinReturns_[ordinal])failed_.store(true,std::memory_order_release);}
            else if(row.returned!=EBUSY){joinUnknown_=true;failed_.store(true,std::memory_order_release);}
        }
        return !created_||joined_;
    }
    bool OriginalNoticeReceived() const noexcept{return noticeReceived_.load(std::memory_order_acquire);}
    bool OriginalCallerOriginalsPublished() const noexcept{return returned_.load(std::memory_order_acquire);}
    const SupervisorNativeOriginal& OriginalWorkerTid() const noexcept{return workerTid_;}
    const SupervisorNativeOriginal& OriginalPoll() const noexcept{return poll_;}
    const OriginalPendingDeadlineNotice& OriginalNotice() const noexcept{return notice_;}
    const OriginalPrivateIngress& OriginalCapture() const noexcept{return received_;}
    bool Failed() const noexcept{return failed_.load(std::memory_order_acquire);}
    bool DisposeAfterOriginalReceiversEligible() noexcept {
        if(disposed_)return true;
        if((created_&&!joined_)||!creatorReceiver_.PhysicalDisposed())return false;
        if(attributesAcquired_&&!attrDestroy_.attempted) {
            Before(attrDestroy_,"original-caller-eligible-attr-destroy");
            Returned(attrDestroy_,pthread_attr_destroy(&attributes_),false);
        }
        bool capturedClosed=true;
        if(received_.OriginalSocket().original>=0)capturedClosed=received_.DisposeEligible();
        else if(callerSocket_.original>=0&&!callerSocket_.closeAttempted) {
            Before(socketClose_,"original-caller-once-socket-close");callerSocket_.closeAttempted=true;
            callerSocket_.closeResult=close(callerSocket_.original);
            if(callerSocket_.closeResult<0)callerSocket_.closeErrno=errno;
            Returned(socketClose_,callerSocket_.closeResult);
            callerSocket_.closed=callerSocket_.closeResult==0;
        }
        if(stop_.original>=0&&!stop_.closeAttempted) {
            Before(stopClose_,"original-caller-once-stop-close");stop_.closeAttempted=true;
            stop_.closeResult=close(stop_.original);if(stop_.closeResult<0)stop_.closeErrno=errno;
            Returned(stopClose_,stop_.closeResult);stop_.closed=stop_.closeResult==0;
        }
        const bool socketClosed=received_.OriginalSocket().original>=0?capturedClosed:
            (callerSocket_.original<0||callerSocket_.closed);
        disposed_=socketClosed&&(stop_.original<0||stop_.closed)&&
            (!attributesAcquired_||(attrDestroy_.returnedObserved&&attrDestroy_.returned==0));
        if(!disposed_)failed_.store(true,std::memory_order_release);return disposed_;
    }
    bool ReservedSourceBytes(size_t& bytes) const noexcept {
        bytes=sizeof(*this);auto add=[&](size_t n){if(n>SIZE_MAX-bytes)return false;bytes+=n;return true;};
        if(!add(capacity_*sizeof(SupervisorNativeOriginal))||!add(joinReturnCapacity_*sizeof(void*)))return false;
        if(startAttempted_&&attributesAcquired_) {
            if(!stackQuery_.returnedObserved||stackQuery_.returned||!guardQuery_.returnedObserved||guardQuery_.returned)return false;
            if(!add(actualStack_)||!add(actualGuard_))return false;
        }
        return true; // Native kernel/TLS/allocator/runtime additionally measured.
    }
    friend class OriginalLinuxPlatformState;
    friend class OriginalLinuxPendingHost;
};
}

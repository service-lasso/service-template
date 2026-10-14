// UNCOMPILED independent notification for an EXISTING monotonic deadline.
// No signal, cancellation, syscall retry, kill, or new timeout is introduced.
#pragma once
#include "linux_original_deadline.hpp"
#include <sys/timerfd.h>
#include <sys/eventfd.h>
#include <sys/syscall.h>
#include <pthread.h>
#include <poll.h>
#include <unistd.h>
#include <fcntl.h>
#include <linux/futex.h>
#include <atomic>
#include <array>
#include <limits>
#include <cstring>
namespace template_creator_linux {
struct SupervisorNativeOriginal {
    OriginalClockBoundary beforeClock{},afterClock{};
    bool attempted=false,returnedObserved=false;
    long returned=-1;int error=0;
    size_t requested=0,destinationOffset=0;
};
inline bool CaptureSupervisorClock(OriginalClockBoundary& row,const OriginalDeadline& origin,
                                   const char* operation,bool after) noexcept {
    row.operation=operation;row.after=after;row.attempted=true;
    row.returned=clock_gettime(CLOCK_MONOTONIC,&row.output);
    if(row.returned<0)row.error=errno;
    if(row.returned!=0||row.output.tv_sec<0||row.output.tv_nsec<0||row.output.tv_nsec>=1000000000||
       static_cast<uint64_t>(row.output.tv_sec)>(UINT64_MAX-static_cast<uint64_t>(row.output.tv_nsec))/1000000000)
        return false;
    row.nanoseconds=static_cast<uint64_t>(row.output.tv_sec)*1000000000+static_cast<uint64_t>(row.output.tv_nsec);
    row.shapeValid=true;
    row.expired=origin.HasOriginalDeadline()&&row.nanoseconds>=origin.OriginalAbsoluteNanoseconds();
    return !row.expired;
}
struct SupervisorFdOriginal {
    SupervisorNativeOriginal acquisition,closeOriginal;
    int original=-1;
    bool CloseOnce(const OriginalDeadline& origin) noexcept {
        if(original<0)return true;
        if(closeOriginal.attempted)return closeOriginal.returnedObserved&&closeOriginal.returned==0;
        CaptureSupervisorClock(closeOriginal.beforeClock,origin,"supervisor-close",false);
        closeOriginal.attempted=true;closeOriginal.returned=close(original);
        if(closeOriginal.returned<0)closeOriginal.error=errno;
        closeOriginal.returnedObserved=true;
        CaptureSupervisorClock(closeOriginal.afterClock,origin,"supervisor-close",true);
        return closeOriginal.returned==0;
    }
};
class OriginalDeadlineReceiver;
class OriginalLinuxCallerReceipt;
struct OriginalPendingDeadlineNotice {
    uint64_t originalAbsoluteNs=0;size_t pendingOriginalCalls=0;
    bool deadlineObserved=false,observationFailed=false;
};
class OriginalDeadlineSupervisor {
    static_assert(sizeof(int)==4 && sizeof(std::atomic<int>)==4 &&
        alignof(std::atomic<int>)>=4 && std::atomic<int>::is_always_lock_free,
        "selected Linux futex word ABI required");
    OriginalDeadline& origin_;
    OriginalDeadlineReceiver* originalReceiver_=nullptr;
    OriginalLinuxCallerReceipt* originalCaller_=nullptr;
    std::atomic<bool> callerCreated_{false},callerFinished_{false};
    std::atomic<bool> receiverCreated_{false},receiverFinished_{false},receiverJoined_{false};
    pthread_t originalThread_{};
    pthread_attr_t attributes_{};
    SupervisorFdOriginal timer_,wake_,responseReader_,responseWriter_;
    std::array<int,2> originalResponsePipe_{{-1,-1}};
    SupervisorNativeOriginal responsePipeCreate_,responseWrite_,publicationWake_;
    std::atomic<int> nativePublication_{0};
    using PendingResponse=OriginalPendingDeadlineNotice;
    PendingResponse originalResponse_{};
    std::array<unsigned char,sizeof(PendingResponse)> responseReadBytes_{};
    std::array<SupervisorNativeOriginal,sizeof(PendingResponse)> responseReads_{};
    size_t responseReadCount_=0,responseReadExtent_=0;
    bool responseReadAttempted_=false,responseOutputUnknown_=false;
    std::atomic<bool> responseReadVerified_{false},responseReadCompleted_{false},responseNoticeComplete_{false};
    SupervisorNativeOriginal attrInit_,attrStack_,attrQueryStack_,attrQueryGuard_,attrDestroy_,
        timerArm_,threadCreate_,pollOriginal_,timerRead_,wakeRead_,stopWrite_,threadSelfTid_;
    std::array<pollfd,2> pollInputs_{},pollOutputs_{};
    itimerspec originalTimer_{},nativeTimer_{};
    std::array<unsigned char,8> timerBytes_{},wakeBytes_{};
    uint64_t originalStopValue_=1;
    pthread_t workerPthreadOriginal_{};
    OriginalClockBoundary pthreadSelfBefore_{},pthreadSelfAfter_{};
    bool pthreadSelfAttempted_=false,pthreadSelfReturned_=false;
    size_t selectedStack_=0,actualStack_=0,actualGuard_=0;
    std::unique_ptr<SupervisorNativeOriginal[]> joins_;
    std::unique_ptr<void*[]> joinReturns_;
    size_t joinCapacity_=0,joinCount_=0,selectedJoins_=0,joinReturnCapacity_=0;
    bool preownAttempted_=false,preowned_=false;
    std::exception_ptr preownException_;
    std::atomic<size_t> pendingCalls_{0};
    std::atomic<bool> expired_{false},failed_{false},workerReturned_{false};
    bool startAttempted_=false,started_=false,created_=false,attributesAcquired_=false,
        joined_=false,joinUnknown_=false,stopAttempted_=false,disposed_=false,cleanupStartupClosed_=false;
    bool Before(SupervisorNativeOriginal& row,const char* operation,bool cleanup=false) noexcept {
        const bool clean=CaptureSupervisorClock(row.beforeClock,origin_,operation,false);
        if(row.beforeClock.expired)expired_.store(true,std::memory_order_release);
        if(!clean)failed_.store(true,std::memory_order_release);
        if(!cleanup&&(!clean||ExpiredOrFailed()))return false;
        row.attempted=true;return true;
    }
    void Returned(SupervisorNativeOriginal& row,long result,bool errnoStyle=true) noexcept {
        row.returned=result;if(errnoStyle&&result<0)row.error=errno;
        if(!errnoStyle&&result)row.error=static_cast<int>(result);
        row.returnedObserved=true;
        const bool after=CaptureSupervisorClock(row.afterClock,origin_,row.beforeClock.operation,true);
        if(row.afterClock.expired)expired_.store(true,std::memory_order_release);
        if(!after || !row.beforeClock.shapeValid || row.afterClock.nanoseconds<row.beforeClock.nanoseconds)
            failed_.store(true,std::memory_order_release);
    }
    void Worker() noexcept {
        if(!CaptureSupervisorClock(pthreadSelfBefore_,origin_,"supervisor-pthread-self",false))
            {if(pthreadSelfBefore_.expired)expired_.store(true,std::memory_order_release);
             failed_.store(true,std::memory_order_release);return;}
        pthreadSelfAttempted_=true;workerPthreadOriginal_=pthread_self();pthreadSelfReturned_=true;
        if(!CaptureSupervisorClock(pthreadSelfAfter_,origin_,"supervisor-pthread-self",true))
            {failed_.store(true,std::memory_order_release);return;}
        if(!Before(threadSelfTid_,"supervisor-gettid"))return;Returned(threadSelfTid_,syscall(SYS_gettid));
        if(threadSelfTid_.returned<=0) {failed_.store(true,std::memory_order_release);return;}
        pollInputs_[0]={timer_.original,POLLIN,0};pollInputs_[1]={wake_.original,POLLIN,0};
        pollOutputs_=pollInputs_;
        if(!Before(pollOriginal_,"supervisor-poll"))return;Returned(pollOriginal_,poll(pollOutputs_.data(),2,-1));
        if(pollOriginal_.returned<=0||pollOriginal_.returned>2) {failed_.store(true,std::memory_order_release);return;}
        if(!(pollOutputs_[0].revents&POLLIN)&&!(pollOutputs_[1].revents&POLLIN))
            failed_.store(true,std::memory_order_release);
        if((pollOutputs_[0].revents&~POLLIN)||(pollOutputs_[1].revents&~POLLIN))
            failed_.store(true,std::memory_order_release);
        if(pollOutputs_[0].revents&POLLIN) {
            Before(timerRead_,"supervisor-timer-read",true);Returned(timerRead_,read(timer_.original,timerBytes_.data(),timerBytes_.size()));
            uint64_t expirations=0;
            if(timerRead_.returned==8)std::memcpy(&expirations,timerBytes_.data(),8);
            if(timerRead_.returned!=8||!expirations)failed_.store(true,std::memory_order_release);
            // A ready timer, even a failed read, cannot recover qualification.
            expired_.store(true,std::memory_order_release);
        }
        if(pollOutputs_[1].revents&POLLIN) {
            Before(wakeRead_,"supervisor-wake-read",true);Returned(wakeRead_,read(wake_.original,wakeBytes_.data(),wakeBytes_.size()));
            if(wakeRead_.returned!=8)failed_.store(true,std::memory_order_release);
        }
    }
    static void* ThreadMain(void* self) noexcept {
        auto& owner=*static_cast<OriginalDeadlineSupervisor*>(self);
        owner.Worker();
        if(owner.expired_.load(std::memory_order_acquire)||owner.failed_.load(std::memory_order_acquire)) {
            owner.originalResponse_.originalAbsoluteNs=owner.origin_.OriginalAbsoluteNanoseconds();
            owner.originalResponse_.pendingOriginalCalls=owner.pendingCalls_.load(std::memory_order_acquire);
            owner.originalResponse_.deadlineObserved=owner.expired_.load(std::memory_order_acquire);
            owner.originalResponse_.observationFailed=owner.failed_.load(std::memory_order_acquire);
            owner.responseWrite_.requested=sizeof(owner.originalResponse_);
            owner.Before(owner.responseWrite_,"supervisor-pending-response",true);
            owner.Returned(owner.responseWrite_,write(owner.responseWriter_.original,
                &owner.originalResponse_,sizeof(owner.originalResponse_)));
            if(owner.responseWrite_.returned!=sizeof(owner.originalResponse_))
                owner.failed_.store(true,std::memory_order_release);
        }
        owner.nativePublication_.store(1,std::memory_order_release);
        owner.Before(owner.publicationWake_,"supervisor-publication-wake",true);
        owner.Returned(owner.publicationWake_,syscall(SYS_futex,
            reinterpret_cast<int*>(&owner.nativePublication_),FUTEX_WAKE_PRIVATE,1,nullptr,nullptr,0));
        owner.workerReturned_.store(true,std::memory_order_release);
        return nullptr;
    }
    ~OriginalDeadlineSupervisor()=default; // Resident registry owns original lifetime.
public:
    OriginalDeadlineSupervisor(DeferredOriginalPreownTag,OriginalDeadline& actualOrigin,size_t selectedStack,size_t selectedJoins) noexcept
        :origin_(actualOrigin),selectedStack_(selectedStack),selectedJoins_(selectedJoins) {}
    OriginalDeadlineSupervisor(OriginalDeadline& actualOrigin,size_t selectedStack,size_t selectedJoins)
        :OriginalDeadlineSupervisor(DeferredOriginalPreown,actualOrigin,selectedStack,selectedJoins) {
        if(!PreownOriginal())std::rethrow_exception(preownException_);
    }
    bool PreownOriginal() noexcept {
        if(preownAttempted_)return false;preownAttempted_=true;
        try {
            if(!selectedStack_||!selectedJoins_||selectedJoins_>SIZE_MAX/sizeof(SupervisorNativeOriginal)||
               selectedJoins_>SIZE_MAX/sizeof(void*))throw std::invalid_argument("original supervisor geometry");
            const size_t rowsBytes=selectedJoins_*sizeof(SupervisorNativeOriginal);
            if(rowsBytes>SIZE_MAX-sizeof(*this)||selectedJoins_>(SIZE_MAX-sizeof(*this)-rowsBytes)/sizeof(void*))
                throw std::invalid_argument("aggregate original supervisor backing");
            joins_=std::make_unique<SupervisorNativeOriginal[]>(selectedJoins_);joinCapacity_=selectedJoins_;
            joinReturns_=std::make_unique<void*[]>(selectedJoins_);joinReturnCapacity_=selectedJoins_;
            preowned_=true;return true;
        }catch(...){preownException_=std::current_exception();failed_.store(true);return false;}
    }
    bool PreownedOriginal() const noexcept{return preowned_;}
    const std::exception_ptr& PreownExceptionOriginal() const noexcept{return preownException_;}
    size_t AllocatedJoinCapacityOriginal() const noexcept{return joins_?joinCapacity_:0;}
    size_t AllocatedJoinReturnCapacityOriginal() const noexcept{return joinReturns_?joinReturnCapacity_:0;}
    OriginalDeadlineSupervisor(const OriginalDeadlineSupervisor&)=delete;
    OriginalDeadlineSupervisor& operator=(const OriginalDeadlineSupervisor&)=delete;
    bool Start() noexcept {
        if(!preowned_||!origin_.PreownedOriginal())return false;
        if(startAttempted_||disposed_||cleanupStartupClosed_)return false;startAttempted_=true;
        if(!origin_.Observe("deadline-supervisor-start",false)){failed_.store(true);return false;}
        if(!origin_.HasOriginalDeadline()){started_=origin_.Observe("deadline-supervisor-no-timer",true);return started_;}
        const uint64_t ns=origin_.OriginalAbsoluteNanoseconds(),seconds=ns/1000000000;
        if(seconds>static_cast<uintmax_t>(std::numeric_limits<time_t>::max())){failed_.store(true);return false;}
        originalTimer_.it_value.tv_sec=static_cast<time_t>(seconds);
        originalTimer_.it_value.tv_nsec=static_cast<long>(ns%1000000000);nativeTimer_=originalTimer_;
        if(!Before(attrInit_,"supervisor-attr-init"))return false;Returned(attrInit_,pthread_attr_init(&attributes_),false);
        attributesAcquired_=attrInit_.returned==0;if(!attributesAcquired_){failed_.store(true);return false;}
        if(!Before(attrStack_,"supervisor-attr-stack"))return false;Returned(attrStack_,pthread_attr_setstacksize(&attributes_,selectedStack_),false);
        if(!Before(attrQueryStack_,"supervisor-query-stack"))return false;Returned(attrQueryStack_,pthread_attr_getstacksize(&attributes_,&actualStack_),false);
        if(!Before(attrQueryGuard_,"supervisor-query-guard"))return false;Returned(attrQueryGuard_,pthread_attr_getguardsize(&attributes_,&actualGuard_),false);
        if(attrStack_.returned||attrQueryStack_.returned||attrQueryGuard_.returned){failed_.store(true);return false;}
        if(!Before(timer_.acquisition,"supervisor-timer-create"))return false;Returned(timer_.acquisition,timerfd_create(CLOCK_MONOTONIC,TFD_CLOEXEC|TFD_NONBLOCK));
        timer_.original=static_cast<int>(timer_.acquisition.returned);
        if(timer_.original<0){failed_.store(true);return false;}
        if(!Before(wake_.acquisition,"supervisor-wake-create"))return false;Returned(wake_.acquisition,eventfd(0,EFD_CLOEXEC|EFD_NONBLOCK));
        wake_.original=static_cast<int>(wake_.acquisition.returned);
        if(wake_.original<0){failed_.store(true);return false;}
        if(!Before(responsePipeCreate_,"supervisor-response-pipe"))return false;
        Returned(responsePipeCreate_,pipe2(originalResponsePipe_.data(),O_NONBLOCK|O_CLOEXEC));
        if(responsePipeCreate_.returned==0 && originalResponsePipe_[0]>=0 &&
           originalResponsePipe_[1]>=0 && originalResponsePipe_[0]!=originalResponsePipe_[1]) {
            responseReader_.original=originalResponsePipe_[0];responseWriter_.original=originalResponsePipe_[1];
        }
        else if(originalResponsePipe_[0]>=0||originalResponsePipe_[1]>=0)responseOutputUnknown_=true;
        if(responsePipeCreate_.returned!=0||responseOutputUnknown_||responseReader_.original<0)
            {failed_.store(true);return false;}
        if(!Before(timerArm_,"supervisor-timer-arm"))return false;Returned(timerArm_,timerfd_settime(timer_.original,TFD_TIMER_ABSTIME,&nativeTimer_,nullptr));
        if(timerArm_.returned){failed_.store(true);return false;}
        if(!Before(threadCreate_,"supervisor-thread-create"))return false;Returned(threadCreate_,pthread_create(&originalThread_,&attributes_,ThreadMain,this),false);
        created_=threadCreate_.returned==0;
        Before(attrDestroy_,"supervisor-attr-destroy",true);Returned(attrDestroy_,pthread_attr_destroy(&attributes_),false);
        const bool afterStart=origin_.Observe("deadline-supervisor-start",true);
        started_=created_&&attrDestroy_.returned==0&&afterStart;
        if(!started_)failed_.store(true,std::memory_order_release);return started_;
    }
    // Exact actual blocking-call sites enter before their syscall and leave only
    // after storing returned/error/affected bytes in their own preowned row.
    bool BeginNativeCall() noexcept {
        if(!started_||stopAttempted_||disposed_||ExpiredOrFailed())return false;
        if(origin_.HasOriginalDeadline()&&(!originalReceiver_||
            !receiverCreated_.load(std::memory_order_acquire)||receiverFinished_.load(std::memory_order_acquire)))return false;
        if(origin_.HasOriginalDeadline()&&(!originalCaller_||
            !callerCreated_.load(std::memory_order_acquire)||callerFinished_.load(std::memory_order_acquire)))return false;
        size_t noPending=0;
        if(!pendingCalls_.compare_exchange_strong(noPending,1,std::memory_order_acq_rel))
            {failed_.store(true,std::memory_order_release);return false;}
        if(ExpiredOrFailed()){pendingCalls_.fetch_sub(1,std::memory_order_acq_rel);return false;}
        return true;
    }
    void OriginalNativeCallReturned() noexcept {
        size_t actualPending=1;
        if(!pendingCalls_.compare_exchange_strong(actualPending,0,std::memory_order_acq_rel))
            failed_.store(true,std::memory_order_release);
    }
    // Existing owned resource cleanup may remain eligible after qualification
    // expiry. It still needs the SAME resident observer/caller ownership and a
    // fresh actual pending slot; expiry is never reset or turned into success.
    bool BeginIndependentlyEligibleCleanupNativeCall() noexcept {
        if(!preowned_||!origin_.PreownedOriginal())return false;
        if(stopAttempted_||disposed_)return false;
        if(!started_){
            // Known no-deadline setup failure has no timer/observer contract to
            // invent. Keep the actual pending close slot and close future start,
            // without fabricating started/native thread/notice observations.
            if(origin_.HasOriginalDeadline()||created_)return false;
            cleanupStartupClosed_=true;
        }
        if(origin_.HasOriginalDeadline()&&(!originalReceiver_||!originalCaller_||
           !receiverCreated_.load(std::memory_order_acquire)||!callerCreated_.load(std::memory_order_acquire)))return false;
        size_t noPending=0;
        if(!pendingCalls_.compare_exchange_strong(noPending,1,std::memory_order_acq_rel)) {
            failed_.store(true,std::memory_order_release);return false;
        }
        return true;
    }
    bool ExpiredOrFailed() const noexcept {return expired_.load(std::memory_order_acquire)||failed_.load(std::memory_order_acquire);}
    bool HasPendingOriginalNativeCall() const noexcept {return pendingCalls_.load(std::memory_order_acquire)!=0;}
    bool StopJoinDisposeEligible() noexcept {
        if(pendingCalls_.load(std::memory_order_acquire))return false;
        if(disposed_)return true;
        if(created_&&!workerReturned_.load(std::memory_order_acquire)&&!stopAttempted_) {
            stopAttempted_=true;Before(stopWrite_,"supervisor-stop-write",true);
            Returned(stopWrite_,write(wake_.original,&originalStopValue_,sizeof(originalStopValue_)));
            if(stopWrite_.returned!=8)failed_.store(true,std::memory_order_release);
        }
        if(created_&&!joined_) {
            if(!workerReturned_.load(std::memory_order_acquire)||joinUnknown_||joinCount_==joinCapacity_)return false;
            const size_t ordinal=joinCount_++;auto& row=joins_[ordinal];Before(row,"supervisor-tryjoin",true);
            Returned(row,pthread_tryjoin_np(originalThread_,&joinReturns_[ordinal]),false);
            if(row.returned==0){joined_=true;if(joinReturns_[ordinal])failed_.store(true,std::memory_order_release);}
            else if(row.returned!=EBUSY){joinUnknown_=true;failed_.store(true,std::memory_order_release);}
            if(!joined_)return false;
        }
        if(attributesAcquired_&&!attrDestroy_.attempted) {
            Before(attrDestroy_,"supervisor-attr-destroy",true);Returned(attrDestroy_,pthread_attr_destroy(&attributes_),false);
        }
        const bool timerClosed=timer_.CloseOnce(origin_),wakeClosed=wake_.CloseOnce(origin_);
        const bool responseReaderClosed=(!responseWrite_.attempted||responseReadVerified_.load(std::memory_order_acquire))
            ?responseReader_.CloseOnce(origin_):false;
        const bool responseWriterClosed=responseWriter_.CloseOnce(origin_);
        for(const auto* fd:{&timer_,&wake_,&responseReader_,&responseWriter_}) if(fd->closeOriginal.attempted &&
            (!fd->closeOriginal.beforeClock.shapeValid||!fd->closeOriginal.afterClock.shapeValid||
             fd->closeOriginal.beforeClock.expired||fd->closeOriginal.afterClock.expired||
             fd->closeOriginal.afterClock.nanoseconds<fd->closeOriginal.beforeClock.nanoseconds))
            failed_.store(true,std::memory_order_release);
        disposed_=(!receiverCreated_.load(std::memory_order_acquire)||receiverJoined_.load(std::memory_order_acquire))&&
            !responseOutputUnknown_&&timerClosed&&wakeClosed&&responseReaderClosed&&responseWriterClosed&&(!created_||joined_)&&
            (!attributesAcquired_||(attrDestroy_.returnedObserved&&attrDestroy_.returned==0));
        return disposed_;
    }
    private:
    // The independent native host receiver may consume this SAME source-owned
    // read endpoint while the original ioctl/pread caller remains pending.
    // No numeric FD, response bytes or source flag is accepted as role authority.
    bool ReadPendingResponseOriginal() noexcept {
        if(nativePublication_.load(std::memory_order_acquire)!=1)return false;
        if(responseReadAttempted_)return responseReadVerified_.load(std::memory_order_acquire);
        responseReadAttempted_=true;
        struct Complete {std::atomic<bool>& output;~Complete(){output.store(true,std::memory_order_release);}}
            complete{responseReadCompleted_};
        if(!responseWrite_.attempted||!responseWrite_.returnedObserved||
            responseWrite_.returned>static_cast<long>(sizeof(originalResponse_))||responseReader_.closeOriginal.attempted)
            return false;
        const size_t expected=responseWrite_.returned>0?static_cast<size_t>(responseWrite_.returned):0;
        while(responseReadExtent_<expected) {
            // Every continuing iteration must consume at least one byte; the
            // body size therefore derives the full maximum native read roster.
            if(responseReadCount_==responseReads_.size())return false;
            auto& row=responseReads_[responseReadCount_++];
            Before(row,"supervisor-response-receive",true);
            const size_t requested=expected-responseReadExtent_;
            row.requested=requested;row.destinationOffset=responseReadExtent_;
            Returned(row,read(responseReader_.original,responseReadBytes_.data()+responseReadExtent_,requested));
            if(row.returned<=0||static_cast<size_t>(row.returned)>requested)return false;
            responseReadExtent_+=static_cast<size_t>(row.returned);
        }
        responseReadVerified_.store((!expected||std::memcmp(responseReadBytes_.data(),&originalResponse_,expected)==0),
            std::memory_order_release);
        responseNoticeComplete_.store(expected==sizeof(originalResponse_)&&
            responseReadVerified_.load(std::memory_order_acquire),std::memory_order_release);
        if(!responseReadVerified_.load(std::memory_order_acquire))failed_.store(true,std::memory_order_release);
        return responseReadVerified_.load(std::memory_order_acquire);
    }
public:
    const OriginalDeadline* OriginalDeadlineOwner() const noexcept{return &origin_;}
    bool PhysicalDisposed() const noexcept{return disposed_;}
    bool WorkerOriginalsPublished() const noexcept{return workerReturned_.load(std::memory_order_acquire);}
    // Direct native-original capture is intentionally private; enclosing session
    // serializes these fixed cells only after actual worker publication/join.
    friend class OriginalLinuxPlatformState;
    friend class OriginalDeadlineReceiver;
    friend class OriginalLinuxPendingInputOwner;
    friend class OriginalLinuxCallerReceipt;
    friend bool VerifyActualDeadlineNotification(const OriginalDeadlineSupervisor&) noexcept;
    friend bool VerifyActualNoDeadlineSupervisor(const OriginalDeadlineSupervisor&) noexcept;
    bool ReservedSourceBytes(size_t& bytes) const noexcept {
        bytes=sizeof(*this);
        auto add=[&](size_t n){if(n>SIZE_MAX-bytes)return false;bytes+=n;return true;};
        if(!add(joinCapacity_*sizeof(SupervisorNativeOriginal))||!add(joinReturnCapacity_*sizeof(void*)))return false;
        if(origin_.HasOriginalDeadline()) {
            if(!attrQueryStack_.returnedObserved||attrQueryStack_.returned||
               !attrQueryGuard_.returnedObserved||attrQueryGuard_.returned)return false;
            if(!add(actualStack_)||!add(actualGuard_))return false;
        }
        return true; // Kernel, TLS, libc, allocator and runtime referents additional.
    }
};
}

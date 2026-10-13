// UNCOMPILED same-process native receipt. This observes the retained creating
// thread while it is alive; it is NOT an external process-exit witness or a
// source/role/native authorization. Python originals remain on the live main.
#pragma once
#include <windows.h>
#include <atomic>
#include <cstdint>
#include <memory>
#include <stdexcept>
namespace template_creator_bridge {
struct OriginalWindowsNativeMain;
struct OriginalMainReceiptPoll {
    DWORD waited=WAIT_FAILED,error=0,exitCode=0;
    BOOL timesReturned=FALSE,exitReturned=FALSE;
    DWORD timesError=0,exitError=0;
    FILETIME born{},exited{},kernel{},user{};
};
class OriginalWindowsMainReceipt {
    friend struct OriginalWindowsNativeMain;
    ~OriginalWindowsMainReceipt()=default;
    HANDLE creatingThread_=nullptr,worker_=nullptr;
    FILETIME expectedBirth_{};
    SIZE_T selectedStackBytes_=0,capacity_=0,count_=0;
    LPSECURITY_ATTRIBUTES creationSecurityOriginal_=nullptr;
    DWORD creationFlagsOriginal_=0;
    std::unique_ptr<OriginalMainReceiptPoll[]> polls_;
    std::atomic<bool> published_{false};
    bool qualificationFailed_=false;
    static DWORD WINAPI Receive(void* opaque) noexcept {
        auto& self=*static_cast<OriginalWindowsMainReceipt*>(opaque);
        self.receiverThreadId=GetCurrentThreadId();
        self.creatingWait=WaitForSingleObject(self.creatingThread_,0);
        if(self.creatingWait==WAIT_FAILED) self.creatingWaitError=GetLastError();
        self.creatingTimesReturned=GetThreadTimes(self.creatingThread_,&self.creatingBorn,
            &self.creatingExited,&self.creatingKernel,&self.creatingUser);
        if(!self.creatingTimesReturned) self.creatingTimesError=GetLastError();
        self.sameCreatingLifetime=self.creatingWait==WAIT_TIMEOUT && self.creatingTimesReturned &&
            self.creatingBorn.dwLowDateTime==self.expectedBirth_.dwLowDateTime &&
            self.creatingBorn.dwHighDateTime==self.expectedBirth_.dwHighDateTime;
        self.published_.store(true,std::memory_order_release);
        return self.sameCreatingLifetime?0:1;
    }
public:
    bool startAttempted=false,createAttempted=false,createCompleted=false,creationUnknown=false,acquired=false,exhausted=false;
    DWORD createError=0,createdThreadId=0,receiverThreadId=0;
    DWORD creatingWait=WAIT_FAILED,creatingWaitError=0,creatingTimesError=0;
    BOOL creatingTimesReturned=FALSE;
    FILETIME creatingBorn{},creatingExited{},creatingKernel{},creatingUser{};
    bool sameCreatingLifetime=false,terminal=false,closeAttempted=false;
    BOOL closeReturned=FALSE;
    DWORD closeError=0;
    explicit OriginalWindowsMainReceipt(SIZE_T selectedPolls,SIZE_T selectedStackBytes)
        :selectedStackBytes_(selectedStackBytes),capacity_(selectedPolls) {
        if(!selectedPolls || selectedPolls>(SIZE_MAX-sizeof(*this))/sizeof(OriginalMainReceiptPoll) || !selectedStackBytes)
            throw std::invalid_argument("precharged native main receipt");
        polls_=std::make_unique<OriginalMainReceiptPoll[]>(selectedPolls);
    }
    OriginalWindowsMainReceipt(const OriginalWindowsMainReceipt&)=delete;
    OriginalWindowsMainReceipt& operator=(const OriginalWindowsMainReceipt&)=delete;
    // Owner is resident until actual worker terminal+once-close. No destructor
    // closes a handle or frees a destination still owned by a native worker.
    bool Start(HANDLE actualCreatingThread,const FILETIME& actualBirth) noexcept {
        if(startAttempted) return false;
        startAttempted=true; creatingThread_=actualCreatingThread; expectedBirth_=actualBirth;
        if(!creatingThread_ || creatingThread_==INVALID_HANDLE_VALUE) return false;
        creationSecurityOriginal_=nullptr;
        creationFlagsOriginal_=STACK_SIZE_PARAM_IS_A_RESERVATION;
        createAttempted=true;
        worker_=CreateThread(creationSecurityOriginal_,selectedStackBytes_,Receive,this,creationFlagsOriginal_,&createdThreadId);
        if(!worker_) createError=GetLastError();
        createCompleted=true;
        if(!worker_) {
            creationUnknown=createdThreadId!=0;
            qualificationFailed_=true; return false;
        }
        acquired=worker_!=INVALID_HANDLE_VALUE;
        if(!acquired || !createdThreadId) { creationUnknown=true; qualificationFailed_=true; return false; }
        return true;
    }
    bool Poll() noexcept {
        if(!acquired || creationUnknown) return false;
        if(closeAttempted) return closeReturned!=FALSE;
        if(count_==capacity_) { exhausted=true; return false; }
        auto& row=polls_[count_++];
        row.waited=WaitForSingleObject(worker_,0);
        if(row.waited==WAIT_FAILED) row.error=GetLastError();
        row.timesReturned=GetThreadTimes(worker_,&row.born,&row.exited,&row.kernel,&row.user);
        if(!row.timesReturned) row.timesError=GetLastError();
        row.exitReturned=GetExitCodeThread(worker_,&row.exitCode);
        if(!row.exitReturned) row.exitError=GetLastError();
        if((row.waited!=WAIT_TIMEOUT && row.waited!=WAIT_OBJECT_0) || !row.timesReturned || !row.exitReturned)
            qualificationFailed_=true;
        if(row.waited!=WAIT_OBJECT_0) return false;
        terminal=true; // Actual wait is physical termination even if queries fail.
        closeAttempted=true; closeReturned=CloseHandle(worker_);
        if(!closeReturned) { closeError=GetLastError(); qualificationFailed_=true; }
        return closeReturned!=FALSE;
    }
    bool Published() const noexcept { return published_.load(std::memory_order_acquire); }
    bool NoWorkerCreated() const noexcept { return createAttempted && createCompleted && !worker_ && !acquired && !creationUnknown; }
    bool PhysicallyDisposed() const noexcept { return acquired && !creationUnknown && terminal && closeAttempted && closeReturned; }
    bool Qualified() const noexcept {
        if(qualificationFailed_ || !PhysicallyDisposed() || !Published() || !sameCreatingLifetime || exhausted || !count_) return false;
        const auto& row=polls_[count_-1];
        return row.waited==WAIT_OBJECT_0 && row.timesReturned && row.exitReturned && row.exitCode==0 &&
            receiverThreadId==createdThreadId;
    }
    SIZE_T ReservedSourceBytes() const noexcept { return sizeof(*this)+capacity_*sizeof(OriginalMainReceiptPoll); }
    SIZE_T SelectedStackReservationBytes() const noexcept { return selectedStackBytes_; }
    const OriginalMainReceiptPoll* PrivatePollOriginals() const noexcept { return polls_.get(); }
    SIZE_T PrivatePollCount() const noexcept { return count_; }
    HANDLE OriginalWorkerHandle() const noexcept { return worker_; }
    HANDLE OriginalCreatingThreadHandle() const noexcept { return creatingThread_; }
    // Native stack rounding/commit, kernel/thread/provider and allocator costs
    // still require actual whole-runtime measurement; source bytes are not fit.
};
}

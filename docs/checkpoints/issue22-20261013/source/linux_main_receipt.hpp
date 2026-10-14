// UNCOMPILED same-process creating-main observation. No external exit proof.
#pragma once
#ifndef _WIN32
#include "linux_task_identity.hpp"
#include <pthread.h>
#include <atomic>
#include "linux_native_sink.hpp"
namespace template_creator_bridge {class OriginalLinuxNativeMain;}
namespace template_creator_linux {
class OriginalLinuxMainReceipt {
    friend class template_creator_bridge::OriginalLinuxNativeMain;
    ~OriginalLinuxMainReceipt()=default; // Resident through actual join.
    std::unique_ptr<ProcTaskObservation> observation_;
    std::unique_ptr<SupervisorNativeOriginal[]> joins_;
    std::unique_ptr<void*[]> joinValues_;
    size_t joinsCapacity_=0,joinCount_=0,selectedStack_=0,actualStack_=0,actualGuard_=0;
    HeldTaskIdentity* mainTask_=nullptr;
    const ProcTaskObservation* baseline_=nullptr;
    OriginalDeadline* deadline_=nullptr;
    OriginalDeadlineSupervisor* supervisor_=nullptr;
    pthread_attr_t attributes_{};pthread_t worker_{};
    SupervisorNativeOriginal attrInit_,attrStack_,stackQuery_,guardQuery_,create_,attrDestroy_,workerTid_,finalJoin_;
    void* finalJoinValue_=nullptr;bool finalJoinAdmissionAttempted_=false;
    std::atomic<bool> startPublished_{false},returned_{false};
    bool attempted_=false,attributesAcquired_=false,created_=false,joined_=false,joinUnknown_=false;
    bool sameMain_=false,workerFailed_=false,mainFailed_=false;
    bool Before(SupervisorNativeOriginal& row,const char* role) noexcept {
        if(!deadline_||!supervisor_||row.attempted)return false;
        const bool clock=CaptureSupervisorClock(row.beforeClock,*deadline_,role,false);
        if(!clock)mainFailed_=true;
        if(!supervisor_->BeginIndependentlyEligibleCleanupNativeCall())return false;
        row.attempted=true;return true;
    }
    void Returned(SupervisorNativeOriginal& row,long result,bool errnoStyle=false) noexcept {
        row.returned=result;
        if(errnoStyle&&result<0)row.error=errno;
        if(!errnoStyle&&result)row.error=static_cast<int>(result);
        row.returnedObserved=true;supervisor_->OriginalNativeCallReturned();
        if(!CaptureSupervisorClock(row.afterClock,*deadline_,row.beforeClock.operation,true)||
           !row.beforeClock.shapeValid||!row.afterClock.shapeValid||row.afterClock.nanoseconds<row.beforeClock.nanoseconds)
            mainFailed_=true;
    }
    static void* Receive(void* opaque) noexcept {
        auto& self=*static_cast<OriginalLinuxMainReceipt*>(opaque);
        while(!self.startPublished_.load(std::memory_order_acquire)){}
        // Only worker-owned cells below; no Python API or main-owned flags.
        self.workerTid_.beforeClock.operation="main-receipt-gettid";
        const bool before=CaptureSupervisorClock(self.workerTid_.beforeClock,*self.deadline_,"main-receipt-gettid",false);
        if(!self.supervisor_->BeginIndependentlyEligibleCleanupNativeCall())self.workerFailed_=true;
        else {
            self.workerTid_.attempted=true;self.workerTid_.returned=syscall(SYS_gettid);
            if(self.workerTid_.returned<0)self.workerTid_.error=errno;
            self.workerTid_.returnedObserved=true;self.supervisor_->OriginalNativeCallReturned();
            const bool after=CaptureSupervisorClock(self.workerTid_.afterClock,*self.deadline_,"main-receipt-gettid",true);
            if(!before||!after||self.workerTid_.returned<=0||
               self.workerTid_.afterClock.nanoseconds<self.workerTid_.beforeClock.nanoseconds)self.workerFailed_=true;
        }
        auto& row=*self.observation_;row.attempted=true;
        const int directory=self.mainTask_->OriginalDirectory().original;
        const bool stat=row.stat.Capture(directory,"stat",*self.deadline_,self.supervisor_,true);
        const bool status=row.status.Capture(directory,"status",*self.deadline_,self.supervisor_,true);
        row.parsed=stat&&status&&row.Parse();
        self.sameMain_=row.parsed&&self.mainTask_->SameOriginal(*self.baseline_)&&
            row.observedPid==self.baseline_->observedPid&&row.observedTgid==self.baseline_->observedTgid&&
            row.startTicks==self.baseline_->startTicks&&row.observedTracer==self.baseline_->observedTracer;
        if(!self.sameMain_)self.workerFailed_=true;
        self.returned_.store(true,std::memory_order_release);return nullptr;
    }
public:
    OriginalLinuxMainReceipt(size_t statBytes,size_t statusBytes,size_t reads,size_t joins,size_t stack)
        :joinsCapacity_(joins),selectedStack_(stack){
        if(!joins||joins>SIZE_MAX/sizeof(SupervisorNativeOriginal)||joins>SIZE_MAX/sizeof(void*)||!stack)
            throw std::invalid_argument("precharged main receipt geometry");
        const size_t taskBytes=ProcTaskObservation::CheckedSourceBacking(statBytes,statusBytes,reads);
        // Bound the entire allocation graph before constructing its first
        // nested owner; each valid product alone does not bound their sum.
        size_t sourceBytes=sizeof(*this);
        const auto add=[&](size_t n) {
            if(n>SIZE_MAX-sourceBytes)return false;sourceBytes+=n;return true;
        };
        if(!add(taskBytes)||
           !add(joins*sizeof(SupervisorNativeOriginal))||!add(joins*sizeof(void*)))
            throw std::invalid_argument("aggregate main receipt source backing");
        // This does not measure allocator, native stack/guard, TLS or kernel fit.
        observation_=std::make_unique<ProcTaskObservation>(statBytes,statusBytes,reads);
        joins_=std::make_unique<SupervisorNativeOriginal[]>(joins);
        joinValues_=std::make_unique<void*[]>(joins);
    }
    OriginalLinuxMainReceipt(const OriginalLinuxMainReceipt&)=delete;
    OriginalLinuxMainReceipt& operator=(const OriginalLinuxMainReceipt&)=delete;
    bool Start(HeldTaskIdentity& actualMain,OriginalDeadline& origin,OriginalDeadlineSupervisor& supervisor,pid_t expectedTid) noexcept {
        if(attempted_)return false;attempted_=true;
        mainTask_=&actualMain;deadline_=&origin;supervisor_=&supervisor;
        if(actualMain.OriginalDeadlineOwner()!=&origin||supervisor.OriginalDeadlineOwner()!=&origin||
           !actualMain.ObservationCount()||actualMain.OriginalDirectory().closeAttempted){mainFailed_=true;return false;}
        baseline_=&actualMain.OriginalObservation(0);
        if(!actualMain.SameOriginal(*baseline_)||baseline_->observedPid!=expectedTid){mainFailed_=true;return false;}
        if(!Before(attrInit_,"main-receipt-attr-init"))return false;
        Returned(attrInit_,pthread_attr_init(&attributes_));attributesAcquired_=attrInit_.returned==0;
        if(!attributesAcquired_){mainFailed_=true;return false;}
        bool ready=Before(attrStack_,"main-receipt-attr-stack");
        if(ready){Returned(attrStack_,pthread_attr_setstacksize(&attributes_,selectedStack_));ready=attrStack_.returned==0;}
        if(ready&&Before(stackQuery_,"main-receipt-stack-query")){Returned(stackQuery_,pthread_attr_getstacksize(&attributes_,&actualStack_));ready=stackQuery_.returned==0;}
        else ready=false;
        if(ready&&Before(guardQuery_,"main-receipt-guard-query")){Returned(guardQuery_,pthread_attr_getguardsize(&attributes_,&actualGuard_));ready=guardQuery_.returned==0;}
        else ready=false;
        if(ready&&Before(create_,"main-receipt-create")){Returned(create_,pthread_create(&worker_,&attributes_,Receive,this));created_=create_.returned==0;}
        if(!created_)mainFailed_=true;
        if(Before(attrDestroy_,"main-receipt-attr-destroy")){Returned(attrDestroy_,pthread_attr_destroy(&attributes_));if(attrDestroy_.returned)mainFailed_=true;}
        else mainFailed_=true;
        startPublished_.store(true,std::memory_order_release);
        return created_;
    }
    bool PollEligibleJoin() noexcept {
        if(!created_||joinUnknown_)return false;
        if(joined_)return true;
        if(!returned_.load(std::memory_order_acquire))return false;
        if(joinCount_==joinsCapacity_){
            mainFailed_=true;
            if(finalJoinAdmissionAttempted_)return false;
            finalJoinAdmissionAttempted_=true;
            // Returned publication does not prove TLS/runtime exit completion.
            // SAME original observers/caller stay live through actual join.
            if(!Before(finalJoin_,"main-receipt-final-join"))return false;
            Returned(finalJoin_,pthread_join(worker_,&finalJoinValue_));
            if(finalJoin_.returned){joinUnknown_=true;return false;}
            joined_=true;if(finalJoinValue_)mainFailed_=true;return true;
        }
        const size_t ordinal=joinCount_++;auto& row=joins_[ordinal];
        if(!Before(row,"main-receipt-tryjoin"))return false;
        Returned(row,pthread_tryjoin_np(worker_,&joinValues_[ordinal]));
        if(row.returned==0)joined_=true;
        else if(row.returned!=EBUSY){joinUnknown_=true;mainFailed_=true;}
        return joined_;
    }
    bool BorrowerSettled() const noexcept {
        return !attempted_||(!created_&&(!create_.attempted||(create_.returnedObserved&&create_.returned!=0)))||joined_;
    }
    bool Qualified() const noexcept {
        return joined_&&returned_.load(std::memory_order_acquire)&&!mainFailed_&&!workerFailed_&&sameMain_&&
            attrDestroy_.returnedObserved&&attrDestroy_.returned==0&&
            (finalJoin_.returnedObserved?finalJoin_.returned==0&&finalJoinValue_==nullptr:
                joinCount_&&joinValues_[joinCount_-1]==nullptr);
    }
    bool ReservedSourceBytes(size_t& bytes) const noexcept {
        bytes=sizeof(*this)+sizeof(ProcTaskObservation);
        const auto add=[&](size_t n){if(n>SIZE_MAX-bytes)return false;bytes+=n;return true;};
        return add(observation_->stat.byteCapacity+1)&&add(observation_->status.byteCapacity+1)&&
            add(observation_->stat.readCapacity*sizeof(ProcReadOriginal))&&add(observation_->status.readCapacity*sizeof(ProcReadOriginal))&&
            add(joinsCapacity_*sizeof(SupervisorNativeOriginal))&&add(joinsCapacity_*sizeof(void*));
    }
    size_t SelectedStackBytes() const noexcept{return selectedStack_;}
    size_t ActualStackBytesAfterJoin() const noexcept{return joined_?actualStack_:0;}
    size_t ActualGuardBytesAfterJoin() const noexcept{return joined_?actualGuard_:0;}
    const ProcTaskObservation* PrivateMainObservationAfterJoin() const noexcept{return joined_?observation_.get():nullptr;}
    const SupervisorNativeOriginal* PrivateWorkerTidAfterJoin() const noexcept{return joined_?&workerTid_:nullptr;}
    const SupervisorNativeOriginal* PrivateJoinOriginals() const noexcept{return joins_.get();}
    size_t PrivateJoinCount() const noexcept{return joinCount_;}
    void* const* PrivateJoinValues() const noexcept{return joinValues_.get();}
    bool CaptureSettledOriginals(OriginalNativeSink& sink) const noexcept {
        if(!BorrowerSettled())return false;
        struct Header{uint64_t version,kind,extent;};
        const auto append=[&](uint64_t kind,const void* bytes,size_t extent) noexcept {
            const Header header{1,kind,extent};
            return sink.Append(reinterpret_cast<const unsigned char*>(&header),sizeof(header))&&
                sink.Append(static_cast<const unsigned char*>(bytes),extent);
        };
        bool copied=append(1,this,sizeof(*this));
        copied=append(2,joins_.get(),joinCount_*sizeof(SupervisorNativeOriginal))&&copied;
        copied=append(3,joinValues_.get(),joinCount_*sizeof(void*))&&copied;
        // No worker-owned observation access without its actual completed join.
        if(joined_){
            copied=append(4,observation_.get(),sizeof(*observation_))&&copied;
            const auto file=[&](const ProcFileObservation& row,uint64_t kind) noexcept {
                if(row.used>row.byteCapacity+1||row.readCount>row.readCapacity)return false;
                return append(kind,row.bytes.get(),row.used)&&
                    append(kind+1,row.reads.get(),row.readCount*sizeof(ProcReadOriginal));
            };
            copied=file(observation_->stat,5)&&copied;copied=file(observation_->status,7)&&copied;
        }
        return copied;
    }
    // No accessor exposes worker-mutated outputs before acquire+actual join.
};
}
#endif

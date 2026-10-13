// UNCOMPILED source-owned post-platform evidence destination. No role grant.
#pragma once
#include "linux_sink_collector.hpp"
#include "linux_initial_objects.hpp"
#include <sys/random.h>
namespace template_creator_bridge {
class OriginalLinuxFinalOutput {
    friend struct NativePlatformState;
    template_creator_linux::OriginalDeadline& deadline_;
    template_creator_linux::OriginalDeadlineSupervisor& supervisor_;
    template_creator_linux::SupervisorNativeOriginal duplicate_,rootStat_,randomCall_,create_,fileStat_,fileClose_,rootClose_;
    struct stat rootMetadata_{},fileMetadata_{};
    std::array<unsigned char,16> random_{};
    std::array<char,64> name_{};
    int root_=-1,file_=-1;
    bool attempted_=false,rootAcquired_=false,fileAcquired_=false,adopted_=false;
    bool failed_=false,collectorAttempted_=false,collectorStarted_=false,collectorJoined_=false;
    bool dispositionRequested_=false;
    bool preownAttempted_=false,preowned_=false;
    std::exception_ptr preownException_;
    bool inputCutAdmitted_=false;
    void AdmitOwnedInputCut() noexcept {
        if(!dispositionRequested_){inputCutAdmitted_=true;sink.finalCaptureAdmitted_=true;}
    }
    bool Begin(template_creator_linux::SupervisorNativeOriginal& row,const char* operation,bool cleanup=false) noexcept {
        const bool clock=template_creator_linux::CaptureSupervisorClock(row.beforeClock,deadline_,operation,false);
        if(!clock){failed_=true;if(!cleanup)return false;}
        if(!(cleanup?supervisor_.BeginIndependentlyEligibleCleanupNativeCall():supervisor_.BeginNativeCall())){
            failed_=true;return false;
        }
        row.attempted=true;return true;
    }
    void Returned(template_creator_linux::SupervisorNativeOriginal& row,long result,int error) noexcept {
        row.returned=result;row.error=error;row.returnedObserved=true;
        supervisor_.OriginalNativeCallReturned();
        if(!template_creator_linux::CaptureSupervisorClock(row.afterClock,deadline_,row.beforeClock.operation,true)||
           !row.beforeClock.shapeValid||!row.afterClock.shapeValid||row.afterClock.nanoseconds<row.beforeClock.nanoseconds)
            failed_=true;
    }
    ~OriginalLinuxFinalOutput()=default; // Native main retains through actual join.
public:
    template_creator_linux::OriginalNativeSink sink;
    template_creator_linux::OriginalLinuxSinkCollector collector;
    OriginalLinuxFinalOutput(template_creator_linux::DeferredOriginalPreownTag,size_t bytes,size_t calls,size_t joins,size_t stack,
        template_creator_linux::OriginalDeadline& deadline,template_creator_linux::OriginalDeadlineSupervisor& supervisor) noexcept
        :deadline_(deadline),supervisor_(supervisor),sink(template_creator_linux::DeferredOriginalPreown,bytes,calls,deadline,supervisor,true),
         collector(template_creator_linux::DeferredOriginalPreown,sink,bytes,calls,joins,stack){}
    OriginalLinuxFinalOutput(size_t bytes,size_t calls,size_t joins,size_t stack,
        template_creator_linux::OriginalDeadline& deadline,template_creator_linux::OriginalDeadlineSupervisor& supervisor)
        :OriginalLinuxFinalOutput(template_creator_linux::DeferredOriginalPreown,bytes,calls,joins,stack,deadline,supervisor){
        if(!PreownOriginal()&&preownException_)std::rethrow_exception(preownException_);
    }
    bool PreownOriginal() noexcept {
        if(preownAttempted_)return false;preownAttempted_=true;
        if(!sink.PreownOriginal()){preownException_=sink.PreownExceptionOriginal();failed_=true;return false;}
        if(!collector.PreownOriginal()){preownException_=collector.PreownExceptionOriginal();failed_=true;return false;}
        preowned_=true;return true;
    }
    bool PreownedOriginal() const noexcept{return preowned_;}
    const std::exception_ptr& PreownExceptionOriginal() const noexcept{return preownException_;}
    OriginalLinuxFinalOutput(const OriginalLinuxFinalOutput&)=delete;
    OriginalLinuxFinalOutput& operator=(const OriginalLinuxFinalOutput&)=delete;
    bool CreateBeforeIngressClosure(const template_creator_linux::OriginalLinuxInitialObjects& initial) noexcept {
        if(!preowned_||dispositionRequested_||attempted_)return false;attempted_=true;
        if(!initial.Captured()){failed_=true;return false;}
        const auto& evidence=initial.PrivateOriginals()[2];
        const auto& candidate=initial.PrivateOriginals()[0];
        if(evidence.originalFd<0||evidence.call.returned!=0||candidate.call.returned!=0||
           !Begin(duplicate_,"final-output-root-duplicate")){failed_=true;return false;}
        const long duplicated=fcntl(evidence.originalFd,F_DUPFD_CLOEXEC,0);const int dupError=duplicated<0?errno:0;
        root_=static_cast<int>(duplicated);rootAcquired_=duplicated>=0;Returned(duplicate_,duplicated,dupError);
        if(!rootAcquired_||!Begin(rootStat_,"final-output-root-fstat"))return false;
        const long statResult=fstat(root_,&rootMetadata_);const int statError=statResult<0?errno:0;Returned(rootStat_,statResult,statError);
        if(statResult||!S_ISDIR(rootMetadata_.st_mode)||rootMetadata_.st_dev!=evidence.output.st_dev||
           rootMetadata_.st_ino!=evidence.output.st_ino||
           (rootMetadata_.st_dev==candidate.output.st_dev&&rootMetadata_.st_ino==candidate.output.st_ino)){
            failed_=true;return false;
        }
        if(!Begin(randomCall_,"final-output-name-random"))return false;
        randomCall_.requested=random_.size();const long randomResult=getrandom(random_.data(),random_.size(),0);
        const int randomError=randomResult<0?errno:0;Returned(randomCall_,randomResult,randomError);
        if(randomResult!=static_cast<long>(random_.size())){failed_=true;return false;}
        constexpr char prefix[]="creator-final-",hex[]="0123456789abcdef";
        size_t used=0;for(;used<sizeof(prefix)-1;++used)name_[used]=prefix[used];
        for(auto byte:random_){name_[used++]=hex[byte>>4];name_[used++]=hex[byte&15];}name_[used]=0;
        if(!Begin(create_,"final-output-openat-exclusive"))return false;
        const long opened=openat(root_,name_.data(),O_RDWR|O_CREAT|O_EXCL|O_CLOEXEC|O_NOFOLLOW,0600);
        const int openError=opened<0?errno:0;file_=static_cast<int>(opened);fileAcquired_=opened>=0;
        Returned(create_,opened,openError);
        if(!fileAcquired_||!Begin(fileStat_,"final-output-file-fstat"))return false;
        const long actual=fstat(file_,&fileMetadata_);const int actualError=actual<0?errno:0;Returned(fileStat_,actual,actualError);
        if(actual||!S_ISREG(fileMetadata_.st_mode)||fileMetadata_.st_size!=0){failed_=true;return false;}
        adopted_=sink.AdoptOriginal(file_);if(!adopted_)failed_=true;return adopted_;
    }
    bool AppendFinalOriginals(const unsigned char* bytes,size_t extent) noexcept {
        if(dispositionRequested_||!inputCutAdmitted_||!adopted_||collectorAttempted_){failed_=true;return false;}
        const bool result=sink.Append(bytes,extent);if(!result)failed_=true;return result;
    }
    bool CanAppendFinalSpanOriginal(size_t extent) const noexcept {
        // Storage-only preflight. Native write rows still reserve individually;
        // this is neither financing nor a promise about future short writes.
        return !dispositionRequested_&&inputCutAdmitted_&&adopted_&&!collectorAttempted_&&
            !sink.extentUnknown_&&!sink.finalWriteBlocked_&&sink.copiedExtent_<=sink.bytesCapacity_&&
            extent<=sink.bytesCapacity_-sink.copiedExtent_;
    }
    bool StartFinalReadbackAfterLastAppend() noexcept {
        if(dispositionRequested_||!inputCutAdmitted_||collectorAttempted_||!adopted_)return false;collectorAttempted_=true;
        const bool flushed=sink.FlushReadbackOriginal();if(!flushed)failed_=true;
        collectorStarted_=collector.Start();if(!collectorStarted_)failed_=true;return collectorStarted_;
    }
    bool ObserveEligibleFinalDisposition() noexcept {
        dispositionRequested_=true; // Freeze future acquisitions/appends even on refusal.
        if(collectorAttempted_){collectorJoined_=collector.PollEligibleJoin();if(!collectorJoined_)return false;}
        if(!sink.AllAttemptedOriginalCallsReturned()||supervisor_.HasPendingOriginalNativeCall())return false;
        // Every independent eligible original is attempted despite earlier error.
        if(fileAcquired_&&!fileClose_.attempted&&Begin(fileClose_,"final-output-file-close",true)){
            const long result=close(file_);const int error=result<0?errno:0;Returned(fileClose_,result,error);if(result)failed_=true;
        }
        if(rootAcquired_&&!rootClose_.attempted&&Begin(rootClose_,"final-output-root-close",true)){
            const long result=close(root_);const int error=result<0?errno:0;Returned(rootClose_,result,error);if(result)failed_=true;
        }
        return PhysicalDisposed();
    }
    bool PhysicalDisposed() const noexcept {
        return (!collectorAttempted_||collectorJoined_)&&(!fileAcquired_||(fileClose_.returnedObserved&&!fileClose_.returned))&&
            (!rootAcquired_||(rootClose_.returnedObserved&&!rootClose_.returned))&&sink.AllAttemptedOriginalCallsReturned();
    }
    bool Qualified() const noexcept{return adopted_&&collectorStarted_&&collectorJoined_&&collector.Qualified()&&!failed_&&PhysicalDisposed();}
    bool Adopted() const noexcept{return adopted_;}
    bool ReservedSourceBytes(size_t& bytes) const noexcept {
        bytes=sizeof(*this);size_t n=0;
        if(!sink.ReservedSourceBytes(n)||n<sizeof(sink)||n-sizeof(sink)>SIZE_MAX-bytes)return false;bytes+=n-sizeof(sink);
        if(!collector.ReservedSourceBytes(n)||n<sizeof(collector)||n-sizeof(collector)>SIZE_MAX-bytes)return false;
        bytes+=n-sizeof(collector);return true;
    }
    // Caller must actually attach/preown this owner before Create and retain all
    // backing until joined. Native source/rights, aliases, exclusion and whole
    // physical fit remain independent proof; source sharing is no admission.
};
}

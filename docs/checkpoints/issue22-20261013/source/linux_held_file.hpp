// UNCOMPILED native physical input capture. Matching bytes/identity are not
// immutable execution-view, namespace, caller or tool authority.
#pragma once
#include "linux_ingress.hpp"
#include "linux_deadline_supervisor.hpp"
#include "original_sha256.hpp"
#include <memory>
#include <stdexcept>
#include <fcntl.h>
#include <limits>
namespace template_creator_linux {
struct HeldFileCall {
    enum class Operation {Open,BeforeStat,Read,Eof,AfterStat,Close};
    Operation operation{};
    IngressNativeClock clock;
    bool attempted=false,returnedObserved=false,affectedExtentUnknown=false;
    ssize_t returned=-1;
    int error=0;
    size_t requested=0,destinationOffset=0;
    int originalDirectory=-1,originalOpenFlags=0;
    bool procExeCandidate=false;
    struct stat metadata{};
    unsigned char eofByte=0;
};
class OriginalLinuxHeldFile {
public:
    enum class CaptureMode {CompleteFile,SourceUnitIdentity};
private:
    const char* selectedPathOriginal_=nullptr;size_t selectedReadRows_=0;
    size_t allocatedPathBytes_=0,allocatedBodyBytes_=0;
    bool preownAttempted_=false,preownReturned_=false,preowned_=false;std::exception_ptr preownException_;
    OriginalDeadline& deadline_;
    OriginalDeadlineSupervisor& supervisor_;
    std::unique_ptr<char[]> path_;
    std::unique_ptr<unsigned char[]> body_;
    std::unique_ptr<HeldFileCall[]> reads_;
    size_t pathBytes_=0,expectedBytes_=0,readCapacity_=0,readCount_=0,retainedBytes_=0;
    std::array<unsigned char,32> expectedDigest_{};
    template_creator_source::OriginalSha256 hash_;
    HeldFileCall open_,before_,eof_,after_,close_;
    OriginalClockBoundary finalVerification_{};
    CaptureMode mode_=CaptureMode::CompleteFile;
    int originalFd_=-1;
    bool attempted_=false,captured_=false,identityCaptured_=false,failed_=false,closed_=false,readExtentUnknown_=false;
    bool Begin(HeldFileCall& row,HeldFileCall::Operation op,bool eligible=false) noexcept {
        row.operation=op;
        if(!row.clock.Before(&deadline_,"held-input-native",eligible)){failed_=true;return false;}
        const bool pending=eligible?supervisor_.BeginIndependentlyEligibleCleanupNativeCall():supervisor_.BeginNativeCall();
        if(!pending){failed_=true;return false;}
        row.attempted=true;return true;
    }
    void Returned(HeldFileCall& row,ssize_t result,int originalError) noexcept {
        row.returned=result;row.error=originalError;row.returnedObserved=true;
        supervisor_.OriginalNativeCallReturned();
        row.clock.After(&deadline_,"held-input-native");if(!row.clock.clean)failed_=true;
    }
    static bool SameStable(const struct stat& first,const struct stat& last) noexcept {
        return first.st_dev==last.st_dev&&first.st_ino==last.st_ino&&first.st_mode==last.st_mode&&
            first.st_size==last.st_size&&first.st_mtim.tv_sec==last.st_mtim.tv_sec&&
            first.st_mtim.tv_nsec==last.st_mtim.tv_nsec&&first.st_ctim.tv_sec==last.st_ctim.tv_sec&&
            first.st_ctim.tv_nsec==last.st_ctim.tv_nsec;
        // Access metadata may change because of these actual owned reads.
    }
public:
    OriginalLinuxHeldFile(DeferredOriginalPreownTag,const char* originalPath,size_t pathBytes,size_t expectedBytes,
        const unsigned char* expectedDigest,size_t readRows,OriginalDeadline& deadline,
        OriginalDeadlineSupervisor& supervisor,CaptureMode mode=CaptureMode::CompleteFile)
        :deadline_(deadline),supervisor_(supervisor),mode_(mode) {
        if(!originalPath||!pathBytes||pathBytes==SIZE_MAX||!expectedDigest||!readRows||
           readRows>SIZE_MAX/sizeof(HeldFileCall)||expectedBytes>UINT64_MAX/8||
           supervisor.OriginalDeadlineOwner()!=&deadline)
            throw std::invalid_argument("original Linux held input geometry");
        if(mode!=CaptureMode::CompleteFile&&mode!=CaptureMode::SourceUnitIdentity)
            throw std::invalid_argument("original Linux held input mode");
        if(mode==CaptureMode::SourceUnitIdentity&&expectedBytes>static_cast<uintmax_t>(std::numeric_limits<off_t>::max()))
            throw std::invalid_argument("source-unit identity offset");
        for(size_t i=0;i<pathBytes;++i)if(!originalPath[i])throw std::invalid_argument("original Linux input embedded NUL");
        size_t total=sizeof(*this);
        if(pathBytes+1>SIZE_MAX-total)throw std::invalid_argument("original Linux input path geometry");total+=pathBytes+1;
        const size_t bodyBytes=mode==CaptureMode::CompleteFile?expectedBytes:0;
        if(bodyBytes>SIZE_MAX-total)throw std::invalid_argument("original Linux input body geometry");total+=bodyBytes;
        if(readRows>(SIZE_MAX-total)/sizeof(HeldFileCall))throw std::invalid_argument("original Linux input rows geometry");
        selectedPathOriginal_=originalPath;selectedReadRows_=readRows;
        std::memcpy(expectedDigest_.data(),expectedDigest,32);
        pathBytes_=pathBytes;expectedBytes_=expectedBytes;
    }
    OriginalLinuxHeldFile(const char* path,size_t pathBytes,size_t bytes,const unsigned char* digest,
        size_t rows,OriginalDeadline& deadline,OriginalDeadlineSupervisor& supervisor,CaptureMode mode=CaptureMode::CompleteFile)
        :OriginalLinuxHeldFile(DeferredOriginalPreown,path,pathBytes,bytes,digest,rows,deadline,supervisor,mode){
        if(!PreownStorageOriginal())std::rethrow_exception(preownException_);
    }
    bool PreownStorageOriginal() noexcept {
        if(preownAttempted_)return false;preownAttempted_=true;
        struct Returned {bool& value;~Returned(){value=true;}} returned{preownReturned_};
        try {
            path_=std::make_unique<char[]>(pathBytes_+1);allocatedPathBytes_=pathBytes_+1;
            std::memcpy(path_.get(),selectedPathOriginal_,pathBytes_);path_[pathBytes_]=0;
            if(mode_==CaptureMode::CompleteFile){body_=std::make_unique<unsigned char[]>(expectedBytes_);allocatedBodyBytes_=expectedBytes_;}
            reads_=std::make_unique<HeldFileCall[]>(selectedReadRows_);readCapacity_=selectedReadRows_;
            preowned_=true;return true;
        }catch(...){if(!preownException_)preownException_=std::current_exception();failed_=true;return false;}
    }
    const std::exception_ptr& PreownExceptionOriginal() const noexcept{return preownException_;}
    bool PreownedOriginal() const noexcept{return preowned_;}
    bool PreownReturnedOriginal() const noexcept{return preownReturned_;}
    size_t AllocatedPathBytesOriginal() const noexcept{return allocatedPathBytes_;}
    size_t AllocatedBodyBytesOriginal() const noexcept{return allocatedBodyBytes_;}    OriginalLinuxHeldFile(const OriginalLinuxHeldFile&)=delete;
    OriginalLinuxHeldFile& operator=(const OriginalLinuxHeldFile&)=delete;
    // Caller is the SAME original creating native thread. Complete preownership
    // of ALL input objects precedes the first Capture in the connected platform.
    bool Capture() noexcept {return CaptureNative(-1,false);}
    // Caller must independently join this borrowed directory to its SAME
    // saved EXEC stop. These bytes grant no prior lookup/loader authority.
    bool CaptureProcExeCandidate(int originalTaskDirectory) noexcept {
        if(originalTaskDirectory<0)return false;
        return CaptureNative(originalTaskDirectory,true);
    }
private:
    bool CaptureNative(int originalTaskDirectory,bool procExe) noexcept {
        if(!preowned_)return false;
        if(attempted_)return false;attempted_=true;
        open_.originalDirectory=originalTaskDirectory;
        open_.originalOpenFlags=O_RDONLY|O_CLOEXEC;open_.procExeCandidate=procExe;
        if(!Begin(open_,HeldFileCall::Operation::Open))return false;
        // proc exe is intentionally dereferenced; O_NOFOLLOW would observe
        // the proc link rather than the kernel-selected executable referent.
        const int result=procExe?openat(originalTaskDirectory,"exe",open_.originalOpenFlags):
            open(path_.get(),open_.originalOpenFlags);const int error=result<0?errno:0;
        originalFd_=result; // Actual acquisition attaches before clocks/validation.
        Returned(open_,result,error);if(result<0||failed_)return false;
        if(!Begin(before_,HeldFileCall::Operation::BeforeStat))return false;
        const int statResult=fstat(originalFd_,&before_.metadata);const int statError=statResult<0?errno:0;
        Returned(before_,statResult,statError);
        if(statResult||failed_||!S_ISREG(before_.metadata.st_mode)||before_.metadata.st_size<0||
           static_cast<uintmax_t>(before_.metadata.st_size)!=expectedBytes_){failed_=true;return false;}
        while(mode_==CaptureMode::CompleteFile&&retainedBytes_<expectedBytes_){
            if(readCount_==readCapacity_){failed_=true;return false;}
            auto& row=reads_[readCount_++];row.destinationOffset=retainedBytes_;
            row.requested=expectedBytes_-retainedBytes_>65536?65536:expectedBytes_-retainedBytes_;
            if(!Begin(row,HeldFileCall::Operation::Read))return false;
            const ssize_t got=read(originalFd_,body_.get()+row.destinationOffset,row.requested);
            const int rawError=got<0?errno:0;
            // Preserve actual bytes before interpreting native failure/EOF.
            if(got>=0&&static_cast<size_t>(got)<=row.requested)retainedBytes_+=static_cast<size_t>(got);
            else {row.affectedExtentUnknown=true;readExtentUnknown_=true;}
            Returned(row,got,rawError);
            if(got<=0||static_cast<size_t>(got)>row.requested||failed_){failed_=true;return false;}
            if(!hash_.Update(body_.get()+row.destinationOffset,static_cast<size_t>(got))){failed_=true;return false;}
        }
        eof_.requested=1;eof_.destinationOffset=mode_==CaptureMode::CompleteFile?retainedBytes_:expectedBytes_;
        if(!Begin(eof_,HeldFileCall::Operation::Eof))return false;
        const ssize_t eofResult=mode_==CaptureMode::CompleteFile?read(originalFd_,&eof_.eofByte,1):
            pread(originalFd_,&eof_.eofByte,1,static_cast<off_t>(expectedBytes_));
        const int eofError=eofResult<0?errno:0;
        if(eofResult<0||eofResult>1)eof_.affectedExtentUnknown=true;
        Returned(eof_,eofResult,eofError);
        if(eofResult!=0||failed_){failed_=true;return false;}
        if(!Begin(after_,HeldFileCall::Operation::AfterStat))return false;
        const int finalStat=fstat(originalFd_,&after_.metadata);const int finalError=finalStat<0?errno:0;
        Returned(after_,finalStat,finalError);
        const bool stable=finalStat==0&&!failed_&&SameStable(before_.metadata,after_.metadata);
        const bool body=stable&&(mode_==CaptureMode::SourceUnitIdentity||(hash_.Finish()&&hash_.PrivateDigest()==expectedDigest_));
        const bool finalClock=IngressNativeClock::Capture(finalVerification_,&deadline_,"held-input-final-verification",true);
        identityCaptured_=stable&&body&&finalClock&&finalVerification_.shapeValid&&!finalVerification_.expired&&
            finalVerification_.nanoseconds>=after_.clock.after.nanoseconds;
        captured_=identityCaptured_&&mode_==CaptureMode::CompleteFile;
        failed_|=!identityCaptured_;return identityCaptured_;
    }
public:
    const std::array<unsigned char,32>& OriginalExpectedDigest() const noexcept{return expectedDigest_;}
    size_t OriginalReadCapacity() const noexcept{return readCapacity_;}
    bool SameCapturedFileCandidate(const OriginalLinuxHeldFile& original) const noexcept {
        return CapturedSourceBytes()&&original.CapturedSourceBytes()&&
            SameStable(before_.metadata,original.before_.metadata)&&
            SameStable(after_.metadata,original.after_.metadata)&&retainedBytes_==original.retainedBytes_&&
            !std::memcmp(body_.get(),original.body_.get(),retainedBytes_);
    }
    // Called only after concrete platform consumer eligibility. Failure of one
    // owner must not suppress a later independent owner's once-only close.
    bool CloseEligibleOnce() noexcept {
        if(originalFd_<0)return true;if(close_.attempted)return closed_;
        if(!Begin(close_,HeldFileCall::Operation::Close,true))return false;
        const int result=close(originalFd_);const int error=result<0?errno:0;
        Returned(close_,result,error);closed_=result==0;if(!closed_)failed_=true;return closed_;
    }
    bool PhysicalDisposed() const noexcept{return originalFd_<0||closed_;}
    bool CapturedSourceBytes() const noexcept{return captured_&&!failed_&&!close_.attempted&&!readExtentUnknown_;}
    bool CapturedNativeIdentity() const noexcept{return identityCaptured_&&!failed_&&!close_.attempted&&!readExtentUnknown_;}
    CaptureMode OriginalCaptureMode() const noexcept{return mode_;}
    const OriginalDeadline* OriginalDeadlineOwner() const noexcept{return &deadline_;}
    const OriginalDeadlineSupervisor* OriginalSupervisorOwner() const noexcept{return &supervisor_;}
    int OriginalDescriptor() const noexcept{return originalFd_;}
    const char* PrivateOriginalPath() const noexcept{return path_.get();}
    size_t OriginalPathBytes() const noexcept{return pathBytes_;}
    const unsigned char* PrivateBody() const noexcept{return body_.get();}
    size_t RetainedByteCount() const noexcept{return retainedBytes_;}
    size_t BodyCapacity() const noexcept{return mode_==CaptureMode::CompleteFile?expectedBytes_:0;}
    const HeldFileCall* PrivateReads() const noexcept{return reads_.get();}
    size_t ReadCount() const noexcept{return readCount_;}
    const HeldFileCall& PrivateOpen() const noexcept{return open_;}
    const HeldFileCall& PrivateBefore() const noexcept{return before_;}
    const HeldFileCall& PrivateAfter() const noexcept{return after_;}
    const HeldFileCall& PrivateEof() const noexcept{return eof_;}
    const HeldFileCall& PrivateClose() const noexcept{return close_;}
    size_t ReservedSourceBytes() const noexcept{return sizeof(*this)+allocatedPathBytes_+allocatedBodyBytes_+readCapacity_*sizeof(HeldFileCall);}
};
}

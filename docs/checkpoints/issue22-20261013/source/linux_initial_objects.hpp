// UNCOMPILED actual initial object observations, never native role authority.
#pragma once
#include "linux_ingress.hpp"
#include "linux_held_file.hpp"
namespace template_creator_linux {
struct InitialObjectStat {
    size_t originalOrdinal=SIZE_MAX;int originalFd=-1;
    SupervisorNativeOriginal call;struct stat output{};
};
class OriginalLinuxInitialObjects {
    OriginalPrivateIngress& ingress_;
    const OriginalDeadline& deadline_;
    OriginalDeadlineSupervisor& supervisor_;
    std::array<InitialObjectStat,3> rows_{};
    bool selected_=false,attempted_=false,captured_=false,failed_=false;
public:
    OriginalLinuxInitialObjects(OriginalPrivateIngress& ingress,const OriginalDeadline& deadline,OriginalDeadlineSupervisor& supervisor)
        :ingress_(ingress),deadline_(deadline),supervisor_(supervisor){
        if(supervisor.OriginalDeadlineOwner()!=&deadline)throw std::invalid_argument("same initial objects deadline");
    }
    bool SelectOriginalOrdinals(size_t root,size_t tool,size_t evidence,size_t sink,size_t caller,size_t actualCount) noexcept {
        if(selected_||attempted_)return false;
        const std::array<size_t,3> values{root,tool,evidence};
        for(size_t i=0;i<values.size();++i){
            if(values[i]>=actualCount||values[i]==sink||values[i]==caller)return false;
            for(size_t j=0;j<i;++j)if(values[i]==values[j])return false;
        }
        for(size_t i=0;i<values.size();++i)rows_[i].originalOrdinal=values[i];
        selected_=true;return true;
    }
    bool CaptureOriginals() noexcept {
        if(attempted_)return false;attempted_=true;
        if(!selected_||!ingress_.Captured()){failed_=true;return false;}
        for(size_t i=0;i<rows_.size();++i){
            auto& row=rows_[i];
            if(row.originalOrdinal>=ingress_.OriginalDescriptorCount()){failed_=true;continue;}
            const auto& original=ingress_.OriginalDescriptors()[row.originalOrdinal];
            row.originalFd=original.original;
            if(row.originalFd<0||original.closeAttempted){failed_=true;continue;}
            const char* operation=i==0?"initial-root-fstat":i==1?"initial-tool-fstat":"initial-evidence-fstat";
            if(!CaptureSupervisorClock(row.call.beforeClock,deadline_,operation,false)||!supervisor_.BeginNativeCall()){
                failed_=true;continue;
            }
            row.call.attempted=true;row.call.returned=fstat(row.originalFd,&row.output);
            if(row.call.returned<0)row.call.error=errno;
            row.call.returnedObserved=true;supervisor_.OriginalNativeCallReturned();
            if(!CaptureSupervisorClock(row.call.afterClock,deadline_,operation,true)||
                !row.call.afterClock.shapeValid||row.call.afterClock.nanoseconds<row.call.beforeClock.nanoseconds||
                row.call.returned!=0)failed_=true;
        }
        if(!S_ISDIR(rows_[0].output.st_mode)||!S_ISREG(rows_[1].output.st_mode)||!S_ISDIR(rows_[2].output.st_mode)||
           (rows_[0].output.st_dev==rows_[2].output.st_dev&&rows_[0].output.st_ino==rows_[2].output.st_ino))failed_=true;
        captured_=!failed_;return captured_;
    }
    bool SameCapturedTool(const OriginalLinuxHeldFile& file) const noexcept {
        if(!captured_||!file.CapturedSourceBytes()||rows_[1].originalOrdinal>=ingress_.OriginalDescriptorCount())return false;
        const auto& descriptor=ingress_.OriginalDescriptors()[rows_[1].originalOrdinal];
        const auto& after=file.PrivateAfter();
        return descriptor.original==rows_[1].originalFd&&!descriptor.closeAttempted&&
            after.returnedObserved&&after.returned==0&&after.metadata.st_dev==rows_[1].output.st_dev&&
            after.metadata.st_ino==rows_[1].output.st_ino;
    }
    bool Captured() const noexcept{return captured_&&!failed_;}
    bool SameCapturedRoot(const struct stat& actual) const noexcept {
        return Captured()&&S_ISDIR(actual.st_mode)&&actual.st_dev==rows_[0].output.st_dev&&actual.st_ino==rows_[0].output.st_ino;
    }
    const InitialObjectStat* PrivateOriginals() const noexcept{return rows_.data();}
    // Borrowed originals remain owned by SAME ingress. Source rows neither
    // establish outside-writer exclusion nor close/dispose any received FD.
};
class OriginalLinuxCommandDirectory {
    OriginalLinuxInitialObjects& initial_;
    const OriginalDeadline& deadline_;
    OriginalDeadlineSupervisor& supervisor_;
    std::unique_ptr<char[]> path_;
    size_t pathBytes_=0;
    int fd_=-1;
    SupervisorNativeOriginal open_,stat_,close_;
    struct stat metadata_{};
    bool attempted_=false,captured_=false,closed_=false,failed_=false,disposalRequested_=false;
    bool Begin(SupervisorNativeOriginal& row,const char* operation,bool cleanup=false) noexcept {
        const bool before=CaptureSupervisorClock(row.beforeClock,deadline_,operation,false);
        if(!cleanup&&!before){failed_=true;return false;}
        if(!(cleanup?supervisor_.BeginIndependentlyEligibleCleanupNativeCall():supervisor_.BeginNativeCall())){
            failed_=true;return false;
        }
        row.attempted=true;return true;
    }
    void Returned(SupervisorNativeOriginal& row,long value,int error,const char* operation) noexcept {
        row.returned=value;row.error=error;row.returnedObserved=true;
        supervisor_.OriginalNativeCallReturned();
        if(!CaptureSupervisorClock(row.afterClock,deadline_,operation,true)||!row.beforeClock.shapeValid||
           row.afterClock.nanoseconds<row.beforeClock.nanoseconds)failed_=true;
    }
public:
    OriginalLinuxCommandDirectory(const char* original,size_t bytes,OriginalLinuxInitialObjects& initial,
        const OriginalDeadline& deadline,OriginalDeadlineSupervisor& supervisor)
        :initial_(initial),deadline_(deadline),supervisor_(supervisor),pathBytes_(bytes) {
        if(!original||!bytes||bytes>SIZE_MAX-sizeof(*this)-1||original[0]!='/'||supervisor.OriginalDeadlineOwner()!=&deadline)
            throw std::invalid_argument("same original Linux cwd geometry");
        for(size_t i=0;i<bytes;++i)if(!original[i])throw std::invalid_argument("original Linux cwd NUL");
        path_=std::make_unique<char[]>(bytes+1);std::memcpy(path_.get(),original,bytes);path_[bytes]=0;
    }
    bool Capture() noexcept {
        if(attempted_||disposalRequested_)return false;attempted_=true;
        if(!initial_.Captured()||!Begin(open_,"command-cwd-open")){failed_=true;return false;}
        fd_=open(path_.get(),O_RDONLY|O_DIRECTORY|O_CLOEXEC);const int error=fd_<0?errno:0;
        Returned(open_,fd_,error,"command-cwd-open");
        if(fd_<0||failed_||!Begin(stat_,"command-cwd-fstat")){failed_=true;return false;}
        const int result=fstat(fd_,&metadata_);const int statError=result<0?errno:0;
        Returned(stat_,result,statError,"command-cwd-fstat");
        captured_=result==0&&!failed_&&initial_.SameCapturedRoot(metadata_);
        failed_|=!captured_;return captured_;
    }
    bool CloseEligibleOnce() noexcept {
        disposalRequested_=true;
        if(fd_<0)return true;if(close_.attempted)return closed_;
        if(!Begin(close_,"command-cwd-close",true))return false;
        const int result=close(fd_);const int error=result<0?errno:0;
        Returned(close_,result,error,"command-cwd-close");closed_=result==0;failed_|=!closed_;return closed_;
    }
    bool PhysicalDisposed() const noexcept{return fd_<0||closed_;}
    bool Captured() const noexcept{return captured_&&!failed_&&!disposalRequested_;}
    int OriginalDescriptor() const noexcept{return Captured()?fd_:-1;}
    const char* PrivateOriginalPath() const noexcept{return path_.get();}
    size_t OriginalPathBytes() const noexcept{return pathBytes_;}
    const struct stat& PrivateMetadata() const noexcept{return metadata_;}
    const SupervisorNativeOriginal& PrivateOpen() const noexcept{return open_;}
    const SupervisorNativeOriginal& PrivateStat() const noexcept{return stat_;}
    const SupervisorNativeOriginal& PrivateClose() const noexcept{return close_;}
    size_t ReservedSourceBytes() const noexcept{return sizeof(*this)+pathBytes_+1;}
};
}

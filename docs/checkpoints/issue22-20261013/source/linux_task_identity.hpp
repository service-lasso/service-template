// UNCOMPILED held Linux task identity observations for concrete session gates.
// Kernel proc FD lifetime prevents reopening a recycled numeric TID. Ptrace /
// pidfd / native EXEC associations remain distinct session-owned predicates.
#pragma once
#include <sys/stat.h>
#include <sys/statfs.h>
#include <linux/magic.h>
#include <unistd.h>
#include <fcntl.h>
#include <cerrno>
#include <cstdint>
#include <limits>
#include <memory>
#include <charconv>
#include <cstring>
#include <stdexcept>
#include "linux_original_deadline.hpp"
#include "linux_deadline_supervisor.hpp"
namespace template_creator_linux {
struct ProcOwnedFd {
    ProcOwnedFd() = default;
    ProcOwnedFd(const ProcOwnedFd&) = delete;
    ProcOwnedFd& operator=(const ProcOwnedFd&) = delete;
    ProcOwnedFd(ProcOwnedFd&&) = delete;
    ProcOwnedFd& operator=(ProcOwnedFd&&) = delete;
    int original = -1, openResult = -1, openError = 0, closeResult = -1, closeError = 0;
    bool openAttempted=false,openReturned=false,closeAttempted=false,closeReturned=false,closed=false;
    bool CloseOnce(OriginalDeadline* deadline = nullptr,OriginalDeadlineSupervisor* originalSupervisor=nullptr) noexcept {
        if (original < 0) return true;
        if(closeAttempted)return closed;
        if(originalSupervisor&&(!deadline||originalSupervisor->OriginalDeadlineOwner()!=deadline||
            !originalSupervisor->BeginIndependentlyEligibleCleanupNativeCall()))return false;
        // Expiry never suppresses an independently eligible once-only close.
        if (deadline) deadline->Observe("proc-owned-close",false);
        closeAttempted = true; closeResult = close(original);
        if (closeResult == 0) closed = true; else closeError = errno;
        closeReturned=true;
        if(originalSupervisor)originalSupervisor->OriginalNativeCallReturned();
        if (deadline) deadline->Observe("proc-owned-close",true);
        return closed;
    }
};
struct ProcReadOriginal { size_t destinationOffset=0,requested=0;ssize_t returned=-1;int error=0;bool attempted=false,returnedObserved=false; };
// One retained root capability supplies every task-directory traversal. A
// caller cannot substitute an independently numbered proc mount at a later join.
class OriginalProcRoot {
    ProcOwnedFd descriptor_;
    struct statfs filesystem_{}; struct stat metadata_{};
    int filesystemResult_ = -1, filesystemError_ = 0, statResult_ = -1, statError_ = 0;
    int suppliedOriginal_ = -1; bool attempted_ = false, captured_ = false;
    OriginalDeadline* deadline_ = nullptr;
public:
    OriginalProcRoot() = default;
    OriginalProcRoot(const OriginalProcRoot&) = delete;
    OriginalProcRoot& operator=(const OriginalProcRoot&) = delete;
    bool Capture(int suppliedOriginal, OriginalDeadline& deadline) noexcept {
        if (attempted_) return false; attempted_ = true; suppliedOriginal_ = suppliedOriginal;
        deadline_ = &deadline;
        if (!deadline.Observe("proc-root-duplicate",false)) return false;
        descriptor_.openAttempted = true;
        descriptor_.openResult = fcntl(suppliedOriginal,F_DUPFD_CLOEXEC,4);
        descriptor_.original = descriptor_.openResult;
        if (descriptor_.openResult < 0) descriptor_.openError = errno;
        descriptor_.openReturned=true;
        const bool duplicateClock = deadline.Observe("proc-root-duplicate",true);
        if (descriptor_.openResult < 0 || !duplicateClock) return false;
        if (!deadline.Observe("proc-root-fstatfs",false)) return false;
        filesystemResult_ = fstatfs(descriptor_.original,&filesystem_);
        if (filesystemResult_ < 0) filesystemError_ = errno;
        const bool filesystemClock = deadline.Observe("proc-root-fstatfs",true);
        if (!deadline.Observe("proc-root-fstat",false)) return false;
        statResult_ = fstat(descriptor_.original,&metadata_);
        if (statResult_ < 0) statError_ = errno;
        const bool statClock = deadline.Observe("proc-root-fstat",true);
        captured_ = duplicateClock && filesystemClock && statClock && filesystemResult_ == 0 && statResult_ == 0 &&
            filesystem_.f_type == PROC_SUPER_MAGIC && S_ISDIR(metadata_.st_mode);
        return captured_;
    }
    bool Available() const noexcept { return captured_ && !descriptor_.closeAttempted; }
    OriginalDeadline* OriginalDeadlineOwner() const noexcept { return deadline_; }
    const ProcOwnedFd& OriginalDescriptor() const noexcept { return descriptor_; }
    const struct statfs& OriginalFilesystem() const noexcept { return filesystem_; }
    const struct stat& OriginalMetadata() const noexcept { return metadata_; }
    int OriginalSuppliedDescriptor() const noexcept { return suppliedOriginal_; }
    int OriginalFilesystemResult() const noexcept { return filesystemResult_; }
    int OriginalFilesystemError() const noexcept { return filesystemError_; }
    int OriginalStatResult() const noexcept { return statResult_; }
    int OriginalStatError() const noexcept { return statError_; }
    // Only the session may call this after every dependent task observation.
    bool DisposeEligible(OriginalDeadlineSupervisor* supervisor=nullptr) noexcept {
        descriptor_.CloseOnce(deadline_,supervisor);return descriptor_.original<0||descriptor_.closed;
    }
};
struct ProcFileObservation {
    ProcOwnedFd descriptor;
    std::unique_ptr<unsigned char[]> bytes;
    std::unique_ptr<ProcReadOriginal[]> reads;
    size_t byteCapacity = 0, readCapacity = 0, used = 0, readCount = 0;
    bool attempted = false, eof = false, shapeFailure = false;
    size_t selectedBytes_=0,selectedReadRows_=0;
    bool preownAttempted=false,preowned=false;
    std::exception_ptr preownException;
    ProcFileObservation(DeferredOriginalPreownTag,size_t selectedBytes,size_t selectedReadRows) noexcept
        :selectedBytes_(selectedBytes),selectedReadRows_(selectedReadRows) {}
    ProcFileObservation(size_t selectedBytes, size_t selectedReadRows)
        :ProcFileObservation(DeferredOriginalPreown,selectedBytes,selectedReadRows) {
        if(!PreownOriginal())std::rethrow_exception(preownException);
    }
    bool PreownOriginal() noexcept {
        if(preownAttempted)return false;preownAttempted=true;
        try {
            if(!selectedBytes_||selectedBytes_==SIZE_MAX||!selectedReadRows_||
               selectedBytes_+1>SIZE_MAX-sizeof(*this)||
               selectedReadRows_>(SIZE_MAX-sizeof(*this)-(selectedBytes_+1))/sizeof(ProcReadOriginal))
                throw std::invalid_argument("aggregate task proc file backing");
            bytes=std::make_unique<unsigned char[]>(selectedBytes_+1);byteCapacity=selectedBytes_;
            reads=std::make_unique<ProcReadOriginal[]>(selectedReadRows_);readCapacity=selectedReadRows_;
            preowned=true;return true;
        }catch(...){preownException=std::current_exception();shapeFailure=true;return false;}
    }
    bool Capture(int heldTaskDirectory, const char* literalLeaf, OriginalDeadline& deadline,
                 OriginalDeadlineSupervisor* originalSupervisor=nullptr,bool independentlyEligible=false) noexcept {
        if(!preowned){shapeFailure=true;return false;}
        if (attempted) return false;
        attempted = true;
        if(independentlyEligible&&!originalSupervisor){shapeFailure=true;return false;}
        if(originalSupervisor&&originalSupervisor->OriginalDeadlineOwner()!=&deadline){shapeFailure=true;return false;}
        const bool beforeOpen=deadline.Observe("proc-file-open",false);
        if (!beforeOpen&&!independentlyEligible) return false;
        if(originalSupervisor&&!(independentlyEligible?originalSupervisor->BeginIndependentlyEligibleCleanupNativeCall():
            originalSupervisor->BeginNativeCall()))return false;
        descriptor.openAttempted = true;
        descriptor.openResult = openat(heldTaskDirectory, literalLeaf, O_RDONLY|O_NOFOLLOW|O_CLOEXEC);
        descriptor.original = descriptor.openResult;
        if (descriptor.openResult < 0) descriptor.openError = errno;
        descriptor.openReturned=true;
        if(originalSupervisor)originalSupervisor->OriginalNativeCallReturned();
        const bool openClock = deadline.Observe("proc-file-open",true);
        if (descriptor.openResult < 0) return false;
        if (!openClock&&!independentlyEligible) { descriptor.CloseOnce(&deadline,originalSupervisor); return false; }
        bool complete = false;
        while (used <= byteCapacity) {
            if (readCount == readCapacity) { shapeFailure = true; break; }
            auto& row = reads[readCount++]; row.destinationOffset = used;
            const size_t available = byteCapacity+1-used;
            row.requested = available > 65536 ? 65536 : available;
            const bool beforeRead=deadline.Observe("proc-file-read",false);
            if (!beforeRead&&!independentlyEligible) break;
            if(originalSupervisor&&!(independentlyEligible?originalSupervisor->BeginIndependentlyEligibleCleanupNativeCall():
                originalSupervisor->BeginNativeCall()))break;
            row.attempted = true;
            row.returned = read(descriptor.original, bytes.get()+used, row.requested);
            if (row.returned < 0) row.error = errno;
            row.returnedObserved=true;
            const bool extentShape=row.returned>=0&&static_cast<size_t>(row.returned)<=row.requested;
            if(extentShape)used+=static_cast<size_t>(row.returned);
            if(originalSupervisor)originalSupervisor->OriginalNativeCallReturned();
            const bool readClock = deadline.Observe("proc-file-read",true);
            if (row.returned < 0) break;
            if (static_cast<size_t>(row.returned) > row.requested) { shapeFailure = true; break; }
            if (!readClock&&!independentlyEligible) break;
            if (!row.returned) { eof = true; complete = used <= byteCapacity; break; }
            if (used > byteCapacity) { shapeFailure = true; break; }
        }
        descriptor.CloseOnce(&deadline,originalSupervisor); // Independent original close, never a retry.
        // The cleanup result is physical capture only; original deadline
        // failure remains absorbing and cannot become qualified execution.
        return complete && descriptor.closed && !shapeFailure && (independentlyEligible||deadline.QualificationClean());
    }
};
struct ProcTaskObservation {
    ProcFileObservation stat, status;
    pid_t observedPid = 0, observedTgid = 0, observedTracer = 0;
    uint64_t startTicks = 0;
    bool attempted = false, parsed = false;
    size_t selectedStatBytes_=0,selectedStatusBytes_=0,selectedReadRows_=0;
    bool preownAttempted=false,preowned=false;
    std::exception_ptr preownException;
    static size_t CheckedSourceBacking(size_t statBytes,size_t statusBytes,size_t readsPerFile) {
        if(!statBytes||statBytes==SIZE_MAX||!statusBytes||statusBytes==SIZE_MAX||
           !readsPerFile||readsPerFile>SIZE_MAX/sizeof(ProcReadOriginal))
            throw std::invalid_argument("task proc observation geometry");
        size_t total=sizeof(ProcTaskObservation);
        const auto add=[&](size_t n) {
            if(n>SIZE_MAX-total)return false;total+=n;return true;
        };
        if(!add(statBytes+1)||!add(statusBytes+1)||
           !add(readsPerFile*sizeof(ProcReadOriginal))||!add(readsPerFile*sizeof(ProcReadOriginal)))
            throw std::invalid_argument("aggregate task proc observation backing");
        return total;
    }
    static size_t CheckedStatBacking(size_t statBytes,size_t statusBytes,size_t readsPerFile) {
        (void)CheckedSourceBacking(statBytes,statusBytes,readsPerFile);
        return statBytes;
    }
    ProcTaskObservation(DeferredOriginalPreownTag,size_t statBytes,size_t statusBytes,size_t readsPerFile) noexcept
        :stat(DeferredOriginalPreown,statBytes,readsPerFile),status(DeferredOriginalPreown,statusBytes,readsPerFile),
         selectedStatBytes_(statBytes),selectedStatusBytes_(statusBytes),selectedReadRows_(readsPerFile) {}
    ProcTaskObservation(size_t statBytes, size_t statusBytes, size_t readsPerFile)
        :ProcTaskObservation(DeferredOriginalPreown,statBytes,statusBytes,readsPerFile) {
        if(!PreownOriginal())std::rethrow_exception(preownException);
    }
    bool PreownOriginal() noexcept {
        if(preownAttempted)return false;preownAttempted=true;
        try {
            (void)CheckedSourceBacking(selectedStatBytes_,selectedStatusBytes_,selectedReadRows_);
            if(!stat.PreownOriginal()){preownException=stat.preownException;return false;}
            if(!status.PreownOriginal()){preownException=status.preownException;return false;}
            preowned=true;return true;
        }catch(...){preownException=std::current_exception();return false;}
    }
    static bool Unsigned(const unsigned char* begin, const unsigned char* end, uint64_t& value) noexcept {
        if (begin == end) return false; value = 0;
        for (auto* p=begin; p<end; ++p) {
            if (*p < '0' || *p > '9' || value > (UINT64_MAX-(*p-'0'))/10) return false;
            value = value*10+(*p-'0');
        }
        return true;
    }
    bool Parse() noexcept {
        if(!preowned)return false;
        const auto* begin = stat.bytes.get(); const auto* end = begin+stat.used;
        const unsigned char* space = begin;
        while (space != end && *space != ' ') ++space;
        uint64_t pid = 0;
        if (!Unsigned(begin, space, pid) || !pid || pid > INT32_MAX || space == end ||
            end-space < 4 || space[1] != '(') return false;
        const unsigned char* close = end;
        while (close != space+1 && close[-1] != ')') --close;
        if (close == space+1 || end-close < 3 || close[0] != ' ' || close[2] != ' ') return false;
        const unsigned char* token = close+3; // field4 follows state(field3).
        for (unsigned field=4; field<=22; ++field) {
            while (token != end && *token == ' ') ++token;
            const unsigned char* stop = token;
            while (stop != end && *stop != ' ' && *stop != '\n') ++stop;
            if (field == 22 && !Unsigned(token, stop, startTicks)) return false;
            if (token == stop) return false; token = stop;
        }
        observedPid = static_cast<pid_t>(pid);
        unsigned seenPid = 0, seenTgid = 0, seenTracer = 0;
        const auto* cursor = status.bytes.get(); const auto* finish = cursor+status.used;
        while (cursor != finish) {
            const unsigned char* lineEnd = cursor;
            while (lineEnd != finish && *lineEnd != '\n') ++lineEnd;
            const char* key = nullptr; size_t keyLength = 0; unsigned* seen = nullptr; pid_t* destination = nullptr;
            if (lineEnd-cursor >= 4 && memcmp(cursor,"Pid:",4)==0) { key="Pid:"; keyLength=4; seen=&seenPid; destination=&observedPid; }
            else if (lineEnd-cursor >= 5 && memcmp(cursor,"Tgid:",5)==0) { key="Tgid:"; keyLength=5; seen=&seenTgid; destination=&observedTgid; }
            else if (lineEnd-cursor >= 10 && memcmp(cursor,"TracerPid:",10)==0) { key="TracerPid:"; keyLength=10; seen=&seenTracer; destination=&observedTracer; }
            if (key) {
                if (++*seen != 1) return false;
                const unsigned char* valueBegin = cursor+keyLength;
                while (valueBegin != lineEnd && (*valueBegin == ' ' || *valueBegin == '\t')) ++valueBegin;
                const unsigned char* valueEnd = lineEnd;
                while (valueEnd != valueBegin && (valueEnd[-1] == ' ' || valueEnd[-1] == '\t')) --valueEnd;
                uint64_t value = 0;
                if (!Unsigned(valueBegin,valueEnd,value) || value > INT32_MAX) return false;
                if (destination == &observedPid && value != pid) return false;
                *destination = static_cast<pid_t>(value);
            }
            cursor = lineEnd == finish ? finish : lineEnd+1;
        }
        return seenPid == 1 && seenTgid == 1 && seenTracer == 1 && observedTgid > 0;
    }
};
class HeldTaskIdentity {
    OriginalProcRoot* root_ = nullptr;
    ProcOwnedFd directory_;
    struct statfs filesystem_{}; int filesystemResult_ = -1, filesystemError_ = 0;
    struct stat directoryStat_{}; int statResult_ = -1, statError_ = 0;
    std::unique_ptr<std::unique_ptr<ProcTaskObservation>[]> observations_;
    size_t capacity_ = 0, count_ = 0,bornCount_=0;
    size_t selectedObservations_=0,selectedStatBytes_=0,selectedStatusBytes_=0,selectedReadRows_=0;
    bool preownAttempted_=false,preowned_=false;
    std::exception_ptr preownException_;
    pid_t firstPid_ = 0, firstTgid_ = 0; uint64_t firstStartTicks_ = 0;
    bool openAttempted_ = false, bound_ = false;
    OriginalDeadlineSupervisor* supervisor_ = nullptr;
public:
    static size_t CheckedSourceBacking(size_t selectedObservations,size_t statBytes,size_t statusBytes,size_t readsPerFile) {
        if (!selectedObservations || selectedObservations > SIZE_MAX/sizeof(std::unique_ptr<ProcTaskObservation>))
            throw std::invalid_argument("selected original task identity roster");
        const size_t rowBytes=ProcTaskObservation::CheckedSourceBacking(statBytes,statusBytes,readsPerFile);
        const size_t pointerBytes=selectedObservations*sizeof(std::unique_ptr<ProcTaskObservation>);
        if(pointerBytes>SIZE_MAX-sizeof(HeldTaskIdentity) ||
           selectedObservations>(SIZE_MAX-sizeof(HeldTaskIdentity)-pointerBytes)/rowBytes)
            throw std::invalid_argument("aggregate original task identity roster backing");
        return sizeof(HeldTaskIdentity)+pointerBytes+selectedObservations*rowBytes;
    }
    HeldTaskIdentity(DeferredOriginalPreownTag,size_t selectedObservations,size_t statBytes,size_t statusBytes,size_t readsPerFile) noexcept
        :selectedObservations_(selectedObservations),selectedStatBytes_(statBytes),selectedStatusBytes_(statusBytes),selectedReadRows_(readsPerFile) {}
    HeldTaskIdentity(size_t selectedObservations, size_t statBytes, size_t statusBytes, size_t readsPerFile)
        :HeldTaskIdentity(DeferredOriginalPreown,selectedObservations,statBytes,statusBytes,readsPerFile) {
        if(!PreownOriginal())std::rethrow_exception(preownException_);
    }
    bool PreownOriginal() noexcept {
        if(preownAttempted_)return false;preownAttempted_=true;
        try {
            (void)CheckedSourceBacking(selectedObservations_,selectedStatBytes_,selectedStatusBytes_,selectedReadRows_);
            observations_=std::make_unique<std::unique_ptr<ProcTaskObservation>[]>(selectedObservations_);capacity_=selectedObservations_;
            for(size_t i=0;i<selectedObservations_;++i){
                observations_[i]=std::make_unique<ProcTaskObservation>(DeferredOriginalPreown,selectedStatBytes_,selectedStatusBytes_,selectedReadRows_);
                bornCount_=i+1;
                if(!observations_[i]->PreownOriginal()){preownException_=observations_[i]->preownException;return false;}
            }
            preowned_=true;return true;
        }catch(...){preownException_=std::current_exception();return false;}
    }
    bool PreownedOriginal() const noexcept{return preowned_;}
    const std::exception_ptr& PreownExceptionOriginal() const noexcept{return preownException_;}
    bool OutputBackingSeparateOriginal(const void* output,size_t bytes) const noexcept {
        if(!OriginalBackingStorageSeparate(output,bytes,this,1,sizeof(*this))||bornCount_>capacity_||
           !OriginalBackingStorageSeparate(output,bytes,observations_.get(),capacity_,sizeof(std::unique_ptr<ProcTaskObservation>)))return false;
        for(size_t i=0;i<bornCount_;++i){
            const auto* row=observations_[i].get();
            if(!row||!OriginalBackingStorageSeparate(output,bytes,row,1,sizeof(*row)))return false;
            for(const auto* file:{&row->stat,&row->status}){
                if(file->byteCapacity==SIZE_MAX||file->used>file->byteCapacity||file->readCount>file->readCapacity||
                   !OriginalBackingStorageSeparate(output,bytes,file->bytes.get(),file->bytes?file->byteCapacity+1:0)||
                   !OriginalBackingStorageSeparate(output,bytes,file->reads.get(),file->readCapacity,sizeof(ProcReadOriginal)))return false;
            }
        }
        return true;
    }
    HeldTaskIdentity(const HeldTaskIdentity&) = delete;
    HeldTaskIdentity& operator=(const HeldTaskIdentity&) = delete;
    bool BindFromOriginalProcRoot(OriginalProcRoot& root, pid_t originalEnrolledTid,
            OriginalDeadlineSupervisor* originalSupervisor=nullptr,bool independentlyEligible=false) noexcept {
        if (!preowned_||openAttempted_ || originalEnrolledTid <= 0) return false;
        openAttempted_ = true;
        root_ = &root;
        if (!root.Available()) return false;
        supervisor_=originalSupervisor;
        if(independentlyEligible&&!supervisor_)return false;
        if(supervisor_&&supervisor_->OriginalDeadlineOwner()!=root.OriginalDeadlineOwner())return false;
        auto& deadline = *root.OriginalDeadlineOwner();
        char literal[32]{};
        auto converted = std::to_chars(literal,literal+sizeof(literal)-1,originalEnrolledTid);
        if (converted.ec != std::errc{}) return false;
        const auto begin=[&](const char* role) noexcept {
            const bool clock=deadline.Observe(role,false);
            if(!clock&&!independentlyEligible)return false;
            return !supervisor_||(independentlyEligible?
                supervisor_->BeginIndependentlyEligibleCleanupNativeCall():supervisor_->BeginNativeCall());
        };
        if (!begin("proc-task-directory-open")) return false;
        directory_.openAttempted = true;
        directory_.openResult = openat(root.OriginalDescriptor().original,literal,O_PATH|O_DIRECTORY|O_NOFOLLOW|O_CLOEXEC);
        directory_.original = directory_.openResult;
        if (directory_.openResult < 0) directory_.openError = errno;
        directory_.openReturned=true;
        if(supervisor_)supervisor_->OriginalNativeCallReturned();
        const bool openClock = deadline.Observe("proc-task-directory-open",true);
        if (directory_.openResult < 0 || (!openClock&&!independentlyEligible)) return false;
        if (!begin("proc-task-fstatfs")) return false;
        filesystemResult_ = fstatfs(directory_.original,&filesystem_);
        if (filesystemResult_ < 0) filesystemError_ = errno;
        if(supervisor_)supervisor_->OriginalNativeCallReturned();
        const bool fsClock = deadline.Observe("proc-task-fstatfs",true);
        if (!begin("proc-task-fstat")) return false;
        statResult_ = fstat(directory_.original,&directoryStat_);
        if (statResult_ < 0) statError_ = errno;
        if(supervisor_)supervisor_->OriginalNativeCallReturned();
        const bool statClock = deadline.Observe("proc-task-fstat",true);
        if ((!independentlyEligible&&(!fsClock||!statClock)) || filesystemResult_ < 0 || statResult_ < 0 || filesystem_.f_type != PROC_SUPER_MAGIC || !S_ISDIR(directoryStat_.st_mode)) return false;
        auto* observation = Observe(independentlyEligible);
        if (!observation || !observation->parsed || observation->observedPid != originalEnrolledTid) return false;
        firstPid_ = observation->observedPid; firstTgid_ = observation->observedTgid; firstStartTicks_ = observation->startTicks;
        bound_ = true; return true;
    }
    const ProcTaskObservation* Observe(bool independentlyEligible=false) noexcept {
        if(!preowned_)return nullptr;
        if(independentlyEligible&&!supervisor_)return nullptr;
        if (!root_ || !root_->Available() || directory_.original < 0 || directory_.closeAttempted || count_ == capacity_) return nullptr;
        auto& row = *observations_[count_++]; row.attempted = true;
        // Both independent acquisitions/captures are attempted and retained.
        const bool statOk = row.stat.Capture(directory_.original,"stat",*root_->OriginalDeadlineOwner(),supervisor_,independentlyEligible);
        const bool statusOk = row.status.Capture(directory_.original,"status",*root_->OriginalDeadlineOwner(),supervisor_,independentlyEligible);
        row.parsed = statOk && statusOk && row.Parse(); return &row;
    }
    bool SameOriginal(const ProcTaskObservation& observation) const noexcept {
        bool retainedHere = false;
        for (size_t i=0; i<count_; ++i)
            if (observations_[i].get() == &observation) { retainedHere = true; break; }
        return retainedHere && bound_ && observation.parsed && observation.observedPid == firstPid_ &&
            observation.observedTgid == firstTgid_ && observation.startTicks == firstStartTicks_;
    }
    bool DisposeEligible(OriginalDeadlineSupervisor* actualSupervisor=nullptr) noexcept {
        auto* closeSupervisor=actualSupervisor?actualSupervisor:supervisor_;
        bool all = true;
        for (size_t i=0; i<count_; ++i) {
            auto& row = *observations_[i];
            row.stat.descriptor.CloseOnce(root_ ? root_->OriginalDeadlineOwner() : nullptr,closeSupervisor);
            row.status.descriptor.CloseOnce(root_ ? root_->OriginalDeadlineOwner() : nullptr,closeSupervisor);
            all &= (row.stat.descriptor.original < 0 || row.stat.descriptor.closed) &&
                   (row.status.descriptor.original < 0 || row.status.descriptor.closed);
        }
        directory_.CloseOnce(root_ ? root_->OriginalDeadlineOwner() : nullptr,closeSupervisor);
        return all && (directory_.original < 0 || directory_.closed);
    }
    bool ReservedSourceBytes(size_t& bytes) const noexcept {
        bytes = sizeof(*this);
        auto add = [&](size_t amount) noexcept {
            if (amount > SIZE_MAX-bytes) return false; bytes += amount; return true;
        };
        if (!add(capacity_*sizeof(std::unique_ptr<ProcTaskObservation>))) return false;
        for (size_t i=0; i<bornCount_; ++i) {
            const auto& row = *observations_[i];
            if (!add(sizeof(ProcTaskObservation)) || !add(row.stat.bytes?row.stat.byteCapacity+1:0) || !add(row.status.bytes?row.status.byteCapacity+1:0) ||
                !add(row.stat.readCapacity*sizeof(ProcReadOriginal)) || !add(row.status.readCapacity*sizeof(ProcReadOriginal))) return false;
        }
        return true; // No allocator/kernel/native runtime fit is claimed.
    }
    const ProcOwnedFd& OriginalDirectory() const noexcept { return directory_; }
    OriginalProcRoot* OriginalRootOwner() const noexcept { return root_; }
    OriginalDeadline* OriginalDeadlineOwner() const noexcept {return root_?root_->OriginalDeadlineOwner():nullptr;}
    OriginalDeadlineSupervisor* OriginalSupervisorOwner() const noexcept {return supervisor_;}
    const struct statfs& OriginalFilesystem() const noexcept { return filesystem_; }
    int OriginalFilesystemResult() const noexcept { return filesystemResult_; }
    int OriginalFilesystemError() const noexcept { return filesystemError_; }
    const struct stat& OriginalDirectoryStat() const noexcept { return directoryStat_; }
    int OriginalDirectoryStatResult() const noexcept { return statResult_; }
    int OriginalDirectoryStatError() const noexcept { return statError_; }
    const ProcTaskObservation& OriginalObservation(size_t ordinal) const { return *observations_[ordinal]; }
    size_t ObservationCount() const noexcept { return count_; }
    size_t AllocatedObservationCapacityOriginal() const noexcept{return observations_?capacity_:0;}
    const void* AllocatedObservationSlotsOriginal() const noexcept{return observations_.get();}
    size_t BornObservationCountOriginal() const noexcept{return bornCount_;}
    const ProcTaskObservation* AllocatedObservationOriginal(size_t ordinal) const noexcept {
        return observations_&&ordinal<capacity_?observations_[ordinal].get():nullptr;
    }
};
// A terminal wait reaps metadata: concrete BeforeWait association and exact same
// wait event must bind the retained prior observation before BeforeTerminal.
// Nonleader EXEC requires original/current TGID/pidfd and GETEVENTMSG relation;
// this observer never fabricates that relation or sibling terminal statuses.
}

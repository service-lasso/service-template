// UNCOMPILED fresh private-candidate file producer. Never adopts an ambient
// installed tool as lease-eligible and never truncates/replaces an existing name.
#pragma once
#include "linux_original_deadline.hpp"
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <array>
#include <cstring>
namespace template_creator_linux {
class OriginalLeaseOwner;
class OriginalPrivateVerity;
struct CandidateFdOriginal {
    int returned=-1, error=0, closeReturned=-1, closeError=0;
    bool attempted=false, closeAttempted=false, closed=false;
    bool CloseOnce(OriginalDeadline& clock) noexcept {
        if (!attempted || returned<0) return true;
        if (closeAttempted) return closed;
        clock.Observe("candidate-close",false);
        closeAttempted=true; closeReturned=close(returned);
        if (closeReturned<0) closeError=errno;
        closed=closeReturned==0;
        clock.Observe("candidate-close",true);
        return closed;
    }
};
struct CandidateWriteOriginal {
    size_t offset=0, requested=0;
    ssize_t returned=-1; int error=0; bool attempted=false;
};
class OriginalPrivateCandidateFile {
    friend class OriginalLeaseOwner;
    friend class OriginalPrivateVerity;
    std::unique_ptr<unsigned char[]> source_;
    std::unique_ptr<CandidateWriteOriginal[]> writes_;
    size_t bytes_=0, writeCapacity_=0, writeCount_=0, written_=0;
    std::array<char,256> leaf_{};
    int originalRoot_=-1;
    mode_t selectedCreateMode_=0600;
    CandidateFdOriginal writer_, reader_;
    struct stat rootStat_{}, bornStat_{}, completedStat_{}, readerStat_{};
    int rootStatResult_=-1, rootStatError_=0, bornStatResult_=-1, bornStatError_=0;
    int completedStatResult_=-1, completedStatError_=0, readerStatResult_=-1, readerStatError_=0;
    bool attempted_=false, produced_=false, failed_=false, leaseAttached_=false;
    bool disposalAttempted_=false, disposalComplete_=false;
    OriginalDeadline* clock_=nullptr;
    // This type's only lease-eligible descriptor comes from its actual fresh
    // O_EXCL creation and SAME-inode reopen, never a supplied eligibility flag.
    bool LeaseEligibleOriginal() const noexcept {
        return produced_ && !failed_ && !leaseAttached_ && writer_.closed &&
            reader_.returned>=0 && !reader_.closeAttempted;
    }
    ~OriginalPrivateCandidateFile()=default; // Resident through any lease worker.
public:
    OriginalPrivateCandidateFile(const char* literalLeaf,const void* originalBytes,size_t bytes,size_t writeRows,mode_t selectedCreateMode=0600) {
        if ((selectedCreateMode!=0600 && selectedCreateMode!=0700) ||
            !literalLeaf || (!originalBytes && bytes) || !writeRows ||
            writeRows>SIZE_MAX/sizeof(CandidateWriteOriginal))
            throw std::invalid_argument("fresh candidate source geometry");
        size_t length=0;
        while (length<leaf_.size() && literalLeaf[length]) {
            const unsigned char c=static_cast<unsigned char>(literalLeaf[length]);
            if (!((c>='a'&&c<='z')||(c>='A'&&c<='Z')||(c>='0'&&c<='9')||c=='_'||c=='-'||c=='.'))
                throw std::invalid_argument("literal private candidate leaf");
            ++length;
        }
        if (!length || length==leaf_.size() || (length==1&&literalLeaf[0]=='.') ||
            (length==2&&literalLeaf[0]=='.'&&literalLeaf[1]=='.'))
            throw std::invalid_argument("private candidate leaf extent");
        std::memcpy(leaf_.data(),literalLeaf,length);
        source_=std::make_unique<unsigned char[]>(bytes);
        if (bytes) std::memcpy(source_.get(),originalBytes,bytes);
        writes_=std::make_unique<CandidateWriteOriginal[]>(writeRows);
        bytes_=bytes; writeCapacity_=writeRows; selectedCreateMode_=selectedCreateMode;
    }
    OriginalPrivateCandidateFile(const OriginalPrivateCandidateFile&)=delete;
    OriginalPrivateCandidateFile& operator=(const OriginalPrivateCandidateFile&)=delete;
    bool Produce(int actualOwnedCandidateRoot,OriginalDeadline& clock) noexcept {
        if (attempted_) return false;
        attempted_=true; originalRoot_=actualOwnedCandidateRoot; clock_=&clock;
        if (!clock.Observe("candidate-root-stat",false)) { failed_=true; return false; }
        rootStatResult_=fstat(originalRoot_,&rootStat_);
        if (rootStatResult_<0) rootStatError_=errno;
        const bool rootAfter=clock.Observe("candidate-root-stat",true);
        if (rootStatResult_ || !S_ISDIR(rootStat_.st_mode) || !rootAfter) { failed_=true; return false; }
        if (!clock.Observe("candidate-exclusive-create",false)) { failed_=true; return false; }
        writer_.attempted=true;
        writer_.returned=openat(originalRoot_,leaf_.data(),O_WRONLY|O_CREAT|O_EXCL|O_NOFOLLOW|O_CLOEXEC,selectedCreateMode_);
        if (writer_.returned<0) writer_.error=errno;
        const bool createAfter=clock.Observe("candidate-exclusive-create",true);
        if (writer_.returned<0 || !createAfter) { failed_=true; return false; }
        if (!clock.Observe("candidate-birth-stat",false)) { failed_=true; return false; }
        bornStatResult_=fstat(writer_.returned,&bornStat_);
        if (bornStatResult_<0) bornStatError_=errno;
        const bool birthAfter=clock.Observe("candidate-birth-stat",true);
        if (bornStatResult_ || !S_ISREG(bornStat_.st_mode) || bornStat_.st_size!=0 ||
            (bornStat_.st_mode&07777)!=selectedCreateMode_ || !birthAfter) {
            failed_=true; return false;
        }
        while (written_<bytes_) {
            if (writeCount_==writeCapacity_) { failed_=true; return false; }
            auto& row=writes_[writeCount_++]; row.offset=written_;
            row.requested=bytes_-written_>65536?65536:bytes_-written_;
            if (!clock.Observe("candidate-write",false)) { failed_=true; return false; }
            row.attempted=true; row.returned=write(writer_.returned,source_.get()+row.offset,row.requested);
            if (row.returned<0) row.error=errno;
            const bool after=clock.Observe("candidate-write",true);
            if (row.returned>0 && static_cast<size_t>(row.returned)<=row.requested)
                written_+=static_cast<size_t>(row.returned);
            if (row.returned<=0 || static_cast<size_t>(row.returned)>row.requested || !after) { failed_=true; return false; }
        }
        if (!clock.Observe("candidate-completed-stat",false)) { failed_=true; return false; }
        completedStatResult_=fstat(writer_.returned,&completedStat_);
        if (completedStatResult_<0) completedStatError_=errno;
        const bool completedAfter=clock.Observe("candidate-completed-stat",true);
        if (completedStatResult_ || completedStat_.st_dev!=bornStat_.st_dev || completedStat_.st_ino!=bornStat_.st_ino ||
            completedStat_.st_size<0 || static_cast<uintmax_t>(completedStat_.st_size)!=bytes_ || !completedAfter) {
            failed_=true; return false;
        }
        if (!writer_.CloseOnce(clock) || !clock.QualificationClean()) { failed_=true; return false; }
        if (!clock.Observe("candidate-readonly-open",false)) { failed_=true; return false; }
        reader_.attempted=true;
        reader_.returned=openat(originalRoot_,leaf_.data(),O_RDONLY|O_NOFOLLOW|O_CLOEXEC);
        if (reader_.returned<0) reader_.error=errno;
        const bool readerAfter=clock.Observe("candidate-readonly-open",true);
        if (reader_.returned<0 || !readerAfter) { failed_=true; return false; }
        if (!clock.Observe("candidate-readonly-stat",false)) { failed_=true; return false; }
        readerStatResult_=fstat(reader_.returned,&readerStat_);
        if (readerStatResult_<0) readerStatError_=errno;
        const bool statAfter=clock.Observe("candidate-readonly-stat",true);
        produced_=statAfter && readerStatResult_==0 && S_ISREG(readerStat_.st_mode) &&
            readerStat_.st_dev==bornStat_.st_dev && readerStat_.st_ino==bornStat_.st_ino &&
            (readerStat_.st_mode&07777)==selectedCreateMode_ &&
            readerStat_.st_size>=0 && static_cast<uintmax_t>(readerStat_.st_size)==bytes_;
        failed_|=!produced_; return produced_;
    }
    bool DisposeUnleasedEligible() noexcept {
        if (leaseAttached_) return false;
        if (disposalAttempted_) return disposalComplete_;
        disposalAttempted_=true;
        if (!clock_) { disposalComplete_=!attempted_; return disposalComplete_; }
        const bool writerClosed=writer_.CloseOnce(*clock_);
        const bool readerClosed=reader_.CloseOnce(*clock_);
        disposalComplete_=writerClosed&&readerClosed;
        return disposalComplete_;
    }
    const unsigned char* PrivateSourceBytes() const noexcept { return source_.get(); }
    size_t SourceByteCount() const noexcept { return bytes_; }
    const CandidateWriteOriginal* PrivateWrites() const noexcept { return writes_.get(); }
    size_t WriteCount() const noexcept { return writeCount_; }
    const CandidateFdOriginal& OriginalReader() const noexcept { return reader_; }
    const CandidateFdOriginal& OriginalWriter() const noexcept { return writer_; }
    mode_t OriginalCreateMode() const noexcept {return selectedCreateMode_;}
    const struct stat& OriginalBirthStat() const noexcept {return bornStat_;}
    const struct stat& OriginalReaderStat() const noexcept {return readerStat_;}
    bool Produced() const noexcept { return produced_&&!failed_; }
    bool ReservedSourceBytes(size_t& bytes) const noexcept {
        bytes=sizeof(*this);
        if (bytes_>SIZE_MAX-bytes) return false; bytes+=bytes_;
        const size_t rows=writeCapacity_*sizeof(CandidateWriteOriginal);
        if (rows>SIZE_MAX-bytes) return false; bytes+=rows; return true;
    }
};
// Produced() is a physical creation/reopen fact, NOT source authority, full
// content readback, root membership exclusion, durability or native acceptance.
}

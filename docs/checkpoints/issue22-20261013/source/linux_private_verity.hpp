// UNCOMPILED optional private-content backend. Never touches installed/shared files.
// Not selected platform authority: path/mount/source/loaded joins are separate.
#pragma once
#include "linux_private_candidate_file.hpp"
#include "linux_deadline_supervisor.hpp"
#include <linux/fsverity.h>
#include <sys/ioctl.h>
#include <algorithm>
#include <limits>
namespace template_creator_linux {
struct VerityReadOriginal {
    size_t offset=0,requested=0; ssize_t returned=-1; int error=0;
    bool attempted=false;
};
class OriginalPrivateVerity {
    OriginalPrivateCandidateFile* file_=nullptr;
    OriginalDeadline* clock_=nullptr;
    OriginalDeadlineSupervisor& supervisor_;
    fsverity_enable_arg originalEnable_{},nativeEnable_{};
    std::unique_ptr<unsigned char[]> observed_;
    std::unique_ptr<VerityReadOriginal[]> reads_;
    size_t capacity_=0,count_=0,extent_=0;
    bool attempted_=false,enableNativeAttempted_=false,enabled_=false,readback_=false,failed_=false;
    int enableReturned_=-1,enableError_=0;
    ~OriginalPrivateVerity()=default; // Original owner remains resident.
public:
    OriginalPrivateVerity(OriginalPrivateCandidateFile& file,size_t readRows,uint32_t selectedBlockBytes,
                          OriginalDeadlineSupervisor& actualSupervisor)
        :file_(&file),supervisor_(actualSupervisor),capacity_(readRows) {
        if(!readRows || readRows>SIZE_MAX/sizeof(VerityReadOriginal) ||
            file.SourceByteCount()==SIZE_MAX ||
            file.SourceByteCount()>static_cast<uintmax_t>(std::numeric_limits<off_t>::max()) ||
            selectedBlockBytes<1024 || (selectedBlockBytes&(selectedBlockBytes-1)))
            throw std::invalid_argument("private verity read geometry");
        observed_=std::make_unique<unsigned char[]>(file.SourceByteCount()+1);
        reads_=std::make_unique<VerityReadOriginal[]>(readRows);
        originalEnable_.version=1;
        originalEnable_.hash_algorithm=FS_VERITY_HASH_ALG_SHA256;
        originalEnable_.block_size=selectedBlockBytes;
        // Actual kernel/page/filesystem support is checked by the retained ioctl.
        // All reserved fields remain zero.
        // No signature/keyring/feature-enabling operation is fabricated.
    }
    OriginalPrivateVerity(const OriginalPrivateVerity&)=delete;
    OriginalPrivateVerity& operator=(const OriginalPrivateVerity&)=delete;
    bool EnableAndReadback(OriginalDeadline& clock) noexcept {
        if(attempted_) return false;
        attempted_=true;clock_=&clock;
        // Actual producer association, closed writer and SAME retained reader.
        // A held lease is not consumed as permission for this irreversible ioctl.
        if(supervisor_.OriginalDeadlineOwner()!=&clock || !file_->LeaseEligibleOriginal()) { failed_=true;return false; }
        nativeEnable_=originalEnable_;
        if(!clock.Observe("private-verity-enable",false)) { failed_=true;return false; }
        if(!supervisor_.BeginNativeCall()){failed_=true;return false;}
        enableNativeAttempted_=true;
        enableReturned_=ioctl(file_->reader_.returned,FS_IOC_ENABLE_VERITY,&nativeEnable_);
        if(enableReturned_<0) enableError_=errno;
        enabled_=enableReturned_==0; // Preserve physical result before clock result.
        supervisor_.OriginalNativeCallReturned();
        const bool after=clock.Observe("private-verity-enable",true);
        if(!enabled_ || !after || supervisor_.ExpiredOrFailed()) { failed_=true;return false; }
        const size_t size=file_->SourceByteCount();
        while(extent_<=size) {
            if(count_==capacity_) { failed_=true;return false; }
            auto& row=reads_[count_++];row.offset=extent_;
            row.requested=extent_==size?1:std::min<size_t>(65536,size-extent_);
            if(!clock.Observe("private-verity-readback",false)) { failed_=true;return false; }
            if(!supervisor_.BeginNativeCall()){failed_=true;return false;}
            row.attempted=true;
            row.returned=pread(file_->reader_.returned,observed_.get()+row.offset,row.requested,
                               static_cast<off_t>(row.offset));
            if(row.returned<0) row.error=errno;
            if(row.returned>0 && static_cast<size_t>(row.returned)<=row.requested)
                extent_+=static_cast<size_t>(row.returned);
            supervisor_.OriginalNativeCallReturned();
            const bool readAfter=clock.Observe("private-verity-readback",true);
            if(row.returned<0 || static_cast<size_t>(row.returned)>row.requested || !readAfter || supervisor_.ExpiredOrFailed()) {
                failed_=true;return false;
            }
            if(row.offset==size) {
                readback_=row.returned==0 && (!size ||
                    std::memcmp(observed_.get(),file_->PrivateSourceBytes(),size)==0);
                failed_|=!readback_;return readback_;
            }
            if(row.returned==0) { failed_=true;return false; }
        }
        failed_=true;return false;
    }
    bool ContentSealedAndReadback() const noexcept {
        return enabled_&&readback_&&!failed_;
    }
    const OriginalPrivateCandidateFile& OriginalFile() const noexcept {return *file_;}
    const fsverity_enable_arg& OriginalEnable() const noexcept {return originalEnable_;}
    const fsverity_enable_arg& ReturnedEnableStorage() const noexcept {return nativeEnable_;}
    bool EnableNativeAttempted() const noexcept {return enableNativeAttempted_;}
    int EnableReturned() const noexcept {return enableReturned_;}
    int EnableError() const noexcept {return enableError_;}
    const VerityReadOriginal* PrivateReads() const noexcept {return reads_.get();}
    size_t ReadCount() const noexcept {return count_;}
    const unsigned char* PrivateObservedBytes() const noexcept {return observed_.get();}
    size_t RetainedExtent() const noexcept {return extent_;}
    bool ReservedSourceBytes(size_t& bytes) const noexcept {
        bytes=sizeof(*this);const size_t body=file_->SourceByteCount()+1;
        if(body>SIZE_MAX-bytes)return false;bytes+=body;
        const size_t rows=capacity_*sizeof(VerityReadOriginal);
        if(rows>SIZE_MAX-bytes)return false;bytes+=rows;return true;
    }
};
// Successful ioctl + post-enable exact full readback binds this private inode's
// contents only. It neither protects its directory entry nor authenticates the
// original runtime/tool source, and does not permit any native command launch.
}

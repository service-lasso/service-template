// UNCOMPILED native range custody for SP1. No structural or source admission.
#pragma once
#ifndef _WIN32
#include "linux_received_subject_readback.hpp"
namespace template_creator_linux {
class OriginalReceivedSourceUnitRange {
    friend class template_creator_bridge::OriginalLinuxSelectionStartup;
    friend class OriginalInitialProviderArchive;
    friend bool VerifySurvivingPendingHostPreownRegressionOriginal();
    friend class OriginalReceivedSourceUnitObservation;
    friend class OriginalUnitPayloadArchive;
    friend struct template_creator_bridge::NativePlatformState;
public:
    struct NativeOriginal : OriginalReceivedSubjectReadback::NativeOriginal {
        size_t destinationOffset=0;
    };
private:
    const OriginalInitialProviderReceiver& receiver_;
    OriginalDeadline& deadline_;OriginalDeadlineSupervisor& supervisor_;
    const OriginalInitialProviderReceiver::OfferOriginal* offer_=nullptr;
    std::unique_ptr<unsigned char[]> bytes_;
    std::unique_ptr<NativeOriginal[]> reads_;
    NativeOriginal before_{},access_{},eof_{},after_{};
    OriginalClockBoundary finalClock_{};
    OriginalClockBoundary digestClock_{};
    template_creator_source::OriginalSha256 digest_;
    std::array<unsigned char,32> expectedDigest_{};
    size_t offset_=0,extent_=0,unitBytes_=0,capacity_=0,count_=0,retained_=0,sourceBytes_=0;
    int borrowedFd_=-1;
    bool started_=false,observed_=false,failed_=false,borrowReleased_=false;
    bool digestAttempted_=false,digestObserved_=false;
    ~OriginalReceivedSourceUnitRange()=default;
    static bool Same(const struct stat& a,const struct stat& b) noexcept {
        return a.st_dev==b.st_dev&&a.st_ino==b.st_ino&&a.st_mode==b.st_mode&&a.st_size==b.st_size&&
            a.st_mtim.tv_sec==b.st_mtim.tv_sec&&a.st_mtim.tv_nsec==b.st_mtim.tv_nsec&&
            a.st_ctim.tv_sec==b.st_ctim.tv_sec&&a.st_ctim.tv_nsec==b.st_ctim.tv_nsec;
    }
    bool Begin(NativeOriginal& row) noexcept {
        if(!row.clock.Before(&deadline_,"received-source-unit-range",false)||
           !supervisor_.BeginNativeCall()){failed_=true;return false;}
        row.attempted=true;return true;
    }
    void Returned(NativeOriginal& row,ssize_t value,int error) noexcept {
        row.result=value;row.error=error;row.returned=true;
        supervisor_.OriginalNativeCallReturned();row.clock.After(&deadline_,"received-source-unit-range");
        if(!row.clock.clean)failed_=true;
    }
public:
    static size_t CheckedSourceBacking(size_t offset,size_t extent,size_t unitBytes,size_t rows){
        if(!rows||offset>unitBytes||extent>unitBytes-offset||
           unitBytes>static_cast<uintmax_t>(std::numeric_limits<off_t>::max())||
           extent>SIZE_MAX-sizeof(OriginalReceivedSourceUnitRange)||
           rows>(SIZE_MAX-sizeof(OriginalReceivedSourceUnitRange)-extent)/sizeof(NativeOriginal))
            throw std::invalid_argument("received source-unit range geometry");
        return sizeof(OriginalReceivedSourceUnitRange)+extent+rows*sizeof(NativeOriginal);
    }
private:
    OriginalReceivedSourceUnitRange(const OriginalInitialProviderReceiver& receiver,
        size_t offset,size_t extent,size_t unitBytes,size_t rows,OriginalDeadline& deadline,
        OriginalDeadlineSupervisor& supervisor):receiver_(receiver),deadline_(deadline),supervisor_(supervisor),
        offset_(offset),extent_(extent),unitBytes_(unitBytes),capacity_(rows),
        sourceBytes_(0) {
        if(supervisor.OriginalDeadlineOwner()!=&deadline)throw std::invalid_argument("source-unit deadline owner");
    }
    bool preownAttempted_=false,preownReturned_=false,preowned_=false;
    size_t allocatedBytes_=0,allocatedReads_=0;std::exception_ptr preownException_;
    bool PreownStorageOriginal() noexcept {
        if(preownAttempted_)return false;preownAttempted_=true;
        struct Returned {bool& value;~Returned(){value=true;}} returned{preownReturned_};
        try {
            sourceBytes_=CheckedSourceBacking(offset_,extent_,unitBytes_,capacity_);
            bytes_=std::make_unique<unsigned char[]>(extent_);allocatedBytes_=extent_;
            reads_=std::make_unique<NativeOriginal[]>(capacity_);allocatedReads_=capacity_;
            preowned_=true;return true;
        }catch(...){if(!preownException_)preownException_=std::current_exception();failed_=true;return false;}
    }    bool CaptureOriginal(size_t offerOrdinal) noexcept {
        if(!preowned_||started_||borrowReleased_)return false;started_=true;
        offer_=receiver_.OriginalOffer(offerOrdinal);
        if(!offerOrdinal||!offer_||!offer_->rawOfferObserved||!offer_->returned||offer_->rightsCount!=1||
           offer_->readinessSupervisor!=&supervisor_||supervisor_.OriginalDeadlineOwner()!=&deadline_||
           offer_->decoded.role==SLCLI_BQ_SUBJECT_REFERENCE)return false;
        const auto& fd=offer_->rights[0];
        if(fd.original<0||fd.closeAttempted||!fd.statReturned||fd.statResult||!S_ISREG(fd.status.st_mode))return false;
        borrowedFd_=fd.original;
        if(!Begin(before_))return false;
        const int first=fstat(borrowedFd_,&before_.metadata);const int firstError=first<0?errno:0;
        Returned(before_,first,firstError);
        if(first||failed_||before_.metadata.st_size<0||
           static_cast<uintmax_t>(before_.metadata.st_size)!=unitBytes_||!Same(fd.status,before_.metadata))return false;
        if(!Begin(access_))return false;
        const int flags=fcntl(borrowedFd_,F_GETFL);const int flagsError=flags<0?errno:0;
        Returned(access_,flags,flagsError);
        if(flags<0||failed_||(flags&O_ACCMODE)!=O_RDONLY||(flags&O_PATH))return false;
        while(retained_<extent_){
            if(count_==capacity_){failed_=true;return false;}
            auto& row=reads_[count_++];row.offset=offset_+retained_;
            row.destinationOffset=retained_;
            row.requested=std::min<size_t>(65536,extent_-retained_);
            if(!Begin(row))return false;
            const ssize_t got=pread(borrowedFd_,bytes_.get()+retained_,row.requested,static_cast<off_t>(row.offset));
            const int error=got<0?errno:0;Returned(row,got,error);
            if(got<0||static_cast<size_t>(got)>row.requested){row.extentUnknown=true;failed_=true;return false;}
            retained_+=static_cast<size_t>(got);if(!got||failed_)return false;
        }
        eof_.offset=unitBytes_;eof_.requested=1;if(!Begin(eof_))return false;
        const ssize_t end=pread(borrowedFd_,&eof_.eofByte,1,static_cast<off_t>(unitBytes_));
        const int endError=end<0?errno:0;Returned(eof_,end,endError);
        if(end<0||end>1)eof_.extentUnknown=true;if(end||failed_)return false;
        if(!Begin(after_))return false;
        const int last=fstat(borrowedFd_,&after_.metadata);const int lastError=last<0?errno:0;
        Returned(after_,last,lastError);
        const bool identity=last==0&&!failed_&&Same(before_.metadata,after_.metadata);
        const bool clock=IngressNativeClock::Capture(finalClock_,&deadline_,"source-unit-range-final",true);
        observed_=identity&&clock&&finalClock_.shapeValid&&!finalClock_.expired&&
            finalClock_.nanoseconds>=after_.clock.after.nanoseconds;
        return observed_; // Neither member/hash equality nor canonical unit verification.
    }
    bool ObserveRangeDigestOriginal(const std::array<unsigned char,32>& expected) noexcept {
        if(digestAttempted_||!observed_||borrowReleased_||retained_!=extent_||
           supervisor_.HasPendingOriginalNativeCall())return false;
        digestAttempted_=true;expectedDigest_=expected;
        const bool equal=digest_.Update(bytes_.get(),retained_)&&digest_.Finish()&&
            digest_.PrivateDigest()==expectedDigest_;
        const bool clock=IngressNativeClock::Capture(digestClock_,&deadline_,"source-unit-range-digest",true);
        digestObserved_=equal&&clock&&digestClock_.shapeValid&&!digestClock_.expired&&
            digestClock_.nanoseconds>=finalClock_.nanoseconds;
        return digestObserved_; // Correspondence only; authentic A/member relation remains separate.
    }
    bool ReleaseBeforeExposureOriginal() noexcept {
        if(supervisor_.HasPendingOriginalNativeCall()||eof_.extentUnknown||
           (before_.attempted&&!before_.returned)||(after_.attempted&&!after_.returned)||
           (access_.attempted&&!access_.returned)||
           (eof_.attempted&&!eof_.returned))return false;
        for(size_t i=0;i<count_;++i)if(reads_[i].extentUnknown||(reads_[i].attempted&&!reads_[i].returned))return false;
        borrowReleased_=true;return true;
    }
public:
    struct RetainedPreownBackingView {
        const OriginalReceivedSourceUnitRange* ownerOriginal;const unsigned char* bytesOriginal;
        const NativeOriginal* readsOriginal;const std::exception_ptr* exceptionOriginal;
        size_t offset,extent,byteCapacity,readCapacity,readCount,retainedBytes;
        bool attempted,returned,preowned,released;
    };
    bool BackingStorageSeparateOriginal(const void* output,size_t bytes) const noexcept {
        return OriginalBackingStorageSeparate(output,bytes,this,1,sizeof(*this))&&
            OriginalBackingStorageSeparate(output,bytes,bytes_.get(),allocatedBytes_)&&
            OriginalBackingStorageSeparate(output,bytes,reads_.get(),allocatedReads_,sizeof(NativeOriginal));
    }
    bool ObserveRetainedPreownBackingOriginal(RetainedPreownBackingView& out) const noexcept {
        const auto output=reinterpret_cast<uintptr_t>(&out),owner=reinterpret_cast<uintptr_t>(this);
        if(!BackingStorageSeparateOriginal(&out,sizeof(out))||!borrowReleased_||(preownAttempted_&&!preownReturned_)||count_>allocatedReads_||retained_>allocatedBytes_||
           output>UINTPTR_MAX-sizeof(out)||owner>UINTPTR_MAX-sizeof(*this)||
           (output<owner+sizeof(*this)&&owner<output+sizeof(out)))return false;
        std::memset(&out,0,sizeof(out));out.ownerOriginal=this;out.bytesOriginal=bytes_.get();out.readsOriginal=reads_.get();
        out.exceptionOriginal=&preownException_;out.offset=offset_;out.extent=extent_;out.byteCapacity=allocatedBytes_;
        out.readCapacity=allocatedReads_;out.readCount=count_;out.retainedBytes=retained_;
        out.attempted=preownAttempted_;out.returned=preownReturned_;out.preowned=preowned_;out.released=borrowReleased_;return true;
    }
    size_t SourceBackingBytes() const noexcept{return sourceBytes_;}
    bool AllocatedBackingBytesOriginal(size_t& bytes) const noexcept {
        bytes=sizeof(*this);if(allocatedBytes_>SIZE_MAX-bytes)return false;bytes+=allocatedBytes_;
        if(allocatedReads_>(SIZE_MAX-bytes)/sizeof(NativeOriginal))return false;
        bytes+=allocatedReads_*sizeof(NativeOriginal);return true;
    }
    const unsigned char* OriginalBytes() const noexcept{return bytes_.get();}
    size_t RetainedBytes() const noexcept{return retained_;}
    bool RangeObserved() const noexcept{return observed_&&!borrowReleased_;}
};
}
#endif

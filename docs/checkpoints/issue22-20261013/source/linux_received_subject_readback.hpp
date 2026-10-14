// UNCOMPILED received regular-file correspondence, never source admission.
#pragma once
#ifndef _WIN32
#include "linux_upstream_subject_receiver.hpp"
#include "original_sha256.hpp"
#include "upstream-cli/source_association_index.h"
#include "upstream-cli/bq_provider_expectations.h"
#include <algorithm>
#include <limits>
namespace template_creator_linux {
class OriginalReceivedSubjectReadback {
    friend class template_creator_bridge::OriginalLinuxSelectionStartup;
    friend class OriginalInitialProviderReceiver;
    friend class OriginalInitialProviderArchive;
    friend bool VerifySurvivingPendingHostPreownRegressionOriginal();
public:
    struct NativeOriginal {
        IngressNativeClock clock{};struct stat metadata{};
        size_t offset=0,requested=0;ssize_t result=-1;int error=0;
        bool attempted=false,returned=false,extentUnknown=false;
        unsigned char eofByte=0;
    };
private:
    const OriginalInitialProviderReceiver& receiver_;
    const OriginalInitialProviderReceiver::OfferOriginal* offer_=nullptr;
    OriginalDeadline& deadline_;OriginalDeadlineSupervisor& supervisor_;
    std::unique_ptr<unsigned char[]> body_;
    std::unique_ptr<NativeOriginal[]> reads_;
    NativeOriginal before_,eof_,after_;
    OriginalClockBoundary finalVerificationClock_{};
    template_creator_source::OriginalSha256 digest_;
    std::array<unsigned char,32> expectedDigest_{};
    const slcli_bq_provider_expectations* associationSource_=nullptr;
    struct slcli_source_association_reader associationParser_{};
    std::unique_ptr<struct slcli_source_association_record[]> associationRecords_;
    size_t associationCount_=0,associationParsed_=0;
    bool associationAttempted_=false,associationObserved_=false;
    size_t bytes_=0,capacity_=0,count_=0,retained_=0,sourceBytes_=0,offerOrdinal_=SIZE_MAX;
    int borrowedFd_=-1;
    bool captureStarted_=false,complete_=false,failed_=false,borrowReleased_=false;
    bool finalVerificationAttempted_=false,finalVerificationObserved_=false;
    bool preownAttempted_=false,preownReturned_=false,preowned_=false;
    size_t allocatedBodyBytes_=0,allocatedReadCapacity_=0,allocatedAssociationCapacity_=0;
    std::exception_ptr preownException_;
    ~OriginalReceivedSubjectReadback()=default;
    static bool Stable(const struct stat& a,const struct stat& b) noexcept {
        return a.st_dev==b.st_dev&&a.st_ino==b.st_ino&&a.st_mode==b.st_mode&&a.st_size==b.st_size&&
            a.st_mtim.tv_sec==b.st_mtim.tv_sec&&a.st_mtim.tv_nsec==b.st_mtim.tv_nsec&&
            a.st_ctim.tv_sec==b.st_ctim.tv_sec&&a.st_ctim.tv_nsec==b.st_ctim.tv_nsec;
    }
    bool Begin(NativeOriginal& row) noexcept {
        if(!row.clock.Before(&deadline_,"initial-provider-subject-readback",false)||
           !supervisor_.BeginNativeCall()){failed_=true;return false;}
        row.attempted=true;return true;
    }
    void Returned(NativeOriginal& row,ssize_t result,int error) noexcept {
        row.result=result;row.error=error;row.returned=true;
        supervisor_.OriginalNativeCallReturned();row.clock.After(&deadline_,"initial-provider-subject-readback");
        if(!row.clock.clean)failed_=true;
    }
public:
    static size_t CheckedSourceBacking(size_t bytes,size_t rows,const slcli_bq_provider_expectations* source=nullptr) {
        if(!rows||bytes>UINT64_MAX/8||bytes>static_cast<uintmax_t>(std::numeric_limits<off_t>::max())||
           bytes>SIZE_MAX-sizeof(OriginalReceivedSubjectReadback)||
           rows>(SIZE_MAX-sizeof(OriginalReceivedSubjectReadback)-bytes)/sizeof(NativeOriginal))
            throw std::invalid_argument("received subject readback geometry");
        size_t total=sizeof(OriginalReceivedSubjectReadback)+bytes+rows*sizeof(NativeOriginal);
        if(source){
            const auto count=source->association_index_record_count,names=source->association_index_names_bytes;
            if(!count||count!=source->source_unit_record_count||count>(UINT64_MAX-128)/320||
               count>(SIZE_MAX-128)/320||names<count||names>UINT64_MAX-(128+320*count)||
               names>SIZE_MAX-(128+320*count)||128+320*count+names!=bytes||bytes!=source->association_index_bytes||
               count>(SIZE_MAX-total)/sizeof(struct slcli_source_association_record))
                throw std::invalid_argument("received association-index source geometry");
            total+=static_cast<size_t>(count)*sizeof(struct slcli_source_association_record);
        }
        return total;
    }
private:
    OriginalReceivedSubjectReadback(const OriginalInitialProviderReceiver& receiver,size_t bytes,
        const unsigned char* expectedDigest,size_t rows,OriginalDeadline& deadline,OriginalDeadlineSupervisor& supervisor,
        const slcli_bq_provider_expectations* associationSource=nullptr)
        :receiver_(receiver),deadline_(deadline),supervisor_(supervisor),bytes_(bytes),capacity_(rows),
         sourceBytes_(0) {
        if(!expectedDigest||supervisor.OriginalDeadlineOwner()!=&deadline)
            throw std::invalid_argument("received subject original owner");
        std::memcpy(expectedDigest_.data(),expectedDigest,32);associationSource_=associationSource;
    }
    bool PreownStorageOriginal() noexcept {
        if(preownAttempted_)return false;preownAttempted_=true;
        struct Returned {bool& value;~Returned(){value=true;}} returned{preownReturned_};
        try {
            sourceBytes_=CheckedSourceBacking(bytes_,capacity_,associationSource_);
            body_=std::make_unique<unsigned char[]>(bytes_);allocatedBodyBytes_=bytes_;
            reads_=std::make_unique<NativeOriginal[]>(capacity_);allocatedReadCapacity_=capacity_;
            if(associationSource_){associationCount_=static_cast<size_t>(associationSource_->association_index_record_count);
                associationRecords_=std::make_unique<struct slcli_source_association_record[]>(associationCount_);
                allocatedAssociationCapacity_=associationCount_;}
            preowned_=true;return true;
        }catch(...){if(!preownException_)preownException_=std::current_exception();failed_=true;return false;}
    }    bool CaptureOriginal(size_t ordinal) noexcept {
        if(!preowned_||captureStarted_||borrowReleased_)return false;captureStarted_=true;offerOrdinal_=ordinal;
        offer_=receiver_.OriginalOffer(ordinal);
        if(!offer_||!ordinal||!offer_->rawOfferObserved||!offer_->returned||offer_->rightsCount!=1||
           offer_->decoded.role==SLCLI_BQ_SUBJECT_REFERENCE||offer_->decoded.member_bytes!=bytes_)return false;
        const auto& fd=offer_->rights[0];
        if(fd.original<0||fd.closeAttempted||!fd.statReturned||fd.statResult!=0||!S_ISREG(fd.status.st_mode))return false;
        borrowedFd_=fd.original;
        if(!Begin(before_))return false;
        const int first=fstat(borrowedFd_,&before_.metadata);const int firstError=first<0?errno:0;
        Returned(before_,first,firstError);
        if(first||failed_||!S_ISREG(before_.metadata.st_mode)||before_.metadata.st_size<0||
           static_cast<uintmax_t>(before_.metadata.st_size)!=bytes_||!Stable(fd.status,before_.metadata))return false;
        while(retained_<bytes_){
            if(count_==capacity_){failed_=true;return false;}
            auto& row=reads_[count_++];row.offset=retained_;row.requested=std::min<size_t>(65536,bytes_-retained_);
            if(!Begin(row))return false;
            const ssize_t got=pread(borrowedFd_,body_.get()+row.offset,row.requested,static_cast<off_t>(row.offset));
            const int error=got<0?errno:0;Returned(row,got,error);
            if(got<0||static_cast<size_t>(got)>row.requested){row.extentUnknown=true;failed_=true;return false;}
            retained_+=static_cast<size_t>(got);
            if(!got||failed_||!digest_.Update(body_.get()+row.offset,static_cast<size_t>(got))){failed_=true;return false;}
        }
        eof_.offset=bytes_;eof_.requested=1;if(!Begin(eof_))return false;
        const ssize_t end=pread(borrowedFd_,&eof_.eofByte,1,static_cast<off_t>(bytes_));const int endError=end<0?errno:0;
        Returned(eof_,end,endError);if(end<0||end>1)eof_.extentUnknown=true;
        if(end!=0||failed_)return false;
        if(!Begin(after_))return false;
        const int last=fstat(borrowedFd_,&after_.metadata);const int lastError=last<0?errno:0;
        Returned(after_,last,lastError);
        finalVerificationAttempted_=true;
        finalVerificationObserved_=last==0&&!failed_&&Stable(before_.metadata,after_.metadata)&&digest_.Finish()&&
            digest_.PrivateDigest()==expectedDigest_;
        if(finalVerificationObserved_&&associationSource_){
            associationAttempted_=true;
            if(retained_!=bytes_||bytes_<128||!slcli_source_association_header(&associationParser_,body_.get(),bytes_,associationCount_)||
               associationParser_.names_bytes!=associationSource_->association_index_names_bytes)return false;
            for(size_t i=0;i<associationCount_;++i){
                if(!slcli_source_association_record(&associationParser_,body_.get()+128+i*320))return false;
                associationRecords_[i]=associationParser_.previous;++associationParsed_;
            }
            if(!associationParser_.table_complete||associationParser_.failed)return false;
            size_t consumed=0;const size_t names=static_cast<size_t>(associationParser_.names_bytes);
            while(consumed<names){const size_t window=std::min<size_t>(16384,names-consumed);
                if(!slcli_source_association_names(&associationParser_,associationParser_.names_offset+consumed,window))return false;
                consumed+=window;
            }
            associationObserved_=associationParser_.names_complete&&!associationParser_.failed&&associationParsed_==associationCount_;
            if(!associationObserved_)return false;
        }
        const bool finalClock=IngressNativeClock::Capture(finalVerificationClock_,&deadline_,
            "initial-provider-subject-verification",true);
        complete_=finalVerificationObserved_&&finalClock&&finalVerificationClock_.shapeValid&&
            finalVerificationClock_.nanoseconds>=after_.clock.after.nanoseconds;
        return complete_; // Exact bytes/native identity only; no member/source grant.
    }
    bool ReleaseBeforeSubjectExposureOriginal() noexcept {
        if(supervisor_.HasPendingOriginalNativeCall())return false;
        if(eof_.extentUnknown||(before_.attempted&&!before_.returned)||(eof_.attempted&&!eof_.returned)||
           (after_.attempted&&!after_.returned))return false;
        for(size_t i=0;i<count_;++i)if(reads_[i].extentUnknown||(reads_[i].attempted&&!reads_[i].returned))return false;
        borrowReleased_=true;return true; // Does not close the receiver-owned FD.
    }
public:
    struct RetainedPreownBackingView {
        const OriginalReceivedSubjectReadback* ownerOriginal;
        const unsigned char* bodyOriginal;const NativeOriginal* readsOriginal;
        const struct slcli_source_association_record* associationsOriginal;
        const std::exception_ptr* exceptionOriginal;
        size_t bodyCapacity,readCapacity,associationCapacity,readCount,retainedBytes,parsedAssociations;
        bool attempted,returned,preowned,borrowReleased;
    };
    bool ObserveRetainedPreownBackingOriginal(RetainedPreownBackingView& out) const noexcept {
        const auto output=reinterpret_cast<uintptr_t>(&out),owner=reinterpret_cast<uintptr_t>(this);
        if(!OriginalBackingStorageSeparate(&out,sizeof(out),body_.get(),allocatedBodyBytes_)||
           !OriginalBackingStorageSeparate(&out,sizeof(out),reads_.get(),allocatedReadCapacity_,sizeof(NativeOriginal))||
           !OriginalBackingStorageSeparate(&out,sizeof(out),associationRecords_.get(),allocatedAssociationCapacity_,sizeof(struct slcli_source_association_record))||
           !borrowReleased_||(preownAttempted_&&!preownReturned_)||count_>allocatedReadCapacity_||
           retained_>allocatedBodyBytes_||associationParsed_>allocatedAssociationCapacity_||
           output>UINTPTR_MAX-sizeof(out)||owner>UINTPTR_MAX-sizeof(*this)||
           (output<owner+sizeof(*this)&&owner<output+sizeof(out)))return false;
        std::memset(&out,0,sizeof(out));out.ownerOriginal=this;out.bodyOriginal=body_.get();out.readsOriginal=reads_.get();
        out.associationsOriginal=associationRecords_.get();out.exceptionOriginal=&preownException_;
        out.bodyCapacity=allocatedBodyBytes_;out.readCapacity=allocatedReadCapacity_;out.associationCapacity=allocatedAssociationCapacity_;
        out.readCount=count_;out.retainedBytes=retained_;out.parsedAssociations=associationParsed_;
        out.attempted=preownAttempted_;out.returned=preownReturned_;out.preowned=preowned_;out.borrowReleased=borrowReleased_;
        return true; // SAME-realm settled view, never byte serialization or source authority.
    }
    size_t SourceBackingBytes() const noexcept{return sourceBytes_;} // Requested geometry only.
    bool PreownedOriginal() const noexcept{return preowned_;}
    bool PreownReturnedOriginal() const noexcept{return preownReturned_;}
    const std::exception_ptr& PreownExceptionOriginal() const noexcept{return preownException_;}
    bool AllocatedBackingBytesOriginal(size_t& bytes) const noexcept {
        bytes=sizeof(*this);const auto add=[&](size_t n,size_t width){if(width&&n>(SIZE_MAX-bytes)/width)return false;bytes+=n*width;return true;};
        return add(allocatedBodyBytes_,1)&&add(allocatedReadCapacity_,sizeof(NativeOriginal))&&
            add(allocatedAssociationCapacity_,sizeof(struct slcli_source_association_record));
    }
    const unsigned char* OriginalBody() const noexcept{return body_.get();}
    size_t RetainedBytes() const noexcept{return retained_;}
    bool BorrowerReleasedOriginal() const noexcept{return borrowReleased_;}
    bool ReadbackObserved() const noexcept{return complete_&&!borrowReleased_;}
    const slcli_bq_provider_expectations* AssociationSourceOriginal() const noexcept{return associationSource_;}
    const struct slcli_source_association_reader& AssociationParserOriginal() const noexcept{return associationParser_;}
    const struct slcli_source_association_record* AssociationRecordsOriginal() const noexcept{return associationRecords_.get();}
    size_t AssociationRecordCount() const noexcept{return associationParsed_;}
    size_t AssociationRecordCapacity() const noexcept{return allocatedAssociationCapacity_;}
    size_t AllocatedBodyBytesOriginal() const noexcept{return allocatedBodyBytes_;}
    size_t AllocatedReadCapacityOriginal() const noexcept{return allocatedReadCapacity_;}
    bool AssociationObserved() const noexcept{return associationObserved_;}
    const OriginalClockBoundary& FinalVerificationClockOriginal() const noexcept{return finalVerificationClock_;}
};
inline bool OriginalInitialProviderReceiver::AcknowledgeReadbackOriginal(const OriginalReceivedSubjectReadback& original) noexcept {
    if(&original.receiver_!=this||&original.deadline_!=deadline_||&original.supervisor_!=supervisor_||
       !original.complete_||original.borrowReleased_||!original.finalVerificationObserved_||
       !original.finalVerificationClock_.shapeValid||original.finalVerificationClock_.expired||
       original.offer_!=OriginalOffer(original.offerOrdinal_)||!original.offer_||original.offer_->rightsCount!=1||
       original.offer_->rights[0].closeAttempted||original.borrowedFd_!=original.offer_->rights[0].original||
       !OriginalReceivedSubjectReadback::Stable(original.offer_->rights[0].status,original.before_.metadata))return false;
    return SendReadbackAckOriginal(original.offerOrdinal_);
}
}
#endif

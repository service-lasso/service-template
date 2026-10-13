// UNCOMPILED. Destination-native custody/correspondence, never policy authority.
#pragma once
#ifndef _WIN32
#include "linux_ingress.hpp"
#include "linux_deadline_supervisor.hpp"
#include "linux_receipt_foundation_geometry.hpp"
#include "original_sha256.hpp"
#include "bq_provider_expectations.h"
#include <array>
#include <memory>
#include <exception>
#include <cerrno>
#include <stdexcept>
#include <cstring>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
namespace template_creator_linux {
class OriginalLinuxPrebirthRetentionPolicy {
public:
    struct CallOriginal {
        enum class Operation {ParentOpen,PolicyOpen,ParentStat,PolicyStat,Flags,Read,Eof,ParentAfter,PolicyAfter,PolicyClose,ParentClose};
        Operation operation{};
        int descriptor=-1,directory=-1,flags=0;
        const char* path=nullptr;
        unsigned char* destination=nullptr;
        size_t requested=0;off_t offset=0;
        bool captureStarted=false,attempted=false,returnedObserved=false,extentUnknown=false;
        ssize_t result=-1;int error=0;struct stat metadata{};
        IngressNativeClock clock;
    };
    struct RetainedOriginalView {
        const OriginalLinuxPrebirthRetentionPolicy* owner=nullptr;
        const slcli_bq_destination_retention_constraints* source=nullptr;
        const slcli_bq_destination_retention_constraints* snapshot=nullptr;
        const char* parentPath=nullptr;const char* policyName=nullptr;
        size_t parentPathBytes=0,policyNameBytes=0;
        const unsigned char* body=nullptr;size_t bodyCapacity=0,verifiedCursor=0;
        const CallOriginal* readRows=nullptr;size_t readCount=0,readCapacity=0;
        const CallOriginal* parentOpen=nullptr;const CallOriginal* policyOpen=nullptr;
        const CallOriginal* parentBefore=nullptr;const CallOriginal* before=nullptr;const CallOriginal* flags=nullptr;
        const CallOriginal* eof=nullptr;const CallOriginal* parentAfter=nullptr;const CallOriginal* after=nullptr;
        const CallOriginal* policyClose=nullptr;const CallOriginal* parentClose=nullptr;
        const slcli_bq_retention_policy_values* parsedValues=nullptr;
        const OriginalClockBoundary* finalClock=nullptr;
        const std::exception_ptr* allocationException=nullptr;
        bool parentAllocationAttempted=false,nameAllocationAttempted=false,rowsAllocationAttempted=false;
        bool preownAttempted=false,preowned=false,captureStarted=false,captureReturned=false;
        bool parseAttempted=false,parsed=false,correspondence=false,failed=false;
    };
private:
    const slcli_bq_destination_retention_constraints* source_;
    slcli_bq_destination_retention_constraints snapshot_{};
    OriginalDeadline& deadline_;OriginalDeadlineSupervisor& supervisor_;
    std::unique_ptr<char[]> parentPath_,name_;
    std::unique_ptr<CallOriginal[]> reads_;
    std::array<unsigned char,SLCLI_BQ_RETENTION_POLICY_BYTES> body_{};
    unsigned char eofByte_=0;
    CallOriginal parentOpen_,policyOpen_,parentBefore_,before_,flags_,eof_,parentAfter_,after_,policyClose_,parentClose_;
    slcli_bq_retention_policy_values values_{};
    template_creator_receipts::ReservationShape reservation_{};
    template_creator_source::OriginalSha256 digest_;
    OriginalClockBoundary final_{};
    int parent_=-1,file_=-1;
    size_t capacity_=0,count_=0,used_=0;
    bool preownAttempted_=false,preowned_=false,captureStarted_=false,captureReturned_=false;
    bool futureCallsClosed_=false,correspondence_=false,failed_=false,parseAttempted_=false,parsed_=false;
    bool parentAllocationAttempted_=false,nameAllocationAttempted_=false,rowsAllocationAttempted_=false;
    std::exception_ptr allocationException_;
    RetainedOriginalView retainedView_{};bool retainedViewPublished_=false;
    bool Begin(CallOriginal& r,CallOriginal::Operation op,bool cleanup=false) noexcept {
        r.operation=op;r.captureStarted=true;
        if(!r.clock.Before(&deadline_,"installed-retention-policy",cleanup)){failed_=true;return false;}
        if(!(cleanup?supervisor_.BeginIndependentlyEligibleCleanupNativeCall():supervisor_.BeginNativeCall())){failed_=true;return false;}
        r.attempted=true;return true;
    }
    void Returned(CallOriginal& r,ssize_t value,int error) noexcept {
        r.result=value;r.error=error;r.returnedObserved=true;
        supervisor_.OriginalNativeCallReturned();
        r.clock.After(&deadline_,"installed-retention-policy");if(!r.clock.clean)failed_=true;
    }
    bool Stat(CallOriginal& r,int fd,CallOriginal::Operation op) noexcept {
        r.descriptor=fd;if(!Begin(r,op))return false;
        const int value=fstat(fd,&r.metadata),error=value<0?errno:0;Returned(r,value,error);
        return value==0&&!failed_;
    }
    static bool Stable(const struct stat& a,const struct stat& b) noexcept {
        return a.st_dev==b.st_dev&&a.st_ino==b.st_ino&&a.st_mode==b.st_mode&&a.st_uid==b.st_uid&&a.st_gid==b.st_gid&&
            a.st_size==b.st_size&&a.st_mtim.tv_sec==b.st_mtim.tv_sec&&a.st_mtim.tv_nsec==b.st_mtim.tv_nsec&&
            a.st_ctim.tv_sec==b.st_ctim.tv_sec&&a.st_ctim.tv_nsec==b.st_ctim.tv_nsec;
    }
    bool ValuesMatch() const noexcept {
#define SLCLI_POLICY_EQUAL(field) if(values_.field!=snapshot_.field)return false;
        SLCLI_POLICY_EQUAL(maximum_bytes) SLCLI_POLICY_EQUAL(maximum_record_bytes)
        SLCLI_POLICY_EQUAL(partial_attempts) SLCLI_POLICY_EQUAL(failure_tail_bytes)
        SLCLI_POLICY_EQUAL(whole_lineage_bytes) SLCLI_POLICY_EQUAL(whole_lineage_attempts) SLCLI_POLICY_EQUAL(whole_lineage_nodes)
        SLCLI_POLICY_EQUAL(records_offset) SLCLI_POLICY_EQUAL(records_bytes) SLCLI_POLICY_EQUAL(index_offset) SLCLI_POLICY_EQUAL(index_bytes)
        SLCLI_POLICY_EQUAL(terminal_offset) SLCLI_POLICY_EQUAL(terminal_bytes) SLCLI_POLICY_EQUAL(native_cell_bytes)
        SLCLI_POLICY_EQUAL(source_window_bytes) SLCLI_POLICY_EQUAL(write_window_bytes) SLCLI_POLICY_EQUAL(readback_window_bytes)
        SLCLI_POLICY_EQUAL(pair_window_bytes) SLCLI_POLICY_EQUAL(active_index_bytes)
#undef SLCLI_POLICY_EQUAL
        return true;
    }
    static bool ReturnedOrSkipped(const CallOriginal& r) noexcept {return !r.attempted||r.returnedObserved;}
public:
    static bool InitialContainingReservationQualifiedOriginal() noexcept {
        // The actual independently acquired installer birth/reserve/source
        // original has no implemented constructor/verifier yet. This check
        // dominates startup copies/allocations/effects, not merely final ACK.
        return false;
    }
    static size_t CheckedStorageOriginal(const slcli_bq_destination_retention_constraints& source) {
        size_t total=sizeof(OriginalLinuxPrebirthRetentionPolicy);
        if(!source.parent_path_bytes||source.parent_path_bytes==SIZE_MAX||!source.policy_name_bytes||source.policy_name_bytes==SIZE_MAX||
           !source.policy_read_rows||source.parent_path_bytes+1>SIZE_MAX-total)throw std::invalid_argument("policy aggregate parent");
        total+=source.parent_path_bytes+1;
        if(source.policy_name_bytes+1>SIZE_MAX-total)throw std::invalid_argument("policy aggregate name");total+=source.policy_name_bytes+1;
        if(source.policy_read_rows>(SIZE_MAX-total)/sizeof(CallOriginal))throw std::invalid_argument("policy aggregate rows");
        return total+static_cast<size_t>(source.policy_read_rows)*sizeof(CallOriginal);
    }
    OriginalLinuxPrebirthRetentionPolicy(const slcli_bq_destination_retention_constraints& source,
        OriginalDeadline& deadline,OriginalDeadlineSupervisor& supervisor) noexcept
        :source_(&source),snapshot_(source),deadline_(deadline),supervisor_(supervisor) {}
    OriginalLinuxPrebirthRetentionPolicy(const OriginalLinuxPrebirthRetentionPolicy&)=delete;
    OriginalLinuxPrebirthRetentionPolicy& operator=(const OriginalLinuxPrebirthRetentionPolicy&)=delete;
    bool PreownStorageOriginal() noexcept {
        if(preownAttempted_||futureCallsClosed_)return false;preownAttempted_=true;
        try {
            if(supervisor_.OriginalDeadlineOwner()!=&deadline_||!snapshot_.parent_path||!snapshot_.parent_path_bytes||
               snapshot_.parent_path_bytes==SIZE_MAX||snapshot_.parent_path[0]!='/'||!snapshot_.policy_name||
               !snapshot_.policy_name_bytes||snapshot_.policy_name_bytes==SIZE_MAX||snapshot_.policy_body_bytes!=body_.size()||
               !snapshot_.policy_read_rows||snapshot_.policy_read_rows>SIZE_MAX/sizeof(CallOriginal))throw std::invalid_argument("installed policy geometry");
            for(size_t i=0;i<snapshot_.parent_path_bytes;++i)if(!snapshot_.parent_path[i])throw std::invalid_argument("policy parent NUL");
            for(size_t i=0;i<snapshot_.policy_name_bytes;++i)if(!snapshot_.policy_name[i]||snapshot_.policy_name[i]=='/')throw std::invalid_argument("policy name");
            if((snapshot_.policy_name_bytes==1&&snapshot_.policy_name[0]=='.')||
               (snapshot_.policy_name_bytes==2&&snapshot_.policy_name[0]=='.'&&snapshot_.policy_name[1]=='.'))throw std::invalid_argument("policy dot name");
            size_t total=sizeof(*this);
            if(snapshot_.parent_path_bytes+1>SIZE_MAX-total)throw std::invalid_argument("policy parent storage");total+=snapshot_.parent_path_bytes+1;
            if(snapshot_.policy_name_bytes+1>SIZE_MAX-total)throw std::invalid_argument("policy name storage");total+=snapshot_.policy_name_bytes+1;
            if(snapshot_.policy_read_rows>(SIZE_MAX-total)/sizeof(CallOriginal))throw std::invalid_argument("policy row storage");
            parentAllocationAttempted_=true;parentPath_=std::make_unique<char[]>(snapshot_.parent_path_bytes+1);
            std::memcpy(parentPath_.get(),snapshot_.parent_path,snapshot_.parent_path_bytes);parentPath_[snapshot_.parent_path_bytes]=0;
            nameAllocationAttempted_=true;name_=std::make_unique<char[]>(snapshot_.policy_name_bytes+1);
            std::memcpy(name_.get(),snapshot_.policy_name,snapshot_.policy_name_bytes);name_[snapshot_.policy_name_bytes]=0;
            rowsAllocationAttempted_=true;reads_=std::make_unique<CallOriginal[]>(static_cast<size_t>(snapshot_.policy_read_rows));
            capacity_=static_cast<size_t>(snapshot_.policy_read_rows);preowned_=true;return true;
        }catch(...){allocationException_=std::current_exception();failed_=true;return false;}
    }
    bool CaptureOriginal() noexcept {
        if(!preowned_||captureStarted_||futureCallsClosed_)return false;captureStarted_=true;
        struct ReturnOriginal {OriginalLinuxPrebirthRetentionPolicy* owner;~ReturnOriginal(){owner->captureReturned_=true;}} returned{this};
        parentOpen_.path=parentPath_.get();parentOpen_.flags=O_PATH|O_DIRECTORY|O_NOFOLLOW|O_CLOEXEC;
        if(!Begin(parentOpen_,CallOriginal::Operation::ParentOpen))return false;
        parent_=open(parentOpen_.path,parentOpen_.flags);const int parentError=parent_<0?errno:0;Returned(parentOpen_,parent_,parentError);
        if(parent_<0||failed_||!Stat(parentBefore_,parent_,CallOriginal::Operation::ParentStat)||!S_ISDIR(parentBefore_.metadata.st_mode))return false;
        policyOpen_.directory=parent_;policyOpen_.path=name_.get();policyOpen_.flags=O_RDONLY|O_NOFOLLOW|O_CLOEXEC;
        if(!Begin(policyOpen_,CallOriginal::Operation::PolicyOpen))return false;
        file_=openat(policyOpen_.directory,policyOpen_.path,policyOpen_.flags);const int fileError=file_<0?errno:0;Returned(policyOpen_,file_,fileError);
        if(file_<0||failed_||!Stat(before_,file_,CallOriginal::Operation::PolicyStat)||!S_ISREG(before_.metadata.st_mode)||before_.metadata.st_size!=static_cast<off_t>(body_.size()))return false;
        flags_.descriptor=file_;flags_.flags=F_GETFL;if(!Begin(flags_,CallOriginal::Operation::Flags))return false;
        const int flags=fcntl(flags_.descriptor,flags_.flags),flagsError=flags<0?errno:0;Returned(flags_,flags,flagsError);
        if(flags<0||failed_||(flags&O_ACCMODE)!=O_RDONLY||(flags&O_PATH))return false;
        while(used_<body_.size()) {
            if(count_==capacity_){failed_=true;return false;}auto& row=reads_[count_++];
            row.descriptor=file_;row.offset=static_cast<off_t>(used_);row.destination=body_.data()+used_;row.requested=body_.size()-used_;
            if(!Begin(row,CallOriginal::Operation::Read))return false;
            const ssize_t value=pread(row.descriptor,row.destination,row.requested,row.offset);const int error=value<0?errno:0;
            if(value<0||static_cast<size_t>(value)>row.requested)row.extentUnknown=true;
            Returned(row,value,error);
            if(value<=0||row.extentUnknown||failed_){failed_=true;return false;}
            if(!digest_.Update(row.destination,static_cast<size_t>(value))){failed_=true;return false;}used_+=static_cast<size_t>(value);
        }
        eof_.descriptor=file_;eof_.offset=static_cast<off_t>(body_.size());eof_.destination=&eofByte_;eof_.requested=1;
        if(!Begin(eof_,CallOriginal::Operation::Eof))return false;
        const ssize_t eof=pread(eof_.descriptor,eof_.destination,eof_.requested,eof_.offset);const int eofError=eof<0?errno:0;
        eof_.extentUnknown=eof<0||eof>1;Returned(eof_,eof,eofError);
        if(eof!=0||failed_||!Stat(after_,file_,CallOriginal::Operation::PolicyAfter)||
           !Stat(parentAfter_,parent_,CallOriginal::Operation::ParentAfter))return false;
        parseAttempted_=true;
        // Independently selected producer recipe only: these snapshot fields
        // are not inferred from parsed bytes and do not size destination rows.
        // Actual destination record/index/RAM/failure layout must independently
        // fit SAME reservation before journal effects; acquired gate is closed.
        parsed_=slcli_bq_retention_policy_decode(&values_,body_.data(),body_.size(),snapshot_.native_cell_bytes,snapshot_.active_index_bytes)!=0;
        const bool matches=parsed_&&ValuesMatch()&&Stable(before_.metadata,after_.metadata)&&Stable(parentBefore_.metadata,parentAfter_.metadata)&&
            digest_.Finish()&&!std::memcmp(digest_.PrivateDigest().data(),snapshot_.policy_body_digest,32);
        const bool final=IngressNativeClock::Capture(final_,&deadline_,"installed-policy-final",true);
        correspondence_=matches&&final&&final_.shapeValid&&!final_.expired&&final_.nanoseconds>=parentAfter_.clock.after.nanoseconds;
        failed_|=!correspondence_;return correspondence_;
    }
    bool ReturningCorrespondenceOriginal() const noexcept {return correspondence_&&captureReturned_&&!failed_&&!futureCallsClosed_;}
    bool SameOriginals(const slcli_bq_destination_retention_constraints& source,const OriginalDeadline& deadline,
        const OriginalDeadlineSupervisor& supervisor) const noexcept {
        return source_==&source && &deadline_==&deadline && &supervisor_==&supervisor;
    }
    bool SourceDispositionEstablishedOriginal() const noexcept {
        return futureCallsClosed_&&(!captureStarted_||captureReturned_)&&
            (file_<0||(policyClose_.attempted&&policyClose_.returnedObserved&&policyClose_.result==0))&&
            (parent_<0||(parentClose_.attempted&&parentClose_.returnedObserved&&parentClose_.result==0));
    }
    const RetainedOriginalView* PublishRetainedOriginalView() noexcept {
        if(!SourceDispositionEstablishedOriginal()||count_>capacity_||(count_&&!reads_))return nullptr;
        if(retainedViewPublished_)return &retainedView_;
        auto& view=retainedView_;view.owner=this;view.source=source_;view.snapshot=&snapshot_;
        view.parentPath=parentPath_.get();view.policyName=name_.get();
        view.parentPathBytes=parentPath_?snapshot_.parent_path_bytes:0;view.policyNameBytes=name_?snapshot_.policy_name_bytes:0;
        view.body=body_.data();view.bodyCapacity=body_.size();view.verifiedCursor=used_;
        view.readRows=reads_.get();view.readCount=count_;view.readCapacity=capacity_;
        view.parentOpen=&parentOpen_;view.policyOpen=&policyOpen_;view.parentBefore=&parentBefore_;view.before=&before_;view.flags=&flags_;
        view.eof=&eof_;view.parentAfter=&parentAfter_;view.after=&after_;view.policyClose=&policyClose_;view.parentClose=&parentClose_;
        view.parsedValues=&values_;view.finalClock=&final_;view.allocationException=&allocationException_;
        view.parentAllocationAttempted=parentAllocationAttempted_;view.nameAllocationAttempted=nameAllocationAttempted_;
        view.rowsAllocationAttempted=rowsAllocationAttempted_;view.preownAttempted=preownAttempted_;view.preowned=preowned_;
        view.captureStarted=captureStarted_;view.captureReturned=captureReturned_;view.parseAttempted=parseAttempted_;
        view.parsed=parsed_;view.correspondence=correspondence_;view.failed=failed_;
        retainedViewPublished_=true;return &view;
    }
    bool SameRetainedOriginalView(const RetainedOriginalView* view) const noexcept {
        return retainedViewPublished_&&view==&retainedView_&&view->owner==this&&SourceDispositionEstablishedOriginal()&&
            view->source==source_&&view->snapshot==&snapshot_&&view->body==body_.data()&&view->bodyCapacity==body_.size()&&
            view->readRows==reads_.get()&&view->readCount==count_&&view->readCapacity==capacity_&&view->allocationException==&allocationException_;
    }
    bool AcquiredInstalledPolicyOriginal() const noexcept {
        // Missing genuine initial containing reserve, installed source/native
        // namespace/entry and qualified ABI originals cannot be replaced by
        // parsed maxima, equality, this local owner or caller success flags.
        return false;
    }
    const template_creator_receipts::ReservationShape* OriginalReservationShape() const noexcept {
        return AcquiredInstalledPolicyOriginal()?&reservation_:nullptr;
    }
    bool ObserveIndependentDispositionOriginal() noexcept {
        futureCallsClosed_=true;
        if(captureStarted_&&!captureReturned_)return false;
        for(size_t i=0;i<count_;++i)if(!ReturnedOrSkipped(reads_[i])||reads_[i].extentUnknown)return false;
        const CallOriginal* rows[]={&parentOpen_,&policyOpen_,&parentBefore_,&before_,&flags_,&eof_,&parentAfter_,&after_};
        for(const auto* row:rows)if(!ReturnedOrSkipped(*row)||row->extentUnknown)return false;
        // Actual independently eligible close is once-only; failure/unknown
        // keeps the descriptor and all native output backing resident.
        if(file_>=0&&!policyClose_.attempted){policyClose_.descriptor=file_;if(!Begin(policyClose_,CallOriginal::Operation::PolicyClose,true))return false;
            const int value=close(file_),error=value<0?errno:0;Returned(policyClose_,value,error);}
        if(file_>=0&&(!policyClose_.returnedObserved||policyClose_.result!=0))return false;
        if(parent_>=0&&!parentClose_.attempted){parentClose_.descriptor=parent_;if(!Begin(parentClose_,CallOriginal::Operation::ParentClose,true))return false;
            const int value=close(parent_),error=value<0?errno:0;Returned(parentClose_,value,error);}
        return parent_<0||(parentClose_.returnedObserved&&parentClose_.result==0);
    }
    bool SourceBackingBytesOriginal(size_t& bytes) const noexcept {
        bytes=sizeof(*this);
        if(parentPath_){if(snapshot_.parent_path_bytes+1>SIZE_MAX-bytes)return false;bytes+=snapshot_.parent_path_bytes+1;}
        if(name_){if(snapshot_.policy_name_bytes+1>SIZE_MAX-bytes)return false;bytes+=snapshot_.policy_name_bytes+1;}
        if(reads_){if(capacity_>(SIZE_MAX-bytes)/sizeof(CallOriginal))return false;bytes+=capacity_*sizeof(CallOriginal);}return true;
    }
};
}
#endif

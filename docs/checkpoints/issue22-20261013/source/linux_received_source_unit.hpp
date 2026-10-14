// UNCOMPILED bounded complete table and selected range observations, not admission.
#pragma once
#ifndef _WIN32
#include "linux_received_source_unit_range.hpp"
#include "linux_received_subject_readback.hpp"
#include "upstream-cli/source_unit12.h"
#include "upstream-cli/bq_provider_expectations.h"
namespace template_creator_bridge {struct NativePlatformState;}
namespace template_creator_linux {
class OriginalReceivedSourceUnitObservation {
    friend class template_creator_bridge::OriginalLinuxSelectionStartup;
    friend class OriginalInitialProviderArchive;
    friend bool VerifySurvivingPendingHostPreownRegressionOriginal();
    friend class OriginalInitialProviderReceiver;
    friend class OriginalUnitPayloadArchive;
    friend struct template_creator_bridge::NativePlatformState;
    friend struct InitialProviderUnitRowsSourceRegression;
    struct RangeDelete {void operator()(OriginalReceivedSourceUnitRange* p) const noexcept {delete p;}};
    using RangeOwner=std::unique_ptr<OriginalReceivedSourceUnitRange,RangeDelete>;
    const slcli_bq_provider_expectations& source_;
    const slcli_bq_expected_subject& subject_;
    const OriginalInitialProviderReceiver& receiver_;
    OriginalDeadline& deadline_;OriginalDeadlineSupervisor& supervisor_;
    RangeOwner header_,table_,member_;
    std::unique_ptr<struct slcli_source_unit12_record[]> records_;
    struct slcli_source_unit12_reader parser_{};
    OriginalClockBoundary finalClock_{};
    const OriginalReceivedSubjectReadback* associationOriginal_=nullptr;
    OriginalClockBoundary associationClock_{};
    size_t associationRowsCompared_=0;
    bool associationAttempted_=false,associationRowsObserved_=false;
    static bool CompareAssociationRowsOriginal(const struct slcli_source_association_record* a,size_t aCount,
        const struct slcli_source_unit12_record* unit,size_t unitCount,size_t& compared) noexcept {
        compared=0;if(!a||!unit||!aCount||aCount!=unitCount)return false;
        for(size_t i=0;i<aCount;++i){
            if(a[i].index!=i||unit[i].index!=i||a[i].kind!=unit[i].kind||a[i].project!=unit[i].source_project||
               a[i].body_bytes!=unit[i].bytes||std::memcmp(a[i].association,unit[i].association,32)||
               std::memcmp(a[i].capture,unit[i].capture,32)||std::memcmp(a[i].digest,unit[i].digest,32))return false;
            ++compared;
        }
        return true;
    }
    size_t recordCount_=0,parsed_=0,sourceBytes_=0,offerOrdinal_=SIZE_MAX;
    bool started_=false,tableObserved_=false,memberObserved_=false,observed_=false,borrowReleased_=false;
    ~OriginalReceivedSourceUnitObservation()=default;
public:
    static size_t CheckedSourceBacking(const slcli_bq_provider_expectations& source,
        const slcli_bq_expected_subject& subject,size_t rows){
        const auto& occurrence=subject.source_occurrence;
        if(!source.source_unit_record_count||source.source_unit_record_count>(SIZE_MAX-64)/128||
           source.source_unit_bytes>SIZE_MAX||64+128*source.source_unit_record_count>source.source_unit_bytes||
           occurrence.native_file_bytes!=source.source_unit_bytes||
           (occurrence.reader_class!=SLCLI_BQ_EXPECTED_UNIT_PREREQUISITE&&
            occurrence.reader_class!=SLCLI_BQ_EXPECTED_SP1_MEMBER_RANGE))
            throw std::invalid_argument("source-unit exact record geometry");
        size_t bytes=sizeof(OriginalReceivedSourceUnitObservation);
        const size_t count=static_cast<size_t>(source.source_unit_record_count);
        if(count>(SIZE_MAX-bytes)/sizeof(struct slcli_source_unit12_record))
            throw std::invalid_argument("source-unit record backing");
        bytes+=count*sizeof(struct slcli_source_unit12_record);
        const auto add=[&](size_t offset,size_t extent){
            const size_t charge=OriginalReceivedSourceUnitRange::CheckedSourceBacking(offset,extent,
                static_cast<size_t>(source.source_unit_bytes),rows);
            if(charge>SIZE_MAX-bytes)throw std::invalid_argument("source-unit aggregate backing");bytes+=charge;
        };
        add(0,64);add(64,count*128);
        if(occurrence.reader_class==SLCLI_BQ_EXPECTED_SP1_MEMBER_RANGE){
            if(subject.source_record_index>=count||occurrence.read_offset>SIZE_MAX||occurrence.read_bytes>SIZE_MAX)
                throw std::invalid_argument("source-unit selected member geometry");
            add(static_cast<size_t>(occurrence.read_offset),static_cast<size_t>(occurrence.read_bytes));
        }
        return bytes; // Requested backing only; containing financing stays independent.
    }
private:
    OriginalReceivedSourceUnitObservation(const slcli_bq_provider_expectations& source,
        const slcli_bq_expected_subject& subject,const OriginalInitialProviderReceiver& receiver,
        size_t rows,OriginalDeadline& deadline,OriginalDeadlineSupervisor& supervisor)
        :source_(source),subject_(subject),receiver_(receiver),deadline_(deadline),supervisor_(supervisor),
         recordCount_(static_cast<size_t>(source.source_unit_record_count)),
         sourceBytes_(0),selectedRows_(rows) {}
    size_t selectedRows_=0,allocatedRecords_=0;
    bool preownAttempted_=false,preownReturned_=false,preowned_=false;
    std::exception_ptr preownException_;
    bool PreownStorageOriginal() noexcept {
        if(preownAttempted_)return false;preownAttempted_=true;
        struct Returned {bool& value;~Returned(){value=true;}} returned{preownReturned_};
        try {
            sourceBytes_=CheckedSourceBacking(source_,subject_,selectedRows_);
            const auto unit=static_cast<size_t>(source_.source_unit_bytes);
            header_.reset(new OriginalReceivedSourceUnitRange(receiver_,0,64,unit,selectedRows_,deadline_,supervisor_));
            if(!header_->PreownStorageOriginal()){preownException_=header_->preownException_;return false;}
            table_.reset(new OriginalReceivedSourceUnitRange(receiver_,64,recordCount_*128,unit,selectedRows_,deadline_,supervisor_));
            if(!table_->PreownStorageOriginal()){preownException_=table_->preownException_;return false;}
            if(subject_.source_occurrence.reader_class==SLCLI_BQ_EXPECTED_SP1_MEMBER_RANGE){
                member_.reset(new OriginalReceivedSourceUnitRange(receiver_,static_cast<size_t>(subject_.source_occurrence.read_offset),
                    static_cast<size_t>(subject_.source_occurrence.read_bytes),unit,selectedRows_,deadline_,supervisor_));
                if(!member_->PreownStorageOriginal()){preownException_=member_->preownException_;return false;}}
            records_=std::make_unique<struct slcli_source_unit12_record[]>(recordCount_);allocatedRecords_=recordCount_;
            preowned_=true;return true;
        }catch(...){if(!preownException_)preownException_=std::current_exception();return false;}
    }    bool CaptureOriginal(size_t ordinal) noexcept {
        if(!preowned_||started_||borrowReleased_)return false;started_=true;offerOrdinal_=ordinal;
        if(!header_->CaptureOriginal(ordinal)||header_->RetainedBytes()!=64||
           !slcli_source_unit12_header(&parser_,header_->OriginalBytes(),source_.source_unit_bytes)||
           parser_.count!=recordCount_||parser_.body_offset!=64+recordCount_*128||
           parser_.file_bytes!=source_.source_unit_bytes)return false;
        if(!table_->CaptureOriginal(ordinal)||table_->RetainedBytes()!=recordCount_*128||
           !OriginalReceivedSourceUnitRange::Same(header_->after_.metadata,table_->before_.metadata))return false;
        for(size_t i=0;i<recordCount_;++i){
            if(!slcli_source_unit12_record(&parser_,table_->OriginalBytes()+i*128))return false;
            records_[i]=parser_.previous;++parsed_;
        }
        tableObserved_=parser_.table_complete&&!parser_.failed&&parsed_==recordCount_;
        if(!tableObserved_)return false;
        if(member_){
            const auto& record=records_[static_cast<size_t>(subject_.source_record_index)];
            if(record.index!=subject_.source_record_index||record.offset!=subject_.source_occurrence.read_offset||
               record.bytes!=subject_.source_occurrence.read_bytes||record.kind!=subject_.source_record_kind||
               record.source_project!=subject_.source_project||
               std::memcmp(record.association,subject_.source_association,32)||
               std::memcmp(record.capture,subject_.source_capture,32)||std::memcmp(record.digest,subject_.body_digest,32))return false;
            if(!member_->CaptureOriginal(ordinal)||
               !OriginalReceivedSourceUnitRange::Same(table_->after_.metadata,member_->before_.metadata))return false;
            std::array<unsigned char,32> digest{};std::memcpy(digest.data(),record.digest,32);
            if(!member_->ObserveRangeDigestOriginal(digest))return false;
            memberObserved_=true;
        }
        const bool final=IngressNativeClock::Capture(finalClock_,&deadline_,"source-unit-complete-table-cut",true);
        const auto& prior=member_?member_->digestClock_:table_->finalClock_;
        observed_=final&&finalClock_.shapeValid&&!finalClock_.expired&&finalClock_.nanoseconds>=prior.nanoseconds;
        return observed_;
        // Authentic A/producer, all source edges/extras and publication relation remain separate.
    }
    bool BorrowerReleasedOriginal() const noexcept{return borrowReleased_;}
    bool ReleaseBeforeExposureOriginal() noexcept {
        if(supervisor_.HasPendingOriginalNativeCall())return false;
        if((header_&&!header_->ReleaseBeforeExposureOriginal())||(table_&&!table_->ReleaseBeforeExposureOriginal())||
           (member_&&!member_->ReleaseBeforeExposureOriginal()))return false;
        borrowReleased_=true;return true;
    }
    bool ObserveAssociationRowsOriginal(const OriginalReceivedSubjectReadback& original) noexcept {
        if(associationAttempted_||borrowReleased_)return false;associationAttempted_=true;associationOriginal_=&original;
        if(!observed_||!tableObserved_||parsed_!=recordCount_||!original.ReadbackObserved()||
           !original.AssociationObserved()||original.AssociationSourceOriginal()!=&source_||
           original.AssociationRecordCount()!=recordCount_||original.AssociationRecordCapacity()!=recordCount_||
           !original.AssociationRecordsOriginal())return false;
        if(!CompareAssociationRowsOriginal(original.AssociationRecordsOriginal(),original.AssociationRecordCount(),
            records_.get(),recordCount_,associationRowsCompared_))return false;
        const bool clock=IngressNativeClock::Capture(associationClock_,&deadline_,"source-unit-association-rows",true);
        associationRowsObserved_=clock&&associationClock_.shapeValid&&!associationClock_.expired&&
            associationClock_.nanoseconds>=finalClock_.nanoseconds&&
            associationClock_.nanoseconds>=original.FinalVerificationClockOriginal().nanoseconds;
        return associationRowsObserved_; // Complete row correspondence only, not authentic A or source edges.
    }
public:
    struct RetainedPreownBackingView {
        const OriginalReceivedSourceUnitObservation* ownerOriginal;
        const struct slcli_source_unit12_record* recordsOriginal;const std::exception_ptr* exceptionOriginal;
        OriginalReceivedSourceUnitRange::RetainedPreownBackingView ranges[3];
        size_t recordCapacity,parsedRecords,allocatedBackingBytes;
        bool attempted,returned,preowned,released;
    };
    bool ObserveRetainedPreownBackingOriginal(RetainedPreownBackingView& out) const noexcept {
        const auto output=reinterpret_cast<uintptr_t>(&out),owner=reinterpret_cast<uintptr_t>(this);size_t bytes=0;
        if(!borrowReleased_||(preownAttempted_&&!preownReturned_)||parsed_>allocatedRecords_||
           !AllocatedBackingBytesOriginal(bytes)||output>UINTPTR_MAX-sizeof(out)||owner>UINTPTR_MAX-sizeof(*this)||
           (output<owner+sizeof(*this)&&owner<output+sizeof(out)))return false;
        const OriginalReceivedSourceUnitRange* ranges[3]={header_.get(),table_.get(),member_.get()};
        if(!OriginalBackingStorageSeparate(&out,sizeof(out),records_.get(),allocatedRecords_,sizeof(struct slcli_source_unit12_record)))return false;
        for(const auto* range:ranges)if(range&&!range->BackingStorageSeparateOriginal(&out,sizeof(out)))return false;
        std::memset(&out,0,sizeof(out));
        for(size_t i=0;i<3;++i)if(ranges[i]&&!ranges[i]->ObserveRetainedPreownBackingOriginal(out.ranges[i]))return false;
        out.ownerOriginal=this;out.recordsOriginal=records_.get();out.exceptionOriginal=&preownException_;
        out.recordCapacity=allocatedRecords_;out.parsedRecords=parsed_;out.allocatedBackingBytes=bytes;
        out.attempted=preownAttempted_;out.returned=preownReturned_;out.preowned=preowned_;out.released=borrowReleased_;return true;
    }
    size_t SourceBackingBytes() const noexcept{return sourceBytes_;}
    const std::exception_ptr& PreownExceptionOriginal() const noexcept{return preownException_;}
    bool AllocatedBackingBytesOriginal(size_t& bytes) const noexcept {
        bytes=sizeof(*this);const auto add=[&](size_t amount){if(amount>SIZE_MAX-bytes)return false;bytes+=amount;return true;};
        size_t n=0;for(const auto* range:{header_.get(),table_.get(),member_.get()})
            if(range&&(!range->AllocatedBackingBytesOriginal(n)||!add(n)))return false;
        if(allocatedRecords_>SIZE_MAX/sizeof(struct slcli_source_unit12_record))return false;
        return add(allocatedRecords_*sizeof(struct slcli_source_unit12_record));
    }
};
inline bool OriginalInitialProviderReceiver::AcknowledgeSourceUnitOriginal(const OriginalReceivedSourceUnitObservation& original) noexcept {
    if(&original.receiver_!=this||&original.deadline_!=deadline_||&original.supervisor_!=supervisor_||
       !original.observed_||original.borrowReleased_||!original.tableObserved_||
       !original.associationRowsObserved_||original.associationRowsCompared_!=original.recordCount_||
       !original.associationOriginal_||!original.associationClock_.shapeValid||original.associationClock_.expired||
       original.parsed_!=original.recordCount_||!original.finalClock_.shapeValid||original.finalClock_.expired||
       (original.member_&&!original.memberObserved_))return false;
    return SendReadbackAckOriginal(original.offerOrdinal_); // Independent archive qualification remains mandatory.
}
}
#endif

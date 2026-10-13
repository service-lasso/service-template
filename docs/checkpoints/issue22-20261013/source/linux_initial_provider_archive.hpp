// UNCOMPILED fixed retained archive cuts. Structural observation is not admission.
#pragma once
#ifndef _WIN32
#include "linux_initial_provider_process_association.hpp"
#include "linux_received_subject_readback.hpp"
#include "linux_received_source_unit.hpp"
#include "linux_unit_payload_archive.hpp"
namespace template_creator_bridge {struct NativePlatformState;}
namespace template_creator_linux {
class OriginalInitialProviderArchive {
    friend class template_creator_bridge::OriginalLinuxSelectionStartup;
    friend struct template_creator_bridge::NativePlatformState;
    friend struct InitialProviderReceiptSourceRegression;
public:
    struct CutOriginal;
    struct RetainedFinalView {
        const OriginalInitialProviderArchive* owner=nullptr;
        const template_creator_bridge::NativePlatformState* platform=nullptr;
        const OriginalInitialProviderReceiver* receiver=nullptr;
        const CutOriginal* cuts=nullptr;
        const IngressFd* endpoint=nullptr;
        size_t count=0,sourceBytes=0;
        bool published=false;
    };
    struct CutOriginal {
        const OriginalInitialProviderArchive* owner=nullptr;
        const OriginalInitialProviderReceiver::OfferOriginal* offer=nullptr;
        OriginalInitialProviderProcessAssociation* process=nullptr;
        OriginalReceivedSubjectReadback* file=nullptr;
        OriginalReceivedSourceUnitObservation* unit=nullptr;
        OriginalUnitPayloadArchive* unitPayload=nullptr;
        const CutOriginal* earlierUnit=nullptr;
        const unsigned char* payload=nullptr;
        const OriginalReceivedSubjectReadback::NativeOriginal* reads=nullptr;
        const OriginalReceivedSubjectReadback::NativeOriginal* before=nullptr;
        const OriginalReceivedSubjectReadback::NativeOriginal* eof=nullptr;
        const OriginalReceivedSubjectReadback::NativeOriginal* after=nullptr;
        const OriginalClockBoundary* verificationClock=nullptr;
        size_t ordinal=SIZE_MAX,payloadBytes=0,readCount=0;
        OriginalClockBoundary finalClock{};
        bool attempted=false,prefixRetained=false;
    };
private:
    OriginalInitialProviderReceiver& receiver_;
    OriginalDeadline& deadline_;OriginalDeadlineSupervisor& supervisor_;
    std::unique_ptr<CutOriginal[]> cuts_;
    size_t capacity_=0,count_=0,sourceBytes_=0;
    bool failed_=false;
    const template_creator_bridge::NativePlatformState* usefulInputCutOriginal_=nullptr;
    bool usefulDisposalAttempted_=false;
    RetainedFinalView finalView_{};
    ~OriginalInitialProviderArchive()=default;
    OriginalInitialProviderArchive(OriginalInitialProviderReceiver& receiver,size_t count,
        OriginalDeadline& deadline,OriginalDeadlineSupervisor& supervisor)
        :receiver_(receiver),deadline_(deadline),supervisor_(supervisor),capacity_(count),
         sourceBytes_(CheckedSourceBacking(count)) {
        if(receiver.archiveOriginal_||receiver.count_||receiver.deadline_!=&deadline||
           receiver.supervisor_!=&supervisor||receiver.capacity_!=count)
            throw std::invalid_argument("archive containing original identity");
        cuts_=std::make_unique<CutOriginal[]>(count);
        receiver.archiveOriginal_=this; // Bind only after complete allocation; no effect yet.
    }
    static bool ClockPairRetainedOriginal(const IngressNativeClock& clock) noexcept {
        return clock.clean&&clock.before.attempted&&clock.after.attempted&&
            clock.before.returned==0&&clock.after.returned==0&&clock.before.shapeValid&&clock.after.shapeValid&&
            !clock.before.expired&&!clock.after.expired&&clock.after.nanoseconds>=clock.before.nanoseconds;
    }
    static bool ReceivePrefixRetainedOriginal(const OriginalInitialProviderReceiver::OfferOriginal& offer) noexcept {
        if(!offer.reserved||!offer.attempted||!offer.returned||!offer.rawOfferObserved||
           offer.result!=SLCLI_BQ_SUBJECT_RECORD_BYTES||offer.controlExtentUnknown||offer.shapeFailed||
           !offer.routeDecoded||offer.rightsCount!=1||offer.rightsMessages!=1||offer.credentialMessages!=1||
           (offer.message.msg_flags&(MSG_TRUNC|MSG_CTRUNC))||
           !offer.readabilityObserved||!OriginalInitialProviderReceiver::ReadabilityOriginal(offer)||
           !ClockPairRetainedOriginal(offer.readinessClock)||!ClockPairRetainedOriginal(offer.receiveClock)||
           !offer.rightsFlagsAttempted||!offer.rightsFlagsReturned||!ClockPairRetainedOriginal(offer.rightsFlagsClock)||
           offer.rightsFlags<0||!(offer.rightsFlags&FD_CLOEXEC))return false;
        const auto& fd=offer.rights[0];
        return fd.original>=0&&!fd.closeAttempted&&fd.statAttempted&&fd.statReturned&&
            fd.statResult==0&&ClockPairRetainedOriginal(fd.statClock);
    }
    CutOriginal* Reserve(size_t ordinal) noexcept {
        if(failed_||ordinal!=count_||count_==capacity_||supervisor_.HasPendingOriginalNativeCall())return nullptr;
        auto& cut=cuts_[count_++];cut.owner=this;cut.ordinal=ordinal;cut.attempted=true;
        cut.offer=receiver_.OriginalOffer(ordinal);
        if(!cut.offer||!ReceivePrefixRetainedOriginal(*cut.offer)){failed_=true;return nullptr;}
        return &cut;
    }
    static bool CutChronologyOriginal(const OriginalClockBoundary& finalClock,uint64_t precedingNanoseconds) noexcept {
        return finalClock.attempted&&finalClock.returned==0&&finalClock.shapeValid&&!finalClock.expired&&
            finalClock.nanoseconds>=precedingNanoseconds;
    }
    bool Finish(CutOriginal& cut,bool observed,uint64_t precedingNanoseconds) noexcept {
        const bool clock=IngressNativeClock::Capture(cut.finalClock,&deadline_,"initial-provider-archive-cut",true);
        cut.prefixRetained=observed&&clock&&CutChronologyOriginal(cut.finalClock,precedingNanoseconds);
        failed_|=!cut.prefixRetained;return cut.prefixRetained;
    }
    bool CaptureOriginal(OriginalInitialProviderProcessAssociation& process) noexcept {
        auto* cut=Reserve(0);if(!cut)return false;cut->process=&process;
        const bool observed=&process.receiver_==&receiver_&&process.offer_==cut->offer&&
            process.deadline_==&deadline_&&process.supervisor_==&supervisor_&&process.associated_&&
            !process.disposalRequested_&&process.signalReturned_&&process.signalResult_==0&&
            process.before_.returnedObserved&&process.after_.returnedObserved&&
            process.before_.returned==0&&process.after_.returned==0&&
            !process.before_.output.revents&&!process.after_.output.revents;
        // Process payload is the concrete native graph, not a fabricated file.
        return Finish(*cut,observed&&process.finalCorrespondenceClock_.shapeValid&&
            !process.finalCorrespondenceClock_.expired,process.finalCorrespondenceClock_.nanoseconds);
    }
    bool CaptureOriginal(OriginalReceivedSubjectReadback& file) noexcept {
        auto* cut=Reserve(file.offerOrdinal_);if(!cut)return false;cut->file=&file;
        cut->payload=file.body_.get();cut->payloadBytes=file.retained_;
        cut->reads=file.reads_.get();cut->readCount=file.count_;
        cut->before=&file.before_;cut->eof=&file.eof_;cut->after=&file.after_;
        cut->verificationClock=&file.finalVerificationClock_;
        bool observed=&file.receiver_==&receiver_&&file.offer_==cut->offer&&
            &file.deadline_==&deadline_&& &file.supervisor_==&supervisor_&&file.complete_&&
            !file.borrowReleased_&&file.retained_==file.bytes_&&file.finalVerificationObserved_&&
            file.finalVerificationClock_.shapeValid&&!file.finalVerificationClock_.expired&&
            file.borrowedFd_==cut->offer->rights[0].original&&
            OriginalReceivedSubjectReadback::Stable(cut->offer->rights[0].status,file.before_.metadata)&&
            !file.eof_.extentUnknown&&file.eof_.returned&&file.eof_.result==0;
        for(size_t i=0;i<file.count_;++i)observed=observed&&file.reads_[i].returned&&!file.reads_[i].extentUnknown;
        return Finish(*cut,observed,file.finalVerificationClock_.nanoseconds);
    }
    bool DisposeAfterUsefulInputCutOriginal(const template_creator_bridge::NativePlatformState&) noexcept;
    bool CaptureOriginal(OriginalReceivedSourceUnitObservation& unit,OriginalUnitPayloadArchive* payload=nullptr,
                         const CutOriginal* earlierUnit=nullptr) noexcept {
        auto* cut=Reserve(unit.offerOrdinal_);if(!cut)return false;cut->unit=&unit;
        cut->unitPayload=payload;
        if(!unit.observed_||!unit.tableObserved_||unit.borrowReleased_||!unit.header_||!unit.table_||
           unit.header_->retained_>unit.header_->extent_||unit.table_->retained_>unit.table_->extent_){
            failed_=true;return false;
        }
        const auto readerClass=unit.subject_.source_occurrence.reader_class;
        cut->earlierUnit=earlierUnit;
        if((readerClass==SLCLI_BQ_EXPECTED_UNIT_PREREQUISITE&&!payload)||
           (readerClass==SLCLI_BQ_EXPECTED_SP1_MEMBER_RANGE&&payload)||
           (readerClass!=SLCLI_BQ_EXPECTED_UNIT_PREREQUISITE&&readerClass!=SLCLI_BQ_EXPECTED_SP1_MEMBER_RANGE)){
            failed_=true;return false;
        }
        if(readerClass==SLCLI_BQ_EXPECTED_SP1_MEMBER_RANGE){
            bool ownedEarlier=false;
            for(size_t i=0;i<cut->ordinal;++i)if(earlierUnit==&cuts_[i])ownedEarlier=true;
            if(!ownedEarlier||!earlierUnit->prefixRetained||!earlierUnit->unit||!earlierUnit->unitPayload||
               earlierUnit->unit->subject_.source_occurrence.reader_class!=SLCLI_BQ_EXPECTED_UNIT_PREREQUISITE||
               &earlierUnit->unit->source_!=&unit.source_||
               earlierUnit->unit->associationOriginal_!=unit.associationOriginal_||
               !earlierUnit->unit->observed_||!earlierUnit->unit->tableObserved_||earlierUnit->unit->borrowReleased_||
               !earlierUnit->unitPayload->observed_||earlierUnit->unitPayload->failed_||
               earlierUnit->unitPayload->borrowReleased_){failed_=true;return false;}
            const auto& selector=unit.subject_.source_occurrence.source_unit;
            const auto& original=earlierUnit->unit->subject_;
            if(selector.phase!=original.phase||selector.command!=original.command||selector.requirement!=original.requirement){
                failed_=true;return false;
            }
            const auto& prior=*earlierUnit->unit;
            if(!OriginalReceivedSubjectReadback::Stable(earlierUnit->offer->rights[0].status,cut->offer->rights[0].status)||
               prior.recordCount_!=unit.recordCount_||!prior.header_||!prior.table_||!unit.header_||!unit.table_||
               prior.header_->retained_!=unit.header_->retained_||prior.table_->retained_!=unit.table_->retained_||
               std::memcmp(prior.header_->bytes_.get(),unit.header_->bytes_.get(),unit.header_->retained_)||
               std::memcmp(prior.table_->bytes_.get(),unit.table_->bytes_.get(),unit.table_->retained_)||
               unit.associationClock_.nanoseconds<earlierUnit->unitPayload->finalClock_.nanoseconds){
                failed_=true;return false;
            }
            // Native identity/prefix correspondence only: the independent
            // own-writer/Layer0 lifetime and authenticated graph remain closed.
        }else if(earlierUnit){failed_=true;return false;}
        if(payload&&(&payload->unit_!=&unit||&payload->deadline_!=&deadline_||
            &payload->supervisor_!=&supervisor_||!payload->storagePreowned_||
            !payload->observed_||payload->failed_||payload->borrowReleased_)){
            failed_=true;return false;
        }
        if(!unit.associationRowsObserved_||unit.associationRowsCompared_!=unit.recordCount_||!unit.associationOriginal_)return false;
        cut->verificationClock=&unit.associationClock_;
        const bool observed=&unit.receiver_==&receiver_&& &unit.deadline_==&deadline_&&
            &unit.supervisor_==&supervisor_&&unit.observed_&&!unit.borrowReleased_&&unit.tableObserved_&&
            unit.parsed_==unit.recordCount_&&unit.parser_.table_complete&&!unit.parser_.failed&&
            unit.associationClock_.shapeValid&&!unit.associationClock_.expired&&
            unit.header_->offer_==cut->offer&&unit.table_->offer_==cut->offer&&
            (!unit.member_||(unit.memberObserved_&&unit.member_->offer_==cut->offer));
        return Finish(*cut,observed,payload?payload->finalClock_.nanoseconds:unit.associationClock_.nanoseconds);
    }
public:
    const RetainedFinalView& PublishRetainedFinalViewOriginal(
        const template_creator_bridge::NativePlatformState* platform) noexcept {
        if(finalView_.published)return finalView_;
        if(!NativeBorrowersSettledForOriginal(platform))return finalView_;
        for(size_t i=0;i<count_;++i){
            const auto& offer=*cuts_[i].offer;const auto& ack=offer.ack;
            // Complete original route/payload/poll/timeout/msghdr/native clocks
            // remain in these exact owners; selected file views are not the
            // complete graph. No later reader or cell writer is invented.
            if((ack.waitAttempted&&!ack.waitReturned)||(ack.sendAttempted&&!ack.sendReturned)||
               !offer.rights[0].closeReturned||!receiver_.endpoint_.closeReturned)return finalView_;
        }
        finalView_.owner=this;finalView_.platform=platform;finalView_.receiver=&receiver_;
        finalView_.cuts=cuts_.get();finalView_.endpoint=&receiver_.endpoint_;
        finalView_.count=count_;finalView_.sourceBytes=sourceBytes_;finalView_.published=true;
        return finalView_; // Resident immutable view only; no qualification grant.
    }
    bool NativeBorrowersSettledForOriginal(const template_creator_bridge::NativePlatformState* platform) const noexcept {
        return platform&&usefulInputCutOriginal_==platform&&NativeBorrowersSettledOriginal();
    }
    bool NativeBorrowersSettledOriginal() const noexcept {
        if(!usefulInputCutOriginal_||!usefulDisposalAttempted_||supervisor_.HasPendingOriginalNativeCall()||
           !receiver_.endpoint_.closed||receiver_.count_!=capacity_||count_!=capacity_)return false;
        for(size_t i=0;i<count_;++i){
            const auto& cut=cuts_[i];
            if(cut.owner!=this||cut.offer!=receiver_.OriginalOffer(i)||!cut.prefixRetained)return false;
            for(size_t r=0;r<cut.offer->rightsCount;++r)if(!cut.offer->rights[r].closed)return false;
            if(i==0){if(!cut.process||!cut.process->disposed_)return false;}
            else if(cut.unit){if(cut.file||!cut.unit->borrowReleased_||
                (cut.unitPayload&&!cut.unitPayload->borrowReleased_))return false;}
            else if(!cut.file||!cut.file->borrowReleased_)return false;
        }
        return true; // Native descriptors settled; archive body stays resident.
    }
    static size_t CheckedSourceBacking(size_t count) {
        if(!count||count>(SIZE_MAX-sizeof(OriginalInitialProviderArchive))/sizeof(CutOriginal))
            throw std::invalid_argument("archive complete cut backing");
        return sizeof(OriginalInitialProviderArchive)+count*sizeof(CutOriginal);
    }
    bool QualifiedAtCutOriginal(size_t ordinal) const noexcept {
        if(ordinal>=count_||!cuts_[ordinal].prefixRetained||failed_)return false;
        // Genuine independently financed containing owner and acquired initial
        // principal/image/entry/source/member verification are not installed.
        // Retained storage and a digest cannot supply those original relations.
        return false;
    }
    const CutOriginal* OriginalCut(size_t ordinal) const noexcept{return ordinal<count_?&cuts_[ordinal]:nullptr;}
    const IngressFd& OriginalEndpoint() const noexcept{return receiver_.endpoint_;}
    size_t OriginalCutCount() const noexcept{return count_;}
    size_t SourceBackingBytes() const noexcept{return sourceBytes_;}
};
}
#endif

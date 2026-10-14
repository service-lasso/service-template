// UNCOMPILED all-member two-pass native observations. No authentic archive grant.
#pragma once
#ifndef _WIN32
#include "linux_received_source_unit.hpp"
#include <exception>
#include "original_git_object_sha1.hpp"
namespace template_creator_linux {
class OriginalUnitPayloadArchive {
    friend class OriginalInitialProviderArchive;
    friend bool VerifySurvivingPendingHostPreownRegressionOriginal();
    friend class template_creator_bridge::OriginalLinuxSelectionStartup;
    friend struct template_creator_bridge::NativePlatformState;
    enum class Operation {Access,UnitBefore,MemberBefore,Read,MemberAfter,UnitAfter,WholeEof,FinalStat};
    struct NativeOriginal : OriginalReceivedSubjectReadback::NativeOriginal {
        Operation operation=Operation::Access;size_t pass=SIZE_MAX,member=SIZE_MAX;
        struct slcli_source_unit12_window windowGeometry{};
        bool windowGeometryObserved=false;
    };
    struct MemberPass {
        template_creator_source::OriginalSha256 digest;
        template_creator::OriginalGitObjectSha1 gitDigest;
        struct slcli_source_unit12_git_edge_state edgeState{};
        OriginalClockBoundary finalClock{};
        bool started=false,digestMatched=false,observed=false;
        bool gitRequired=false,gitMatched=false,edgesBegan=false,edgesFinished=false;
    };
    struct EdgeFeedOriginal {
        const NativeOriginal* readOriginal=nullptr;
        size_t pass=SIZE_MAX,member=SIZE_MAX,windowOffset=0,offered=0,consumed=0;
        uint64_t relativeOffset=0;
        struct slcli_source_unit12_git_edge edge{};
        int status=0,resumeStatus=0;
        bool attempted=false,returned=false,resumeAttempted=false,resumeReturned=false;
    };
    struct AllocationOriginal {
        size_t bytes=0;const void* output=nullptr;
        bool attempted=false,returned=false,threw=false;
        std::exception_ptr exceptionOriginal;
    };
    struct IngressAliasOriginal {
        const IngressFd* descriptorOriginal=nullptr;
        size_t ordinal=SIZE_MAX;
        IngressNativeClock clock{};
        int flags=-1,error=0;
        bool attempted=false,returned=false,sameInode=false,writableAlias=false,observed=false;
    };
    const OriginalPrivateIngress* ingressOriginal_=nullptr;
    std::array<IngressAliasOriginal,OriginalPrivateIngress::FdCapacity> ingressAliases_{};
    size_t ingressAliasCount_=0;
    bool ingressAliasesAttempted_=false,ingressAliasesObserved_=false;
    std::array<AllocationOriginal,4> allocations_{};
    const OriginalReceivedSourceUnitObservation& unit_;
    OriginalDeadline& deadline_;OriginalDeadlineSupervisor& supervisor_;
    std::unique_ptr<NativeOriginal[]> rows_;
    std::unique_ptr<MemberPass[]> members_;
    std::unique_ptr<unsigned char[]> window_;
    std::unique_ptr<EdgeFeedOriginal[]> edgeFeeds_;
    size_t edgeFeedCapacity_=0,edgeFeedCount_=0;
    std::array<template_creator_source::OriginalSha256,2> wholeDigests_;
    std::array<OriginalClockBoundary,2> wholeClocks_{};
    std::array<bool,2> wholeMatched_{};
    OriginalClockBoundary finalClock_{};
    const OriginalClockBoundary* precedingClockOriginal_=nullptr;
    size_t rowCapacity_=0,rowCount_=0,memberCount_=0,windowCapacity_=0,sourceBytes_=0;
    size_t membersObserved_=0,passesObserved_=0;
    int borrowedFd_=-1;
    bool storageAttempted_=false,storagePreowned_=false;
    bool started_=false,failed_=false,observed_=false,borrowReleased_=false;
    ~OriginalUnitPayloadArchive()=default;
    NativeOriginal* Begin(Operation op,size_t pass=SIZE_MAX,size_t member=SIZE_MAX) noexcept {
        if(failed_||rowCount_==rowCapacity_){failed_=true;return nullptr;}
        auto& row=rows_[rowCount_++];row.operation=op;row.pass=pass;row.member=member;
        if(!row.clock.Before(&deadline_,"unit-payload-archive",false)||
           (precedingClockOriginal_&&(!precedingClockOriginal_->shapeValid||precedingClockOriginal_->expired||
            row.clock.before.nanoseconds<precedingClockOriginal_->nanoseconds))||!supervisor_.BeginNativeCall()){
            failed_=true;return nullptr;
        }
        row.attempted=true;return &row;
    }
    bool Returned(NativeOriginal& row,ssize_t result,int error) noexcept {
        row.result=result;row.error=error;row.returned=true;
        supervisor_.OriginalNativeCallReturned();row.clock.After(&deadline_,"unit-payload-archive");
        if(!row.clock.clean)failed_=true;else precedingClockOriginal_=&row.clock.after;return !failed_;
    }
    bool Stat(Operation op,size_t pass=SIZE_MAX,size_t member=SIZE_MAX) noexcept {
        auto* row=Begin(op,pass,member);if(!row)return false;
        const int result=fstat(borrowedFd_,&row->metadata);const int error=result<0?errno:0;
        const bool clock=Returned(*row,result,error);
        const auto* offer=unit_.receiver_.OriginalOffer(unit_.offerOrdinal_);
        const bool same=clock&&!result&&offer&&
            OriginalReceivedSourceUnitRange::Same(offer->rights[0].status,row->metadata);
        if(!same)failed_=true;return same;
    }
    bool PrefixMatches(size_t offset,const unsigned char* bytes,size_t extent) const noexcept {
        while(extent){
            const auto* range=offset<64?unit_.header_.get():unit_.table_.get();
            const size_t start=offset<64?0:64;
            if(!range||offset<start||offset-start>range->retained_)return false;
            const size_t available=range->retained_-(offset-start);
            const size_t take=std::min(available,extent);if(!take)return false;
            if(std::memcmp(bytes,range->bytes_.get()+offset-start,take))return false;
            offset+=take;bytes+=take;extent-=take;
        }
        return true;
    }
    bool ReadSpan(size_t pass,size_t member,size_t offset,size_t extent,
                  template_creator_source::OriginalSha256* digest,bool prefix,
                  template_creator::OriginalGitObjectSha1* git=nullptr) noexcept {
        size_t used=0;
        while(used<extent){
            if(rowCount_==rowCapacity_){failed_=true;return false;}
            auto* row=&rows_[rowCount_];
            row->offset=offset+used;row->requested=std::min(windowCapacity_,extent-used);
            if(member!=SIZE_MAX){
                const auto& record=unit_.records_[member];
                row->windowGeometryObserved=slcli_source_unit12_window_geometry(
                    &record,unit_.source_.source_unit_bytes,used,windowCapacity_,&row->windowGeometry)!=0;
                if(!row->windowGeometryObserved||row->windowGeometry.member_index!=member||
                   row->windowGeometry.member_offset!=offset||row->windowGeometry.relative_offset!=used||
                   row->windowGeometry.source_offset!=row->offset||row->windowGeometry.requested!=row->requested){
                    row->operation=Operation::Read;row->pass=pass;row->member=member;
                    ++rowCount_;failed_=true;return false; // No native reservation or syscall.
                }
            }
            row=Begin(Operation::Read,pass,member);if(!row)return false;
            const ssize_t result=pread(borrowedFd_,window_.get(),row->requested,static_cast<off_t>(row->offset));
            const int error=result<0?errno:0;const bool clock=Returned(*row,result,error);
            if(result<0||static_cast<size_t>(result)>row->requested){row->extentUnknown=true;failed_=true;return false;}
            if(!result||!clock){failed_=true;return false;}
            const size_t actual=static_cast<size_t>(result);
            if((prefix&&!PrefixMatches(row->offset,window_.get(),actual))||
               !wholeDigests_[pass].Update(window_.get(),actual)||(digest&&!digest->Update(window_.get(),actual))||
               (git&&!git->Update(window_.get(),actual))){
                failed_=true;return false;
            }
            if(member!=SIZE_MAX){
                auto& state=members_[pass*memberCount_+member].edgeState;
                size_t parsed=0;
                while(parsed<actual){
                    if(edgeFeedCount_==edgeFeedCapacity_){failed_=true;return false;}
                    auto& feed=edgeFeeds_[edgeFeedCount_++];feed.readOriginal=row;
                    feed.pass=pass;feed.member=member;feed.windowOffset=parsed;
                    feed.relativeOffset=static_cast<uint64_t>(used)+parsed;feed.offered=actual-parsed;
                    feed.attempted=true;
                    feed.status=slcli_source_unit12_git_edge_feed(&state,feed.relativeOffset,
                        window_.get()+parsed,feed.offered,&feed.consumed,&feed.edge);
                    feed.returned=true;
                    if((feed.status!=1&&feed.status!=2)||!feed.consumed||feed.consumed>feed.offered){
                        failed_=true;return false;
                    }
                    if(feed.status==2){
                        feed.resumeAttempted=true;
                        feed.resumeStatus=slcli_source_unit12_git_edge_resume(&state,&feed.edge);
                        feed.resumeReturned=true;
                        if(feed.resumeStatus!=1){failed_=true;return false;}
                        // SAME retained edge remains in this row. Resume only
                        // clears structural pending state, never graph/archive authority.
                    }
                    parsed+=feed.consumed;
                }
            }
            used+=actual;
            // Reuse only after returned/count-known and complete digest consumption.
            // Original bytes remain in native Layer0 only if its separate proof qualifies.
        }
        return true;
    }
public:
    static size_t CheckedRows(const slcli_bq_provider_expectations& source){
        if(!source.source_unit_record_count||source.source_unit_bytes>SIZE_MAX||
           source.source_unit_record_count>(SIZE_MAX-7)/4||
           source.source_unit_record_count>(SIZE_MAX-64)/128||
           source.source_unit_bytes<64+128*source.source_unit_record_count||
           source.source_unit_bytes>static_cast<uintmax_t>(std::numeric_limits<off_t>::max()))
            throw std::invalid_argument("unit payload row geometry");
        const size_t fixed=4*static_cast<size_t>(source.source_unit_record_count)+7;
        if(source.source_unit_bytes>(SIZE_MAX-fixed)/2)throw std::invalid_argument("unit payload row overflow");
        return 2*static_cast<size_t>(source.source_unit_bytes)+fixed;
    }
    static size_t CheckedSourceBacking(const slcli_bq_provider_expectations& source){
        const size_t rows=CheckedRows(source);size_t bytes=sizeof(OriginalUnitPayloadArchive);
        const auto add=[&](size_t count,size_t width){
            if(width&&count>(SIZE_MAX-bytes)/width)throw std::invalid_argument("unit payload backing overflow");
            bytes+=count*width;
        };
        if(source.source_unit_record_count>SIZE_MAX/2)throw std::invalid_argument("unit payload digest count");
        add(rows,sizeof(NativeOriginal));add(2*static_cast<size_t>(source.source_unit_record_count),sizeof(MemberPass));
        add(2*static_cast<size_t>(source.source_unit_bytes),sizeof(EdgeFeedOriginal));
        add(std::min<size_t>(static_cast<size_t>(source.source_unit_bytes),65536),1);return bytes;
    }
private:
    OriginalUnitPayloadArchive(const OriginalReceivedSourceUnitObservation& unit,
        OriginalDeadline& deadline,OriginalDeadlineSupervisor& supervisor)
        :unit_(unit),deadline_(deadline),supervisor_(supervisor),rowCapacity_(CheckedRows(unit.source_)),
         memberCount_(static_cast<size_t>(unit.source_.source_unit_record_count)),
         windowCapacity_(std::min<size_t>(static_cast<size_t>(unit.source_.source_unit_bytes),65536)),
         sourceBytes_(CheckedSourceBacking(unit.source_)) {
        edgeFeedCapacity_=2*static_cast<size_t>(unit.source_.source_unit_bytes);
        if(&unit.deadline_!=&deadline||&unit.supervisor_!=&supervisor||supervisor.OriginalDeadlineOwner()!=&deadline)
            throw std::invalid_argument("unit payload original owner");
        // No backing allocation here: startup first attaches this surviving
        // owner, then its explicit attempt method attaches each output.
    }
    bool PreownStorageOriginal() noexcept {
        if(storageAttempted_||started_||borrowReleased_)return false;storageAttempted_=true;
        size_t current=0;
        try{
            auto& rows=allocations_[current];rows.bytes=rowCapacity_*sizeof(NativeOriginal);rows.attempted=true;
            rows_=std::make_unique<NativeOriginal[]>(rowCapacity_);
            rows.output=rows_.get();rows.returned=true;
            current=1;auto& members=allocations_[current];members.bytes=2*memberCount_*sizeof(MemberPass);members.attempted=true;
            members_=std::make_unique<MemberPass[]>(2*memberCount_);
            members.output=members_.get();members.returned=true;
            current=2;auto& window=allocations_[current];window.bytes=windowCapacity_;window.attempted=true;
            window_=std::make_unique<unsigned char[]>(windowCapacity_);
            window.output=window_.get();window.returned=true;
            current=3;auto& edges=allocations_[current];edges.bytes=edgeFeedCapacity_*sizeof(EdgeFeedOriginal);edges.attempted=true;
            edgeFeeds_=std::make_unique<EdgeFeedOriginal[]>(edgeFeedCapacity_);
            edges.output=edgeFeeds_.get();edges.returned=true;
            storagePreowned_=true;return true;
        }catch(...){
            allocations_[current].threw=true;allocations_[current].exceptionOriginal=std::current_exception();
            failed_=true;return false; // Earlier attached outputs stay in SAME owner.
        }
    }
    bool ObserveIngressAliasesOriginal(const OriginalPrivateIngress& ingress,const IngressFd& unitFd) noexcept {
        if(ingressAliasesAttempted_)return false;
        ingressAliasesAttempted_=true;ingressOriginal_=&ingress;
        const auto& descriptors=ingress.OriginalDescriptors();
        const size_t count=ingress.OriginalDescriptorCount();
        if(!count||count>ingressAliases_.size()||!unitFd.statReturned||unitFd.statResult||
           unitFd.closeAttempted||!S_ISREG(unitFd.status.st_mode)){failed_=true;return false;}
        for(size_t i=0;i<count;++i){
            auto& row=ingressAliases_[ingressAliasCount_++];
            const auto& fd=descriptors[i];row.descriptorOriginal=&fd;row.ordinal=i;
            if(fd.original<0||fd.closeAttempted||!fd.statAttempted||!fd.statReturned||fd.statResult||
               !fd.statClock.clean||!fd.statClock.after.shapeValid||fd.statClock.after.expired||
               !row.clock.Before(&deadline_,"unit-ingress-alias-flags",false)||
               row.clock.before.nanoseconds<fd.statClock.after.nanoseconds||
               (precedingClockOriginal_&&row.clock.before.nanoseconds<precedingClockOriginal_->nanoseconds)||
               !supervisor_.BeginNativeCall()){failed_=true;return false;}
            row.attempted=true;row.flags=fcntl(fd.original,F_GETFL);row.error=row.flags<0?errno:0;row.returned=true;
            supervisor_.OriginalNativeCallReturned();row.clock.After(&deadline_,"unit-ingress-alias-flags");
            row.sameInode=fd.status.st_dev==unitFd.status.st_dev&&fd.status.st_ino==unitFd.status.st_ino;
            row.writableAlias=row.flags>=0&&row.sameInode&&(row.flags&O_ACCMODE)!=O_RDONLY;
            row.observed=row.clock.clean&&row.flags>=0&&!row.writableAlias;
            if(!row.observed){failed_=true;return false;}
            precedingClockOriginal_=&row.clock.after;
        }
        // Birth KEEP/stdio completeness and no-close/no-reuse own-writer lifetime
        // remain independent; these native identity/rights observations grant no ACK.
        ingressAliasesObserved_=true;return true;
    }
    bool CaptureOriginal(const OriginalPrivateIngress& ingress) noexcept {
        if(started_||borrowReleased_)return false;started_=true;
        if(!storagePreowned_||!unit_.observed_||!unit_.tableObserved_||!unit_.associationRowsObserved_||unit_.borrowReleased_||
           unit_.recordCount_!=memberCount_||unit_.parsed_!=memberCount_||
           supervisor_.HasPendingOriginalNativeCall()){failed_=true;return false;}
        const auto* offer=unit_.receiver_.OriginalOffer(unit_.offerOrdinal_);
        if(!offer||!offer->rawOfferObserved||offer->rightsCount!=1||offer->rights[0].closeAttempted||
           !offer->rights[0].statReturned||offer->rights[0].statResult){failed_=true;return false;}
        precedingClockOriginal_=&unit_.associationClock_;
        borrowedFd_=offer->rights[0].original;
        if(!ObserveIngressAliasesOriginal(ingress,offer->rights[0]))return false;
        auto* access=Begin(Operation::Access);if(!access)return false;
        const int flags=fcntl(borrowedFd_,F_GETFL);const int flagError=flags<0?errno:0;
        if(!Returned(*access,flags,flagError)||flags<0||(flags&O_ACCMODE)!=O_RDONLY||(flags&O_PATH)){
            failed_=true;return false;
        }
        for(size_t pass=0;pass<2;++pass){
            if(!Stat(Operation::UnitBefore,pass)||
               !ReadSpan(pass,SIZE_MAX,0,static_cast<size_t>(unit_.parser_.body_offset),nullptr,true))return false;
            for(size_t member=0;member<memberCount_;++member){
                const auto& record=unit_.records_[member];auto& result=members_[pass*memberCount_+member];
                result.started=true;
                result.edgesBegan=slcli_source_unit12_git_edge_begin(&result.edgeState,&record)==1;
                if(!result.edgesBegan){failed_=true;return false;}
                result.gitRequired=record.kind!=0;
                if(result.gitRequired&&!result.gitDigest.Begin(record.kind,record.bytes)){failed_=true;return false;}
                if(record.index!=member||record.offset>unit_.source_.source_unit_bytes||
                   record.bytes>unit_.source_.source_unit_bytes-record.offset||
                   !Stat(Operation::MemberBefore,pass,member)||
                   !ReadSpan(pass,member,static_cast<size_t>(record.offset),static_cast<size_t>(record.bytes),&result.digest,false,
                       result.gitRequired?&result.gitDigest:nullptr))return false;
                result.edgesFinished=slcli_source_unit12_git_edge_finish(&result.edgeState)==1;
                result.digestMatched=result.digest.Finish()&&std::memcmp(result.digest.PrivateDigest().data(),record.digest,32)==0;
                result.gitMatched=!result.gitRequired||(result.gitDigest.Finish()&&unit_.associationOriginal_&&
                    std::memcmp(result.gitDigest.Digest().data(),unit_.associationOriginal_->AssociationRecordsOriginal()[member].git,20)==0);
                if(!result.edgesFinished||!result.digestMatched||!result.gitMatched||!Stat(Operation::MemberAfter,pass,member)){failed_=true;return false;}
                const bool clock=IngressNativeClock::Capture(result.finalClock,&deadline_,"unit-payload-member-digest",true);
                result.observed=clock&&result.finalClock.shapeValid&&!result.finalClock.expired&&
                    rowCount_&&result.finalClock.nanoseconds>=rows_[rowCount_-1].clock.after.nanoseconds;
                if(!result.observed){failed_=true;return false;}++membersObserved_;
                precedingClockOriginal_=&result.finalClock;
            }
            wholeMatched_[pass]=wholeDigests_[pass].Finish()&&
                std::memcmp(wholeDigests_[pass].PrivateDigest().data(),unit_.source_.source_unit_digest,32)==0;
            if(!wholeMatched_[pass]||!Stat(Operation::UnitAfter,pass)){failed_=true;return false;}
            const bool clock=IngressNativeClock::Capture(wholeClocks_[pass],&deadline_,"unit-payload-whole-digest",true);
            if(!clock||!wholeClocks_[pass].shapeValid||wholeClocks_[pass].expired||
               wholeClocks_[pass].nanoseconds<rows_[rowCount_-1].clock.after.nanoseconds){failed_=true;return false;}
            ++passesObserved_;
            precedingClockOriginal_=&wholeClocks_[pass];
        }
        auto* eof=Begin(Operation::WholeEof);if(!eof)return false;
        eof->offset=static_cast<size_t>(unit_.source_.source_unit_bytes);eof->requested=1;
        const ssize_t end=pread(borrowedFd_,&eof->eofByte,1,static_cast<off_t>(eof->offset));const int error=end<0?errno:0;
        const bool clock=Returned(*eof,end,error);if(end<0||end>1)eof->extentUnknown=true;
        if(!clock||end||!Stat(Operation::FinalStat)){failed_=true;return false;}
        const bool final=IngressNativeClock::Capture(finalClock_,&deadline_,"unit-payload-archive-cut",true);
        observed_=final&&finalClock_.shapeValid&&!finalClock_.expired&&passesObserved_==2&&
            membersObserved_==2*memberCount_&&finalClock_.nanoseconds>=rows_[rowCount_-1].clock.after.nanoseconds&&
            finalClock_.nanoseconds>=unit_.associationClock_.nanoseconds&&
            wholeDigests_[0].PrivateDigest()==wholeDigests_[1].PrivateDigest();
        if(!observed_)failed_=true;return observed_;
    }
    bool BorrowerReleasedOriginal() const noexcept{return borrowReleased_;}
    bool ReleaseBeforeExposureOriginal() noexcept {
        if(supervisor_.HasPendingOriginalNativeCall())return false;
        for(size_t i=0;i<ingressAliasCount_;++i)
            if(ingressAliases_[i].attempted&&!ingressAliases_[i].returned)return false;
        for(size_t i=0;i<rowCount_;++i)if(rows_[i].extentUnknown||(rows_[i].attempted&&!rows_[i].returned))return false;
        borrowReleased_=true;return true;
    }
public:
    struct RetainedPreownBackingView {
        const OriginalUnitPayloadArchive* ownerOriginal;const OriginalReceivedSourceUnitObservation* unitOriginal;
        const AllocationOriginal* allocationsOriginal;const NativeOriginal* rowsOriginal;const MemberPass* membersOriginal;
        const unsigned char* windowOriginal;const EdgeFeedOriginal* edgesOriginal;
        size_t allocationCount,rowCapacity,memberCapacity,windowCapacity,edgeCapacity,rowCount,edgeCount,allocatedBackingBytes;
        bool attempted,preowned,released;
    };
    bool ObserveRetainedPreownBackingOriginal(RetainedPreownBackingView& out) const noexcept {
        const auto output=reinterpret_cast<uintptr_t>(&out),owner=reinterpret_cast<uintptr_t>(this);size_t bytes=0;
        if(!borrowReleased_||!AllocatedBackingBytesOriginal(bytes)||rowCount_>(rows_?rowCapacity_:0)||
           edgeFeedCount_>(edgeFeeds_?edgeFeedCapacity_:0)||output>UINTPTR_MAX-sizeof(out)||owner>UINTPTR_MAX-sizeof(*this)||
           (output<owner+sizeof(*this)&&owner<output+sizeof(out)))return false;
        if(!OriginalBackingStorageSeparate(&out,sizeof(out),rows_.get(),rows_?rowCapacity_:0,sizeof(NativeOriginal))||
           !OriginalBackingStorageSeparate(&out,sizeof(out),members_.get(),members_?memberCount_:0,2*sizeof(MemberPass))||
           !OriginalBackingStorageSeparate(&out,sizeof(out),window_.get(),window_?windowCapacity_:0)||
           !OriginalBackingStorageSeparate(&out,sizeof(out),edgeFeeds_.get(),edgeFeeds_?edgeFeedCapacity_:0,sizeof(EdgeFeedOriginal)))return false;
        for(const auto& row:allocations_)if(row.attempted&&!row.returned&&!row.threw)return false;
        std::memset(&out,0,sizeof(out));out.ownerOriginal=this;out.unitOriginal=&unit_;out.allocationsOriginal=allocations_.data();
        out.rowsOriginal=rows_.get();out.membersOriginal=members_.get();out.windowOriginal=window_.get();out.edgesOriginal=edgeFeeds_.get();
        out.allocationCount=allocations_.size();out.rowCapacity=rows_?rowCapacity_:0;out.memberCapacity=members_?2*memberCount_:0;
        out.windowCapacity=window_?windowCapacity_:0;out.edgeCapacity=edgeFeeds_?edgeFeedCapacity_:0;
        out.rowCount=rowCount_;out.edgeCount=edgeFeedCount_;out.allocatedBackingBytes=bytes;
        out.attempted=storageAttempted_;out.preowned=storagePreowned_;out.released=borrowReleased_;return true;
    }
    size_t SourceBackingBytes() const noexcept{return sourceBytes_;}
    bool AllocatedBackingBytesOriginal(size_t& bytes) const noexcept {
        bytes=sizeof(*this);
        const void* outputs[4]={rows_.get(),members_.get(),window_.get(),edgeFeeds_.get()};
        for(size_t i=0;i<allocations_.size();++i){const auto& row=allocations_[i];
            if(row.output!=outputs[i]||(row.output&&(!row.attempted||!row.returned||row.threw)))return false;
            if(row.output){if(row.bytes>SIZE_MAX-bytes)return false;bytes+=row.bytes;}}
        return true; // Attached storage subtotal, not allocator/native/runtime fit.
    }
};
}
#endif

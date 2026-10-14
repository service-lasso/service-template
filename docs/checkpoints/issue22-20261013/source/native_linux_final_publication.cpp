// UNCOMPILED post-platform original capture. Same-ABI observations are private;
// Python referents/native exceptions stay owned, not serialized by pointer.
#ifndef _WIN32
#include "native_linux_platform.hpp"
#include "linux_session_gate.hpp"
#include "linux_initial_provider_archive.hpp"
namespace template_creator_bridge {
bool NativePlatformState::PublishFinalPlatformOriginals() noexcept {
    if(finalPublicationAttempted)return finalPublicationReturned;
    if(!finalPublicationInputCut||pending.OriginalSupervisorOwner().HasPendingOriginalNativeCall())return false;
    finalPublicationAttempted=true;
    if(!finalOutput.Adopted()){
        finalPublicationFailed=qualificationFailed=true;
        finalOutput.ObserveEligibleFinalDisposition();
        finalPublicationReturned=true;return true; // Actual call returned; physical result remains owned separately.
    }
    try{
        bool copied=true;
        struct Frame{uint64_t version,kind,ordinal,extent;};
        const auto append=[&](uint64_t kind,size_t ordinal,const void* body,size_t extent) noexcept {
            const Frame header{1,kind,ordinal,extent};
            const bool h=finalOutput.AppendFinalOriginals(reinterpret_cast<const unsigned char*>(&header),sizeof(header));
            const bool b=h&&finalOutput.AppendFinalOriginals(static_cast<const unsigned char*>(body),extent);
            copied=b&&copied;
        };
        // The inline pendingHostStorage can still have live observer writers.
        // Copy only this creating thread's settled input-cut values, not that
        // mutable native graph. Its real originals stay resident through joins.
        struct PlatformInputCutRecord {
            uint64_t version,phase,commandCount,creatorTid;
            const void* platformOriginal;
            const void* pendingHostOriginal;
            const void* pendingOriginal;
            const void* routeOriginal;
            const void* inputOwnersOriginal;
            bool inputCut,inputDisposalStarted,nativeInputsDisposed,qualificationFailed;
            bool externalPendingHost,externalPendingHostOwned;
        };
        PlatformInputCutRecord platformCut;
        std::memset(&platformCut,0,sizeof(platformCut)); // Initialize private ABI padding too.
        platformCut.version=2;platformCut.phase=static_cast<uint64_t>(originalPhase);
        platformCut.commandCount=route.CommandCount();
        platformCut.creatorTid=static_cast<uint64_t>(pending.OriginalCreatorTid());
        platformCut.platformOriginal=this;platformCut.pendingHostOriginal=&pendingHost;
        platformCut.pendingOriginal=&pending;platformCut.routeOriginal=&route;
        platformCut.inputOwnersOriginal=inputOwners.get();platformCut.inputCut=finalPublicationInputCut;
        platformCut.inputDisposalStarted=inputDisposalStarted;platformCut.nativeInputsDisposed=nativeInputDisposalResult;
        platformCut.qualificationFailed=qualificationFailed;platformCut.externalPendingHost=externalPendingHost;
        platformCut.externalPendingHostOwned=externalPendingHostOwned;
        RetainedPreownView preownView{};
        if(!ObserveRetainedPreownViewOriginal(preownView)||
           sizeof(preownView)>SIZE_MAX-sizeof(Frame)||
           !finalOutput.CanAppendFinalSpanOriginal(sizeof(Frame)+sizeof(preownView)))copied=false;
        else append(111,0,&preownView,sizeof(preownView));
        append(1,0,&platformCut,sizeof(platformCut));
        append(2,0,sink.PrivateOriginalCopiedBytes(),sink.OriginalCopiedExtent());
        append(3,0,sink.PrivateOriginalReadbackBytes(),sink.OriginalReadbackExtent());
        if(sink.PrivateOriginalCallCount()>SIZE_MAX/sizeof(template_creator_linux::SinkOriginalCall))copied=false;
        else append(4,0,sink.PrivateOriginalCalls(),sink.PrivateOriginalCallCount()*sizeof(template_creator_linux::SinkOriginalCall));
        // Once per SAME startup pair. All native result/clock cells are inline;
        // receiver rights and later close/ACK originals retain separate owners.
        if(const auto* pair=pendingHost.ReceiptFoundationPairOriginal()){
            if(!pair->SourceDispositionEstablishedOriginal()||
               sizeof(*pair)>SIZE_MAX-sizeof(Frame)||
               !finalOutput.CanAppendFinalSpanOriginal(sizeof(Frame)+sizeof(*pair)))copied=false;
            else append(110,0,pair,sizeof(*pair));
        }
        for(size_t ordinal=0;ordinal<route.BornCommandCountOriginal();++ordinal){
            const auto& command=route.PrivateCommand(ordinal);
            append(5,ordinal,&command,sizeof(command));
            if(command.driver){
                const auto& state=command.driver->PrivateOriginals();append(6,ordinal,&state,sizeof(state));
                const auto stream=[&](const template_creator_linux::Reader& reader,uint64_t kind) noexcept {
                    if(reader.used>template_creator_linux::Reader::ByteLimit||reader.discardRetained>template_creator_linux::Reader::ByteLimit||
                       reader.resultCount>template_creator_linux::EventLimit||reader.discardResultCount>template_creator_linux::EventLimit){copied=false;return;}
                    append(kind,ordinal,reader.bytes.get(),reader.used);
                    append(kind+1,ordinal,reader.results.get(),reader.resultCount*sizeof(template_creator_linux::ReadResult));
                    append(kind+2,ordinal,reader.discard.get(),reader.discardRetained);
                    append(kind+3,ordinal,reader.discardResults.get(),reader.discardResultCount*sizeof(template_creator_linux::ReadResult));
                };
                stream(state.stdoutStream,10);stream(state.stderrStream,14);stream(state.setupStream,18);
            }
            if(ordinal>=allocatedInputOwnerCapacity)continue;
            const auto& owner=inputOwners[ordinal];
            if(owner.initialProviderOriginal){
                const auto* archive=owner.initialProviderOriginal->OriginalArchiveOwner();
                if(!archive||!archive->NativeBorrowersSettledForOriginal(this))copied=false;
                bool alreadyExported=false;
                for(size_t prior=0;prior<ordinal;++prior)
                    if(inputOwners[prior].initialProviderOriginal==owner.initialProviderOriginal)alreadyExported=true;
                struct ArchiveCommandAssociation {uint64_t version,command;const void* receiverOriginal;const void* archiveOriginal;} association;
                std::memset(&association,0,sizeof(association));association.version=1;association.command=ordinal;
                association.receiverOriginal=owner.initialProviderOriginal;association.archiveOriginal=archive;
                append(43,ordinal,&association,sizeof(association));
                if(archive&&!alreadyExported&&archive->NativeBorrowersSettledForOriginal(this))
                for(size_t cutOrdinal=0;cutOrdinal<archive->OriginalCutCount();++cutOrdinal){
                    const auto* cut=archive->OriginalCut(cutOrdinal);
                    if(!cut||cut->owner!=archive||!cut->prefixRetained){copied=false;continue;}
                    // Immutable completed cut only. Future ACK/close cells stay
                    // resident in SAME native owners and are not copied live.
                    struct ArchivePrefixView {
                        uint64_t version,ordinal,payloadBytes,readCount;
                        const void* archiveOriginal;
                        const void* cutOriginal;
                        const void* receiveOriginal;
                        const void* processOriginal;
                        const void* fileOriginal;
                        const void* unitOriginal;
                        const void* futureAckOriginal;
                        const void* futureCloseOriginal;
                    } view;
                    std::memset(&view,0,sizeof(view));view.version=2;view.ordinal=cutOrdinal;
                    view.payloadBytes=cut->payloadBytes;view.readCount=cut->readCount;
                    view.archiveOriginal=archive;view.cutOriginal=cut;view.receiveOriginal=cut->offer;
                    view.processOriginal=cut->process;view.fileOriginal=cut->file;
                    view.unitOriginal=cut->unit;
                    view.futureAckOriginal=&cut->offer->ack;view.futureCloseOriginal=&cut->offer->rights[0];
                    append(38,cutOrdinal,&view,sizeof(view));
                    append(39,cutOrdinal,cut->offer->payload.data(),cut->offer->payload.size());
                    append(40,cutOrdinal,cut->offer->control.data(),cut->offer->control.size());
                    // The actual input-cut disposer has now settled these
                    // formerly mutable cells before supervisor stop. Capture
                    // initialized typed outputs, without another ACK or IO.
                    const auto& ack=cut->offer->ack;
                    struct AckSettlementView {
                        uint64_t version,sequence;
                        const void* original;
                        int waitResult,waitError;
                        ssize_t sendResult;
                        int sendError;
                        bool waitAttempted,waitReturned,sendAttempted,sendReturned,complete;
                    } ackView;
                    std::memset(&ackView,0,sizeof(ackView));ackView.version=1;ackView.sequence=ack.route.sequence;
                    ackView.original=&ack;ackView.waitResult=ack.waitResult;ackView.waitError=ack.waitError;
                    ackView.sendResult=ack.sendResult;ackView.sendError=ack.sendError;
                    ackView.waitAttempted=ack.waitAttempted;ackView.waitReturned=ack.waitReturned;
                    ackView.sendAttempted=ack.sendAttempted;ackView.sendReturned=ack.sendReturned;ackView.complete=ack.complete;
                    append(56,cutOrdinal,&ackView,sizeof(ackView));
                    const auto closeView=[&](const template_creator_linux::IngressFd& fd,uint64_t kind) noexcept {
                        struct CloseSettlementView {
                            uint64_t version;const void* original;int descriptor,result,error;
                            bool attempted,returned,closed;
                        } closed;
                        std::memset(&closed,0,sizeof(closed));closed.version=1;closed.original=&fd;
                        closed.descriptor=fd.original;closed.result=fd.closeResult;closed.error=fd.closeErrno;
                        closed.attempted=fd.closeAttempted;closed.returned=fd.closeReturned;closed.closed=fd.closed;
                        append(kind,cutOrdinal,&closed,sizeof(closed));
                    };
                    closeView(cut->offer->rights[0],57);
                    if(cutOrdinal==0)closeView(archive->OriginalEndpoint(),58);
                    if(cut->process){
                        const auto& process=*cut->process;
                        struct ProcessPrefixView {
                            uint64_t version,archiveNanoseconds;
                            const void* processOriginal;
                            const void* callerTaskOriginal;
                            const void* fdinfoOriginal;
                            long signalResult;
                            int signalError,beforeResult,beforeError,afterResult,afterError;
                            short beforeRevents,afterRevents;
                            bool signalAttempted,signalReturned,beforeAttempted,beforeReturned,afterAttempted,afterReturned;
                        } processView;
                        std::memset(&processView,0,sizeof(processView));processView.version=1;
                        processView.archiveNanoseconds=cut->finalClock.nanoseconds;processView.processOriginal=&process;
                        processView.callerTaskOriginal=&process.OriginalCallerTask();processView.fdinfoOriginal=&process.OriginalFdinfo();
                        processView.signalResult=process.OriginalSignalZeroResult();processView.signalError=process.OriginalSignalZeroError();
                        processView.signalAttempted=process.OriginalSignalZeroAttempted();processView.signalReturned=process.OriginalSignalZeroReturned();
                        const auto& before=process.OriginalBeforePoll();const auto& after=process.OriginalAfterPoll();
                        processView.beforeResult=before.returned;processView.beforeError=before.error;processView.beforeRevents=before.output.revents;
                        processView.afterResult=after.returned;processView.afterError=after.error;processView.afterRevents=after.output.revents;
                        processView.beforeAttempted=before.attempted;processView.beforeReturned=before.returnedObserved;
                        processView.afterAttempted=after.attempted;processView.afterReturned=after.returnedObserved;
                        append(44,cutOrdinal,&processView,sizeof(processView));
                        const auto procFile=[&](const template_creator_linux::ProcFileObservation& file,uint64_t bodyKind,uint64_t rowsKind) noexcept {
                            if(file.shapeFailure||!file.eof||file.used>file.byteCapacity||file.readCount>file.readCapacity||
                               file.readCount>SIZE_MAX/sizeof(template_creator_linux::ProcReadOriginal)){copied=false;return;}
                            for(size_t i=0;i<file.readCount;++i)
                                if(file.reads[i].attempted&&!file.reads[i].returnedObserved){copied=false;return;}
                            append(bodyKind,cutOrdinal,file.bytes.get(),file.used);
                            append(rowsKind,cutOrdinal,file.reads.get(),file.readCount*sizeof(template_creator_linux::ProcReadOriginal));
                        };
                        procFile(process.OriginalFdinfo(),49,50);
                        const auto& callerTask=process.OriginalCallerTask();
                        for(size_t observation=0;observation<callerTask.ObservationCount();++observation){
                            const auto& task=callerTask.OriginalObservation(observation);
                            struct TaskAssociationView {uint64_t version,offer,observation;const void* taskOwner;const void* observationOriginal;} taskView;
                            std::memset(&taskView,0,sizeof(taskView));taskView.version=1;taskView.offer=cutOrdinal;
                            taskView.observation=observation;taskView.taskOwner=&callerTask;taskView.observationOriginal=&task;
                            append(51,cutOrdinal,&taskView,sizeof(taskView));
                            procFile(task.stat,52,53);procFile(task.status,54,55);
                        }
                    }
                    if(cut->unit){
                        const auto& unit=*cut->unit;
                        size_t unitSpan=0;bool unitSpanValid=true,unitEmit=false;
                        const auto unitAppend=[&](uint64_t kind,const void* body,size_t extent) noexcept {
                            if(!unitSpanValid)return;
                            if(!unitEmit){
                                if(extent>SIZE_MAX-sizeof(Frame)||extent+sizeof(Frame)>SIZE_MAX-unitSpan){
                                    unitSpanValid=false;return;
                                }
                                unitSpan+=sizeof(Frame)+extent;
                            }else append(kind,cutOrdinal,body,extent);
                        };
                        struct UnitCorrespondenceView {
                            uint64_t version,offer,recordCount,parsed,compared;
                            const void* unitOriginal;const void* sourceOriginal;
                            const void* subjectOriginal;const void* associationOriginal;
                             const void* payloadOriginal;const void* earlierUnitCutOriginal;
                            bool tableObserved,memberObserved,observed,associationAttempted,associationObserved;
                        } unitView;
                        std::memset(&unitView,0,sizeof(unitView));unitView.version=1;
                        unitView.offer=unit.offerOrdinal_;unitView.recordCount=unit.recordCount_;
                        unitView.parsed=unit.parsed_;unitView.compared=unit.associationRowsCompared_;
                        unitView.unitOriginal=&unit;unitView.sourceOriginal=&unit.source_;
                        unitView.subjectOriginal=&unit.subject_;unitView.associationOriginal=unit.associationOriginal_;
                         unitView.payloadOriginal=cut->unitPayload;unitView.earlierUnitCutOriginal=cut->earlierUnit;
                        unitView.tableObserved=unit.tableObserved_;unitView.memberObserved=unit.memberObserved_;
                        unitView.observed=unit.observed_;unitView.associationAttempted=unit.associationAttempted_;
                        unitView.associationObserved=unit.associationRowsObserved_;
                        if(&unit.receiver_!=owner.initialProviderOriginal||unit.offerOrdinal_!=cutOrdinal||
                           unit.parsed_>unit.recordCount_||unit.recordCount_>SIZE_MAX/sizeof(struct slcli_source_unit12_record))unitSpanValid=false;
                        else unitSpanValid=true;
                        const auto unitPrefix=[&]() noexcept {
                            unitAppend(62,&unitView,sizeof(unitView));
                            unitAppend(63,&unit.parser_,sizeof(unit.parser_));
                            unitAppend(64,unit.records_.get(),unit.parsed_*sizeof(struct slcli_source_unit12_record));
                            unitAppend(65,&unit.finalClock_,sizeof(unit.finalClock_));
                            unitAppend(66,&unit.associationClock_,sizeof(unit.associationClock_));
                        };
                        const auto range=[&](const template_creator_linux::OriginalReceivedSourceUnitRange* original,
                                             uint64_t firstKind) noexcept {
                            if(!original){unitSpanValid=false;return;}
                            const auto& r=*original;
                            if(&r.receiver_!=owner.initialProviderOriginal||r.offer_!=cut->offer||
                               r.retained_>r.extent_||r.count_>r.capacity_||
                               r.count_>SIZE_MAX/sizeof(template_creator_linux::OriginalReceivedSourceUnitRange::NativeOriginal)){
                                unitSpanValid=false;return;
                            }
                            const auto settled=[](const auto& row) noexcept {
                                return !row.extentUnknown&&(!row.attempted||row.returned);
                            };
                            if(!settled(r.before_)||!settled(r.access_)||!settled(r.eof_)||!settled(r.after_)){
                                unitSpanValid=false;return;
                            }
                            for(size_t i=0;i<r.count_;++i)if(!settled(r.reads_[i])){unitSpanValid=false;return;}
                            struct RangeView {
                                uint64_t version,offset,extent,unitBytes,retained,readCount;
                                const void* original;const void* offerOriginal;
                                bool started,observed,failed,digestAttempted,digestObserved,borrowReleased;
                            } rangeView;
                            std::memset(&rangeView,0,sizeof(rangeView));rangeView.version=1;
                            rangeView.offset=r.offset_;rangeView.extent=r.extent_;rangeView.unitBytes=r.unitBytes_;
                            rangeView.retained=r.retained_;rangeView.readCount=r.count_;
                            rangeView.original=&r;rangeView.offerOriginal=r.offer_;
                            rangeView.started=r.started_;rangeView.observed=r.observed_;rangeView.failed=r.failed_;
                            rangeView.digestAttempted=r.digestAttempted_;rangeView.digestObserved=r.digestObserved_;
                            rangeView.borrowReleased=r.borrowReleased_;
                            unitAppend(firstKind,&rangeView,sizeof(rangeView));
                            unitAppend(firstKind+1,r.bytes_.get(),r.retained_);
                            unitAppend(firstKind+2,r.reads_.get(),r.count_*sizeof(template_creator_linux::OriginalReceivedSourceUnitRange::NativeOriginal));
                            unitAppend(firstKind+3,&r.before_,sizeof(r.before_));
                            unitAppend(firstKind+4,&r.access_,sizeof(r.access_));
                            unitAppend(firstKind+5,&r.eof_,sizeof(r.eof_));
                            unitAppend(firstKind+6,&r.after_,sizeof(r.after_));
                            unitAppend(firstKind+7,&r.finalClock_,sizeof(r.finalClock_));
                            unitAppend(firstKind+8,&r.digestClock_,sizeof(r.digestClock_));
                            unitAppend(firstKind+9,r.expectedDigest_.data(),r.expectedDigest_.size());
                            const auto& actualDigest=r.digest_.PrivateDigest();
                            unitAppend(firstKind+10,actualDigest.data(),actualDigest.size());
                        };
                        const auto unitGraph=[&]() noexcept {
                            unitPrefix();range(unit.header_.get(),67);range(unit.table_.get(),78);
                            if(unit.member_)range(unit.member_.get(),89);
                             if(cut->unitPayload){
                                 const auto& payload=*cut->unitPayload;
                                 if(&payload.unit_!=&unit||payload.rowCount_>payload.rowCapacity_||
                                    payload.rowCount_>SIZE_MAX/sizeof(template_creator_linux::OriginalUnitPayloadArchive::NativeOriginal)||
                                     !payload.borrowReleased_||payload.ingressAliasCount_>payload.ingressAliases_.size()||
                                     payload.edgeFeedCount_>payload.edgeFeedCapacity_||
                                     payload.edgeFeedCount_>SIZE_MAX/sizeof(template_creator_linux::OriginalUnitPayloadArchive::EdgeFeedOriginal)){
                                      unitSpanValid=false;return;}
                                 for(size_t n=0;n<payload.ingressAliasCount_;++n)
                                     if(payload.ingressAliases_[n].attempted&&!payload.ingressAliases_[n].returned){
                                         unitSpanValid=false;return;}
                                 for(size_t n=0;n<payload.rowCount_;++n)if(
                                     (payload.rows_[n].attempted&&!payload.rows_[n].returned)||payload.rows_[n].extentUnknown){
                                     unitSpanValid=false;return;
                                 }
                                 struct PayloadView {
                                      uint64_t version,rows,capacity,members,passes,membersObserved,sourceBytes,ingressAliasCount;
                                      const void* owner;const void* unit;const void* memberStates;const void* ingress;
                                      bool storageAttempted,storagePreowned,started,failed,observed,borrowReleased;
                                      bool ingressAliasesAttempted,ingressAliasesObserved;
                                 } view;
                                  std::memset(&view,0,sizeof(view));view.version=2;
                                 view.rows=payload.rowCount_;view.capacity=payload.rowCapacity_;view.members=payload.memberCount_;
                                 view.passes=payload.passesObserved_;view.membersObserved=payload.membersObserved_;
                                 view.sourceBytes=payload.sourceBytes_;view.owner=&payload;view.unit=&unit;
                                 view.memberStates=payload.members_.get();view.storageAttempted=payload.storageAttempted_;
                                 view.storagePreowned=payload.storagePreowned_;view.started=payload.started_;
                                 view.failed=payload.failed_;view.observed=payload.observed_;view.borrowReleased=payload.borrowReleased_;
                                  view.ingress=payload.ingressOriginal_;view.ingressAliasCount=payload.ingressAliasCount_;
                                  view.ingressAliasesAttempted=payload.ingressAliasesAttempted_;
                                  view.ingressAliasesObserved=payload.ingressAliasesObserved_;
                                 unitAppend(100,&view,sizeof(view));
                                 unitAppend(101,payload.rows_.get(),payload.rowCount_*sizeof(payload.rows_[0]));
                                 unitAppend(102,payload.wholeClocks_.data(),sizeof(payload.wholeClocks_));
                                 unitAppend(103,&payload.finalClock_,sizeof(payload.finalClock_));
                                  unitAppend(109,payload.edgeFeeds_.get(),payload.edgeFeedCount_*sizeof(payload.edgeFeeds_[0]));
                                  for(size_t n=0;n<payload.ingressAliasCount_;++n){
                                      struct AliasView {uint64_t ordinal;const void* ingress;const void* descriptor;
                                          template_creator_linux::IngressNativeClock clock;int flags,error;
                                          template_creator_linux::IngressNativeClock statClock;
                                          struct stat status;int descriptorNumber,statResult,statError;
                                          bool statAttempted,statReturned,closeAttemptedAtCut;
                                          bool attempted,returned,sameInode,writableAlias,observed;} alias;
                                      std::memset(&alias,0,sizeof(alias));const auto& original=payload.ingressAliases_[n];
                                      if(!payload.ingressOriginal_||original.ordinal>=payload.ingressOriginal_->OriginalDescriptorCount()||
                                         original.ordinal>=payload.ingressOriginal_->OriginalDescriptors().size()||
                                         original.descriptorOriginal!=&payload.ingressOriginal_->OriginalDescriptors()[original.ordinal]){
                                          unitSpanValid=false;return;
                                      }
                                      const auto& descriptor=*original.descriptorOriginal;
                                      alias.ordinal=original.ordinal;alias.ingress=payload.ingressOriginal_;
                                      alias.descriptor=original.descriptorOriginal;alias.clock=original.clock;
                                      alias.descriptorNumber=descriptor.original;alias.status=descriptor.status;
                                      alias.statClock=descriptor.statClock;alias.statResult=descriptor.statResult;
                                      alias.statError=descriptor.statErrno;alias.statAttempted=descriptor.statAttempted;
                                      alias.statReturned=descriptor.statReturned;alias.closeAttemptedAtCut=descriptor.closeAttempted;
                                      alias.flags=original.flags;alias.error=original.error;alias.attempted=original.attempted;
                                      alias.returned=original.returned;alias.sameInode=original.sameInode;
                                      alias.writableAlias=original.writableAlias;alias.observed=original.observed;
                                      unitAppend(107,&alias,sizeof(alias));
                                  }
                                 for(size_t pass=0;pass<2;++pass){
                                     const auto& digest=payload.wholeDigests_[pass].PrivateDigest();
                                     unitAppend(105,digest.data(),digest.size());
                                 }
                                 if(payload.members_)for(size_t n=0;n<2*payload.memberCount_;++n){
                                     struct MemberView {uint64_t ordinal;template_creator_linux::OriginalClockBoundary clock;
                                          unsigned char digest[32],gitDigest[20];bool started,digestMatched,observed,gitRequired,gitMatched,
                                          edgesBegan,edgesFinished;} member;
                                     std::memset(&member,0,sizeof(member));const auto& original=payload.members_[n];
                                     member.ordinal=n;member.clock=original.finalClock;
                                     std::memcpy(member.digest,original.digest.PrivateDigest().data(),sizeof(member.digest));
                                     std::memcpy(member.gitDigest,original.gitDigest.Digest().data(),sizeof(member.gitDigest));
                                     member.gitRequired=original.gitRequired;member.gitMatched=original.gitMatched;
                                      member.edgesBegan=original.edgesBegan;member.edgesFinished=original.edgesFinished;
                                     member.started=original.started;member.digestMatched=original.digestMatched;member.observed=original.observed;
                                     unitAppend(106,&member,sizeof(member));
                                      unitAppend(108,&original.edgeState,sizeof(original.edgeState));
                                 }
                                 // Digest/member objects and allocation exceptions remain
                                 // resident in the exact owner, never copied as opaque C++ bytes.
                                 for(size_t n=0;n<payload.allocations_.size();++n){
                                     struct AllocationView {uint64_t ordinal,bytes;const void* output;
                                         bool attempted,returned,threw;} allocation;
                                     std::memset(&allocation,0,sizeof(allocation));const auto& original=payload.allocations_[n];
                                     allocation.ordinal=n;allocation.bytes=original.bytes;allocation.output=original.output;
                                     allocation.attempted=original.attempted;allocation.returned=original.returned;allocation.threw=original.threw;
                                     unitAppend(104,&allocation,sizeof(allocation));
                                 }
                             }
                        };
                        // First walk validates every retained prefix and checks
                        // exact frame+payload storage before this unit's first write.
                        unitGraph();
                        if(!unitSpanValid||!finalOutput.CanAppendFinalSpanOriginal(unitSpan))copied=false;
                        else{unitEmit=true;unitGraph();}
                    }
                    if(cut->file){
                        if(cut->file->AssociationSourceOriginal()){
                            const auto& file=*cut->file;
                            if(!file.AssociationObserved()||file.AssociationRecordCount()!=file.AssociationRecordCapacity()||
                               file.AssociationRecordCount()>SIZE_MAX/sizeof(struct slcli_source_association_record))copied=false;
                            else{
                                append(60,cutOrdinal,&file.AssociationParserOriginal(),sizeof(file.AssociationParserOriginal()));
                                append(61,cutOrdinal,file.AssociationRecordsOriginal(),
                                    file.AssociationRecordCount()*sizeof(struct slcli_source_association_record));
                            }
                        }
                        append(41,cutOrdinal,cut->payload,cut->payloadBytes);
                        if(cut->readCount>SIZE_MAX/sizeof(template_creator_linux::OriginalReceivedSubjectReadback::NativeOriginal))copied=false;
                        else append(42,cutOrdinal,cut->reads,cut->readCount*sizeof(template_creator_linux::OriginalReceivedSubjectReadback::NativeOriginal));
                        if(!cut->before||!cut->eof||!cut->after||!cut->verificationClock)copied=false;
                        else{
                            append(45,cutOrdinal,cut->before,sizeof(*cut->before));
                            append(46,cutOrdinal,cut->eof,sizeof(*cut->eof));
                            append(47,cutOrdinal,cut->after,sizeof(*cut->after));
                            append(48,cutOrdinal,cut->verificationClock,sizeof(*cut->verificationClock));
                        }
                    }
                }
            }
            append(28,ordinal,&owner.prelaunchOriginal,sizeof(owner.prelaunchOriginal));
            if(owner.interpreterObservations)
                append(37,ordinal,owner.interpreterObservations.get(),owner.count*sizeof(LinuxInterpreterRequirementOriginal));
            if(owner.receivedSubjectAssociations){
                if(owner.count>SIZE_MAX/sizeof(LinuxReceivedSubjectAssociationOriginal))copied=false;
                else append(59,ordinal,owner.receivedSubjectAssociations.get(),owner.count*sizeof(LinuxReceivedSubjectAssociationOriginal));
            }
            if(owner.gate)copied=static_cast<OriginalLinuxSessionGate*>(owner.gate.get())->CaptureOriginals(finalOutput.sink)&&copied;
            if(owner.cwd){append(22,ordinal,owner.cwd.get(),sizeof(*owner.cwd));
                append(23,ordinal,owner.cwd->PrivateOriginalPath(),owner.cwd->OriginalPathBytes()+1);}
            for(size_t i=0;i<owner.count;++i)if(owner.files&&owner.files[i]){
                const auto& file=*owner.files[i];append(24,ordinal,&file,sizeof(file));
                append(25,ordinal,file.PrivateOriginalPath(),file.OriginalPathBytes()+1);
                if(file.RetainedByteCount()>file.BodyCapacity()||file.ReadCount()>SIZE_MAX/sizeof(template_creator_linux::HeldFileCall)){
                    copied=false;continue;
                }
                append(26,ordinal,file.PrivateBody(),file.RetainedByteCount());
                append(27,ordinal,file.PrivateReads(),file.ReadCount()*sizeof(template_creator_linux::HeldFileCall));
            }
        }
        finalPublicationFailed=!copied;
        // Start only after last append. Later collector/join/close originals
        // remain resident and are not asserted to exist in this earlier file.
        if(!finalOutput.StartFinalReadbackAfterLastAppend())finalPublicationFailed=true;
    }catch(...){finalPublicationOriginal=std::current_exception();finalPublicationFailed=true;}
    if(finalPublicationFailed)qualificationFailed=true;
    finalPublicationReturned=true;return true; // Calls returned, not readback/disposal/qualification.
}
}
#endif

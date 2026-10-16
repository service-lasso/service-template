// UNRUN allocation-fault source regression. No native capture/thread/FD effects.
// Only this test executable replaces global allocation; production is unchanged.
#include "linux_pending_host.hpp"
#include "native_linux_platform.hpp"
#include "linux_initial_provider_process_association.hpp"
#include "linux_received_subject_readback.hpp"
#include "linux_received_source_unit.hpp"
#include "linux_unit_payload_archive.hpp"
#include <cstring>
#include <cstddef>
#include <cstdlib>
#include <new>
namespace {
size_t allocationOrdinal=0,failOrdinal=0;
void* TestAllocate(size_t bytes){
    if(failOrdinal&&++allocationOrdinal==failOrdinal)throw std::bad_alloc();
    if(void* p=std::malloc(bytes?bytes:1))return p;
    throw std::bad_alloc();
}
}
void* operator new(size_t n){return TestAllocate(n);}
void* operator new[](size_t n){return TestAllocate(n);}
void operator delete(void* p) noexcept{std::free(p);}
void operator delete[](void* p) noexcept{std::free(p);}
void operator delete(void* p,size_t) noexcept{std::free(p);}
void operator delete[](void* p,size_t) noexcept{std::free(p);}
namespace template_creator_linux {
bool VerifySurvivingPendingHostPreownRegressionOriginal(){
    PendingInputGeometry g{};
    g.clockRows=8;g.taskObservations=1;g.statBytes=64;g.statusBytes=64;g.procReads=2;
    g.fdinfoBytes=64;g.supervisorStack=65536;g.supervisorJoins=2;
    g.receiverStack=65536;g.receiverWaits=2;g.receiverJoins=2;
    g.mapsBytes=64;g.mapsReads=2;g.memoryRegions=2;g.memoryBytes=128;g.memoryReads=2;
    bool hostOverlapExercised=false,processOverlapExercised=false,rangeOverlapExercised=false,payloadOverlapExercised=false;
    for(size_t failure:{size_t{2},size_t{6}}){
        auto* host=new OriginalLinuxPendingHost(DeferredOriginalPreown,g,false,0,65536,2);
        allocationOrdinal=0;failOrdinal=failure;
        const bool accepted=host->PreownOriginal();
        failOrdinal=0;
        auto& pending=host->OriginalPendingOwner();
        bool okay=!accepted&&!host->PreownedOriginal()&&bool(host->PreownExceptionOriginal())&&
            pending.OriginalDeadlineOwner().OriginalCapacity()==g.clockRows&&
            !pending.OriginalSupervisorOwner().PreownedOriginal()&&
            !pending.OriginalSupervisorOwner().HasPendingOriginalNativeCall();
        OriginalLinuxPendingHost::RetainedPreownBackingView backing;
        std::memset(&backing,0xa5,sizeof(backing));
        okay=okay&&host->ObserveRetainedPreownBackingOriginal(backing)&&backing.hostOriginal==host&&
            backing.pendingOriginal==&pending&&backing.clockCapacity==g.clockRows&&!backing.preowned&&
            backing.exceptionOriginal&&bool(*backing.exceptionOriginal)&&backing.supervisorJoins==0&&backing.receiverWaits==0;
        const auto* clocks=pending.OriginalDeadlineOwner().PrivateOriginals();
        if(clocks&&pending.OriginalDeadlineOwner().OriginalCapacity()*sizeof(OriginalClockBoundary)>=sizeof(backing)){
            unsigned char preserved[sizeof(backing)];std::memcpy(preserved,clocks,sizeof(preserved));
            hostOverlapExercised=true;
            auto& overlap=*reinterpret_cast<OriginalLinuxPendingHost::RetainedPreownBackingView*>(const_cast<OriginalClockBoundary*>(clocks));
            okay=okay&&!host->ObserveRetainedPreownBackingOriginal(overlap)&&!std::memcmp(preserved,clocks,sizeof(preserved));
        }
        const auto& task=pending.OriginalCreatorTaskOwner();
        if(failure==2)okay=okay&&task.AllocatedObservationCapacityOriginal()==0&&task.BornObservationCountOriginal()==0;
        else{
            const auto* row=task.AllocatedObservationOriginal(0);
            okay=okay&&task.AllocatedObservationCapacityOriginal()==1&&task.BornObservationCountOriginal()==1&&
                row&&row->stat.bytes&&row->stat.reads&&!row->status.bytes&&!row->preowned;
        }
        // Begin refuses before host-channel/native effects on incomplete owner.
        okay=okay&&!host->BeginOriginal(nullptr,0,nullptr,0,-1);
        // No native owner was acquired: retire only this fixture's storage once.
        delete host;
        if(!okay)return false;
    }
    // Actual process owner's task stat/status and fdinfo byte/read boundaries.
    // Receiver storage is born before fault injection; no endpoint is acquired.
    auto* receiver=new OriginalInitialProviderReceiver(3,nullptr,nullptr);
    auto* readbackHost=new OriginalLinuxPendingHost(DeferredOriginalPreown,g,false,0,65536,2);
    auto& deadline=readbackHost->OriginalPendingOwner().OriginalDeadlineOwner();
    auto& supervisor=readbackHost->OriginalPendingOwner().OriginalSupervisorOwner();
    for(size_t failure=3;failure<=8;++failure){
        auto* process=new OriginalInitialProviderProcessAssociation(*receiver,1,64,64,4,64);
        allocationOrdinal=0;failOrdinal=failure;
        const bool accepted=process->PreownStorageOriginal();failOrdinal=0;
        const auto firstException=process->PreownExceptionOriginal();
        const auto* row=process->OriginalCallerTask().AllocatedObservationOriginal(0);
        const auto& fdinfo=process->OriginalFdinfo();size_t allocated=0;
        bool okay=!accepted&&process->PreownReturnedOriginal()&&!process->PreownedOriginal()&&
            bool(firstException)&&row&&process->AllocatedBackingBytesOriginal(allocated)&&
            allocated<process->SourceBackingBytes()&&!process->KernelCorrespondenceObserved();
        okay=okay&&bool(row->stat.bytes)==(failure>3)&&bool(row->stat.reads)==(failure>4)&&
            bool(row->status.bytes)==(failure>5)&&bool(row->status.reads)==(failure>6)&&
            bool(fdinfo.bytes)==(failure>7)&&!fdinfo.reads;
        // One-shot refusal preserves the actual first exception and prefix.
        okay=okay&&!process->PreownStorageOriginal()&&process->PreownExceptionOriginal()==firstException;
        OriginalInitialProviderProcessAssociation::RetainedPreownBackingView backing;
        std::memset(&backing,0x5a,sizeof(backing));
        okay=okay&&process->DisposeBeforeSubjectExposureOriginal(supervisor)&&
            process->ObserveRetainedPreownBackingOriginal(backing)&&backing.ownerOriginal==process&&
            backing.bornTasks==1&&backing.taskCapacity==1&&backing.allocatedBackingBytes==allocated&&
            backing.returned&&!backing.preowned&&backing.disposed;
        if(row->stat.reads&&row->stat.readCapacity*sizeof(ProcReadOriginal)>=sizeof(backing)){
            unsigned char preserved[sizeof(backing)];std::memcpy(preserved,row->stat.reads.get(),sizeof(preserved));
            processOverlapExercised=true;
            auto& overlap=*reinterpret_cast<OriginalInitialProviderProcessAssociation::RetainedPreownBackingView*>(row->stat.reads.get());
            okay=okay&&!process->ObserveRetainedPreownBackingOriginal(overlap)&&
                !std::memcmp(preserved,row->stat.reads.get(),sizeof(preserved));
        }
        delete process;
        if(!okay){delete readbackHost;delete receiver;return false;}
    }
    // Actual received A owner: body survives read-row failure; body+rows
    // survive parsed-association-array failure. No read/capture/native effect.

    slcli_bq_provider_expectations source{};
    source.source_unit_record_count=1;source.association_index_record_count=1;
    source.association_index_names_bytes=1;source.association_index_bytes=449;
    unsigned char expected[32]{};
    for(size_t failure=1;failure<=3;++failure){
        auto* readback=new OriginalReceivedSubjectReadback(*receiver,449,expected,2,deadline,supervisor,&source);
        allocationOrdinal=0;failOrdinal=failure;
        const bool accepted=readback->PreownStorageOriginal();failOrdinal=0;
        const auto first=readback->PreownExceptionOriginal();size_t actual=0;
        bool okay=!accepted&&readback->PreownReturnedOriginal()&&!readback->PreownedOriginal()&&bool(first)&&
            readback->AllocatedBodyBytesOriginal()==(failure>1?449:0)&&
            readback->AllocatedReadCapacityOriginal()==(failure>2?2:0)&&!readback->AssociationRecordCapacity()&&
            readback->AllocatedBackingBytesOriginal(actual)&&actual<readback->SourceBackingBytes()&&
            !readback->PreownStorageOriginal()&&readback->PreownExceptionOriginal()==first;
        OriginalReceivedSubjectReadback::RetainedPreownBackingView backing;
        std::memset(&backing,0xa5,sizeof(backing));
        okay=okay&&readback->ReleaseBeforeSubjectExposureOriginal()&&readback->ObserveRetainedPreownBackingOriginal(backing)&&
            backing.ownerOriginal==readback&&backing.bodyCapacity==(failure>1?449:0)&&
            backing.readCapacity==(failure>2?2:0)&&!backing.readCount&&!backing.retainedBytes&&
            !backing.associationCapacity&&!backing.parsedAssociations&&backing.returned&&!backing.preowned&&backing.borrowReleased;
        if(readback->body_&&readback->allocatedBodyBytes_>=sizeof(backing)){
            unsigned char preserved[sizeof(backing)];std::memcpy(preserved,readback->body_.get(),sizeof(preserved));
            auto& overlap=*reinterpret_cast<OriginalReceivedSubjectReadback::RetainedPreownBackingView*>(readback->body_.get());
            okay=okay&&!readback->ObserveRetainedPreownBackingOriginal(overlap)&&
                !std::memcmp(preserved,readback->body_.get(),sizeof(preserved));
        }
        delete readback;if(!okay){delete readbackHost;delete receiver;return false;}
    }
    source.source_unit_bytes=193;
    slcli_bq_expected_subject subject{};
    subject.source_occurrence.reader_class=SLCLI_BQ_EXPECTED_UNIT_PREREQUISITE;
    subject.source_occurrence.native_file_bytes=193;
    for(size_t failure=1;failure<=7;++failure){
        auto* unit=new OriginalReceivedSourceUnitObservation(source,subject,*receiver,2,deadline,supervisor);
        allocationOrdinal=0;failOrdinal=failure;
        const bool accepted=unit->PreownStorageOriginal();failOrdinal=0;
        const auto first=unit->PreownExceptionOriginal();size_t actual=0;
        bool okay=!accepted&&unit->preownReturned_&&!unit->preowned_&&bool(first)&&
            bool(unit->header_)==(failure>1)&&bool(unit->table_)==(failure>4)&&!unit->member_&&!unit->records_&&
            unit->AllocatedBackingBytesOriginal(actual)&&actual<unit->SourceBackingBytes();
        if(unit->header_)okay=okay&&bool(unit->header_->bytes_)==(failure>2)&&bool(unit->header_->reads_)==(failure>3);
        if(unit->table_)okay=okay&&bool(unit->table_->bytes_)==(failure>5)&&bool(unit->table_->reads_)==(failure>6);
        okay=okay&&!unit->PreownStorageOriginal()&&unit->PreownExceptionOriginal()==first&&
            unit->ReleaseBeforeExposureOriginal()&&unit->BorrowerReleasedOriginal();
        OriginalReceivedSourceUnitObservation::RetainedPreownBackingView backing;
        std::memset(&backing,0xa5,sizeof(backing));
        okay=okay&&unit->ObserveRetainedPreownBackingOriginal(backing)&&backing.ownerOriginal==unit&&
            backing.ranges[0].ownerOriginal==unit->header_.get()&&backing.ranges[1].ownerOriginal==unit->table_.get()&&
            backing.ranges[2].ownerOriginal==unit->member_.get()&&!backing.recordCapacity&&!backing.parsedRecords&&
            backing.allocatedBackingBytes==actual&&backing.returned&&!backing.preowned&&backing.released;
        if(unit->table_&&unit->table_->reads_&&unit->table_->allocatedReads_*sizeof(OriginalReceivedSourceUnitRange::NativeOriginal)>=sizeof(backing)){
            unsigned char preserved[sizeof(backing)];std::memcpy(preserved,unit->table_->reads_.get(),sizeof(preserved));
            auto& overlap=*reinterpret_cast<OriginalReceivedSourceUnitObservation::RetainedPreownBackingView*>(unit->table_->reads_.get());
            okay=okay&&!unit->ObserveRetainedPreownBackingOriginal(overlap)&&
                !std::memcmp(preserved,unit->table_->reads_.get(),sizeof(preserved));
        }
        if(unit->header_){
            unsigned char saved[sizeof(backing)];std::memcpy(saved,&backing,sizeof(saved));
            const bool released=unit->header_->borrowReleased_;
            unit->header_->borrowReleased_=false;
            okay=okay&&!unit->ObserveRetainedPreownBackingOriginal(backing)&&!std::memcmp(saved,&backing,sizeof(backing));
            unit->header_->borrowReleased_=released;
        }
        for(auto* range:{unit->header_.get(),unit->table_.get(),unit->member_.get()}){
            if(!range||!range->reads_||range->allocatedReads_*sizeof(OriginalReceivedSourceUnitRange::NativeOriginal)<
                sizeof(OriginalReceivedSourceUnitRange::RetainedPreownBackingView))continue;
            using View=OriginalReceivedSourceUnitRange::RetainedPreownBackingView;
            unsigned char preserved[sizeof(View)];std::memcpy(preserved,range->reads_.get(),sizeof(preserved));
            rangeOverlapExercised=true;
            auto& overlap=*reinterpret_cast<View*>(range->reads_.get());
            okay=okay&&!range->ObserveRetainedPreownBackingOriginal(overlap)&&
                !std::memcmp(preserved,range->reads_.get(),sizeof(preserved));
        }
        delete unit;if(!okay){delete readbackHost;delete receiver;return false;}
    }
    // Optional SP1 member range: exact source offset192/length1 in unit193.
    subject.source_occurrence.reader_class=SLCLI_BQ_EXPECTED_SP1_MEMBER_RANGE;
    subject.source_occurrence.read_offset=192;subject.source_occurrence.read_bytes=1;
    subject.source_record_index=0;
    for(size_t failure=1;failure<=10;++failure){
        auto* unit=new OriginalReceivedSourceUnitObservation(source,subject,*receiver,2,deadline,supervisor);
        allocationOrdinal=0;failOrdinal=failure;
        const bool accepted=unit->PreownStorageOriginal();failOrdinal=0;
        const auto first=unit->PreownExceptionOriginal();size_t actual=0;
        bool okay=!accepted&&unit->preownReturned_&&!unit->preowned_&&bool(first)&&
            bool(unit->header_)==(failure>1)&&bool(unit->table_)==(failure>4)&&
            bool(unit->member_)==(failure>7)&&!unit->records_&&
            unit->AllocatedBackingBytesOriginal(actual)&&actual<unit->SourceBackingBytes();
        if(unit->header_)okay=okay&&bool(unit->header_->bytes_)==(failure>2)&&bool(unit->header_->reads_)==(failure>3);
        if(unit->table_)okay=okay&&bool(unit->table_->bytes_)==(failure>5)&&bool(unit->table_->reads_)==(failure>6);
        if(unit->member_)okay=okay&&bool(unit->member_->bytes_)==(failure>8)&&bool(unit->member_->reads_)==(failure>9)&&
            unit->member_->offset_==192&&unit->member_->extent_==1&&unit->member_->unitBytes_==193;
        okay=okay&&!unit->PreownStorageOriginal()&&unit->PreownExceptionOriginal()==first&&
            unit->ReleaseBeforeExposureOriginal()&&unit->BorrowerReleasedOriginal();
        OriginalReceivedSourceUnitObservation::RetainedPreownBackingView backing;
        std::memset(&backing,0xa5,sizeof(backing));
        okay=okay&&unit->ObserveRetainedPreownBackingOriginal(backing)&&backing.ownerOriginal==unit&&
            backing.ranges[0].ownerOriginal==unit->header_.get()&&backing.ranges[1].ownerOriginal==unit->table_.get()&&
            backing.ranges[2].ownerOriginal==unit->member_.get()&&!backing.recordCapacity&&!backing.parsedRecords&&
            backing.allocatedBackingBytes==actual&&backing.returned&&!backing.preowned&&backing.released;
        if(unit->table_&&unit->table_->reads_&&unit->table_->allocatedReads_*sizeof(OriginalReceivedSourceUnitRange::NativeOriginal)>=sizeof(backing)){
            unsigned char preserved[sizeof(backing)];std::memcpy(preserved,unit->table_->reads_.get(),sizeof(preserved));
            auto& overlap=*reinterpret_cast<OriginalReceivedSourceUnitObservation::RetainedPreownBackingView*>(unit->table_->reads_.get());
            okay=okay&&!unit->ObserveRetainedPreownBackingOriginal(overlap)&&
                !std::memcmp(preserved,unit->table_->reads_.get(),sizeof(preserved));
        }
        if(unit->header_){
            unsigned char saved[sizeof(backing)];std::memcpy(saved,&backing,sizeof(saved));
            const bool released=unit->header_->borrowReleased_;
            unit->header_->borrowReleased_=false;
            okay=okay&&!unit->ObserveRetainedPreownBackingOriginal(backing)&&!std::memcmp(saved,&backing,sizeof(backing));
            unit->header_->borrowReleased_=released;
        }
        for(auto* range:{unit->header_.get(),unit->table_.get(),unit->member_.get()}){
            if(!range||!range->reads_||range->allocatedReads_*sizeof(OriginalReceivedSourceUnitRange::NativeOriginal)<
                sizeof(OriginalReceivedSourceUnitRange::RetainedPreownBackingView))continue;
            using View=OriginalReceivedSourceUnitRange::RetainedPreownBackingView;
            unsigned char preserved[sizeof(View)];std::memcpy(preserved,range->reads_.get(),sizeof(preserved));
            rangeOverlapExercised=true;
            auto& overlap=*reinterpret_cast<View*>(range->reads_.get());
            okay=okay&&!range->ObserveRetainedPreownBackingOriginal(overlap)&&
                !std::memcmp(preserved,range->reads_.get(),sizeof(preserved));
        }
        delete unit;if(!okay){delete readbackHost;delete receiver;return false;}
    }
    subject.source_occurrence.reader_class=SLCLI_BQ_EXPECTED_UNIT_PREREQUISITE;
    auto* payloadUnit=new OriginalReceivedSourceUnitObservation(source,subject,*receiver,2,deadline,supervisor);
    if(!payloadUnit->PreownStorageOriginal()){delete payloadUnit;delete readbackHost;delete receiver;return false;}
    for(size_t failure=1;failure<=4;++failure){
        auto* payload=new OriginalUnitPayloadArchive(*payloadUnit,deadline,supervisor);
        allocationOrdinal=0;failOrdinal=failure;
        const bool accepted=payload->PreownStorageOriginal();failOrdinal=0;
        OriginalUnitPayloadArchive::RetainedPreownBackingView backing;
        std::memset(&backing,0x5a,sizeof(backing));size_t actual=0;
        bool okay=!accepted&&payload->ReleaseBeforeExposureOriginal()&&payload->ObserveRetainedPreownBackingOriginal(backing)&&
            backing.ownerOriginal==payload&&backing.unitOriginal==payloadUnit&&backing.allocationCount==4&&
            bool(backing.rowsOriginal)==(failure>1)&&bool(backing.membersOriginal)==(failure>2)&&
            bool(backing.windowOriginal)==(failure>3)&&!backing.edgesOriginal&&!backing.rowCount&&!backing.edgeCount&&
            backing.attempted&&!backing.preowned&&backing.released&&payload->AllocatedBackingBytesOriginal(actual)&&
            actual==backing.allocatedBackingBytes&&actual<payload->SourceBackingBytes()&&
            payload->allocations_[failure-1].threw&&bool(payload->allocations_[failure-1].exceptionOriginal)&&
            !payload->PreownStorageOriginal();
        if(payload->rows_&&payload->rowCapacity_*sizeof(OriginalUnitPayloadArchive::NativeOriginal)>=sizeof(backing)){
            unsigned char preserved[sizeof(backing)];std::memcpy(preserved,payload->rows_.get(),sizeof(preserved));
            payloadOverlapExercised=true;
            auto& overlap=*reinterpret_cast<OriginalUnitPayloadArchive::RetainedPreownBackingView*>(payload->rows_.get());
            okay=okay&&!payload->ObserveRetainedPreownBackingOriginal(overlap)&&
                !std::memcmp(preserved,payload->rows_.get(),sizeof(preserved));
        }
        delete payload;if(!okay){delete payloadUnit;delete readbackHost;delete receiver;return false;}
    }
    delete payloadUnit;
    for(const auto mode:{OriginalLinuxHeldFile::CaptureMode::CompleteFile,OriginalLinuxHeldFile::CaptureMode::SourceUnitIdentity}){
        const size_t last=mode==OriginalLinuxHeldFile::CaptureMode::CompleteFile?3:2;
        for(size_t failure=1;failure<=last;++failure){
            auto* held=new OriginalLinuxHeldFile(DeferredOriginalPreown,"/x",2,1,expected,2,deadline,supervisor,mode);
            allocationOrdinal=0;failOrdinal=failure;
            const bool accepted=held->PreownStorageOriginal();failOrdinal=0;
            const auto first=held->PreownExceptionOriginal();
            const bool okay=!accepted&&held->PreownReturnedOriginal()&&!held->PreownedOriginal()&&bool(first)&&
                held->AllocatedPathBytesOriginal()==(failure>1?3:0)&&
                held->AllocatedBodyBytesOriginal()==(mode==OriginalLinuxHeldFile::CaptureMode::CompleteFile&&failure>2?1:0)&&
                held->OriginalReadCapacity()==0&&!held->Capture()&&
                !held->PreownStorageOriginal()&&held->PreownExceptionOriginal()==first;
            delete held;if(!okay){delete readbackHost;delete receiver;return false;}
        }
    }
    delete readbackHost;delete receiver;
    // Actual platform Preown failure plus SAME production final-view consumer.
    auto* platform=new template_creator_bridge::NativePlatformState(
        template_creator_bridge::OriginalPhase::NativeHelper,1,128,16,g,false,0,65536,2);
    allocationOrdinal=0;failOrdinal=2;
    const bool platformAccepted=platform->PreownStorageOriginal();failOrdinal=0;
    template_creator_bridge::NativePlatformState::RetainedPreownView first,second;
    std::memset(&first,0xa5,sizeof(first));std::memset(&second,0x5a,sizeof(second));
    bool stable=!platformAccepted&&platform->FailedPreownSourceFrontierOriginal()==
        template_creator_bridge::NativePlatformState::FailedPreownHostFrontier::OwnedBeforeBegin&&
        platform->ObserveRetainedPreownViewOriginal(first)&&
        platform->ObserveRetainedPreownViewOriginal(second)&&!std::memcmp(&first,&second,sizeof(first));
    const auto* platformClocks=platform->pending.OriginalDeadlineOwner().PrivateOriginals();
    if(platformClocks&&platform->pending.OriginalDeadlineOwner().OriginalCapacity()*sizeof(OriginalClockBoundary)>=sizeof(first)){
        unsigned char preserved[sizeof(first)];std::memcpy(preserved,platformClocks,sizeof(preserved));
        auto& overlap=*reinterpret_cast<template_creator_bridge::NativePlatformState::RetainedPreownView*>(const_cast<OriginalClockBoundary*>(platformClocks));
        stable=stable&&!platform->ObserveRetainedPreownViewOriginal(overlap)&&
            !std::memcmp(preserved,platformClocks,sizeof(preserved));
    }else stable=false; // The regression must actually reach the dynamic overlap.
    // Explicit serialized tail-gap assertion; differing initial byte patterns
    // above also detect any untouched internal representation gap.
    const auto* raw=reinterpret_cast<const unsigned char*>(&first);
    constexpr size_t tail=offsetof(template_creator_bridge::NativePlatformState::RetainedPreownView,adoptedHost)+sizeof(bool);
    for(size_t i=tail;i<sizeof(first);++i)if(raw[i])stable=false;
    stable=stable&&first.attempted&&first.returned&&!first.preowned&&!first.inputOwnersOriginal&&
        first.allocatedInputs==0&&first.bornCommands==0&&first.exceptionOriginal&&bool(*first.exceptionOriginal);
    stable=stable&&platform->ObserveFailedPreownDispositionOriginal()&&platform->AllPhysicalOriginalsDisposed()&&
        platform->SameRetainedFailedPreownViewOriginal(&platform->failedPreownRetainedView)&&platform->qualificationFailed&&
        !platform->finalPublicationAttempted&&!platform->collectorStartAttempted;
    platform->transferAttempted=true;
    stable=stable&&platform->FailedPreownSourceFrontierOriginal()==
        template_creator_bridge::NativePlatformState::FailedPreownHostFrontier::None;
    // This readonly-predicate mutation is synthetic; no transfer was performed.
    platform->transferAttempted=false;
    delete platform;
    return stable&&hostOverlapExercised&&processOverlapExercised&&rangeOverlapExercised&&payloadOverlapExercised;
}
}
int main(){return template_creator_linux::VerifySurvivingPendingHostPreownRegressionOriginal()?0:1;}

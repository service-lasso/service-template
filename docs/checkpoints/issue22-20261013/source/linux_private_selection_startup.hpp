// UNCOMPILED source-owned pre-interpreter acquisition and SAME host handoff.
// Caller native capabilities/profile remain independent authentic inputs.
#pragma once
#ifndef _WIN32
#include "linux_private_selection_decoder.hpp"
#include "linux_upstream_subject_receiver.hpp"
#include "linux_initial_provider_process_association.hpp"
#include "linux_received_subject_readback.hpp"
#include "linux_initial_provider_archive.hpp"
#include "linux_source_occurrence_preflight.hpp"
#include "linux_received_source_unit_range.hpp"
#include "linux_unit_payload_archive.hpp"
#include "linux_receipt_foundation_pair.hpp"
#include "linux_prebirth_retention_policy.hpp"
namespace template_creator_bridge {
struct OriginalLinuxSelectionStartupInput {
    template_creator_linux::PendingInputGeometry geometry{};
    bool hasDeadline=false;std::uint64_t absoluteNanoseconds=0;
    std::size_t callerStack=0,callerJoins=0;
    int procRoot=-1;
    int initialProviderEndpoint=-1;
    std::size_t initialProviderOfferCount=0;
    // SAME-realm reviewed source geometry only. Never decoded from argv,
    // Python or a parent-process pointer. The actual local resolver is missing.
    const slcli_bq_provider_expectations* sourceExpectationsOriginal=nullptr;
    const slcli_bq_destination_retention_constraints* retentionConstraintsOriginal=nullptr;
    std::vector<unsigned char> ingressBody;
    std::vector<int> capabilities;
    std::string selectionPath;
    std::array<unsigned char,32> selectionSha256{};
    std::size_t selectionBytes=0,selectionReadRows=0;
};
class OriginalLinuxSelectionStartup {
    OriginalLinuxSelectionStartupInput original_;
    bool InitialProviderAcquiredOriginalsQualified() const noexcept {
        // Count/shape/custody and zero-offer physical disposal cannot qualify
        // this graph. Native candidate readback/ACK is not genuine authenticated
        // producer/image/entry/source/member acquisition or final actual
        // borrower/archive/closure qualification. Fixed whole consumer remains
        // unfinished and explicitly closed.
        return false;
    }
    ~OriginalLinuxSelectionStartup()=default;
public:
    template_creator_linux::OriginalLinuxPendingHost* host=nullptr;
    template_creator_linux::OriginalLinuxHeldFile* selection=nullptr;
    OriginalLinuxSelectionDecoder* decoder=nullptr;
    template_creator_linux::OriginalInitialProviderReceiver* initialProvider=nullptr;
    template_creator_linux::OriginalInitialProviderProcessAssociation* initialProviderProcess=nullptr;
    template_creator_linux::OriginalInitialProviderArchive* initialProviderArchive=nullptr;
    template_creator_linux::OriginalReceiptFoundationPair* receiptFoundationPair=nullptr;
    bool receiptFoundationPairDisposed=false;
    template_creator_linux::OriginalLinuxPrebirthRetentionPolicy* retentionPolicy=nullptr;
    bool retentionPolicyDisposed=false;
    std::unique_ptr<template_creator_linux::OriginalReceivedSubjectReadback*[]> subjectReadbacks;
    std::unique_ptr<template_creator_linux::OriginalReceivedSourceUnitObservation*[]> subjectUnits;
    std::unique_ptr<template_creator_linux::OriginalUnitPayloadArchive*[]> unitPayloads;
    // Constructed is the enrolled construction prefix, including the final
    // null/partial slot on exception; each actual pointer remains separate.
    std::size_t subjectReadbackCapacity=0,subjectReadbacksConstructed=0;
    bool subjectReadbackPreownAttempted=false,subjectReadbacksPreowned=false;
    bool processCorrespondenceAttempted=false,processCorrespondenceObserved=false,processAssociationDisposed=false;
    std::size_t decodedOfferCountOriginal=0;
    bool offerCountCompared=false,offerCountMatched=false;
    OriginalLinuxSourceOccurrencePreflight sourceOccurrencePreflight;
    std::size_t sourceCommandCountOriginal=0;
    bool processReferenceCandidateAttempted=false,processReferenceCandidateObserved=false;
    bool processReadbackAckAttempted=false,processReadbackAckObserved=false,subjectLoopAttempted=false;
    std::size_t subjectReadbacksObserved=0,subjectReadbackAcksObserved=0;
    bool initialProviderDisposed=false;
    std::exception_ptr preownOriginal;
    bool preownAttempted=false,preowned=false,beginAttempted=false;
    bool hostBeginAttempted=false,hostBegan=false,captureAttempted=false,captured=false;
    bool decodeAttempted=false,decoded=false,invocationAttempted=false;
    OriginalLinuxInvocation* invocation=nullptr;
    bool startupAuxiliaryDisposed=false,startupObserversDisposed=false;
    bool ingressDisposalAttempted=false,ingressDisposalResult=false;
    bool physicallyDisposed=false,failed=false;
    bool dispositionRequested=false;
    struct LocalSettlementOriginal {
        const OriginalLinuxSelectionStartup* owner=nullptr;
        bool published=false,sourceOriginalsSettled=false,qualificationFailed=true;
        const OriginalLinuxInvocation* invocationOriginal=nullptr;
        const NativePlatformState* failedPreownPlatformOriginal=nullptr;
        const NativePlatformState::RetainedPreownView* failedPreownViewOriginal=nullptr;
        const template_creator_linux::OriginalInitialProviderArchive::RetainedFinalView* initialArchiveOriginal=nullptr;
        // Resident, SAME-realm originals include partial acquisitions which
        // never became a completed archive cut. This is not wire serialization.
        const OriginalLinuxSelectionStartupInput* inputOriginal=nullptr;
        const OriginalLinuxSourceOccurrencePreflight* sourcePreflightOriginal=nullptr;
        const template_creator_linux::OriginalInitialProviderReceiver* receiverOriginal=nullptr;
        const template_creator_linux::OriginalInitialProviderProcessAssociation* processOriginal=nullptr;
        const template_creator_linux::OriginalReceiptFoundationPair* receiptFoundationPairOriginal=nullptr;
        bool receiptFoundationPairDisposition=false;
        const template_creator_linux::OriginalLinuxPrebirthRetentionPolicy* retentionPolicyOriginal=nullptr;
        bool retentionPolicyDisposition=false;
        const template_creator_linux::OriginalLinuxPrebirthRetentionPolicy::RetainedOriginalView* retentionPolicyView=nullptr;
        template_creator_linux::OriginalReceivedSubjectReadback* const* fileOwnersOriginal=nullptr;
        template_creator_linux::OriginalReceivedSourceUnitObservation* const* unitOwnersOriginal=nullptr;
        template_creator_linux::OriginalUnitPayloadArchive* const* unitPayloadOwnersOriginal=nullptr;
        const std::exception_ptr* preownExceptionOriginal=nullptr;
        std::size_t subjectCapacity=0,subjectsConstructed=0,subjectsObserved=0,acksObserved=0;
    } localSettlement;
    OriginalLinuxSelectionStartup()=default;
    OriginalLinuxSelectionStartup(const OriginalLinuxSelectionStartup&)=delete;
    OriginalLinuxSelectionStartup& operator=(const OriginalLinuxSelectionStartup&)=delete;
    bool PreownOriginal(const OriginalLinuxSelectionStartupInput& source) noexcept {
        if(dispositionRequested||preownAttempted)return false;preownAttempted=true;
        try{
            if(!source.sourceExpectationsOriginal||!source.retentionConstraintsOriginal||source.initialProviderEndpoint<0||!source.initialProviderOfferCount||source.procRoot<0||source.ingressBody.empty()||
               source.ingressBody.size()>template_creator_linux::OriginalPrivateIngress::BodyCapacity||
               source.capabilities.empty()||source.capabilities.size()>=template_creator_linux::OriginalPrivateIngress::FdCapacity||
               source.selectionPath.empty()||source.selectionPath.find('\0')!=std::string::npos||
               !source.selectionBytes||!source.selectionReadRows||(!source.hasDeadline&&source.absoluteNanoseconds))
                throw std::invalid_argument("original Linux startup source inputs");
            if(!template_creator_linux::OriginalLinuxPrebirthRetentionPolicy::InitialContainingReservationQualifiedOriginal())
                throw std::invalid_argument("genuine initial containing reservation/source original missing");
            original_=source; // Complete native original copies precede effects.
            host=new template_creator_linux::OriginalLinuxPendingHost(template_creator_linux::DeferredOriginalPreown,source.geometry,source.hasDeadline,
                source.absoluteNanoseconds,source.callerStack,source.callerJoins);
            if(!host->PreownOriginal()){
                if(host->PreownExceptionOriginal())std::rethrow_exception(host->PreownExceptionOriginal());
                throw std::invalid_argument("original pending host preownership failed");
            }
            auto& pending=host->OriginalPendingOwner();
            const auto& geometry=original_.geometry;
            const size_t receiverBytes=template_creator_linux::OriginalInitialProviderReceiver::CheckedSourceBacking(original_.initialProviderOfferCount);
            const size_t processBytes=template_creator_linux::OriginalInitialProviderProcessAssociation::CheckedSourceBacking(
                geometry.taskObservations,geometry.statBytes,geometry.statusBytes,geometry.procReads,geometry.fdinfoBytes);
            const size_t archiveBytes=template_creator_linux::OriginalInitialProviderArchive::CheckedSourceBacking(original_.initialProviderOfferCount);
            size_t hostBytes=0;
            const size_t pairBytes=sizeof(template_creator_linux::OriginalReceiptFoundationPair);
            const size_t policyBytes=template_creator_linux::OriginalLinuxPrebirthRetentionPolicy::CheckedStorageOriginal(*source.retentionConstraintsOriginal);
            if(!host->ReservedSourceBytes(hostBytes)||receiverBytes>SIZE_MAX-hostBytes||processBytes>SIZE_MAX-hostBytes-receiverBytes||
               archiveBytes>SIZE_MAX-hostBytes-receiverBytes-processBytes||
               pairBytes>SIZE_MAX-hostBytes-receiverBytes-processBytes-archiveBytes||
               policyBytes>SIZE_MAX-hostBytes-receiverBytes-processBytes-archiveBytes-pairBytes)
                throw std::invalid_argument("initial provider combined source backing");
            initialProvider=new template_creator_linux::OriginalInitialProviderReceiver(
                original_.initialProviderOfferCount,&pending.OriginalDeadlineOwner(),&pending.OriginalSupervisorOwner());
            if(!initialProvider->AttachOriginalInheritedEndpoint(original_.initialProviderEndpoint))
                throw std::invalid_argument("initial provider inherited endpoint custody");
            if(!host->BindInitialProviderBeforeBeginOriginal(initialProvider))
                throw std::invalid_argument("initial provider same host custody association");
            initialProviderProcess=new template_creator_linux::OriginalInitialProviderProcessAssociation(*initialProvider,
                geometry.taskObservations,geometry.statBytes,geometry.statusBytes,geometry.procReads,geometry.fdinfoBytes);
            if(!initialProviderProcess->PreownStorageOriginal())std::rethrow_exception(initialProviderProcess->PreownExceptionOriginal());
            initialProviderArchive=new template_creator_linux::OriginalInitialProviderArchive(*initialProvider,
                original_.initialProviderOfferCount,pending.OriginalDeadlineOwner(),pending.OriginalSupervisorOwner());
            receiptFoundationPair=new template_creator_linux::OriginalReceiptFoundationPair(*initialProvider,
                *original_.sourceExpectationsOriginal,pending.OriginalDeadlineOwner(),pending.OriginalSupervisorOwner());
            if(!host->BindReceiptFoundationPairBeforeBeginOriginal(receiptFoundationPair))
                throw std::invalid_argument("same startup native pair custody association");
            // Surviving owner attaches before its first fallible allocation.
            retentionPolicy=new template_creator_linux::OriginalLinuxPrebirthRetentionPolicy(*original_.retentionConstraintsOriginal,
                pending.OriginalDeadlineOwner(),pending.OriginalSupervisorOwner());
            if(!host->BindRetentionPolicyBeforeBeginOriginal(retentionPolicy))throw std::invalid_argument("same prebirth policy host custody");
            if(!retentionPolicy->PreownStorageOriginal())throw std::invalid_argument("independent installed policy storage");
            selection=new template_creator_linux::OriginalLinuxHeldFile(template_creator_linux::DeferredOriginalPreown,original_.selectionPath.data(),
                original_.selectionPath.size(),original_.selectionBytes,original_.selectionSha256.data(),
                original_.selectionReadRows,pending.OriginalDeadlineOwner(),pending.OriginalSupervisorOwner());
            if(!selection->PreownStorageOriginal())std::rethrow_exception(selection->PreownExceptionOriginal());
            decoder=new OriginalLinuxSelectionDecoder(*selection,original_.procRoot,
                original_.capabilities.data(),original_.capabilities.size(),host);
            preowned=true;return true;
        }catch(...){preownOriginal=std::current_exception();failed=true;return false;}
    }
    bool BeginOriginal() noexcept {
        if(dispositionRequested||beginAttempted||!preowned)return false;beginAttempted=true;
        hostBeginAttempted=true;hostBegan=host->BeginOriginal(original_.ingressBody.data(),original_.ingressBody.size(),
            original_.capabilities.data(),original_.capabilities.size(),original_.procRoot);
        if(!hostBegan){failed=true;return false;}
        // Existing SAME supervisor/caller/receiver is started before policy
        // native effects. Installed-source/initial finance is still closed;
        // absent direct constraint occurrence already refuses in Preown.
        if(!retentionPolicy||host->RetentionPolicyOriginal()!=retentionPolicy||
           !retentionPolicy->SameOriginals(*original_.retentionConstraintsOriginal,host->OriginalPendingOwner().OriginalDeadlineOwner(),
                host->OriginalPendingOwner().OriginalSupervisorOwner())||
           !retentionPolicy->CaptureOriginal()||!retentionPolicy->ReturningCorrespondenceOriginal()||
           !retentionPolicy->AcquiredInstalledPolicyOriginal()){failed=true;return false;}
        captureAttempted=true;captured=selection->Capture();
        if(!captured){failed=true;return false;}
        decodeAttempted=true;decoded=decoder->DecodeOriginal();
        if(!decoded){failed=true;return false;}
        offerCountCompared=true;
        offerCountMatched=decoder->InitialProviderOfferCountOriginal(decodedOfferCountOriginal)&&
            decodedOfferCountOriginal==original_.initialProviderOfferCount;
        if(!offerCountMatched){failed=true;return false;}
        const auto* selectedRoute=decoder->InitialProviderRouteOriginal();
        if(!selectedRoute){failed=true;return false;}
        if(!decoder->InitialProviderCommandCountOriginal(sourceCommandCountOriginal)||
           !sourceOccurrencePreflight.ObserveOriginal(original_.sourceExpectationsOriginal,
               *selectedRoute,sourceCommandCountOriginal)){failed=true;return false;}
        if(!initialProvider->BindBootstrapOriginal(*original_.sourceExpectationsOriginal,
            static_cast<uint32_t>(selectedRoute->phase))){failed=true;return false;}
        auto& pending=host->OriginalPendingOwner();
        subjectReadbackPreownAttempted=true;
        try{
            subjectReadbackCapacity=decodedOfferCountOriginal-1;
            std::size_t total=0;
            constexpr size_t pointerBytes=sizeof(subjectReadbacks[0])+sizeof(subjectUnits[0])+sizeof(unitPayloads[0]);
            if(!ReservedStartupSourceBytes(total)||subjectReadbackCapacity>(SIZE_MAX-total)/pointerBytes)
                throw std::invalid_argument("complete received subject pointer backing");
            total+=subjectReadbackCapacity*pointerBytes;
            // SAME preowned pair/policy are already counted by ReservedStartupSourceBytes.
            size_t sourceOrdinal=0;
            for(std::size_t command=0;command<selectedRoute->requirements.size();++command)
                for(std::size_t requirement=0;requirement<selectedRoute->requirements[command].size();++requirement){
                    const auto& selectedInput=selectedRoute->requirements[command][requirement];std::size_t rows=0;
                    if(RequirementNativeExtent(selectedInput)>SIZE_MAX||
                       (FileRequirement(selectedInput)?!decoder->InitialProviderReadRowsOriginal(command,requirement,rows):
                        !decoder->InitialProviderNativePrerequisiteNoReadRowsOriginal(command,requirement)))
                        throw std::invalid_argument("same decoder received subject geometry");
                    const auto& expected=original_.sourceExpectationsOriginal->subjects[sourceOrdinal++];
                    const auto kind=expected.source_occurrence.reader_class;
                    if(!FileRequirement(selectedInput)){
                        if(rows)throw std::invalid_argument("native archive immutable read rows");
                        continue; // Pair native backing is separately preowned, never file readback.
                    }
                    const auto& input=*FileRequirement(selectedInput);
                    const auto bytes=(kind==SLCLI_BQ_EXPECTED_UNIT_PREREQUISITE||kind==SLCLI_BQ_EXPECTED_SP1_MEMBER_RANGE)?
                        template_creator_linux::OriginalReceivedSourceUnitObservation::CheckedSourceBacking(
                            *original_.sourceExpectationsOriginal,expected,rows):
                        template_creator_linux::OriginalReceivedSubjectReadback::CheckedSourceBacking(
                            static_cast<std::size_t>(input.size),rows,
                            kind==SLCLI_BQ_EXPECTED_A_PREREQUISITE?original_.sourceExpectationsOriginal:nullptr);
                    if(!subjectReadbacks[slot]->PreownStorageOriginal())std::rethrow_exception(subjectReadbacks[slot]->PreownExceptionOriginal());}

                    if(bytes>SIZE_MAX-total)throw std::invalid_argument("complete received subject aggregate");total+=bytes;
                    if(kind==SLCLI_BQ_EXPECTED_UNIT_PREREQUISITE){
                        const size_t payloadBytes=template_creator_linux::OriginalUnitPayloadArchive::CheckedSourceBacking(
                            *original_.sourceExpectationsOriginal);
                        if(payloadBytes>SIZE_MAX-total)throw std::invalid_argument("complete unit payload aggregate");
                        total+=payloadBytes;
                    }
                }
            // Checked requested backing only; independent containing financing
            // and full runtime/allocator/kernel overlap remain required.
            subjectReadbacks=std::make_unique<template_creator_linux::OriginalReceivedSubjectReadback*[]>(subjectReadbackCapacity);
            subjectUnits=std::make_unique<template_creator_linux::OriginalReceivedSourceUnitObservation*[]>(subjectReadbackCapacity);
            unitPayloads=std::make_unique<template_creator_linux::OriginalUnitPayloadArchive*[]>(subjectReadbackCapacity);
            if(!receiptFoundationPair||host->ReceiptFoundationPairOriginal()!=receiptFoundationPair||
               !receiptFoundationPair->SameOriginals(*initialProvider,*original_.sourceExpectationsOriginal,
                   pending.OriginalDeadlineOwner(),pending.OriginalSupervisorOwner()))
                throw std::invalid_argument("same preowned startup native pair custody");
            for(std::size_t command=0;command<selectedRoute->requirements.size();++command)
                for(std::size_t requirement=0;requirement<selectedRoute->requirements[command].size();++requirement){
                    const auto& selectedInput=selectedRoute->requirements[command][requirement];std::size_t rows=0;
                    if(subjectReadbacksConstructed==subjectReadbackCapacity||
                       (FileRequirement(selectedInput)?!decoder->InitialProviderReadRowsOriginal(command,requirement,rows):
                        !decoder->InitialProviderNativePrerequisiteNoReadRowsOriginal(command,requirement)))
                        throw std::invalid_argument("received subject preown count");
                    const auto& expected=original_.sourceExpectationsOriginal->subjects[subjectReadbacksConstructed];
                    const auto kind=expected.source_occurrence.reader_class;
                    // Enroll the slot before either constructor. If payload
                    // allocation throws, its already-owned unit stays counted.
                    const size_t slot=subjectReadbacksConstructed++;
                    if(!FileRequirement(selectedInput)){
                        if(rows)throw std::invalid_argument("native archive immutable read rows");
                        continue;
                    }
                    const auto& input=*FileRequirement(selectedInput);
                    if(kind==SLCLI_BQ_EXPECTED_UNIT_PREREQUISITE||kind==SLCLI_BQ_EXPECTED_SP1_MEMBER_RANGE){
                        subjectUnits[slot]=new template_creator_linux::OriginalReceivedSourceUnitObservation(
                            *original_.sourceExpectationsOriginal,expected,*initialProvider,rows,
                            pending.OriginalDeadlineOwner(),pending.OriginalSupervisorOwner());
                        if(!subjectUnits[slot]->PreownStorageOriginal())std::rethrow_exception(subjectUnits[slot]->PreownExceptionOriginal());
                        if(kind==SLCLI_BQ_EXPECTED_UNIT_PREREQUISITE){
                            unitPayloads[slot]=new template_creator_linux::OriginalUnitPayloadArchive(
                                *subjectUnits[slot],pending.OriginalDeadlineOwner(),pending.OriginalSupervisorOwner());
                            if(!unitPayloads[slot]->PreownStorageOriginal()){
                                failed=true;return false; // Surviving owner retains partial allocation originals.
                            }
                        }
                    }else {subjectReadbacks[slot]=new template_creator_linux::OriginalReceivedSubjectReadback(
                            *initialProvider,static_cast<std::size_t>(input.size),input.sha256.data(),rows,
                            pending.OriginalDeadlineOwner(),pending.OriginalSupervisorOwner(),
                            kind==SLCLI_BQ_EXPECTED_A_PREREQUISITE?original_.sourceExpectationsOriginal:nullptr);
                    if(!subjectReadbacks[slot]->PreownStorageOriginal())std::rethrow_exception(subjectReadbacks[slot]->PreownExceptionOriginal());}

                }
            subjectReadbacksPreowned=subjectReadbacksConstructed==subjectReadbackCapacity;
        }catch(...){preownOriginal=std::current_exception();failed=true;return false;}
        if(!subjectReadbacksPreowned){failed=true;return false;}
        processReferenceCandidateAttempted=true;
        processReferenceCandidateObserved=initialProvider->ReceiveCandidateOriginal({1,0,
            static_cast<uint32_t>(selectedRoute->phase),UINT32_MAX,UINT32_MAX},&pending.OriginalSupervisorOwner());
        if(!processReferenceCandidateObserved){failed=true;return false;}
        processCorrespondenceAttempted=true;
        processCorrespondenceObserved=initialProviderProcess->CaptureOriginal(pending.OriginalCreatorTaskOwner(),
            pending.OriginalCreatorTid(),pending.OriginalDeadlineOwner(),pending.OriginalSupervisorOwner());
        if(!processCorrespondenceObserved){failed=true;return false;}
        for(size_t i=0;i<2;++i){const auto& expected=original_.sourceExpectationsOriginal->subjects[i];
            if(!initialProvider->ReceiveCandidateOriginal({static_cast<uint64_t>(i)+2,
                expected.source_occurrence.native_file_bytes,static_cast<uint32_t>(selectedRoute->phase),0,
                static_cast<uint32_t>(i)},&pending.OriginalSupervisorOwner())){failed=true;return false;}
        }
        if(!receiptFoundationPair||!receiptFoundationPair->CaptureOriginal(static_cast<uint32_t>(selectedRoute->phase))){failed=true;return false;}
        if(!receiptFoundationPair->AcquiredFoundationOriginalsQualified()){
            failed=true;return false; // Fixed genuine prebirth/pair/storage/archive consumer remains unfinished.
        }
        if(!initialProviderArchive->CaptureOriginal(*initialProviderProcess)){failed=true;return false;}
        processReadbackAckAttempted=true;
        processReadbackAckObserved=initialProvider->AcknowledgeProcessReferenceOriginal(*initialProviderProcess);
        if(!processReadbackAckObserved){failed=true;return false;}
        subjectLoopAttempted=true;
        std::size_t subject=2;
        for(std::size_t command=0;command<selectedRoute->requirements.size();++command)
            for(std::size_t requirement=0;requirement<selectedRoute->requirements[command].size();++requirement){
                const auto& selectedInput=selectedRoute->requirements[command][requirement];
                if(!FileRequirement(selectedInput))continue; // Both actual bootstrap rows remain retained separately.
                if(subject>=subjectReadbacksConstructed||(!subjectReadbacks[subject]&&!subjectUnits[subject])||
                   subject>UINT64_MAX-2||command>UINT32_MAX||requirement>UINT32_MAX||
                    !initialProvider->ReceiveCandidateOriginal({static_cast<uint64_t>(subject)+2,RequirementNativeExtent(selectedInput),
                       static_cast<uint32_t>(selectedRoute->phase),static_cast<uint32_t>(command),
                       static_cast<uint32_t>(requirement)},&pending.OriginalSupervisorOwner())){failed=true;return false;}
                if(subjectUnits[subject]){
                    const auto& selector=original_.sourceExpectationsOriginal->subjects[subject].source_occurrence.association_index;
                    size_t association=SIZE_MAX;
                    for(size_t n=0;n<subject;++n){const auto& prior=original_.sourceExpectationsOriginal->subjects[n];
                        if(prior.phase==selector.phase&&prior.command==selector.command&&prior.requirement==selector.requirement){
                            if(association!=SIZE_MAX){failed=true;return false;}association=n;
                        }
                    }
                    const auto* aCut=association==SIZE_MAX?nullptr:initialProviderArchive->OriginalCut(association+1);
                    if(association==SIZE_MAX||!subjectReadbacks[association]||!aCut||aCut->owner!=initialProviderArchive||
                       aCut->file!=subjectReadbacks[association]||!aCut->prefixRetained||
                       !subjectReadbacks[association]->AssociationObserved()||
                       !subjectUnits[subject]->CaptureOriginal(subject+1)||
                       !subjectUnits[subject]->ObserveAssociationRowsOriginal(*subjectReadbacks[association])){failed=true;return false;}
                    if(original_.sourceExpectationsOriginal->subjects[subject].source_occurrence.reader_class==SLCLI_BQ_EXPECTED_UNIT_PREREQUISITE&&
                       (!unitPayloads[subject]||!unitPayloads[subject]->CaptureOriginal(pending.OriginalIngressOwner()))){failed=true;return false;}
                    ++subjectReadbacksObserved;
                    const template_creator_linux::OriginalInitialProviderArchive::CutOriginal* earlierUnit=nullptr;
                    if(original_.sourceExpectationsOriginal->subjects[subject].source_occurrence.reader_class==SLCLI_BQ_EXPECTED_SP1_MEMBER_RANGE){
                        const auto& selected=original_.sourceExpectationsOriginal->subjects[subject].source_occurrence.source_unit;
                        for(size_t n=0;n<subject;++n){const auto& prior=original_.sourceExpectationsOriginal->subjects[n];
                            if(prior.phase==selected.phase&&prior.command==selected.command&&prior.requirement==selected.requirement){
                                if(earlierUnit){failed=true;return false;}
                                earlierUnit=initialProviderArchive->OriginalCut(n+1);
                            }
                        }
                    }
                    if(!initialProviderArchive->CaptureOriginal(*subjectUnits[subject],unitPayloads[subject],earlierUnit)||
                       !initialProvider->AcknowledgeSourceUnitOriginal(*subjectUnits[subject])){failed=true;return false;}
                }else{
                    if(!subjectReadbacks[subject]->CaptureOriginal(subject+1)){failed=true;return false;}
                    ++subjectReadbacksObserved;
                    if(!initialProviderArchive->CaptureOriginal(*subjectReadbacks[subject])||
                       !initialProvider->AcknowledgeReadbackOriginal(*subjectReadbacks[subject])){failed=true;return false;}
                }
                ++subjectReadbackAcksObserved;++subject;
            }
        if(subject!=subjectReadbackCapacity||initialProvider->OriginalOfferCount()!=decodedOfferCountOriginal){failed=true;return false;}
        if(!InitialProviderAcquiredOriginalsQualified()){failed=true;return false;}
        // Native subject acquisition/authentication is still an independent
        // source obligation; complete geometry equality alone admits no offer.
        invocationAttempted=true;invocation=decoder->BeginDecodedOriginal();
        if(!invocation||invocation->failed){failed=true;return false;}
        return true; // Creator result only; not physical settlement/qualification.
    }
    bool FailedPreownBorrowersSettledOriginal(const NativePlatformState& platform) const noexcept {
        if(!dispositionRequested||!invocationAttempted||!invocation||!invocation->main||!invocation->main->inputDispositionRequested||
           !invocation->main->host||invocation->main->host->platform!=&platform||!host||
           !host->IsOriginalPlatformOwner(&platform)||&platform.pendingHost!=host||
           platform.FailedPreownSourceFrontierOriginal()!=NativePlatformState::FailedPreownHostFrontier::AdoptedOriginalHost||
           !initialProvider||!initialProviderDisposed||!initialProvider->IndependentOriginalsClosedAtCutOriginal()||
           !initialProviderProcess||!processAssociationDisposed||!initialProviderProcess->DisposedBeforeExposureOriginal())return false;
        if(retentionPolicy&&(!retentionPolicyDisposed||!retentionPolicy->SourceDispositionEstablishedOriginal()))return false;
        if(receiptFoundationPair&&(!receiptFoundationPairDisposed||!receiptFoundationPair->SourceDispositionEstablishedOriginal()))return false;
        for(size_t i=0;i<subjectReadbacksConstructed;++i){
            if(subjectReadbacks&&subjectReadbacks[i]&&!subjectReadbacks[i]->BorrowerReleasedOriginal())return false;
            if(subjectUnits&&subjectUnits[i]&&!subjectUnits[i]->BorrowerReleasedOriginal())return false;
            if(unitPayloads&&unitPayloads[i]&&!unitPayloads[i]->BorrowerReleasedOriginal())return false;
        }
        return true; // Exact closed source borrowers, no archive qualification.
    }
    bool ObserveIndependentlyEligibleClosure() noexcept {
        dispositionRequested=true;
        if(physicallyDisposed)return true;
        NativePlatformState* failedPreownPlatform=nullptr;
        if(invocationAttempted){
            if(!decoder||!host||!initialProviderArchive||decoder->PrivateInvocation()!=invocation)return false;
            if(invocation&&invocation->main)invocation->main->RecoverOriginalPlatform();
            if(host->HasOriginalPlatformOwner()){
                if(!invocation||!invocation->main||!invocation->main->host||!invocation->main->host->platform||
                   !host->IsOriginalPlatformOwner(invocation->main->host->platform))return false;
            auto* actualPlatform=invocation->main->host->platform;
            if(actualPlatform->FailedPreownSourceFrontierOriginal()!=NativePlatformState::FailedPreownHostFrontier::None){
                if(!decoder->ObserveFailedPreownInputEligibility(actualPlatform))return false;
                failedPreownPlatform=actualPlatform;
            }else{
            // Actual platform input-cut closure settles command/native FD
            // borrowers before its supervisor stops; archive bytes remain
            // resident through actual final publication/readback and main.
            const bool closed=decoder->ObserveIndependentlyEligibleClosure();
            if(!initialProviderArchive->NativeBorrowersSettledForOriginal(invocation->main->host->platform))return false;
            if(receiptFoundationPair){
                if(host->ReceiptFoundationPairOriginal()!=receiptFoundationPair||
                   !receiptFoundationPair->SourceDispositionEstablishedOriginal())return false;
                receiptFoundationPairDisposed=true;
            }
            if(retentionPolicy){
                if(host->RetentionPolicyOriginal()!=retentionPolicy||!retentionPolicy->SourceDispositionEstablishedOriginal())return false;
                retentionPolicyDisposed=true;
            }
            processAssociationDisposed=true;initialProviderDisposed=true;
            physicallyDisposed=closed;if(invocation->failed)failed=true;
            return physicallyDisposed;
            }
            }
            // Null/partial construction can precede platform transfer. Freeze
            // actual future caller admission through the existing decoder's
            // independent pre-platform owner/borrower predicate before closing.
            if(!failedPreownPlatform&&((!invocation&&!decoder->ObserveIndependentlyEligibleClosure())||
               !decoder->ObservePrePlatformInputEligibility()))return false;
        }
        if(subjectReadbacks){
            for(std::size_t i=0;i<subjectReadbacksConstructed;++i)if(subjectReadbacks[i]&&
                !subjectReadbacks[i]->ReleaseBeforeSubjectExposureOriginal())return false;
        }
        if(unitPayloads)for(size_t i=0;i<subjectReadbacksConstructed;++i)if(unitPayloads[i]&&
            !unitPayloads[i]->ReleaseBeforeExposureOriginal())return false;
        if(subjectUnits)for(size_t i=0;i<subjectReadbacksConstructed;++i)if(subjectUnits[i]&&
            !subjectUnits[i]->ReleaseBeforeExposureOriginal())return false;
        if(retentionPolicy&&!retentionPolicyDisposed){
            if(!host||host->RetentionPolicyOriginal()!=retentionPolicy)return false;
            retentionPolicyDisposed=retentionPolicy->ObserveIndependentDispositionOriginal();
            if(!retentionPolicyDisposed)return false;
        }
        if(receiptFoundationPair&&!receiptFoundationPairDisposed){
            receiptFoundationPairDisposed=receiptFoundationPair->ObserveIndependentDispositionOriginal();
            if(!receiptFoundationPairDisposed)return false;
        }
        if(initialProviderProcess&&!processAssociationDisposed){
            if(!host)return false;
            processAssociationDisposed=initialProviderProcess->DisposeBeforeSubjectExposureOriginal(
                host->OriginalPendingOwner().OriginalSupervisorOwner());
            if(!processAssociationDisposed)return false;
        }
        if(initialProvider&&!initialProviderDisposed){
            initialProviderDisposed=initialProvider->DisposeBeforeSubjectExposureOriginal();
            if(!initialProviderDisposed)return false;
        }
        if(failedPreownPlatform){
            if(!failedPreownPlatform->BindFailedPreownStartupOriginal(*this))return false;
            physicallyDisposed=decoder->ObserveIndependentlyEligibleClosure();failed=true;
            return physicallyDisposed;
        }
        if(invocation&&invocation->main)invocation->main->RecoverOriginalPlatform();
        if(host&&host->HasOriginalPlatformOwner()){
            // Exact actual transfer, including failed wrapper/import/creator
            // construction, leaves all native closes with published platform.
            if(!decoder||!decoder->PrivateInvocation()||!invocation||!invocation->main||
               !invocation->main->host||!invocation->main->host->platform||
               !host->IsOriginalPlatformOwner(invocation->main->host->platform))return false;
            physicallyDisposed=decoder->ObserveIndependentlyEligibleClosure();
            if(invocation&&invocation->failed)failed=true;
            return physicallyDisposed;
        }
        if(decoder&&!decoder->ObservePrePlatformInputEligibility()){failed=true;return false;}
        // Actual invocation eligibility has frozen further Connect/producer/
        // creator admission. Interpreter/main may exist and remain resident.
        // Do not synthesize their disposal; close independently eligible
        // selection first while SAME observer/supervisor owners remain alive.
        bool selectionClosed=true;
        if(selection){selectionClosed=selection->CloseEligibleOnce()&&selection->PhysicalDisposed();}
        if(!host){physicallyDisposed=selectionClosed;return physicallyDisposed;}
        auto& pending=host->OriginalPendingOwner();
        if(pending.OriginalSupervisorOwner().HasPendingOriginalNativeCall())return false;
        if(!startupAuxiliaryDisposed)startupAuxiliaryDisposed=pending.DisposeAuxiliaryOriginalsBeforeObserverStop();
        if(!selectionClosed||!startupAuxiliaryDisposed){failed=true;return false;}
        if(!startupObserversDisposed)startupObserversDisposed=host->ObserveEligibleObserverClosure();
        if(!startupObserversDisposed)return false;
        if(!ingressDisposalAttempted){
            ingressDisposalAttempted=true;ingressDisposalResult=pending.OriginalIngressOwner().DisposeEligible();
        }
        physicallyDisposed=ingressDisposalResult;
        if(host->Failed()||!physicallyDisposed)failed=true;
        return physicallyDisposed;
        // No interpreter/future external exit/final publication grant.
    }
    bool ReservedStartupSourceBytes(std::size_t& bytes) const noexcept {
        bytes=sizeof(*this);std::size_t n=0;
        const auto add=[&](std::size_t count,std::size_t width) noexcept {
            if(width&&count>(SIZE_MAX-bytes)/width)return false;bytes+=count*width;return true;
        };
        if(!add(original_.ingressBody.capacity(),1)||!add(original_.capabilities.capacity(),sizeof(int))||
           original_.selectionPath.capacity()==SIZE_MAX||!add(original_.selectionPath.capacity()+1,1))return false;
        if(decoder&&(!decoder->ReservedDecodedSourceBytes(n)||!add(n,1)))return false;
        if(initialProvider&&!add(initialProvider->SourceBackingBytes(),1))return false;
        if(initialProviderProcess&&(!initialProviderProcess->AllocatedBackingBytesOriginal(n)||!add(n,1)))return false;
        if(receiptFoundationPair&&!add(receiptFoundationPair->ReservedSourceBytes(),1))return false;
        if(retentionPolicy&&(!retentionPolicy->SourceBackingBytesOriginal(n)||!add(n,1)))return false;
        if(initialProviderArchive&&!add(initialProviderArchive->SourceBackingBytes(),1))return false;
        if(subjectReadbacks){
            if(!add(subjectReadbackCapacity,sizeof(subjectReadbacks[0])))return false;
            for(std::size_t i=0;i<subjectReadbacksConstructed;++i)if(subjectReadbacks[i]&&
                (!subjectReadbacks[i]->AllocatedBackingBytesOriginal(n)||!add(n,1)))return false;
        }
        if(subjectUnits){
            if(!add(subjectReadbackCapacity,sizeof(subjectUnits[0])))return false;
            for(size_t i=0;i<subjectReadbacksConstructed;++i)if(subjectUnits[i]&&
                (!subjectUnits[i]->AllocatedBackingBytesOriginal(n)||!add(n,1)))return false;
        }
        if(unitPayloads){
            if(!add(subjectReadbackCapacity,sizeof(unitPayloads[0])))return false;
            for(size_t i=0;i<subjectReadbacksConstructed;++i)if(unitPayloads[i]&&
                (!unitPayloads[i]->AllocatedBackingBytesOriginal(n)||!add(n,1)))return false;
        }
        if(!host||!host->HasOriginalPlatformOwner()){
            if(host&&(!host->ReservedSourceBytes(n)||!add(n,1)))return false;
            if(selection&&!add(selection->ReservedSourceBytes(),1))return false;
        }
        return true; // Transferred host/selection counted by platform; full invocation/runtime/overlap additional.
    }
    bool RetainedSubjectBackingOriginalsAtCut() const noexcept {
        if(!dispositionRequested||!physicallyDisposed||subjectReadbacksConstructed>subjectReadbackCapacity)return false;
        for(size_t i=0;i<subjectReadbacksConstructed;++i)if(subjectReadbacks&&subjectReadbacks[i]){
            template_creator_linux::OriginalReceivedSubjectReadback::RetainedPreownBackingView view;
            if(!subjectReadbacks[i]->ObserveRetainedPreownBackingOriginal(view)||view.ownerOriginal!=subjectReadbacks[i]||
               view.readCount>view.readCapacity||view.retainedBytes>view.bodyCapacity||
               view.parsedAssociations>view.associationCapacity||!view.borrowReleased)return false;
        }
        if(initialProviderProcess){
            template_creator_linux::OriginalInitialProviderProcessAssociation::RetainedPreownBackingView view;
            if(!initialProviderProcess->ObserveRetainedPreownBackingOriginal(view)||view.ownerOriginal!=initialProviderProcess||
               view.bornTasks>view.taskCapacity||!view.disposed)return false;
        }
        for(size_t i=0;i<subjectReadbacksConstructed;++i)if(subjectUnits&&subjectUnits[i]){
            template_creator_linux::OriginalReceivedSourceUnitObservation::RetainedPreownBackingView view;
            if(!subjectUnits[i]->ObserveRetainedPreownBackingOriginal(view)||view.ownerOriginal!=subjectUnits[i]||
               view.parsedRecords>view.recordCapacity||!view.released)return false;
        }
        for(size_t i=0;i<subjectReadbacksConstructed;++i)if(unitPayloads&&unitPayloads[i]){
            template_creator_linux::OriginalUnitPayloadArchive::RetainedPreownBackingView view;
            if(!unitPayloads[i]->ObserveRetainedPreownBackingOriginal(view)||view.ownerOriginal!=unitPayloads[i]||
               !subjectUnits||view.unitOriginal!=subjectUnits[i]||view.rowCount>view.rowCapacity||
               view.edgeCount>view.edgeCapacity||!view.released)return false;
        }
        return true; // Settled resident typed backing; not portable payload serialization.
    }
    bool LocalSettlementRetainsStartupOriginals() const noexcept {
        const auto& view=localSettlement;
        if(view.failedPreownPlatformOriginal){
            if(!invocation||!invocation->main||!invocation->main->host||
               invocation->main->host->platform!=view.failedPreownPlatformOriginal||
               !FailedPreownBorrowersSettledOriginal(*view.failedPreownPlatformOriginal)||
               !view.failedPreownPlatformOriginal->SameRetainedFailedPreownViewOriginal(view.failedPreownViewOriginal)||
               !view.qualificationFailed)return false;
        }else if(view.failedPreownViewOriginal)return false;
        return RetainedSubjectBackingOriginalsAtCut()&&view.published&&view.sourceOriginalsSettled&&dispositionRequested&&physicallyDisposed&&
            view.owner==this&&view.invocationOriginal==invocation&&view.inputOriginal==&original_&&
            view.sourcePreflightOriginal==&sourceOccurrencePreflight&&view.receiverOriginal==initialProvider&&
            view.processOriginal==initialProviderProcess&&view.fileOwnersOriginal==subjectReadbacks.get()&&
            view.receiptFoundationPairOriginal==receiptFoundationPair&&
            view.receiptFoundationPairDisposition==receiptFoundationPairDisposed&&
            view.retentionPolicyOriginal==retentionPolicy&&view.retentionPolicyDisposition==retentionPolicyDisposed&&
            (!retentionPolicy||retentionPolicy->SameRetainedOriginalView(view.retentionPolicyView))&&
            (!receiptFoundationPair||(receiptFoundationPairDisposed&&receiptFoundationPair->SourceDispositionEstablishedOriginal()))&&
            view.unitOwnersOriginal==subjectUnits.get()&&view.unitPayloadOwnersOriginal==unitPayloads.get()&&
            view.preownExceptionOriginal==&preownOriginal&&
            view.subjectCapacity==subjectReadbackCapacity&&view.subjectsConstructed==subjectReadbacksConstructed&&
            view.subjectsObserved==subjectReadbacksObserved&&view.acksObserved==subjectReadbackAcksObserved&&
            view.subjectsConstructed<=view.subjectCapacity&&view.subjectsObserved<=view.subjectsConstructed&&
            view.acksObserved<=view.subjectsObserved;
    }
    const LocalSettlementOriginal& PublishLocalSettlementOriginal() noexcept {
        if(localSettlement.published||!dispositionRequested||!physicallyDisposed)return localSettlement;
        if(invocation&&invocation->main){
            const auto& main=*invocation->main;
            // Partial initialization has no fabricated interpreter disposition.
            if(main.interpreter&&main.interpreter->initializeAttempted&&!main.interpreter->initialized)
                return localSettlement;
            if(host&&host->HasOriginalPlatformOwner()){
                if(main.host&&main.host->platform&&main.host->platform->failedPreownPhysicalDisposed){
                    const auto& platform=*main.host->platform;
                    if(!host->IsOriginalPlatformOwner(&platform)||!FailedPreownBorrowersSettledOriginal(platform)||
                       !platform.AllPhysicalOriginalsDisposed()||!platform.SameRetainedFailedPreownViewOriginal(&platform.failedPreownRetainedView)||
                       !main.host->core||!main.host->core->PrivateHostClosure().linuxFinalHostConsumed)return localSettlement;
                    localSettlement.failedPreownPlatformOriginal=&platform;
                    localSettlement.failedPreownViewOriginal=&platform.failedPreownRetainedView;
                    localSettlement.qualificationFailed=true;
                }else{
                if(!main.host||!main.host->platform||!host->IsOriginalPlatformOwner(main.host->platform)||
                   !main.finalOutputDisposed||!main.lateMainCaptureReturned||
                   !main.host->platform->finalOutput.PhysicalDisposed()||
                   (main.receipt&&!main.receipt->BorrowerSettled()))return localSettlement;
                const auto& platform=*main.host->platform;
                if(!initialProviderArchive)return localSettlement;
                const auto& archiveView=initialProviderArchive->PublishRetainedFinalViewOriginal(main.host->platform);
                if(!archiveView.published||archiveView.owner!=initialProviderArchive||
                   archiveView.platform!=main.host->platform||archiveView.receiver!=initialProvider||
                   archiveView.count!=original_.initialProviderOfferCount)return localSettlement;
                localSettlement.initialArchiveOriginal=&archiveView;
                localSettlement.qualificationFailed=failed||invocation->failed||main.failed||
                    !initialProvider||!offerCountMatched||initialProvider->OriginalOfferCount()!=original_.initialProviderOfferCount||
                    !InitialProviderAcquiredOriginalsQualified()||
                    !main.lateMainCaptureClean||!platform.finalOutput.Qualified()||
                    !invocation->creatorReturned||!platform.commands.QualificationComplete()||
                    platform.qualificationFailed||!platform.ExecutedOriginalCommandsQualified()||
                    !platform.settledHostOutcomeObserved||!platform.settledHostNormalOutcome||
                    !main.host->core->PrivateHostClosure().linuxFinalHostConsumed||
                    main.host->core->PrivateHostClosure().qualificationFailed;
                }
            }else{
                // Actual pre-platform cleanup froze all future callers and
                // established no creator/command borrower. Interpreter presence
                // alone is not an additional external-custody prerequisite.
                if(!main.inputDispositionRequested||main.platformPointerConflict||
                   main.connected||main.invokeAttempted||
                   (main.routeProducer&&main.routeProducer->attempted)||
                   (main.host&&(main.host->platform||main.host->connected||
                       main.host->constructionAttempted||main.host->invokeAttempted)))return localSettlement;
                localSettlement.qualificationFailed=true;
            }
        }
        localSettlement.owner=this;localSettlement.invocationOriginal=invocation;
        if(subjectReadbacksConstructed>subjectReadbackCapacity||
           subjectReadbacksObserved>subjectReadbacksConstructed||
           subjectReadbackAcksObserved>subjectReadbacksObserved)return localSettlement;
        localSettlement.inputOriginal=&original_;localSettlement.sourcePreflightOriginal=&sourceOccurrencePreflight;
        localSettlement.receiverOriginal=initialProvider;localSettlement.processOriginal=initialProviderProcess;
        if(receiptFoundationPair&&(!receiptFoundationPairDisposed||
           !receiptFoundationPair->SourceDispositionEstablishedOriginal()))return localSettlement;
        localSettlement.receiptFoundationPairOriginal=receiptFoundationPair;
        localSettlement.receiptFoundationPairDisposition=receiptFoundationPairDisposed;
        if(retentionPolicy&&!retentionPolicyDisposed)return localSettlement;
        localSettlement.retentionPolicyOriginal=retentionPolicy;localSettlement.retentionPolicyDisposition=retentionPolicyDisposed;
        if(retentionPolicy){
            localSettlement.retentionPolicyView=retentionPolicy->PublishRetainedOriginalView();
            if(!retentionPolicy->SameRetainedOriginalView(localSettlement.retentionPolicyView))return localSettlement;
        }
        localSettlement.fileOwnersOriginal=subjectReadbacks.get();localSettlement.unitOwnersOriginal=subjectUnits.get();
        localSettlement.unitPayloadOwnersOriginal=unitPayloads.get();
        localSettlement.preownExceptionOriginal=&preownOriginal;
        localSettlement.subjectCapacity=subjectReadbackCapacity;
        localSettlement.subjectsConstructed=subjectReadbacksConstructed;
        localSettlement.subjectsObserved=subjectReadbacksObserved;localSettlement.acksObserved=subjectReadbackAcksObserved;
        if(!RetainedSubjectBackingOriginalsAtCut())return localSettlement;
        localSettlement.sourceOriginalsSettled=true;localSettlement.published=true;
        // Same source composition only: future external exit/EOF, authentic
        // installed runtime/rights and any admitted caller ACK remain separate.
        return localSettlement;
    }
};
OriginalLinuxSelectionStartup* BeginOriginalLinuxSelectionStartup(const OriginalLinuxSelectionStartupInput&);
inline bool NativePlatformState::BindFailedPreownStartupOriginal(const OriginalLinuxSelectionStartup& startup) noexcept {
    if((failedPreownStartupOriginal&&failedPreownStartupOriginal!=&startup)||
       !startup.FailedPreownBorrowersSettledOriginal(*this))return false;
    failedPreownStartupOriginal=&startup;return true;
}

}
#endif

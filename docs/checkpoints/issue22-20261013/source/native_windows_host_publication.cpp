// UNCOMPILED private post-product-close original capture. SAME-ABI frames are
// observations, not pointer restoration, Python graph serialization or native
// authority. All strong referents remain resident in the original interpreter.
#ifdef _WIN32
#include "windows_standalone_host.hpp"
namespace template_creator_bridge {
bool OriginalWindowsStandaloneHost::PublishPostCloseOriginals() noexcept {
    if (finalPublicationAttempted || !finalOutput || !finalOutput->adopted || !core || !platform ||
        !core->OnOriginalPythonThread() || !core->OnCreatingNativeThread() ||
        !core->PrivateHostClosure().eligible || !platform->hostDisposalAttempted ||
        !hostHandlesCloseObserved || channel.writeCompletionUnknown_ || handoff.unknownRead ||
        !platform->sink.CompletionKnown() || !pendingCalls || !pendingCalls->EndpointsIndependentlyEligible() ||
        !noticeOwner || !noticeOwner->eventsClosed_) return false;
    if (platform->collector && !platform->collector->PhysicalDisposed()) return false;
    // An outstanding native input read can still own its destination/output
    // cells. Do not copy those cells concurrently or pretend their close is done.
    for (SIZE_T i=0;i<platform->route.SelectedCount();++i) {
        const auto& input=platform->inputOwners[i];
        if (input.files) for (SIZE_T j=0;j<input.files->FileCount();++j)
            if (input.files->PrivateFiles()[j].readCompletionUnknown) return false;
    }
    finalPublicationAttempted=true;
    struct Frame { std::uint64_t version,kind,group,ordinal,bytes; };
    // The two passes visit SAME retained immutable rows. The first charges the
    // full actual payload before any write; there is no clipped delta fallback.
    SIZE_T total=0;
    SIZE_T group=SIZE_MAX;
    bool charging=true, clean=true;
    auto emit=[&](std::uint64_t kind,SIZE_T ordinal,const void* original,SIZE_T bytes) noexcept {
        if (!clean) return;
        if (bytes && !original) { clean=false; return; }
        if (charging) {
            if (bytes>SIZE_MAX-sizeof(Frame) || bytes+sizeof(Frame)>SIZE_MAX-total) { clean=false; return; }
            total+=bytes+sizeof(Frame); return;
        }
        const Frame frame{1,kind,static_cast<std::uint64_t>(group),static_cast<std::uint64_t>(ordinal),static_cast<std::uint64_t>(bytes)};
        if (!finalOutput->sink.AppendOriginalBytes(&frame,sizeof(frame)) ||
            !finalOutput->sink.AppendOriginalBytes(original,bytes)) clean=false;
    };
    auto rows=[&]() noexcept {
        group=SIZE_MAX;
        emit(1,0,this,sizeof(*this));
        emit(30,0,progress.get(),progressCount*sizeof(OriginalHostProgress));
        // All thread/IO/event borrowers settled before the SAME owner froze.
        // Retained row/copy referents are additional payloads, never restored pointers.
        emit(66,0,pendingCalls.get(),sizeof(*pendingCalls));
        emit(67,0,pendingCalls->rows_.get(),pendingCalls->count_*sizeof(OriginalWindowsPendingCall));
        emit(68,0,pendingCalls->joins_.get(),pendingCalls->joinCount_*sizeof(OriginalWindowsWorkerJoinAttempt));
        emit(69,0,noticeOwner,sizeof(*noticeOwner));
        for(SIZE_T slot=0;slot<2;++slot) {
            group=slot;
            const auto* io=slot?noticeOwner->receiverIo_.get():noticeOwner->noticeIo_.get();
            if(!io || io->count_>io->capacity_){clean=false;return;}
            emit(70,slot,io,sizeof(*io));
            emit(71,slot,io->rows_.get(),io->count_*sizeof(OriginalWindowsWorkerIoOriginal));
            for(SIZE_T row=0;row<io->count_;++row) {
                const auto& original=io->rows_[row];
                if(!original.activePublished.load(std::memory_order_acquire) ||
                    !original.resultPublished.load(std::memory_order_acquire) || original.completionUnknown ||
                    original.requested>io->frameBytes_ || original.bytes!=io->bytes_.get()+row*io->frameBytes_){clean=false;return;}
                emit(72,row,original.bytes,original.requested);
            }
        }
        group=SIZE_MAX;
        emit(2,0,&core->PrivateHostClosure(),sizeof(OriginalHostClosure));
        emit(3,0,&core->PrivateCreatorInvocation(),sizeof(OriginalCreatorInvocation));
        emit(4,0,core->PrivateDispatches(),core->PrivateDispatchCount()*sizeof(OriginalDispatch));
        emit(5,0,core->PrivateNativeThreadChecks(),core->PrivateNativeThreadCheckCount()*sizeof(OriginalNativeThreadCheck));
        emit(6,0,platform,sizeof(*platform));
        emit(22,0,platform->sink.PrivateCalls(),platform->sink.CallCount()*sizeof(template_creator::SinkOriginalCall));
        emit(23,0,platform->sink.PrivateOriginalBytes(),platform->sink.CopiedOriginalBytes());
        emit(24,0,platform->sink.PrivateReadbackBytes(),platform->sink.ReadbackByteCount());
        if (platform->collector) {
            emit(25,0,platform->collector,sizeof(*platform->collector));
            emit(26,0,platform->collector->PrivateThreadObservations(),
                platform->collector->PrivateThreadObservationCount()*sizeof(template_creator::CollectorThreadObservation));
            emit(27,0,platform->collector->PrivateReadsAfterPublication(),
                platform->collector->PrivateReadCountAfterPublication()*sizeof(template_creator::CollectorReadOriginal));
            emit(28,0,platform->collector->PrivateBytesAfterPublication(),platform->collector->PrivateByteExtentAfterPublication());
        }
        for (SIZE_T i=0;i<platform->route.SelectedCount();++i) {
            group=i;
            const auto& input=platform->inputOwners[i];
            emit(7,i,&input,sizeof(input));
            if (input.files) {
                emit(8,i,input.files.get(),sizeof(*input.files));
                emit(9,i,input.files->PrivateFiles(),input.files->FileCount()*sizeof(template_creator::HeldNativeFile));
                emit(10,i,input.files->PrivateReads(),input.files->ReadCount()*sizeof(template_creator::NativeRead));
                emit(11,i,input.files->PrivateReadBytes(),input.files->RetainedReadByteCount());
            }
            const auto& route=platform->route.PrivateOriginal(i);
            emit(12,i,&route,sizeof(route));
            if (route.nativeOriginal) {
                const auto& native=*route.nativeOriginal;
                if (native.streams.out.readCompletionUnknown || native.streams.err.readCompletionUnknown) {
                    clean=false; return;
                }
                if (native.streams.out.used>template_creator::StreamLimit || native.streams.err.used>template_creator::StreamLimit ||
                    native.streams.out.discardUsedBytes>template_creator::StreamLimit || native.streams.err.discardUsedBytes>template_creator::StreamLimit) {
                    clean=false; return;
                }
                emit(13,i,&native,sizeof(native));
                emit(14,i,native.streams.out.bytes.get(),native.streams.out.used);
                emit(15,i,native.streams.out.discard.get(),native.streams.out.discardUsedBytes);
                emit(16,i,native.streams.err.bytes.get(),native.streams.err.used);
                emit(17,i,native.streams.err.discard.get(),native.streams.err.discardUsedBytes);
            }
            if (route.gate) {
                if(!route.gate->CoreClrNativeOutputsSettledOriginal()){clean=false;return;}
                emit(29,i,route.gate.get(),sizeof(*route.gate));
                if(const auto* runtime=route.gate->PrivateParserRuntime()) {
                    if(const auto* list=runtime->PrivateFrameworkListsSource()) {
                        if(list->RowCount()>SIZE_MAX/sizeof(template_creator::OriginalFrameworkListSource::Row)){clean=false;return;}
                        emit(64,i,list,sizeof(*list));
                        emit(65,i,list->PrivateRows(),list->RowCount()*sizeof(template_creator::OriginalFrameworkListSource::Row));
                    }
                    const auto& relationship=runtime->PrivateSourceRelationship();
                    const SIZE_T retainedCapacity=runtime->RetainedArrayCapacityOriginal();
                    if(runtime->OccurrenceCount()>retainedCapacity||runtime->ConsumerResultCount()>retainedCapacity||
                        retainedCapacity>SIZE_MAX/sizeof(template_creator::OriginalParserRuntimeAssociation::AssetOccurrence)||
                        retainedCapacity>SIZE_MAX/sizeof(template_creator::OriginalParserRuntimeAssociation::ConsumerContractResult)){clean=false;return;}
                    if(runtime->PrivateConsumerResults())emit(62,i,runtime->PrivateConsumerResults(),retainedCapacity*sizeof(template_creator::OriginalParserRuntimeAssociation::ConsumerContractResult));
                    emit(42,i,runtime,sizeof(*runtime));
                    if(runtime->PrivateOccurrences())emit(43,i,runtime->PrivateOccurrences(),retainedCapacity*sizeof(template_creator::OriginalParserRuntimeAssociation::AssetOccurrence));
                    const SIZE_T artifacts=runtime->CompilerArtifactCount();
                    if(artifacts>SIZE_MAX/sizeof(template_creator::OriginalHeldMsfSource)||
                        artifacts>SIZE_MAX/sizeof(template_creator::OriginalHeldSourceDigest)){clean=false;return;}
                    if(runtime->PrivateCompilerSourceDigests())emit(112,i,runtime->PrivateCompilerSourceDigests(),artifacts*sizeof(template_creator::OriginalHeldSourceDigest));
                    if(const auto* owners=runtime->PrivateCompilerArtifacts()){
                        emit(113,i,owners,artifacts*sizeof(template_creator::OriginalHeldMsfSource));
                        for(SIZE_T artifact=0;artifact<artifacts;++artifact){
                            std::array<template_creator::OriginalHeldMsfSource::RetainedAllocationSpan,8> spans{};
                            if(!owners[artifact].RetainedAllocationSpansOriginal(spans)){clean=false;return;}
                            emit(114,artifact,spans.data(),sizeof(spans));
                            for(SIZE_T slot=0;slot<spans.size();++slot)
                                if(spans[slot].original)emit(115+slot,artifact,spans[slot].original,spans[slot].bytes);
                        }
                    }
                    if(relationship.retainedMembershipObserved) {
                        emit(44,i,relationship.buildBytesOriginal,relationship.buildExtent);
                        emit(45,i,relationship.sourceBytesOriginal,relationship.sourceExtent);
                    }
                    const auto& supported=runtime->PrivateSupportedSourceCase();
                    emit(46,i,&supported,sizeof(supported));
                    for(SIZE_T source=0;source<3;++source)
                        if(supported.bodies[source])emit(47,i,supported.bodies[source],supported.extents[source]);
                    if(const auto* directory=runtime->PrivateVendorDirectory()) {
                        if(directory->MemberCount()>SIZE_MAX/sizeof(template_creator::OriginalZipDirectory::Member)){clean=false;return;}
                        emit(54,i,directory,sizeof(*directory));
                        emit(55,i,directory->PrivateMembers(),directory->MemberCount()*sizeof(template_creator::OriginalZipDirectory::Member));
                        emit(56,i,directory->PrivateOriginal(),directory->OriginalExtent());
                    }
                    if(runtime->VendorNameCount()>SIZE_MAX/sizeof(template_creator::OriginalParserRuntimeAssociation::VendorNameOccurrence)){clean=false;return;}
                    emit(61,i,runtime->PrivateVendorNames(),runtime->VendorNameCount()*sizeof(template_creator::OriginalParserRuntimeAssociation::VendorNameOccurrence));
                    for(SIZE_T member=0;member<runtime->VendorComparisonCount();++member){
                        const auto* comparison=runtime->PrivateVendorComparison(member);
                        if(!comparison){clean=false;return;}
                        emit(57,i,comparison,sizeof(*comparison));
                        if(const auto* decoder=comparison->PrivateInflater()){
                            if(decoder->CallCount()>decoder->CallCapacity()||decoder->CallCapacity()>SIZE_MAX/sizeof(template_creator::OriginalRawMember::Call)){clean=false;return;}
                            emit(58,i,decoder,sizeof(*decoder));
                            emit(59,i,decoder->PrivateCalls(),decoder->CallCapacity()*sizeof(template_creator::OriginalRawMember::Call));
                            emit(60,i,decoder->PrivateOutput(),decoder->PrivateOutputCapacity());
                        }
                    }
                    if(const auto* configuration=runtime->PrivateHostConfiguration()) {
                        emit(48,i,configuration,sizeof(*configuration));
                        if(configuration->AssetCandidateCount()>configuration->RetainedAssetCapacityOriginal()||
                            configuration->RetainedAssetCapacityOriginal()>SIZE_MAX/sizeof(template_creator::OriginalRuntimeHostConfiguration::AssetCandidate)){clean=false;return;}
                        if(configuration->RetainedAssetCapacityOriginal())emit(52,i,configuration->PrivateAssetCandidates(),configuration->RetainedAssetCapacityOriginal()*sizeof(template_creator::OriginalRuntimeHostConfiguration::AssetCandidate));
                        if(configuration->LibraryMetadataOccurrenceCount()>configuration->RetainedLibraryCapacityOriginal()||
                            configuration->RetainedLibraryCapacityOriginal()>SIZE_MAX/sizeof(template_creator::OriginalRuntimeHostConfiguration::LibraryMetadataOccurrence)){clean=false;return;}
                        if(configuration->RetainedLibraryCapacityOriginal())emit(53,i,configuration->PrivateLibraryMetadataOccurrences(),configuration->RetainedLibraryCapacityOriginal()*sizeof(template_creator::OriginalRuntimeHostConfiguration::LibraryMetadataOccurrence));
                        emit(63,i,configuration->PrivateLibraryTraversalOriginal(),configuration->LibraryTraversalOriginalBytes());
                        const template_creator::OriginalJson* documents[]={&configuration->PrivateRuntimeConfigSyntax(),&configuration->PrivateDepsSyntax(),configuration->PrivateDevConfigSyntax()};
                        const template_creator::OriginalRuntimeHostConfiguration::Document* originals[]={&configuration->PrivateRuntimeConfigOriginal(),&configuration->PrivateDepsOriginal(),&configuration->PrivateDevConfigOriginal()};
                        for(SIZE_T document=0;document<3;++document){
                            if(!documents[document])continue;
                            if(documents[document]->Count()>documents[document]->RetainedTokenCapacityOriginal()||
                                documents[document]->RetainedTokenCapacityOriginal()>SIZE_MAX/sizeof(template_creator::OriginalJson::Token)){clean=false;return;}
                            emit(49,i,originals[document],sizeof(*originals[document]));
                            emit(123,document,documents[document],sizeof(*documents[document]));
                            if(documents[document]->RetainedTokenCapacityOriginal())emit(50,i,documents[document]->PrivateTokens(),documents[document]->RetainedTokenCapacityOriginal()*sizeof(template_creator::OriginalJson::Token));
                            if(documents[document]->RetainedFramesOriginal())emit(124,document,documents[document]->RetainedFramesOriginal(),documents[document]->RetainedFrameBytesOriginal());
                            if(originals[document]->spanObserved)emit(51,i,originals[document]->body,originals[document]->extent);
                        }
                    }
                }
                const auto emitCode=[&](const template_creator::OriginalAliasCodeGuard& guard,SIZE_T k) {
                    const auto& generated=guard.PrivateCopy().PrivateGenerated();
                    for(SIZE_T j=0;j<generated.FileCount();++j)if(generated.PrivateFiles()[j].readCompletionUnknown){clean=false;return;}
                    if(generated.FileCount()>SIZE_MAX/sizeof(template_creator::HeldNativeFile) ||
                        generated.ReadCount()>SIZE_MAX/sizeof(template_creator::NativeRead) ||
                        generated.RetainedReadByteCount()>generated.ReadByteCapacity()){clean=false;return;}
                    emit(32,k,&guard,sizeof(guard));
                    emit(33,k,generated.PrivateFiles(),generated.FileCount()*sizeof(template_creator::HeldNativeFile));
                    emit(34,k,generated.PrivateReads(),generated.ReadCount()*sizeof(template_creator::NativeRead));
                    emit(35,k,generated.PrivateReadBytes(),generated.RetainedReadByteCount());
                };
                for(SIZE_T kind=0;kind<2;++kind)for(SIZE_T k=0;k<route.gate->AliasCodeGuardCount(kind);++k)
                    emitCode(route.gate->PrivateAliasCodeGuard(kind,k),k);
                for(SIZE_T kind=0;kind<2;++kind)for(SIZE_T k=0;k<route.gate->HelperCodeGuardCount(kind);++k) {
                    const auto& guard=route.gate->PrivateHelperCodeGuard(kind,k);
                    emit(37,k,&guard,sizeof(guard));emitCode(guard.PrivateHelper(),k);
                    if(guard.PrivateEcho())emitCode(*guard.PrivateEcho(),k);
                }
                for(SIZE_T k=0;k<route.gate->ParserInputCount();++k) {
                    const auto& parser=route.gate->PrivateParserInput(k);
                    const auto& generated=parser.PrivateCopy().PrivateGenerated();
                    for(SIZE_T f=0;f<generated.FileCount();++f)
                        if(generated.PrivateFiles()[f].readCompletionUnknown){clean=false;return;}
                    if(generated.FileCount()>SIZE_MAX/sizeof(template_creator::HeldNativeFile) ||
                        generated.ReadCount()>SIZE_MAX/sizeof(template_creator::NativeRead) ||
                        generated.RetainedReadByteCount()>generated.ReadByteCapacity()){clean=false;return;}
                    emit(38,k,&parser,sizeof(parser));
                    emit(39,k,generated.PrivateFiles(),generated.FileCount()*sizeof(template_creator::HeldNativeFile));
                    emit(40,k,generated.PrivateReads(),generated.ReadCount()*sizeof(template_creator::NativeRead));
                    emit(41,k,generated.PrivateReadBytes(),generated.RetainedReadByteCount());
                }
                for (SIZE_T j=0;j<route.gate->CommandLineCount();++j) {
                    const auto& command=route.gate->PrivateCommandLine(j);
                    emit(31,j,&command,sizeof(command));
                    const auto& helper=route.gate->PrivateHelperProjection(j);
                    emit(36,j,&helper,sizeof(helper));
                }
                emit(18,i,route.gate->PrivateCallerRows(),route.gate->CallerRowCount()*sizeof(template_creator::GateCallerObservation));
                if(!route.gate->CoreClrNativeOutputsSettledOriginal()){clean=false;return;}
                if(const auto* custody=route.gate->PrivateCoreClrCustody()) {
                    if(custody->PrivateThreadCount()>custody->RetainedThreadCapacityOriginal()||
                        custody->PrivateChangeCount()>custody->RetainedChangeCapacityOriginal()||
                        custody->RetainedThreadCapacityOriginal()>SIZE_MAX/sizeof(template_creator::OriginalCoreClrThreadCustody) ||
                        custody->RetainedChangeCapacityOriginal()>SIZE_MAX/sizeof(template_creator::OriginalCoreClrContextChange)) {
                        clean=false;return;
                    }
                    // Same two-pass private publication retains actual T/R
                    // referents, including cleanup cause/context and native
                    // disarm results. Raw addresses never recreate authority.
                    emit(73,i,custody,sizeof(*custody));
                    if(custody->RetainedThreadCapacityOriginal())emit(74,i,custody->PrivateThreads(),custody->RetainedThreadCapacityOriginal()*sizeof(template_creator::OriginalCoreClrThreadCustody));
                    if(custody->RetainedChangeCapacityOriginal())emit(75,i,custody->PrivateChanges(),custody->RetainedChangeCapacityOriginal()*sizeof(template_creator::OriginalCoreClrContextChange));
                }
                emit(90,i,route.gate.get(),sizeof(*route.gate));
                template_creator::OriginalWindowsGate::RetainedPreownView preown{};
                if(!route.gate->ObserveRetainedPreownViewOriginal(preown)){clean=false;return;}
                emit(111,i,&preown,sizeof(preown));
                for(SIZE_T k=0;k<preown.invocationCapacity;++k) {
                    const auto* attached=route.gate->RetainedPreownInvocationOriginal(preown,k);
                    if(!attached)continue;
                    const auto& invocation=*attached;
                    emit(76,k,&invocation,sizeof(invocation));
                    if(invocation.instructionCount>invocation.instructionCapacity ||
                        invocation.instructionCapacity>SIZE_MAX/sizeof(template_creator::GateCoreClrInstruction)) {clean=false;return;}
                    emit(86,k,invocation.instructionOriginals.get(),invocation.instructionCapacity*sizeof(template_creator::GateCoreClrInstruction));
                    if(invocation.callerBoundary){
                        const auto& boundary=*invocation.callerBoundary;
                        if(boundary.PrivateInstructionCount()>boundary.PrivateAllocationCapacity()||
                            boundary.PrivateWorkCount()>boundary.PrivateAllocationCapacity()||
                            boundary.PrivateAllocationCapacity()>SIZE_MAX/sizeof(template_creator::OriginalAmd64Instruction)||
                            boundary.PrivateAllocationCapacity()>SIZE_MAX/sizeof(DWORD)){clean=false;return;}
                        emit(87,k,&boundary,sizeof(boundary));
                        if(boundary.PrivateInstructions())emit(88,k,boundary.PrivateInstructions(),boundary.PrivateAllocationCapacity()*sizeof(template_creator::OriginalAmd64Instruction));
                        if(boundary.PrivateWork())emit(89,k,boundary.PrivateWork(),boundary.PrivateAllocationCapacity()*sizeof(DWORD));
                    }
                    if(!invocation.inputs)continue;
                    const auto& input=*invocation.inputs;
                    SIZE_T readsBackingBytes=0,spansBackingBytes=0;
                    if(!input.RetainedRowBackingExtentsOriginal(readsBackingBytes,spansBackingBytes)){clean=false;return;}
                    const SIZE_T pointers=input.PrivatePointerCapacity();
                    if(pointers>(SIZE_MAX-2)/2 || pointers>SIZE_MAX/sizeof(DWORD64) ||
                        input.readCount>input.PrivateReadCapacity() ||
                        input.PrivateReadCapacity()>SIZE_MAX/sizeof(template_creator::OriginalCoreClrEntryInputs::ReadOriginal) ||
                        input.spanCount>2*pointers+2 ||
                        2*pointers+2>SIZE_MAX/sizeof(template_creator::OriginalCoreClrEntryInputs::StringOriginal) ||
                        (input.readCount&&!input.PrivateReads()) || (input.spanCount&&!input.PrivateStrings())){clean=false;return;}
                    emit(77,k,&input,sizeof(input));
                    if(readsBackingBytes)emit(78,k,input.PrivateReads(),readsBackingBytes);
                    if(spansBackingBytes)emit(79,k,input.PrivateStrings(),spansBackingBytes);
                    // Full preowned destinations preserve known partial failed
                    // reads as well as completed prefixes, independently of
                    // later semantic string/property interpretation.
                    if(input.PrivateStringBytes())emit(80,k,input.PrivateStringBytes(),input.PrivateStringCapacity());
                    if(input.PrivateRevalidatedStringBytes())emit(81,k,input.PrivateRevalidatedStringBytes(),input.PrivateStringCapacity());
                    for(SIZE_T role=0;role<2;++role) {
                        if(input.PrivatePointers(role))emit(82+role,k,input.PrivatePointers(role),pointers*sizeof(DWORD64));
                        if(input.PrivateRevalidatedPointers(role))emit(84+role,k,input.PrivateRevalidatedPointers(role),pointers*sizeof(DWORD64));
                    }
                }
                for (SIZE_T j=0;j<preown.imageCapacity;++j) {
                    const auto* attached=route.gate->RetainedPreownImageOriginal(preown,j);
                    if(!attached)continue;const auto& image=*attached;
                    if (image.loaded.retainedByteExtent>image.loaded.byteCapacity || image.loaded.regionCount>image.loaded.regionCapacity) {
                        clean=false; return;
                    }
                    emit(19,j,&image,sizeof(image));
                    if(image.loaded.regionCapacity>SIZE_MAX/sizeof(template_creator::MemoryRegion)){clean=false;return;}
                    if(image.loaded.regions)emit(20,j,image.loaded.regions.get(),image.loaded.regionCapacity*sizeof(template_creator::MemoryRegion));
                    if(image.loaded.bytes)emit(21,j,image.loaded.bytes.get(),image.loaded.byteCapacity);
                }
            }
        }
    };
    rows();
    if (!clean || total>finalOutput->sink.OriginalByteCapacity()) return false;
    charging=false;
    rows();
    // Failed/partial publication may still flush/read back its completed prefix.
    // Qualification stays failed; no uncertain write is retried.
    const bool prefix=finalOutput->sink.FlushReadbackOriginal();
    finalPublicationVerified=clean && prefix && finalOutput->sink.Verified();
    return finalPublicationVerified; // Not interpreter exit or whole-host acceptance.
}
}
#endif

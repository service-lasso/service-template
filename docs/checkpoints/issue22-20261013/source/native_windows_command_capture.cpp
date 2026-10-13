// UNCOMPILED private, same-ABI memory observations of retained original owners.
// These bytes are never deserialized as capabilities or exported as authority.
// Live owners retain C++ exceptions, pointer referents and original handles;
// copying their representations cannot replace that retained native custody.
#ifdef _WIN32
#include "native_windows_platform.hpp"
#include <cstdint>
namespace template_creator_bridge {
namespace {
struct PrivateCaptureFrame {
    std::uint64_t version = 1, kind = 0, ordinal = 0, bytes = 0;
};
bool CaptureBytes(template_creator::OriginalWindowsSink& sink,std::uint64_t kind,
                  SIZE_T ordinal,const void* original,SIZE_T bytes) noexcept {
    const PrivateCaptureFrame frame{1,kind,static_cast<std::uint64_t>(ordinal),static_cast<std::uint64_t>(bytes)};
    return sink.AppendOriginalBytes(&frame,sizeof(frame)) && sink.AppendOriginalBytes(original,bytes);
}
bool CaptureState(template_creator::OriginalWindowsSink& sink,SIZE_T ordinal,
                  const template_creator::State& original) noexcept {
    if (original.streams.out.readCompletionUnknown || original.streams.err.readCompletionUnknown) return false;
    // State's inline ABI observation includes native results/events/resources,
    // setup/disposal flags and original exception representations. Dynamic byte
    // stores are captured separately; the original State remains resident.
    if (!CaptureBytes(sink,1,ordinal,&original,sizeof(original))) return false;
    const auto captureStream = [&](const template_creator::Stream& stream,std::uint64_t kind) noexcept {
        if (stream.used>template_creator::StreamLimit || stream.discardUsedBytes>template_creator::StreamLimit) return false;
        return CaptureBytes(sink,kind,ordinal,stream.bytes.get(),stream.used) &&
               CaptureBytes(sink,kind+1,ordinal,stream.discard.get(),stream.discardUsedBytes);
    };
    return captureStream(original.streams.out,2) && captureStream(original.streams.err,4);
}
}
PyObject* NativePlatformState::CaptureCommand(PyObject* owner) {
    SIZE_T ordinal = 0;
    const auto* command = commands.ExactPrivateOriginal(owner,ordinal);
    if (!command) { qualificationFailed = true; return nullptr; }
    const auto* current = commands.ExactPrivateNative(owner,ordinal);
    const auto* bootstrap = commands.PrivateBootstrapNative();
    // A no-process failed Begin still has its preowned session originals. A
    // missing native pointer is retained as absence, never a terminal witness.
    const PrivateCaptureFrame commandHeader{1,0,static_cast<std::uint64_t>(ordinal),0};
    bool captured = sink.AppendOriginalBytes(&commandHeader,sizeof(commandHeader));
    if (captured && bootstrap) captured = CaptureState(sink,0,*bootstrap);
    if (captured && current && current!=bootstrap) captured = CaptureState(sink,ordinal+1,*current);
    const auto captureGate = [&](SIZE_T routeOrdinal) noexcept {
        const auto& routeRow = route.PrivateOriginal(routeOrdinal);
        if (!routeRow.gate) return true; // No observer was acquired before precharge.
        if(!routeRow.gate->CoreClrNativeOutputsSettledOriginal())return false;
        struct GateCounts { SIZE_T commands,callers,images; };
        const GateCounts counts{routeRow.gate->CommandLineCount(),routeRow.gate->CallerRowCount(),routeRow.gate->ImageRowCount()};
        if (!CaptureBytes(sink,7,routeOrdinal,&counts,sizeof(counts)) ||
            counts.callers>SIZE_MAX/sizeof(template_creator::GateCallerObservation) ||
            !CaptureBytes(sink,8,routeOrdinal,routeRow.gate->PrivateCallerRows(),
                counts.callers*sizeof(template_creator::GateCallerObservation))) return false;
        if(const auto* custody=routeRow.gate->PrivateCoreClrCustody()) {
            if(custody->PrivateThreadCount()>SIZE_MAX/sizeof(template_creator::OriginalCoreClrThreadCustody) ||
                custody->PrivateChangeCount()>SIZE_MAX/sizeof(template_creator::OriginalCoreClrContextChange) ||
                !CaptureBytes(sink,47,routeOrdinal,custody,sizeof(*custody)) ||
                !CaptureBytes(sink,48,routeOrdinal,custody->PrivateThreads(),
                    custody->PrivateThreadCount()*sizeof(template_creator::OriginalCoreClrThreadCustody)) ||
                !CaptureBytes(sink,49,routeOrdinal,custody->PrivateChanges(),
                    custody->PrivateChangeCount()*sizeof(template_creator::OriginalCoreClrContextChange)))return false;
        }
        if(!CaptureBytes(sink,64,routeOrdinal,routeRow.gate.get(),sizeof(*routeRow.gate)))return false;
        for(SIZE_T k=0;k<routeRow.gate->PrivateCoreClrInvocationCount();++k) {
            const auto& invocation=routeRow.gate->PrivateCoreClrInvocation(k);
            const auto& input=*invocation.inputs;const auto& boundary=*invocation.callerBoundary;
            const SIZE_T pointers=input.PrivatePointerCapacity();
            if(pointers>(SIZE_MAX-2)/2 || pointers>SIZE_MAX/sizeof(DWORD64) ||
                input.readCount>input.PrivateReadCapacity() ||
                input.readCount>SIZE_MAX/sizeof(template_creator::OriginalCoreClrEntryInputs::ReadOriginal) ||
                input.spanCount>2*pointers+2 ||
                input.spanCount>SIZE_MAX/sizeof(template_creator::OriginalCoreClrEntryInputs::StringOriginal) ||
                invocation.instructionCount>invocation.instructionCapacity ||
                invocation.instructionCount>SIZE_MAX/sizeof(template_creator::GateCoreClrInstruction) ||
                boundary.PrivateInstructionCount()>SIZE_MAX/sizeof(template_creator::OriginalAmd64Instruction) ||
                boundary.PrivateWorkCount()>SIZE_MAX/sizeof(DWORD))return false;
            if(!CaptureBytes(sink,50,k,&invocation,sizeof(invocation)) || !CaptureBytes(sink,51,k,&input,sizeof(input)) ||
                !CaptureBytes(sink,52,k,input.PrivateReads(),input.readCount*sizeof(template_creator::OriginalCoreClrEntryInputs::ReadOriginal)) ||
                !CaptureBytes(sink,53,k,input.PrivateStrings(),input.spanCount*sizeof(template_creator::OriginalCoreClrEntryInputs::StringOriginal)) ||
                !CaptureBytes(sink,54,k,input.PrivateStringBytes(),input.PrivateStringCapacity()) ||
                !CaptureBytes(sink,55,k,input.PrivateRevalidatedStringBytes(),input.PrivateStringCapacity()))return false;
            for(SIZE_T role=0;role<2;++role)
                if(!CaptureBytes(sink,56+role,k,input.PrivatePointers(role),pointers*sizeof(DWORD64)) ||
                    !CaptureBytes(sink,58+role,k,input.PrivateRevalidatedPointers(role),pointers*sizeof(DWORD64)))return false;
            if(!CaptureBytes(sink,60,k,invocation.instructionOriginals.get(),invocation.instructionCount*sizeof(template_creator::GateCoreClrInstruction)) ||
                !CaptureBytes(sink,61,k,&boundary,sizeof(boundary)) ||
                !CaptureBytes(sink,62,k,boundary.PrivateInstructions(),boundary.PrivateInstructionCount()*sizeof(template_creator::OriginalAmd64Instruction)) ||
                !CaptureBytes(sink,63,k,boundary.PrivateWork(),boundary.PrivateWorkCount()*sizeof(DWORD)))return false;
        }
        for (SIZE_T i=0; i<counts.images; ++i) {
            const auto& image = routeRow.gate->PrivateImage(i);
            const auto& loaded = image.loaded;
            if (loaded.regionCount>loaded.regionCapacity || loaded.retainedByteExtent>loaded.byteCapacity ||
                loaded.regionCount>SIZE_MAX/sizeof(template_creator::MemoryRegion)) return false;
            // Inline identity/header and every actual region result survive
            // independently of whether loaded.complete is true. The dynamic
            // returned byte prefix is separate from the resident pointer owner.
            if (!CaptureBytes(sink,9,routeOrdinal,&image,sizeof(image)) ||
                !CaptureBytes(sink,10,routeOrdinal,loaded.regions.get(),
                    loaded.regionCount*sizeof(template_creator::MemoryRegion)) ||
                !CaptureBytes(sink,11,routeOrdinal,loaded.bytes.get(),loaded.retainedByteExtent)) return false;
        }
        for (SIZE_T i=0; i<counts.commands; ++i) {
            const auto& original = routeRow.gate->PrivateCommandLine(i);
            // The body and all partial native-read buffers are inline in this
            // original row; its live owner and borrowed process remain resident.
            if (!CaptureBytes(sink,6,routeOrdinal,&original,sizeof(original))) return false;
            const auto& helper=routeRow.gate->PrivateHelperProjection(i);
            if(!CaptureBytes(sink,17,routeOrdinal,&helper,sizeof(helper)))return false;
        }
        for(SIZE_T i=0;i<routeRow.gate->CanonicalAliasSourceCount();++i) {
            const auto& source=routeRow.gate->PrivateCanonicalAliasSource(i);
            if(!CaptureBytes(sink,12,routeOrdinal,&source,sizeof(source)))return false;
        }
        for(SIZE_T i=0;i<routeRow.gate->CanonicalHelperSourceCount();++i) {
            const auto& source=routeRow.gate->PrivateCanonicalHelperSource(i);
            if(!CaptureBytes(sink,18,routeOrdinal,&source,sizeof(source)))return false;
        }
        for(SIZE_T i=0;i<routeRow.gate->CanonicalParserSourceCount();++i) {
            const auto& source=routeRow.gate->PrivateCanonicalParserSource(i);
            if(!CaptureBytes(sink,20,routeOrdinal,&source,sizeof(source)))return false;
        }
        if(const auto* runtime=routeRow.gate->PrivateParserRuntime()) {
            if(const auto* list=runtime->PrivateFrameworkListsSource()) {
                if(list->RowCount()>SIZE_MAX/sizeof(template_creator::OriginalFrameworkListSource::Row)||
                    !CaptureBytes(sink,47,routeOrdinal,list,sizeof(*list))||
                    !CaptureBytes(sink,48,routeOrdinal,list->PrivateRows(),list->RowCount()*sizeof(template_creator::OriginalFrameworkListSource::Row)))return false;
            }
            const auto& relationship=runtime->PrivateSourceRelationship();
            if(runtime->OccurrenceCount()>SIZE_MAX/sizeof(template_creator::OriginalParserRuntimeAssociation::AssetOccurrence))return false;
            if(runtime->ConsumerResultCount()>SIZE_MAX/sizeof(template_creator::OriginalParserRuntimeAssociation::ConsumerContractResult)||
                !CaptureBytes(sink,45,routeOrdinal,runtime->PrivateConsumerResults(),runtime->ConsumerResultCount()*sizeof(template_creator::OriginalParserRuntimeAssociation::ConsumerContractResult)))return false;
            if(!CaptureBytes(sink,25,routeOrdinal,runtime,sizeof(*runtime)) ||
                !CaptureBytes(sink,26,routeOrdinal,runtime->PrivateOccurrences(),runtime->OccurrenceCount()*sizeof(template_creator::OriginalParserRuntimeAssociation::AssetOccurrence)))return false;
            if(relationship.retainedMembershipObserved &&
                (!CaptureBytes(sink,27,routeOrdinal,relationship.buildBytesOriginal,relationship.buildExtent) ||
                 !CaptureBytes(sink,28,routeOrdinal,relationship.sourceBytesOriginal,relationship.sourceExtent)))return false;
            const auto& supported=runtime->PrivateSupportedSourceCase();
            if(!CaptureBytes(sink,29,routeOrdinal,&supported,sizeof(supported)))return false;
            for(SIZE_T source=0;source<3;++source)
                if(supported.bodies[source] && !CaptureBytes(sink,30,routeOrdinal,supported.bodies[source],supported.extents[source]))return false;
            if(const auto* directory=runtime->PrivateVendorDirectory()) {
                if(directory->MemberCount()>SIZE_MAX/sizeof(template_creator::OriginalZipDirectory::Member)||
                    !CaptureBytes(sink,37,routeOrdinal,directory,sizeof(*directory))||
                    !CaptureBytes(sink,38,routeOrdinal,directory->PrivateMembers(),directory->MemberCount()*sizeof(template_creator::OriginalZipDirectory::Member))||
                    !CaptureBytes(sink,39,routeOrdinal,directory->PrivateOriginal(),directory->OriginalExtent()))return false;
            }
            if(runtime->VendorNameCount()>SIZE_MAX/sizeof(template_creator::OriginalParserRuntimeAssociation::VendorNameOccurrence)||
                !CaptureBytes(sink,44,routeOrdinal,runtime->PrivateVendorNames(),runtime->VendorNameCount()*sizeof(template_creator::OriginalParserRuntimeAssociation::VendorNameOccurrence)))return false;
            for(SIZE_T member=0;member<runtime->VendorComparisonCount();++member){
                const auto* comparison=runtime->PrivateVendorComparison(member);
                if(!comparison||!CaptureBytes(sink,40,routeOrdinal,comparison,sizeof(*comparison)))return false;
                if(const auto* decoder=comparison->PrivateInflater()){
                    if(decoder->CallCount()>SIZE_MAX/sizeof(template_creator::OriginalRawMember::Call)||
                        !CaptureBytes(sink,41,routeOrdinal,decoder,sizeof(*decoder))||
                        !CaptureBytes(sink,42,routeOrdinal,decoder->PrivateCalls(),decoder->CallCount()*sizeof(template_creator::OriginalRawMember::Call))||
                        !CaptureBytes(sink,43,routeOrdinal,decoder->PrivateOutput(),decoder->PrivateOutputCapacity()))return false;
                }
            }
            if(const auto* configuration=runtime->PrivateHostConfiguration()) {
                if(!CaptureBytes(sink,31,routeOrdinal,configuration,sizeof(*configuration)))return false;
                if(configuration->AssetCandidateCount()>SIZE_MAX/sizeof(template_creator::OriginalRuntimeHostConfiguration::AssetCandidate)||
                    !CaptureBytes(sink,35,routeOrdinal,configuration->PrivateAssetCandidates(),configuration->AssetCandidateCount()*sizeof(template_creator::OriginalRuntimeHostConfiguration::AssetCandidate)))return false;
                if(configuration->LibraryMetadataOccurrenceCount()>SIZE_MAX/sizeof(template_creator::OriginalRuntimeHostConfiguration::LibraryMetadataOccurrence)||
                    !CaptureBytes(sink,36,routeOrdinal,configuration->PrivateLibraryMetadataOccurrences(),configuration->LibraryMetadataOccurrenceCount()*sizeof(template_creator::OriginalRuntimeHostConfiguration::LibraryMetadataOccurrence)))return false;
                if(!CaptureBytes(sink,46,routeOrdinal,configuration->PrivateLibraryTraversalOriginal(),configuration->LibraryTraversalOriginalBytes()))return false;
                const template_creator::OriginalJson* documents[]={&configuration->PrivateRuntimeConfigSyntax(),&configuration->PrivateDepsSyntax(),configuration->PrivateDevConfigSyntax()};
                const template_creator::OriginalRuntimeHostConfiguration::Document* originals[]={&configuration->PrivateRuntimeConfigOriginal(),&configuration->PrivateDepsOriginal(),&configuration->PrivateDevConfigOriginal()};
                for(SIZE_T document=0;document<3;++document){
                    if(!documents[document])continue;
                    if(documents[document]->Count()>SIZE_MAX/sizeof(template_creator::OriginalJson::Token))return false;
                    if(!CaptureBytes(sink,32,routeOrdinal,originals[document],sizeof(*originals[document])) ||
                        !CaptureBytes(sink,33,routeOrdinal,documents[document]->PrivateTokens(),documents[document]->Count()*sizeof(template_creator::OriginalJson::Token)) ||
                        (originals[document]->spanObserved&&!CaptureBytes(sink,34,routeOrdinal,originals[document]->body,originals[document]->extent)))return false;
                }
            }
        }
        for(SIZE_T i=0;i<routeRow.gate->ParserInputCount();++i) {
            const auto& parser=routeRow.gate->PrivateParserInput(i);
            const auto& generated=parser.PrivateCopy().PrivateGenerated();
            for(SIZE_T f=0;f<generated.FileCount();++f)if(generated.PrivateFiles()[f].readCompletionUnknown)return false;
            if(generated.FileCount()>SIZE_MAX/sizeof(template_creator::HeldNativeFile) ||
                generated.ReadCount()>SIZE_MAX/sizeof(template_creator::NativeRead) ||
                generated.RetainedReadByteCount()>generated.ReadByteCapacity())return false;
            if(!CaptureBytes(sink,21,routeOrdinal,&parser,sizeof(parser)) ||
                !CaptureBytes(sink,22,routeOrdinal,generated.PrivateFiles(),generated.FileCount()*sizeof(template_creator::HeldNativeFile)) ||
                !CaptureBytes(sink,23,routeOrdinal,generated.PrivateReads(),generated.ReadCount()*sizeof(template_creator::NativeRead)) ||
                !CaptureBytes(sink,24,routeOrdinal,generated.PrivateReadBytes(),generated.RetainedReadByteCount()))return false;
        }
        const auto captureCode=[&](const template_creator::OriginalAliasCodeGuard& guard) noexcept {
            const auto& generated=guard.PrivateCopy().PrivateGenerated();
            for(SIZE_T i=0;i<generated.FileCount();++i)if(generated.PrivateFiles()[i].readCompletionUnknown)return false;
            if(generated.FileCount()>SIZE_MAX/sizeof(template_creator::HeldNativeFile) ||
                generated.ReadCount()>SIZE_MAX/sizeof(template_creator::NativeRead) ||
                generated.RetainedReadByteCount()>generated.ReadByteCapacity())return false;
            if(!CaptureBytes(sink,13,routeOrdinal,&guard,sizeof(guard)) ||
                !CaptureBytes(sink,14,routeOrdinal,generated.PrivateFiles(),generated.FileCount()*sizeof(template_creator::HeldNativeFile)) ||
                !CaptureBytes(sink,15,routeOrdinal,generated.PrivateReads(),generated.ReadCount()*sizeof(template_creator::NativeRead)) ||
                !CaptureBytes(sink,16,routeOrdinal,generated.PrivateReadBytes(),generated.RetainedReadByteCount()))return false;
            return true;
        };
        for(SIZE_T kind=0;kind<2;++kind)for(SIZE_T i=0;i<routeRow.gate->AliasCodeGuardCount(kind);++i)
            if(!captureCode(routeRow.gate->PrivateAliasCodeGuard(kind,i)))return false;
        for(SIZE_T kind=0;kind<2;++kind)for(SIZE_T i=0;i<routeRow.gate->HelperCodeGuardCount(kind);++i) {
            const auto& guard=routeRow.gate->PrivateHelperCodeGuard(kind,i);
            if(!CaptureBytes(sink,19,routeOrdinal,&guard,sizeof(guard)) || !captureCode(guard.PrivateHelper()))return false;
            if(guard.PrivateEcho()&&!captureCode(*guard.PrivateEcho()))return false;
        }
        return true;
    };
    if (captured) captured = captureGate(0);
    if (captured) captured = captureGate(ordinal+1);
    if (!captured) {
        qualificationFailed = true;
        if (!PyErr_Occurred()) PyErr_SetString(PyExc_RuntimeError,"original native command observation capture failed");
        return nullptr;
    }
    Py_RETURN_NONE; // Durable readback is a distinct later native operation.
}
}
#endif

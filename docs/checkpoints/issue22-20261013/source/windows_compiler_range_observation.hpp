// Structural compiler-range candidate only; no source/compiler/currentness or continuation authority.
#pragma once
#include "windows_msf_source_owner.hpp"
#include "windows_coreclr_compiled_geometry.hpp"
#include "windows_tls_observation.hpp"
namespace template_creator {
struct OriginalCompilerRangeObservation {
    enum class Result {Unobserved,MissingMetadata,Translated,SectionMismatch,InvalidProcedure,CodeMismatch,Observed};
    const OriginalHeldMsfSource* artifactOriginal=nullptr;
    const OriginalCoreClrCompiledGeometry* geometryOriginal=nullptr;
    const LoadedImageObservation* imageOriginal=nullptr;
    Result result=Result::Unobserved;
    SIZE_T sectionsVisited=0,symbolsVisited=0,proceduresMatched=0,procedureByteSum=0;
    SIZE_T firstFailureOrdinal=SIZE_MAX;
    bool attempted=false;
    bool Observe(const OriginalHeldMsfSource& artifact,const OriginalCoreClrCompiledGeometry& geometry,
                 const LoadedImageObservation& image) noexcept {
        if(attempted)return false;
        attempted=true;artifactOriginal=&artifact;geometryOriginal=&geometry;imageOriginal=&image;
        result=Result::MissingMetadata;
        const auto& debug=artifact.OriginalOptionalDebug();
        const unsigned char* artifactBody=nullptr;SIZE_T artifactExtent=0;
        if(artifact.FilesOriginal()!=geometry.filesOriginal||!artifact.RequirementOriginal()||
           !geometry.filesOriginal||artifact.FileOrdinal()>=geometry.filesOriginal->FileCount()||
           geometry.filesOriginal->PrivateFiles()[artifact.FileOrdinal()].closeAttempted||
           !geometry.filesOriginal->ExactCapturedSourceSpan(artifact.FileOrdinal(),artifactBody,artifactExtent)||
           artifactBody!=artifact.BodyOriginal()||artifactExtent!=artifact.BodyExtent()||
           !artifact.SymbolRosterComplete()||!debug.complete||!debug.sectionHeadersObserved||
           !geometry.derived||!image.complete||image.failed||!image.bytes||
           image.imageSize!=geometry.nt.OptionalHeader.SizeOfImage||image.used!=image.imageSize||
           image.retainedByteExtent<image.imageSize||image.imageSize>image.byteCapacity)return false;
        if(debug.omapToSourcePresent||debug.omapFromSourcePresent){result=Result::Translated;return false;}
        result=Result::SectionMismatch;
        if(debug.sectionCount!=geometry.nt.FileHeader.NumberOfSections||
           geometry.sectionOffset>image.imageSize||debug.sectionCount>(image.imageSize-geometry.sectionOffset)/sizeof(IMAGE_SECTION_HEADER))return false;
        for(SIZE_T i=0;i<debug.sectionCount;++i){
            firstFailureOrdinal=i;IMAGE_SECTION_HEADER metadata{},original{},mapped{};
            if(!artifact.ReadOriginalSectionHeader(i,false,metadata)||!geometry.OriginalSectionHeader(i,original))return false;
            memcpy(&mapped,image.bytes.get()+geometry.sectionOffset+i*sizeof(mapped),sizeof(mapped));
            if(memcmp(&metadata,&original,sizeof(original))||memcmp(&mapped,&original,sizeof(original)))return false;
            if(debug.originalSectionHeadersObserved){
                IMAGE_SECTION_HEADER before{};
                if(debug.originalSectionCount!=debug.sectionCount||
                   !artifact.ReadOriginalSectionHeader(i,true,before)||memcmp(&before,&original,sizeof(original)))return false;
            }
            ++sectionsVisited;
        }
        result=Result::InvalidProcedure;
        const auto* symbols=artifact.OriginalSymbols();
        for(SIZE_T i=0;i<artifact.SymbolCount();++i){
            firstFailureOrdinal=i;const auto& symbol=symbols[i];++symbolsVisited;
            if(!symbol.complete)return false;
            if(!symbol.procedure)continue;
            if(!symbol.codeSection||symbol.codeSection>debug.sectionCount||!symbol.codeBytes)return false;
            IMAGE_SECTION_HEADER section{};
            if(!geometry.OriginalSectionHeader(symbol.codeSection-1,section)||
               !(section.Characteristics&IMAGE_SCN_MEM_EXECUTE))return false;
            const SIZE_T extent=section.Misc.VirtualSize>section.SizeOfRawData?section.Misc.VirtualSize:section.SizeOfRawData;
            if(symbol.codeOffset>extent||symbol.codeBytes>extent-symbol.codeOffset||
               symbol.codeOffset>section.SizeOfRawData||symbol.codeBytes>section.SizeOfRawData-symbol.codeOffset||
               symbol.codeOffset>UINT32_MAX-section.VirtualAddress||symbol.codeBytes>SIZE_MAX-procedureByteSum)return false;
            const DWORD rva=section.VirtualAddress+symbol.codeOffset;
            if(!geometry.MatchesMappedCodeSpan(image,rva,symbol.codeBytes)){result=Result::CodeMismatch;return false;}
            procedureByteSum+=symbol.codeBytes;++proceduresMatched;
        }
        if(!proceduresMatched){result=Result::MissingMetadata;return false;}
        firstFailureOrdinal=SIZE_MAX;result=Result::Observed;return true;
        // Duplicate/overlapping symbols and unknown record kinds remain retained, not a complete graph.
    }
};
// This connects actual TLS targets to retained compiler/source candidates. It
// deliberately does not assert compiler authenticity, callback execution,
// exact source-line attribution or closure of indirect CRT/init actor graphs.
struct OriginalCompilerTlsCandidates {
    enum class Result {Unobserved,MissingRelation,AbsentTls,MalformedTarget,AmbiguousProcedure,
        MissingCompiler,MissingSourceFragment,Arithmetic,Candidates};
    const OriginalCompilerRangeObservation* rangesOriginal=nullptr;
    const OriginalLoadedTlsObservation* tlsOriginal=nullptr;
    SIZE_T callbacksVisited=0,symbolVisits=0,lineBlockVisits=0;
    SIZE_T compilerRecordsMatched=0,sourceFragmentsMatched=0,firstFailureCallback=SIZE_MAX;
    Result result=Result::Unobserved;
    bool attempted=false;
    bool Observe(const OriginalCompilerRangeObservation& ranges,const OriginalLoadedTlsObservation& tls) noexcept {
        if(attempted)return false;attempted=true;rangesOriginal=&ranges;tlsOriginal=&tls;
        result=Result::MissingRelation;
        if(ranges.result!=OriginalCompilerRangeObservation::Result::Observed||!ranges.artifactOriginal||
           !ranges.geometryOriginal||!ranges.imageOriginal||tls.imageOriginal!=ranges.imageOriginal)return false;
        if(tls.outcome==OriginalLoadedTlsObservation::Outcome::Absent){result=Result::AbsentTls;return true;}
        if(tls.outcome!=OriginalLoadedTlsObservation::Outcome::Complete)return false;
        const auto& artifact=*ranges.artifactOriginal;const auto& image=*ranges.imageOriginal;
        if(!artifact.SymbolRosterComplete()||!artifact.LineBlocksComplete())return false;
        const auto increment=[&](SIZE_T& count) noexcept {
            if(count==SIZE_MAX){result=Result::Arithmetic;return false;}++count;return true;
        };
        const auto base=reinterpret_cast<std::uintptr_t>(image.nativeBase);
        if(!tls.callbackCount&&!tls.callbackArrayExtent&&!tls.directory.AddressOfCallBacks){
            result=Result::Candidates;return true;
        }
        if(tls.callbackArrayOffset>image.imageSize||tls.callbackArrayExtent>image.imageSize-tls.callbackArrayOffset||
           tls.callbackCount>=tls.callbackArrayExtent/sizeof(ULONGLONG))return false;
        for(SIZE_T callback=0;callback<tls.callbackCount;++callback){
            firstFailureCallback=callback;ULONGLONG address=0;
            memcpy(&address,image.bytes.get()+tls.callbackArrayOffset+callback*sizeof(address),sizeof(address));
            if(!increment(callbacksVisited))return false;
            result=Result::MalformedTarget;
            if(address<base||address-base>=image.imageSize||address-base>UINT32_MAX)return false;
            const DWORD rva=static_cast<DWORD>(address-base);
            const OriginalHeldMsfSource::SymbolOccurrence* procedure=nullptr;SIZE_T matches=0;
            for(SIZE_T i=0;i<artifact.SymbolCount();++i){
                if(!increment(symbolVisits))return false;
                const auto& symbol=artifact.OriginalSymbols()[i];
                if(!symbol.complete||!symbol.procedure||!symbol.codeSection)continue;
                IMAGE_SECTION_HEADER section{};
                if(!ranges.geometryOriginal->OriginalSectionHeader(symbol.codeSection-1,section)||
                   symbol.codeOffset>UINT32_MAX-section.VirtualAddress)return false;
                if(section.VirtualAddress+symbol.codeOffset==rva){procedure=&symbol;++matches;}
            }
            result=Result::AmbiguousProcedure;if(matches!=1||!procedure)return false;
            SIZE_T compilerMatches=0,sourceMatches=0;
            for(SIZE_T i=0;i<artifact.SymbolCount();++i){
                if(!increment(symbolVisits))return false;
                const auto& symbol=artifact.OriginalSymbols()[i];
                if(symbol.complete&&symbol.compile3&&symbol.moduleOrdinal==procedure->moduleOrdinal&&
                   symbol.compilerMachine==0x00d0){
                    ++compilerMatches;if(!increment(compilerRecordsMatched))return false;
                }
            }
            result=Result::MissingCompiler;if(!compilerMatches)return false;
            for(SIZE_T i=0;i<artifact.LineBlockCount();++i){
                if(!increment(lineBlockVisits))return false;
                const auto& line=artifact.OriginalLineBlocks()[i];
                if(!line.complete||!line.lineCount||line.moduleOrdinal!=procedure->moduleOrdinal||line.codeSection!=procedure->codeSection||
                   procedure->codeOffset<line.codeOffset||procedure->codeOffset-line.codeOffset>=line.codeBytes||
                   !line.sourceCorrespondenceAttempted||!line.sourceChecksumCorrespondenceObserved||
                   line.heldSourceOrdinal==SIZE_MAX||!line.heldSourceRequirementOriginal)continue;
                ++sourceMatches;if(!increment(sourceFragmentsMatched))return false;
            }
            result=Result::MissingSourceFragment;if(!sourceMatches)return false;
        }
        firstFailureCallback=SIZE_MAX;result=Result::Candidates;return true;
    }
};
struct OriginalCompilerInstructionCandidate {
    const OriginalCompilerRangeObservation* rangesOriginal=nullptr;
    const OriginalAmd64Instruction* instructionOriginal=nullptr;
    SIZE_T procedureMatches=0,procedureOrdinal=SIZE_MAX,sourceFragmentMatches=0;
    SIZE_T firstSourceBlockOrdinal=SIZE_MAX;
    OriginalAmd64Instruction heldInstructionOriginal{};
    bool heldDecodeObserved=false;
    bool attempted=false,candidateObserved=false;
    bool Observe(const OriginalCompilerRangeObservation& ranges,const OriginalAmd64Instruction& instruction) noexcept {
        if(attempted)return false;attempted=true;rangesOriginal=&ranges;instructionOriginal=&instruction;
        if(ranges.result!=OriginalCompilerRangeObservation::Result::Observed||!ranges.artifactOriginal||
           !ranges.geometryOriginal||!ranges.imageOriginal||!instruction.length)return false;
        const auto& artifact=*ranges.artifactOriginal;
        if(!ranges.geometryOriginal->DecodeHeldInstruction(instruction.rva,heldInstructionOriginal))return false;
        const auto& held=heldInstructionOriginal;
        heldDecodeObserved=held.rva==instruction.rva&&held.length==instruction.length&&held.flow==instruction.flow&&
            held.relative==instruction.relative&&held.opcode==instruction.opcode&&held.opcode2==instruction.opcode2&&
            held.rex==instruction.rex&&held.modrm==instruction.modrm&&held.hasModrm==instruction.hasModrm&&
            held.ripRelative==instruction.ripRelative&&held.registerOperand==instruction.registerOperand&&
            held.displacement==instruction.displacement;
        if(!heldDecodeObserved)return false;
        for(SIZE_T i=0;i<artifact.SymbolCount();++i){const auto& symbol=artifact.OriginalSymbols()[i];
            IMAGE_SECTION_HEADER section{};
            if(!symbol.complete||!symbol.procedure||!symbol.codeSection||!symbol.codeBytes||
               !ranges.geometryOriginal->OriginalSectionHeader(symbol.codeSection-1,section)||
               symbol.codeOffset>UINT32_MAX-section.VirtualAddress)continue;
            const DWORD start=section.VirtualAddress+symbol.codeOffset;
            if(instruction.rva>=start&&SIZE_T(instruction.rva)-start<=symbol.codeBytes&&
               instruction.length<=symbol.codeBytes-(SIZE_T(instruction.rva)-start)){
                ++procedureMatches;procedureOrdinal=i;
            }
        }
        if(procedureMatches!=1||!artifact.LineBlocksComplete())return false;
        const auto& procedure=artifact.OriginalSymbols()[procedureOrdinal];
        IMAGE_SECTION_HEADER section{};
        if(!ranges.geometryOriginal->OriginalSectionHeader(procedure.codeSection-1,section)||
           instruction.rva<section.VirtualAddress)return false;
        const DWORD offset=instruction.rva-section.VirtualAddress;
        for(SIZE_T i=0;i<artifact.LineBlockCount();++i){const auto& line=artifact.OriginalLineBlocks()[i];
            if(line.complete&&line.lineCount&&line.moduleOrdinal==procedure.moduleOrdinal&&
               line.codeSection==procedure.codeSection&&line.sourceChecksumCorrespondenceObserved&&
               line.heldSourceOrdinal!=SIZE_MAX&&line.heldSourceRequirementOriginal&&offset>=line.codeOffset&&
               offset-line.codeOffset<=line.codeBytes&&instruction.length<=line.codeBytes-(offset-line.codeOffset)){
                if(!sourceFragmentMatches)firstSourceBlockOrdinal=i;++sourceFragmentMatches;
            }
        }
        candidateObserved=sourceFragmentMatches!=0;return candidateObserved;
        // Trace-node membership only. Fragment coverage does not identify an instruction's source semantics.
    }
};
struct OriginalCompilerPostWriterSourceCandidate {
    const OriginalCompilerRangeObservation* rangesOriginal=nullptr;
    DWORD64 addressOriginal=0;
    SIZE_T heldSourceOrdinal=SIZE_MAX,procedureMatches=0,procedureOrdinal=SIZE_MAX;
    const unsigned char* heldSourceBodyOriginal=nullptr;
    SIZE_T heldSourceExtentOriginal=0;
    SIZE_T sourcePositionMatches=0,positionsVisited=0;
    OriginalHeldMsfSource::LinePositionOriginal firstPositionOriginal;
    bool attempted=false,candidateObserved=false;
    bool Observe(const OriginalCompilerRangeObservation& ranges,DWORD64 address,SIZE_T sourceOrdinal,
                 const unsigned char* sourceBody,SIZE_T sourceExtent) noexcept {
        if(attempted)return false;attempted=true;rangesOriginal=&ranges;
        addressOriginal=address;heldSourceOrdinal=sourceOrdinal;
        heldSourceBodyOriginal=sourceBody;heldSourceExtentOriginal=sourceExtent;
        if(ranges.result!=OriginalCompilerRangeObservation::Result::Observed||!ranges.artifactOriginal||
           !ranges.geometryOriginal||!ranges.imageOriginal||sourceOrdinal==SIZE_MAX)return false;
        const auto& artifact=*ranges.artifactOriginal;const auto& image=*ranges.imageOriginal;
        const unsigned char* heldBody=nullptr;SIZE_T heldExtent=0;
        if(!sourceBody||!sourceExtent||!artifact.FilesOriginal()||
           !artifact.FilesOriginal()->ExactCapturedSourceSpan(sourceOrdinal,heldBody,heldExtent)||
           heldBody!=sourceBody||heldExtent!=sourceExtent)return false;
        const auto base=reinterpret_cast<std::uintptr_t>(image.nativeBase);
        if(address<base||address-base>=image.used||address-base>UINT32_MAX)return false;
        const DWORD rva=static_cast<DWORD>(address-base);
        for(SIZE_T i=0;i<artifact.SymbolCount();++i){const auto& symbol=artifact.OriginalSymbols()[i];
            IMAGE_SECTION_HEADER section{};
            if(!symbol.complete||!symbol.procedure||!symbol.codeSection||!symbol.codeBytes||
               !ranges.geometryOriginal->OriginalSectionHeader(symbol.codeSection-1,section)||
               symbol.codeOffset>UINT32_MAX-section.VirtualAddress)continue;
            const DWORD start=section.VirtualAddress+symbol.codeOffset;
            if(rva>=start&&SIZE_T(rva)-start<symbol.codeBytes){++procedureMatches;procedureOrdinal=i;}
        }
        if(procedureMatches!=1)return false;
        const auto& procedure=artifact.OriginalSymbols()[procedureOrdinal];
        IMAGE_SECTION_HEADER section{};
        if(!ranges.geometryOriginal->OriginalSectionHeader(procedure.codeSection-1,section)||rva<section.VirtualAddress)return false;
        const DWORD offset=rva-section.VirtualAddress;
        if(!artifact.LineBlocksComplete())return false;
        for(SIZE_T i=0;i<artifact.LineBlockCount();++i){const auto& block=artifact.OriginalLineBlocks()[i];
            if(!block.complete||block.moduleOrdinal!=procedure.moduleOrdinal||block.codeSection!=procedure.codeSection||
               !block.sourceChecksumCorrespondenceObserved||block.heldSourceOrdinal!=sourceOrdinal||
               !block.heldSourceRequirementOriginal||offset<block.codeOffset||offset-block.codeOffset>=block.codeBytes)continue;
            for(SIZE_T j=0;j<block.lineCount;++j){
                OriginalHeldMsfSource::LinePositionOriginal position;
                if(!artifact.ReadOriginalLinePosition(i,j,position)||positionsVisited==SIZE_MAX)return false;
                ++positionsVisited;
                // Exact start only. No next-line ordering or interval attribution is guessed.
                if(position.alwaysStep||position.neverStep||position.sectionOffset!=offset||
                   position.startLine<68||position.startLine>79)continue;
                if(sourcePositionMatches==SIZE_MAX)return false;
                if(!sourcePositionMatches)firstPositionOriginal=position;
                ++sourcePositionMatches;
            }
        }
        candidateObserved=sourcePositionMatches!=0;return candidateObserved;
        // A line start is a locator; authentic compiled semantics and full graph remain separate.
    }
};
// Source-declared names are a candidate locator, never proof this is the selected CRT build.
struct OriginalCompilerTlsInitializerCandidates {
    const OriginalCompilerRangeObservation* rangesOriginal=nullptr;
    SIZE_T beginMatches=0,endMatches=0,beginSymbolOrdinal=SIZE_MAX,endSymbolOrdinal=SIZE_MAX;
    const OriginalHeldMsfSource::SymbolOccurrence* beginSymbolOriginal=nullptr;
    const OriginalHeldMsfSource::SymbolOccurrence* endSymbolOriginal=nullptr;
    SIZE_T beginModuleOriginal=SIZE_MAX,endModuleOriginal=SIZE_MAX;
    bool sentinelCandidatesConflict=false;
    SIZE_T spanOffset=0,spanExtent=0,entriesVisited=0,nonnullEntries=0,unresolvedTargets=0;
    SIZE_T sourceMatchedTargets=0,sourceUnresolvedTargets=0;
    SIZE_T sourcePositionsVisited=0,sourcePositionsAtTarget=0;
    bool attempted=false,sentinelsLocated=false,spanObserved=false;
    static bool ExactName(const OriginalHeldMsfSource& artifact,
                          const OriginalHeldMsfSource::SymbolOccurrence& symbol,const char* name,SIZE_T extent) noexcept {
        if(!symbol.complete||symbol.nameExtent!=extent)return false;
        for(SIZE_T i=0;i<extent;++i){unsigned char original=0;
            if(!artifact.ReadStream(symbol.streamOrdinal,symbol.nameOffset+i,&original,1)||
               original!=static_cast<unsigned char>(name[i]))return false;
        }
        return true;
    }
    bool Observe(const OriginalCompilerRangeObservation& ranges) noexcept {
        if(attempted)return false;attempted=true;rangesOriginal=&ranges;
        if(ranges.result!=OriginalCompilerRangeObservation::Result::Observed||!ranges.artifactOriginal||
           !ranges.geometryOriginal||!ranges.imageOriginal)return false;
        const auto& artifact=*ranges.artifactOriginal;const auto& image=*ranges.imageOriginal;
        const auto* symbols=artifact.OriginalSymbols();
        const auto retain=[&](const OriginalHeldMsfSource::SymbolOccurrence& symbol,SIZE_T ordinal,SIZE_T module){
            if(!symbol.complete||!symbol.data||module==SIZE_MAX)return;
            const auto match=[&](const char* name,SIZE_T extent,SIZE_T& count,SIZE_T& selectedOrdinal,
                                 const OriginalHeldMsfSource::SymbolOccurrence*& selected,SIZE_T& selectedModule){
                if(!ExactName(artifact,symbol,name,extent))return;
                ++count;
                if(selected){if(selectedModule!=module||selected->dataSection!=symbol.dataSection||
                    selected->dataOffset!=symbol.dataOffset||selected->dataTypeOrPublicFlags!=symbol.dataTypeOrPublicFlags)
                    sentinelCandidatesConflict=true;
                }else{selected=&symbol;selectedOrdinal=ordinal;selectedModule=module;}
            };
            match("__xd_a",sizeof("__xd_a"),beginMatches,beginSymbolOrdinal,beginSymbolOriginal,beginModuleOriginal);
            match("__xd_z",sizeof("__xd_z"),endMatches,endSymbolOrdinal,endSymbolOriginal,endModuleOriginal);
        };
        for(SIZE_T i=0;i<artifact.SymbolCount();++i)retain(symbols[i],i,symbols[i].moduleOrdinal);
        if(artifact.GlobalSymbolRecordsComplete())for(SIZE_T i=0;i<artifact.GlobalSymbolCount();++i){
            const auto& symbol=artifact.OriginalGlobalSymbols()[i];
            if(symbol.contributionComparisonAttempted&&symbol.contributionMatches==1)
                retain(symbol,i,symbol.contributionModuleOrdinal);
        }
        if(!beginSymbolOriginal||!endSymbolOriginal||sentinelCandidatesConflict)return false;
        const auto& begin=*beginSymbolOriginal;const auto& end=*endSymbolOriginal;
        if(beginModuleOriginal!=endModuleOriginal||
           !begin.dataSection||begin.dataSection!=end.dataSection||end.dataOffset<=begin.dataOffset)return false;
        IMAGE_SECTION_HEADER section{};
        if(!ranges.geometryOriginal->OriginalSectionHeader(begin.dataSection-1,section))return false;
        const SIZE_T sectionExtent=section.Misc.VirtualSize>section.SizeOfRawData?
            section.Misc.VirtualSize:section.SizeOfRawData;
        if(end.dataOffset>sectionExtent||sizeof(ULONGLONG)>sectionExtent-end.dataOffset||
           begin.dataOffset>UINT32_MAX-section.VirtualAddress)return false;
        spanOffset=section.VirtualAddress+begin.dataOffset;
        spanExtent=SIZE_T(end.dataOffset)-begin.dataOffset+sizeof(ULONGLONG);
        if(spanOffset%sizeof(ULONGLONG)||spanOffset>image.used||spanExtent>image.used-spanOffset||
           spanExtent%sizeof(ULONGLONG))return false;
        sentinelsLocated=true;
        ULONGLONG first=0,last=0;
        memcpy(&first,image.bytes.get()+spanOffset,sizeof(first));
        memcpy(&last,image.bytes.get()+spanOffset+spanExtent-sizeof(last),sizeof(last));
        if(first||last)return false;
        const auto base=reinterpret_cast<std::uintptr_t>(image.nativeBase);
        for(SIZE_T at=sizeof(ULONGLONG);at<spanExtent-sizeof(ULONGLONG);at+=sizeof(ULONGLONG)){
            ULONGLONG target=0;memcpy(&target,image.bytes.get()+spanOffset+at,sizeof(target));++entriesVisited;
            if(!target)continue;++nonnullEntries;SIZE_T matches=0;
            const OriginalHeldMsfSource::SymbolOccurrence* selectedProcedure=nullptr;
            if(target>=base&&target-base<image.used){
                for(SIZE_T j=0;j<artifact.SymbolCount();++j){const auto& procedure=symbols[j];
                    IMAGE_SECTION_HEADER code{};
                    if(procedure.complete&&procedure.procedure&&procedure.codeSection&&
                       ranges.geometryOriginal->OriginalSectionHeader(procedure.codeSection-1,code)&&
                       procedure.codeOffset<=UINT32_MAX-code.VirtualAddress&&
                       code.VirtualAddress+procedure.codeOffset==target-base){++matches;selectedProcedure=&procedure;}
                }
            }
            if(matches!=1){++unresolvedTargets;continue;}
            bool sourceCandidate=false;
            if(artifact.LineBlocksComplete())for(SIZE_T j=0;j<artifact.LineBlockCount();++j){
                const auto& line=artifact.OriginalLineBlocks()[j];
                if(line.complete&&line.lineCount&&line.moduleOrdinal==selectedProcedure->moduleOrdinal&&
                   line.codeSection==selectedProcedure->codeSection&&selectedProcedure->codeOffset>=line.codeOffset&&
                   selectedProcedure->codeOffset-line.codeOffset<line.codeBytes&&line.sourceCorrespondenceAttempted&&
                   line.sourceChecksumCorrespondenceObserved&&line.heldSourceOrdinal!=SIZE_MAX&&
                   line.heldSourceRequirementOriginal){
                    sourceCandidate=true;
                    for(SIZE_T position=0;position<line.lineCount;++position){
                        OriginalHeldMsfSource::LinePositionOriginal original;
                        if(!artifact.ReadOriginalLinePosition(j,position,original))return false;
                        if(sourcePositionsVisited==SIZE_MAX)return false;++sourcePositionsVisited;
                        if(!original.alwaysStep&&!original.neverStep&&
                           original.sectionOffset==selectedProcedure->codeOffset){
                            if(sourcePositionsAtTarget==SIZE_MAX)return false;++sourcePositionsAtTarget;
                        }
                    }
                }
            }
            if(sourceCandidate)++sourceMatchedTargets;else ++sourceUnresolvedTargets;
        }
        spanObserved=true;return unresolvedTargets==0&&sourceUnresolvedTargets==0;
        // No snapshot-to-lifetime, name-to-source or procedure-to-effect promotion.
    }
};
}

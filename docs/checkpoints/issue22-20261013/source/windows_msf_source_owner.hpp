// Optional held MSF7 source owner. Structural success is never build/source authority.
#pragma once
#include "windows_held_files.hpp"
#include "windows_source_checksum_algorithms.hpp"
#include <cstdint>
#include <memory>
#include <cstring>
#include <array>
namespace template_creator {
struct OriginalHeldSourceDigest {
    const NativeHeldFiles* filesOriginal=nullptr;
    const void* requirementOriginal=nullptr;
    const unsigned char* bodyOriginal=nullptr;
    SIZE_T ordinal=SIZE_MAX,extent=0;
    OriginalSourceChecksumAlgorithms algorithms;
    bool attempted=false,complete=false;
    void Observe(const NativeHeldFiles& files,SIZE_T selected,const void* requirement) noexcept {
        if(attempted)return;
        attempted=true;filesOriginal=&files;ordinal=selected;requirementOriginal=requirement;
        if(!requirement||selected>=files.FileCount()||files.PrivateFiles()[selected].closeAttempted||
           !files.ExactCapturedSourceSpan(selected,bodyOriginal,extent))return;
        complete=algorithms.Compute(bodyOriginal,extent);
    }
};
class OriginalHeldMsfSource {
#if defined(TEMPLATE_NATIVE_MSF_SYNTHETIC_REGRESSION)
    friend class OriginalMsfSyntheticRegression;
#endif
public:
    enum class Result {Unobserved,Unavailable,NotMsf,Unsupported,Malformed,Geometry};
    struct Stream {std::uint32_t size=UINT32_MAX;SIZE_T directoryBlocksOffset=0,blocks=0;};
    struct InfoHeader {
        std::array<unsigned char,28> original{};
        std::uint32_t version=0,signature=0,age=0;
        std::array<unsigned char,16> guid{};
        bool attempted=false,read=false,supported=false;
    };
    struct DbiHeader {
        std::array<unsigned char,64> original{};
        std::array<std::uint32_t,7> substreamSizes{};
        std::uint32_t versionSignature=0,version=0,age=0;
        std::uint16_t globalStream=UINT16_MAX,publicStream=UINT16_MAX,symbolStream=UINT16_MAX;
        std::uint16_t buildNumber=0,pdbDllVersion=0,pdbDllRebuild=0,flags=0,machine=0;
        SIZE_T retainedExtent=0;
        bool attempted=false,read=false,extentsObserved=false,supported=false,privateSymbolsStripped=false;
    };
    struct SectionContribution {
        std::array<unsigned char,32> original{};
        SIZE_T streamOffset=0,originalExtent=0;
        std::uint16_t section=0,moduleIndex=0;
        std::uint32_t offsetBits=0,sizeBits=0,characteristics=0,dataCrc=0,relocationCrc=0,coffSection=0;
        bool read=false,nonnegativeRange=false;
    };
    struct Module {
        std::array<unsigned char,64> original{};
        SIZE_T dbiOffset=0,moduleNameOffset=0,moduleNameExtent=0,objectNameOffset=0,objectNameExtent=0;
        std::uint16_t symbolStream=UINT16_MAX,sourceFileCount=0,flags=0,section=0;
        std::uint32_t symbolBytes=0,c11Bytes=0,c13Bytes=0,sectionOffset=0,sectionBytes=0;
        SIZE_T c13Subsections=0,c13UnknownSubsections=0,c13IgnoredSubsections=0;
        SIZE_T checksumSubsections=0,checksumOffset=0,checksumExtent=0;
        SIZE_T stringSubsections=0,stringOffset=0,stringExtent=0;
        bool c13Attempted=false,c13Complete=false;
        SIZE_T checksumBegin=0,checksumCount=0;
        bool extentsObserved=false;
        std::uint32_t symbolSignature=0;
        SIZE_T symbolRecords=0,procedureRecords=0,compile3Records=0,dataRecords=0,publicRecords=0,unknownSymbolRecords=0;
        SIZE_T symbolCursorOriginal=0;
        std::uint16_t symbolLengthOriginal=0,symbolKindOriginal=0;
        std::array<unsigned char,22> compile3Original{};
        std::array<unsigned char,35> procedureOriginal{};
        std::array<unsigned char,10> dataOrPublicOriginal{};
        SIZE_T dataOrPublicOffsetOriginal=0;
        SIZE_T compile3OffsetOriginal=0,procedureOffsetOriginal=0;
        bool symbolsAttempted=false,symbolsComplete=false;
    };
    struct SourceChecksum {
        SIZE_T moduleOrdinal=SIZE_MAX,moduleStreamOffset=0;
        std::uint32_t fileNameOffset=0;
        unsigned char kind=0,size=0;
        std::array<unsigned char,32> bytes{};
        bool originalCopied=false,supportedShape=false;
        SIZE_T nameStream=SIZE_MAX,nameOffset=0,nameExtent=0;
        bool nameObserved=false;
        SIZE_T heldSourceMatches=0,heldSourceOrdinal=SIZE_MAX;
        const void* heldSourceRequirementOriginal=nullptr;
        bool sourceComparisonAttempted=false;
    };
    struct SymbolOccurrence {
        SIZE_T moduleOrdinal=SIZE_MAX,streamOrdinal=SIZE_MAX,streamOffset=0,payloadOffset=0,payloadExtent=0;
        SIZE_T nameOffset=0,nameExtent=0;
        std::uint16_t originalLength=0,kind=0;
        std::array<unsigned char,35> fixedOriginal{};
        SIZE_T fixedExtent=0;
        std::uint32_t codeBytes=0,codeOffset=0;
        std::uint16_t codeSection=0,compilerMachine=0;
        std::uint32_t dataTypeOrPublicFlags=0,dataOffset=0;
        std::uint16_t dataSection=0;
        SIZE_T contributionMatches=0,contributionOrdinal=SIZE_MAX,contributionModuleOrdinal=SIZE_MAX;
        bool contributionComparisonAttempted=false;
        bool procedure=false,compile3=false,data=false,publicSymbol=false,complete=false;
    };
    struct LineBlockOccurrence {
        SIZE_T moduleOrdinal=SIZE_MAX,fragmentOffset=0,blockOffset=0,linesOffset=0,columnsOffset=0;
        std::array<unsigned char,12> fragmentOriginal{},blockOriginal{};
        std::uint32_t codeOffset=0,codeBytes=0,checksumOffset=0,lineCount=0,blockBytes=0;
        std::uint16_t codeSection=0,flags=0;
        SIZE_T checksumMatches=0,checksumOrdinal=SIZE_MAX;
        SIZE_T heldSourceOrdinal=SIZE_MAX;
        const void* heldSourceRequirementOriginal=nullptr;
        bool sourceCorrespondenceAttempted=false,sourceChecksumCorrespondenceObserved=false;
        bool complete=false;
    };
    struct LinePositionOriginal {
        const OriginalHeldMsfSource* artifactOriginal=nullptr;
        SIZE_T blockOrdinal=SIZE_MAX,lineOrdinal=SIZE_MAX,streamOrdinal=SIZE_MAX,streamOffset=0;
        std::array<unsigned char,8> line{};
        std::array<unsigned char,4> columns{};
        std::uint32_t relativeOffset=0,sectionOffset=0,rawFlags=0,startLine=0,endLine=0;
        std::uint16_t startColumn=0,endColumn=0;
        bool attempted=false,read=false,columnsRead=false,statement=false,alwaysStep=false,neverStep=false;
    };
    struct NamedMap {
        SIZE_T stringOffset=0,stringExtent=0,presentOffset=0,presentWords=0,deletedOffset=0,deletedWords=0;
        SIZE_T entriesOffset=0,entriesVisited=0,featureOffset=0,featureExtent=0;
        std::uint32_t size=0,capacity=0;
        SIZE_T globalNamesMatches=0,globalNamesStream=SIZE_MAX;
        bool attempted=false,complete=false;
    };
    struct GlobalStrings {
        std::array<unsigned char,12> original{};
        std::uint32_t signature=0,hashVersion=0,stringBytes=0,buckets=0,nameCount=0;
        SIZE_T stream=SIZE_MAX,bucketOffset=0,nonemptyBuckets=0;
        bool attempted=false,read=false,supported=false,complete=false;
    };
    struct OptionalDebug {
        std::array<std::uint16_t,11> streams{};
        SIZE_T offset=0,extent=0,indices=0,unknownIndices=0;
        SIZE_T sectionCount=0,originalSectionCount=0;
        bool attempted=false,complete=false,sectionHeadersObserved=false,originalSectionHeadersObserved=false;
        bool omapToSourcePresent=false,omapFromSourcePresent=false;
    };
private:
    void ObserveModuleSymbols(Module& module) noexcept {
        module.symbolsAttempted=true;
        if(!module.symbolBytes){module.symbolsComplete=true;return;}
        if(module.symbolBytes<4||!StreamU32(module.symbolStream,0,module.symbolSignature)||
           module.symbolSignature!=4)return;
        SIZE_T cursor=4;
        while(cursor<module.symbolBytes){
            module.symbolCursorOriginal=cursor;
            if(module.symbolBytes-cursor<4)return;
            unsigned char header[4]{};
            if(!ReadStream(module.symbolStream,cursor,header,4))return;
            module.symbolLengthOriginal=U16(header);module.symbolKindOriginal=U16(header+2);
            const SIZE_T length=module.symbolLengthOriginal;
            if(length<2||length>module.symbolBytes-cursor-2)return;
            const SIZE_T end=cursor+2+length,payload=cursor+4;
            const auto kind=module.symbolKindOriginal;
            const bool procedure=kind==0x110f||kind==0x1110||kind==0x1146||kind==0x1147||
                                 kind==0x1155||kind==0x1156;
            SIZE_T name=0;
            if(procedure){
                ++module.procedureRecords;
                if(end-payload<36)return;
                module.procedureOffsetOriginal=payload;
                if(!ReadStream(module.symbolStream,payload,module.procedureOriginal.data(),35))return;
                name=payload+35;
            }else if(kind==0x113c){
                ++module.compile3Records;
                if(end-payload<23)return;
                module.compile3OffsetOriginal=payload;
                if(!ReadStream(module.symbolStream,payload,module.compile3Original.data(),22))return;
                name=payload+22;
            }else if(kind==0x110c||kind==0x110d||kind==0x110e){
                if(kind==0x110e)++module.publicRecords;else ++module.dataRecords;
                if(end-payload<11)return;
                module.dataOrPublicOffsetOriginal=payload;
                if(!ReadStream(module.symbolStream,payload,module.dataOrPublicOriginal.data(),10))return;
                name=payload+10;
            }else ++module.unknownSymbolRecords;
            if(name){
                bool terminated=false;
                for(SIZE_T at=name;at<end;++at){
                    unsigned char byte=0;if(!ReadStream(module.symbolStream,at,&byte,1))return;
                    if(!byte){terminated=true;break;}
                }
                if(!terminated)return;
            }
            ++module.symbolRecords;cursor=end;
        }
        module.symbolCursorOriginal=cursor;module.symbolsComplete=cursor==module.symbolBytes;
        // Prefixes/counts only: every function/source/current mapped range is a separate consumer.
    }
    const NativeHeldFiles* filesOriginal_=nullptr;
    const void* requirementOriginal_=nullptr;
    const unsigned char* bodyOriginal_=nullptr;
    SIZE_T fileOrdinal_=SIZE_MAX,bodyExtent_=0,sourceBytes_=sizeof(OriginalHeldMsfSource);
    std::uint32_t blockSize_=0,freeMap_=0,blocks_=0,directoryBytes_=0,unknown_=0,blockMap_=0;
    SIZE_T directoryBlocks_=0,streamCount_=0,claimedBlocks_=0;
    std::unique_ptr<unsigned char[]> blockOwners_;
    std::unique_ptr<Stream[]> streams_;
    Result result_=Result::Unobserved;
    bool attempted_=false;
    InfoHeader info_;
    DbiHeader dbi_;
    std::unique_ptr<Module[]> modules_;
    SIZE_T moduleCapacity_=0,moduleCount_=0;
    bool modulesAttempted_=false,modulesComplete_=false;
    std::unique_ptr<SymbolOccurrence[]> symbols_;
    SIZE_T symbolCapacity_=0,symbolCount_=0;
    bool symbolRosterAttempted_=false,symbolRosterComplete_=false;
    std::unique_ptr<SymbolOccurrence[]> globalSymbols_;
    SIZE_T globalSymbolCount_=0,globalSymbolCursorOriginal_=0;
    SymbolOccurrence globalSymbolPrefixOriginal_;
    bool globalSymbolsAttempted_=false,globalSymbolsComplete_=false;
    std::unique_ptr<LineBlockOccurrence[]> lineBlocks_;
    SIZE_T lineBlockCapacity_=0,lineBlockCount_=0,lineFailureModule_=SIZE_MAX,lineFailureOffset_=0;
    bool linesAttempted_=false,linesComplete_=false;
    bool WalkLineBlocks(bool retain) noexcept {
        for(SIZE_T i=0;i<moduleCount_;++i){
            const auto& module=modules_[i];
            if(!module.c13Complete)return false;
            SIZE_T cursor=SIZE_T(module.symbolBytes)+module.c11Bytes;
            const SIZE_T end=cursor+module.c13Bytes;
            while(cursor<end){
                lineFailureModule_=i;lineFailureOffset_=cursor;
                unsigned char subsection[8]{};
                if(end-cursor<8||!ReadStream(module.symbolStream,cursor,subsection,8))return false;
                const auto kind=U32(subsection),length=U32(subsection+4);
                const SIZE_T start=cursor+8;
                if(length>end-start)return false;
                const SIZE_T subEnd=start+length;
                if(kind==0xf2){
                    LineBlockOccurrence fragment;
                    fragment.moduleOrdinal=i;fragment.fragmentOffset=start;
                    if(length<12||!ReadStream(module.symbolStream,start,fragment.fragmentOriginal.data(),12))return false;
                    const auto* original=fragment.fragmentOriginal.data();
                    fragment.codeOffset=U32(original);fragment.codeSection=U16(original+4);
                    fragment.flags=U16(original+6);fragment.codeBytes=U32(original+8);
                    if(fragment.flags&~std::uint16_t(1))return false;
                    SIZE_T block=start+12;
                    while(block<subEnd){
                        lineFailureOffset_=block;auto row=fragment;row.blockOffset=block;
                        if(subEnd-block<12||!ReadStream(module.symbolStream,block,row.blockOriginal.data(),12))return false;
                        const auto* header=row.blockOriginal.data();
                        row.checksumOffset=U32(header);row.lineCount=U32(header+4);row.blockBytes=U32(header+8);
                        if(row.blockBytes<12||row.blockBytes>subEnd-block)return false;
                        const SIZE_T width=(row.flags&1)?12:8;
                        if(row.lineCount>(row.blockBytes-12)/width)return false;
                        row.linesOffset=block+12;row.columnsOffset=row.linesOffset+SIZE_T(row.lineCount)*8;
                        for(SIZE_T line=0;line<row.lineCount;++line){
                            unsigned char entry[8]{};
                            if(!ReadStream(module.symbolStream,row.linesOffset+line*8,entry,8)||
                               U32(entry)>row.codeBytes)return false;
                        }
                        // NameIndex is an exact offset of a checksum record, never an arbitrary name ID.
                        if(module.checksumSubsections==1){
                            if(module.checksumBegin>checksumCount_||
                               module.checksumCount>checksumCount_-module.checksumBegin)return false;
                            SIZE_T low=module.checksumBegin,high=low+module.checksumCount;
                            // Actual parser appends strictly increasing record offsets within this module.
                            while(low<high){
                                const SIZE_T middle=low+(high-low)/2;
                                const auto& checksum=checksums_[middle];
                                if(checksum.moduleOrdinal!=i||checksum.moduleStreamOffset<module.checksumOffset)return false;
                                if(checksum.moduleStreamOffset-module.checksumOffset<row.checksumOffset)low=middle+1;
                                else high=middle;
                            }
                            if(low<module.checksumBegin+module.checksumCount){
                                const auto& checksum=checksums_[low];
                                if(checksum.moduleOrdinal==i&&checksum.moduleStreamOffset>=module.checksumOffset&&
                                   checksum.moduleStreamOffset-module.checksumOffset==row.checksumOffset){
                                    row.checksumMatches=1;row.checksumOrdinal=low;
                                }
                            }
                        }
                        if(row.checksumMatches!=1)row.checksumOrdinal=SIZE_MAX;
                        row.complete=true;
                        if(retain){if(lineBlockCount_==lineBlockCapacity_)return false;
                            lineBlocks_[lineBlockCount_++]=row;
                        }else {if(lineBlockCapacity_==SIZE_MAX)return false;++lineBlockCapacity_;}
                        block+=row.blockBytes;
                    }
                }
                if(subEnd>SIZE_MAX-3)return false;
                cursor=(subEnd+3)&~SIZE_T(3);if(cursor>end)return false;
            }
        }
        return true;
    }
    void ObserveLineBlocks(SIZE_T aggregateRemaining) {
        linesAttempted_=true;
        if(!WalkLineBlocks(false))return;
        if(lineBlockCapacity_>(SIZE_MAX-sourceBytes_)/sizeof(LineBlockOccurrence)||
           sourceBytes_+lineBlockCapacity_*sizeof(LineBlockOccurrence)>aggregateRemaining)
            throw std::invalid_argument("MSF line block aggregate geometry");
        lineBlocks_=std::make_unique<LineBlockOccurrence[]>(lineBlockCapacity_);
        sourceBytes_+=lineBlockCapacity_*sizeof(LineBlockOccurrence);
        linesComplete_=WalkLineBlocks(true)&&lineBlockCount_==lineBlockCapacity_;
        // Full held line/column arrays stay in the original stream; no source/compiled-range authority.
    }
    bool ReadSymbolOccurrence(SIZE_T stream,SIZE_T cursor,SIZE_T end,SymbolOccurrence& row) const noexcept {
        row.streamOrdinal=stream;row.streamOffset=cursor;
        unsigned char header[4]{};
        if(cursor>end||end-cursor<4||!ReadStream(stream,cursor,header,4))return false;
        row.originalLength=U16(header);row.kind=U16(header+2);
        if(row.originalLength<2||row.originalLength>end-cursor-2)return false;
        const SIZE_T next=cursor+2+row.originalLength;
        row.payloadOffset=cursor+4;row.payloadExtent=row.originalLength-2;
        row.procedure=row.kind==0x110f||row.kind==0x1110||row.kind==0x1146||
            row.kind==0x1147||row.kind==0x1155||row.kind==0x1156;
        row.compile3=row.kind==0x113c;
        row.data=row.kind==0x110c||row.kind==0x110d;row.publicSymbol=row.kind==0x110e;
        row.fixedExtent=row.procedure?35:(row.compile3?22:((row.data||row.publicSymbol)?10:0));
        if(row.fixedExtent){
            if(row.payloadExtent<=row.fixedExtent||!ReadStream(stream,row.payloadOffset,
                row.fixedOriginal.data(),row.fixedExtent))return false;
            row.nameOffset=row.payloadOffset+row.fixedExtent;
            bool terminated=false;
            while(row.nameExtent<next-row.nameOffset){
                unsigned char byte=0;
                if(!ReadStream(stream,row.nameOffset+row.nameExtent,&byte,1))return false;
                ++row.nameExtent;if(!byte){terminated=true;break;}
            }
            if(!terminated)return false;
            if(row.procedure){row.codeBytes=U32(row.fixedOriginal.data()+12);
                row.codeOffset=U32(row.fixedOriginal.data()+28);row.codeSection=U16(row.fixedOriginal.data()+32);
            }else if(row.compile3)row.compilerMachine=U16(row.fixedOriginal.data()+4);
            else {row.dataTypeOrPublicFlags=U32(row.fixedOriginal.data());
                row.dataOffset=U32(row.fixedOriginal.data()+4);row.dataSection=U16(row.fixedOriginal.data()+8);}
        }
        row.complete=true;return true;
    }
    std::unique_ptr<SectionContribution[]> contributions_;
    SIZE_T contributionCount_=0,contributionCursor_=0;
    std::uint32_t contributionVersion_=0;
    bool contributionsAttempted_=false,contributionsComplete_=false;
    void ObserveSectionContributions(SIZE_T aggregateRemaining) {
        contributionsAttempted_=true;
        const SIZE_T begin=64+SIZE_T(dbi_.substreamSizes[0]);
        const SIZE_T bytes=dbi_.substreamSizes[1];contributionCursor_=begin;
        if(!bytes){contributionsComplete_=true;return;}
        unsigned char version[4]{};
        if(bytes<4||!ReadStream(3,begin,version,4))return;
        contributionVersion_=U32(version);
        const SIZE_T width=contributionVersion_==0xeffe0000u+19970605u?28:
            (contributionVersion_==0xeffe0000u+20140516u?32:0);
        if(!width)return;
        contributionCount_=(bytes-4)/width;
        if(contributionCount_>(SIZE_MAX-sourceBytes_)/sizeof(SectionContribution)||
           sourceBytes_+contributionCount_*sizeof(SectionContribution)>aggregateRemaining)
            throw std::invalid_argument("MSF section contribution aggregate geometry");
        if(contributionCount_)contributions_=std::make_unique<SectionContribution[]>(contributionCount_);
        sourceBytes_+=contributionCount_*sizeof(SectionContribution);
        contributionCursor_=begin+4;
        for(SIZE_T i=0;i<contributionCount_;++i){
            auto& row=contributions_[i];row.streamOffset=contributionCursor_;row.originalExtent=width;
            if(!ReadStream(3,row.streamOffset,row.original.data(),width))return;
            row.read=true;const auto* p=row.original.data();
            row.section=U16(p);row.offsetBits=U32(p+4);row.sizeBits=U32(p+8);
            row.characteristics=U32(p+12);row.moduleIndex=U16(p+16);
            row.dataCrc=U32(p+20);row.relocationCrc=U32(p+24);
            if(width==32)row.coffSection=U32(p+28);
            row.nonnegativeRange=row.offsetBits<=INT32_MAX&&row.sizeBits<=INT32_MAX;
            contributionCursor_+=width;
        }
        contributionsComplete_=(bytes-4)%width==0;
        // Original zero-based compiland candidates only; overlaps and signed sentinels are retained.
    }
    void BindGlobalContributionCandidates() noexcept {
        if(!globalSymbolsComplete_||!contributionsComplete_||!modulesComplete_)return;
        for(SIZE_T i=0;i<globalSymbolCount_;++i){
            auto& symbol=globalSymbols_[i];
            if(!symbol.complete||(!symbol.data&&!symbol.publicSymbol))continue;
            symbol.contributionComparisonAttempted=true;
            for(SIZE_T j=0;j<contributionCount_;++j){
                const auto& contribution=contributions_[j];
                if(!contribution.read||!contribution.nonnegativeRange||!contribution.section||!contribution.sizeBits||
                   contribution.section!=symbol.dataSection||symbol.dataOffset<contribution.offsetBits||
                   symbol.dataOffset-contribution.offsetBits>=contribution.sizeBits)continue;
                ++symbol.contributionMatches;symbol.contributionOrdinal=j;
                symbol.contributionModuleOrdinal=contribution.moduleIndex<moduleCount_?
                    SIZE_T(contribution.moduleIndex):SIZE_MAX;
            }
            if(symbol.contributionMatches!=1){symbol.contributionOrdinal=SIZE_MAX;
                symbol.contributionModuleOrdinal=SIZE_MAX;}
        }
        // This point membership never changes the raw record's absent moduleOrdinal.
        // Public/global index membership, data extent/type and source authenticity remain independent.
    }
    void ObserveGlobalSymbolRecords(SIZE_T aggregateRemaining) {
        if(globalSymbolsAttempted_)return;globalSymbolsAttempted_=true;
        if(!dbi_.supported||dbi_.symbolStream==UINT16_MAX||dbi_.symbolStream>=streamCount_)return;
        const SIZE_T end=streams_[dbi_.symbolStream].size;
        if(end==UINT32_MAX)return;
        SIZE_T cursor=0;
        while(cursor<end){
            globalSymbolCursorOriginal_=cursor;SymbolOccurrence row;
            const bool complete=ReadSymbolOccurrence(dbi_.symbolStream,cursor,end,row);
            globalSymbolPrefixOriginal_=row;
            if(!complete)break;
            ++globalSymbolCount_;cursor+=2+row.originalLength;
        }
        globalSymbolCursorOriginal_=cursor;globalSymbolsComplete_=cursor==end;
        if(globalSymbolCount_>(SIZE_MAX-sourceBytes_)/sizeof(SymbolOccurrence)||
           sourceBytes_+globalSymbolCount_*sizeof(SymbolOccurrence)>aggregateRemaining)
            throw std::invalid_argument("MSF DBI symbol record aggregate geometry");
        if(globalSymbolCount_)globalSymbols_=std::make_unique<SymbolOccurrence[]>(globalSymbolCount_);
        sourceBytes_+=globalSymbolCount_*sizeof(SymbolOccurrence);
        cursor=0;
        for(SIZE_T i=0;i<globalSymbolCount_;++i){
            if(!ReadSymbolOccurrence(dbi_.symbolStream,cursor,end,globalSymbols_[i])){globalSymbolsComplete_=false;return;}
            cursor+=2+globalSymbols_[i].originalLength;
        }
        // Raw records only. Hash/index membership and compiland ownership are separate.
    }
    void ObserveSymbolRoster(SIZE_T aggregateRemaining) {
        symbolRosterAttempted_=true;
        for(SIZE_T i=0;i<moduleCount_;++i){
            if(modules_[i].symbolRecords>SIZE_MAX-symbolCapacity_)
                throw std::invalid_argument("MSF symbol occurrence count");
            symbolCapacity_+=modules_[i].symbolRecords;
        }
        if(symbolCapacity_>(SIZE_MAX-sourceBytes_)/sizeof(SymbolOccurrence)||
           sourceBytes_+symbolCapacity_*sizeof(SymbolOccurrence)>aggregateRemaining)
            throw std::invalid_argument("MSF symbol occurrence aggregate geometry");
        symbols_=std::make_unique<SymbolOccurrence[]>(symbolCapacity_);
        sourceBytes_+=symbolCapacity_*sizeof(SymbolOccurrence);
        bool allComplete=true;
        for(SIZE_T i=0;i<moduleCount_;++i){
            const auto& module=modules_[i];SIZE_T cursor=4;
            allComplete=allComplete&&module.symbolsComplete;
            for(SIZE_T j=0;j<module.symbolRecords;++j){
                if(symbolCount_==symbolCapacity_)return;
                auto& row=symbols_[symbolCount_++];row.moduleOrdinal=i;row.streamOffset=cursor;
                if(!ReadSymbolOccurrence(module.symbolStream,cursor,module.symbolBytes,row))return;
                cursor+=2+row.originalLength;
            }
        }
        symbolRosterComplete_=allComplete&&symbolCount_==symbolCapacity_;
        // Record identity and extents are retained; names/types are not source or mapped-code authority.
    }
    std::unique_ptr<SourceChecksum[]> checksums_;
    SIZE_T checksumCapacity_=0,checksumCount_=0;
    bool checksumsAttempted_=false,checksumsComplete_=false;
    NamedMap named_;
    GlobalStrings globalStrings_;
    OptionalDebug optionalDebug_;
    void ObserveOptionalDebug() noexcept {
        optionalDebug_.attempted=true;optionalDebug_.streams.fill(UINT16_MAX);
        SIZE_T offset=64;
        for(SIZE_T i=0;i<5;++i){if(dbi_.substreamSizes[i]>SIZE_MAX-offset)return;
            offset+=dbi_.substreamSizes[i];}
        if(dbi_.substreamSizes[6]>SIZE_MAX-offset)return;
        offset+=dbi_.substreamSizes[6]; // EC precedes optional debug in the original physical stream.
        optionalDebug_.offset=offset;optionalDebug_.extent=dbi_.substreamSizes[5];
        if(optionalDebug_.extent%2||offset>streams_[3].size||
           optionalDebug_.extent>streams_[3].size-offset)return;
        optionalDebug_.indices=optionalDebug_.extent/2;
        for(SIZE_T i=0;i<optionalDebug_.indices;++i){
            unsigned char bytes[2]{};if(!ReadStream(3,offset+i*2,bytes,2))return;
            const auto index=U16(bytes);
            if(index!=UINT16_MAX&&(index>=streamCount_||streams_[index].size==UINT32_MAX))return;
            if(i<optionalDebug_.streams.size())optionalDebug_.streams[i]=index;
            else ++optionalDebug_.unknownIndices;
        }
        optionalDebug_.complete=true;
        optionalDebug_.omapToSourcePresent=optionalDebug_.streams[3]!=UINT16_MAX;
        optionalDebug_.omapFromSourcePresent=optionalDebug_.streams[4]!=UINT16_MAX;
        const auto sections=[&](SIZE_T slot,SIZE_T& count) noexcept {
            const auto index=optionalDebug_.streams[slot];
            if(index==UINT16_MAX||streams_[index].size%sizeof(IMAGE_SECTION_HEADER))return false;
            count=streams_[index].size/sizeof(IMAGE_SECTION_HEADER);return count!=0;
        };
        optionalDebug_.sectionHeadersObserved=sections(5,optionalDebug_.sectionCount);
        optionalDebug_.originalSectionHeadersObserved=sections(10,optionalDebug_.originalSectionCount);
    }
    bool sourceDigestsAttempted_=false;
    bool GlobalStringSpan(std::uint32_t index,SIZE_T& offset,SIZE_T& extent) const noexcept {
        if(!globalStrings_.supported||index>=globalStrings_.stringBytes)return false;
        offset=12+SIZE_T(index);extent=0;
        while(extent<globalStrings_.stringBytes-index){
            unsigned char byte=0;
            if(!ReadStream(globalStrings_.stream,offset+extent,&byte,1))return false;
            ++extent;if(!byte)return true;
        }
        return false;
    }
    void ObserveGlobalStrings() noexcept {
        globalStrings_.attempted=true;
        if(!named_.complete||named_.globalNamesMatches!=1||named_.globalNamesStream>=streamCount_)return;
        globalStrings_.stream=named_.globalNamesStream;
        if(!ReadStream(globalStrings_.stream,0,globalStrings_.original.data(),12))return;
        globalStrings_.read=true;
        const auto* header=globalStrings_.original.data();
        globalStrings_.signature=U32(header);globalStrings_.hashVersion=U32(header+4);
        globalStrings_.stringBytes=U32(header+8);
        if(globalStrings_.signature!=0xeffeeffeu||
           (globalStrings_.hashVersion!=1&&globalStrings_.hashVersion!=2))return;
        const SIZE_T extent=streams_[globalStrings_.stream].size;
        if(extent<12||globalStrings_.stringBytes>extent-12)return;
        globalStrings_.supported=true;
        SIZE_T cursor=12+SIZE_T(globalStrings_.stringBytes);
        if(!StreamU32(globalStrings_.stream,cursor,globalStrings_.buckets))return;
        cursor+=4;globalStrings_.bucketOffset=cursor;
        if(cursor>extent||globalStrings_.buckets>(extent-cursor)/4)return;
        for(SIZE_T i=0;i<globalStrings_.buckets;++i){
            std::uint32_t id=0;if(!StreamU32(globalStrings_.stream,cursor+i*4,id))return;
            if(!id)continue;
            SIZE_T offset=0,length=0;
            if(!GlobalStringSpan(id,offset,length))return;
            ++globalStrings_.nonemptyBuckets;
        }
        cursor+=SIZE_T(globalStrings_.buckets)*4;
        if(!StreamU32(globalStrings_.stream,cursor,globalStrings_.nameCount)||extent-cursor!=4||
           globalStrings_.nameCount>globalStrings_.buckets)return;
        globalStrings_.complete=true; // Hash lookup/unique source correspondence remains independent.
    }
    bool StreamU32(SIZE_T stream,SIZE_T offset,std::uint32_t& value) const noexcept {
        unsigned char b[4]{};if(!ReadStream(stream,offset,b,4))return false;value=U32(b);return true;
    }
    void ObserveNamedMap() noexcept {
        named_.attempted=true;
        std::uint32_t strings=0;
        if(!StreamU32(1,28,strings)||streams_[1].size<32||strings>streams_[1].size-32)return;
        named_.stringOffset=32;named_.stringExtent=strings;
        SIZE_T cursor=32+SIZE_T(strings);
        if(cursor>streams_[1].size||streams_[1].size-cursor<8)return;
        if(!StreamU32(1,cursor,named_.size)||!StreamU32(1,cursor+4,named_.capacity))return;
        cursor+=8;
        if(!named_.capacity||named_.size>named_.capacity)return;
        const auto vector=[&](SIZE_T& offset,SIZE_T& words) noexcept {
            std::uint32_t count=0;if(!StreamU32(1,cursor,count))return false;
            cursor+=4;if(cursor>streams_[1].size||count>(streams_[1].size-cursor)/4)return false;
            offset=cursor;words=count;cursor+=SIZE_T(count)*4;return true;
        };
        if(!vector(named_.presentOffset,named_.presentWords)||!vector(named_.deletedOffset,named_.deletedWords))return;
        named_.entriesOffset=cursor;
        if(cursor>streams_[1].size||named_.size>(streams_[1].size-cursor)/8)return;
        const auto validBits=[&](std::uint32_t bits,SIZE_T word) noexcept {
            for(unsigned bit=0;bit<32;++bit)
                if((bits&(std::uint32_t(1)<<bit))&&std::uint64_t(word)*32+bit>=named_.capacity)return false;
            return true;
        };
        for(SIZE_T word=0;word<named_.deletedWords;++word){
            std::uint32_t deleted=0;
            if(!StreamU32(1,named_.deletedOffset+word*4,deleted)||!validBits(deleted,word))return;
        }
        for(SIZE_T word=0;word<named_.presentWords;++word){
            std::uint32_t present=0,deleted=0;
            if(!StreamU32(1,named_.presentOffset+word*4,present)||!validBits(present,word))return;
            if(word<named_.deletedWords&&!StreamU32(1,named_.deletedOffset+word*4,deleted))return;
            if(present&deleted)return;
            for(unsigned bit=0;bit<32;++bit){
                if(!(present&(std::uint32_t(1)<<bit)))continue;
                if(named_.entriesVisited==named_.size)return;
                std::uint32_t key=0,value=0;
                if(!StreamU32(1,cursor,key)||!StreamU32(1,cursor+4,value)||key>=strings||
                   value>=streamCount_||streams_[value].size==UINT32_MAX)return;
                cursor+=8;++named_.entriesVisited;
                SIZE_T extent=0;bool terminated=false,globalName=true;
                constexpr unsigned char expected[]={'/','n','a','m','e','s',0};
                while(extent<strings-key){
                    unsigned char byte=0;
                    if(!ReadStream(1,32+SIZE_T(key)+extent,&byte,1))return;
                    if(extent>=sizeof(expected)||byte!=expected[extent])globalName=false;
                    ++extent;if(!byte){terminated=true;break;}
                }
                if(!terminated)return;
                if(globalName&&extent==sizeof(expected)){
                    ++named_.globalNamesMatches;named_.globalNamesStream=value;
                }
            }
        }
        if(named_.entriesVisited!=named_.size)return;
        named_.featureOffset=cursor;named_.featureExtent=streams_[1].size-cursor;
        if(named_.featureExtent%4)return;
        if(named_.globalNamesMatches!=1)named_.globalNamesStream=SIZE_MAX;
        named_.complete=true; // Serialized membership only; feature semantics remain separate.
    }
    static std::uint16_t U16(const unsigned char* p) noexcept {
        return std::uint16_t(p[0])|(std::uint16_t(p[1])<<8);
    }
    static std::uint32_t U32(const unsigned char* p) noexcept {
        return std::uint32_t(p[0])|(std::uint32_t(p[1])<<8)|
            (std::uint32_t(p[2])<<16)|(std::uint32_t(p[3])<<24);
    }
    bool Claim(std::uint32_t block,unsigned char kind) noexcept {
        if(block>=blocks_||blockOwners_[block])return false;
        blockOwners_[block]=kind;++claimedBlocks_;return true;
    }
    bool DirectoryRead(SIZE_T offset,unsigned char* out,SIZE_T extent) const noexcept {
        if(offset>directoryBytes_||extent>directoryBytes_-offset)return false;
        while(extent){
            const SIZE_T page=offset/blockSize_,within=offset%blockSize_;
            if(page>=directoryBlocks_)return false;
            const auto block=U32(bodyOriginal_+SIZE_T(blockMap_)*blockSize_+page*4);
            if(block>=blocks_)return false;
            SIZE_T chunk=blockSize_-within;if(chunk>extent)chunk=extent;
            memcpy(out,bodyOriginal_+SIZE_T(block)*blockSize_+within,chunk);
            offset+=chunk;out+=chunk;extent-=chunk;
        }
        return true;
    }
    bool DirectoryU32(SIZE_T offset,std::uint32_t& value) const noexcept {
        unsigned char bytes[4]{};
        if(!DirectoryRead(offset,bytes,sizeof(bytes)))return false;
        value=U32(bytes);return true;
    }
public:
    OriginalHeldMsfSource()=default;
    OriginalHeldMsfSource(const OriginalHeldMsfSource&)=delete;
    OriginalHeldMsfSource& operator=(const OriginalHeldMsfSource&)=delete;
    void Observe(const NativeHeldFiles& files,SIZE_T ordinal,const void* requirement,SIZE_T aggregateRemaining) {
        if(attempted_)return;
        attempted_=true;filesOriginal_=&files;fileOrdinal_=ordinal;requirementOriginal_=requirement;result_=Result::Unavailable;
        if(!requirement)return;
        if(ordinal>=files.FileCount()||files.PrivateFiles()[ordinal].closeAttempted||
           !files.ExactCapturedSourceSpan(ordinal,bodyOriginal_,bodyExtent_))return;
        ObserveCapturedBody(aggregateRemaining);
    }
private:
    void ObserveCapturedBody(SIZE_T aggregateRemaining) {
        constexpr char magic[]="Microsoft C/C++ MSF 7.00\r\n\x1a" "DS\0\0\0";
        static_assert(sizeof(magic)==33,"MSF7 magic extent");
        if(bodyExtent_<32||memcmp(bodyOriginal_,magic,32)){result_=Result::NotMsf;return;}
        result_=Result::Malformed;
        if(bodyExtent_<56)return;
        blockSize_=U32(bodyOriginal_+32);freeMap_=U32(bodyOriginal_+36);
        blocks_=U32(bodyOriginal_+40);directoryBytes_=U32(bodyOriginal_+44);
        unknown_=U32(bodyOriginal_+48);blockMap_=U32(bodyOriginal_+52);
        if(blockSize_!=4096){result_=Result::Unsupported;return;}
        if((freeMap_!=1&&freeMap_!=2)||!blocks_||blocks_>SIZE_MAX/blockSize_||
           SIZE_T(blocks_)*blockSize_!=bodyExtent_||directoryBytes_<4||blockMap_>=blocks_)return;
        directoryBlocks_=directoryBytes_/blockSize_+(directoryBytes_%blockSize_!=0);
        if(directoryBlocks_>blockSize_/4){result_=Result::Unsupported;return;}
        // All counts/arithmetic and aggregate table bytes are checked before either allocation.
        if(blocks_>SIZE_MAX-sourceBytes_)throw std::invalid_argument("MSF block owner geometry");
        const SIZE_T blockCharge=sourceBytes_+blocks_;
        // Directory page references are read directly from the held body, not copied.
        for(SIZE_T i=0;i<directoryBlocks_;++i)
            if(U32(bodyOriginal_+SIZE_T(blockMap_)*blockSize_+i*4)>=blocks_)return;
        std::uint32_t count=0;
        if(!DirectoryU32(0,count)||count>(directoryBytes_-4)/4)return;
        streamCount_=count;
        if(streamCount_>(SIZE_MAX-blockCharge)/sizeof(Stream))
            throw std::invalid_argument("MSF stream owner geometry");
        const SIZE_T charge=blockCharge+streamCount_*sizeof(Stream);
        if(charge>aggregateRemaining)throw std::invalid_argument("MSF aggregate source geometry");
        blockOwners_=std::make_unique<unsigned char[]>(blocks_);
        sourceBytes_=blockCharge; // Survives a later stream allocation exception.
        streams_=std::make_unique<Stream[]>(streamCount_);sourceBytes_=charge;
        if(!Claim(0,1))return;
        for(SIZE_T block=1;block<blocks_;++block)
            if(block%blockSize_==1||block%blockSize_==2)
                if(!Claim(static_cast<std::uint32_t>(block),2))return;
        if(!Claim(blockMap_,3))return;
        for(SIZE_T i=0;i<directoryBlocks_;++i)
            if(!Claim(U32(bodyOriginal_+SIZE_T(blockMap_)*blockSize_+i*4),4))return;
        SIZE_T cursor=4+streamCount_*4;
        for(SIZE_T i=0;i<streamCount_;++i){
            auto& stream=streams_[i];
            if(!DirectoryU32(4+i*4,stream.size))return;
            stream.directoryBlocksOffset=cursor;
            if(stream.size==UINT32_MAX)continue; // Absent differs from an empty stream.
            stream.blocks=stream.size/blockSize_+(stream.size%blockSize_!=0);
            if(cursor>directoryBytes_||stream.blocks>(directoryBytes_-cursor)/4)return;
            for(SIZE_T j=0;j<stream.blocks;++j){
                std::uint32_t block=0;
                if(!DirectoryU32(cursor+j*4,block)||!Claim(block,5))return;
            }
            cursor+=stream.blocks*4;
        }
        if(cursor!=directoryBytes_)return;
        result_=Result::Geometry;
        info_.attempted=true;
        if(!ReadStream(1,0,info_.original.data(),info_.original.size()))return;
        info_.read=true;
        info_.version=U32(info_.original.data());info_.signature=U32(info_.original.data()+4);
        info_.age=U32(info_.original.data()+8);memcpy(info_.guid.data(),info_.original.data()+12,16);
        info_.supported=info_.version==20000404;
        if(info_.supported){ObserveNamedMap();ObserveGlobalStrings();}
        dbi_.attempted=true;
        if(!ReadStream(3,0,dbi_.original.data(),dbi_.original.size()))return;
        dbi_.read=true;
        const auto* header=dbi_.original.data();
        dbi_.versionSignature=U32(header);dbi_.version=U32(header+4);dbi_.age=U32(header+8);
        dbi_.globalStream=U16(header+12);dbi_.buildNumber=U16(header+14);
        dbi_.publicStream=U16(header+16);dbi_.pdbDllVersion=U16(header+18);
        dbi_.symbolStream=U16(header+20);dbi_.pdbDllRebuild=U16(header+22);
        dbi_.flags=U16(header+56);dbi_.machine=U16(header+58);
        dbi_.privateSymbolsStripped=(dbi_.flags&2)!=0;
        constexpr SIZE_T sizeOffsets[]{24,28,32,36,40,48,52};
        SIZE_T total=dbi_.original.size();
        for(SIZE_T i=0;i<dbi_.substreamSizes.size();++i){
            const auto size=U32(header+sizeOffsets[i]);dbi_.substreamSizes[i]=size;
            // Original fields are signed int32 lengths, never huge unsigned extents.
            if(size>INT32_MAX||size>SIZE_MAX-total)return;
            total+=size;
        }
        dbi_.retainedExtent=total;
        if(total!=streams_[3].size)return;
        dbi_.extentsObserved=true;
        const auto streamIndex=[&](std::uint16_t index) noexcept {
            return index==UINT16_MAX||(index<streamCount_&&streams_[index].size!=UINT32_MAX);
        };
        dbi_.supported=dbi_.versionSignature==UINT32_MAX&&dbi_.version==19990903&&
            dbi_.age==info_.age&&dbi_.machine==0x8664&&(dbi_.buildNumber&0x8000)!=0&&
            streamIndex(dbi_.globalStream)&&streamIndex(dbi_.publicStream)&&streamIndex(dbi_.symbolStream);
        if(!dbi_.supported)return;
        ObserveOptionalDebug();
        ObserveSectionContributions(aggregateRemaining);
        ObserveGlobalSymbolRecords(aggregateRemaining);
        modulesAttempted_=true;
        const SIZE_T moduleEnd=64+SIZE_T(dbi_.substreamSizes[0]);
        moduleCapacity_=dbi_.substreamSizes[0]/64;
        if(moduleCapacity_>(SIZE_MAX-sourceBytes_)/sizeof(Module)||
           sourceBytes_+moduleCapacity_*sizeof(Module)>aggregateRemaining)
            throw std::invalid_argument("MSF module owner aggregate geometry");
        modules_=std::make_unique<Module[]>(moduleCapacity_);
        sourceBytes_+=moduleCapacity_*sizeof(Module);
        SIZE_T moduleCursor=64;
        const auto stringSpan=[&](SIZE_T start,SIZE_T& extent) noexcept {
            extent=0;
            while(start<moduleEnd&&extent<moduleEnd-start){
                unsigned char byte=0;
                if(!ReadStream(3,start+extent,&byte,1))return false;
                ++extent;if(!byte)return true;
            }
            return false;
        };
        while(moduleCursor<moduleEnd){
            if(moduleCount_==moduleCapacity_||moduleEnd-moduleCursor<64)return;
            auto& module=modules_[moduleCount_++];module.dbiOffset=moduleCursor;
            if(!ReadStream(3,moduleCursor,module.original.data(),module.original.size()))return;
            const auto* record=module.original.data();
            module.section=U16(record+4);module.sectionOffset=U32(record+8);module.sectionBytes=U32(record+12);
            module.flags=U16(record+32);module.symbolStream=U16(record+34);
            module.symbolBytes=U32(record+36);module.c11Bytes=U32(record+40);module.c13Bytes=U32(record+44);
            module.sourceFileCount=U16(record+48);
            module.moduleNameOffset=moduleCursor+64;
            if(!stringSpan(module.moduleNameOffset,module.moduleNameExtent))return;
            module.objectNameOffset=module.moduleNameOffset+module.moduleNameExtent;
            if(!stringSpan(module.objectNameOffset,module.objectNameExtent))return;
            const SIZE_T namesEnd=module.objectNameOffset+module.objectNameExtent;
            if(namesEnd>SIZE_MAX-3)return;
            moduleCursor=(namesEnd+3)&~SIZE_T(3);
            if(moduleCursor>moduleEnd)return;
            const std::uint64_t payload=std::uint64_t(module.symbolBytes)+module.c11Bytes+module.c13Bytes;
            if(module.symbolStream==UINT16_MAX){if(payload)return;}
            else if(module.symbolStream>=streamCount_||streams_[module.symbolStream].size==UINT32_MAX||
                    payload>streams_[module.symbolStream].size)return;
            // Section offset/size are signed originals; mapping/source semantics are later consumers.
            if(module.sectionOffset>INT32_MAX||module.sectionBytes>INT32_MAX)return;
            module.extentsObserved=true;
            ObserveModuleSymbols(module);
            module.c13Attempted=true;
            SIZE_T c13Cursor=SIZE_T(module.symbolBytes)+module.c11Bytes;
            const SIZE_T c13End=c13Cursor+module.c13Bytes;
            while(c13Cursor<c13End){
                if(c13End-c13Cursor<8)return;
                unsigned char subsection[8]{};
                if(!ReadStream(module.symbolStream,c13Cursor,subsection,sizeof(subsection)))return;
                const auto kind=U32(subsection),length=U32(subsection+4);
                const SIZE_T start=c13Cursor+8;
                if(length>c13End-start)return;
                ++module.c13Subsections;
                if(kind&0x80000000u)++module.c13IgnoredSubsections;
                else if(kind==0xf4){
                    ++module.checksumSubsections;
                    if(module.checksumSubsections==1){module.checksumOffset=start;module.checksumExtent=length;}
                }else if(kind==0xf3){
                    ++module.stringSubsections;
                    if(module.stringSubsections==1){module.stringOffset=start;module.stringExtent=length;}
                }else ++module.c13UnknownSubsections;
                const SIZE_T end=start+length;
                if(end>SIZE_MAX-3)return;
                c13Cursor=(end+3)&~SIZE_T(3);
                if(c13Cursor>c13End)return;
            }
            module.c13Complete=c13Cursor==c13End;
        }
        modulesComplete_=moduleCursor==moduleEnd;
        if(!modulesComplete_)return;
        BindGlobalContributionCandidates();
        ObserveSymbolRoster(aggregateRemaining);
        checksumsAttempted_=true;
        for(SIZE_T i=0;i<moduleCount_;++i){
            const auto& module=modules_[i];
            if(module.checksumSubsections!=1)continue;
            const SIZE_T capacity=module.checksumExtent/6;
            if(capacity>SIZE_MAX-checksumCapacity_)throw std::invalid_argument("MSF checksum count geometry");
            checksumCapacity_+=capacity;
        }
        if(checksumCapacity_>(SIZE_MAX-sourceBytes_)/sizeof(SourceChecksum)||
           sourceBytes_+checksumCapacity_*sizeof(SourceChecksum)>aggregateRemaining)
            throw std::invalid_argument("MSF checksum owner aggregate geometry");
        checksums_=std::make_unique<SourceChecksum[]>(checksumCapacity_);
        sourceBytes_+=checksumCapacity_*sizeof(SourceChecksum);
        for(SIZE_T i=0;i<moduleCount_;++i){
            auto& module=modules_[i];module.checksumBegin=checksumCount_;
            if(module.checksumSubsections!=1)continue;
            SIZE_T cursor=module.checksumOffset;
            const SIZE_T end=cursor+module.checksumExtent;
            while(cursor<end){
                if(end-cursor<6||checksumCount_==checksumCapacity_)return;
                unsigned char header[6]{};
                if(!ReadStream(module.symbolStream,cursor,header,sizeof(header)))return;
                auto& checksum=checksums_[checksumCount_++];++module.checksumCount;
                checksum.moduleOrdinal=i;checksum.moduleStreamOffset=cursor;
                checksum.fileNameOffset=U32(header);checksum.size=header[4];checksum.kind=header[5];
                if(checksum.size>end-cursor-6)return;
                if(checksum.size<=checksum.bytes.size()){
                    if(!ReadStream(module.symbolStream,cursor+6,checksum.bytes.data(),checksum.size))return;
                    checksum.originalCopied=true;
                }
                checksum.supportedShape=checksum.originalCopied&&
                    ((checksum.kind==0&&checksum.size==0)||(checksum.kind==1&&checksum.size==16)||
                     (checksum.kind==2&&checksum.size==20)||(checksum.kind==3&&checksum.size==32));
                if(globalStrings_.complete){
                    checksum.nameStream=globalStrings_.stream;
                    checksum.nameObserved=GlobalStringSpan(checksum.fileNameOffset,checksum.nameOffset,checksum.nameExtent);
                }
                const SIZE_T recordEnd=cursor+6+checksum.size;
                if(recordEnd>SIZE_MAX-3)return;
                cursor=(recordEnd+3)&~SIZE_T(3);
                if(cursor>end)return;
            }
        }
        checksumsComplete_=true;
        ObserveLineBlocks(aggregateRemaining);
    }
public:
    void ObserveSourceDigests(const OriginalHeldSourceDigest* caches,SIZE_T count) noexcept {
        if(sourceDigestsAttempted_)return;
        sourceDigestsAttempted_=true;
        if(!filesOriginal_||count!=filesOriginal_->FileCount()||(!caches&&count)||
           !checksumsAttempted_||!checksumsComplete_)return;
        for(SIZE_T row=0;row<checksumCount_;++row){
            auto& checksum=checksums_[row];checksum.sourceComparisonAttempted=true;
            if(!checksum.supportedShape||checksum.kind==0)continue;
            for(SIZE_T i=0;i<count;++i){
                const auto& cache=caches[i];
                const unsigned char* original=nullptr;SIZE_T extent=0;
                if(!cache.attempted||!cache.complete||cache.filesOriginal!=filesOriginal_||cache.ordinal!=i||
                   !cache.requirementOriginal||!cache.algorithms.Complete()||
                   !filesOriginal_->ExactCapturedSourceSpan(i,original,extent)||
                   cache.bodyOriginal!=original||cache.extent!=extent||
                   cache.algorithms.BodyOriginal()!=original||cache.algorithms.ExtentOriginal()!=extent)continue;
                const unsigned char* digest=nullptr;
                if(checksum.kind==1)digest=cache.algorithms.Md5().data();
                else if(checksum.kind==2)digest=cache.algorithms.Sha1().data();
                else if(checksum.kind==3)digest=filesOriginal_->PrivateFiles()[i].actualSha256.data();
                if(!digest||memcmp(checksum.bytes.data(),digest,checksum.size))continue;
                ++checksum.heldSourceMatches;checksum.heldSourceOrdinal=i;
                checksum.heldSourceRequirementOriginal=cache.requirementOriginal;
            }
            if(checksum.heldSourceMatches!=1){
                checksum.heldSourceOrdinal=SIZE_MAX;checksum.heldSourceRequirementOriginal=nullptr;
            }
        }
        for(SIZE_T i=0;i<lineBlockCount_;++i){
            auto& line=lineBlocks_[i];line.sourceCorrespondenceAttempted=true;
            if(!line.complete||line.checksumMatches!=1||line.checksumOrdinal>=checksumCount_)continue;
            const auto& checksum=checksums_[line.checksumOrdinal];
            if(checksum.moduleOrdinal!=line.moduleOrdinal||!checksum.sourceComparisonAttempted||
               !checksum.supportedShape||checksum.kind==0||checksum.heldSourceMatches!=1||
               checksum.heldSourceOrdinal>=count||!checksum.heldSourceRequirementOriginal)continue;
            line.heldSourceOrdinal=checksum.heldSourceOrdinal;
            line.heldSourceRequirementOriginal=checksum.heldSourceRequirementOriginal;
            line.sourceChecksumCorrespondenceObserved=true;
        }
    }
    bool ReadStream(SIZE_T ordinal,SIZE_T offset,unsigned char* out,SIZE_T extent) const noexcept {
        if(result_!=Result::Geometry||ordinal>=streamCount_||(!out&&extent))return false;
        const auto& stream=streams_[ordinal];
        if(stream.size==UINT32_MAX||offset>stream.size||extent>stream.size-offset)return false;
        while(extent){
            const SIZE_T page=offset/blockSize_,within=offset%blockSize_;
            std::uint32_t block=0;
            if(page>=stream.blocks||!DirectoryU32(stream.directoryBlocksOffset+page*4,block)||block>=blocks_)return false;
            SIZE_T chunk=blockSize_-within;if(chunk>extent)chunk=extent;
            memcpy(out,bodyOriginal_+SIZE_T(block)*blockSize_+within,chunk);
            offset+=chunk;out+=chunk;extent-=chunk;
        }
        return true;
    }
    Result Outcome() const noexcept{return result_;}
    const InfoHeader& OriginalInfoHeader() const noexcept{return info_;}
    const DbiHeader& OriginalDbiHeader() const noexcept{return dbi_;}
    const NamedMap& OriginalNamedMap() const noexcept{return named_;}
    const GlobalStrings& OriginalGlobalStrings() const noexcept{return globalStrings_;}
    const Module* OriginalModules() const noexcept{return modules_.get();}
    const OptionalDebug& OriginalOptionalDebug() const noexcept{return optionalDebug_;}
    bool ReadOriginalSectionHeader(SIZE_T ordinal,bool beforeTranslation,IMAGE_SECTION_HEADER& header) const noexcept {
        if(!optionalDebug_.complete)return false;
        const SIZE_T slot=beforeTranslation?10:5;
        const SIZE_T count=beforeTranslation?optionalDebug_.originalSectionCount:optionalDebug_.sectionCount;
        const bool observed=beforeTranslation?optionalDebug_.originalSectionHeadersObserved:optionalDebug_.sectionHeadersObserved;
        if(!observed||ordinal>=count)return false;
        return ReadStream(optionalDebug_.streams[slot],ordinal*sizeof(header),
                          reinterpret_cast<unsigned char*>(&header),sizeof(header));
    }
    SIZE_T ModuleCount() const noexcept{return moduleCount_;}
    const SymbolOccurrence* OriginalSymbols() const noexcept{return symbols_.get();}
    SIZE_T SymbolCount() const noexcept{return symbolCount_;}
    const SectionContribution* OriginalSectionContributions() const noexcept{return contributions_.get();}
    SIZE_T SectionContributionCount() const noexcept{return contributionCount_;}
    bool SectionContributionsComplete() const noexcept{return contributionsAttempted_&&contributionsComplete_;}
    SIZE_T SectionContributionCursorOriginal() const noexcept{return contributionCursor_;}
    std::uint32_t SectionContributionVersionOriginal() const noexcept{return contributionVersion_;}
    const SymbolOccurrence* OriginalGlobalSymbols() const noexcept{return globalSymbols_.get();}
    SIZE_T GlobalSymbolCount() const noexcept{return globalSymbolCount_;}
    bool GlobalSymbolRecordsComplete() const noexcept{return globalSymbolsAttempted_&&globalSymbolsComplete_;}
    SIZE_T GlobalSymbolCursorOriginal() const noexcept{return globalSymbolCursorOriginal_;}
    const SymbolOccurrence& GlobalSymbolPrefixOriginal() const noexcept{return globalSymbolPrefixOriginal_;}
    const LineBlockOccurrence* OriginalLineBlocks() const noexcept{return lineBlocks_.get();}
    SIZE_T LineBlockCount() const noexcept{return lineBlockCount_;}
    bool LineBlocksComplete() const noexcept{return linesAttempted_&&linesComplete_;}
    bool ReadOriginalLinePosition(SIZE_T blockOrdinal,SIZE_T lineOrdinal,LinePositionOriginal& original) const noexcept {
        if(original.attempted)return false;original.attempted=true;
        original.artifactOriginal=this;original.blockOrdinal=blockOrdinal;original.lineOrdinal=lineOrdinal;
        if(blockOrdinal>=lineBlockCount_)return false;
        const auto& block=lineBlocks_[blockOrdinal];
        if(!block.complete||block.moduleOrdinal>=moduleCount_||lineOrdinal>=block.lineCount)return false;
        original.streamOrdinal=modules_[block.moduleOrdinal].symbolStream;
        original.streamOffset=block.linesOffset+lineOrdinal*8;
        if(!ReadStream(original.streamOrdinal,original.streamOffset,original.line.data(),8))return false;
        original.relativeOffset=U32(original.line.data());original.rawFlags=U32(original.line.data()+4);
        if(original.relativeOffset>block.codeBytes||original.relativeOffset>UINT32_MAX-block.codeOffset)return false;
        original.sectionOffset=block.codeOffset+original.relativeOffset;
        original.startLine=original.rawFlags&0x00ffffffu;
        original.endLine=original.startLine+((original.rawFlags&0x7f000000u)>>24);
        original.statement=(original.rawFlags&0x80000000u)!=0;
        original.alwaysStep=original.startLine==0xfeefee;original.neverStep=original.startLine==0xf00f00;
        if(block.flags&1){
            if(!ReadStream(original.streamOrdinal,block.columnsOffset+lineOrdinal*4,original.columns.data(),4))return false;
            original.columnsRead=true;original.startColumn=U16(original.columns.data());original.endColumn=U16(original.columns.data()+2);
        }
        original.read=true;return true;
    }
    bool SymbolRosterComplete() const noexcept{return symbolRosterAttempted_&&symbolRosterComplete_;}
    bool ModulesComplete() const noexcept{return modulesAttempted_&&modulesComplete_;}
    const SourceChecksum* OriginalChecksums() const noexcept{return checksums_.get();}
    SIZE_T ChecksumCount() const noexcept{return checksumCount_;}
    bool UniqueChecksumSubsectionsEnumerated() const noexcept{return checksumsAttempted_&&checksumsComplete_;}
    SIZE_T ReservedSourceBytes() const noexcept{return sourceBytes_;}
    SIZE_T StreamCount() const noexcept{return streamCount_;}
    struct RetainedAllocationSpan {const void* original=nullptr;SIZE_T bytes=0;};
    bool RetainedAllocationSpansOriginal(std::array<RetainedAllocationSpan,8>& out) const noexcept {
        std::array<RetainedAllocationSpan,8> value{};
        const auto set=[&](SIZE_T slot,const void* original,SIZE_T count,SIZE_T width) noexcept {
            if(!original)return true;
            if(width&&count>SIZE_MAX/width)return false;
            value[slot]={original,count*width};return true;
        };
        if(!set(0,blockOwners_.get(),blocks_,1)||!set(1,streams_.get(),streamCount_,sizeof(Stream))||
            !set(2,modules_.get(),moduleCapacity_,sizeof(Module))||!set(3,symbols_.get(),symbolCapacity_,sizeof(SymbolOccurrence))||
            !set(4,globalSymbols_.get(),globalSymbolCount_,sizeof(SymbolOccurrence))||
            !set(5,lineBlocks_.get(),lineBlockCapacity_,sizeof(LineBlockOccurrence))||
            !set(6,contributions_.get(),contributionCount_,sizeof(SectionContribution))||
            !set(7,checksums_.get(),checksumCapacity_,sizeof(SourceChecksum)))return false;
        out=value;return true;
    }
    const Stream* Streams() const noexcept{return streams_.get();}
    const NativeHeldFiles* FilesOriginal() const noexcept{return filesOriginal_;}
    const void* RequirementOriginal() const noexcept{return requirementOriginal_;}
    SIZE_T FileOrdinal() const noexcept{return fileOrdinal_;}
    const unsigned char* BodyOriginal() const noexcept{return bodyOriginal_;}
    SIZE_T BodyExtent() const noexcept{return bodyExtent_;}
};
}

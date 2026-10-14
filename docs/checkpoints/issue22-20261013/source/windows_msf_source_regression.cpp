// Synthetic format corpus only. No installed artifact, native load or build authority.
#define TEMPLATE_NATIVE_MSF_SYNTHETIC_REGRESSION 1
#include "windows_compiler_range_observation.hpp"
#include <vector>
namespace template_creator {
bool OriginalJsonSourceRegression();
static bool SyntheticDigestVector(const unsigned char* body,SIZE_T extent,const char* md5,const char* sha1) {
    OriginalSourceChecksumAlgorithms digest;
    if(!digest.Compute(body,extent)||!digest.Complete()||digest.Compute(body,extent))return false;
    const SIZE_T expectedTransforms=extent/64+(extent%64<56?1:2);
    if(digest.Md5Transforms()!=expectedTransforms||digest.Sha1Transforms()!=expectedTransforms)return false;
    const auto equal=[](const auto& bytes,const char* expected){
        constexpr char hex[]="0123456789abcdef";
        for(SIZE_T i=0;i<bytes.size();++i)
            if(expected[i*2]!=hex[bytes[i]>>4]||expected[i*2+1]!=hex[bytes[i]&15])return false;
        return expected[bytes.size()*2]==0;
    };
    return (!md5||equal(digest.Md5(),md5))&&(!sha1||equal(digest.Sha1(),sha1));
}
static bool SyntheticDigestKnownAnswers() {
    if(!SyntheticDigestVector(nullptr,0,"d41d8cd98f00b204e9800998ecf8427e","da39a3ee5e6b4b0d3255bfef95601890afd80709"))return false;
    constexpr unsigned char abc[]={'a','b','c'};
    if(!SyntheticDigestVector(abc,sizeof(abc),"900150983cd24fb0d6963f7d28e17f72","a9993e364706816aba3e25717850c26c9cd0d89d"))return false;
    constexpr unsigned char twoTail[]="abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq";
    if(!SyntheticDigestVector(twoTail,sizeof(twoTail)-1,nullptr,"84983e441c3bd26ebaae4aa1f95129e5e54670f1"))return false;
    constexpr unsigned char multipleBlocks[]="12345678901234567890123456789012345678901234567890123456789012345678901234567890";
    if(!SyntheticDigestVector(multipleBlocks,sizeof(multipleBlocks)-1,"57edf4a22be3c955ac49da2e2107b67a",nullptr))return false;
    const std::vector<unsigned char> million(1000000,'a');
    return SyntheticDigestVector(million.data(),million.size(),"7707d6ae4e027c70eea2a935c2296f21","34aa973cd4c4daa4f61eeb2bdbad27316534016f");
}
class OriginalMsfSyntheticRegression {
    static constexpr SIZE_T Page=4096;
    static void Put(std::vector<unsigned char>& b,SIZE_T at,std::uint32_t value) {
        for(unsigned i=0;i<4;++i)b[at+i]=static_cast<unsigned char>(value>>(i*8));
    }
    static std::vector<unsigned char> Body(std::uint32_t moduleBytes=0,std::uint32_t c13Bytes=0) {
        std::vector<unsigned char> b(9*Page);
        constexpr char magic[]="Microsoft C/C++ MSF 7.00\r\n\x1a" "DS\0\0\0";
        memcpy(b.data(),magic,32);
        Put(b,32,Page);Put(b,36,1);Put(b,40,9);Put(b,44,36);Put(b,52,3);
        Put(b,3*Page,4);
        Put(b,4*Page,5);
        Put(b,4*Page+4,UINT32_MAX);Put(b,4*Page+8,28);Put(b,4*Page+12,UINT32_MAX);
        Put(b,4*Page+16,64+moduleBytes);Put(b,4*Page+20,c13Bytes);
        Put(b,4*Page+24,5);Put(b,4*Page+28,6);
        // Empty stream4 has no block reference; retain exact directory size.
        if(c13Bytes)Put(b,4*Page+32,8);else Put(b,44,32);
        Put(b,5*Page,20000404);Put(b,5*Page+8,1);
        Put(b,6*Page,UINT32_MAX);Put(b,6*Page+4,19990903);Put(b,6*Page+8,1);
        b[6*Page+15]=0x80;b[6*Page+58]=0x64;b[6*Page+59]=0x86;
        // Missing symbol indices are distinct from stream0.
        for(const SIZE_T offset:{SIZE_T(12),SIZE_T(16),SIZE_T(20)}){
            b[6*Page+offset]=0xff;b[6*Page+offset+1]=0xff;
        }
        Put(b,6*Page+24,moduleBytes);
        if(moduleBytes>=64){
            const SIZE_T m=6*Page+64;
            b[m+34]=c13Bytes?4:0xff;b[m+35]=c13Bytes?0:0xff;
            Put(b,m+44,c13Bytes);
        }
        return b;
    }
    static void Parse(OriginalHeldMsfSource& owner,const std::vector<unsigned char>& b) {
        owner.attempted_=true;owner.bodyOriginal_=b.data();owner.bodyExtent_=b.size();
        owner.ObserveCapturedBody(SIZE_MAX);
        // Never install NativeHeldFiles/Requirement identity on a synthetic owner.
    }
public:
    static bool Run() {
        {
            // Synthetic candidate consumer only: the range prerequisite is supplied,
            // never observed from NativeHeldFiles and never promoted to native authority.
            auto b=Body(68,52);const SIZE_T module=6*Page+64;
            Put(b,module+36,52);Put(b,module+44,0);Put(b,8*Page,4);
            for(SIZE_T i=0;i<2;++i){const SIZE_T record=8*Page+4+i*24;
                b[record]=22;b[record+2]=0x0c;b[record+3]=0x11;
                Put(b,record+4,1);Put(b,record+8,static_cast<std::uint32_t>(i*16));
                b[record+12]=1;memcpy(b.data()+record+14,i?"__xd_z":"__xd_a",7);
            }
            OriginalHeldMsfSource artifact;Parse(artifact,b);
            if(!artifact.SymbolRosterComplete()||artifact.SymbolCount()!=2)return false;
            IMAGE_SECTION_HEADER section{};section.VirtualAddress=0x1000;section.Misc.VirtualSize=32;
            OriginalCoreClrCompiledGeometry geometry;geometry.derived=true;
            geometry.bodyOriginal=reinterpret_cast<const unsigned char*>(&section);geometry.bodyExtent=sizeof(section);
            geometry.nt.FileHeader.NumberOfSections=1;
            LoadedImageObservation image(0x1020,1);if(!image.PreownOriginal())return false;image.used=0x1020;
            OriginalCompilerRangeObservation supplied;supplied.result=OriginalCompilerRangeObservation::Result::Observed;
            supplied.artifactOriginal=&artifact;supplied.geometryOriginal=&geometry;supplied.imageOriginal=&image;
            OriginalCompilerTlsInitializerCandidates empty;
            if(!empty.Observe(supplied)||!empty.spanObserved||empty.spanOffset!=0x1000||empty.spanExtent!=24||
               empty.entriesVisited!=1||empty.nonnullEntries||empty.beginMatches!=1||empty.endMatches!=1)return false;
            image.bytes[0x1000]=1;OriginalCompilerTlsInitializerCandidates sentinel;
            if(sentinel.Observe(supplied)||sentinel.spanObserved)return false;
            image.bytes[0x1000]=0;image.bytes[0x1008]=1;
            OriginalCompilerTlsInitializerCandidates target;
            if(target.Observe(supplied)||!target.spanObserved||target.nonnullEntries!=1||target.unresolvedTargets!=1)return false;
        }
        {
            // Same actual DBI producer/consumer, synthetic compiland and raw public record.
            auto b=Body(68,16);const SIZE_T dbi=6*Page,module=dbi+64,sc=dbi+132;
            b[module+34]=0xff;b[module+35]=0xff;Put(b,module+44,0);
            Put(b,4*Page+16,164);Put(b,dbi+28,32);Put(b,sc,0xeffe0000u+19970605u);
            b[dbi+20]=4;b[dbi+21]=0;
            b[sc+4]=3;Put(b,sc+8,0x1200);Put(b,sc+12,0x80);
            b[8*Page]=14;b[8*Page+2]=0x0e;b[8*Page+3]=0x11;
            Put(b,8*Page+4,2);Put(b,8*Page+8,0x1234);b[8*Page+12]=3;b[8*Page+14]='v';
            OriginalHeldMsfSource owner;Parse(owner,b);
            if(!owner.SectionContributionsComplete()||owner.SectionContributionCount()!=1||
               !owner.ModulesComplete()||!owner.GlobalSymbolRecordsComplete())return false;
            const auto& symbol=owner.OriginalGlobalSymbols()[0];
            if(!symbol.contributionComparisonAttempted||symbol.contributionMatches!=1||
               symbol.contributionOrdinal!=0||symbol.contributionModuleOrdinal!=0||
               symbol.moduleOrdinal!=SIZE_MAX)return false;
            Put(b,4*Page+16,168);Put(b,dbi+28,36);Put(b,sc,0xeffe0000u+20140516u);
            Put(b,sc+32,0x12345678);
            OriginalHeldMsfSource v2;Parse(v2,b);
            if(!v2.SectionContributionsComplete()||v2.SectionContributionCount()!=1||
               v2.OriginalSectionContributions()[0].originalExtent!=32||
               v2.OriginalSectionContributions()[0].coffSection!=0x12345678||
               v2.OriginalGlobalSymbols()[0].contributionMatches!=1)return false;
            Put(b,sc,0x12345678);OriginalHeldMsfSource unsupported;Parse(unsupported,b);
            if(unsupported.SectionContributionsComplete()||unsupported.SectionContributionCount()||
               unsupported.SectionContributionVersionOriginal()!=0x12345678)return false;
            Put(b,sc,0xeffe0000u+19970605u);
            // An overlapping original contribution cannot select either compiland by order.
            Put(b,4*Page+16,192);Put(b,dbi+28,60);
            memcpy(b.data()+sc+32,b.data()+sc+4,28);
            OriginalHeldMsfSource overlap;Parse(overlap,b);
            const auto& ambiguous=overlap.OriginalGlobalSymbols()[0];
            if(!overlap.SectionContributionsComplete()||ambiguous.contributionMatches!=2||
               ambiguous.contributionOrdinal!=SIZE_MAX||ambiguous.contributionModuleOrdinal!=SIZE_MAX)return false;
            // Signed sentinel and incomplete tail remain original observations, not memberships.
            Put(b,sc+8,UINT32_MAX);Put(b,sc+36,UINT32_MAX);
            OriginalHeldMsfSource sentinel;Parse(sentinel,b);
            if(sentinel.OriginalGlobalSymbols()[0].contributionMatches||
               sentinel.OriginalSectionContributions()[0].nonnegativeRange)return false;
            Put(b,4*Page+16,193);Put(b,dbi+28,61);
            OriginalHeldMsfSource tail;Parse(tail,b);
            if(tail.SectionContributionsComplete()||tail.SectionContributionCount()!=2||
               tail.OriginalGlobalSymbols()[0].contributionComparisonAttempted)return false;
        }
        {
            auto b=Body(68,40);const SIZE_T dbi=6*Page,module=dbi+64;
            Put(b,module+44,0);Put(b,4*Page+16,148);
            Put(b,dbi+52,4);Put(b,dbi+48,12);Put(b,dbi+132,0x12345678);
            for(SIZE_T i=0;i<12;++i)b[dbi+136+i]=0xff;
            b[dbi+146]=4;b[dbi+147]=0;
            Put(b,8*Page+8,100);Put(b,8*Page+12,0x1000);
            OriginalHeldMsfSource owner;Parse(owner,b);
            const auto& debug=owner.OriginalOptionalDebug();IMAGE_SECTION_HEADER section{};
            if(!debug.complete||debug.offset!=136||debug.indices!=6||debug.streams[5]!=4||
               !debug.sectionHeadersObserved||debug.sectionCount!=1||debug.omapFromSourcePresent||
               !owner.ReadOriginalSectionHeader(0,false,section)||section.VirtualAddress!=0x1000||
               owner.ReadOriginalSectionHeader(1,false,section))return false;
            b[dbi+142]=4;b[dbi+143]=0;
            OriginalHeldMsfSource translated;Parse(translated,b);
            if(!translated.OriginalOptionalDebug().omapToSourcePresent)return false;
            Put(b,dbi+48,11);Put(b,4*Page+16,147);
            OriginalHeldMsfSource odd;Parse(odd,b);
            if(odd.OriginalOptionalDebug().complete)return false;
        }
        {
            auto b=Body(68,56);const SIZE_T stream=8*Page;
            Put(b,stream,0xf4);Put(b,stream+4,8); // One original zero-size checksum record.
            Put(b,stream+16,0xf2);Put(b,stream+20,32);
            Put(b,stream+24,100);b[stream+28]=1;Put(b,stream+32,20);
            Put(b,stream+36,0);Put(b,stream+40,1);Put(b,stream+44,20);
            Put(b,stream+48,0);Put(b,stream+52,12);
            OriginalHeldMsfSource owner;Parse(owner,b);
            if(!owner.LineBlocksComplete()||owner.LineBlockCount()!=1)return false;
            const auto& line=owner.OriginalLineBlocks()[0];
            if(!line.complete||line.moduleOrdinal||line.fragmentOffset!=24||line.blockOffset!=36||
               line.linesOffset!=48||line.codeOffset!=100||line.codeSection!=1||line.codeBytes!=20||
               line.lineCount!=1||line.checksumMatches!=1||line.checksumOrdinal!=0)return false;
            OriginalHeldMsfSource::LinePositionOriginal position;
            if(!owner.ReadOriginalLinePosition(0,0,position)||!position.attempted||!position.read||
               position.artifactOriginal!=&owner||position.streamOrdinal!=4||position.streamOffset!=48||
               position.sectionOffset!=100||position.startLine!=12||position.endLine!=12||
               position.columnsRead||position.alwaysStep||position.neverStep||
               owner.ReadOriginalLinePosition(1,1,position)||position.blockOrdinal!=0||position.lineOrdinal!=0)return false;
            OriginalHeldMsfSource::LinePositionOriginal absent;
            if(owner.ReadOriginalLinePosition(0,1,absent)||!absent.attempted||absent.read)return false;
            Put(b,stream+52,0x8300000cu);OriginalHeldMsfSource statement;Parse(statement,b);
            OriginalHeldMsfSource::LinePositionOriginal marked;
            if(!statement.ReadOriginalLinePosition(0,0,marked)||!marked.statement||marked.startLine!=12||marked.endLine!=15)return false;
            Put(b,stream+52,0xfeefee);OriginalHeldMsfSource special;Parse(special,b);
            OriginalHeldMsfSource::LinePositionOriginal marker;
            if(!special.ReadOriginalLinePosition(0,0,marker)||!marker.alwaysStep||marker.neverStep)return false;
            Put(b,stream+52,12);
            owner.ObserveSourceDigests(nullptr,0);
            if(line.sourceCorrespondenceAttempted||line.sourceChecksumCorrespondenceObserved||
               line.heldSourceOrdinal!=SIZE_MAX||line.heldSourceRequirementOriginal)return false;
            Put(b,stream+40,2);OriginalHeldMsfSource overrun;Parse(overrun,b);
            if(overrun.LineBlocksComplete()||overrun.LineBlockCount())return false;
            Put(b,stream+40,1);b[stream+30]=2;
            OriginalHeldMsfSource flags;Parse(flags,b);
            if(flags.LineBlocksComplete()||flags.LineBlockCount())return false;
        }
        {
            auto b=Body(68,32);const SIZE_T module=6*Page+64;
            Put(b,module+36,32);Put(b,module+44,0);
            Put(b,8*Page,4);Put(b,8*Page+4,26u|(0x113cu<<16));
            b[8*Page+12]=0xd0;b[8*Page+30]='v';
            OriginalHeldMsfSource owner;Parse(owner,b);
            if(!owner.ModulesComplete()||owner.ModuleCount()!=1)return false;
            const auto& row=owner.OriginalModules()[0];
            if(!row.symbolsComplete||row.symbolRecords!=1||row.compile3Records!=1||
               row.symbolSignature!=4||row.compile3Original[4]!=0xd0)return false;
            if(!owner.SymbolRosterComplete()||owner.SymbolCount()!=1)return false;
            const auto& symbol=owner.OriginalSymbols()[0];
            if(!symbol.complete||!symbol.compile3||symbol.procedure||symbol.moduleOrdinal||
               symbol.streamOffset!=4||symbol.originalLength!=26||symbol.compilerMachine!=0xd0||
               symbol.nameOffset!=30||symbol.nameExtent!=2)return false;
            b[8*Page+31]='x';OriginalHeldMsfSource unterminated;Parse(unterminated,b);
            const auto& bad=unterminated.OriginalModules()[0];
            if(bad.symbolsComplete||bad.compile3Records!=1||bad.symbolRecords||
               bad.symbolCursorOriginal!=4||bad.symbolLengthOriginal!=26)return false;
            if(unterminated.SymbolRosterComplete()||unterminated.SymbolCount())return false;
            Put(b,8*Page+4,31u|(0x113cu<<16));
            OriginalHeldMsfSource overrun;Parse(overrun,b);
            if(overrun.OriginalModules()[0].symbolsComplete||
               overrun.OriginalModules()[0].compile3Records)return false;
        }
        {
            auto b=Body(68,48);const SIZE_T module=6*Page+64;
            Put(b,module+36,48);Put(b,module+44,0);Put(b,8*Page,4);
            // Unknown complete record is retained before a concrete GPROC32 record.
            Put(b,8*Page+4,2u|(0x7777u<<16));
            Put(b,8*Page+8,38u|(0x1110u<<16));
            Put(b,8*Page+12+12,19);Put(b,8*Page+12+28,0x1234);
            b[8*Page+12+32]=3;b[8*Page+47]=0;
            OriginalHeldMsfSource owner;Parse(owner,b);
            if(!owner.SymbolRosterComplete()||owner.SymbolCount()!=2)return false;
            const auto* rows=owner.OriginalSymbols();
            if(!rows[0].complete||rows[0].kind!=0x7777||rows[0].fixedExtent||
               !rows[1].procedure||rows[1].compile3||rows[1].streamOffset!=8||
               rows[1].codeBytes!=19||rows[1].codeOffset!=0x1234||rows[1].codeSection!=3||
               rows[1].nameExtent!=1)return false;
        }
        for(unsigned kind=0x110c;kind<=0x110e;++kind){
            auto b=Body(68,20);const SIZE_T module=6*Page+64,stream=8*Page;
            Put(b,module+36,20);Put(b,module+44,0);Put(b,stream,4);
            Put(b,stream+4,14u|(kind<<16));Put(b,stream+8,0x12345678);
            Put(b,stream+12,0x9876);b[stream+16]=2;b[stream+18]='v';
            OriginalHeldMsfSource owner;Parse(owner,b);
            if(!owner.SymbolRosterComplete()||owner.SymbolCount()!=1)return false;
            const auto& symbol=owner.OriginalSymbols()[0];
            if(!symbol.complete||symbol.kind!=kind||symbol.procedure||symbol.compile3||
               symbol.data!=(kind!=0x110e)||symbol.publicSymbol!=(kind==0x110e)||
               symbol.fixedExtent!=10||symbol.dataTypeOrPublicFlags!=0x12345678||
               symbol.dataOffset!=0x9876||symbol.dataSection!=2||symbol.nameExtent!=2)return false;
            b[stream+19]='x';OriginalHeldMsfSource unterminated;Parse(unterminated,b);
            if(unterminated.OriginalModules()[0].symbolsComplete||unterminated.SymbolRosterComplete())return false;
            Put(b,stream+4,11u|(kind<<16));OriginalHeldMsfSource truncated;Parse(truncated,b);
            if(truncated.OriginalModules()[0].symbolsComplete||truncated.SymbolRosterComplete())return false;
        }
        {
            auto b=Body(0,16);const SIZE_T stream=8*Page;
            b[6*Page+20]=4;b[6*Page+21]=0;
            // DBI record stream starts at its first record, with no module signature.
            Put(b,stream,14u|(0x110eu<<16));Put(b,stream+4,2);
            Put(b,stream+8,0x1234);b[stream+12]=3;b[stream+14]='v';
            OriginalHeldMsfSource owner;Parse(owner,b);
            if(!owner.GlobalSymbolRecordsComplete()||owner.GlobalSymbolCount()!=1)return false;
            const auto& symbol=owner.OriginalGlobalSymbols()[0];
            if(!symbol.complete||!symbol.publicSymbol||symbol.moduleOrdinal!=SIZE_MAX||
               symbol.streamOrdinal!=4||symbol.streamOffset||symbol.dataTypeOrPublicFlags!=2||
               symbol.dataOffset!=0x1234||symbol.dataSection!=3||symbol.nameExtent!=2)return false;
            b[stream+15]='x';OriginalHeldMsfSource name;Parse(name,b);
            if(name.GlobalSymbolRecordsComplete()||name.GlobalSymbolCount()||
               name.GlobalSymbolPrefixOriginal().originalLength!=14||
               name.GlobalSymbolPrefixOriginal().nameExtent!=2)return false;
            b=Body(0,18);b[6*Page+20]=4;b[6*Page+21]=0;
            Put(b,stream,14u|(0x110du<<16));b[stream+14]='v';
            OriginalHeldMsfSource tail;Parse(tail,b);
            if(tail.GlobalSymbolRecordsComplete()||tail.GlobalSymbolCount()!=1||
               tail.GlobalSymbolCursorOriginal()!=16||!tail.OriginalGlobalSymbols()[0].complete||
               !tail.OriginalGlobalSymbols()[0].data||tail.GlobalSymbolPrefixOriginal().complete)return false;
        }
        {
            auto b=Body();OriginalHeldMsfSource owner;Parse(owner,b);
            if(owner.Outcome()!=OriginalHeldMsfSource::Result::Geometry||
               !owner.OriginalInfoHeader().supported||!owner.OriginalDbiHeader().supported||
               owner.FilesOriginal()||owner.RequirementOriginal())return false;
        }
        {
            auto b=Body();Put(b,44,36);Put(b,4*Page+8,4100);
            Put(b,4*Page+24,5);Put(b,4*Page+28,7);Put(b,4*Page+32,6);
            b[5*Page+4094]=1;b[5*Page+4095]=2;
            for(unsigned i=0;i<4;++i)b[7*Page+i]=static_cast<unsigned char>(i+3);
            OriginalHeldMsfSource owner;Parse(owner,b);unsigned char result[6]{};
            if(!owner.ReadStream(1,4094,result,6))return false;
            for(unsigned i=0;i<6;++i)if(result[i]!=i+1)return false;
            Put(b,4*Page+28,5);OriginalHeldMsfSource aliased;Parse(aliased,b);
            if(aliased.Outcome()==OriginalHeldMsfSource::Result::Geometry)return false;
        }
        {
            auto b=Body();Put(b,4*Page+16,63);OriginalHeldMsfSource owner;Parse(owner,b);
            if(owner.OriginalDbiHeader().read)return false;
        }
        {
            auto b=Body();Put(b,4*Page+8,67);Put(b,5*Page+28,7);
            memcpy(b.data()+5*Page+32,"/names",7);
            Put(b,5*Page+39,1);Put(b,5*Page+43,1);
            Put(b,5*Page+47,1);Put(b,5*Page+51,1);Put(b,5*Page+55,0);
            Put(b,5*Page+59,0);Put(b,5*Page+63,4);
            OriginalHeldMsfSource owner;Parse(owner,b);
            if(!owner.OriginalNamedMap().complete||owner.OriginalNamedMap().globalNamesStream!=4)return false;
            Put(b,4*Page+8,71);Put(b,5*Page+55,1);Put(b,5*Page+59,1);
            Put(b,5*Page+63,0);Put(b,5*Page+67,4);
            OriginalHeldMsfSource overlap;Parse(overlap,b);
            if(overlap.OriginalNamedMap().complete)return false;
        }
        {
            auto b=Body(68);for(SIZE_T i=64;i<68;++i)b[6*Page+64+i]='x';
            OriginalHeldMsfSource owner;Parse(owner,b);
            if(owner.ModulesComplete()||owner.ModuleCount()!=1)return false;
        }
        {
            auto b=Body(0,28);Put(b,4*Page+8,67);Put(b,5*Page+28,7);
            memcpy(b.data()+5*Page+32,"/names",7);
            Put(b,5*Page+39,1);Put(b,5*Page+43,1);Put(b,5*Page+47,1);
            Put(b,5*Page+51,1);Put(b,5*Page+55,0);Put(b,5*Page+59,0);Put(b,5*Page+63,4);
            Put(b,8*Page,0xeffeeffeu);Put(b,8*Page+4,1);Put(b,8*Page+8,4);
            b[8*Page+13]='a';Put(b,8*Page+16,1);Put(b,8*Page+20,1);Put(b,8*Page+24,1);
            OriginalHeldMsfSource owner;Parse(owner,b);
            if(!owner.OriginalGlobalStrings().complete||owner.OriginalGlobalStrings().nonemptyBuckets!=1)return false;
            Put(b,8*Page+20,4);OriginalHeldMsfSource outside;Parse(outside,b);
            if(outside.OriginalGlobalStrings().complete)return false;
        }
        {
            auto b=Body(68,8);Put(b,8*Page,0xf4);Put(b,8*Page+4,100);
            OriginalHeldMsfSource owner;Parse(owner,b);
            if(owner.ModulesComplete()||owner.OriginalModules()[0].c13Complete)return false;
        }
        {
            auto b=Body(68,32);
            Put(b,8*Page,0xf4);Put(b,8*Page+8,0xf4);
            Put(b,8*Page+16,0xf3);Put(b,8*Page+24,0xf3);
            OriginalHeldMsfSource owner;Parse(owner,b);
            if(!owner.ModulesComplete()||owner.OriginalModules()[0].checksumSubsections!=2||
               owner.OriginalModules()[0].stringSubsections!=2||owner.ChecksumCount()!=0)return false;
        }
        {
            auto b=Body(72);OriginalHeldMsfSource owner;Parse(owner,b);
            if(owner.ModulesComplete()||owner.ModuleCount()!=1||
               !owner.OriginalModules()[0].extentsObserved)return false;
            // SAME actual parser's partial module allocation is exported even
            // though the complete-module transition failed. No native failure
            // or authenticity claim is made by this synthetic body.
            std::array<OriginalHeldMsfSource::RetainedAllocationSpan,8> spans{};
            if(!owner.RetainedAllocationSpansOriginal(spans)||
                spans[2].original!=owner.OriginalModules()||
                spans[2].bytes!=owner.moduleCapacity_*sizeof(OriginalHeldMsfSource::Module)||
                !spans[2].bytes||spans[7].original||spans[7].bytes)return false;
        }
        return true;
    }
};
}
int main(){return template_creator::SyntheticDigestKnownAnswers()&&template_creator::OriginalJsonSourceRegression()&&template_creator::OriginalMsfSyntheticRegression::Run()?0:1;}

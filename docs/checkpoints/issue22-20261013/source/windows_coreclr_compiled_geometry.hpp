// UNCOMPILED held-file PE geometry. Conservative byte bounds pay for every
// mapped executable range, including leaf code outside .pdata. They establish
// no source/build identity, complete graph, supported path or native fit.
#pragma once
#include "windows_held_files.hpp"
#include "windows_memory.hpp"
#include "windows_amd64_caller_boundary.hpp"
namespace template_creator {
struct OriginalCoreClrCompiledGeometry {
    const NativeHeldFiles* filesOriginal=nullptr;
    const HeldNativeFile* fileOriginal=nullptr;
    const unsigned char* bodyOriginal=nullptr;
    SIZE_T fileOrdinal=SIZE_MAX,bodyExtent=0,sectionOffset=0;
    IMAGE_DOS_HEADER dos{};IMAGE_NT_HEADERS64 nt{};
    SIZE_T executableByteBound=0,executableSections=0;
    bool attempted=false,derived=false;
    const char* firstFailure=nullptr;
private:
    bool Fail(const char* reason) noexcept {if(!firstFailure)firstFailure=reason;return false;}
    bool Span(SIZE_T start,SIZE_T extent) const noexcept {return start<=bodyExtent && extent<=bodyExtent-start;}
    bool Section(SIZE_T ordinal,IMAGE_SECTION_HEADER& section) const noexcept {
        if(ordinal>=nt.FileHeader.NumberOfSections || ordinal>(SIZE_MAX-sectionOffset)/sizeof(section))return false;
        const SIZE_T offset=sectionOffset+ordinal*sizeof(section);
        if(!Span(offset,sizeof(section)))return false;memcpy(&section,bodyOriginal+offset,sizeof(section));return true;
    }
    bool FileSpanForRva(DWORD rva,SIZE_T extent,SIZE_T& offset) const noexcept {
        bool found=false;
        for(SIZE_T i=0;i<nt.FileHeader.NumberOfSections;++i) {
            IMAGE_SECTION_HEADER section{};if(!Section(i,section))return false;
            if(rva<section.VirtualAddress)continue;
            const SIZE_T delta=SIZE_T(rva)-section.VirtualAddress;
            if(delta>section.SizeOfRawData || extent>section.SizeOfRawData-delta)continue;
            if(found || delta>SIZE_MAX-section.PointerToRawData)return false;
            offset=SIZE_T(section.PointerToRawData)+delta;
            if(!Span(offset,extent))return false;found=true;
        }return found;
    }
public:
    bool OriginalSectionHeader(SIZE_T ordinal,IMAGE_SECTION_HEADER& section) const noexcept {
        return derived&&Section(ordinal,section);
    }
    bool Derive(const NativeHeldFiles& files,SIZE_T ordinal) noexcept {
        if(attempted)return false;attempted=true;filesOriginal=&files;fileOrdinal=ordinal;
        if(files.Failed() || ordinal>=files.FileCount())return Fail("compiled_geometry_original_file_missing");
        fileOriginal=&files.PrivateFiles()[ordinal];
        if(!fileOriginal->valid || fileOriginal->closeAttempted || fileOriginal->readCompletionUnknown ||
            !files.ExactCapturedSourceSpan(ordinal,bodyOriginal,bodyExtent))return Fail("compiled_geometry_file_custody");
        if(!Span(0,sizeof(dos)))return Fail("compiled_geometry_DOS_span");
        memcpy(&dos,bodyOriginal,sizeof(dos));
        if(dos.e_magic!=IMAGE_DOS_SIGNATURE || dos.e_lfanew<0 || !Span(static_cast<SIZE_T>(dos.e_lfanew),sizeof(nt)))
            return Fail("compiled_geometry_NT_span");
        const SIZE_T offset=static_cast<SIZE_T>(dos.e_lfanew);memcpy(&nt,bodyOriginal+offset,sizeof(nt));
        if(nt.Signature!=IMAGE_NT_SIGNATURE || nt.FileHeader.Machine!=IMAGE_FILE_MACHINE_AMD64 ||
            nt.OptionalHeader.Magic!=IMAGE_NT_OPTIONAL_HDR64_MAGIC ||
            nt.FileHeader.SizeOfOptionalHeader<sizeof(IMAGE_OPTIONAL_HEADER64) ||
            !nt.FileHeader.NumberOfSections || nt.FileHeader.NumberOfSections>96 || !nt.OptionalHeader.SizeOfImage ||
            nt.OptionalHeader.SizeOfHeaders>bodyExtent ||
            !Span(offset+offsetof(IMAGE_NT_HEADERS64,OptionalHeader),nt.FileHeader.SizeOfOptionalHeader))
            return Fail("compiled_geometry_PE_shape");
        const SIZE_T optionalOffset=offset+offsetof(IMAGE_NT_HEADERS64,OptionalHeader);
        if(nt.FileHeader.SizeOfOptionalHeader>SIZE_MAX-optionalOffset)return Fail("compiled_geometry_section_offset_width");
        sectionOffset=optionalOffset+nt.FileHeader.SizeOfOptionalHeader;
        if(nt.FileHeader.NumberOfSections>(bodyExtent-sectionOffset)/sizeof(IMAGE_SECTION_HEADER))
            return Fail("compiled_geometry_section_table_span");
        for(SIZE_T i=0;i<nt.FileHeader.NumberOfSections;++i) {
            IMAGE_SECTION_HEADER section{};if(!Section(i,section))return Fail("compiled_geometry_section_read");
            // Loader mapping extent must be bound separately to the actual
            // captured image. Include raw padding conservatively in the bound.
            const SIZE_T extent=section.Misc.VirtualSize>section.SizeOfRawData?section.Misc.VirtualSize:section.SizeOfRawData;
            if(section.VirtualAddress>nt.OptionalHeader.SizeOfImage ||
                extent>nt.OptionalHeader.SizeOfImage-section.VirtualAddress ||
                (section.SizeOfRawData && (!section.PointerToRawData || !Span(section.PointerToRawData,section.SizeOfRawData))))
                return Fail("compiled_geometry_section_extent");
            for(SIZE_T prior=0;prior<i;++prior) {
                IMAGE_SECTION_HEADER other{};if(!Section(prior,other))return Fail("compiled_geometry_prior_section");
                const SIZE_T otherExtent=other.Misc.VirtualSize>other.SizeOfRawData?other.Misc.VirtualSize:other.SizeOfRawData;
                if(extent && otherExtent && section.VirtualAddress<SIZE_T(other.VirtualAddress)+otherExtent &&
                    other.VirtualAddress<SIZE_T(section.VirtualAddress)+extent)return Fail("compiled_geometry_overlapping_mapped_ranges");
            }
            if(!(section.Characteristics&IMAGE_SCN_MEM_EXECUTE) || !extent)continue;
            if(extent>SIZE_MAX-executableByteBound)return Fail("compiled_geometry_executable_sum_width");
            executableByteBound+=extent;++executableSections;
        }
        if(!executableByteBound)return Fail("compiled_geometry_no_executable_range");
        derived=true;return true;
    }
    bool DecodeHeldInstruction(DWORD rva,OriginalAmd64Instruction& instruction) const noexcept {
        if(!derived||!filesOriginal||!fileOriginal||!fileOriginal->valid||fileOriginal->closeAttempted||
           fileOriginal->readCompletionUnknown)return false;
        const unsigned char* held=nullptr;SIZE_T extent=0;
        if(!filesOriginal->ExactCapturedSourceSpan(fileOrdinal,held,extent)||held!=bodyOriginal||extent!=bodyExtent)return false;
        SIZE_T selected=SIZE_MAX,offset=0,available=0;
        for(SIZE_T i=0;i<nt.FileHeader.NumberOfSections;++i){IMAGE_SECTION_HEADER section{};
            if(!Section(i,section))return false;
            if(!(section.Characteristics&IMAGE_SCN_MEM_EXECUTE)||rva<section.VirtualAddress)continue;
            const SIZE_T delta=SIZE_T(rva)-section.VirtualAddress;
            if(delta>=section.SizeOfRawData)continue;
            if(selected!=SIZE_MAX||delta>SIZE_MAX-section.PointerToRawData)return false;
            selected=i;offset=SIZE_T(section.PointerToRawData)+delta;
            available=section.SizeOfRawData-delta;if(available>15)available=15;
        }
        if(selected==SIZE_MAX||!available||!Span(offset,available))return false;
        unsigned char prefix[15]{};memcpy(prefix,bodyOriginal+offset,available);
        if(!OriginalAmd64CallerBoundary::DecodeSuppliedInstructionContract(prefix,available,instruction))return false;
        instruction.rva=rva;return true;
        // Held byte projection only; no root boundary, complete graph or executed instruction authority.
    }
    bool MatchesMappedCodeSpan(const LoadedImageObservation& image,DWORD rva,SIZE_T extent) const noexcept {
        if(!derived || !filesOriginal || !fileOriginal || !fileOriginal->valid || fileOriginal->closeAttempted ||
            fileOriginal->readCompletionUnknown || !image.complete || image.failed || !image.bytes ||
            image.imageSize!=nt.OptionalHeader.SizeOfImage || image.used!=image.imageSize ||
            image.retainedByteExtent<image.imageSize || image.imageSize>image.byteCapacity ||
            !extent || rva>image.imageSize || extent>image.imageSize-rva)return false;
        SIZE_T match=SIZE_MAX;
        for(SIZE_T i=0;i<nt.FileHeader.NumberOfSections;++i) {
            IMAGE_SECTION_HEADER section{};if(!Section(i,section))return false;
            const SIZE_T mapped=section.Misc.VirtualSize>section.SizeOfRawData?section.Misc.VirtualSize:section.SizeOfRawData;
            if(!(section.Characteristics&IMAGE_SCN_MEM_EXECUTE) || rva<section.VirtualAddress ||
                SIZE_T(rva)-section.VirtualAddress>mapped || extent>mapped-(SIZE_T(rva)-section.VirtualAddress))continue;
            if(match!=SIZE_MAX)return false;match=i;
            const SIZE_T delta=SIZE_T(rva)-section.VirtualAddress;
            for(SIZE_T byte=0;byte<extent;++byte) {
                const SIZE_T at=delta+byte;
                // Zero-filled virtual tail is explicit. Alignment padding
                // outside these summed spans cannot qualify by native Protect.
                unsigned char expected=0;
                if(at<section.SizeOfRawData) {
                    if(!Span(SIZE_T(section.PointerToRawData)+at,1))return false;
                    expected=bodyOriginal[SIZE_T(section.PointerToRawData)+at];
                }
                if(image.bytes[SIZE_T(rva)+byte]!=expected)return false;
            }
        }
        // Relocated/patched code is not accepted by byte resemblance; an actual
        // required profile needing a fixup needs its genuine source semantics.
        return match!=SIZE_MAX;
    }
    bool ContainingRuntimeFunction(const LoadedImageObservation& image,DWORD rva,
            OriginalAmd64RuntimeFunction& function,SIZE_T& ordinal) const noexcept {
        if(!derived || !image.complete || image.failed || !image.bytes || image.imageSize!=nt.OptionalHeader.SizeOfImage ||
            image.used!=image.imageSize || image.retainedByteExtent<image.imageSize || image.imageSize>image.byteCapacity ||
            nt.OptionalHeader.NumberOfRvaAndSizes<=IMAGE_DIRECTORY_ENTRY_EXCEPTION)return false;
        const auto table=nt.OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXCEPTION];SIZE_T raw=0;
        if(!table.VirtualAddress || table.VirtualAddress%4 || !table.Size || table.Size%sizeof(function) ||
            table.VirtualAddress>image.imageSize || table.Size>image.imageSize-table.VirtualAddress ||
            !FileSpanForRva(table.VirtualAddress,table.Size,raw) ||
            memcmp(bodyOriginal+raw,image.bytes.get()+table.VirtualAddress,table.Size))return false;
        bool found=false;DWORD previousBegin=0;
        for(SIZE_T i=0;i<table.Size/sizeof(function);++i) {
            OriginalAmd64RuntimeFunction candidate{};memcpy(&candidate,bodyOriginal+raw+i*sizeof(candidate),sizeof(candidate));
            if(candidate.BeginAddress>=candidate.EndAddress || candidate.EndAddress>image.imageSize ||
                (i && candidate.BeginAddress<=previousBegin))return false;
            previousBegin=candidate.BeginAddress;
            if(rva<candidate.BeginAddress || rva>=candidate.EndAddress)continue;
            if(found)return false;found=true;function=candidate;ordinal=i;
        }
        if(!found || function.UnwindData%4 || !FileSpanForRva(function.UnwindData,4,raw) ||
            function.UnwindData>image.imageSize || 4>image.imageSize-function.UnwindData ||
            memcmp(bodyOriginal+raw,image.bytes.get()+function.UnwindData,4))return false;
        // This is range/metadata association, not an unwind of an untrusted
        // stack or proof that a caller executed. Chained/dynamic variants need
        // their selected genuine profile; no fabricated function start.
        const unsigned unwind=bodyOriginal[raw];
        return (unwind&7)==1 && !((unwind>>3)&~7u) && !((unwind>>3)&4u);
    }
    // The complete actual graph must additionally prove unique boundaries,
    // acyclicity or genuine finite loop expansion, every indirect/CFG range,
    // immutable source/currentness and mapped file correspondence. One byte per
    // instruction then makes this a conservative capacity bound, never a path.
};
}

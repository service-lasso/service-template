// Source-only projection of SAME retained mapped image bytes. No PDB or build authority.
#pragma once
#include "windows_memory.hpp"
#include <array>
namespace template_creator {
struct OriginalLoadedCodeView {
    enum class Result {Unobserved,IncompleteImage,Malformed,Missing,Ambiguous,Unsupported,Rsds};
    const LoadedImageObservation* imageOriginal=nullptr;
    Result result=Result::Unobserved;
    IMAGE_DOS_HEADER dos{}; IMAGE_NT_HEADERS64 nt{};
    IMAGE_DATA_DIRECTORY directorySpan{};
    IMAGE_DEBUG_DIRECTORY selectedDirectory{};
    std::array<unsigned char,16> guid{};
    DWORD age=0;
    SIZE_T directoriesVisited=0,selectedOrdinal=SIZE_MAX;
    SIZE_T payloadOffset=0,payloadExtent=0,pathOffset=0,pathExtent=0;
    bool attempted=false;
    bool Observe(const LoadedImageObservation& image) noexcept {
        if(attempted)return false;
        attempted=true;imageOriginal=&image;result=Result::IncompleteImage;
        if(!image.complete||image.failed||!image.bytes||!image.nativeBase||
           !image.imageSize||image.used!=image.imageSize||image.imageSize>image.byteCapacity||
           image.retainedByteExtent<image.imageSize)return false;
        result=Result::Malformed;
        const auto* bytes=image.bytes.get();
        const auto span=[&](SIZE_T offset,SIZE_T extent) noexcept {
            return offset<=image.imageSize&&extent<=image.imageSize-offset;
        };
        if(!span(0,sizeof(dos)))return false;
        memcpy(&dos,bytes,sizeof(dos));
        if(dos.e_magic!=IMAGE_DOS_SIGNATURE||dos.e_lfanew<0)return false;
        const SIZE_T ntOffset=static_cast<SIZE_T>(dos.e_lfanew);
        if(!span(ntOffset,sizeof(nt)))return false;
        memcpy(&nt,bytes+ntOffset,sizeof(nt));
        if(nt.Signature!=IMAGE_NT_SIGNATURE)return false;
        if(nt.FileHeader.Machine!=IMAGE_FILE_MACHINE_AMD64||
           nt.OptionalHeader.Magic!=IMAGE_NT_OPTIONAL_HDR64_MAGIC){result=Result::Unsupported;return false;}
        if(nt.FileHeader.SizeOfOptionalHeader<sizeof(IMAGE_OPTIONAL_HEADER64)||
           !span(ntOffset+offsetof(IMAGE_NT_HEADERS64,OptionalHeader),nt.FileHeader.SizeOfOptionalHeader)||
           nt.OptionalHeader.SizeOfImage!=image.imageSize||
           nt.OptionalHeader.NumberOfRvaAndSizes<=IMAGE_DIRECTORY_ENTRY_DEBUG)return false;
        directorySpan=nt.OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_DEBUG];
        if(!directorySpan.VirtualAddress&&!directorySpan.Size){result=Result::Missing;return false;}
        if(!directorySpan.VirtualAddress||!directorySpan.Size||
           directorySpan.Size%sizeof(IMAGE_DEBUG_DIRECTORY)||
           !span(directorySpan.VirtualAddress,directorySpan.Size))return false;
        const SIZE_T count=directorySpan.Size/sizeof(IMAGE_DEBUG_DIRECTORY);
        for(SIZE_T i=0;i<count;++i){
            IMAGE_DEBUG_DIRECTORY row{};
            memcpy(&row,bytes+directorySpan.VirtualAddress+i*sizeof(row),sizeof(row));
            ++directoriesVisited;
            if(row.Type!=IMAGE_DEBUG_TYPE_CODEVIEW)continue;
            if(selectedOrdinal!=SIZE_MAX){result=Result::Ambiguous;return false;}
            selectedOrdinal=i;selectedDirectory=row;
        }
        if(selectedOrdinal==SIZE_MAX){result=Result::Missing;return false;}
        // PointerToRawData is a file offset, never a mapped-image RVA fallback.
        payloadOffset=selectedDirectory.AddressOfRawData;payloadExtent=selectedDirectory.SizeOfData;
        if(!payloadOffset){result=Result::Unsupported;return false;}
        if(payloadExtent<25||!span(payloadOffset,payloadExtent))return false;
        if(memcmp(bytes+payloadOffset,"RSDS",4)){result=Result::Unsupported;return false;}
        memcpy(guid.data(),bytes+payloadOffset+4,guid.size());
        memcpy(&age,bytes+payloadOffset+20,sizeof(age));
        pathOffset=payloadOffset+24;
        const SIZE_T available=payloadExtent-24;
        while(pathExtent<available&&bytes[pathOffset+pathExtent])++pathExtent;
        if(!pathExtent||pathExtent==available)return false;
        ++pathExtent; // Original first-NUL span; trailing payload stays retained in image.
        result=Result::Rsds;return true;
    }
};
}

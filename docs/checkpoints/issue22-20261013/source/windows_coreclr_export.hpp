// UNCOMPILED read-only source projection. An export address is not runtime,
// caller/build/source/ABI authority or permission to change a thread context.
#pragma once
#include "windows_memory.hpp"
#include <cstddef>
namespace template_creator {
struct OriginalCoreClrInitializeExport {
    enum class Symbol { Initialize, ErrorWriter };
    Symbol symbol=Symbol::Initialize;
    enum class Result { Unattempted, IncompleteImage, Malformed, UnsupportedArchitecture,
        Missing, Ambiguous, ZeroTarget, Forwarded, Direct };
    const LoadedImageObservation* imageOriginal=nullptr;
    Result result=Result::Unattempted;
    IMAGE_DOS_HEADER dos{};
    IMAGE_NT_HEADERS64 nt{};
    IMAGE_EXPORT_DIRECTORY directory{};
    IMAGE_DATA_DIRECTORY exportSpan{};
    SIZE_T ntOffset=0,namesVisited=0,matchedIndex=SIZE_MAX;
    DWORD matchedNameRva=0,targetRva=0,forwarderLength=0;
    WORD ordinal=0;
    const void* address=nullptr;
    const MemoryRegion* targetRegionOriginal=nullptr;
    SIZE_T targetRegionOrdinal=SIZE_MAX;
    DWORD targetProtection=0;
    bool targetRegionAmbiguous=false,targetExecutableObserved=false;
    bool attempted=false;
    bool Observe(const LoadedImageObservation& image,Symbol selected=Symbol::Initialize) noexcept {
        if(attempted)return false;
        attempted=true;symbol=selected;imageOriginal=&image;result=Result::IncompleteImage;
        if(!image.complete || image.failed || !image.bytes || !image.nativeBase ||
            !image.imageSize || image.used!=image.imageSize || image.imageSize>image.byteCapacity ||
            image.retainedByteExtent<image.imageSize)return false;
        result=Result::Malformed;
        const auto* bytes=image.bytes.get();
        const auto span=[&](SIZE_T start,SIZE_T length) noexcept {
            return start<=image.imageSize && length<=image.imageSize-start;
        };
        if(!span(0,sizeof(dos)))return false;
        memcpy(&dos,bytes,sizeof(dos));
        if(dos.e_magic!=IMAGE_DOS_SIGNATURE || dos.e_lfanew<0)return false;
        ntOffset=static_cast<SIZE_T>(dos.e_lfanew);
        if(!span(ntOffset,sizeof(nt)))return false;
        memcpy(&nt,bytes+ntOffset,sizeof(nt));
        if(nt.Signature!=IMAGE_NT_SIGNATURE)return false;
        if(nt.FileHeader.Machine!=IMAGE_FILE_MACHINE_AMD64 ||
            nt.OptionalHeader.Magic!=IMAGE_NT_OPTIONAL_HDR64_MAGIC) {
            result=Result::UnsupportedArchitecture;return false;
        }
        if(nt.FileHeader.SizeOfOptionalHeader<sizeof(IMAGE_OPTIONAL_HEADER64) ||
            !span(ntOffset+offsetof(IMAGE_NT_HEADERS64,OptionalHeader),nt.FileHeader.SizeOfOptionalHeader) ||
            nt.OptionalHeader.SizeOfImage!=image.imageSize ||
            nt.OptionalHeader.NumberOfRvaAndSizes<=IMAGE_DIRECTORY_ENTRY_EXPORT)return false;
        exportSpan=nt.OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT];
        if(!exportSpan.VirtualAddress && !exportSpan.Size){result=Result::Missing;return false;}
        if(!exportSpan.VirtualAddress || exportSpan.Size<sizeof(directory) ||
            !span(exportSpan.VirtualAddress,exportSpan.Size))return false;
        memcpy(&directory,bytes+exportSpan.VirtualAddress,sizeof(directory));
        if((directory.NumberOfNames && (!directory.AddressOfNames || !directory.AddressOfNameOrdinals)) ||
            (directory.NumberOfFunctions && !directory.AddressOfFunctions))return false;
        if(directory.NumberOfNames>image.imageSize/sizeof(DWORD) ||
            directory.NumberOfFunctions>image.imageSize/sizeof(DWORD) ||
            !span(directory.AddressOfNames,SIZE_T(directory.NumberOfNames)*sizeof(DWORD)) ||
            !span(directory.AddressOfNameOrdinals,SIZE_T(directory.NumberOfNames)*sizeof(WORD)) ||
            !span(directory.AddressOfFunctions,SIZE_T(directory.NumberOfFunctions)*sizeof(DWORD)))return false;
        constexpr char initializer[]="coreclr_initialize",writer[]="coreclr_set_error_writer";
        const char* expected=symbol==Symbol::Initialize?initializer:writer;
        const SIZE_T expectedExtent=symbol==Symbol::Initialize?sizeof(initializer):sizeof(writer);
        for(DWORD i=0;i<directory.NumberOfNames;++i) {
            DWORD nameRva=0;
            memcpy(&nameRva,bytes+directory.AddressOfNames+SIZE_T(i)*sizeof(DWORD),sizeof(nameRva));
            if(!nameRva || !span(nameRva,1))return false;
            SIZE_T length=0;
            while(length<image.imageSize-nameRva && bytes[nameRva+length])++length;
            if(length==image.imageSize-nameRva)return false;
            ++namesVisited;
            if(length!=expectedExtent-1 || memcmp(bytes+nameRva,expected,expectedExtent))continue;
            if(matchedIndex!=SIZE_MAX){result=Result::Ambiguous;return false;}
            matchedIndex=i;matchedNameRva=nameRva;
            memcpy(&ordinal,bytes+directory.AddressOfNameOrdinals+SIZE_T(i)*sizeof(WORD),sizeof(ordinal));
            if(ordinal>=directory.NumberOfFunctions)return false;
            memcpy(&targetRva,bytes+directory.AddressOfFunctions+SIZE_T(ordinal)*sizeof(DWORD),sizeof(targetRva));
        }
        if(matchedIndex==SIZE_MAX){result=Result::Missing;return false;}
        if(!targetRva){result=Result::ZeroTarget;return false;}
        if(!span(targetRva,1))return false;
        if(targetRva>=exportSpan.VirtualAddress &&
            SIZE_T(targetRva)-exportSpan.VirtualAddress<exportSpan.Size) {
            const SIZE_T remaining=exportSpan.Size-(SIZE_T(targetRva)-exportSpan.VirtualAddress);
            SIZE_T length=0;
            while(length<remaining && bytes[targetRva+length])++length;
            if(!length || length==remaining)return false;
            forwarderLength=static_cast<DWORD>(length);
            result=Result::Forwarded;return false; // Exact retained bytes, no guessed destination.
        }
        const auto base=reinterpret_cast<std::uintptr_t>(image.nativeBase);
        if(targetRva>UINTPTR_MAX-base)return false;
        address=reinterpret_cast<const void*>(base+targetRva);
        // This is the protection observed during the SAME retained image read,
        // not a fresh query or an exclusion of another writable mapping.
        if(image.regions && image.regionCount<=image.regionCapacity) {
            for(SIZE_T i=0;i<image.regionCount;++i) {
                const auto& region=image.regions[i];
                if(region.queryReturned!=sizeof(region.original) || !region.readSucceeded ||
                    targetRva<region.dataOffset || SIZE_T(targetRva)-region.dataOffset>=region.readReturned)continue;
                if(targetRegionOriginal) {
                    targetRegionAmbiguous=true;targetRegionOriginal=nullptr;
                    targetRegionOrdinal=SIZE_MAX;targetProtection=0;break;
                }
                targetRegionOriginal=&region;targetRegionOrdinal=i;
                targetProtection=region.original.Protect;
            }
        }
        if(targetRegionOriginal && !targetRegionAmbiguous &&
            targetRegionOriginal->original.State==MEM_COMMIT &&
            targetRegionOriginal->original.Type==MEM_IMAGE &&
            targetRegionOriginal->original.AllocationBase==image.nativeBase &&
            !(targetProtection&(PAGE_GUARD|PAGE_NOACCESS))) {
            const DWORD access=targetProtection&0xff;
            targetExecutableObserved=access==PAGE_EXECUTE || access==PAGE_EXECUTE_READ ||
                access==PAGE_EXECUTE_READWRITE || access==PAGE_EXECUTE_WRITECOPY;
        }
        result=Result::Direct;
        return true; // Source address only; execution/source/occurrence joins remain required.
    }
};
}

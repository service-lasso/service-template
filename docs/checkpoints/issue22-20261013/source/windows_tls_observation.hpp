// UNCOMPILED projection of SAME retained loaded bytes, not callback invocation,
// source/build identity or product-owned actor/control-writer qualification.
#pragma once
#include "windows_memory.hpp"
namespace template_creator {
struct OriginalLoadedTlsObservation {
    enum class Outcome {Unobserved,Incomplete,Malformed,Unsupported,Absent,Complete};
    const LoadedImageObservation* imageOriginal=nullptr;
    Outcome outcome=Outcome::Unobserved;
    IMAGE_DATA_DIRECTORY span{};
    IMAGE_TLS_DIRECTORY64 directory{};
    SIZE_T callbackArrayOffset=0,callbackArrayExtent=0,callbackCount=0;
    bool attempted=false;
    bool Observe(const LoadedImageObservation& image) noexcept {
        if(attempted)return false;attempted=true;imageOriginal=&image;outcome=Outcome::Incomplete;
        if(!image.complete || image.failed || !image.bytes || !image.nativeBase ||
            !image.imageSize || image.imageSize>image.byteCapacity || image.used!=image.imageSize ||
            image.retainedByteExtent<image.imageSize)return false;
        outcome=Outcome::Malformed;
        const auto within=[&](SIZE_T at,SIZE_T length){return at<=image.imageSize && length<=image.imageSize-at;};
        IMAGE_DOS_HEADER dos{};IMAGE_NT_HEADERS64 nt{};
        if(!within(0,sizeof(dos)))return false;
        memcpy(&dos,image.bytes.get(),sizeof(dos));
        if(dos.e_magic!=IMAGE_DOS_SIGNATURE || dos.e_lfanew<0 ||
            !within(static_cast<SIZE_T>(dos.e_lfanew),sizeof(nt)))return false;
        memcpy(&nt,image.bytes.get()+static_cast<SIZE_T>(dos.e_lfanew),sizeof(nt));
        if(nt.Signature!=IMAGE_NT_SIGNATURE)return false;
        if(nt.FileHeader.Machine!=IMAGE_FILE_MACHINE_AMD64 || nt.OptionalHeader.Magic!=IMAGE_NT_OPTIONAL_HDR64_MAGIC) {
            outcome=Outcome::Unsupported;return false;
        }
        if(nt.FileHeader.SizeOfOptionalHeader<sizeof(IMAGE_OPTIONAL_HEADER64) ||
            nt.OptionalHeader.SizeOfImage!=image.imageSize)return false;
        if(nt.OptionalHeader.NumberOfRvaAndSizes<=IMAGE_DIRECTORY_ENTRY_TLS) {
            outcome=Outcome::Absent;return true;
        }
        span=nt.OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_TLS];
        if(!span.VirtualAddress && !span.Size){outcome=Outcome::Absent;return true;}
        if(!span.VirtualAddress || span.Size<sizeof(directory) || !within(span.VirtualAddress,span.Size))return false;
        memcpy(&directory,image.bytes.get()+span.VirtualAddress,sizeof(directory));
        if(!directory.AddressOfCallBacks){outcome=Outcome::Complete;return true;}
        const auto base=reinterpret_cast<std::uintptr_t>(image.nativeBase);
        if(directory.AddressOfCallBacks<base || directory.AddressOfCallBacks-base>SIZE_MAX) {
            outcome=Outcome::Unsupported;return false;
        }
        callbackArrayOffset=static_cast<SIZE_T>(directory.AddressOfCallBacks-base);
        if(!within(callbackArrayOffset,sizeof(ULONGLONG))) {
            outcome=Outcome::Unsupported;return false;
        }
        const SIZE_T cells=(image.imageSize-callbackArrayOffset)/sizeof(ULONGLONG);
        for(SIZE_T i=0;i<cells;++i) {
            ULONGLONG callback=0;
            memcpy(&callback,image.bytes.get()+callbackArrayOffset+i*sizeof(callback),sizeof(callback));
            callbackArrayExtent=(i+1)*sizeof(callback);
            if(!callback){outcome=Outcome::Complete;return true;}
            ++callbackCount;
            // Preserve external callback arrays/targets as unsupported source
            // geometry; no guess that every valid callback must be local.
            if(callback<base || callback-base>=image.imageSize) {
                outcome=Outcome::Unsupported;return false;
            }
        }
        return false; // No terminator in the actual retained image extent.
    }
};
}

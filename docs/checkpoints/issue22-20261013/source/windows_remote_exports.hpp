// UNCOMPILED export lookup over the actual retained remote image observation.
// This resolves no physical-to-loaded equality or trust/issuer relationship.
#pragma once
#include "windows_memory.hpp"
namespace template_creator {
inline const void* OriginalDbgBreakPoint(const LoadedImageObservation& image) noexcept {
    if (!image.complete || image.failed || image.used != image.imageSize ||
        image.imageSize < sizeof(IMAGE_DOS_HEADER)) return nullptr;
    const auto* bytes = image.bytes.get();
    auto span = [&](SIZE_T offset, SIZE_T count) noexcept {
        return offset <= image.imageSize && count <= image.imageSize-offset;
    };
    IMAGE_DOS_HEADER dos{}; memcpy(&dos, bytes, sizeof(dos));
    if (dos.e_magic != IMAGE_DOS_SIGNATURE || dos.e_lfanew < 0) return nullptr;
    const SIZE_T nt = static_cast<SIZE_T>(dos.e_lfanew);
    if (!span(nt, sizeof(DWORD)+sizeof(IMAGE_FILE_HEADER)+sizeof(WORD))) return nullptr;
    DWORD signature = 0; WORD magic = 0;
    memcpy(&signature, bytes+nt, sizeof(signature));
    memcpy(&magic, bytes+nt+sizeof(DWORD)+sizeof(IMAGE_FILE_HEADER), sizeof(magic));
    if (signature != IMAGE_NT_SIGNATURE) return nullptr;
    IMAGE_DATA_DIRECTORY exports{};
    if (magic == IMAGE_NT_OPTIONAL_HDR64_MAGIC) {
        if (!span(nt, sizeof(IMAGE_NT_HEADERS64))) return nullptr;
        IMAGE_NT_HEADERS64 header{}; memcpy(&header, bytes+nt, sizeof(header));
        if (header.OptionalHeader.NumberOfRvaAndSizes <= IMAGE_DIRECTORY_ENTRY_EXPORT) return nullptr;
        exports = header.OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT];
    } else if (magic == IMAGE_NT_OPTIONAL_HDR32_MAGIC) {
        if (!span(nt, sizeof(IMAGE_NT_HEADERS32))) return nullptr;
        IMAGE_NT_HEADERS32 header{}; memcpy(&header, bytes+nt, sizeof(header));
        if (header.OptionalHeader.NumberOfRvaAndSizes <= IMAGE_DIRECTORY_ENTRY_EXPORT) return nullptr;
        exports = header.OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT];
    } else return nullptr;
    if (!exports.VirtualAddress || exports.Size < sizeof(IMAGE_EXPORT_DIRECTORY) ||
        !span(exports.VirtualAddress, exports.Size)) return nullptr;
    IMAGE_EXPORT_DIRECTORY directory{}; memcpy(&directory, bytes+exports.VirtualAddress, sizeof(directory));
    if (directory.NumberOfNames > image.imageSize/sizeof(DWORD) ||
        directory.NumberOfFunctions > image.imageSize/sizeof(DWORD) ||
        !span(directory.AddressOfNames, static_cast<SIZE_T>(directory.NumberOfNames)*sizeof(DWORD)) ||
        !span(directory.AddressOfNameOrdinals, static_cast<SIZE_T>(directory.NumberOfNames)*sizeof(WORD)) ||
        !span(directory.AddressOfFunctions, static_cast<SIZE_T>(directory.NumberOfFunctions)*sizeof(DWORD))) return nullptr;
    constexpr char name[] = "DbgBreakPoint";
    for (DWORD i=0; i<directory.NumberOfNames; ++i) {
        DWORD rva = 0; memcpy(&rva, bytes+directory.AddressOfNames+static_cast<SIZE_T>(i)*sizeof(DWORD), sizeof(rva));
        if (!span(rva, sizeof(name)) || memcmp(bytes+rva, name, sizeof(name)) != 0) continue;
        WORD ordinal = 0;
        memcpy(&ordinal, bytes+directory.AddressOfNameOrdinals+static_cast<SIZE_T>(i)*sizeof(WORD), sizeof(ordinal));
        if (ordinal >= directory.NumberOfFunctions) return nullptr;
        DWORD function = 0;
        memcpy(&function, bytes+directory.AddressOfFunctions+static_cast<SIZE_T>(ordinal)*sizeof(DWORD), sizeof(function));
        if (!function || !span(function, 1) || (function >= exports.VirtualAddress &&
            static_cast<SIZE_T>(function)-exports.VirtualAddress < exports.Size)) return nullptr; // No forwarder guess.
        const auto base = reinterpret_cast<std::uintptr_t>(image.nativeBase);
        if (function > UINTPTR_MAX-base) return nullptr;
        return reinterpret_cast<const void*>(base+function);
    }
    return nullptr;
}
}
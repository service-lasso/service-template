// UNCOMPILED original loaded-image byte observer, deliberately not an equality
// assertion between a physical PE and relocated/loader-modified process memory.
#pragma once
#include <windows.h>
#include <array>
#include <cstdint>
#include <cstring>
#include <memory>
#include <stdexcept>
#include <exception>
#include "windows_pending_call_owner.hpp"
namespace template_creator {
struct OriginalImageMemoryCallInput {
    HANDLE process=nullptr;
    const void* address=nullptr;
    void* destination=nullptr;
    SIZE_T requested=0;
};
struct MemoryRegion {
    OriginalImageMemoryCallInput queryInput{},readInput{};
    template_creator_bridge::OriginalWindowsPendingCall* queryPending=nullptr;
    template_creator_bridge::OriginalWindowsPendingCall* readPending=nullptr;
    MEMORY_BASIC_INFORMATION original{};
    SIZE_T queryReturned = 0, readReturned = 0;
    BOOL readSucceeded = FALSE;
    DWORD queryError = 0, readError = 0;
    SIZE_T dataOffset = 0;
};
struct LoadedImageObservation {
    template_creator_bridge::OriginalWindowsPendingCalls* pendingOwnerOriginal=nullptr;
    const void* commandOriginal=nullptr;
    OriginalImageMemoryCallInput headerInput{};
    template_creator_bridge::OriginalWindowsPendingCall* headerPending=nullptr;
    HANDLE retainedProcess = nullptr; // Borrowed retained lifetime, no PID reopen.
    const void* nativeBase = nullptr;  // Original create/load debug-event base.
    FILETIME birth{};
    std::unique_ptr<unsigned char[]> bytes;
    std::unique_ptr<MemoryRegion[]> regions;
    SIZE_T byteCapacity = 0, regionCapacity = 0;
    SIZE_T used = 0, retainedByteExtent = 0, regionCount = 0, imageSize = 0;
    std::array<unsigned char, 4096> header{};
    SIZE_T headerReturned = 0;
    BOOL headerSucceeded = FALSE;
    DWORD headerError = 0;
    bool complete = false, failed = false;
    const char* failureOperation = nullptr;
    DWORD failureCode = 0;
    SIZE_T selectedImageBytesOriginal=0,selectedRegionRowsOriginal=0;
    bool preownAttempted=false,preowned=false;
    std::exception_ptr preownExceptionOriginal;
    LoadedImageObservation(SIZE_T selectedImageBytes, SIZE_T selectedRegionRows)
        :selectedImageBytesOriginal(selectedImageBytes),selectedRegionRowsOriginal(selectedRegionRows) {}
    bool PreownOriginal() noexcept {
        if(preownAttempted)return false;preownAttempted=true;
        try {
        const SIZE_T selectedImageBytes=selectedImageBytesOriginal,selectedRegionRows=selectedRegionRowsOriginal;
        // SizeOfImage is a DWORD in PE32/PE32+. No old arbitrary 64MiB cap:
        // original full Node/native tool geometry selects finite storage before
        // any process memory read. Actual whole-runtime fit remains separate.
        if (!selectedImageBytes || selectedImageBytes > MAXDWORD || !selectedRegionRows ||
                selectedRegionRows > selectedImageBytes ||
                selectedRegionRows > SIZE_MAX / sizeof(MemoryRegion))
            throw std::invalid_argument("selected original image observation capacity");
        if (selectedImageBytes > SIZE_MAX - sizeof(*this) ||
                selectedRegionRows > (SIZE_MAX - sizeof(*this) - selectedImageBytes) / sizeof(MemoryRegion))
            throw std::invalid_argument("selected image aggregate source size overflow");
        bytes = std::make_unique<unsigned char[]>(selectedImageBytes);
        byteCapacity = selectedImageBytes;
        regions = std::make_unique<MemoryRegion[]>(selectedRegionRows);
        regionCapacity = selectedRegionRows;preowned=true;return true;
        }catch(...){preownExceptionOriginal=std::current_exception();return false;}
    }
    LoadedImageObservation(const LoadedImageObservation&) = delete;
    LoadedImageObservation& operator=(const LoadedImageObservation&) = delete;
    LoadedImageObservation(LoadedImageObservation&&) = delete;
    LoadedImageObservation& operator=(LoadedImageObservation&&) = delete;
    SIZE_T ReservedSourceBytes() const noexcept {
        return sizeof(*this) + byteCapacity + regionCapacity * sizeof(MemoryRegion);
    }
    void Fail(const char* operation, DWORD code) noexcept {
        failed = true;
        if (!failureOperation) { failureOperation = operation; failureCode = code; }
    }
    bool Capture(HANDLE process, const void* base, FILETIME processBirth,
            template_creator_bridge::OriginalWindowsPendingCalls& pending,const void* command) noexcept {
        if (!preowned || retainedProcess || !process || !base || !command) return false;
        pendingOwnerOriginal=&pending;commandOriginal=command;
        retainedProcess = process; nativeBase = base; birth = processBirth;
        headerInput={process,base,header.data(),header.size()};
        headerPending=pending.BeginOriginal(this,&headerInput,headerInput.destination,headerInput.requested,
            commandOriginal,sizeof(headerInput));
        if(!headerPending){Fail("image_header_pending_capacity_or_freeze",ERROR_NOT_ENOUGH_MEMORY);return false;}
        headerSucceeded = ReadProcessMemory(process, base, header.data(), header.size(), &headerReturned);
        if(!headerSucceeded)headerError=GetLastError();
        if(!pending.ReturnedOriginal(*headerPending,headerSucceeded,headerError,headerReturned)) {
            Fail("image_header_completion_unknown",headerError);return false;
        }
        if (!headerSucceeded) { Fail("image_header_read", headerError); return false; }
        if (headerReturned != header.size()) { Fail("image_header_short", ERROR_PARTIAL_COPY); return false; }
        // memcpy avoids unaligned pointer reinterpretation. Only source-owned
        // bounded header bytes select the PE image extent, never path/hash labels.
        IMAGE_DOS_HEADER dos{};
        memcpy(&dos, header.data(), sizeof(dos));
        if (dos.e_magic != IMAGE_DOS_SIGNATURE || dos.e_lfanew < 0 ||
            static_cast<SIZE_T>(dos.e_lfanew) > header.size() - sizeof(IMAGE_NT_HEADERS64)) {
            Fail("image_dos_header", ERROR_BAD_EXE_FORMAT); return false;
        }
        DWORD signature = 0; WORD magic = 0;
        SIZE_T nt = static_cast<SIZE_T>(dos.e_lfanew);
        memcpy(&signature, header.data() + nt, sizeof(signature));
        memcpy(&magic, header.data() + nt + sizeof(DWORD) + sizeof(IMAGE_FILE_HEADER), sizeof(magic));
        if (signature != IMAGE_NT_SIGNATURE) { Fail("image_nt_signature", ERROR_BAD_EXE_FORMAT); return false; }
        if (magic == IMAGE_NT_OPTIONAL_HDR64_MAGIC) {
            IMAGE_NT_HEADERS64 h{}; memcpy(&h, header.data() + nt, sizeof(h)); imageSize = h.OptionalHeader.SizeOfImage;
        } else if (magic == IMAGE_NT_OPTIONAL_HDR32_MAGIC) {
            IMAGE_NT_HEADERS32 h{}; memcpy(&h, header.data() + nt, sizeof(h)); imageSize = h.OptionalHeader.SizeOfImage;
        } else { Fail("image_optional_magic", ERROR_BAD_EXE_FORMAT); return false; }
        if (!imageSize || imageSize > byteCapacity) { Fail("image_extent_bound", ERROR_NOT_ENOUGH_MEMORY); return false; }
        auto begin = reinterpret_cast<std::uintptr_t>(base);
        if (imageSize > UINTPTR_MAX - begin) { Fail("image_extent_overflow", ERROR_ARITHMETIC_OVERFLOW); return false; }
        SIZE_T offset = 0;
        while (offset < imageSize) {
            if (regionCount == regionCapacity) { Fail("image_region_capacity", ERROR_NOT_ENOUGH_MEMORY); return false; }
            // Original API result cells and destination bytes charged BEFORE read.
            MemoryRegion& region = regions[regionCount++];
            auto address = reinterpret_cast<const void*>(begin + offset);
            region.queryInput={process,address,&region.original,sizeof(region.original)};
            region.queryPending=pending.BeginOriginal(this,&region.queryInput,region.queryInput.destination,
                region.queryInput.requested,commandOriginal,sizeof(region.queryInput));
            if(!region.queryPending){Fail("image_query_pending_capacity_or_freeze",ERROR_NOT_ENOUGH_MEMORY);return false;}
            region.queryReturned = VirtualQueryEx(process, address, &region.original, sizeof(region.original));
            if(!region.queryReturned)region.queryError=GetLastError();
            // The actual SIZE_T return remains above; BOOL only classifies native
            // success for shared borrowing settlement, never replaces raw shape.
            if(!pending.ReturnedOriginal(*region.queryPending,region.queryReturned?TRUE:FALSE,
                    region.queryError,region.queryReturned)) {
                Fail("image_query_completion_unknown",region.queryError);return false;
            }
            if (region.queryReturned != sizeof(region.original)) {
                if (!region.queryReturned) {
                    Fail("image_region_query", region.queryError);
                } else Fail("image_region_query_short_shape", ERROR_INVALID_DATA);
                return false;
            }
            auto regionStart = reinterpret_cast<std::uintptr_t>(region.original.BaseAddress);
            if (regionStart > begin + offset || !region.original.RegionSize ||
                region.original.RegionSize > UINTPTR_MAX - regionStart ||
                regionStart + region.original.RegionSize <= begin + offset) {
                Fail("image_region_extent", ERROR_INVALID_DATA); return false;
            }
            SIZE_T count = regionStart + region.original.RegionSize - (begin + offset);
            if (count > imageSize - offset) count = imageSize - offset;
            if (region.original.State != MEM_COMMIT || region.original.AllocationBase != base ||
                region.original.Type != MEM_IMAGE || region.original.Protect & (PAGE_NOACCESS | PAGE_GUARD)) {
                // No invented zero bytes for inaccessible/decommitted PE pages.
                Fail("image_region_unreadable", ERROR_NOACCESS); return false;
            }
            if (count > byteCapacity - used) { Fail("image_byte_capacity", ERROR_NOT_ENOUGH_MEMORY); return false; }
            region.dataOffset = used;
            region.readInput={process,address,bytes.get()+used,count};
            region.readPending=pending.BeginOriginal(this,&region.readInput,region.readInput.destination,
                region.readInput.requested,commandOriginal,sizeof(region.readInput));
            if(!region.readPending){Fail("image_read_pending_capacity_or_freeze",ERROR_NOT_ENOUGH_MEMORY);return false;}
            region.readSucceeded = ReadProcessMemory(process, address, bytes.get() + used, count, &region.readReturned);
            if (!region.readSucceeded) region.readError = GetLastError();
            if(!pending.ReturnedOriginal(*region.readPending,region.readSucceeded,region.readError,region.readReturned)) {
                Fail("image_region_completion_unknown",region.readError);return false;
            }
            // Retain the affected prefix independently of full-success parsing.
            // The preowned destination survives even when the native BOOL fails.
            if (region.readReturned <= count) retainedByteExtent = used + region.readReturned;
            else { Fail("image_read_extent_exceeds_request", ERROR_INVALID_DATA); return false; }
            if (!region.readSucceeded) {
                Fail("image_region_read", region.readError); return false;
            }
            if (region.readReturned != count) { Fail("image_region_short", ERROR_PARTIAL_COPY); return false; }
            used += count; offset += count;
        }
        complete = true;
        return true;
    }
};
// Capture is evidence of actual read calls at a stopped native event. It does
// not prove loader timing, immutable mappings, compiler trust, whole-runtime
// memory, or absence of another thread/process capable of changing these bytes.
} // namespace template_creator

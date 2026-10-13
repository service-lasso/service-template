// UNCOMPILED native held-file byte/identity verifier. This is a concrete gate
// predicate, not complete input selection, ROOT authority or native tool trust.
#pragma once
#include <windows.h>
#include <bcrypt.h>
#include <array>
#include <cstdint>
#include <cstring>
#include <memory>
#include <stdexcept>
#include "windows_identity.hpp"
#pragma comment(lib, "bcrypt.lib")
namespace template_creator {
struct NativeRead {
    SIZE_T fileOrdinal = SIZE_MAX;
    HANDLE original = nullptr;
    DWORD requested = 0, returned = 0;
    BOOL result = FALSE;
    DWORD originalError = 0;
    bool growthProbe = false;
    SIZE_T destinationOffset = 0;
    NTSTATUS hashResult = 0;
};
struct HeldNativeFile {
    SIZE_T readBegin = 0, readEnd = 0, bodyBegin = 0, bodyExtent = 0;
    uint64_t sourceExpectedSize = 0;
    std::array<wchar_t, 32768> originalPath{};
    SIZE_T originalPathChars = 0;
    std::array<unsigned char, 32> sourceExpectedSha256{};
    HANDLE original = nullptr, openResult = nullptr;
    bool openAttempted = false, closeAttempted = false, closed = false;
    DWORD openError = 0, closeError = 0;
    BOOL closeResult = FALSE;
    FileIdentityObservation first, after;
    LARGE_INTEGER actualSize{};
    BOOL sizeResult = FALSE;
    DWORD sizeError = 0;
    std::array<unsigned char, 32> actualSha256{};
    bool bytesCaptured = false, valid = false;
    const unsigned char* fixedExpectedBodyOriginal=nullptr;
    bool fixedExpectedBodyCompared=false,fixedExpectedBodyEqual=false;
    bool readCompletionUnknown = false;
    BCRYPT_ALG_HANDLE algorithm = nullptr;
    BCRYPT_HASH_HANDLE hash = nullptr;
    bool algorithmAcquired = false, hashAcquired = false;
    bool algorithmOutputUnknown = false, hashOutputUnknown = false;
    NTSTATUS algorithmResult = 0, createHashResult = 0, hashResult = 0, finishResult = 0;
    NTSTATUS hashCloseResult = 0, algorithmCloseResult = 0;
    bool hashCloseAttempted = false, algorithmCloseAttempted = false;
};
class NativeHeldFiles {
public:
    static constexpr SIZE_T FileLimit = 1024, ReadLimit = 65536;
private:
    std::unique_ptr<HeldNativeFile[]> files_;
    std::unique_ptr<NativeRead[]> reads_;
    SIZE_T fileCapacity_ = 0, readCapacity_ = 0;
    static constexpr SIZE_T ReadChunk = 65536;
    std::unique_ptr<unsigned char[]> retainedBytes_;
    SIZE_T byteCapacity_ = 0, retainedByteCount_ = 0;
    SIZE_T fileCount_ = 0, readCount_ = 0;
    bool failed_ = false;
    const char* firstShapeFailure_ = nullptr;
    void ShapeFailure(const char* operation) noexcept {
        failed_ = true;
        if (!firstShapeFailure_) firstShapeFailure_ = operation;
    }
    bool Read(HeldNativeFile& file, DWORD requested, bool probe) noexcept {
        if (failed_ || file.readCompletionUnknown) return false;
        if (readCount_ == readCapacity_) { ShapeFailure("read_result_capacity_before_call"); return false; }
        if (requested > byteCapacity_ - retainedByteCount_) {
            ShapeFailure("native_read_bytes_capacity_before_call"); return false;
        }
        NativeRead& row = reads_[readCount_++];
        row.fileOrdinal = static_cast<SIZE_T>(&file-files_.get());
        file.readEnd = readCount_;
        row.original = file.original; row.requested = requested; row.growthProbe = probe;
        row.destinationOffset = retainedByteCount_;
        row.result = ReadFile(file.original, retainedBytes_.get() + row.destinationOffset, requested, &row.returned, nullptr);
        if (!row.result) row.originalError = GetLastError();
        if (!row.result && row.originalError==ERROR_IO_PENDING) {
            file.readCompletionUnknown=true; failed_=true; return false;
        }
        // Actual partial bytes survive before failed-BOOL or shape interpretation.
        if (row.returned <= requested) {
            retainedByteCount_ += row.returned;
            if (!probe) file.bodyExtent += row.returned;
        }
        else { file.readCompletionUnknown = true; ShapeFailure("native_read_reported_extent_exceeds_request"); return false; }
        if (!row.result) { file.readCompletionUnknown = row.originalError==ERROR_IO_PENDING; failed_ = true; return false; }
        if (row.returned > requested || (probe && row.returned) || (!probe && !row.returned)) {
            ShapeFailure(probe ? "original_file_grew" : "original_file_short"); return false;
        }
        if (!probe) {
            file.hashResult = BCryptHashData(file.hash, retainedBytes_.get() + row.destinationOffset, row.returned, 0);
            row.hashResult = file.hashResult;
            if (file.hashResult < 0) { failed_ = true; return false; }
        }
        return true;
    }
    static bool SameObject(const FileIdentityObservation& a, const FileIdentityObservation& b) noexcept {
        return a.captured && b.captured && a.id.VolumeSerialNumber == b.id.VolumeSerialNumber &&
               memcmp(a.id.FileId.Identifier, b.id.FileId.Identifier, sizeof(a.id.FileId.Identifier)) == 0 &&
               a.basic.CreationTime.QuadPart == b.basic.CreationTime.QuadPart &&
               a.basic.LastWriteTime.QuadPart == b.basic.LastWriteTime.QuadPart &&
               a.basic.ChangeTime.QuadPart == b.basic.ChangeTime.QuadPart &&
               a.basic.FileAttributes == b.basic.FileAttributes &&
               a.standard.EndOfFile.QuadPart == b.standard.EndOfFile.QuadPart &&
               a.standard.NumberOfLinks == b.standard.NumberOfLinks &&
               a.standard.DeletePending == b.standard.DeletePending &&
               a.standard.Directory == b.standard.Directory &&
               a.pathResult == b.pathResult && memcmp(a.finalPath.data(), b.finalPath.data(),
                    static_cast<SIZE_T>(a.pathResult) * sizeof(wchar_t)) == 0 &&
               a.securityRequired == b.securityRequired && a.securityRequired <= a.security.size() &&
               memcmp(a.security.data(), b.security.data(), a.securityRequired) == 0;
    }
public:
    NativeHeldFiles(const NativeHeldFiles&) = delete;
    NativeHeldFiles& operator=(const NativeHeldFiles&) = delete;
    NativeHeldFiles(NativeHeldFiles&&) = delete;
    NativeHeldFiles& operator=(NativeHeldFiles&&) = delete;
    // Exact selected capacities, derived from the original input selection and
    // actual read policy, must be supplied before any native capability exists.
    // The maxima bound source shape; they are not a whole-runtime budget.
    static SIZE_T CheckedSourceBytes(SIZE_T selectedFiles, SIZE_T selectedReadRows, SIZE_T selectedByteCapacity) {
        if (!selectedFiles || selectedFiles > FileLimit || !selectedReadRows ||
                selectedReadRows > ReadLimit || !selectedByteCapacity || selectedByteCapacity > 1024ull * 1024 * 1024)
            throw std::invalid_argument("selected native input capacities");
        SIZE_T bytes=sizeof(NativeHeldFiles);
        const auto add=[&](SIZE_T count,SIZE_T width) {
            if(count>(SIZE_MAX-bytes)/width)throw std::invalid_argument("held input aggregate source geometry");
            bytes+=count*width;
        };
        add(selectedFiles,sizeof(HeldNativeFile));add(selectedReadRows,sizeof(NativeRead));add(selectedByteCapacity,1);
        return bytes;
    }
    NativeHeldFiles(SIZE_T selectedFiles, SIZE_T selectedReadRows, SIZE_T selectedByteCapacity) {
        (void)CheckedSourceBytes(selectedFiles,selectedReadRows,selectedByteCapacity);
        files_ = std::make_unique<HeldNativeFile[]>(selectedFiles);
        reads_ = std::make_unique<NativeRead[]>(selectedReadRows);
        retainedBytes_ = std::make_unique<unsigned char[]>(selectedByteCapacity);
        fileCapacity_ = selectedFiles; readCapacity_ = selectedReadRows;
        byteCapacity_ = selectedByteCapacity;
    }
    SIZE_T ReservedSourceBytes() const noexcept {
        return sizeof(*this) + fileCapacity_ * sizeof(HeldNativeFile) +
               readCapacity_ * sizeof(NativeRead) + byteCapacity_;
    }
    SIZE_T FileCapacity() const noexcept { return fileCapacity_; }
    SIZE_T ReadCapacity() const noexcept { return readCapacity_; }
    const HeldNativeFile* PrivateFiles() const noexcept { return files_.get(); }
    SIZE_T FileCount() const noexcept { return fileCount_; }
    const NativeRead* PrivateReads() const noexcept { return reads_.get(); }
    SIZE_T ReadCount() const noexcept { return readCount_; }
    const unsigned char* PrivateReadBytes() const noexcept { return retainedBytes_.get(); }
    SIZE_T RetainedReadByteCount() const noexcept { return retainedByteCount_; }
    SIZE_T ReadByteCapacity() const noexcept { return byteCapacity_; }
    bool ExactCapturedSourceSpan(SIZE_T ordinal,const unsigned char*& bytes,SIZE_T& size) const noexcept {
        bytes=nullptr;size=0;
        if(ordinal>=fileCount_)return false;
        const auto& file=files_[ordinal];
        if(!file.valid || !file.bytesCaptured || file.readCompletionUnknown ||
            file.bodyBegin>retainedByteCount_ || file.bodyExtent>retainedByteCount_-file.bodyBegin ||
            file.bodyExtent!=file.sourceExpectedSize)return false;
        bytes=retainedBytes_.get()+file.bodyBegin;size=file.bodyExtent;
        return true; // Same owner backing; no reopening, copy or authority grant.
    }
    bool Failed() const noexcept { return failed_; }
    const char* FirstShapeFailure() const noexcept { return firstShapeFailure_; }
    SIZE_T Capture(const wchar_t* literalPath, uint64_t expectedSize,
                   const std::array<unsigned char, 32>& sourceExpectedSha256) noexcept {
        return CaptureOriginal(literalPath,expectedSize,sourceExpectedSha256,nullptr);
    }
    static const char* FixedNodeStubBody() noexcept {
        return "@echo off\r\necho %~1>>\"%TEMPLATE_NATIVE_CALLS%\"\r\nif \"%~1\"==\"--test\" exit /b %TEMPLATE_NATIVE_FIRST%\r\nif \"%~1\"==\"validate-contract\" exit /b %TEMPLATE_NATIVE_FIRST%\r\nexit /b %TEMPLATE_NATIVE_LAST%\r\n";
    }
    static SIZE_T FixedNodeStubExtent() noexcept{return strlen(FixedNodeStubBody());}
    SIZE_T CaptureFixedNodeStub(const wchar_t* literalPath) noexcept {
        const std::array<unsigned char,32> unusedDigest{};
        return CaptureOriginal(literalPath,FixedNodeStubExtent(),unusedDigest,
            reinterpret_cast<const unsigned char*>(FixedNodeStubBody()));
    }
private:
    SIZE_T CaptureOriginal(const wchar_t* literalPath,uint64_t expectedSize,
        const std::array<unsigned char,32>& sourceExpectedSha256,const unsigned char* fixedExpectedBody) noexcept {
        if (failed_ || fileCount_ == fileCapacity_ || expectedSize > 1024ull * 1024 * 1024) {
            ShapeFailure("file_source_capacity"); return FileLimit;
        }
        uint64_t minimumRows = (expectedSize + ReadChunk - 1) / ReadChunk + 1;
        if (expectedSize >= byteCapacity_ - retainedByteCount_) {
            ShapeFailure("original_file_and_growth_probe_byte_capacity"); return FileLimit;
        }
        if (minimumRows > readCapacity_ - readCount_) {
            ShapeFailure("minimum_read_rows_before_acquisition"); return FileLimit;
        }
        // This necessary lower bound does not assume native reads fill buffers;
        // partial reads still consume actual rows and can fail capacity honestly.
        // Preowned cell exists before acquisition; no vector/string allocations.
        SIZE_T index = fileCount_++;
        HeldNativeFile& file = files_[index];
        file.readBegin=file.readEnd=readCount_;file.bodyBegin=retainedByteCount_;
        file.sourceExpectedSize=expectedSize;
        file.fixedExpectedBodyOriginal=fixedExpectedBody;
        if (!literalPath) { ShapeFailure("literal_original_path_missing"); return index; }
        for (SIZE_T i=0; i<file.originalPath.size(); ++i) {
            file.originalPath[i] = literalPath[i];
            if (!literalPath[i]) { file.originalPathChars = i + 1; break; }
        }
        if (!file.originalPathChars) { ShapeFailure("literal_original_path_capacity"); return index; }
        file.sourceExpectedSha256 = sourceExpectedSha256;
        file.openAttempted = true;
        file.openResult = CreateFileW(file.originalPath.data(), GENERIC_READ | READ_CONTROL, FILE_SHARE_READ,
            nullptr, OPEN_EXISTING, FILE_FLAG_OPEN_REPARSE_POINT | FILE_FLAG_SEQUENTIAL_SCAN, nullptr);
        file.original = file.openResult;
        if (file.openResult == INVALID_HANDLE_VALUE) {
            file.openError = GetLastError(); file.original = nullptr; failed_ = true; return index;
        }
        file.first.Capture(file.original);
        if (!file.first.captured || file.first.basic.FileAttributes & FILE_ATTRIBUTE_REPARSE_POINT ||
                file.first.standard.Directory || file.first.standard.NumberOfLinks != 1) {
            ShapeFailure("native_regular_unique_nonreparse_file"); return index;
        }
        file.sizeResult = GetFileSizeEx(file.original, &file.actualSize);
        if (!file.sizeResult) { file.sizeError = GetLastError(); failed_ = true; return index; }
        if (file.actualSize.QuadPart < 0 || static_cast<uint64_t>(file.actualSize.QuadPart) != expectedSize) {
            ShapeFailure("native_exact_source_size"); return index;
        }
        file.algorithmResult = BCryptOpenAlgorithmProvider(&file.algorithm, BCRYPT_SHA256_ALGORITHM, nullptr, 0);
        if (file.algorithmResult < 0) {
            file.algorithmOutputUnknown = file.algorithm != nullptr; failed_ = true; return index;
        }
        file.algorithmAcquired = file.algorithm != nullptr;
        if (!file.algorithmAcquired) { ShapeFailure("successful_algorithm_missing_handle"); return index; }
        file.createHashResult = BCryptCreateHash(file.algorithm, &file.hash, nullptr, 0, nullptr, 0, 0);
        if (file.createHashResult < 0) {
            file.hashOutputUnknown = file.hash != nullptr; failed_ = true; return index;
        }
        file.hashAcquired = file.hash != nullptr;
        if (!file.hashAcquired) { ShapeFailure("successful_hash_missing_handle"); return index; }
        uint64_t remaining = expectedSize;
        while (remaining) {
            DWORD requested = remaining > ReadChunk ? static_cast<DWORD>(ReadChunk) : static_cast<DWORD>(remaining);
            SIZE_T row = readCount_;
            if (!Read(file, requested, false)) return index;
            remaining -= reads_[row].returned;
        }
        if (!Read(file, 1, true)) return index;
        file.finishResult = BCryptFinishHash(file.hash, file.actualSha256.data(),
                                             static_cast<ULONG>(file.actualSha256.size()), 0);
        if (file.finishResult < 0) { failed_ = true; return index; }
        file.bytesCaptured = true;
        file.after.Capture(file.original);
        bool content=file.actualSha256==file.sourceExpectedSha256;
        if(fixedExpectedBody) {
            file.fixedExpectedBodyCompared=true;
            file.fixedExpectedBodyEqual=file.bodyExtent==expectedSize &&
                (!expectedSize || !memcmp(retainedBytes_.get()+file.bodyBegin,fixedExpectedBody,static_cast<SIZE_T>(expectedSize)));
            content=file.fixedExpectedBodyEqual;
        }
        file.valid = SameObject(file.first, file.after) && content;
        if (!file.valid) ShapeFailure("native_original_source_changed_or_different");
        return index;
    }
public:
    // Explicit disposal visits ALL independently eligible original resources.
    // A failed close remains uncertain and never gets repeated on another call.
    bool Dispose() noexcept {
        bool all = true;
        for (SIZE_T i=0; i<fileCount_; ++i) {
            auto& file = files_[i];
            if (file.hashAcquired && !file.hashCloseAttempted) {
                file.hashCloseAttempted = true; file.hashCloseResult = BCryptDestroyHash(file.hash);
            }
            if (file.algorithmAcquired && !file.algorithmCloseAttempted) {
                file.algorithmCloseAttempted = true; file.algorithmCloseResult = BCryptCloseAlgorithmProvider(file.algorithm, 0);
            }
            if (file.original && !file.closeAttempted && !file.readCompletionUnknown) {
                file.closeAttempted = true;
                file.closeResult = CloseHandle(file.original);
                if (file.closeResult) file.closed = true;
                else file.closeError = GetLastError();
            }
            all = all && !file.readCompletionUnknown && !file.algorithmOutputUnknown && !file.hashOutputUnknown && (!file.original || file.closed) &&
                  (!file.hashAcquired || (file.hashCloseAttempted && file.hashCloseResult >= 0)) &&
                  (!file.algorithmAcquired || (file.algorithmCloseAttempted && file.algorithmCloseResult >= 0));
        }
        if (!all) failed_ = true;
        return all;
    }
    bool PhysicallyDisposed() const noexcept {
        for (SIZE_T i=0;i<fileCount_;++i) {
            const auto& file = files_[i];
            if (file.readCompletionUnknown || file.algorithmOutputUnknown || file.hashOutputUnknown || (file.original && !file.closed) ||
                (file.hashAcquired && (!file.hashCloseAttempted || file.hashCloseResult<0)) ||
                (file.algorithmAcquired && (!file.algorithmCloseAttempted || file.algorithmCloseResult<0))) return false;
        }
        return true;
    }
};
// Hash equality is one physical byte predicate only. Ancestor chains, named-vs-
// held continuity, immutable actual selection, original owner/ACL/mapping rights,
// interpreter/generated input policy and native loaded images remain separate.
} // namespace template_creator

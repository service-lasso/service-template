// UNCOMPILED native Windows sink used by the retained session. Borrowed sink
// handle belongs to original ingress; no path reopen or automatic close occurs.
#pragma once
#include <windows.h>
#include <memory>
#include <cstring>
#include <stdexcept>
#include <cstdint>
#include <initializer_list>
namespace template_creator {
enum class SinkCall { Identity, Size, Seek, Write, Flush, Read };
struct SinkOriginalCall {
    SinkCall operation{};
    bool attempted = false;
    BOOL result = FALSE;
    DWORD error = 0, requested = 0, actual = 0;
    SIZE_T sourceOffset = 0, observedOffset = 0;
    LARGE_INTEGER requestedPosition{}, returnedPosition{}, size{};
    FILE_ID_INFO identity{};
    bool matched = false;
};
class OriginalWindowsSink {
    HANDLE borrowedOriginal_ = nullptr;
    std::unique_ptr<unsigned char[]> originals_, readbacks_;
    std::unique_ptr<SinkOriginalCall[]> calls_;
    SIZE_T originalCapacity_ = 0, readbackCapacity_ = 0, callCapacity_ = 0;
    SIZE_T copied_ = 0, written_ = 0, readbackUsed_ = 0, callCount_ = 0;
    bool adoptAttempted_ = false, adopted_ = false, failed_ = false, verified_ = false;
    bool prefixReadbackVerified_ = false, writeCompletionUnknown_ = false, readCompletionUnknown_ = false;
    bool originalsSealed_ = false;
    const char* firstShapeFailure_ = nullptr;
    bool Fail(const char* operation) noexcept {
        failed_ = true; if (!firstShapeFailure_) firstShapeFailure_ = operation; return false;
    }
    SinkOriginalCall* Call(SinkCall operation) noexcept {
        if (callCount_ == callCapacity_) { Fail("native_sink_call_capacity_before_call"); return nullptr; }
        auto& row = calls_[callCount_++]; row.operation = operation; row.attempted = true;
        return &row;
    }
    bool Seek(LONGLONG offset) noexcept {
        auto* row = Call(SinkCall::Seek); if (!row) return false;
        row->requestedPosition.QuadPart = offset;
        row->result = SetFilePointerEx(borrowedOriginal_, row->requestedPosition, &row->returnedPosition, FILE_BEGIN);
        if (!row->result) row->error = GetLastError();
        if (!row->result) { failed_ = true; return false; }
        if (row->returnedPosition.QuadPart != offset) return Fail("native_sink_actual_seek_position");
        return true;
    }
public:
    OriginalWindowsSink(SIZE_T selectedOriginalBytes, SIZE_T selectedReadbackBytes, SIZE_T selectedCallRows) {
        if (!selectedOriginalBytes || !selectedReadbackBytes || !selectedCallRows ||
            selectedOriginalBytes > MAXLONGLONG || selectedCallRows > SIZE_MAX/sizeof(SinkOriginalCall))
            throw std::invalid_argument("complete original native sink capacities");
        SIZE_T requestedSourceBytes=sizeof(OriginalWindowsSink);
        for (const SIZE_T bytes : {selectedOriginalBytes,selectedReadbackBytes,
                selectedCallRows*sizeof(SinkOriginalCall)}) {
            if (bytes>SIZE_MAX-requestedSourceBytes)
                throw std::invalid_argument("complete original native sink source graph");
            requestedSourceBytes+=bytes;
        }
        originals_ = std::make_unique<unsigned char[]>(selectedOriginalBytes);
        readbacks_ = std::make_unique<unsigned char[]>(selectedReadbackBytes);
        calls_ = std::make_unique<SinkOriginalCall[]>(selectedCallRows);
        originalCapacity_ = selectedOriginalBytes; readbackCapacity_ = selectedReadbackBytes; callCapacity_ = selectedCallRows;
    }
    OriginalWindowsSink(const OriginalWindowsSink&) = delete;
    OriginalWindowsSink& operator=(const OriginalWindowsSink&) = delete;
    bool AdoptBorrowedOriginal(HANDLE original) noexcept {
        if (adoptAttempted_) return Fail("native_sink_original_adoption_once");
        adoptAttempted_ = true; borrowedOriginal_ = original; // Attach before queries.
        if (!original || original == INVALID_HANDLE_VALUE) return Fail("native_sink_original_missing");
        auto* identity = Call(SinkCall::Identity); if (!identity) return false;
        identity->result = GetFileInformationByHandleEx(original, FileIdInfo, &identity->identity, sizeof(identity->identity));
        if (!identity->result) identity->error = GetLastError();
        auto* size = Call(SinkCall::Size); if (!size) return false;
        size->result = GetFileSizeEx(original, &size->size);
        if (!size->result) size->error = GetLastError();
        if (!identity->result || !size->result) { failed_ = true; return false; }
        if (size->size.QuadPart != 0) return Fail("native_sink_must_be_original_empty_file");
        adopted_ = Seek(0); return adopted_;
    }
    bool AppendOriginalBytes(const void* originalBytes, SIZE_T bytes) noexcept {
        if (originalsSealed_) return Fail("native_sink_original_copies_sealed");
        if (failed_ || !adopted_) return false;
        verified_ = false; prefixReadbackVerified_ = false;
        if ((!originalBytes && bytes) || bytes > originalCapacity_-copied_) return Fail("native_sink_original_copy_capacity_before_write");
        const SIZE_T begin = copied_;
        if (bytes) memcpy(originals_.get()+begin, originalBytes, bytes);
        copied_ += bytes; // The exact immutable input copy precedes native WriteFile.
        SIZE_T offset = begin;
        while (offset < copied_) {
            auto* row = Call(SinkCall::Write); if (!row) return false;
            row->sourceOffset = offset;
            const SIZE_T remaining = copied_-offset;
            row->requested = remaining > 65536 ? 65536 : static_cast<DWORD>(remaining);
            row->result = WriteFile(borrowedOriginal_, originals_.get()+offset, row->requested, &row->actual, nullptr);
            if (!row->result) {
                row->error = GetLastError();
                if (row->error == ERROR_IO_PENDING) {
                    writeCompletionUnknown_ = failed_ = true; return false;
                }
            }
            if (row->actual > row->requested) { writeCompletionUnknown_ = true; return Fail("native_sink_write_actual_extent_shape"); }
            written_ += row->actual; offset += row->actual;
            if (!row->result) { failed_ = true; return false; }
            if (!row->actual) return Fail("native_sink_zero_progress_write");
        }
        return true;
    }
    bool FlushReadbackOriginal() noexcept {
        // An absorbing qualification failure does not erase independently
        // eligible synchronous prefix readback. Unknown pending IO is retained.
        if (!adopted_ || writeCompletionUnknown_ || readCompletionUnknown_) return false;
        verified_ = false; prefixReadbackVerified_ = false;
        auto* flush = Call(SinkCall::Flush); if (!flush) return false;
        flush->result = FlushFileBuffers(borrowedOriginal_);
        if (!flush->result) { flush->error = GetLastError(); failed_ = true; return false; }
        if (!Seek(0)) return false;
        SIZE_T offset = 0;
        while (offset < written_) {
            const SIZE_T remaining = written_-offset;
            const DWORD request = remaining > 65536 ? 65536 : static_cast<DWORD>(remaining);
            if (request > readbackCapacity_-readbackUsed_) return Fail("native_sink_readback_byte_capacity_before_call");
            auto* row = Call(SinkCall::Read); if (!row) return false;
            row->sourceOffset = offset; row->observedOffset = readbackUsed_; row->requested = request;
            row->result = ReadFile(borrowedOriginal_, readbacks_.get()+readbackUsed_, request, &row->actual, nullptr);
            if (!row->result) {
                row->error = GetLastError();
                if (row->error == ERROR_IO_PENDING) {
                    readCompletionUnknown_ = failed_ = true; return false;
                }
            }
            if (row->actual > row->requested) { readCompletionUnknown_ = true; return Fail("native_sink_readback_actual_extent_shape"); }
            readbackUsed_ += row->actual; // Partial failed output survives before interpretation.
            if (!row->result) { failed_ = true; return false; }
            if (!row->actual) return Fail("native_sink_readback_short");
            row->matched = memcmp(originals_.get()+offset, readbacks_.get()+row->observedOffset, row->actual) == 0;
            if (!row->matched) return Fail("native_sink_readback_original_bytes_differ");
            offset += row->actual;
        }
        if (readbackUsed_ == readbackCapacity_) return Fail("native_sink_EOF_byte_capacity_before_call");
        auto* eof = Call(SinkCall::Read); if (!eof) return false;
        eof->sourceOffset = written_; eof->observedOffset = readbackUsed_; eof->requested = 1;
        eof->result = ReadFile(borrowedOriginal_, readbacks_.get()+readbackUsed_, 1, &eof->actual, nullptr);
        if (!eof->result) {
            eof->error = GetLastError();
            if (eof->error == ERROR_IO_PENDING) {
                readCompletionUnknown_ = failed_ = true; return false;
            }
        }
        if (eof->actual > 1) { readCompletionUnknown_ = true; return Fail("native_sink_EOF_actual_extent_shape"); }
        readbackUsed_ += eof->actual;
        if (!eof->result) { failed_ = true; return false; }
        if (eof->actual) return Fail("native_sink_not_exact_original_EOF");
        eof->matched = true;
        if (!Seek(static_cast<LONGLONG>(written_))) return false;
        prefixReadbackVerified_ = true;
        verified_ = !failed_ && written_ == copied_;
        return prefixReadbackVerified_; // Physical prefix result, never qualification.
    }
    bool ReservedSourceBytes(SIZE_T& bytes) const noexcept {
        bytes = sizeof(*this);
        for (SIZE_T amount : {originalCapacity_, readbackCapacity_, callCapacity_*sizeof(SinkOriginalCall)}) {
            if (amount > SIZE_MAX-bytes) return false; bytes += amount;
        }
        return true; // Source arithmetic, not native/runtime measured fit.
    }
    bool Failed() const noexcept { return failed_; }
    SIZE_T OriginalByteCapacity() const noexcept { return originalCapacity_; }
    bool Verified() const noexcept { return verified_ && !failed_; }
    bool PrefixReadbackVerified() const noexcept { return prefixReadbackVerified_; }
    bool SealOriginalCopies() noexcept {
        if (!adopted_ || writeCompletionUnknown_ || readCompletionUnknown_) return false;
        originalsSealed_ = true; return true;
    }
    bool CompletionKnown() const noexcept { return !writeCompletionUnknown_ && !readCompletionUnknown_; }
    bool WriteCompletionUnknown() const noexcept { return writeCompletionUnknown_; }
    bool ReadCompletionUnknown() const noexcept { return readCompletionUnknown_; }
    HANDLE BorrowedOriginal() const noexcept { return borrowedOriginal_; }
    const SinkOriginalCall* PrivateCalls() const noexcept { return calls_.get(); }
    SIZE_T CallCount() const noexcept { return callCount_; }
    const unsigned char* PrivateOriginalBytes() const noexcept { return originals_.get(); }
    SIZE_T CopiedOriginalBytes() const noexcept { return copied_; }
    SIZE_T WrittenOriginalBytes() const noexcept { return written_; }
    const unsigned char* PrivateReadbackBytes() const noexcept { return readbacks_.get(); }
    SIZE_T ReadbackByteCount() const noexcept { return readbackUsed_; }
    const char* FirstShapeFailure() const noexcept { return firstShapeFailure_; }
};
// Sink handle rights/identity/private destination association and eligible close
// belong to the connected ingress/session owner. Exact bytes/readback never
// prove those missing native relationships or serialize live handles as grants.
}

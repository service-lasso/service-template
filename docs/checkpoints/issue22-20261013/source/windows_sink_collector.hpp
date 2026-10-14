// UNCOMPILED independent native readback. No Python access, source/tool grant,
// path reopening or borrowed-handle closure. The original host thread stays alive.
#pragma once
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include "windows_native_sink.hpp"
#include <atomic>
namespace template_creator_bridge { struct NativePlatformState; struct OriginalWindowsStandaloneHost; }
namespace template_creator {
struct CollectorReadOriginal {
    BOOL returned = FALSE; DWORD error = 0, requested = 0, actual = 0;
    SIZE_T offset = 0; bool attempted = false;
};
struct CollectorThreadObservation {
    DWORD requestedWaitMilliseconds = 0;
    DWORD waitReturned = WAIT_FAILED, waitError = 0;
    BOOL timesReturned = FALSE, codeReturned = FALSE;
    DWORD timesError = 0, codeError = 0, code = STILL_ACTIVE;
    FILETIME born{}, exited{}, kernel{}, user{};
};
class OriginalWindowsSinkCollector {
    OriginalWindowsSink& sink_;
    std::unique_ptr<unsigned char[]> bytes_;
    std::unique_ptr<CollectorReadOriginal[]> reads_;
    std::unique_ptr<CollectorThreadObservation[]> observations_;
    SIZE_T byteCapacity_ = 0, readCapacity_ = 0, observationCapacity_ = 0;
    DWORD selectedWaitMilliseconds_ = 0;
    SIZE_T readCount_ = 0, observationCount_ = 0, retained_ = 0, expected_ = 0;
    HANDLE reader_ = INVALID_HANDLE_VALUE, thread_ = nullptr;
    DWORD reopenError_ = 0, creationError_ = 0, createdThreadId_ = 0;
    bool startAttempted_ = false, readerAcquired_ = false, threadAcquired_ = false;
    bool readerCloseAttempted_ = false, readerClosed_ = false, threadCloseAttempted_ = false, threadClosed_ = false;
    BOOL readerCloseReturned_ = FALSE, threadCloseReturned_ = FALSE;
    DWORD readerCloseError_ = 0, threadCloseError_ = 0;
    BOOL identityReturned_ = FALSE;
    DWORD identityError_ = 0;
    FILE_ID_INFO identity_{};
    BOOL seekReturned_ = FALSE;
    DWORD seekError_ = 0;
    LARGE_INTEGER seekReturnedPosition_{};
    bool seekAttempted_ = false;
    bool matched_ = false, eof_ = false, failed_ = false, threadTerminal_ = false;
    bool readCompletionUnknown_ = false;
    bool observationFailed_ = false, observationExhausted_ = false;
    bool readerOutcomeUnknown_ = false;
    bool creationFailed_ = false; // Creating-thread only; worker owns failed_ after birth.
    std::atomic<bool> threadOutcomeUnknown_{false};
    const unsigned char* expectedBytes_ = nullptr;
    FILE_ID_INFO expectedIdentity_{};
    std::atomic<bool> startPublished_{false};
    std::atomic<bool> bodyReturned_{false};
    static DWORD WINAPI ThreadMain(void* parameter) noexcept {
        auto& self = *static_cast<OriginalWindowsSinkCollector*>(parameter);
        if (!self.startPublished_.load(std::memory_order_acquire)) {
            self.failed_ = true; self.CloseReaderOnce();
            self.bodyReturned_.store(true,std::memory_order_release); return 1;
        }
        self.ReadOriginal();
        self.bodyReturned_.store(true,std::memory_order_release);
        return self.matched_ && self.eof_ && !self.failed_ ? 0 : 1;
    }
    void CloseReaderOnce() noexcept {
        if (!readerAcquired_ || readerCloseAttempted_ || readCompletionUnknown_ ||
            readerOutcomeUnknown_ || threadOutcomeUnknown_.load(std::memory_order_acquire)) return;
        readerCloseAttempted_ = true; readerCloseReturned_ = CloseHandle(reader_);
        if (readerCloseReturned_) readerClosed_ = true;
        else { readerCloseError_ = GetLastError(); failed_ = true; }
    }
    void ReadOriginal() noexcept {
        LARGE_INTEGER zero{};
        seekAttempted_ = true;
        seekReturned_ = SetFilePointerEx(reader_,zero,&seekReturnedPosition_,FILE_BEGIN);
        if (!seekReturned_) seekError_ = GetLastError();
        if (!seekReturned_ || seekReturnedPosition_.QuadPart!=0) failed_ = true;
        identityReturned_ = GetFileInformationByHandleEx(reader_,FileIdInfo,&identity_,sizeof(identity_));
        if (!identityReturned_) identityError_ = GetLastError();
        if (!identityReturned_ || identity_.VolumeSerialNumber != expectedIdentity_.VolumeSerialNumber ||
            memcmp(identity_.FileId.Identifier,expectedIdentity_.FileId.Identifier,sizeof(identity_.FileId.Identifier))) failed_ = true;
        bool equal = !failed_;
        while (retained_ < expected_ && !failed_) {
            if (readCount_==readCapacity_) { failed_ = true; break; }
            auto& row = reads_[readCount_++]; row.offset = retained_;
            row.requested = expected_-retained_>65536 ? 65536 : static_cast<DWORD>(expected_-retained_);
            row.attempted = true; row.returned = ReadFile(reader_,bytes_.get()+retained_,row.requested,&row.actual,nullptr);
            if (!row.returned) row.error = GetLastError();
            if (!row.returned && row.error==ERROR_IO_PENDING) {
                readCompletionUnknown_=failed_=true; break;
            }
            if (row.actual>row.requested) { readCompletionUnknown_ = failed_ = true; break; }
            const SIZE_T before = retained_; retained_ += row.actual;
            if (!row.returned) { readCompletionUnknown_ = row.error==ERROR_IO_PENDING; failed_ = true; break; }
            if (!row.actual) { failed_ = true; break; }
            if (memcmp(bytes_.get()+before,expectedBytes_+before,row.actual)) equal = false;
        }
        if (!failed_) {
            if (readCount_==readCapacity_) failed_ = true;
            else {
                auto& row = reads_[readCount_++]; row.offset = retained_; row.requested = 1; row.attempted = true;
                row.returned = ReadFile(reader_,bytes_.get()+retained_,1,&row.actual,nullptr);
                if (!row.returned) row.error = GetLastError();
                if (!row.returned && row.error==ERROR_IO_PENDING) readCompletionUnknown_=failed_=true;
                else if (row.actual>1) readCompletionUnknown_ = failed_ = true;
                else {
                    retained_ += row.actual;
                    if (!row.returned) { readCompletionUnknown_ = row.error==ERROR_IO_PENDING; failed_ = true; }
                    else eof_ = row.actual==0;
                }
            }
        }
        matched_ = equal && retained_==expected_ && eof_;
        if (!matched_) failed_ = true;
        CloseReaderOnce();
    }
    friend struct template_creator_bridge::NativePlatformState;
    friend struct template_creator_bridge::OriginalWindowsStandaloneHost;
    ~OriginalWindowsSinkCollector() = default;
    OriginalWindowsSinkCollector(OriginalWindowsSink& sink,SIZE_T selectedBytes,SIZE_T selectedReads,SIZE_T selectedObservations,DWORD selectedWaitMilliseconds)
        : sink_(sink) {
        if (!selectedBytes || selectedBytes==SIZE_MAX || !selectedReads || !selectedObservations || selectedWaitMilliseconds==INFINITE ||
            selectedReads>SIZE_MAX/sizeof(CollectorReadOriginal) || selectedObservations>SIZE_MAX/sizeof(CollectorThreadObservation))
            throw std::invalid_argument("selected independent collector capacity");
        SIZE_T requestedSourceBytes=sizeof(OriginalWindowsSinkCollector);
        for (const SIZE_T bytes : {selectedBytes+1,selectedReads*sizeof(CollectorReadOriginal),
                selectedObservations*sizeof(CollectorThreadObservation)}) {
            if (bytes>SIZE_MAX-requestedSourceBytes)
                throw std::invalid_argument("complete independent collector source graph");
            requestedSourceBytes+=bytes;
        }
        bytes_ = std::make_unique<unsigned char[]>(selectedBytes+1);
        reads_ = std::make_unique<CollectorReadOriginal[]>(selectedReads);
        observations_ = std::make_unique<CollectorThreadObservation[]>(selectedObservations);
        byteCapacity_ = selectedBytes; readCapacity_ = selectedReads; observationCapacity_ = selectedObservations;
        selectedWaitMilliseconds_ = selectedWaitMilliseconds;
    }
public:
    OriginalWindowsSinkCollector(const OriginalWindowsSinkCollector&) = delete;
    OriginalWindowsSinkCollector& operator=(const OriginalWindowsSinkCollector&) = delete;
    bool Start() noexcept {
        if (startAttempted_) return false; startAttempted_ = true;
        if (!sink_.SealOriginalCopies() || sink_.WrittenOriginalBytes()>byteCapacity_) { failed_ = true; return false; }
        expected_ = sink_.WrittenOriginalBytes();
        if (!sink_.CallCount() || !sink_.PrivateCalls()[0].result) { failed_ = true; return false; }
        expectedIdentity_ = sink_.PrivateCalls()[0].identity;
        expectedBytes_ = sink_.PrivateOriginalBytes();
        // ReOpenFile names the held object. The worker explicitly seeks and
        // retains the actual position rather than assuming its initial value.
        // Requested rights are not presumed granted; raw failure stays.
        reader_ = ReOpenFile(sink_.BorrowedOriginal(),GENERIC_READ,FILE_SHARE_READ|FILE_SHARE_WRITE|FILE_SHARE_DELETE,FILE_FLAG_SEQUENTIAL_SCAN);
        if (reader_==INVALID_HANDLE_VALUE) { reopenError_ = GetLastError(); failed_ = true; return false; }
        if (!reader_) { reopenError_=GetLastError(); readerOutcomeUnknown_=failed_=true; return false; }
        readerAcquired_ = true;
        startPublished_.store(true,std::memory_order_release);
        thread_ = CreateThread(nullptr,0,&ThreadMain,this,0,&createdThreadId_);
        if (!thread_) {
            creationError_ = GetLastError(); creationFailed_ = true;
            if (createdThreadId_) threadOutcomeUnknown_=true;
            else { failed_=true; CloseReaderOnce(); }
            return false;
        }
        if (thread_==INVALID_HANDLE_VALUE || !createdThreadId_) {
            creationFailed_=true; threadOutcomeUnknown_=true; return false;
        }
        threadAcquired_ = true; return true;
    }
    bool Poll() noexcept {
        if (!threadAcquired_ || threadCloseAttempted_ || threadOutcomeUnknown_.load(std::memory_order_acquire)) return false;
        if (observationCount_==observationCapacity_) { observationExhausted_ = true; return false; }
        auto& row = observations_[observationCount_++];
        row.requestedWaitMilliseconds = selectedWaitMilliseconds_;
        row.waitReturned = WaitForSingleObject(thread_,row.requestedWaitMilliseconds);
        if (row.waitReturned==WAIT_FAILED) row.waitError = GetLastError();
        row.timesReturned = GetThreadTimes(thread_,&row.born,&row.exited,&row.kernel,&row.user);
        if (!row.timesReturned) row.timesError = GetLastError();
        row.codeReturned = GetExitCodeThread(thread_,&row.code);
        if (!row.codeReturned) row.codeError = GetLastError();
        if (row.waitReturned==WAIT_FAILED || !row.timesReturned || !row.codeReturned) observationFailed_ = true;
        if (row.waitReturned!=WAIT_OBJECT_0) return false;
        threadTerminal_ = true;
        // Publication is read only after actual native terminal plus acquire.
        if (!bodyReturned_.load(std::memory_order_acquire) || !row.timesReturned || !row.codeReturned || row.code!=0) failed_ = true;
        if (!threadCloseAttempted_) {
            threadCloseAttempted_ = true; threadCloseReturned_ = CloseHandle(thread_);
            if (threadCloseReturned_) threadClosed_ = true; else { threadCloseError_ = GetLastError(); failed_ = true; }
        }
        return threadClosed_;
    }
    bool Qualified() const noexcept { return threadTerminal_ && threadClosed_ && !creationFailed_ && !observationFailed_ && !observationExhausted_ && bodyReturned_.load(std::memory_order_acquire) && matched_ && eof_ && readerClosed_ && !failed_; }
    bool ObserveSelectedClosure() noexcept {
        // This selected observation roster is storage/wait composition, not a
        // replacement deadline for the protected commands. Exhaustion retains
        // unknown custody; it never terminates, detaches or repeats a close.
        while (threadAcquired_ && !threadCloseAttempted_ && observationCount_<observationCapacity_) Poll();
        if (threadAcquired_ && !threadCloseAttempted_ && observationCount_==observationCapacity_) observationExhausted_ = true;
        return PhysicalDisposed();
    }
    bool PhysicalDisposed() const noexcept { return !readerOutcomeUnknown_ && !threadOutcomeUnknown_.load(std::memory_order_acquire) &&
        (!threadAcquired_ || threadClosed_) && (!readerAcquired_ || readerClosed_) && !readCompletionUnknown_; }
    bool ReservedSourceBytes(SIZE_T& bytes) const noexcept {
        bytes = sizeof(*this);
        const auto add = [&](SIZE_T amount) noexcept { if (amount>SIZE_MAX-bytes) return false; bytes+=amount; return true; };
        return add(byteCapacity_+1) && add(readCapacity_*sizeof(CollectorReadOriginal)) && add(observationCapacity_*sizeof(CollectorThreadObservation));
    }
    const CollectorReadOriginal* PrivateReadsAfterPublication() const noexcept { return bodyReturned_.load(std::memory_order_acquire) && !readCompletionUnknown_ ? reads_.get() : nullptr; }
    const unsigned char* PrivateBytesAfterPublication() const noexcept { return bodyReturned_.load(std::memory_order_acquire) && !readCompletionUnknown_ ? bytes_.get() : nullptr; }
    SIZE_T PrivateReadCountAfterPublication() const noexcept { return bodyReturned_.load(std::memory_order_acquire) && !readCompletionUnknown_ ? readCount_ : 0; }
    SIZE_T PrivateByteExtentAfterPublication() const noexcept { return bodyReturned_.load(std::memory_order_acquire) && !readCompletionUnknown_ ? retained_ : 0; }
    const CollectorThreadObservation* PrivateThreadObservations() const noexcept { return observations_.get(); }
    SIZE_T PrivateThreadObservationCount() const noexcept { return observationCount_; }
};
}

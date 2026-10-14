// UNCOMPILED. Explicit inherited stream capabilities and finite pump operations.
// Source-owned pipe byte storage is bounded; actual whole native/runtime memory,
// DLL/compiler provenance and complete input admission are separate requirements.
#pragma once
#include <windows.h>
#include <array>
#include <cstddef>
#include <cstdint>
#include <memory>
namespace template_creator {
constexpr SIZE_T StreamLimit = 8 * 1024 * 1024;
struct PipeHandle {
    HANDLE original = nullptr;
    bool attempted = false, closed = false, closeReported = false, outputUnknown = false;
    DWORD closeError = ERROR_SUCCESS;
    BOOL closeResult = FALSE;
    void CloseOnce() noexcept {
        if (!original || attempted || outputUnknown) return;
        attempted = true;
        closeResult = CloseHandle(original);
        if (closeResult) closed = true;
        else closeError = GetLastError();
    }
};
struct StreamAttempt {
    BOOL peekResult = FALSE, readResult = FALSE;
    DWORD peekError = 0, readError = 0, available = 0, requested = 0, actual = 0;
    SIZE_T destinationOffset = 0;
    bool readAttempted = false;
};
struct Stream {
    static constexpr SIZE_T AttemptLimit = 65536;
    PipeHandle read, write;
    std::unique_ptr<unsigned char[]> bytes = std::make_unique<unsigned char[]>(StreamLimit);
    std::unique_ptr<unsigned char[]> discard = std::make_unique<unsigned char[]>(StreamLimit);
    std::array<StreamAttempt, AttemptLimit> attempts{}, discardAttempts{};
    SIZE_T attemptCount = 0, discardAttemptCount = 0, used = 0, discardUsedBytes = 0;
    bool eof = false, failed = false, overflow = false;
    bool readCompletionUnknown = false;
    DWORD peekError = 0, readError = 0;
    bool discardUsed = false, discardEof = false, discardFailed = false;
    std::uint64_t discardBytes = 0;
    DWORD discardPeekError = 0, discardReadError = 0;
    // Every attempt is preowned before Peek/Read; raw count and affected bytes
    // survive even when the native BOOL is FALSE. Idle observations consume rows.
    bool Pump(bool draining) noexcept {
        if (readCompletionUnknown || read.outputUnknown) return false;
        auto& count = draining ? discardAttemptCount : attemptCount;
        if (count == AttemptLimit) {
            if (draining) discardFailed = true; else overflow = failed = true;
            return false;
        }
        StreamAttempt& row = draining ? discardAttempts[count++] : attempts[count++];
        row.destinationOffset = draining ? discardUsedBytes : used;
        row.peekResult = PeekNamedPipe(read.original, nullptr, 0, nullptr, &row.available, nullptr);
        if (!row.peekResult) {
            row.peekError = GetLastError();
            if(row.peekError==ERROR_IO_PENDING){
                readCompletionUnknown=true;
                if(draining){discardPeekError=row.peekError;discardFailed=true;}
                else{peekError=row.peekError;failed=true;}
                // Unexpected pending preserves this original native output cell
                // and endpoint; no count inspection, reuse or retirement follows.
                return false;
            }
            if (draining) {
                discardPeekError = row.peekError;
                if (row.peekError == ERROR_BROKEN_PIPE) discardEof = true;
                else discardFailed = true;
            } else {
                peekError = row.peekError;
                if (row.peekError == ERROR_BROKEN_PIPE) eof = true;
                else failed = true;
            }
            return false;
        }
        if (!row.available) return false;
        SIZE_T& retained = draining ? discardUsedBytes : used;
        if (retained == StreamLimit) {
            if (draining) discardFailed = true; else overflow = failed = true;
            return false;
        }
        row.requested = row.available > 65536 ? 65536 : row.available;
        if (row.requested > StreamLimit - retained)
            row.requested = static_cast<DWORD>(StreamLimit - retained);
        row.readAttempted = true;
        auto destination = (draining ? discard.get() : bytes.get()) + retained;
        row.readResult = ReadFile(read.original, destination, row.requested, &row.actual, nullptr);
        if (!row.readResult) row.readError = GetLastError();
        if (!row.readResult && row.readError==ERROR_IO_PENDING) {
            readCompletionUnknown=true;
            if (draining) { discardReadError=row.readError; discardFailed=true; }
            else { readError=row.readError; failed=true; }
            // Native count/destination may still be borrowed. Retain their full
            // backing without reading or publishing a purported final prefix.
            return false;
        }
        if (row.actual > row.requested) {
            readCompletionUnknown=true;
            if (draining) discardFailed = true; else failed = true;
            return false;
        }
        retained += row.actual; // Preserve partial bytes before interpreting FALSE.
        if (draining) discardBytes += row.actual;
        if (!row.readResult) {
            if (draining) {
                discardReadError = row.readError;
                if (row.readError == ERROR_BROKEN_PIPE) discardEof = true;
                else discardFailed = true;
            } else {
                readError = row.readError;
                if (row.readError == ERROR_BROKEN_PIPE) eof = true;
                else failed = true;
            }
        }
        return row.actual != 0;
    }
    bool Poll() noexcept {
        if (readCompletionUnknown || read.outputUnknown || failed || eof || !read.original || read.attempted) return false;
        return Pump(false);
    }
    // Failure-only bytes remain private and separate; their EOF cannot PASS.
    bool DrainPoll() noexcept {
        if (readCompletionUnknown || read.outputUnknown || !failed || discardEof || discardFailed || !read.original || read.attempted) return false;
        discardUsed = true;
        return Pump(true);
    }
};
class LaunchStreams {
    // Attribute storage allocated before native handles. The exact required
    // native size is queried and checked against this preallocated capacity.
    alignas(void*) std::array<unsigned char, 65536> attributes_{};
    bool attributesInitialized_ = false, attributesDeleted_ = false;
    std::array<HANDLE, 3> inherited_{};
    SECURITY_ATTRIBUTES pipeSecurityOriginal_{};
    DWORD pipeSizeOriginal_ = 0;
    bool CreatePipeOriginal(SIZE_T ordinal,PipeHandle& reader,PipeHandle& writer) noexcept {
        setupAttempted[ordinal]=true;
        setupResults[ordinal]=CreatePipe(&reader.original,&writer.original,&pipeSecurityOriginal_,pipeSizeOriginal_);
        if (!setupResults[ordinal]) {
            setupErrors[ordinal]=GetLastError(); primaryError=setupErrors[ordinal];
            if (reader.original || writer.original) reader.outputUnknown=writer.outputUnknown=true;
            return false;
        }
        if (!reader.original || !writer.original || reader.original==INVALID_HANDLE_VALUE ||
            writer.original==INVALID_HANDLE_VALUE || reader.original==writer.original) {
            reader.outputUnknown=writer.outputUnknown=true;
            setupShapeFailure="pipe_creation_output_shape"; return false;
        }
        return true;
    }
public:
    Stream out, err;
    PipeHandle inputRead, inputWrite;
    STARTUPINFOEXW startup{};
    DWORD primaryError = 0;
    const char* setupShapeFailure = nullptr;
    bool prepared = false, prepareAttempted = false, writersClosed = false;
    std::array<bool, 9> setupAttempted{};
    std::array<BOOL, 9> setupResults{};
    std::array<DWORD, 9> setupErrors{};
    SIZE_T queriedAttributeBytes = 0, initializedAttributeBytes = 0;
    bool Prepare() noexcept {
        if (prepareAttempted) return false;
        prepareAttempted = true;
        pipeSecurityOriginal_={sizeof(pipeSecurityOriginal_),nullptr,TRUE};
        pipeSizeOriginal_=65536;
        auto retain = [this](SIZE_T ordinal, BOOL result) noexcept {
            setupAttempted[ordinal] = true; setupResults[ordinal] = result;
            if (!result) { setupErrors[ordinal] = GetLastError(); primaryError = setupErrors[ordinal]; }
            return result != FALSE;
        };
        if (!CreatePipeOriginal(0,inputRead,inputWrite)) return false;
        if (!CreatePipeOriginal(1,out.read,out.write)) return false;
        if (!CreatePipeOriginal(2,err.read,err.write)) return false;
        if (!retain(3, SetHandleInformation(inputWrite.original, HANDLE_FLAG_INHERIT, 0))) return false;
        if (!retain(4, SetHandleInformation(out.read.original, HANDLE_FLAG_INHERIT, 0))) return false;
        if (!retain(5, SetHandleInformation(err.read.original, HANDLE_FLAG_INHERIT, 0))) return false;
        setupAttempted[6] = true;
        setupResults[6] = InitializeProcThreadAttributeList(nullptr, 1, 0, &queriedAttributeBytes);
        if (!setupResults[6]) setupErrors[6] = GetLastError();
        if (setupResults[6] || setupErrors[6] != ERROR_INSUFFICIENT_BUFFER ||
                !queriedAttributeBytes || queriedAttributeBytes > attributes_.size()) {
            setupShapeFailure = "attribute_query_return_or_capacity";
            if (!setupResults[6] && setupErrors[6] != ERROR_INSUFFICIENT_BUFFER)
                primaryError = setupErrors[6];
            return false;
        }
        startup.StartupInfo.cb = sizeof(startup);
        startup.lpAttributeList = reinterpret_cast<LPPROC_THREAD_ATTRIBUTE_LIST>(attributes_.data());
        initializedAttributeBytes = queriedAttributeBytes;
        if (!retain(7, InitializeProcThreadAttributeList(startup.lpAttributeList, 1, 0, &initializedAttributeBytes)))
            return false;
        attributesInitialized_ = true;
        inherited_ = {inputRead.original, out.write.original, err.write.original};
        if (!retain(8, UpdateProcThreadAttribute(startup.lpAttributeList, 0,
                PROC_THREAD_ATTRIBUTE_HANDLE_LIST, inherited_.data(), sizeof(inherited_), nullptr, nullptr)))
            return false;
        startup.StartupInfo.dwFlags = STARTF_USESTDHANDLES;
        startup.StartupInfo.hStdInput = inputRead.original;
        startup.StartupInfo.hStdOutput = out.write.original;
        startup.StartupInfo.hStdError = err.write.original;
        prepared = true;
        return true;
    }
    // Parent copies of child ends are independently closed after creation.
    // Input is empty by this concrete operation; a stdin payload requires its
    // own source-owned admitted writer protocol, never implicit ambient input.
    bool AfterCreate() noexcept {
        inputRead.CloseOnce(); inputWrite.CloseOnce();
        out.write.CloseOnce(); err.write.CloseOnce();
        writersClosed = inputRead.closed && inputWrite.closed && out.write.closed && err.write.closed;
        return writersClosed;
    }
    bool Poll() noexcept {
        bool progressed = out.Poll();
        return err.Poll() || progressed;
    }
    bool NativeEof() const noexcept {
        return writersClosed && out.eof && err.eof && !out.failed && !err.failed;
    }
    bool DisposeEligible(bool nativeMembersTerminal) noexcept {
        bool disposalEof = (out.eof || out.discardEof) && (err.eof || err.discardEof);
        if (!nativeMembersTerminal || !disposalEof || out.readCompletionUnknown || err.readCompletionUnknown) return false;
        out.read.CloseOnce(); err.read.CloseOnce();
        if (attributesInitialized_ && !attributesDeleted_) {
            DeleteProcThreadAttributeList(startup.lpAttributeList);
            attributesDeleted_ = true; // Native API returns void, no fake result.
        }
        return out.read.closed && err.read.closed && writersClosed &&
               (!attributesInitialized_ || attributesDeleted_);
    }
    bool DisposeUnlaunched() noexcept {
        if (out.readCompletionUnknown || err.readCompletionUnknown) return false;
        // No child was created, so all acquired pipe ends remain exclusively
        // this setup owner's resources; no process/EOF inference is needed.
        inputRead.CloseOnce(); inputWrite.CloseOnce();
        out.read.CloseOnce(); out.write.CloseOnce();
        err.read.CloseOnce(); err.write.CloseOnce();
        if (attributesInitialized_ && !attributesDeleted_) {
            DeleteProcThreadAttributeList(startup.lpAttributeList);
            attributesDeleted_ = true;
        }
        const PipeHandle* cells[] = {&inputRead, &inputWrite, &out.read, &out.write, &err.read, &err.write};
        bool all = true;
        for (auto cell : cells) all = all && !cell->outputUnknown && (!cell->original || cell->closed);
        return all && (!attributesInitialized_ || attributesDeleted_);
    }
    // Setup failures retain all acquired handles and raw error state. Explicit
    // owner policy decides eligibility; no destructor hides uncertain disposal.
};
} // namespace template_creator

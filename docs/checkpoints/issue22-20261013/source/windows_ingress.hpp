// UNCOMPILED private original handoff capture. No issuer/native authority grant.
#pragma once
#include <windows.h>
#include <array>
#include "windows_identity.hpp"
#include "windows_pending_call_owner.hpp"
namespace template_creator {
struct IngressCapability {
    HANDLE original = nullptr;
    bool closeAttempted = false, closed = false, aliasesEarlier = false;
    BOOL closeResult = FALSE;
    DWORD closeError = 0;
    BOOL informationResult = FALSE;
    DWORD information = 0, informationError = 0;
    void Observe() noexcept {
        informationResult = GetHandleInformation(original, &information);
        if (!informationResult) informationError = GetLastError();
    }
    void CloseOnce() noexcept {
        if (!original || original == INVALID_HANDLE_VALUE || closeAttempted || aliasesEarlier) return;
        closeAttempted = true;
        closeResult = CloseHandle(original);
        if (closeResult) closed = true;
        else closeError = GetLastError();
    }
};
class OriginalPrivateIngress {
    // Actual original capabilities transferred directly by the private native
    // caller, not reconstructed from public argv, ENV, paths or JSON integers.
    // Slots: pipe server, original caller process, root directory, original
    // compiler/tool file, private sink. Association remains a concrete gate task.
    std::array<IngressCapability, 5> capabilities_{};
    std::array<unsigned char, 65536> body_{};
    bool attempted_ = false, captured_ = false, shapeFailure_ = false;
    DWORD callerPid_ = 0, callerPidError_ = 0, pipeCallerPid_ = 0;
    BOOL pipeCallerResult_ = FALSE;
    DWORD pipeCallerError_ = 0;
    FILETIME callerBirth_{}, callerExit_{}, callerKernel_{}, callerUser_{};
    BOOL timesResult_ = FALSE;
    DWORD timesError_ = 0;
    BOOL pipeInfoResult_ = FALSE;
    DWORD pipeFlags_ = 0, outSize_ = 0, inSize_ = 0, instances_ = 0, pipeInfoError_ = 0;
    BOOL pipeStateResult_ = FALSE;
    DWORD pipeState_ = 0, pipeStateError_ = 0;
    BOOL peekResult_ = FALSE;
    DWORD available_ = 0, firstMessageBytes_ = 0, peekError_ = 0;
    bool readAttempted_ = false;
    BOOL readResult_ = FALSE;
    DWORD readRequested_ = 0, readActual_ = 0, readError_ = 0;
    struct BodyReadOriginal {
        HANDLE endpoint=nullptr;void* destination=nullptr;DWORD requested=0;
        template_creator_bridge::OriginalWindowsPendingCall* pending=nullptr;
    } bodyReadOriginal_;
    template_creator_bridge::OriginalWindowsPendingCalls* pendingCallsOriginal_=nullptr;
    bool pendingReadUnknown_=false;
    TokenObservation callerToken_{};
public:
    OriginalPrivateIngress() = default;
    OriginalPrivateIngress(const OriginalPrivateIngress&) = delete;
    OriginalPrivateIngress& operator=(const OriginalPrivateIngress&) = delete;
    OriginalPrivateIngress(OriginalPrivateIngress&&) = delete;
    OriginalPrivateIngress& operator=(OriginalPrivateIngress&&) = delete;
    bool CaptureTransferredOriginals(const std::array<HANDLE, 5>& originals,
        template_creator_bridge::OriginalWindowsPendingCalls& pendingCalls) noexcept {
        if (attempted_) return false;
        attempted_ = true;pendingCallsOriginal_=&pendingCalls;
        // Attach ALL transferred capabilities before a native query can fail.
        for (SIZE_T i=0; i<originals.size(); ++i) {
            capabilities_[i].original = originals[i];
            for (SIZE_T earlier=0; earlier<i; ++earlier)
                if (originals[i] == originals[earlier]) {
                    capabilities_[i].aliasesEarlier = true;
                    shapeFailure_ = true;
                }
        }
        if (shapeFailure_) return false;
        for (auto& capability : capabilities_) capability.Observe();
        for (const auto& capability : capabilities_) if (!capability.informationResult) return false;
        callerPid_ = GetProcessId(capabilities_[1].original);
        if (!callerPid_) callerPidError_ = GetLastError();
        timesResult_ = GetProcessTimes(capabilities_[1].original, &callerBirth_, &callerExit_,
                                      &callerKernel_, &callerUser_);
        if (!timesResult_) timesError_ = GetLastError();
        callerToken_.Capture(capabilities_[1].original);
        pipeCallerResult_ = GetNamedPipeClientProcessId(capabilities_[0].original, &pipeCallerPid_);
        if (!pipeCallerResult_) pipeCallerError_ = GetLastError();
        pipeInfoResult_ = GetNamedPipeInfo(capabilities_[0].original, &pipeFlags_, &outSize_, &inSize_, &instances_);
        if (!pipeInfoResult_) pipeInfoError_ = GetLastError();
        pipeStateResult_ = GetNamedPipeHandleStateW(capabilities_[0].original, &pipeState_, nullptr,
                                                 nullptr, nullptr, nullptr, 0);
        if (!pipeStateResult_) pipeStateError_ = GetLastError();
        if (!callerPid_ || !timesResult_ || !pipeCallerResult_ || !pipeInfoResult_ || !pipeStateResult_ || !callerToken_.captured) return false;
        if (callerPid_ != pipeCallerPid_ || !(pipeFlags_ & PIPE_SERVER_END) ||
                !(pipeFlags_ & PIPE_TYPE_MESSAGE) || !(pipeState_ & PIPE_READMODE_MESSAGE)) {
            shapeFailure_ = true; return false;
        }
        peekResult_ = PeekNamedPipe(capabilities_[0].original, nullptr, 0, nullptr, &available_, &firstMessageBytes_);
        if (!peekResult_) { peekError_ = GetLastError(); return false; }
        if (!firstMessageBytes_ || firstMessageBytes_ > body_.size() || firstMessageBytes_ > available_) {
            shapeFailure_ = true; return false;
        }
        readRequested_ = firstMessageBytes_;

        bodyReadOriginal_.endpoint=capabilities_[0].original;bodyReadOriginal_.destination=body_.data();
        bodyReadOriginal_.requested=readRequested_;
        bodyReadOriginal_.pending=pendingCalls.BeginOriginal(this,&bodyReadOriginal_,body_.data(),readRequested_,
            nullptr,offsetof(BodyReadOriginal,pending));
        if(!bodyReadOriginal_.pending)return false;
        readAttempted_=true;
        readResult_ = ReadFile(bodyReadOriginal_.endpoint,bodyReadOriginal_.destination,bodyReadOriginal_.requested,&readActual_,nullptr);
        if (!readResult_) readError_ = GetLastError();
        pendingReadUnknown_=!pendingCalls.ReturnedOriginal(*bodyReadOriginal_.pending,readResult_,readError_,readActual_);
        // Preserve actual output and partial bytes BEFORE interpretation. There
        // is no retry after uncertain reads, no inferred EOF or authority.
        if (pendingReadUnknown_ || !readResult_) return false;
        if (readActual_ != readRequested_) { shapeFailure_ = true; return false; }
        captured_ = true;
        return true;
    }
    void DisposeEligible() noexcept {
        callerToken_.Dispose();
        for (SIZE_T i=0;i<capabilities_.size();++i) {
            if (i==0 && (ReadCompletionUnknown() || !pendingCallsOriginal_ ||
                !pendingCallsOriginal_->EndpointsIndependentlyEligible())) continue;
            capabilities_[i].CloseOnce();
        }
    }
    bool ReadCompletionUnknown() const noexcept {
        return pendingReadUnknown_ || (readAttempted_ && ((!readResult_ && readError_==ERROR_IO_PENDING) || readActual_>readRequested_));
    }
    bool PhysicallyDisposed() const noexcept {
        if (ReadCompletionUnknown() || callerToken_.outputUnknown || (callerToken_.acquired && !callerToken_.closed)) return false;
        for (SIZE_T i=0;i<capabilities_.size();++i) {
            const auto& capability = capabilities_[i];
            if (!capability.original || capability.original==INVALID_HANDLE_VALUE) continue;
            if (capability.aliasesEarlier) {
                bool earlierClosed = false;
                for (SIZE_T j=0;j<i;++j)
                    if (capabilities_[j].original==capability.original && capabilities_[j].closed) earlierClosed = true;
                if (!earlierClosed) return false;
            } else if (!capability.closed) return false;
        }
        return true;
    }
    // Retain the entire source-owned object in the private original sink/session;
    // public projection must never expose handles, token bytes or private body.
    const template_creator_bridge::OriginalWindowsPendingCalls* OriginalPendingCalls() const noexcept {return pendingCallsOriginal_;}
    const auto& OriginalCapabilities() const noexcept { return capabilities_; }
    const auto& OriginalBody() const noexcept { return body_; }
    const TokenObservation& OriginalCallerToken() const noexcept { return callerToken_; }
    DWORD OriginalCallerPid() const noexcept { return callerPid_; }
    DWORD OriginalCallerPidError() const noexcept { return callerPidError_; }
    DWORD OriginalPipeCallerPid() const noexcept { return pipeCallerPid_; }
    BOOL OriginalPipeCallerResult() const noexcept { return pipeCallerResult_; }
    DWORD OriginalPipeCallerError() const noexcept { return pipeCallerError_; }
    BOOL OriginalTimesResult() const noexcept { return timesResult_; }
    DWORD OriginalTimesError() const noexcept { return timesError_; }
    const FILETIME& OriginalCallerBirth() const noexcept { return callerBirth_; }
    const FILETIME& OriginalCallerExit() const noexcept { return callerExit_; }
    const FILETIME& OriginalCallerKernel() const noexcept { return callerKernel_; }
    const FILETIME& OriginalCallerUser() const noexcept { return callerUser_; }
    BOOL OriginalPipeInfoResult() const noexcept { return pipeInfoResult_; }
    DWORD OriginalPipeInfoError() const noexcept { return pipeInfoError_; }
    DWORD OriginalPipeFlags() const noexcept { return pipeFlags_; }
    DWORD OriginalPipeOutSize() const noexcept { return outSize_; }
    DWORD OriginalPipeInSize() const noexcept { return inSize_; }
    DWORD OriginalPipeInstances() const noexcept { return instances_; }
    BOOL OriginalPipeStateResult() const noexcept { return pipeStateResult_; }
    DWORD OriginalPipeStateError() const noexcept { return pipeStateError_; }
    DWORD OriginalPipeState() const noexcept { return pipeState_; }
    BOOL OriginalPeekResult() const noexcept { return peekResult_; }
    DWORD OriginalPeekError() const noexcept { return peekError_; }
    DWORD OriginalAvailableBytes() const noexcept { return available_; }
    DWORD OriginalFirstMessageBytes() const noexcept { return firstMessageBytes_; }
    BOOL OriginalReadResult() const noexcept { return readResult_; }
    DWORD OriginalReadRequested() const noexcept { return readRequested_; }
    DWORD OriginalReadActual() const noexcept { return readActual_; }
    DWORD OriginalReadError() const noexcept { return readError_; }
    bool ReadAttempted() const noexcept { return readAttempted_; }
    bool Captured() const noexcept { return captured_; }
    bool ShapeFailure() const noexcept { return shapeFailure_; }
};
// Same live caller association, root/tools rights, complete byte selection,
// capacity and immutable sender/private-pipe custody still require concrete gate
// composition. Connect-time PID equality alone does not establish those facts.
}

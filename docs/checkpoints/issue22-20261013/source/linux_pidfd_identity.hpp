// UNCOMPILED actual SCM_CREDENTIALS-to-transferred-pidfd association. No issuer,
// signature, declared PID or role Boolean substitutes for these kernel outputs.
#pragma once
#include "linux_ingress.hpp"
#include "linux_task_identity.hpp"
#include "linux_original_deadline.hpp"
#include <sys/syscall.h>
#include <poll.h>
namespace template_creator_linux {
struct PidfdPollOriginal {
    struct pollfd output{};
    int returned = -1, error = 0;
    bool captureStarted=false,attempted=false,returnedObserved=false;
    const OriginalClockBoundary* postClockOriginal=nullptr;
    bool Capture(int originalPidfd,OriginalDeadline& deadline,OriginalDeadlineSupervisor* supervisor=nullptr) noexcept {
        if(captureStarted)return false;captureStarted=true;
        output.fd = originalPidfd; output.events = POLLIN;
        if(supervisor&&supervisor->OriginalDeadlineOwner()!=&deadline)return false;
        if (!deadline.Observe("pidfd-poll",false)) return false;
        if(supervisor&&!supervisor->BeginNativeCall())return false;
        attempted=true;
        returned = poll(&output,1,0);
        if (returned < 0) error = errno;
        returnedObserved=true;
        if(supervisor)supervisor->OriginalNativeCallReturned();
        const size_t postOrdinal=deadline.OriginalCount();
        const bool clockClean = deadline.Observe("pidfd-poll",true);
        if(postOrdinal<SIZE_MAX&&deadline.OriginalCount()==postOrdinal+1)
            postClockOriginal=deadline.PrivateOriginals()+postOrdinal;
        return clockClean && postClockOriginal && postClockOriginal->shapeValid &&
            !postClockOriginal->expired && returned == 0 && output.revents == 0;
    }
};
class OriginalPidfdHandoffAssociation {
    OriginalPrivateIngress& ingress_;
    HeldTaskIdentity& receiverTask_;
    HeldTaskIdentity callerTask_;
    ProcOwnedFd fdinfoDirectory_;
    ProcFileObservation fdinfo_;
    PidfdPollOriginal before_, after_;
    int borrowedPidfd_ = -1;
    OriginalDeadline* deadline_ = nullptr;
    long signalZeroResult_ = -1; int signalZeroError_ = 0;
    bool signalZeroAttempted_ = false, attempted_ = false, associated_ = false;
    pid_t originalMessagePid_ = 0;
    bool preownAttempted_=false,preowned_=false;
    std::exception_ptr preownException_;
public:
    bool OutputBackingSeparateOriginal(const void* output,size_t bytes) const noexcept {
        return OriginalBackingStorageSeparate(output,bytes,this,1,sizeof(*this))&&
            callerTask_.OutputBackingSeparateOriginal(output,bytes)&&fdinfo_.byteCapacity!=SIZE_MAX&&
            OriginalBackingStorageSeparate(output,bytes,fdinfo_.bytes.get(),fdinfo_.bytes?fdinfo_.byteCapacity+1:0)&&
            OriginalBackingStorageSeparate(output,bytes,fdinfo_.reads.get(),fdinfo_.readCapacity,sizeof(ProcReadOriginal));
    }
    static bool OneNamespacePid(const ProcTaskObservation& receiver, pid_t actualReceiverTid) noexcept {
        const auto* cursor = receiver.status.bytes.get(); const auto* end = cursor+receiver.status.used;
        bool seen = false;
        while (cursor != end) {
            const auto* lineEnd = cursor; while (lineEnd != end && *lineEnd != '\n') ++lineEnd;
            if (lineEnd-cursor >= 6 && memcmp(cursor,"NSpid:",6)==0) {
                if (seen) return false; seen = true;
                const auto* start = cursor+6; while (start != lineEnd && (*start==' ' || *start=='\t')) ++start;
                const auto* stop = lineEnd; while (stop != start && (stop[-1]==' ' || stop[-1]=='\t')) --stop;
                uint64_t parsed = 0;
                if (!ProcTaskObservation::Unsigned(start,stop,parsed) || parsed != static_cast<uint64_t>(actualReceiverTid)) return false;
            }
            cursor = lineEnd == end ? end : lineEnd+1;
        }
        return seen;
    }
    static bool FdinfoPidOriginal(const ProcFileObservation& original,pid_t expected) noexcept {
        const auto* cursor = original.bytes.get(); const auto* end = cursor+original.used;
        bool seen = false;
        while (cursor != end) {
            const auto* lineEnd = cursor; while (lineEnd != end && *lineEnd != '\n') ++lineEnd;
            if (lineEnd-cursor >= 4 && memcmp(cursor,"Pid:",4)==0) {
                if (seen) return false; seen = true;
                const auto* begin = cursor+4; while (begin != lineEnd && (*begin==' ' || *begin=='\t')) ++begin;
                const auto* finish = lineEnd; while (finish != begin && (finish[-1]==' ' || finish[-1]=='\t')) --finish;
                uint64_t value = 0;
                if (!ProcTaskObservation::Unsigned(begin,finish,value) || value != static_cast<uint64_t>(expected)) return false;
            }
            cursor = lineEnd == end ? end : lineEnd+1;
        }
        return seen;
    }
public:
    OriginalPidfdHandoffAssociation(DeferredOriginalPreownTag,OriginalPrivateIngress& ingress, HeldTaskIdentity& receiver,
            size_t callerObservations, size_t statBytes, size_t statusBytes, size_t readRows, size_t fdinfoBytes) noexcept
        : ingress_(ingress), receiverTask_(receiver), callerTask_(DeferredOriginalPreown,callerObservations,statBytes,statusBytes,readRows),
          fdinfo_(DeferredOriginalPreown,fdinfoBytes,readRows) {}
    OriginalPidfdHandoffAssociation(OriginalPrivateIngress& ingress, HeldTaskIdentity& receiver,
            size_t callerObservations, size_t statBytes, size_t statusBytes, size_t readRows, size_t fdinfoBytes)
        : OriginalPidfdHandoffAssociation(DeferredOriginalPreown,ingress,receiver,callerObservations,statBytes,statusBytes,readRows,fdinfoBytes) {
        if(!PreownOriginal()&&preownException_)std::rethrow_exception(preownException_);
    }
    bool PreownOriginal() noexcept {
        if(preownAttempted_)return false;preownAttempted_=true;
        if(!callerTask_.PreownOriginal()){preownException_=callerTask_.PreownExceptionOriginal();return false;}
        if(!fdinfo_.PreownOriginal()){preownException_=fdinfo_.preownException;return false;}
        preowned_=true;return true;
    }
    bool PreownedOriginal() const noexcept{return preowned_;}
    const std::exception_ptr& PreownExceptionOriginal() const noexcept{return preownException_;}
    OriginalPidfdHandoffAssociation(const OriginalPidfdHandoffAssociation&) = delete;
    OriginalPidfdHandoffAssociation& operator=(const OriginalPidfdHandoffAssociation&) = delete;
    bool Capture(size_t actualTransferredOrdinal, pid_t actualReceiverTid, OriginalDeadline& deadline) noexcept {
        if (!preowned_ || attempted_) return false; attempted_ = true;
        deadline_ = &deadline;
        if (!ingress_.Captured() || !ingress_.NativeClocksClean() || ingress_.OriginalDeadlineOwner()!=&deadline ||
            !ingress_.MessageCredentialsPresent() ||
            actualTransferredOrdinal >= ingress_.OriginalDescriptorCount()) return false;
        const auto& descriptor = ingress_.OriginalDescriptors()[actualTransferredOrdinal];
        if (descriptor.original < 0 || descriptor.closeAttempted) return false;
        borrowedPidfd_ = descriptor.original;
        originalMessagePid_ = ingress_.OriginalMessageCredentials().pid;
        if (originalMessagePid_ <= 0 || !receiverTask_.ObservationCount()) return false;
        const auto& receiver = receiverTask_.OriginalObservation(receiverTask_.ObservationCount()-1);
        auto* originalProcRoot = receiverTask_.OriginalRootOwner();
        if (!originalProcRoot || !originalProcRoot->Available() || originalProcRoot->OriginalDeadlineOwner() != &deadline) return false;
        if (!receiverTask_.SameOriginal(receiver) || receiver.observedPid != actualReceiverTid ||
            !OneNamespacePid(receiver,actualReceiverTid)) return false;
        if (!before_.Capture(borrowedPidfd_,deadline)) return false;
        // Signal zero performs permission/type/liveness checking; no signal is
        // delivered and no process state/worker is modified by this observation.
        if (!deadline.Observe("pidfd-send-signal-zero",false)) return false;
        signalZeroAttempted_ = true;
        signalZeroResult_ = syscall(SYS_pidfd_send_signal,borrowedPidfd_,0,nullptr,0);
        if (signalZeroResult_ < 0) signalZeroError_ = errno;
        const bool signalClockClean = deadline.Observe("pidfd-send-signal-zero",true);
        if (signalZeroResult_ < 0 || !signalClockClean) return false;
        if (!deadline.Observe("pidfd-fdinfo-directory-open",false)) return false;
        fdinfoDirectory_.openAttempted = true;
        fdinfoDirectory_.openResult = openat(receiverTask_.OriginalDirectory().original,"fdinfo",O_PATH|O_DIRECTORY|O_NOFOLLOW|O_CLOEXEC);
        fdinfoDirectory_.original = fdinfoDirectory_.openResult;
        if (fdinfoDirectory_.openResult < 0) fdinfoDirectory_.openError = errno;
        const bool directoryClockClean = deadline.Observe("pidfd-fdinfo-directory-open",true);
        if (fdinfoDirectory_.openResult < 0 || !directoryClockClean) return false;
        char literal[32]{};
        const auto converted = std::to_chars(literal,literal+sizeof(literal)-1,borrowedPidfd_);
        bool captured = false;
        if (converted.ec == std::errc{}) captured = fdinfo_.Capture(fdinfoDirectory_.original,literal,deadline);
        fdinfoDirectory_.CloseOnce(&deadline);
        if (!captured || !fdinfoDirectory_.closed || !FdinfoPidOriginal(fdinfo_,originalMessagePid_)) return false;
        if (!callerTask_.BindFromOriginalProcRoot(*originalProcRoot,originalMessagePid_)) return false;
        const auto& caller = callerTask_.OriginalObservation(callerTask_.ObservationCount()-1);
        if (!callerTask_.SameOriginal(caller) || caller.observedTgid != originalMessagePid_) return false;
        if (!after_.Capture(borrowedPidfd_,deadline)) return false;
        associated_ = true; return true;
    }
    bool Associated() const noexcept { return associated_; }
    const OriginalPrivateIngress& OriginalIngressOwner() const noexcept { return ingress_; }
    const OriginalDeadline* OriginalDeadlineOwner() const noexcept { return deadline_; }
    int BorrowedOriginalPidfd() const noexcept { return borrowedPidfd_; }
    pid_t OriginalMessagePid() const noexcept { return originalMessagePid_; }
    HeldTaskIdentity& OriginalCallerTask() noexcept { return callerTask_; }
    const HeldTaskIdentity& OriginalCallerTask() const noexcept { return callerTask_; }
    const PidfdPollOriginal& OriginalBeforePoll() const noexcept { return before_; }
    const PidfdPollOriginal& OriginalAfterPoll() const noexcept { return after_; }
    const ProcFileObservation& OriginalFdinfo() const noexcept { return fdinfo_; }
    const ProcOwnedFd& OriginalFdinfoDirectory() const noexcept { return fdinfoDirectory_; }
    long OriginalSignalZeroResult() const noexcept { return signalZeroResult_; }
    int OriginalSignalZeroError() const noexcept { return signalZeroError_; }
    bool OriginalSignalZeroAttempted() const noexcept { return signalZeroAttempted_; }
    bool ReservedSourceBytes(size_t& bytes) const noexcept {
        size_t callerBytes = 0;
        if (!callerTask_.ReservedSourceBytes(callerBytes)) return false;
        bytes = sizeof(*this);
        const auto add = [&](size_t amount) noexcept {
            if (amount > SIZE_MAX-bytes) return false; bytes += amount; return true;
        };
        // Inline caller/fdinfo structs are already counted by sizeof(*this).
        return add(callerBytes-sizeof(HeldTaskIdentity)) && add(fdinfo_.bytes?fdinfo_.byteCapacity+1:0) &&
            add(fdinfo_.readCapacity*sizeof(ProcReadOriginal));
    }
    bool DisposeEligible(OriginalDeadlineSupervisor* supervisor=nullptr) noexcept {
        fdinfo_.descriptor.CloseOnce(deadline_,supervisor);fdinfoDirectory_.CloseOnce(deadline_,supervisor);
        const bool callerClosed=callerTask_.DisposeEligible(supervisor);
        return callerClosed && (fdinfo_.descriptor.original < 0 || fdinfo_.descriptor.closed) &&
            (fdinfoDirectory_.original < 0 || fdinfoDirectory_.closed);
    }
};
// Association derives from the same received message's actual credentials,
// live transferred kernel pidfd and same namespace fdinfo, not connect-time PID
// alone. Root/rights/tool/capacity/source admission are distinct session duties.
}

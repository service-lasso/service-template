// UNCOMPILED original private handoff capture; observations are not authority.
#pragma once
#include <array>
#include <cerrno>
#include <cstddef>
#include <cstring>
#include <sys/socket.h>
#include <sys/stat.h>
#include <unistd.h>
#include "linux_original_deadline.hpp"
namespace template_creator_linux {
// Per-owner rows avoid racing the creator's OriginalDeadline observation array
// when the independent native caller receives during a pending creator syscall.
struct IngressNativeClock {
    OriginalClockBoundary before{},after{};
    bool nativeAttempted=false,clean=true;
    static bool Capture(OriginalClockBoundary& row,const OriginalDeadline* origin,const char* op,bool isAfter) noexcept {
        if(!origin)return true;
        row.operation=op;row.after=isAfter;row.attempted=true;
        row.returned=clock_gettime(CLOCK_MONOTONIC,&row.output);if(row.returned<0)row.error=errno;
        if(row.returned!=0||row.output.tv_sec<0||row.output.tv_nsec<0||row.output.tv_nsec>=1000000000||
           static_cast<uint64_t>(row.output.tv_sec)>(UINT64_MAX-static_cast<uint64_t>(row.output.tv_nsec))/1000000000)return false;
        row.nanoseconds=static_cast<uint64_t>(row.output.tv_sec)*1000000000+static_cast<uint64_t>(row.output.tv_nsec);
        row.shapeValid=true;row.expired=origin->HasOriginalDeadline()&&row.nanoseconds>=origin->OriginalAbsoluteNanoseconds();
        return !row.expired;
    }
    bool Before(const OriginalDeadline* origin,const char* op,bool independentlyEligible) noexcept {
        clean=Capture(before,origin,op,false);
        nativeAttempted=clean||independentlyEligible;return nativeAttempted;
    }
    void After(const OriginalDeadline* origin,const char* op) noexcept {
        const bool afterClean=Capture(after,origin,op,true);
        clean=clean&&afterClean&&(!origin||after.nanoseconds>=before.nanoseconds);
    }
};
struct IngressFd {
    int original = -1;
    bool closeAttempted = false, closeReturned=false, closed = false;
    int closeResult = 0, closeErrno = 0;
    struct stat status{};
    bool statAttempted = false,statReturned=false;
    int statResult = -1, statErrno = 0;
    bool statObservationAttempted=false;
    IngressNativeClock statClock,closeClock;
    void Observe(const OriginalDeadline* origin=nullptr,bool independentlyEligible=false) noexcept {
        if (statObservationAttempted) return;statObservationAttempted=true;
        if(!statClock.Before(origin,"ingress-fstat",independentlyEligible))return;
        statAttempted = true;
        statResult = fstat(original, &status);
        if (statResult < 0) statErrno = errno;
        statReturned=true;
        statClock.After(origin,"ingress-fstat");
    }
    void CloseOnce(const OriginalDeadline* origin=nullptr) noexcept {
        if (original < 0 || closeAttempted) return;
        closeClock.Before(origin,"ingress-once-close",true);
        closeAttempted = true;
        closeResult = close(original);
        if (closeResult < 0) closeErrno = errno;
        else closed = true;
        closeReturned=true;
        closeClock.After(origin,"ingress-once-close");
    }
};
class OriginalPrivateIngress {
public:
    static constexpr size_t BodyCapacity = 65536, FdCapacity = 64;
private:
    IngressFd socket_{};
    std::array<IngressFd, FdCapacity> transferred_{};
    size_t transferredCount_ = 0;
    std::array<unsigned char, BodyCapacity> body_{};
    alignas(cmsghdr) std::array<unsigned char, CMSG_SPACE(sizeof(int) * FdCapacity) + CMSG_SPACE(sizeof(ucred))> control_{};
    ucred peer_{}, messageCredentials_{};
    bool messageCredentialsPresent_ = false;
    int passCredentials_ = 0, passResult_ = -1, passErrno_ = 0;
    socklen_t passBytes_ = sizeof(passCredentials_);
    socklen_t peerBytes_ = sizeof(peer_);
    int peerResult_ = -1, peerErrno_ = 0;
    int socketType_ = 0, typeResult_ = -1, typeErrno_ = 0;
    socklen_t typeBytes_ = sizeof(socketType_);
    msghdr message_{};
    iovec vector_{};
    ssize_t receiveResult_ = -1;
    int receiveErrno_ = 0;
    bool attempted_ = false, receiveAttempted_ = false;
    bool shapeFailure_ = false, captured_ = false;
    const OriginalDeadline* originalDeadline_=nullptr;
    bool clockClean_=true;
    std::array<IngressNativeClock,4> nativeClocks_{}; // type, peer, passcred, recvmsg
public:
    OriginalPrivateIngress() = default;
    OriginalPrivateIngress(const OriginalPrivateIngress&) = delete;
    OriginalPrivateIngress& operator=(const OriginalPrivateIngress&) = delete;
    OriginalPrivateIngress(OriginalPrivateIngress&&) = delete;
    OriginalPrivateIngress& operator=(OriginalPrivateIngress&&) = delete;
    // Caller transfers this exact socket capability. No path, numeric peer PID
    // or arbitrary manifest authenticates the independently supplied authority.
    bool CaptureTransferredSocket(int originalSocket,const OriginalDeadline* originalDeadline=nullptr,
                                  bool independentlyEligibleFailureCapture=false) noexcept {
        if (attempted_ || originalSocket < 0) return false;
        attempted_ = true;
        originalDeadline_=originalDeadline;
        socket_.original = originalSocket; // Attach before native observation.
        socket_.Observe(originalDeadline_,independentlyEligibleFailureCapture);
        clockClean_ &= socket_.statClock.clean;
        if(!socket_.statAttempted)return false;
        if(!nativeClocks_[0].Before(originalDeadline_,"ingress-socket-type",independentlyEligibleFailureCapture)){clockClean_=false;return false;}
        typeResult_ = getsockopt(originalSocket, SOL_SOCKET, SO_TYPE, &socketType_, &typeBytes_);
        if (typeResult_ < 0) typeErrno_ = errno;
        nativeClocks_[0].After(originalDeadline_,"ingress-socket-type");clockClean_ &=nativeClocks_[0].clean;
        if(!nativeClocks_[1].Before(originalDeadline_,"ingress-peer-credentials",independentlyEligibleFailureCapture)){clockClean_=false;return false;}
        peerResult_ = getsockopt(originalSocket, SOL_SOCKET, SO_PEERCRED, &peer_, &peerBytes_);
        if (peerResult_ < 0) peerErrno_ = errno;
        nativeClocks_[1].After(originalDeadline_,"ingress-peer-credentials");clockClean_ &=nativeClocks_[1].clean;
        if(!nativeClocks_[2].Before(originalDeadline_,"ingress-pass-credentials",independentlyEligibleFailureCapture)){clockClean_=false;return false;}
        passResult_ = getsockopt(originalSocket, SOL_SOCKET, SO_PASSCRED, &passCredentials_, &passBytes_);
        if (passResult_ < 0) passErrno_ = errno;
        nativeClocks_[2].After(originalDeadline_,"ingress-pass-credentials");clockClean_ &=nativeClocks_[2].clean;
        if (socket_.statResult < 0 || typeResult_ < 0 || passResult_ < 0) return false;
        if (!S_ISSOCK(socket_.status.st_mode) || socketType_ != SOCK_SEQPACKET ||
                typeBytes_ != sizeof(socketType_) || passBytes_ != sizeof(passCredentials_) || !passCredentials_) {
            shapeFailure_ = true; return false;
        }
        vector_.iov_base = body_.data(); vector_.iov_len = body_.size();
        message_.msg_iov = &vector_; message_.msg_iovlen = 1;
        message_.msg_control = control_.data(); message_.msg_controllen = control_.size();
        if(!nativeClocks_[3].Before(originalDeadline_,"ingress-recvmsg",independentlyEligibleFailureCapture)){clockClean_=false;return false;}
        receiveAttempted_ = true;
        receiveResult_ = recvmsg(originalSocket, &message_, MSG_CMSG_CLOEXEC | MSG_DONTWAIT);
        if(receiveResult_<0)receiveErrno_=errno;
        nativeClocks_[3].After(originalDeadline_,"ingress-recvmsg");clockClean_ &=nativeClocks_[3].clean;
        if (receiveResult_ < 0) return false;
        // First retain every descriptor actually delivered in the bounded raw
        // control message, even when body/control truncation later denies intake.
        for (cmsghdr* header = CMSG_FIRSTHDR(&message_); header;
                header = CMSG_NXTHDR(&message_, header)) {
            const auto start = reinterpret_cast<unsigned char*>(header);
            const auto end = control_.data() + message_.msg_controllen;
            if (start < control_.data() || start > end ||
                    static_cast<size_t>(end - start) < sizeof(cmsghdr) ||
                    header->cmsg_len < CMSG_LEN(0) || header->cmsg_len > static_cast<size_t>(end - start)) {
                shapeFailure_ = true; break;
            }
            if (header->cmsg_level == SOL_SOCKET && header->cmsg_type == SCM_CREDENTIALS) {
                if (header->cmsg_len != CMSG_LEN(sizeof(ucred)) || messageCredentialsPresent_) {
                    shapeFailure_ = true; continue;
                }
                std::memcpy(&messageCredentials_, CMSG_DATA(header), sizeof(messageCredentials_));
                messageCredentialsPresent_ = true;
                continue;
            }
            if (header->cmsg_level != SOL_SOCKET || header->cmsg_type != SCM_RIGHTS) {
                shapeFailure_ = true; continue;
            }
            const size_t bytes = header->cmsg_len - CMSG_LEN(0);
            if (bytes % sizeof(int)) shapeFailure_ = true;
            const auto originals = reinterpret_cast<const int*>(CMSG_DATA(header));
            for (size_t index = 0; index < bytes / sizeof(int); ++index) {
                if (transferredCount_ == transferred_.size()) { shapeFailure_ = true; break; }
                auto& cell = transferred_[transferredCount_++];
                cell.original = originals[index]; // Preowned before fstat.
                // Delivered originals are already owned. Preserve independent
                // post-receive observations even when that receive saw expiry.
                cell.Observe(originalDeadline_,true);clockClean_ &=cell.statClock.clean;
                if (cell.original < 0 || cell.statResult < 0) shapeFailure_ = true;
            }
        }
        if (message_.msg_flags & (MSG_TRUNC | MSG_CTRUNC)) shapeFailure_ = true;
        if (receiveResult_ <= 0 || static_cast<size_t>(receiveResult_) > body_.size()) shapeFailure_ = true;
        if (!messageCredentialsPresent_) shapeFailure_ = true;
        captured_ = !shapeFailure_;
        return captured_&&(independentlyEligibleFailureCapture||clockClean_);
    }
    // Only after the source-bound caller proves transfer/custody eligibility.
    // Descriptor close errors never prevent other independent eligible closes.
    bool DisposeEligible() noexcept {
        bool all=true;
        for (size_t index = 0; index < transferredCount_; ++index) {
            auto& fd=transferred_[index];fd.CloseOnce(originalDeadline_);
            clockClean_ &=fd.closeClock.clean;all &=fd.original<0||fd.closed;
        }
        socket_.CloseOnce(originalDeadline_);clockClean_ &=socket_.closeClock.clean;
        return all&&(socket_.original<0||socket_.closed); // Physical-only.
    }
    const IngressFd& OriginalSocket() const noexcept { return socket_; }
    const std::array<IngressFd, FdCapacity>& OriginalDescriptors() const noexcept { return transferred_; }
    size_t OriginalDescriptorCount() const noexcept { return transferredCount_; }
    const std::array<unsigned char, BodyCapacity>& OriginalBody() const noexcept { return body_; }
    const auto& OriginalControlBytes() const noexcept { return control_; }
    const msghdr& OriginalMessageOutput() const noexcept { return message_; }
    ssize_t OriginalReceiveResult() const noexcept { return receiveResult_; }
    int OriginalReceiveErrno() const noexcept { return receiveErrno_; }
    bool ReceiveAttempted() const noexcept { return receiveAttempted_; }
    const ucred& OriginalPeerCredentials() const noexcept { return peer_; }
    int OriginalPeerResult() const noexcept { return peerResult_; }
    int OriginalPeerErrno() const noexcept { return peerErrno_; }
    socklen_t OriginalPeerBytes() const noexcept { return peerBytes_; }
    int OriginalSocketType() const noexcept { return socketType_; }
    int OriginalTypeResult() const noexcept { return typeResult_; }
    int OriginalTypeErrno() const noexcept { return typeErrno_; }
    socklen_t OriginalTypeBytes() const noexcept { return typeBytes_; }
    const ucred& OriginalMessageCredentials() const noexcept { return messageCredentials_; }
    bool MessageCredentialsPresent() const noexcept { return messageCredentialsPresent_; }
    int OriginalPassCredentialResult() const noexcept { return passResult_; }
    int OriginalPassCredentialErrno() const noexcept { return passErrno_; }
    int OriginalPassCredentialValue() const noexcept { return passCredentials_; }
    socklen_t OriginalPassCredentialBytes() const noexcept { return passBytes_; }
    bool Captured() const noexcept { return captured_; }
    bool ShapeFailure() const noexcept { return shapeFailure_; }
    bool NativeClocksClean() const noexcept{return clockClean_;}
    const auto& OriginalNativeClocks() const noexcept{return nativeClocks_;}
    const OriginalDeadline* OriginalDeadlineOwner() const noexcept{return originalDeadline_;}
};
// Exact role assignment, caller native lifetime, file/root/tools identity and
// selected capacity association remain separate concrete gate obligations.
// SO_PEERCRED is a raw kernel observation, never a signed issuer substitute.
}

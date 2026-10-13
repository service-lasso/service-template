// UNCOMPILED source-owned private channel, not an external issuer/service.
// Only fresh socketpair endpoints are configured. No ambient descriptor flags,
// masks, processes, tools or installed filesystem objects are changed.
#pragma once
#include "linux_deadline_supervisor.hpp"
#include "linux_ingress.hpp"
namespace template_creator_linux {
class OriginalLinuxHostChannel {
    OriginalDeadline& deadline_;
    std::array<int,2> nativePair_{{-1,-1}};
    std::array<IngressFd,2> sockets_{};
    IngressFd originalSelfPidfd_{};
    SupervisorNativeOriginal pair_,selfPid_,selfPidfd_,send_;
    std::array<SupervisorNativeOriginal,2> pass_{};
    std::array<SupervisorNativeOriginal,3> closes_{};
    int originalPassValue_=1;
    std::array<unsigned char,OriginalPrivateIngress::BodyCapacity> selectedBody_{};
    size_t bodyBytes_=0,descriptorCount_=0;
    std::array<int,OriginalPrivateIngress::FdCapacity> selectedDescriptors_{};
    alignas(cmsghdr) std::array<unsigned char,CMSG_SPACE(sizeof(int)*OriginalPrivateIngress::FdCapacity)> sendControl_{};
    iovec sendVector_{};msghdr sendMessage_{};
    bool constructedAttempted_=false,constructed_=false,sendAttempted_=false,
        sendComplete_=false,pairOutputUnknown_=false,selfPidfdOutputUnknown_=false,creatorTransferred_=false,callerTransferred_=false,failed_=false;
    bool Before(SupervisorNativeOriginal& row,const char* op) noexcept {
        const bool ok=CaptureSupervisorClock(row.beforeClock,deadline_,op,false);
        if(!ok){failed_=true;return false;}row.attempted=true;return true;
    }
    void Returned(SupervisorNativeOriginal& row,long value) noexcept {
        row.returned=value;if(value<0)row.error=errno;row.returnedObserved=true;
        if(!CaptureSupervisorClock(row.afterClock,deadline_,row.beforeClock.operation,true)||
           row.afterClock.nanoseconds<row.beforeClock.nanoseconds)failed_=true;
    }
    void CloseOriginal(IngressFd& fd,SupervisorNativeOriginal& row) noexcept {
        if(fd.original<0||fd.closeAttempted)return;
        // Expiry does not suppress a source-owned independently eligible close.
        CaptureSupervisorClock(row.beforeClock,deadline_,"host-private-once-close",false);
        row.attempted=true;fd.closeAttempted=true;
        fd.closeResult=close(fd.original);if(fd.closeResult<0)fd.closeErrno=errno;
        row.returned=fd.closeResult;row.error=fd.closeErrno;row.returnedObserved=true;
        fd.closed=fd.closeResult==0;
        if(!CaptureSupervisorClock(row.afterClock,deadline_,"host-private-once-close",true)||
           !row.beforeClock.shapeValid||row.beforeClock.expired||
           row.afterClock.nanoseconds<row.beforeClock.nanoseconds||!fd.closed)failed_=true;
    }
    ~OriginalLinuxHostChannel()=default; // Original native host retains lifetime.
public:
    explicit OriginalLinuxHostChannel(OriginalDeadline& originalDeadline):deadline_(originalDeadline) {}
    OriginalLinuxHostChannel(const OriginalLinuxHostChannel&)=delete;
    OriginalLinuxHostChannel& operator=(const OriginalLinuxHostChannel&)=delete;
    bool ConstructOriginal() noexcept {
        if(constructedAttempted_)return false;constructedAttempted_=true;
        if(!Before(pair_,"host-private-socketpair"))return false;
        Returned(pair_,socketpair(AF_UNIX,SOCK_SEQPACKET|SOCK_NONBLOCK|SOCK_CLOEXEC,0,nativePair_.data()));
        // Preserve every raw output, including failed-call unknown outputs.
        for(size_t i=0;i<2;++i)sockets_[i].original=nativePair_[i];
        if(pair_.returned!=0){pairOutputUnknown_=nativePair_[0]>=0||nativePair_[1]>=0;failed_=true;return false;}
        if(nativePair_[0]<0||nativePair_[1]<0||nativePair_[0]==nativePair_[1]){pairOutputUnknown_=true;failed_=true;return false;}
        for(size_t i=0;i<2;++i){
            if(!Before(pass_[i],"host-private-passcred"))return false;
            Returned(pass_[i],setsockopt(sockets_[i].original,SOL_SOCKET,SO_PASSCRED,&originalPassValue_,sizeof(originalPassValue_)));
            if(pass_[i].returned!=0)return false;
        }
        if(!Before(selfPid_,"host-original-self-getpid"))return false;
        Returned(selfPid_,syscall(SYS_getpid));
        if(selfPid_.returned<=0||static_cast<unsigned long>(selfPid_.returned)>
           static_cast<unsigned long>(std::numeric_limits<pid_t>::max())){failed_=true;return false;}
        // Executing original caller cannot be PID-recycled during this self
        // acquisition. This is not reopening any other worker by observed PID.
        if(!Before(selfPidfd_,"host-original-self-pidfd"))return false;
        Returned(selfPidfd_,syscall(SYS_pidfd_open,static_cast<pid_t>(selfPid_.returned),0));
        if(selfPidfd_.returned<0){failed_=true;return false;}
        if(static_cast<unsigned long>(selfPidfd_.returned)>
           static_cast<unsigned long>(std::numeric_limits<int>::max())){selfPidfdOutputUnknown_=true;failed_=true;return false;}
        originalSelfPidfd_.original=static_cast<int>(selfPidfd_.returned);
        constructed_=!failed_;return constructed_;
    }
    // Copies complete selected body/capability roster before send; descriptor
    // values are actual transferred originals, not authority claims. Caller
    // source/role/root/tool admission is separately joined by the native host.
    bool PublishOriginal(const unsigned char* body,size_t bytes,const int* descriptors,size_t count) noexcept {
        if(sendAttempted_||!constructed_||failed_)return false;sendAttempted_=true;
        if(!body||!bytes||bytes>selectedBody_.size()||count>=selectedDescriptors_.size()||
           (count&&!descriptors)){failed_=true;return false;}
        bodyBytes_=bytes;std::memcpy(selectedBody_.data(),body,bytes);
        selectedDescriptors_[0]=originalSelfPidfd_.original;descriptorCount_=count+1;
        for(size_t i=0;i<count;++i){
            selectedDescriptors_[i+1]=descriptors[i];
            if(descriptors[i]<0){failed_=true;return false;}
        }
        sendVector_.iov_base=selectedBody_.data();sendVector_.iov_len=bodyBytes_;
        sendMessage_.msg_iov=&sendVector_;sendMessage_.msg_iovlen=1;
        sendMessage_.msg_control=sendControl_.data();
        sendMessage_.msg_controllen=CMSG_SPACE(sizeof(int)*descriptorCount_);
        auto* header=CMSG_FIRSTHDR(&sendMessage_);header->cmsg_level=SOL_SOCKET;header->cmsg_type=SCM_RIGHTS;
        header->cmsg_len=CMSG_LEN(sizeof(int)*descriptorCount_);
        std::memcpy(CMSG_DATA(header),selectedDescriptors_.data(),sizeof(int)*descriptorCount_);
        if(!Before(send_,"host-private-original-send"))return false;
        send_.requested=bodyBytes_;
        Returned(send_,sendmsg(sockets_[0].original,&sendMessage_,MSG_DONTWAIT|MSG_NOSIGNAL));
        sendComplete_=send_.returned==static_cast<long>(bodyBytes_);
        if(!sendComplete_)failed_=true;return sendComplete_&&!failed_;
    }
    // Nonallocating transfer partitions endpoint ownership BEFORE ingress can
    // perform effects/fail. The source host calls CaptureOriginal on this SAME
    // returned capability exactly once; it never closes it from this owner.
    int TransferOriginalCreatorEndpoint() noexcept {
        if(!sendComplete_||creatorTransferred_||pairOutputUnknown_)return -1;
        creatorTransferred_=true;return sockets_[1].original;
    }
    const IngressFd& OriginalCallerEndpoint() const noexcept{return sockets_[0];}
    const SupervisorNativeOriginal& OriginalSend() const noexcept{return send_;}
    const SupervisorNativeOriginal& OriginalSelfPidfdAcquisition() const noexcept{return selfPidfd_;}
private:
    int TransferOriginalCallerEndpoint() noexcept {
        if(!constructed_||callerTransferred_||pairOutputUnknown_)return -1;
        callerTransferred_=true;return sockets_[0].original;
    }
    bool DisposeUntransferredEligible() noexcept {
        // Actual caller receiver must finish before caller endpoint close. Full
        // host invokes this only after its independent caller custody ends.
        if(pairOutputUnknown_)return false;
        if(!callerTransferred_)CloseOriginal(sockets_[0],closes_[0]);
        if(!creatorTransferred_)CloseOriginal(sockets_[1],closes_[1]);
        CloseOriginal(originalSelfPidfd_,closes_[2]);
        return(callerTransferred_||sockets_[0].original<0||sockets_[0].closed)&&
            (creatorTransferred_||sockets_[1].original<0||sockets_[1].closed)&&
            !selfPidfdOutputUnknown_&&(originalSelfPidfd_.original<0||originalSelfPidfd_.closed);
    }
public:
    size_t ReservedSourceBytes() const noexcept{return sizeof(*this);}
    friend class OriginalLinuxPlatformState;
    friend class OriginalLinuxCallerReceipt;
    friend class OriginalLinuxPendingHost;
};
}

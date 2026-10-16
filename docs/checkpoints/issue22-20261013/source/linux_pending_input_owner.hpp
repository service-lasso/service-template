// UNCOMPILED connected ownership for pending private-input native calls.
// This owner does not grant launch roles or replace the full creator platform.
#pragma once
#include "linux_deadline_receiver.hpp"
#include "linux_memory.hpp"
namespace template_creator_bridge { struct NativePlatformState; }
namespace template_creator_linux {
struct PendingInputGeometry {
    size_t clockRows=0,taskObservations=0,statBytes=0,statusBytes=0,procReads=0,
        fdinfoBytes=0,supervisorStack=0,supervisorJoins=0,
        receiverStack=0,receiverWaits=0,receiverJoins=0,
        mapsBytes=0,mapsReads=0,memoryRegions=0,memoryBytes=0,memoryReads=0;
};
class OriginalLinuxPendingInputOwner {
    static size_t CheckedClockRows(const PendingInputGeometry& g) {
        // Validate the whole selected root-memory geometry before the first
        // pending allocation or subsequent observer/native setup, not at launch.
        (void)OriginalLinuxMappedMemory::CheckedGeometrySourceBacking(
            g.mapsBytes,g.mapsReads,g.memoryRegions,g.memoryBytes,g.memoryReads);
        return g.clockRows;
    }
    PendingInputGeometry selectedGeometry_;
    bool preownAttempted_=false,preowned_=false;
    std::exception_ptr preownException_;
    OriginalDeadline deadline_;
    OriginalPrivateIngress ingress_;
    OriginalProcRoot procRoot_;
    HeldTaskIdentity creatorTask_;
    OriginalPidfdHandoffAssociation caller_;
    OriginalDeadlineSupervisor supervisor_;
    OriginalDeadlineReceiver receiver_;
    SupervisorNativeOriginal creatorTid_;
    pid_t originalCreatorTid_=0;
    bool captureAttempted_=false,captured_=false,startAttempted_=false,
        started_=false,failed_=false,observersDisposed_=false,
        auxiliaryDisposalAttempted_=false,auxiliaryDisposed_=false;
    ~OriginalLinuxPendingInputOwner()=default; // Native host retains through joins.
public:
    // All dynamic observation/thread storage is constructed before any capture.
    // Absolute origin comes from the actual selected operation, never this ctor.
    OriginalLinuxPendingInputOwner(DeferredOriginalPreownTag,const PendingInputGeometry& g,bool hasDeadline,uint64_t absoluteNs) noexcept
        :selectedGeometry_(g),deadline_(DeferredOriginalPreown,g.clockRows,hasDeadline,absoluteNs),
         creatorTask_(DeferredOriginalPreown,g.taskObservations,g.statBytes,g.statusBytes,g.procReads),
         caller_(DeferredOriginalPreown,ingress_,creatorTask_,g.taskObservations,g.statBytes,g.statusBytes,g.procReads,g.fdinfoBytes),
         supervisor_(DeferredOriginalPreown,deadline_,g.supervisorStack,g.supervisorJoins),
         receiver_(DeferredOriginalPreown,supervisor_,ingress_,caller_,g.receiverStack,g.receiverWaits,g.receiverJoins) {}
    OriginalLinuxPendingInputOwner(const PendingInputGeometry& g,bool hasDeadline,uint64_t absoluteNs)
        :OriginalLinuxPendingInputOwner(DeferredOriginalPreown,g,hasDeadline,absoluteNs){
        if(!PreownOriginal()&&preownException_)std::rethrow_exception(preownException_);
    }
    bool PreownOriginal() noexcept {
        if(preownAttempted_)return false;preownAttempted_=true;
        try{(void)CheckedClockRows(selectedGeometry_);}catch(...){preownException_=std::current_exception();failed_=true;return false;}
        if(!deadline_.PreownOriginal()){preownException_=deadline_.PreownExceptionOriginal();failed_=true;return false;}
        if(!creatorTask_.PreownOriginal()){preownException_=creatorTask_.PreownExceptionOriginal();failed_=true;return false;}
        if(!caller_.PreownOriginal()){preownException_=caller_.PreownExceptionOriginal();failed_=true;return false;}
        if(!supervisor_.PreownOriginal()){preownException_=supervisor_.PreownExceptionOriginal();failed_=true;return false;}
        if(!receiver_.PreownOriginal()){preownException_=receiver_.PreownExceptionOriginal();failed_=true;return false;}
        preowned_=true;return true;
    }
    bool OutputBackingSeparateOriginal(const void* output,size_t bytes) const noexcept {
        return OriginalBackingStorageSeparate(output,bytes,this,1,sizeof(*this))&&
            deadline_.OutputBackingSeparateOriginal(output,bytes)&&creatorTask_.OutputBackingSeparateOriginal(output,bytes)&&
            caller_.OutputBackingSeparateOriginal(output,bytes)&&supervisor_.OutputBackingSeparateOriginal(output,bytes)&&
            receiver_.OutputBackingSeparateOriginal(output,bytes);
    }
    bool PreownedOriginal() const noexcept{return preowned_;}
    const std::exception_ptr& PreownExceptionOriginal() const noexcept{return preownException_;}
    OriginalLinuxPendingInputOwner(const OriginalLinuxPendingInputOwner&)=delete;
    OriginalLinuxPendingInputOwner& operator=(const OriginalLinuxPendingInputOwner&)=delete;
    bool CaptureOriginal(int transferredSocket,int originalProcRoot,size_t transferredCallerPidfdOrdinal) noexcept {
        if(!preowned_||captureAttempted_)return false;captureAttempted_=true;
        // Attach actual transferred socket and all delivered FDs before the
        // later caller/proc association can fail. No path/PID reopening fallback.
        const bool ingressCaptured=ingress_.CaptureTransferredSocket(transferredSocket,&deadline_);
        if(!ingressCaptured){failed_=true;return false;}
        if(!procRoot_.Capture(originalProcRoot,deadline_)){failed_=true;return false;}
        const bool before=CaptureSupervisorClock(creatorTid_.beforeClock,deadline_,"pending-input-creator-gettid",false);
        if(!before){failed_=true;return false;}
        creatorTid_.attempted=true;creatorTid_.returned=syscall(SYS_gettid);
        if(creatorTid_.returned<0)creatorTid_.error=errno;
        creatorTid_.returnedObserved=true;
        const bool after=CaptureSupervisorClock(creatorTid_.afterClock,deadline_,"pending-input-creator-gettid",true);
        if(!after||creatorTid_.returned<=0||
           static_cast<unsigned long>(creatorTid_.returned)>static_cast<unsigned long>(std::numeric_limits<pid_t>::max())||
           creatorTid_.afterClock.nanoseconds<creatorTid_.beforeClock.nanoseconds){failed_=true;return false;}
        originalCreatorTid_=static_cast<pid_t>(creatorTid_.returned);
        if(!creatorTask_.BindFromOriginalProcRoot(procRoot_,originalCreatorTid_)||
           !caller_.Capture(transferredCallerPidfdOrdinal,originalCreatorTid_,deadline_)) {failed_=true;return false;}
        captured_=true;return true;
    }
    bool StartOriginalObservers() noexcept {
        if(startAttempted_||!captured_||failed_)return false;startAttempted_=true;
        const bool supervisorStarted=supervisor_.Start();
        // Even a failed supervisor start has retained native acquisition state;
        // receiver setup/cleanup remains its own original route.
        const bool receiverStarted=receiver_.Start();
        started_=supervisorStarted&&receiverStarted;
        if(!started_)failed_=true;return started_;
    }
    bool StopOriginalObserversEligible() noexcept {
        // MUST request supervisor stop first. A normal receiver waits on that
        // worker's publication; joining it first would prevent normal closure.
        // Pending ioctl/pread prevents stop/disposal, while expiry publication
        // continues independently through receiver->original caller socket.
        const bool firstSupervisor=supervisor_.StopJoinDisposeEligible();
        const bool receiverDisposed=receiver_.ObserveEligibleJoinAndDisposal();
        const bool finalSupervisor=supervisor_.StopJoinDisposeEligible();
        observersDisposed_=receiverDisposed&&finalSupervisor;
        if(supervisor_.ExpiredOrFailed()||receiver_.Failed())failed_=true;
        (void)firstSupervisor; // Physical result is retained in native owners.
        return observersDisposed_;
    }
    bool DisposeAuxiliaryOriginalsBeforeObserverStop() noexcept {
        if(auxiliaryDisposalAttempted_)return auxiliaryDisposed_;
        if(supervisor_.HasPendingOriginalNativeCall())return false;
        auxiliaryDisposalAttempted_=true;
        // Concrete platform calls only after command/gate borrowers settle.
        // Receiver Run uses retained association metadata and live ingress
        // socket/pidfd, not these completed proc/task acquisition descriptors.
        // Keep actual ingress originals until receiver/caller joins separately.
        const bool callerClosed=caller_.DisposeEligible(&supervisor_);
        const bool creatorClosed=creatorTask_.DisposeEligible(&supervisor_);
        const bool rootClosed=procRoot_.DisposeEligible(&supervisor_);
        auxiliaryDisposed_=callerClosed&&creatorClosed&&rootClosed;
        if(!auxiliaryDisposed_)failed_=true;
        return auxiliaryDisposed_;
    }
    bool AuxiliaryOriginalsDisposed() const noexcept{return auxiliaryDisposed_;}
    OriginalDeadline& OriginalDeadlineOwner() noexcept{return deadline_;}
    const OriginalDeadline& OriginalDeadlineOwner() const noexcept{return deadline_;}
    OriginalDeadlineSupervisor& OriginalSupervisorOwner() noexcept{return supervisor_;}
    const OriginalDeadlineSupervisor& OriginalSupervisorOwner() const noexcept{return supervisor_;}
    pid_t OriginalCreatorTid() const noexcept{return originalCreatorTid_;}
    HeldTaskIdentity& OriginalCreatorTaskOwner() noexcept{return creatorTask_;}
    OriginalPrivateIngress& OriginalIngressOwner() noexcept{return ingress_;}
    OriginalProcRoot& OriginalProcRootOwner() noexcept{return procRoot_;}
    const OriginalDeadlineReceiver& OriginalReceiverOwner() const noexcept{return receiver_;}
    OriginalDeadlineReceiver& OriginalReceiverOwner() noexcept{return receiver_;}
    bool ObserversPhysicalDisposed() const noexcept{return observersDisposed_;}
    bool Failed() const noexcept{return failed_||supervisor_.ExpiredOrFailed()||receiver_.Failed();}
    bool ReservedSourceBytes(size_t& bytes) const noexcept {
        bytes=sizeof(*this);size_t n=0;
        auto add=[&](size_t whole,size_t embedded){if(whole<embedded||whole-embedded>SIZE_MAX-bytes)return false;bytes+=whole-embedded;return true;};
        n=deadline_.ReservedSourceBytes();if(!add(n,sizeof(deadline_)))return false;
        if(!creatorTask_.ReservedSourceBytes(n)||!add(n,sizeof(creatorTask_)))return false;
        if(!caller_.ReservedSourceBytes(n)||!add(n,sizeof(caller_)))return false;
        if(!supervisor_.ReservedSourceBytes(n)||!add(n,sizeof(supervisor_)))return false;
        if(!receiver_.ReservedSourceBytes(n)||!add(n,sizeof(receiver_)))return false;
        return true; // Native kernel/TLS/allocator/runtime and full creator extra.
    }
    friend class OriginalLinuxPlatformState;
    friend class OriginalLinuxPendingHost;
};
// Ingress descriptors/proc/task originals remain resident here. Observer join
// alone does NOT make transferred inputs eligible for close while commands may
// still consume them. Full native host performs that later ownership join.
}

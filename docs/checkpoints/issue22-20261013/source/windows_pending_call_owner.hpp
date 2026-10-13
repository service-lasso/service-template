// UNCOMPILED shared custody for the original duplex host channel. Row storage
// is selected/preowned before effects; this object is not deadline supervision
// or a caller receipt. Native notice/worker bodies must attach here before use.
#pragma once
#include <windows.h>
#include <atomic>
#include <cstdint>
#include <memory>
#include <stdexcept>
#include <cstring>
namespace template_creator_bridge {
struct OriginalWindowsObserverGeometry {
    SIZE_T threads=0,invocations=0,pointers=0,stringBytes=0,readRows=0;
    bool Complete() const noexcept{return threads && invocations && stringBytes && readRows;}
};
enum class OriginalWindowsPendingResultShape { Unreturned, Boolean, Handle, BooleanWithoutCount, NtQueryStatus };
struct OriginalWindowsPendingCall {
    const void* operationOwner=nullptr;
    const void* commandOriginal=nullptr;
    const void* originalInput=nullptr;
    SIZE_T originalInputExtent=0;
    void* originalDestination=nullptr;
    SIZE_T requested=0, actual=0;
    OriginalWindowsPendingResultShape resultShape=OriginalWindowsPendingResultShape::Unreturned;
    HANDLE handleReturned=nullptr,handleFailureSentinel=nullptr;
    bool handleOutputUnknown=false;
    BOOL returned=FALSE;
    DWORD error=0;
    LONG nativeStatusReturned=0;
    bool attempted=false, completionUnknown=false, actualObserved=false;
    std::atomic<bool> activePublished{false};
    std::atomic<bool> resultPublished{false};
};
class OriginalWindowsPendingCalls;
class OriginalWindowsNoticeOwner;
enum class OriginalWindowsWorkerScope {Command,Host};
struct OriginalWindowsWorkerIoOriginal {
    HANDLE endpoint=nullptr;const void* commandOriginal=nullptr;
    unsigned char* bytes=nullptr;DWORD requested=0,actual=0;
    BOOL returned=FALSE;DWORD error=0;
    bool write=false,attempted=false,completionUnknown=false;
    std::atomic<bool> activePublished{false},resultPublished{false};
};
// Closed original type. Only the concrete source notice owner can construct or
// operate it; all kernel-borrowed payloads AND output cells are resident here.
class OriginalWindowsWorkerIoOwner final {
    friend class OriginalWindowsPendingCalls;
    friend class OriginalWindowsNoticeOwner;
    friend struct OriginalWindowsStandaloneHost;
    OriginalWindowsPendingCalls& pending_;
    const void* const operationOwner_;
    const SIZE_T slot_,capacity_,frameBytes_;
    HANDLE endpoint_;
    const LPTHREAD_START_ROUTINE body_;
    void* const input_;
    const SIZE_T inputExtent_;
    std::unique_ptr<OriginalWindowsWorkerIoOriginal[]> rows_;
    std::unique_ptr<unsigned char[]> bytes_;
    SIZE_T count_=0;
    std::atomic<bool> bodyReturned_{false};
    OriginalWindowsWorkerIoOwner(OriginalWindowsPendingCalls& pending,const void* owner,SIZE_T slot,
        HANDLE endpoint,LPTHREAD_START_ROUTINE body,void* input,SIZE_T inputExtent,SIZE_T rows,SIZE_T frameBytes)
        :pending_(pending),operationOwner_(owner),slot_(slot),capacity_(rows),frameBytes_(frameBytes),
         endpoint_(endpoint),body_(body),input_(input),inputExtent_(inputExtent) {
        if(!owner || slot>=2 || endpoint==INVALID_HANDLE_VALUE || !body || !input || !inputExtent ||
            !rows || !frameBytes || frameBytes>MAXDWORD || frameBytes>SIZE_MAX-sizeof(OriginalWindowsWorkerIoOriginal) ||
            rows>(SIZE_MAX-sizeof(*this))/(sizeof(OriginalWindowsWorkerIoOriginal)+frameBytes))
            throw std::invalid_argument("complete original worker IO backing");
        rows_=std::make_unique<OriginalWindowsWorkerIoOriginal[]>(rows);
        bytes_=std::make_unique<unsigned char[]>(rows*frameBytes);
    }
    bool BindOriginalEndpointBeforeBirth(HANDLE endpoint) noexcept {
        if(endpoint_ || !endpoint || endpoint==INVALID_HANDLE_VALUE || count_ || bodyReturned_.load(std::memory_order_acquire))return false;
        endpoint_=endpoint;return true;
    }
    bool CallOriginal(bool write,const void* originalBytes,DWORD requested,const OriginalWindowsPendingCall* originalCall) noexcept;
    void PublishBodyReturnedOriginal() noexcept {bodyReturned_.store(true,std::memory_order_release);}
    bool NativeBorrowersSettledAfterTermination() const noexcept {
        if(!bodyReturned_.load(std::memory_order_acquire))return false;
        for(SIZE_T i=0;i<count_;++i) {
            const auto& row=rows_[i];
            if(!row.activePublished.load(std::memory_order_acquire) ||
                !row.resultPublished.load(std::memory_order_acquire) || row.completionUnknown)return false;
        }
        return true;
    }
public:
    OriginalWindowsWorkerIoOwner(const OriginalWindowsWorkerIoOwner&)=delete;
    OriginalWindowsWorkerIoOwner& operator=(const OriginalWindowsWorkerIoOwner&)=delete;
    SIZE_T ReservedSourceBytes() const noexcept {return sizeof(*this)+capacity_*(sizeof(OriginalWindowsWorkerIoOriginal)+frameBytes_);}
};
struct OriginalWindowsChannelWorker {
    const void* operationOwner=nullptr;
    const void* commandOriginal=nullptr;
    const void* inputOriginal=nullptr;
    SIZE_T inputExtent=0,stackReservation=0;
    HANDLE endpointOriginal=nullptr,threadOriginal=nullptr;
    LPTHREAD_START_ROUTINE bodyOriginal=nullptr;
    LPSECURITY_ATTRIBUTES creationSecurityOriginal=nullptr;
    DWORD creationFlagsOriginal=0;
    DWORD threadId=0,createError=0;
    OriginalWindowsWorkerScope scope=OriginalWindowsWorkerScope::Command;
    OriginalWindowsWorkerIoOwner* ioOriginal=nullptr;
    std::atomic<bool> enrollmentPublished{false};
    std::atomic<bool> creationPublished{false};
    bool nativeTerminationObserved=false;
    bool reserved=false,createAttempted=false,createReturned=false,creationUnknown=false,joined=false,joinUnresolved=false;
};
struct OriginalWindowsWorkerJoinAttempt {
    const OriginalWindowsChannelWorker* workerOriginal=nullptr;
    DWORD waitReturned=WAIT_FAILED,waitError=0,exitCode=0,exitCodeError=0,closeError=0;
    BOOL exitCodeReturned=FALSE,closeReturned=FALSE;
    bool waitAttempted=false,exitCodeAttempted=false,closeAttempted=false;
};
class OriginalWindowsPendingCalls {
    friend class OriginalWindowsWorkerIoOwner;
    friend class OriginalWindowsNoticeOwner;
    friend struct OriginalWindowsStandaloneHost;
    const OriginalWindowsObserverGeometry observerGeometry_;
    std::unique_ptr<OriginalWindowsPendingCall[]> rows_;
    OriginalWindowsChannelWorker workers_[2]{};
    const OriginalWindowsNoticeOwner* noticeOriginal_=nullptr;
    const void* noticeIngressOriginal_=nullptr;
    const void* noticeRouteOriginal_=nullptr;
    LPTHREAD_START_ROUTINE noticeBodiesOriginal_[2]{};
    OriginalWindowsWorkerIoOwner* noticeIoOriginal_[2]{};
    HANDLE noticeEndpointsOriginal_[2]{};
    SIZE_T noticeInputExtentOriginal_=0,noticeStacksOriginal_[2]{};
    std::atomic<bool> noticeBindingPublished_{false};
    std::unique_ptr<OriginalWindowsWorkerJoinAttempt[]> joins_;
    SIZE_T joinCount_=0;
    SIZE_T capacity_=0, count_=0;
    static constexpr SIZE_T ClosedBit=SIZE_T(1)<<(sizeof(SIZE_T)*8-1);
    static constexpr SIZE_T ReservationMask=ClosedBit-1;
    // Admission and freeze have ONE linearization order. A closer cannot see
    // closed-zero between an accepted caller's check and its reservation.
    std::atomic<SIZE_T> reservations_{0};
    bool ReserveOriginal() noexcept {
        auto observed=reservations_.load(std::memory_order_acquire);
        for(;;){
            if((observed&ClosedBit) || (observed&ReservationMask)==ReservationMask)return false;
            if(reservations_.compare_exchange_weak(observed,observed+1,
                    std::memory_order_acq_rel,std::memory_order_acquire))return true;
        }
    }
public:
    explicit OriginalWindowsPendingCalls(SIZE_T originalCallRows,OriginalWindowsObserverGeometry observer={})
        :observerGeometry_(observer),capacity_(originalCallRows) {
        if(!capacity_ || capacity_>ReservationMask ||
                capacity_>(SIZE_MAX-sizeof(*this))/(sizeof(OriginalWindowsPendingCall)+sizeof(OriginalWindowsWorkerJoinAttempt)))
            throw std::invalid_argument("original pending call geometry");
        rows_=std::make_unique<OriginalWindowsPendingCall[]>(capacity_);
        joins_=std::make_unique<OriginalWindowsWorkerJoinAttempt[]>(capacity_);
    }
    OriginalWindowsPendingCalls(const OriginalWindowsPendingCalls&)=delete;
    OriginalWindowsPendingCalls& operator=(const OriginalWindowsPendingCalls&)=delete;
    const OriginalWindowsNoticeOwner* OriginalNoticeOwner() const noexcept{return noticeOriginal_;}
    bool OriginalNoticeWorkerBodiesMatch(const void* ingress,const void* route) const noexcept {
        if(!noticeBindingPublished_.load(std::memory_order_acquire)||!noticeOriginal_||
           !ingress||!route||noticeIngressOriginal_!=ingress||noticeRouteOriginal_!=route)return false;
        for(SIZE_T i=0;i<2;++i){
            const auto& worker=workers_[i];
            if(!worker.enrollmentPublished.load(std::memory_order_acquire)||
               !worker.creationPublished.load(std::memory_order_acquire)||
               !worker.createAttempted||!worker.createReturned||worker.creationUnknown||
               !worker.threadOriginal||worker.threadOriginal==INVALID_HANDLE_VALUE||!worker.threadId||
               worker.operationOwner!=noticeOriginal_||worker.commandOriginal||
               worker.scope!=OriginalWindowsWorkerScope::Host||worker.bodyOriginal!=noticeBodiesOriginal_[i]||
               worker.inputOriginal!=noticeOriginal_||worker.inputExtent!=noticeInputExtentOriginal_||
               worker.ioOriginal!=noticeIoOriginal_[i]||worker.endpointOriginal!=noticeEndpointsOriginal_[i]||
               worker.stackReservation!=noticeStacksOriginal_[i]||worker.creationSecurityOriginal||
               worker.creationFlagsOriginal!=STACK_SIZE_PARAM_IS_A_RESERVATION)return false;
        }
        return true; // Only these fixed original bodies, never full actor/APC authority.
    }
    // Creating-thread only: the independent observer reads publication states,
    // never mutates count or the caller's raw output cells.
    OriginalWindowsPendingCall* BeginOriginal(const void* owner,const void* input,
            void* destination,SIZE_T requested,const void* originalCommand=nullptr,SIZE_T inputExtent=0) noexcept {
        if(!owner || (inputExtent&&!input) || (requested&&!destination) ||
            count_==capacity_ || !ReserveOriginal())return nullptr;
        auto& row=rows_[count_++];
        row.operationOwner=owner;row.commandOriginal=originalCommand;row.originalInput=input;
        row.originalInputExtent=inputExtent;
        row.originalDestination=destination;row.requested=requested;row.attempted=true;
        // An independent observer may inspect the immutable input/destination
        // fields only after this acquire publication, never by racing count_.
        row.activePublished.store(true,std::memory_order_release);
        return &row;
    }
    template<class NativeCount>
    bool ReturnedOriginal(OriginalWindowsPendingCall& row,BOOL result,DWORD error,const NativeCount& actual) noexcept {
        bool owned=false;
        for(SIZE_T i=0;i<count_;++i)if(&rows_[i]==&row){owned=true;break;}
        if(!owned || !row.attempted || row.resultPublished.load(std::memory_order_acquire))return false;
        row.resultShape=OriginalWindowsPendingResultShape::Boolean;
        row.returned=result;row.error=error;
        row.actualObserved=result || error!=ERROR_IO_PENDING;
        if(row.actualObserved)row.actual=actual;
        row.completionUnknown=!row.actualObserved || row.actual>row.requested;
        row.resultPublished.store(true,std::memory_order_release);
        // Unknown completion keeps the exact destination/channel borrowed.
        if(!row.completionUnknown)reservations_.fetch_sub(1,std::memory_order_acq_rel);
        return !row.completionUnknown;
    }
    bool ReturnedOriginalBoolean(OriginalWindowsPendingCall& row,BOOL result,DWORD error) noexcept {
        bool owned=false;
        for(SIZE_T i=0;i<count_;++i)if(&rows_[i]==&row){owned=true;break;}
        if(!owned || !row.attempted || row.resultPublished.load(std::memory_order_acquire))return false;
        row.resultShape=OriginalWindowsPendingResultShape::BooleanWithoutCount;
        row.returned=result;row.error=error;row.actualObserved=false;
        row.completionUnknown=!result && error==ERROR_IO_PENDING;
        row.resultPublished.store(true,std::memory_order_release);
        if(!row.completionUnknown)reservations_.fetch_sub(1,std::memory_order_acq_rel);
        return !row.completionUnknown;
    }
    // Exact current NtQueryInformationProcess shape: raw NTSTATUS is neither
    // a BOOL nor GetLastError. Only STATUS_SUCCESS qualifies the returned length.
    bool ReturnedOriginalNtQuery(OriginalWindowsPendingCall& row,LONG status,const ULONG& length) noexcept {
        bool owned=false;
        for(SIZE_T i=0;i<count_;++i)if(&rows_[i]==&row){owned=true;break;}
        if(!owned || !row.attempted || row.resultPublished.load(std::memory_order_acquire))return false;
        row.resultShape=OriginalWindowsPendingResultShape::NtQueryStatus;
        row.nativeStatusReturned=status;row.actualObserved=status==0;
        if(row.actualObserved)row.actual=length;
        // Positive informational/pending forms are not this selected query's
        // completed-success profile. Retain unknown output custody unchanged.
        row.completionUnknown=status>0 || (row.actualObserved && row.actual>row.requested);
        row.resultPublished.store(true,std::memory_order_release);
        if(!row.completionUnknown)reservations_.fetch_sub(1,std::memory_order_acq_rel);
        return !row.completionUnknown;
    }
    // HANDLE-returning APIs retain their genuine return shape. The caller
    // supplies the failure sentinel belonging to that exact native API.
    bool ReturnedOriginalHandle(OriginalWindowsPendingCall& row,HANDLE result,DWORD error,HANDLE failureSentinel,bool outputUnknown) noexcept {
        bool owned=false;
        for(SIZE_T i=0;i<count_;++i)if(&rows_[i]==&row){owned=true;break;}
        if(!owned || !row.attempted || row.resultPublished.load(std::memory_order_acquire) ||
                (failureSentinel!=nullptr && failureSentinel!=INVALID_HANDLE_VALUE))return false;
        row.resultShape=OriginalWindowsPendingResultShape::Handle;row.handleReturned=result;
        row.handleFailureSentinel=failureSentinel;row.handleOutputUnknown=outputUnknown;row.error=error;
        row.completionUnknown=outputUnknown || (result==failureSentinel && error==ERROR_IO_PENDING);
        row.resultPublished.store(true,std::memory_order_release);
        if(!row.completionUnknown)reservations_.fetch_sub(1,std::memory_order_acq_rel);
        return !row.completionUnknown;
    }
    // Creating-thread only. Reserve precedes CreateThread; the worker may use
    // the original endpoint immediately, even before CreateThread returns.
    bool StartOriginalWorker(SIZE_T slot,const void* owner,const void* command,
            LPTHREAD_START_ROUTINE body,void* input,SIZE_T extent,HANDLE endpoint,SIZE_T stackReservation,
            OriginalWindowsWorkerIoOwner& io,OriginalWindowsWorkerScope scope=OriginalWindowsWorkerScope::Command) noexcept {
        if(slot>=2 || !owner || (scope!=OriginalWindowsWorkerScope::Command&&scope!=OriginalWindowsWorkerScope::Host) ||
                (scope==OriginalWindowsWorkerScope::Command&&!command) ||
                (scope==OriginalWindowsWorkerScope::Host&&command) || !body || !input || !extent ||
                !endpoint || endpoint==INVALID_HANDLE_VALUE || !stackReservation)return false;
        if(&io.pending_!=this || io.operationOwner_!=owner || io.slot_!=slot || io.endpoint_!=endpoint ||
                io.body_!=body || io.input_!=input || io.inputExtent_!=extent)return false;
        auto& worker=workers_[slot];
        if(worker.createAttempted || worker.reserved || !ReserveOriginal())return false;
        worker.reserved=true;worker.operationOwner=owner;worker.commandOriginal=command;
        worker.scope=scope;worker.ioOriginal=&io;
        worker.bodyOriginal=body;worker.inputOriginal=input;worker.inputExtent=extent;
        worker.endpointOriginal=endpoint;worker.stackReservation=stackReservation;worker.createAttempted=true;
        worker.creationSecurityOriginal=nullptr;worker.creationFlagsOriginal=STACK_SIZE_PARAM_IS_A_RESERVATION;
        worker.enrollmentPublished.store(true,std::memory_order_release);
        worker.threadOriginal=CreateThread(worker.creationSecurityOriginal,worker.stackReservation,worker.bodyOriginal,input,
            worker.creationFlagsOriginal,&worker.threadId);
        if(!worker.threadOriginal)worker.createError=GetLastError();
        worker.createReturned=true;
        if(!worker.threadOriginal){
            // A returned null CreateThread created no worker. No worker body
            // or endpoint borrower is inferred from its error code.
            worker.reserved=false;reservations_.fetch_sub(1,std::memory_order_acq_rel);
            worker.creationPublished.store(true,std::memory_order_release);return false;
        }
        if(worker.threadOriginal==INVALID_HANDLE_VALUE || !worker.threadId){
            worker.creationUnknown=true;worker.creationPublished.store(true,std::memory_order_release);return false;
        }
        worker.creationPublished.store(true,std::memory_order_release);
        return true;
    }
    bool TryJoinOriginalWorker(SIZE_T slot) noexcept {
        if(slot>=2)return false;auto& worker=workers_[slot];
        if(worker.joined)return worker.ioOriginal && worker.ioOriginal->NativeBorrowersSettledAfterTermination();
        if(worker.creationUnknown || worker.joinUnresolved || !worker.reserved || !worker.createReturned || !worker.threadOriginal || joinCount_==capacity_)return false;
        auto& row=joins_[joinCount_++];row.workerOriginal=&worker;row.waitAttempted=true;
        row.waitReturned=WaitForSingleObject(worker.threadOriginal,0);
        if(row.waitReturned==WAIT_FAILED)row.waitError=GetLastError();
        if(row.waitReturned!=WAIT_OBJECT_0){
            if(row.waitReturned!=WAIT_TIMEOUT)worker.joinUnresolved=true;return false;
        }
        worker.nativeTerminationObserved=true;
        row.exitCodeAttempted=true;row.exitCodeReturned=GetExitCodeThread(worker.threadOriginal,&row.exitCode);
        if(!row.exitCodeReturned){row.exitCodeError=GetLastError();worker.joinUnresolved=true;return false;}
        // Signaled thread handle proves termination. Exit code 259 remains a
        // valid actual exit code; it is not reinterpreted as a running thread.
        row.closeAttempted=true;row.closeReturned=CloseHandle(worker.threadOriginal);
        if(!row.closeReturned){row.closeError=GetLastError();worker.joinUnresolved=true;return false;}
        worker.joined=true;worker.reserved=false;
        reservations_.fetch_sub(1,std::memory_order_acq_rel);
        return worker.ioOriginal && worker.ioOriginal->NativeBorrowersSettledAfterTermination();
    }
    const OriginalWindowsChannelWorker* OriginalWorker(SIZE_T slot) const noexcept {
        return slot<2?&workers_[slot]:nullptr;
    }
    const OriginalWindowsWorkerJoinAttempt* OriginalJoinAttempts() const noexcept{return joins_.get();}
    SIZE_T OriginalJoinAttemptCount() const noexcept{return joinCount_;}
    // Creating-thread only, like Begin/Return. The SAME debugger thread calls
    // this at terminal disposal and performs no later native call for that
    // command. Worker rows borrow channel endpoints, not target handles.
    bool CommandNativeCallsSettled(const void* command) const noexcept {
        if(!command)return false;
        for(SIZE_T i=0;i<count_;++i) {
            const auto& row=rows_[i];
            if(row.commandOriginal && row.commandOriginal!=command)continue;
            if(!row.activePublished.load(std::memory_order_acquire) ||
                !row.resultPublished.load(std::memory_order_acquire) || row.completionUnknown)return false;
        }
        return true;
    }
    bool CreatingThreadCallsSettledOriginal() const noexcept {
        for(SIZE_T i=0;i<count_;++i) {
            const auto& row=rows_[i];
            if(!row.activePublished.load(std::memory_order_acquire) ||
                !row.resultPublished.load(std::memory_order_acquire) || row.completionUnknown)return false;
        }
        return true;
    }
    void CloseFutureEffects() noexcept {reservations_.fetch_or(ClosedBit,std::memory_order_acq_rel);}
    bool EndpointsIndependentlyEligible() const noexcept {
        return reservations_.load(std::memory_order_acquire)==ClosedBit;
    }
    SIZE_T ReservedSourceBytes() const noexcept {
        return sizeof(*this)+capacity_*(sizeof(OriginalWindowsPendingCall)+sizeof(OriginalWindowsWorkerJoinAttempt));
    }
    const OriginalWindowsObserverGeometry& OriginalObserverGeometry() const noexcept{return observerGeometry_;}
};
inline bool OriginalWindowsWorkerIoOwner::CallOriginal(bool write,const void* originalBytes,DWORD requested,const OriginalWindowsPendingCall* originalCall) noexcept {
    if(bodyReturned_.load(std::memory_order_acquire) || !requested || requested>frameBytes_ ||
        (write&&!originalBytes) || count_==capacity_)return false;
    const auto& worker=pending_.workers_[slot_];
    if(!worker.enrollmentPublished.load(std::memory_order_acquire) || worker.ioOriginal!=this ||
        worker.operationOwner!=operationOwner_ || worker.bodyOriginal!=body_ || worker.inputOriginal!=input_ ||
        worker.inputExtent!=inputExtent_ || worker.endpointOriginal!=endpoint_)return false;
    const void* command=nullptr;
    if(originalCall) {
        bool owned=false;
        // Capacity is immutable; never race the creating thread's count_.
        for(SIZE_T i=0;i<pending_.capacity_;++i)if(&pending_.rows_[i]==originalCall){owned=true;break;}
        if(!owned || !originalCall->activePublished.load(std::memory_order_acquire))return false;
        command=originalCall->commandOriginal;
    }
    if(worker.scope==OriginalWindowsWorkerScope::Command && command!=worker.commandOriginal)return false;
    if(worker.scope!=OriginalWindowsWorkerScope::Command && worker.scope!=OriginalWindowsWorkerScope::Host)return false;
    if(!pending_.ReserveOriginal())return false;
    auto& row=rows_[count_];row.endpoint=endpoint_;row.commandOriginal=command;
    row.bytes=bytes_.get()+count_*frameBytes_;row.requested=requested;row.write=write;++count_;
    if(write)memcpy(row.bytes,originalBytes,requested);
    row.attempted=true;row.activePublished.store(true,std::memory_order_release);
    row.returned=write?WriteFile(row.endpoint,row.bytes,row.requested,&row.actual,nullptr):
        ReadFile(row.endpoint,row.bytes,row.requested,&row.actual,nullptr);
    if(!row.returned)row.error=GetLastError();
    row.completionUnknown=(!row.returned&&row.error==ERROR_IO_PENDING) || row.actual>row.requested;
    row.resultPublished.store(true,std::memory_order_release);
    if(!row.completionUnknown)pending_.reservations_.fetch_sub(1,std::memory_order_acq_rel);
    return !row.completionUnknown && row.returned && row.actual==row.requested;
}
}

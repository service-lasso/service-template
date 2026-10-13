// UNCOMPILED selected None-origin transport on the original duplex channel.
// Host preowns this concrete owner before effects and retains it through every
// genuine thread/IO/event borrower. No new deadline or expiry clock is created.
#pragma once
#include "windows_host_channel.hpp"
#include "windows_command_route.hpp"
namespace template_creator_bridge {
struct OriginalWindowsNoticeGeometry {
    SIZE_T noticeStack=0,receiverStack=0,noticeIoRows=0,receiverIoRows=0;
    bool Complete() const noexcept{return noticeStack&&receiverStack&&noticeIoRows&&receiverIoRows;}
};
class OriginalWindowsNoticeOwner final {
    friend struct OriginalWindowsStandaloneHost;
    struct Frame {
        std::uint64_t version=1,kind=1; // source-owned Stop, never a completion ACK
        std::uintptr_t owner=0,channel=0,route=0;
        std::uint64_t callOrdinal=UINT64_MAX,hasDeadline=0,originalAbsolute=0;
    } stopFrame_{};
    struct EventOriginal {
        HANDLE handle=nullptr;DWORD createError=0,setError=0,closeError=0;
        BOOL setReturned=FALSE,closeReturned=FALSE;
        bool createAttempted=false,creationUnknown=false,setAttempted=false,closeAttempted=false;
    } stopEvent_{},receiverEvent_{};
    struct WaitOriginal {DWORD returned=WAIT_FAILED,error=0;bool attempted=false;}
        noticeWait_{},receiverWait_{};
    struct ModeOriginal {
        HANDLE endpoint=nullptr;DWORD state=0,flags=0,outBytes=0,inBytes=0,instances=0;
        BOOL stateReturned=FALSE,infoReturned=FALSE;DWORD stateError=0,infoError=0;
        OriginalWindowsPendingCall* stateCall=nullptr;OriginalWindowsPendingCall* infoCall=nullptr;
        bool attempted=false,settled=false;
    } modes_[2]{};
    OriginalWindowsPendingCalls& pending_;
    const OriginalWindowsNoticeGeometry geometry_;
    std::unique_ptr<OriginalWindowsWorkerIoOwner> noticeIo_,receiverIo_;
    const OriginalHostChannel* channelOriginal_=nullptr;
    const template_creator::OriginalPrivateIngress* ingressOriginal_=nullptr;
    const template_creator::OriginalWindowsCommandRoute* routeOriginal_=nullptr;
    const void* hostOriginal_=nullptr;
    bool attachAttempted_=false,attached_=false,startAttempted_=false;
    bool receiverStarted_=false,noticeStarted_=false,stopRequested_=false;
    bool eventsClosed_=false,qualified_=false;
    std::atomic<bool> writePublished_{false},stopWritten_{false},stopReceived_{false};
    std::atomic<bool> noticeReturned_{false},receiverReturned_{false};
    static bool CreateEventOriginal(EventOriginal& row) noexcept {
        if(row.createAttempted)return false;row.createAttempted=true;
        row.handle=CreateEventW(nullptr,TRUE,FALSE,nullptr);
        if(!row.handle)row.createError=GetLastError();
        row.creationUnknown=row.handle==INVALID_HANDLE_VALUE;
        return row.handle && !row.creationUnknown;
    }
    static bool SetEventOriginal(EventOriginal& row) noexcept {
        if(row.setAttempted)return row.setReturned!=FALSE;
        if(!row.handle || row.creationUnknown)return false;
        row.setAttempted=true;row.setReturned=SetEvent(row.handle);
        if(!row.setReturned)row.setError=GetLastError();return row.setReturned!=FALSE;
    }
    static bool WaitEventOriginal(WaitOriginal& row,const EventOriginal& event) noexcept {
        row.attempted=true;row.returned=WaitForSingleObject(event.handle,INFINITE);
        if(row.returned==WAIT_FAILED)row.error=GetLastError();return row.returned==WAIT_OBJECT_0;
    }
    static DWORD WINAPI NoticeBody(void* input) noexcept {
        auto& owner=*static_cast<OriginalWindowsNoticeOwner*>(input);
        const bool woke=WaitEventOriginal(owner.noticeWait_,owner.stopEvent_);
        bool wrote=false;
        if(woke)wrote=owner.noticeIo_->CallOriginal(true,&owner.stopFrame_,sizeof(Frame),nullptr);
        owner.stopWritten_.store(wrote,std::memory_order_release);
        owner.writePublished_.store(true,std::memory_order_release);
        // Even a known failed write wakes the receiver to retain that failure;
        // it must not wait forever for a message which was never published.
        SetEventOriginal(owner.receiverEvent_);
        owner.noticeIo_->PublishBodyReturnedOriginal();
        owner.noticeReturned_.store(true,std::memory_order_release);return wrote?0:1;
    }
    static DWORD WINAPI ReceiverBody(void* input) noexcept {
        auto& owner=*static_cast<OriginalWindowsNoticeOwner*>(input);
        bool received=false;
        if(WaitEventOriginal(owner.receiverWait_,owner.receiverEvent_) &&
            owner.writePublished_.load(std::memory_order_acquire) && owner.stopWritten_.load(std::memory_order_acquire) &&
            owner.receiverIo_->CallOriginal(false,nullptr,sizeof(Frame),nullptr)) {
            const auto& row=owner.receiverIo_->rows_[0];
            // Both are genuine source-owned original occurrences. Bytes alone
            // never create constructor/channel/command authority.
            received=row.resultPublished.load(std::memory_order_acquire) && row.actual==sizeof(Frame) &&
                memcmp(row.bytes,&owner.stopFrame_,sizeof(Frame))==0;
        }
        owner.stopReceived_.store(received,std::memory_order_release);
        owner.receiverIo_->PublishBodyReturnedOriginal();
        owner.receiverReturned_.store(true,std::memory_order_release);return received?0:1;
    }
    bool CaptureModeOriginal(SIZE_T ordinal,HANDLE endpoint) noexcept {
        auto& row=modes_[ordinal];if(row.attempted)return false;row.attempted=true;row.endpoint=endpoint;
        row.stateCall=pending_.BeginOriginal(this,&row,&row.state,sizeof(row.state),nullptr,sizeof(row.endpoint));
        if(!row.stateCall)return false;
        row.stateReturned=GetNamedPipeHandleStateW(endpoint,&row.state,nullptr,nullptr,nullptr,nullptr,0);
        if(!row.stateReturned)row.stateError=GetLastError();
        if(!pending_.ReturnedOriginal(*row.stateCall,row.stateReturned,row.stateError,row.stateReturned?sizeof(row.state):0))return false;
        row.infoCall=pending_.BeginOriginal(this,&row,&row.flags,4*sizeof(DWORD),nullptr,sizeof(row.endpoint));
        if(!row.infoCall)return false;
        row.infoReturned=GetNamedPipeInfo(endpoint,&row.flags,&row.outBytes,&row.inBytes,&row.instances);
        if(!row.infoReturned)row.infoError=GetLastError();
        row.settled=pending_.ReturnedOriginal(*row.infoCall,row.infoReturned,row.infoError,row.infoReturned?4*sizeof(DWORD):0);
        return row.settled && row.stateReturned && row.infoReturned &&
            (row.state&(PIPE_READMODE_MESSAGE|PIPE_NOWAIT))==(PIPE_READMODE_MESSAGE|PIPE_NOWAIT) &&
            (row.flags&PIPE_TYPE_MESSAGE)==PIPE_TYPE_MESSAGE;
    }
    ~OriginalWindowsNoticeOwner()=default; // Only retained host may destroy after complete borrower settlement.
    OriginalWindowsNoticeOwner(OriginalWindowsPendingCalls& pending,OriginalWindowsNoticeGeometry geometry)
        :pending_(pending),geometry_(geometry) {
        if(!geometry.Complete())throw std::invalid_argument("complete original notice backing");
        // Endpoint association is deferred; every possible IO payload/output
        // cell is already preowned before any original main/native effect.
        noticeIo_.reset(new OriginalWindowsWorkerIoOwner(pending,this,0,nullptr,&NoticeBody,this,sizeof(*this),geometry.noticeIoRows,sizeof(Frame)));
        receiverIo_.reset(new OriginalWindowsWorkerIoOwner(pending,this,1,nullptr,&ReceiverBody,this,sizeof(*this),geometry.receiverIoRows,sizeof(Frame)));
        if(pending.noticeOriginal_)throw std::invalid_argument("same original notice owner already bound");
        pending.noticeOriginal_=this; // Last constructor action, after all throwing allocations.
    }
    bool AttachNoneOriginal(const OriginalHostChannel& channel,const template_creator::OriginalPrivateIngress& ingress,
        const template_creator::OriginalWindowsCommandRoute& route,const void* host) noexcept {
        if(attachAttempted_)return false;attachAttempted_=true;
        if(!host || channel.pendingCallsOriginal_!=&pending_ || !channel.constructed_ ||
            !channel.wireWriteReturned_ || !channel.bodyWriteReturned_ || channel.writeCompletionUnknown_ ||
            !ingress.Captured() || ingress.ReadCompletionUnknown() || ingress.OriginalPendingCalls()!=&pending_ ||
            ingress.OriginalCapabilities()[0].original!=channel.server_)return false;
        for(SIZE_T i=0;i<route.SelectedCount();++i) {
            const auto& row=route.PrivateOriginal(i);
            if(!row.precharged || !row.command.copied || row.command.shapeFailure || row.command.hasOriginalDeadline)return false;
        }
        channelOriginal_=&channel;ingressOriginal_=&ingress;routeOriginal_=&route;hostOriginal_=host;
        stopFrame_.owner=reinterpret_cast<std::uintptr_t>(this);
        stopFrame_.channel=reinterpret_cast<std::uintptr_t>(&channel);
        stopFrame_.route=reinterpret_cast<std::uintptr_t>(&route);
        if(!CaptureModeOriginal(0,channel.server_) || !CaptureModeOriginal(1,channel.client_) ||
            !noticeIo_->BindOriginalEndpointBeforeBirth(channel.server_) ||
            !receiverIo_->BindOriginalEndpointBeforeBirth(channel.client_))return false;
        if(pending_.noticeOriginal_!=this||pending_.noticeBindingPublished_.load(std::memory_order_acquire))return false;
        pending_.noticeIngressOriginal_=&ingress;pending_.noticeRouteOriginal_=&route;
        pending_.noticeBodiesOriginal_[0]=&NoticeBody;pending_.noticeBodiesOriginal_[1]=&ReceiverBody;
        pending_.noticeIoOriginal_[0]=noticeIo_.get();pending_.noticeIoOriginal_[1]=receiverIo_.get();
        pending_.noticeEndpointsOriginal_[0]=channel.server_;pending_.noticeEndpointsOriginal_[1]=channel.client_;
        pending_.noticeStacksOriginal_[0]=geometry_.noticeStack;pending_.noticeStacksOriginal_[1]=geometry_.receiverStack;
        pending_.noticeInputExtentOriginal_=sizeof(*this);
        pending_.noticeBindingPublished_.store(true,std::memory_order_release);
        attached_=true;return true;
    }
    bool StartOriginal() noexcept {
        if(startAttempted_ || !attached_)return false;startAttempted_=true;
        if(!CreateEventOriginal(stopEvent_) || !CreateEventOriginal(receiverEvent_))return false;
        receiverStarted_=pending_.StartOriginalWorker(1,this,nullptr,&ReceiverBody,this,sizeof(*this),
            channelOriginal_->client_,geometry_.receiverStack,*receiverIo_,OriginalWindowsWorkerScope::Host);
        if(!receiverStarted_)return false;
        noticeStarted_=pending_.StartOriginalWorker(0,this,nullptr,&NoticeBody,this,sizeof(*this),
            channelOriginal_->server_,geometry_.noticeStack,*noticeIo_,OriginalWindowsWorkerScope::Host);
        if(!noticeStarted_ && !pending_.OriginalWorker(0)->creationUnknown)SetEventOriginal(receiverEvent_);
        return noticeStarted_;
    }
    bool StopAndJoinOriginal() noexcept {
        if(!pending_.CreatingThreadCallsSettledOriginal())return false;
        if(noticeStarted_ && !stopRequested_) {
            stopRequested_=true;if(!SetEventOriginal(stopEvent_))return false;
        }
        bool joined=true;
        for(SIZE_T slot=0;slot<2;++slot) {
            const auto* worker=pending_.OriginalWorker(slot);
            if(worker->creationUnknown)return false;
            if(worker->createAttempted && worker->threadOriginal && !pending_.TryJoinOriginalWorker(slot))joined=false;
        }
        if(!joined)return false;
        // Real thread termination cannot settle a separately unknown kernel IO.
        if((noticeStarted_&&!noticeIo_->NativeBorrowersSettledAfterTermination()) ||
            (receiverStarted_&&!receiverIo_->NativeBorrowersSettledAfterTermination()))return false;
        bool closed=true;
        for(auto* event:{&stopEvent_,&receiverEvent_}) {
            if(event->creationUnknown)return false;
            if(!event->handle)continue;
            if(!event->closeAttempted) {
                event->closeAttempted=true;event->closeReturned=CloseHandle(event->handle);
                if(!event->closeReturned)event->closeError=GetLastError();
            }
            if(!event->closeReturned)closed=false;
        }
        eventsClosed_=closed;
        qualified_=closed && noticeStarted_ && receiverStarted_ && stopWritten_.load(std::memory_order_acquire) &&
            stopReceived_.load(std::memory_order_acquire);
        return closed;
    }
public:
    OriginalWindowsNoticeOwner(const OriginalWindowsNoticeOwner&)=delete;
    OriginalWindowsNoticeOwner& operator=(const OriginalWindowsNoticeOwner&)=delete;
    bool ReservedSourceBytes(SIZE_T& bytes) const noexcept {
        bytes=sizeof(*this);
        const auto first=noticeIo_->ReservedSourceBytes(),second=receiverIo_->ReservedSourceBytes();
        if(first>SIZE_MAX-bytes)return false;bytes+=first;
        if(second>SIZE_MAX-bytes)return false;bytes+=second;return true;
    }
};
}

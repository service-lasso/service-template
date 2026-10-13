// UNCOMPILED source-owned EOF for fixed protected Node routes ONLY.
#pragma once
#include "linux_command_route.hpp"
#include "linux_deadline_supervisor.hpp"
namespace template_creator_linux {
class OriginalProtectedEofStdin {
    OriginalCommandRoute& route_;
    const OriginalDeadline& deadline_;
    OriginalDeadlineSupervisor& supervisor_;
    std::array<int,2> outputs_{{-1,-1}};
    SupervisorNativeOriginal pipe_,writerClose_,eof_,readerClose_;
    unsigned char eofByte_=0;
    bool attempted_=false,acquired_=false,unknown_=false,qualified_=false,failed_=false;
    bool Before(SupervisorNativeOriginal& row,const char* operation,bool cleanup) noexcept {
        const bool clock=CaptureSupervisorClock(row.beforeClock,deadline_,operation,false);
        if(!clock){failed_=true;if(!cleanup)return false;}
        const bool admitted=cleanup?supervisor_.BeginIndependentlyEligibleCleanupNativeCall():supervisor_.BeginNativeCall();
        if(!admitted){failed_=true;return false;}row.attempted=true;return true;
    }
    void Returned(SupervisorNativeOriginal& row,long value) noexcept {
        row.returned=value;if(value<0)row.error=errno;row.returnedObserved=true;
        supervisor_.OriginalNativeCallReturned();
        if(!CaptureSupervisorClock(row.afterClock,deadline_,row.beforeClock.operation,true)||
            !row.beforeClock.shapeValid||!row.afterClock.shapeValid||row.afterClock.nanoseconds<row.beforeClock.nanoseconds)failed_=true;
    }
public:
    OriginalProtectedEofStdin(OriginalCommandRoute& route,const OriginalDeadline& deadline,OriginalDeadlineSupervisor& supervisor)
        :route_(route),deadline_(deadline),supervisor_(supervisor){
        if(supervisor.OriginalDeadlineOwner()!=&deadline)throw std::invalid_argument("same EOF stdin original deadline");
    }
    OriginalProtectedEofStdin(const OriginalProtectedEofStdin&)=delete;
    OriginalProtectedEofStdin& operator=(const OriginalProtectedEofStdin&)=delete;
    bool CaptureProtectedOriginal() noexcept {
        if(attempted_||route_.AnyCommandAcquisitionAttempted())return false;attempted_=true;
        if(!Before(pipe_,"protected-stdin-pipe",false))return false;
        Returned(pipe_,pipe2(outputs_.data(),O_CLOEXEC));
        if(pipe_.returned!=0){unknown_=outputs_[0]>=0||outputs_[1]>=0;failed_=true;return false;}
        acquired_=outputs_[0]>=0&&outputs_[1]>=0&&outputs_[0]!=outputs_[1];
        if(!acquired_){unknown_=failed_=true;return false;}
        if(!Before(writerClose_,"protected-stdin-writer-close",true))return false;
        Returned(writerClose_,close(outputs_[1]));
        if(writerClose_.returned){failed_=true;return false;}
        // Source never handed out/duplicated this writer; actual successful
        // once-close precedes probe, so no owned writer can keep read pending.
        if(!Before(eof_,"protected-stdin-eof",false))return false;
        eof_.requested=1;Returned(eof_,read(outputs_[0],&eofByte_,1));
        qualified_=eof_.returnedObserved&&eof_.returned==0&&!failed_;
        if(!qualified_)failed_=true;return qualified_;
    }
    int OriginalReader() const noexcept {
        return qualified_&&!unknown_&&!readerClose_.attempted?outputs_[0]:-1;
    }
    bool DisposeAfterOriginalRouteEligible() noexcept {
        if(!route_.AllPhysicalCommandsDisposed())return false;
        if(!acquired_)return !unknown_;
        // A failed writer close never authorizes retry of that numeric FD.
        if(!writerClose_.attempted&&Before(writerClose_,"protected-stdin-eligible-writer-close",true))
            Returned(writerClose_,close(outputs_[1]));
        if(!readerClose_.attempted&&Before(readerClose_,"protected-stdin-reader-close",true))
            Returned(readerClose_,close(outputs_[0]));
        return writerClose_.returnedObserved&&writerClose_.returned==0&&readerClose_.returnedObserved&&readerClose_.returned==0;
    }
    bool PhysicalDisposed() const noexcept {
        return !unknown_&&(!acquired_||(writerClose_.returnedObserved&&writerClose_.returned==0&&
            readerClose_.returnedObserved&&readerClose_.returned==0));
    }
    bool Qualified() const noexcept{return qualified_&&!failed_;}
};
}

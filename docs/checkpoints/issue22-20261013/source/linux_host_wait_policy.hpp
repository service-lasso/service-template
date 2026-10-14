// UNCOMPILED read-only native host wait-policy observations. No signal action
// or mask is changed. These outputs alone do not establish exclusive reaping.
#pragma once
#include "linux_deadline_supervisor.hpp"
#include <signal.h>
namespace template_creator_linux {
struct HostWaitPolicyOriginal {
    SupervisorNativeOriginal selfPid,selfTid,action;
    struct sigaction actualAction{};
    bool observationAttempted=false,shapeAccepted=false;
};
class OriginalHostWaitPolicy {
    OriginalDeadline& deadline_;
    std::unique_ptr<HostWaitPolicyOriginal[]> rows_;
    size_t capacity_=0,count_=0,selectedRows_=0;
    bool preownAttempted_=false,preowned_=false;
    std::exception_ptr preownException_;
    pid_t originalPid_=0,originalTid_=0;
    bool failed_=false;
    bool Before(SupervisorNativeOriginal& row,const char* operation) noexcept {
        const bool clean=CaptureSupervisorClock(row.beforeClock,deadline_,operation,false);
        if(!clean){failed_=true;return false;}row.attempted=true;return true;
    }
    void Returned(SupervisorNativeOriginal& row,long value) noexcept {
        row.returned=value;if(value<0)row.error=errno;row.returnedObserved=true;
        if(!CaptureSupervisorClock(row.afterClock,deadline_,row.beforeClock.operation,true)||
           row.afterClock.nanoseconds<row.beforeClock.nanoseconds)failed_=true;
    }
public:
    OriginalHostWaitPolicy(DeferredOriginalPreownTag,OriginalDeadline& origin,size_t observations) noexcept
        :deadline_(origin),selectedRows_(observations) {}
    OriginalHostWaitPolicy(OriginalDeadline& origin,size_t observations):OriginalHostWaitPolicy(DeferredOriginalPreown,origin,observations) {
        if(!PreownOriginal())std::rethrow_exception(preownException_);
    }
    bool PreownOriginal() noexcept {
        if(preownAttempted_)return false;preownAttempted_=true;
        try {
            if(!selectedRows_||selectedRows_>(SIZE_MAX-sizeof(*this))/sizeof(HostWaitPolicyOriginal))
                throw std::invalid_argument("host original wait-policy geometry");
            rows_=std::make_unique<HostWaitPolicyOriginal[]>(selectedRows_);capacity_=selectedRows_;
            preowned_=true;return true;
        }catch(...){preownException_=std::current_exception();failed_=true;return false;}
    }
    bool PreownedOriginal() const noexcept{return preowned_;}
    const std::exception_ptr& PreownExceptionOriginal() const noexcept{return preownException_;}
    size_t AllocatedCapacityOriginal() const noexcept{return rows_?capacity_:0;}
    OriginalHostWaitPolicy(const OriginalHostWaitPolicy&)=delete;
    OriginalHostWaitPolicy& operator=(const OriginalHostWaitPolicy&)=delete;
    const HostWaitPolicyOriginal* ObserveOriginalMain() noexcept {
        if(!preowned_||failed_||count_==capacity_){failed_=true;return nullptr;}
        auto& row=rows_[count_++];row.observationAttempted=true;
        if(!Before(row.selfPid,"host-wait-policy-getpid"))return &row;
        Returned(row.selfPid,syscall(SYS_getpid));
        if(!Before(row.selfTid,"host-wait-policy-gettid"))return &row;
        Returned(row.selfTid,syscall(SYS_gettid));
        if(!Before(row.action,"host-original-sigchld-query"))return &row;
        Returned(row.action,sigaction(SIGCHLD,nullptr,&row.actualAction));
        const bool identity=row.selfPid.returned>0&&row.selfTid.returned==row.selfPid.returned&&
            static_cast<unsigned long>(row.selfPid.returned)<=static_cast<unsigned long>(std::numeric_limits<pid_t>::max());
        if(!identity||row.action.returned!=0||row.actualAction.sa_handler!=SIG_DFL||
           (row.actualAction.sa_flags&SA_NOCLDWAIT)||failed_){failed_=true;return &row;}
        const pid_t pid=static_cast<pid_t>(row.selfPid.returned),tid=static_cast<pid_t>(row.selfTid.returned);
        if(count_==1){originalPid_=pid;originalTid_=tid;}
        else if(pid!=originalPid_||tid!=originalTid_){failed_=true;return &row;}
        row.shapeAccepted=true;return &row;
    }
    bool OwnsOriginal(const HostWaitPolicyOriginal& row) const noexcept {
        for(size_t i=0;i<count_;++i)if(&rows_[i]==&row)return true;return false;
    }
    bool SameObservedPolicy(const HostWaitPolicyOriginal& row) const noexcept {
        return OwnsOriginal(row)&&row.shapeAccepted&&!failed_&&row.selfPid.returned==originalPid_&&
            row.selfTid.returned==originalTid_;
    }
    const HostWaitPolicyOriginal* PrivateOriginals() const noexcept{return rows_.get();}
    size_t OriginalCount() const noexcept{return count_;}
    size_t ReservedSourceBytes() const noexcept{return sizeof(*this)+capacity_*sizeof(HostWaitPolicyOriginal);}
};
// Primary wait(2) distinguishes default SIGCHLD from explicit SIG_IGN and
// documents SA_NOCLDWAIT plus cross-thread wait behavior:
// https://man7.org/linux/man-pages/man2/waitpid.2.html
// This read-only query is necessary host-policy evidence, not proof that no
// other runtime thread/handler can reap. The complete owned host must bind its
// real thread/source/runtime/loaded closure and exact wait callsite roster.
}

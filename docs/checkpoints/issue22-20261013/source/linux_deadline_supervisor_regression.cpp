// UNRUN native result assertions; the harness supplies actual original owner.
#include "linux_deadline_supervisor.hpp"
namespace template_creator_linux {
bool VerifyActualDeadlineNotification(const OriginalDeadlineSupervisor& owner) noexcept {
    if(!owner.origin_.HasOriginalDeadline()||!owner.startAttempted_||!owner.created_||
        !owner.WorkerOriginalsPublished()||!owner.expired_.load(std::memory_order_acquire))return false;
    const uint64_t ns=owner.origin_.OriginalAbsoluteNanoseconds();
    if(owner.originalTimer_.it_value.tv_sec<0||
       static_cast<uint64_t>(owner.originalTimer_.it_value.tv_sec)!=ns/1000000000||
       static_cast<uint64_t>(owner.originalTimer_.it_value.tv_nsec)!=ns%1000000000||
       owner.originalTimer_.it_interval.tv_sec||owner.originalTimer_.it_interval.tv_nsec)return false;
    if(!owner.timer_.acquisition.attempted||!owner.timer_.acquisition.returnedObserved||
       owner.timer_.original<0||!owner.timerArm_.attempted||!owner.timerArm_.returnedObserved||
       owner.timerArm_.returned||!owner.threadCreate_.attempted||owner.threadCreate_.returned||
       !owner.pollOriginal_.attempted||!owner.pollOriginal_.returnedObserved||
       owner.pollOriginal_.returned<1||owner.pollOriginal_.returned>2||
       !(owner.pollOutputs_[0].revents&POLLIN)||!owner.timerRead_.attempted||
       !owner.timerRead_.returnedObserved||owner.timerRead_.returned!=8||owner.timerRead_.error)return false;
    uint64_t expirations=0;std::memcpy(&expirations,owner.timerBytes_.data(),8);
    return expirations!=0;
}
bool VerifyActualNoDeadlineSupervisor(const OriginalDeadlineSupervisor& owner) noexcept {
    return !owner.origin_.HasOriginalDeadline()&&owner.startAttempted_&&owner.started_&&
        !owner.created_&&!owner.threadCreate_.attempted&&!owner.timer_.acquisition.attempted&&
        !owner.wake_.acquisition.attempted&&!owner.timerArm_.attempted&&
        !owner.ExpiredOrFailed();
}
// Actual timer notification proves neither timely scheduler delivery, pending
// syscall completion, native command disposal, content seal nor host exit.
}

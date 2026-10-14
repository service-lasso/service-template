// UNRUN actual-native output assertions. The admitted harness must arrange an
// actual pending original call and retain every owner; no synthetic rows here.
#include "linux_pending_host.hpp"
namespace template_creator_linux {
bool VerifyActualPendingCallerNotice(const OriginalLinuxPendingHost& host) noexcept {
    const auto& pending=host.OriginalPendingOwner();
    const auto& receiver=pending.OriginalReceiverOwner();
    const auto& caller=host.OriginalCallerReceipt();
    // Acquire terminal publications before reading worker-owned raw originals.
    if(!receiver.OriginalReceiverReturned()||!caller.OriginalCallerOriginalsPublished()||
       !receiver.OriginalNoticeReceived()||!receiver.OriginalCallerNoticeDelivered()||
       !caller.OriginalNoticeReceived())return false;
    const auto& send=receiver.OriginalCallerSend();
    const auto& receive=caller.OriginalCapture();
    const auto& tid=caller.OriginalWorkerTid();const auto& poll=caller.OriginalPoll();
    const auto& notice=caller.OriginalNotice();
    if(!send.attempted||!send.returnedObserved||send.returned!=static_cast<long>(sizeof(OriginalPendingDeadlineNotice))||send.error||
       send.requested!=sizeof(OriginalPendingDeadlineNotice)||
       !send.beforeClock.shapeValid||!send.afterClock.shapeValid||
       send.afterClock.nanoseconds<send.beforeClock.nanoseconds)return false;
    if(!tid.attempted||!tid.returnedObserved||tid.returned<=0||tid.error||
       tid.returned==pending.OriginalCreatorTid()||!poll.attempted||!poll.returnedObserved||poll.returned<=0)return false;
    if(!receive.Captured()||receive.OriginalDescriptorCount()!=0||!receive.MessageCredentialsPresent()||
       !receive.ReceiveAttempted()||receive.OriginalReceiveResult()!=static_cast<ssize_t>(sizeof(OriginalPendingDeadlineNotice)))return false;
    // Positive pending count is the observer's original observation, not a
    // claim that the syscall completed or a fabricated success result.
    return pending.OriginalDeadlineOwner().HasOriginalDeadline()&&notice.pendingOriginalCalls==1&&notice.deadlineObserved&&
        notice.originalAbsoluteNs==pending.OriginalDeadlineOwner().OriginalAbsoluteNanoseconds()&&
        std::memcmp(receive.OriginalBody().data(),&notice,sizeof(notice))==0;
}
}

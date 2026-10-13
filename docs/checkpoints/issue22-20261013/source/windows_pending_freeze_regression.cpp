// UNRUN owner-contract regression, not native API/deadline/notice qualification.
// A later admitted harness calls these functions; no implicit process launch.
#ifdef _WIN32
#include "windows_pending_call_owner.hpp"
#include <thread>
namespace template_creator_bridge::regression {
bool PendingBeginVersusFreezeOriginal() {
    // Repetition explores actual thread interleavings; it is not a public cap.
    for(unsigned iteration=0;iteration<128;++iteration) {
        OriginalWindowsPendingCalls owner(1);
        unsigned char destination=0;
        std::atomic<bool> start{false};
        bool closerEligible=false;
        std::thread closer([&]() noexcept {
            while(!start.load(std::memory_order_acquire))std::this_thread::yield();
            owner.CloseFutureEffects();
            closerEligible=owner.EndpointsIndependentlyEligible();
        });
        start.store(true,std::memory_order_release);
        // Only THIS creating thread owns count_/native result cells. Never
        // publish a supplied return until the actual closer has joined.
        auto* call=owner.BeginOriginal(&owner,nullptr,&destination,sizeof(destination));
        closer.join();
        if(call) {
            if(closerEligible || owner.EndpointsIndependentlyEligible() ||
                !call->activePublished.load(std::memory_order_acquire) ||
                call->operationOwner!=&owner || call->originalDestination!=&destination ||
                !owner.ReturnedOriginal(*call,TRUE,0,0))return false;
        } else if(!closerEligible)return false;
        if(!owner.EndpointsIndependentlyEligible() ||
            owner.BeginOriginal(&owner,nullptr,&destination,sizeof(destination)))return false;
    }
    return true;
}
bool PendingUnknownContractRetainsOriginal() {
    // These are explicit supplied-result contract cases, NOT fabricated native
    // operation observations or proof that an API actually returns IO_PENDING.
    OriginalWindowsPendingCalls unknown(1),overextent(1),known(1),knownPartial(1);
    unsigned char destinations[3]{};
    unsigned char partialDestination[2]{};
    auto* a=unknown.BeginOriginal(&unknown,nullptr,&destinations[0],1);
    auto* b=overextent.BeginOriginal(&overextent,nullptr,&destinations[1],1);
    auto* c=known.BeginOriginal(&known,nullptr,&destinations[2],1);
    auto* d=knownPartial.BeginOriginal(&knownPartial,nullptr,partialDestination,sizeof(partialDestination));
    if(!a||!b||!c||!d)return false;
    unknown.CloseFutureEffects();overextent.CloseFutureEffects();known.CloseFutureEffects();knownPartial.CloseFutureEffects();
    // Nonzero supplied pending count must remain explicitly unobserved; it is
    // not a native final prefix. Settled partial FALSE independently releases.
    if(unknown.ReturnedOriginal(*a,FALSE,ERROR_IO_PENDING,1) ||
        overextent.ReturnedOriginal(*b,TRUE,0,2) ||
        !known.ReturnedOriginal(*c,FALSE,ERROR_ACCESS_DENIED,0) ||
        !knownPartial.ReturnedOriginal(*d,FALSE,ERROR_ACCESS_DENIED,1))return false;
    return !unknown.EndpointsIndependentlyEligible() &&
        !overextent.EndpointsIndependentlyEligible() && known.EndpointsIndependentlyEligible() &&
        knownPartial.EndpointsIndependentlyEligible() && d->actualObserved && d->actual==1 &&
        !a->actualObserved && b->actualObserved && b->actual==2 && c->actualObserved && c->actual==0 &&
        !unknown.ReturnedOriginal(*a,TRUE,0,0) &&
        a->resultPublished.load(std::memory_order_acquire) && a->completionUnknown &&
        a->originalDestination==&destinations[0] && b->originalDestination==&destinations[1];
}
bool PendingUnexpectedHandleContractRetainsOriginal() {
    // Explicit supplied FindFirst result interpretation. No native query runs
    // here; the real later harness must exercise genuine API outcomes too.
    OriginalWindowsPendingCalls unexpected(1),knownFailure(1);
    WIN32_FIND_DATAW destinations[2]{};
    auto* a=unexpected.BeginOriginal(&unexpected,nullptr,&destinations[0],sizeof(destinations[0]));
    auto* b=knownFailure.BeginOriginal(&knownFailure,nullptr,&destinations[1],sizeof(destinations[1]));
    if(!a||!b)return false;
    unexpected.CloseFutureEffects();knownFailure.CloseFutureEffects();
    if(unexpected.ReturnedOriginalHandle(*a,nullptr,0,INVALID_HANDLE_VALUE,true) ||
        !knownFailure.ReturnedOriginalHandle(*b,INVALID_HANDLE_VALUE,ERROR_FILE_NOT_FOUND,INVALID_HANDLE_VALUE,false))return false;
    return !unexpected.EndpointsIndependentlyEligible() && knownFailure.EndpointsIndependentlyEligible() &&
        a->resultShape==OriginalWindowsPendingResultShape::Handle && a->handleReturned==nullptr &&
        a->handleOutputUnknown && a->completionUnknown && a->resultPublished.load(std::memory_order_acquire) &&
        b->handleReturned==INVALID_HANDLE_VALUE && b->error==ERROR_FILE_NOT_FOUND;
}
bool CommandPendingSettlementContractOriginal() {
    // Supplied owner-contract cases only, UNRUN. Tokens are explicit surrogate
    // command identities, never authentic command/native disposal evidence.
    OriginalWindowsPendingCalls owner(4);
    unsigned commandA=0,commandB=0;
    unsigned char destinations[4]{};
    auto* a=owner.BeginOriginal(&owner,nullptr,&destinations[0],1,&commandA);
    if(!a || owner.CommandNativeCallsSettled(&commandA) || !owner.CommandNativeCallsSettled(&commandB))return false;
    if(owner.ReturnedOriginal(*a,FALSE,ERROR_IO_PENDING,0) || owner.CommandNativeCallsSettled(&commandA))return false;
    // Per-command settlement cannot freeze admission for the later original.
    auto* b=owner.BeginOriginal(&owner,nullptr,&destinations[1],1,&commandB);
    if(!b || owner.CommandNativeCallsSettled(&commandB) || !owner.ReturnedOriginal(*b,TRUE,0,1) ||
        !owner.CommandNativeCallsSettled(&commandB))return false;
    auto* unassociated=owner.BeginOriginal(&owner,nullptr,&destinations[2],1);
    if(!unassociated || owner.CommandNativeCallsSettled(&commandB) ||
        !owner.ReturnedOriginal(*unassociated,FALSE,ERROR_ACCESS_DENIED,0) ||
        !owner.CommandNativeCallsSettled(&commandB))return false;
    owner.CloseFutureEffects();
    return !owner.CommandNativeCallsSettled(&commandA) && !owner.EndpointsIndependentlyEligible() &&
        !owner.CommandNativeCallsSettled(nullptr) && a->originalDestination==&destinations[0];
}
}
#endif

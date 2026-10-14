// UNCOMPILED exact retained Python-held input disposal eligibility. This allows
// one existing owner to attempt cleanup, never supplies native role authority.
#ifndef _WIN32
#include "native_linux_platform.hpp"
#include "linux_prebirth_retention_policy.hpp"
namespace template_creator_bridge {
bool NativePlatformState::DisposeNativeInputOriginalsEligible() noexcept {
    if(externalPendingHost&&!externalPendingHostOwned)return false;
    inputDisposalStarted=true;
    if(nativeInputDisposalAttempted)return nativeInputDisposalResult;
    if(!commands.AllPhysicalCommandsDisposed()||!sink.AllAttemptedOriginalCallsReturned()||
       pending.OriginalSupervisorOwner().HasPendingOriginalNativeCall())return false;
    nativeInputDisposalAttempted=true;bool all=true;
    if(auto* policy=pendingHost.RetentionPolicyOriginal()){
        if(!policy->ObserveIndependentDispositionOriginal()||!policy->SourceDispositionEstablishedOriginal()){
            qualificationFailed=true;return false;
        }
    }
    if(auto* pair=pendingHost.ReceiptFoundationPairOriginal()){
        if(!pair->ObserveIndependentDispositionOriginal()){
            qualificationFailed=true;return false;
        }
    }
    // Do not stop the SAME original observers first: each real close occupies
    // their independently eligible pending-call slot through native return.
    for(std::size_t ordinal=0;ordinal<allocatedInputOwnerCapacity;++ordinal){
        auto& owner=inputOwners[ordinal];
        if(owner.kernelExeCandidate){
            const bool closed=owner.kernelExeCandidate->CloseEligibleOnce();
            all=closed&&owner.kernelExeCandidate->PhysicalDisposed()&&all;
        }
        for(std::size_t i=0;i<owner.count;++i)if(owner.files&&owner.files[i]){
            const bool closed=owner.files[i]->CloseEligibleOnce();
            all=closed&&owner.files[i]->PhysicalDisposed()&&all;
        }
        if(owner.cwd){
            const bool closed=owner.cwd->CloseEligibleOnce();
            all=closed&&owner.cwd->PhysicalDisposed()&&all;
        }
        if(owner.gate)all=owner.gate->DisposeOriginalTaskInputsEligible()&&all;
    }
    if(selectionOriginal){
        const bool closed=selectionOriginal->CloseEligibleOnce();
        all=closed&&selectionOriginal->PhysicalDisposed()&&all;
    }
    const bool auxiliaryClosed=pending.DisposeAuxiliaryOriginalsBeforeObserverStop();
    nativeInputDisposalResult=all&&auxiliaryClosed;
    if(!nativeInputDisposalResult)qualificationFailed=true;
    return nativeInputDisposalResult; // Physical closes remain separate from qualification.
}
PyObject* NativePlatformState::InputDisposalEligible(PyObject* ordinal,PyObject* held){
    // This is a terminal source phase: later command starts are denied even
    // when this cleanup check fails. Independent eligible cleanup remains open.
    inputDisposalStarted=true;
    const auto fail=[&](const char* reason)->PyObject*{
        qualificationFailed=true;if(!PyErr_Occurred())PyErr_SetString(PyExc_RuntimeError,reason);return nullptr;
    };
    if(!ordinal||!PyLong_CheckExact(ordinal)||!held||!creatorEntryOriginal||!creatorHeldInputs)
        return fail("same retained Linux entry/input originals required");
    const auto index=PyLong_AsSize_t(ordinal);
    if(PyErr_Occurred())return fail("original Linux input ordinal conversion");
    const bool same=creatorHeldInputs->entry==creatorEntryOriginal&&creatorHeldInputs->Contains(index,held);
    if(!creatorHeldInputs->CurrentFieldIsOriginal())qualificationFailed=true;
    if(PyErr_Occurred())return fail("original Linux entry field observation failed");
    // Source-owned command disposition is physical-only and also covers known
    // failed launch/no-child setup disposal. No synthetic terminal is required.
    // The pending counter is an actual observer-owned operation slot, not a
    // substitute for command/worker joins or arbitrary foreign borrower proof.
    if(!same||!commands.AllPhysicalCommandsDisposed()||!sink.AllAttemptedOriginalCallsReturned()||
       pending.OriginalSupervisorOwner().HasPendingOriginalNativeCall())
        return fail("original Linux input has unresolved dependent native custody");
    Py_RETURN_NONE;
}
}
#endif

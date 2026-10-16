// UNRUN native ROOT consumer. Positive rows MUST come from the actual gate.
// Altered readonly views below are source-surrogate rejection checks, never
// observations of failed native calls, foreign events or unknown kernel IO.
#include "windows_session_gate.hpp"
namespace template_creator {
// Called from the actual settled gate publication route. This is an alias
// mutation rejection test over genuine attached backing, not native authority.
bool ObserveOriginalWindowsRetainedBackingRegression(const OriginalWindowsGate& gate) noexcept {
    using View=OriginalWindowsGate::RetainedPreownView;
    View view{};if(!gate.ObserveRetainedPreownViewOriginal(view))return false;
    SIZE_T exercised=0;
    const auto reject=[&](const void* backing,SIZE_T extent) noexcept {
        if(!backing||extent<sizeof(View))return true;
        unsigned char preserved[sizeof(View)];std::memcpy(preserved,backing,sizeof(preserved));
        auto& overlap=*reinterpret_cast<View*>(const_cast<void*>(backing));++exercised;
        return !gate.ObserveRetainedPreownViewOriginal(overlap)&&!std::memcmp(preserved,backing,sizeof(preserved));
    };
    if(!reject(&gate,sizeof(gate))||!reject(view.commandOriginal,sizeof(*view.commandOriginal)))return false;
    if(const auto* custody=gate.PrivateCoreClrCustody()){
        if(custody->RetainedThreadCapacityOriginal()>SIZE_MAX/sizeof(OriginalCoreClrThreadCustody)||
           custody->RetainedChangeCapacityOriginal()>SIZE_MAX/sizeof(OriginalCoreClrContextChange)||!reject(custody,sizeof(*custody))||
           !reject(custody->PrivateThreads(),custody->RetainedThreadCapacityOriginal()*sizeof(OriginalCoreClrThreadCustody))||
           !reject(custody->PrivateChanges(),custody->RetainedChangeCapacityOriginal()*sizeof(OriginalCoreClrContextChange)))return false;
    }
    if(view.imageCapacity>SIZE_MAX/sizeof(std::unique_ptr<GateImageObservation>)||
       view.invocationCapacity>SIZE_MAX/sizeof(std::unique_ptr<GateCoreClrInvocation>)||
       !reject(view.imageSlotsOriginal,view.imageCapacity*sizeof(std::unique_ptr<GateImageObservation>))||
       !reject(view.invocationSlotsOriginal,view.invocationCapacity*sizeof(std::unique_ptr<GateCoreClrInvocation>)))return false;
    for(SIZE_T i=0;i<view.imageCapacity;++i){const auto* row=gate.RetainedPreownImageOriginal(view,i);if(!row)continue;
        if(row->loaded.regionCapacity>SIZE_MAX/sizeof(MemoryRegion)||!reject(row,sizeof(*row))||
           !reject(row->loaded.bytes.get(),row->loaded.byteCapacity)||
           !reject(row->loaded.regions.get(),row->loaded.regionCapacity*sizeof(MemoryRegion)))return false;
    }
    for(SIZE_T i=0;i<view.invocationCapacity;++i){const auto* row=gate.RetainedPreownInvocationOriginal(view,i);if(!row)continue;
        if(row->instructionCapacity>SIZE_MAX/sizeof(GateCoreClrInstruction)||!reject(row,sizeof(*row))||
           !reject(row->instructionOriginals.get(),row->instructionCapacity*sizeof(GateCoreClrInstruction)))return false;
        if(row->inputs){const auto& input=*row->inputs;SIZE_T reads=0,spans=0;
            if(!input.RetainedRowBackingExtentsOriginal(reads,spans)||!reject(&input,sizeof(input))||
               !reject(input.PrivateReads(),reads)||!reject(input.PrivateStrings(),spans)||
               !reject(input.PrivateStringBytes(),input.PrivateStringCapacity())||
               !reject(input.PrivateRevalidatedStringBytes(),input.PrivateStringCapacity())||
               input.PrivatePointerCapacity()>SIZE_MAX/sizeof(DWORD64))return false;
            for(SIZE_T k=0;k<2;++k)if(!reject(input.PrivatePointers(k),input.PrivatePointerCapacity()*sizeof(DWORD64))||
                !reject(input.PrivateRevalidatedPointers(k),input.PrivatePointerCapacity()*sizeof(DWORD64)))return false;
        }
    }
    return exercised!=0;
}
// Synthetic allocation-prefix case, not a failed native allocation observation.
// It exercises the SAME extent body used by the final publication caller.
bool ObservePartialPreownBackingRegression() noexcept {
    try {
        OriginalCoreClrEntryInputs prefix(1,1,1);
        prefix.spans_=std::make_unique<OriginalCoreClrEntryInputs::StringOriginal[]>(4);
        SIZE_T reads=SIZE_MAX,spans=SIZE_MAX;
        if(!prefix.RetainedRowBackingExtentsOriginal(reads,spans)||reads||
            spans!=4*sizeof(OriginalCoreClrEntryInputs::StringOriginal)||prefix.spanCount||prefix.readCount||prefix.reads_)return false;
        prefix.readCount=1;
        return !prefix.RetainedRowBackingExtentsOriginal(reads,spans);
    }catch(...){return false;}
}
bool ObserveOriginalChainRegression(const GateCoreClrInvocation& invocation,const State& state) noexcept {
    if(!ObservePartialPreownBackingRegression())return false;
#if defined(_M_AMD64) || defined(__x86_64__)
    const auto continued=[](const Event* event) noexcept {
        return OriginalChainEventUsefulContinued(event);
    };
    if(invocation.stateOriginal!=&state||!state.terminal||!invocation.initializerCallOriginal||
       !invocation.entryContext.captured||!continued(invocation.entryContext.stoppedOriginal)||
       !invocation.instructionCount||invocation.instructionCount>invocation.instructionCapacity)return false;
    if(invocation.entryPredecessorOriginal!=&invocation.instructionOriginals[invocation.instructionCount-1]||
       !OriginalChainPredecessorContinued(*invocation.entryPredecessorOriginal,invocation.entryContext,
           invocation.entryPairOriginal))return false;
    SIZE_T initializerOrdinal=SIZE_MAX;
    for(SIZE_T i=0;i<invocation.instructionCount;++i)
        if(&invocation.instructionOriginals[i]==invocation.initializerCallOriginal)initializerOrdinal=i;
    if(initializerOrdinal==SIZE_MAX)return false;
    bool helperReturn=false,wrongRspRejected=false,changedTargetRejected=false,foreignViewRejected=false,unknownViewRejected=false;
    for(SIZE_T i=0;i<invocation.instructionCount;++i){
        const auto& row=invocation.instructionOriginals[i];
        if(!row.context.captured||row.context.stateOriginal!=&state||
           row.context.threadCreateOriginal!=invocation.threadCreateOriginal||
           row.context.commandOriginal!=invocation.entryContext.commandOriginal||
           row.context.pendingOwnerOriginal!=invocation.entryContext.pendingOwnerOriginal||
           !row.cause.ownedOnlyObserved||row.cause.contextOriginal!=&row.context||
           row.cause.pairOriginal!=row.pairOriginal||row.step.causeOriginal!=&row.cause||
           !row.movementOriginal||!row.movementOriginal->effectiveVerified||
           row.movementOriginal->precedingPairOriginal!=row.pairOriginal||
           !continued(row.context.stoppedOriginal)||!row.step.predicted)return false;
        if(!i){if(row.predecessorOriginal)return false;}
        else if(row.predecessorOriginal!=&invocation.instructionOriginals[i-1]||
                !OriginalChainPredecessorContinued(invocation.instructionOriginals[i-1],row))return false;
        if(row.step.instruction.flow!=OriginalAmd64Instruction::Flow::Return)continue;
        const auto* call=row.returningCallOriginal;
        SIZE_T callOrdinal=SIZE_MAX;
        for(SIZE_T j=0;j<i;++j)if(&invocation.instructionOriginals[j]==call)callOrdinal=j;
        if(callOrdinal==SIZE_MAX||i>=initializerOrdinal)return false;
        if(!OriginalChainReturnMatchesCall(row,call)||call==invocation.initializerCallOriginal||
           i+1>=invocation.instructionCount)return false;
        const auto& next=invocation.instructionOriginals[i+1];
        if(OriginalChainNextStopFailure(row,call,next.context.effective))return false;
        helperReturn=true;
        CONTEXT wrong=next.context.effective;wrong.Rsp^=8;
        wrongRspRejected=OriginalChainNextStopFailure(row,call,wrong)!=nullptr;
        // Copy only scalar view; no native API consumes the copied step.
        GateCoreClrInstruction changed;changed.step=row.step;
        changed.step.revalidatedReturnTarget^=1;
        changedTargetRejected=!OriginalChainReturnMatchesCall(changed,call);
        Event foreign=*row.context.stoppedOriginal;
        foreign.original.u.Exception.ExceptionRecord.ExceptionCode=EXCEPTION_ACCESS_VIOLATION;
        OriginalCoreClrDebugCause rejected;
        foreignViewRejected=row.pairOriginal&&!rejected.Observe(state,foreign,row.context,*row.pairOriginal,row.pairOriginal->movableSlot);
        Event unknown;OriginalCoreClrDebugCause unknownCause;
        unknownViewRejected=row.pairOriginal&&!unknownCause.Observe(state,unknown,row.context,*row.pairOriginal,row.pairOriginal->movableSlot);
        if(!wrongRspRejected||!changedTargetRejected||!foreignViewRejected||!unknownViewRejected)return false;
    }
    const auto& initializer=*invocation.initializerCallOriginal;
    return helperReturn&&wrongRspRejected&&changedTargetRejected&&foreignViewRejected&&unknownViewRejected&&
        initializer.step.predicted&&initializer.step.instruction.flow==OriginalAmd64Instruction::Flow::Call&&
        !OriginalChainNextStopFailure(initializer,&initializer,invocation.entryContext.effective)&&
        OriginalChainRowAvailable(0,1)&&!OriginalChainRowAvailable(1,1)&&!OriginalChainRowAvailable(2,1);
#else
    return false;
#endif
}
}

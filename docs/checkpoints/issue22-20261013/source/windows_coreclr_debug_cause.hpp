// UNCOMPILED exact saved-event cause observation. This is not CPU/source/ABI
// admission and cannot authorize Consume or clear any native debug status.
#pragma once
#include "windows_coreclr_context_change.hpp"
namespace template_creator {
struct OriginalCoreClrDebugCause {
    const State* stateOriginal=nullptr;
    const Event* stoppedOriginal=nullptr;
    const OriginalCoreClrThreadContext* contextOriginal=nullptr;
    const OriginalCoreClrContextChange* pairOriginal=nullptr;
    DWORD64 retainedDr6=0,retainedDr7=0;
    unsigned expectedSlot=4;
    bool attempted=false,ownedOnlyObserved=false;
    enum class Failure {None,Identity,Context,Exception,Pair,WrongAddress,ForeignOrMixed,
        UnsupportedCpuStatus,TrapOrGeneralDetect};
    Failure failure=Failure::None;
    bool Observe(const State& state,const Event& stopped,const OriginalCoreClrThreadContext& context,
            const OriginalCoreClrContextChange& pair,unsigned expected) noexcept {
        if(attempted)return false;attempted=true;
        stateOriginal=&state;stoppedOriginal=&stopped;contextOriginal=&context;
        pairOriginal=&pair;expectedSlot=expected;
#if defined(_M_AMD64) || defined(__x86_64__)
        const auto reject=[&](Failure why){failure=why;return false;};
        if(!state.eventCount || &stopped!=&state.events[state.eventCount-1] || stopped.continued ||
            context.stateOriginal!=&state || context.stoppedOriginal!=&stopped ||
            context.threadCreateOriginal!=pair.before.threadCreateOriginal)
            return reject(Failure::Identity);
        if(!context.captured || !OriginalCoreClrContextChange::CompleteContext(context.effective))
            return reject(Failure::Context);
        retainedDr6=context.effective.Dr6;retainedDr7=context.effective.Dr7;
        if(stopped.original.dwDebugEventCode!=EXCEPTION_DEBUG_EVENT ||
            !stopped.original.u.Exception.dwFirstChance ||
            stopped.original.u.Exception.ExceptionRecord.ExceptionCode!=EXCEPTION_SINGLE_STEP)
            return reject(Failure::Exception);
        if(expected>3 || (expected!=pair.slot && expected!=pair.movableSlot) ||
            (expected==pair.movableSlot && !pair.movableEnabled) || !pair.OwnsPair(context.effective))
            return reject(Failure::Pair);
        const DWORD64 address=OriginalCoreClrContextChange::Address(context.effective,expected);
        if(context.effective.Rip!=address ||
            reinterpret_cast<std::uintptr_t>(stopped.original.u.Exception.ExceptionRecord.ExceptionAddress)!=address)
            return reject(Failure::WrongAddress);
        // B0..B3 are interpreted with effective enables. A stale disabled-slot
        // status is not ignored; this selected positive subset requires exactly
        // the one expected status, preventing coincident/foreign splitting.
        if((retainedDr6&15)!=(DWORD64(1)<<expected) ||
            (retainedDr6&((DWORD64(1)<<13)|(DWORD64(1)<<14)|(DWORD64(1)<<15))))
            return reject(Failure::ForeignOrMixed);
        // Active-low bus-lock and RTM indications are not guessed away. CPU
        // admission must independently establish these selected meanings.
        if(!(retainedDr6&(DWORD64(1)<<11)) || !(retainedDr6&(DWORD64(1)<<16)))
            return reject(Failure::UnsupportedCpuStatus);
        if((context.effective.EFlags&(DWORD(1)<<8)) || (retainedDr7&(DWORD64(1)<<13)))
            return reject(Failure::TrapOrGeneralDetect);
        ownedOnlyObserved=true;return true;
#else
        failure=Failure::Context;return false;
#endif
    }
};
}

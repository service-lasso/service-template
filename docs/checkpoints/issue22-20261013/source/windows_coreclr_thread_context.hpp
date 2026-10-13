// UNCOMPILED selected AMD64 saved-event context acquisition. No installation,
// export/build admission or continuation grant follows from this read alone.
#pragma once
#include "windows_driver.hpp"
#include "windows_pending_call_owner.hpp"
#include "native_command_plan.hpp"
namespace template_creator {
struct OriginalCoreClrThreadContext {
    const State* stateOriginal=nullptr;
    const Event* stoppedOriginal=nullptr;
    const Event* threadCreateOriginal=nullptr;
    const template_creator_bridge::WindowsOriginalCommand* commandOriginal=nullptr;
    template_creator_bridge::OriginalWindowsPendingCalls* pendingOwnerOriginal=nullptr;
    HANDLE threadOriginal=nullptr;
    CONTEXT requested{}, effective{};
    template_creator_bridge::OriginalWindowsPendingCall* pendingOriginal=nullptr;
    BOOL nativeReturned=FALSE;
    DWORD nativeError=0;
    bool attempted=false, returnedObserved=false, captured=false;
    bool Capture(const State& state,const Event& stopped,
            template_creator_bridge::OriginalWindowsPendingCalls& pending,
            const template_creator_bridge::WindowsOriginalCommand& command,const Event* selectedBorn=nullptr) noexcept {
        if(attempted)return false;attempted=true;
        stateOriginal=&state;stoppedOriginal=&stopped;commandOriginal=&command;pendingOwnerOriginal=&pending;
        if(!command.copied || !command.snapshotOriginal)return false;
#if defined(_M_AMD64) || defined(__x86_64__)
        threadCreateOriginal=selectedBorn?OriginalStoppedThreadOccurrenceFor(state,stopped,*selectedBorn):
            OriginalStoppedThreadOccurrence(state,stopped);
        if(!threadCreateOriginal)return false;
        threadOriginal=state.resources[threadCreateOriginal->createdThreadResource].original;
        requested.ContextFlags=CONTEXT_CONTROL|CONTEXT_INTEGER|CONTEXT_DEBUG_REGISTERS;
        effective=requested;
        pendingOriginal=pending.BeginOriginal(this,&requested,&effective,sizeof(effective),&command,sizeof(requested));
        if(!pendingOriginal)return false;
        nativeReturned=GetThreadContext(threadOriginal,&effective);
        if(!nativeReturned)nativeError=GetLastError();
        returnedObserved=true;
        // GetThreadContext has no byte-count output. Preserve that API shape;
        // sizeof(CONTEXT) is the selected destination, not a fabricated count.
        if(!pending.ReturnedOriginalBoolean(*pendingOriginal,nativeReturned,nativeError))return false;
        captured=nativeReturned!=FALSE;
        return captured;
#else
        (void)pending;
        return false; // No guessed cross-architecture context representation.
#endif
    }
};
}

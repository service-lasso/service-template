// UNCOMPILED selected-slot context transition. This native row does not supply
// runtime/caller/CPU/source admission or an independent continuation decision.
#pragma once
#include "windows_coreclr_thread_context.hpp"
namespace template_creator {
struct OriginalCoreClrContextChange {
    enum class Kind {None,Install,ResumeOwnedInstruction,Disarm,InstallPair,MovePair,DisarmPair};
    Kind kind=Kind::None;
    OriginalCoreClrThreadContext before,after;
    CONTEXT desired{};
    const OriginalCoreClrContextChange* installationOriginal=nullptr;
    const OriginalCoreClrThreadContext* entryOriginal=nullptr;
    template_creator_bridge::OriginalWindowsPendingCall* setPendingOriginal=nullptr;
    unsigned slot=4;
    DWORD64 address=0;
    unsigned movableSlot=4;
    DWORD64 movableAddress=0;
    bool movableEnabled=false;
    const OriginalCoreClrContextChange* precedingPairOriginal=nullptr;
    BOOL setReturned=FALSE;
    DWORD setError=0;
    bool attempted=false,setAttempted=false,setReturnedObserved=false,effectiveVerified=false;
#if defined(_M_AMD64) || defined(__x86_64__)
    static DWORD64 Address(const CONTEXT& context,unsigned selected) noexcept {
        switch(selected){case 0:return context.Dr0;case 1:return context.Dr1;
            case 2:return context.Dr2;case 3:return context.Dr3;default:return 0;}
    }
    static void Address(CONTEXT& context,unsigned selected,DWORD64 value) noexcept {
        switch(selected){case 0:context.Dr0=value;break;case 1:context.Dr1=value;break;
            case 2:context.Dr2=value;break;case 3:context.Dr3=value;break;default:break;}
    }
    static DWORD64 SlotMask(unsigned selected) noexcept {
        return selected<4?(DWORD64(3)<<(selected*2))|(DWORD64(15)<<(16+selected*4)):0;
    }
    static DWORD64 LocalEnable(unsigned selected) noexcept{return selected<4?DWORD64(1)<<(selected*2):0;}
    static bool CompleteContext(const CONTEXT& context) noexcept {
        const DWORD required=CONTEXT_CONTROL|CONTEXT_INTEGER|CONTEXT_DEBUG_REGISTERS;
        return (context.ContextFlags&required)==required;
    }
    static bool GeneralEqual(const CONTEXT& a,const CONTEXT& b) noexcept {
        return a.Rip==b.Rip&&a.Rsp==b.Rsp&&a.EFlags==b.EFlags&&a.SegCs==b.SegCs&&a.SegSs==b.SegSs&&
            a.Rax==b.Rax&&a.Rbx==b.Rbx&&a.Rcx==b.Rcx&&a.Rdx==b.Rdx&&a.Rsi==b.Rsi&&a.Rdi==b.Rdi&&
            a.Rbp==b.Rbp&&a.R8==b.R8&&a.R9==b.R9&&a.R10==b.R10&&a.R11==b.R11&&
            a.R12==b.R12&&a.R13==b.R13&&a.R14==b.R14&&a.R15==b.R15;
    }
    static bool DebugEqual(const CONTEXT& a,const CONTEXT& b) noexcept {
        return a.Dr0==b.Dr0&&a.Dr1==b.Dr1&&a.Dr2==b.Dr2&&a.Dr3==b.Dr3&&a.Dr6==b.Dr6&&a.Dr7==b.Dr7;
    }
    bool Owns(const CONTEXT& current) const noexcept {
        return kind==Kind::Install&&effectiveVerified&&slot<4&&address&&
            Address(current,slot)==address&&(current.Dr7&SlotMask(slot))==LocalEnable(slot);
    }
    bool OwnsPair(const CONTEXT& current) const noexcept {
        return (kind==Kind::InstallPair || kind==Kind::MovePair) && effectiveVerified &&
            slot<4 && movableSlot<4 && slot!=movableSlot && address && movableAddress &&
            Address(current,slot)==address && Address(current,movableSlot)==movableAddress &&
            (current.Dr7&SlotMask(slot))==LocalEnable(slot) &&
            (current.Dr7&SlotMask(movableSlot))==(movableEnabled?LocalEnable(movableSlot):0);
    }
private:
    bool Commit(const State& state,const Event& stopped,
            template_creator_bridge::OriginalWindowsPendingCalls& pending,
            const template_creator_bridge::WindowsOriginalCommand& command,const Event& born) noexcept {
        if(!before.captured || !CompleteContext(before.effective) || !OriginalStoppedThreadOccurrenceFor(state,stopped,born))return false;
        setPendingOriginal=pending.BeginOriginal(this,&desired,nullptr,0,&command,sizeof(desired));
        if(!setPendingOriginal)return false;
        setAttempted=true;setReturned=SetThreadContext(before.threadOriginal,&desired);
        if(!setReturned)setError=GetLastError();setReturnedObserved=true;
        if(!pending.ReturnedOriginalBoolean(*setPendingOriginal,setReturned,setError)||!setReturned)return false;
        if(!after.Capture(state,stopped,pending,command,&born))return false;
        // Protected-field normalization is not guessed. Any unaccounted
        // effective change stays negative for this selected source profile.
        const DWORD required=CONTEXT_CONTROL|CONTEXT_INTEGER|CONTEXT_DEBUG_REGISTERS;
        effectiveVerified=(after.effective.ContextFlags&required)==required &&
            GeneralEqual(after.effective,desired)&&DebugEqual(after.effective,desired);
        return effectiveVerified;
    }
public:
    bool InstallPair(const State& state,const Event& stopped,const Event& born,
            DWORD64 initializer,DWORD64 writer,
            template_creator_bridge::OriginalWindowsPendingCalls& pending,
            const template_creator_bridge::WindowsOriginalCommand& command) noexcept {
        if(attempted)return false;attempted=true;kind=Kind::InstallPair;
        address=initializer;movableAddress=writer;movableEnabled=true;
        if(!initializer || !writer || initializer==writer ||
            !before.Capture(state,stopped,pending,command,&born) || !CompleteContext(before.effective))return false;
        const auto& original=before.effective;
        for(unsigned candidate=0;candidate<4;++candidate) {
            if(Address(original,candidate) || (original.Dr7&SlotMask(candidate)) ||
                (original.Dr6&(DWORD64(1)<<candidate)))continue;
            if(slot==4)slot=candidate;else {movableSlot=candidate;break;}
        }
        if(slot==4 || movableSlot==4)return false;
        desired=original;desired.ContextFlags=CONTEXT_DEBUG_REGISTERS;
        Address(desired,slot,initializer);Address(desired,movableSlot,writer);
        desired.Dr7=(desired.Dr7&~(SlotMask(slot)|SlotMask(movableSlot)))|
            LocalEnable(slot)|LocalEnable(movableSlot);
        return Commit(state,stopped,pending,command,born);
    }
    // Cause interpretation and next-instruction/source authority belong to the
    // concrete controller. This transition accepts no callback/boolean proof.
    // It only verifies and changes the SAME previously effective owned pair.
    bool MovePair(const State& state,const Event& stopped,const OriginalCoreClrContextChange& preceding,
            const OriginalCoreClrThreadContext& stoppedContext,DWORD64 next,
            template_creator_bridge::OriginalWindowsPendingCalls& pending,
            const template_creator_bridge::WindowsOriginalCommand& command) noexcept {
        if(attempted)return false;attempted=true;kind=Kind::MovePair;
        precedingPairOriginal=&preceding;
        installationOriginal=preceding.kind==Kind::InstallPair?&preceding:preceding.installationOriginal;
        slot=preceding.slot;movableSlot=preceding.movableSlot;address=preceding.address;
        const Event* born=preceding.before.threadCreateOriginal;
        if(!born || !installationOriginal || !next || !stoppedContext.captured ||
            stoppedContext.stateOriginal!=&state || stoppedContext.stoppedOriginal!=&stopped ||
            stoppedContext.threadCreateOriginal!=born ||
            !before.Capture(state,stopped,pending,command,born) || !CompleteContext(before.effective) ||
            !preceding.OwnsPair(before.effective) ||
            !GeneralEqual(before.effective,stoppedContext.effective) ||
            !DebugEqual(before.effective,stoppedContext.effective))return false;
        // Collision protocol: disable the movable guard at its distinct current
        // address when next is permanent entry; never arm both at entry.
        movableEnabled=next!=address;
        movableAddress=movableEnabled?next:preceding.movableAddress;
        if(!movableEnabled && before.effective.Rip==address)return false;
        desired=before.effective;desired.ContextFlags=CONTEXT_CONTROL|CONTEXT_DEBUG_REGISTERS;
        Address(desired,movableSlot,movableAddress);
        desired.Dr7=(desired.Dr7&~SlotMask(movableSlot))|(movableEnabled?LocalEnable(movableSlot):0);
        // Only owned causes are cleared. Mixed-cause rejection must already
        // have occurred in the controller; clearing cannot confer Consume.
        desired.Dr6&=~((DWORD64(1)<<slot)|(DWORD64(1)<<movableSlot));
        desired.EFlags|=DWORD(1)<<16;
        return Commit(state,stopped,pending,command,*born);
    }
    bool DisarmPair(const State& state,const Event& stopped,const OriginalCoreClrContextChange& preceding,
            template_creator_bridge::OriginalWindowsPendingCalls& pending,
            const template_creator_bridge::WindowsOriginalCommand& command) noexcept {
        if(attempted)return false;attempted=true;kind=Kind::DisarmPair;
        precedingPairOriginal=&preceding;
        installationOriginal=preceding.kind==Kind::InstallPair?&preceding:preceding.installationOriginal;
        slot=preceding.slot;movableSlot=preceding.movableSlot;address=preceding.address;
        movableAddress=preceding.movableAddress;movableEnabled=preceding.movableEnabled;
        const Event* born=preceding.before.threadCreateOriginal;
        if(!born || !installationOriginal || installationOriginal->kind!=Kind::InstallPair ||
            !before.Capture(state,stopped,pending,command,born) || !CompleteContext(before.effective) ||
            !preceding.OwnsPair(before.effective))return false;
        const auto& original=installationOriginal->before.effective;
        desired=before.effective;desired.ContextFlags=CONTEXT_DEBUG_REGISTERS;
        Address(desired,slot,Address(original,slot));Address(desired,movableSlot,Address(original,movableSlot));
        const auto masks=SlotMask(slot)|SlotMask(movableSlot);
        desired.Dr7=(desired.Dr7&~masks)|(original.Dr7&masks);
        const auto status=(DWORD64(1)<<slot)|(DWORD64(1)<<movableSlot);
        desired.Dr6=(desired.Dr6&~status)|(original.Dr6&status);
        return Commit(state,stopped,pending,command,*born);
    }
    bool Install(const State& state,const Event& stopped,const Event& born,DWORD64 entry,
            template_creator_bridge::OriginalWindowsPendingCalls& pending,
            const template_creator_bridge::WindowsOriginalCommand& command) noexcept {
        if(attempted)return false;attempted=true;kind=Kind::Install;address=entry;
        if(!entry || !before.Capture(state,stopped,pending,command,&born)||!CompleteContext(before.effective))return false;
        const auto& original=before.effective;
        for(unsigned candidate=0;candidate<4;++candidate)
            if(!Address(original,candidate)&&!(original.Dr7&SlotMask(candidate))&&!(original.Dr6&(DWORD64(1)<<candidate))){slot=candidate;break;}
        if(slot==4)return false;
        desired=original;desired.ContextFlags=CONTEXT_DEBUG_REGISTERS;
        Address(desired,slot,entry);desired.Dr7=(desired.Dr7&~SlotMask(slot))|LocalEnable(slot);
        return Commit(state,stopped,pending,command,born);
    }
    bool ResumeOwned(const State& state,const Event& stopped,const OriginalCoreClrContextChange& installation,
            const OriginalCoreClrThreadContext& entry,template_creator_bridge::OriginalWindowsPendingCalls& pending,
            const template_creator_bridge::WindowsOriginalCommand& command) noexcept {
        if(attempted)return false;attempted=true;kind=Kind::ResumeOwnedInstruction;
        installationOriginal=&installation;entryOriginal=&entry;slot=installation.slot;address=installation.address;
        const Event* born=installation.before.threadCreateOriginal;
        if(!born || !entry.captured || entry.stateOriginal!=&state || entry.stoppedOriginal!=&stopped ||
            entry.threadCreateOriginal!=born || !before.Capture(state,stopped,pending,command,born) ||
            !CompleteContext(before.effective)||!CompleteContext(entry.effective)||!installation.Owns(before.effective)||!GeneralEqual(before.effective,entry.effective)||
            !DebugEqual(before.effective,entry.effective)||!(before.effective.Dr6&(DWORD64(1)<<slot)))return false;
        desired=before.effective;desired.ContextFlags=CONTEXT_CONTROL|CONTEXT_DEBUG_REGISTERS;
        desired.EFlags|=DWORD(1)<<16;desired.Dr6&=~(DWORD64(1)<<slot);
        return Commit(state,stopped,pending,command,*born);
    }
    bool Disarm(const State& state,const Event& stopped,const OriginalCoreClrContextChange& installation,
            template_creator_bridge::OriginalWindowsPendingCalls& pending,
            const template_creator_bridge::WindowsOriginalCommand& command) noexcept {
        if(attempted)return false;attempted=true;kind=Kind::Disarm;installationOriginal=&installation;
        slot=installation.slot;address=installation.address;const Event* born=installation.before.threadCreateOriginal;
        if(!born || !before.Capture(state,stopped,pending,command,born)||!CompleteContext(before.effective)||!installation.Owns(before.effective))return false;
        desired=before.effective;desired.ContextFlags=CONTEXT_DEBUG_REGISTERS;
        Address(desired,slot,Address(installation.before.effective,slot));
        const auto mask=SlotMask(slot);desired.Dr7=(desired.Dr7&~mask)|(installation.before.effective.Dr7&mask);
        const auto status=DWORD64(1)<<slot;desired.Dr6=(desired.Dr6&~status)|(installation.before.effective.Dr6&status);
        // Fresh unrelated state remains intact; never restore the old entire
        // CONTEXT or a stale application RF bit at an unload/later event.
        return Commit(state,stopped,pending,command,*born);
    }
#else
    bool Owns(const CONTEXT&) const noexcept{return false;}
    bool OwnsPair(const CONTEXT&) const noexcept{return false;}
#endif
};
}

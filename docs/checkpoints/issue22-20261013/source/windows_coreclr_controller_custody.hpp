// UNCOMPILED production-gate custody backing. Enrollment and native terminal
// publication are not source/CPU/caller authority or permission to install.
#pragma once
#include "windows_retained_backing.hpp"
#include <exception>
#include "windows_coreclr_context_change.hpp"
#include "windows_coreclr_debug_cause.hpp"
namespace template_creator {
struct LoadedImageObservation;
struct OriginalCoreClrThreadCustody {
    const State* stateOriginal=nullptr;
    const Event* createOriginal=nullptr;
    const Event* installationImageEventOriginal=nullptr;
    const LoadedImageObservation* installationImageOriginal=nullptr;
    const Event* exitThreadOriginal=nullptr;
    const Event* lastThreadProcessExitOriginal=nullptr;
    const OriginalCoreClrContextChange* installationOriginal=nullptr;
    const OriginalCoreClrContextChange* currentPairOriginal=nullptr;
    const OriginalCoreClrContextChange* disarmOriginal=nullptr;
    const Event* cleanupEventOriginal=nullptr;
    OriginalCoreClrThreadContext cleanupContext;
    OriginalCoreClrDebugCause cleanupCause;
    bool enrolled=false;
    bool NativeTerminalContinued() const noexcept {
        if(!enrolled || !stateOriginal || !createOriginal)return false;
        if(exitThreadOriginal && lastThreadProcessExitOriginal)return false;
        if(exitThreadOriginal)
            return exitThreadOriginal->original.dwDebugEventCode==EXIT_THREAD_DEBUG_EVENT &&
                exitThreadOriginal->continued && exitThreadOriginal->continuationResult &&
                createOriginal->threadExitObserved && createOriginal->threadExitContinued;
        if(lastThreadProcessExitOriginal) {
            const auto& exit=*lastThreadProcessExitOriginal;
            return exit.original.dwDebugEventCode==EXIT_PROCESS_DEBUG_EVENT &&
                exit.continued && exit.continuationResult &&
                exit.exitingMemberOrdinal<stateOriginal->memberCount &&
                stateOriginal->members[exit.exitingMemberOrdinal].continuedExit;
        }
        return false;
    }
};
class OriginalCoreClrControllerCustody {
    std::unique_ptr<OriginalCoreClrThreadCustody[]> threads_;
    std::unique_ptr<OriginalCoreClrContextChange[]> changes_;
    SIZE_T threadCapacity_=0,threadCount_=0,changeCapacity_=0,changeCount_=0;
    bool exhausted_=false,unknown_=false;
    const Event* overflowOriginal_=nullptr;
    const Event* firstUnresolvedOriginal_=nullptr;
    bool Unknown(const Event& saved) noexcept {
        unknown_=true;if(!firstUnresolvedOriginal_)firstUnresolvedOriginal_=&saved;return false;
    }
    OriginalCoreClrThreadCustody* Find(const State& state,const Event& born) noexcept {
        for(SIZE_T i=0;i<threadCount_;++i)
            if(threads_[i].stateOriginal==&state && threads_[i].createOriginal==&born)return &threads_[i];
        return nullptr;
    }
public:
    bool preownAttempted=false,preowned=false;
    std::exception_ptr preownExceptionOriginal;
    OriginalCoreClrControllerCustody(SIZE_T threads,SIZE_T changes):threadCapacity_(threads),changeCapacity_(changes) {}
    bool PreownOriginal() noexcept {
        if(preownAttempted)return false;preownAttempted=true;
        try {
        const SIZE_T threads=threadCapacity_,changes=changeCapacity_;
        if(!threads || threads>ResourceLimit || !changes ||
            threads>SIZE_MAX/sizeof(OriginalCoreClrThreadCustody) ||
            changes>SIZE_MAX/sizeof(OriginalCoreClrContextChange))
            throw std::invalid_argument("complete original CoreCLR custody geometry");
        const SIZE_T t=threads*sizeof(OriginalCoreClrThreadCustody),r=changes*sizeof(OriginalCoreClrContextChange);
        if(t>SIZE_MAX-sizeof(*this) || r>SIZE_MAX-sizeof(*this)-t)
            throw std::invalid_argument("aggregate original CoreCLR custody backing");
        threads_=std::make_unique<OriginalCoreClrThreadCustody[]>(threads);
        changes_=std::make_unique<OriginalCoreClrContextChange[]>(changes);
        preowned=true;return true;
        }catch(...){preownExceptionOriginal=std::current_exception();return false;}
    }
    // Called only by the creating debugger thread, before this exact saved
    // event's decision. No worker may mutate counts or context-change cells.
    bool ObserveSavedEvent(const State& state,const Event& saved) noexcept {
        if(!preowned)return false;
        if(!state.eventCount || &saved!=&state.events[state.eventCount-1] || saved.continued)return false;
        const DWORD kind=saved.original.dwDebugEventCode;
        if(kind==CREATE_PROCESS_DEBUG_EVENT || kind==CREATE_THREAD_DEBUG_EVENT) {
            if(Find(state,saved))return !unknown_ && !exhausted_; // Retained failure revisit, no new owner.
            if(!OriginalStoppedThreadOccurrenceFor(state,saved,saved)){return Unknown(saved);}
            if(threadCount_==threadCapacity_) {
                exhausted_=true;if(!overflowOriginal_)overflowOriginal_=&saved;return false;
            }
            auto& row=threads_[threadCount_++];row.stateOriginal=&state;row.createOriginal=&saved;row.enrolled=true;
        } else if(kind==EXIT_THREAD_DEBUG_EVENT) {
            if(saved.exitingThreadCreateEvent>=state.eventCount-1){return Unknown(saved);}
            const auto& born=state.events[saved.exitingThreadCreateEvent];auto* row=Find(state,born);
            if(!row || !born.threadExitObserved || row->lastThreadProcessExitOriginal ||
                (row->exitThreadOriginal && row->exitThreadOriginal!=&saved) ||
                born.original.dwProcessId!=saved.original.dwProcessId || born.original.dwThreadId!=saved.original.dwThreadId) {
                return Unknown(saved);
            }
            row->exitThreadOriginal=&saved;
        } else if(kind==EXIT_PROCESS_DEBUG_EVENT) {
            const SIZE_T ordinal=state.eventCount-1;
            if(saved.exitingMemberOrdinal>=state.memberCount ||
                state.members[saved.exitingMemberOrdinal].process!=saved.exitingProcessResource) {
                return Unknown(saved);
            }
            // Windows reports the final thread through EXIT_PROCESS. Retain
            // that actual event; never synthesize an EXIT_THREAD occurrence.
            for(SIZE_T i=0;i<threadCount_;++i) {
                auto& row=threads_[i];if(row.stateOriginal!=&state || !row.createOriginal)continue;
                const auto& born=*row.createOriginal;
                if(born.createdThreadMemberOrdinal!=saved.exitingMemberOrdinal ||
                    born.processLastThreadExitEvent!=ordinal)continue;
                if(row.exitThreadOriginal || (row.lastThreadProcessExitOriginal && row.lastThreadProcessExitOriginal!=&saved)) {
                    return Unknown(saved);
                }
                row.lastThreadProcessExitOriginal=&saved;
            }
        }
        return !unknown_ && !exhausted_;
    }
    OriginalCoreClrContextChange* ReserveChangeOriginal() noexcept {
        if(!preowned)return nullptr;
        if(unknown_ || exhausted_)return nullptr;
        if(changeCount_==changeCapacity_){exhausted_=true;return nullptr;}
        return &changes_[changeCount_++]; // Reservation precedes the native attempt.
    }
    OriginalCoreClrContextChange* ReserveInstallationOriginal(const State& state,const Event& born) noexcept {
        auto* thread=Find(state,born);
        if(!thread || thread->installationOriginal || thread->exitThreadOriginal || thread->lastThreadProcessExitOriginal)return nullptr;
        auto* row=ReserveChangeOriginal();if(row)thread->installationOriginal=row;return row;
    }
    OriginalCoreClrContextChange* ReserveDisarmOriginal(const State& state,const Event& born) noexcept {
        auto* thread=Find(state,born);
        if(!thread || !thread->installationOriginal || thread->disarmOriginal)return nullptr;
        auto* row=ReserveChangeOriginal();if(row)thread->disarmOriginal=row;return row;
    }
    OriginalCoreClrContextChange* ReservePairInstallationOriginal(const State& state,const Event& born,
            const Event& imageEvent,const LoadedImageObservation& image) noexcept {
        if(imageEvent.original.dwProcessId!=born.original.dwProcessId||
           (imageEvent.original.dwDebugEventCode!=LOAD_DLL_DEBUG_EVENT&&
            imageEvent.original.dwDebugEventCode!=CREATE_PROCESS_DEBUG_EVENT))return nullptr;
        bool enrolled=false;for(SIZE_T i=0;i<state.eventCount;++i)if(&state.events[i]==&imageEvent){enrolled=true;break;}
        if(!enrolled)return nullptr;
        auto* row=ReserveInstallationOriginal(state,born);
        auto* thread=Find(state,born);
        if(row && thread){thread->installationImageEventOriginal=&imageEvent;
            thread->installationImageOriginal=&image;thread->currentPairOriginal=row;}
        return row;
    }
    OriginalCoreClrContextChange* ReservePairMovementOriginal(const State& state,const Event& born) noexcept {
        auto* thread=Find(state,born);
        if(!thread || !thread->installationOriginal || !thread->currentPairOriginal || thread->disarmOriginal ||
            thread->exitThreadOriginal || thread->lastThreadProcessExitOriginal ||
            !thread->currentPairOriginal->effectiveVerified)return nullptr;
        auto* row=ReserveChangeOriginal();
        // Attach BEFORE native effect. A failed/unknown new row replaces the
        // current owner and prevents subsequent moves, while retaining lineage.
        if(row){row->precedingPairOriginal=thread->currentPairOriginal;thread->currentPairOriginal=row;}
        return row;
    }
    const OriginalCoreClrContextChange* CurrentPairOriginal(const State& state,const Event& born) noexcept {
        const auto* thread=Find(state,born);return thread?thread->currentPairOriginal:nullptr;
    }
    bool TerminalOriginalCoversPairBorrowers(const State& state,const Event& saved) const noexcept {
        if(!state.eventCount||&saved!=&state.events[state.eventCount-1]||saved.continued||
           (saved.original.dwDebugEventCode!=EXIT_THREAD_DEBUG_EVENT&&saved.original.dwDebugEventCode!=EXIT_PROCESS_DEBUG_EVENT))return false;
        if(saved.original.dwDebugEventCode==EXIT_THREAD_DEBUG_EVENT){
            if(saved.exitingThreadCreateEvent>=state.eventCount-1)return false;
            const auto& born=state.events[saved.exitingThreadCreateEvent];bool joined=false;
            if(!born.threadExitObserved||born.original.dwProcessId!=saved.original.dwProcessId||
               born.original.dwThreadId!=saved.original.dwThreadId)return false;
            for(SIZE_T i=0;i<threadCount_;++i)if(threads_[i].stateOriginal==&state&&
                threads_[i].createOriginal==&born&&threads_[i].exitThreadOriginal==&saved)joined=true;
            if(!joined)return false;
        }else {
            if(saved.exitingMemberOrdinal>=state.memberCount||saved.exitingProcessResource>=state.resourceCount)return false;
            const auto& member=state.members[saved.exitingMemberOrdinal];
            const auto& process=state.resources[saved.exitingProcessResource];
            if(member.pid!=saved.original.dwProcessId||member.process!=saved.exitingProcessResource||!member.exitEvent||
               !positive(member.birth)||process.kind!=Kind::Process||!process.acquired||process.unknownOutput)return false;
        }
        for(SIZE_T i=0;i<threadCount_;++i){const auto& thread=threads_[i];
            if(thread.stateOriginal!=&state||!thread.createOriginal||
               thread.createOriginal->original.dwProcessId!=saved.original.dwProcessId||
               !thread.installationOriginal||!thread.installationOriginal->setAttempted||thread.NativeTerminalContinued())continue;
            if(thread.disarmOriginal&&thread.disarmOriginal->effectiveVerified)continue;
            if(saved.original.dwDebugEventCode==EXIT_THREAD_DEBUG_EVENT&&thread.exitThreadOriginal==&saved)continue;
            if(saved.original.dwDebugEventCode==EXIT_PROCESS_DEBUG_EVENT&&thread.lastThreadProcessExitOriginal==&saved)continue;
            return false;
        }
        return true; // Eligible to attempt original Continue, never continued already.
    }
    bool DisarmStoppedPairsOriginal(const State& state,const Event& saved,
            template_creator_bridge::OriginalWindowsPendingCalls& pending,
            const template_creator_bridge::WindowsOriginalCommand& command) noexcept {
        if(!state.eventCount||&saved!=&state.events[state.eventCount-1]||saved.continued||
           saved.original.dwDebugEventCode==EXCEPTION_DEBUG_EVENT||!pending.CommandNativeCallsSettled(&command))return false;
        for(SIZE_T i=0;i<threadCount_;++i){auto& thread=threads_[i];
            if(thread.stateOriginal!=&state||!thread.createOriginal||
               thread.createOriginal->original.dwProcessId!=saved.original.dwProcessId||
               !thread.installationOriginal||!thread.installationOriginal->setAttempted||thread.NativeTerminalContinued())continue;
            // Only these actual saved terminal occurrences can settle through
            // their future real Continue result. They are not already terminal.
            if((saved.original.dwDebugEventCode==EXIT_THREAD_DEBUG_EVENT&&thread.exitThreadOriginal==&saved)||
               (saved.original.dwDebugEventCode==EXIT_PROCESS_DEBUG_EVENT&&thread.lastThreadProcessExitOriginal==&saved))continue;
            if(thread.disarmOriginal){if(!thread.disarmOriginal->effectiveVerified)return false;continue;}
            if(!thread.currentPairOriginal||!thread.currentPairOriginal->effectiveVerified||
               thread.currentPairOriginal->kind==OriginalCoreClrContextChange::Kind::None||
               thread.currentPairOriginal->before.stateOriginal!=&state||
               thread.currentPairOriginal->before.commandOriginal!=&command||
               thread.currentPairOriginal->before.pendingOwnerOriginal!=&pending||
               thread.installationOriginal->before.commandOriginal!=&command||
               thread.installationOriginal->before.pendingOwnerOriginal!=&pending)return false;
            auto* disarm=ReserveDisarmOriginal(state,*thread.createOriginal);
            if(!disarm||!disarm->DisarmPair(state,saved,*thread.currentPairOriginal,pending,command))return false;
        }
        return pending.CommandNativeCallsSettled(&command);
    }
    bool PrepareOwnedCleanupOriginal(const State& state,const Event& saved,const Event& born,
            template_creator_bridge::OriginalWindowsPendingCalls& pending,
            const template_creator_bridge::WindowsOriginalCommand& command) noexcept {
        auto* thread=Find(state,born);
        if(!thread || thread->cleanupEventOriginal || thread->disarmOriginal ||
            !thread->installationOriginal || !thread->currentPairOriginal ||
            !thread->currentPairOriginal->effectiveVerified ||
            !pending.CommandNativeCallsSettled(&command))return false;
        const auto* pair=thread->currentPairOriginal;
        thread->cleanupEventOriginal=&saved; // Retain before first native capture.
        if(!thread->cleanupContext.Capture(state,saved,pending,command,&born))return false;
#if defined(_M_AMD64) || defined(__x86_64__)
        const DWORD64 rip=thread->cleanupContext.effective.Rip;
        const unsigned expected=rip==pair->address?pair->slot:
            (pair->movableEnabled && rip==pair->movableAddress?pair->movableSlot:4);
        if(!thread->cleanupCause.Observe(state,saved,thread->cleanupContext,*pair,expected))return false;
        auto* disarm=ReserveDisarmOriginal(state,born);
        if(!disarm || !disarm->DisarmPair(state,saved,*pair,pending,command) ||
            !OriginalCoreClrContextChange::GeneralEqual(thread->cleanupContext.effective,disarm->before.effective) ||
            !OriginalCoreClrContextChange::DebugEqual(thread->cleanupContext.effective,disarm->before.effective))return false;
        return pending.CommandNativeCallsSettled(&command);
#else
        return false;
#endif
    }
    bool SlotBorrowersSettled(const State& state) const noexcept {
        for(SIZE_T i=0;i<threadCount_;++i) {
            const auto& thread=threads_[i];if(thread.stateOriginal!=&state)continue;
            const auto* install=thread.installationOriginal;
            if(!install || !install->setAttempted || thread.NativeTerminalContinued())continue;
            const auto* disarm=thread.disarmOriginal;
            const bool pair=install->kind==OriginalCoreClrContextChange::Kind::InstallPair;
            if(!disarm || disarm->kind!=(pair?OriginalCoreClrContextChange::Kind::DisarmPair:OriginalCoreClrContextChange::Kind::Disarm) ||
                disarm->installationOriginal!=install || !disarm->effectiveVerified)return false;
        }
        return true;
    }
    bool ValidateOwnedCleanupOriginal(const State& state,const Event& saved,
            const template_creator_bridge::OriginalWindowsPendingCalls& pending,
            const template_creator_bridge::WindowsOriginalCommand& command) const noexcept {
        if(!state.eventCount || &saved!=&state.events[state.eventCount-1] || saved.continued ||
            !pending.CommandNativeCallsSettled(&command))return false;
        const OriginalCoreClrThreadCustody* found=nullptr;
        for(SIZE_T i=0;i<threadCount_;++i) {
            const auto& row=threads_[i];
            if(row.stateOriginal!=&state || row.cleanupEventOriginal!=&saved)continue;
            if(found)return false;found=&row;
        }
        if(!found || !found->createOriginal || !found->installationOriginal || !found->currentPairOriginal ||
            !found->disarmOriginal || !found->cleanupCause.ownedOnlyObserved ||
            found->cleanupCause.stateOriginal!=&state || found->cleanupCause.stoppedOriginal!=&saved ||
            found->cleanupCause.contextOriginal!=&found->cleanupContext ||
            found->cleanupCause.pairOriginal!=found->currentPairOriginal ||
            !found->cleanupContext.captured || found->cleanupContext.stateOriginal!=&state ||
            found->cleanupContext.stoppedOriginal!=&saved ||
            found->cleanupContext.threadCreateOriginal!=found->createOriginal ||
            !OriginalStoppedThreadOccurrenceFor(state,saved,*found->createOriginal))return false;
        const auto& disarm=*found->disarmOriginal;
        return OwnsChangeOriginal(disarm) && OwnsChangeOriginal(*found->currentPairOriginal) &&
            OwnsChangeOriginal(*found->installationOriginal) &&
            found->installationOriginal->kind==OriginalCoreClrContextChange::Kind::InstallPair &&
            disarm.kind==OriginalCoreClrContextChange::Kind::DisarmPair &&
            disarm.precedingPairOriginal==found->currentPairOriginal &&
            disarm.installationOriginal==found->installationOriginal && disarm.effectiveVerified &&
            disarm.before.captured && disarm.after.captured &&
            disarm.before.stateOriginal==&state && disarm.after.stateOriginal==&state &&
            disarm.before.stoppedOriginal==&saved && disarm.after.stoppedOriginal==&saved &&
            disarm.before.threadCreateOriginal==found->createOriginal &&
            disarm.after.threadCreateOriginal==found->createOriginal;
    }
    bool OwnsChangeOriginal(const OriginalCoreClrContextChange& row) const noexcept {
        for(SIZE_T i=0;i<changeCount_;++i)if(&changes_[i]==&row)return true;
        return false;
    }
    bool ReservedSourceBytes(SIZE_T& bytes) const noexcept {
        bytes=sizeof(*this);
        if((threads_&&threadCapacity_>SIZE_MAX/sizeof(OriginalCoreClrThreadCustody))||
            (changes_&&changeCapacity_>SIZE_MAX/sizeof(OriginalCoreClrContextChange)))return false;
        const SIZE_T t=threads_?threadCapacity_*sizeof(OriginalCoreClrThreadCustody):0;
        const SIZE_T r=changes_?changeCapacity_*sizeof(OriginalCoreClrContextChange):0;
        if(t>SIZE_MAX-bytes)return false;bytes+=t;if(r>SIZE_MAX-bytes)return false;bytes+=r;return true;
    }
    const OriginalCoreClrThreadCustody* PrivateThreads() const noexcept{return threads_.get();}
    const OriginalCoreClrContextChange* PrivateChanges() const noexcept{return changes_.get();}
    SIZE_T PrivateThreadCount() const noexcept{return threadCount_;}
    SIZE_T RetainedThreadCapacityOriginal() const noexcept{return threads_?threadCapacity_:0;}
    SIZE_T RetainedChangeCapacityOriginal() const noexcept{return changes_?changeCapacity_:0;}
    SIZE_T PrivateChangeCount() const noexcept{return changeCount_;}
    bool UnresolvedSourceState() const noexcept{return unknown_ || exhausted_;}
public:
    bool OutputBackingSeparateOriginal(const void* output,std::size_t bytes) const noexcept {
        return OriginalRetainedStorageSeparate(output,bytes,this,1,sizeof(*this))&&OriginalRetainedArraySeparate(output,bytes,threads_,threadCapacity_)&&
            OriginalRetainedArraySeparate(output,bytes,changes_,changeCapacity_);
    }
};
}

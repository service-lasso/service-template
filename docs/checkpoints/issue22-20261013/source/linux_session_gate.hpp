// UNCOMPILED concrete session gate composition. Initial source associations
// never stand in for immutable kernel execution-view or native ROOT admission.
#pragma once
#ifndef _WIN32
#include "native_linux_platform.hpp"
#include "linux_memory.hpp"
#include "linux_elf_interpreter_source.hpp"
#ifdef TEMPLATE_NATIVE_ROOT_MEMORY_REGRESSION
#include "linux_memory_regression_receipt.hpp"
#endif
#include <poll.h>
#include <sys/sysmacros.h>
namespace template_creator_bridge {
struct LinuxGateTaskOriginal {
    std::unique_ptr<template_creator_linux::HeldTaskIdentity> task;
    const template_creator_linux::Tracee* occurrence=nullptr;
    const template_creator_linux::ProcTaskObservation* association=nullptr;
    bool bindAttempted=false,bound=false;
};
struct LinuxGatePollOriginal {
    template_creator_linux::SupervisorNativeOriginal call;
    struct pollfd descriptor{};
};
struct LinuxGateWaitOriginal {
    std::unique_ptr<template_creator_linux::ProcTaskObservation> native;
    const template_creator_linux::Tracee* tracee=nullptr;
    const template_creator_linux::Event* event=nullptr;
    const template_creator_linux::ProcTaskObservation* observation=nullptr;
    bool attempted=false,associated=false,unseizedAtomicOriginal=false;
};
struct LinuxRootExecRequestOriginal {
    const template_creator_linux::State* state=nullptr;
    const template_creator_linux::Tracee* tracee=nullptr;
    const template_creator_linux::Event* saved=nullptr;
    const template_creator_linux::ChildNativeOutcome* childAttempt=nullptr;
    const template_creator_linux::OriginalLinuxHeldFile* launchFileOriginal=nullptr;
    const template_creator_linux::OriginalLinuxCommandDirectory* directoryOriginal=nullptr;
    size_t launchFileOrdinal=SIZE_MAX;
    int launchDescriptor=-1,directoryDescriptor=-1;
    std::array<uint64_t,3> syscallArguments{},sourceAddresses{};
    size_t attemptMatches=0,childSequenceCount=0;
    uint64_t childCaptureBegin=0,childCaptureEnd=0;
    std::array<bool,5> childSpanMatched{};
    std::array<bool,5> childSharedWriterDisjoint{};
    uintptr_t childSharedWriterBegin=0,childSharedWriterEnd=0;
    bool childSharedWriterAttempted=false,childSharedWriterObserved=false;
    size_t childArgumentPrivateRegionCount=0;
    bool childArgumentMappingsAttempted=false,childArgumentMappingsObserved=false;
    bool attempted=false,originalRequestMatched=false,ownChildSequenceMatched=false;
    bool childMemoryAttempted=false,childMemoryMatched=false;
    bool sourceCapabilitiesAttempted=false,sourceCapabilitiesMatched=false;
    std::array<const template_creator_linux::SetupOutcome*,4> childPipeOriginals{};
    std::array<const template_creator_linux::Fd*,4> childPipeSourceOwners{};
    std::array<const template_creator_linux::Fd*,6> childDuplicateParentCloseOwners{};
    std::array<size_t,4> childPipeOrdinals{SIZE_MAX,SIZE_MAX,SIZE_MAX,SIZE_MAX};
    bool ownPipeSourcesAttempted=false,ownPipeSourcesObserved=false;
    bool regressionAttempted=false,regressionReturned=false,regressionResult=false;
};
class OriginalLinuxSessionGate final:public template_creator_linux::Gate {
    NativePlatformState& platform_;
    LinuxInputOwner& input_;
    const template_creator_linux::OriginalRouteCommand& command_;
    std::unique_ptr<LinuxGateTaskOriginal[]> tasks_;
    std::array<LinuxGatePollOriginal,2> rootPolls_{};
    std::array<LinuxGateWaitOriginal,template_creator_linux::EventLimit> waits_{};
    const template_creator_linux::State* state_=nullptr;
    enum class RootReadPhase { NotObserved, ReadEntered, ReturnedToTrampoline, MatchedAtCloseEntry,
        Close4Returned, DirectoryEntered, DirectoryReturned, Close5Entered, Close5Returned };
    RootReadPhase rootReadPhase_=RootReadPhase::NotObserved;
    size_t rootReadEvent_=template_creator_linux::EventLimit,rootCloseEntryEvent_=template_creator_linux::EventLimit;
    size_t rootReadEntryEvent_=template_creator_linux::EventLimit;
    size_t trampolineEvent_=template_creator_linux::EventLimit;
    LinuxRootExecRequestOriginal rootExecRequest_;
    enum class RootExecStage { NotObserved, RequestValidated, ExecStopObserved };
    RootExecStage rootExecStage_=RootExecStage::NotObserved;
    const template_creator_linux::Event* kernelExeStopOriginal_=nullptr;
    const template_creator_linux::ProcTaskObservation* kernelExeTaskOriginal_=nullptr;
    bool kernelExeAttempted_=false,kernelExeCaptured_=false,kernelExeMatched_=false;
    template_creator_linux::OriginalLinuxElfInterpreterSource launchElfSource_,kernelElfSource_;
    const template_creator_linux::OriginalLinuxHeldFile* interpreterSourceFileOriginal_=nullptr;
    PyObject* interpreterRequirementOriginal_=nullptr;
    size_t interpreterSourceMatchCount_=0;
    std::unique_ptr<template_creator_linux::OriginalLinuxMappedMemory> rootMemory_,execMemory_;
    template_creator_linux::OriginalLinuxElfInterpreterSource interpreterElfSource_;
    template_creator_linux::SupervisorNativeOriginal interpreterRegistersCall_;
    pid_t interpreterRegistersTidOriginal_=-1;
#if defined(__x86_64__)
    user_regs_struct interpreterRegisters_{};
#endif
    size_t interpreterMappingOrdinal_=SIZE_MAX,interpreterPrefixBytes_=0;
    uint64_t interpreterEntryFileOffset_=0;
    bool interpreterEntryAttempted_=false,interpreterEntryMatched_=false;
    bool interpreterBackingAttempted_=false,interpreterBackingPrepared_=false;
    size_t interpreterSpanFactor_=0,interpreterBackingSourceBytes_=0,interpreterBackingOverlapSourceBytes_=0;
    uint64_t interpreterLoadBias_=0,interpreterMatchedFileBytes_=0;
    size_t interpreterCapturedLoadSpans_=0;
    size_t interpreterMappedLoadSegmentsObserved_=0,interpreterLoadSegmentsMatched_=0,interpreterFirstFailedLoadOrdinal_=SIZE_MAX;
    bool interpreterFileSpansAttempted_=false,interpreterFileSpansMatched_=false;
    size_t interpreterMemorySegmentsMapped_=0,interpreterTailSpansObserved_=0,interpreterNonzeroTailSpans_=0;
    uint64_t interpreterTailBytesObserved_=0;
    bool interpreterMemorySpansCaptured_=false,interpreterTailsObserved_=false;
    bool kernelElfCaptureAttempted_=false,kernelElfCaptureReturned_=false,kernelElfCaptureResult_=false;
    bool interpreterEntryCaptureAttempted_=false,interpreterEntryCaptureReturned_=false,interpreterEntryCaptureResult_=false;
    bool CaptureInterpreterEntryCandidate(const template_creator_linux::Tracee&,
        const template_creator_linux::Event&,const template_creator_linux::State&) noexcept;
    bool rootBindingAttempted_=false,failed_=false,taskDisposalRequested_=false,consumptionUnimplemented_=false;
    static bool SameVector(const char* const* actual,char* const* original,size_t count) noexcept {
        if(!actual||!original)return false;
        for(size_t i=0;i<count;++i)if(!actual[i]||std::strcmp(actual[i],original[i]))return false;
        return actual[count]==nullptr&&original[count]==nullptr;
    }
    bool LiveRootPidfd(LinuxGatePollOriginal&,const template_creator_linux::State&) noexcept;
    size_t OriginalOrdinal(const template_creator_linux::Tracee&,const template_creator_linux::State&) const noexcept;
    bool ExactEvent(const template_creator_linux::Event&,const template_creator_linux::State&) const noexcept;
    bool SameWaitOccurrence(const LinuxGateWaitOriginal&,const LinuxGateTaskOriginal&) const noexcept;
    bool ValidateRootExecRequestOriginal(const template_creator_linux::Tracee& tracee,
        const template_creator_linux::Event& event,const template_creator_linux::State& state,
        template_creator_linux::RestartAdmissionCut cut=template_creator_linux::RestartAdmissionCut::BeforeReservation) const noexcept {
        using namespace template_creator_linux;
        const auto& request=rootExecRequest_;const auto& plan=command_.plan;
        if(failed_||state.failed||platform_.pending.Failed()||taskDisposalRequested_||state_!=&state||!ExactEvent(event,state)||
           state.traceeCount!=1||OriginalOrdinal(tracee,state)!=0||tracee.terminal||tracee.supersededByExec||
           request.state!=&state||request.tracee!=&tracee||request.saved!=&event||
           !request.attempted||!request.originalRequestMatched||!request.ownChildSequenceMatched||
           !SameOwnChildExecSequence(event,state)||!ValidateOwnPipeSources(request,state)||
           !request.sourceCapabilitiesAttempted||!request.sourceCapabilitiesMatched||
           !request.childMemoryAttempted||!request.childMemoryMatched||
           !request.childSharedWriterAttempted||!request.childSharedWriterObserved||
           !request.childArgumentMappingsAttempted||!request.childArgumentMappingsObserved||
           !rootMemory_||rootMemory_->Failed()||!rootMemory_->RetainedStopStillObserved()||
           rootMemory_->OriginalStoppedState()!=&state||rootMemory_->OriginalStoppedTracee()!=&tracee||
           rootMemory_->OriginalStoppedEvent()!=&event||
           platform_.pending.OriginalSupervisorOwner().HasPendingOriginalNativeCall()||
           platform_.inputDisposalStarted||!input_.sourceAssociated||!input_.captureClean||!input_.launchFileAssociated||!plan.copied||
           request.launchFileOrdinal!=input_.launchFileOrdinal||request.launchFileOrdinal>=input_.count||
           input_.files[request.launchFileOrdinal].get()!=request.launchFileOriginal||
           !request.launchFileOriginal||!request.launchFileOriginal->CapturedSourceBytes()||
           request.launchFileOriginal->OriginalDescriptor()!=request.launchDescriptor||
           !platform_.initialObjects.SameCapturedTool(*request.launchFileOriginal)||
           input_.cwd.get()!=request.directoryOriginal||!request.directoryOriginal||
           !request.directoryOriginal->Captured()||request.directoryOriginal->OriginalDescriptor()!=request.directoryDescriptor||
           !RetainedBirthAssociation(tracee,state)||!waits_[state.eventCount-1].associated||
           !SameWaitOccurrence(waits_[state.eventCount-1],tasks_[0])||
           !event.waitAttempted||event.returnedTid!=tracee.tid||!OriginalRestartAdmissionShape(event,cut)||
           !WIFSTOPPED(event.status)||event.nativeStop!=NativeStopKind::SyscallEntry||
           !event.syscallInfoShape||event.syscallInfo.op!=PTRACE_SYSCALL_INFO_ENTRY||
           event.syscallInfo.arch!=AUDIT_ARCH_X86_64||event.syscallInfo.entry.nr!=SYS_execve||
           !event.registersCaptured)return false;
        for(bool matched:request.childSpanMatched)if(!matched)return false;
        for(bool disjoint:request.childSharedWriterDisjoint)if(!disjoint)return false;
        if(request.sourceAddresses!=std::array<uint64_t,3>{reinterpret_cast<uintptr_t>(state.originalImage.data()),
             reinterpret_cast<uintptr_t>(state.originalArgv.data()),reinterpret_cast<uintptr_t>(state.originalEnvironment.data())}||
           request.syscallArguments!=request.sourceAddresses)return false;
        for(size_t i=0;i<3;++i)if(event.syscallInfo.entry.args[i]!=request.syscallArguments[i])return false;
        return !std::strcmp(state.originalImage.data(),plan.image.data())&&
            !std::strcmp(state.originalCwd.data(),plan.cwd.data())&&
            !std::strcmp(state.originalCwd.data(),request.directoryOriginal->PrivateOriginalPath())&&
            SameVector(state.originalArgv.data(),plan.argv.data(),plan.argumentCount)&&
            SameVector(state.originalEnvironment.data(),plan.environment.data(),plan.environmentCount);
    }
    bool CaptureRootExecChildMemory(const template_creator_linux::Tracee& tracee,
            const template_creator_linux::Event& event,const template_creator_linux::State& state) noexcept {
        auto& request=rootExecRequest_;
        if(request.childMemoryAttempted||!request.originalRequestMatched||request.state!=&state||
           request.tracee!=&tracee||request.saved!=&event||!rootMemory_||
           !ExactEvent(event,state)||!waits_[state.eventCount-1].observation||
           !SameWaitOccurrence(waits_[state.eventCount-1],tasks_[0]))return false;
        request.childMemoryAttempted=true;
        if(!state.imageBytes||state.imageBytes>state.originalImage.size()||
           state.argumentBytes>state.originalArgBytes.size()||state.environmentBytes>state.originalEnvBytes.size()||
           state.argumentCount>=state.originalArgv.size()||state.environmentCount>=state.originalEnvironment.size())return false;
        struct Span {const void* source;size_t bytes;};
        const std::array<Span,5> spans{{{state.originalImage.data(),state.imageBytes},
            {state.originalArgv.data(),(state.argumentCount+1)*sizeof(char*)},
            {state.originalEnvironment.data(),(state.environmentCount+1)*sizeof(char*)},
            {state.originalArgBytes.data(),state.argumentBytes},{state.originalEnvBytes.data(),state.environmentBytes}}};
        // The child's explicitly writable observation mapping is an own writer,
        // even on the selected trusted operator host. Retain its actual mmap
        // extent separately from the five kernel argument spans; no pointer
        // equality or external actor premise substitutes for this exclusion.
        request.childSharedWriterAttempted=true;
        if(!state.childMapAttempted||state.originalChildMapResult==MAP_FAILED||
           !state.originalChildMapResult||!state.childShared||state.childUnmapped||
           state.childUnmapAttempted||!state.childMapBytes||
           state.originalCloneArguments.flags!=CLONE_PIDFD||
           state.childMapAddressOriginal!=nullptr||state.childMapProtectionOriginal!=(PROT_READ|PROT_WRITE)||
           state.childMapFlagsOriginal!=(MAP_SHARED|MAP_ANONYMOUS)||
           state.childMapFdOriginal!=-1||state.childMapOffsetOriginal!=0||
           static_cast<void*>(state.childShared)!=state.originalChildMapResult)return false;
        const auto sharedBegin=reinterpret_cast<uintptr_t>(state.originalChildMapResult);
        if(state.childMapBytes>UINTPTR_MAX-sharedBegin)return false;
        request.childSharedWriterBegin=sharedBegin;
        request.childSharedWriterEnd=sharedBegin+state.childMapBytes;
        bool disjoint=true;
        for(size_t i=0;i<spans.size();++i){
            const auto address=reinterpret_cast<uintptr_t>(spans[i].source);
            if(spans[i].bytes>UINTPTR_MAX-address)return false;
            request.childSharedWriterDisjoint[i]=!spans[i].bytes||
                address>=request.childSharedWriterEnd||address+spans[i].bytes<=sharedBegin;
            disjoint=request.childSharedWriterDisjoint[i]&&disjoint;
        }
        request.childSharedWriterObserved=disjoint;
        if(!disjoint)return false;
        uint64_t begin=UINT64_MAX,end=0;
        for(const auto& span:spans){
            if(!span.bytes)continue;
            const auto address=reinterpret_cast<uintptr_t>(span.source);
            if(span.bytes>UINT64_MAX-address)return false;
            if(address<begin)begin=address;if(address+span.bytes>end)end=address+span.bytes;
        }
        if(begin>=end)return false;request.childCaptureBegin=begin;request.childCaptureEnd=end;
        const auto* currentTask=tasks_[0].task->Observe();
        if(!currentTask||!rootMemory_->CaptureMaps(*currentTask,state,tracee,event))return false;
        request.childArgumentMappingsAttempted=true;
        for(size_t i=0;i<rootMemory_->RegionCount();++i){
            const auto& region=rootMemory_->PrivateRegions()[i];
            struct Interval {uint64_t first,last;};std::array<Interval,5> intersections{};size_t count=0;
            for(const auto& span:spans){
                if(!span.bytes)continue;const uint64_t address=reinterpret_cast<uintptr_t>(span.source);
                const uint64_t first=region.begin>address?region.begin:address;
                const uint64_t limit=address+span.bytes,last=region.end<limit?region.end:limit;
                if(first<last)intersections[count++]={first,last};
            }
            if(count){
                // These are the fork-private creator-owned argument buffers,
                // not the separate MAP_SHARED child outcome producer. A shared
                // alias cannot inherit this source-owned argument relation.
                if(region.permissions[0]!='r'||region.permissions[3]!='p'||
                   request.childArgumentPrivateRegionCount==SIZE_MAX)return false;
                ++request.childArgumentPrivateRegionCount;
            }
            for(size_t j=0;j<count;++j)for(size_t k=j+1;k<count;++k)
                if(intersections[k].first<intersections[j].first){const auto old=intersections[j];intersections[j]=intersections[k];intersections[k]=old;}
            for(size_t j=0;j<count;){
                const uint64_t first=intersections[j].first;uint64_t last=intersections[j++].last;
                while(j<count&&intersections[j].first<=last){if(intersections[j].last>last)last=intersections[j].last;++j;}
                if(!rootMemory_->CaptureOwnedMappedSpan(i,first,last))return false;
            }
        }
        bool matched=true;
        for(size_t i=0;i<spans.size();++i){
            request.childSpanMatched[i]=rootMemory_->SameCapturedBytes(
                reinterpret_cast<uintptr_t>(spans[i].source),spans[i].source,spans[i].bytes);
            matched=request.childSpanMatched[i]&&matched;
        }
        request.childArgumentMappingsObserved=matched&&request.childArgumentPrivateRegionCount!=0;
        request.childMemoryMatched=matched;return matched&&request.childArgumentMappingsObserved;
    }
    static bool KnownOriginalParentClose(const template_creator_linux::Fd& owner) noexcept {
        return owner.original>=0 && owner.attempted && owner.closed && owner.closeResult==0 && !owner.closeError;
    }
    static bool ValidateOwnPipeSources(const LinuxRootExecRequestOriginal& request,
        const template_creator_linux::State& state) noexcept {
        if(request.state!=&state || !request.ownPipeSourcesAttempted || !request.saved ||
            state.setupOutcomeCount>state.setupOutcomes.size() || state.fdCount>state.fds.size() ||
            state.eventCount>state.events.size())return false;
        bool savedOwned=false;
        for(size_t i=0;i<state.eventCount;++i)if(&state.events[i]==request.saved)savedOwned=true;
        if(!savedOwned || !SameOwnChildExecSequence(*request.saved,state))return false;
        constexpr size_t slots[]={1,2,3,4};
        for(size_t role=0;role<4;++role){
            if(request.childPipeOrdinals[role]>=state.setupOutcomeCount ||
                request.childPipeOriginals[role]!=&state.setupOutcomes[request.childPipeOrdinals[role]])return false;
            const auto& row=*request.childPipeOriginals[role];
            if(!row.operation || std::strcmp(row.operation,role==3?"private_launch_pipe2":"pipe2") ||
                !row.attempted || row.result!=0 || row.originalErrno || row.descriptors[0]<0 ||
                row.descriptors[1]<0 || row.descriptors[0]==row.descriptors[1] ||
                row.descriptors[role==3?0:1]!=state.childSourceOriginals[slots[role]])return false;
            bool ownerFound=false;
            for(size_t fd=0;fd<state.fdCount;++fd)if(&state.fds[fd]==request.childPipeSourceOwners[role])ownerFound=true;
            if(!ownerFound || !KnownOriginalParentClose(*request.childPipeSourceOwners[role]) ||
                request.childPipeSourceOwners[role]->original!=state.childSourceOriginals[slots[role]])return false;
            for(size_t prior=0;prior<role;++prior)
                if(request.childPipeOriginals[prior]==request.childPipeOriginals[role] ||
                    request.childPipeSourceOwners[prior]==request.childPipeSourceOwners[role])return false;
        }
        for(size_t i=0;i<state.childSources.size();++i)
            if(state.childSources[i]>=state.fdCount ||
                request.childDuplicateParentCloseOwners[i]!=&state.fds[state.childSources[i]] ||
                !KnownOriginalParentClose(*request.childDuplicateParentCloseOwners[i]))return false;
        return true;
    }
#ifdef TEMPLATE_NATIVE_ROOT_MEMORY_REGRESSION
    static bool VerifyOwnPipeSourceRegression(const LinuxRootExecRequestOriginal& original,
        const template_creator_linux::State& state) noexcept {
        if(!original.ownPipeSourcesObserved || !ValidateOwnPipeSources(original,state))return false;
        // Nonowning view copies only: the genuine State/native rows stay intact.
        auto missing=original;missing.childPipeOriginals[0]=nullptr;
        if(ValidateOwnPipeSources(missing,state))return false;
        auto duplicate=original;duplicate.childPipeOriginals[1]=duplicate.childPipeOriginals[0];
        duplicate.childPipeOrdinals[1]=duplicate.childPipeOrdinals[0];
        if(ValidateOwnPipeSources(duplicate,state))return false;
        auto foreignOwner=original;foreignOwner.childPipeSourceOwners[0]=foreignOwner.childPipeSourceOwners[1];
        if(ValidateOwnPipeSources(foreignOwner,state))return false;
        auto missingClose=original;missingClose.childDuplicateParentCloseOwners[0]=nullptr;
        if(ValidateOwnPipeSources(missingClose,state))return false;
        // Scalar receipt surrogates test the exact production predicate only;
        // they are NOT genuine failed/uncertain native close observations.
        auto failedClose=*original.childPipeSourceOwners[0];failedClose.closed=false;failedClose.closeResult=-1;failedClose.closeError=EBADF;
        if(KnownOriginalParentClose(failedClose))return false;
        auto unknownClose=*original.childPipeSourceOwners[0];unknownClose.closed=false;unknownClose.closeResult=-1;unknownClose.closeError=EINTR;
        if(KnownOriginalParentClose(unknownClose))return false;
        auto absentClose=*original.childPipeSourceOwners[0];absentClose.attempted=false;
        return !KnownOriginalParentClose(absentClose);
    }
#endif
    static bool ObserveOwnPipeSources(LinuxRootExecRequestOriginal& request,
        const template_creator_linux::State& state) noexcept {
        if(request.ownPipeSourcesAttempted)return false;request.ownPipeSourcesAttempted=true;
        if(request.state!=&state || !request.ownChildSequenceMatched ||
            state.setupOutcomeCount>state.setupOutcomes.size() || state.fdCount>state.fds.size())return false;
        constexpr size_t sourceSlots[]={1,2,3,4};
        size_t ordinary=0,launch=0;
        for(size_t i=0;i<state.setupOutcomeCount;++i){
            const auto& row=state.setupOutcomes[i];if(!row.operation)continue;
            const bool ordinaryPipe=!std::strcmp(row.operation,"pipe2");
            const bool launchPipe=!std::strcmp(row.operation,"private_launch_pipe2");
            if(!ordinaryPipe && !launchPipe)continue;
            if(!row.attempted || row.result!=0 || row.originalErrno ||
                row.descriptors[0]<0 || row.descriptors[1]<0 || row.descriptors[0]==row.descriptors[1])return false;
            if(ordinaryPipe)++ordinary;else ++launch;
            for(size_t role=0;role<4;++role){
                if((role==3)!=launchPipe || row.descriptors[role==3?0:1]!=state.childSourceOriginals[sourceSlots[role]])continue;
                if(request.childPipeOriginals[role])return false;
                request.childPipeOriginals[role]=&row;request.childPipeOrdinals[role]=i;
            }
        }
        if(ordinary!=3 || launch!=1)return false;
        for(size_t i=0;i<state.childSources.size();++i){
            if(state.childSources[i]>=state.fdCount)return false;
            const auto& owner=state.fds[state.childSources[i]];
            request.childDuplicateParentCloseOwners[i]=&owner;
            if(!KnownOriginalParentClose(owner))return false;
        }
        for(size_t role=0;role<4;++role){
            if(!request.childPipeOriginals[role])return false;
            const int original=state.childSourceOriginals[sourceSlots[role]];
            for(size_t fd=0;fd<state.fdCount;++fd){
                if(state.fds[fd].original!=original)continue;
                if(request.childPipeSourceOwners[role])return false;
                request.childPipeSourceOwners[role]=&state.fds[fd];
            }
            const auto* owner=request.childPipeSourceOwners[role];
            if(!owner || !KnownOriginalParentClose(*owner))return false;
            for(size_t earlier=0;earlier<role;++earlier)
                if(request.childPipeOriginals[earlier]==request.childPipeOriginals[role] ||
                    request.childPipeSourceOwners[earlier]==owner)return false;
        }
        request.ownPipeSourcesObserved=ValidateOwnPipeSources(request,state);return request.ownPipeSourcesObserved;
        // These are original parent close receipts only. Child duplicates remain
        // separately owned until their genuine child close/exec/terminal cuts.
    }
    static bool SameOwnChildExecSequence(const template_creator_linux::Event& event,
            const template_creator_linux::State& state) noexcept {
        using namespace template_creator_linux;
        // These are source-owned original observations. They do not certify
        // private child memory, external actors, or a future kernel lookup.
        if(!state.childMapAttempted||state.originalChildMapResult==MAP_FAILED||
           !state.originalChildMapResult||!event.childPrefixMapLiveOriginal||
           event.childPrefixMapOriginal!=state.originalChildMapResult||
           event.childPrefixMapBytesOriginal!=state.childMapBytes||
           state.childShared!=state.originalChildMapResult||state.childMapBytes<sizeof(SharedChildOutcomes)||
           state.childMapAddressOriginal!=nullptr||state.childMapProtectionOriginal!=(PROT_READ|PROT_WRITE)||
           state.childMapFlagsOriginal!=(MAP_SHARED|MAP_ANONYMOUS)||state.childMapFdOriginal!=-1||
           state.childMapOffsetOriginal!=0||
           !event.childPrefixCaptureAttempted||!event.childPrefixCaptureReturned||
           event.childPrefixAfter!=15||event.childPrefixAfter>state.childOriginalCount||
           state.originalCloneArguments.flags!=CLONE_PIDFD||
           state.originalCloneArguments.exit_signal!=SIGCHLD||
           state.nativeCloneArguments.flags!=state.originalCloneArguments.flags||
           state.nativeCloneArguments.exit_signal!=state.originalCloneArguments.exit_signal||
           std::memcmp(&state.nativeCloneArguments,&state.originalCloneArguments,sizeof(state.originalCloneArguments))!=0||
           state.fdCount>state.fds.size()||state.rootPidfd>=state.fdCount||state.originalCloneResult<=0||
           state.originalClonePidfdOutput<0||
           state.originalCloneArguments.pidfd!=reinterpret_cast<uintptr_t>(&state.fds[state.rootPidfd].original)||
           state.nativeCloneArguments.pidfd!=state.originalCloneArguments.pidfd)return false;
        const std::array<int,15> operations{22,7,8,9,10,11,12,1,2,23,24,25,5,26,16};
        const std::array<long,15> results{0,0,1,2,3,4,5,0,0,1,1,0,0,0,0};
        for(size_t i=0;i<operations.size();++i){
            const auto& row=state.childOriginals[i];
            const bool returned=i!=9&&i!=14;
            if(row.operation!=operations[i]||row.originalResult!=results[i]||
               row.originalErrno!=0||row.nativeReturned!=returned)return false;
        }
        if(state.setupOutcomeCount>state.setupOutcomes.size())return false;
        size_t duplicates=0;
        for(size_t i=0;i<state.setupOutcomeCount;++i){
            const auto& row=state.setupOutcomes[i];
            if(!row.operation||std::strcmp(row.operation,"child_source_duplicate"))continue;
            if(duplicates==state.childSources.size()||!row.attempted||row.originalErrno!=0||
               state.childSources[duplicates]>=state.fdCount||
               row.descriptors[0]!=state.childSourceOriginals[duplicates]||
               row.result!=state.fds[state.childSources[duplicates]].original)return false;
            ++duplicates;
        }
        if(duplicates!=state.childSources.size())return false;
        for(size_t i=0;i<state.childSources.size();++i){
            if(state.childSources[i]>=state.fdCount||state.childSourceOriginals[i]<0||
               state.fds[state.childSources[i]].original<6)return false;
            for(size_t j=0;j<i;++j)
                if(state.fds[state.childSources[i]].original==state.fds[state.childSources[j]].original)return false;
        }
        return true;
    }
public:
    bool ActualExecutionQualificationComplete(const template_creator_linux::State& state) const noexcept {
        if(state_!=&state || rootExecRequest_.state!=&state || !rootExecRequest_.saved ||
             !rootExecRequest_.sourceCapabilitiesAttempted || !rootExecRequest_.sourceCapabilitiesMatched ||
             !rootExecRequest_.ownPipeSourcesAttempted || !rootExecRequest_.ownPipeSourcesObserved ||
             !ValidateOwnPipeSources(rootExecRequest_,state) ||
            !rootExecRequest_.originalRequestMatched || !rootExecRequest_.childMemoryMatched||
            !rootExecRequest_.childSharedWriterAttempted||!rootExecRequest_.childSharedWriterObserved||
            !rootExecRequest_.childArgumentMappingsAttempted||!rootExecRequest_.childArgumentMappingsObserved)return false;
        // Saved EXEC/map/entry-prefix observations are not kernel lookup,
        // interpreter/provider/actor qualification. The concrete consumer is
        // still required; this actual late-stage caller remains default-closed.
        return false;
    }
    LinuxInterpreterCorrespondence ObserveInterpreterRequirementOriginal(size_t ordinal,PyObject* originalRequirement) noexcept {
        if(ordinal>=input_.count||!input_.interpreterObservations)return LinuxInterpreterCorrespondence::Invalid;
        auto& observation=input_.interpreterObservations[ordinal];
        if(observation.attempted)return LinuxInterpreterCorrespondence::Invalid;
        observation.attempted=true;observation.requirementOriginal=originalRequirement;
        observation.result=LinuxInterpreterCorrespondence::Invalid;
        const auto finish=[&](LinuxInterpreterCorrespondence result) noexcept {
            observation.result=result;observation.returned=true;return result;
        };
        if(taskDisposalRequested_||input_.launchFileOrdinal>=input_.count||
           !input_.files[input_.launchFileOrdinal]||!input_.files[ordinal]||!originalRequirement)
            return finish(LinuxInterpreterCorrespondence::Invalid);
        observation.launchOriginal=input_.files[input_.launchFileOrdinal].get();
        observation.candidateOriginal=input_.files[ordinal].get();
        if(!launchElfSource_.attempted)launchElfSource_.Capture(*input_.files[input_.launchFileOrdinal]);
        const char* path=launchElfSource_.PrivateFirstInterpreterPath();
        const auto& candidate=*input_.files[ordinal];
        if(!launchElfSource_.projected||!candidate.CapturedSourceBytes())
            return finish(LinuxInterpreterCorrespondence::Invalid);
        if(!path||std::strcmp(path,candidate.PrivateOriginalPath()))
            return finish(LinuxInterpreterCorrespondence::Nonmatch);
        ++interpreterSourceMatchCount_;
        interpreterSourceFileOriginal_=&candidate;interpreterRequirementOriginal_=originalRequirement;
        // Actual bindings pass SAME retained requirement row with interpreter
        // role. This is source correspondence only, never kernel-open proof.
        return finish(LinuxInterpreterCorrespondence::Match);
    }
    bool PrepareInterpreterMemoryBackingOriginal() {
        if(interpreterBackingAttempted_)return false;interpreterBackingAttempted_=true;
        if(failed_||state_||taskDisposalRequested_||input_.launchFileOrdinal>=input_.count||
           !input_.files[input_.launchFileOrdinal])return false;
        if(!launchElfSource_.attempted)launchElfSource_.Capture(*input_.files[input_.launchFileOrdinal]);
        if(!launchElfSource_.projected)return false;
        if(!launchElfSource_.interpreterCount){interpreterBackingPrepared_=true;return true;}
        if(interpreterSourceMatchCount_!=1||!interpreterSourceFileOriginal_||!interpreterRequirementOriginal_||
           !interpreterElfSource_.Capture(*interpreterSourceFileOriginal_)||!interpreterElfSource_.header.e_phnum||
           !execMemory_||execMemory_->OriginalStoppedState()||execMemory_->RegionCount()||execMemory_->SpanCount())return false;
        interpreterSpanFactor_=interpreterElfSource_.header.e_phnum;
        const auto& g=platform_.gateGeometry;
        interpreterBackingSourceBytes_=template_creator_linux::OriginalLinuxMappedMemory::CheckedSourceBacking(
            platform_.pending.OriginalSupervisorOwner(),platform_.pending.OriginalDeadlineOwner(),
            g.mapsBytes,g.mapsReads,g.memoryRegions,g.memoryBytes,g.memoryReads,interpreterSpanFactor_);
        size_t existing=0;
        if(!ReservedSourceBytes(existing)||interpreterBackingSourceBytes_>SIZE_MAX-existing)return false;
        interpreterBackingOverlapSourceBytes_=existing+interpreterBackingSourceBytes_;
        // This exact old owner has never received a stopped State, so its source
        // methods cannot have acquired maps/mem or native borrowers. Do not reset
        // an observed/unknown owner. Preserve allocation errors in the caller.
        execMemory_.reset();
        execMemory_=std::make_unique<template_creator_linux::OriginalLinuxMappedMemory>(*tasks_[0].task,
            platform_.pending.OriginalSupervisorOwner(),platform_.pending.OriginalDeadlineOwner(),
            g.mapsBytes,g.mapsReads,g.memoryRegions,g.memoryBytes,g.memoryReads,interpreterSpanFactor_);
        interpreterBackingPrepared_=true;return true;
    }
    OriginalLinuxSessionGate(NativePlatformState& platform,LinuxInputOwner& input,size_t ordinal)
        :platform_(platform),input_(input),command_(platform.route.PrivateCommand(ordinal)) {
        const auto& g=platform.gateGeometry;
        const size_t taskBytes=template_creator_linux::HeldTaskIdentity::CheckedSourceBacking(
            1,g.statBytes,g.statusBytes,g.procReads);
        const size_t rootTaskBytes=template_creator_linux::HeldTaskIdentity::CheckedSourceBacking(
            3,g.statBytes,g.statusBytes,g.procReads);
        const size_t waitBytes=template_creator_linux::ProcTaskObservation::CheckedSourceBacking(
            g.statBytes,g.statusBytes,g.procReads);
        const size_t memoryBytes=template_creator_linux::OriginalLinuxMappedMemory::CheckedSourceBacking(
            platform.pending.OriginalSupervisorOwner(),platform.pending.OriginalDeadlineOwner(),
            g.mapsBytes,g.mapsReads,g.memoryRegions,g.memoryBytes,g.memoryReads);
        size_t sourceBytes=sizeof(*this);
        const auto addRows=[&](size_t count,size_t extent) {
            if(!extent||count>(SIZE_MAX-sourceBytes)/extent)return false;
            sourceBytes+=count*extent;return true;
        };
        if(!addRows(template_creator_linux::TraceeLimit,sizeof(LinuxGateTaskOriginal))||
            !addRows(template_creator_linux::TraceeLimit-1,taskBytes)||!addRows(1,rootTaskBytes)||
            !addRows(template_creator_linux::EventLimit,waitBytes)||!addRows(2,memoryBytes))
            throw std::invalid_argument("aggregate Linux session gate backing");
        // Complete source graph precedes the first allocation. Native kernel,
        // allocator, stack/runtime overlap still needs whole qualification.
        tasks_=std::make_unique<LinuxGateTaskOriginal[]>(template_creator_linux::TraceeLimit);
        for(size_t i=0;i<template_creator_linux::TraceeLimit;++i)
            tasks_[i].task=std::make_unique<template_creator_linux::HeldTaskIdentity>(
                i==0?3:1,g.statBytes,g.statusBytes,g.procReads);
        for(auto& wait:waits_)
            wait.native=std::make_unique<template_creator_linux::ProcTaskObservation>(g.statBytes,g.statusBytes,g.procReads);
        rootMemory_=std::make_unique<template_creator_linux::OriginalLinuxMappedMemory>(*tasks_[0].task,
            platform.pending.OriginalSupervisorOwner(),platform.pending.OriginalDeadlineOwner(),
            g.mapsBytes,g.mapsReads,g.memoryRegions,g.memoryBytes,g.memoryReads);
        execMemory_=std::make_unique<template_creator_linux::OriginalLinuxMappedMemory>(*tasks_[0].task,
            platform.pending.OriginalSupervisorOwner(),platform.pending.OriginalDeadlineOwner(),
            g.mapsBytes,g.mapsReads,g.memoryRegions,g.memoryBytes,g.memoryReads);
    }
    bool BeforeFork(const char* image,const char* const* argv,const char* const* env,const char* cwd,int stdinFd,int cwdFd) override {
        const auto& p=command_.plan;
        if(failed_||taskDisposalRequested_||!interpreterBackingPrepared_||!p.copied||platform_.inputDisposalStarted||
           platform_.pendingHost.Failed()||!platform_.initialObjects.Captured()||!input_.sourceAssociated||
           !input_.captureClean||!input_.launchFileAssociated||input_.launchFileOrdinal>=input_.count||
           !input_.files[input_.launchFileOrdinal]||!platform_.initialObjects.SameCapturedTool(*input_.files[input_.launchFileOrdinal])||
           !input_.cwd||!input_.cwd->Captured()||cwdFd!=input_.cwd->OriginalDescriptor()||
           stdinFd!=platform_.protectedStdin.OriginalReader()||!image||!cwd||
           std::strcmp(image,p.image.data())||std::strcmp(cwd,p.cwd.data())||
           std::strcmp(cwd,input_.cwd->PrivateOriginalPath())||
           !SameVector(argv,p.argv.data(),p.argumentCount)||!SameVector(env,p.environment.data(),p.environmentCount)) {
            failed_=true;return false;
        }
        // This is the already-owned source/command boundary. Future syscall
        // gates must independently establish each actual consumption relation.
        launchElfSource_.Capture(*input_.files[input_.launchFileOrdinal]);
        // Retain unsupported source shapes; this is not kernel admission.
        return true;
    }
    bool BeforeOwnedSeize(const template_creator_linux::Tracee&,const template_creator_linux::State&) override;
    bool BeforeOwnedInterrupt(const template_creator_linux::Tracee&,const template_creator_linux::OwnedInterruptOriginal&,const template_creator_linux::State&) override;
    bool BeforeSyscallEntry(const template_creator_linux::Tracee&,template_creator_linux::Event&,const template_creator_linux::State&) override;
    bool BeforeSyscallExit(const template_creator_linux::Tracee&,template_creator_linux::Event&,const template_creator_linux::State&) override;
    template_creator_linux::WaitAssociation BeforeWait(const template_creator_linux::Tracee&,const template_creator_linux::Event&,const template_creator_linux::State&) override;
    bool BeforeContinue(const template_creator_linux::Tracee&,const template_creator_linux::Event&,const template_creator_linux::State&) override;
    bool BeforeTerminal(const template_creator_linux::Tracee&,const template_creator_linux::Event&,const template_creator_linux::State&) override;
    bool BeforeFailureContinue(const template_creator_linux::Tracee&,const template_creator_linux::Event&,const template_creator_linux::State&) override;
    bool BeforeFailureDrain(const template_creator_linux::State&) override;
    const LinuxRootExecRequestOriginal& PrivateRootExecRequest() const noexcept{return rootExecRequest_;}
    const template_creator_linux::OriginalLinuxMappedMemory* PrivateRootMemoryOriginal() const noexcept{return rootMemory_.get();}
    bool ReservedSourceBytes(size_t& bytes) const noexcept override {
        bytes=sizeof(*this);
        if(template_creator_linux::TraceeLimit>(SIZE_MAX-bytes)/sizeof(LinuxGateTaskOriginal))return false;
        bytes+=template_creator_linux::TraceeLimit*sizeof(LinuxGateTaskOriginal);
        for(size_t i=0;i<template_creator_linux::TraceeLimit;++i){
            size_t actual=0;if(!tasks_[i].task->ReservedSourceBytes(actual)||actual>SIZE_MAX-bytes)return false;bytes+=actual;
        }
        for(const auto& wait:waits_){
            const auto& row=*wait.native;
            auto add=[&](size_t n){if(n>SIZE_MAX-bytes)return false;bytes+=n;return true;};
            if(!add(sizeof(row))||!add(row.stat.byteCapacity+1)||!add(row.status.byteCapacity+1)||
               row.stat.readCapacity>SIZE_MAX/sizeof(template_creator_linux::ProcReadOriginal)||
               row.status.readCapacity>SIZE_MAX/sizeof(template_creator_linux::ProcReadOriginal)||
               !add(row.stat.readCapacity*sizeof(template_creator_linux::ProcReadOriginal))||
               !add(row.status.readCapacity*sizeof(template_creator_linux::ProcReadOriginal)))return false;
        }
        for(const auto* memory:{rootMemory_.get(),execMemory_.get()}){
            size_t memoryBytes=0;
            if(!memory||!memory->ReservedSourceBytes(memoryBytes)||memoryBytes>SIZE_MAX-bytes)return false;
            bytes+=memoryBytes;
        }
        return true; // Python/kernel/allocator/stack/overlap are still additional.
    }
    bool DisposeOriginalTaskInputsEligible() noexcept override;
    bool CaptureOriginals(template_creator_linux::OriginalNativeSink& sink) noexcept {
        using namespace template_creator_linux;
        struct Frame {std::uint64_t version,kind,taskOrdinal,eventOrdinal,extent;};
        bool clean=true;
        const auto append=[&](std::uint64_t kind,size_t task,size_t event,const void* source,size_t extent) noexcept {
            const Frame header{1,kind,task,event,extent};
            const bool framed=sink.Append(reinterpret_cast<const unsigned char*>(&header),sizeof(header));
            const bool copied=framed&&sink.Append(static_cast<const unsigned char*>(source),extent);
            clean=copied&&clean;return copied;
        };
        const auto observation=[&](const ProcTaskObservation& original,size_t task,size_t event) noexcept {
            append(4,task,event,&original,sizeof(original));
            const auto file=[&](const ProcFileObservation& row,std::uint64_t kind) noexcept {
                if(row.byteCapacity==SIZE_MAX||row.used>row.byteCapacity+1||row.readCount>row.readCapacity||
                   row.readCount>SIZE_MAX/sizeof(ProcReadOriginal)){clean=false;return;}
                append(kind,task,event,row.bytes.get(),row.used);
                append(kind+1,task,event,row.reads.get(),row.readCount*sizeof(ProcReadOriginal));
            };
            file(original.stat,5);file(original.status,7);
        };
        // SAME original creating-thread dispatch, after synchronous gate calls.
        // These private ABI records do not serialize referents or grant identity.
        append(0,SIZE_MAX,SIZE_MAX,this,sizeof(*this));
        for(size_t i=0;i<TraceeLimit;++i){
            const auto& row=tasks_[i];if(!row.bindAttempted)continue;
            append(1,i,SIZE_MAX,&row,sizeof(row));
            if(!row.task){clean=false;continue;}
            const auto& task=*row.task;
            append(2,i,SIZE_MAX,&task,sizeof(task));
            append(3,i,SIZE_MAX,&task.OriginalDirectory(),sizeof(task.OriginalDirectory()));
            // Copy typed native outputs separately from the enclosing ABI
            // snapshot, including failed-call output storage and raw returns.
            // Neither metadata equality nor successful copying grants admission.
            struct DirectoryOutcomes {
                int filesystemResult,filesystemError,statResult,statError;
            };
            const DirectoryOutcomes outcomes{task.OriginalFilesystemResult(),task.OriginalFilesystemError(),
                task.OriginalDirectoryStatResult(),task.OriginalDirectoryStatError()};
            append(10,i,SIZE_MAX,&outcomes,sizeof(outcomes));
            append(11,i,SIZE_MAX,&task.OriginalFilesystem(),sizeof(task.OriginalFilesystem()));
            append(12,i,SIZE_MAX,&task.OriginalDirectoryStat(),sizeof(task.OriginalDirectoryStat()));
            for(size_t n=0;n<task.ObservationCount();++n)observation(task.OriginalObservation(n),i,n);
        }
        for(size_t i=0;i<EventLimit;++i){
            const auto& row=waits_[i];if(!row.attempted)continue;
            append(9,SIZE_MAX,i,&row,sizeof(row));
            if(!row.native){clean=false;continue;}
            observation(*row.native,SIZE_MAX,i);
        }
        if(input_.kernelExeCandidate){
            const auto& candidate=*input_.kernelExeCandidate;
            append(21,0,SIZE_MAX,&candidate,sizeof(candidate));
            append(22,0,SIZE_MAX,candidate.PrivateOriginalPath(),candidate.OriginalPathBytes()+1);
            if(candidate.RetainedByteCount()>candidate.BodyCapacity()||
               candidate.ReadCount()>SIZE_MAX/sizeof(HeldFileCall))clean=false;
            else{
                append(23,0,SIZE_MAX,candidate.PrivateBody(),candidate.RetainedByteCount());
                append(24,0,SIZE_MAX,candidate.PrivateReads(),candidate.ReadCount()*sizeof(HeldFileCall));
            }
        }
        const auto appendMemory=[&](const OriginalLinuxMappedMemory* original,unsigned kindOffset){
            if(!original){clean=false;return;}
            const auto& memory=*original;const auto& maps=memory.OriginalMaps();
            append(13+kindOffset,0,SIZE_MAX,&memory,sizeof(memory));
            if(maps.used>maps.byteCapacity+1||maps.readCount>maps.readCapacity||
               maps.readCount>SIZE_MAX/sizeof(ProcReadOriginal)||
               memory.RegionCount()>SIZE_MAX/sizeof(LinuxMappedOriginal)||
               memory.ReadCount()>SIZE_MAX/sizeof(LinuxMemoryReadOriginal)||
               memory.SpanCount()>SIZE_MAX/sizeof(LinuxMemorySpanOriginal)||
               memory.ThreadOriginalCount()>SIZE_MAX/sizeof(LinuxMemoryThreadOriginal))clean=false;
            else {
                append(14+kindOffset,0,SIZE_MAX,maps.bytes.get(),maps.used);
                append(15+kindOffset,0,SIZE_MAX,maps.reads.get(),maps.readCount*sizeof(ProcReadOriginal));
                append(16+kindOffset,0,SIZE_MAX,memory.PrivateRegions(),memory.RegionCount()*sizeof(LinuxMappedOriginal));
                append(17+kindOffset,0,SIZE_MAX,memory.PrivateReads(),memory.ReadCount()*sizeof(LinuxMemoryReadOriginal));
                append(18+kindOffset,0,SIZE_MAX,memory.PrivateThreadOriginals(),memory.ThreadOriginalCount()*sizeof(LinuxMemoryThreadOriginal));
                // Raw reserved backing includes any initialized unread tail;
                // only each read's actual return establishes observed bytes.
                append(19+kindOffset,0,SIZE_MAX,memory.PrivateBytes(),memory.ReservedByteExtent());
                append(20+kindOffset,0,SIZE_MAX,memory.PrivateSpans(),memory.SpanCount()*sizeof(LinuxMemorySpanOriginal));
            }
        };
        appendMemory(rootMemory_.get(),0);appendMemory(execMemory_.get(),16);
        if(!clean)failed_=true;
        return clean;
    }
};
inline size_t OriginalLinuxSessionGate::OriginalOrdinal(const template_creator_linux::Tracee& tracee,
    const template_creator_linux::State& state) const noexcept {
    if(state_!=&state)return template_creator_linux::TraceeLimit;
    for(size_t i=0;i<state.traceeCount;++i)if(&state.tracees[i]==&tracee)return i;
    return template_creator_linux::TraceeLimit;
}
inline bool OriginalLinuxSessionGate::ExactEvent(const template_creator_linux::Event& event,
    const template_creator_linux::State& state) const noexcept {
    return state_==&state&&state.eventCount&&state.eventCount<=template_creator_linux::EventLimit&&
        &state.events[state.eventCount-1]==&event;
}
inline bool OriginalLinuxSessionGate::SameWaitOccurrence(const LinuxGateWaitOriginal& wait,
    const LinuxGateTaskOriginal& task) const noexcept {
    if(!wait.attempted||!wait.native||wait.observation!=wait.native.get()||!wait.observation->parsed||
       !task.bound||!task.association||!task.task->SameOriginal(*task.association))return false;
    const auto& baseline=*task.association;const auto& current=*wait.observation;
    return baseline.observedPid==current.observedPid&&baseline.observedTgid==current.observedTgid&&
        baseline.startTicks==current.startTicks&&current.observedTracer==platform_.pending.OriginalCreatorTid();
}
inline bool OriginalLinuxSessionGate::LiveRootPidfd(LinuxGatePollOriginal& row,
    const template_creator_linux::State& state) noexcept {
    using namespace template_creator_linux;
    if(row.call.attempted||state.rootPidfd>=state.fdCount||state.fds[state.rootPidfd].attempted||
       state.fds[state.rootPidfd].original!=state.originalClonePidfdOutput)return false;
    row.descriptor={state.originalClonePidfdOutput,POLLIN,0};
    auto& deadline=platform_.pending.OriginalDeadlineOwner();auto& supervisor=platform_.pending.OriginalSupervisorOwner();
    if(!CaptureSupervisorClock(row.call.beforeClock,deadline,"gate-root-pidfd-poll",false)||!supervisor.BeginNativeCall())return false;
    row.call.attempted=true;row.call.returned=poll(&row.descriptor,1,0);
    if(row.call.returned<0)row.call.error=errno;
    row.call.returnedObserved=true;supervisor.OriginalNativeCallReturned();
    const bool after=CaptureSupervisorClock(row.call.afterClock,deadline,"gate-root-pidfd-poll",true);
    return after&&row.call.afterClock.nanoseconds>=row.call.beforeClock.nanoseconds&&row.call.returned==0&&row.descriptor.revents==0;
}
inline bool OriginalLinuxSessionGate::BeforeOwnedSeize(const template_creator_linux::Tracee& tracee,
    const template_creator_linux::State& state) {
    using namespace template_creator_linux;
    if(rootBindingAttempted_||taskDisposalRequested_)return false;rootBindingAttempted_=true;state_=&state;
    if(OriginalOrdinal(tracee,state)!=0||!RetainedBirthAssociation(tracee,state)||state.seizeAttempted||
       !LiveRootPidfd(rootPolls_[0],state)){failed_=true;return false;}
    auto& row=tasks_[0];row.occurrence=&tracee;row.bindAttempted=true;
    row.bound=row.task->BindFromOriginalProcRoot(platform_.pending.OriginalProcRootOwner(),tracee.tid,
        &platform_.pending.OriginalSupervisorOwner());
    if(row.task->ObservationCount())row.association=&row.task->OriginalObservation(row.task->ObservationCount()-1);
    const bool after=LiveRootPidfd(rootPolls_[1],state);
    if(!row.bound||!row.association||!row.task->SameOriginal(*row.association)||
       row.association->observedTgid!=tracee.tid||row.association->observedTracer!=0||!after){failed_=true;return false;}
    return true; // Actual atomic birth + held task + two live pidfd observations.
}
inline template_creator_linux::WaitAssociation OriginalLinuxSessionGate::BeforeWait(
    const template_creator_linux::Tracee& tracee,const template_creator_linux::Event& event,
    const template_creator_linux::State& state) {
    using namespace template_creator_linux;
    // Failed pre-seize setup owns the exact atomic clone child and pidfd;
    // waiting for that SAME original child's natural terminal needs neither
    // a fabricated seized task nor permission to execute/restart any child.
    if(!state.seized&&state.failed&&!state.rootCreationUnknown&&!state.rootPidfdOutputUnknown&&
       state.traceeCount==1&& &state.tracees[0]==&tracee&&RetainedBirthAssociation(tracee,state)&&
       !state.launchReleased&&state.rootPidfd<state.fdCount&&
       state.fds[state.rootPidfd].original==state.originalClonePidfdOutput&&
       state.originalClonePidfdOutput>=0&&!state.fds[state.rootPidfd].attempted){
        if(!state_)state_=&state;
        if(ExactEvent(event,state)&&event.enrolledTraceeOrdinal==0&&event.requestedTid==tracee.tid&&!event.waitAttempted){
            auto& original=waits_[state.eventCount-1];original.attempted=true;original.associated=true;
            original.tracee=&tracee;original.event=&event;original.unseizedAtomicOriginal=true;failed_=true;
            return WaitAssociation::BoundOriginal;
        }
    }
    const size_t ordinal=OriginalOrdinal(tracee,state);
    if(taskDisposalRequested_||!ExactEvent(event,state)||ordinal>=TraceeLimit||
       event.enrolledTraceeOrdinal!=ordinal||event.requestedTid!=tracee.tid||event.waitAttempted||
       !RetainedBirthAssociation(tracee,state)||!state.seized||!tracee.seizedOriginal)return WaitAssociation::Denied;
    auto& original=waits_[state.eventCount-1];original.attempted=true;original.tracee=&tracee;original.event=&event;
    auto& row=tasks_[ordinal];
    const bool cleanup=state.failed||failed_||platform_.pending.Failed();
    if(cleanup)failed_=true;
    if(!row.bindAttempted){
        // An autoattached native birth belongs to the SAME saved parent wait,
        // not a later PID-only search. EXEC replacement remains a distinct
        // unresolved native transition and cannot borrow this birth relation.
        if(!ordinal||tracee.execEventOrdinal<state.eventCount||tracee.creatorTraceeOrdinal>=ordinal||
           tracee.creatorEventOrdinal>=state.eventCount-1){consumptionUnimplemented_=true;return WaitAssociation::RetainedPending;}
        const auto& birth=state.events[tracee.creatorEventOrdinal];
        const auto& parent=state.tracees[tracee.creatorTraceeOrdinal];
        const auto& parentWait=waits_[tracee.creatorEventOrdinal];
        const unsigned kind=static_cast<unsigned>(birth.status)>>16;
        if(parentWait.tracee!=&parent||parentWait.event!=&birth||!parentWait.associated||
           !SameWaitOccurrence(parentWait,tasks_[tracee.creatorTraceeOrdinal])||
           (kind!=PTRACE_EVENT_FORK&&kind!=PTRACE_EVENT_VFORK&&kind!=PTRACE_EVENT_CLONE)||
           birth.messageResult!=0||birth.message!=static_cast<unsigned long>(tracee.tid))return WaitAssociation::Denied;
        row.bindAttempted=true;row.occurrence=&tracee;
        row.bound=row.task->BindFromOriginalProcRoot(platform_.pending.OriginalProcRootOwner(),tracee.tid,
            &platform_.pending.OriginalSupervisorOwner(),cleanup);
        if(row.task->ObservationCount())row.association=&row.task->OriginalObservation(0);
        if(!row.bound||!row.association||!row.task->SameOriginal(*row.association)||
           row.association->observedTracer!=platform_.pending.OriginalCreatorTid()){
            failed_=true;return WaitAssociation::RetainedPending;
        }
    }
    if(!row.bound||row.occurrence!=&tracee)return WaitAssociation::Denied;
    auto& native=*original.native;native.attempted=true;original.observation=&native;
    auto& deadline=platform_.pending.OriginalDeadlineOwner();auto* supervisor=&platform_.pending.OriginalSupervisorOwner();
    const int directory=row.task->OriginalDirectory().original;
    const bool stat=native.stat.Capture(directory,"stat",deadline,supervisor,cleanup);
    const bool status=native.status.Capture(directory,"status",deadline,supervisor,cleanup);
    native.parsed=stat&&status&&native.Parse();
    original.associated=SameWaitOccurrence(original,row);
    if(!original.associated){failed_=true;return WaitAssociation::RetainedPending;}
    return WaitAssociation::BoundOriginal;
}
inline bool OriginalLinuxSessionGate::BeforeTerminal(const template_creator_linux::Tracee& tracee,
    const template_creator_linux::Event& event,const template_creator_linux::State& state) {
    using namespace template_creator_linux;
    const size_t ordinal=OriginalOrdinal(tracee,state);
    if(!ExactEvent(event,state)||ordinal>=TraceeLimit||!event.waitAttempted||event.returnedTid!=tracee.tid||
       !(WIFEXITED(event.status)||WIFSIGNALED(event.status)))return false;
    const auto& original=waits_[state.eventCount-1];
    if(original.unseizedAtomicOriginal)return !state.seized&&state.failed&&ordinal==0&&
        !state.launchReleased&&!state.rootCreationUnknown&&!state.rootPidfdOutputUnknown&&
        RetainedBirthAssociation(tracee,state)&&state.rootPidfd<state.fdCount&&
        !state.fds[state.rootPidfd].attempted&&state.fds[state.rootPidfd].original==state.originalClonePidfdOutput&&
        original.tracee==&tracee&&original.event==&event;
    return original.attempted&&original.associated&&original.tracee==&tracee&&original.event==&event&&
        SameWaitOccurrence(original,tasks_[ordinal]);
}
inline bool OriginalLinuxSessionGate::BeforeOwnedInterrupt(const template_creator_linux::Tracee& tracee,
    const template_creator_linux::OwnedInterruptOriginal& interrupt,const template_creator_linux::State& state) {
    const size_t ordinal=OriginalOrdinal(tracee,state);
    if(ordinal>=template_creator_linux::TraceeLimit||taskDisposalRequested_||!tracee.seizedOriginal||tracee.terminal||
       !tasks_[ordinal].bound||tasks_[ordinal].occurrence!=&tracee||interrupt.traceeOrdinal!=ordinal)return false;
    for(size_t i=0;i<state.ownedInterruptCount;++i)if(&state.ownedInterrupts[i]==&interrupt)return true;
    return false;
}
inline bool OriginalLinuxSessionGate::BeforeSyscallEntry(const template_creator_linux::Tracee& tracee,
    template_creator_linux::Event& event,const template_creator_linux::State& state) {
    using namespace template_creator_linux;
#if defined(__x86_64__) && !defined(__ILP32__)
    if(!failed_&&rootReadPhase_==RootReadPhase::Close5Returned&&OriginalOrdinal(tracee,state)==0&&
       ExactEvent(event,state)&&trampolineEvent_<state.eventCount-1&&OriginalRestartSucceeded(state.events[trampolineEvent_])&&
       event.nativeStop==NativeStopKind::SyscallEntry&&event.syscallInfoShape&&
       event.syscallInfo.arch==AUDIT_ARCH_X86_64&&event.syscallInfo.entry.nr==SYS_execve&&
       waits_[state.eventCount-1].associated){
        // Retain the actual request before interpreting any missing relation.
        // Parent-owned source addresses are candidates in the forked address
        // space, not observations of current child bytes or kernel file opens.
        auto& request=rootExecRequest_;
        if(request.attempted){failed_=true;return false;}
        request.attempted=true;request.state=&state;request.tracee=&tracee;request.saved=&event;
        request.syscallArguments={event.syscallInfo.entry.args[0],event.syscallInfo.entry.args[1],event.syscallInfo.entry.args[2]};
        request.sourceAddresses={reinterpret_cast<uintptr_t>(state.originalImage.data()),
            reinterpret_cast<uintptr_t>(state.originalArgv.data()),reinterpret_cast<uintptr_t>(state.originalEnvironment.data())};
        if(event.childPrefixCaptureAttempted&&event.childPrefixCaptureReturned&&event.childPrefixAfter<=state.childOriginalCount)
            for(size_t i=0;i<event.childPrefixAfter;++i)if(state.childOriginals[i].operation==16){
                ++request.attemptMatches;request.childAttempt=&state.childOriginals[i];
            }
        request.childSequenceCount=event.childPrefixAfter;
        request.ownChildSequenceMatched=SameOwnChildExecSequence(event,state);
        if(event.registersAttempted||event.enrolledTraceeOrdinal!=0||!event.waitAttempted||
           event.requestedTid!=tracee.tid||event.returnedTid!=tracee.tid||
           !WIFSTOPPED(event.status)||event.continued||event.restartAttempted||
           tracee.terminal||tracee.supersededByExec||tracee.nativeStop!=NativeStopKind::SyscallEntry||
           !RetainedBirthAssociation(tracee,state)){failed_=true;return false;}
        auto& deadline=platform_.pending.OriginalDeadlineOwner();
        auto& supervisor=platform_.pending.OriginalSupervisorOwner();
        if(!CaptureSupervisorClock(event.registersBeforeClock,deadline,"root-exec-entry-GETREGS",false)||
           !supervisor.BeginNativeCall()){failed_=true;return false;}
        event.registersAttempted=true;
        event.registersResult=ptrace(PTRACE_GETREGS,tracee.tid,nullptr,&event.originalRegisters);
        if(event.registersResult<0)event.registersError=errno;
        event.registersReturned=true;supervisor.OriginalNativeCallReturned();
        const bool after=CaptureSupervisorClock(event.registersAfterClock,deadline,"root-exec-entry-GETREGS",true);
        event.registersCaptured=event.registersResult==0&&after&&
            event.registersAfterClock.nanoseconds>=event.registersBeforeClock.nanoseconds;
        request.originalRequestMatched=event.registersCaptured&&
            event.originalRegisters.orig_rax==SYS_execve&&
            event.originalRegisters.rip==event.syscallInfo.instruction_pointer&&
            event.originalRegisters.rsp==event.syscallInfo.stack_pointer&&
            event.originalRegisters.rdi==request.syscallArguments[0]&&
            event.originalRegisters.rsi==request.syscallArguments[1]&&
            event.originalRegisters.rdx==request.syscallArguments[2]&&
            request.syscallArguments==request.sourceAddresses&&request.ownChildSequenceMatched&&
            request.attemptMatches==1&&request.childAttempt&&
            !request.childAttempt->nativeReturned&&request.childAttempt->originalResult==0&&request.childAttempt->originalErrno==0;
        request.sourceCapabilitiesAttempted=true;
        const bool ownPipeSources=ObserveOwnPipeSources(request,state);
        request.launchFileOrdinal=input_.launchFileOrdinal;
        request.launchFileOriginal=input_.launchFileOrdinal<input_.count?
            input_.files[input_.launchFileOrdinal].get():nullptr;
        request.directoryOriginal=input_.cwd.get();
        if(request.launchFileOriginal)request.launchDescriptor=request.launchFileOriginal->OriginalDescriptor();
        if(request.directoryOriginal)request.directoryDescriptor=request.directoryOriginal->OriginalDescriptor();
        const auto& originalPlan=command_.plan;
        request.sourceCapabilitiesMatched=request.originalRequestMatched&&ownPipeSources&&originalPlan.copied&&
            !platform_.inputDisposalStarted&&input_.sourceAssociated&&input_.captureClean&&input_.launchFileAssociated&&
            request.launchFileOriginal&&request.launchFileOriginal->CapturedSourceBytes()&&request.launchDescriptor>=0&&
            platform_.initialObjects.SameCapturedTool(*request.launchFileOriginal)&&
            request.directoryOriginal&&request.directoryOriginal->Captured()&&request.directoryDescriptor>=0&&
            state.childSourceOriginals[0]==platform_.protectedStdin.OriginalReader()&&
            state.childSourceOriginals[5]==request.directoryDescriptor&&
            !std::strcmp(state.originalImage.data(),originalPlan.image.data())&&
            !std::strcmp(state.originalCwd.data(),originalPlan.cwd.data())&&
            !std::strcmp(state.originalCwd.data(),request.directoryOriginal->PrivateOriginalPath())&&
            SameVector(state.originalArgv.data(),originalPlan.argv.data(),originalPlan.argumentCount)&&
            SameVector(state.originalEnvironment.data(),originalPlan.environment.data(),originalPlan.environmentCount);
        if(request.originalRequestMatched){
            CaptureRootExecChildMemory(tracee,event,state);
#ifdef TEMPLATE_NATIVE_ROOT_MEMORY_REGRESSION
            request.regressionAttempted=true;
            request.regressionResult=rootMemory_&&
                template_creator_linux::InvokeActualRootExecFiveSpanRegression(*rootMemory_,
                    VerifyOwnPipeSourceRegression(request,state));
            request.regressionReturned=true;
#endif
        }
        // Intermediate request stage only: EXEC remains a separately held stop
        // before user entry, with kernel/interpreter/provider gates still owed.
        if(!ValidateRootExecRequestOriginal(tracee,event,state)){failed_=true;return false;}
#ifdef TEMPLATE_NATIVE_ROOT_MEMORY_REGRESSION
        if(!request.regressionReturned||!request.regressionResult){failed_=true;return false;}
#endif
        rootExecStage_=RootExecStage::RequestValidated;return true;
    }
    if(!failed_&&OriginalOrdinal(tracee,state)==0&&ExactEvent(event,state)&&trampolineEvent_<state.eventCount-1&&
       event.nativeStop==NativeStopKind::SyscallEntry&&event.syscallInfoShape&&event.syscallInfo.arch==AUDIT_ARCH_X86_64&&
       waits_[state.eventCount-1].associated&&event.childPrefixCaptureAttempted&&event.childPrefixCaptureReturned&&
       event.childPrefixAfter<=state.childOriginalCount&&OriginalRestartSucceeded(state.events[trampolineEvent_])){
        int operation=0;RootReadPhase next=rootReadPhase_;
        if(rootReadPhase_==RootReadPhase::Close4Returned&&event.syscallInfo.entry.nr==SYS_fchdir&&
           event.syscallInfo.entry.args[0]==5&&input_.cwd&&input_.cwd->Captured()&&
           state.childSourceOriginals[5]==input_.cwd->OriginalDescriptor()){
            operation=25;next=RootReadPhase::DirectoryEntered;
        }else if(rootReadPhase_==RootReadPhase::DirectoryReturned&&event.syscallInfo.entry.nr==SYS_close&&
                 event.syscallInfo.entry.args[0]==5){operation=5;next=RootReadPhase::Close5Entered;}
        if(operation){
            size_t matches=0;const ChildNativeOutcome* original=nullptr;
            for(size_t i=0;i<event.childPrefixAfter;++i)if(state.childOriginals[i].operation==operation){++matches;original=&state.childOriginals[i];}
            if(matches==1&&original&&original->nativeReturned&&original->originalResult==0&&original->originalErrno==0&&
               original->originalResult==state.events[trampolineEvent_].syscallInfo.exit.rval){
                rootReadPhase_=next;trampolineEvent_=state.eventCount-1;return true;
            }
        }
    }
    if(!failed_&&rootReadPhase_==RootReadPhase::NotObserved&&OriginalOrdinal(tracee,state)==0&&ExactEvent(event,state)&&
       event.nativeStop==NativeStopKind::SyscallEntry&&event.syscallInfoShape&&
       event.syscallInfo.arch==AUDIT_ARCH_X86_64&&event.syscallInfo.entry.nr==SYS_read&&
       event.syscallInfo.entry.args[0]==4&&event.syscallInfo.entry.args[2]==1&&
       state.childShared&&!state.childUnmapped&&event.syscallInfo.entry.args[1]==reinterpret_cast<uintptr_t>(&state.childShared->launchByte)&&
       waits_[state.eventCount-1].associated&&state.launchReleased&&state.launchWriteResult==1&&
       state.launchWriteError==0&&state.originalLaunchByte==1&&
       event.childPrefixCaptureAttempted&&event.childPrefixCaptureReturned&&event.childPrefixAfter<=state.childOriginalCount){
        size_t attempts=0;const ChildNativeOutcome* attempt=nullptr;
        for(size_t i=0;i<event.childPrefixAfter;++i)if(state.childOriginals[i].operation==23){++attempts;attempt=&state.childOriginals[i];}
        if(attempts==1&&attempt&&!attempt->nativeReturned&&attempt->originalResult==1&&attempt->originalErrno==0){
            rootReadPhase_=RootReadPhase::ReadEntered;rootReadEntryEvent_=state.eventCount-1;return true;
        }
    }
    if(!failed_&&OriginalOrdinal(tracee,state)==0&&ExactEvent(event,state)&&
       rootReadPhase_==RootReadPhase::ReturnedToTrampoline&&rootReadEvent_<state.eventCount-1&&
       event.nativeStop==NativeStopKind::SyscallEntry&&event.syscallInfoShape&&
       event.syscallInfo.arch==AUDIT_ARCH_X86_64&&event.syscallInfo.entry.nr==SYS_close&&
       event.syscallInfo.entry.args[0]==4&&waits_[state.eventCount-1].associated&&
       event.childPrefixCaptureAttempted&&event.childPrefixCaptureReturned&&
       event.childPrefixAfter<=state.childOriginalCount&&state.childShared&&!state.childUnmapped){
        const auto& prior=state.events[rootReadEvent_];
        size_t returns=0;const ChildNativeOutcome* returned=nullptr;
        for(size_t i=0;i<event.childPrefixAfter;++i)if(state.childOriginals[i].operation==24){
            ++returns;returned=&state.childOriginals[i];
        }
        const bool originalRead=prior.registersCaptured||
            (rootReadEntryEvent_<rootReadEvent_&&prior.linkedEntryOrdinal==rootReadEntryEvent_&&OriginalRestartSucceeded(state.events[rootReadEntryEvent_]));
        if(OriginalRestartSucceeded(prior)&&originalRead&&returns==1&&returned&&returned->nativeReturned&&
           returned->originalResult==prior.syscallInfo.exit.rval&&returned->originalResult==1&&
           returned->originalErrno==0&&event.childLaunchByteCaptured&&prior.childLaunchByteCaptured&&
           event.childLaunchByteOriginal==prior.childLaunchByteOriginal&&event.childLaunchByteOriginal==state.originalLaunchByte&&
           state.originalLaunchByte==1){
            rootReadPhase_=RootReadPhase::MatchedAtCloseEntry;rootCloseEntryEvent_=state.eventCount-1;
            return true;
        }
    }
#endif
    consumptionUnimplemented_=true;failed_=true;return false;
}
inline bool OriginalLinuxSessionGate::BeforeSyscallExit(const template_creator_linux::Tracee& tracee,
    template_creator_linux::Event& event,const template_creator_linux::State& state) {
    using namespace template_creator_linux;
    const size_t ordinal=OriginalOrdinal(tracee,state);
#if defined(__x86_64__) && !defined(__ILP32__)
    const size_t entering=rootReadPhase_==RootReadPhase::MatchedAtCloseEntry?rootCloseEntryEvent_:trampolineEvent_;
    if(!failed_&&ordinal==0&&ExactEvent(event,state)&&entering<state.eventCount-1&&event.linkedEntryOrdinal==entering&&
       OriginalRestartSucceeded(state.events[entering])&&event.nativeStop==NativeStopKind::SyscallExit&&event.syscallInfoShape&&
       event.syscallInfo.arch==AUDIT_ARCH_X86_64&&event.syscallInfo.op==PTRACE_SYSCALL_INFO_EXIT&&
       event.syscallInfo.exit.is_error==0&&event.syscallInfo.exit.rval==0&&waits_[state.eventCount-1].associated){
        RootReadPhase next=rootReadPhase_;
        if(rootReadPhase_==RootReadPhase::MatchedAtCloseEntry)next=RootReadPhase::Close4Returned;
        else if(rootReadPhase_==RootReadPhase::DirectoryEntered)next=RootReadPhase::DirectoryReturned;
        else if(rootReadPhase_==RootReadPhase::Close5Entered)next=RootReadPhase::Close5Returned;
        if(next!=rootReadPhase_){rootReadPhase_=next;trampolineEvent_=state.eventCount-1;return true;}
    }
    if(!failed_&&rootReadPhase_==RootReadPhase::ReadEntered&&ordinal==0&&ExactEvent(event,state)&&
       rootReadEntryEvent_<state.eventCount-1&&event.linkedEntryOrdinal==rootReadEntryEvent_&&
       OriginalRestartSucceeded(state.events[rootReadEntryEvent_])&&event.nativeStop==NativeStopKind::SyscallExit&&
       event.syscallInfoShape&&event.syscallInfo.arch==AUDIT_ARCH_X86_64&&
       event.syscallInfo.op==PTRACE_SYSCALL_INFO_EXIT&&event.syscallInfo.exit.is_error==0&&event.syscallInfo.exit.rval==1&&
       waits_[state.eventCount-1].associated&&event.childLaunchByteCaptured&&event.childLaunchByteOriginal==1){
        rootReadPhase_=RootReadPhase::ReturnedToTrampoline;rootReadEvent_=state.eventCount-1;return true;
    }
#endif
    if(!ExactEvent(event,state)||ordinal!=0||event.enrolledTraceeOrdinal!=ordinal||
       event.returnedTid!=tracee.tid||!event.waitAttempted||!WIFSTOPPED(event.status)||
       !waits_[state.eventCount-1].associated||!SameWaitOccurrence(waits_[state.eventCount-1],tasks_[ordinal])||
       event.continued||event.restartAttempted||event.nativeStop!=NativeStopKind::SyscallExit||
       !event.syscallInfoShape||event.syscallInfo.op!=PTRACE_SYSCALL_INFO_EXIT||
       event.partialSyscallOrigin!=PartialSyscallOrigin::OriginalSeizeBoundary||
       event.partialInitialStopOrdinal!=tracee.initialSeizedStopOrdinal||
       event.partialInitialStopOrdinal>=state.eventCount-1||event.registersAttempted){
        failed_=true;return false;
    }
#if defined(__x86_64__) && !defined(__ILP32__)
    if(event.syscallInfo.arch!=AUDIT_ARCH_X86_64){failed_=true;return false;}
    auto& deadline=platform_.pending.OriginalDeadlineOwner();
    auto& supervisor=platform_.pending.OriginalSupervisorOwner();
    if(!CaptureSupervisorClock(event.registersBeforeClock,deadline,"partial-syscall-GETREGS",false)||
       !supervisor.BeginNativeCall()){failed_=true;return false;}
    event.registersAttempted=true;
    event.registersResult=ptrace(PTRACE_GETREGS,tracee.tid,nullptr,&event.originalRegisters);
    if(event.registersResult<0)event.registersError=errno;
    event.registersReturned=true;
    supervisor.OriginalNativeCallReturned();
    const bool after=CaptureSupervisorClock(event.registersAfterClock,deadline,"partial-syscall-GETREGS",true);
    event.registersCaptured=event.registersResult==0&&after&&
        event.registersAfterClock.nanoseconds>=event.registersBeforeClock.nanoseconds;
    size_t attempts=0;const ChildNativeOutcome* attempt=nullptr;
    if(event.childPrefixCaptureAttempted&&event.childPrefixCaptureReturned&&event.childPrefixAfter<=state.childOriginalCount)
        for(size_t i=0;i<event.childPrefixAfter;++i)if(state.childOriginals[i].operation==23){
            ++attempts;attempt=&state.childOriginals[i];
        }
    // Built from authentic admitted headers/source in an eventual native run.
    // Full nr equality excludes x32, which shares AUDIT_ARCH_X86_64.
    if(!failed_&&rootReadPhase_==RootReadPhase::NotObserved&&event.registersCaptured&&
       event.originalRegisters.orig_rax==static_cast<unsigned long long>(SYS_read)&&
       event.originalRegisters.rdi==4&&event.originalRegisters.rdx==1&&
       state.childShared&&!state.childUnmapped&&state.originalCloneArguments.flags==CLONE_PIDFD&&
       event.originalRegisters.rsi==reinterpret_cast<uintptr_t>(&state.childShared->launchByte)&&
       event.syscallInfo.exit.is_error==0&&event.syscallInfo.exit.rval==1&&event.originalRegisters.rax==1&&
       attempts==1&&attempt&&!attempt->nativeReturned&&attempt->originalResult==1&&attempt->originalErrno==0&&
       state.launchReleased&&state.launchWriteResult==1&&state.launchWriteError==0&&state.originalLaunchByte==1&&
       event.childLaunchByteCaptured&&event.childLaunchByteOriginal==1){
        rootReadPhase_=RootReadPhase::ReturnedToTrampoline;rootReadEvent_=state.eventCount-1;
        return true;
    }
#endif
    // Anything outside the selected source case remains denied. Authentic
    // native source/ABI admission is still owed before any eventual execution.
    consumptionUnimplemented_=true;failed_=true;return false;
}
inline bool OriginalLinuxSessionGate::BeforeContinue(const template_creator_linux::Tracee& tracee,
    const template_creator_linux::Event& event,const template_creator_linux::State& state) {
    using namespace template_creator_linux;
    const size_t ordinal=OriginalOrdinal(tracee,state);
    if(failed_||!ExactEvent(event,state)||ordinal>=TraceeLimit||!event.waitAttempted||event.returnedTid!=tracee.tid||
       !waits_[state.eventCount-1].associated||!WIFSTOPPED(event.status)||
       !OriginalRestartAdmissionShape(event,RestartAdmissionCut::ReservedGate))return false;
    if(ordinal==0&&event.nativeStop==NativeStopKind::Event&&
       (static_cast<unsigned>(event.status)>>16)==PTRACE_EVENT_EXEC){
        // This candidate is only the original single-task child trampoline's
        // first exec. Nonleader replacement retains former TID in message and
        // requires its separate authentic replacement join; numeric current
        // TID equality cannot qualify that later profile.
        kernelExeStopOriginal_=&event;
        size_t sourceEntry=EventLimit;
        if(rootExecRequest_.state==&state)
            for(size_t i=0;i+1<state.eventCount;++i)
                if(&state.events[i]==rootExecRequest_.saved)sourceEntry=i;
        if(kernelExeAttempted_||sourceEntry==EventLimit||!OriginalRestartSucceeded(state.events[sourceEntry])||
           !rootExecRequest_.originalRequestMatched||!rootExecRequest_.ownChildSequenceMatched||
            !rootExecRequest_.sourceCapabilitiesAttempted||!rootExecRequest_.sourceCapabilitiesMatched||
           !rootExecRequest_.childMemoryMatched||!rootExecRequest_.childSharedWriterAttempted||
           !rootExecRequest_.childSharedWriterObserved||!rootExecRequest_.childArgumentMappingsAttempted||
           !rootExecRequest_.childArgumentMappingsObserved||event.messageResult!=0||
           event.message!=static_cast<unsigned long>(tracee.tid)||tracee.supersededByExec||
           !OriginalRestartAdmissionShape(event,RestartAdmissionCut::ReservedGate)||!SameWaitOccurrence(waits_[state.eventCount-1],tasks_[0])||
           !input_.kernelExeCandidate||input_.kernelExeSourceOrdinal!=input_.launchFileOrdinal||
           input_.launchFileOrdinal>=input_.count||!input_.files[input_.launchFileOrdinal]){
            failed_=true;return false;
        }
        kernelExeAttempted_=true;
         rootExecStage_=RootExecStage::ExecStopObserved;
#ifdef TEMPLATE_NATIVE_ROOT_MEMORY_REGRESSION
         if(!ObserveActualRootExecContinuedRegression(state.events[sourceEntry])){failed_=true;return false;}
          if(launchElfSource_.interpreterCount&&!ExpectActualInterpreterFileSpansRegression()){failed_=true;return false;}
#endif
        kernelExeTaskOriginal_=tasks_[0].task->Observe();
        if(kernelExeTaskOriginal_&&tasks_[0].task->SameOriginal(*kernelExeTaskOriginal_)&&
           kernelExeTaskOriginal_->observedTracer==platform_.pending.OriginalCreatorTid()&&
           !tasks_[0].task->OriginalDirectory().closeAttempted){
            kernelExeCaptured_=input_.kernelExeCandidate->CaptureProcExeCandidate(
                tasks_[0].task->OriginalDirectory().original);
            kernelExeMatched_=kernelExeCaptured_&&input_.kernelExeCandidate->SameCapturedFileCandidate(
                *input_.files[input_.launchFileOrdinal]);
            if(kernelExeCaptured_){
                kernelElfCaptureAttempted_=true;
                kernelElfCaptureResult_=kernelElfSource_.Capture(*input_.kernelExeCandidate);
                kernelElfCaptureReturned_=true;
            }
             if(kernelExeMatched_){
                 interpreterEntryCaptureAttempted_=true;
                 interpreterEntryCaptureResult_=CaptureInterpreterEntryCandidate(tracee,event,state);
                 interpreterEntryCaptureReturned_=true;
             }
        }
        // Candidate observation is connected, but cannot retroactively seal
        // exec name lookup or establish ELF interpreter/provider/actor inputs.
        consumptionUnimplemented_=true;failed_=true;return false;
    }
    if(ordinal==0&&event.originalResumeRequest==PTRACE_SYSCALL&&event.deliveredSignal==0){
        if(rootExecStage_==RootExecStage::RequestValidated&&rootExecRequest_.saved==&event){
            if(!ValidateRootExecRequestOriginal(tracee,event,state,RestartAdmissionCut::ReservedGate))return false;
#ifdef TEMPLATE_NATIVE_ROOT_MEMORY_REGRESSION
            if(!ObserveActualRootExecReservedGateRegression(event))return false;
#endif
            return true;
        }
        if(trampolineEvent_==state.eventCount-1&&
           ((event.nativeStop==NativeStopKind::SyscallEntry&&(rootReadPhase_==RootReadPhase::DirectoryEntered||rootReadPhase_==RootReadPhase::Close5Entered))||
            (event.nativeStop==NativeStopKind::SyscallExit&&(rootReadPhase_==RootReadPhase::Close4Returned||rootReadPhase_==RootReadPhase::DirectoryReturned||rootReadPhase_==RootReadPhase::Close5Returned))))return true;
        if(rootReadPhase_==RootReadPhase::ReadEntered&&rootReadEntryEvent_==state.eventCount-1&&
           event.nativeStop==NativeStopKind::SyscallEntry)return true;
        if(rootReadPhase_==RootReadPhase::ReturnedToTrampoline&&rootReadEvent_==state.eventCount-1&&
           event.nativeStop==NativeStopKind::SyscallExit)return true;
        if(rootReadPhase_==RootReadPhase::MatchedAtCloseEntry&&rootCloseEntryEvent_==state.eventCount-1&&
           event.nativeStop==NativeStopKind::SyscallEntry)return true;
    }
    // Later trampoline/EXEC and product syscall consumers remain unfinished.
    return ordinal==0&&event.nativeStop==NativeStopKind::InitialSeized&&
        (static_cast<unsigned>(event.status)>>16)==PTRACE_EVENT_STOP&&event.originalResumeRequest==PTRACE_SYSCALL;
}
inline bool OriginalLinuxSessionGate::BeforeFailureContinue(const template_creator_linux::Tracee& tracee,
    const template_creator_linux::Event& event,const template_creator_linux::State& state) {
    const size_t ordinal=OriginalOrdinal(tracee,state);
    return ExactEvent(event,state)&&ordinal<template_creator_linux::TraceeLimit&&
        template_creator_linux::OriginalRestartAdmissionShape(event,template_creator_linux::RestartAdmissionCut::ReservedGate)&&
        waits_[state.eventCount-1].associated&&event.waitAttempted&&event.returnedTid==tracee.tid&&
        WIFSTOPPED(event.status)&&(static_cast<unsigned>(event.status)>>16)==PTRACE_EVENT_EXIT&&
        event.originalResumeRequest==PTRACE_SYSCALL;
}
inline bool OriginalLinuxSessionGate::BeforeFailureDrain(const template_creator_linux::State& state) {
    return state_==&state&&state.forkAttempted&&state.originalCloneResult>0&&!state.rootCreationUnknown;
}
inline bool OriginalLinuxSessionGate::CaptureInterpreterEntryCandidate(
        const template_creator_linux::Tracee& tracee,const template_creator_linux::Event& event,
        const template_creator_linux::State& state) noexcept {
    using namespace template_creator_linux;
    if(interpreterEntryAttempted_||!kernelExeMatched_||kernelExeStopOriginal_!=&event||
       !ExactEvent(event,state)||!SameWaitOccurrence(waits_[state.eventCount-1],tasks_[0])||
       !kernelExeTaskOriginal_||!tasks_[0].task->SameOriginal(*kernelExeTaskOriginal_)||
       !execMemory_||interpreterSourceMatchCount_!=1||!interpreterRequirementOriginal_||
       !interpreterSourceFileOriginal_||!interpreterSourceFileOriginal_->CapturedSourceBytes())return false;
    interpreterEntryAttempted_=true;
    if(!interpreterBackingPrepared_||!interpreterElfSource_.projected||
       !interpreterElfSource_.UniqueExecutableEntryFileOffset(interpreterEntryFileOffset_)||
       !execMemory_->CaptureMaps(*kernelExeTaskOriginal_,state,tracee,event))return false;
#if defined(__x86_64__)
    auto& row=interpreterRegistersCall_;
    auto& supervisor=platform_.pending.OriginalSupervisorOwner();
    auto& deadline=platform_.pending.OriginalDeadlineOwner();
    row.requested=sizeof(interpreterRegisters_);
    interpreterRegistersTidOriginal_=tracee.tid;
    if(!CaptureSupervisorClock(row.beforeClock,deadline,"exec-interpreter-GETREGS",false)||
       !supervisor.BeginNativeCall())return false;
    row.attempted=true;errno=0;
    row.returned=ptrace(PTRACE_GETREGS,interpreterRegistersTidOriginal_,nullptr,&interpreterRegisters_);
    row.error=row.returned<0?errno:0;row.returnedObserved=true;supervisor.OriginalNativeCallReturned();
    const bool after=CaptureSupervisorClock(row.afterClock,deadline,"exec-interpreter-GETREGS",true);
    if(!after||row.afterClock.nanoseconds<row.beforeClock.nanoseconds||row.returned!=0||
       !OriginalRestartAdmissionShape(event,RestartAdmissionCut::ReservedGate))return false;
    const uint64_t rip=interpreterRegisters_.rip;
    const auto& metadata=interpreterSourceFileOriginal_->PrivateAfter().metadata;
    size_t matches=0;
    for(size_t i=0;i<execMemory_->RegionCount();++i){
        const auto& map=execMemory_->PrivateRegions()[i];
        if(map.permissions[2]!='x'||rip<map.begin||rip>=map.end||
           map.deviceMajor!=static_cast<uint64_t>(major(metadata.st_dev))||
           map.deviceMinor!=static_cast<uint64_t>(minor(metadata.st_dev))||
           map.inode!=static_cast<uint64_t>(metadata.st_ino))continue;
        const uint64_t delta=rip-map.begin;
        if(delta>UINT64_MAX-map.fileOffset||map.fileOffset+delta!=interpreterEntryFileOffset_)continue;
        interpreterMappingOrdinal_=i;++matches;
    }
    if(matches!=1)return false;
    if(rip<interpreterElfSource_.header.e_entry)return false;
    interpreterLoadBias_=rip-interpreterElfSource_.header.e_entry;
    if(interpreterElfSource_.header.e_type==ET_EXEC&&interpreterLoadBias_)return false;
    interpreterFileSpansAttempted_=true;
    // Establish each complete source-selected file span's mapped identity and
    // offset coverage BEFORE it can become an input to a native memory read.
    for(size_t ph=0;ph<interpreterElfSource_.header.e_phnum;++ph){
        Elf64_Phdr load{};if(!interpreterElfSource_.OriginalProgramHeader(ph,load))return false;
        if(load.p_type!=PT_LOAD||!load.p_filesz)continue;
        if(load.p_vaddr>UINT64_MAX-interpreterLoadBias_)return false;
        const uint64_t start=interpreterLoadBias_+load.p_vaddr;
        if(load.p_filesz>UINT64_MAX-start)return false;
        const uint64_t stop=start+load.p_filesz;uint64_t cursor=start;
        while(cursor<stop){
            const LinuxMappedOriginal* found=nullptr;
            for(size_t i=0;i<execMemory_->RegionCount();++i){const auto& region=execMemory_->PrivateRegions()[i];
                if(cursor>=region.begin&&cursor<region.end){if(found)return false;found=&region;}
            }
            if(!found){interpreterFirstFailedLoadOrdinal_=ph;return false;}
            const uint64_t delta=cursor-found->begin,sourceDelta=cursor-start;
            if(found->permissions[0]!='r'||found->permissions[3]!='p'||
               found->deviceMajor!=static_cast<uint64_t>(major(metadata.st_dev))||
               found->deviceMinor!=static_cast<uint64_t>(minor(metadata.st_dev))||
               found->inode!=static_cast<uint64_t>(metadata.st_ino)||
               delta>UINT64_MAX-found->fileOffset||sourceDelta>UINT64_MAX-load.p_offset||
               found->fileOffset+delta!=load.p_offset+sourceDelta){interpreterFirstFailedLoadOrdinal_=ph;return false;}
            cursor=found->end<stop?found->end:stop;
        }
        ++interpreterMappedLoadSegmentsObserved_;
    }
    // Before reads, separately require complete declared memory-range mapping
    // coverage. Anonymous tails are candidates, not attributed kernel outputs.
    for(size_t ph=0;ph<interpreterElfSource_.header.e_phnum;++ph){
        Elf64_Phdr load{};if(!interpreterElfSource_.OriginalProgramHeader(ph,load))return false;
        if(load.p_type!=PT_LOAD||!load.p_memsz)continue;
        if(load.p_vaddr>UINT64_MAX-interpreterLoadBias_)return false;
        const uint64_t start=interpreterLoadBias_+load.p_vaddr;
        if(load.p_memsz>UINT64_MAX-start)return false;
        const uint64_t stop=start+load.p_memsz;uint64_t cursor=start;
        while(cursor<stop){const LinuxMappedOriginal* found=nullptr;
            for(size_t i=0;i<execMemory_->RegionCount();++i){const auto& region=execMemory_->PrivateRegions()[i];
                if(region.begin<=cursor&&cursor<region.end){if(found)return false;found=&region;}
            }
            if(!found){interpreterFirstFailedLoadOrdinal_=ph;return false;}
            const bool sameFile=found->deviceMajor==static_cast<uint64_t>(major(metadata.st_dev))&&
                found->deviceMinor==static_cast<uint64_t>(minor(metadata.st_dev))&&
                found->inode==static_cast<uint64_t>(metadata.st_ino);
            const bool anonymousCandidate=!found->deviceMajor&&!found->deviceMinor&&!found->inode;
            if(found->permissions[0]!='r'||found->permissions[3]!='p'||(!sameFile&&!anonymousCandidate)){
                interpreterFirstFailedLoadOrdinal_=ph;return false;
            }
            cursor=found->end<stop?found->end:stop;
        }
        ++interpreterMemorySegmentsMapped_;
    }
    // Derive actual PT_LOAD memory intersections in each SAME captured
    // mapping. Merge overlap/adjacency there, never a global min/max hull. The
    // phnum-derived preowned roster pays for at most phnum unions per region.
    for(size_t regionOrdinal=0;regionOrdinal<execMemory_->RegionCount();++regionOrdinal){
        const auto& region=execMemory_->PrivateRegions()[regionOrdinal];
        const bool sameFile=region.deviceMajor==static_cast<uint64_t>(major(metadata.st_dev))&&
            region.deviceMinor==static_cast<uint64_t>(minor(metadata.st_dev))&&
            region.inode==static_cast<uint64_t>(metadata.st_ino);
        const bool anonymousCandidate=!region.deviceMajor&&!region.deviceMinor&&!region.inode;
        if(!sameFile&&!anonymousCandidate)continue;
        uint64_t cursor=region.begin;
        while(cursor<region.end){
            uint64_t begin=UINT64_MAX,end=0;
            for(size_t ph=0;ph<interpreterElfSource_.header.e_phnum;++ph){
                Elf64_Phdr load{};if(!interpreterElfSource_.OriginalProgramHeader(ph,load))return false;
                if(load.p_type!=PT_LOAD||!load.p_memsz)continue;
                if(load.p_vaddr>UINT64_MAX-interpreterLoadBias_)return false;
                const uint64_t start=interpreterLoadBias_+load.p_vaddr;
                if(load.p_memsz>UINT64_MAX-start)return false;
                const uint64_t stop=start+load.p_memsz;
                const uint64_t left=start>cursor?start:cursor,right=stop<region.end?stop:region.end;
                if(left>=right)continue;
                if(left<begin){begin=left;end=right;}
            }
            if(begin==UINT64_MAX)break;
            // Every enlargement takes an endpoint from the finite header table.
            for(size_t pass=0;pass<interpreterElfSource_.header.e_phnum;++pass){
                const uint64_t previous=end;
                for(size_t ph=0;ph<interpreterElfSource_.header.e_phnum;++ph){
                    Elf64_Phdr load{};if(!interpreterElfSource_.OriginalProgramHeader(ph,load))return false;
                    if(load.p_type!=PT_LOAD||!load.p_memsz)continue;
                    if(load.p_vaddr>UINT64_MAX-interpreterLoadBias_)return false;
                    const uint64_t start=interpreterLoadBias_+load.p_vaddr;
                    if(load.p_memsz>UINT64_MAX-start)return false;
                    const uint64_t stop=start+load.p_memsz;
                    const uint64_t left=start>region.begin?start:region.begin,right=stop<region.end?stop:region.end;
                    if(left<=end&&right>begin&&right>end)end=right;
                }
                if(end==previous)break;
            }
            if(region.permissions[0]!='r'||region.permissions[3]!='p'||
               interpreterCapturedLoadSpans_==SIZE_MAX||
               !interpreterElfSource_.MemorySpanCovered(interpreterLoadBias_,begin,end)||
               !execMemory_->CaptureOwnedMappedSpan(regionOrdinal,begin,end))return false;
            ++interpreterCapturedLoadSpans_;cursor=end;
        }
    }
    for(size_t ph=0;ph<interpreterElfSource_.header.e_phnum;++ph){
        Elf64_Phdr load{};if(!interpreterElfSource_.OriginalProgramHeader(ph,load))return false;
        if(load.p_type!=PT_LOAD||!load.p_filesz)continue;
        if(load.p_vaddr>UINT64_MAX-interpreterLoadBias_)return false;
        const uint64_t start=interpreterLoadBias_+load.p_vaddr;
        if(load.p_filesz>SIZE_MAX||load.p_filesz>UINT64_MAX-start||
           load.p_filesz>UINT64_MAX-interpreterMatchedFileBytes_||
           !execMemory_->SameCapturedBytes(start,interpreterSourceFileOriginal_->PrivateBody()+static_cast<size_t>(load.p_offset),
                static_cast<size_t>(load.p_filesz))){interpreterFirstFailedLoadOrdinal_=ph;return false;}
        // Each intersecting mapping must independently retain the exact original
        // file-offset relationship; inode equality alone does not supply it.
        for(size_t i=0;i<execMemory_->RegionCount();++i){const auto& region=execMemory_->PrivateRegions()[i];
            const uint64_t left=start>region.begin?start:region.begin;
            const uint64_t stop=start+load.p_filesz,right=stop<region.end?stop:region.end;
            if(left>=right)continue;
            const uint64_t delta=left-region.begin,sourceDelta=left-start;
            if(region.deviceMajor!=static_cast<uint64_t>(major(metadata.st_dev))||
               region.deviceMinor!=static_cast<uint64_t>(minor(metadata.st_dev))||
               region.inode!=static_cast<uint64_t>(metadata.st_ino)||
               delta>UINT64_MAX-region.fileOffset||sourceDelta>UINT64_MAX-load.p_offset||
               region.fileOffset+delta!=load.p_offset+sourceDelta){interpreterFirstFailedLoadOrdinal_=ph;return false;}
        }
        interpreterMatchedFileBytes_+=load.p_filesz;++interpreterLoadSegmentsMatched_;
    }
    interpreterFileSpansMatched_=interpreterLoadSegmentsMatched_!=0&&interpreterCapturedLoadSpans_!=0;
    if(!interpreterFileSpansMatched_)return false;
    // These are actual captured tail observations, not a claim that the kernel
    // must zero every declared tail despite overlapping/ordered PT_LOAD ranges.
    // Nonzero tails remain visible; no useful admission follows either result.
    for(size_t ph=0;ph<interpreterElfSource_.header.e_phnum;++ph){
        Elf64_Phdr load{};if(!interpreterElfSource_.OriginalProgramHeader(ph,load))return false;
        if(load.p_type!=PT_LOAD||!load.p_memsz)continue;
        if(load.p_vaddr>UINT64_MAX-interpreterLoadBias_)return false;
        const uint64_t start=interpreterLoadBias_+load.p_vaddr;
        if(load.p_memsz>UINT64_MAX-start||
           !interpreterElfSource_.MemorySpanCovered(interpreterLoadBias_,start,start+load.p_memsz))return false;
        const uint64_t tail=load.p_memsz-load.p_filesz;
        bool allZero=false;
        if(tail>SIZE_MAX||tail>UINT64_MAX-interpreterTailBytesObserved_||
           !execMemory_->ObserveCapturedZeroSpan(start+load.p_filesz,static_cast<size_t>(tail),allZero)){
            interpreterFirstFailedLoadOrdinal_=ph;return false;
        }
        if(tail){
            if(interpreterTailSpansObserved_==SIZE_MAX)return false;
            ++interpreterTailSpansObserved_;interpreterTailBytesObserved_+=tail;
            if(!allZero)++interpreterNonzeroTailSpans_;
        }
    }
    interpreterMemorySpansCaptured_=true;interpreterTailsObserved_=true;
    const auto& map=execMemory_->PrivateRegions()[interpreterMappingOrdinal_];
    const size_t fileBytes=interpreterSourceFileOriginal_->RetainedByteCount();
    if(interpreterEntryFileOffset_>=fileBytes)return false;
    uint64_t prefix=map.end-rip;
    if(prefix>15)prefix=15;
    if(prefix>fileBytes-interpreterEntryFileOffset_)prefix=fileBytes-interpreterEntryFileOffset_;
    if(!prefix)return false;interpreterPrefixBytes_=static_cast<size_t>(prefix);
    interpreterEntryMatched_=execMemory_->SameCapturedBytes(rip,
        interpreterSourceFileOriginal_->PrivateBody()+static_cast<size_t>(interpreterEntryFileOffset_),
        interpreterPrefixBytes_);
#ifdef TEMPLATE_NATIVE_ROOT_MEMORY_REGRESSION
    if(interpreterEntryMatched_&&!ObserveActualInterpreterFileSpansRegression(*execMemory_,interpreterElfSource_,interpreterLoadBias_))return false;
#endif
    // File correspondence and observed memory tails do not prove padding, kernel
    // loader semantics, later relocation/provider/own-writer or alias authority.
    return interpreterEntryMatched_;
#else
    return false;
#endif
}
inline bool OriginalLinuxSessionGate::DisposeOriginalTaskInputsEligible() noexcept {
    if(state_&&!state_->disposed&&!state_->unlaunchedDisposalResult)return false;
    taskDisposalRequested_=true;bool all=true;
    // Actual memory descriptor borrows the SAME held task; close it before
    // task-directory inputs and while the original supervisor remains live.
    if(rootMemory_)all=rootMemory_->DisposeIndependentlyEligible()&&all;
    if(execMemory_)all=execMemory_->DisposeIndependentlyEligible()&&all;
    auto* supervisor=&platform_.pending.OriginalSupervisorOwner();auto* deadline=&platform_.pending.OriginalDeadlineOwner();
    for(auto& wait:waits_)if(wait.native->attempted){
        const bool stat=wait.native->stat.descriptor.CloseOnce(deadline,supervisor);
        const bool status=wait.native->status.descriptor.CloseOnce(deadline,supervisor);
        all=stat&&status&&all;
    }
    for(size_t i=0;i<template_creator_linux::TraceeLimit;++i)
        if(tasks_[i].bindAttempted)all=tasks_[i].task->DisposeEligible(&platform_.pending.OriginalSupervisorOwner())&&all;
    return all;
}
}
#endif

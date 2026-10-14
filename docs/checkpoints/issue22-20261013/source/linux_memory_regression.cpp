// UNRUN actual-native mapped-span assertions; no manufactured memory rows.
#include "linux_memory.hpp"
#include "linux_memory_regression_receipt.hpp"
#include "linux_elf_interpreter_source.hpp"
#include <atomic>
namespace template_creator_linux {
namespace {
// Fixed source storage; no worker stack or fabricated native observations.
std::atomic<unsigned> rootMemoryRegressionReceipt{0};
std::atomic<unsigned> interpreterFileRegressionReceipt{0};
}
bool InvokeActualRootExecFiveSpanRegression(const OriginalLinuxMappedMemory& owner,bool ownPipeSourceChecks) noexcept;
// Call only with the genuine owner from an admitted native root exec-entry
// observation. This does not manufacture a stopped task or select admission.
bool VerifyActualRootExecFiveSpanCoverage(const OriginalLinuxMappedMemory& owner) noexcept {
    const auto* state=owner.OriginalStoppedState();const auto* event=owner.OriginalStoppedEvent();
    if(owner.Failed()||!state||!event||!owner.OriginalStoppedTracee()||!owner.RetainedStopStillObserved()||
       !OriginalRestartAdmissionShape(*event,RestartAdmissionCut::BeforeReservation)||
       event->nativeStop!=NativeStopKind::SyscallEntry||!event->syscallInfoShape||
       event->syscallInfo.op!=PTRACE_SYSCALL_INFO_ENTRY||event->syscallInfo.entry.nr!=SYS_execve||
       !event->registersCaptured||!state->imageBytes||!owner.SpanCount()||
       owner.ThreadOriginalCount()!=owner.SpanCount()+1||
       state->imageBytes>state->originalImage.size()||state->argumentBytes>state->originalArgBytes.size()||
       state->environmentBytes>state->originalEnvBytes.size()||state->argumentCount>=state->originalArgv.size()||
       state->environmentCount>=state->originalEnvironment.size())return false;
    // Readonly source-stage surrogates exercise the SAME production predicate;
    // no copied view is submitted to ptrace or represented as a native return.
    Event reserved=*event;reserved.restartAttempted=true;reserved.restartAdmissionActive=true;
    if(!OriginalRestartAdmissionShape(reserved,RestartAdmissionCut::ReservedGate)||
       OriginalRestartAdmissionShape(reserved,RestartAdmissionCut::BeforeReservation)||
       OriginalRestartAdmissionShape(reserved,static_cast<RestartAdmissionCut>(-1)))return false;
    reserved.restartAdmissionActive=false;
    if(OriginalRestartAdmissionShape(reserved,RestartAdmissionCut::ReservedGate))return false;
    reserved.restartAdmissionActive=true;reserved.continueNativeAttempted=true;
    if(OriginalRestartAdmissionShape(reserved,RestartAdmissionCut::ReservedGate))return false;
    reserved.continueNativeAttempted=false;reserved.continueNativeReturned=true;
    if(OriginalRestartAdmissionShape(reserved,RestartAdmissionCut::ReservedGate))return false;
    struct Source {const void* bytes;size_t count;};
    const std::array<Source,5> sources{{{state->originalImage.data(),state->imageBytes},
        {state->originalArgv.data(),(state->argumentCount+1)*sizeof(char*)},
        {state->originalEnvironment.data(),(state->environmentCount+1)*sizeof(char*)},
        {state->originalArgBytes.data(),state->argumentBytes},{state->originalEnvBytes.data(),state->environmentBytes}}};
    for(const auto& source:sources){const auto address=reinterpret_cast<uintptr_t>(source.bytes);
        if(source.count>UINT64_MAX-address||!owner.SameCapturedBytes(address,source.bytes,source.count))return false;
    }
    for(size_t i=0;i<owner.SpanCount();++i){const auto& span=owner.PrivateSpans()[i];const auto& row=span.captured;
        if(span.mappingOrdinal>=owner.RegionCount()||!row.captureAttempted||!row.captured||row.shapeFailure||
           row.capturedBegin>=row.capturedEnd||row.retainedBytes!=row.capturedEnd-row.capturedBegin||
           row.firstReadOrdinal>owner.ReadCount()||!row.readCount||row.readCount>owner.ReadCount()-row.firstReadOrdinal)return false;
        const auto& mapping=owner.PrivateRegions()[span.mappingOrdinal];
        if(row.capturedBegin<mapping.begin||row.capturedEnd>mapping.end||mapping.permissions[0]!='r'||
           mapping.permissions[3]!='p')return false;
        // Actual source-owned argument mappings must also remain disjoint from
        // the independently retained shared child-outcome writer. This checks
        // real captured span extents, not a manufactured kernel read result.
        if(!state->childMapAttempted||!state->originalChildMapResult||
           state->originalChildMapResult==MAP_FAILED||!state->childMapBytes||
           state->childUnmapped||state->childUnmapAttempted)return false;
        const auto sharedBegin=reinterpret_cast<uintptr_t>(state->originalChildMapResult);
        if(state->childMapBytes>UINTPTR_MAX-sharedBegin)return false;
        const auto sharedEnd=sharedBegin+state->childMapBytes;
        if(row.capturedBegin<sharedEnd&&sharedBegin<row.capturedEnd)return false;
        for(size_t j=0;j<i;++j){const auto& previous=owner.PrivateSpans()[j].captured;
            if(row.capturedBegin<previous.capturedEnd&&previous.capturedBegin<row.capturedEnd)return false;
        }
        // Every captured address must belong to an actual required source span;
        // global min/max envelopes cannot pass merely by containing all five.
        uint64_t covered=row.capturedBegin;
        while(covered<row.capturedEnd){uint64_t next=covered;
            for(const auto& source:sources){const uint64_t first=reinterpret_cast<uintptr_t>(source.bytes),last=first+source.count;
                if(first<=covered&&covered<last&&last>next)next=last;
            }
            if(next==covered)return false;covered=next<row.capturedEnd?next:row.capturedEnd;
        }
        size_t observed=0;
        for(size_t j=0;j<row.readCount;++j){const auto& read=owner.PrivateReads()[row.firstReadOrdinal+j];
            if(!read.attempted||read.error||read.returned<=0||static_cast<uint64_t>(read.returned)>read.requested||
               !read.requested||read.requested>65536||read.virtualAddress!=row.capturedBegin+observed||
               read.destinationOffset!=row.destinationOffset+observed||
               static_cast<uint64_t>(read.returned)>row.retainedBytes-observed||
               !read.before.shapeValid||!read.after.shapeValid||read.before.expired||read.after.expired||
               read.after.nanoseconds<read.before.nanoseconds)return false;
            observed+=static_cast<size_t>(read.returned);
        }
        if(observed!=row.retainedBytes)return false;
    }
    return true;
}
bool InvokeActualRootExecFiveSpanRegression(const OriginalLinuxMappedMemory& owner,bool ownPipeSourceChecks) noexcept {
    unsigned expected=0;
    if(!rootMemoryRegressionReceipt.compare_exchange_strong(expected,1)){
        rootMemoryRegressionReceipt.store(3,std::memory_order_release);return false;
    }
    const bool memoryResult=VerifyActualRootExecFiveSpanCoverage(owner);
    const bool result=memoryResult&&ownPipeSourceChecks;
    rootMemoryRegressionReceipt.store(result?2:3,std::memory_order_release);
    return result;
}
bool ActualRootExecFiveSpanRegressionPassed() noexcept {
    const unsigned interpreter=interpreterFileRegressionReceipt.load(std::memory_order_acquire);
    return rootMemoryRegressionReceipt.load(std::memory_order_acquire)==5&&(interpreter==0||interpreter==2);
}
bool ExpectActualInterpreterFileSpansRegression() noexcept {
    unsigned expected=0;
    if(interpreterFileRegressionReceipt.compare_exchange_strong(expected,1))return true;
    interpreterFileRegressionReceipt.store(3,std::memory_order_release);return false;
}
bool ObserveActualInterpreterFileSpansRegression(const OriginalLinuxMappedMemory& owner,
    const OriginalLinuxElfInterpreterSource& source,unsigned long long bias) noexcept {
    const auto check=[&]() noexcept {
        const auto* event=owner.OriginalStoppedEvent();
        if(owner.Failed()||!owner.RetainedStopStillObserved()||!event||!owner.OriginalStoppedState()||
           event->nativeStop!=NativeStopKind::Event||
           (static_cast<unsigned>(event->status)>>16)!=PTRACE_EVENT_EXEC||
           !OriginalRestartAdmissionShape(*event,RestartAdmissionCut::ReservedGate)||
           !source.projected||!source.fileOriginal||!source.fileOriginal->CapturedSourceBytes()||
           !owner.SpanCount()||owner.ThreadOriginalCount()!=owner.SpanCount()+1)return false;
        uint64_t minimum=UINT64_MAX;
        for(size_t ph=0;ph<source.header.e_phnum;++ph){Elf64_Phdr load{};
            if(!source.OriginalProgramHeader(ph,load))return false;
            if(load.p_type!=PT_LOAD||!load.p_filesz)continue;
            if(load.p_vaddr>UINT64_MAX-bias||load.p_filesz>SIZE_MAX)return false;
            const uint64_t start=bias+load.p_vaddr;
            if(load.p_filesz>UINT64_MAX-start||!source.FileSpanCovered(bias,start,start+load.p_filesz)||
               !owner.SameCapturedBytes(start,source.fileOriginal->PrivateBody()+static_cast<size_t>(load.p_offset),
                    static_cast<size_t>(load.p_filesz)))return false;
            if(start<minimum)minimum=start;
        }
        if(minimum==UINT64_MAX||!minimum||source.FileSpanCovered(bias,minimum-1,minimum))return false;
        // Real captured p_memsz coverage and tail observation. A nonzero result
        // is retained by production, not relabeled as a native loader failure.
        uint64_t memoryMinimum=UINT64_MAX;
        for(size_t ph=0;ph<source.header.e_phnum;++ph){Elf64_Phdr load{};
            if(!source.OriginalProgramHeader(ph,load))return false;
            if(load.p_type!=PT_LOAD||!load.p_memsz)continue;
            if(load.p_vaddr>UINT64_MAX-bias||load.p_memsz<load.p_filesz)return false;
            const uint64_t start=bias+load.p_vaddr,tail=load.p_memsz-load.p_filesz;
            bool allZero=false;
            if(load.p_memsz>UINT64_MAX-start||tail>SIZE_MAX||
               !source.MemorySpanCovered(bias,start,start+load.p_memsz)||
               !owner.ObserveCapturedZeroSpan(start+load.p_filesz,static_cast<size_t>(tail),allZero))return false;
            if(start<memoryMinimum)memoryMinimum=start;
        }
        if(memoryMinimum==UINT64_MAX||!memoryMinimum||
           source.MemorySpanCovered(bias,memoryMinimum-1,memoryMinimum))return false;
        bool outsideZero=true;
        if(owner.ObserveCapturedZeroSpan(memoryMinimum-1,1,outsideZero)||outsideZero)return false;
        outsideZero=true;
        if(owner.ObserveCapturedZeroSpan(UINT64_MAX,1,outsideZero)||outsideZero)return false;
        for(size_t i=0;i<owner.SpanCount();++i){const auto& row=owner.PrivateSpans()[i].captured;
            if(!row.captureAttempted||!row.captured||row.shapeFailure||row.capturedBegin>=row.capturedEnd||
               row.retainedBytes!=row.capturedEnd-row.capturedBegin||
                !source.MemorySpanCovered(bias,row.capturedBegin,row.capturedEnd)||
               row.firstReadOrdinal>owner.ReadCount()||!row.readCount||row.readCount>owner.ReadCount()-row.firstReadOrdinal)return false;
            size_t observed=0;
            for(size_t j=0;j<row.readCount;++j){const auto& read=owner.PrivateReads()[row.firstReadOrdinal+j];
                if(!read.attempted||read.error||read.returned<=0||!read.requested||read.requested>65536||
                   static_cast<uint64_t>(read.returned)>read.requested||
                   static_cast<uint64_t>(read.returned)>row.retainedBytes-observed||
                   read.virtualAddress!=row.capturedBegin+observed||read.destinationOffset!=row.destinationOffset+observed||
                   !read.before.shapeValid||!read.after.shapeValid||read.before.expired||read.after.expired||
                   read.after.nanoseconds<read.before.nanoseconds)return false;
                observed+=static_cast<size_t>(read.returned);
            }
            if(observed!=row.retainedBytes)return false;
            for(size_t j=0;j<i;++j){const auto& prior=owner.PrivateSpans()[j].captured;
                if(row.capturedBegin<prior.capturedEnd&&prior.capturedBegin<row.capturedEnd)return false;
                // Readonly range surrogate: a real gap cannot be laundered into
                // a captured min/max hull by the SAME production union predicate.
                if(prior.capturedEnd<row.capturedBegin&&
                    !source.MemorySpanCovered(bias,prior.capturedEnd,row.capturedBegin)&&
                    source.MemorySpanCovered(bias,prior.capturedBegin,row.capturedEnd))return false;
            }
        }
        return true;
    };
    const bool result=check();unsigned expected=1;
    if(!result||!interpreterFileRegressionReceipt.compare_exchange_strong(expected,2)){
        interpreterFileRegressionReceipt.store(3,std::memory_order_release);return false;
    }
    return true;
}
bool ObserveActualRootExecReservedGateRegression(const Event& original) noexcept {
    unsigned expected=2;
    const bool shape=OriginalRestartAdmissionShape(original,RestartAdmissionCut::ReservedGate)&&
        original.nativeStop==NativeStopKind::SyscallEntry&&original.syscallInfoShape&&
        original.syscallInfo.op==PTRACE_SYSCALL_INFO_ENTRY&&original.syscallInfo.entry.nr==SYS_execve&&
        original.originalResumeRequest==PTRACE_SYSCALL&&original.deliveredSignal==0;
    if(!shape||!rootMemoryRegressionReceipt.compare_exchange_strong(expected,4)){
        rootMemoryRegressionReceipt.store(3,std::memory_order_release);return false;
    }
    return true;
}
bool ObserveActualRootExecContinuedRegression(const Event& original) noexcept {
    unsigned expected=4;
    const bool shape=OriginalRestartSucceeded(original)&&
        original.nativeStop==NativeStopKind::SyscallEntry&&original.syscallInfoShape&&
        original.syscallInfo.op==PTRACE_SYSCALL_INFO_ENTRY&&original.syscallInfo.entry.nr==SYS_execve;
    // Nonowning copied receipt negatives, never claimed as native failures.
    Event nonzero=original;nonzero.continueResult=1;
    Event missingReturn=original;missingReturn.continueNativeReturned=false;
    Event active=original;active.restartAdmissionActive=true;
    if(!shape||OriginalRestartSucceeded(nonzero)||OriginalRestartSucceeded(missingReturn)||
       OriginalRestartSucceeded(active)||!rootMemoryRegressionReceipt.compare_exchange_strong(expected,5)){
        rootMemoryRegressionReceipt.store(3,std::memory_order_release);return false;
    }
    return true;
}
bool VerifyActualWholeMappedSpan(const OriginalLinuxMappedMemory& owner,size_t ordinal) noexcept {
    if(owner.Failed()||ordinal>=owner.RegionCount()||!owner.OriginalStoppedState()||
       !owner.OriginalStoppedTracee()||!owner.OriginalStoppedEvent())return false;
    if(owner.ThreadOriginalCount()<2)return false;
    for(size_t i=0;i<owner.ThreadOriginalCount();++i){
        const auto& thread=owner.PrivateThreadOriginals()[i];
        if(!thread.attempted||thread.error||thread.returned<=0||
           thread.returned!=owner.OriginalStoppedState()->originalCreatingThread.result||
           !thread.before.shapeValid||!thread.after.shapeValid||thread.before.expired||thread.after.expired||
           thread.after.nanoseconds<thread.before.nanoseconds)return false;
    }
    const auto& maps=owner.OriginalMaps();const auto& region=owner.PrivateRegions()[ordinal];
    if(!maps.attempted||!maps.eof||maps.shapeFailure||!maps.descriptor.closed||
       !region.captureAttempted||!region.captured||region.shapeFailure||region.permissions[0]!='r'||
       region.end<=region.begin||region.retainedBytes!=region.end-region.begin||
       !region.readCount||region.firstReadOrdinal>owner.ReadCount()||
       region.readCount>owner.ReadCount()-region.firstReadOrdinal)return false;
    size_t extent=0;
    for(size_t i=0;i<region.readCount;++i){
        const auto& row=owner.PrivateReads()[region.firstReadOrdinal+i];
        if(!row.attempted||row.error||row.returned<=0||static_cast<uint64_t>(row.returned)>row.requested||
           !row.requested||row.requested>65536||row.virtualAddress!=region.begin+extent||
           row.destinationOffset!=region.destinationOffset+extent||
           !row.before.shapeValid||!row.after.shapeValid||row.before.expired||row.after.expired||
           row.after.nanoseconds<row.before.nanoseconds)return false;
        extent+=static_cast<size_t>(row.returned);
    }
    return extent==region.retainedBytes;
}
// Permission/unreadable/native errors must stay failed with their real raw
// output. No zero page or physical-file equivalence is supplied by this test.
bool VerifyActualMappedReadFailure(const OriginalLinuxMappedMemory& owner) noexcept {
    if(!owner.Failed())return false;
    for(size_t i=0;i<owner.ReadCount();++i){const auto& row=owner.PrivateReads()[i];
        if(row.attempted&&row.returned<0&&row.error)return true;
    }return false;
}
}

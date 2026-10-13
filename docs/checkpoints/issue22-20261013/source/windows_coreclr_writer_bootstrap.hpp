// UNCOMPILED writer-entry candidate capture. Neither an actual writer stop nor
// its return slot proves a preceding CALL or grants initializer consumption.
#pragma once
#include "windows_coreclr_debug_cause.hpp"
#include "windows_coreclr_compiled_geometry.hpp"
namespace template_creator {
struct OriginalCoreClrWriterBootstrap {
    struct ReadOriginal {
        HANDLE process=nullptr;const void* address=nullptr;void* destination=nullptr;SIZE_T requested=0;
        SIZE_T actual=0;BOOL returned=FALSE;DWORD error=0;
        template_creator_bridge::OriginalWindowsPendingCall* pendingOriginal=nullptr;
        bool attempted=false,returnedObserved=false;
    };
    OriginalCoreClrThreadContext context,after;
    OriginalCoreClrDebugCause cause;
    const OriginalCoreClrInitializeExport* writerOriginal=nullptr;
    const LoadedImageObservation* callerImageOriginal=nullptr;
    const OriginalCoreClrCompiledGeometry* callerGeometryOriginal=nullptr;
    std::array<ReadOriginal,2> reads{};
    DWORD64 returnCandidate=0,revalidatedReturn=0,callback=0;
    OriginalAmd64RuntimeFunction callerFunction{};
    SIZE_T callerFunctionOrdinal=SIZE_MAX,readCount=0;
    bool attempted=false,captured=false,nonNullPrecallCandidate=false;
    const char* firstFailure=nullptr;
private:
    bool Fail(const char* reason) noexcept {if(!firstFailure)firstFailure=reason;return false;}
    bool ReadReturn(DWORD64 stackAddress,template_creator_bridge::OriginalWindowsPendingCalls& pending,
            const template_creator_bridge::WindowsOriginalCommand& command,DWORD64& destination) noexcept {
        if(readCount==reads.size() || !context.captured || !writerOriginal || !writerOriginal->imageOriginal ||
            !stackAddress || stackAddress>UINTPTR_MAX-7)return Fail("writer_return_read_shape");
        auto& row=reads[readCount++];row.process=writerOriginal->imageOriginal->retainedProcess;
        row.address=reinterpret_cast<const void*>(static_cast<std::uintptr_t>(stackAddress));
        row.destination=&destination;row.requested=sizeof(destination);
        row.pendingOriginal=pending.BeginOriginal(this,&row,row.destination,row.requested,&command,offsetof(ReadOriginal,actual));
        if(!row.pendingOriginal)return Fail("writer_return_pending");
        row.attempted=true;row.returned=ReadProcessMemory(row.process,row.address,row.destination,row.requested,&row.actual);
        if(!row.returned)row.error=GetLastError();row.returnedObserved=true;
        if(!pending.ReturnedOriginal(*row.pendingOriginal,row.returned,row.error,row.actual))return Fail("writer_return_unknown");
        return row.returned && row.actual==row.requested?true:Fail("writer_return_failed_or_partial");
    }
public:
    bool Capture(const State& state,const Event& saved,const Event& born,const OriginalCoreClrContextChange& pair,
            const OriginalCoreClrInitializeExport& writer,const LoadedImageObservation& caller,
            const OriginalCoreClrCompiledGeometry& geometry,
            template_creator_bridge::OriginalWindowsPendingCalls& pending,
            const template_creator_bridge::WindowsOriginalCommand& command) noexcept {
        if(attempted)return false;attempted=true;writerOriginal=&writer;callerImageOriginal=&caller;callerGeometryOriginal=&geometry;
#if defined(_M_AMD64) || defined(__x86_64__)
        if(writer.symbol!=OriginalCoreClrInitializeExport::Symbol::ErrorWriter ||
            writer.result!=OriginalCoreClrInitializeExport::Result::Direct || !writer.targetExecutableObserved ||
            !writer.imageOriginal || !pair.movableEnabled || pair.movableAddress!=reinterpret_cast<std::uintptr_t>(writer.address) ||
            !context.Capture(state,saved,pending,command,&born) || !cause.Observe(state,saved,context,pair,pair.movableSlot))
            return Fail("writer_original_cause_or_export");
        if(born.createdThreadProcessResource>=state.resourceCount || born.createdThreadMemberOrdinal>=state.memberCount)
            return Fail("writer_original_process");
        const auto& process=state.resources[born.createdThreadProcessResource];
        const auto& member=state.members[born.createdThreadMemberOrdinal];
        if(process.kind!=Kind::Process || !process.acquired || process.attempted || process.unknownOutput ||
            process.original!=writer.imageOriginal->retainedProcess || process.original!=caller.retainedProcess ||
            !same_time(member.birth,writer.imageOriginal->birth) || !same_time(member.birth,caller.birth))
            return Fail("writer_caller_process_occurrence");
        callback=context.effective.Rcx;
        if(!ReadReturn(context.effective.Rsp,pending,command,returnCandidate) ||
            !ReadReturn(context.effective.Rsp,pending,command,revalidatedReturn) ||
            returnCandidate!=revalidatedReturn)return Fail("writer_return_changed");
        const DWORD64 base=reinterpret_cast<std::uintptr_t>(caller.nativeBase);
        if(!caller.nativeBase || returnCandidate<base || returnCandidate-base>UINT32_MAX ||
            !geometry.MatchesMappedCodeSpan(caller,static_cast<DWORD>(returnCandidate-base),1) ||
            !geometry.ContainingRuntimeFunction(caller,static_cast<DWORD>(returnCandidate-base),callerFunction,callerFunctionOrdinal))
            return Fail("writer_continuation_compiled_range");
        if(!after.Capture(state,saved,pending,command,&born) ||
            !OriginalCoreClrContextChange::CompleteContext(after.effective) ||
            !OriginalCoreClrContextChange::GeneralEqual(context.effective,after.effective) ||
            !OriginalCoreClrContextChange::DebugEqual(context.effective,after.effective))
            return Fail("writer_context_changed");
        nonNullPrecallCandidate=callback!=0;captured=true;
        return true; // Candidate only. Null post-call is retained distinctly.
#else
        return Fail("writer_architecture_unsupported");
#endif
    }
};
}

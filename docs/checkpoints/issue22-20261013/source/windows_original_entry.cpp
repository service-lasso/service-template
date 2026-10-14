// UNCOMPILED private standalone entry. Arguments select finite storage and poll
// geometry; they never grant input/native/source authority. Original module and
// runtime admission, LaunchAssociationsEstablished and whole fit remain required.
#ifdef _WIN32
#include "windows_private_selection_decoder.hpp"
#include <exception>
namespace template_creator_bridge {
struct OriginalEntryProgress {
    bool closureAttempted=false,closureReturned=false;
    bool sleepAttempted=false,sleepCompleted=false;
    DWORD sleepReturned=0;
};
struct OriginalWindowsEntry {
    int originalArgc=0;
    wchar_t** originalArgv=nullptr; // Native startup originals remain live in wmain.
    OriginalWindowsSelectionReader* reader=nullptr;
    OriginalWindowsSelectionDecoder* decoder=nullptr;
    OriginalWindowsInvocation* invocation=nullptr;
    std::unique_ptr<OriginalEntryProgress[]> progress;
    SIZE_T progressCapacity=0,progressCount=0;
    DWORD pollMilliseconds=0;
    bool argumentsAccepted=false,captureReturned=false,decodeReturned=false;
    bool invocationReturned=false,progressExhausted=false;
    bool retainedWaitAttempted=false,retainedWaitCompleted=false;
    HANDLE retainedProcessPseudo=nullptr;
    DWORD retainedWaitReturned=WAIT_FAILED,retainedWaitError=0;
    std::exception_ptr nativeOriginal;
    static SIZE_T Decimal(const wchar_t* text) {
        if(!text || !*text)throw std::invalid_argument("private finite geometry integer");
        SIZE_T value=0,units=0;
        for(;*text;++text) {
            if(++units>20 || *text<L'0' || *text>L'9')throw std::invalid_argument("private finite decimal geometry");
            const SIZE_T digit=static_cast<SIZE_T>(*text-L'0');
            if(value>(SIZE_MAX-digit)/10)throw std::invalid_argument("private geometry overflow");
            value=value*10+digit;
        }
        if(!value)throw std::invalid_argument("positive private geometry");
        return value;
    }
    void Begin(int argc,wchar_t** argv) noexcept {
        originalArgc=argc;originalArgv=argv;
        try {
            // path, maximum selection bytes, read rows, progress rows, poll ms.
            if(argc!=6 || !argv)throw std::invalid_argument("private native entry arguments");
            const SIZE_T bytes=Decimal(argv[2]),reads=Decimal(argv[3]);
            const SIZE_T observations=Decimal(argv[4]),poll=Decimal(argv[5]);
            if(poll>=INFINITE || observations>SIZE_MAX/sizeof(OriginalEntryProgress))
                throw std::invalid_argument("private native poll representation");
            progress=std::make_unique<OriginalEntryProgress[]>(observations);
            progressCapacity=observations;pollMilliseconds=static_cast<DWORD>(poll);
            reader=new OriginalWindowsSelectionReader(argv[1],bytes,reads);
            // Attach decoder before first Capture so failed captures still have
            // a source-owned no-consumer disposal path.
            decoder=new OriginalWindowsSelectionDecoder(*reader);
            argumentsAccepted=true;
            captureReturned=reader->CaptureOriginal();
            if(!captureReturned)return;
            decodeReturned=decoder->DecodeOriginal();
            if(!decodeReturned)return;
            invocation=decoder->PrechargeInvocationOriginal();
            if(!invocation)return;
            invocationReturned=invocation->BeginAndInvokeOriginal();
        } catch(...) {nativeOriginal=std::current_exception();}
    }
    bool ObserveLocalSettlement() noexcept {
        if(!decoder)return !reader || reader->ClosedKnownOriginal(); // No Capture before decoder attachment.
        if(progressCount==progressCapacity){progressExhausted=true;return false;}
        auto& row=progress[progressCount++];row.closureAttempted=true;
        row.closureReturned=decoder->ObserveIndependentlyEligibleClosure();
        if(!row.closureReturned)return false;
        const auto* retained=decoder->PrivateInvocation();
        if(!retained)return true; // Malformed selection, no interpreter/child effect.
        // Decoder retains failed Precharge's original invocation even when its
        // returned pointer was null. Publication must use that SAME owner.
        invocation=const_cast<OriginalWindowsInvocation*>(retained);
        return invocation->PublishLocalSettlementOriginal().published;
    }
    void PollDelay() noexcept {
        auto& row=progress[progressCount-1];
        row.sleepAttempted=true;
        row.sleepReturned=SleepEx(pollMilliseconds,FALSE);
        row.sleepCompleted=true;
        // This is caller scheduling only, never a renewed command deadline.
    }
    bool ReservedEntrySourceBytes(SIZE_T& bytes) const noexcept {
        bytes=sizeof(*this);
        if(progressCapacity>(SIZE_MAX-bytes)/sizeof(OriginalEntryProgress))return false;
        bytes+=progressCapacity*sizeof(OriginalEntryProgress);
        // This narrow counter intentionally excludes reader/decoder/invocation,
        // Python/runtime/CRT startup argv, allocators, native/kernel and stacks.
        // Their simultaneous originals require the separate complete profile.
        return true;
    }
    void RetainUnresolvedOwner() noexcept {
        // No selected owner is destroyed on finite observation exhaustion.
        // Waiting on our own original process pseudo capability preserves the
        // actual main/interpreter/registry while no further source observations
        // can fit. It is not a timeout, exit proof, cleanup or qualification.
        retainedProcessPseudo=GetCurrentProcess();retainedWaitAttempted=true;
        retainedWaitReturned=WaitForSingleObject(retainedProcessPseudo,INFINITE);
        if(retainedWaitReturned==WAIT_FAILED)retainedWaitError=GetLastError();
        retainedWaitCompleted=true;
        // An unexpected wait return grants no disposal. Keep the same owner;
        // do not retry an uncertain operation or return from this entry.
        for(;;)YieldProcessor();
    }
};
// Resident original, including startup/parser failures and final Python triples.
// No automatic destructor invokes Python or closes a retained native original.
static OriginalWindowsEntry* originalEntry=nullptr;
}
#if defined(TEMPLATE_NATIVE_CHAIN_REGRESSION)
#include "windows_chain_regression_receipt.hpp"
#endif
int wmain(int argc,wchar_t** argv) {
    using namespace template_creator_bridge;
    try {originalEntry=new OriginalWindowsEntry();}
    catch(...) {return 2;} // Allocation failed before any native/input effect.
    originalEntry->Begin(argc,argv);
    while(!originalEntry->ObserveLocalSettlement()) {
        if(originalEntry->progressExhausted)originalEntry->RetainUnresolvedOwner();
        originalEntry->PollDelay();
    }
    // No Py_Finalize/GC/destructor is invoked by this source return. Independently
    // admitted runtime and own cleanup relations remain required for useful acceptance;
    // later external exact exit/family/EOF qualification is not self-observed.
#if defined(TEMPLATE_NATIVE_CHAIN_REGRESSION)
    return originalEntry->invocation && originalEntry->invocation->localSettlement.published &&
        !originalEntry->nativeOriginal && template_creator::OriginalChainRegressionPassed()?0:2;
#else
    return originalEntry->invocation && originalEntry->invocation->localSettlement.published &&
        !originalEntry->invocation->localSettlement.qualificationFailed && !originalEntry->nativeOriginal &&
        originalEntry->invocation->SourceQualified()?0:2;
#endif
}
#endif

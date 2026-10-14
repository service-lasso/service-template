// UNCOMPILED source-owned standalone main lifetime. Native original capabilities
// and installed interpreter/source inputs still need complete admission. This
// owner has no argv integer-to-handle importer, borrowed issuer or exit callback.
#pragma once
#include "windows_standalone_host.hpp"
#include "windows_main_receipt.hpp"
#include "windows_original_route_producer.hpp"
#include "windows_original_route_arguments.hpp"
#include "windows_original_bootstrap.hpp"
#include "windows_private_selection_reader.hpp"
namespace template_creator_bridge {
struct OriginalMainThreadObservation {
    HANDLE currentPseudo=nullptr;
    DWORD currentId=0;
    DWORD waited=WAIT_FAILED,waitError=0;
    BOOL timesReturned=FALSE;
    DWORD timesError=0;
    FILETIME born{},exited{},kernel{},user{};
    bool attempted=false,sameOriginal=false;
};
struct OriginalWindowsNativeMain;
struct OriginalWindowsInvocation;
struct OriginalMainRetainedOutcome {
    const OriginalWindowsNativeMain* owner=nullptr;
    const OriginalWindowsInvocation* invocation=nullptr;
    const OriginalWindowsStandaloneHost* host=nullptr;
    const OriginalWindowsInterpreter* interpreter=nullptr;
    const OriginalWindowsMainReceipt* receipt=nullptr;
    SIZE_T hostProgressOrdinal=SIZE_MAX;
    bool published=false,physicalHandlesDisposed=false,sourceQualificationFailed=true;
};
struct OriginalWindowsNativeMain {
    const OriginalWindowsInvocation* invocationOriginal=nullptr;
    OriginalWindowsInterpreter* interpreter=nullptr;
    OriginalWindowsStandaloneHost* host=nullptr;
    OriginalWindowsMainReceipt* receipt=nullptr;
    const OriginalWindowsSelectionReader* selectionInputOriginal=nullptr;
    OriginalBootstrapCapture* bootstrapOriginal=nullptr;
    OriginalWindowsRouteProducer* routeProducer=nullptr;
    OriginalWindowsRouteArguments* nativeRouteArguments=nullptr;
    bool nativeRouteBindingAttempted=false;
    SIZE_T receiptHostProgressOrdinal=SIZE_MAX;
    bool receiptStartAttempted=false,receiptStartReturned=false;
    bool mainHandleDisposalAttempted=false;
    OriginalMainRetainedOutcome retainedOutcome{};
    std::unique_ptr<OriginalMainThreadObservation[]> threadObservations;
    SIZE_T observationCapacity=0,observationCount=0;
    HANDLE originalProcessPseudo=nullptr,originalThreadPseudo=nullptr,originalThread=nullptr;
    DWORD originalThreadId=0,duplicateError=0;
    DWORD duplicateAccessOriginal=0,duplicateOptionsOriginal=0;
    BOOL duplicateInheritOriginal=FALSE;
    BOOL duplicateReturned=FALSE;
    FILETIME born{},exited{},kernel{},user{};
    BOOL birthReturned=FALSE;
    DWORD birthError=0;
    bool beginAttempted=false,threadAcquired=false,threadUnknown=false;
    bool initialized=false,connectAttempted=false,connected=false,creatorStarted=false,observationExhausted=false;
    bool threadCloseAttempted=false;
    BOOL threadCloseReturned=FALSE;
    DWORD threadCloseError=0;
    std::exception_ptr originalException;
private:
    ~OriginalWindowsNativeMain()=default;
public:
    explicit OriginalWindowsNativeMain(SIZE_T selectedMainObservations,SIZE_T selectedReceiptPolls,
        SIZE_T selectedReceiptStackBytes) {
        if (!selectedMainObservations || selectedMainObservations>SIZE_MAX/sizeof(OriginalMainThreadObservation))
            throw std::invalid_argument("precharged original main lifetime observations");
        if (!selectedReceiptPolls || !selectedReceiptStackBytes ||
            selectedReceiptPolls>(SIZE_MAX-sizeof(OriginalWindowsMainReceipt))/sizeof(OriginalMainReceiptPoll))
            throw std::invalid_argument("precharged original main receipt backing");
        // Check the complete backing allocated by THIS constructor before its
        // first allocation. Individually valid row products do not establish
        // that their sum with both separately allocated owners fits SIZE_T.
        SIZE_T constructorSourceBytes=sizeof(*this);
        const auto add=[&](SIZE_T amount) {
            if(amount>SIZE_MAX-constructorSourceBytes)return false;
            constructorSourceBytes+=amount;return true;
        };
        if(!add(selectedMainObservations*sizeof(OriginalMainThreadObservation)) ||
            !add(sizeof(OriginalWindowsMainReceipt)) ||
            !add(selectedReceiptPolls*sizeof(OriginalMainReceiptPoll)) ||
            !add(sizeof(OriginalWindowsRouteProducer)))
            throw std::invalid_argument("aggregate original main constructor backing");
        // This is source arithmetic, not allocator/native stack/kernel/runtime
        // fit. BeginOriginal's later interpreter/host owners remain additional.
        threadObservations=std::make_unique<OriginalMainThreadObservation[]>(selectedMainObservations);
        observationCapacity=selectedMainObservations;
        receipt=new OriginalWindowsMainReceipt(selectedReceiptPolls,selectedReceiptStackBytes);
        try { routeProducer=new OriginalWindowsRouteProducer(); }
        catch(...) {
            // No native/Python effect has begun in this constructor. Avoid an
            // unreachable precharge allocation if the second owner allocation fails.
            delete receipt;receipt=nullptr;throw;
        }
    }
    OriginalWindowsNativeMain(const OriginalWindowsNativeMain&)=delete;
    OriginalWindowsNativeMain& operator=(const OriginalWindowsNativeMain&)=delete;
    bool BeginOriginal(const OriginalPythonPaths& paths,SIZE_T selectedPathUnits,
        const OriginalWindowsHostSelection& selected) noexcept {
        if (beginAttempted) return false;
        beginAttempted=true;
        try {
            // Allocate the source owners before acquiring this original thread.
            host=new OriginalWindowsStandaloneHost();
            // Attach the full input-selected call backing before native thread,
            // interpreter or channel effects. Failed precharge stays resident.
            if(!host->PreownPendingCallsOriginal(selected.pendingCallRows,selected.observerGeometry,selected.noticeGeometry))return false;
            // Interpreter construction itself observes GetCurrentThreadId.
            // Preown SAME pending/notice backing before that native observation.
            interpreter=new OriginalWindowsInterpreter(paths,selectedPathUnits);
            originalProcessPseudo=GetCurrentProcess(); originalThreadPseudo=GetCurrentThread();
            originalThreadId=GetCurrentThreadId();
            duplicateAccessOriginal=THREAD_QUERY_LIMITED_INFORMATION|SYNCHRONIZE;
            duplicateInheritOriginal=FALSE; duplicateOptionsOriginal=0;
            duplicateReturned=DuplicateHandle(originalProcessPseudo,originalThreadPseudo,originalProcessPseudo,
                &originalThread,duplicateAccessOriginal,duplicateInheritOriginal,duplicateOptionsOriginal);
            if (!duplicateReturned) { duplicateError=GetLastError(); threadUnknown=originalThread!=nullptr; return false; }
            threadAcquired=originalThread && originalThread!=INVALID_HANDLE_VALUE;
            if (!threadAcquired) { threadUnknown=true; return false; }
            birthReturned=GetThreadTimes(originalThread,&born,&exited,&kernel,&user);
            if (!birthReturned) { birthError=GetLastError(); return false; }
            initialized=interpreter->InitializeOriginal() && host->BindOriginalInterpreter(*interpreter) && host->Precharge(selected);
            return initialized;
        } catch (...) { originalException=std::current_exception(); return false; }
    }
    bool ObserveOriginalThread() noexcept {
        if (!threadAcquired || threadUnknown || threadCloseAttempted) return false;
        if (observationCount==observationCapacity) { observationExhausted=true; return false; }
        auto& row=threadObservations[observationCount++]; row.attempted=true;
        row.currentPseudo=GetCurrentThread(); row.currentId=GetCurrentThreadId();
        row.waited=WaitForSingleObject(originalThread,0);
        if (row.waited==WAIT_FAILED) row.waitError=GetLastError();
        row.timesReturned=GetThreadTimes(originalThread,&row.born,&row.exited,&row.kernel,&row.user);
        if (!row.timesReturned) row.timesError=GetLastError();
        row.sameOriginal=row.currentId==originalThreadId && row.waited==WAIT_TIMEOUT && row.timesReturned && birthReturned &&
            row.born.dwLowDateTime==born.dwLowDateTime && row.born.dwHighDateTime==born.dwHighDateTime;
        // Held original came directly from this source's current-thread pseudo
        // capability. Numeric ID equality alone is never this association.
        return row.sameOriginal;
    }
    bool ConnectOriginals(const std::array<HANDLE,3>& actualRootToolSink,HANDLE actualEvidenceRoot,
        const void* actualBody,DWORD actualBodyBytes) noexcept {
        if (!initialized || connectAttempted || !ObserveOriginalThread()) return false;
        connectAttempted=true;
        connected=host->Connect(actualRootToolSink,actualEvidenceRoot,actualBody,actualBodyBytes);
        return connected;
    }
    bool InvokeSelectedCreator(PyObject* actualSelectedSteps) noexcept {
        if (!connected || creatorStarted || !ObserveOriginalThread()) return false;
        creatorStarted=true;
        return host->StartConnectedCreator(actualSelectedSteps);
    }
    bool ProduceAndInvokeSelectedRoute(OriginalPhase phase,PyObject* originalArguments) noexcept {
        if(!connected || creatorStarted || !routeProducer || routeProducer->attempted ||
            !host || !host->core || phase!=host->selected.phase ||
            !ObserveOriginalThread() || !host->core->OnOriginalPythonThread())return false;
        if(!routeProducer->Produce(phase,originalArguments))return false;
        return InvokeSelectedCreator(routeProducer->steps);
    }
    bool BuildAndInvokeNativeSelectedRoute(OriginalWindowsRouteArguments& original) noexcept {
        if(nativeRouteBindingAttempted || !connected || creatorStarted || !host || !host->core ||
            original.OriginalSelectedPhase()!=host->selected.phase || !ObserveOriginalThread() ||
            !host->core->OnOriginalPythonThread())return false;
        // Attach SAME resident conversion owner before imports/allocations can
        // fail. Its selected records are inputs, never an admission grant.
        nativeRouteBindingAttempted=true;nativeRouteArguments=&original;
        if(!original.BuildOriginalArguments())return false;
        return ProduceAndInvokeSelectedRoute(original.OriginalSelectedPhase(),original.OriginalArguments());
    }
    bool ObserveRetainedCustody() noexcept {
        if (receiptStartAttempted || !host || !ObserveOriginalThread()) return false;
        return host->ObserveConnectedHost(); // Physical result only; main remains alive.
    }
    bool StartSameProcessReceipt() noexcept {
        if(receiptStartAttempted || !receipt || !host || !host->progressCount ||
            !ObserveOriginalThread()) return false;
        const auto ordinal=host->progressCount-1;
        if(!host->progress[ordinal].physicalDisposed) return false;
        // This exact completed source progress row and all Python/native
        // originals stay resident. No further host writes are authorized by
        // this receipt; it observes main lifetime, not serialized referents.
        receiptHostProgressOrdinal=ordinal; receiptStartAttempted=true;
        receiptStartReturned=receipt->Start(originalThread,born);
        return receiptStartReturned;
    }
    bool PollSameProcessReceipt() noexcept {
        if(!receiptStartReturned || !ObserveOriginalThread()) return false;
        return receipt->Poll();
    }
    bool CloseCreatingThreadHandleEligible() noexcept {
        if(mainHandleDisposalAttempted) return threadCloseReturned!=FALSE;
        if(!receipt || !((receipt->PhysicallyDisposed() && receipt->Published()) || receipt->NoWorkerCreated()) ||
            !host || receiptHostProgressOrdinal>=host->progressCount ||
            !host->progress[receiptHostProgressOrdinal].physicalDisposed ||
            !threadAcquired || threadUnknown) return false;
        mainHandleDisposalAttempted=true; threadCloseAttempted=true;
        threadCloseReturned=CloseHandle(originalThread);
        if(!threadCloseReturned) threadCloseError=GetLastError();
        return threadCloseReturned!=FALSE;
    }
    bool CloseUnconnectedThreadHandleEligible() noexcept {
        // Before Connect, this source has not handed this original to the host
        // or receipt worker. Closing the known acquired query handle is
        // independent of retaining failed interpreter/configuration originals;
        // it does not finalize Python or allow main to return.
        if(mainHandleDisposalAttempted) return threadCloseReturned!=FALSE;
        if(connectAttempted || creatorStarted || receiptStartAttempted ||
            !threadAcquired || threadUnknown) return false;
        mainHandleDisposalAttempted=true; threadCloseAttempted=true;
        threadCloseReturned=CloseHandle(originalThread);
        if(!threadCloseReturned) threadCloseError=GetLastError();
        return threadCloseReturned!=FALSE;
    }
    const OriginalMainRetainedOutcome& PublishRetainedSourceOutcome() noexcept {
        if(retainedOutcome.published) return retainedOutcome;
        // This publishes references to SAME resident owners only. It neither
        // serializes exception graphs nor attests future interpreter/process
        // exit, and it does not grant destruction of any owner.
        if(!mainHandleDisposalAttempted || !receipt ||
            !((receipt->PhysicallyDisposed() && receipt->Published()) || receipt->NoWorkerCreated()) ||
            !host || receiptHostProgressOrdinal>=host->progressCount)
            return retainedOutcome;
        retainedOutcome.owner=this; retainedOutcome.invocation=invocationOriginal; retainedOutcome.host=host;
        retainedOutcome.interpreter=interpreter; retainedOutcome.receipt=receipt;
        retainedOutcome.hostProgressOrdinal=receiptHostProgressOrdinal;
        retainedOutcome.physicalHandlesDisposed=threadCloseReturned &&
            host->progress[receiptHostProgressOrdinal].physicalDisposed;
        retainedOutcome.sourceQualificationFailed=!retainedOutcome.physicalHandlesDisposed ||
            !receipt->Qualified() || host->progress[receiptHostProgressOrdinal].qualificationFailed ||
            (routeProducer && routeProducer->attempted && !routeProducer->returned) ||
            (nativeRouteBindingAttempted && (!nativeRouteArguments || !nativeRouteArguments->Returned())) ||
            (selectionInputOriginal && !selectionInputOriginal->QualifiedFinalInput()) ||
            !bootstrapOriginal || !bootstrapOriginal->disposalReturned ||
            observationExhausted || originalException || !initialized || !connected || !creatorStarted;
        retainedOutcome.published=true;
        return retainedOutcome;
    }
    // Same-process receipt and original-handle close do not attest future
    // process exit. Parent direction 48D01F4C... separates settled local
    // ownership from later independent process/family exit and raw EOF.
    // Actual pending native IO, children, controls or borrowers keep this owner
    // alive. A failed qualification alone is not evidence of pending custody.
    // No universal Python-heap serialization or implicit Py_Finalize follows;
    // selected external ACK relations and actual finalization work, if invoked,
    // must still have their original source-owned outcomes accounted for.
};
}

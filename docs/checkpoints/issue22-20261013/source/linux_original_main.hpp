// UNCOMPILED resident native main caller; no process-return/finalization grant.
#pragma once
#ifndef _WIN32
#include "linux_original_interpreter.hpp"
#include "linux_standalone_host.hpp"
#include "linux_original_route_producer.hpp"
#include "linux_main_receipt.hpp"
#include "linux_original_route_arguments.hpp"
namespace template_creator_bridge {
struct OriginalLinuxMainObservation {
    long thread=-1;int error=0,gil=0;
    PyThreadState* python=nullptr;PyInterpreterState* interpreter=nullptr;
    bool attempted=false,returned=false,same=false;
};
class OriginalLinuxNativeMain {
    ~OriginalLinuxNativeMain()=default;
    std::unique_ptr<OriginalLinuxMainObservation[]> observations_;
    std::size_t capacity_=0,count_=0;
public:
    OriginalLinuxInterpreter* interpreter=nullptr;
    RetainedNativeRegistry* registry=nullptr;
    OriginalLinuxStandaloneHost* host=nullptr;
    OriginalLinuxRouteProducer* routeProducer=nullptr;
    OriginalLinuxRouteArguments* routeArguments=nullptr;
    template_creator_linux::OriginalLinuxMainReceipt* receipt=nullptr;
    PyObject* selectedSteps=nullptr;
    PyObject* constructionResult=nullptr;
    PyObject* invocationResult=nullptr;
    std::exception_ptr nativeOriginal;
    bool beginAttempted=false,initialized=false,connectAttempted=false;
    bool connected=false,invokeAttempted=false,exhausted=false,failed=false;
    bool receiptStartAttempted=false,receiptStarted=false,receiptJoined=false;
    bool routeArgumentPreownAttempted=false;
    bool inputDispositionRequested=false,platformPointerConflict=false;
    bool finalPublicationReturned=false,finalOutputDisposed=false;
    bool lateMainCaptureAttempted=false,lateMainCaptureReturned=false,lateMainCaptureClean=false;
    PyObject *latePendingType=nullptr,*latePendingValue=nullptr,*latePendingTraceback=nullptr;
    explicit OriginalLinuxNativeMain(std::size_t selectedObservations){
        // Connect, route producer, Invoke and receipt Start each observe.
        // Cleanup uses its owning core admission, never these diagnostics.
        if(selectedObservations<4||selectedObservations>SIZE_MAX/sizeof(OriginalLinuxMainObservation))
            throw std::invalid_argument("original Linux main observations");
        observations_=std::make_unique<OriginalLinuxMainObservation[]>(selectedObservations);
        capacity_=selectedObservations;
    }
    OriginalLinuxNativeMain(const OriginalLinuxNativeMain&)=delete;
    OriginalLinuxNativeMain& operator=(const OriginalLinuxNativeMain&)=delete;
    bool BeginOriginal(const OriginalLinuxPythonPaths& paths,std::size_t units,std::size_t sessions) noexcept {
        if(beginAttempted)return false;beginAttempted=true;
        try {
            // Registry/host allocation precedes interpreter's first native query.
            registry=new RetainedNativeRegistry(sessions);
            host=new OriginalLinuxStandaloneHost(*registry);
            routeProducer=new OriginalLinuxRouteProducer();
            interpreter=new OriginalLinuxInterpreter(paths,units);
            initialized=interpreter->InitializeOriginal();failed=!initialized;
            return initialized;
        }catch(...){nativeOriginal=std::current_exception();failed=true;return false;}
    }
    bool ObserveOriginalThread() noexcept {
        if(!initialized||!interpreter)return false;
        if(count_==capacity_){exhausted=true;failed=true;return false;}
        auto& row=observations_[count_++];row.attempted=true;
        row.thread=syscall(SYS_gettid);if(row.thread<0)row.error=errno;row.returned=true;
        // Do not access Python state from a different native thread.
        if(row.thread<=0||row.thread!=interpreter->threads[0].result){failed=true;return false;}
        row.gil=PyGILState_Check();if(!row.gil){failed=true;return false;}
        row.python=PyThreadState_GetUnchecked();
        if(row.python)row.interpreter=PyThreadState_GetInterpreter(row.python);
        row.same=row.python==interpreter->originalPythonThread&&row.python&&
            row.interpreter==interpreter->originalPythonInterpreter;
        if(!row.same)failed=true;return row.same;
    }
    bool ConnectOriginal(const OriginalLinuxHostSelection& selection) noexcept {
        if(inputDispositionRequested||connectAttempted||!ObserveOriginalThread()||!host)return false;
        connectAttempted=true;
        try{
            receipt=new template_creator_linux::OriginalLinuxMainReceipt(selection.geometry.statBytes,
                selection.geometry.statusBytes,selection.geometry.procReads,selection.callerJoins,selection.callerStack);
            connected=host->Connect(selection);if(!connected)failed=true;return connected;
        }
        catch(...){if(!nativeOriginal)nativeOriginal=std::current_exception();failed=true;return false;}
    }
    bool PreownOriginalRouteArguments(const OriginalLinuxRouteSelection& source,std::size_t stringBytes) noexcept {
        if(inputDispositionRequested||routeArgumentPreownAttempted||connectAttempted)return false;
        routeArgumentPreownAttempted=true;
        try{routeArguments=new OriginalLinuxRouteArguments(source,stringBytes);return true;}
        catch(...){if(!nativeOriginal)nativeOriginal=std::current_exception();failed=true;return false;}
    }
    bool ProduceAndInvokeOriginalRouteArguments() noexcept {
        if(inputDispositionRequested||!connected||invokeAttempted||!routeArguments||!routeProducer||routeProducer->attempted||
           !host||!host->core||routeArguments->OriginalSelectedPhase()!=host->originalSelection.phase||
           !ObserveOriginalThread())return false;
        // Same original-thread observation covers both real source conversions
        // and producer invocation; Invoke retains its own later observation.
        if(!routeArguments->BuildOriginalArguments()||
           !routeProducer->Produce(routeArguments->OriginalSelectedPhase(),routeArguments->OriginalArguments())){
            failed=true;return false;
        }
        return InvokeSelectedCreator(routeProducer->steps);
    }
    bool InvokeSelectedCreator(PyObject* steps) noexcept {
        if(inputDispositionRequested||invokeAttempted||!connected||!ObserveOriginalThread()||!steps)return false;
        invokeAttempted=true;selectedSteps=Py_NewRef(steps);
        try {
            constructionResult=host->ConstructCreatorEntry(selectedSteps);
            if(!constructionResult||PyErr_Occurred()){failed=true;return false;}
            invocationResult=host->InvokeCreator();
            if(!invocationResult||PyErr_Occurred()){failed=true;return false;}
            return true;
        }catch(...){if(!nativeOriginal)nativeOriginal=std::current_exception();failed=true;return false;}
    }
    NativePlatformState* RecoverOriginalPlatform() noexcept {
        if(!host)return nullptr;
        auto* actual=host->core?host->core->PrivatePlatformOriginal():nullptr;
        if(actual&&host->platform&&actual!=host->platform){platformPointerConflict=true;failed=true;return nullptr;}
        if(actual&&!host->platform)host->platform=actual;
        return host->platform;
    }
    bool PrePlatformInputCleanupEligible() noexcept {
        inputDispositionRequested=true; // Close future source entry, even on refusal.
        if(RecoverOriginalPlatform()||platformPointerConflict||connected||invokeAttempted||
           (routeProducer&&routeProducer->attempted)||
           (host&&(host->connected||host->constructionAttempted||host->invokeAttempted)))return false;
        // Registry/core configuration/gettid and CPython initialization may
        // already exist. They stay resident. No creator import/method/command
        // has borrowed the startup selection/channel inputs on this branch:
        // route production is strictly after successful platform Connect.
        return true;
    }
    bool FailedPreownInputCleanupEligible(NativePlatformState* exactPlatform) noexcept {
        inputDispositionRequested=true;
        auto* actual=RecoverOriginalPlatform();
        return actual&&actual==exactPlatform&&!platformPointerConflict&&!connected&&!invokeAttempted&&
            (!routeProducer||!routeProducer->attempted)&&host&&!host->connected&&
            !host->constructionAttempted&&!host->invokeAttempted&&
            actual->FailedPreownSourceFrontierOriginal()!=NativePlatformState::FailedPreownHostFrontier::None;
    }
    bool ObservePhysicalHostClosure() noexcept {
        // FinishLinuxHostOriginals itself checks SAME original Python/native
        // creating thread before any cleanup. Diagnostic-row exhaustion here
        // must not suppress that independently eligible owning admission.
        RecoverOriginalPlatform();
        if(platformPointerConflict||!host||!host->core)return false;
        if(host->platform&&host->platform->FailedPreownSourceFrontierOriginal()!=NativePlatformState::FailedPreownHostFrontier::None){
            if(!FailedPreownInputCleanupEligible(host->platform))return false;
            failed=true;
            return host->ObserveHostClose(); // Actual core consumer; no never-acquired final IO.
        }
        if(invokeAttempted&&!receiptStartAttempted){
            receiptStartAttempted=true;
            if(!receipt||!host->platform||!ObserveOriginalThread())failed=true;
            else {
                auto& pending=host->platform->pending;
                receiptStarted=receipt->Start(pending.OriginalCreatorTaskOwner(),pending.OriginalDeadlineOwner(),
                    pending.OriginalSupervisorOwner(),pending.OriginalCreatorTid());
                if(!receiptStarted)failed=true;
            }
        }
        if(receipt&&receiptStartAttempted){
            if(receiptStarted)receiptJoined=receipt->PollEligibleJoin();
            if(!receipt->BorrowerSettled())return false;
            if(!receipt->Qualified())failed=true;
        }
        if(!host->core->PrepareLinuxFinalPublicationInputs())return false;
        if(!host->platform)return false;
        if(!lateMainCaptureAttempted){
            lateMainCaptureAttempted=true;
            // Preserve the actual final Python error original, including one
            // appended after an earlier private_failures snapshot. This is
            // reference custody, not pointer serialization of Python graphs.
            if(host->core->OnOriginalPythonThread()&&PyErr_Occurred()){
                PyErr_Fetch(&latePendingType,&latePendingValue,&latePendingTraceback);
                Py_XINCREF(latePendingType);Py_XINCREF(latePendingValue);Py_XINCREF(latePendingTraceback);
                PyErr_Restore(latePendingType,latePendingValue,latePendingTraceback);
            }
            auto& destination=host->platform->finalOutput;
            const auto append=[&](const void* bytes,size_t extent) noexcept {
                return destination.AppendFinalOriginals(static_cast<const unsigned char*>(bytes),extent);
            };
            // Counts precede variable rows; actual source ABI is private.
            struct Header{uint64_t version,observations,dispatches,threadChecks;};
            const Header header{1,count_,host->core->PrivateDispatchCount(),host->core->PrivateNativeThreadCheckCount()};
            bool captured=append(&header,sizeof(header));
            captured=append(this,sizeof(*this))&&captured;
            captured=append(observations_.get(),count_*sizeof(OriginalLinuxMainObservation))&&captured;
            captured=append(host,sizeof(*host))&&captured;
            captured=append(&host->core->PrivateHostClosure(),sizeof(host->core->PrivateHostClosure()))&&captured;
            captured=append(&host->core->PrivateCreatorInvocation(),sizeof(host->core->PrivateCreatorInvocation()))&&captured;
            captured=append(host->core->PrivateDispatches(),host->core->PrivateDispatchCount()*sizeof(OriginalDispatch))&&captured;
            captured=append(host->core->PrivateNativeThreadChecks(),host->core->PrivateNativeThreadCheckCount()*sizeof(OriginalNativeThreadCheck))&&captured;
            if(receipt)captured=receipt->CaptureSettledOriginals(destination.sink)&&captured;
            lateMainCaptureClean=captured;lateMainCaptureReturned=true;if(!captured)failed=true;
        }
        finalPublicationReturned=host->platform->PublishFinalPlatformOriginals();
        if(!finalPublicationReturned)return false;
        finalOutputDisposed=host->platform->finalOutput.ObserveEligibleFinalDisposition();
        if(!finalOutputDisposed)return false;
        if(host->platform->finalPublicationFailed||!host->platform->finalOutput.Qualified())failed=true;
        // Only after final file IO/collector/once closes have completed may
        // SAME supervisor/caller/ingress shut down. Their later originals stay
        // resident and are not claimed in the earlier final-file snapshot.
        return host->ObserveHostClose();
    }
    bool ProduceAndInvokeSelectedRoute(OriginalPhase phase,PyObject* originalArguments) noexcept {
        if(inputDispositionRequested||!connected||invokeAttempted||!host||phase!=host->originalSelection.phase||
            !routeProducer||routeProducer->attempted||!host->core||
            !ObserveOriginalThread())return false;
        if(!routeProducer->Produce(phase,originalArguments)){failed=true;return false;}
        return InvokeSelectedCreator(routeProducer->steps);
    }
    const OriginalLinuxMainObservation* PrivateObservations() const noexcept{return observations_.get();}
    std::size_t ObservationCount() const noexcept{return count_;}
    bool ReservedMainOwnedSourceBytes(std::size_t& bytes) const noexcept {
        bytes=sizeof(*this);
        if(capacity_>(SIZE_MAX-bytes)/sizeof(OriginalLinuxMainObservation))return false;
        bytes+=capacity_*sizeof(OriginalLinuxMainObservation);
        if(receipt){std::size_t actual=0;if(!receipt->ReservedSourceBytes(actual)||actual>SIZE_MAX-bytes)return false;bytes+=actual;}
        if(routeArguments){std::size_t actual=0;if(!routeArguments->ReservedSourceBytes(actual)||actual>SIZE_MAX-bytes)return false;bytes+=actual;}
        return true; // Separate registry/host/interpreter/Python/kernel/stack/allocator/overlap remain additional.
    }
    // Resident owners and every original return remain alive. Actual final
    // independent receipt, input/host qualification and executable entry policy
    // remain required; this caller offers no exit/finalize/dispose shortcut.
};
}
#endif

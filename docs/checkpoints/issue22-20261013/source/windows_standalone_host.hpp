// UNCOMPILED native-only host connection. Resident owners are allocated before
// channel effects. This is not a public argv/JSON capability importer and does
// not initialize/finalize Python or authenticate selected tool/source roles.
#pragma once
#include "windows_host_handoff.hpp"
#include "native_session_registry.hpp"
#include "native_windows_platform.hpp"
#include "windows_host_final_output.hpp"
#include "windows_original_interpreter.hpp"
#include "windows_notice_owner.hpp"
namespace template_creator_bridge {
struct OriginalWindowsHostSelection {
    OriginalPhase phase{};
    SIZE_T pendingCallRows=0;
    OriginalWindowsObserverGeometry observerGeometry{};
    OriginalWindowsNoticeGeometry noticeGeometry{};
    SIZE_T dispatchRows=0, hostClosureObservations=0, protectedCommands=0, sinkBytes=0, readbackBytes=0, sinkCalls=0;
    SIZE_T collectorBytes=0, collectorReads=0, collectorObservations=0;
    SIZE_T finalBytes=0, finalReadbackBytes=0, finalCalls=0;
    SIZE_T hostProgressObservations=0;
    SIZE_T finalCollectorBytes=0,finalCollectorReads=0,finalCollectorObservations=0;
    DWORD finalCollectorWaitMilliseconds=0;
    DWORD collectorWaitMilliseconds=0;
};
struct OriginalHostHandleClose {
    HANDLE original=nullptr;
    BOOL returned=FALSE;
    DWORD error=0;
    bool eligible=false, attempted=false, completionUnknown=false;
};
struct OriginalHostProgress {
    PyObject* pendingType=nullptr;
    PyObject* pendingValue=nullptr;
    PyObject* pendingTraceback=nullptr;
    std::exception_ptr nativeOriginal;
    SIZE_T reservedHostSourceBytes=0;bool hostSourceCountKnown=false;
    bool attempted=false, collectorPollAttempted=false, collectorPollReturned=false;
    bool sessionCloseAttempted=false, sessionCloseReturned=false;
    bool hostCloseAttempted=false, hostCloseReturned=false;
    bool publicationAttempted=false, publicationReturned=false;
    bool finalCloseAttempted=false, finalCloseReturned=false;
    bool finalCollectorStartAttempted=false,finalCollectorStartReturned=false;
    bool finalCollectorPollAttempted=false,finalCollectorPollReturned=false;
    bool physicalDisposed=false, qualificationFailed=true;
};
struct OriginalWindowsStandaloneHost {
    const OriginalWindowsInterpreter* interpreterOriginal=nullptr;
    DWORD interpreterBindingNativeThread=0;
    PyThreadState* interpreterBindingPythonThread=nullptr;
    PyInterpreterState* interpreterBindingPythonInterpreter=nullptr;
    bool interpreterBindingAttempted=false,interpreterBound=false;
    OriginalHostChannel channel;
    std::unique_ptr<OriginalWindowsPendingCalls> pendingCalls;
    OriginalWindowsNoticeOwner* noticeOwner=nullptr; // Host custody; no automatic/reset disposal.
    bool pendingPrechargeAttempted=false;
    std::exception_ptr pendingPrechargeOriginal;
    OriginalWindowsHostHandoff handoff;
    RetainedNativeRegistry* registry=nullptr;
    NativeSessionCore* core=nullptr;
    NativePlatformState* platform=nullptr;
    OriginalWindowsFinalOutput* finalOutput=nullptr;
    template_creator::OriginalWindowsSinkCollector* finalCollector=nullptr;
    PyObject* wrapper=nullptr;
    PyObject* bridgeModule=nullptr;
    PyObject* bridgeClass=nullptr;
    PyObject* bridgeOriginal=nullptr;
    PyObject* creatorModule=nullptr;
    PyObject* creatorClass=nullptr;
    PyObject* phaseClass=nullptr;
    PyObject* phaseOriginal=nullptr;
    PyObject* stepsOriginal=nullptr;
    PyObject* entryOriginal=nullptr;
    PyObject* constructionType=nullptr;
    PyObject* constructionValue=nullptr;
    PyObject* constructionTraceback=nullptr;
    bool entryConstructionAttempted=false;
    OriginalWindowsHostSelection selected{};
    std::exception_ptr prechargeOriginal, connectOriginal, creatorStartOriginal;
    bool prechargeAttempted=false, connectAttempted=false, transferred=false;
    bool creatorInvoked=false, hostCloseObserved=false;
    std::array<OriginalHostHandleClose,6> hostCloses{};
    bool hostHandlesCloseObserved=false, hostHandlesPhysicallyClosed=false;
    bool finalPublicationAttempted=false, finalPublicationVerified=false;
    std::unique_ptr<OriginalHostProgress[]> progress;
    SIZE_T progressCount=0;
    bool progressExhausted=false, creatorStartAttempted=false;
    PyObject* creatorConstructionReturnOriginal=nullptr;
    PyObject* creatorReturnOriginal=nullptr;
    bool finalCloseAttempted=false, finalCloseReturned=false;
    bool finalCollectorStartAttempted=false,finalCollectorStartReturned=false;
private:
    // Native host retains this object through unresolved native custody. No
    // stack/GC destruction can release channel, Python refs or registry owners.
    ~OriginalWindowsStandaloneHost()=default;
public:
    OriginalWindowsStandaloneHost()=default;
    OriginalWindowsStandaloneHost(const OriginalWindowsStandaloneHost&)=delete;
    OriginalWindowsStandaloneHost& operator=(const OriginalWindowsStandaloneHost&)=delete;
    bool PreownPendingCallsOriginal(SIZE_T selectedCallRows,OriginalWindowsObserverGeometry observer,OriginalWindowsNoticeGeometry notice) noexcept {
        if(pendingPrechargeAttempted || connectAttempted)return false;
        pendingPrechargeAttempted=true;
        try {
            if(!observer.Complete() || !notice.Complete())return false;
            pendingCalls=std::make_unique<OriginalWindowsPendingCalls>(selectedCallRows,observer);
            channel.pendingCallsOriginal_=pendingCalls.get();
            noticeOwner=new OriginalWindowsNoticeOwner(*pendingCalls,notice);
            return true;
        } catch(...) {pendingPrechargeOriginal=std::current_exception();return false;}
    }
    bool BindOriginalInterpreter(const OriginalWindowsInterpreter& actual) noexcept {
        if (interpreterBindingAttempted || prechargeAttempted) return false;
        interpreterBindingAttempted=true; interpreterOriginal=&actual;
        interpreterBindingNativeThread=GetCurrentThreadId();
        if (!actual.initialized || actual.failed || !actual.originalPythonThread ||
            interpreterBindingNativeThread!=actual.originalNativeThread) return false;
        interpreterBindingPythonThread=PyThreadState_Get();
        interpreterBindingPythonInterpreter=PyThreadState_GetInterpreter(interpreterBindingPythonThread);
        interpreterBound=interpreterBindingPythonThread==actual.originalPythonThread &&
            interpreterBindingPythonInterpreter==actual.originalPythonInterpreter;
        return interpreterBound; // Actual source lifetime association, not runtime source authority.
    }
    bool Precharge(const OriginalWindowsHostSelection& selection) noexcept {
        if (prechargeAttempted || !interpreterBound || !interpreterOriginal) return false;
        prechargeAttempted=true; selected=selection;
        try {
            if (!selection.hostProgressObservations) return false;
            progress=std::make_unique<OriginalHostProgress[]>(selection.hostProgressObservations);
            registry=new RetainedNativeRegistry(1);
            finalOutput=new OriginalWindowsFinalOutput(selection.finalBytes,selection.finalReadbackBytes,selection.finalCalls);
            finalCollector=new template_creator::OriginalWindowsSinkCollector(finalOutput->sink,
                selection.finalCollectorBytes,selection.finalCollectorReads,selection.finalCollectorObservations,
                selection.finalCollectorWaitMilliseconds);
            core=registry->PreownSession(selection.dispatchRows,selection.hostClosureObservations);
            return core!=nullptr;
        } catch (...) { prechargeOriginal=std::current_exception(); return false; }
    }
    bool Connect(const std::array<HANDLE,3>& originalCapabilities,HANDLE actualPrivateEvidenceRoot,
                 const void* originalBody,DWORD bodyBytes) noexcept {
        if (connectAttempted || !core || !registry || !finalOutput || !pendingCalls ||
            channel.pendingCallsOriginal_!=pendingCalls.get()) return false;
        connectAttempted=true;
        try {
            if (!channel.Construct() || !finalOutput->Create(actualPrivateEvidenceRoot,originalCapabilities[0]) ||
                !channel.PublishNativeOriginals(static_cast<std::uint32_t>(selected.phase),
                    originalCapabilities,originalBody,bodyBytes) || !handoff.Capture(channel) ||
                    !channel.PublishSelectedBodyAfterHandoff(handoff)) return false;
            const auto actual=handoff.OriginalCapabilities();
            platform=registry->PreownWindowsPlatform(*core,selected.phase,selected.protectedCommands,
                selected.sinkBytes,selected.readbackBytes,selected.sinkCalls,selected.collectorBytes,
                selected.collectorReads,selected.collectorObservations,selected.collectorWaitMilliseconds,actual,*pendingCalls);
            // Compare exact attachment, including failed capture: returning a
            // platform pointer alone does not mean transfer actually occurred.
            transferred=platform && platform->transferredOriginals==actual;
            if (transferred) {
                platform->sourceHostOriginal=this;
                platform->pendingCallsOriginal=channel.pendingCallsOriginal_;
            }
            if (!transferred || PyErr_Occurred()) return false;
            wrapper=WrapRetainedOriginal(*core); // Retain original wrapper, never borrowed temporary.
            return wrapper && platform->ingressBound && SourceChannelAssociationEstablished(*platform);
        } catch (...) {
            connectOriginal=std::current_exception();
            // A factory exception after registry publication/transfer must not
            // leave the host believing it still owns the same five originals.
            // CaptureTransferredOriginals is noexcept and attaches all five
            // before any query; exact published array is the transfer boundary.
            platform=core->PrivatePlatformOriginal();
            transferred=platform && handoff.captured && platform->transferredOriginals==handoff.OriginalCapabilities();
            if (transferred) {
                platform->sourceHostOriginal=this;
                platform->pendingCallsOriginal=channel.pendingCallsOriginal_;
            }
            return false;
        }
    }
    PyObject* InvokeCreator(PyObject* originalEntry) {
        if (creatorInvoked || !wrapper || !platform || !transferred || !entryOriginal || originalEntry!=entryOriginal ||
            !SourceChannelAssociationEstablished(*platform)) {
            if (!PyErr_Occurred()) PyErr_SetString(PyExc_RuntimeError,"connected original host required");
            return nullptr;
        }
        creatorInvoked=true;
        return core->RunRetainedCreatorEntry(originalEntry);
    }
    PyObject* ConstructCreatorEntry(PyObject* actualSelectedSteps);
    bool StartConnectedCreator(PyObject* actualSelectedSteps) noexcept;
    // One original, precharged host observation. This never starts a new
    // command/collector or finalizes Python; pending custody keeps this SAME
    // original main/interpreter thread resident for the next admitted tick.
    bool ObserveConnectedHost() noexcept;
    bool PublishPostCloseOriginals() noexcept;
    bool SourceChannelAssociationEstablished(const NativePlatformState& actualPlatform) const noexcept {
        // Derive this narrow transport/source relationship from the actual
        // retained constructor, writes, handoff and receiver captures. It is
        // deliberately separate from root/tool/ACL/loaded-memory authority.
        if (&actualPlatform!=platform || actualPlatform.sourceHostOriginal!=this || !transferred ||
            !channel.constructed_ || !channel.bodyWriteAttempted_ || !channel.bodyWriteReturned_ ||
            channel.bodyWritten_!=channel.selectedBodyBytes_ || channel.writeCompletionUnknown_ ||
            handoff.channelOriginal!=&channel || !handoff.captured || handoff.unknownRead ||
            actualPlatform.transferredOriginals!=handoff.OriginalCapabilities() || !actualPlatform.ingress.Captured() ||
            actualPlatform.pendingCallsOriginal!=channel.pendingCallsOriginal_ ||
            actualPlatform.ingress.OriginalPendingCalls()!=channel.pendingCallsOriginal_ ||
            channel.pendingCallsOriginal_!=pendingCalls.get()) return false;
        const auto& ingress=actualPlatform.ingress;
        const auto& born=ingress.OriginalCallerBirth();
        return ingress.OriginalCallerPid()==handoff.heldCallerPid &&
            born.dwLowDateTime==channel.born_.dwLowDateTime && born.dwHighDateTime==channel.born_.dwHighDateTime &&
            ingress.OriginalReadResult() && ingress.OriginalReadActual()==channel.selectedBodyBytes_ &&
            memcmp(ingress.OriginalBody().data(),channel.selectedBody_.data(),channel.selectedBodyBytes_)==0;
    }
    bool ReservedHostSourceBytes(SIZE_T& bytes) const noexcept {
        bytes=sizeof(*this); // Inline channel/handoff and all retained IO rows.
        const auto add=[&](SIZE_T amount) noexcept {if(amount>SIZE_MAX-bytes)return false;bytes+=amount;return true;};
        if(pendingCalls && !add(pendingCalls->ReservedSourceBytes()))return false;
        SIZE_T noticeBytes=0;
        if(noticeOwner && (!noticeOwner->ReservedSourceBytes(noticeBytes) || !add(noticeBytes)))return false;
        if(progress && (selected.hostProgressObservations>SIZE_MAX/sizeof(OriginalHostProgress) ||
            !add(selected.hostProgressObservations*sizeof(OriginalHostProgress))))return false;
        if(registry) {const auto count=registry->ReservedSourceBytes();if(count.overflow || !add(count.value))return false;}
        SIZE_T amount=0;
        if(platform && (!platform->ReservedPlatformSourceBytes(amount) || !add(amount)))return false;
        if(finalOutput) {
            if(!finalOutput->sink.ReservedSourceBytes(amount) || amount<sizeof(finalOutput->sink) ||
                !add(sizeof(*finalOutput)-sizeof(finalOutput->sink)) || !add(amount))return false;
        }
        if(finalCollector && (!finalCollector->ReservedSourceBytes(amount) || !add(amount)))return false;
        // Borrowed interpreter/main, Python graphs/config allocations, allocator
        // slack, native stacks/kernel/runtime and child overlap are not this sum.
        return true;
    }
    bool ObserveHostClose() noexcept {
        // Failed bridge/import/entry construction still owns transferred native
        // originals. No synthetic creator invocation is needed to dispose an
        // actually unlaunched platform through the same original-thread owner.
        if (!core || !platform) return false;
        hostCloseObserved=true;
        return core->FinishWindowsHostOriginals();
    }
    bool StartOriginalNoticeForRoute(const NativePlatformState& actual) const noexcept {
        return noticeOwner && &actual==platform && actual.sourceHostOriginal==this &&
            SourceChannelAssociationEstablished(actual) &&
            noticeOwner->AttachNoneOriginal(channel,actual.ingress,actual.route,this) && noticeOwner->StartOriginal();
    }
    bool StopOriginalNoticeForPlatform(const NativePlatformState& actual) const noexcept {
        return noticeOwner && &actual==platform && actual.sourceHostOriginal==this &&
            actual.pendingCallsOriginal==pendingCalls.get() && noticeOwner->StopAndJoinOriginal();
    }
    bool CloseHostOwnedEligible() noexcept {
        // Before transfer there are no product commands. After transfer the
        // retained session must first establish actual physical disposal.
        if (transferred && (!core || !core->PrivateHostClosure().physicalDisposed)) return false;
        if (!transferred && creatorInvoked) return false;
        if(noticeOwner && !noticeOwner->StopAndJoinOriginal())return false;
        if(channel.pendingCallsOriginal_) {
            channel.pendingCallsOriginal_->CloseFutureEffects();
            if(!channel.pendingCallsOriginal_->EndpointsIndependentlyEligible())return false;
        }
        hostHandlesCloseObserved=true;
        const std::array<HANDLE,6> actual{channel.server_,channel.client_,channel.caller_,
            handoff.duplicates[0].output,handoff.duplicates[1].output,handoff.duplicates[2].output};
        bool all=true;
        for (SIZE_T i=0;i<actual.size();++i) {
            auto& row=hostCloses[i];
            if (transferred && i!=1) continue; // Original platform owns these exact five.
            if (row.attempted) { if (!row.returned) all=false; continue; }
            row.original=actual[i];
            // A failed duplication with a nonnull output is retained UNKNOWN
            // even when the numeric value resembles INVALID_HANDLE_VALUE.
            // Do not normalize that output into an absent, disposed owner.
            if ((i==0 && channel.serverOutputUnknown_) || (i==1 && channel.clientOutputUnknown_) ||
                (i==2 && channel.callerOutputUnknown_) ||
                (i==2 && !channel.callerDuplicateReturned_ && row.original) ||
                (i>=3 && handoff.duplicates[i-3].outputUnknown)) {
                row.completionUnknown=true; all=false; continue;
            }
            if (!row.original || row.original==INVALID_HANDLE_VALUE) continue;
            bool known=true;
            if (i==0) known=!handoff.unknownRead && !channel.writeCompletionUnknown_;
            if (i==1) known=!channel.writeCompletionUnknown_;
            if (i==2) known=channel.callerDuplicateReturned_!=FALSE && !handoff.unknownRead && !channel.writeCompletionUnknown_;
            if (i>=3) known=handoff.duplicates[i-3].acquired && !handoff.duplicates[i-3].outputUnknown;
            row.completionUnknown=!known;
            if (!known) { all=false; continue; }
            row.eligible=true;
            row.attempted=true; // Before CloseHandle; uncertain failure is never retried.
            row.returned=CloseHandle(row.original);
            if (!row.returned) { row.error=GetLastError(); all=false; }
        }
        hostHandlesPhysicallyClosed=all;
        return all; // Independent physical fact; no qualification or host exit.
    }
    // Actual main/interpreter lifetime, original input layout binding, channel
    // post-close original publication
    // still must be joined. Neither Connect nor ObserveHostClose grants exit.
};
}

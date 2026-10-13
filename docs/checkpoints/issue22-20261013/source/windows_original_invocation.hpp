// UNCOMPILED closed native caller composition. A native source constructor
// supplies actual selections; this owner never imports numeric capabilities
// from argv/JSON or treats selected paths/hashes as execution authority.
#pragma once
#include "windows_original_capabilities.hpp"
#include "windows_original_route_arguments.hpp"
#include "windows_original_bootstrap.hpp"
#include "windows_private_selection_reader.hpp"
namespace template_creator_bridge {
struct OriginalNativeInputLayoutSelection {
    SIZE_T callerRows=0,readRows=0,ntdllOrdinal=SIZE_MAX;
    std::vector<template_creator::GateImageGeometry> images;
};
struct OriginalWindowsInvocation {
    OriginalWindowsSelectionReader* selectionOriginal=nullptr;
    bool selectionBindingAttempted=false,selectionBound=false;
    OriginalWindowsNativeMain* main=nullptr;
    OriginalWindowsSourceCapabilities* capabilities=nullptr;
    OriginalWindowsRouteArguments* routeArguments=nullptr;
    OriginalBootstrapCapture* bootstrap=nullptr;
    std::vector<OriginalNativeInputLayoutSelection> inputLayouts;
    bool nativeConfigurationAttempted=false,nativeConfigured=false;
    std::exception_ptr configurationOriginal;
    OriginalPythonPaths pythonPaths;
    OriginalWindowsHostSelection hostSelection{};
    std::unique_ptr<unsigned char[]> ingressBody;
    DWORD ingressBytes=0;
    SIZE_T pythonPathUnits=0;
    bool prechargeAttempted=false,precharged=false,beginAttempted=false,beginReturned=false;
    bool capabilityAttempted=false,capabilityReturned=false,connectionAttempted=false,connectionReturned=false;
    bool invocationAttempted=false,invocationReturned=false;
    std::exception_ptr prechargeOriginal;
private:
    ~OriginalWindowsInvocation()=default; // No return/GC release of live owners.
public:
    OriginalWindowsInvocation()=default;
    OriginalWindowsInvocation(const OriginalWindowsInvocation&)=delete;
    OriginalWindowsInvocation& operator=(const OriginalWindowsInvocation&)=delete;
    bool Precharge(const OriginalPythonPaths& paths,SIZE_T selectedPythonPathUnits,
        const OriginalWindowsHostSelection& host,const OriginalWindowsCapabilityPaths& capabilityPaths,
        const OriginalNativeRouteSelection& route,SIZE_T selectedRouteStringUnits,
        const OriginalNativeRouteSelection& bootstrapSelection,SIZE_T bootstrapStringUnits,
        const std::vector<OriginalNativeInputLayoutSelection>& layouts,
        const void* body,DWORD bodyBytes,SIZE_T selectedMainObservations,
        SIZE_T selectedReceiptPolls,SIZE_T selectedReceiptStackBytes) noexcept {
        if(prechargeAttempted)return false;prechargeAttempted=true;
        if(selectionBindingAttempted && !selectionBound)return false;
        try {
            if(!host.pendingCallRows || !host.observerGeometry.Complete() || !host.noticeGeometry.Complete() || host.phase!=route.phase ||
                host.protectedCommands!=(route.phase==OriginalPhase::Generation?1:2) ||
                !body || !bodyBytes || bodyBytes>65536)
                throw std::invalid_argument("same selected phase and private original ingress body");
            // Complete source path preflight before copying potentially large
            // selection vectors. This is capacity validation, not path authority.
            SIZE_T units=0;
            const auto charge=[&](const std::wstring& text) {
                if(text.empty() || text.find(L'\0')!=std::wstring::npos || text.size()>=SIZE_MAX ||
                    text.size()+1>SIZE_MAX-units)throw std::invalid_argument("native interpreter selection text");
                units+=text.size()+1;
            };
            charge(paths.home);charge(paths.program);charge(paths.executable);charge(paths.baseExecutable);
            charge(paths.prefix);charge(paths.execPrefix);charge(paths.basePrefix);charge(paths.baseExecPrefix);
            for(const auto& path:paths.modulePaths)charge(path);
            if(paths.modulePaths.empty() || !selectedPythonPathUnits || units>selectedPythonPathUnits ||
                units>SIZE_MAX/sizeof(wchar_t))throw std::invalid_argument("native interpreter path precharge");
            if(host.protectedCommands>64 || layouts.size()!=host.protectedCommands+1)
                throw std::invalid_argument("complete bootstrap and command native layouts");
            for(const auto& layout:layouts)if(!layout.callerRows || !layout.readRows || layout.readRows>65536 ||
                layout.images.empty() || layout.images.size()>SIZE_MAX/sizeof(template_creator::GateImageGeometry))
                throw std::invalid_argument("precharged native input observation layout");
            pythonPaths=paths;pythonPathUnits=selectedPythonPathUnits;hostSelection=host;inputLayouts=layouts;
            ingressBody=std::make_unique<unsigned char[]>(bodyBytes);
            memcpy(ingressBody.get(),body,bodyBytes);ingressBytes=bodyBytes;
            // Attach each allocation immediately into this resident owner. A
            // subsequent construction failure retains every earlier owner.
            main=new OriginalWindowsNativeMain(selectedMainObservations,selectedReceiptPolls,selectedReceiptStackBytes);
            main->selectionInputOriginal=selectionOriginal;
            main->invocationOriginal=this; // Resident outer originals remain reachable from final receipt.
            capabilities=new OriginalWindowsSourceCapabilities(capabilityPaths,host.sinkBytes,host.readbackBytes,host.sinkCalls);
            routeArguments=new OriginalWindowsRouteArguments(route,selectedRouteStringUnits);
            bootstrap=new OriginalBootstrapCapture(bootstrapSelection,bootstrapStringUnits);
            main->bootstrapOriginal=bootstrap;
            precharged=true;return true;
        } catch(...) {prechargeOriginal=std::current_exception();return false;}
    }
    bool BindCapturedSelectionOriginal(OriginalWindowsSelectionReader& original) noexcept {
        if(selectionBindingAttempted || prechargeAttempted || beginAttempted)return false;
        selectionBindingAttempted=true;selectionOriginal=&original;
        selectionBound=original.Captured();return selectionBound;
        // Parser/selection associations must separately bind these exact bytes
        // to the copied original fields; capture alone never establishes that.
    }
    bool BeginAndInvokeOriginal() noexcept {
        if(beginAttempted || !precharged || !main || !capabilities || !routeArguments ||
            capabilities->DisposalAttempted() || (selectionOriginal && !selectionOriginal->Captured()) ||
            (selectionBindingAttempted && !selectionBound))return false;
        beginAttempted=true;
        beginReturned=main->BeginOriginal(pythonPaths,pythonPathUnits,hostSelection);
        if(!beginReturned)return false;
        capabilityAttempted=true;capabilityReturned=capabilities->AcquireOriginals();
        if(!capabilityReturned)return false;
        if(!routeArguments->BindOriginalFixtureRoot(capabilities->PrivateFixtureRoot()))return false;
        connectionAttempted=true;
        connectionReturned=capabilities->ConnectOriginalMain(*main,ingressBody.get(),ingressBytes);
        if(!connectionReturned)return false;
        nativeConfigurationAttempted=true;
        try {
            if(!main->host || !main->host->platform)return false;
            auto& platform=*main->host->platform;
            if(!platform.BindSourceOriginals(*capabilities,*main))return false;
            for(SIZE_T i=0;i<inputLayouts.size();++i) {
                const auto& layout=inputLayouts[i];
                if(!platform.PreownInputLayout(i,layout.callerRows,layout.readRows,layout.images.data(),layout.images.size(),layout.ntdllOrdinal))return false;
            }
            if(!bootstrap->CaptureAndBind(platform))return false;
            nativeConfigured=true;
        } catch(...) {configurationOriginal=std::current_exception();return false;}
        invocationAttempted=true;invocationReturned=main->BuildAndInvokeNativeSelectedRoute(*routeArguments);
        return invocationReturned; // Creator return only; not host/collector closure.
    }
    bool ObserveIndependentlyEligibleClosure() noexcept {
        if(!main) {
            // Failed precharge has begun no interpreter/channel/child effect.
            return !beginAttempted && (!selectionOriginal || selectionOriginal->CloseAfterConsumers());
        }
        if(main->connectAttempted) {
            // Failed transfer/construction can still require native host cleanup.
            // No command, collector or receipt is synthesized for that failure.
            main->ObserveRetainedCustody();
            if(!main->host || !main->host->progressCount ||
                !main->host->progress[main->host->progressCount-1].physicalDisposed)return false;
            bool bootstrapDisposed=bootstrap && !bootstrap->held;
            if(bootstrap && bootstrap->held && main->host->platform)
                bootstrapDisposed=bootstrap->DisposeAfterPlatformClosure(*main->host->platform);
            bool inputs=capabilities && capabilities->DisposeIndependentlyEligible();
            bool selectionDisposed=!selectionOriginal || selectionOriginal->CloseAfterConsumers();
            inputs=inputs && bootstrapDisposed && selectionDisposed;
            if(!main->receiptStartAttempted)main->StartSameProcessReceipt();
            if(main->receiptStartReturned)main->PollSameProcessReceipt();
            main->CloseCreatingThreadHandleEligible();
            const auto& outcome=main->PublishRetainedSourceOutcome();
            return inputs && outcome.published && outcome.physicalHandlesDisposed;
        }
        // Before any native handoff, acquired source handles have no consumer.
        // Preserve failed initialization/acquisition originals independently.
        bool inputs=!capabilities || capabilities->DisposeIndependentlyEligible();
        main->CloseUnconnectedThreadHandleEligible();
        bool selectionDisposed=!selectionOriginal || selectionOriginal->CloseAfterConsumers();
        return inputs && selectionDisposed && !main->threadUnknown && (!main->threadAcquired || main->threadCloseReturned);
    }
    // This retained cut describes only the source-owned composition below.
    // The standalone caller must settle its actual own runtime/control/IO/input
    // borrowers before natural return; future external exit follows afterward.
    struct LocalSettlementOriginal {
        bool published=false,physicalClosureReturned=false,qualificationFailed=true;
        SIZE_T hostProgressOrdinal=SIZE_MAX;
        const OriginalWindowsInvocation* owner=nullptr;
        PyObject* pendingType=nullptr;
        PyObject* pendingValue=nullptr;
        PyObject* pendingTraceback=nullptr;
    } localSettlement;
    const LocalSettlementOriginal& PublishLocalSettlementOriginal() noexcept {
        if(localSettlement.published)return localSettlement;
        // Reuse actual completed ownership outcomes. Do not run new cleanup,
        // create a receipt or consume a new native observation in publication.
        bool physical=false;
        if(!main) {
            // Precharge did not reach any interpreter/native child acquisition.
            physical=!beginAttempted && (!selectionOriginal || selectionOriginal->ClosedKnownOriginal());
        } else if(!main->connectAttempted) {
            physical=(!capabilities || capabilities->DisposalReturned()) &&
                !main->threadUnknown && (!main->threadAcquired || main->threadCloseReturned) &&
                (!selectionOriginal || selectionOriginal->ClosedKnownOriginal());
            // A partial interpreter initialization has no declared safe-return
            // disposition here. Keep its actual owner resident for resolution.
            if(main->interpreter && main->interpreter->initializeAttempted)physical=false;
        } else {
            physical=main->retainedOutcome.published && main->retainedOutcome.physicalHandlesDisposed &&
                capabilities && capabilities->DisposalReturned() && bootstrap && (!bootstrap->held || bootstrap->physicallyDisposed) &&
                (!selectionOriginal || selectionOriginal->ClosedKnownOriginal());
        }
        if(!physical)return localSettlement;
        // Preserve the final pending Python original independently of earlier
        // private_failures snapshots and native dispatch triples. No decref,
        // serialization, interpreter finalization or new borrower is performed.
        if(main && main->interpreter && main->interpreter->initialized) {
            if(!main->host || !main->host->core || !main->host->core->OnOriginalPythonThread())
                return localSettlement;
            if(PyErr_Occurred()) {
                PyErr_Fetch(&localSettlement.pendingType,&localSettlement.pendingValue,&localSettlement.pendingTraceback);
                Py_XINCREF(localSettlement.pendingType);Py_XINCREF(localSettlement.pendingValue);
                Py_XINCREF(localSettlement.pendingTraceback);
                PyErr_Restore(localSettlement.pendingType,localSettlement.pendingValue,localSettlement.pendingTraceback);
            }
        }
        localSettlement.owner=this;
        localSettlement.physicalClosureReturned=true;
        localSettlement.qualificationFailed=!SourceQualified() || localSettlement.pendingType;
        if(main)localSettlement.hostProgressOrdinal=main->retainedOutcome.hostProgressOrdinal;
        localSettlement.published=true;
        return localSettlement;
    }
    bool SourceQualified() const noexcept {
        return (!selectionOriginal || selectionOriginal->QualifiedFinalInput()) &&
            precharged && beginReturned && capabilityReturned && connectionReturned && nativeConfigured && invocationReturned &&
            bootstrap && bootstrap->disposalReturned &&
            main && main->retainedOutcome.published && !main->retainedOutcome.sourceQualificationFailed &&
            capabilities && capabilities->DisposalReturned() &&
            capabilities->PrivateFixtureRoot().RemovalSucceeded();
    }
    // The actual standalone wmain, authentic installed/native inputs and complete
    // launch associations remain required. Independent exact process/family
    // exit is later qualification, not this process observing its future exit.
    // Pending/uncertain original custody must retain its owner. SourceQualified
    // is a qualification predicate, not an exit or finalization permission.
    // No implicit Py_Finalize or interpreter-heap serialization is selected.
};
}

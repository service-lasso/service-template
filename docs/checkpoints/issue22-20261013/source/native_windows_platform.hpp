// UNCOMPILED connected Windows platform owner layout. Native host acquisition,
// ingress/input/capture operation bodies are still being implemented in this
// source unit; no dynamically supplied callbacks fill those obligations.
#pragma once
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include "native_session_core.hpp"
#include "windows_session_commands.hpp"
#include "windows_native_sink.hpp"
#include "windows_sink_collector.hpp"
#include "windows_pending_call_owner.hpp"
namespace template_creator_bridge {
struct OriginalWindowsStandaloneHost;
struct OriginalWindowsNativeMain;
class OriginalWindowsSourceCapabilities;
struct OriginalTransferredObject {
    HANDLE original = nullptr;
    FILE_ID_INFO id{};
    FILE_STANDARD_INFO standard{};
    BOOL idReturned = FALSE, standardReturned = FALSE;
    DWORD idError = 0, standardError = 0;
    bool attempted = false, captured = false;
    bool Capture(HANDLE actual) noexcept {
        if(attempted)return false;attempted=true;original=actual;
        idReturned=GetFileInformationByHandleEx(actual,FileIdInfo,&id,sizeof(id));
        if(!idReturned)idError=GetLastError();
        standardReturned=GetFileInformationByHandleEx(actual,FileStandardInfo,&standard,sizeof(standard));
        if(!standardReturned)standardError=GetLastError();
        captured=idReturned && standardReturned;return captured;
    }
};
struct OriginalWindowsInputOwner {
    std::unique_ptr<template_creator::NativeHeldFiles> files;
    std::unique_ptr<template_creator::GateImageGeometry[]> geometry;
    SIZE_T imageRows = 0, callerRows = 0, readRows = 0, ntdllOrdinal = SIZE_MAX;
    bool layoutAttempted = false, layoutBound = false, captureAttempted = false;
    bool classViewAttempted = false;
    PyObject* classViewOriginal = nullptr;
    bool disposalAttempted = false, disposalResult = false;
    PyObject* snapshot = nullptr;
    std::exception_ptr originalException;
    std::array<wchar_t,32768> directoryPath{};
    HANDLE directoryReturned = INVALID_HANDLE_VALUE;
    template_creator::FileIdentityObservation directoryIdentity;
    DWORD directoryError = 0, directoryCloseError = 0;
    BOOL directoryCloseReturned = FALSE;
    bool directoryAttempted = false, directoryAcquired = false, directoryUnknown = false;
    bool directoryAssociated = false, directoryCloseAttempted = false;
};
struct NativePlatformState {
    OriginalWindowsPendingCalls* pendingCallsOriginal = nullptr; // SAME host-owned channel borrower owner.
    const OriginalWindowsStandaloneHost* sourceHostOriginal = nullptr;
    const OriginalWindowsSourceCapabilities* sourceCapabilitiesOriginal = nullptr;
    const OriginalWindowsNativeMain* sourceMainOriginal = nullptr;
    std::array<OriginalTransferredObject,3> sourceTransferredIdentities{};
    OriginalTransferredObject sourceSinkIdentity;
    bool sourceBindingAttempted = false, sourceObjectsAssociated = false;
    template_creator::OriginalPrivateIngress ingress;
    template_creator::OriginalWindowsCommandRoute route;
    template_creator::OriginalWindowsSessionCommands commands;
    template_creator::OriginalWindowsSink sink;
    PyObject* originalIngressIdentity = nullptr;
    std::array<HANDLE,5> transferredOriginals{};
    bool ingressBound = false;
    const OriginalPhase originalPhase;
    PyObject* phaseOriginal = nullptr;
    PyObject* rolesOriginal = nullptr;
    PyObject* stepsOriginal = nullptr;
    bool adoptAttempted = false, prechargeAttempted = false, adopted = false, stepsBound = false;
    bool qualificationFailed = false;
    template_creator::OriginalWindowsSinkCollector* collector = nullptr;
    bool collectorPrechargeAttempted = false, collectorStartAttempted = false;
    std::exception_ptr collectorPrechargeOriginal;
    PyObject* creatorOriginals = nullptr;
    PyObject* creatorEntryOriginal = nullptr;
    OriginalCreatorHeldInputs* creatorHeldInputs = nullptr;
    PyObject* creatorFailures = nullptr;
    bool creatorCaptureAttempted = false, creatorRecordAppended = false;
    bool creatorPrefixReadback = false, creatorCollectorStarted = false;
    bool hostDisposalAttempted = false, hostDisposalResult = false;
    std::unique_ptr<OriginalWindowsInputOwner[]> inputOwners;
    PyObject* bootstrapSnapshotOriginal = nullptr;
    PyObject* inputObjectsOriginal = nullptr;
    PyObject* snapshotsOriginal = nullptr;
    bool bootstrapBindingAttempted = false, inputBindingAttempted = false, inputCapturesClean = false;
private:
    friend class RetainedNativeRegistry;
    ~NativePlatformState() = default; // Resident registry, never wrapper/stack disposal.
    NativePlatformState(OriginalPhase phase,SIZE_T selectedProtectedCommands,SIZE_T sinkBytes,SIZE_T readbackBytes,SIZE_T sinkCalls)
        : route(selectedProtectedCommands+1),commands(route,selectedProtectedCommands),sink(sinkBytes,readbackBytes,sinkCalls),originalPhase(phase),
          inputOwners(std::make_unique<OriginalWindowsInputOwner[]>(selectedProtectedCommands+1)) {}
    // Called only by the final creator-capture body after its last append;
    // never exposed as a Python operation or a caller-supplied completion bit.
    bool StartFinalCollector() noexcept;
public:
    NativePlatformState(const NativePlatformState&) = delete;
    NativePlatformState& operator=(const NativePlatformState&) = delete;
    // These source-owned bodies must join the original ingress, complete native
    // input owners, sink roster and route. None is a virtual caller policy hook.
    PyObject* AdoptIngress(PyObject* original,PyObject* phase,PyObject* roles);
    PyObject* PrechargeRoute(PyObject* steps);
    PyObject* CaptureInputClasses(PyObject*,PyObject*);
    PyObject* BindInputs(PyObject* inputs,PyObject* snapshots);
    PyObject* PrepareSink();
    PyObject* CaptureCommand(PyObject* owner);
    PyObject* VerifyCommand(PyObject* owner,PyObject* exactStep);
    PyObject* DisposeEligible(PyObject* owner);
    PyObject* InputDisposalEligible(PyObject* ordinal,PyObject* held);
    PyObject* CaptureCreator(PyObject* originals,PyObject* failures);
    PyObject* VerifyClosure();
    bool AllPhysicalOriginalsDisposed() const noexcept;
    bool LaunchAssociationsEstablished() const noexcept;
    bool BindSourceOriginals(const OriginalWindowsSourceCapabilities& actual,
                            const OriginalWindowsNativeMain& main) noexcept;
    bool BindOriginalBootstrap(PyObject* snapshot);
    bool PreownInputLayout(SIZE_T ordinal,SIZE_T callerRows,SIZE_T readRows,const template_creator::GateImageGeometry* geometry,
                          SIZE_T imageRows,SIZE_T originalNtdllOrdinal);
    bool PreownCollector(SIZE_T selectedBytes,SIZE_T selectedReads,SIZE_T selectedObservations,DWORD selectedWaitMilliseconds) noexcept;
    bool PollFinalCollector() noexcept;
    bool CollectorPhysicalDisposed() const noexcept;
    bool CollectorQualified() const noexcept;
    bool DisposeHostEligibleOriginals() noexcept;
    bool ReservedPlatformSourceBytes(SIZE_T& bytes) const noexcept;
};
}

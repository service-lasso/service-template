// UNCOMPILED resident Linux platform composition. Native role/source and exact
// syscall consumption joins remain separate from captured ingress structure.
#pragma once
#ifndef _WIN32
#include "native_session_core.hpp"
#include "linux_pending_input_owner.hpp"
#include "linux_pending_host.hpp"
#include "linux_session_commands.hpp"
#include "linux_held_file.hpp"
#include "linux_sink_collector.hpp"
#include "linux_original_eof_stdin.hpp"
#include "linux_initial_objects.hpp"
#include "linux_host_final_output.hpp"
#include "linux_initial_provider_archive.hpp"
#include "linux_receipt_foundation_pair.hpp"
namespace template_creator_linux {bool VerifySurvivingPendingHostPreownRegressionOriginal();}
namespace template_creator_bridge {
struct LinuxReceivedSubjectAssociationOriginal {
    enum class Result : unsigned char {Unobserved,Missing,Ambiguous,Invalid,Different,Matched,MatchedNativeUnitIdentity,MatchedNativeArchiveCorrespondence};
    const template_creator_linux::OriginalInitialProviderReceiver* receiver=nullptr;
    const template_creator_linux::OriginalInitialProviderArchive* archive=nullptr;
    const template_creator_linux::OriginalInitialProviderArchive::CutOriginal* cut=nullptr;
    const template_creator_linux::OriginalLinuxHeldFile* heldFile=nullptr;
    const template_creator_linux::OriginalReceivedSourceUnitObservation* sourceUnit=nullptr;
    const template_creator_linux::OriginalReceiptFoundationPair* receiptFoundationPairOriginal=nullptr;
    PyObject* nativeRequirementDictionaryOriginal=nullptr;
    PyObject* requirementOriginal=nullptr;
    size_t command=SIZE_MAX,requirement=SIZE_MAX,bytes=0;
    template_creator_linux::OriginalClockBoundary finalClock{};
    bool attempted=false;
    Result result=Result::Unobserved;
};
struct LinuxPrelaunchAssociationOriginal {
    const template_creator_linux::OriginalRouteCommand* commandOriginal=nullptr;
    const template_creator_linux::OriginalLinuxHeldFile* toolOriginal=nullptr;
    const template_creator_linux::OriginalLinuxCommandDirectory* cwdOriginal=nullptr;
    const template_creator_linux::OriginalLinuxPendingHost* callerHostOriginal=nullptr;
    const template_creator_linux::InitialObjectStat* rootOriginal=nullptr;
    const template_creator_linux::InitialObjectStat* evidenceOriginal=nullptr;
    PyObject* snapshotOriginal=nullptr;
    size_t toolOrdinal=SIZE_MAX;
    bool attempted=false,associated=false;
};
enum class LinuxInterpreterCorrespondence : unsigned char { Unobserved, Invalid, Nonmatch, Match, SourceRangeNotApplicable };
struct LinuxInterpreterRequirementOriginal {
    PyObject* requirementOriginal=nullptr;
    const template_creator_linux::OriginalLinuxHeldFile* launchOriginal=nullptr;
    const template_creator_linux::OriginalLinuxHeldFile* candidateOriginal=nullptr;
    bool attempted=false,returned=false;
    LinuxInterpreterCorrespondence result=LinuxInterpreterCorrespondence::Unobserved;
};
struct LinuxInputOwner {
    const template_creator_linux::OriginalInitialProviderReceiver* initialProviderOriginal=nullptr;
    const template_creator_linux::OriginalReceiptFoundationPair* receiptFoundationPairOriginal=nullptr;
    LinuxPrelaunchAssociationOriginal prelaunchOriginal;
    PyObject* snapshot=nullptr;
    PyObject* commandInputsOriginal=nullptr;
    PyObject* commandInputsDictionary=nullptr;
    PyObject* capturedFieldOriginal=nullptr;
    PyObject* heldFieldOriginal=nullptr;
    int capturedLookupResult=0,heldLookupResult=0;
    bool capturedLookupAttempted=false,capturedLookupReturned=false;
    bool heldLookupAttempted=false,heldLookupReturned=false,sourceAssociated=false;
    std::unique_ptr<std::size_t[]> selectedReadRows;
    std::unique_ptr<LinuxInterpreterRequirementOriginal[]> interpreterObservations;
    std::unique_ptr<LinuxReceivedSubjectAssociationOriginal[]> receivedSubjectAssociations;
    std::unique_ptr<std::unique_ptr<template_creator_linux::OriginalLinuxHeldFile>[]> files;
    std::unique_ptr<template_creator_linux::OriginalLinuxCommandDirectory> cwd;
    std::unique_ptr<template_creator_linux::OriginalLinuxHeldFile> kernelExeCandidate;
    std::size_t kernelExeSourceOrdinal=SIZE_MAX;
    std::unique_ptr<template_creator_linux::Gate> gate;
    std::size_t launchFileOrdinal=SIZE_MAX;
    bool launchFileAssociated=false;
    std::size_t count=0;
    bool layoutAttempted=false,layoutBound=false,captureAttempted=false,captureClean=false;
    bool classViewAttempted=false;
    PyObject* classViewOriginal=nullptr;
    std::exception_ptr originalException;
};
struct NativePlatformState {
    template_creator_linux::OriginalLinuxPendingHost pendingHostStorage;
    template_creator_linux::OriginalLinuxPendingHost& pendingHost;
    template_creator_linux::OriginalLinuxPendingInputOwner& pending;
    template_creator_linux::OriginalCommandRoute route;
    template_creator_linux::OriginalProtectedEofStdin protectedStdin;
    template_creator_linux::OriginalLinuxInitialObjects initialObjects;
    template_creator_linux::OriginalLinuxSessionCommands commands;
    template_creator_linux::OriginalNativeSink sink;
    template_creator_linux::OriginalLinuxSinkCollector collector;
    OriginalLinuxFinalOutput finalOutput;
    const OriginalPhase originalPhase;
    const template_creator_linux::PendingInputGeometry gateGeometry;
    int originalProcRoot=-1;
    std::size_t callerPidfdOrdinal=SIZE_MAX,sinkOrdinal=SIZE_MAX;
    bool transferAttempted=false,captureReturned=false,captureResult=false;
    bool adoptAttempted=false,adopted=false,prechargeAttempted=false,stepsBound=false;
    bool sinkPrepareAttempted=false,sinkPrepared=false,qualificationFailed=false;
    bool hostDisposalAttempted=false,hostDisposalResult=false;
    bool hostObserverClosure=false,ingressDisposalAttempted=false,ingressDisposalResult=false;
    PyObject* originalIngressIdentity=nullptr;
    PyObject* originalPhaseObject=nullptr;
    PyObject* originalRoles=nullptr;
    PyObject* originalSteps=nullptr;
    PyObject* originalInputs=nullptr;
    PyObject* originalSnapshots=nullptr;
    PyObject* creatorOriginals=nullptr;
    PyObject* creatorFailures=nullptr;
    PyObject* creatorEntryOriginal=nullptr;
    OriginalCreatorHeldInputs* creatorHeldInputs=nullptr;
    bool inputDisposalStarted=false;
    bool nativeInputDisposalAttempted=false,nativeInputDisposalResult=false;
    bool settledHostOutcomeObserved=false,settledHostNormalOutcome=false;
    bool creatorCaptureAttempted=false,creatorRecordAppended=false,creatorPrefixReadback=false;
    bool collectorStartAttempted=false,collectorStartReturned=false;
    std::unique_ptr<LinuxInputOwner[]> inputOwners;
    bool inputBindingAttempted=false,inputCapturesClean=false;
    std::exception_ptr originalCaptureException;
    template_creator_linux::OriginalLinuxHeldFile* selectionOriginal=nullptr;
    bool externalPendingHost=false,externalPendingHostOwned=false;
    bool storagePreownAttempted=false,storagePreownReturned=false,storagePreowned=false;
    const OriginalLinuxSelectionStartup* failedPreownStartupOriginal=nullptr;
    bool failedPreownDispositionAttempted=false,failedPreownPhysicalDisposed=false;
    std::size_t selectedCommandCapacity=0,allocatedInputOwnerCapacity=0;
    std::exception_ptr storagePreownException;
    bool finalPublicationAttempted=false,finalPublicationReturned=false,finalPublicationFailed=false;
    bool finalPublicationInputCut=false;
    std::exception_ptr finalPublicationOriginal;
private:
    friend class RetainedNativeRegistry;
    friend bool template_creator_linux::VerifySurvivingPendingHostPreownRegressionOriginal();
    ~NativePlatformState()=default; // Resident registry; never wrapper disposal.
    NativePlatformState(OriginalPhase phase,std::size_t selectedCommands,std::size_t sinkBytes,
        std::size_t sinkCalls,const template_creator_linux::PendingInputGeometry& geometry,
        bool hasOriginalDeadline,std::uint64_t originalAbsoluteNanoseconds,
        std::size_t callerStack,std::size_t callerJoins,
        template_creator_linux::OriginalLinuxPendingHost* adoptedHost=nullptr)
        :pendingHostStorage(template_creator_linux::DeferredOriginalPreown,geometry,hasOriginalDeadline,originalAbsoluteNanoseconds,callerStack,callerJoins),
         pendingHost(adoptedHost?*adoptedHost:pendingHostStorage),
         pending(pendingHost.OriginalPendingOwner()),route(template_creator_linux::DeferredOriginalPreown,selectedCommands),
         protectedStdin(route,pending.OriginalDeadlineOwner(),pending.OriginalSupervisorOwner()),
         initialObjects(pending.OriginalIngressOwner(),pending.OriginalDeadlineOwner(),pending.OriginalSupervisorOwner()),commands(template_creator_linux::DeferredOriginalPreown,route),
         sink(template_creator_linux::DeferredOriginalPreown,sinkBytes,sinkCalls,pending.OriginalDeadlineOwner(),pending.OriginalSupervisorOwner()),
         collector(template_creator_linux::DeferredOriginalPreown,sink,sinkBytes,sinkCalls,callerJoins,callerStack),
         finalOutput(template_creator_linux::DeferredOriginalPreown,sinkBytes,sinkCalls,callerJoins,callerStack,pending.OriginalDeadlineOwner(),pending.OriginalSupervisorOwner()),
         originalPhase(phase),gateGeometry(geometry) {
        selectedCommandCapacity=selectedCommands;
        externalPendingHost=adoptedHost!=nullptr;
    }
    bool PreownStorageOriginal() noexcept {
        if(storagePreownAttempted)return false;storagePreownAttempted=true;
        struct PublishReturn{bool& original;~PublishReturn(){original=true;}} publishReturn{storagePreownReturned};
        try{
            if(selectedCommandCapacity>SIZE_MAX/sizeof(LinuxInputOwner))throw std::invalid_argument("Linux input owner geometry");
            // Adopted host is the sole active pending graph. Inline fallback is
            // an allocation-free dormant operand holder on that route.
            if(!externalPendingHost&&!pendingHostStorage.PreownOriginal()){
                storagePreownException=pendingHostStorage.PreownExceptionOriginal();qualificationFailed=true;return false;}
            if(externalPendingHost&&!pendingHost.PreownedOriginal()){qualificationFailed=true;return false;}
            if(!route.PreownOriginal()){storagePreownException=route.PreownExceptionOriginal();qualificationFailed=true;return false;}
            if(!commands.PreownOriginal()){storagePreownException=commands.PreownExceptionOriginal();qualificationFailed=true;return false;}
            if(!sink.PreownOriginal()){storagePreownException=sink.PreownExceptionOriginal();qualificationFailed=true;return false;}
            if(!collector.PreownOriginal()){storagePreownException=collector.PreownExceptionOriginal();qualificationFailed=true;return false;}
            if(!finalOutput.PreownOriginal()){storagePreownException=finalOutput.PreownExceptionOriginal();qualificationFailed=true;return false;}
            inputOwners=std::make_unique<LinuxInputOwner[]>(selectedCommandCapacity);allocatedInputOwnerCapacity=selectedCommandCapacity;
            storagePreowned=true;return true;
        }catch(...){storagePreownException=std::current_exception();qualificationFailed=true;return false;}
    }
public:
    struct RetainedPreownView {
        const NativePlatformState* platformOriginal=nullptr;
        const template_creator_linux::OriginalLinuxPendingHost* pendingHostOriginal=nullptr;
        const template_creator_linux::OriginalCommandRoute* routeOriginal=nullptr;
        const LinuxInputOwner* inputOwnersOriginal=nullptr;
        const std::exception_ptr* exceptionOriginal=nullptr;
        template_creator_linux::OriginalLinuxPendingHost::RetainedPreownBackingView pendingBacking{};
        std::size_t selectedCommands=0,allocatedCommandSlots=0,bornCommands=0,allocatedInputs=0;
        std::size_t sinkCopied=0,sinkReadbacks=0,sinkCalls=0;
        std::size_t collectorBytes=0,collectorReads=0,collectorJoins=0,collectorReturns=0;
        std::size_t finalCopied=0,finalReadbacks=0,finalCalls=0;
        std::size_t finalCollectorBytes=0,finalCollectorReads=0,finalCollectorJoins=0,finalCollectorReturns=0;
        bool attempted=false,returned=false,preowned=false,adoptedHost=false;
    };
    bool ObserveRetainedPreownViewOriginal(RetainedPreownView& out) const noexcept {
        if(!storagePreownAttempted||!storagePreownReturned||pending.OriginalSupervisorOwner().HasPendingOriginalNativeCall())return false;
        const auto destination=reinterpret_cast<std::uintptr_t>(&out),source=reinterpret_cast<std::uintptr_t>(this);
        if(destination>UINTPTR_MAX-sizeof(out)||source>UINTPTR_MAX-sizeof(*this)||
           (destination<source+sizeof(*this)&&source<destination+sizeof(out)))return false;
        std::memset(&out,0,sizeof(out)); // Initialize private serialized padding in actual destination.
        auto& view=out;
        if(!pendingHost.ObserveRetainedPreownBackingOriginal(view.pendingBacking))return false;
        view.platformOriginal=this;view.pendingHostOriginal=&pendingHost;view.routeOriginal=&route;
        view.inputOwnersOriginal=inputOwners.get();view.exceptionOriginal=&storagePreownException;
        view.selectedCommands=selectedCommandCapacity;view.allocatedCommandSlots=route.AllocatedCommandCapacityOriginal();
        view.bornCommands=route.BornCommandCountOriginal();view.allocatedInputs=allocatedInputOwnerCapacity;
        if(view.bornCommands>view.allocatedCommandSlots||view.allocatedCommandSlots>view.selectedCommands||
           view.allocatedInputs>view.selectedCommands||bool(view.inputOwnersOriginal)!=bool(view.allocatedInputs))return false;
        view.sinkCopied=sink.AllocatedCopiedCapacityOriginal();view.sinkReadbacks=sink.AllocatedReadbackCapacityOriginal();
        view.sinkCalls=sink.AllocatedCallCapacityOriginal();
        view.collectorBytes=collector.AllocatedByteCapacityOriginal();view.collectorReads=collector.AllocatedReadCapacityOriginal();
        view.collectorJoins=collector.AllocatedJoinCapacityOriginal();view.collectorReturns=collector.AllocatedJoinReturnCapacityOriginal();
        view.finalCopied=finalOutput.sink.AllocatedCopiedCapacityOriginal();view.finalReadbacks=finalOutput.sink.AllocatedReadbackCapacityOriginal();
        view.finalCalls=finalOutput.sink.AllocatedCallCapacityOriginal();
        view.finalCollectorBytes=finalOutput.collector.AllocatedByteCapacityOriginal();
        view.finalCollectorReads=finalOutput.collector.AllocatedReadCapacityOriginal();
        view.finalCollectorJoins=finalOutput.collector.AllocatedJoinCapacityOriginal();
        view.finalCollectorReturns=finalOutput.collector.AllocatedJoinReturnCapacityOriginal();
        view.attempted=storagePreownAttempted;view.returned=storagePreownReturned;view.preowned=storagePreowned;view.adoptedHost=externalPendingHost;
        return true; // Resident owner relation, not portable full graph or fit.
    }
    RetainedPreownView failedPreownRetainedView{};
    bool failedPreownViewPublished=false;
    bool SameRetainedFailedPreownViewOriginal(const RetainedPreownView* view) const noexcept {
        if(!failedPreownPhysicalDisposed||!failedPreownViewPublished||view!=&failedPreownRetainedView)return false;
        RetainedPreownView current{};
        return ObserveRetainedPreownViewOriginal(current)&&!std::memcmp(&current,view,sizeof(current));
    }
    enum class FailedPreownHostFrontier { None, OwnedBeforeBegin, AdoptedOriginalHost };
    FailedPreownHostFrontier FailedPreownSourceFrontierOriginal() const noexcept {
        if(!storagePreownAttempted||!storagePreownReturned||storagePreowned||!qualificationFailed||
           !storagePreownException||transferAttempted||captureReturned||captureResult||
           adoptAttempted||adopted||prechargeAttempted||stepsBound||sinkPrepareAttempted||sinkPrepared||
           collectorStartAttempted||collectorStartReturned||inputBindingAttempted||
           route.AnyCommandAcquisitionAttempted()||finalOutput.attempted_||
           finalPublicationAttempted||originalIngressIdentity||originalSteps||originalInputs||originalSnapshots)
            return FailedPreownHostFrontier::None;
        if(externalPendingHost){
            if(!externalPendingHostOwned||!pendingHost.IsOriginalPlatformOwner(this)||!pendingHost.BeganOriginal())
                return FailedPreownHostFrontier::None;
            return FailedPreownHostFrontier::AdoptedOriginalHost;
        }
        if(&pendingHost!=&pendingHostStorage||pendingHost.BeginAttemptedOriginal()||pendingHost.BeganOriginal())
            return FailedPreownHostFrontier::None;
        return FailedPreownHostFrontier::OwnedBeforeBegin;
    }
    NativePlatformState(const NativePlatformState&)=delete;
    NativePlatformState& operator=(const NativePlatformState&)=delete;
    PyObject* AdoptIngress(PyObject*,PyObject*,PyObject*);
    PyObject* PrechargeRoute(PyObject*);
    PyObject* CaptureInputClasses(PyObject*,PyObject*);
    PyObject* BindInputs(PyObject*,PyObject*);
    bool PreownInputLayout(std::size_t,const std::size_t*,std::size_t);
    bool DisposeNativeInputOriginalsEligible() noexcept;
    bool ConsumeSettledHostOriginals() noexcept;
    PyObject* PrepareSink();
    PyObject* CaptureCommand(PyObject*);
    PyObject* VerifyCommand(PyObject*,PyObject*);
    PyObject* DisposeEligible(PyObject*);
    PyObject* InputDisposalEligible(PyObject*,PyObject*);
    PyObject* CaptureCreator(PyObject*,PyObject*);
    PyObject* VerifyClosure();
    bool RetainPrelaunchOriginalAssociations() noexcept;
    bool PrelaunchOriginalAssociationsEstablished() const noexcept;
    bool ExecutedOriginalCommandsQualified() const noexcept;
    bool BindFailedPreownStartupOriginal(const OriginalLinuxSelectionStartup&) noexcept;
    bool ObserveFailedPreownDispositionOriginal() noexcept;
    bool ObservePhysicalHostClosureEligible() noexcept;
    bool AllPhysicalOriginalsDisposed() const noexcept;
    bool ReservedPlatformSourceBytes(std::size_t&) const noexcept;
    bool PublishFinalPlatformOriginals() noexcept;
    bool PrepareFinalPublicationInputCut() noexcept;
};
}
#endif

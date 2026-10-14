// UNCOMPILED initial source-owner join. Authentic complete-input admission,
// measured whole-runtime fit and future lifecycle outcomes are independent.
#ifdef _WIN32
#include "windows_original_capabilities.hpp"
namespace template_creator_bridge {
bool NativePlatformState::LaunchAssociationsEstablished() const noexcept {
    if(qualificationFailed || !sourceBindingAttempted || !sourceObjectsAssociated ||
        !sourceCapabilitiesOriginal || !sourceMainOriginal || !sourceHostOriginal ||
        !ingressBound || !ingress.Captured() || !adopted || !stepsBound ||
        !inputCapturesClean || !inputOwners || route.Failed() ||
        !bootstrapSnapshotOriginal || !snapshotsOriginal || !stepsOriginal ||
        !PyTuple_CheckExact(snapshotsOriginal) || !PyTuple_CheckExact(stepsOriginal))return false;
    const auto& source=*sourceCapabilitiesOriginal;
    const auto& main=*sourceMainOriginal;
    const auto& host=*sourceHostOriginal;
    if(!source.AcquisitionReturned() || !source.ConnectionReturned() || source.DisposalAttempted() ||
        source.ConnectedMainOriginal()!=&main || !main.connected || main.host!=&host ||
        host.platform!=this || !host.core || !host.core->OnOriginalPythonThread() ||
        !host.SourceChannelAssociationEstablished(*this))return false;
    if(!main.routeProducer || !main.routeProducer->returned || main.routeProducer->nativeOriginal ||
        main.routeProducer->failureType || main.routeProducer->steps!=stepsOriginal ||
        !main.nativeRouteArguments || main.nativeRouteArguments->OriginalSelectedPhase()!=originalPhase ||
        main.routeProducer->arguments!=main.nativeRouteArguments->OriginalArguments() ||
        !main.nativeRouteArguments->OriginalEnvironment())return false;
    if(!main.bootstrapOriginal || main.bootstrapOriginal->snapshot!=bootstrapSnapshotOriginal)return false;
    const auto& invocation=host.core->PrivateCreatorInvocation();
    if(!invocation.attempted || !invocation.callAttempted || !invocation.entry ||
        invocation.entry!=host.entryOriginal)return false;
    const auto& originals=source.PrivateOriginals();
    for(const auto& original:originals)
        if(!original.valid || !original.acquired || original.outputUnknown || original.closeAttempted)return false;
    const auto& received=ingress.OriginalCapabilities();
    for(SIZE_T i=0;i<received.size();++i)
        if(received[i].original!=transferredOriginals[i] || !received[i].informationResult ||
            received[i].closeAttempted || received[i].aliasesEarlier || !received[i].original ||
            received[i].original==INVALID_HANDLE_VALUE)return false;
    const auto& originalSink=source.PrivateSink();
    if(!originalSink.fileAcquired || originalSink.creationUnknown || originalSink.fileCloseAttempted ||
        !originalSink.sink.CompletionKnown() || !sink.CompletionKnown() ||
        sink.BorrowedOriginal()!=received[4].original)return false;
    const auto count=originalPhase==OriginalPhase::Generation?1:
        originalPhase==OriginalPhase::FinalQualification?2:0;
    if(!count || route.SelectedCount()!=static_cast<SIZE_T>(count+1) ||
        PyTuple_GET_SIZE(snapshotsOriginal)!=count || PyTuple_GET_SIZE(stepsOriginal)!=count)return false;
    const auto& fixture=source.PrivateFixtureRoot();
    if(main.nativeRouteArguments->OriginalFixtureRoot()!=&fixture)return false;
    for(SIZE_T ordinal=0;ordinal<route.SelectedCount();++ordinal) {
        const auto& owner=inputOwners[ordinal];
        const auto& row=route.PrivateOriginal(ordinal);
        PyObject* snapshot=ordinal?PyTuple_GET_ITEM(snapshotsOriginal,static_cast<Py_ssize_t>(ordinal-1)):bootstrapSnapshotOriginal;
        if(owner.snapshot!=snapshot || !owner.captureAttempted || !owner.files || owner.originalException ||
            owner.disposalAttempted || !row.precharged || row.prechargeOriginal || !row.gate ||
            !row.command.SameSnapshot(snapshot) ||
            !row.gate->InitialSourceAssociations(ingress,*owner.files,row.command,ordinal?&fixture:nullptr))return false;
        if(ordinal && (!owner.directoryAcquired || owner.directoryUnknown || !owner.directoryAssociated ||
            owner.directoryCloseAttempted || !owner.directoryIdentity.captured))return false;
        if(ordinal) {
            if(!PyTuple_Check(snapshot) || PyTuple_GET_SIZE(snapshot)!=5)return false;
            PyObject* originalCommand=PyTuple_GET_ITEM(snapshot,0);
            if(!originalCommand || !PyTuple_Check(originalCommand) || PyTuple_GET_SIZE(originalCommand)!=6 ||
                PyTuple_GET_ITEM(originalCommand,3)!=main.nativeRouteArguments->OriginalEnvironment())return false;
            const auto launch=row.gate->OriginalLaunchFileOrdinal();
            if(launch>=owner.files->FileCount())return false;
            const auto& heldTool=owner.files->PrivateFiles()[launch].first;
            const auto& transferredTool=sourceTransferredIdentities[1];
            if(!heldTool.captured || !transferredTool.captured ||
                heldTool.id.VolumeSerialNumber!=transferredTool.id.VolumeSerialNumber ||
                memcmp(heldTool.id.FileId.Identifier,transferredTool.id.FileId.Identifier,
                    sizeof(heldTool.id.FileId.Identifier)))return false;
        }
    }
    // No CREATE/image/EXIT or generated-code borrower result is demanded here.
    // Their exact native owners remain subject to continuation and final gates.
    return true;
}
}
#endif

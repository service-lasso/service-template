// UNCOMPILED source-owned transfer association. Object identity joins actual
// acquisitions and duplicates; it is not tool authenticity or launch authority.
#ifdef _WIN32
#include "windows_original_capabilities.hpp"
namespace template_creator_bridge {
template<class Source,class Receiver>
static bool SameTransferredObject(const Source& source,const Receiver& receiver) noexcept {
    return source.captured && receiver.captured &&
        source.id.VolumeSerialNumber==receiver.id.VolumeSerialNumber &&
        memcmp(source.id.FileId.Identifier,receiver.id.FileId.Identifier,
            sizeof(source.id.FileId.Identifier))==0 &&
        source.standard.Directory==receiver.standard.Directory;
}
bool NativePlatformState::BindSourceOriginals(const OriginalWindowsSourceCapabilities& actual,
                                             const OriginalWindowsNativeMain& main) noexcept {
    if(sourceBindingAttempted)return false;
    sourceBindingAttempted=true;
    // Attach both exact source owners before native observation, including a
    // failed association. Neither owner is disposed by this method.
    sourceCapabilitiesOriginal=&actual;sourceMainOriginal=&main;
    if(!actual.AcquisitionReturned() || !actual.ConnectionReturned() ||
        actual.ConnectedMainOriginal()!=&main || actual.DisposalAttempted() ||
        !main.connected || !main.host || main.host!=sourceHostOriginal ||
        main.host->platform!=this || !main.host->core ||
        !main.host->core->OnOriginalPythonThread() ||
        !main.host->SourceChannelAssociationEstablished(*this) ||
        !ingressBound || !ingress.Captured()) {
        qualificationFailed=true;return false;
    }
    const auto& originals=actual.PrivateOriginals();
    const auto& received=ingress.OriginalCapabilities();
    const auto& sinkOriginal=actual.PrivateSink();
    if(!sinkOriginal.fileAcquired || sinkOriginal.fileCloseAttempted ||
        sinkOriginal.creationUnknown || !sinkOriginal.sink.CompletionKnown()) {
        qualificationFailed=true;return false;
    }
    bool clean=true;
    for(SIZE_T i=0;i<3;++i) {
        // Slots 2/3/4 are the exact channel's root/tool/sink duplicates.
        // All rows exist in the platform before the first native query.
        const auto& capability=received[i+2];
        if(!capability.informationResult || capability.closeAttempted ||
            !capability.original || capability.original==INVALID_HANDLE_VALUE) {
            clean=false;continue;
        }
        sourceTransferredIdentities[i].Capture(capability.original);
        if(i==2)sourceSinkIdentity.Capture(actual.PrivateSink().file);
        if((i<2 && (!originals[i].valid || originals[i].closeAttempted || originals[i].outputUnknown)) ||
            (i<2 ? !SameTransferredObject(originals[i].identity,sourceTransferredIdentities[i]) :
                   !SameTransferredObject(sourceSinkIdentity,sourceTransferredIdentities[i])))clean=false;
    }
    sourceObjectsAssociated=clean;
    if(!clean)qualificationFailed=true;
    return clean;
}
}
#endif

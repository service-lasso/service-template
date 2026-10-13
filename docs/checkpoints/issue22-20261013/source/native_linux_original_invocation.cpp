// UNCOMPILED connected native entry. No target execution/native qualification.
#ifndef _WIN32
#include "linux_private_selection_startup.hpp"
namespace template_creator_bridge {
OriginalLinuxSelectionStartup* BeginOriginalLinuxSelectionStartup(const OriginalLinuxSelectionStartupInput& source){
    auto* startup=new OriginalLinuxSelectionStartup();
    if(startup->PreownOriginal(source))startup->BeginOriginal();
    return startup; // Resident owner on every partial original phase.
}
OriginalLinuxSelectionDecoder* BeginOriginalLinuxCapturedSelection(
    template_creator_linux::OriginalLinuxHeldFile& original,int originalProcRoot,
    const int* capabilities,std::size_t count,template_creator_linux::OriginalLinuxPendingHost* originalHost){
    auto* decoder=new OriginalLinuxSelectionDecoder(original,originalProcRoot,capabilities,count,originalHost);
    // No constructor effect except owned copies. Decode/Begin retain every
    // later failure, so this pointer returns on partial native phases too.
    if(decoder->DecodeOriginal())decoder->BeginDecodedOriginal();
    return decoder;
}
OriginalLinuxInvocation* BeginOriginalLinuxInvocation(
    const OriginalLinuxPythonPaths& paths,std::size_t pythonUnits,std::size_t sessions,
    const OriginalLinuxHostSelection& host,const OriginalLinuxRouteSelection& route,
    std::size_t routeStringBytes,std::size_t mainObservations){
    // The initial allocation may throw before effects. Once attached, no stack
    // unwind destroys acquired capability/interpreter/native/exception owners.
    auto* owner=new OriginalLinuxInvocation();owner->attempted=true;
    owner->hostSelectionOriginal=&host;owner->routeSelectionOriginal=&route;owner->pythonPathsOriginal=&paths;
    try{
        owner->main=new OriginalLinuxNativeMain(mainObservations);owner->mainAttached=true;
        if(host.phase!=route.phase){owner->failed=true;return owner;}
        owner->routePreownAttempted=true;
        owner->routePreowned=owner->main->PreownOriginalRouteArguments(route,routeStringBytes);
        if(!owner->routePreowned){owner->failed=true;return owner;}
        owner->interpreterAttempted=true;
        owner->interpreterReturned=owner->main->BeginOriginal(paths,pythonUnits,sessions);
        if(!owner->interpreterReturned){owner->failed=true;return owner;}
        owner->connectAttempted=true;owner->connectReturned=owner->main->ConnectOriginal(host);
        if(!owner->connectReturned){owner->failed=true;return owner;}
        owner->creatorAttempted=true;owner->creatorReturned=owner->main->ProduceAndInvokeOriginalRouteArguments();
        if(!owner->creatorReturned)owner->failed=true;
    }catch(...){owner->nativeOriginal=std::current_exception();owner->failed=true;}
    return owner; // Resident state, never successful process/interpreter exit.
}
bool ObserveOriginalLinuxInvocationClosure(OriginalLinuxInvocation& owner) noexcept {
    if(!owner.attempted||!owner.mainAttached||!owner.main)return false;
    owner.closureObserved=true;
    // Diagnostic/source qualification failure cannot suppress independently
    // eligible closure of an actually published partial core/platform owner.
    owner.physicalHostDisposed=owner.main->ObservePhysicalHostClosure();
    if(owner.main->failed||owner.main->nativeOriginal)owner.failed=true;
    return owner.physicalHostDisposed; // Physical-only, no normal result grant.
}
bool ObserveOriginalLinuxFailedPreownInputEligibility(OriginalLinuxInvocation& owner,NativePlatformState* exactPlatform) noexcept {
    if(!owner.attempted||!owner.mainAttached||!owner.main||!exactPlatform)return false;
    return owner.main->FailedPreownInputCleanupEligible(exactPlatform);
}
bool ObserveOriginalLinuxPrePlatformInputEligibility(OriginalLinuxInvocation& owner) noexcept {
    owner.prePlatformInputEligibilityAttempted=true;
    // An attached main may already own a live/partially initialized interpreter.
    // Keep it and its registry/Python originals; this is not their disposal.
    owner.prePlatformInputEligibility=!owner.mainAttached||
        (owner.main&&owner.main->PrePlatformInputCleanupEligible());
    if(!owner.prePlatformInputEligibility)owner.failed=true;
    return owner.prePlatformInputEligibility;
}
}
#endif

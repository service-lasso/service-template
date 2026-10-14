// UNCOMPILED closed native caller; not executable startup or an exit grant.
#pragma once
#ifndef _WIN32
#include "linux_original_main.hpp"
namespace template_creator_bridge {
struct OriginalLinuxInvocation {
    OriginalLinuxNativeMain* main=nullptr;
    const OriginalLinuxHostSelection* hostSelectionOriginal=nullptr;
    const OriginalLinuxRouteSelection* routeSelectionOriginal=nullptr;
    const OriginalLinuxPythonPaths* pythonPathsOriginal=nullptr;
    std::exception_ptr nativeOriginal;
    bool attempted=false,mainAttached=false,routePreownAttempted=false,routePreowned=false;
    bool interpreterAttempted=false,interpreterReturned=false,connectAttempted=false,connectReturned=false;
    bool creatorAttempted=false,creatorReturned=false;
    bool closureObserved=false,physicalHostDisposed=false,failed=false;
    bool prePlatformInputEligibilityAttempted=false,prePlatformInputEligibility=false;
    // Main/registry/error graphs remain resident on every failed/pending path.
    // Original constructor references require real enclosing caller custody;
    // they are never read later as a substitute for captured native owners.
};
OriginalLinuxInvocation* BeginOriginalLinuxInvocation(
    const OriginalLinuxPythonPaths&,std::size_t pythonUnits,std::size_t sessions,
    const OriginalLinuxHostSelection&,const OriginalLinuxRouteSelection&,
    std::size_t routeStringBytes,std::size_t mainObservations);
bool ObserveOriginalLinuxInvocationClosure(OriginalLinuxInvocation&) noexcept;
bool ObserveOriginalLinuxPrePlatformInputEligibility(OriginalLinuxInvocation&) noexcept;
bool ObserveOriginalLinuxFailedPreownInputEligibility(OriginalLinuxInvocation&,NativePlatformState*) noexcept;
}
#endif

// UNCOMPILED actual owned source-allocation composition. This is arithmetic,
// not measured physical residency, observer capacity or launch authorization.
#ifdef _WIN32
#include "native_windows_platform.hpp"
namespace template_creator_bridge {
bool NativePlatformState::ReservedPlatformSourceBytes(SIZE_T& bytes) const noexcept {
    // The three inline owners below return counters including their own sizeof;
    // subtract those inline cells before composing to avoid double counting.
    bytes=sizeof(*this)-sizeof(route)-sizeof(commands)-sizeof(sink);
    const auto add=[&](SIZE_T amount) noexcept {
        if(amount>SIZE_MAX-bytes)return false;
        bytes+=amount;return true;
    };
    SIZE_T amount=0;
    if(!route.ReservedSourceBytes(amount)||!add(amount)||
        !add(commands.ReservedSourceBytes())||!sink.ReservedSourceBytes(amount)||!add(amount))return false;
    const auto count=route.SelectedCount();
    if(count>SIZE_MAX/sizeof(OriginalWindowsInputOwner)||!add(count*sizeof(OriginalWindowsInputOwner)))return false;
    for(SIZE_T i=0;i<count;++i) {
        const auto& owner=inputOwners[i];
        if(owner.geometry) {
            if(owner.imageRows>SIZE_MAX/sizeof(template_creator::GateImageGeometry)||
                !add(owner.imageRows*sizeof(template_creator::GateImageGeometry)))return false;
        }
        if(owner.files&&!add(owner.files->ReservedSourceBytes()))return false;
    }
    if(collector&&(!collector->ReservedSourceBytes(amount)||!add(amount)))return false;
    return true;
}
// Python referents/error graphs, registry/core/host/interpreter/final sink/main
// owners, allocator slack, native stacks/kernel objects, loaded runtime/JIT and
// overlapping process residency are separate actual charges. This method
// includes the route's current+retained lifecycle stores and all four dynamic
// stream byte stores via its nested counter, not merely inline unique_ptrs.
}
#endif

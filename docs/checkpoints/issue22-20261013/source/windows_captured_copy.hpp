// UNCOMPILED exact SAME-owner byte comparison for copied CODE producers.
// This is byte/source-owner association only, never producer or launch authority.
#pragma once
#include "windows_held_files.hpp"
namespace template_creator {
enum class OriginalCopyOutcome { Unavailable, Equal, Different };
struct OriginalCapturedCopyComparison {
    const NativeHeldFiles* sourceOwner=nullptr;
    const NativeHeldFiles* generatedOwner=nullptr;
    SIZE_T sourceOrdinal=SIZE_MAX,generatedOrdinal=SIZE_MAX;
    const unsigned char* sourceBytes=nullptr;
    const unsigned char* generatedBytes=nullptr;
    SIZE_T sourceExtent=0,generatedExtent=0;
    bool attempted=false,sourceAvailable=false,generatedAvailable=false;
    OriginalCopyOutcome outcome=OriginalCopyOutcome::Unavailable;
    bool Observe(const NativeHeldFiles& source,SIZE_T sourceIndex,
                 const NativeHeldFiles& generated,SIZE_T generatedIndex) noexcept {
        if(attempted)return false;attempted=true;
        // Preserve BOTH actual references/ordinals before availability checks.
        sourceOwner=&source;generatedOwner=&generated;
        sourceOrdinal=sourceIndex;generatedOrdinal=generatedIndex;
        sourceAvailable=source.ExactCapturedSourceSpan(sourceIndex,sourceBytes,sourceExtent);
        generatedAvailable=generated.ExactCapturedSourceSpan(generatedIndex,generatedBytes,generatedExtent);
        if(!sourceAvailable || !generatedAvailable)return false;
        if(sourceExtent!=generatedExtent)outcome=OriginalCopyOutcome::Different;
        else if(!sourceExtent || memcmp(sourceBytes,generatedBytes,sourceExtent)==0)
            outcome=OriginalCopyOutcome::Equal;
        else outcome=OriginalCopyOutcome::Different;
        return true; // Different is an observed comparison, not an exception.
    }
};
// Both native owners and their complete retained byte backing outlive this row.
// Equality supplies no path/alias stability, original producer, caller/source
// family, runtime convention, writer exclusion or generated-consumer permission.
}

// UNCOMPILED actual retained initializer key/value representation. Native
// provider correctness, private alias custody and continuation remain separate.
#pragma once
#include "windows_coreclr_entry_inputs.hpp"
#include "windows_host_runtime_contract_observation.hpp"
namespace template_creator {
struct OriginalCoreClrEntryProperties {
    const OriginalCoreClrEntryInputs* inputsOriginal=nullptr;
    OriginalHostRuntimeContractObservation hostContractOriginal;
    std::array<SIZE_T,10> firstCommonIndex{},commonMatches{};
    SIZE_T propertiesVisited=0,emptyKeys=0;
    bool attempted=false,representationObserved=false;
    bool Observe(const OriginalCoreClrEntryInputs& input) noexcept {
        if(attempted)return false;attempted=true;inputsOriginal=&input;
        firstCommonIndex.fill(SIZE_MAX);
        if(!input.captured || !input.revalidated || input.failed || input.propertyCount<0)return false;
        const SIZE_T count=static_cast<SIZE_T>(input.propertyCount);
        if(count>input.PrivatePointerCapacity() || count>(SIZE_MAX-2)/2 ||
            input.spanCount!=2+2*count || input.stringBytes>input.PrivateStringCapacity())return false;
        const auto* spans=input.PrivateStrings();
        const auto* bytes=input.PrivateStringBytes();
        const auto* again=input.PrivateRevalidatedStringBytes();
        if(!spans || !bytes || !again)return false;
        if(count && (!input.PrivatePointers(0) || !input.PrivatePointers(1) ||
            !input.PrivateRevalidatedPointers(0) || !input.PrivateRevalidatedPointers(1)))return false;
        SIZE_T nextOffset=0;
        for(SIZE_T i=0;i<input.spanCount;++i) {
            const auto& span=spans[i];
            if(!span.attempted || !span.terminated || !span.revalidated || !span.extent ||
                span.offset!=nextOffset || span.offset>input.stringBytes || span.extent>input.stringBytes-span.offset ||
                bytes[span.offset+span.extent-1] ||
                memchr(bytes+span.offset,0,span.extent-1) ||
                memcmp(bytes+span.offset,again+span.offset,span.extent))return false;
            const DWORD64 expected=i==0?input.exePath:i==1?input.friendlyName:
                input.PrivatePointers(i<2+count?0:1)[i<2+count?i-2:i-2-count];
            if(span.address!=expected)return false;
            nextOffset+=span.extent;
        }
        if(nextOffset!=input.stringBytes)return false;
        constexpr const char* common[]={"TRUSTED_PLATFORM_ASSEMBLIES","NATIVE_DLL_SEARCH_DIRECTORIES",
            "PLATFORM_RESOURCE_ROOTS","APP_CONTEXT_BASE_DIRECTORY","APP_CONTEXT_DEPS_FILES",
            "FX_DEPS_FILE","PROBING_DIRECTORIES","STARTUP_HOOKS","APP_PATHS","RUNTIME_IDENTIFIER"};
        for(SIZE_T i=0;i<count;++i) {
            const auto& key=spans[2+i];
            const auto& value=spans[2+count+i];
            if(input.PrivatePointers(0)[i]!=input.PrivateRevalidatedPointers(0)[i] ||
                input.PrivatePointers(1)[i]!=input.PrivateRevalidatedPointers(1)[i] ||
                key.address!=input.PrivatePointers(0)[i] || value.address!=input.PrivatePointers(1)[i])return false;
            ++propertiesVisited;if(key.extent==1)++emptyKeys;
            for(SIZE_T role=0;role<commonMatches.size();++role) {
                const SIZE_T length=strlen(common[role]);
                if(key.extent!=length+1 || memcmp(bytes+key.offset,common[role],length+1))continue;
                if(!commonMatches[role])firstCommonIndex[role]=i;
                ++commonMatches[role];
            }
        }
        representationObserved=true;return true;
        // No sorting, replacement, duplicate rejection or inferred provider role.
    }
};
}

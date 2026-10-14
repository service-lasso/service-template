// UNCOMPILED original initializer contract referent. No callback is executed.
#pragma once
#include "windows_coreclr_entry_inputs.hpp"
#include "primary-current-runtime-host_runtime_contract.h"
namespace template_creator {
struct OriginalHostRuntimeContractObservation {
    const OriginalCoreClrEntryInputs* inputsOriginal=nullptr;
    SIZE_T propertyIndex=SIZE_MAX,propertyMatches=0;
    DWORD64 addressOriginal=0;
    host_runtime_contract first{},again{};
    std::array<OriginalCoreClrEntryInputs::ReadOriginal,2> reads{};
    bool attempted=false,representationObserved=false;
    const char* firstFailure=nullptr;
    bool Fail(const char* reason) noexcept {
        if(!firstFailure)firstFailure=reason;return false;
    }
    bool SameStop() const noexcept {
        if(!inputsOriginal || !inputsOriginal->stateOriginal || !inputsOriginal->stoppedOriginal ||
            !inputsOriginal->contextOriginal || !inputsOriginal->contextOriginal->threadCreateOriginal)return false;
        const auto& state=*inputsOriginal->stateOriginal;
        return state.eventCount && &state.events[state.eventCount-1]==inputsOriginal->stoppedOriginal &&
            !inputsOriginal->stoppedOriginal->continued &&
            OriginalStoppedThreadOccurrence(state,*inputsOriginal->stoppedOriginal)==
                inputsOriginal->contextOriginal->threadCreateOriginal;
    }
    bool Read(SIZE_T ordinal,host_runtime_contract& destination) noexcept {
        if(ordinal>=reads.size() || !SameStop() || !inputsOriginal->pendingOriginal ||
            !inputsOriginal->commandOriginal || !inputsOriginal->processOriginal || !addressOriginal || addressOriginal>UINTPTR_MAX ||
            sizeof(destination)-1>UINTPTR_MAX-static_cast<std::uintptr_t>(addressOriginal))
            return Fail("host_contract_original_read_inputs");
        auto& row=reads[ordinal];if(row.attempted)return Fail("host_contract_read_repeated");
        row.process=inputsOriginal->processOriginal;
        row.address=reinterpret_cast<const void*>(static_cast<std::uintptr_t>(addressOriginal));
        row.destination=&destination;row.requested=sizeof(destination);row.attempted=true;
        row.pending=inputsOriginal->pendingOriginal->BeginOriginal(this,&row,row.destination,row.requested,
            inputsOriginal->commandOriginal,offsetof(OriginalCoreClrEntryInputs::ReadOriginal,actual));
        if(!row.pending)return Fail("host_contract_pending_capacity_or_freeze");
        row.returned=ReadProcessMemory(row.process,row.address,row.destination,row.requested,&row.actual);
        if(!row.returned)row.error=GetLastError();row.returnedObserved=true;
        if(!inputsOriginal->pendingOriginal->ReturnedOriginal(*row.pending,row.returned,row.error,row.actual))
            return Fail("host_contract_read_completion_unknown");
        if(!row.returned || row.actual!=row.requested || !SameStop())
            return Fail("host_contract_read_failed_partial_or_stop_changed");
        return true;
    }
    bool Capture(const OriginalCoreClrEntryInputs& input) noexcept {
        if(attempted)return false;attempted=true;inputsOriginal=&input;
#if defined(_M_AMD64) || defined(__x86_64__)
        static_assert(sizeof(host_runtime_contract)==48 && offsetof(host_runtime_contract,context)==8 &&
            offsetof(host_runtime_contract,get_runtime_property)==16 &&
            offsetof(host_runtime_contract,bundle_probe)==24 && offsetof(host_runtime_contract,pinvoke_override)==32 &&
            offsetof(host_runtime_contract,external_assembly_probe)==40,"selected upstream AMD64 contract ABI");
        if(!input.captured || !input.revalidated || input.failed || input.propertyCount<0 || !SameStop())
            return Fail("host_contract_entry_not_captured");
        const SIZE_T count=static_cast<SIZE_T>(input.propertyCount);
        if(count>input.PrivatePointerCapacity() || count>(SIZE_MAX-2)/2 || input.spanCount!=2+2*count)
            return Fail("host_contract_property_roster");
        const auto* spans=input.PrivateStrings();const auto* bytes=input.PrivateStringBytes();
        if(!spans || !bytes)return Fail("host_contract_property_backing");
        constexpr char key[]=HOST_PROPERTY_RUNTIME_CONTRACT;
        for(SIZE_T i=0;i<count;++i) {
            const auto& span=spans[2+i];
            if(span.offset>input.stringBytes || span.extent>input.stringBytes-span.offset)
                return Fail("host_contract_key_extent");
            if(span.extent==sizeof(key) && !memcmp(bytes+span.offset,key,sizeof(key))) {
                if(propertyMatches==SIZE_MAX)return Fail("host_contract_property_count_width");
                ++propertyMatches;if(propertyIndex==SIZE_MAX)propertyIndex=i;
            }
        }
        if(propertyMatches!=1)return Fail("host_contract_property_missing_or_ambiguous");
        const auto& value=spans[2+count+propertyIndex];
        if(!value.terminated || !value.revalidated || value.offset>input.stringBytes ||
            value.extent>input.stringBytes-value.offset || value.extent<4 || value.extent>19)
            return Fail("host_contract_pointer_extent");
        const auto* text=bytes+value.offset;
        if(text[0]!='0' || text[1]!='x' || text[value.extent-1])return Fail("host_contract_pointer_form");
        for(SIZE_T i=2;i+1<value.extent;++i) {
            const unsigned char c=text[i];
            const DWORD64 digit=c>='0'&&c<='9'?c-'0':c>='a'&&c<='f'?c-'a'+10:16;
            if(digit==16 || addressOriginal>(UINT64_MAX-digit)/16)return Fail("host_contract_pointer_digits");
            addressOriginal=addressOriginal*16+digit;
        }
        if(!addressOriginal)return Fail("host_contract_null_pointer");
        if(!Read(0,first) || !Read(1,again))return false;
        if(first.size!=sizeof(first) || memcmp(&first,&again,sizeof(first)))
            return Fail("host_contract_size_or_revalidation");
        representationObserved=true;return true;
#else
        return Fail("host_contract_selected_architecture_missing");
#endif
    }
};
}

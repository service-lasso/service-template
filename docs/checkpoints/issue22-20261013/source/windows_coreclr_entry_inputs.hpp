// UNCOMPILED AMD64 initializer input bytes. This owner does not establish the
// calling binary/source ABI, an owned breakpoint, or a continuation decision.
#pragma once
#include "windows_retained_backing.hpp"
#include "windows_coreclr_export.hpp"
#include "windows_coreclr_thread_context.hpp"
#include <exception>
namespace template_creator {
class OriginalCoreClrEntryInputs {
#if defined(TEMPLATE_NATIVE_CHAIN_REGRESSION)
    friend bool ObservePartialPreownBackingRegression() noexcept;
#endif
public:
    struct ReadOriginal {
        HANDLE process=nullptr;
        const void* address=nullptr;
        void* destination=nullptr;
        SIZE_T requested=0,actual=0;
        BOOL returned=FALSE;
        DWORD error=0;
        template_creator_bridge::OriginalWindowsPendingCall* pending=nullptr;
        bool attempted=false,returnedObserved=false;
    };
    struct StringOriginal {
        DWORD64 address=0;
        SIZE_T offset=0,extent=0;
        bool attempted=false,terminated=false,revalidated=false;
    };
private:
    SIZE_T pointerCapacity_=0,stringCapacity_=0,readCapacity_=0,reservedBytes_=0;
    std::array<std::unique_ptr<DWORD64[]>,2> pointers_,revalidatedPointers_;
    std::unique_ptr<unsigned char[]> strings_,revalidatedStrings_;
    std::unique_ptr<StringOriginal[]> spans_;
    std::unique_ptr<ReadOriginal[]> reads_;
    bool Read(DWORD64 address,void* destination,SIZE_T extent) noexcept {
        if(!extent)return true; // No fabricated zero-byte API observation.
        if(!address || address>UINTPTR_MAX || extent-1>UINTPTR_MAX-static_cast<std::uintptr_t>(address))
            return Fail("entry_read_address_extent");
        if(readCount==readCapacity_) {
            if(!overflowPresent) {
                overflowPresent=true;overflow.process=processOriginal;
                overflow.address=reinterpret_cast<const void*>(static_cast<std::uintptr_t>(address));
                overflow.destination=destination;overflow.requested=extent;
            }
            return Fail("entry_read_capacity_before_call");
        }
        auto& row=reads_[readCount++];row.process=processOriginal;
        row.address=reinterpret_cast<const void*>(static_cast<std::uintptr_t>(address));
        row.destination=destination;row.requested=extent;
        row.pending=pendingOriginal->BeginOriginal(this,&row,row.destination,row.requested,commandOriginal,
            offsetof(ReadOriginal,actual)); // Immutable prefix only; result cells publish after return.
        if(!row.pending)return Fail("entry_pending_capacity_or_frozen");
        row.attempted=true;
        row.returned=ReadProcessMemory(row.process,row.address,row.destination,row.requested,&row.actual);
        if(!row.returned)row.error=GetLastError();
        row.returnedObserved=true;
        if(!pendingOriginal->ReturnedOriginal(*row.pending,row.returned,row.error,row.actual))
            return Fail("entry_read_completion_unknown");
        if(!row.returned || row.actual!=row.requested)return Fail("entry_read_failed_or_partial");
        return true;
    }
    bool Fail(const char* reason) noexcept {
        failed=true;if(!firstFailure)firstFailure=reason;return false;
    }
    bool String(DWORD64 address) noexcept {
        if(spanCount==2*pointerCapacity_+2)return Fail("entry_string_span_capacity");
        auto& span=spans_[spanCount++];span.address=address;span.offset=stringBytes;span.attempted=true;
        for(;;) {
            if(stringBytes==stringCapacity_)return Fail("entry_string_byte_capacity");
            if(span.extent>UINT64_MAX-address)return Fail("entry_string_address_overflow");
            if(!Read(address+span.extent,strings_.get()+stringBytes,1))return false;
            const unsigned char byte=strings_[stringBytes++];++span.extent;
            if(!byte){span.terminated=true;return true;}
        }
    }
    bool SameStoppedOriginal() const noexcept {
        return stateOriginal && contextOriginal && contextOriginal->threadCreateOriginal &&
            stoppedOriginal && stateOriginal->eventCount &&
            &stateOriginal->events[stateOriginal->eventCount-1]==stoppedOriginal && !stoppedOriginal->continued &&
            OriginalStoppedThreadOccurrence(*stateOriginal,*stoppedOriginal)==contextOriginal->threadCreateOriginal;
    }
public:
    const OriginalCoreClrThreadContext* contextOriginal=nullptr;
    const OriginalCoreClrInitializeExport* exportOriginal=nullptr;
    const State* stateOriginal=nullptr;
    const Event* stoppedOriginal=nullptr;
    const template_creator_bridge::WindowsOriginalCommand* commandOriginal=nullptr;
    template_creator_bridge::OriginalWindowsPendingCalls* pendingOriginal=nullptr;
    HANDLE processOriginal=nullptr;
    std::array<DWORD64,8> stack{},revalidatedStack{};
    DWORD64 exePath=0,friendlyName=0,keysAddress=0,valuesAddress=0,hostHandleAddress=0,domainIdAddress=0;
    DWORD64 returnAddress=0;
    std::int32_t propertyCount=0;
    SIZE_T readCount=0,spanCount=0,stringBytes=0;
    ReadOriginal overflow{};
    bool overflowPresent=false,attempted=false,captured=false,revalidationAttempted=false,revalidated=false,failed=false;
    const char* firstFailure=nullptr;
    static SIZE_T CheckedSourceBytes(SIZE_T pointers,SIZE_T stringBytes,SIZE_T reads) {
        SIZE_T bytes=sizeof(OriginalCoreClrEntryInputs);
        const auto add=[&](SIZE_T count,SIZE_T width) {
            if(width && count>(SIZE_MAX-bytes)/width)throw std::invalid_argument("entry input aggregate geometry");
            bytes+=count*width;
        };
        if(!stringBytes || !reads || pointers>(SIZE_MAX-2)/2)
            throw std::invalid_argument("entry input geometry");
        add(pointers,4*sizeof(DWORD64));add(stringBytes,2);
        add(2*pointers+2,sizeof(StringOriginal));add(reads,sizeof(ReadOriginal));
        return bytes;
    }
    OriginalCoreClrEntryInputs(SIZE_T pointers,SIZE_T stringBytes,SIZE_T reads)
        :pointerCapacity_(pointers),stringCapacity_(stringBytes),readCapacity_(reads),reservedBytes_(sizeof(*this)) {}
    bool preownAttempted=false,preowned=false;
    std::exception_ptr preownExceptionOriginal;
    bool PreownOriginal() noexcept {
        if(preownAttempted)return false;preownAttempted=true;
        try {
        const SIZE_T pointers=pointerCapacity_,stringBytes=stringCapacity_,reads=readCapacity_;
        (void)CheckedSourceBytes(pointers,stringBytes,reads);
        for(SIZE_T role=0;role<2;++role) {
            pointers_[role]=std::make_unique<DWORD64[]>(pointers);
            reservedBytes_+=pointers*sizeof(DWORD64);
            revalidatedPointers_[role]=std::make_unique<DWORD64[]>(pointers);
            reservedBytes_+=pointers*sizeof(DWORD64);
        }
        strings_=std::make_unique<unsigned char[]>(stringBytes);
        reservedBytes_+=stringBytes;
        revalidatedStrings_=std::make_unique<unsigned char[]>(stringBytes);
        reservedBytes_+=stringBytes;
        spans_=std::make_unique<StringOriginal[]>(2*pointers+2);
        reservedBytes_+=(2*pointers+2)*sizeof(StringOriginal);
        reads_=std::make_unique<ReadOriginal[]>(reads);
        reservedBytes_+=reads*sizeof(ReadOriginal);preowned=true;return true;
        }catch(...){preownExceptionOriginal=std::current_exception();return false;}
    }
    OriginalCoreClrEntryInputs(const OriginalCoreClrEntryInputs&)=delete;
    OriginalCoreClrEntryInputs& operator=(const OriginalCoreClrEntryInputs&)=delete;
    bool CaptureOriginal(const OriginalCoreClrThreadContext& context,
            const OriginalCoreClrInitializeExport& exported,
            template_creator_bridge::OriginalWindowsPendingCalls& pending,
            const template_creator_bridge::WindowsOriginalCommand& command) noexcept {
        if(!preowned||attempted)return false;attempted=true;contextOriginal=&context;exportOriginal=&exported;
        stateOriginal=context.stateOriginal;stoppedOriginal=context.stoppedOriginal;
        commandOriginal=&command;pendingOriginal=&pending;
#if defined(_M_AMD64) || defined(__x86_64__)
        constexpr DWORD required=CONTEXT_CONTROL|CONTEXT_INTEGER|CONTEXT_DEBUG_REGISTERS;
        if(!context.captured || context.commandOriginal!=&command || context.pendingOwnerOriginal!=&pending ||
            (context.effective.ContextFlags&required)!=required || !command.copied ||
            !SameStoppedOriginal() || !exported.imageOriginal ||
            stoppedOriginal->original.dwDebugEventCode!=EXCEPTION_DEBUG_EVENT ||
            stoppedOriginal->original.dwThreadId!=context.threadCreateOriginal->original.dwThreadId ||
            stoppedOriginal->original.u.Exception.ExceptionRecord.ExceptionCode!=EXCEPTION_SINGLE_STEP ||
            stoppedOriginal->original.u.Exception.dwFirstChance!=1 ||
            stoppedOriginal->original.u.Exception.ExceptionRecord.ExceptionAddress!=exported.address ||
            exported.symbol!=OriginalCoreClrInitializeExport::Symbol::Initialize ||
            exported.result!=OriginalCoreClrInitializeExport::Result::Direct ||
            !exported.targetExecutableObserved || context.effective.Rip!=reinterpret_cast<std::uintptr_t>(exported.address))
            return Fail("entry_context_export_occurrence_unresolved");
        const auto& born=*context.threadCreateOriginal;
        if(born.createdThreadProcessResource>=stateOriginal->resourceCount ||
            born.createdThreadMemberOrdinal>=stateOriginal->memberCount)return Fail("entry_process_occurrence_unresolved");
        const auto& process=stateOriginal->resources[born.createdThreadProcessResource];
        const auto& member=stateOriginal->members[born.createdThreadMemberOrdinal];
        if(process.kind!=Kind::Process || !process.acquired || process.closed || process.attempted || process.unknownOutput ||
            process.original!=exported.imageOriginal->retainedProcess ||
            !same_time(member.birth,exported.imageOriginal->birth))return Fail("entry_loaded_process_occurrence_unresolved");
        processOriginal=process.original;
        // The admitted standard AMD64 seven-argument signature is a separate
        // source/build join. Right-justified int uses only the low 32 bits.
        const std::uint32_t low=static_cast<std::uint32_t>(context.effective.R8);
        memcpy(&propertyCount,&low,sizeof(propertyCount));
        if(propertyCount<0 || static_cast<SIZE_T>(propertyCount)>pointerCapacity_)
            return Fail("entry_property_count_geometry");
        exePath=context.effective.Rcx;friendlyName=context.effective.Rdx;keysAddress=context.effective.R9;
        if(!Read(context.effective.Rsp,stack.data(),sizeof(stack)))return false;
        returnAddress=stack[0];valuesAddress=stack[5];hostHandleAddress=stack[6];domainIdAddress=stack[7];
        const SIZE_T extent=static_cast<SIZE_T>(propertyCount)*sizeof(DWORD64);
        if(!Read(keysAddress,pointers_[0].get(),extent) || !Read(valuesAddress,pointers_[1].get(),extent))return false;
        if(!String(exePath) || !String(friendlyName))return false;
        for(SIZE_T role=0;role<2;++role)for(SIZE_T i=0;i<static_cast<SIZE_T>(propertyCount);++i)
            if(!String(pointers_[role][i]))return false;
        captured=true;return true; // Byte capture only; never Consume authority.
#else
        (void)exported;return Fail("entry_unsupported_context_architecture");
#endif
    }
    bool RevalidateOriginal() noexcept {
        if(revalidationAttempted)return false;revalidationAttempted=true;
        if(!captured || !SameStoppedOriginal())return Fail("entry_revalidation_occurrence_changed");
#if defined(_M_AMD64) || defined(__x86_64__)
        if(!Read(contextOriginal->effective.Rsp,revalidatedStack.data(),sizeof(revalidatedStack)) ||
            memcmp(stack.data(),revalidatedStack.data(),sizeof(stack)))return Fail("entry_stack_changed_or_unreadable");
        const SIZE_T extent=static_cast<SIZE_T>(propertyCount)*sizeof(DWORD64);
        const DWORD64 addresses[]={keysAddress,valuesAddress};
        for(SIZE_T role=0;role<2;++role) {
            if(!Read(addresses[role],revalidatedPointers_[role].get(),extent) ||
                (extent && memcmp(pointers_[role].get(),revalidatedPointers_[role].get(),extent)))
                return Fail("entry_pointer_array_changed_or_unreadable");
        }
        for(SIZE_T i=0;i<spanCount;++i) {
            auto& span=spans_[i];
            if(!span.terminated || !Read(span.address,revalidatedStrings_.get()+span.offset,span.extent) ||
                memcmp(strings_.get()+span.offset,revalidatedStrings_.get()+span.offset,span.extent))
                return Fail("entry_string_changed_or_unreadable");
            span.revalidated=true;
        }
        revalidated=true;return true; // No immutable-mapping or register-currentness assertion.
#else
        return Fail("entry_unsupported_context_architecture");
#endif
    }
    SIZE_T ReservedSourceBytes() const noexcept{return reservedBytes_;}
    bool RetainedRowBackingExtentsOriginal(SIZE_T& readsBytes,SIZE_T& spansBytes) const noexcept {
        if(pointerCapacity_>(SIZE_MAX-2)/2 || readCount>readCapacity_ || spanCount>2*pointerCapacity_+2 ||
            readCapacity_>SIZE_MAX/sizeof(ReadOriginal)||(2*pointerCapacity_+2)>SIZE_MAX/sizeof(StringOriginal)||
            (readCount&&!reads_)||(spanCount&&!spans_))return false;
        readsBytes=reads_?readCapacity_*sizeof(ReadOriginal):0;
        spansBytes=spans_?(2*pointerCapacity_+2)*sizeof(StringOriginal):0;return true;
    }
    const ReadOriginal* PrivateReads() const noexcept{return reads_.get();}
    const StringOriginal* PrivateStrings() const noexcept{return spans_.get();}
    const unsigned char* PrivateStringBytes() const noexcept{return strings_.get();}
    const unsigned char* PrivateRevalidatedStringBytes() const noexcept{return revalidatedStrings_.get();}
    const DWORD64* PrivatePointers(SIZE_T role) const noexcept{return role<2?pointers_[role].get():nullptr;}
    const DWORD64* PrivateRevalidatedPointers(SIZE_T role) const noexcept{return role<2?revalidatedPointers_[role].get():nullptr;}
    SIZE_T PrivatePointerCapacity() const noexcept{return pointerCapacity_;}
    SIZE_T PrivateStringCapacity() const noexcept{return stringCapacity_;}
    SIZE_T PrivateReadCapacity() const noexcept{return readCapacity_;}
public:
    bool OutputBackingSeparateOriginal(const void* output,std::size_t bytes) const noexcept {
        if(!OriginalRetainedStorageSeparate(output,bytes,this,1,sizeof(*this))||pointerCapacity_>(SIZE_MAX-2)/2)return false;
        for(SIZE_T i=0;i<2;++i)if(!OriginalRetainedArraySeparate(output,bytes,pointers_[i],pointerCapacity_)||
            !OriginalRetainedArraySeparate(output,bytes,revalidatedPointers_[i],pointerCapacity_))return false;
        return OriginalRetainedArraySeparate(output,bytes,strings_,stringCapacity_)&&OriginalRetainedArraySeparate(output,bytes,revalidatedStrings_,stringCapacity_)&&
            OriginalRetainedArraySeparate(output,bytes,spans_,2*pointerCapacity_+2)&&OriginalRetainedArraySeparate(output,bytes,reads_,readCapacity_);
    }
};
}

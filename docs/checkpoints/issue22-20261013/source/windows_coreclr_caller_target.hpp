// UNCOMPILED original native CALL-target observations. A matching target is not
// proof of executed CALL, selected CPU semantics, source ABI or alias exclusion.
#pragma once
#include "windows_amd64_caller_boundary.hpp"
#include "windows_coreclr_entry_inputs.hpp"
#include "windows_coreclr_context_change.hpp"
namespace template_creator {
class OriginalCoreClrCallerTarget {
public:
    struct ReadOriginal {
        HANDLE process=nullptr;const void* address=nullptr;void* destination=nullptr;SIZE_T requested=0;
        SIZE_T actual=0;BOOL returned=FALSE;DWORD error=0;
        template_creator_bridge::OriginalWindowsPendingCall* pendingOriginal=nullptr;
        bool attempted=false,returnedObserved=false;
    };
private:
    std::array<ReadOriginal,6> reads_{};
    bool Fail(const char* reason) noexcept {failed=true;if(!firstFailure)firstFailure=reason;return false;}
    bool SameStop() const noexcept {
        return entryOriginal && entryOriginal->stateOriginal && entryOriginal->stoppedOriginal &&
            entryOriginal->contextOriginal && entryOriginal->contextOriginal->threadCreateOriginal &&
            OriginalStoppedThreadOccurrence(*entryOriginal->stateOriginal,*entryOriginal->stoppedOriginal)==
                entryOriginal->contextOriginal->threadCreateOriginal;
    }
    bool Read(DWORD64 address,void* destination,SIZE_T extent) noexcept {
        if(!SameStop() || !extent || !address || address>UINTPTR_MAX || extent-1>UINTPTR_MAX-address ||
            readCount==reads_.size())return Fail("caller_native_read_original_unresolved");
        auto& row=reads_[readCount++];row.process=entryOriginal->processOriginal;
        row.address=reinterpret_cast<const void*>(static_cast<std::uintptr_t>(address));row.destination=destination;row.requested=extent;
        row.pendingOriginal=entryOriginal->pendingOriginal->BeginOriginal(this,&row,destination,extent,
            entryOriginal->commandOriginal,offsetof(ReadOriginal,actual));
        if(!row.pendingOriginal)return Fail("caller_native_read_pending_unavailable");
        row.attempted=true;row.returned=ReadProcessMemory(row.process,row.address,row.destination,row.requested,&row.actual);
        if(!row.returned)row.error=GetLastError();row.returnedObserved=true;
        if(!entryOriginal->pendingOriginal->ReturnedOriginal(*row.pendingOriginal,row.returned,row.error,row.actual))
            return Fail("caller_native_read_completion_unknown");
        return row.returned && row.actual==row.requested ? true:Fail("caller_native_read_failed_or_partial");
    }
    bool DispatchExecutableObserved(const LoadedImageObservation& image,DWORD64 address,SIZE_T extent) const noexcept {
        const auto base=reinterpret_cast<std::uintptr_t>(image.nativeBase);
        if(address<base || !image.regions || image.regionCount>image.regionCapacity)return false;
        const auto rva=address-base;const MemoryRegion* found=nullptr;
        for(SIZE_T i=0;i<image.regionCount;++i) {
            const auto& row=image.regions[i];
            if(row.queryReturned!=sizeof(row.original) || !row.readSucceeded || rva<row.dataOffset ||
                rva-row.dataOffset>row.readReturned || extent>row.readReturned-(rva-row.dataOffset))continue;
            if(found)return false;found=&row;
        }
        if(!found || found->original.State!=MEM_COMMIT || found->original.Type!=MEM_IMAGE ||
            found->original.AllocationBase!=image.nativeBase || (found->original.Protect&(PAGE_GUARD|PAGE_NOACCESS)))return false;
        const DWORD access=found->original.Protect&0xff;
        return access==PAGE_EXECUTE || access==PAGE_EXECUTE_READ || access==PAGE_EXECUTE_READWRITE || access==PAGE_EXECUTE_WRITECOPY;
    }
    bool ObserveDispatchCell(const LoadedImageObservation& image) noexcept {
        auto copy=[&](SIZE_T offset,void* destination,SIZE_T extent) noexcept {
            if(offset>image.imageSize || extent>image.imageSize-offset)return false;
            memcpy(destination,image.bytes.get()+offset,extent);return true;
        };
        IMAGE_DOS_HEADER dos{};IMAGE_NT_HEADERS64 nt{};
        if(!copy(0,&dos,sizeof(dos)) || dos.e_magic!=IMAGE_DOS_SIGNATURE || dos.e_lfanew<0 ||
            !copy(static_cast<SIZE_T>(dos.e_lfanew),&nt,sizeof(nt)) || nt.Signature!=IMAGE_NT_SIGNATURE ||
            nt.FileHeader.Machine!=IMAGE_FILE_MACHINE_AMD64 || nt.OptionalHeader.Magic!=IMAGE_NT_OPTIONAL_HDR64_MAGIC ||
            nt.FileHeader.SizeOfOptionalHeader<sizeof(IMAGE_OPTIONAL_HEADER64) ||
            static_cast<SIZE_T>(dos.e_lfanew)>image.imageSize ||
            offsetof(IMAGE_NT_HEADERS64,OptionalHeader)>image.imageSize-static_cast<SIZE_T>(dos.e_lfanew) ||
            nt.FileHeader.SizeOfOptionalHeader>image.imageSize-static_cast<SIZE_T>(dos.e_lfanew)-offsetof(IMAGE_NT_HEADERS64,OptionalHeader) ||
            nt.OptionalHeader.SizeOfImage!=image.imageSize ||
            nt.OptionalHeader.NumberOfRvaAndSizes<=IMAGE_DIRECTORY_ENTRY_LOAD_CONFIG ||
            !(nt.OptionalHeader.DllCharacteristics&IMAGE_DLLCHARACTERISTICS_GUARD_CF))
            return Fail("caller_CFG_PE_metadata_unavailable");
        const auto directory=nt.OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_LOAD_CONFIG];
        static_assert(offsetof(IMAGE_LOAD_CONFIG_DIRECTORY64,GuardCFDispatchFunctionPointer)==120);
        static_assert(offsetof(IMAGE_LOAD_CONFIG_DIRECTORY64,GuardFlags)==144);
        constexpr SIZE_T required=offsetof(IMAGE_LOAD_CONFIG_DIRECTORY64,GuardFlags)+sizeof(DWORD);
        DWORD size=0;
        if(!directory.VirtualAddress || directory.Size<required || directory.VirtualAddress>image.imageSize ||
            directory.Size>image.imageSize-directory.VirtualAddress ||
            !copy(directory.VirtualAddress,&size,sizeof(size)) || size<required || size>directory.Size ||
            !copy(SIZE_T(directory.VirtualAddress)+offsetof(IMAGE_LOAD_CONFIG_DIRECTORY64,GuardFlags),&dispatchGuardFlags,sizeof(dispatchGuardFlags)) ||
            !copy(SIZE_T(directory.VirtualAddress)+offsetof(IMAGE_LOAD_CONFIG_DIRECTORY64,GuardCFDispatchFunctionPointer),&dispatchCellOriginal,sizeof(dispatchCellOriginal)) ||
            !(dispatchGuardFlags&IMAGE_GUARD_CF_INSTRUMENTED) ||
            !(dispatchGuardFlags&IMAGE_GUARD_CF_FUNCTION_TABLE_PRESENT) || dispatchCellOriginal!=cellAddress)
            return Fail("caller_CFG_cell_not_original_load_config_dispatch");
        dispatchMetadataObserved=true;return true;
    }
    static bool AddSigned(DWORD64 base,std::int64_t displacement,DWORD64& result) noexcept {
        if(displacement>=0) {
            const auto amount=static_cast<DWORD64>(displacement);if(amount>UINT64_MAX-base)return false;result=base+amount;
        } else {
            // Only signed 32-bit displacements enter this method.
            if(displacement<INT32_MIN)return false;const auto amount=static_cast<DWORD64>(-displacement);
            if(amount>base)return false;result=base-amount;
        }
        return result<=UINTPTR_MAX;
    }
#if defined(_M_AMD64) || defined(__x86_64__)
    static bool Register(const CONTEXT& context,unsigned ordinal,DWORD64& value) noexcept {
        switch(ordinal) {
        case 0:value=context.Rax;break;case 1:value=context.Rcx;break;case 2:value=context.Rdx;break;case 3:value=context.Rbx;break;
        case 4:if(context.Rsp>UINT64_MAX-8)return false;value=context.Rsp+8;break; // CALL evaluates operand before pushing return.
        case 5:value=context.Rbp;break;case 6:value=context.Rsi;break;case 7:value=context.Rdi;break;
        case 8:value=context.R8;break;case 9:value=context.R9;break;case 10:value=context.R10;break;case 11:value=context.R11;break;
        case 12:value=context.R12;break;case 13:value=context.R13;break;case 14:value=context.R14;break;case 15:value=context.R15;break;
        default:return false;
        }
        return true;
    }
#endif
public:
    const OriginalCoreClrEntryInputs* entryOriginal=nullptr;
    const OriginalAmd64CallerBoundary* boundaryOriginal=nullptr;
    OriginalCoreClrThreadContext beforeOriginal,afterOriginal;
    std::array<unsigned char,15> instructionOriginal{},instructionRevalidated{};
    DWORD64 instructionAddress=0,cellAddress=0,targetOriginal=0,targetRevalidated=0;
    DWORD64 dispatchCellOriginal=0,resolvedCalleeOriginal=0;DWORD dispatchGuardFlags=0;
    std::array<unsigned char,2> dispatchInstructionOriginal{},dispatchInstructionRevalidated{};
    bool dispatchMetadataObserved=false,defaultDispatchObserved=false;
    SIZE_T readCount=0;
    bool attempted=false,failed=false,instructionMatched=false,targetMatched=false;
    const char* firstFailure=nullptr;
    bool Capture(const OriginalCoreClrEntryInputs& entry,const OriginalAmd64CallerBoundary& boundary) noexcept {
        if(attempted)return false;attempted=true;entryOriginal=&entry;boundaryOriginal=&boundary;
#if defined(_M_AMD64) || defined(__x86_64__)
        if(!entry.captured || !entry.revalidated || entry.failed || !entry.commandOriginal || !entry.pendingOriginal ||
            !entry.contextOriginal || !entry.contextOriginal->threadCreateOriginal || !entry.exportOriginal ||
            !boundary.boundaryObserved || boundary.failed || !boundary.callOriginal || !boundary.imageOriginal ||
            !SameStop())return Fail("caller_entry_boundary_original_unresolved");
        const auto& image=*boundary.imageOriginal;const auto& call=*boundary.callOriginal;
        if(image.retainedProcess!=entry.processOriginal || !entry.exportOriginal->imageOriginal ||
            !same_time(image.birth,entry.exportOriginal->imageOriginal->birth) ||
            !call.length || call.length>instructionOriginal.size() || call.rva>image.imageSize ||
            call.length>image.imageSize-call.rva ||
            !AddSigned(reinterpret_cast<std::uintptr_t>(image.nativeBase),call.rva,instructionAddress) ||
            instructionAddress>UINT64_MAX-call.length || instructionAddress+call.length!=entry.returnAddress)
            return Fail("caller_native_image_occurrence_unresolved");
        const auto& state=*entry.stateOriginal;const auto& stopped=*entry.stoppedOriginal;
        const auto& born=*entry.contextOriginal->threadCreateOriginal;
        if(!beforeOriginal.Capture(state,stopped,*entry.pendingOriginal,*entry.commandOriginal,&born) ||
            !OriginalCoreClrContextChange::CompleteContext(beforeOriginal.effective) ||
            !OriginalCoreClrContextChange::GeneralEqual(beforeOriginal.effective,entry.contextOriginal->effective) ||
            !OriginalCoreClrContextChange::DebugEqual(beforeOriginal.effective,entry.contextOriginal->effective))
            return Fail("caller_entry_registers_changed");
        if(!Read(instructionAddress,instructionOriginal.data(),call.length) ||
            memcmp(instructionOriginal.data(),image.bytes.get()+call.rva,call.length))return Fail("caller_instruction_changed");
        bool cell=false;
        if(call.opcode==0xe8 && !call.opcode2) {
            if(!AddSigned(entry.returnAddress,call.relative,targetOriginal))return Fail("caller_relative_target_width");
            targetRevalidated=targetOriginal;
        } else if(call.opcode==0xff && !call.opcode2 && call.hasModrm && ((call.modrm>>3)&7)==2 && !(call.rex&4)) {
            if(call.registerOperand) {
                if(call.rex&2 || !Register(beforeOriginal.effective,(call.modrm&7)|((call.rex&1)?8:0),targetOriginal))
                    return Fail("caller_register_target_unsupported");
                targetRevalidated=targetOriginal;
            } else if(call.ripRelative && !(call.rex&3)) {
                if(!AddSigned(entry.returnAddress,call.displacement,cellAddress) ||
                    cellAddress<reinterpret_cast<std::uintptr_t>(image.nativeBase) ||
                    cellAddress-reinterpret_cast<std::uintptr_t>(image.nativeBase)>image.imageSize ||
                    sizeof(targetOriginal)>image.imageSize-(cellAddress-reinterpret_cast<std::uintptr_t>(image.nativeBase)) ||
                    !Read(cellAddress,&targetOriginal,sizeof(targetOriginal)))return Fail("caller_indirect_cell_original_unavailable");
                cell=true;
            } else return Fail("caller_memory_operand_or_thunk_unsupported");
        } else return Fail("caller_call_form_unsupported");
        if(targetOriginal!=reinterpret_cast<std::uintptr_t>(entry.exportOriginal->address)) {
            // Microsoft PE metadata specifies the default AMD64 CFG dispatcher
            // as JMP RAX. This exact supported body preserves target/arguments;
            // an OS-replaced dispatcher needs its own actual native body proof.
            if(!cell || !ObserveDispatchCell(image) ||
                beforeOriginal.effective.Rax!=reinterpret_cast<std::uintptr_t>(entry.exportOriginal->address) ||
                targetOriginal<reinterpret_cast<std::uintptr_t>(image.nativeBase) ||
                targetOriginal-reinterpret_cast<std::uintptr_t>(image.nativeBase)>image.imageSize ||
                dispatchInstructionOriginal.size()>image.imageSize-(targetOriginal-reinterpret_cast<std::uintptr_t>(image.nativeBase)))
                return Fail("caller_CFG_dispatch_body_original_unresolved");
            if(!DispatchExecutableObserved(image,targetOriginal,dispatchInstructionOriginal.size()) ||
                !Read(targetOriginal,dispatchInstructionOriginal.data(),dispatchInstructionOriginal.size()) ||
                memcmp(dispatchInstructionOriginal.data(),image.bytes.get()+(targetOriginal-reinterpret_cast<std::uintptr_t>(image.nativeBase)),dispatchInstructionOriginal.size()) ||
                dispatchInstructionOriginal[0]!=0xff || dispatchInstructionOriginal[1]!=0xe0)
                return Fail("caller_CFG_OS_dispatch_semantics_unresolved");
            if(!Read(targetOriginal,dispatchInstructionRevalidated.data(),dispatchInstructionRevalidated.size()) ||
                dispatchInstructionOriginal!=dispatchInstructionRevalidated)
                return Fail("caller_CFG_dispatch_body_changed");
            defaultDispatchObserved=true;resolvedCalleeOriginal=beforeOriginal.effective.Rax;
        }
        if(!defaultDispatchObserved)resolvedCalleeOriginal=targetOriginal;
        if(cell && !Read(cellAddress,&targetRevalidated,sizeof(targetRevalidated)))return false;
        if(targetRevalidated!=targetOriginal || !Read(instructionAddress,instructionRevalidated.data(),call.length) ||
            memcmp(instructionOriginal.data(),instructionRevalidated.data(),call.length))return Fail("caller_target_or_instruction_changed");
        if(!afterOriginal.Capture(state,stopped,*entry.pendingOriginal,*entry.commandOriginal,&born) ||
            !OriginalCoreClrContextChange::CompleteContext(afterOriginal.effective) ||
            !OriginalCoreClrContextChange::GeneralEqual(afterOriginal.effective,beforeOriginal.effective) ||
            !OriginalCoreClrContextChange::DebugEqual(afterOriginal.effective,beforeOriginal.effective))
            return Fail("caller_entry_register_revalidation_changed");
        instructionMatched=true;targetMatched=true;return true; // Observation only; not Consume.
#else
        (void)boundary;return Fail("caller_context_architecture_unsupported");
#endif
    }
    const ReadOriginal* PrivateReads() const noexcept{return reads_.data();}
};
}
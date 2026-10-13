// UNCOMPILED original instruction/next-address capture. A predicted address is
// not an executed transition, source/CPU/ABI admission or Consume permission.
#pragma once
#include "windows_coreclr_debug_cause.hpp"
#include "windows_amd64_caller_boundary.hpp"
#include "windows_coreclr_compiled_geometry.hpp"
namespace template_creator {
struct OriginalCoreClrInstructionStep {
    struct ReadOriginal {
        HANDLE process=nullptr;const void* address=nullptr;void* destination=nullptr;SIZE_T requested=0;
        SIZE_T actual=0;BOOL returned=FALSE;DWORD error=0;
        template_creator_bridge::OriginalWindowsPendingCall* pendingOriginal=nullptr;
        bool attempted=false,returnedObserved=false;
    };
    const OriginalCoreClrDebugCause* causeOriginal=nullptr;
    const LoadedImageObservation* imageOriginal=nullptr;
    const OriginalCoreClrCompiledGeometry* geometryOriginal=nullptr;
    OriginalCoreClrThreadContext before,after;
    OriginalAmd64Instruction instruction{};
    std::array<ReadOriginal,6> reads{};
    std::array<unsigned char,15> originalBytes{},revalidatedBytes{};
    DWORD64 originalCell=0,revalidatedCell=0,cellAddress=0,nextAddress=0;
    std::array<DWORD64,7> callStack{},revalidatedCallStack{};
    bool callStackCaptured=false;
    DWORD64 originalReturnTarget=0,revalidatedReturnTarget=0,predictedReturnRsp=0;
    unsigned returnImmediate=0;
    bool returnStackCaptured=false;
    SIZE_T readCount=0;
    bool attempted=false,predicted=false,failed=false;
    const char* firstFailure=nullptr;
private:
    bool Fail(const char* reason) noexcept {failed=true;if(!firstFailure)firstFailure=reason;return false;}
    bool Read(DWORD64 address,void* destination,SIZE_T extent) noexcept {
        if(!causeOriginal || !causeOriginal->contextOriginal || !causeOriginal->stateOriginal ||
            !causeOriginal->stoppedOriginal || !extent || !address || address>UINTPTR_MAX ||
            extent-1>UINTPTR_MAX-address || readCount==reads.size())return Fail("instruction_read_shape");
        const auto& context=*causeOriginal->contextOriginal;
        const auto& state=*causeOriginal->stateOriginal;
        const Event* born=context.threadCreateOriginal;
        if(!born || !OriginalStoppedThreadOccurrenceFor(state,*causeOriginal->stoppedOriginal,*born) ||
            born->createdThreadMemberOrdinal>=state.memberCount || !context.pendingOwnerOriginal || !context.commandOriginal)
            return Fail("instruction_read_original_owner");
        const auto processOrdinal=state.members[born->createdThreadMemberOrdinal].process;
        if(processOrdinal>=state.resourceCount)return Fail("instruction_process_original");
        const auto& process=state.resources[processOrdinal];
        if(process.kind!=Kind::Process || !process.acquired || process.unknownOutput || process.attempted)
            return Fail("instruction_process_custody");
        auto& row=reads[readCount++];row.process=process.original;
        row.address=reinterpret_cast<const void*>(static_cast<std::uintptr_t>(address));
        row.destination=destination;row.requested=extent;
        row.pendingOriginal=context.pendingOwnerOriginal->BeginOriginal(this,&row,destination,extent,
            context.commandOriginal,offsetof(ReadOriginal,actual));
        if(!row.pendingOriginal)return Fail("instruction_read_pending");
        row.attempted=true;row.returned=ReadProcessMemory(row.process,row.address,row.destination,row.requested,&row.actual);
        if(!row.returned)row.error=GetLastError();row.returnedObserved=true;
        if(!context.pendingOwnerOriginal->ReturnedOriginal(*row.pendingOriginal,row.returned,row.error,row.actual))
            return Fail("instruction_read_unknown");
        return row.returned && row.actual==extent?true:Fail("instruction_read_failed_or_partial");
    }
    static bool Add(DWORD64 base,std::int64_t displacement,DWORD64& result) noexcept {
        if(displacement>=0) {
            const auto amount=static_cast<DWORD64>(displacement);
            if(amount>UINTPTR_MAX-base)return false;result=base+amount;
        } else {
            if(displacement<INT32_MIN)return false;
            const auto amount=static_cast<DWORD64>(-displacement);
            if(amount>base)return false;result=base-amount;
        }
        return result && result<=UINTPTR_MAX;
    }
#if defined(_M_AMD64) || defined(__x86_64__)
    static bool Register(const CONTEXT& c,unsigned ordinal,DWORD64& value) noexcept {
        switch(ordinal) {
        case 0:value=c.Rax;break;case 1:value=c.Rcx;break;case 2:value=c.Rdx;break;case 3:value=c.Rbx;break;
        case 4:value=c.Rsp;break;case 5:value=c.Rbp;break;case 6:value=c.Rsi;break;case 7:value=c.Rdi;break;
        case 8:value=c.R8;break;case 9:value=c.R9;break;case 10:value=c.R10;break;case 11:value=c.R11;break;
        case 12:value=c.R12;break;case 13:value=c.R13;break;case 14:value=c.R14;break;case 15:value=c.R15;break;
        default:return false;
        }return true;
    }
    static bool Condition(unsigned code,DWORD flags) noexcept {
        const bool c=flags&1,p=flags&(1u<<2),z=flags&(1u<<6),s=flags&(1u<<7),o=flags&(1u<<11);
        switch(code&15) {
        case 0:return o;case 1:return !o;case 2:return c;case 3:return !c;
        case 4:return z;case 5:return !z;case 6:return c||z;case 7:return !c&&!z;
        case 8:return s;case 9:return !s;case 10:return p;case 11:return !p;
        case 12:return s!=o;case 13:return s==o;case 14:return z||(s!=o);default:return !z&&(s==o);
        }
    }
#endif
public:
    bool Capture(const OriginalCoreClrDebugCause& cause,const LoadedImageObservation& image,
            const OriginalCoreClrCompiledGeometry& geometry) noexcept {
        if(attempted)return false;attempted=true;causeOriginal=&cause;imageOriginal=&image;geometryOriginal=&geometry;
#if defined(_M_AMD64) || defined(__x86_64__)
        if(!cause.ownedOnlyObserved || !cause.stateOriginal || !cause.stoppedOriginal || !cause.contextOriginal ||
            !cause.pairOriginal || !cause.contextOriginal->pendingOwnerOriginal || !cause.contextOriginal->commandOriginal)
            return Fail("instruction_cause_original_missing");
        const auto& original=*cause.contextOriginal;
        const auto& state=*cause.stateOriginal;const auto& stopped=*cause.stoppedOriginal;
        if(!original.threadCreateOriginal || !before.Capture(state,stopped,*original.pendingOwnerOriginal,
            *original.commandOriginal,original.threadCreateOriginal) ||
            !OriginalCoreClrContextChange::CompleteContext(before.effective) ||
            !OriginalCoreClrContextChange::GeneralEqual(before.effective,original.effective) ||
            !OriginalCoreClrContextChange::DebugEqual(before.effective,original.effective) ||
            !cause.pairOriginal->OwnsPair(before.effective))return Fail("instruction_context_changed_before_read");
        const DWORD64 rip=before.effective.Rip,base=reinterpret_cast<std::uintptr_t>(image.nativeBase);
        if(!image.nativeBase || rip<base || rip-base>UINT32_MAX ||
            !OriginalAmd64CallerBoundary::DecodeRetainedImageInstruction(image,static_cast<DWORD>(rip-base),instruction) ||
            !geometry.MatchesMappedCodeSpan(image,instruction.rva,instruction.length))
            return Fail("instruction_current_image_decode_unsupported");
        if(instruction.length>UINTPTR_MAX-rip)return Fail("instruction_next_width");
        const DWORD64 fallthrough=rip+instruction.length;
        if(!Read(rip,originalBytes.data(),instruction.length) ||
            memcmp(originalBytes.data(),image.bytes.get()+instruction.rva,instruction.length))
            return Fail("instruction_native_source_bytes_differ");
        if(instruction.flow==OriginalAmd64Instruction::Flow::Call &&
            !Read(before.effective.Rsp,callStack.data(),sizeof(callStack)))return false;
        nextAddress=fallthrough;
        if(instruction.flow==OriginalAmd64Instruction::Flow::Return){
            if(instruction.rex||(instruction.opcode!=0xc3&&instruction.opcode!=0xc2)||
               (instruction.opcode==0xc3&&instruction.length!=1)||
               (instruction.opcode==0xc2&&instruction.length!=3))
                return Fail("instruction_near_return_profile_unsupported");
            if(instruction.opcode==0xc2)
                returnImmediate=unsigned(originalBytes[1])|(unsigned(originalBytes[2])<<8);
            if(before.effective.Rsp>UINTPTR_MAX-8||returnImmediate>UINTPTR_MAX-(before.effective.Rsp+8))
                return Fail("instruction_return_stack_width");
            predictedReturnRsp=before.effective.Rsp+8+returnImmediate;
            if(!Read(before.effective.Rsp,&originalReturnTarget,sizeof(originalReturnTarget))||
               !originalReturnTarget||originalReturnTarget>UINTPTR_MAX)
                return Fail("instruction_return_target_original");
            nextAddress=originalReturnTarget;
        }
        if(instruction.flow==OriginalAmd64Instruction::Flow::Conditional) {
            const unsigned code=instruction.opcode==0x0f?instruction.opcode2:instruction.opcode;
            if(Condition(code,before.effective.EFlags) && !Add(fallthrough,instruction.relative,nextAddress))
                return Fail("instruction_conditional_target_width");
        } else if(instruction.flow==OriginalAmd64Instruction::Flow::Jump ||
                  instruction.flow==OriginalAmd64Instruction::Flow::Call) {
            if(instruction.opcode!=0xff) {
                if(!Add(fallthrough,instruction.relative,nextAddress))return Fail("instruction_relative_target_width");
            } else if(instruction.registerOperand) {
                const unsigned ordinal=(instruction.modrm&7)|((instruction.rex&1)?8:0);
                if(!Register(before.effective,ordinal,nextAddress) || !nextAddress || nextAddress>UINTPTR_MAX)
                    return Fail("instruction_register_target_width");
            } else if(instruction.ripRelative && !(instruction.rex&1)) {
                if(!Add(fallthrough,instruction.displacement,cellAddress) ||
                    !Read(cellAddress,&originalCell,sizeof(originalCell)) || !originalCell || originalCell>UINTPTR_MAX)
                    return Fail("instruction_indirect_cell_original");
                nextAddress=originalCell;
            } else return Fail("instruction_indirect_address_profile_unsupported");
        }
        if(!Read(rip,revalidatedBytes.data(),instruction.length) ||
            memcmp(originalBytes.data(),revalidatedBytes.data(),instruction.length))
            return Fail("instruction_native_bytes_changed");
        if(cellAddress && (!Read(cellAddress,&revalidatedCell,sizeof(revalidatedCell)) || originalCell!=revalidatedCell))
            return Fail("instruction_native_cell_changed");
        if(instruction.flow==OriginalAmd64Instruction::Flow::Call) {
            if(!Read(before.effective.Rsp,revalidatedCallStack.data(),sizeof(revalidatedCallStack)) ||
                memcmp(callStack.data(),revalidatedCallStack.data(),sizeof(callStack)))
                return Fail("instruction_call_stack_changed");
            callStackCaptured=true;
        }
        if(instruction.flow==OriginalAmd64Instruction::Flow::Return){
            if(!Read(before.effective.Rsp,&revalidatedReturnTarget,sizeof(revalidatedReturnTarget))||
               originalReturnTarget!=revalidatedReturnTarget)
                return Fail("instruction_return_target_changed");
            returnStackCaptured=true;
        }
        if(!after.Capture(state,stopped,*original.pendingOwnerOriginal,*original.commandOriginal,original.threadCreateOriginal) ||
            !OriginalCoreClrContextChange::CompleteContext(after.effective) ||
            !OriginalCoreClrContextChange::GeneralEqual(before.effective,after.effective) ||
            !OriginalCoreClrContextChange::DebugEqual(before.effective,after.effective))
            return Fail("instruction_context_changed_after_read");
        predicted=true;return true;
#else
        return Fail("instruction_architecture_unsupported");
#endif
    }
};
}

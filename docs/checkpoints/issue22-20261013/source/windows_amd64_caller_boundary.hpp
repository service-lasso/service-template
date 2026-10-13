// UNCOMPILED closed AMD64 instruction-boundary projection over retained bytes.
// This is not a full disassembler, CPU admission, CALL execution observation,
// source/build relationship or target-memory immutability authority.
#pragma once
#include "windows_coreclr_export.hpp"
#include <cstdint>
#include <memory>
#include <stdexcept>
#include <cstring>
#include <exception>
namespace template_creator {
// PE AMD64 exception-table record: three little-endian DWORD RVAs.
struct OriginalAmd64RuntimeFunction {DWORD BeginAddress=0,EndAddress=0,UnwindData=0;};
static_assert(sizeof(OriginalAmd64RuntimeFunction)==12);
struct OriginalAmd64Instruction {
    enum class Flow {Next,Conditional,Jump,Call,Return};
    DWORD rva=0;
    unsigned length=0;
    Flow flow=Flow::Next;
    std::int64_t relative=0;
    unsigned opcode=0,opcode2=0,rex=0,modrm=0;
    bool hasModrm=false,ripRelative=false,registerOperand=false;
    std::int32_t displacement=0;
};
class OriginalAmd64CallerBoundary {
    std::unique_ptr<OriginalAmd64Instruction[]> instructions_;
    std::unique_ptr<DWORD[]> work_;
    SIZE_T capacity_=0,count_=0,workCount_=0,workRead_=0;
    const unsigned char* bytes_=nullptr;
    SIZE_T imageSize_=0;
    OriginalAmd64CallerBoundary()=default; // Nonallocating local decode view only.
    bool Span(SIZE_T start,SIZE_T extent) const noexcept{return start<=imageSize_ && extent<=imageSize_-start;}
    template<class T> bool Copy(SIZE_T start,T& value) const noexcept {
        if(!Span(start,sizeof(value)))return false;memcpy(&value,bytes_+start,sizeof(value));return true;
    }
    bool ExecutableObserved(SIZE_T rva,SIZE_T extent) const noexcept {
        if(!imageOriginal || !imageOriginal->regions || imageOriginal->regionCount>imageOriginal->regionCapacity)return false;
        const MemoryRegion* found=nullptr;
        for(SIZE_T i=0;i<imageOriginal->regionCount;++i) {
            const auto& row=imageOriginal->regions[i];
            if(row.queryReturned!=sizeof(row.original) || !row.readSucceeded || rva<row.dataOffset ||
                rva-row.dataOffset>row.readReturned || extent>row.readReturned-(rva-row.dataOffset))continue;
            if(found)return false;found=&row;
        }
        if(!found || found->original.State!=MEM_COMMIT || found->original.Type!=MEM_IMAGE ||
            found->original.AllocationBase!=imageOriginal->nativeBase ||
            (found->original.Protect&(PAGE_GUARD|PAGE_NOACCESS)))return false;
        const DWORD access=found->original.Protect&0xff;
        return access==PAGE_EXECUTE || access==PAGE_EXECUTE_READ || access==PAGE_EXECUTE_READWRITE || access==PAGE_EXECUTE_WRITECOPY;
    }
    bool Fail(const char* reason) noexcept {if(!firstFailure)firstFailure=reason;failed=true;return false;}
    bool Enqueue(DWORD rva) noexcept {
        if(rva<functionOriginal.BeginAddress || rva>=functionOriginal.EndAddress)return Fail("caller_branch_outside_function");
        for(SIZE_T i=0;i<workCount_;++i)if(work_[i]==rva)return true;
        if(workCount_==capacity_){if(!overflowPresent){overflowPresent=true;overflowRva=rva;}return Fail("caller_control_flow_geometry");}work_[workCount_++]=rva;return true;
    }
    bool RelativeTarget(const OriginalAmd64Instruction& row,DWORD& target) noexcept {
        const std::int64_t next=std::int64_t(row.rva)+row.length;
        const std::int64_t address=next+row.relative;
        if(address<0 || address>UINT32_MAX)return Fail("caller_relative_target_width");
        target=static_cast<DWORD>(address);return true;
    }
    // Strict selected subset. Unknown prefix/opcode/form never supplies a
    // guessed length. All operands are bounded before inspecting their bytes.
    bool Decode(DWORD rva,OriginalAmd64Instruction& row) noexcept {
        row={};row.rva=rva;SIZE_T at=rva;
        auto take=[&](SIZE_T n) noexcept {
            if(n>15 || at-rva>15-n || at>functionOriginal.EndAddress || n>functionOriginal.EndAddress-at)return false;
            at+=n;return true;
        };
        auto byte=[&](unsigned& value) noexcept {if(!take(1))return false;value=bytes_[at-1];return true;};
        unsigned op=0;if(!byte(op))return false;
        // Only a single REX prefix is selected; 66/67/segment/LOCK/REP and
        // VEX/EVEX/APX need separate source qualification, not silent decoding.
        if(op>=0x40 && op<=0x4f){row.rex=op;if(!byte(op))return false;}
        row.opcode=op;
        bool modrm=false;SIZE_T immediate=0;bool relative=false;bool signedByte=false;
        if(op==0x0f) {
            unsigned second=0;if(!byte(second))return false;row.opcode2=second;
            if(second>=0x80 && second<=0x8f){row.flow=OriginalAmd64Instruction::Flow::Conditional;immediate=4;relative=true;}
            else if((second>=0x90 && second<=0x9f)||second==0x1f||second==0xaf||
                second==0xb6||second==0xb7||second==0xbe||second==0xbf||
                second==0x10||second==0x11||second==0x28||second==0x29||second==0x2e||second==0x2f||second==0x57||
                second==0xa3||second==0xba)modrm=true;
            else return false;
        } else if(op>=0x50 && op<=0x5f) {} // PUSH/POP register, no length guess.
        else if(op==0x90 || op==0xc9) {}
        else if(op==0xc3){row.flow=OriginalAmd64Instruction::Flow::Return;}
        else if(op==0xc2){row.flow=OriginalAmd64Instruction::Flow::Return;immediate=2;}
        else if(op>=0xb0 && op<=0xb7)immediate=1;
        else if(op>=0xb8 && op<=0xbf)immediate=(row.rex&8)?8:4;
        else if(op>=0x70 && op<=0x7f){row.flow=OriginalAmd64Instruction::Flow::Conditional;immediate=1;relative=true;signedByte=true;}
        else if(op==0xe8 || op==0xe9 || op==0xeb) {
            row.flow=op==0xe8?OriginalAmd64Instruction::Flow::Call:OriginalAmd64Instruction::Flow::Jump;
            immediate=op==0xeb?1:4;relative=true;signedByte=op==0xeb;
        } else if(op==0xa8)immediate=1;
        else if(op==0xa9 || op==0x05 || op==0x0d || op==0x15 || op==0x1d ||
            op==0x25 || op==0x2d || op==0x35 || op==0x3d)immediate=4;
        else if((op<=0x3b && (op&7)<=3) || op==0x63 || op==0x84 || op==0x85 ||
            op==0x88 || op==0x89 || op==0x8a || op==0x8b || op==0x8d || op==0x69 || op==0x6b ||
            op==0x80 || op==0x81 || op==0x83 || op==0xc0 || op==0xc1 || op==0xc6 || op==0xc7 ||
            op==0xd0 || op==0xd1 || op==0xd2 || op==0xd3 || op==0xf6 || op==0xf7 || op==0xfe || op==0xff)modrm=true;
        else return false;
        if(modrm) {
            unsigned m=0;if(!byte(m))return false;row.hasModrm=true;row.modrm=m;
            const unsigned mode=m>>6,reg=(m>>3)&7,rm=m&7;
            row.registerOperand=mode==3;
            // Intel SDM BT 0F A3 /r; BTR 0F BA /6 ib. This selected
            // register-only subset supplies length, not emulated flags or
            // native execution authority. REX.R extends BT's operand register;
            // it cannot turn a BA opcode extension into another instruction.
            if(op==0x0f && row.opcode2==0xa3 && mode!=3)return false;
            if(op==0x0f && row.opcode2==0xba) {
                if(mode!=3 || reg!=6 || (row.rex&4))return false;
                immediate=1;
            }
            if(op==0x0f && row.opcode2==0x1f && (reg || (row.rex&4)))return false; // Selected documented NOP /0.
            // Intel SETcc explicitly ignores ModR/M.reg; it is not an opcode extension.
            if((op==0xc0 || op==0xc1 || (op>=0xd0 && op<=0xd3)) && reg==6)return false;
            if(op==0x8d && mode==3)return false;
            if(op==0xff) {
                if(row.rex&4)return false;
                if(reg==2)row.flow=OriginalAmd64Instruction::Flow::Call;
                else if(reg==4)row.flow=OriginalAmd64Instruction::Flow::Jump;
                else if(reg!=0 && reg!=1 && reg!=6)return false;
            }
            if(op==0xfe && reg>1)return false;
            if((op==0xc6 || op==0xc7) && reg)return false;
            if(op==0x69 || op==0x81 || op==0xc7)immediate=4;
            if(op==0x6b || op==0x80 || op==0x83 || op==0xc0 || op==0xc1 || op==0xc6)immediate=1;
            if((op==0xf6 || op==0xf7) && reg<=1) {
                if(reg==1)return false;immediate=op==0xf6?1:4;
            }
            SIZE_T displacement=0;
            if(mode!=3 && rm==4) {
                unsigned sib=0;if(!byte(sib))return false;
                if(mode==0 && (sib&7)==5)displacement=4;
            }
            if(mode==0 && rm==5){displacement=4;row.ripRelative=true;}
            if(mode==1)displacement=1;
            if(mode==2)displacement=4;
            const SIZE_T start=at;if(!take(displacement))return false;
            if(displacement==4)memcpy(&row.displacement,bytes_+start,4);
            else if(displacement==1){std::int8_t d=0;memcpy(&d,bytes_+start,1);row.displacement=d;}
        }
        const SIZE_T immediateAt=at;if(!take(immediate))return false;
        if(relative) {
            if(signedByte){std::int8_t d=0;memcpy(&d,bytes_+immediateAt,1);row.relative=d;}
            else {std::int32_t d=0;memcpy(&d,bytes_+immediateAt,4);row.relative=d;}
        }
        row.length=static_cast<unsigned>(at-rva);return row.length && row.length<=15;
    }
public:
    const LoadedImageObservation* imageOriginal=nullptr;
    OriginalAmd64RuntimeFunction functionOriginal{};
    SIZE_T functionTableOrdinal=SIZE_MAX;
    DWORD returnRva=0,overflowRva=0;
    bool overflowPresent=false;
    const OriginalAmd64Instruction* callOriginal=nullptr;
    bool attempted=false,failed=false,boundaryObserved=false;
    const char* firstFailure=nullptr;
    static SIZE_T CheckedSourceBytes(SIZE_T rows) {
        if(!rows || rows>(SIZE_MAX-sizeof(OriginalAmd64CallerBoundary))/(sizeof(OriginalAmd64Instruction)+sizeof(DWORD)))
            throw std::invalid_argument("original AMD64 caller decode geometry");
        return sizeof(OriginalAmd64CallerBoundary)+rows*(sizeof(OriginalAmd64Instruction)+sizeof(DWORD));
    }
    bool preownAttempted=false,preowned=false;
    std::exception_ptr preownExceptionOriginal;
    explicit OriginalAmd64CallerBoundary(SIZE_T rows):capacity_(rows) {}
    bool PreownOriginal() noexcept {
        if(preownAttempted)return false;preownAttempted=true;
        try {
        const SIZE_T rows=capacity_;
        (void)CheckedSourceBytes(rows);
        instructions_=std::make_unique<OriginalAmd64Instruction[]>(rows);work_=std::make_unique<DWORD[]>(rows);
        preowned=true;return true;
        }catch(...){preownExceptionOriginal=std::current_exception();return false;}
    }
    // Explicit supplied-byte regression seam. It cannot set boundaryObserved,
    // source/CPU authority, native result rows or a caller-target match.
    static bool DecodeSuppliedInstructionContract(const unsigned char* bytes,SIZE_T extent,OriginalAmd64Instruction& row) {
        if(!bytes || !extent || extent>UINT32_MAX)return false;
        OriginalAmd64CallerBoundary supplied;supplied.bytes_=bytes;supplied.imageSize_=extent;
        supplied.functionOriginal.BeginAddress=0;supplied.functionOriginal.EndAddress=static_cast<DWORD>(extent);
        return supplied.Decode(0,row);
    }
    // Exact current instruction decoding over an already captured image. This
    // does not establish root reachability, source/native identity or executed
    // control flow. The controller owns those independent native observations.
    static bool DecodeRetainedImageInstruction(const LoadedImageObservation& image,DWORD rva,
            OriginalAmd64Instruction& row) noexcept {
        if(!image.complete || image.failed || !image.bytes || !image.nativeBase ||
            !image.imageSize || image.imageSize>UINT32_MAX || image.used!=image.imageSize ||
            image.retainedByteExtent<image.imageSize || image.imageSize>image.byteCapacity || rva>=image.imageSize)return false;
        OriginalAmd64CallerBoundary view;view.imageOriginal=&image;view.bytes_=image.bytes.get();view.imageSize_=image.imageSize;
        view.functionOriginal.EndAddress=static_cast<DWORD>(image.imageSize);
        return view.Decode(rva,row) && view.ExecutableObserved(rva,row.length);
    }
    bool Observe(const LoadedImageObservation& image,DWORD64 originalReturnAddress) noexcept {
        if(!preowned)return false;
        if(attempted)return false;attempted=true;imageOriginal=&image;
        if(!image.complete || image.failed || !image.nativeBase || !image.bytes || !image.imageSize || image.used!=image.imageSize ||
            image.retainedByteExtent<image.imageSize || image.imageSize>image.byteCapacity)return Fail("caller_image_incomplete");
        bytes_=image.bytes.get();imageSize_=image.imageSize;
        const auto base=reinterpret_cast<std::uintptr_t>(image.nativeBase);
        if(originalReturnAddress<base || originalReturnAddress-base>=imageSize_ || originalReturnAddress-base>UINT32_MAX)
            return Fail("caller_return_address_outside_image");
        returnRva=static_cast<DWORD>(originalReturnAddress-base);
        IMAGE_DOS_HEADER dos{};IMAGE_NT_HEADERS64 nt{};
        if(!Copy(0,dos) || dos.e_magic!=IMAGE_DOS_SIGNATURE || dos.e_lfanew<0 ||
            !Copy(static_cast<SIZE_T>(dos.e_lfanew),nt) || nt.Signature!=IMAGE_NT_SIGNATURE ||
            nt.FileHeader.Machine!=IMAGE_FILE_MACHINE_AMD64 || nt.OptionalHeader.Magic!=IMAGE_NT_OPTIONAL_HDR64_MAGIC ||
            nt.FileHeader.SizeOfOptionalHeader<sizeof(IMAGE_OPTIONAL_HEADER64) ||
            nt.OptionalHeader.NumberOfRvaAndSizes<=IMAGE_DIRECTORY_ENTRY_EXCEPTION)return Fail("caller_PE_unwind_shape");
        const auto table=nt.OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXCEPTION];
        if(!table.VirtualAddress || table.VirtualAddress%4 || !table.Size || table.Size%sizeof(OriginalAmd64RuntimeFunction) ||
            !Span(table.VirtualAddress,table.Size))return Fail("caller_function_table_extent");
        DWORD previous=0;bool found=false;
        for(SIZE_T i=0;i<table.Size/sizeof(OriginalAmd64RuntimeFunction);++i) {
            OriginalAmd64RuntimeFunction row{};
            if(!Copy(SIZE_T(table.VirtualAddress)+i*sizeof(row),row) || row.BeginAddress>=row.EndAddress ||
                row.EndAddress>imageSize_ || (i && row.BeginAddress<=previous))return Fail("caller_function_table_order");
            previous=row.BeginAddress;
            if(returnRva>row.BeginAddress && returnRva<row.EndAddress) {
                if(found)return Fail("caller_function_range_ambiguous");found=true;functionOriginal=row;functionTableOrdinal=i;
            }
        }
        if(!found || functionOriginal.UnwindData%4 || !Span(functionOriginal.UnwindData,4))return Fail("caller_function_boundary_unavailable");
        const unsigned unwind=bytes_[functionOriginal.UnwindData];
        if((unwind&7)!=1 || ((unwind>>3)&~7u) || ((unwind>>3)&4u))return Fail("caller_unwind_profile_unsupported");
        if(!Enqueue(functionOriginal.BeginAddress))return false;
        while(workRead_<workCount_) {
            DWORD current=work_[workRead_++];
            for(;;) {
                bool existing=false;
                for(SIZE_T i=0;i<count_;++i) {
                    const auto& prior=instructions_[i];
                    if(current==prior.rva){existing=true;break;}
                    if(current>prior.rva && current<prior.rva+prior.length)return Fail("caller_branch_inside_instruction");
                }
                if(existing)break;
                if(count_==capacity_){if(!overflowPresent){overflowPresent=true;overflowRva=current;}return Fail("caller_instruction_geometry");}
                auto& row=instructions_[count_];
                if(!Decode(current,row))return Fail("caller_instruction_profile_unsupported");
                if(!ExecutableObserved(row.rva,row.length))return Fail("caller_instruction_not_observed_executable");
                for(SIZE_T i=0;i<count_;++i) {
                    const auto& prior=instructions_[i];
                    if(row.rva<prior.rva+prior.length && prior.rva<row.rva+row.length)return Fail("caller_instruction_overlap");
                }
                ++count_;const DWORD next=row.rva+row.length;
                if(row.flow==OriginalAmd64Instruction::Flow::Call && next==returnRva) {
                    if(callOriginal)return Fail("caller_call_boundary_ambiguous");callOriginal=&row;
                }
                if(row.flow==OriginalAmd64Instruction::Flow::Return)break;
                if(row.flow==OriginalAmd64Instruction::Flow::Conditional || row.flow==OriginalAmd64Instruction::Flow::Jump) {
                    if(row.opcode==0xff)return Fail("caller_indirect_jump_reachability_unresolved");
                    DWORD target=0;if(!RelativeTarget(row,target) || !Enqueue(target))return false;
                    if(row.flow==OriginalAmd64Instruction::Flow::Jump)break;
                }
                if(next>=functionOriginal.EndAddress)return Fail("caller_function_fallthrough_unresolved");current=next;
            }
        }
        if(!callOriginal)return Fail("caller_return_not_supported_call_boundary");
        boundaryObserved=true;return true; // No assertion that a CALL actually executed.
    }
    SIZE_T ReservedSourceBytes() const noexcept {
        return sizeof(*this)+(instructions_?capacity_*sizeof(OriginalAmd64Instruction):0)+
            (work_?capacity_*sizeof(DWORD):0);
    }
    const OriginalAmd64Instruction* PrivateInstructions() const noexcept{return instructions_.get();}
    SIZE_T PrivateAllocationCapacity() const noexcept{return capacity_;}
    SIZE_T PrivateInstructionCount() const noexcept{return count_;}
    const DWORD* PrivateWork() const noexcept{return work_.get();}
    SIZE_T PrivateWorkCount() const noexcept{return workCount_;}
};
}

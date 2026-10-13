// UNCOMPILED source observation, not kernel interpreter/loader admission.
#pragma once
#include "linux_held_file.hpp"
#include <elf.h>
namespace template_creator_linux {
struct OriginalLinuxElfInterpreterSource {
    const OriginalLinuxHeldFile* fileOriginal=nullptr;
    Elf64_Ehdr header{};
    Elf64_Phdr firstInterpreter{};
    size_t interpreterOrdinal=SIZE_MAX,interpreterCount=0,loadCount=0,pathBytes=0;
    bool attempted=false,headerCopied=false,tableBounded=false,projected=false;
    bool unsupportedExtendedCount=false,interpreterAfterLoad=false;
    bool Capture(const OriginalLinuxHeldFile& original) noexcept {
        if(attempted)return false;attempted=true;fileOriginal=&original;
        if(!original.CapturedSourceBytes()||original.RetainedByteCount()<sizeof(header))return false;
        const auto* bytes=original.PrivateBody();const size_t extent=original.RetainedByteCount();
        std::memcpy(&header,bytes,sizeof(header));headerCopied=true;
        if(std::memcmp(header.e_ident,ELFMAG,SELFMAG)||header.e_ident[EI_CLASS]!=ELFCLASS64||
           header.e_ident[EI_DATA]!=ELFDATA2LSB||header.e_ident[EI_VERSION]!=EV_CURRENT||
           header.e_version!=EV_CURRENT||header.e_machine!=EM_X86_64||
           (header.e_type!=ET_EXEC&&header.e_type!=ET_DYN)||header.e_ehsize!=sizeof(header)||
           header.e_phentsize!=sizeof(Elf64_Phdr))return false;
        if(header.e_phnum==PN_XNUM){unsupportedExtendedCount=true;return false;}
        if(!header.e_phnum||header.e_phoff>extent||
           header.e_phnum>(extent-static_cast<size_t>(header.e_phoff))/sizeof(Elf64_Phdr))return false;
        tableBounded=true;
        for(size_t i=0;i<header.e_phnum;++i){
            Elf64_Phdr row{};
            std::memcpy(&row,bytes+static_cast<size_t>(header.e_phoff)+i*sizeof(row),sizeof(row));
            if(row.p_type==PT_LOAD){
                ++loadCount;
                if(row.p_offset>extent||row.p_filesz>extent-static_cast<size_t>(row.p_offset)||
                   row.p_filesz>row.p_memsz||row.p_memsz>UINT64_MAX-row.p_vaddr)return false;
            }
            if(row.p_type!=PT_INTERP)continue;
            ++interpreterCount;
            if(interpreterOrdinal!=SIZE_MAX)continue; // Retain duplicates, never infer singleton.
            firstInterpreter=row;interpreterOrdinal=i;interpreterAfterLoad=loadCount!=0;
            if(row.p_offset>extent||!row.p_filesz||row.p_filesz>extent-static_cast<size_t>(row.p_offset))return false;
            const auto* path=bytes+static_cast<size_t>(row.p_offset);
            while(pathBytes<row.p_filesz&&path[pathBytes])++pathBytes;
            if(pathBytes==row.p_filesz)return false;
        }
        projected=loadCount!=0;return projected;
    }
    const char* PrivateFirstInterpreterPath() const noexcept {
        if(!projected||interpreterOrdinal==SIZE_MAX||!fileOriginal||
           !fileOriginal->CapturedSourceBytes())return nullptr;
        return reinterpret_cast<const char*>(fileOriginal->PrivateBody()+static_cast<size_t>(firstInterpreter.p_offset));
    }
    bool OriginalProgramHeader(size_t ordinal,Elf64_Phdr& row) const noexcept {
        if(!projected||!tableBounded||!fileOriginal||!fileOriginal->CapturedSourceBytes()||ordinal>=header.e_phnum)return false;
        const size_t extent=fileOriginal->RetainedByteCount();
        if(header.e_phoff>extent||ordinal>(extent-static_cast<size_t>(header.e_phoff))/sizeof(row))return false;
        const size_t offset=static_cast<size_t>(header.e_phoff)+ordinal*sizeof(row);
        if(offset>extent||sizeof(row)>extent-offset)return false;
        std::memcpy(&row,fileOriginal->PrivateBody()+offset,sizeof(row));return true;
    }
    bool FileSpanCovered(uint64_t bias,uint64_t begin,uint64_t end) const noexcept {
        return LoadSpanCovered(bias,begin,end,false);
    }
    bool MemorySpanCovered(uint64_t bias,uint64_t begin,uint64_t end) const noexcept {
        return LoadSpanCovered(bias,begin,end,true);
    }
    bool LoadSpanCovered(uint64_t bias,uint64_t begin,uint64_t end,bool memoryExtent) const noexcept {
        if(begin>=end)return false;uint64_t cursor=begin;
        for(size_t pass=0;pass<header.e_phnum&&cursor<end;++pass){
            uint64_t next=cursor;
            for(size_t ph=0;ph<header.e_phnum;++ph){
                Elf64_Phdr row{};if(!OriginalProgramHeader(ph,row))return false;
                const uint64_t extent=memoryExtent?row.p_memsz:row.p_filesz;
                if(row.p_type!=PT_LOAD||!extent)continue;
                if(row.p_vaddr>UINT64_MAX-bias)return false;
                const uint64_t first=bias+row.p_vaddr;
                if(extent>UINT64_MAX-first)return false;
                const uint64_t last=first+extent;
                if(first<=cursor&&cursor<last&&last>next)next=last;
            }
            if(next==cursor)return false;cursor=next<end?next:end;
        }
        return cursor==end; // Source-selected union only, never a native mapping grant.
    }
    bool UniqueExecutableEntryFileOffset(uint64_t& offset) const noexcept {
        if(!projected||!fileOriginal||!fileOriginal->CapturedSourceBytes())return false;
        size_t matches=0;uint64_t candidate=0;
        for(size_t i=0;i<header.e_phnum;++i){
            Elf64_Phdr row{};
            std::memcpy(&row,fileOriginal->PrivateBody()+static_cast<size_t>(header.e_phoff)+i*sizeof(row),sizeof(row));
            if(row.p_type!=PT_LOAD||!(row.p_flags&PF_X)||header.e_entry<row.p_vaddr)continue;
            const uint64_t delta=header.e_entry-row.p_vaddr;
            if(delta>=row.p_filesz)continue;
            if(delta>UINT64_MAX-row.p_offset)return false;
            candidate=row.p_offset+delta;++matches;
        }
        if(matches!=1)return false;offset=candidate;return true;
    }
    // Exact actual kernel source/profile decides duplicate/order/PATH_MAX and
    // extended forms. This observation supplies no lookup or mapped-image proof.
};
}

// UNCOMPILED source geometry only. This never authenticates an expectation.
#pragma once
#ifndef _WIN32
#include "linux_original_route_arguments.hpp"
#include "upstream-cli/bq_provider_expectations.h"
#include <cstring>
namespace template_creator_bridge {
struct OriginalLinuxSourceOccurrencePreflight {
    const slcli_bq_provider_expectations* expectations=nullptr;
    const OriginalLinuxRouteSelection* route=nullptr;
    size_t attemptedSubject=SIZE_MAX,comparedSubjects=0;
    bool attempted=false,missing=false,matched=false;
    static bool Unused(const slcli_bq_expected_occurrence_selector& s) noexcept {
        return s.phase==UINT32_MAX&&s.command==UINT32_MAX&&s.requirement==UINT32_MAX;
    }
    static bool SameSelector(const slcli_bq_expected_occurrence_selector& a,
        const slcli_bq_expected_occurrence_selector& b) noexcept {
        return a.phase==b.phase&&a.command==b.command&&a.requirement==b.requirement;
    }
    const slcli_bq_expected_subject* Earlier(const slcli_bq_expected_occurrence_selector& selector,
        size_t current) const noexcept {
        if(!expectations||Unused(selector)||selector.phase!=static_cast<uint32_t>(route->phase))return nullptr;
        const slcli_bq_expected_subject* found=nullptr;
        for(size_t i=0;i<current;++i){const auto& row=expectations->subjects[i];
            if(row.phase==selector.phase&&row.command==selector.command&&row.requirement==selector.requirement){
                if(found)return nullptr;found=&row;
            }
        }
        return found;
    }
    bool ObserveOriginal(const slcli_bq_provider_expectations* source,const OriginalLinuxRouteSelection& selected,
        size_t commands) noexcept {
        if(attempted)return false;attempted=true;expectations=source;route=&selected;missing=!source;
        if(!source||!commands||commands>selected.requirements.size()||source->command_count!=commands||
           !source->requirements_per_command||!source->subjects||source->principal.platform!=SLCLI_BQ_EXPECTED_LINUX)return false;
        if(!slcli_bq_provider_expectations_bootstrap_shape(source,static_cast<uint32_t>(selected.phase)))return false;
        if(!source->source_unit_record_count||source->source_unit_record_count>(UINT64_MAX-64)/128||
           source->source_unit_record_count>(SIZE_MAX-64)/128||
           64+128*source->source_unit_record_count>source->source_unit_bytes||source->source_unit_bytes>SIZE_MAX)return false;
        if(!source->association_index_record_count||source->association_index_record_count!=source->source_unit_record_count||
           source->association_index_record_count>(UINT64_MAX-128)/320||
           source->association_index_record_count>(SIZE_MAX-128)/320||
           source->association_index_names_bytes<source->association_index_record_count||
           source->association_index_names_bytes>UINT64_MAX-(128+320*source->association_index_record_count)||
           source->association_index_names_bytes>SIZE_MAX-(128+320*source->association_index_record_count)||
           128+320*source->association_index_record_count+source->association_index_names_bytes!=source->association_index_bytes||
           source->association_index_bytes>SIZE_MAX)return false;
        size_t total=0;
        for(size_t c=0;c<commands;++c){
            if(selected.requirements[c].size()>UINT32_MAX||
               source->requirements_per_command[c]!=selected.requirements[c].size()||
               selected.requirements[c].size()>SIZE_MAX-total)return false;
            total+=selected.requirements[c].size();
        }
        if(total!=source->subject_count)return false;
        size_t index=0;
        for(size_t c=0;c<commands;++c)for(size_t r=0;r<selected.requirements[c].size();++r,++index){
            attemptedSubject=index;const auto& expected=source->subjects[index];
            const auto& selectedInput=selected.requirements[c][r];const auto& occurrence=expected.source_occurrence;
            if(!FileRequirement(selectedInput)){
                const auto* writer=std::get_if<NativeArchiveFoundationPrerequisite>(&selectedInput);
                const auto* reader=std::get_if<NativeArchiveReadbackPrerequisite>(&selectedInput);
                if((!writer&&!reader)||c||r!=(writer?0:1)||
                   occurrence.reader_class!=(writer?SLCLI_BQ_EXPECTED_ARCHIVE_FOUNDATION_PREREQUISITE:SLCLI_BQ_EXPECTED_ARCHIVE_READBACK_PREREQUISITE)||
                   RequirementNativeExtent(selectedInput)!=occurrence.native_file_bytes)return false;
                const auto& w=writer?writer->archive_foundation:reader->archive_foundation;
                const auto& rb=writer?writer->archive_readback:reader->archive_readback;
                if(!SameSelector(w,occurrence.archive_foundation)||!SameSelector(rb,occurrence.archive_readback))return false;
                ++comparedSubjects;continue;
            }
            const auto& input=*FileRequirement(selectedInput);
            if(expected.phase!=static_cast<uint32_t>(selected.phase)||expected.command!=c||expected.requirement!=r||
               !expected.selector.member||expected.selector.member_bytes!=input.size||
               expected.selector.family<SLCLI_BQ_SUBJECT_SOURCE||expected.selector.family>SLCLI_BQ_SUBJECT_IMAGE||
               !expected.reader_role||expected.reader_role_bytes!=input.role.size()||
               (input.role.size()&&std::memcmp(expected.reader_role,input.role.data(),input.role.size()))||
               std::memcmp(expected.body_digest,input.sha256.data(),input.sha256.size())||
               occurrence.read_bytes!=input.size||occurrence.read_offset>occurrence.native_file_bytes||
               occurrence.read_bytes>occurrence.native_file_bytes-occurrence.read_offset||
               occurrence.native_file_bytes>SIZE_MAX)return false;
            const auto* a=Earlier(occurrence.association_index,index);
            const auto* unit=Earlier(occurrence.source_unit,index);
            const bool aUnused=Unused(occurrence.association_index),unitUnused=Unused(occurrence.source_unit);
            switch(occurrence.reader_class){
            case SLCLI_BQ_EXPECTED_A_PREREQUISITE:
                if(!aUnused||!unitUnused||occurrence.read_offset||occurrence.read_bytes!=occurrence.native_file_bytes||
                   occurrence.native_file_bytes!=source->association_index_bytes)return false;
                break;
            case SLCLI_BQ_EXPECTED_UNIT_PREREQUISITE:
                if(!a||a->source_occurrence.reader_class!=SLCLI_BQ_EXPECTED_A_PREREQUISITE||!unitUnused||
                   occurrence.native_file_bytes!=source->source_unit_bytes||
                   occurrence.read_offset||occurrence.read_bytes!=occurrence.native_file_bytes)return false;
                break;
            case SLCLI_BQ_EXPECTED_SP1_MEMBER_RANGE:
                if(!a||!unit||a->source_occurrence.reader_class!=SLCLI_BQ_EXPECTED_A_PREREQUISITE||
                   expected.source_record_index>=source->source_unit_record_count||
                   unit->source_occurrence.reader_class!=SLCLI_BQ_EXPECTED_UNIT_PREREQUISITE||
                   !SameSelector(unit->source_occurrence.association_index,occurrence.association_index)||
                   unit->source_occurrence.native_file_bytes!=occurrence.native_file_bytes)return false;
                break;
            case SLCLI_BQ_EXPECTED_LOOSE_NATIVE_SUBJECT:
                if(occurrence.read_offset||occurrence.read_bytes!=occurrence.native_file_bytes||
                   expected.source_record_index>=source->source_unit_record_count)return false;
                if(!a||!unit||a->source_occurrence.reader_class!=SLCLI_BQ_EXPECTED_A_PREREQUISITE||
                   unit->source_occurrence.reader_class!=SLCLI_BQ_EXPECTED_UNIT_PREREQUISITE||
                   !SameSelector(unit->source_occurrence.association_index,occurrence.association_index))return false;
                break;
            default:return false;
            }
            ++comparedSubjects;
        }
        matched=comparedSubjects==source->subject_count;return matched;
        // Geometry/reader-role/digest equality never establishes authentic A,
        // native principal, canonical source membership or a financing original.
    }
};
}
#endif

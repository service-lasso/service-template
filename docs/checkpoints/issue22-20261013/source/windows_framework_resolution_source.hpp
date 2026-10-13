// UNCOMPILED conditional framework resolver source, not native membership or
// installed-runtime admission. Primary case: dotnet/runtime v10.0.5
// src/native/corehost/fx_reference.cpp and fxr/fx_resolver.cpp.
#pragma once
#include "windows_framework_list_source.hpp"
namespace template_creator {
struct OriginalFrameworkReferenceSource {
    enum class Range {Exact,Patch,Minor,Major};
    enum class Outcome {Unobserved,SourceUnresolved,ConditionalSourceReady};
    const OriginalFrameworkListSource::Row* original=nullptr;
    const OriginalFrameworkVersionSource* version=nullptr;
    Range range=Range::Exact;
    bool applyPatches=false,rollHighest=false,preferRelease=false;
    Outcome outcome=Outcome::Unobserved;
    static OriginalFrameworkReferenceSource Observe(const OriginalFrameworkListSource::Row& row) noexcept {
        OriginalFrameworkReferenceSource r;r.original=&row;r.version=&row.nameVersion.versionSource;
        using Completion=OriginalFrameworkListSource::Completion::Outcome;
        if(!row.accepted||row.included||row.completion.outcome!=Completion::ConditionalSourceReturnedTrue||
           !row.completion.preferReleaseKnown||r.version->outcome!=OriginalFrameworkVersionSource::Outcome::SourceParsed){
            r.outcome=Outcome::SourceUnresolved;return r;
        }
        const int roll=row.completion.effectiveRoll;
        if(roll<0||roll>5||(!row.settings.applyAssigned&&!row.settings.defaultApplyKnown)){
            r.outcome=Outcome::SourceUnresolved;return r;
        }
        constexpr Range ranges[]={Range::Exact,Range::Patch,Range::Minor,Range::Minor,Range::Major,Range::Major};
        r.range=ranges[roll];r.rollHighest=roll==3||roll==5;
        r.applyPatches=row.settings.applyAssigned?row.settings.applyValue:row.settings.defaultApply;
        r.preferRelease=row.completion.preferRelease;r.outcome=Outcome::ConditionalSourceReady;return r;
    }
};
struct OriginalFrameworkHigherCompatibility {
    enum class Outcome {Unobserved,SourceUnresolved,LowerCandidatePrecondition,
        ConditionalSourceReturnedFalse,ConditionalSourceReturnedTrue};
    const OriginalFrameworkReferenceSource* reference=nullptr;
    const OriginalFrameworkVersionSource* candidate=nullptr;
    OriginalFrameworkVersionComparison comparison;
    Outcome outcome=Outcome::Unobserved;
    static OriginalFrameworkHigherCompatibility Observe(const OriginalFrameworkReferenceSource& reference,
        const OriginalFrameworkVersionSource& candidate) noexcept {
        OriginalFrameworkHigherCompatibility r;r.reference=&reference;r.candidate=&candidate;
        if(reference.outcome!=OriginalFrameworkReferenceSource::Outcome::ConditionalSourceReady||!reference.version){
            r.outcome=Outcome::SourceUnresolved;return r;
        }
        r.comparison=OriginalFrameworkVersionComparison::Observe(*reference.version,candidate);
        if(r.comparison.outcome!=OriginalFrameworkVersionComparison::Outcome::SourceCompared){
            r.outcome=Outcome::SourceUnresolved;return r;
        }
        if(r.comparison.order>0){r.outcome=Outcome::LowerCandidatePrecondition;return r;}
        if(!r.comparison.order){r.outcome=Outcome::ConditionalSourceReturnedTrue;return r;}
        using Range=OriginalFrameworkReferenceSource::Range;
        const auto& requested=*reference.version;
        // These checks follow fx_reference_t, including minor comparison when
        // major differs and the special patch/applyPatches=false condition.
        if((requested.components[0]!=candidate.components[0]&&reference.range<Range::Major)||
           (requested.components[1]!=candidate.components[1]&&reference.range<Range::Minor)||
           (requested.components[2]!=candidate.components[2]&&reference.range==Range::Patch&&!reference.applyPatches)||
           reference.range==Range::Exact){
            r.outcome=Outcome::ConditionalSourceReturnedFalse;return r;
        }
        // Release preference requires the full original candidate search;
        // this pairwise source function must not invent a release-only filter.
        r.outcome=Outcome::ConditionalSourceReturnedTrue;return r;
    }
};
class OriginalFrameworkCandidateSearch {
public:
    enum class Outcome {Unobserved,SourceUnresolved,ConditionalNoMatch,ConditionalMatch};
    struct Row {
        enum class Phase {Best,Compatibility,Winner,PatchFloor,PatchWinner};
        Phase phase=Phase::Best;
        size_t candidate=SIZE_MAX;
        bool releaseOnly=false;
        OriginalFrameworkVersionComparison comparison;
        OriginalFrameworkHigherCompatibility compatibility;
    };
private:
    const OriginalFrameworkReferenceSource& reference_;
    std::unique_ptr<const OriginalFrameworkVersionSource*[]> candidates_;
    std::unique_ptr<Row[]> rows_;
    size_t count_=0,capacity_=0,used_=0;
    bool attempted_=false;
    Outcome outcome_=Outcome::Unobserved;
    size_t selected_=SIZE_MAX;
    Row* Next(size_t candidate,Row::Phase phase,bool releaseOnly) noexcept {
        if(used_==capacity_){outcome_=Outcome::SourceUnresolved;return nullptr;}
        auto& row=rows_[used_++];row.candidate=candidate;row.phase=phase;row.releaseOnly=releaseOnly;return &row;
    }
    bool Compare(size_t candidate,Row::Phase phase,bool releaseOnly,
        const OriginalFrameworkVersionSource& left,const OriginalFrameworkVersionSource& right,int& order) noexcept {
        auto* row=Next(candidate,phase,releaseOnly);if(!row)return false;
        row->comparison=OriginalFrameworkVersionComparison::Observe(left,right);
        if(row->comparison.outcome!=OriginalFrameworkVersionComparison::Outcome::SourceCompared){
            outcome_=Outcome::SourceUnresolved;return false;
        }
        order=row->comparison.order;return true;
    }
    bool Pass(bool releaseOnly,size_t& best) noexcept {
        using Range=OriginalFrameworkReferenceSource::Range;
        best=SIZE_MAX;
        if(reference_.range<Range::Patch)return true;
        const bool highest=reference_.range!=Range::Patch&&reference_.rollHighest;
        for(size_t i=0;i<count_;++i){
            const auto& version=*candidates_[i];
            if(releaseOnly&&version.prerelease)continue;
            int order=0;
            if(!Compare(i,Row::Phase::Best,releaseOnly,version,*reference_.version,order))return false;
            if(order<0)continue;
            auto* compatibility=Next(i,Row::Phase::Compatibility,releaseOnly);if(!compatibility)return false;
            compatibility->compatibility=OriginalFrameworkHigherCompatibility::Observe(reference_,version);
            using Compatible=OriginalFrameworkHigherCompatibility::Outcome;
            if(compatibility->compatibility.outcome==Compatible::ConditionalSourceReturnedFalse)continue;
            if(compatibility->compatibility.outcome!=Compatible::ConditionalSourceReturnedTrue){
                outcome_=Outcome::SourceUnresolved;return false;
            }
            if(best==SIZE_MAX){best=i;continue;}
            if(!Compare(i,Row::Phase::Winner,releaseOnly,version,*candidates_[best],order))return false;
            if(highest?order>0:order<0)best=i;
        }
        if(best==SIZE_MAX||candidates_[best]->prerelease)return true;
        // automatic_roll_to_latest_patch keeps its original start version
        // constant even while the selected best is advanced through the list.
        const auto& floor=*candidates_[best];
        for(size_t i=0;i<count_;++i){
            const auto& version=*candidates_[i];
            if((releaseOnly&&version.prerelease)||
               (!reference_.applyPatches&&version.components[2]!=floor.components[2]))continue;
            int order=0;
            if(!Compare(i,Row::Phase::PatchFloor,releaseOnly,version,floor,order))return false;
            if(order<0||version.components[0]!=floor.components[0]||version.components[1]!=floor.components[1])continue;
            if(!Compare(i,Row::Phase::PatchWinner,releaseOnly,version,*candidates_[best],order))return false;
            if(order>=0)best=i; // std::max(ver, best) retains the first operand on equality.
        }
        return true;
    }
public:
    OriginalFrameworkCandidateSearch(const OriginalFrameworkReferenceSource& reference,
        const OriginalFrameworkVersionSource* const* originals,size_t count):reference_(reference),count_(count){
        if((count&&!originals)||count>(SIZE_MAX-4)/10||count>SIZE_MAX/sizeof(*originals))
            throw std::invalid_argument("framework search original geometry");
        capacity_=count*10+4;
        if(capacity_>SIZE_MAX/sizeof(Row))throw std::invalid_argument("framework search row geometry");
        candidates_=std::make_unique<const OriginalFrameworkVersionSource*[]>(count);
        rows_=std::make_unique<Row[]>(capacity_);
        for(size_t i=0;i<count;++i)candidates_[i]=originals[i];
    }
    OriginalFrameworkCandidateSearch(const OriginalFrameworkCandidateSearch&)=delete;
    OriginalFrameworkCandidateSearch& operator=(const OriginalFrameworkCandidateSearch&)=delete;
    Outcome ObserveCompleteSourceList() noexcept {
        if(attempted_)return outcome_;attempted_=true;
        if(reference_.outcome!=OriginalFrameworkReferenceSource::Outcome::ConditionalSourceReady||!reference_.version){
            outcome_=Outcome::SourceUnresolved;return outcome_;
        }
        for(size_t i=0;i<count_;++i)if(!candidates_[i]||
            candidates_[i]->outcome!=OriginalFrameworkVersionSource::Outcome::SourceParsed){
            outcome_=Outcome::SourceUnresolved;return outcome_;
        }
        if(reference_.preferRelease){
            if(!Pass(true,selected_))return outcome_;
            if(selected_!=SIZE_MAX){outcome_=Outcome::ConditionalMatch;return outcome_;}
        }
        if(!Pass(false,selected_))return outcome_;
        outcome_=selected_==SIZE_MAX?Outcome::ConditionalNoMatch:Outcome::ConditionalMatch;return outcome_;
    }
    const OriginalFrameworkVersionSource* SelectedConditionalSource() const noexcept {
        return outcome_==Outcome::ConditionalMatch&&selected_<count_?candidates_[selected_]:nullptr;
    }
    const Row* PrivateRows() const noexcept{return rows_.get();}
    size_t RowCount() const noexcept{return used_;}
    bool ReservedSourceBytes(size_t& bytes) const noexcept {
        bytes=sizeof(*this);
        if(count_>(SIZE_MAX-bytes)/sizeof(const OriginalFrameworkVersionSource*))return false;bytes+=count_*sizeof(const OriginalFrameworkVersionSource*);
        if(capacity_>(SIZE_MAX-bytes)/sizeof(Row))return false;bytes+=capacity_*sizeof(Row);return true;
    }
    // Full native membership/disabled names/deps-file existence and cross-hive
    // state are owning resolver obligations. This is a conditional source list,
    // never an authenticated candidate enumeration or loaded provider result.
};
}

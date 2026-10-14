// UNCOMPILED conditional source control flow. This never grants native admission.
#pragma once
#include "windows_runtime_host_configuration.hpp"
namespace template_creator {
class OriginalFrameworkListSource {
public:
    using Host=OriginalRuntimeHostConfiguration;
    struct Completion {
        enum class Outcome {Unobserved,ConditionalSourceReturnedTrue,SourceReturnedFalse,Unresolved};
        Outcome outcome=Outcome::Unobserved;
        const void* originalEnvironment=nullptr;
        const void* originalHelperOptions=nullptr;
        int effectiveRoll=-1;
        unsigned environmentOutcome=0;
        bool preferReleaseKnown=false,preferRelease=false;
    };
    struct Row {
        const OriginalJson* jsonOriginal=nullptr;
        SIZE_T object=SIZE_MAX,duplicateOf=SIZE_MAX;
        Host::FrameworkSettingsSource settings;
        OriginalFrameworkNameVersionSource nameVersion;
        Completion completion;
        bool dev=false,included=false,single=false,accepted=false;
    };
    enum class Outcome {Unobserved,DevInputUnsupplied,SourceReturnedFalse,
        SourceUnresolved,ConditionalSourceReturnedTrue,CapacityFailure};
    struct Stage {
        Host::OrderedOptionSource prefix;
        SIZE_T rowBegin=0,rowEnd=0,stoppedToken=SIZE_MAX;
        Outcome outcome=Outcome::Unobserved;
        bool attempted=false,ignoredDevFalse=false;
    };
private:
    std::unique_ptr<Row[]> rows_;
    SIZE_T capacity_=0,count_=0;
    std::array<Stage,2> stages_;
    bool attempted_=false,dependent_=false;
    unsigned specified_=0;
    Outcome outcome_=Outcome::Unobserved;
    template<class Complete>
    bool Item(Stage& stage,const OriginalJson& json,SIZE_T object,bool dev,
        bool included,bool single,Complete& complete) noexcept {
        if(count_==capacity_){stage.outcome=Outcome::CapacityFailure;return false;}
        const SIZE_T ordinal=count_++;auto& row=rows_[ordinal];
        row.jsonOriginal=&json;row.object=object;row.dev=dev;row.included=included;row.single=single;
        stage.stoppedToken=object;stage.rowEnd=count_;
        if(included){
            row.nameVersion=OriginalFrameworkNameVersionSource::Observe(json,object);
            using Name=OriginalFrameworkNameVersionSource::Outcome;
            if(row.nameVersion.outcome!=Name::SourceReturnedTrue){
                stage.outcome=row.nameVersion.outcome==Name::SourceReturnedFalse?
                    Outcome::SourceReturnedFalse:Outcome::SourceUnresolved;return false;}
        }else {
            row.settings=Host::ObserveFrameworkSettingsSource(json,object,specified_,
                stage.prefix.rollForwardSource,stage.prefix.hasDefaultApplySource,stage.prefix.applyPatchesSource);
            row.nameVersion=row.settings.nameVersion;
            // Record completion even for an earlier failure: prefer-release
            // occurs before later setting conflicts in the actual source.
            row.completion=complete(row.settings);
            if(row.settings.outcome==Host::FrameworkSettingsSource::Outcome::SourceReturnedFalse){
                stage.outcome=Outcome::SourceReturnedFalse;return false;}
            if(row.completion.outcome!=Completion::Outcome::ConditionalSourceReturnedTrue){
                stage.outcome=row.completion.outcome==Completion::Outcome::SourceReturnedFalse?
                    Outcome::SourceReturnedFalse:Outcome::SourceUnresolved;return false;}
        }
        // read_framework_array checks duplicates AFTER parse_framework. The
        // standalone framework push does not perform this duplicate check.
        if(!single)for(SIZE_T prior=0;prior<ordinal;++prior){
            const auto& old=rows_[prior];
            if(old.accepted&&old.included==included&&old.jsonOriginal&&
                OriginalFrameworkNameVersionSource::SameName(*old.jsonOriginal,old.nameVersion.name,
                    json,row.nameVersion.name)){
                row.duplicateOf=prior;stage.outcome=Outcome::SourceReturnedFalse;return false;}
        }
        row.accepted=true;return true;
    }
    template<class Complete>
    void StageSource(Stage& stage,const OriginalJson& json,const Host::OptionStage& options,
        bool dev,const Host::OrderedOptionSource& previous,Complete& complete) noexcept {
        stage.attempted=true;stage.rowBegin=stage.rowEnd=count_;
        auto prefix=options;prefix.framework=prefix.frameworks=prefix.includedFrameworks=SIZE_MAX;
        stage.prefix=Host::ObserveOrderedOptionSource(json,prefix,dev,specified_,dependent_,&previous);
        using Ordered=Host::OrderedOptionSource::Outcome;
        if(stage.prefix.outcome==Ordered::NoOp){stage.outcome=Outcome::ConditionalSourceReturnedTrue;return;}
        if(stage.prefix.outcome!=Ordered::SourceReturnedTrue){
            stage.outcome=stage.prefix.outcome==Ordered::SourceReturnedFalse?
                Outcome::SourceReturnedFalse:Outcome::SourceUnresolved;return;}
        const auto* tokens=json.PrivateTokens();
        if(options.framework!=SIZE_MAX){
            dependent_=true;
            if(!Item(stage,json,options.framework,dev,false,true,complete))return;
        }
        if(options.frameworks!=SIZE_MAX){
            dependent_=true;const SIZE_T list=options.frameworks;stage.stoppedToken=list;
            if(list>=json.Count()||tokens[list].kind!=OriginalJson::Kind::Array){stage.outcome=Outcome::SourceUnresolved;return;}
            for(SIZE_T item=list+1;item<tokens[list].subtreeEnd;item=tokens[item].subtreeEnd)
                if(!Item(stage,json,item,dev,false,false,complete))return;
        }
        if(options.includedFrameworks!=SIZE_MAX){
            const SIZE_T list=options.includedFrameworks;stage.stoppedToken=list;
            if(dependent_){stage.outcome=Outcome::SourceReturnedFalse;return;}
            if(list>=json.Count()||tokens[list].kind!=OriginalJson::Kind::Array){stage.outcome=Outcome::SourceUnresolved;return;}
            for(SIZE_T item=list+1;item<tokens[list].subtreeEnd;item=tokens[item].subtreeEnd)
                if(!Item(stage,json,item,dev,true,false,complete))return;
        }
        stage.stoppedToken=SIZE_MAX;stage.outcome=Outcome::ConditionalSourceReturnedTrue;
    }
public:
    static SIZE_T ReservedSourceBytesFor(SIZE_T capacity){
        if(!capacity||capacity>(SIZE_MAX-sizeof(OriginalFrameworkListSource))/sizeof(Row))
            throw std::invalid_argument("original framework source ledger geometry");
        return sizeof(OriginalFrameworkListSource)+capacity*sizeof(Row);
    }
    explicit OriginalFrameworkListSource(SIZE_T capacity):capacity_(capacity){
        ReservedSourceBytesFor(capacity);rows_=std::make_unique<Row[]>(capacity);
    }
    OriginalFrameworkListSource(const OriginalFrameworkListSource&)=delete;
    OriginalFrameworkListSource& operator=(const OriginalFrameworkListSource&)=delete;
    template<class Complete>
    void Observe(const Host& host,const Host::OrderedOptionSource& seed,Complete complete) noexcept {
        ObserveSourceOriginals(host.PrivateDevConfigSyntax(),host.PrivateDevConfigOriginal().spanObserved,
            host.PrivateDevConfigOriginal().syntaxObserved,host.PrivateDevOptionStage(),
            host.PrivateRuntimeConfigSyntax(),host.PrivateMainOptionStage(),seed,complete);
    }
    // Pure source input interface also used by UNRUN regression. These inputs
    // do not establish held/native provenance; the connected caller above does
    // retain the exact configuration owner separately.
    template<class Complete>
    void ObserveSourceOriginals(const OriginalJson* dev,bool devSpanObserved,bool devSyntaxObserved,
        const Host::OptionStage& devOptions,const OriginalJson& main,const Host::OptionStage& mainOptions,
        const Host::OrderedOptionSource& seed,Complete complete) noexcept {
        if(attempted_)return;attempted_=true;
        if(!dev||!devSpanObserved){outcome_=Outcome::DevInputUnsupplied;return;}
        auto& first=stages_[0];
        if(!devSyntaxObserved){first.attempted=true;first.prefix=seed;first.outcome=Outcome::ConditionalSourceReturnedTrue;}
        else StageSource(first,*dev,devOptions,true,seed,complete);
        if(first.outcome==Outcome::SourceReturnedFalse)first.ignoredDevFalse=true;
        else if(first.outcome!=Outcome::ConditionalSourceReturnedTrue){outcome_=first.outcome;return;}
        StageSource(stages_[1],main,mainOptions,false,first.prefix,complete);
        outcome_=stages_[1].outcome;
    }
    const Row* PrivateRows() const noexcept{return rows_.get();}
    SIZE_T RowCount() const noexcept{return count_;}
    const std::array<Stage,2>& PrivateStages() const noexcept{return stages_;}
    Outcome SourceOutcome() const noexcept{return outcome_;}
    unsigned PrivateSpecifiedMask() const noexcept{return specified_;}
    bool PrivateDependentSource() const noexcept{return dependent_;}
    SIZE_T ReservedSourceBytes() const noexcept{return sizeof(*this)+capacity_*sizeof(Row);}
};
}

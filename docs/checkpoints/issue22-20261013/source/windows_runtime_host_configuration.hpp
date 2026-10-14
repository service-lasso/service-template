// UNCOMPILED retained host configuration projection. These are original
// resolution inputs, never proof of resolved frameworks or managed providers.
#pragma once
#include "windows_original_json.hpp"
#include "windows_held_files.hpp"
#include "windows_framework_version_source.hpp"
namespace template_creator {
class OriginalRuntimeHostConfiguration {
public:
    struct Document {
        const NativeHeldFiles* owner=nullptr;
        SIZE_T ordinal=SIZE_MAX;
        const unsigned char* body=nullptr;
        SIZE_T extent=0;
        bool attempted=false,spanObserved=false,syntaxObserved=false;
    };
    struct Projection {
        SIZE_T runtimeOptions=SIZE_MAX,tfm=SIZE_MAX,framework=SIZE_MAX,frameworks=SIZE_MAX;
        SIZE_T includedFrameworks=SIZE_MAX,rollForward=SIZE_MAX,applyPatches=SIZE_MAX;
        SIZE_T configProperties=SIZE_MAX,additionalProbingPaths=SIZE_MAX;
        SIZE_T runtimeTarget=SIZE_MAX,runtimeTargetName=SIZE_MAX,targets=SIZE_MAX;
        SIZE_T libraries=SIZE_MAX,runtimes=SIZE_MAX;
        SIZE_T namedTarget=SIZE_MAX;
        bool attempted=false,rootShapesObserved=false,namedTargetObserved=false,candidateEnumerationComplete=false;
    };
    struct OptionStage {
        enum class Shape {Unobserved,RootUnsupported,Missing,Null,Object,Other};
        SIZE_T options=SIZE_MAX,configProperties=SIZE_MAX,additionalProbingPaths=SIZE_MAX;
        SIZE_T rollForward=SIZE_MAX,applyPatches=SIZE_MAX,rollForwardOnNoCandidateFx=SIZE_MAX;
        SIZE_T tfm=SIZE_MAX,framework=SIZE_MAX,frameworks=SIZE_MAX,includedFrameworks=SIZE_MAX;
        Shape shape=Shape::Unobserved;
        bool attempted=false;
    };
    static OptionStage ObserveOptionStage(const OriginalJson& json) noexcept {
        OptionStage result;result.attempted=true;
        if(!json.Complete())return result;
        if(!json.Count()||json.PrivateTokens()[0].kind!=OriginalJson::Kind::Object){
            result.shape=OptionStage::Shape::RootUnsupported;return result;
        }
        result.options=json.Field(0,"runtimeOptions");
        if(result.options==SIZE_MAX){result.shape=OptionStage::Shape::Missing;return result;}
        const auto kind=json.PrivateTokens()[result.options].kind;
        if(kind==OriginalJson::Kind::Null){result.shape=OptionStage::Shape::Null;return result;}
        if(kind!=OriginalJson::Kind::Object){result.shape=OptionStage::Shape::Other;return result;}
        result.shape=OptionStage::Shape::Object;
        result.configProperties=json.Field(result.options,"configProperties");
        result.additionalProbingPaths=json.Field(result.options,"additionalProbingPaths");
        result.rollForward=json.Field(result.options,"rollForward");
        result.applyPatches=json.Field(result.options,"applyPatches");
        result.rollForwardOnNoCandidateFx=json.Field(result.options,"rollForwardOnNoCandidateFx");
        result.tfm=json.Field(result.options,"tfm");result.framework=json.Field(result.options,"framework");
        result.frameworks=json.Field(result.options,"frameworks");result.includedFrameworks=json.Field(result.options,"includedFrameworks");
        return result;
    }
    struct LibraryMetadataOccurrence {
        SIZE_T key=SIZE_MAX,value=SIZE_MAX,sha512=SIZE_MAX,serviceable=SIZE_MAX;
        SIZE_T path=SIZE_MAX,hashPath=SIZE_MAX,runtimeStoreManifestName=SIZE_MAX,type=SIZE_MAX;
        bool objectObserved=false;
    };
    enum class AssetKind {Runtime,Native,RuntimeTarget,Resource};
    struct WindowsVersionSource {
        enum class Outcome {Empty,Parsed,Invalid,UnsignedConversionWouldThrow,InvalidOriginal};
        SIZE_T token=SIZE_MAX;
        std::array<std::uint32_t,4> components{UINT32_MAX,UINT32_MAX,UINT32_MAX,UINT32_MAX};
        Outcome outcome=Outcome::Empty;
        bool debugCanonical=true,signedAbiCorrespondenceNeeded=false;
    };
    static WindowsVersionSource ObserveWindowsVersionSource(const OriginalJson& json,SIZE_T token) noexcept {
        WindowsVersionSource result;result.token=token;
        if(token>=json.Count()||json.PrivateTokens()[token].kind!=OriginalJson::Kind::String)return result;
        OriginalJson::OriginalStringCursor cursor;
        if(!json.BeginOriginalString(token,cursor)){result.outcome=WindowsVersionSource::Outcome::InvalidOriginal;return result;}
        std::array<std::uint32_t,4> parsed{UINT32_MAX,UINT32_MAX,UINT32_MAX,UINT32_MAX};
        SIZE_T component=0,digits=0;std::uint32_t value=0;
        bool overflow=false,leadingZero=false,canonical=true;
        for(;;){
            std::uint32_t scalar=0;const auto step=json.NextOriginalString(cursor,scalar);
            if(step==OriginalJson::StringStep::Invalid){result.outcome=WindowsVersionSource::Outcome::InvalidOriginal;return result;}
            const bool end=step==OriginalJson::StringStep::End||scalar==0;
            if(!end&&scalar!=L'.'){
                if(scalar<L'0'||scalar>L'9'){result.outcome=WindowsVersionSource::Outcome::Invalid;return result;}
                if(!digits)leadingZero=scalar==L'0';else if(leadingZero)canonical=false;
                ++digits;const auto digit=scalar-L'0';
                if(value>(UINT32_MAX-digit)/10)overflow=true;
                if(!overflow)value=value*10+digit;
                continue;
            }
            // The fourth component is the complete remaining substring, so
            // an additional dot fails its digit scan before stoul is called.
            if(!end&&component==3){result.outcome=WindowsVersionSource::Outcome::Invalid;return result;}
            if(!digits){
                result.outcome=end&&component==0?WindowsVersionSource::Outcome::Empty:WindowsVersionSource::Outcome::Invalid;
                return result;
            }
            // parse_internal first requires a major/minor separator before
            // attempting even the major unsigned conversion.
            if(end&&component==0){result.outcome=WindowsVersionSource::Outcome::Invalid;return result;}
            // Source-predicted conversion path, NOT an observed CLR exception.
            if(overflow){result.outcome=WindowsVersionSource::Outcome::UnsignedConversionWouldThrow;return result;}
            parsed[component++]=value;
            if(end){
                if(component<2){result.outcome=WindowsVersionSource::Outcome::Invalid;return result;}
                result.components=parsed;result.outcome=WindowsVersionSource::Outcome::Parsed;
                for(SIZE_T i=0;i<component;++i)if(parsed[i]>INT32_MAX){result.signedAbiCorrespondenceNeeded=true;canonical=false;}
                result.debugCanonical=canonical;return result;
            }
            digits=0;value=0;overflow=false;leadingZero=false;
        }
    }
    struct SourceAssetName {
        SIZE_T token=SIZE_MAX,begin=0,extent=0;
        bool observed=false,niSuffixRemoved=false,crtCaseCorrespondenceNeeded=false;
    };
    static SourceAssetName ObserveSourceAssetName(const OriginalJson& json,SIZE_T token) noexcept {
        SourceAssetName result;result.token=token;
        OriginalJson::OriginalStringCursor cursor;
        if(!json.BeginOriginalString(token,cursor))return result;
        SIZE_T count=0,lastDot=SIZE_MAX;
        for(;;){
            std::uint32_t scalar=0;const auto step=json.NextOriginalString(cursor,scalar);
            if(step==OriginalJson::StringStep::Invalid)return result;
            if(step==OriginalJson::StringStep::End||!scalar)break;
            if(scalar==L'/'||scalar==L'\\')result.begin=count+1;
            if(scalar==L'.')lastDot=count;
            ++count;
        }
        const SIZE_T end=lastDot!=SIZE_MAX&&lastDot>=result.begin?lastDot:count;
        result.extent=end-result.begin;
        // Upstream strips a .ni suffix after removing the final extension.
        // Preserve case in the surviving name: its unordered_map key is exact.
        if(result.extent>=3){
            if(!json.BeginOriginalString(token,cursor))return result;
            std::array<std::uint32_t,3> suffix{};
            for(SIZE_T i=0;i<end;++i){
                std::uint32_t scalar=0;
                if(json.NextOriginalString(cursor,scalar)!=OriginalJson::StringStep::Scalar||!scalar)return result;
                if(i>=end-3)suffix[i-(end-3)]=scalar;
            }
            if(suffix[0]==L'.'&&(suffix[1]==L'n'||suffix[1]==L'N')&&(suffix[2]==L'i'||suffix[2]==L'I')){
                result.extent-=3;result.niSuffixRemoved=true;
                // pal::strcasecmp uses the actual CRT _wcsicmp. Retain the
                // ASCII case projection but do not grant its locale-dependent
                // uppercase correspondence without the selected runtime join.
                result.crtCaseCorrespondenceNeeded=suffix[1]==L'N'||suffix[2]==L'I';
            }
        }
        result.observed=true;return result;
    }
    static bool SameSourceAssetName(const OriginalJson& json,const SourceAssetName& a,const SourceAssetName& b) noexcept {
        if(!a.observed||!b.observed||a.extent!=b.extent)return false;
        OriginalJson::OriginalStringCursor ac,bc;
        if(!json.BeginOriginalString(a.token,ac)||!json.BeginOriginalString(b.token,bc))return false;
        std::uint32_t av=0,bv=0;
        for(SIZE_T i=0;i<a.begin;++i)
            if(json.NextOriginalString(ac,av)!=OriginalJson::StringStep::Scalar||!av)return false;
        for(SIZE_T i=0;i<b.begin;++i)
            if(json.NextOriginalString(bc,bv)!=OriginalJson::StringStep::Scalar||!bv)return false;
        for(SIZE_T i=0;i<a.extent;++i){
            if(json.NextOriginalString(ac,av)!=OriginalJson::StringStep::Scalar||!av||
                json.NextOriginalString(bc,bv)!=OriginalJson::StringStep::Scalar||!bv||av!=bv)return false;
        }
        return true;
    }
    struct AssetCandidate {
        AssetKind kind=AssetKind::Runtime;
        SIZE_T libraryKey=SIZE_MAX,libraryMetadata=SIZE_MAX,pathKey=SIZE_MAX,metadata=SIZE_MAX;
        SIZE_T rid=SIZE_MAX,assetType=SIZE_MAX;
        SIZE_T assemblyVersion=SIZE_MAX,fileVersion=SIZE_MAX,localPath=SIZE_MAX;
        WindowsVersionSource assemblyVersionSource,fileVersionSource;
        SourceAssetName sourceName;
    };
    struct VersionPreference {
        enum class Outcome {Unobserved,KeepExisting,ProbeCandidate,ConversionWouldThrow,OriginalInvalid,SignedAbiUnresolved,DifferentSourceName,CrtCaseUnresolved};
        Outcome outcome=Outcome::Unobserved;
        int assemblyOrder=0,fileOrder=0;
        bool fileCompared=false,debugBuildCorrespondenceNeeded=false;
    };
    // This is ONLY deps_resolver's version predicate after its actual name-map
    // hit and extension comparison. Probe success/path replacement, RID and
    // source/runtime admission are not inferred from this pure source result.
    static VersionPreference ObserveVersionPreference(const WindowsVersionSource& candidateAssembly,
        const WindowsVersionSource& candidateFile,const WindowsVersionSource& existingAssembly,
        const WindowsVersionSource& existingFile) noexcept {
        VersionPreference result;
        const WindowsVersionSource* originals[]={&candidateAssembly,&candidateFile,&existingAssembly,&existingFile};
        for(const auto* original:originals){
            if(original->outcome==WindowsVersionSource::Outcome::InvalidOriginal){
                result.outcome=VersionPreference::Outcome::OriginalInvalid;return result;
            }
            if(original->outcome==WindowsVersionSource::Outcome::UnsignedConversionWouldThrow){
                result.outcome=VersionPreference::Outcome::ConversionWouldThrow;return result;
            }
            if(original->signedAbiCorrespondenceNeeded){
                result.outcome=VersionPreference::Outcome::SignedAbiUnresolved;return result;
            }
            if(!original->debugCanonical)result.debugBuildCorrespondenceNeeded=true;
        }
        const auto compare=[](const WindowsVersionSource& a,const WindowsVersionSource& b) noexcept {
            for(SIZE_T i=0;i<4;++i){
                // Failed parse/default versions are -1; omitted build/revision
                // use that same sentinel. Explicit out-of-range unsigned input
                // was refused above, never silently reinterpreted as signed.
                const std::int64_t av=a.components[i]==UINT32_MAX?std::int64_t{-1}:static_cast<std::int64_t>(a.components[i]);
                const std::int64_t bv=b.components[i]==UINT32_MAX?std::int64_t{-1}:static_cast<std::int64_t>(b.components[i]);
                if(av!=bv)return av>bv?1:-1;
            }
            return 0;
        };
        result.assemblyOrder=compare(candidateAssembly,existingAssembly);
        if(result.assemblyOrder==0){
            result.fileCompared=true;result.fileOrder=compare(candidateFile,existingFile);
        }
        result.outcome=result.assemblyOrder>0||(result.assemblyOrder==0&&result.fileOrder>=0)?
            VersionPreference::Outcome::ProbeCandidate:VersionPreference::Outcome::KeepExisting;
        return result;
    }
    struct LibraryAssetJoin {
        SIZE_T metadataOccurrence=SIZE_MAX,assetOccurrence=SIZE_MAX;
        bool ridSelectionPending=false;
    };
    struct ProbePathOriginal {
        const OriginalJson* jsonOriginal=nullptr;
        SIZE_T token=SIZE_MAX;
        bool fromDev=false;
    };
    struct ProbeCursor {
        enum class Outcome {Unstarted,Path,Complete,DevInputUnsupplied,UnsupportedStage,InvalidPath,Overflow};
        const OriginalRuntimeHostConfiguration* ownerOriginal=nullptr;
        SIZE_T stage=0,next=SIZE_MAX,paths=0;
        bool stageEntered=false;
        ProbePathOriginal last;
        Outcome outcome=Outcome::Unstarted;
    };
    struct ConfiguredProperty {
        enum class Outcome {Unobserved,Absent,String,JsonLiteral,NeedsHostWriter,InputUnobserved,UnsupportedStage};
        const OriginalJson* jsonOriginal=nullptr;
        SIZE_T key=SIZE_MAX,value=SIZE_MAX;
        Outcome outcome=Outcome::Unobserved;
        OriginalJson::Kind literalKind=OriginalJson::Kind::Null;
        bool fromDev=false;
    };
    static bool OriginalCStringEquals(const OriginalJson& json,SIZE_T token,const char* ascii) noexcept {
        OriginalJson::OriginalStringCursor cursor;if(!ascii||!json.BeginOriginalString(token,cursor))return false;
        SIZE_T i=0;
        for(;;){
            std::uint32_t scalar=0;const auto step=json.NextOriginalString(cursor,scalar);
            if(step==OriginalJson::StringStep::Invalid)return false;
            if(step==OriginalJson::StringStep::End||!scalar)return !ascii[i];
            if(!ascii[i]||scalar!=static_cast<unsigned char>(ascii[i++]))return false;
        }
    }
    struct SignedIntegerSource {
        const OriginalJson* jsonOriginal=nullptr;SIZE_T token=SIZE_MAX;
        std::int32_t value=0;bool attempted=false,decimalSigned32=false;
    };
    static SignedIntegerSource ObserveSignedIntegerSource(const OriginalJson& json,SIZE_T ordinal) noexcept {
        SignedIntegerSource r;r.attempted=true;r.jsonOriginal=&json;r.token=ordinal;
        if(!json.Complete()||ordinal>=json.Count())return r;
        const auto& t=json.PrivateTokens()[ordinal];const auto* body=json.PrivateOriginalBody();
        if(t.kind!=OriginalJson::Kind::Number||!body||t.begin>=t.end||t.end>json.OriginalExtent())return r;
        SIZE_T i=t.begin;const bool negative=body[i]=='-';if(negative)++i;
        if(i==t.end)return r;
        const std::uint32_t limit=negative?2147483648u:2147483647u;std::uint32_t value=0;
        for(;i<t.end;++i){
            const unsigned c=body[i];if(c<'0'||c>'9')return r;
            const unsigned digit=c-'0';if(value>(limit-digit)/10)return r;value=value*10+digit;
        }
        r.value=negative?(value==2147483648u?INT32_MIN:-static_cast<std::int32_t>(value)):
            static_cast<std::int32_t>(value);
        r.decimalSigned32=true;return r;
    }
    struct FrameworkSettingsSource {
        enum class Outcome {Unobserved,AccessorUnresolved,ComparisonUnresolved,IntegerUnresolved,
            SourceReturnedFalse,EnvironmentAndOverridesPending};
        const OriginalJson* jsonOriginal=nullptr;
        OriginalFrameworkNameVersionSource nameVersion;
        SIZE_T object=SIZE_MAX,stoppedToken=SIZE_MAX;
        unsigned specifiedBefore=0,specifiedAfter=0;
        // These are source-local candidates supplied by the ordered ledger;
        // inherited dev/default/override reconciliation is not yet complete.
        int defaultRollForward=-1,effectiveRollForward=-1;
        bool completeDefaultSettingsAssociated=false;
        bool defaultApplyKnown=false,defaultApply=false,applyAssigned=false,applyValue=false;
        bool preferReleaseDependencyUnresolved=false,attempted=false;
        SignedIntegerSource legacyInteger;
        Outcome outcome=Outcome::Unobserved;
    };
    static FrameworkSettingsSource ObserveFrameworkSettingsSource(const OriginalJson& json,SIZE_T object,
        unsigned& specified,int defaultRoll,bool defaultApplyKnown,bool defaultApply) noexcept {
        FrameworkSettingsSource r;r.attempted=true;r.jsonOriginal=&json;r.object=object;
        r.specifiedBefore=r.specifiedAfter=specified;r.defaultRollForward=r.effectiveRollForward=defaultRoll;
        r.defaultApplyKnown=defaultApplyKnown;r.defaultApply=defaultApply;
        r.nameVersion=OriginalFrameworkNameVersionSource::Observe(json,object);
        using Name=OriginalFrameworkNameVersionSource::Outcome;using Outcome=FrameworkSettingsSource::Outcome;
        if(r.nameVersion.outcome!=Name::SourceReturnedTrue){
            r.outcome=r.nameVersion.outcome==Name::SourceReturnedFalse?Outcome::SourceReturnedFalse:Outcome::AccessorUnresolved;return r;}
        // The actual rollForwardToPrerelease getenv relation is not inferred
        // from source version parsing or an absent manifest field.
        const bool parsedPrerelease=r.nameVersion.versionSource.outcome==
            OriginalFrameworkVersionSource::Outcome::SourceParsed&&r.nameVersion.versionSource.prerelease;
        r.preferReleaseDependencyUnresolved=!parsedPrerelease;
        const auto* tokens=json.PrivateTokens();
        const auto roll=json.Field(object,"rollForward");
        if(roll!=SIZE_MAX){
            r.stoppedToken=roll;
            if(tokens[roll].kind!=OriginalJson::Kind::String){r.outcome=Outcome::AccessorUnresolved;return r;}
            const char* labels[]={"Disable","LatestPatch","Minor","LatestMinor","Major","LatestMajor"};
            int value=-1;for(int i=0;i<6;++i)if(OriginalCStringEquals(json,roll,labels[i])){value=i;break;}
            if(value<0){r.outcome=Outcome::ComparisonUnresolved;return r;}
            r.effectiveRollForward=value;
            if(specified&~1u){r.outcome=Outcome::SourceReturnedFalse;return r;}
            specified|=1u;r.specifiedAfter=specified;
        }
        const auto apply=json.Field(object,"applyPatches");
        if(apply!=SIZE_MAX){
            r.stoppedToken=apply;const auto kind=tokens[apply].kind;
            if(kind!=OriginalJson::Kind::True&&kind!=OriginalJson::Kind::False){r.outcome=Outcome::AccessorUnresolved;return r;}
            r.applyAssigned=true;r.applyValue=kind==OriginalJson::Kind::True;
            if(specified&~2u){r.outcome=Outcome::SourceReturnedFalse;return r;}
            specified|=2u;r.specifiedAfter=specified;
        }
        const auto legacy=json.Field(object,"rollForwardOnNoCandidateFx");
        if(legacy!=SIZE_MAX){
            r.stoppedToken=legacy;r.legacyInteger=ObserveSignedIntegerSource(json,legacy);
            if(!r.legacyInteger.decimalSigned32||r.legacyInteger.value<0||r.legacyInteger.value>2){r.outcome=Outcome::IntegerUnresolved;return r;}
            const int mapping[]={1,2,4};r.effectiveRollForward=mapping[r.legacyInteger.value];
            if(specified&~2u){r.outcome=Outcome::SourceReturnedFalse;return r;}
            specified|=2u;r.specifiedAfter=specified;
        }
        r.stoppedToken=SIZE_MAX;r.outcome=Outcome::EnvironmentAndOverridesPending;return r;
    }
    struct OrderedOptionSource {
        enum class Outcome {Unobserved,NoOp,SourceReturnedTrue,SourceReturnedFalse,
            UnsupportedAccessor,PropertyWriterUnresolved,RollForwardComparisonUnresolved,IntegerConversionUnresolved,FrameworkConsumerPending};
        const OriginalJson* jsonOriginal=nullptr;
        OptionStage optionsOriginal;
        SIZE_T stoppedToken=SIZE_MAX,rollForwardToken=SIZE_MAX,applyPatchesToken=SIZE_MAX,tfmToken=SIZE_MAX;
        unsigned specifiedBefore=0,specifiedAfter=0;
        int rollForwardSource=-1;
        SignedIntegerSource legacyIntegerSource;
        OriginalFrameworkNameVersionSource lastFrameworkSource;
        FrameworkSettingsSource frameworkSettings;
        SIZE_T includedFrameworksAccepted=0,includedFrameworkCursor=SIZE_MAX;
        bool attempted=false,propertiesReached=false,probesReached=false,applyPatchesSource=false;
        bool hasDefaultApplySource=false,defaultSourceInherited=false;
        bool dependencyFlagBefore=false,dependencyFlagAfter=false,tfmAssigned=false;
        Outcome outcome=Outcome::Unobserved;
    };
    static OrderedOptionSource ObserveOrderedOptionSource(const OriginalJson& json,const OptionStage& options,
        bool dev,unsigned& specified,bool& dependent,const OrderedOptionSource* previous=nullptr) noexcept {
        OrderedOptionSource r;r.attempted=true;r.jsonOriginal=&json;r.optionsOriginal=options;
        r.specifiedBefore=r.specifiedAfter=specified;r.dependencyFlagBefore=r.dependencyFlagAfter=dependent;
        // ensure_parsed keeps dev partial default mutations even when its
        // parse_opts return was false; main absent fields do not reset them.
        if(previous&&previous->attempted){
            r.rollForwardSource=previous->rollForwardSource;
            r.hasDefaultApplySource=previous->hasDefaultApplySource;
            r.applyPatchesSource=previous->applyPatchesSource;r.defaultSourceInherited=true;
        }
        using Outcome=OrderedOptionSource::Outcome;
        if(options.shape==OptionStage::Shape::Null||(dev&&options.shape==OptionStage::Shape::Missing)){
            r.outcome=Outcome::NoOp;return r;}
        if(!json.Complete())return r;
        if(options.shape==OptionStage::Shape::RootUnsupported){r.outcome=Outcome::UnsupportedAccessor;return r;}
        if(options.shape!=OptionStage::Shape::Object){r.outcome=Outcome::SourceReturnedFalse;return r;}
        const auto* tokens=json.PrivateTokens();
        r.propertiesReached=true;
        if(options.configProperties!=SIZE_MAX&&tokens[options.configProperties].kind!=OriginalJson::Kind::Object){
            r.stoppedToken=options.configProperties;r.outcome=Outcome::UnsupportedAccessor;return r;}
        if(options.configProperties!=SIZE_MAX){
            for(SIZE_T key=options.configProperties+1;key<tokens[options.configProperties].subtreeEnd;){
                const SIZE_T value=tokens[key].subtreeEnd;const auto kind=tokens[value].kind;
                if(kind!=OriginalJson::Kind::String&&kind!=OriginalJson::Kind::True&&
                    kind!=OriginalJson::Kind::False&&kind!=OriginalJson::Kind::Null){
                    r.stoppedToken=value;r.outcome=Outcome::PropertyWriterUnresolved;return r;}
                key=tokens[value].subtreeEnd;
            }
        }
        r.probesReached=true;
        if(options.additionalProbingPaths!=SIZE_MAX){
            const auto p=options.additionalProbingPaths;const auto kind=tokens[p].kind;
            if(kind==OriginalJson::Kind::Array){
                for(SIZE_T item=p+1;item<tokens[p].subtreeEnd;item=tokens[item].subtreeEnd)
                    if(tokens[item].kind!=OriginalJson::Kind::String){r.stoppedToken=item;r.outcome=Outcome::UnsupportedAccessor;return r;}
            }else if(kind!=OriginalJson::Kind::String){r.stoppedToken=p;r.outcome=Outcome::SourceReturnedFalse;return r;}
        }
        if(options.rollForward!=SIZE_MAX){
            r.rollForwardToken=options.rollForward;
            if(tokens[options.rollForward].kind!=OriginalJson::Kind::String){r.stoppedToken=options.rollForward;r.outcome=Outcome::UnsupportedAccessor;return r;}
            const char* names[]={"Disable","LatestPatch","Minor","LatestMinor","Major","LatestMajor"};
            int selectedRoll=-1;
            for(int i=0;i<6;++i)if(OriginalCStringEquals(json,options.rollForward,names[i])){selectedRoll=i;break;}
            if(selectedRoll<0){r.stoppedToken=options.rollForward;r.outcome=Outcome::RollForwardComparisonUnresolved;return r;}
            r.rollForwardSource=selectedRoll;
            // set_roll_forward precedes mark_specified_setting, including failure.
            if(specified&~1u){r.stoppedToken=options.rollForward;r.outcome=Outcome::SourceReturnedFalse;return r;}
            specified|=1u;r.specifiedAfter=specified;
        }
        if(options.applyPatches!=SIZE_MAX){
            r.applyPatchesToken=options.applyPatches;const auto kind=tokens[options.applyPatches].kind;
            if(kind!=OriginalJson::Kind::True&&kind!=OriginalJson::Kind::False){r.stoppedToken=options.applyPatches;r.outcome=Outcome::UnsupportedAccessor;return r;}
            r.applyPatchesSource=kind==OriginalJson::Kind::True;r.hasDefaultApplySource=true;
            if(specified&~2u){r.stoppedToken=options.applyPatches;r.outcome=Outcome::SourceReturnedFalse;return r;}
            specified|=2u;r.specifiedAfter=specified;
        }
        if(options.rollForwardOnNoCandidateFx!=SIZE_MAX){
            r.legacyIntegerSource=ObserveSignedIntegerSource(json,options.rollForwardOnNoCandidateFx);
            const auto& integer=r.legacyIntegerSource;
            if(!integer.decimalSigned32||integer.value<0||integer.value>2){
                r.stoppedToken=options.rollForwardOnNoCandidateFx;r.outcome=Outcome::IntegerConversionUnresolved;return r;}
            const int mapping[]={1,2,4};r.rollForwardSource=mapping[integer.value];
            if(specified&~2u){r.stoppedToken=options.rollForwardOnNoCandidateFx;r.outcome=Outcome::SourceReturnedFalse;return r;}
            specified|=2u;r.specifiedAfter=specified;
        }
        if(options.tfm!=SIZE_MAX){
            r.tfmToken=options.tfm;
            if(tokens[options.tfm].kind!=OriginalJson::Kind::String){r.stoppedToken=options.tfm;r.outcome=Outcome::UnsupportedAccessor;return r;}
            r.tfmAssigned=true;
        }
        if(options.framework!=SIZE_MAX||options.frameworks!=SIZE_MAX){
            dependent=true;r.dependencyFlagAfter=true;
            r.stoppedToken=options.framework!=SIZE_MAX?options.framework:options.frameworks;
            // Empty frameworks performs no parse_framework calls, but still
            // mutates dependency state; other actual framework consumers remain pending.
            if(options.framework!=SIZE_MAX){
                r.frameworkSettings=ObserveFrameworkSettingsSource(json,options.framework,specified,
                    r.rollForwardSource,r.hasDefaultApplySource,r.applyPatchesSource);
                r.specifiedAfter=specified;r.lastFrameworkSource=r.frameworkSettings.nameVersion;
                r.outcome=r.frameworkSettings.outcome==FrameworkSettingsSource::Outcome::SourceReturnedFalse?
                    Outcome::SourceReturnedFalse:Outcome::FrameworkConsumerPending;return r;
            }
            if(tokens[options.frameworks].kind!=OriginalJson::Kind::Array||
                options.frameworks+1!=tokens[options.frameworks].subtreeEnd){r.outcome=Outcome::FrameworkConsumerPending;return r;}
        }
        if(options.includedFrameworks!=SIZE_MAX){
            r.stoppedToken=options.includedFrameworks;
            if(dependent){r.outcome=Outcome::SourceReturnedFalse;return r;}
            const SIZE_T list=options.includedFrameworks;
            if(tokens[list].kind!=OriginalJson::Kind::Array){r.outcome=Outcome::UnsupportedAccessor;return r;}
            for(SIZE_T item=list+1;item<tokens[list].subtreeEnd;item=tokens[item].subtreeEnd){
                r.includedFrameworkCursor=item;r.lastFrameworkSource=OriginalFrameworkNameVersionSource::Observe(json,item);
                using Framework=OriginalFrameworkNameVersionSource::Outcome;
                if(r.lastFrameworkSource.outcome!=Framework::SourceReturnedTrue){
                    r.stoppedToken=item;
                    r.outcome=r.lastFrameworkSource.outcome==Framework::SourceReturnedFalse?Outcome::SourceReturnedFalse:
                        Outcome::FrameworkConsumerPending;return r;
                }
                for(SIZE_T earlier=list+1;earlier<item;earlier=tokens[earlier].subtreeEnd){
                    const auto name=json.Field(earlier,"name");
                    if(OriginalFrameworkNameVersionSource::SameName(json,name,json,r.lastFrameworkSource.name)){
                        r.stoppedToken=item;r.outcome=Outcome::SourceReturnedFalse;return r;}
                }
                if(previous&&previous->includedFrameworksAccepted){
                    const auto* prior=previous->jsonOriginal;const SIZE_T priorList=previous->optionsOriginal.includedFrameworks;
                    if(!prior||priorList>=prior->Count()){r.stoppedToken=item;r.outcome=Outcome::FrameworkConsumerPending;return r;}
                    SIZE_T priorItem=priorList+1;
                    for(SIZE_T n=0;n<previous->includedFrameworksAccepted;++n){
                        if(priorItem>=prior->PrivateTokens()[priorList].subtreeEnd){r.stoppedToken=item;r.outcome=Outcome::FrameworkConsumerPending;return r;}
                        if(OriginalFrameworkNameVersionSource::SameName(*prior,prior->Field(priorItem,"name"),json,r.lastFrameworkSource.name)){
                            r.stoppedToken=item;r.outcome=Outcome::SourceReturnedFalse;return r;}
                        priorItem=prior->PrivateTokens()[priorItem].subtreeEnd;
                    }
                }
                ++r.includedFrameworksAccepted;
            }
            r.includedFrameworkCursor=tokens[list].subtreeEnd;
        }
        r.stoppedToken=SIZE_MAX;r.outcome=Outcome::SourceReturnedTrue;return r;
    }
    static ConfiguredProperty ObservePropertyStage(const OriginalJson& json,const OptionStage& options,
        const char* originalKey,bool dev) noexcept {
        ConfiguredProperty result;result.fromDev=dev;result.jsonOriginal=&json;
        if(!originalKey){result.outcome=ConfiguredProperty::Outcome::UnsupportedStage;return result;}
        if(!json.Complete()){result.outcome=ConfiguredProperty::Outcome::InputUnobserved;return result;}
        if(options.shape==OptionStage::Shape::Null||(dev&&options.shape==OptionStage::Shape::Missing)){
            result.outcome=ConfiguredProperty::Outcome::Absent;return result;
        }
        if(options.shape!=OptionStage::Shape::Object){result.outcome=ConfiguredProperty::Outcome::UnsupportedStage;return result;}
        if(options.configProperties==SIZE_MAX){result.outcome=ConfiguredProperty::Outcome::Absent;return result;}
        const auto* tokens=json.PrivateTokens();const SIZE_T object=options.configProperties;
        if(object>=json.Count()||tokens[object].kind!=OriginalJson::Kind::Object){result.outcome=ConfiguredProperty::Outcome::UnsupportedStage;return result;}
        result.outcome=ConfiguredProperty::Outcome::Absent;
        // runtime_config's map assignment is last identical pal string key
        // wins, unlike FindMember's first JSON member. Preserve original tokens.
        for(SIZE_T key=object+1;key<tokens[object].subtreeEnd;){
            const SIZE_T value=tokens[key].subtreeEnd;
            if(OriginalCStringEquals(json,key,originalKey)){
                result.key=key;result.value=value;
                const auto kind=tokens[value].kind;
                result.outcome=kind==OriginalJson::Kind::String?ConfiguredProperty::Outcome::String:
                    (kind==OriginalJson::Kind::True||kind==OriginalJson::Kind::False||kind==OriginalJson::Kind::Null)?
                    ConfiguredProperty::Outcome::JsonLiteral:ConfiguredProperty::Outcome::NeedsHostWriter;
                result.literalKind=kind;
            }
            key=tokens[value].subtreeEnd;
        }
        return result;
    }
    struct RidGraphSourceDecision {
        enum class Outcome {Unobserved,SourceTrue,SourceFalse,CaseComparisonUnresolved,WriterUnresolved,InputUnresolved};
        ConfiguredProperty original;
        Outcome outcome=Outcome::Unobserved;
    };
    struct PropertyCursor {
        enum class Outcome {Unstarted,Property,Complete,DevInputUnsupplied,UnsupportedStage,InvalidOriginal,Overflow};
        const OriginalRuntimeHostConfiguration* ownerOriginal=nullptr;
        SIZE_T stage=0,next=SIZE_MAX,properties=0;
        ConfiguredProperty last;
        Outcome outcome=Outcome::Unstarted;
    };
    struct SetAppPathsSourceDecision {
        enum class Outcome {Unobserved,SourceFalse,SourceTrue,InputUnresolved,CaseOrEnumerationUnresolved,WriterUnresolved};
        ConfiguredProperty exactOriginal;
        SIZE_T potentialKeyCount=0;
        bool caseVariant=false,nonAsciiKey=false;
        Outcome outcome=Outcome::Unobserved;
    };
    enum class SetAppPathsKeySource {Different,Exact,CaseCandidate,NonAsciiOrInvalid};
    static SetAppPathsKeySource ObserveSetAppPathsKeySource(const ConfiguredProperty& property) noexcept {
        if(!property.jsonOriginal)return SetAppPathsKeySource::NonAsciiOrInvalid;
        OriginalJson::OriginalStringCursor cursor;
        if(!property.jsonOriginal->BeginOriginalString(property.key,cursor))return SetAppPathsKeySource::NonAsciiOrInvalid;
        const char* target="Microsoft.NETCore.DotNetHostPolicy.SetAppPaths";
        SIZE_T i=0;bool exact=true;
        for(;;){
            std::uint32_t scalar=0;const auto step=property.jsonOriginal->NextOriginalString(cursor,scalar);
            if(step==OriginalJson::StringStep::Invalid)return SetAppPathsKeySource::NonAsciiOrInvalid;
            if(step==OriginalJson::StringStep::End||!scalar){
                if(target[i])return SetAppPathsKeySource::Different;
                return exact?SetAppPathsKeySource::Exact:SetAppPathsKeySource::CaseCandidate;
            }
            if(scalar>127)return SetAppPathsKeySource::NonAsciiOrInvalid;
            if(!target[i])return SetAppPathsKeySource::Different;
            const auto expected=static_cast<unsigned char>(target[i++]);
            if(scalar!=expected)exact=false;
            const auto fold=[](std::uint32_t value){return value>='A'&&value<='Z'?value+('a'-'A'):value;};
            if(fold(scalar)!=fold(expected))return SetAppPathsKeySource::Different;
        }
    }
    enum class OriginalKeyComparison {Equal,Different,Invalid};
    static OriginalKeyComparison CompareOriginalKeys(const OriginalJson& left,SIZE_T leftKey,
        const OriginalJson& right,SIZE_T rightKey) noexcept {
        OriginalJson::OriginalStringCursor a,b;
        if(!left.BeginOriginalString(leftKey,a)||!right.BeginOriginalString(rightKey,b))return OriginalKeyComparison::Invalid;
        for(;;){
            std::uint32_t av=0,bv=0;
            const auto ar=left.NextOriginalString(a,av),br=right.NextOriginalString(b,bv);
            if(ar==OriginalJson::StringStep::Invalid||br==OriginalJson::StringStep::Invalid)return OriginalKeyComparison::Invalid;
            const bool ae=ar==OriginalJson::StringStep::End||!av,be=br==OriginalJson::StringStep::End||!bv;
            if(ae||be)return ae&&be?OriginalKeyComparison::Equal:OriginalKeyComparison::Different;
            if(av!=bv)return OriginalKeyComparison::Different;
        }
    }
    static RidGraphSourceDecision ObserveRidGraphSource(const ConfiguredProperty& property) noexcept {
        RidGraphSourceDecision result;result.original=property;
        using Property=ConfiguredProperty::Outcome;
        using Outcome=RidGraphSourceDecision::Outcome;
        if(property.outcome==Property::Absent)result.outcome=Outcome::SourceFalse;
        else if(property.outcome==Property::JsonLiteral){
            result.outcome=property.literalKind==OriginalJson::Kind::True?Outcome::SourceTrue:
                (property.literalKind==OriginalJson::Kind::False||property.literalKind==OriginalJson::Kind::Null)?
                Outcome::SourceFalse:Outcome::InputUnresolved;
        }else if(property.outcome==Property::String&&property.jsonOriginal){
            // Identical lower-case text needs no inferred case transformation.
            // Different exact text may still compare equal in the actual CRT.
            result.outcome=OriginalCStringEquals(*property.jsonOriginal,property.value,"true")?
                Outcome::SourceTrue:Outcome::CaseComparisonUnresolved;
        }else if(property.outcome==Property::NeedsHostWriter)result.outcome=Outcome::WriterUnresolved;
        else result.outcome=Outcome::InputUnresolved;
        return result;
    }
    enum class PropertyTextComparison {Equal,Different,Unresolved};
    static PropertyTextComparison CompareOriginalStringUtf8(const OriginalJson& json,SIZE_T token,
        const unsigned char* bytes,SIZE_T extent) noexcept {
        if(!bytes || !extent || bytes[extent-1] || memchr(bytes,0,extent-1))return PropertyTextComparison::Unresolved;
        OriginalJson::OriginalStringCursor cursor;
        if(!json.BeginOriginalString(token,cursor))return PropertyTextComparison::Unresolved;
        SIZE_T used=0;
        for(;;){
            std::uint32_t scalar=0;const auto step=json.NextOriginalString(cursor,scalar);
            if(step==OriginalJson::StringStep::Invalid)return PropertyTextComparison::Unresolved;
            // The original source passes c_str() through UTF conversion: first
            // NUL ends the native string even when the JSON token continues.
            if(step==OriginalJson::StringStep::End || !scalar)
                return used==extent-1?PropertyTextComparison::Equal:PropertyTextComparison::Different;
            if(scalar>0x10ffff || (scalar>=0xd800 && scalar<=0xdfff))return PropertyTextComparison::Unresolved;
            unsigned char encoded[4]{};SIZE_T count=0;
            if(scalar<0x80){encoded[0]=static_cast<unsigned char>(scalar);count=1;}
            else if(scalar<0x800){encoded[0]=static_cast<unsigned char>(0xc0|(scalar>>6));encoded[1]=static_cast<unsigned char>(0x80|(scalar&63));count=2;}
            else if(scalar<0x10000){encoded[0]=static_cast<unsigned char>(0xe0|(scalar>>12));encoded[1]=static_cast<unsigned char>(0x80|((scalar>>6)&63));encoded[2]=static_cast<unsigned char>(0x80|(scalar&63));count=3;}
            else {encoded[0]=static_cast<unsigned char>(0xf0|(scalar>>18));encoded[1]=static_cast<unsigned char>(0x80|((scalar>>12)&63));encoded[2]=static_cast<unsigned char>(0x80|((scalar>>6)&63));encoded[3]=static_cast<unsigned char>(0x80|(scalar&63));count=4;}
            if(count>extent-1-used || memcmp(bytes+used,encoded,count))return PropertyTextComparison::Different;
            used+=count;
        }
    }
    static PropertyTextComparison CompareConfiguredPropertyUtf8(const ConfiguredProperty& property,
        const unsigned char* bytes,SIZE_T extent) noexcept {
        if(property.outcome==ConfiguredProperty::Outcome::String && property.jsonOriginal)
            return CompareOriginalStringUtf8(*property.jsonOriginal,property.value,bytes,extent);
        if(!bytes || !extent || bytes[extent-1] || memchr(bytes,0,extent-1))return PropertyTextComparison::Unresolved;
        if(property.outcome==ConfiguredProperty::Outcome::JsonLiteral){
            const char* literal=property.literalKind==OriginalJson::Kind::True?"true":
                property.literalKind==OriginalJson::Kind::False?"false":property.literalKind==OriginalJson::Kind::Null?"null":nullptr;
            if(literal)return extent==strlen(literal)+1 && !memcmp(bytes,literal,extent)?
                PropertyTextComparison::Equal:PropertyTextComparison::Different;
        }
        return PropertyTextComparison::Unresolved;
    }
    static PropertyTextComparison CompareExactPropertySourceText(const ConfiguredProperty& property,const char* ascii) noexcept {
        if(!ascii)return PropertyTextComparison::Unresolved;
        if(property.outcome==ConfiguredProperty::Outcome::String&&property.jsonOriginal)
            return OriginalCStringEquals(*property.jsonOriginal,property.value,ascii)?PropertyTextComparison::Equal:PropertyTextComparison::Different;
        if(property.outcome==ConfiguredProperty::Outcome::JsonLiteral){
            const char* literal=property.literalKind==OriginalJson::Kind::True?"true":
                property.literalKind==OriginalJson::Kind::False?"false":property.literalKind==OriginalJson::Kind::Null?"null":nullptr;
            if(!literal)return PropertyTextComparison::Unresolved;
            SIZE_T i=0;while(literal[i]&&ascii[i]&&literal[i]==ascii[i])++i;
            return literal[i]==ascii[i]?PropertyTextComparison::Equal:PropertyTextComparison::Different;
        }
        // Exact text inequality is NOT the host's case-insensitive comparison;
        // case-distinct map enumeration and actual CRT semantics remain separate.
        return PropertyTextComparison::Unresolved;
    }
 private:
    const NativeHeldFiles& inputs_;
    OriginalJson runtimeConfig_,deps_;
    Document configOriginal_,depsOriginal_;
    std::unique_ptr<OriginalJson> devConfig_;
    Document devOriginal_;
    bool devAttempted_=false;
    OptionStage devOptions_,mainOptions_;
    std::array<OrderedOptionSource,2> orderedOptions_;
    ProbeCursor probeTraversal_;
    std::array<ConfiguredProperty,3> configuredProperties_;
    RidGraphSourceDecision ridGraphSource_;
    PropertyCursor propertyTraversal_;
    SetAppPathsSourceDecision setAppPathsSource_;
    std::array<ConfiguredProperty,8> reservedHostProperties_;
    struct ReservedPropertyDecision {
        SIZE_T sourceCollisionCount=0;
        bool inputUnresolved=false,attempted=false;
        // No first-collision ordinal: actual cfg map enumeration is separate.
    } reservedPropertyDecision_;
    Projection projection_;
    std::unique_ptr<AssetCandidate[]> assets_;
    SIZE_T assetCount_=0,assetCapacity_=0,libraryCount_=0;
    SIZE_T configTokensOriginal_=0,depsTokensOriginal_=0,devTokensOriginal_=0,depthOriginal_=0;
    bool preownAttempted_=false,preowned_=false;
    std::exception_ptr preownExceptionOriginal_;
    std::unique_ptr<LibraryMetadataOccurrence[]> libraries_;
    struct LibraryCursor { SIZE_T library=0,asset=0,pairs=0; bool attempted=false,complete=false,overflow=false; };
    LibraryCursor libraryCursor_;
    SIZE_T libraryAssetCount_=0;
    bool libraryJoinAttempted_=false,libraryJoinComplete_=false;
    static SIZE_T CheckGeometry(SIZE_T configTokens,SIZE_T depsTokens,SIZE_T devTokens=0) {
        SIZE_T total=sizeof(OriginalRuntimeHostConfiguration);
        const SIZE_T config=OriginalJson::DynamicSourceBytesFor(configTokens);
        const SIZE_T deps=OriginalJson::DynamicSourceBytesFor(depsTokens);
        if(config>SIZE_MAX-total)throw std::invalid_argument("original runtime configuration aggregate geometry");total+=config;
        if(deps>SIZE_MAX-total)throw std::invalid_argument("original runtime configuration aggregate geometry");total+=deps;
        if(devTokens){
            const SIZE_T dev=OriginalJson::DynamicSourceBytesFor(devTokens);
            if(sizeof(OriginalJson)>SIZE_MAX-total)throw std::invalid_argument("original dev configuration aggregate geometry");
            total+=sizeof(OriginalJson);
            if(dev>SIZE_MAX-total)throw std::invalid_argument("original dev configuration aggregate geometry");total+=dev;
        }
        if(depsTokens>(SIZE_MAX-total)/(sizeof(AssetCandidate)+sizeof(LibraryMetadataOccurrence)))throw std::invalid_argument("original runtime candidate aggregate geometry");
        return configTokens;
    }
    bool CaptureDocument(Document& original,OriginalJson& reader,SIZE_T ordinal) noexcept {
        original.attempted=true;original.owner=&inputs_;original.ordinal=ordinal;
        if(inputs_.Failed()||ordinal>=inputs_.FileCount())return false;
        const auto& held=inputs_.PrivateFiles()[ordinal];
        if(!held.valid||held.closeAttempted||held.readCompletionUnknown||
            !inputs_.ExactCapturedSourceSpan(ordinal,original.body,original.extent))return false;
        original.spanObserved=true;
        original.syntaxObserved=reader.Capture(original.body,original.extent);
        return original.syntaxObserved;
    }
    static bool Object(const OriginalJson& reader,SIZE_T ordinal) noexcept {
        return ordinal<reader.Count()&&reader.PrivateTokens()[ordinal].kind==OriginalJson::Kind::Object;
    }
    bool Candidates(SIZE_T libraryKey,SIZE_T section,AssetKind kind) noexcept {
        if(section==SIZE_MAX)return true;
        if(!Object(deps_,section))return false;
        const auto* tokens=deps_.PrivateTokens();
        for(SIZE_T key=section+1;key<tokens[section].subtreeEnd;){
            if(assetCount_==assetCapacity_)return false;
            const SIZE_T value=tokens[key].subtreeEnd;
            auto& row=assets_[assetCount_++];row.kind=kind;row.libraryKey=libraryKey;
            // Informational first matching metadata only. Upstream reconciliation
            // iterates ALL libraries, including duplicate names; this is not its resolver.
            row.libraryMetadata=deps_.FieldForOriginalString(projection_.libraries,libraryKey);
            row.pathKey=key;row.metadata=value;
            if(!Object(deps_,value)||!Object(deps_,row.libraryMetadata))return false;
            row.rid=deps_.Field(value,"rid");row.assetType=deps_.Field(value,"assetType");
            row.assemblyVersion=deps_.Field(value,"assemblyVersion");
            row.fileVersion=deps_.Field(value,"fileVersion");row.localPath=deps_.Field(value,"localPath");
            row.assemblyVersionSource=ObserveWindowsVersionSource(deps_,row.assemblyVersion);
            row.fileVersionSource=ObserveWindowsVersionSource(deps_,row.fileVersion);
            row.sourceName=ObserveSourceAssetName(deps_,row.pathKey);
            if(!row.sourceName.observed)return false;
            key=tokens[value].subtreeEnd;
        }return true;
    }
public:
    // Geometry comes from selected complete original input and physical-stack
    // precharge, not an arbitrary new public file-size/deadline restriction.
    static SIZE_T ReservedSourceBytesFor(SIZE_T configTokens,SIZE_T depsTokens,SIZE_T devTokens=0) {
        CheckGeometry(configTokens,depsTokens,devTokens);
        return sizeof(OriginalRuntimeHostConfiguration)+OriginalJson::DynamicSourceBytesFor(configTokens)+
            OriginalJson::DynamicSourceBytesFor(depsTokens)+depsTokens*(sizeof(AssetCandidate)+sizeof(LibraryMetadataOccurrence))+
            (devTokens?sizeof(OriginalJson)+OriginalJson::DynamicSourceBytesFor(devTokens):0);
    }
    OriginalRuntimeHostConfiguration(const NativeHeldFiles& inputs,SIZE_T configTokens,
        SIZE_T depsTokens,SIZE_T originalDepthLimit,SIZE_T devTokens=0)
        :inputs_(inputs),runtimeConfig_(configTokens,originalDepthLimit,OriginalJson::Mode::FirstRootCommentsFirstMember,OriginalJson::DeferredStorage{}),
         deps_(depsTokens,originalDepthLimit,OriginalJson::Mode::FirstRootCommentsFirstMember,OriginalJson::DeferredStorage{}),
         configTokensOriginal_(configTokens),depsTokensOriginal_(depsTokens),devTokensOriginal_(devTokens),depthOriginal_(originalDepthLimit) {}
    bool PreownOriginal() noexcept {
        if(preownAttempted_)return false;preownAttempted_=true;
        try {
        const SIZE_T depsTokens=depsTokensOriginal_,devTokens=devTokensOriginal_,originalDepthLimit=depthOriginal_;
        (void)CheckGeometry(configTokensOriginal_,depsTokens,devTokens);
        if(!runtimeConfig_.PreownOriginal()){preownExceptionOriginal_=runtimeConfig_.PreownExceptionOriginal();return false;}
        if(!deps_.PreownOriginal()){preownExceptionOriginal_=deps_.PreownExceptionOriginal();return false;}
        if(depsTokens>(SIZE_MAX-sizeof(*this))/sizeof(AssetCandidate))throw std::invalid_argument("original deps candidate geometry");
        assets_=std::make_unique<AssetCandidate[]>(depsTokens);
        assetCapacity_=depsTokens;
        libraries_=std::make_unique<LibraryMetadataOccurrence[]>(depsTokens);
        if(devTokens){
            devConfig_=std::make_unique<OriginalJson>(devTokens,originalDepthLimit,OriginalJson::Mode::FirstRootCommentsFirstMember,OriginalJson::DeferredStorage{});
            if(!devConfig_->PreownOriginal()){preownExceptionOriginal_=devConfig_->PreownExceptionOriginal();return false;}
        }
        preowned_=true;return true;
        }catch(...){preownExceptionOriginal_=std::current_exception();return false;}
    }
    const std::exception_ptr& PreownExceptionOriginal() const noexcept{return preownExceptionOriginal_;}
    OriginalRuntimeHostConfiguration(const OriginalRuntimeHostConfiguration&)=delete;
    OriginalRuntimeHostConfiguration& operator=(const OriginalRuntimeHostConfiguration&)=delete;
    bool CaptureOriginalDev(SIZE_T devOrdinal) noexcept {
        if(!preowned_)return false;
        if(devAttempted_||projection_.attempted)return false;devAttempted_=true;
        if(!devConfig_)return false;
        // Keep failed syntax/partial tokens as originals. Upstream's dev parse
        // and parse_opts failure handling is distinct from main-config validity;
        // no successful merge/provider selection is inferred here.
        CaptureDocument(devOriginal_,*devConfig_,devOrdinal);
        devOptions_=ObserveOptionStage(*devConfig_);
        return devOriginal_.spanObserved;
    }
    bool CaptureOriginals(SIZE_T runtimeConfigOrdinal,SIZE_T depsOrdinal) noexcept {
        if(!preowned_)return false;
        if(projection_.attempted)return false;projection_.attempted=true;
        // Independently retain both selected originals even if one is malformed.
        const bool config=CaptureDocument(configOriginal_,runtimeConfig_,runtimeConfigOrdinal);
        const bool deps=CaptureDocument(depsOriginal_,deps_,depsOrdinal);
        mainOptions_=ObserveOptionStage(runtimeConfig_);
        unsigned specified=0;bool dependent=false,devFlowKnown=false;
        if(devConfig_&&devOriginal_.spanObserved){
            if(!devOriginal_.syntaxObserved){
                // Whole-document dev syntax failure precedes parse_opts.
                orderedOptions_[0].attempted=true;orderedOptions_[0].jsonOriginal=devConfig_.get();
                orderedOptions_[0].optionsOriginal=devOptions_;
                orderedOptions_[0].outcome=OrderedOptionSource::Outcome::NoOp;devFlowKnown=true;
            }else {
                orderedOptions_[0]=ObserveOrderedOptionSource(*devConfig_,devOptions_,true,specified,dependent);
                const auto outcome=orderedOptions_[0].outcome;
                devFlowKnown=outcome==OrderedOptionSource::Outcome::NoOp||
                    outcome==OrderedOptionSource::Outcome::SourceReturnedTrue||
                    outcome==OrderedOptionSource::Outcome::SourceReturnedFalse;
            }
        }
        // Failed dev parse_opts returns are ignored upstream; partial source
        // mutations survive. Unknown dev execution cannot be replaced by empty.
        if(devFlowKnown)orderedOptions_[1]=ObserveOrderedOptionSource(runtimeConfig_,mainOptions_,false,specified,dependent,&orderedOptions_[0]);
        const char* consumedKeys[]={"System.Runtime.Loader.UseRidGraph","Microsoft.NETCore.DotNetHostPolicy.SetAppPaths","STARTUP_HOOKS"};
        for(SIZE_T property=0;property<configuredProperties_.size();++property)
            configuredProperties_[property]=ObserveConfiguredProperty(consumedKeys[property]);
        ridGraphSource_=ObserveRidGraphSource(configuredProperties_[0]);
        BeginPropertyCursor(propertyTraversal_);
        while(NextPropertyOriginal(propertyTraversal_)){
            const auto key=ObserveSetAppPathsKeySource(propertyTraversal_.last);
            if(key==SetAppPathsKeySource::NonAsciiOrInvalid)setAppPathsSource_.nonAsciiKey=true;
            else if(key!=SetAppPathsKeySource::Different){
                ++setAppPathsSource_.potentialKeyCount;
                if(key==SetAppPathsKeySource::Exact)setAppPathsSource_.exactOriginal=propertyTraversal_.last;
                else setAppPathsSource_.caseVariant=true;
            }
        }
        using AppPaths=SetAppPathsSourceDecision::Outcome;
        if(propertyTraversal_.outcome!=PropertyCursor::Outcome::Complete)
            setAppPathsSource_.outcome=AppPaths::InputUnresolved;
        else if(setAppPathsSource_.nonAsciiKey||setAppPathsSource_.caseVariant||setAppPathsSource_.potentialKeyCount>1)
            setAppPathsSource_.outcome=AppPaths::CaseOrEnumerationUnresolved;
        else if(!setAppPathsSource_.potentialKeyCount)setAppPathsSource_.outcome=AppPaths::SourceFalse;
        else {
            const auto value=ObserveRidGraphSource(setAppPathsSource_.exactOriginal);
            using Value=RidGraphSourceDecision::Outcome;
            setAppPathsSource_.outcome=value.outcome==Value::SourceTrue?AppPaths::SourceTrue:
                value.outcome==Value::SourceFalse?AppPaths::SourceFalse:
                value.outcome==Value::WriterUnresolved?AppPaths::WriterUnresolved:AppPaths::CaseOrEnumerationUnresolved;
        }
        const char* reservedKeys[]={"TRUSTED_PLATFORM_ASSEMBLIES","NATIVE_DLL_SEARCH_DIRECTORIES",
            "PLATFORM_RESOURCE_ROOTS","APP_CONTEXT_BASE_DIRECTORY","APP_CONTEXT_DEPS_FILES",
            "FX_DEPS_FILE","PROBING_DIRECTORIES","RUNTIME_IDENTIFIER"};
        reservedPropertyDecision_.attempted=true;
        for(SIZE_T property=0;property<reservedHostProperties_.size();++property){
            auto& observed=reservedHostProperties_[property];
            observed=ObserveConfiguredProperty(reservedKeys[property]);
            using Outcome=ConfiguredProperty::Outcome;
            if(observed.outcome==Outcome::String||observed.outcome==Outcome::JsonLiteral||
                observed.outcome==Outcome::NeedsHostWriter)++reservedPropertyDecision_.sourceCollisionCount;
            else if(observed.outcome!=Outcome::Absent)reservedPropertyDecision_.inputUnresolved=true;
        }
        BeginProbeCursor(probeTraversal_);
        while(NextProbeOriginal(probeTraversal_)){}
        if(!config||!deps||!Object(runtimeConfig_,0)||!Object(deps_,0))return false;
        auto& p=projection_;
        p.runtimeOptions=mainOptions_.options;
        // parse_opts accepts null as an actual no-op. A missing main field
        // remains different: ensure_parsed returns false for that case.
        if(mainOptions_.shape!=OptionStage::Shape::Object&&mainOptions_.shape!=OptionStage::Shape::Null)return false;
        p.tfm=runtimeConfig_.Field(p.runtimeOptions,"tfm");
        p.framework=runtimeConfig_.Field(p.runtimeOptions,"framework");
        p.frameworks=runtimeConfig_.Field(p.runtimeOptions,"frameworks");
        p.includedFrameworks=runtimeConfig_.Field(p.runtimeOptions,"includedFrameworks");
        p.rollForward=runtimeConfig_.Field(p.runtimeOptions,"rollForward");
        p.applyPatches=runtimeConfig_.Field(p.runtimeOptions,"applyPatches");
        p.configProperties=runtimeConfig_.Field(p.runtimeOptions,"configProperties");
        p.additionalProbingPaths=runtimeConfig_.Field(p.runtimeOptions,"additionalProbingPaths");
        p.runtimeTarget=deps_.Field(0,"runtimeTarget");
        p.runtimeTargetName=p.runtimeTarget<deps_.Count()&&deps_.PrivateTokens()[p.runtimeTarget].kind==OriginalJson::Kind::String ?
            p.runtimeTarget : deps_.Field(p.runtimeTarget,"name");
        p.targets=deps_.Field(0,"targets");p.libraries=deps_.Field(0,"libraries");
        p.runtimes=deps_.Field(0,"runtimes");
        p.rootShapesObserved=p.runtimeTargetName<deps_.Count()&&
            deps_.PrivateTokens()[p.runtimeTargetName].kind==OriginalJson::Kind::String&&
            Object(deps_,p.targets)&&Object(deps_,p.libraries);
        if(!p.rootShapesObserved)return false;
        p.namedTarget=deps_.FieldForOriginalCString(p.targets,p.runtimeTargetName);
        if(!Object(deps_,p.namedTarget))return false;
        p.namedTargetObserved=true;
        const auto* tokens=deps_.PrivateTokens();
        for(SIZE_T library=p.namedTarget+1;library<tokens[p.namedTarget].subtreeEnd;){
            const SIZE_T value=tokens[library].subtreeEnd;
            if(!Object(deps_,value)||
                !Candidates(library,deps_.Field(value,"runtime"),AssetKind::Runtime)||
                !Candidates(library,deps_.Field(value,"native"),AssetKind::Native)||
                !Candidates(library,deps_.Field(value,"runtimeTargets"),AssetKind::RuntimeTarget)||
                !Candidates(library,deps_.Field(value,"resources"),AssetKind::Resource))return false;
            library=tokens[value].subtreeEnd;
        }
        // Upstream reconcile_libraries_with_targets iterates every metadata
        // occurrence. Preserve duplicates in source order; first-match candidate
        // metadata above is not substituted for this complete original roster.
        for(SIZE_T key=p.libraries+1;key<tokens[p.libraries].subtreeEnd;){
            if(libraryCount_==assetCapacity_)return false;
            auto& row=libraries_[libraryCount_++];row.key=key;row.value=tokens[key].subtreeEnd;
            row.objectObserved=Object(deps_,row.value);
            row.sha512=deps_.Field(row.value,"sha512");row.serviceable=deps_.Field(row.value,"serviceable");
            row.path=deps_.Field(row.value,"path");row.hashPath=deps_.Field(row.value,"hashPath");
            row.runtimeStoreManifestName=deps_.Field(row.value,"runtimeStoreManifestName");
            row.type=deps_.Field(row.value,"type");
            if(!row.objectObserved)return false;
            key=tokens[row.value].subtreeEnd;
        }
        p.candidateEnumerationComplete=true;
        return true;
    }
    const Document& PrivateRuntimeConfigOriginal() const noexcept{return configOriginal_;}
    const Document& PrivateDepsOriginal() const noexcept{return depsOriginal_;}
    const Document& PrivateDevConfigOriginal() const noexcept{return devOriginal_;}
    const OriginalJson* PrivateDevConfigSyntax() const noexcept{return devConfig_.get();}
    const OptionStage& PrivateDevOptionStage() const noexcept{return devOptions_;}
    const OptionStage& PrivateMainOptionStage() const noexcept{return mainOptions_;}
    const std::array<OrderedOptionSource,2>& PrivateOrderedOptionsSource() const noexcept{return orderedOptions_;}
    ConfiguredProperty ObserveConfiguredProperty(const char* originalKey) const noexcept {
        ConfiguredProperty result;
        if(!devConfig_){result.outcome=ConfiguredProperty::Outcome::InputUnobserved;}
        else if(devOriginal_.spanObserved&&!devOriginal_.syntaxObserved){
            result.outcome=ConfiguredProperty::Outcome::Absent;
        }else result=ObservePropertyStage(*devConfig_,devOptions_,originalKey,true);
        const auto main=ObservePropertyStage(runtimeConfig_,mainOptions_,originalKey,false);
        // A genuine main assignment supersedes that key from dev. Missing main
        // does not erase dev, including properties inserted before later error.
        if(main.outcome!=ConfiguredProperty::Outcome::Absent)result=main;
        return result;
    }
    const std::array<ConfiguredProperty,3>& PrivateConfiguredProperties() const noexcept{return configuredProperties_;}
    const RidGraphSourceDecision& PrivateRidGraphSourceDecision() const noexcept{return ridGraphSource_;}
    const std::array<ConfiguredProperty,8>& PrivateReservedHostProperties() const noexcept{return reservedHostProperties_;}
    const auto& PrivateReservedPropertyDecision() const noexcept{return reservedPropertyDecision_;}
    void BeginPropertyCursor(PropertyCursor& cursor) const noexcept {
        cursor=PropertyCursor{};cursor.ownerOriginal=this;
    }
    bool NextPropertyOriginal(PropertyCursor& cursor) const noexcept {
        if(cursor.ownerOriginal!=this||(cursor.outcome!=PropertyCursor::Outcome::Unstarted&&
            cursor.outcome!=PropertyCursor::Outcome::Property))return false;
        while(cursor.stage<2){
            const bool dev=cursor.stage==1;
            const OriginalJson* json=dev?devConfig_.get():&runtimeConfig_;
            const auto& options=dev?devOptions_:mainOptions_;
            if(!json){cursor.outcome=PropertyCursor::Outcome::DevInputUnsupplied;return false;}
            if(dev&&devOriginal_.spanObserved&&!devOriginal_.syntaxObserved){
                ++cursor.stage;cursor.next=SIZE_MAX;continue;
            }
            if(options.shape==OptionStage::Shape::Null||(dev&&options.shape==OptionStage::Shape::Missing)){
                ++cursor.stage;cursor.next=SIZE_MAX;continue;
            }
            if(!json->Complete()||options.shape!=OptionStage::Shape::Object){
                cursor.outcome=PropertyCursor::Outcome::UnsupportedStage;return false;
            }
            const SIZE_T object=options.configProperties;
            if(object==SIZE_MAX){++cursor.stage;cursor.next=SIZE_MAX;continue;}
            const auto* tokens=json->PrivateTokens();
            if(object>=json->Count()||tokens[object].kind!=OriginalJson::Kind::Object){
                cursor.outcome=PropertyCursor::Outcome::UnsupportedStage;return false;
            }
            const SIZE_T end=tokens[object].subtreeEnd;
            if(cursor.next==SIZE_MAX)cursor.next=object+1;
            if(cursor.next<object+1||cursor.next>end){cursor.outcome=PropertyCursor::Outcome::InvalidOriginal;return false;}
            if(cursor.next==end){++cursor.stage;cursor.next=SIZE_MAX;continue;}
            SIZE_T registered=object+1;
            while(registered<cursor.next){
                const SIZE_T registeredValue=tokens[registered].subtreeEnd;
                if(registeredValue>=end){cursor.outcome=PropertyCursor::Outcome::InvalidOriginal;return false;}
                registered=tokens[registeredValue].subtreeEnd;
            }
            if(registered!=cursor.next){cursor.outcome=PropertyCursor::Outcome::InvalidOriginal;return false;}
            const SIZE_T key=cursor.next,value=tokens[key].subtreeEnd;
            if(value>=end||tokens[key].kind!=OriginalJson::Kind::String){
                cursor.outcome=PropertyCursor::Outcome::InvalidOriginal;return false;
            }
            cursor.next=tokens[value].subtreeEnd;
            bool superseded=false;
            // A later identical first-NUL key replaces this entry in m_properties.
            for(SIZE_T later=cursor.next;later<end;){
                const SIZE_T laterValue=tokens[later].subtreeEnd;
                if(laterValue>=end||tokens[later].kind!=OriginalJson::Kind::String){
                    cursor.outcome=PropertyCursor::Outcome::InvalidOriginal;return false;
                }
                const auto same=CompareOriginalKeys(*json,key,*json,later);
                if(same==OriginalKeyComparison::Invalid){cursor.outcome=PropertyCursor::Outcome::InvalidOriginal;return false;}
                if(same==OriginalKeyComparison::Equal){superseded=true;break;}
                later=tokens[laterValue].subtreeEnd;
            }
            if(superseded)continue;
            // Any main assignment replaces the matching dev key, independently
            // of the source iteration order and later dev parse_opts failure.
            if(dev&&mainOptions_.configProperties!=SIZE_MAX){
                const auto* main=runtimeConfig_.PrivateTokens();const SIZE_T mainObject=mainOptions_.configProperties;
                if(mainObject>=runtimeConfig_.Count()||main[mainObject].kind!=OriginalJson::Kind::Object){
                    cursor.outcome=PropertyCursor::Outcome::InvalidOriginal;return false;
                }
                for(SIZE_T candidate=mainObject+1;candidate<main[mainObject].subtreeEnd;){
                    const SIZE_T candidateValue=main[candidate].subtreeEnd;
                    if(candidateValue>=main[mainObject].subtreeEnd||main[candidate].kind!=OriginalJson::Kind::String){
                        cursor.outcome=PropertyCursor::Outcome::InvalidOriginal;return false;
                    }
                    const auto same=CompareOriginalKeys(*json,key,runtimeConfig_,candidate);
                    if(same==OriginalKeyComparison::Invalid){cursor.outcome=PropertyCursor::Outcome::InvalidOriginal;return false;}
                    if(same==OriginalKeyComparison::Equal){superseded=true;break;}
                    candidate=main[candidateValue].subtreeEnd;
                }
            }
            if(superseded)continue;
            if(cursor.properties==SIZE_MAX){cursor.outcome=PropertyCursor::Outcome::Overflow;return false;}
            ConfiguredProperty property;property.jsonOriginal=json;property.key=key;property.value=value;property.fromDev=dev;
            const auto kind=tokens[value].kind;property.literalKind=kind;
            property.outcome=kind==OriginalJson::Kind::String?ConfiguredProperty::Outcome::String:
                (kind==OriginalJson::Kind::True||kind==OriginalJson::Kind::False||kind==OriginalJson::Kind::Null)?
                ConfiguredProperty::Outcome::JsonLiteral:ConfiguredProperty::Outcome::NeedsHostWriter;
            cursor.last=property;++cursor.properties;cursor.outcome=PropertyCursor::Outcome::Property;return true;
        }
        cursor.outcome=PropertyCursor::Outcome::Complete;return false;
    }
    const PropertyCursor& PrivatePropertyTraversalOriginal() const noexcept{return propertyTraversal_;}
    const SetAppPathsSourceDecision& PrivateSetAppPathsSourceDecision() const noexcept{return setAppPathsSource_;}
    void BeginProbeCursor(ProbeCursor& cursor) const noexcept {
        cursor=ProbeCursor{};cursor.ownerOriginal=this;
    }
    bool NextProbeOriginal(ProbeCursor& cursor) const noexcept {
        if(cursor.ownerOriginal!=this||
            (cursor.outcome!=ProbeCursor::Outcome::Unstarted&&cursor.outcome!=ProbeCursor::Outcome::Path))return false;
        while(cursor.stage<2){
            const bool dev=cursor.stage==1;
            const OriginalJson* json=dev?devConfig_.get():&runtimeConfig_;
            const auto& options=dev?devOptions_:mainOptions_;
            if(!json){cursor.outcome=ProbeCursor::Outcome::DevInputUnsupplied;return false;}
            if(dev&&devOriginal_.spanObserved&&!devOriginal_.syntaxObserved){
                // Failed whole JSON parse returns before upstream parse_opts;
                // retain the failed original while applying main config later.
                ++cursor.stage;cursor.stageEntered=false;continue;
            }
            if(options.shape==OptionStage::Shape::Null||
                (dev&&options.shape==OptionStage::Shape::Missing)){
                ++cursor.stage;cursor.stageEntered=false;continue;
            }
            if(options.shape!=OptionStage::Shape::Object){cursor.outcome=ProbeCursor::Outcome::UnsupportedStage;return false;}
            const auto* tokens=json->PrivateTokens();
            if(options.configProperties!=SIZE_MAX&&tokens[options.configProperties].kind!=OriginalJson::Kind::Object){
                cursor.outcome=ProbeCursor::Outcome::UnsupportedStage;return false;
            }
            const SIZE_T probe=options.additionalProbingPaths;
            if(probe==SIZE_MAX){++cursor.stage;cursor.stageEntered=false;continue;}
            const auto kind=tokens[probe].kind;
            if(kind!=OriginalJson::Kind::String&&kind!=OriginalJson::Kind::Array){
                cursor.outcome=ProbeCursor::Outcome::InvalidPath;return false;
            }
            if(!cursor.stageEntered){
                if(kind==OriginalJson::Kind::Array){
                    // GetString on a non-string is not a supported normal
                    // upstream branch. Do not report a forward prefix as the
                    // actual partial reverse-insertion result of that failure.
                    for(SIZE_T item=probe+1;item<tokens[probe].subtreeEnd;item=tokens[item].subtreeEnd)
                        if(tokens[item].kind!=OriginalJson::Kind::String){
                            cursor.next=item;cursor.outcome=ProbeCursor::Outcome::InvalidPath;return false;
                        }
                }
                cursor.next=kind==OriginalJson::Kind::Array?probe+1:probe;cursor.stageEntered=true;
            }
            const SIZE_T end=kind==OriginalJson::Kind::Array?tokens[probe].subtreeEnd:probe+1;
            if(cursor.next<(kind==OriginalJson::Kind::Array?probe+1:probe)||cursor.next>end){
                cursor.outcome=ProbeCursor::Outcome::InvalidPath;return false;
            }
            if(cursor.next>=end){++cursor.stage;cursor.stageEntered=false;continue;}
            const SIZE_T path=cursor.next;
            if(tokens[path].kind!=OriginalJson::Kind::String){cursor.outcome=ProbeCursor::Outcome::InvalidPath;return false;}
            if(cursor.paths==SIZE_MAX){cursor.outcome=ProbeCursor::Outcome::Overflow;return false;}
            cursor.next=tokens[path].subtreeEnd;++cursor.paths;
            cursor.last={json,path,dev};cursor.outcome=ProbeCursor::Outcome::Path;return true;
        }
        cursor.outcome=ProbeCursor::Outcome::Complete;return false;
    }
    const ProbeCursor& PrivateProbeTraversalOriginal() const noexcept{return probeTraversal_;}
    const Projection& PrivateProjection() const noexcept{return projection_;}
    const OriginalJson& PrivateRuntimeConfigSyntax() const noexcept{return runtimeConfig_;}
    SIZE_T RetainedAssetCapacityOriginal() const noexcept{return assets_?assetCapacity_:0;}
    SIZE_T RetainedLibraryCapacityOriginal() const noexcept{return libraries_?assetCapacity_:0;}
    const OriginalJson& PrivateDepsSyntax() const noexcept{return deps_;}
    const AssetCandidate* PrivateAssetCandidates() const noexcept{return assets_.get();}
    const LibraryMetadataOccurrence* PrivateLibraryMetadataOccurrences() const noexcept{return libraries_.get();}
    SIZE_T LibraryMetadataOccurrenceCount() const noexcept{return libraryCount_;}
    SIZE_T AssetCandidateCount() const noexcept{return assetCount_;}
    VersionPreference ObserveOwnedAssetVersionPreference(SIZE_T candidate,SIZE_T existing) const noexcept {
        if(!projection_.candidateEnumerationComplete||candidate>=assetCount_||existing>=assetCount_){
            VersionPreference result;result.outcome=VersionPreference::Outcome::OriginalInvalid;return result;
        }
        const auto& next=assets_[candidate];const auto& previous=assets_[existing];
        if(next.sourceName.crtCaseCorrespondenceNeeded||previous.sourceName.crtCaseCorrespondenceNeeded){
            VersionPreference result;result.outcome=VersionPreference::Outcome::CrtCaseUnresolved;return result;
        }
        if(!SameSourceAssetName(deps_,next.sourceName,previous.sourceName)){
            VersionPreference result;result.outcome=VersionPreference::Outcome::DifferentSourceName;return result;
        }
        return ObserveVersionPreference(next.assemblyVersionSource,next.fileVersionSource,
            previous.assemblyVersionSource,previous.fileVersionSource);
    }
    // SAME retained originals are traversed in metadata-major, asset-minor order.
    // No materialized Cartesian roster and no claimed physical allowance.
    // Provider resolution must consume these originals separately; this records
    // complete input correspondence traversal only, never selected-provider proof.
    void ReconcileOriginalLibraryOccurrences() noexcept {
        if(libraryJoinAttempted_)return;libraryJoinAttempted_=true;
        libraryCursor_.attempted=true;
        if(!projection_.candidateEnumerationComplete)return;
        for(;libraryCursor_.library<libraryCount_;++libraryCursor_.library){
            for(;libraryCursor_.asset<assetCount_;++libraryCursor_.asset){
                if(!deps_.SameOriginalCString(libraries_[libraryCursor_.library].key,
                    assets_[libraryCursor_.asset].libraryKey))continue;
                if(libraryCursor_.pairs==SIZE_MAX){libraryCursor_.overflow=true;return;}
                ++libraryCursor_.pairs;
            }
            libraryCursor_.asset=0;
        }
        libraryAssetCount_=libraryCursor_.pairs;
        libraryCursor_.complete=true;libraryJoinComplete_=true;
    }
    const void* PrivateLibraryTraversalOriginal() const noexcept{return &libraryCursor_;}
    SIZE_T LibraryTraversalOriginalBytes() const noexcept{return sizeof(libraryCursor_);}
    SIZE_T LibraryAssetJoinCount() const noexcept{return libraryAssetCount_;}
    bool LibraryAssetJoinComplete() const noexcept{return libraryJoinComplete_;}
    SIZE_T ReservedSourceBytes() const noexcept {
        return sizeof(*this)+runtimeConfig_.ReservedSourceBytes()-sizeof(runtimeConfig_)+
            deps_.ReservedSourceBytes()-sizeof(deps_)+(assets_?assetCapacity_*sizeof(AssetCandidate):0)+
            (libraries_?assetCapacity_*sizeof(LibraryMetadataOccurrence):0)+
            (devConfig_?devConfig_->ReservedSourceBytes():0); // cursor is inline; dev parser is separately owned

    }
    // No positive admission/result API: apphost embedded path, hostfxr selection,
    // supplied ENV/shared frameworks/hostpolicy, TPA and PowerShell provider
    // resolution still require actual same-source/runtime/occurrence joins.
};
}

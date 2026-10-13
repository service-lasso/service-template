// UNRUN pure source-model cases, not installed CLR/compiler/native evidence.
#include "windows_runtime_host_configuration.hpp"
#include <cstring>
namespace template_creator {
bool OriginalWindowsHostVersionSourceRegression() {
    using Model=OriginalRuntimeHostConfiguration::WindowsVersionSource;
    struct Case {const char* json;Model::Outcome outcome;std::array<std::uint32_t,4> values;bool canonical,abi;};
    const Case cases[]={
        {"\"1.2\"",Model::Outcome::Parsed,{1,2,UINT32_MAX,UINT32_MAX},true,false},
        {"\"1.2.3.4\"",Model::Outcome::Parsed,{1,2,3,4},true,false},
        {"\"01.2\"",Model::Outcome::Parsed,{1,2,UINT32_MAX,UINT32_MAX},false,false},
        {"\"1.2\\u0000ignored\"",Model::Outcome::Parsed,{1,2,UINT32_MAX,UINT32_MAX},true,false},
        {"\"4294967295.2\"",Model::Outcome::Parsed,{UINT32_MAX,2,UINT32_MAX,UINT32_MAX},false,true},
        {"\"\"",Model::Outcome::Empty,{UINT32_MAX,UINT32_MAX,UINT32_MAX,UINT32_MAX},true,false},
        {"123",Model::Outcome::Empty,{UINT32_MAX,UINT32_MAX,UINT32_MAX,UINT32_MAX},true,false},
        {"\"1\"",Model::Outcome::Invalid,{UINT32_MAX,UINT32_MAX,UINT32_MAX,UINT32_MAX},true,false},
        {"\"4294967296\"",Model::Outcome::Invalid,{UINT32_MAX,UINT32_MAX,UINT32_MAX,UINT32_MAX},true,false},
        {"\"4294967296.2\"",Model::Outcome::UnsignedConversionWouldThrow,{UINT32_MAX,UINT32_MAX,UINT32_MAX,UINT32_MAX},true,false},
        {"\"4294967296x.2\"",Model::Outcome::Invalid,{UINT32_MAX,UINT32_MAX,UINT32_MAX,UINT32_MAX},true,false},
        {"\"1.2.3.4294967296.5\"",Model::Outcome::Invalid,{UINT32_MAX,UINT32_MAX,UINT32_MAX,UINT32_MAX},true,false},
        {"\"1.2.3.4294967296\"",Model::Outcome::UnsignedConversionWouldThrow,{UINT32_MAX,UINT32_MAX,UINT32_MAX,UINT32_MAX},true,false},
        {"\"1..2\"",Model::Outcome::Invalid,{UINT32_MAX,UINT32_MAX,UINT32_MAX,UINT32_MAX},true,false},
        {"\"-1.2\"",Model::Outcome::Invalid,{UINT32_MAX,UINT32_MAX,UINT32_MAX,UINT32_MAX},true,false}
    };
    for(const auto& test:cases){
        OriginalJson json(std::strlen(test.json)+1,1);
        if(!json.Capture(reinterpret_cast<const unsigned char*>(test.json),std::strlen(test.json)))return false;
        const auto observed=OriginalRuntimeHostConfiguration::ObserveWindowsVersionSource(json,0);
        if(observed.token!=0||observed.outcome!=test.outcome||observed.components!=test.values||
            observed.debugCanonical!=test.canonical||observed.signedAbiCorrespondenceNeeded!=test.abi)return false;
    }
    using Preference=OriginalRuntimeHostConfiguration::VersionPreference;
    struct Pair {const char* assembly;const char* file;const char* previousAssembly;const char* previousFile;
        Preference::Outcome outcome;int assemblyOrder,fileOrder;bool fileCompared,debugNeeded;};
    const Pair pairs[]={
        {"\"2.0\"","\"0.0\"","\"1.9\"","\"999.0\"",Preference::Outcome::ProbeCandidate,1,0,false,false},
        {"\"1.0\"","\"999.0\"","\"2.0\"","\"0.0\"",Preference::Outcome::KeepExisting,-1,0,false,false},
        {"\"1.0\"","\"2.0\"","\"1.0\"","\"2.0\"",Preference::Outcome::ProbeCandidate,0,0,true,false},
        {"\"1.0\"","\"1.9\"","\"1.0\"","\"2.0\"",Preference::Outcome::KeepExisting,0,-1,true,false},
        {"\"1.0\"","\"2.1\"","\"1.0\"","\"2.0\"",Preference::Outcome::ProbeCandidate,0,1,true,false},
        {"\"1.0\"","\"\"","\"1.0.0\"","\"\"",Preference::Outcome::KeepExisting,-1,0,false,false},
        {"\"invalid\"","\"\"","\"\"","\"\"",Preference::Outcome::ProbeCandidate,0,0,true,false},
        {"\"\"","\"\"","\"0.0\"","\"\"",Preference::Outcome::KeepExisting,-1,0,false,false},
        {"\"01.0\"","\"\"","\"1.0\"","\"\"",Preference::Outcome::ProbeCandidate,0,0,true,true},
        {"\"4294967296.0\"","\"\"","\"1.0\"","\"\"",Preference::Outcome::ConversionWouldThrow,0,0,false,false},
        {"\"4294967295.0\"","\"\"","\"1.0\"","\"\"",Preference::Outcome::SignedAbiUnresolved,0,0,false,false}
    };
    const auto model=[](const char* body){
        OriginalJson json(std::strlen(body)+1,1);
        if(!json.Capture(reinterpret_cast<const unsigned char*>(body),std::strlen(body))){
            Model failed;failed.outcome=Model::Outcome::InvalidOriginal;return failed;
        }
        return OriginalRuntimeHostConfiguration::ObserveWindowsVersionSource(json,0);
    };
    for(const auto& pair:pairs){
        const auto result=OriginalRuntimeHostConfiguration::ObserveVersionPreference(model(pair.assembly),model(pair.file),
            model(pair.previousAssembly),model(pair.previousFile));
        if(result.outcome!=pair.outcome||result.assemblyOrder!=pair.assemblyOrder||result.fileOrder!=pair.fileOrder||
            result.fileCompared!=pair.fileCompared||result.debugBuildCorrespondenceNeeded!=pair.debugNeeded)return false;
    }
    Model invalid;invalid.outcome=Model::Outcome::InvalidOriginal;
    if(OriginalRuntimeHostConfiguration::ObserveVersionPreference(invalid,Model{},Model{},Model{}).outcome!=
        Preference::Outcome::OriginalInvalid)return false;
    struct Names {const char* json;bool same;SIZE_T leftExtent,rightExtent;bool leftNi,rightNi;bool leftCrt=false,rightCrt=false;};
    const Names names[]={
        {"[\"folder/A.ni.dll\",\"other/A.dll\"]",true,1,1,true,false},
        {"[\"folder/A.NI.dll\",\"A.dll\"]",true,1,1,true,false,true,false},
        {"[\"A.dll\",\"a.dll\"]",false,1,1,false,false},
        {"[\"directory.dot/A\",\"A.dll\"]",true,1,1,false,false},
        {"[\"directory\\\\A.dll\",\"A.dll\"]",true,1,1,false,false},
        {"[\"A.dll\\u0000ignored/Z.dll\",\"A.dll\"]",true,1,1,false,false},
        {"[\".dll\",\"\"]",true,0,0,false,false},
        {"[\"A.ni.dll\",\"A.ni.exe\"]",true,1,1,true,true},
        {"[\"\\ud83d\\ude00.dll\",\"x/\\ud83d\\ude00.dll\"]",true,1,1,false,false},
        {"[\"A.nix.dll\",\"A.dll\"]",false,5,1,false,false}
    };
    for(const auto& test:names){
        OriginalJson json(std::strlen(test.json)+1,2);
        if(!json.Capture(reinterpret_cast<const unsigned char*>(test.json),std::strlen(test.json)))return false;
        const auto left=OriginalRuntimeHostConfiguration::ObserveSourceAssetName(json,1);
        const auto right=OriginalRuntimeHostConfiguration::ObserveSourceAssetName(json,2);
        if(!left.observed||!right.observed||left.extent!=test.leftExtent||right.extent!=test.rightExtent||
            left.niSuffixRemoved!=test.leftNi||right.niSuffixRemoved!=test.rightNi||
            left.crtCaseCorrespondenceNeeded!=test.leftCrt||right.crtCaseCorrespondenceNeeded!=test.rightCrt||
            OriginalRuntimeHostConfiguration::SameSourceAssetName(json,left,right)!=test.same)return false;
    }
    using Stage=OriginalRuntimeHostConfiguration::OptionStage;
    struct Options {const char* json;Stage::Shape shape;bool legacy,frameworks;};
    const Options options[]={
        {"{}",Stage::Shape::Missing,false,false},
        {"null",Stage::Shape::RootUnsupported,false,false},
        {"{\"runtimeOptions\":null}",Stage::Shape::Null,false,false},
        {"{\"runtimeOptions\":true}",Stage::Shape::Other,false,false},
        {"{\"runtimeOptions\":{}}",Stage::Shape::Object,false,false},
        {"{\"runtimeOptions\":null,\"runtimeOptions\":{}}",Stage::Shape::Null,false,false},
        {"{\"runtimeOptions\":{\"rollForwardOnNoCandidateFx\":2,\"frameworks\":[]}}",Stage::Shape::Object,true,true},
        {"/* original host comments */{\"runtimeOptions\":{}} trailing",Stage::Shape::Object,false,false}
    };
    for(const auto& test:options){
        OriginalJson json(std::strlen(test.json)+1,4,OriginalJson::Mode::FirstRootCommentsFirstMember);
        if(!json.Capture(reinterpret_cast<const unsigned char*>(test.json),std::strlen(test.json)))return false;
        const auto stage=OriginalRuntimeHostConfiguration::ObserveOptionStage(json);
        if(!stage.attempted||stage.shape!=test.shape||(stage.rollForwardOnNoCandidateFx!=SIZE_MAX)!=test.legacy||
            (stage.frameworks!=SIZE_MAX)!=test.frameworks)return false;
    }
    using Property=OriginalRuntimeHostConfiguration::ConfiguredProperty;
    struct Properties {const char* json;Property::Outcome outcome;const char* value;};
    const Properties properties[]={
        {"{\"runtimeOptions\":{}}",Property::Outcome::Absent,nullptr},
        {"{\"runtimeOptions\":null}",Property::Outcome::Absent,nullptr},
        {"{\"runtimeOptions\":{\"configProperties\":{\"STARTUP_HOOKS\":\"first\",\"STARTUP_HOOKS\":\"last\"}}}",Property::Outcome::String,"last"},
        {"{\"runtimeOptions\":{\"configProperties\":{\"STARTUP_HOOKS\\u0000suffix\":\"nul-key\"}}}",Property::Outcome::String,"nul-key"},
        {"{\"runtimeOptions\":{\"configProperties\":{\"startup_hooks\":\"different-key\"}}}",Property::Outcome::Absent,nullptr},
        {"{\"runtimeOptions\":{\"configProperties\":{\"STARTUP_HOOKS\":true}}}",Property::Outcome::JsonLiteral,nullptr},
        {"{\"runtimeOptions\":{\"configProperties\":{\"STARTUP_HOOKS\":false}}}",Property::Outcome::JsonLiteral,nullptr},
        {"{\"runtimeOptions\":{\"configProperties\":{\"STARTUP_HOOKS\":null}}}",Property::Outcome::JsonLiteral,nullptr},
        {"{\"runtimeOptions\":{\"configProperties\":{\"STARTUP_HOOKS\":1.5}}}",Property::Outcome::NeedsHostWriter,nullptr},
        {"{\"runtimeOptions\":{\"configProperties\":[]}}",Property::Outcome::UnsupportedStage,nullptr},
        {"{\"runtimeOptions\":{\"configProperties\":{\"STARTUP_HOOKS\":\"kept\"},\"rollForward\":\"invalid\"}}",Property::Outcome::String,"kept"}
    };
    for(const auto& test:properties){
        OriginalJson json(std::strlen(test.json)+1,4,OriginalJson::Mode::FirstRootCommentsFirstMember);
        if(!json.Capture(reinterpret_cast<const unsigned char*>(test.json),std::strlen(test.json)))return false;
        const auto stage=OriginalRuntimeHostConfiguration::ObserveOptionStage(json);
        const auto observed=OriginalRuntimeHostConfiguration::ObservePropertyStage(json,stage,"STARTUP_HOOKS",true);
        if(observed.outcome!=test.outcome||observed.jsonOriginal!=&json||!observed.fromDev||
            (test.value&&!json.StringEquals(observed.value,test.value)))return false;
        if(observed.outcome==Property::Outcome::JsonLiteral){
            const char* expected=observed.literalKind==OriginalJson::Kind::True?"true":
                observed.literalKind==OriginalJson::Kind::False?"false":"null";
            if(OriginalRuntimeHostConfiguration::CompareExactPropertySourceText(observed,expected)!=
                OriginalRuntimeHostConfiguration::PropertyTextComparison::Equal||
                OriginalRuntimeHostConfiguration::CompareExactPropertySourceText(observed,"TRUE")!=
                OriginalRuntimeHostConfiguration::PropertyTextComparison::Different)return false;
        }
    }
    using Rid=OriginalRuntimeHostConfiguration::RidGraphSourceDecision;
    struct RidCase {const char* json;Rid::Outcome expected;};
    const RidCase ridCases[]={
        {"{\"runtimeOptions\":{}}",Rid::Outcome::SourceFalse},
        {"{\"runtimeOptions\":{\"configProperties\":{\"System.Runtime.Loader.UseRidGraph\":true}}}",Rid::Outcome::SourceTrue},
        {"{\"runtimeOptions\":{\"configProperties\":{\"System.Runtime.Loader.UseRidGraph\":false}}}",Rid::Outcome::SourceFalse},
        {"{\"runtimeOptions\":{\"configProperties\":{\"System.Runtime.Loader.UseRidGraph\":null}}}",Rid::Outcome::SourceFalse},
        {"{\"runtimeOptions\":{\"configProperties\":{\"System.Runtime.Loader.UseRidGraph\":\"true\"}}}",Rid::Outcome::SourceTrue},
        {"{\"runtimeOptions\":{\"configProperties\":{\"System.Runtime.Loader.UseRidGraph\":\"TRUE\"}}}",Rid::Outcome::CaseComparisonUnresolved},
        {"{\"runtimeOptions\":{\"configProperties\":{\"System.Runtime.Loader.UseRidGraph\":1}}}",Rid::Outcome::WriterUnresolved}
    };
    for(const auto& test:ridCases){
        OriginalJson json(std::strlen(test.json)+1,4,OriginalJson::Mode::FirstRootCommentsFirstMember);
        if(!json.Capture(reinterpret_cast<const unsigned char*>(test.json),std::strlen(test.json)))return false;
        const auto property=OriginalRuntimeHostConfiguration::ObservePropertyStage(json,
            OriginalRuntimeHostConfiguration::ObserveOptionStage(json),"System.Runtime.Loader.UseRidGraph",false);
        const auto observed=OriginalRuntimeHostConfiguration::ObserveRidGraphSource(property);
        if(observed.outcome!=test.expected||observed.original.jsonOriginal!=&json||
            observed.original.value!=property.value)return false;
    }
    using AppKey=OriginalRuntimeHostConfiguration::SetAppPathsKeySource;
    struct AppKeyCase {const char* json;AppKey expected;};
    const AppKeyCase appKeys[]={
        {"{\"runtimeOptions\":{\"configProperties\":{\"Microsoft.NETCore.DotNetHostPolicy.SetAppPaths\":true}}}",AppKey::Exact},
        {"{\"runtimeOptions\":{\"configProperties\":{\"microsoft.NETCore.DotNetHostPolicy.SetAppPaths\":true}}}",AppKey::CaseCandidate},
        {"{\"runtimeOptions\":{\"configProperties\":{\"OTHER\":true}}}",AppKey::Different},
        {"{\"runtimeOptions\":{\"configProperties\":{\"M\\u0131crosoft.NETCore.DotNetHostPolicy.SetAppPaths\":true}}}",AppKey::NonAsciiOrInvalid},
        {"{\"runtimeOptions\":{\"configProperties\":{\"Microsoft.NETCore.DotNetHostPolicy.SetAppPaths\\u0000suffix\":true}}}",AppKey::Exact}
    };
    for(const auto& test:appKeys){
        OriginalJson json(std::strlen(test.json)+1,4,OriginalJson::Mode::FirstRootCommentsFirstMember);
        if(!json.Capture(reinterpret_cast<const unsigned char*>(test.json),std::strlen(test.json)))return false;
        const auto options=OriginalRuntimeHostConfiguration::ObserveOptionStage(json);
        if(options.configProperties==SIZE_MAX)return false;
        Property property;property.jsonOriginal=&json;property.key=options.configProperties+1;
        property.value=json.PrivateTokens()[property.key].subtreeEnd;
        if(OriginalRuntimeHostConfiguration::ObserveSetAppPathsKeySource(property)!=test.expected)return false;
    }
    return true;
}
}

// UNCOMPILED conditional .NET10.0.5 Windows AMD64 source consumer.
// Original source: hostpolicy/deps_format.cpp try_get_matching_rid and
// load_framework_dependent; pal.h HOST_RID_PLATFORM, configure.h.in architecture.
// https://github.com/dotnet/runtime/blob/v10.0.5/src/native/corehost/hostpolicy/deps_format.cpp
// https://github.com/dotnet/runtime/blob/v10.0.5/src/native/corehost/hostmisc/pal.h
// This does not observe compiled build constants, native getenv, selected host
// mode, fallback-graph use or an actual loaded provider.
#pragma once
#include "windows_runtime_host_configuration.hpp"
#include "native_command_plan.hpp"
namespace template_creator {
struct OriginalPortableRidSource {
    enum class SourceCase {FrameworkDependentPortableWindowsAMD64};
    SourceCase sourceCase=SourceCase::FrameworkDependentPortableWindowsAMD64;
    enum class Outcome {Unobserved,InputUnresolved,SourceComparisonUnresolved,
        OriginalInvalid,EnvironmentRid,PortableRid,NonRidFallback};
    const OriginalRuntimeHostConfiguration* configurationOriginal=nullptr;
    const template_creator_bridge::WindowsOriginalCommand* environmentOriginal=nullptr;
    SIZE_T libraryOrdinal=SIZE_MAX,assetType=SIZE_MAX,ridToken=SIZE_MAX;
    SIZE_T matchingAssets=0,portableOrdinal=SIZE_MAX;
    Outcome outcome=Outcome::Unobserved;
    bool attempted=false;
    enum class Equal {No,Yes,Invalid};
    static Equal CStringAscii(const OriginalJson& json,SIZE_T token,const char* text) noexcept {
        OriginalJson::OriginalStringCursor cursor;
        if(!text||!json.BeginOriginalString(token,cursor))return Equal::Invalid;
        SIZE_T index=0;
        for(;;){std::uint32_t scalar=0;const auto step=json.NextOriginalString(cursor,scalar);
            if(step==OriginalJson::StringStep::Invalid)return Equal::Invalid;
            if(step==OriginalJson::StringStep::End||!scalar)return text[index]?Equal::No:Equal::Yes;
            if(!text[index]||scalar!=static_cast<unsigned char>(text[index++]))return Equal::No;
        }
    }
    static Equal CStringUtf16(const OriginalJson& json,SIZE_T token,const char16_t* text,SIZE_T units) noexcept {
        OriginalJson::OriginalStringCursor cursor;
        if((!text&&units)||!json.BeginOriginalString(token,cursor))return Equal::Invalid;
        SIZE_T index=0;
        for(;;){std::uint32_t scalar=0;const auto step=json.NextOriginalString(cursor,scalar);
            if(step==OriginalJson::StringStep::Invalid)return Equal::Invalid;
            if(step==OriginalJson::StringStep::End||!scalar)return index==units?Equal::Yes:Equal::No;
            if(index==units)return Equal::No;
            std::uint32_t actual=text[index++];
            if(actual>=0xd800&&actual<=0xdbff){
                if(index==units||text[index]<0xdc00||text[index]>0xdfff)return Equal::Invalid;
                actual=0x10000+((actual-0xd800)<<10)+(text[index++]-0xdc00);
            }else if(actual>=0xdc00&&actual<=0xdfff)return Equal::Invalid;
            if(!actual||actual!=scalar)return Equal::No;
        }
    }
    template<class EnvironmentSpan>
    static OriginalPortableRidSource Observe(const OriginalRuntimeHostConfiguration& host,
        SIZE_T library,SIZE_T type,const EnvironmentSpan& env) noexcept {
        OriginalPortableRidSource result;result.attempted=true;result.configurationOriginal=&host;
        result.environmentOriginal=env.commandOriginal;result.libraryOrdinal=library;result.assetType=type;
        using EnvOutcome=typename EnvironmentSpan::Outcome;
        if(!host.PrivateProjection().candidateEnumerationComplete||library>=host.LibraryMetadataOccurrenceCount()||type>=3){
            result.outcome=Outcome::OriginalInvalid;return result;
        }
        if(!env.attempted||!env.commandOriginal||!env.commandOriginal->copied||
           env.commandOriginal->environmentUnits<2||env.commandOriginal->environmentUnits>env.commandOriginal->environment.size()||
           (env.outcome!=EnvOutcome::SourceAbsent&&env.outcome!=EnvOutcome::SourceEmpty&&env.outcome!=EnvOutcome::SourceNonempty)){
            result.outcome=Outcome::InputUnresolved;return result;
        }
        const auto& json=host.PrivateDepsSyntax();
        const auto& metadata=host.PrivateLibraryMetadataOccurrences()[library];
        const auto* assets=host.PrivateAssetCandidates();
        constexpr const char* types[]={"runtime","resources","native"};
        // The normal exact lower-case upstream spellings are supported here.
        // Other spellings remain unresolved rather than claiming CRT locale
        // case equivalence from this source-only comparison.
        const auto find=[&](const char* portable,bool environment)->bool {
            SIZE_T count=0,first=SIZE_MAX;
            for(SIZE_T i=0;i<host.AssetCandidateCount();++i){
                const auto& asset=assets[i];
                if(asset.kind!=OriginalRuntimeHostConfiguration::AssetKind::RuntimeTarget||
                   !json.SameOriginalCString(asset.libraryKey,metadata.key))continue;
                SIZE_T observedType=SIZE_MAX;
                for(SIZE_T t=0;t<3;++t)if(CStringAscii(json,asset.assetType,types[t])==Equal::Yes){observedType=t;break;}
                if(observedType==SIZE_MAX){result.outcome=Outcome::SourceComparisonUnresolved;return false;}
                if(asset.rid>=json.Count()||json.PrivateTokens()[asset.rid].kind!=OriginalJson::Kind::String){
                    result.outcome=Outcome::OriginalInvalid;return false;
                }
                if(observedType!=type)continue;
                Equal equal=Equal::No;
                if(environment){
                    const auto& command=*env.commandOriginal;
                    if(env.valueBegin>command.environmentUnits||env.valueUnits>command.environmentUnits-env.valueBegin){
                        result.outcome=Outcome::OriginalInvalid;return false;
                    }
                    equal=CStringUtf16(json,asset.rid,command.environment.data()+env.valueBegin,env.valueUnits);
                }else equal=CStringAscii(json,asset.rid,portable);
                if(equal==Equal::Invalid){result.outcome=Outcome::OriginalInvalid;return false;}
                if(equal==Equal::Yes){if(count==SIZE_MAX){result.outcome=Outcome::OriginalInvalid;return false;}
                    ++count;if(first==SIZE_MAX)first=asset.rid;}
            }
            if(!count)return false;
            result.matchingAssets=count;result.ridToken=first;return true;
        };
        if(env.outcome==EnvOutcome::SourceNonempty&&find(nullptr,true)){
            result.outcome=Outcome::EnvironmentRid;return result;
        }
        if(result.outcome!=Outcome::Unobserved)return result;
        constexpr const char* portable[]={"win-x64","win","any"};
        for(SIZE_T i=0;i<3;++i){
            if(find(portable[i],false)){result.portableOrdinal=i;result.outcome=Outcome::PortableRid;return result;}
            if(result.outcome!=Outcome::Unobserved)return result;
        }
        result.outcome=Outcome::NonRidFallback;return result;
        // NonRidFallback is the conditional source's ordinary asset-vector
        // fallback, not proof that that vector exists or resolves any path.
    }
};
}

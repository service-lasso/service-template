// UNRUN validator over retained source-owned associations supplied by harness.
// Three inputs carry all three keys absent, empty, and value "1" respectively.
// This does not observe child getenv or manufacture native association proof.
#include "windows_parser_runtime_association.hpp"
#include <cstring>
namespace template_creator {
bool OriginalFrameworkEnvironmentConsumerRegression() {
    using Association=OriginalParserRuntimeAssociation;using Host=OriginalRuntimeHostConfiguration;
    const char* text="{\"name\":\"A\",\"version\":\"1.2.3-\"}";
    OriginalJson json(std::strlen(text)+1,8,OriginalJson::Mode::FirstRootCommentsFirstMember);
    if(!json.Capture(reinterpret_cast<const unsigned char*>(text),std::strlen(text)))return false;
    unsigned mask=0;const auto framework=Host::ObserveFrameworkSettingsSource(json,0,mask,2,true,true);
    auto command=std::make_unique<template_creator_bridge::WindowsOriginalCommand>();
    const char16_t label[]=u"Major";
    for(SIZE_T i=0;i<5;++i)command->environment[i]=label[i];
    command->environmentUnits=7;command->copied=true;
    Association::RuntimeEnvironmentSourceSpan span;span.commandOriginal=command.get();span.valueBegin=0;span.valueUnits=5;
    span.outcome=decltype(span.outcome)::SourceNonempty;
    const auto result=Association::ObserveFrameworkEnvironmentSource(framework,span,true,false);
    using Outcome=Association::FrameworkEnvironmentSourceConsumer::Outcome;
    if(result.frameworkOriginal!=&framework||result.rollOriginal!=&span||!result.preferReleaseReached||
        !result.preferReleaseKnown||!result.preferRelease||!result.rollAssigned||result.rollForwardSource!=4||
        result.outcome!=Outcome::CommandOverridesPending)return false;
    const char* conflictText="{\"name\":\"A\",\"version\":\"1.2.3-\",\"rollForward\":\"Major\",\"applyPatches\":false}";
    OriginalJson conflictJson(std::strlen(conflictText)+1,8,OriginalJson::Mode::FirstRootCommentsFirstMember);
    if(!conflictJson.Capture(reinterpret_cast<const unsigned char*>(conflictText),std::strlen(conflictText)))return false;
    mask=0;const auto failed=Host::ObserveFrameworkSettingsSource(conflictJson,0,mask,2,true,true);
    const auto skipped=Association::ObserveFrameworkEnvironmentSource(failed,span,true,false);
    return skipped.preferReleaseReached&&skipped.preferReleaseKnown&&skipped.preferRelease&&
        !skipped.rollAssigned&&skipped.outcome==Outcome::EarlierStageIncomplete;
}
bool OriginalRuntimeEnvironmentIntegerSourceRegression() {
    // Deliberately synthetic text owner for pure conversion only, not ingress
    // or command/environment authority. No native calls are made by this case.
    using Association=OriginalParserRuntimeAssociation;
    struct Case {const char16_t* text;std::int32_t value;bool overflow,known;};
    const Case cases[]={
        {u"1",1,false,true},{u"  +1tail",1,false,true},{u"-0",0,false,true},
        {u"nonnumeric",0,false,true},{u"+",0,false,true},
        {u"2147483647",INT32_MAX,false,true},{u"2147483648",INT32_MAX,true,true},
        {u"-2147483648",INT32_MIN,false,true},{u"-2147483649",INT32_MIN,true,true},
        {u"\r1",0,false,false},{u"\u00a01",0,false,false}
    };
    auto command=std::make_unique<template_creator_bridge::WindowsOriginalCommand>();
    for(const auto& c:cases){
        SIZE_T units=0;while(c.text[units])++units;
        for(SIZE_T i=0;i<units;++i)command->environment[i]=c.text[i];
        command->environment[units]=command->environment[units+1]=0;
        command->environmentUnits=units+2;command->copied=true;
        Association::RuntimeEnvironmentSourceSpan span;
        span.commandOriginal=command.get();span.valueBegin=0;span.valueUnits=units;
        span.outcome=decltype(span.outcome)::SourceNonempty;
        const auto result=Association::ObserveRuntimeEnvironmentIntegerSource(span);
        if(!result.attempted||result.span.commandOriginal!=command.get()||
            result.sourceValueKnown!=c.known||result.sourceOverflow!=c.overflow||
            (c.known&&result.value!=c.value)||(!c.known&&!result.localeUnresolved))return false;
    }
    return true;
}
bool OriginalRuntimeEnvironmentSourceRegression(
    const std::array<const OriginalParserRuntimeAssociation*,3>& associations) {
    using Span=OriginalParserRuntimeAssociation::RuntimeEnvironmentSourceSpan;
    const char* keys[]={"DOTNET_ROLL_FORWARD_ON_NO_CANDIDATE_FX",
        "DOTNET_ROLL_FORWARD_TO_PRERELEASE","DOTNET_ROLL_FORWARD"};
    for(SIZE_T i=0;i<associations.size();++i){
        if(!associations[i])return false;
        for(const auto* key:keys){
            const auto original=associations[i]->ObserveRuntimeEnvironmentSource(key);
            if(!original.attempted||!original.commandOriginal||!original.commandOriginal->copied||
                original.sourceKey!=key||original.keyComparisonUnresolved||original.matchingKeys!=(i?1:0))return false;
            const auto expected=i==0?Span::Outcome::SourceAbsent:i==1?Span::Outcome::SourceEmpty:Span::Outcome::SourceNonempty;
            if(original.outcome!=expected||original.valueUnits!=(i==2?1:0))return false;
            if(i){
                const auto units=original.commandOriginal->environmentUnits;
                if(original.valueUnits>units||original.valueBegin>units-original.valueUnits)return false;
                if(i==2&&original.commandOriginal->environment[original.valueBegin]!=u'1')return false;
            }else if(original.valueBegin!=SIZE_MAX)return false;
        }
    }
    return true;
}
}

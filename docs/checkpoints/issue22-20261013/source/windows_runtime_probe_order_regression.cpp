// UNRUN source regression over actual admitted held fixture originals.
// This does not execute hostpolicy or authenticate an installed runtime.
#include "windows_runtime_host_configuration.hpp"
#include <cstring>
namespace template_creator {
bool OriginalWindowsHostProbeOrderRegression(const NativeHeldFiles& files,
    SIZE_T mainOrdinal,SIZE_T depsOrdinal,SIZE_T devOrdinal) {
    const char* fixtures[]={
        "{\"runtimeOptions\":{\"configProperties\":{\"STARTUP_HOOKS\":\"superseded\",\"STARTUP_HOOKS\":\"main-hooks\",\"RUNTIME_IDENTIFIER\":\"override\",\"Microsoft.NETCore.DotNetHostPolicy.SetAppPaths\":true},\"additionalProbingPaths\":[\"main-a\",\"main-b\"]}}",
        "{\"runtimeTarget\":\"x\",\"targets\":{\"x\":{}},\"libraries\":{}}",
        // Upstream prepends probes BEFORE rejecting this rollForward option.
        "{\"runtimeOptions\":{\"configProperties\":{\"STARTUP_HOOKS\":\"dev-hooks\",\"DEV_ONLY\":\"retained\"},\"additionalProbingPaths\":[\"dev-a\",\"dev-b\"],\"rollForward\":\"invalid\"}}"
    };
    const SIZE_T ordinals[]={mainOrdinal,depsOrdinal,devOrdinal};
    SIZE_T extents[3]{};
    for(SIZE_T i=0;i<3;++i){
        const unsigned char* body=nullptr;
        if(!files.ExactCapturedSourceSpan(ordinals[i],body,extents[i])||
            extents[i]!=std::strlen(fixtures[i])||std::memcmp(body,fixtures[i],extents[i]))return false;
    }
    SIZE_T depth=extents[0]>extents[1]?extents[0]:extents[1];if(extents[2]+1>depth)depth=extents[2]+1;
    OriginalRuntimeHostConfiguration configuration(files,extents[0],extents[1],depth,extents[2]+1);
    if(!configuration.PreownOriginal())return false;
    if(!configuration.CaptureOriginalDev(devOrdinal)||!configuration.CaptureOriginals(mainOrdinal,depsOrdinal))return false;
    OriginalRuntimeHostConfiguration::ProbeCursor cursor;configuration.BeginProbeCursor(cursor);
    const char* expected[]={"main-a","main-b","dev-a","dev-b"};
    for(SIZE_T i=0;i<4;++i){
        if(!configuration.NextProbeOriginal(cursor)||!cursor.last.jsonOriginal||cursor.last.fromDev!=(i>=2)||
            !cursor.last.jsonOriginal->StringEquals(cursor.last.token,expected[i]))return false;
    }
    if(configuration.NextProbeOriginal(cursor)||cursor.outcome!=decltype(cursor)::Outcome::Complete||cursor.paths!=4)return false;
    const auto& reserved=configuration.PrivateReservedPropertyDecision();
    const auto& collision=configuration.PrivateReservedHostProperties()[7];
    if(!reserved.attempted||reserved.inputUnresolved||reserved.sourceCollisionCount!=1||
        collision.fromDev||collision.jsonOriginal!=&configuration.PrivateRuntimeConfigSyntax()||
        !collision.jsonOriginal->StringEquals(collision.value,"override"))return false;
    OriginalRuntimeHostConfiguration::PropertyCursor properties;configuration.BeginPropertyCursor(properties);
    const char* expectedKeys[]={"STARTUP_HOOKS","RUNTIME_IDENTIFIER","Microsoft.NETCore.DotNetHostPolicy.SetAppPaths","DEV_ONLY"};
    const char* expectedValues[]={"main-hooks","override",nullptr,"retained"};
    for(SIZE_T i=0;i<4;++i){
        const auto* expectedOwner=i==3?configuration.PrivateDevConfigSyntax():&configuration.PrivateRuntimeConfigSyntax();
        if(!configuration.NextPropertyOriginal(properties)||properties.last.fromDev!=(i==3)||
            properties.last.jsonOriginal!=expectedOwner||
            !properties.last.jsonOriginal->StringEquals(properties.last.key,expectedKeys[i])||
            (expectedValues[i]&&!properties.last.jsonOriginal->StringEquals(properties.last.value,expectedValues[i])))return false;
    }
    if(configuration.NextPropertyOriginal(properties)||properties.properties!=4||
        properties.outcome!=decltype(properties)::Outcome::Complete)return false;
    const auto& appPaths=configuration.PrivateSetAppPathsSourceDecision();
    if(appPaths.outcome!=decltype(appPaths.outcome)::SourceTrue||appPaths.potentialKeyCount!=1||
        appPaths.caseVariant||appPaths.nonAsciiKey||appPaths.exactOriginal.jsonOriginal!=&configuration.PrivateRuntimeConfigSyntax())return false;
    const auto& property=configuration.PrivateConfiguredProperties()[2];
    if(property.outcome!=decltype(property.outcome)::String||property.fromDev||!property.jsonOriginal||
        !property.jsonOriginal->StringEquals(property.value,"main-hooks"))return false;
    const auto* dev=configuration.PrivateDevConfigSyntax();
    if(!dev)return false;
    const auto earlier=OriginalRuntimeHostConfiguration::ObservePropertyStage(*dev,configuration.PrivateDevOptionStage(),"STARTUP_HOOKS",true);
    if(earlier.outcome!=decltype(earlier.outcome)::String||!dev->StringEquals(earlier.value,"dev-hooks"))return false;
    // Not supplying a dev original cannot be promoted to observed absence.
    OriginalRuntimeHostConfiguration unsupplied(files,extents[0],extents[1],depth);
    if(!unsupplied.PreownOriginal())return false;
    if(!unsupplied.CaptureOriginals(mainOrdinal,depsOrdinal))return false;
    // Known main collision survives unknown dev presence; unrelated absences
    // remain unknown. Neither branch qualifies successful initialization.
    if(unsupplied.PrivateReservedPropertyDecision().sourceCollisionCount!=1||
        !unsupplied.PrivateReservedPropertyDecision().inputUnresolved)return false;
    if(unsupplied.PrivateSetAppPathsSourceDecision().outcome!=
        OriginalRuntimeHostConfiguration::SetAppPathsSourceDecision::Outcome::InputUnresolved)return false;
    unsupplied.BeginProbeCursor(cursor);
    for(SIZE_T i=0;i<2;++i)if(!unsupplied.NextProbeOriginal(cursor)||cursor.last.fromDev)return false;
    return !unsupplied.NextProbeOriginal(cursor)&&
        cursor.outcome==decltype(cursor)::Outcome::DevInputUnsupplied&&cursor.paths==2;
}
}

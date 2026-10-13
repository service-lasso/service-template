// UNRUN source-order cases, not native runtime or environment observations.
#include "windows_runtime_host_configuration.hpp"
#include <cstring>
namespace template_creator {
bool OriginalFrameworkSettingsSourceRegression() {
    using Host=OriginalRuntimeHostConfiguration;using Outcome=Host::FrameworkSettingsSource::Outcome;
    const char* original="{\"name\":\"A\",\"version\":\"1.2.3\",\"rollForward\":\"Major\",\"applyPatches\":false}";
    OriginalJson json(std::strlen(original)+1,24,OriginalJson::Mode::FirstRootCommentsFirstMember);
    if(!json.Capture(reinterpret_cast<const unsigned char*>(original),std::strlen(original)))return false;
    for(const char* malformed:{"{\"name\":\"A\",\"version\":\"1.2.3-\"}",
                               "{\"name\":\"A\",\"version\":\"1.2.3-01\"}"}){
        OriginalJson bad(std::strlen(malformed)+1,16,OriginalJson::Mode::FirstRootCommentsFirstMember);
        if(!bad.Capture(reinterpret_cast<const unsigned char*>(malformed),std::strlen(malformed)))return false;
        unsigned mask=0;const auto observed=Host::ObserveFrameworkSettingsSource(bad,0,mask,2,true,true);
        if(observed.nameVersion.versionSource.outcome!=OriginalFrameworkVersionSource::Outcome::SourceReturnedFalse||
            !observed.nameVersion.versionSource.prerelease||!observed.preferReleaseDependencyUnresolved||
            observed.outcome!=Outcome::EnvironmentAndOverridesPending)return false;
    }
    unsigned specified=0;
    const auto result=Host::ObserveFrameworkSettingsSource(json,0,specified,2,true,true);
    if(result.outcome!=Outcome::SourceReturnedFalse||result.effectiveRollForward!=4||
        !result.applyAssigned||result.applyValue||specified!=1||result.specifiedAfter!=1||
        !result.preferReleaseDependencyUnresolved||result.jsonOriginal!=&json)return false;
    // Assignment is retained before mark_specified_setting rejects the mix.
    const char* reverse="{\"name\":\"B\",\"version\":\"bad\",\"rollForward\":\"LatestPatch\"}";
    OriginalJson other(std::strlen(reverse)+1,16,OriginalJson::Mode::FirstRootCommentsFirstMember);
    if(!other.Capture(reinterpret_cast<const unsigned char*>(reverse),std::strlen(reverse)))return false;
    specified=2;
    const auto conflict=Host::ObserveFrameworkSettingsSource(other,0,specified,4,false,false);
    if(!(conflict.outcome==Outcome::SourceReturnedFalse&&conflict.effectiveRollForward==1&&
        conflict.nameVersion.versionSource.outcome==OriginalFrameworkVersionSource::Outcome::SourceReturnedFalse&&
        specified==2&&conflict.specifiedAfter==2))return false;
    const char* dev="{\"runtimeOptions\":{\"applyPatches\":false}}";
    const char* main="{\"runtimeOptions\":{\"framework\":{\"name\":\"A\",\"version\":\"1.2.3\"}}}";
    OriginalJson d(std::strlen(dev)+1,16,OriginalJson::Mode::FirstRootCommentsFirstMember);
    OriginalJson m(std::strlen(main)+1,24,OriginalJson::Mode::FirstRootCommentsFirstMember);
    if(!d.Capture(reinterpret_cast<const unsigned char*>(dev),std::strlen(dev))||
        !m.Capture(reinterpret_cast<const unsigned char*>(main),std::strlen(main)))return false;
    specified=0;bool dependent=false;
    const auto first=Host::ObserveOrderedOptionSource(d,Host::ObserveOptionStage(d),true,specified,dependent);
    const auto second=Host::ObserveOrderedOptionSource(m,Host::ObserveOptionStage(m),false,specified,dependent,&first);
    if(!(first.hasDefaultApplySource&&!first.applyPatchesSource&&second.defaultSourceInherited&&
        second.frameworkSettings.defaultApplyKnown&&!second.frameworkSettings.defaultApply&&
        !second.frameworkSettings.completeDefaultSettingsAssociated&&
        second.frameworkSettings.outcome==Outcome::EnvironmentAndOverridesPending))return false;
    Host::OrderedOptionSource seed;seed.attempted=true;seed.rollForwardSource=4;
    seed.hasDefaultApplySource=true;seed.applyPatchesSource=true;
    specified=0;dependent=false;
    const auto seededDev=Host::ObserveOrderedOptionSource(d,Host::ObserveOptionStage(d),true,specified,dependent,&seed);
    const auto seededMain=Host::ObserveOrderedOptionSource(m,Host::ObserveOptionStage(m),false,specified,dependent,&seededDev);
    return seededDev.rollForwardSource==4&&seededMain.frameworkSettings.defaultRollForward==4&&
        !seededMain.frameworkSettings.defaultApply&&seededMain.frameworkSettings.defaultApplyKnown&&
        seededMain.frameworkSettings.outcome==Outcome::EnvironmentAndOverridesPending;
}
}

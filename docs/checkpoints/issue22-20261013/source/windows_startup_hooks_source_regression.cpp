// UNRUN source-composition regression. Caller supplies associations retaining
// actual copied original command/snapshot owners: missing, empty, env-hooks.
// No command is launched here and no child getenv/native authority is inferred.
#include "windows_parser_runtime_association.hpp"
#include <cstring>
namespace template_creator {
bool OriginalWindowsStartupHooksSourceRegression(
    const std::array<const OriginalParserRuntimeAssociation*,3>& associations) {
    const char* text="{\"runtimeOptions\":{\"configProperties\":{\"STARTUP_HOOKS\":\"config-hooks\"}}}";
    OriginalJson json(std::strlen(text)+1,4,OriginalJson::Mode::FirstRootCommentsFirstMember);
    if(!json.Capture(reinterpret_cast<const unsigned char*>(text),std::strlen(text)))return false;
    const auto configured=OriginalRuntimeHostConfiguration::ObservePropertyStage(json,
        OriginalRuntimeHostConfiguration::ObserveOptionStage(json),"STARTUP_HOOKS",false);
    const char* emptyText="{\"runtimeOptions\":{\"configProperties\":{\"STARTUP_HOOKS\":\"\"}}}";
    OriginalJson emptyJson(std::strlen(emptyText)+1,4,OriginalJson::Mode::FirstRootCommentsFirstMember);
    if(!emptyJson.Capture(reinterpret_cast<const unsigned char*>(emptyText),std::strlen(emptyText)))return false;
    const auto emptyConfigured=OriginalRuntimeHostConfiguration::ObservePropertyStage(emptyJson,
        OriginalRuntimeHostConfiguration::ObserveOptionStage(emptyJson),"STARTUP_HOOKS",false);
    using Plan=OriginalParserRuntimeAssociation::StartupHooksSourcePlan;
    for(SIZE_T i=0;i<associations.size();++i){
        if(!associations[i])return false;
        const auto result=associations[i]->ObserveStartupHooksSource(configured);
        if(!result.attempted||!result.environmentOwnerOriginal||!result.environmentOwnerOriginal->copied||
            result.appConfigOriginal.jsonOriginal!=&json||result.appConfigOriginal.value!=configured.value||
            result.environmentKeyComparisonUnresolved||result.separator!=u';'||
            result.matchingEnvironmentKeys!=(i?1:0))return false;
        if(i<2){
            if(result.outcome!=Plan::Outcome::SourceConfigOnly||result.environmentValueUnits)return false;
        }else {
            const char16_t expected[]=u"env-hooks";
            if(result.outcome!=Plan::Outcome::SourceEnvThenConfig||result.environmentValueUnits!=std::size(expected)-1||
                result.environmentValueUnits>result.environmentOwnerOriginal->environmentUnits||
                result.environmentValueBegin>result.environmentOwnerOriginal->environmentUnits-result.environmentValueUnits||
                memcmp(result.environmentOwnerOriginal->environment.data()+result.environmentValueBegin,
                    expected,result.environmentValueUnits*sizeof(char16_t)))return false;
        }
        auto absent=configured;absent.outcome=decltype(absent.outcome)::Absent;
        const auto noConfig=associations[i]->ObserveStartupHooksSource(absent);
        if(noConfig.outcome!=(i==2?Plan::Outcome::SourceEnvOnly:Plan::Outcome::SourceNeither))return false;
        auto unknown=configured;unknown.outcome=decltype(unknown.outcome)::InputUnobserved;
        if(associations[i]->ObserveStartupHooksSource(unknown).outcome!=Plan::Outcome::InputUnresolved)return false;
        // Upstream appends the separator whenever config is present, including
        // an empty string; an empty source is distinct from absence.
        const auto emptyConfig=associations[i]->ObserveStartupHooksSource(emptyConfigured);
        if(emptyConfig.outcome!=(i==2?Plan::Outcome::SourceEnvThenConfig:Plan::Outcome::SourceConfigOnly)||
            emptyConfig.appConfigOriginal.jsonOriginal!=&emptyJson||emptyConfig.separator!=u';')return false;
    }
    return true;
}
}

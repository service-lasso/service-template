// UNRUN pure synthetic command/source projection, not actual host mode/argv.
#include "windows_parser_runtime_association.hpp"
namespace template_creator {
bool OriginalHelperHostOverrideSourceRegression() {
    auto observation=std::make_unique<OriginalCommandLineObservation>();
    auto scratch=std::make_unique<wchar_t[]>(32769);
    const wchar_t* cases[]={
        L"pwsh -NoLogo -NoProfile -NonInteractive -File \"C:\\--roll-forward\\test.ps1\"",
        L"pwsh --roll-forward Major -NoLogo -NoProfile -NonInteractive -File test.ps1"
    };
    for(SIZE_T i=0;i<std::size(cases);++i){
        auto helper=std::make_unique<OriginalHelperSourceProjection>();
        const SIZE_T bytes=wcslen(cases[i])*sizeof(wchar_t);
        if(bytes>observation->commandBytes.size()-sizeof(wchar_t))return false;
        observation->commandBytes.fill(0);memcpy(observation->commandBytes.data(),cases[i],bytes);
        observation->retainedCommandExtent=bytes;observation->captured=true;
        const bool complete=helper->Observe(*observation,scratch.get(),32769);
        const auto result=OriginalParserRuntimeAssociation::ObserveHelperHostOverrideSource(*helper);
        if(complete!=(i==0)||!result.attempted||result.helperOriginal!=helper.get()||
            result.commandOriginal!=observation.get()||result.actualHostModeAssociated||result.actualArgumentOffsetAssociated||
            result.sourceAssumesRunningExecutableIncluded!=(i==0)||
            result.conditionalApphostEmptyOptions!=(i==0)||result.sourceParsedHostOptions)return false;
        if(result.firstUnrecognizedArgument!=(i==0?1:SIZE_MAX))return false;
    }
    return true;
}
}

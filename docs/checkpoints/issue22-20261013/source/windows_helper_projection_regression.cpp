// UNRUN synthetic command projection only, never PowerShell/native acceptance.
#ifdef _WIN32
#include "windows_helper_source_projection.hpp"
namespace template_creator {
bool HelperProjectionRegression() {
    auto observation=std::make_unique<OriginalCommandLineObservation>();
    auto scratch=std::make_unique<wchar_t[]>(32769);
    const auto check=[&](const wchar_t* command,const wchar_t* expected,bool actions) {
        auto projection=std::make_unique<OriginalHelperSourceProjection>();
        observation->commandBytes.fill(0);
        const SIZE_T bytes=wcslen(command)*sizeof(wchar_t);
        if(bytes>observation->commandBytes.size()-sizeof(wchar_t))return false;
        memcpy(observation->commandBytes.data(),command,bytes);
        observation->retainedCommandExtent=bytes;observation->captured=true;
        const bool result=projection->Observe(*observation,scratch.get(),32769);
        return result==(expected!=nullptr)&&(!result||(!wcscmp(projection->helperPath.data(),expected)&&projection->actions==actions));
    };
    return check(L"pwsh -NoLogo -NoProfile -NonInteractive -File \"C:\\private\\scripts\\test.ps1\"",L"C:\\private\\scripts\\test.ps1",false) &&
        check(L"pwsh -NoLogo -NoProfile -NonInteractive -Command \"$ErrorActionPreference = 'Stop'; $PSNativeCommandUseErrorActionPreference = $false; & 'C:\\a''b\\test.ps1'; if ((Test-Path -LiteralPath variable:\\LASTEXITCODE)) { exit $LASTEXITCODE }\"",L"C:\\a'b\\test.ps1",true) &&
        check(L"pwsh -NoLogo -NoProfile -NonInteractive -File test.ps1 extra",nullptr,false) &&
        check(L"pwsh -NoLogo -NoProfile -NonInteractive -Command \"& 'test.ps1'\"",nullptr,false) &&
        check(L"pwsh -NoLogo -NoProfile -NonInteractive -File \"\"",nullptr,false);
}
}
#endif

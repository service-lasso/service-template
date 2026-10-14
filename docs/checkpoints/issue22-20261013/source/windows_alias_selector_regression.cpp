// UNRUN pure selector regression. Synthetic rows prove no native borrower,
// alias identity, content correspondence, filesystem exclusion or permission.
#ifdef _WIN32
#include "windows_alias_code_guard.hpp"
namespace template_creator {
bool AliasSelectorRegression() {
    auto root=std::make_unique<OriginalWindowsFixtureRoot>();
    auto command=std::make_unique<OriginalCommandLineObservation>();
    const wchar_t base[]=L"C:\\private";
    memcpy(root->environmentPath.data(),base,sizeof(base));
    root->environmentPathUnits=std::size(base)-1;root->environmentPathBound=true;
    const auto check=[&](const wchar_t* literal,bool expected,SIZE_T expectedKind,bool expectedEncoded) {
        command->nodeScript.fill(0);command->nodeScriptUnits=wcslen(literal);
        if(command->nodeScriptUnits>=command->nodeScript.size())return false;
        memcpy(command->nodeScript.data(),literal,(command->nodeScriptUnits+1)*sizeof(wchar_t));
        command->nodeScriptSeen=true;command->nodeSourceFormComplete=true;
        SIZE_T kind=SIZE_MAX,fixture=0;bool encoded=false;
        const bool selected=OriginalAliasCodeGuard::Select(*root,*command,kind,fixture,encoded);
        return selected==expected&&(!selected||(kind==expectedKind&&encoded==expectedEncoded&&fixture>22));
    };
    if(!check(L"C:\\private\\template-physical-cli-abc\\alias\\workflow.mjs",true,0,false) ||
        !check(L"C:\\private\\template-physical-cli-xyz\\alias space # percent% é\\publish-development-candidate.mjs",true,1,true) ||
        !check(L"C:\\private-other\\template-physical-cli-abc\\alias\\workflow.mjs",false,0,false) ||
        !check(L"C:\\private\\template-physical-cli-\\alias\\workflow.mjs",false,0,false) ||
        !check(L"C:\\private\\template-physical-cli-abc\\physical\\workflow.mjs",false,0,false) ||
        !check(L"C:\\private\\template-physical-cli-abc\\alias\\workflow.mjs\\data",false,0,false))return false;
    command->nodeScriptUnits=command->nodeScript.size();
    SIZE_T kind=0,fixture=0;bool encoded=false;
    return !OriginalAliasCodeGuard::Select(*root,*command,kind,fixture,encoded);
}
}
#endif

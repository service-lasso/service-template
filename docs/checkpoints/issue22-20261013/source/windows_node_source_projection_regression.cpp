// UNRUN pure source-grammar fixtures. Synthetic rows are never native evidence.
#ifdef _WIN32
#include "windows_node_source_projection.hpp"
#include <memory>
namespace template_creator {
static bool ProjectFixture(const wchar_t* text,OriginalCommandLineObservation& row) noexcept {
    const auto bytes=wcslen(text)*sizeof(wchar_t);
    if(bytes>row.commandBytes.size())return false;
    memcpy(row.commandBytes.data(),text,bytes);row.retainedCommandExtent=bytes;
    row.captured=true;row.nodeImageAssociated=true; // Deliberate pure fixture only.
    std::array<wchar_t,32769> scratch{};
    return ProjectTemplateNodeSourceForm(row,scratch.data(),scratch.size());
}
bool OriginalNodeSourceProjectionRegression() {
    // Large observation backings are heap-preowned; do not silently exceed the
    // Windows default harness stack. Allocation exceptions belong to harness.
    auto rows=std::make_unique<OriginalCommandLineObservation[]>(7);
    auto& preload=rows[0];
    if(!ProjectFixture(LR"(node.exe --import "data:text/javascript,throw new Error('x')" "C:\alias space\publisher.mjs" unrelated missing-candidate)",preload) ||
        !preload.nodePreloadSeen || !preload.nodeScriptSeen || preload.nodeEvalSeen ||
        wcscmp(preload.nodeScript.data(),LR"(C:\alias space\publisher.mjs)") ||
        wcscmp(preload.nodePreload.data(),L"data:text/javascript,throw new Error('x')"))return false;
    auto& eval=rows[1];
    if(!ProjectFixture(LR"(node.exe --input-type=module --eval "await import('file:///C:/original.mjs')")",eval) ||
        !eval.nodeEvalSeen || eval.nodeScriptSeen || eval.nodePreloadSeen ||
        wcscmp(eval.nodeEval.data(),L"await import('file:///C:/original.mjs')"))return false;
    auto& emptyEval=rows[2];
    if(!ProjectFixture(LR"(node.exe -e "")",emptyEval) || !emptyEval.nodeEvalSeen || emptyEval.nodeEvalUnits)return false;
    auto& missing=rows[3];
    if(ProjectFixture(L"node.exe --import",missing) || !missing.nodeUnsupportedForm || missing.nodeSourceFormComplete)return false;
    auto& rootTest=rows[4];
    if(ProjectFixture(L"node.exe --test tests/template-contract.test.mjs",rootTest) ||
        !rootTest.nodeUnsupportedForm || rootTest.nodeSourceFormComplete)return false;
    auto& duplicate=rows[5];
    if(ProjectFixture(L"node.exe --import a --import b script.mjs",duplicate) ||
        !duplicate.nodeUnsupportedForm || duplicate.nodeSourceFormComplete)return false;
    auto& unassociated=rows[6];
    std::array<wchar_t,8> scratch{};
    return !ProjectTemplateNodeSourceForm(unassociated,scratch.data(),scratch.size()) &&
        !unassociated.nodeSourceFormComplete;
}
}
#endif

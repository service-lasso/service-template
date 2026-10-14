// UNRUN pure grammar regressions. Synthetic input rows below are deliberately
// NOT native CREATE/argv/input authority and cannot qualify any launch.
#ifdef _WIN32
#include "windows_crt_argument_projection.hpp"
#include <cwchar>
#include <memory>
namespace template_creator {
namespace {
bool ProjectionCase(const wchar_t* input,const wchar_t* const* expected,SIZE_T count) {
    auto backing=std::make_unique<OriginalCommandLineObservation>();
    auto& synthetic=*backing;
    const SIZE_T units=wcslen(input);
    if(units*sizeof(wchar_t)>synthetic.commandBytes.size())return false;
    memcpy(synthetic.commandBytes.data(),input,units*sizeof(wchar_t));
    synthetic.retainedCommandExtent=units*sizeof(wchar_t);
    synthetic.captured=true; // Pure parser fixture only; Capture was never called.
    OriginalCrtArgumentCursor cursor(synthetic);
    std::array<wchar_t,32769> scratch{};
    OriginalCrtWord word;
    for(SIZE_T i=0;i<count;++i) {
        if(!cursor.Next(scratch.data(),scratch.size(),word) || word.ordinal!=i ||
           word.decodedUnits!=wcslen(expected[i]) || wcscmp(scratch.data(),expected[i]))return false;
    }
    return !cursor.Next(scratch.data(),scratch.size(),word) && cursor.Finished() && !cursor.Failed();
}
}
bool OriginalCrtProjectionRegression() {
    // Public Microsoft grammar examples, with a separate ordinary argv0.
    const wchar_t* quoted[]={L"node.exe",L"a b c",L"d",L"e"};
    if(!ProjectionCase(LR"(node.exe "a b c" d e)",quoted,4))return false;
    const wchar_t* joined[]={L"node.exe",LR"(a\\b)",L"de fg",L"h"};
    if(!ProjectionCase(LR"(node.exe a\\b d"e f"g h)",joined,4))return false;
    const wchar_t* escaped[]={L"node.exe",L"ab\"c",LR"(\)",L"d"};
    if(!ProjectionCase(LR"(node.exe "ab\"c" "\\" d)",escaped,4))return false;
    const wchar_t* literalSlashes[]={L"node.exe",LR"(a\\\b)",L"de fg",L"h"};
    if(!ProjectionCase(LR"(node.exe a\\\b d"e f"g h)",literalSlashes,4))return false;
    const wchar_t* oddQuote[]={L"node.exe",LR"(a\"b)",L"c",L"d"};
    if(!ProjectionCase(LR"(node.exe a\\\"b c d)",oddQuote,4))return false;
    const wchar_t* evenQuote[]={L"node.exe",LR"(a\\b c)",L"d",L"e"};
    if(!ProjectionCase(LR"(node.exe a\\\\"b c" d e)",evenQuote,4))return false;
    const wchar_t* leading[]={L"",L"node.exe",L"x"};
    if(!ProjectionCase(L" \tnode.exe x",leading,3))return false;
    const wchar_t* emptyInput[]={L""};
    if(!ProjectionCase(L"",emptyInput,1))return false;
    const wchar_t* trailingEmpty[]={L"node.exe",L""};
    if(!ProjectionCase(LR"(node.exe "")",trailingEmpty,2))return false;
    const wchar_t* empty[]={L"node path.exe",L"",L"x"};
    if(!ProjectionCase(LR"("node path.exe" "" x)",empty,3))return false;
    const wchar_t* unmatched[]={L"node.exe",L"ab\" c d"};
    if(!ProjectionCase(LR"(node.exe a"b"" c d)",unmatched,2))return false;
    auto backing=std::make_unique<OriginalCommandLineObservation>();
    auto& synthetic=*backing;
    const wchar_t body[]={L'n',L' ',L'x',0,L'z'};
    memcpy(synthetic.commandBytes.data(),body,sizeof(body));
    synthetic.retainedCommandExtent=sizeof(body);synthetic.captured=true;
    OriginalCrtArgumentCursor cursor(synthetic);
    wchar_t scratch[2]{};OriginalCrtWord word;
    if(!cursor.Next(scratch,2,word) || scratch[0]!=L'n' ||
       !cursor.Next(scratch,2,word) || scratch[0]!=L'x' ||
       !cursor.Finished() || cursor.SourceOffset()!=3 || cursor.SourceUnits()!=5)return false;
    OriginalCrtArgumentCursor tooSmall(synthetic);
    return !tooSmall.Next(scratch,1,word) && word.capacityFailure && tooSmall.Failed();
}
}
#endif

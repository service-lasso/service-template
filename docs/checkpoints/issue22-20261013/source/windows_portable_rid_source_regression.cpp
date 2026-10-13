// UNRUN/UNCOMPILED pure source comparison cases. No native ENV/build/provider
// facts or full RID-selection harness are supplied by this regression.
#include "windows_portable_rid_source.hpp"
namespace template_creator::regression {
bool PortableRidOriginalStringCases(){
    static constexpr unsigned char body[]=R"(["win-x64","win-x64\u0000ignored","x\ud83d\ude00","runtime","Runtime",""])";
    OriginalJson json(64,8);
    if(!json.Capture(body,sizeof(body)-1))return false;
    SIZE_T tokens[6]{},next=1;
    for(SIZE_T i=0;i<6;++i){if(next>=json.Count())return false;tokens[i]=next;next=json.PrivateTokens()[next].subtreeEnd;}
    using Source=OriginalPortableRidSource;using Equal=Source::Equal;
    const char16_t unicode[]={u'x',0xd83d,0xde00};
    const char16_t bad[]={u'x',0xd83d};
    return Source::CStringAscii(json,tokens[0],"win-x64")==Equal::Yes&&
        Source::CStringAscii(json,tokens[1],"win-x64")==Equal::Yes&&
        !json.SameOriginalString(tokens[0],tokens[1])&&
        Source::CStringUtf16(json,tokens[2],unicode,3)==Equal::Yes&&
        Source::CStringUtf16(json,tokens[2],bad,2)==Equal::Invalid&&
        Source::CStringAscii(json,tokens[3],"runtime")==Equal::Yes&&
        Source::CStringAscii(json,tokens[4],"runtime")==Equal::No&&
        Source::CStringUtf16(json,tokens[5],nullptr,0)==Equal::Yes&&
        Source::CStringAscii(json,SIZE_MAX,"win")==Equal::Invalid;
}
}

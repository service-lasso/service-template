// UNRUN/UNCOMPILED pure source regressions, no native host/provider proof.
#include "windows_resolver_path_source.hpp"
#include <array>
namespace template_creator {
bool OriginalResolverPathSourceRegression() noexcept {
    using P=OriginalResolverPathSource;using L=P::Layout;using A=P::Asset;
    const auto span=[](const char16_t* s)->P::Span {std::size_t n=0;while(s[n])++n;return {s,n};};
    const auto check=[&](const char16_t* relative,const char16_t* local,L layout,A asset,bool pack,
        const char16_t* libraryPath,const char16_t* expected)->bool {
        P p;std::array<char16_t,256> output{};
        if(!p.Form(span(u"C:\\app"),span(relative),span(local),span(libraryPath),span(u"pkg"),span(u"1.0"),
            layout,asset,pack,output.data(),output.size())||p.outcome!=P::Outcome::Formed)return false;
        const auto wanted=span(expected);if(p.written!=wanted.units)return false;
        for(std::size_t i=0;i<wanted.units;++i)if(output[i]!=expected[i])return false;
        return !output[p.written]&&!p.Form({}, {}, {}, {}, {}, {},layout,asset,pack,output.data(),output.size());
    };
    if(!check(u"lib/net/a.dll",u"",L::Directory,A::Runtime,false,u"",u"C:\\app\\a.dll"))return false;
    if(!check(u"runtimes/win-x64/native/a.dll",u"",L::Package,A::Native,false,u"",u"C:\\app\\runtimes\\win-x64\\native\\a.dll"))return false;
    if(!check(u"lib/net/en-US/a.resources.dll",u"",L::Directory,A::Resources,false,u"",u"C:\\app\\en-US\\a.resources.dll"))return false;
    if(!check(u"a.resources.dll",u"",L::Directory,A::Resources,false,u"",u"C:\\app\\a.resources.dll\\a.resources.dll"))return false;
    if(!check(u"lib/net/a.dll",u"",L::Directory,A::Runtime,true,u"",u"C:\\app\\lib\\net\\a.dll"))return false;
    if(!check(u"unused",u"custom/a.dll",L::Directory,A::Native,false,u"",u"C:\\app\\custom\\a.dll"))return false;
    if(!check(u"lib/a.dll",u"",L::LibraryPackage,A::Runtime,false,u"",u"C:\\app\\pkg\\1.0\\lib\\a.dll"))return false;
    if(!check(u"lib/a.dll",u"",L::LibraryPackage,A::Runtime,false,u"custom/pkg",u"C:\\app\\custom/pkg\\lib\\a.dll"))return false;
    // Absolute-looking input appends; it does not replace the source base.
    if(!check(u"/other/a.dll",u"",L::Package,A::Runtime,false,u"",u"C:\\app\\other\\a.dll"))return false;
    if(!check(u"",u"",L::Directory,A::Resources,false,u"",u"C:\\app"))return false;
    if(!check(u"///",u"",L::Directory,A::Resources,false,u"",u"C:\\app"))return false;
    if(!check(u"/en/a.dll",u"",L::Directory,A::Resources,false,u"",u"C:\\app\\en\\a.dll"))return false;
    if(!check(u"/a.dll",u"",L::Directory,A::Resources,false,u"",u"C:\\app\\a.dll"))return false;
    P small;std::array<char16_t,4> tooSmall{};
    if(small.Form(span(u"base"),span(u"a"),{}, {}, {}, {},L::Package,A::Runtime,false,tooSmall.data(),tooSmall.size())||
       small.outcome!=P::Outcome::InsufficientPrecharge)return false;
    P empty;std::array<char16_t,8> output{};
    if(empty.Form({},span(u"a"),{}, {}, {}, {},L::Package,A::Runtime,false,output.data(),output.size())||
       empty.outcome!=P::Outcome::SourceFalseEmptyBase)return false;
    // Full original culture retains its suffix, but final CString append stops
    // at first NUL before the subsequently appended file name.
    const char16_t embedded[]={u'l',u'/',u'e',0,u'x',u'/',u'a'};
    P nul;
    if(!nul.Form(span(u"b"),{embedded,7},{},{},{},{},L::Directory,A::Resources,false,output.data(),output.size())||
       nul.written!=3||output[0]!=u'b'||output[1]!=u'\\'||output[2]!=u'e'||output[3])return false;
    return true;
}
}
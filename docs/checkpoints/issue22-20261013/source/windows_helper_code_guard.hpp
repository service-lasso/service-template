// UNCOMPILED actual copied helper/echo CODE holds for one observed consumer.
// Native PowerShell image/source semantics are a separate selected-input join.
#pragma once
#include "windows_alias_code_guard.hpp"
#include "windows_helper_source_projection.hpp"
namespace template_creator {
class OriginalHelperCodeGuard {
    OriginalAliasCodeGuard helper_;
    std::unique_ptr<OriginalAliasCodeGuard> echo_;
    const OriginalHelperSourceProjection* projectionOriginal_=nullptr;
    std::array<wchar_t,32768> echoLiteral_{},echoRelative_{};
    bool attempted_=false,associated_=false;
public:
    static SIZE_T CheckedSourceBytes(SIZE_T helperRows,SIZE_T helperBytes,SIZE_T echoRows,SIZE_T echoBytes,bool test) {
        constexpr SIZE_T own=sizeof(OriginalHelperCodeGuard)-sizeof(OriginalAliasCodeGuard);
        const SIZE_T helper=OriginalAliasCodeGuard::CheckedSourceBytes(helperRows,helperBytes);
        if(helper>SIZE_MAX-own)throw std::invalid_argument("helper guard aggregate geometry");
        SIZE_T bytes=own+helper;
        if(test) {
            const SIZE_T echo=OriginalAliasCodeGuard::CheckedSourceBytes(echoRows,echoBytes);
            if(echo>SIZE_MAX-bytes)throw std::invalid_argument("helper echo aggregate geometry");
            bytes+=echo;
        }
        return bytes;
    }
    static SIZE_T CheckedHelperRows(SIZE_T helperRows,SIZE_T helperBytes,SIZE_T echoRows,SIZE_T echoBytes,bool test) {
        (void)CheckedSourceBytes(helperRows,helperBytes,echoRows,echoBytes,test);return helperRows;
    }
    OriginalHelperCodeGuard(SIZE_T helperRows,SIZE_T helperBytes,SIZE_T echoRows,SIZE_T echoBytes,bool test)
        :helper_(CheckedHelperRows(helperRows,helperBytes,echoRows,echoBytes,test),helperBytes) {
        if(test)echo_=std::make_unique<OriginalAliasCodeGuard>(echoRows,echoBytes);
    }
    static bool Select(const OriginalWindowsFixtureRoot& root,const OriginalHelperSourceProjection& projection,
        bool& test,SIZE_T& fixtureUnits) noexcept {
        if(!projection.complete || !root.environmentPathBound || projection.helperPathUnits>=projection.helperPath.size() ||
            root.environmentPathUnits>=root.environmentPath.size())return false;
        const auto* path=projection.helperPath.data();SIZE_T offset=root.environmentPathUnits;
        if(projection.helperPathUnits<=offset || path[projection.helperPathUnits] ||
            memcmp(path,root.environmentPath.data(),offset*sizeof(wchar_t)) || path[offset]!=L'\\')return false;
        const SIZE_T begin=++offset;const wchar_t prefix[]=L"template-windows-native-exit-";
        if(projection.helperPathUnits-offset<std::size(prefix)-1 ||
            memcmp(path+offset,prefix,(std::size(prefix)-1)*sizeof(wchar_t)))return false;
        while(offset<projection.helperPathUnits&&path[offset]!=L'\\')++offset;
        if(offset==projection.helperPathUnits || offset==begin+std::size(prefix)-1)return false;
        fixtureUnits=offset-begin;
        const auto* tail=path+offset;
        test=!wcscmp(tail,L"\\scripts\\test.ps1");
        return test || !wcscmp(tail,L"\\scripts\\verify.ps1");
    }
    bool Capture(const State& state,const Event& create,const OriginalCommandLineObservation& command,
        const OriginalHelperSourceProjection& projection,const OriginalWindowsFixtureRoot& root,
        const NativeHeldFiles& source,SIZE_T helperOrdinal,SIZE_T echoOrdinal) noexcept {
        if(attempted_)return false;attempted_=true;projectionOriginal_=&projection;
        bool test=false;SIZE_T fixtureUnits=0;
        if(projection.commandOriginal!=&command || !Select(root,projection,test,fixtureUnits) || test!=static_cast<bool>(echo_))return false;
        const auto* relative=projection.helperPath.data()+root.environmentPathUnits+1;
        const bool helper=helper_.Capture(state,create,command,root,source,helperOrdinal,
            projection.helperPath.data(),relative,relative,false);
        if(!helper)return false;
        if(echo_) {
            const wchar_t tail[]=L"\\runtime\\win32\\echo-service.ps1";
            const SIZE_T relativeUnits=fixtureUnits+std::size(tail)-1;
            if(relativeUnits>=echoRelative_.size() || root.environmentPathUnits+1+relativeUnits>=echoLiteral_.size())return false;
            memcpy(echoRelative_.data(),relative,fixtureUnits*sizeof(wchar_t));
            memcpy(echoRelative_.data()+fixtureUnits,tail,sizeof(tail));
            memcpy(echoLiteral_.data(),root.environmentPath.data(),root.environmentPathUnits*sizeof(wchar_t));
            echoLiteral_[root.environmentPathUnits]=L'\\';
            memcpy(echoLiteral_.data()+root.environmentPathUnits+1,echoRelative_.data(),(relativeUnits+1)*sizeof(wchar_t));
            if(!echo_->Capture(state,create,command,root,source,echoOrdinal,echoLiteral_.data(),echoRelative_.data(),echoRelative_.data(),false))return false;
        }
        associated_=true;return true;
    }
    bool BelongsTo(const State& state,const Event& create) const noexcept{return helper_.BelongsTo(state,create);}
    bool ObserveLastThreadExit(const State& state,const Event& exit) noexcept {
        const bool helper=helper_.ObserveLastThreadExit(state,exit);
        // Echo may never have acquired anything after failed helper capture.
        const bool echo=!echo_ || echo_->PhysicallyDisposed() || echo_->ObserveLastThreadExit(state,exit);
        return associated_&&helper&&echo;
    }
    bool PhysicallyDisposed() const noexcept{return helper_.PhysicallyDisposed()&&(!echo_||echo_->PhysicallyDisposed());}
    SIZE_T ReservedSourceBytes() const noexcept {
        return sizeof(*this)-sizeof(helper_)+helper_.ReservedSourceBytes()+(echo_?echo_->ReservedSourceBytes():0);
    }
    const OriginalAliasCodeGuard& PrivateHelper() const noexcept{return helper_;}
    const OriginalAliasCodeGuard* PrivateEcho() const noexcept{return echo_.get();}
};
}

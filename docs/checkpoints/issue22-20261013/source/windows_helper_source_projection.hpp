// UNCOMPILED pure projection of the unchanged fixture's two PowerShell forms.
// Authentic selected PowerShell/CRT consumption is a separate native input join.
#pragma once
#include "windows_crt_argument_projection.hpp"
namespace template_creator {
struct OriginalHelperSourceProjection {
    const OriginalCommandLineObservation* commandOriginal=nullptr;
    std::array<wchar_t,32769> helperPath{};
    SIZE_T helperPathUnits=0;
    bool attempted=false,complete=false,actions=false;
    bool Observe(const OriginalCommandLineObservation& command,wchar_t* scratch,SIZE_T capacity) noexcept {
        if(attempted)return false;attempted=true;commandOriginal=&command;
        if(!command.captured || !scratch || capacity<helperPath.size())return false;
        OriginalCrtArgumentCursor cursor(command);OriginalCrtWord word;
        if(!cursor.Next(scratch,capacity,word))return false; // observed image argv0
        const wchar_t* fixed[]={L"-NoLogo",L"-NoProfile",L"-NonInteractive"};
        for(const auto* expected:fixed) {
            if(!cursor.Next(scratch,capacity,word) || wcscmp(scratch,expected))return false;
        }
        if(!cursor.Next(scratch,capacity,word))return false;
        const bool file=!wcscmp(scratch,L"-File");actions=!wcscmp(scratch,L"-Command");
        if(!file&&!actions)return false;
        if(!cursor.Next(scratch,capacity,word))return false;
        const SIZE_T units=wcslen(scratch);
        if(file) {
            if(!units || units>=helperPath.size())return false;
            memcpy(helperPath.data(),scratch,(units+1)*sizeof(wchar_t));helperPathUnits=units;
        } else {
            const wchar_t prefix[]=L"$ErrorActionPreference = 'Stop'; $PSNativeCommandUseErrorActionPreference = $false; & '";
            const wchar_t suffix[]=L"; if ((Test-Path -LiteralPath variable:\\LASTEXITCODE)) { exit $LASTEXITCODE }";
            const SIZE_T prefixUnits=std::size(prefix)-1,suffixUnits=std::size(suffix)-1;
            if(units<=prefixUnits+suffixUnits+1 || memcmp(scratch,prefix,prefixUnits*sizeof(wchar_t)))return false;
            SIZE_T offset=prefixUnits;bool closed=false;
            while(offset<units) {
                const auto c=scratch[offset++];
                if(c==L'\'') {
                    if(offset<units&&scratch[offset]==L'\''){++offset;}
                    else {closed=true;break;}
                }
                if(helperPathUnits==helperPath.size()-1)return false;
                helperPath[helperPathUnits++]=c;
            }
            if(!closed || !helperPathUnits || units-offset!=suffixUnits ||
                memcmp(scratch+offset,suffix,suffixUnits*sizeof(wchar_t)))return false;
        }
        if(cursor.Next(scratch,capacity,word) || !cursor.Valid() || !cursor.Finished() || cursor.Failed())return false;
        complete=true;return true;
    }
};
}

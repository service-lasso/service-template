// UNCOMPILED source projection of actual unchanged Template Node call forms.
// Authentic Node22.23.2 wmain/CRT/native image is a separate admitted input.
#pragma once
#include "windows_crt_argument_projection.hpp"
#include <cwchar>
namespace template_creator {
inline bool ProjectTemplateNodeSourceForm(OriginalCommandLineObservation& original,
                                         wchar_t* scratch,SIZE_T capacity) noexcept {
    if(original.nodeProjectionAttempted)return false;
    original.nodeProjectionAttempted=true;
    if(!original.nodeImageAssociated || !original.captured || !scratch)return false;
    OriginalCrtArgumentCursor cursor(original);OriginalCrtWord word;
    if(!cursor.Next(scratch,capacity,word))return false; // program-name word
    enum class Pending {None,Import,Eval,Script};Pending pending=Pending::None;
    const auto copy=[&](auto& destination,SIZE_T& units) noexcept {
        if(word.decodedUnits>=destination.size())return false;
        units=word.decodedUnits;
        memcpy(destination.data(),scratch,(units+1)*sizeof(wchar_t));return true;
    };
    while(cursor.Next(scratch,capacity,word)) {
        if(pending==Pending::Import) {
            if(original.nodePreloadSeen || !copy(original.nodePreload,original.nodePreloadUnits))break;
            original.nodePreloadSeen=true;pending=Pending::None;continue;
        }
        if(pending==Pending::Eval) {
            if(!copy(original.nodeEval,original.nodeEvalUnits))break;
            original.nodeEvalSeen=true;original.nodeSourceFormComplete=true;return true;
        }
        if(pending==Pending::Script || scratch[0]!=L'-') {
            if(!copy(original.nodeScript,original.nodeScriptUnits))break;
            original.nodeScriptSeen=true;original.nodeSourceFormComplete=true;return true;
        }
        if(!wcscmp(scratch,L"--import")){pending=Pending::Import;continue;}
        if(!wcscmp(scratch,L"--eval") || !wcscmp(scratch,L"-e")){pending=Pending::Eval;continue;}
        if(!wcscmp(scratch,L"--input-type=module"))continue;
        if(!wcscmp(scratch,L"--")){pending=Pending::Script;continue;}
        original.nodeUnsupportedForm=true;return false;
    }
    original.nodeUnsupportedForm=true;return false;
}
// Script operands, eval text and preload source stay separate from later DATA
// argv. Unknown forms remain unknown; this projector never grants continuation.
// Node source: https://raw.githubusercontent.com/nodejs/node/v22.23.2/src/node_main.cc
}

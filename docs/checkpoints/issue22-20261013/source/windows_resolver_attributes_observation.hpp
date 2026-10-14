// UNCOMPILED collector-native original, not a child resolver call transcript.
// Source seam: .NET v10.0.5 pal.windows.cpp fullpath/GetFileAttributesExW.
// Fullpath/cwd, pending original deadline and loaded identity joins belong to
// the connected probe owner; this component grants none of those relations.
#pragma once
#include <windows.h>
#include <memory>
#include <limits>
#include <stdexcept>
#include "windows_resolver_path_source.hpp"
namespace template_creator {
class OriginalResolverAttributesObservation {
    std::unique_ptr<wchar_t[]> path_;
    SIZE_T pathCapacity_=0,pathUnits_=0;
    const OriginalResolverPathSource* sourceOriginal_=nullptr;
    bool copied_=false;
public:
    struct OriginalCall {
        const wchar_t* pathOriginal=nullptr;
        WIN32_FILE_ATTRIBUTE_DATA output{};
        BOOL result=FALSE;
        DWORD error=0;
        bool attempted=false,returned=false;
    } call;
    bool copyAttempted=false,copyFailed=false;
    explicit OriginalResolverAttributesObservation(SIZE_T originalPathUnits) {
        if(originalPathUnits==std::numeric_limits<SIZE_T>::max()||
           originalPathUnits+1>(std::numeric_limits<SIZE_T>::max()-sizeof(*this))/sizeof(wchar_t))
            throw std::overflow_error("resolver original path geometry");
        pathCapacity_=originalPathUnits+1;
        path_=std::make_unique<wchar_t[]>(pathCapacity_);
    }
    OriginalResolverAttributesObservation(const OriginalResolverAttributesObservation&)=delete;
    OriginalResolverAttributesObservation& operator=(const OriginalResolverAttributesObservation&)=delete;
    OriginalResolverAttributesObservation(OriginalResolverAttributesObservation&&)=delete;
    OriginalResolverAttributesObservation& operator=(OriginalResolverAttributesObservation&&)=delete;
    bool CopyFormedOriginal(const OriginalResolverPathSource& source) noexcept {
        if(copyAttempted)return false;
        copyAttempted=true;sourceOriginal_=&source;
        if(!source.attempted||source.outcome!=OriginalResolverPathSource::Outcome::Formed||
           !source.destinationOriginal||source.written>=source.capacity||
           source.destinationOriginal[source.written]||source.written>=pathCapacity_){copyFailed=true;return false;}
        for(SIZE_T i=0;i<source.written;++i)path_[i]=static_cast<wchar_t>(source.destinationOriginal[i]);
        pathUnits_=source.written;path_[pathUnits_]=0;copied_=true;return true;
    }
    // The native session must already own this stable object/path and enroll
    // this exact call in its existing pending/deadline role before invoking.
    // This is synchronous; accessors/disposition require same-thread return or
    // a real worker join. No destructor closes or cancels a pending syscall.
    bool ObserveOriginal() noexcept {
        if(call.attempted||!copied_||copyFailed)return false;
        call.pathOriginal=path_.get();call.attempted=true;
        call.result=GetFileAttributesExW(call.pathOriginal,GetFileExInfoStandard,&call.output);
        if(!call.result)call.error=GetLastError();
        call.returned=true;
        return call.result!=FALSE;
    }
    SIZE_T ReservedSourceBytes() const noexcept{return sizeof(*this)+pathCapacity_*sizeof(wchar_t);}
    const OriginalResolverPathSource* SourceOriginal() const noexcept{return sourceOriginal_;}
    const wchar_t* PrivateOriginalPath() const noexcept{return path_.get();}
    SIZE_T PrivateOriginalPathUnits() const noexcept{return pathUnits_;}
    bool ActualCallReturned() const noexcept{return call.attempted&&call.returned;}
    // A FALSE with any error stays the actual source query outcome; callers
    // separately classify source branch versus qualification completeness.
};
}
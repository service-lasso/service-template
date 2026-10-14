// UNCOMPILED pure Microsoft C-startup projection, not actual target argv proof.
#pragma once
#include "windows_command_line_observation.hpp"
namespace template_creator {
struct OriginalCrtWord {
    SIZE_T sourceBegin=0,sourceEnd=0,decodedUnits=0,ordinal=0;
    bool returned=false,capacityFailure=false;
};
class OriginalCrtArgumentCursor {
    const OriginalCommandLineObservation& original_;
    SIZE_T units_=0,offset_=0,ordinal_=0;
    bool valid_=false,finished_=false,failed_=false;
    wchar_t Unit(SIZE_T index) const noexcept {
        wchar_t value=0;
        if(index<units_)memcpy(&value,original_.commandBytes.data()+index*sizeof(wchar_t),sizeof(value));
        return value;
    }
    static bool Space(wchar_t value) noexcept { return value==L' ' || value==L'\t'; }
public:
    explicit OriginalCrtArgumentCursor(const OriginalCommandLineObservation& original) noexcept : original_(original) {
        static_assert(sizeof(wchar_t)==2);
        valid_=original.captured && original.retainedCommandExtent%sizeof(wchar_t)==0 &&
            original.retainedCommandExtent<=original.commandBytes.size();
        units_=original.retainedCommandExtent/sizeof(wchar_t);
    }
    bool Next(wchar_t* output,SIZE_T capacity,OriginalCrtWord& word) noexcept {
        word={};word.ordinal=ordinal_;
        if(!valid_ || failed_ || finished_)return false;
        if(!output || !capacity){failed_=word.capacityFailure=true;return false;}
        if(ordinal_) {
            while(offset_<units_ && Space(Unit(offset_)))++offset_;
            if(offset_==units_ || !Unit(offset_)){finished_=true;return false;}
        }
        word.sourceBegin=offset_;
        const auto emit=[&](wchar_t value) noexcept {
            // Reserve the local NUL before writing each derived character.
            if(word.decodedUnits>=capacity-1){failed_=word.capacityFailure=true;return false;}
            output[word.decodedUnits++]=value;return true;
        };
        bool quoted=false;
        if(!ordinal_) {
            // argv0 is the program-name grammar, not the later backslash grammar.
            while(offset_<units_ && Unit(offset_)) {
                const wchar_t value=Unit(offset_);
                if(value==L'"'){quoted=!quoted;++offset_;continue;}
                if(!quoted && Space(value))break;
                if(!emit(value))return false;
                ++offset_;
            }
        } else {
            while(offset_<units_ && Unit(offset_)) {
                if(!quoted && Space(Unit(offset_)))break;
                SIZE_T slashes=0;
                while(offset_<units_ && Unit(offset_)==L'\\'){++slashes;++offset_;}
                if(offset_<units_ && Unit(offset_)==L'"') {
                    for(SIZE_T i=0;i<slashes/2;++i)if(!emit(L'\\'))return false;
                    if(slashes%2){if(!emit(L'"'))return false;++offset_;}
                    else if(quoted && offset_+1<units_ && Unit(offset_+1)==L'"') {
                        if(!emit(L'"'))return false;offset_+=2;
                    } else {quoted=!quoted;++offset_;}
                    continue;
                }
                for(SIZE_T i=0;i<slashes;++i)if(!emit(L'\\'))return false;
                if(offset_==units_ || !Unit(offset_) || (!quoted && Space(Unit(offset_))))break;
                if(!emit(Unit(offset_)))return false;
                ++offset_;
            }
        }
        word.sourceEnd=offset_;output[word.decodedUnits]=0;
        word.returned=true;++ordinal_;
        // An embedded NUL ends CRT input; original trailing bytes remain captured.
        if(offset_==units_ || !Unit(offset_))finished_=true;
        return true;
    }
    bool Valid() const noexcept { return valid_; }
    bool Failed() const noexcept { return failed_; }
    bool Finished() const noexcept { return finished_; }
    SIZE_T WordCount() const noexcept { return ordinal_; }
    SIZE_T SourceOffset() const noexcept { return offset_; }
    SIZE_T SourceUnits() const noexcept { return units_; }
};
// Callers keep original_ alive and preown the output backing before native effects.
// Reusing derived scratch does not discard any original remote input bytes.
// https://learn.microsoft.com/en-us/cpp/c-language/parsing-c-command-line-arguments?view=msvc-170
}
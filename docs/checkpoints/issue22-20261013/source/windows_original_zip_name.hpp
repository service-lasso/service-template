// UNCOMPILED decoded ZIP-name cursor. No extraction, native namespace or vendor
// authority. CP437 data: Unicode mapping table2.00 dated1996-04-24 retained
// as primary-cp437-unicode-2.txt. UTF8 follows ZIP EFS bit11, no locale guessing.
#pragma once
#include <cstddef>
#include <cstdint>
#include "windows_original_zip_crc.hpp"
namespace template_creator {
class OriginalZipNameCursor {
    const unsigned char* original_;std::size_t extent_,offset_=0;
    bool utf8_,failed_=false;
    inline static constexpr std::uint16_t Legacy[128]={0x00c7,0x00fc,0x00e9,0x00e2,0x00e4,0x00e0,0x00e5,0x00e7,0x00ea,0x00eb,0x00e8,0x00ef,0x00ee,0x00ec,0x00c4,0x00c5,0x00c9,0x00e6,0x00c6,0x00f4,0x00f6,0x00f2,0x00fb,0x00f9,0x00ff,0x00d6,0x00dc,0x00a2,0x00a3,0x00a5,0x20a7,0x0192,0x00e1,0x00ed,0x00f3,0x00fa,0x00f1,0x00d1,0x00aa,0x00ba,0x00bf,0x2310,0x00ac,0x00bd,0x00bc,0x00a1,0x00ab,0x00bb,0x2591,0x2592,0x2593,0x2502,0x2524,0x2561,0x2562,0x2556,0x2555,0x2563,0x2551,0x2557,0x255d,0x255c,0x255b,0x2510,0x2514,0x2534,0x252c,0x251c,0x2500,0x253c,0x255e,0x255f,0x255a,0x2554,0x2569,0x2566,0x2560,0x2550,0x256c,0x2567,0x2568,0x2564,0x2565,0x2559,0x2558,0x2552,0x2553,0x256b,0x256a,0x2518,0x250c,0x2588,0x2584,0x258c,0x2590,0x2580,0x03b1,0x00df,0x0393,0x03c0,0x03a3,0x03c3,0x00b5,0x03c4,0x03a6,0x0398,0x03a9,0x03b4,0x221e,0x03c6,0x03b5,0x2229,0x2261,0x00b1,0x2265,0x2264,0x2320,0x2321,0x00f7,0x2248,0x00b0,0x2219,0x00b7,0x221a,0x207f,0x00b2,0x25a0,0x00a0};
public:
    OriginalZipNameCursor(const unsigned char* original,std::size_t extent,bool utf8) noexcept
        :original_(original),extent_(extent),utf8_(utf8){failed_=!original&&extent;}
    bool Next(std::uint32_t& scalar) noexcept {
        if(failed_||offset_==extent_)return false;
        auto first=original_[offset_++];
        if(!utf8_){scalar=first<128?first:Legacy[first-128];return true;}
        if(first<128){scalar=first;return true;}
        unsigned continuation=0;std::uint32_t minimum=0;
        if(first>=0xc2&&first<=0xdf){continuation=1;minimum=0x80;scalar=first&31;}
        else if(first>=0xe0&&first<=0xef){continuation=2;minimum=0x800;scalar=first&15;}
        else if(first>=0xf0&&first<=0xf4){continuation=3;minimum=0x10000;scalar=first&7;}
        else {failed_=true;return false;}
        if(continuation>extent_-offset_){failed_=true;return false;}
        for(unsigned i=0;i<continuation;++i){const auto byte=original_[offset_++];
            if((byte&0xc0)!=0x80){failed_=true;return false;}scalar=(scalar<<6)|(byte&63);}
        if(scalar<minimum||scalar>0x10ffff||(scalar>=0xd800&&scalar<=0xdfff)){failed_=true;return false;}
        return true;
    }
    bool CompleteOriginal() const noexcept{return !failed_&&offset_==extent_;}
    bool FailedOriginal() const noexcept{return failed_;}
    std::size_t OriginalOffset() const noexcept{return offset_;}
};
class OriginalZipNameSelection {
    const unsigned char* original_=nullptr;std::size_t extent_=0,name_=0,nameBytes_=0;
    std::size_t selected_=0,selectedBytes_=0;bool utf8_=false,attempted_=false,complete_=false,failed_=false;
    bool unicodeObserved_=false,unicodeVersionSupported_=false,unicodeCrcMatched_=false,encodingUnsupported_=false;
    OriginalZipCrc nameCrc_;
    static unsigned U16(const unsigned char* p) noexcept{return unsigned(p[0])|(unsigned(p[1])<<8);}
    static std::uint32_t U32(const unsigned char* p) noexcept{return U16(p)|(std::uint32_t(U16(p+2))<<16);}
public:
    bool Capture(const unsigned char* original,std::size_t extent,std::size_t name,std::size_t nameBytes,
        std::size_t extra,std::size_t extraBytes,bool efs) noexcept {
        if(attempted_)return false;attempted_=true;original_=original;extent_=extent;name_=name;nameBytes_=nameBytes;
        if(!original||name>extent||nameBytes>extent-name||extra>extent||extraBytes>extent-extra){failed_=true;return false;}
        selected_=name;selectedBytes_=nameBytes;utf8_=efs;
        const auto end=extra+extraBytes;
        for(auto p=extra;p<end;){
            if(end-p<4){failed_=true;return false;}
            const auto tag=U16(original+p),bytes=U16(original+p+2);p+=4;
            if(bytes>end-p){failed_=true;return false;}
            if(tag==8)encodingUnsupported_=true; // Undefined alternate encoding, never guess.
            if(tag==0x7075){
                if(unicodeObserved_||!bytes){failed_=true;return false;}unicodeObserved_=true;
                unicodeVersionSupported_=original[p]==1;
                if(!unicodeVersionSupported_){p+=bytes;continue;}
                if(bytes<5){failed_=true;return false;}
                if(!nameCrc_.Capture(original+name,nameBytes)){failed_=true;return false;}
                unicodeCrcMatched_=nameCrc_.OriginalResult()==U32(original+p+1);
                if(!efs&&unicodeVersionSupported_&&unicodeCrcMatched_){selected_=p+5;selectedBytes_=bytes-5;utf8_=true;}
            }
            p+=bytes;
        }
        if(encodingUnsupported_||!selectedBytes_){failed_=true;return false;}
        OriginalZipNameCursor cursor(original+selected_,selectedBytes_,utf8_);std::uint32_t scalar=0;
        bool first=true;while(cursor.Next(scalar)){
            // ZIP relative path syntax only. Windows object/case/alias joins
            // remain independent; no extraction or normalization occurs here.
            if(!scalar||scalar==L'\\'||scalar==L':'||(first&&scalar==L'/')){failed_=true;return false;}
            first=false;
        }
        complete_=cursor.CompleteOriginal();failed_=!complete_;return complete_;
    }
    bool CompleteOriginal() const noexcept{return complete_&&!failed_;}
    const unsigned char* SelectedOriginal() const noexcept{return original_?original_+selected_:nullptr;}
    std::size_t SelectedExtent() const noexcept{return selectedBytes_;}
    bool SelectedUtf8() const noexcept{return utf8_;}
};
}

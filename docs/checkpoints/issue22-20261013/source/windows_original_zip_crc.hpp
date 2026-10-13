// UNCOMPILED ZIP CRC32 primitive over an exact retained original span. CRC is
// corruption detection only; package/vendor/native asset authority is separate.
#pragma once
#include <cstddef>
#include <cstdint>
namespace template_creator {
class OriginalZipCrc {
    const unsigned char* original_=nullptr;
    std::size_t extent_=0,processed_=0;
    std::uint32_t accumulator_=0xffffffffu,result_=0;
    bool attempted_=false,returned_=false,failed_=false;
public:
    OriginalZipCrc()=default;
    OriginalZipCrc(const OriginalZipCrc&)=delete;
    OriginalZipCrc& operator=(const OriginalZipCrc&)=delete;
    bool Capture(const unsigned char* sameOriginal,std::size_t extent) noexcept {
        if(attempted_)return false;attempted_=true;original_=sameOriginal;extent_=extent;
        if(!sameOriginal&&extent){failed_=true;returned_=true;return false;}
        for(;processed_<extent_;++processed_){
            accumulator_^=sameOriginal[processed_];
            for(unsigned bit=0;bit<8;++bit)
                accumulator_=(accumulator_>>1)^((accumulator_&1u)?0xedb88320u:0u);
        }
        result_=accumulator_^0xffffffffu;returned_=true;return true;
    }
    bool CompleteOriginal() const noexcept{return attempted_&&returned_&&!failed_&&processed_==extent_;}
    std::uint32_t OriginalResult() const noexcept{return result_;}
    const unsigned char* PrivateOriginal() const noexcept{return original_;}
    std::size_t OriginalExtent() const noexcept{return extent_;}
};
}

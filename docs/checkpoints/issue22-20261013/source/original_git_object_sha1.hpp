// UNCOMPILED streaming Git object-ID correspondence, never authentication.
#pragma once
#include "windows_source_checksum_algorithms.hpp"
namespace template_creator {
class OriginalGitObjectSha1 {
    OriginalSourceChecksumAlgorithms algorithm_;
    std::array<unsigned char,64> block_{};
    std::array<unsigned char,32> preamble_{};
    std::array<unsigned char,20> digest_{};
    uint64_t bytes_=0,bodyBytes_=0,bodyConsumed_=0;
    size_t used_=0,preambleBytes_=0;
    bool started_=false,finished_=false,failed_=false;
    bool Feed(const unsigned char* bytes,size_t extent) noexcept {
        if((!bytes&&extent)||extent>UINT64_MAX/8-bytes_)return false;
        bytes_+=extent;
        while(extent){const size_t take=extent<64-used_?extent:64-used_;
            std::memcpy(block_.data()+used_,bytes,take);used_+=take;bytes+=take;extent-=take;
            if(used_==64){algorithm_.Sha1Block(block_.data());used_=0;}
        }return true;
    }
public:
    bool Begin(uint32_t kind,uint64_t bodyBytes) noexcept {
        if(started_||failed_||finished_)return false;started_=true;bodyBytes_=bodyBytes;
        const char* type=kind==1?"commit":kind==2?"tree":kind==3?"blob":kind==4?"tag":nullptr;
        if(!type||bodyBytes>UINT64_MAX/8-preamble_.size()){failed_=true;return false;}
        while(*type)preamble_[preambleBytes_++]=static_cast<unsigned char>(*type++);
        preamble_[preambleBytes_++]=' ';
        unsigned char decimal[20]{};size_t count=0;uint64_t value=bodyBytes;
        do{decimal[count++]=static_cast<unsigned char>('0'+value%10);value/=10;}while(value);
        while(count)preamble_[preambleBytes_++]=decimal[--count];
        preamble_[preambleBytes_++]=0;
        if(!Feed(preamble_.data(),preambleBytes_)){failed_=true;return false;}return true;
    }
    bool Update(const unsigned char* bytes,size_t extent) noexcept {
        if(!started_||finished_||failed_||extent>bodyBytes_-bodyConsumed_){failed_=true;return false;}
        if(!Feed(bytes,extent)){failed_=true;return false;}bodyConsumed_+=extent;return true;
    }
    bool Finish() noexcept {
        if(!started_||finished_||failed_||bodyConsumed_!=bodyBytes_){failed_=true;return false;}
        const uint64_t bits=bytes_*8;block_[used_++]=0x80;
        if(used_>56){std::memset(block_.data()+used_,0,64-used_);algorithm_.Sha1Block(block_.data());used_=0;}
        std::memset(block_.data()+used_,0,56-used_);
        for(unsigned i=0;i<8;++i)block_[56+i]=static_cast<unsigned char>(bits>>((7-i)*8));
        algorithm_.Sha1Block(block_.data());
        for(unsigned i=0;i<20;++i)digest_[i]=static_cast<unsigned char>(algorithm_.sha1State_[i/4]>>((3-i%4)*8));
        finished_=true;return true;
    }
    const auto& Digest() const noexcept{return digest_;}
};
}

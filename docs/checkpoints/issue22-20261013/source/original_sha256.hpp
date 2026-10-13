// UNCOMPILED source-owned SHA-256 byte correspondence, NOT FIPS validation or
// authenticity. Algorithm: FIPS180-4 §§4.1.2/4.2.2/5/6.2 (August2015).
// https://nvlpubs.nist.gov/nistpubs/FIPS/NIST.FIPS.180-4.pdf
#pragma once
#include <array>
#include <cstdint>
#include <cstddef>
#include <cstring>
namespace template_creator_source {
class OriginalSha256 {
    std::array<std::uint32_t,8> hash_{{0x6a09e667u,0xbb67ae85u,0x3c6ef372u,0xa54ff53au,
        0x510e527fu,0x9b05688cu,0x1f83d9abu,0x5be0cd19u}};
    std::array<std::uint32_t,64> words_{};
    std::array<unsigned char,64> block_{};
    std::array<unsigned char,32> digest_{};
    std::uint64_t bytes_=0,blocks_=0;
    std::size_t used_=0;
    bool failed_=false,finished_=false;
    static constexpr std::array<std::uint32_t,64> constants_{{
        0x428a2f98u,0x71374491u,0xb5c0fbcfu,0xe9b5dba5u,0x3956c25bu,0x59f111f1u,0x923f82a4u,0xab1c5ed5u,
        0xd807aa98u,0x12835b01u,0x243185beu,0x550c7dc3u,0x72be5d74u,0x80deb1feu,0x9bdc06a7u,0xc19bf174u,
        0xe49b69c1u,0xefbe4786u,0x0fc19dc6u,0x240ca1ccu,0x2de92c6fu,0x4a7484aau,0x5cb0a9dcu,0x76f988dau,
        0x983e5152u,0xa831c66du,0xb00327c8u,0xbf597fc7u,0xc6e00bf3u,0xd5a79147u,0x06ca6351u,0x14292967u,
        0x27b70a85u,0x2e1b2138u,0x4d2c6dfcu,0x53380d13u,0x650a7354u,0x766a0abbu,0x81c2c92eu,0x92722c85u,
        0xa2bfe8a1u,0xa81a664bu,0xc24b8b70u,0xc76c51a3u,0xd192e819u,0xd6990624u,0xf40e3585u,0x106aa070u,
        0x19a4c116u,0x1e376c08u,0x2748774cu,0x34b0bcb5u,0x391c0cb3u,0x4ed8aa4au,0x5b9cca4fu,0x682e6ff3u,
        0x748f82eeu,0x78a5636fu,0x84c87814u,0x8cc70208u,0x90befffau,0xa4506cebu,0xbef9a3f7u,0xc67178f2u}};
    static std::uint32_t Rotate(std::uint32_t x,unsigned n) noexcept{return (x>>n)|(x<<(32-n));}
    void Compress() noexcept {
        for(std::size_t i=0;i<16;++i)words_[i]=
            (std::uint32_t(block_[4*i])<<24)|(std::uint32_t(block_[4*i+1])<<16)|
            (std::uint32_t(block_[4*i+2])<<8)|std::uint32_t(block_[4*i+3]);
        for(std::size_t i=16;i<64;++i){
            const auto a=words_[i-15],b=words_[i-2];
            words_[i]=(Rotate(b,17)^Rotate(b,19)^(b>>10))+words_[i-7]+
                (Rotate(a,7)^Rotate(a,18)^(a>>3))+words_[i-16];
        }
        auto a=hash_[0],b=hash_[1],c=hash_[2],d=hash_[3],e=hash_[4],f=hash_[5],g=hash_[6],h=hash_[7];
        for(std::size_t i=0;i<64;++i){
            const std::uint32_t t1=h+(Rotate(e,6)^Rotate(e,11)^Rotate(e,25))+((e&f)^((~e)&g))+constants_[i]+words_[i];
            const std::uint32_t t2=(Rotate(a,2)^Rotate(a,13)^Rotate(a,22))+((a&b)^(a&c)^(b&c));
            h=g;g=f;f=e;e=d+t1;d=c;c=b;b=a;a=t1+t2;
        }
        hash_[0]+=a;hash_[1]+=b;hash_[2]+=c;hash_[3]+=d;
        hash_[4]+=e;hash_[5]+=f;hash_[6]+=g;hash_[7]+=h;++blocks_;used_=0;
    }
public:
    bool Update(const unsigned char* original,std::size_t extent) noexcept {
        if(failed_||finished_||(!original&&extent)||extent>UINT64_MAX/8-bytes_){failed_=true;return false;}
        bytes_+=extent;
        while(extent){
            const auto count=extent<block_.size()-used_?extent:block_.size()-used_;
            std::memcpy(block_.data()+used_,original,count);used_+=count;original+=count;extent-=count;
            if(used_==block_.size())Compress();
        }
        return true;
    }
    bool Finish() noexcept {
        if(failed_||finished_)return false;
        const std::uint64_t bits=bytes_*8;block_[used_++]=0x80;
        if(used_>56){while(used_<64)block_[used_++]=0;Compress();}
        while(used_<56)block_[used_++]=0;
        for(unsigned i=0;i<8;++i)block_[56+i]=static_cast<unsigned char>(bits>>(56-8*i));
        Compress();
        for(std::size_t i=0;i<8;++i)for(unsigned j=0;j<4;++j)
            digest_[4*i+j]=static_cast<unsigned char>(hash_[i]>>(24-8*j));
        finished_=true;return true;
    }
    const std::array<unsigned char,32>& PrivateDigest() const noexcept{return digest_;}
    bool Finished() const noexcept{return finished_&&!failed_;}
    std::uint64_t OriginalByteCount() const noexcept{return bytes_;}
    std::uint64_t CompressedBlockCount() const noexcept{return blocks_;}
};
}

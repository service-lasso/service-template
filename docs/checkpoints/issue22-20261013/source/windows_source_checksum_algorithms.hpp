// Fixed-state source correspondence checksums, not security authenticity.
// Algorithms: RFC1321 MD5 and RFC3174 SHA1. UNCOMPILED/UNRUN.
#pragma once
#include <array>
#include <cstdint>
#include <cstddef>
#include <cstring>
namespace template_creator {
class OriginalSourceChecksumAlgorithms {
    friend class OriginalGitObjectSha1;
    std::array<std::uint32_t,4> md5State_{0x67452301,0xefcdab89,0x98badcfe,0x10325476};
    std::array<std::uint32_t,5> sha1State_{0x67452301,0xefcdab89,0x98badcfe,0x10325476,0xc3d2e1f0};
    std::array<std::uint32_t,80> words_{};
    std::array<unsigned char,128> tail_{};
    std::array<unsigned char,16> md5_{};
    std::array<unsigned char,20> sha1_{};
    const unsigned char* bodyOriginal_=nullptr;
    std::size_t extentOriginal_=0,blocksVisited_=0;
    std::size_t md5Transforms_=0,sha1Transforms_=0;
    bool attempted_=false,complete_=false;
    static std::uint32_t Rotate(std::uint32_t x,unsigned n) noexcept{return (x<<n)|(x>>(32-n));}
    void Md5Block(const unsigned char* p) noexcept {
        static constexpr std::uint32_t constants[64]={
            0xd76aa478,0xe8c7b756,0x242070db,0xc1bdceee,0xf57c0faf,0x4787c62a,0xa8304613,0xfd469501,
            0x698098d8,0x8b44f7af,0xffff5bb1,0x895cd7be,0x6b901122,0xfd987193,0xa679438e,0x49b40821,
            0xf61e2562,0xc040b340,0x265e5a51,0xe9b6c7aa,0xd62f105d,0x02441453,0xd8a1e681,0xe7d3fbc8,
            0x21e1cde6,0xc33707d6,0xf4d50d87,0x455a14ed,0xa9e3e905,0xfcefa3f8,0x676f02d9,0x8d2a4c8a,
            0xfffa3942,0x8771f681,0x6d9d6122,0xfde5380c,0xa4beea44,0x4bdecfa9,0xf6bb4b60,0xbebfbc70,
            0x289b7ec6,0xeaa127fa,0xd4ef3085,0x04881d05,0xd9d4d039,0xe6db99e5,0x1fa27cf8,0xc4ac5665,
            0xf4292244,0x432aff97,0xab9423a7,0xfc93a039,0x655b59c3,0x8f0ccc92,0xffeff47d,0x85845dd1,
            0x6fa87e4f,0xfe2ce6e0,0xa3014314,0x4e0811a1,0xf7537e82,0xbd3af235,0x2ad7d2bb,0xeb86d391};
        static constexpr unsigned shifts[16]={7,12,17,22,5,9,14,20,4,11,16,23,6,10,15,21};
        for(unsigned i=0;i<16;++i)words_[i]=std::uint32_t(p[i*4])|(std::uint32_t(p[i*4+1])<<8)|
            (std::uint32_t(p[i*4+2])<<16)|(std::uint32_t(p[i*4+3])<<24);
        auto a=md5State_[0],b=md5State_[1],c=md5State_[2],d=md5State_[3];
        for(unsigned i=0;i<64;++i){
            std::uint32_t f=0;unsigned g=0,round=i/16;
            if(round==0){f=(b&c)|(~b&d);g=i;}
            else if(round==1){f=(d&b)|(~d&c);g=(5*i+1)%16;}
            else if(round==2){f=b^c^d;g=(3*i+5)%16;}
            else{f=c^(b|~d);g=(7*i)%16;}
            const auto next=b+Rotate(a+f+constants[i]+words_[g],shifts[round*4+i%4]);
            a=d;d=c;c=b;b=next;
        }
        md5State_[0]+=a;md5State_[1]+=b;md5State_[2]+=c;md5State_[3]+=d;
        ++md5Transforms_;
    }
    void Sha1Block(const unsigned char* p) noexcept {
        for(unsigned i=0;i<16;++i)words_[i]=(std::uint32_t(p[i*4])<<24)|(std::uint32_t(p[i*4+1])<<16)|
            (std::uint32_t(p[i*4+2])<<8)|std::uint32_t(p[i*4+3]);
        for(unsigned i=16;i<80;++i)words_[i]=Rotate(words_[i-3]^words_[i-8]^words_[i-14]^words_[i-16],1);
        auto a=sha1State_[0],b=sha1State_[1],c=sha1State_[2],d=sha1State_[3],e=sha1State_[4];
        for(unsigned i=0;i<80;++i){
            std::uint32_t f=0,k=0;
            if(i<20){f=(b&c)|(~b&d);k=0x5a827999;}
            else if(i<40){f=b^c^d;k=0x6ed9eba1;}
            else if(i<60){f=(b&c)|(b&d)|(c&d);k=0x8f1bbcdc;}
            else{f=b^c^d;k=0xca62c1d6;}
            const auto next=Rotate(a,5)+f+e+k+words_[i];
            e=d;d=c;c=Rotate(b,30);b=a;a=next;
        }
        sha1State_[0]+=a;sha1State_[1]+=b;sha1State_[2]+=c;sha1State_[3]+=d;sha1State_[4]+=e;
        ++sha1Transforms_;
    }
public:
    bool Compute(const unsigned char* bytes,std::size_t extent) noexcept {
        if(attempted_)return false;
        attempted_=true;bodyOriginal_=bytes;extentOriginal_=extent;
        if((!bytes&&extent)||extent>UINT64_MAX/8)return false;
        const std::size_t full=extent/64,remainder=extent%64;
        for(std::size_t i=0;i<full;++i){Md5Block(bytes+i*64);Sha1Block(bytes+i*64);++blocksVisited_;}
        if(remainder)memcpy(tail_.data(),bytes+full*64,remainder);
        tail_[remainder]=0x80;
        const std::size_t padded=remainder<56?64:128;
        const auto bits=std::uint64_t(extent)*8;
        for(unsigned i=0;i<8;++i)tail_[padded-8+i]=static_cast<unsigned char>(bits>>(i*8));
        for(std::size_t i=0;i<padded;i+=64)Md5Block(tail_.data()+i);
        for(unsigned i=0;i<8;++i)tail_[padded-8+i]=static_cast<unsigned char>(bits>>((7-i)*8));
        for(std::size_t i=0;i<padded;i+=64)Sha1Block(tail_.data()+i);
        for(unsigned i=0;i<16;++i)md5_[i]=static_cast<unsigned char>(md5State_[i/4]>>((i%4)*8));
        for(unsigned i=0;i<20;++i)sha1_[i]=static_cast<unsigned char>(sha1State_[i/4]>>((3-i%4)*8));
        complete_=true;return true;
    }
    bool Complete() const noexcept{return complete_;}
    const unsigned char* BodyOriginal() const noexcept{return bodyOriginal_;}
    std::size_t ExtentOriginal() const noexcept{return extentOriginal_;}
    std::size_t Md5Transforms() const noexcept{return md5Transforms_;}
    std::size_t Sha1Transforms() const noexcept{return sha1Transforms_;}
    const auto& Md5() const noexcept{return md5_;}
    const auto& Sha1() const noexcept{return sha1_;}
};
}

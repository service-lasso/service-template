// UNRUN source algorithm regressions. No native input/admission or FIPS claim.
#include "original_sha256.hpp"
#include <cstring>
namespace template_creator_source {
static bool Vector(const char* input,const char* expected) noexcept {
    const auto size=std::strlen(input);
    // Every two-part split includes empty updates and padding-block boundaries.
    for(std::size_t split=0;split<=size;++split){
        OriginalSha256 hash;
        const auto* bytes=reinterpret_cast<const unsigned char*>(input);
        if(!hash.Update(bytes,split)||!hash.Update(bytes+split,size-split)||!hash.Finish()||
           !hash.Finished()||hash.OriginalByteCount()!=size||
           hash.CompressedBlockCount()!=(size+9+63)/64)return false;
        static constexpr char hex[]="0123456789ABCDEF";
        for(std::size_t i=0;i<32;++i)if(expected[2*i]!=hex[hash.PrivateDigest()[i]>>4]||
            expected[2*i+1]!=hex[hash.PrivateDigest()[i]&15])return false;
        if(hash.Finish())return false; // No repeated finalization.
    }
    return true;
}
bool OriginalSha256SourceRegression() noexcept {
    // Official NIST worked examples, SHA256.pdf, one and two padded blocks.
    // https://csrc.nist.gov/CSRC/media/Projects/Cryptographic-Standards-and-Guidelines/documents/examples/SHA256.pdf
    if(!Vector("abc","BA7816BF8F01CFEA414140DE5DAE2223B00361A396177A9CB410FF61F20015AD")||
       !Vector("abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq",
           "248D6A61D20638B8E5C026930C3E6039A33CE45964FF2167F6ECEDD419DB06C1"))return false;
    OriginalSha256 invalid;
    if(invalid.Update(nullptr,1)||invalid.Update(nullptr,0)||invalid.Finish())return false;
    OriginalSha256 finished;
    if(!finished.Update(nullptr,0)||!finished.Finish()||finished.Update(nullptr,0)||finished.Finished())return false;
    return true;
}
}

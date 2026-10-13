// Synthetic source regression, UNRUN. No native/provider/authentication credit.
#include "original_git_object_sha1.hpp"
#include <array>
#include <cstring>
int main(){
    using template_creator::OriginalGitObjectSha1;
    using template_creator::OriginalSourceChecksumAlgorithms;
    const unsigned char emptyHeader[]={'b','l','o','b',' ','0',0};
    OriginalSourceChecksumAlgorithms emptyReference;OriginalGitObjectSha1 empty;
    if(!emptyReference.Compute(emptyHeader,sizeof(emptyHeader))||!empty.Begin(3,0)||!empty.Finish()||
       empty.Digest()!=emptyReference.Sha1())return 1;
    // Independently assembled exact header plus body crosses SHA1 block/padding
    // boundaries; actual streaming API receives differently split windows.
    std::array<unsigned char,72> whole{};
    const unsigned char header[]={'b','l','o','b',' ','6','4',0};
    std::memcpy(whole.data(),header,sizeof(header));
    for(size_t i=0;i<64;++i)whole[8+i]=static_cast<unsigned char>(i);
    OriginalSourceChecksumAlgorithms reference;OriginalGitObjectSha1 split;
    if(!reference.Compute(whole.data(),whole.size())||!split.Begin(3,64)||
       !split.Update(whole.data()+8,1)||!split.Update(whole.data()+9,55)||
       !split.Update(whole.data()+64,8)||!split.Finish()||split.Digest()!=reference.Sha1())return 2;
    OriginalGitObjectSha1 partial;if(!partial.Begin(2,1)||partial.Finish()||partial.Update(whole.data(),1))return 3;
    OriginalGitObjectSha1 overrun;if(!overrun.Begin(4,1)||overrun.Update(whole.data(),2)||overrun.Finish())return 4;
    OriginalGitObjectSha1 invalid;if(invalid.Begin(0,0)||invalid.Finish())return 5;
    if(split.Finish()||split.Update(nullptr,0)||empty.Begin(3,0))return 6;
    const char* prefixes[]={"commit 0","tree 0","blob 0","tag 0"};
    for(uint32_t kind=1;kind<=4;++kind){
        OriginalSourceChecksumAlgorithms expected;OriginalGitObjectSha1 actual;
        if(!expected.Compute(reinterpret_cast<const unsigned char*>(prefixes[kind-1]),std::strlen(prefixes[kind-1])+1)||
           !actual.Begin(kind,0)||!actual.Finish()||actual.Digest()!=expected.Sha1())return 7;
    }
    std::array<unsigned char,56> padded{};
    const unsigned char paddedHeader[]={'b','l','o','b',' ','4','8',0};
    std::memcpy(padded.data(),paddedHeader,sizeof(paddedHeader));
    for(size_t i=0;i<48;++i)padded[8+i]=static_cast<unsigned char>(i);
    OriginalSourceChecksumAlgorithms paddedReference;OriginalGitObjectSha1 paddedStream;
    if(!paddedReference.Compute(padded.data(),padded.size())||!paddedStream.Begin(3,48)||
       !paddedStream.Update(padded.data()+8,48)||!paddedStream.Finish()||
       paddedStream.Digest()!=paddedReference.Sha1())return 8;
    // Published independent Git object vector (echo includes LF):
    // https://git-scm.com/book/en/v2/Git-Internals-Git-Objects
    const unsigned char publishedBody[]={'t','e','s','t',' ','c','o','n','t','e','n','t','\n'};
    const std::array<unsigned char,20> publishedDigest={0xd6,0x70,0x46,0x0b,0x4b,0x4a,0xec,0xe5,0x91,0x5c,
        0xaf,0x5c,0x68,0xd1,0x2f,0x56,0x0a,0x9f,0xe3,0xe4};
    OriginalGitObjectSha1 published;
    if(!published.Begin(3,sizeof(publishedBody))||!published.Update(publishedBody,4)||
       !published.Update(publishedBody+4,sizeof(publishedBody)-4)||!published.Finish()||
       published.Digest()!=publishedDigest)return 9;
    OriginalGitObjectSha1 premature;if(premature.Finish()||premature.Begin(3,0))return 10;
    return 0;
}

// UNRUN source cases; no Windows extraction/native name equivalence claim.
#include "windows_original_zip_name.hpp"
namespace template_creator {
bool OriginalZipNameRegression(){
    const unsigned char cp[]={0x82};std::uint32_t scalar=0;
    OriginalZipNameCursor legacy(cp,sizeof(cp),false);
    if(!legacy.Next(scalar)||scalar!=0xe9||!legacy.CompleteOriginal())return false;
    const unsigned char utf[]={0xc3,0xa9,0xf0,0x9f,0x98,0x80};
    OriginalZipNameCursor unicode(utf,sizeof(utf),true);
    if(!unicode.Next(scalar)||scalar!=0xe9||!unicode.Next(scalar)||scalar!=0x1f600||!unicode.CompleteOriginal())return false;
    const unsigned char overlong[]={0xc0,0xaf};
    OriginalZipNameCursor bad(overlong,sizeof(overlong),true);
    if(bad.Next(scalar)||!bad.FailedOriginal())return false;
    const unsigned char surrogate[]={0xed,0xa0,0x80};
    OriginalZipNameCursor invalid(surrogate,sizeof(surrogate),true);
    if(invalid.Next(scalar)||!invalid.FailedOriginal())return false;
    const unsigned char shortScalar[]={0xf0,0x9f};
    OriginalZipNameCursor truncated(shortScalar,sizeof(shortScalar),true);
    return !truncated.Next(scalar)&&truncated.FailedOriginal();
}
// NameCRC32 binds the alternate UTF8 name to the actual header name.
bool OriginalZipUnicodePathRegression(){
    unsigned char bytes[]={'a',0x75,0x70,7,0,1,0x43,0xbe,0xb7,0xe8,0xc3,0xa9};
    OriginalZipNameSelection selected;
    if(!selected.Capture(bytes,sizeof(bytes),0,1,1,11,false)||!selected.SelectedUtf8()||selected.SelectedExtent()!=2)return false;
    OriginalZipNameCursor utf(selected.SelectedOriginal(),selected.SelectedExtent(),true);std::uint32_t scalar=0;
    if(!utf.Next(scalar)||scalar!=0xe9||!utf.CompleteOriginal())return false;
    bytes[6]^=1;OriginalZipNameSelection stale;
    if(!stale.Capture(bytes,sizeof(bytes),0,1,1,11,false)||stale.SelectedUtf8())return false;
    OriginalZipNameCursor original(stale.SelectedOriginal(),stale.SelectedExtent(),false);
    return original.Next(scalar)&&scalar=='a'&&original.CompleteOriginal();
}
bool OriginalZipUnknownNameVersionRegression(){
    const unsigned char unknown[]={'a',0x75,0x70,1,0,2};
    OriginalZipNameSelection fallback;
    if(!fallback.Capture(unknown,sizeof(unknown),0,1,1,5,false)||fallback.SelectedUtf8()||fallback.SelectedExtent()!=1)return false;
    const unsigned char malformed[]={'a',0x75,0x70,1,0,1};
    OriginalZipNameSelection badVersionOne;
    return !badVersionOne.Capture(malformed,sizeof(malformed),0,1,1,5,false);
}
}

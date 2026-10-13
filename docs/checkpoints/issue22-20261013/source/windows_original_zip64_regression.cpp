// UNRUN source regression: real wire bytes, no native/package authority.
#include "windows_original_zip_directory.hpp"
#include <array>
namespace template_creator {
static void ZipPut16(unsigned char* p,unsigned v){p[0]=v;p[1]=v>>8;}
static void ZipPut32(unsigned char* p,unsigned v){ZipPut16(p,v);ZipPut16(p+2,v>>16);}
static void ZipPut64(unsigned char* p,unsigned long long v){ZipPut32(p,unsigned(v));ZipPut32(p+4,unsigned(v>>32));}
bool OriginalZip64EndRegression(){
    // Empty archive, offset independently sentinel, concrete zero counts/size.
    std::array<unsigned char,98> body{};
    ZipPut32(body.data(),0x06064b50);ZipPut64(body.data()+4,44);
    ZipPut16(body.data()+12,45);ZipPut16(body.data()+14,45);
    ZipPut32(body.data()+56,0x07064b50);ZipPut32(body.data()+72,1);
    ZipPut32(body.data()+76,0x06054b50);ZipPut32(body.data()+92,0xffffffff);
    OriginalZipDirectory mixed(body.size());
    if(!mixed.Capture(body.data(),body.size()))return false;
    auto higherVersion=body;ZipPut16(higherVersion.data()+14,46);
    OriginalZipDirectory unsupportedVersion(higherVersion.size());
    if(!unsupportedVersion.Capture(higherVersion.data(),higherVersion.size())||unsupportedVersion.SupportedVersionOriginal())return false;
    auto contradictory=body;ZipPut16(contradictory.data()+84,1);
    OriginalZipDirectory badCount(contradictory.size());
    if(badCount.Capture(contradictory.data(),contradictory.size()))return false;
    contradictory=body;ZipPut32(contradictory.data()+88,1);
    OriginalZipDirectory badSize(contradictory.size());
    if(badSize.Capture(contradictory.data(),contradictory.size()))return false;
    // Add one correctly framed extensible sector, then exceed its true extent.
    std::array<unsigned char,105> sector{};
    for(unsigned i=0;i<56;++i)sector[i]=body[i];
    ZipPut64(sector.data()+4,51);ZipPut16(sector.data()+56,0x1234);
    ZipPut32(sector.data()+58,1);sector[62]=0x42;
    for(unsigned i=56;i<body.size();++i)sector[i+7]=body[i];
    OriginalZipDirectory framed(sector.size());
    if(!framed.Capture(sector.data(),sector.size()))return false;
    ZipPut32(sector.data()+58,2);
    OriginalZipDirectory overrun(sector.size());
    return !overrun.Capture(sector.data(),sector.size());
}
// A ZIP64 offset-only central extra still selects a 64-bit descriptor.
// Its local header has ordinary zero sizes and no ZIP64 extra.
bool OriginalZip64DescriptorRegression(){
    std::array<unsigned char,132> body{};
    ZipPut32(body.data(),0x04034b50);ZipPut16(body.data()+4,45);
    ZipPut16(body.data()+6,8);ZipPut16(body.data()+26,1);body[30]='a';
    // Empty stored body followed by signatureless CRC/size64/size64 descriptor.
    const unsigned central=51;
    ZipPut32(body.data()+central,0x02014b50);ZipPut16(body.data()+central+6,45);
    ZipPut16(body.data()+central+8,8);ZipPut16(body.data()+central+28,1);
    ZipPut16(body.data()+central+30,12);ZipPut32(body.data()+central+42,0xffffffff);
    body[central+46]='a';ZipPut16(body.data()+central+47,1);
    ZipPut16(body.data()+central+49,8); // offset-only ZIP64 value is zero
    const unsigned end=110;
    ZipPut32(body.data()+end,0x06054b50);ZipPut16(body.data()+end+8,1);
    ZipPut16(body.data()+end+10,1);ZipPut32(body.data()+end+12,59);
    ZipPut32(body.data()+end+16,central);
    OriginalZipDirectory valid(body.size());
    return valid.Capture(body.data(),body.size())&&valid.MemberCount()==1&&
        valid.PrivateMembers()[0].zip64Sizes&&valid.PrivateMembers()[0].recordEnd==central;
}
}

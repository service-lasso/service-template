// UNCOMPILED read-only original ZIP framing. No extraction or vendor/native
// authority. Name-to-Windows-asset interpretation and member decoding are
// separate joins; unsupported native input is never an alternate package.
#pragma once
#include "windows_retained_backing.hpp"
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <limits>
#include <memory>
#include <stdexcept>
namespace template_creator {
class OriginalZipDirectory {
public:
    struct Member {
        std::size_t central=0,centralEnd=0,local=0,name=0,nameBytes=0,extra=0,extraBytes=0;
        std::size_t localName=0,localExtra=0,localExtraBytes=0,body=0,bodyEnd=0,recordEnd=0;
        std::uint64_t compressed=0,uncompressed=0,localOffset=0;
        std::uint32_t crc=0,disk=0;
        std::uint16_t flags=0,method=0,neededVersion=0,madeVersion=0;
        bool supportedVersion=false;
        bool centralObserved=false,localObserved=false,descriptorObserved=false,zip64Sizes=false;
    };
private:
    const unsigned char* original_=nullptr;
    std::size_t extent_=0,capacity_=0,count_=0,directory_=0,directoryEnd_=0,endRecord_=0;
    std::uint64_t expectedCount_=0;
    std::unique_ptr<Member[]> members_;
    bool attempted_=false,complete_=false,failed_=false,unsupported_=false,zip64_=false;
    std::uint16_t zip64MadeVersion_=0,zip64NeededVersion_=0;
    bool zip64VersionSupported_=true;
    std::size_t failureOffset_=0;
    bool Span(std::size_t offset,std::size_t bytes) const noexcept{return offset<=extent_&&bytes<=extent_-offset;}
    std::uint16_t U16(std::size_t p) const noexcept{return std::uint16_t(original_[p])|(std::uint16_t(original_[p+1])<<8);}
    std::uint32_t U32(std::size_t p) const noexcept{return std::uint32_t(U16(p))|(std::uint32_t(U16(p+2))<<16);}
    std::uint64_t U64(std::size_t p) const noexcept{return std::uint64_t(U32(p))|(std::uint64_t(U32(p+4))<<32);}
    bool Fail(std::size_t p,bool unsupported=false) noexcept{failed_=true;unsupported_|=unsupported;failureOffset_=p;return false;}
    bool Narrow(std::uint64_t value,std::size_t& result) const noexcept {
        if(value>std::numeric_limits<std::size_t>::max())return false;result=static_cast<std::size_t>(value);return true;
    }
    bool Zip64Fields(std::size_t extra,std::size_t bytes,bool needUncompressed,bool needCompressed,
        bool needOffset,bool needDisk,bool& present,std::uint64_t& uncompressed,std::uint64_t& compressed,
        std::uint64_t& offset,std::uint32_t& disk) noexcept {
        bool found=false;const auto end=extra+bytes;
        for(auto p=extra;p<end;){
            if(end-p<4)return Fail(p);
            const auto tag=U16(p),size=U16(p+2);p+=4;if(size>end-p)return Fail(p);
            if(tag==1){
                if(found)return Fail(p);found=true;present=true;auto q=p;
                auto read64=[&](std::uint64_t& out) noexcept {if(p+size-q<8)return false;out=U64(q);q+=8;return true;};
                if((needUncompressed&&!read64(uncompressed))||(needCompressed&&!read64(compressed))||
                    (needOffset&&!read64(offset)))return Fail(q);
                if(needDisk){if(p+size-q<4)return Fail(q);disk=U32(q);q+=4;}
                if(q!=p+size)return Fail(q);
            }
            p+=size;
        }
        return found||!(needUncompressed||needCompressed||needOffset||needDisk);
    }
    bool EndRecords() noexcept {
        if(extent_<22)return Fail(0);
        const auto earliest=extent_>65557?extent_-65557:0;
        std::size_t matches=0;
        for(std::size_t p=extent_-22;;--p){
            if(U32(p)==0x06054b50u&&Span(p,22)&&U16(p+20)==extent_-p-22){endRecord_=p;++matches;}
            if(p==earliest)break;
        }
        if(matches!=1)return Fail(endRecord_);
        const auto p=endRecord_;
        if(U16(p+4)||U16(p+6))return Fail(p,true);
        const auto entriesHere=U16(p+8),entries=U16(p+10);
        std::uint64_t size=U32(p+12),offset=U32(p+16);expectedCount_=entries;
        std::size_t centralBoundary=p;
        if(entries==0xffff||entriesHere==0xffff||size==0xffffffffu||offset==0xffffffffu){
            zip64_=true;if(p<20||U32(p-20)!=0x07064b50u)return Fail(p);
            if(U32(p-16)||U32(p-4)!=1)return Fail(p-20,true);
            std::size_t record=0;if(!Narrow(U64(p-12),record)||!Span(record,56)||U32(record)!=0x06064b50u)return Fail(p-20);
            const auto extra=U64(record+4);if(extra<44||extra>p-20||record>p-20-static_cast<std::size_t>(extra)||
                record+static_cast<std::size_t>(extra)!=p-32)return Fail(record);
            zip64MadeVersion_=U16(record+12);zip64NeededVersion_=U16(record+14);
            zip64VersionSupported_=zip64NeededVersion_==45;
            if(U32(record+16)||U32(record+20))return Fail(record,true);
            if(U64(record+24)!=U64(record+32))return Fail(record);
            // Each legacy field independently falls back only on its own
            // sentinel. Concrete legacy originals must agree with ZIP64.
            if((entriesHere!=0xffff&&entriesHere!=U64(record+24))||
                (entries!=0xffff&&entries!=U64(record+32))||
                (size!=0xffffffffu&&size!=U64(record+40))||
                (offset!=0xffffffffu&&offset!=U64(record+48)))return Fail(p);
            // ZIP64 extensible data sectors have a two-byte Header ID and a
            // four-byte Data Size; preserve their bytes, validate every frame.
            const auto sectorEnd=p-20;
            for(auto sector=record+56;sector<sectorEnd;){
                if(sectorEnd-sector<6)return Fail(sector);
                const auto bodyBytes=U32(sector+2);sector+=6;
                if(bodyBytes>sectorEnd-sector)return Fail(sector);
                sector+=bodyBytes;
            }
            expectedCount_=U64(record+32);size=U64(record+40);offset=U64(record+48);centralBoundary=record;
        }else if(entriesHere!=entries)return Fail(p);
        std::size_t nativeSize=0;
        if(!Narrow(offset,directory_)||!Narrow(size,nativeSize)||!Span(directory_,nativeSize)||
            directory_>centralBoundary||nativeSize!=centralBoundary-directory_)return Fail(p);
        directoryEnd_=directory_+nativeSize;
        if(expectedCount_>capacity_||expectedCount_>nativeSize/46)return Fail(directory_);
        return true;
    }
    bool Local(Member& row) noexcept {
        if(!Narrow(row.localOffset,row.local)||!Span(row.local,30)||U32(row.local)!=0x04034b50u)return Fail(row.local);
        const auto p=row.local;
        if(U16(p+4)!=row.neededVersion||U16(p+6)!=row.flags||U16(p+8)!=row.method)return Fail(p);
        const auto nameBytes=U16(p+26),extraBytes=U16(p+28);
        row.localName=p+30;
        if(!Span(row.localName,nameBytes)||nameBytes!=row.nameBytes||
            std::memcmp(original_+row.name,original_+row.localName,nameBytes))return Fail(p);
        row.localExtra=row.localName+nameBytes;row.localExtraBytes=extraBytes;
        if(!Span(row.localExtra,extraBytes))return Fail(row.localExtra);
        row.body=row.localExtra+extraBytes;
        std::size_t compressed=0;if(!Narrow(row.compressed,compressed)||!Span(row.body,compressed))return Fail(row.body);
        row.bodyEnd=row.body+compressed;row.recordEnd=row.bodyEnd;
        std::uint64_t localUncompressed=U32(p+22),localCompressed=U32(p+18),dummyOffset=0;std::uint32_t dummyDisk=0;bool localZip64=false;
        if(!Zip64Fields(row.localExtra,extraBytes,localUncompressed==0xffffffffu,localCompressed==0xffffffffu,
            false,false,localZip64,localUncompressed,localCompressed,dummyOffset,dummyDisk))return Fail(row.localExtra);
        if(localZip64&&(U32(p+22)!=0xffffffffu||U32(p+18)!=0xffffffffu))return Fail(row.localExtra);
        row.zip64Sizes|=localZip64;
        if(!(row.flags&8u)){
            if(U32(p+14)!=row.crc||localCompressed!=row.compressed||localUncompressed!=row.uncompressed)return Fail(p);
        }else{
            // Do not guess whether a CRC equal to the optional descriptor
            // signature is itself a signature: test both layouts and deny ambiguity.
            std::size_t matching=0,matchedEnd=0;
            for(unsigned signature=0;signature<2;++signature){
                auto q=row.bodyEnd;
                if(signature){if(!Span(q,4)||U32(q)!=0x08074b50u)continue;q+=4;}
                const std::size_t fields=row.zip64Sizes?20:12;
                if(!Span(q,fields)||U32(q)!=row.crc)continue;
                const auto c=row.zip64Sizes?U64(q+4):U32(q+4);
                const auto u=row.zip64Sizes?U64(q+12):U32(q+8);
                if(c==row.compressed&&u==row.uncompressed){++matching;matchedEnd=q+fields;}
            }
            if(matching!=1)return Fail(row.bodyEnd);
            row.descriptorObserved=true;row.recordEnd=matchedEnd;
        }
        if(row.recordEnd>directory_)return Fail(row.recordEnd);
        row.localObserved=true;return true;
    }
public:
    static std::size_t ReservedSourceBytesFor(std::size_t originalExtent) {
        const auto count=originalExtent/46;
        if(count>(std::numeric_limits<std::size_t>::max()-sizeof(OriginalZipDirectory))/sizeof(Member))
            throw std::invalid_argument("original ZIP directory geometry");
        return sizeof(OriginalZipDirectory)+count*sizeof(Member);
    }
    explicit OriginalZipDirectory(std::size_t originalExtent)
        :extent_(originalExtent),capacity_(originalExtent/46) {
        if(capacity_>(std::numeric_limits<std::size_t>::max()-sizeof(*this))/sizeof(Member))throw std::invalid_argument("original ZIP directory geometry");
        members_=std::make_unique<Member[]>(capacity_);
    }
    OriginalZipDirectory(const OriginalZipDirectory&)=delete;
    OriginalZipDirectory& operator=(const OriginalZipDirectory&)=delete;
    bool Capture(const unsigned char* sameOriginal,std::size_t extent) noexcept {
        if(attempted_)return false;attempted_=true;original_=sameOriginal;
        if(!sameOriginal||extent!=extent_)return Fail(0);
        if(!EndRecords())return false;
        auto p=directory_;
        while(p<directoryEnd_){
            if(count_==capacity_||directoryEnd_-p<46||U32(p)!=0x02014b50u)return Fail(p);
            auto& row=members_[count_++];row.central=p;row.madeVersion=U16(p+4);row.neededVersion=U16(p+6);row.flags=U16(p+8);row.method=U16(p+10);
            row.crc=U32(p+16);row.compressed=U32(p+20);row.uncompressed=U32(p+24);
            row.nameBytes=U16(p+28);row.extraBytes=U16(p+30);const auto comment=U16(p+32);
            row.disk=U16(p+34);row.localOffset=U32(p+42);row.name=p+46;
            if(!row.nameBytes||!Span(row.name,row.nameBytes))return Fail(p);
            row.extra=row.name+row.nameBytes;
            if(!Span(row.extra,row.extraBytes)||!Span(row.extra+row.extraBytes,comment))return Fail(p);
            row.centralEnd=row.extra+row.extraBytes+comment;if(row.centralEnd>directoryEnd_)return Fail(p);
            row.centralObserved=true;
            row.zip64Sizes=row.compressed==0xffffffffu||row.uncompressed==0xffffffffu;
            if(!Zip64Fields(row.extra,row.extraBytes,row.uncompressed==0xffffffffu,row.compressed==0xffffffffu,
                row.localOffset==0xffffffffu,row.disk==0xffff,row.zip64Sizes,row.uncompressed,row.compressed,row.localOffset,row.disk))return Fail(row.extra);
            if(row.disk||row.flags&~std::uint16_t(0x080e)|| (row.method!=0&&row.method!=8))return Fail(p,true);
            if(row.method==0&&((row.flags&6u)||row.compressed!=row.uncompressed))return Fail(p);
            if(!Local(row))return false;
            // Feature-profile observation is distinct from complete framing.
            row.supportedVersion=row.zip64Sizes||row.localOffset>0xffffffffu ? row.neededVersion==45 :
                (row.neededVersion==10||row.neededVersion==20||row.neededVersion==45);
            if(row.method==8&&row.neededVersion<20)row.supportedVersion=false;
            for(std::size_t i=0;i+1<count_;++i){const auto& previous=members_[i];
                if(previous.nameBytes==row.nameBytes&&!std::memcmp(original_+previous.name,original_+row.name,row.nameBytes))return Fail(row.name);
                if(row.local<previous.recordEnd&&previous.local<row.recordEnd)return Fail(row.local);
            }
            p=row.centralEnd;
        }
        if(count_!=expectedCount_)return Fail(p);
        // This initial supported profile has contiguous local records from
        // byte zero. Other archive prefix/gap/signature records stay unsupported.
        std::size_t localCoverage=0;
        for(std::size_t i=0;i<count_;++i){const auto bytes=members_[i].recordEnd-members_[i].local;
            if(bytes>directory_-localCoverage)return Fail(members_[i].local);localCoverage+=bytes;}
        if(localCoverage!=directory_)return Fail(directory_,true);
        complete_=true;return true;
    }
    // Framing only: UTF8/legacy names, Windows mapping, CRC, inflate and
    // SAME held-asset/vendor relationship must still be checked by the caller.
    bool CompleteFramingOriginal() const noexcept{return complete_&&!failed_;}
    bool SupportedVersionOriginal() const noexcept{return complete_&&!failed_&&zip64VersionSupported_;}
    bool UnsupportedOriginal() const noexcept{return unsupported_;}
    const Member* PrivateMembers() const noexcept{return members_.get();}
    std::size_t MemberCount() const noexcept{return count_;}
    const unsigned char* PrivateOriginal() const noexcept{return original_;}
    std::size_t OriginalExtent() const noexcept{return extent_;}
    std::size_t ReservedSourceBytes() const noexcept{return sizeof(*this)+capacity_*sizeof(Member);}
public:
    bool OutputBackingSeparateOriginal(const void* output,std::size_t bytes) const noexcept {
        return OriginalRetainedStorageSeparate(output,bytes,this,1,sizeof(*this))&&OriginalRetainedArraySeparate(output,bytes,members_,capacity_);
    }
};
}

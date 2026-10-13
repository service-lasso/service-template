#pragma once
#include "linux_receipt_destination_layout.hpp"
#include <array>
#include <cstring>
#include <span>
#include <type_traits>
#include <fcntl.h>

namespace template_creator_receipts {
using DestinationHeaderBytes=std::array<unsigned char,80>;
using DestinationIndexRecordBytes=std::array<unsigned char,88>;
struct DestinationDecodedHeader {
    std::uint32_t version=0,operation=0;
    std::uint64_t sequence=0,predecessor=0,control=0,input=0,output=0;
    std::uint64_t requested=0,sourceOffset=0,nativeLayout=0;
};
inline void DestinationPutLE(unsigned char* p,std::uint64_t value,std::size_t n) noexcept {
    for(std::size_t i=0;i<n;++i){p[i]=static_cast<unsigned char>(value);value>>=8;}
}
inline std::uint64_t DestinationGetLE(const unsigned char* p,std::size_t n) noexcept {
    std::uint64_t value=0;
    for(std::size_t i=0;i<n;++i)value|=std::uint64_t(p[i])<<(8*i);
    return value;
}
inline bool DestinationCodecOutputSeparate(const void* output,std::size_t bytes,
    const DestinationClaimGeometryOriginal& c) noexcept {
    // No pointed original is dereferenced until these whole-output exclusions.
    return StorageSeparate(output,bytes,&c,sizeof(c))&&c.geometry.reservationOriginal&&
        c.geometry.sourceOwnedSlotOriginal&&c.roleOriginal&&
        StorageSeparate(output,bytes,c.geometry.reservationOriginal,sizeof(*c.geometry.reservationOriginal))&&
        StorageSeparate(output,bytes,c.geometry.sourceOwnedSlotOriginal,sizeof(*c.geometry.sourceOwnedSlotOriginal))&&
        StorageSeparate(output,bytes,c.roleOriginal,sizeof(*c.roleOriginal))&&
        (!c.geometry.previousSlotOriginal||StorageSeparate(output,bytes,c.geometry.previousSlotOriginal,sizeof(*c.geometry.previousSlotOriginal)))&&
        (!c.previousRoleOriginal||StorageSeparate(output,bytes,c.previousRoleOriginal,sizeof(*c.previousRoleOriginal)));
}
inline bool EncodeDestinationHeader(const DestinationClaimGeometryOriginal& c,
    std::uint64_t nativeLayout,DestinationHeaderBytes& output) noexcept {
    if(!nativeLayout||!DestinationCodecOutputSeparate(&output,sizeof(output),c)||
       !DestinationClaimReturnMatches(c))return false;
    DestinationHeaderBytes value{};
    constexpr unsigned char magic[8]={'S','L','R','C','P','D','0','1'};
    std::memcpy(value.data(),magic,8);
    const auto& s=c.geometry.value;
    DestinationPutLE(value.data()+8,s.abiVersion,4);DestinationPutLE(value.data()+12,s.operation,4);
    const std::uint64_t fields[8]={s.sequence,s.predecessorSequence,s.control.bytes,s.input.bytes,
        s.output.bytes,s.requested,s.sourceOffset,nativeLayout};
    for(std::size_t i=0;i<8;++i)DestinationPutLE(value.data()+16+8*i,fields[i],8);
    output=value;return true;
}
inline bool DecodeDestinationHeader(const DestinationHeaderBytes& raw,
    const DestinationClaimGeometryOriginal& c,std::uint64_t expectedNativeLayout,
    DestinationDecodedHeader& output) noexcept {
    if(!expectedNativeLayout||!StorageSeparate(&output,sizeof(output),&raw,sizeof(raw))||
       !DestinationCodecOutputSeparate(&output,sizeof(output),c)||!DestinationClaimReturnMatches(c))return false;
    constexpr unsigned char magic[8]={'S','L','R','C','P','D','0','1'};
    if(std::memcmp(raw.data(),magic,8))return false;
    DestinationDecodedHeader v{};
    v.version=static_cast<std::uint32_t>(DestinationGetLE(raw.data()+8,4));
    v.operation=static_cast<std::uint32_t>(DestinationGetLE(raw.data()+12,4));
    v.sequence=DestinationGetLE(raw.data()+16,8);v.predecessor=DestinationGetLE(raw.data()+24,8);
    v.control=DestinationGetLE(raw.data()+32,8);v.input=DestinationGetLE(raw.data()+40,8);
    v.output=DestinationGetLE(raw.data()+48,8);v.requested=DestinationGetLE(raw.data()+56,8);
    v.sourceOffset=DestinationGetLE(raw.data()+64,8);v.nativeLayout=DestinationGetLE(raw.data()+72,8);
    const auto& s=c.geometry.value;
    if(v.version!=s.abiVersion||v.operation!=s.operation||v.sequence!=s.sequence||
       v.predecessor!=s.predecessorSequence||v.control!=s.control.bytes||v.input!=s.input.bytes||
       v.output!=s.output.bytes||v.requested!=s.requested||v.sourceOffset!=s.sourceOffset||
       v.nativeLayout!=expectedNativeLayout)return false;
    output=v;return true;
}
inline bool DestinationHeaderReturnMatches(const DestinationHeaderBytes& raw,
    const DestinationClaimGeometryOriginal& c,std::uint64_t expectedNativeLayout) noexcept {
    DestinationDecodedHeader decoded{};
    return DecodeDestinationHeader(raw,c,expectedNativeLayout,decoded);
}
inline bool EncodeDestinationIndex(const DestinationClaimGeometryOriginal& c,
    DestinationIndexRecordBytes& output) noexcept {
    if(!DestinationCodecOutputSeparate(&output,sizeof(output),c)||!DestinationClaimReturnMatches(c))return false;
    DestinationIndexRecordBytes value{};const auto& s=c.geometry.value;
    DestinationPutLE(value.data(),s.sequence,8);DestinationPutLE(value.data()+8,s.predecessorSequence,8);
    const Interval intervals[4]={s.slot,s.control,s.input,s.output};
    for(std::size_t i=0;i<4;++i){DestinationPutLE(value.data()+16+16*i,intervals[i].offset,8);
        DestinationPutLE(value.data()+24+16*i,intervals[i].bytes,8);}
    DestinationPutLE(value.data()+80,s.operation,4);DestinationPutLE(value.data()+84,s.abiVersion,4);
    output=value;return true;
}
inline bool DecodeDestinationIndex(const DestinationIndexRecordBytes& raw,
    const DestinationClaimGeometryOriginal& c,SlotShape& output) noexcept {
    if(!StorageSeparate(&output,sizeof(output),&raw,sizeof(raw))||
       !DestinationCodecOutputSeparate(&output,sizeof(output),c)||!DestinationClaimReturnMatches(c))return false;
    SlotShape v{};v.sequence=DestinationGetLE(raw.data(),8);v.predecessorSequence=DestinationGetLE(raw.data()+8,8);
    Interval* intervals[4]={&v.slot,&v.control,&v.input,&v.output};
    for(std::size_t i=0;i<4;++i){intervals[i]->offset=DestinationGetLE(raw.data()+16+16*i,8);
        intervals[i]->bytes=DestinationGetLE(raw.data()+24+16*i,8);}
    v.operation=static_cast<std::uint32_t>(DestinationGetLE(raw.data()+80,4));
    v.abiVersion=static_cast<std::uint32_t>(DestinationGetLE(raw.data()+84,4));
    // Index omits redundant q/sourceOffset; derive only from SAME retained claim.
    v.requested=c.geometry.value.requested;v.sourceOffset=c.geometry.value.sourceOffset;
    if(!SameSlot(v,c.geometry.value))return false;
    output=v;return true;
}
inline bool DestinationIndexReturnMatches(const DestinationIndexRecordBytes& raw,
    const DestinationClaimGeometryOriginal& c) noexcept {
    SlotShape decoded{};return DecodeDestinationIndex(raw,c,decoded);
}
inline bool DestinationByteOperation(DestinationOperation op,bool& write) noexcept {
    switch(op){
    case DestinationOperation::SourcePread:case DestinationOperation::StorePread:write=false;return true;
    case DestinationOperation::SourcePwrite:case DestinationOperation::StorePwrite:write=true;return true;
    default:return false;
    }
}
inline bool DestinationByteRecordShape(const DestinationClaimGeometryOriginal& c,
    const NativeByteControl& control,std::uint64_t nativeLayout,std::size_t& extent,
    std::size_t& observed,bool& write) noexcept {
    if(!nativeLayout||!DestinationClaimReturnMatches(c)||!DestinationByteOperation(c.roleValue.operation,write)||
       c.geometry.value.slot.bytes>SIZE_MAX||c.roleValue.requested>SIZE_MAX||
       control.requested!=c.roleValue.requested||control.requested>control.bufferExtent||
       (control.bufferExtent&&!control.bufferOriginal)||control.offset<0||
       static_cast<std::uint64_t>(control.offset)!=c.geometry.value.sourceOffset||
       !control.captureStarted||!control.callAttempted||!control.callReturned)return false;
    // A completed raw control is retained even if the native extent is unknown.
    // Unknown counts never cause a payload read or a useful commit.
    observed=0;
    if(control.extentKnown&&control.returned>=0){
        if(static_cast<std::uint64_t>(control.returned)>control.requested||
           control.actual!=static_cast<std::size_t>(control.returned))return false;
        observed=control.actual;
    }else if(control.actual)return false;
    extent=static_cast<std::size_t>(c.geometry.value.slot.bytes);return true;
}
inline bool EncodeDestinationByteRecord(const DestinationClaimGeometryOriginal& c,
    const NativeByteControl& control,std::uint64_t nativeLayout,std::span<unsigned char> output) noexcept {
    static_assert(std::is_trivially_copyable_v<NativeByteControl>);
    std::size_t extent=0,observed=0;bool write=false;
    if(!DestinationCodecOutputSeparate(output.data(),output.size(),c)||
       !StorageSeparate(output.data(),output.size(),&control,sizeof(control))||
       !DestinationByteRecordShape(c,control,nativeLayout,extent,observed,write)||output.size()!=extent||
       (control.bufferExtent&&!StorageSeparate(output.data(),output.size(),control.bufferOriginal,control.bufferExtent)))return false;
    DestinationHeaderBytes header{};
    if(!EncodeDestinationHeader(c,nativeLayout,header))return false;
    // Source prerequisite: actual native call and issuing continuation are
    // settled; complete control representation and supplied byte owner remain
    // unchanged throughout this pure copy. The scalar flags above are only
    // representation consistency, never that borrower/profile/source proof.
    std::memset(output.data(),0,output.size());
    std::memcpy(output.data(),header.data(),header.size());
    std::memcpy(output.data()+header.size(),&control,sizeof(control));
    const auto dataOffset=static_cast<std::size_t>(c.geometry.value.control.bytes);
    if(write&&control.requested)std::memcpy(output.data()+dataOffset,control.bufferOriginal,control.requested);
    if(!write&&observed)std::memcpy(output.data()+dataOffset,control.bufferOriginal,observed);
    return true;
}
inline bool DestinationByteRecordReturnMatches(std::span<const unsigned char> raw,
    const DestinationClaimGeometryOriginal& c,const NativeByteControl& original,
    std::uint64_t nativeLayout) noexcept {
    std::size_t extent=0,observed=0;bool write=false;
    if(!DestinationByteRecordShape(c,original,nativeLayout,extent,observed,write)||raw.size()!=extent||
       !DestinationCodecOutputSeparate(raw.data(),raw.size(),c)||
       !raw.data()||!StorageSeparate(raw.data(),raw.size(),&original,sizeof(original))||
       (original.bufferExtent&&!StorageSeparate(raw.data(),raw.size(),original.bufferOriginal,original.bufferExtent)))return false;
    DestinationHeaderBytes expected{};
    if(!EncodeDestinationHeader(c,nativeLayout,expected)||
       std::memcmp(raw.data(),expected.data(),expected.size())||
       std::memcmp(raw.data()+expected.size(),&original,sizeof(original)))return false;
    const auto dataOffset=static_cast<std::size_t>(c.geometry.value.control.bytes);
    const auto bytes=write?original.requested:observed;
    if(bytes&&std::memcmp(raw.data()+dataOffset,original.bufferOriginal,bytes))return false;
    // Reserved unobserved suffix is explicit initialized backing, never a
    // fabricated successful native return or an independently captured prefix.
    for(std::size_t i=dataOffset+bytes;i<raw.size();++i)if(raw[i])return false;
    return true;
}
namespace destination_codec_detail {
struct Parts {
    std::span<const unsigned char> control,inputFirst,inputSecond,output;
};
template<class T> inline std::span<const unsigned char> Bytes(const T& value) noexcept {
    static_assert(std::is_trivially_copyable_v<T>);
    return {reinterpret_cast<const unsigned char*>(&value),sizeof(value)};
}
inline bool PartsShape(const DestinationClaimGeometryOriginal& c,const Parts& p,std::size_t& total) noexcept {
    if(!DestinationClaimReturnMatches(c)||c.geometry.value.slot.bytes>SIZE_MAX||
       p.inputSecond.size()>SIZE_MAX-p.inputFirst.size()||
       p.control.size()!=c.geometry.value.control.bytes-DestinationRecordHeaderBytes||
       p.inputFirst.size()+p.inputSecond.size()!=c.geometry.value.input.bytes||
       p.output.size()!=c.geometry.value.output.bytes)return false;
    for(const auto part:{p.control,p.inputFirst,p.inputSecond,p.output})if(part.size()&&!part.data())return false;
    total=static_cast<std::size_t>(c.geometry.value.slot.bytes);return true;
}
inline bool PartsSeparate(const void* p,std::size_t n,const Parts& source) noexcept {
    for(const auto part:{source.control,source.inputFirst,source.inputSecond,source.output})
        if(part.size()&&!StorageSeparate(p,n,part.data(),part.size()))return false;
    return true;
}
template<class T> inline bool PointerSeparate(const void* p,std::size_t n,const T* original) noexcept {
    return !original||StorageSeparate(p,n,original,sizeof(*original));
}
inline bool EdgePointersSeparate(const void* p,std::size_t n,const EdgeControl& control,const EdgeOutput& result) noexcept {
    return PointerSeparate(p,n,control.stateOriginal)&&PointerSeparate(p,n,control.edgeOriginal)&&
        PointerSeparate(p,n,control.stateBefore.pending_original)&&PointerSeparate(p,n,result.state.pending_original);
}
inline bool NamePointersSeparate(const void* p,std::size_t n,const NameControl& control,const NameOutput& result) noexcept {
    return PointerSeparate(p,n,control.stateOriginal)&&PointerSeparate(p,n,control.stateBefore.member_original)&&
        PointerSeparate(p,n,control.stateBefore.previous_original)&&PointerSeparate(p,n,control.stateBefore.current_original)&&
        PointerSeparate(p,n,result.state.member_original)&&PointerSeparate(p,n,result.state.previous_original)&&
        PointerSeparate(p,n,result.state.current_original);
}
inline bool Encode(const DestinationClaimGeometryOriginal& c,const Parts& parts,
    std::uint64_t nativeLayout,std::span<unsigned char> output) noexcept {
    std::size_t total=0;
    if(!DestinationCodecOutputSeparate(output.data(),output.size(),c)||!PartsSeparate(output.data(),output.size(),parts)||
       !PartsShape(c,parts,total)||output.size()!=total)return false;
    DestinationHeaderBytes header{};if(!EncodeDestinationHeader(c,nativeLayout,header))return false;
    std::memcpy(output.data(),header.data(),header.size());std::size_t cursor=header.size();
    for(const auto part:{parts.control,parts.inputFirst,parts.inputSecond,parts.output}){
        if(part.size())std::memcpy(output.data()+cursor,part.data(),part.size());cursor+=part.size();}
    return true;
}
inline bool Matches(std::span<const unsigned char> raw,const DestinationClaimGeometryOriginal& c,
    const Parts& parts,std::uint64_t nativeLayout) noexcept {
    std::size_t total=0;
    if(!DestinationCodecOutputSeparate(raw.data(),raw.size(),c)||!PartsSeparate(raw.data(),raw.size(),parts)||
       !PartsShape(c,parts,total)||raw.size()!=total)return false;
    DestinationHeaderBytes header{};if(!EncodeDestinationHeader(c,nativeLayout,header)||
       std::memcmp(raw.data(),header.data(),header.size()))return false;
    std::size_t cursor=header.size();
    for(const auto part:{parts.control,parts.inputFirst,parts.inputSecond,parts.output}){
        if(part.size()&&std::memcmp(raw.data()+cursor,part.data(),part.size()))return false;cursor+=part.size();}
    return true;
}
}
inline bool EncodeDestinationStatRecord(const DestinationClaimGeometryOriginal& c,const NativeStatControl& control,
    const struct stat& status,std::uint64_t nativeLayout,std::span<unsigned char> output) noexcept {
    if(c.roleValue.operation!=DestinationOperation::Fstat||!control.captureStarted||!control.callAttempted||!control.callReturned)return false;
    return destination_codec_detail::Encode(c,{destination_codec_detail::Bytes(control),{},{},destination_codec_detail::Bytes(status)},nativeLayout,output);
}
inline bool DestinationStatRecordReturnMatches(std::span<const unsigned char> raw,const DestinationClaimGeometryOriginal& c,
    const NativeStatControl& control,const struct stat& status,std::uint64_t nativeLayout) noexcept {
    if(c.roleValue.operation!=DestinationOperation::Fstat||!control.captureStarted||!control.callAttempted||!control.callReturned)return false;
    return destination_codec_detail::Matches(raw,c,{destination_codec_detail::Bytes(control),{},{},destination_codec_detail::Bytes(status)},nativeLayout);
}
inline bool EncodeDestinationFlagsRecord(const DestinationClaimGeometryOriginal& c,const NativeFlagsControl& control,
    std::uint64_t nativeLayout,std::span<unsigned char> output) noexcept {
    if(c.roleValue.operation!=DestinationOperation::GetFlags||control.command!=F_GETFL||!control.captureStarted||!control.callAttempted||!control.callReturned)return false;
    return destination_codec_detail::Encode(c,{destination_codec_detail::Bytes(control),{},{},{}},nativeLayout,output);
}
inline bool DestinationFlagsRecordReturnMatches(std::span<const unsigned char> raw,const DestinationClaimGeometryOriginal& c,
    const NativeFlagsControl& control,std::uint64_t nativeLayout) noexcept {
    if(c.roleValue.operation!=DestinationOperation::GetFlags||control.command!=F_GETFL||!control.captureStarted||!control.callAttempted||!control.callReturned)return false;
    return destination_codec_detail::Matches(raw,c,{destination_codec_detail::Bytes(control),{},{},{}},nativeLayout);
}
inline bool EncodeDestinationEdgeRecord(const DestinationClaimGeometryOriginal& c,const EdgeControl& control,
    const EdgeOutput& result,std::uint64_t nativeLayout,std::span<unsigned char> output) noexcept {
    if(c.roleValue.operation!=DestinationOperation::GitEdgeFeed||!control.attempted||!control.callReturned||
       control.consumed!=result.consumed||control.inputExtent!=c.roleValue.inputBytes||
       !destination_codec_detail::EdgePointersSeparate(output.data(),output.size(),control,result))return false;
    return destination_codec_detail::Encode(c,{destination_codec_detail::Bytes(control),
        {control.inputOriginal,control.inputExtent},{},destination_codec_detail::Bytes(result)},nativeLayout,output);
}
inline bool DestinationEdgeRecordReturnMatches(std::span<const unsigned char> raw,const DestinationClaimGeometryOriginal& c,
    const EdgeControl& control,const EdgeOutput& result,std::uint64_t nativeLayout) noexcept {
    if(c.roleValue.operation!=DestinationOperation::GitEdgeFeed||!control.attempted||!control.callReturned||
       control.consumed!=result.consumed||control.inputExtent!=c.roleValue.inputBytes||
       !destination_codec_detail::EdgePointersSeparate(raw.data(),raw.size(),control,result))return false;
    return destination_codec_detail::Matches(raw,c,{destination_codec_detail::Bytes(control),
        {control.inputOriginal,control.inputExtent},{},destination_codec_detail::Bytes(result)},nativeLayout);
}
inline bool EncodeDestinationNameRecord(const DestinationClaimGeometryOriginal& c,const NameControl& control,
    const NameOutput& result,std::uint64_t nativeLayout,std::span<unsigned char> output) noexcept {
    if(c.roleValue.operation!=DestinationOperation::GitNameFeed||!control.attempted||!control.callReturned||
       control.previousUsed!=result.previousUsed||control.currentUsed!=result.currentUsed||
       control.previousExtent!=c.roleValue.previousInputBytes||control.currentExtent!=c.roleValue.currentInputBytes||
       !destination_codec_detail::NamePointersSeparate(output.data(),output.size(),control,result))return false;
    return destination_codec_detail::Encode(c,{destination_codec_detail::Bytes(control),
        {control.previousOriginal,control.previousExtent},{control.currentOriginal,control.currentExtent},
        destination_codec_detail::Bytes(result)},nativeLayout,output);
}
inline bool DestinationNameRecordReturnMatches(std::span<const unsigned char> raw,const DestinationClaimGeometryOriginal& c,
    const NameControl& control,const NameOutput& result,std::uint64_t nativeLayout) noexcept {
    if(c.roleValue.operation!=DestinationOperation::GitNameFeed||!control.attempted||!control.callReturned||
       control.previousUsed!=result.previousUsed||control.currentUsed!=result.currentUsed||
       control.previousExtent!=c.roleValue.previousInputBytes||control.currentExtent!=c.roleValue.currentInputBytes||
       !destination_codec_detail::NamePointersSeparate(raw.data(),raw.size(),control,result))return false;
    return destination_codec_detail::Matches(raw,c,{destination_codec_detail::Bytes(control),
        {control.previousOriginal,control.previousExtent},{control.currentOriginal,control.currentExtent},
        destination_codec_detail::Bytes(result)},nativeLayout);
}
// UNRUN pure serialized-copy transforms. These are not the mapped native-object
// placement layout. The source owner must separately construct/retain aligned
// original objects and charge every disjoint serialized copy it actually needs.
// The nonzero native-layout number is a routing constraint,
// never authentic ABI/profile/source membership or finance. Byte-record copy
// cannot acquire controls/payload/pointed subjects, settle a pending native call,
// prove its own input/control nonmutation, perform IO, commit or permit ACK.
// No production caller is installed until the concrete actual caller/profile/
// native/borrower originals can independently dominate these pure transforms.
}

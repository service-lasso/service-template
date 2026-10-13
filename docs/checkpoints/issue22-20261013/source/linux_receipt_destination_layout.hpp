#pragma once
#include "linux_receipt_foundation_geometry.hpp"
#include "linux_original_deadline.hpp"
#include "source_unit12.h"
#include <sys/stat.h>
#include <sys/uio.h>
#include <sys/types.h>
#include <cstddef>
#include <cstdint>

// UNCOMPILED serialized-copy source recipe. No IO, reservation, commit or authority.
// These 80-byte-header intervals never supply placement storage for native
// objects. The mapped native-original owner needs a separately checked aligned
// envelope, actual constructed object lifetimes and whole predecessor extent.
// A serialized copy remains disjoint from every live native original; neither
// mmap nor a decoded byte representation creates one of those originals.
namespace template_creator_receipts {
enum class DestinationOperation : std::uint32_t {
    SourcePread=1, SourcePwrite=2, Fstat=3, GetFlags=4,
    GitEdgeFeed=5, GitNameFeed=6, StorePwrite=7, StorePread=8
};
inline constexpr std::uint32_t DestinationLayoutVersion=1;
inline constexpr std::uint64_t DestinationRecordHeaderBytes=80;
inline constexpr std::uint64_t DestinationIndexBytes=88;
using NativeClockOriginal=template_creator_linux::OriginalClockBoundary;
struct NativeByteControl {
    int descriptor=-1;
    const void* bufferOriginal=nullptr;
    std::size_t bufferExtent=0,requested=0;
    off_t offset=0;
    ssize_t returned=-1;
    int error=0;
    bool captureStarted=false,callAttempted=false,callReturned=false,extentKnown=false;
    std::size_t actual=0;
    NativeClockOriginal before{},after{};
};
struct NativeStatControl {
    int descriptor=-1,result=-1,error=0;
    bool captureStarted=false,callAttempted=false,callReturned=false;
    NativeClockOriginal before{},after{};
};
struct NativeFlagsControl {
    int descriptor=-1,command=0,result=-1,error=0;
    bool captureStarted=false,callAttempted=false,callReturned=false;
    NativeClockOriginal before{},after{};
};
struct EdgeControl {
    const slcli_source_unit12_git_edge_state* stateOriginal=nullptr;
    slcli_source_unit12_git_edge_state stateBefore{};
    const unsigned char* inputOriginal=nullptr;
    slcli_source_unit12_git_edge* edgeOriginal=nullptr;
    std::uint64_t relativeOffset=0;
    std::size_t inputExtent=0,consumed=0;
    int returned=0;
    bool attempted=false,callReturned=false;
    NativeClockOriginal before{},after{};
};
struct EdgeOutput {
    slcli_source_unit12_git_edge_state state{};
    slcli_source_unit12_git_edge edge{};
    std::size_t consumed=0;
};
struct NameControl {
    const slcli_source_unit12_git_name_order_state* stateOriginal=nullptr;
    slcli_source_unit12_git_name_order_state stateBefore{};
    const unsigned char* previousOriginal=nullptr;
    const unsigned char* currentOriginal=nullptr;
    std::uint64_t previousOffset=0,currentOffset=0;
    std::size_t previousExtent=0,currentExtent=0,previousUsed=0,currentUsed=0;
    int returned=0;
    bool attempted=false,callReturned=false;
    NativeClockOriginal before{},after{};
};
struct NameOutput {
    slcli_source_unit12_git_name_order_state state{};
    std::size_t previousUsed=0,currentUsed=0;
};
struct DestinationIndexOriginal {
    SlotShape slot{};
    bool claimAttempted=false,claimReturned=false,nativeAttempted=false,nativeReturned=false;
    bool storageAttempted=false,storageReturned=false,readbackAttempted=false,readbackReturned=false;
    bool committed=false,extentUnknown=false;
};
struct DestinationFdOriginal {
    int descriptor=-1;
    struct stat status{};
    int statResult=-1,statError=0,flagsResult=-1,flagsError=0;
    bool statAttempted=false,statReturned=false,flagsAttempted=false,flagsReturned=false;
    NativeClockOriginal statBefore{},statAfter{},flagsBefore{},flagsAfter{};
};
struct DestinationStorageControl {
    const ClaimGeometryOriginal* claimOriginal=nullptr;
    const DestinationFdOriginal* writeFdOriginal=nullptr;
    const DestinationFdOriginal* readFdOriginal=nullptr;
    int writeDescriptor=-1,readDescriptor=-1;
    struct iovec writeVector{},readVector{};
    NativeByteControl write{},read{};
    std::size_t comparedPrefix=0;
    bool comparisonAttempted=false,comparisonReturned=false,comparisonFailed=false;
};
// A terminal native storage cut remains resident, never recursively certified
// by pretending the final write's result was inside its own earlier payload.
struct DestinationTerminalOriginal {
    ClaimGeometryOriginal activeClaim{};
    DestinationStorageControl activeStorage{};
    DestinationIndexOriginal activeIndex{};
    DestinationFdOriginal writer{},readback{};
    NativeClockOriginal finalVerification{};
    std::size_t recordPrefix=0,readbackPrefix=0;
    bool issuingReturned=false,borrowersSettled=false,unknownExtent=false,failed=false;
};
struct DestinationRoleGeometry {
    DestinationOperation operation{};
    std::uint64_t inputBytes=0,outputBytes=0,requested=0;
    // Two-window pure comparison retains the exact separate split.
    std::uint64_t previousInputBytes=0,currentInputBytes=0;
};
inline bool CheckedLayoutAdd(std::uint64_t a,std::uint64_t b,std::uint64_t& out) noexcept {
    if(b>UINT64_MAX-a)return false;out=a+b;return true;
}
inline bool DestinationRoleExtents(const DestinationRoleGeometry& r,
    std::uint64_t& control,std::uint64_t& input,std::uint64_t& output) noexcept {
    std::uint64_t nativeControl=0;
    switch(r.operation) {
    case DestinationOperation::SourcePread: case DestinationOperation::StorePread:
        if(r.inputBytes||r.outputBytes!=r.requested||r.previousInputBytes||r.currentInputBytes)return false;
        nativeControl=sizeof(NativeByteControl);break;
    case DestinationOperation::SourcePwrite: case DestinationOperation::StorePwrite:
        if(r.outputBytes||r.inputBytes!=r.requested||r.previousInputBytes||r.currentInputBytes)return false;
        nativeControl=sizeof(NativeByteControl);break;
    case DestinationOperation::Fstat:
        if(r.inputBytes||r.outputBytes!=sizeof(struct stat)||r.requested!=sizeof(struct stat)||
           r.previousInputBytes||r.currentInputBytes)return false;
        nativeControl=sizeof(NativeStatControl);break;
    case DestinationOperation::GetFlags:
        // fcntl's scalar return is a control original, no byte-output buffer.
        if(r.inputBytes||r.outputBytes||r.requested||r.previousInputBytes||r.currentInputBytes)return false;
        nativeControl=sizeof(NativeFlagsControl);break;
    case DestinationOperation::GitEdgeFeed:
        if(r.inputBytes!=r.requested||r.outputBytes!=sizeof(EdgeOutput)||r.previousInputBytes||r.currentInputBytes)return false;
        nativeControl=sizeof(EdgeControl);break;
    case DestinationOperation::GitNameFeed: {
        std::uint64_t pair=0;
        if(!CheckedLayoutAdd(r.previousInputBytes,r.currentInputBytes,pair)||r.inputBytes!=pair||
           r.requested!=pair||r.outputBytes!=sizeof(NameOutput))return false;
        nativeControl=sizeof(NameControl);break;
    }
    default:return false;
    }
    std::uint64_t completeControl=0;
    if(!CheckedLayoutAdd(DestinationRecordHeaderBytes,nativeControl,completeControl))return false;
    control=completeControl;input=r.inputBytes;output=r.outputBytes;return true;
}
inline bool DestinationSlotRolesMatch(const SlotShape& slot,const DestinationRoleGeometry& r) noexcept {
    std::uint64_t control=0,input=0,output=0;
    switch(r.operation){
    case DestinationOperation::Fstat:case DestinationOperation::GetFlags:
    case DestinationOperation::GitEdgeFeed:case DestinationOperation::GitNameFeed:
        // These APIs have no single native file-position operand. Pure window
        // offsets remain in their distinct actual control operands.
        if(slot.sourceOffset)return false;break;
    default:break;
    }
    return DestinationRoleExtents(r,control,input,output)&&slot.abiVersion==DestinationLayoutVersion&&
        slot.operation==static_cast<std::uint32_t>(r.operation)&&slot.requested==r.requested&&
        slot.control.bytes==control&&slot.input.bytes==input&&slot.output.bytes==output;
}
struct DestinationClaimGeometryOriginal {
    ClaimGeometryOriginal geometry{};
    const DestinationRoleGeometry* roleOriginal=nullptr;
    const DestinationRoleGeometry* previousRoleOriginal=nullptr;
    DestinationRoleGeometry roleValue{},previousRoleValue{};
};
inline bool SameDestinationRole(const DestinationRoleGeometry& a,const DestinationRoleGeometry& b) noexcept {
    return a.operation==b.operation&&a.inputBytes==b.inputBytes&&a.outputBytes==b.outputBytes&&
        a.requested==b.requested&&a.previousInputBytes==b.previousInputBytes&&a.currentInputBytes==b.currentInputBytes;
}
inline bool DestinationClaimSeparate(const DestinationClaimGeometryOriginal& out,
    const ReservationShape* reservation,const SlotShape* source,const SlotShape* previous,
    const DestinationRoleGeometry* role,const DestinationRoleGeometry* previousRole) noexcept {
    return reservation&&source&&role&&StorageSeparate(&out,sizeof(out),reservation,sizeof(*reservation))&&
        StorageSeparate(&out,sizeof(out),source,sizeof(*source))&&
        StorageSeparate(&out,sizeof(out),role,sizeof(*role))&&
        (!previous||StorageSeparate(&out,sizeof(out),previous,sizeof(*previous)))&&
        (!previousRole||StorageSeparate(&out,sizeof(out),previousRole,sizeof(*previousRole)));
}
inline bool DestinationClaimGeometry(const ReservationShape& reservation,std::uint64_t next,
    const SlotShape& source,const SlotShape* previous,const DestinationRoleGeometry& role,
    const DestinationRoleGeometry* previousRole,DestinationClaimGeometryOriginal& out) noexcept {
    if(!DestinationClaimSeparate(out,&reservation,&source,previous,&role,previousRole)||
       bool(previous)!=bool(previousRole)||!DestinationSlotRolesMatch(source,role)||
       (previous&&!DestinationSlotRolesMatch(*previous,*previousRole)))return false;
    ClaimGeometryOriginal geometry{};
    if(!ClaimGeometry(reservation,next,source,previous,geometry))return false;
    const DestinationClaimGeometryOriginal value{geometry,&role,previousRole,role,
        previousRole?*previousRole:DestinationRoleGeometry{}};
    out=value;return true;
}
inline bool DestinationClaimReturnMatches(const DestinationClaimGeometryOriginal& c) noexcept {
    // Whole owner exclusions precede pointed original dereferences; a pointer
    // into either retained snapshot cannot manufacture its own history.
    if(!DestinationClaimSeparate(c,c.geometry.reservationOriginal,c.geometry.sourceOwnedSlotOriginal,
        c.geometry.previousSlotOriginal,c.roleOriginal,c.previousRoleOriginal)||
       bool(c.geometry.previousSlotOriginal)!=bool(c.previousRoleOriginal)||
       !SameDestinationRole(*c.roleOriginal,c.roleValue)||
       (c.previousRoleOriginal&&!SameDestinationRole(*c.previousRoleOriginal,c.previousRoleValue))||
       !ClaimGeometryReturnMatches(c.geometry))return false;
    DestinationClaimGeometryOriginal reconstructed{};
    return DestinationClaimGeometry(*c.geometry.reservationOriginal,c.geometry.value.sequence,
        *c.geometry.sourceOwnedSlotOriginal,c.geometry.previousSlotOriginal,*c.roleOriginal,
        c.previousRoleOriginal,reconstructed)&&SameSlot(c.geometry.value,reconstructed.geometry.value)&&
        SameDestinationRole(c.roleValue,reconstructed.roleValue);
}
// All native bytes, including initialized padding, are retained local ABI
// observations. They are not portable structs, source authenticity or authority.
// Header LE fields: magic8; version/op u32; seq,prior,C,I,O,q,sourceOffset,
// native-layout discriminator u64 (80 bytes). Index LE fields: seq/prior u64,
// slot/control/input/output each {offset,bytes} u64 pair, op/version u32 (88).
// Full raw control bytes follow header; exact input then maximum output follows.
// Pure header/index/control/input/output transforms now live in the destination
// codec header. Actual native-layout discriminator/profile acquisition, settled
// issuing caller and input nonmutation, genuine reserve/effect owner, independent
// 100/storage/finite-terminal/last-borrower joins remain unimplemented.
// Pure byte equality never supplies these source/native authority relations.
}

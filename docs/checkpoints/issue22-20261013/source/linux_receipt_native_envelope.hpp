#pragma once
#include "linux_receipt_destination_layout.hpp"
#include <type_traits>

// Pure native-storage geometry, not mapped storage or acquired reservation.
// Native objects must be constructed by the separate actual owning caller.
namespace template_creator_receipts {
struct NativeEnvelopeShape {
    std::uint64_t sequence=0,predecessorSequence=0;
    DestinationOperation operation{};
    Interval whole{},leadingPadding{},control{},input{},outputPadding{},output{};
    std::uint64_t controlAlignment=0,outputAlignment=0,requested=0,sourceOffset=0;
};
inline bool NativeEnvelopeTypes(const DestinationRoleGeometry& role,
    std::uint64_t& controlBytes,std::uint64_t& controlAlignment,
    std::uint64_t& outputAlignment) noexcept {
    std::uint64_t serializedControl=0,input=0,output=0;
    if(!DestinationRoleExtents(role,serializedControl,input,output))return false;
    controlBytes=serializedControl-DestinationRecordHeaderBytes;
    switch(role.operation){
    case DestinationOperation::SourcePread:case DestinationOperation::SourcePwrite:
    case DestinationOperation::StorePwrite:case DestinationOperation::StorePread:
        controlAlignment=alignof(NativeByteControl);outputAlignment=1;break;
    case DestinationOperation::Fstat:
        controlAlignment=alignof(NativeStatControl);outputAlignment=alignof(struct stat);break;
    case DestinationOperation::GetFlags:
        controlAlignment=alignof(NativeFlagsControl);outputAlignment=1;break;
    case DestinationOperation::GitEdgeFeed:
        controlAlignment=alignof(EdgeControl);outputAlignment=alignof(EdgeOutput);break;
    case DestinationOperation::GitNameFeed:
        controlAlignment=alignof(NameControl);outputAlignment=alignof(NameOutput);break;
    default:return false;
    }
    return controlBytes&&controlAlignment&&outputAlignment;
}
inline bool NativeEnvelopeAlign(std::uint64_t offset,std::uint64_t alignment,
    std::uint64_t& aligned) noexcept {
    if(!alignment)return false;
    const auto remainder=offset%alignment,padding=remainder?alignment-remainder:0;
    if(padding>UINT64_MAX-offset)return false;
    aligned=offset+padding;return true;
}
inline bool NativeEnvelopeGeometry(const ReservationShape& reservation,std::uint64_t sequence,
    const NativeEnvelopeShape* previous,const DestinationRoleGeometry& role,
    std::uint64_t sourceOffset,NativeEnvelopeShape& out) noexcept {
    // Output cannot alias any source input. No pointer presence grants custody.
    if(!StorageSeparate(&out,sizeof(out),&reservation,sizeof(reservation))||
       !StorageSeparate(&out,sizeof(out),&role,sizeof(role))||
       (previous&&!StorageSeparate(&out,sizeof(out),previous,sizeof(*previous))))return false;
    if(!sequence||!ShapePreflight(reservation,reservation.fileBytes))return false;
    NativeEnvelopeShape value{};value.sequence=sequence;value.operation=role.operation;
    value.requested=role.requested;value.sourceOffset=sourceOffset;
    if((role.operation==DestinationOperation::Fstat||role.operation==DestinationOperation::GetFlags||
        role.operation==DestinationOperation::GitEdgeFeed||role.operation==DestinationOperation::GitNameFeed)&&sourceOffset)return false;
    std::uint64_t start=reservation.records.offset;
    if(previous){
        if(previous->sequence==UINT64_MAX||sequence!=previous->sequence+1||
           !Within(previous->whole,reservation.records)||!End(previous->whole,start))return false;
        value.predecessorSequence=previous->sequence;
    }else if(sequence!=1)return false;
    std::uint64_t controlBytes=0,controlStart=0,controlEnd=0,inputEnd=0,outputStart=0,end=0;
    if(!NativeEnvelopeTypes(role,controlBytes,value.controlAlignment,value.outputAlignment)||
       !NativeEnvelopeAlign(start,value.controlAlignment,controlStart))return false;
    value.leadingPadding={start,controlStart-start};value.control={controlStart,controlBytes};
    if(!End(value.control,controlEnd))return false;
    value.input={controlEnd,role.inputBytes};
    if(!End(value.input,inputEnd)||!NativeEnvelopeAlign(inputEnd,value.outputAlignment,outputStart))return false;
    value.outputPadding={inputEnd,outputStart-inputEnd};value.output={outputStart,role.outputBytes};
    if(!End(value.output,end)||end<start)return false;
    value.whole={start,end-start};
    if(!value.whole.bytes||value.whole.bytes>reservation.maximumRecordBytes||
       !Within(value.whole,reservation.records))return false;
    out=value;return true;
}
inline bool NativeEnvelopeMappedAlignment(const NativeEnvelopeShape& shape,
    std::uintptr_t actualMappingBase,std::uint64_t mappedFileOffset,std::size_t mappedBytes) noexcept {
    // An actual owning caller must supply the successful live mapped extent,
    // never a guessed base or a serialized locator. This is only arithmetic.
    if(!actualMappingBase||mappedBytes>UINTPTR_MAX-actualMappingBase||
       !shape.controlAlignment||!shape.outputAlignment||
       !Within(shape.whole,{mappedFileOffset,mappedBytes})||
       !Within(shape.control,shape.whole)||!Within(shape.output,shape.whole)||
       shape.control.offset<mappedFileOffset||shape.output.offset<mappedFileOffset)return false;
    const auto controlDelta=shape.control.offset-mappedFileOffset,outputDelta=shape.output.offset-mappedFileOffset;
    if(controlDelta>UINTPTR_MAX-actualMappingBase||outputDelta>UINTPTR_MAX-actualMappingBase)return false;
    return (actualMappingBase+static_cast<std::uintptr_t>(controlDelta))%shape.controlAlignment==0&&
        (actualMappingBase+static_cast<std::uintptr_t>(outputDelta))%shape.outputAlignment==0;
}
inline bool SameNativeEnvelope(const NativeEnvelopeShape& a,const NativeEnvelopeShape& b) noexcept {
    const auto same=[](const Interval& x,const Interval& y){return x.offset==y.offset&&x.bytes==y.bytes;};
    return a.sequence==b.sequence&&a.predecessorSequence==b.predecessorSequence&&a.operation==b.operation&&
        same(a.whole,b.whole)&&same(a.leadingPadding,b.leadingPadding)&&same(a.control,b.control)&&
        same(a.input,b.input)&&same(a.outputPadding,b.outputPadding)&&same(a.output,b.output)&&
        a.controlAlignment==b.controlAlignment&&a.outputAlignment==b.outputAlignment&&
        a.requested==b.requested&&a.sourceOffset==b.sourceOffset;
}
struct NativeEnvelopeClaimOriginal {
    const ReservationShape* reservationOriginal=nullptr;
    const DestinationRoleGeometry* roleOriginal=nullptr;
    const NativeEnvelopeShape* previousOriginal=nullptr;
    std::uint64_t sequenceSnapshot=0,sourceOffsetSnapshot=0;
    ReservationShape reservationSnapshot{};
    DestinationRoleGeometry roleSnapshot{};
    NativeEnvelopeShape previousSnapshot{},value{};
};
inline bool SameNativeReservation(const ReservationShape& a,const ReservationShape& b) noexcept {
    return a.reservationSequence==b.reservationSequence&&a.fileBytes==b.fileBytes&&
        a.maximumRecordBytes==b.maximumRecordBytes&&a.nativeAttempts==b.nativeAttempts&&
        a.pureAttempts==b.pureAttempts&&a.indexNodes==b.indexNodes&&a.failureTailBytes==b.failureTailBytes&&
        SameInterval(a.records,b.records)&&SameInterval(a.indexes,b.indexes)&&SameInterval(a.terminal,b.terminal);
}
inline bool NativeEnvelopeClaimSourcesSeparate(const NativeEnvelopeClaimOriginal& owner,
    const ReservationShape* reservation,const DestinationRoleGeometry* role,
    const NativeEnvelopeShape* previous) noexcept {
    return reservation&&role&&StorageSeparate(&owner,sizeof(owner),reservation,sizeof(*reservation))&&
        StorageSeparate(&owner,sizeof(owner),role,sizeof(*role))&&
        (!previous||StorageSeparate(&owner,sizeof(owner),previous,sizeof(*previous)));
}
inline bool ClaimNativeEnvelope(const ReservationShape& reservation,const DestinationRoleGeometry& role,
    std::uint64_t sequence,const NativeEnvelopeShape* previous,std::uint64_t sourceOffset,
    NativeEnvelopeClaimOriginal& owner) noexcept {
    if(!NativeEnvelopeClaimSourcesSeparate(owner,&reservation,&role,previous))return false;
    NativeEnvelopeClaimOriginal value{};
    if(!NativeEnvelopeGeometry(reservation,sequence,previous,role,sourceOffset,value.value))return false;
    value.reservationOriginal=&reservation;value.roleOriginal=&role;value.previousOriginal=previous;
    value.sequenceSnapshot=sequence;value.sourceOffsetSnapshot=sourceOffset;
    value.reservationSnapshot=reservation;value.roleSnapshot=role;if(previous)value.previousSnapshot=*previous;
    owner=value;return true;
}
inline bool NativeEnvelopeClaimReturnMatches(const NativeEnvelopeClaimOriginal& owner) noexcept {
    // Actual owner/origin exclusions precede any pointed source dereference.
    if(!NativeEnvelopeClaimSourcesSeparate(owner,owner.reservationOriginal,owner.roleOriginal,owner.previousOriginal)||
       !SameNativeReservation(*owner.reservationOriginal,owner.reservationSnapshot)||
       !SameDestinationRole(*owner.roleOriginal,owner.roleSnapshot)||
       (owner.previousOriginal&&!SameNativeEnvelope(*owner.previousOriginal,owner.previousSnapshot))||
       owner.value.sequence!=owner.sequenceSnapshot||owner.value.sourceOffset!=owner.sourceOffsetSnapshot)return false;
    NativeEnvelopeShape rebuilt{};
    return NativeEnvelopeGeometry(*owner.reservationOriginal,owner.sequenceSnapshot,owner.previousOriginal,
        *owner.roleOriginal,owner.sourceOffsetSnapshot,rebuilt)&&SameNativeEnvelope(rebuilt,owner.value);
    // These retained snapshots detect changed local geometry; neither pointers
    // nor snapshots independently authenticate the predecessor/source/reserve.
}
// Returning checks do read retained source originals after owner exclusions.
// No construct_at, mmap, mapping-base conversion/dereference, IO, lifetime
// settlement, native-output or source/reserve acquisition is performed here.
}

#include "linux_receipt_foundation_geometry.hpp"
#include "linux_receipt_destination_layout.hpp"
#include "linux_receipt_destination_codec.hpp"
#include "linux_receipt_native_envelope.hpp"
#include "linux_receipt_native_storage_plan.hpp"
// Synthetic pure geometry only; UNRUN. No authentic reservation or native IO.
int main(){
    using namespace template_creator_receipts;
    ReservationShape r{1,4096,512,8,8,4,64,{0,2048},{2048,1024},{3072,1024}};
    SlotShape first{1,0,1,1,{0,256},{0,64},{64,64},{128,128},128,0};
    SlotShape second{2,1,1,1,{256,256},{256,64},{320,64},{384,128},128,128};
    ClaimGeometryOriginal a{},b{};
    if(!ClaimGeometry(r,1,first,nullptr,a)||!ClaimGeometryReturnMatches(a)||
       !ClaimGeometry(r,2,second,&first,b)||!ClaimGeometryReturnMatches(b))return 1;
    const auto retained=b;
    auto gap=second;gap.slot.offset++;
    if(ClaimGeometry(r,2,gap,&first,b)||b.sourceOwnedSlotOriginal!=retained.sourceOwnedSlotOriginal)return 2;
    auto badPredecessor=second;badPredecessor.predecessorSequence=0;
    if(ClaimGeometry(r,2,badPredecessor,&first,b))return 3;
    first.output.bytes--;
    if(ClaimGeometryReturnMatches(a)||ClaimGeometryReturnMatches(b))return 4;
    first.output.bytes++;
    auto self=a;self.sourceOwnedSlotOriginal=&self.value;
    if(ClaimGeometryReturnMatches(self))return 7;
    r.indexNodes++;
    if(ClaimGeometryReturnMatches(a))return 8;
    r.indexNodes--;
    auto overlap=r;overlap.indexes.offset=1024;
    if(ShapePreflight(overlap,4096))return 5;
    auto overflow=second;overflow.slot={UINT64_MAX-1,256};
    if(ClaimGeometry(r,2,overflow,&first,b))return 6;
    // Exact production role/claim bodies, synthetic geometry only. No fake IO.
    DestinationRoleGeometry write{DestinationOperation::SourcePwrite,7,0,7,0,0};
    std::uint64_t control=0,input=0,output=0;
    if(!DestinationRoleExtents(write,control,input,output)||input!=7||output)return 9;
    const auto bytes=control+input+output;
    ReservationShape local{1,bytes+512,bytes,2,0,1,64,{0,bytes},{bytes,128},{bytes+128,384}};
    SlotShape writeSlot{1,0,static_cast<std::uint32_t>(write.operation),DestinationLayoutVersion,
        {0,bytes},{0,control},{control,input},{control+input,output},7,0};
    DestinationClaimGeometryOriginal writeClaim{};
    if(!DestinationClaimGeometry(local,1,writeSlot,nullptr,write,nullptr,writeClaim)||
       !DestinationClaimReturnMatches(writeClaim))return 10;
    DestinationRoleGeometry wrongRead{DestinationOperation::SourcePread,7,0,7,0,0};
    const auto preserved=writeClaim;
    if(DestinationClaimGeometry(local,1,writeSlot,nullptr,wrongRead,nullptr,writeClaim)||
       writeClaim.roleOriginal!=preserved.roleOriginal)return 11;
    write.inputBytes=6;
    if(DestinationClaimReturnMatches(writeClaim))return 12;
    write.inputBytes=7;
    auto roleSelf=writeClaim;roleSelf.roleOriginal=&roleSelf.roleValue;
    if(DestinationClaimReturnMatches(roleSelf))return 13;
    auto slotSelf=writeClaim;slotSelf.geometry.sourceOwnedSlotOriginal=&slotSelf.geometry.value;
    if(DestinationClaimReturnMatches(slotSelf))return 14;
    auto badSlot=writeSlot;badSlot.output.bytes=7;
    if(DestinationClaimGeometry(local,1,badSlot,nullptr,write,nullptr,writeClaim))return 15;
    DestinationRoleGeometry pair{DestinationOperation::GitNameFeed,0,sizeof(NameOutput),0,UINT64_MAX,1};
    if(DestinationRoleExtents(pair,control,input,output))return 16;
    DestinationHeaderBytes header{};DestinationIndexRecordBytes index{};
    if(!EncodeDestinationHeader(writeClaim,7,header)||!DestinationHeaderReturnMatches(header,writeClaim,7)||
       !EncodeDestinationIndex(writeClaim,index)||!DestinationIndexReturnMatches(index,writeClaim))return 17;
    const auto savedHeader=header;
    if(EncodeDestinationHeader(writeClaim,0,header)||header!=savedHeader||
       DestinationHeaderReturnMatches(header,writeClaim,8))return 18;
    auto corruptHeader=header;corruptHeader[0]^=1;
    DestinationDecodedHeader decoded{};decoded.version=999;
    if(DecodeDestinationHeader(corruptHeader,writeClaim,7,decoded)||decoded.version!=999)return 19;
    corruptHeader=header;corruptHeader[40]^=1;
    if(DestinationHeaderReturnMatches(corruptHeader,writeClaim,7))return 20;
    auto corruptIndex=index;corruptIndex[24]^=1;
    SlotShape decodedSlot{};decodedSlot.sequence=999;
    if(DecodeDestinationIndex(corruptIndex,writeClaim,decodedSlot)||decodedSlot.sequence!=999)return 21;
    if(DestinationCodecOutputSeparate(&writeClaim.roleValue,sizeof(writeClaim.roleValue),writeClaim))return 22;
    // Synthetic representation cells only, never actual syscall observations.
    unsigned char sourceBytes[3]={0x61,0x62,0x63};
    DestinationRoleGeometry read{DestinationOperation::SourcePread,0,3,3,0,0};
    if(!DestinationRoleExtents(read,control,input,output))return 23;
    const auto readBytes=control+input+output;
    ReservationShape readReservation{1,readBytes+512,readBytes,1,0,1,64,{0,readBytes},{readBytes,128},{readBytes+128,384}};
    SlotShape readSlot{1,0,1,1,{0,readBytes},{0,control},{control,0},{control,3},3,0};
    DestinationClaimGeometryOriginal readClaim{};
    if(!DestinationClaimGeometry(readReservation,1,readSlot,nullptr,read,nullptr,readClaim))return 24;
    NativeByteControl native{};native.descriptor=9;native.bufferOriginal=sourceBytes;native.bufferExtent=3;
    native.requested=3;native.returned=2;native.actual=2;
    native.captureStarted=true;native.callAttempted=true;native.callReturned=true;native.extentKnown=true;
    std::array<unsigned char,80+sizeof(NativeByteControl)+3> record{};
    if(!EncodeDestinationByteRecord(readClaim,native,7,record)||
       !DestinationByteRecordReturnMatches(record,readClaim,native,7)||record.back())return 25;
    record.back()=1;
    if(DestinationByteRecordReturnMatches(record,readClaim,native,7))return 26;
    record.back()=0;sourceBytes[0]^=1;
    if(DestinationByteRecordReturnMatches(record,readClaim,native,7))return 27;
    sourceBytes[0]^=1;
    native.returned=7;native.actual=0;native.extentKnown=false;
    if(!EncodeDestinationByteRecord(readClaim,native,7,record)||
       !DestinationByteRecordReturnMatches(record,readClaim,native,7))return 28;
    const auto unknownRecord=record;native.callReturned=false;
    if(EncodeDestinationByteRecord(readClaim,native,7,record)||record!=unknownRecord)return 29;
    // Query/pure cells below are synthetic representation inputs, not native
    // observations or successful parser semantics. Exercise the actual codec.
    auto claimFor=[](DestinationRoleGeometry& role,ReservationShape& reserve,
        SlotShape& slot,DestinationClaimGeometryOriginal& claim){
        std::uint64_t c=0,i=0,o=0;
        if(!DestinationRoleExtents(role,c,i,o))return false;
        const auto n=c+i+o;
        reserve={1,n+512,n,1,0,1,64,{0,n},{n,128},{n+128,384}};
        slot={1,0,static_cast<std::uint32_t>(role.operation),1,
            {0,n},{0,c},{c,i},{c+i,o},role.requested,0};
        return DestinationClaimGeometry(reserve,1,slot,nullptr,role,nullptr,claim);
    };
    DestinationRoleGeometry flagsRole{DestinationOperation::GetFlags,0,0,0,0,0};
    ReservationShape flagsReserve{};SlotShape flagsSlot{};DestinationClaimGeometryOriginal flagsClaim{};
    if(!claimFor(flagsRole,flagsReserve,flagsSlot,flagsClaim))return 30;
    NativeFlagsControl flags{};flags.command=F_GETFL;flags.captureStarted=true;
    flags.callAttempted=true;flags.callReturned=true;
    std::array<unsigned char,80+sizeof(NativeFlagsControl)> flagsRecord{};
    if(!EncodeDestinationFlagsRecord(flagsClaim,flags,7,flagsRecord)||
       !DestinationFlagsRecordReturnMatches(flagsRecord,flagsClaim,flags,7))return 31;
    const auto savedFlags=flagsRecord;flags.command=F_GETFD;
    if(EncodeDestinationFlagsRecord(flagsClaim,flags,7,flagsRecord)||flagsRecord!=savedFlags)return 32;
    flags.command=F_GETFL;flagsSlot.sourceOffset=1;
    if(DestinationFlagsRecordReturnMatches(flagsRecord,flagsClaim,flags,7))return 33;
    DestinationRoleGeometry statRole{DestinationOperation::Fstat,0,sizeof(struct stat),sizeof(struct stat),0,0};
    ReservationShape statReserve{};SlotShape statSlot{};DestinationClaimGeometryOriginal statClaim{};
    if(!claimFor(statRole,statReserve,statSlot,statClaim))return 34;
    NativeStatControl statControl{};statControl.captureStarted=true;
    statControl.callAttempted=true;statControl.callReturned=true;struct stat statOutput{};
    std::array<unsigned char,80+sizeof(NativeStatControl)+sizeof(struct stat)> statRecord{};
    if(!EncodeDestinationStatRecord(statClaim,statControl,statOutput,7,statRecord)||
       !DestinationStatRecordReturnMatches(statRecord,statClaim,statControl,statOutput,7))return 35;
    statOutput.st_size=1;
    if(DestinationStatRecordReturnMatches(statRecord,statClaim,statControl,statOutput,7))return 36;
    DestinationRoleGeometry edgeRole{DestinationOperation::GitEdgeFeed,3,sizeof(EdgeOutput),3,0,0};
    ReservationShape edgeReserve{};SlotShape edgeSlot{};DestinationClaimGeometryOriginal edgeClaim{};
    if(!claimFor(edgeRole,edgeReserve,edgeSlot,edgeClaim))return 37;
    EdgeControl edgeControl{};EdgeOutput edgeResult{};
    edgeControl.inputOriginal=sourceBytes;edgeControl.inputExtent=3;
    edgeControl.attempted=true;edgeControl.callReturned=true;
    std::array<unsigned char,80+sizeof(EdgeControl)+3+sizeof(EdgeOutput)> edgeRecord{};
    if(!EncodeDestinationEdgeRecord(edgeClaim,edgeControl,edgeResult,7,edgeRecord)||
       !DestinationEdgeRecordReturnMatches(edgeRecord,edgeClaim,edgeControl,edgeResult,7))return 38;
    edgeResult.consumed=1;
    if(DestinationEdgeRecordReturnMatches(edgeRecord,edgeClaim,edgeControl,edgeResult,7))return 39;
    edgeResult.consumed=0;
    // The helper checks interval aliasing without dereferencing or manufacturing
    // a typed object inside the byte destination.
    edgeControl.edgeOriginal=&edgeResult.edge;
    if(destination_codec_detail::EdgePointersSeparate(&edgeResult.edge,sizeof(edgeResult.edge),edgeControl,edgeResult))return 40;
    unsigned char previousBytes[2]={0x61,0x62},currentBytes[1]={0x63};
    DestinationRoleGeometry nameRole{DestinationOperation::GitNameFeed,3,sizeof(NameOutput),3,2,1};
    ReservationShape nameReserve{};SlotShape nameSlot{};DestinationClaimGeometryOriginal nameClaim{};
    if(!claimFor(nameRole,nameReserve,nameSlot,nameClaim))return 41;
    NameControl nameControl{};NameOutput nameResult{};
    nameControl.previousOriginal=previousBytes;nameControl.currentOriginal=currentBytes;
    nameControl.previousExtent=2;nameControl.currentExtent=1;
    nameControl.attempted=true;nameControl.callReturned=true;
    std::array<unsigned char,80+sizeof(NameControl)+3+sizeof(NameOutput)> nameRecord{};
    if(!EncodeDestinationNameRecord(nameClaim,nameControl,nameResult,7,nameRecord)||
       !DestinationNameRecordReturnMatches(nameRecord,nameClaim,nameControl,nameResult,7))return 42;
    previousBytes[1]^=1;
    if(DestinationNameRecordReturnMatches(nameRecord,nameClaim,nameControl,nameResult,7))return 43;
    previousBytes[1]^=1;currentBytes[0]^=1;
    if(DestinationNameRecordReturnMatches(nameRecord,nameClaim,nameControl,nameResult,7))return 44;
    currentBytes[0]^=1;nameControl.currentUsed=1;
    const auto savedName=nameRecord;
    if(EncodeDestinationNameRecord(nameClaim,nameControl,nameResult,7,nameRecord)||nameRecord!=savedName)return 45;
    // Native-envelope layout remains a different production recipe from bytes.
    ReservationShape nativeReserve{1,4096,2048,8,8,8,64,{1,2047},{2048,1024},{3072,1024}};
    NativeEnvelopeShape nativeFirst{},nativeSecond{};
    if(!NativeEnvelopeGeometry(nativeReserve,1,nullptr,statRole,0,nativeFirst)||
       nativeFirst.control.bytes!=sizeof(NativeStatControl)||
       nativeFirst.output.bytes!=sizeof(struct stat)||
       !NativeEnvelopeGeometry(nativeReserve,2,&nativeFirst,nameRole,0,nativeSecond))return 46;
    std::uint64_t priorEnd=0;
    if(!End(nativeFirst.whole,priorEnd)||nativeSecond.whole.offset!=priorEnd)return 47;
    const auto savedNative=nativeSecond;
    if(NativeEnvelopeGeometry(nativeReserve,2,&nativeFirst,nameRole,1,nativeSecond)||
       nativeSecond.whole.offset!=savedNative.whole.offset)return 48;
    if(NativeEnvelopeGeometry(nativeReserve,2,&nativeSecond,nameRole,0,nativeSecond))return 49;
    // Synthetic numeric base only; never a live mapping or pointer dereference.
    if(!NativeEnvelopeMappedAlignment(nativeFirst,4096,0,4096))return 50;
    auto malformedNative=nativeFirst;malformedNative.output.offset=4096;
    if(NativeEnvelopeMappedAlignment(malformedNative,4096,0,4096))return 51;
    NativeEnvelopeClaimOriginal nativeClaim{};
    if(!ClaimNativeEnvelope(nativeReserve,nameRole,2,&nativeFirst,0,nativeClaim)||
       !NativeEnvelopeClaimReturnMatches(nativeClaim))return 52;
    nativeFirst.whole.bytes++;
    if(NativeEnvelopeClaimReturnMatches(nativeClaim))return 53;
    nativeFirst.whole.bytes--;
    auto aliasedNative=nativeClaim;aliasedNative.previousOriginal=&aliasedNative.previousSnapshot;
    if(NativeEnvelopeClaimReturnMatches(aliasedNative))return 54;
    nativeReserve.nativeAttempts++;
    if(NativeEnvelopeClaimReturnMatches(nativeClaim))return 55;
    nativeReserve.nativeAttempts--;
    NativeEnvelopeClaimOriginal byteNative{};
    if(!ClaimNativeEnvelope(nativeReserve,read,1,nullptr,7,byteNative)||
       !NativeEnvelopeClaimReturnMatches(byteNative))return 56;
    byteNative.value.sourceOffset=8;
    if(NativeEnvelopeClaimReturnMatches(byteNative))return 57;
    const std::uint64_t indexBytes=sizeof(NativeStorageIndexOriginal)+alignof(NativeStorageIndexOriginal)-1;
    const std::uint64_t terminalBytes=sizeof(NativeStorageTerminalCut)+alignof(NativeStorageTerminalCut)-1+64;
    ReservationShape backingReserve{1,2048+indexBytes+terminalBytes,2048,8,8,1,64,
        {0,2048},{2048,indexBytes},{2048+indexBytes,terminalBytes}};
    NativeStorageBackingShape backing{};
    if(!NativeStorageBackingGeometry(backingReserve,backing)||backing.indexRows!=1||
       backing.indexRowBytes!=sizeof(NativeStorageIndexOriginal)||
       backing.residentTerminalBytes!=sizeof(NativeStorageTerminalOriginal))return 58;
    const auto savedBacking=backing;backingReserve.indexes.bytes=sizeof(NativeStorageIndexOriginal)-1;
    if(NativeStorageBackingGeometry(backingReserve,backing)||
       backing.mappingVirtualBytes!=savedBacking.mappingVirtualBytes)return 59;
    return 0;
}

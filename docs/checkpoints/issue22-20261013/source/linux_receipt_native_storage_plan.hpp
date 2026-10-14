#pragma once
#include "linux_receipt_native_envelope.hpp"
#include <sys/mman.h>

namespace template_creator_receipts {
// Resident native outputs are owned before a mapping effect. Raw returned
// address is not interpreted as storage until the actual returned cut is clean.
struct NativeMappingOriginal {
    int descriptor=-1,protection=0,flags=0,error=0;
    void* requestedAddress=nullptr;
    void* returnedAddress=MAP_FAILED;
    std::size_t length=0;
    off_t fileOffset=0;
    bool captureStarted=false,callAttempted=false,callReturned=false;
    NativeClockOriginal before{},after{};
};
struct NativeUnmappingOriginal {
    void* actualAddress=nullptr;
    std::size_t actualLength=0;
    int returned=-1,error=0;
    bool captureStarted=false,callAttempted=false,callReturned=false;
    NativeClockOriginal before{},after{};
};
// Each row is an actual destination-native object, never an 88-byte decoded
// index. Construction, source origins and native/pure writers remain separate
// obligations of the real mapped owner. These flags cannot issue authority.
struct NativeStorageIndexOriginal {
    NativeEnvelopeClaimOriginal claim{};
    const NativeMappingOriginal* mappingOriginal=nullptr;
    void* controlOriginal=nullptr;
    void* outputOriginal=nullptr;
    bool constructionStarted=false,constructionReturned=false;
    bool effectStarted=false,effectReturned=false,unknownOutputExtent=false;
    bool independentReadbackStarted=false,independentReadbackReturned=false;
    NativeClockOriginal constructionBefore{},constructionAfter{},readbackFinal{};
};
struct NativeStorageTerminalOriginal {
    NativeMappingOriginal mapping{};
    NativeUnmappingOriginal unmapping{};
    const NativeStorageIndexOriginal* lastIndexOriginal=nullptr;
    const NativeEnvelopeClaimOriginal* pendingClaimOriginal=nullptr;
    NativeClockOriginal finalVerification{};
    bool issuingReturned=false,futureCallsClosed=false,failed=false;
    // Actual native/edge/graph/archive borrower proof cannot be replaced by
    // a free-standing count or a mutable caller-supplied settled boolean.
};
// This earlier mapped cut is not the owner of the later munmap result. The
// actual NativeStorageTerminalOriginal above stays resident outside the mapping.
struct NativeStorageTerminalCut {
    const NativeMappingOriginal* residentMappingOriginal=nullptr;
    const NativeStorageIndexOriginal* lastIndexOriginal=nullptr;
    const NativeEnvelopeClaimOriginal* pendingClaimOriginal=nullptr;
    NativeClockOriginal cut{};
};
struct NativeStorageBackingShape {
    std::uint64_t mappingVirtualBytes=0;
    Interval nativeIndexes{},indexLeadingPadding{},nativeTerminal{},terminalLeadingPadding{},failureTail{};
    std::uint64_t indexRows=0,indexRowBytes=0,terminalBytes=0,residentTerminalBytes=0;
};
inline bool NativeStorageBackingGeometry(const ReservationShape& reservation,
    NativeStorageBackingShape& out) noexcept {
    if(!StorageSeparate(&out,sizeof(out),&reservation,sizeof(reservation))||
       !ShapePreflight(reservation,reservation.fileBytes)||!reservation.indexNodes||
       reservation.fileBytes>SIZE_MAX)return false;
    NativeStorageBackingShape value{};value.mappingVirtualBytes=reservation.fileBytes;
    value.indexRows=reservation.indexNodes;value.indexRowBytes=sizeof(NativeStorageIndexOriginal);
    value.terminalBytes=sizeof(NativeStorageTerminalCut);
    value.residentTerminalBytes=sizeof(NativeStorageTerminalOriginal);
    std::uint64_t indexStart=0,terminalStart=0,indexBytes=0,terminalEnd=0;
    if(!NativeEnvelopeAlign(reservation.indexes.offset,alignof(NativeStorageIndexOriginal),indexStart)||
       !NativeEnvelopeAlign(reservation.terminal.offset,alignof(NativeStorageTerminalCut),terminalStart)||
       value.indexRows>UINT64_MAX/value.indexRowBytes)return false;
    indexBytes=value.indexRows*value.indexRowBytes;
    value.indexLeadingPadding={reservation.indexes.offset,indexStart-reservation.indexes.offset};
    value.nativeIndexes={indexStart,indexBytes};
    value.terminalLeadingPadding={reservation.terminal.offset,terminalStart-reservation.terminal.offset};
    value.nativeTerminal={terminalStart,value.terminalBytes};
    if(!Within(value.nativeIndexes,reservation.indexes)||!Within(value.nativeTerminal,reservation.terminal)||
       !End(value.nativeTerminal,terminalEnd))return false;
    value.failureTail={terminalEnd,reservation.failureTailBytes};
    if(!Within(value.failureTail,reservation.terminal))return false;
    out=value;return true;
}
// This is requested destination-native backing geometry only. Complete source
// rows, initialized/failed/pending overlap, virtual/page-table/file-cache/kernel
// RAM, CPU, runtime/CRT/stack/TLS/allocator, references and native attempts must
// independently fit the actual qualified initial profile before any mmap or
// object construction. No producer sizeof or successful mmap supplies that fit.
}

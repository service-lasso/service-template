#pragma once
#include <cstddef>
#include <cstdint>
#include <limits>

// Local pure geometry. No reservation, native effect, commit or admission.
namespace template_creator_receipts {
struct Interval { std::uint64_t offset{}, bytes{}; };
struct ReservationShape {
    std::uint64_t reservationSequence{}, fileBytes{}, maximumRecordBytes{};
    std::uint64_t nativeAttempts{}, pureAttempts{}, indexNodes{}, failureTailBytes{};
    Interval records{}, indexes{}, terminal{};
};
struct SlotShape {
    std::uint64_t sequence{}, predecessorSequence{};
    std::uint32_t operation{}, abiVersion{};
    Interval slot{}, control{}, input{}, output{};
    std::uint64_t requested{}, sourceOffset{};
};
struct ClaimGeometryOriginal {
    // Actual local origins must remain live. Fieldwise snapshots detect changed
    // geometry; they do not establish immutable ownership or authentication.
    const ReservationShape* reservationOriginal{};
    const SlotShape* sourceOwnedSlotOriginal{};
    const SlotShape* previousSlotOriginal{};
    ReservationShape reservationValue{};
    SlotShape previousValue{};
    SlotShape value{};
};
inline bool StorageSeparate(const void* a,std::size_t an,const void* b,std::size_t bn) noexcept {
    const auto av=reinterpret_cast<std::uintptr_t>(a),bv=reinterpret_cast<std::uintptr_t>(b);
    return a&&b&&an<=UINTPTR_MAX-av&&bn<=UINTPTR_MAX-bv&&(av+an<=bv||bv+bn<=av);
}
inline bool End(const Interval& i, std::uint64_t& end) noexcept {
    if(i.bytes>UINT64_MAX-i.offset)return false;
    end=i.offset+i.bytes;return true;
}
inline bool Within(const Interval& inner,const Interval& outer) noexcept {
    std::uint64_t ie{},oe{};
    return End(inner,ie)&&End(outer,oe)&&inner.offset>=outer.offset&&ie<=oe;
}
inline bool Separate(const Interval& a,const Interval& b) noexcept {
    std::uint64_t ae{},be{};
    return End(a,ae)&&End(b,be)&&(ae<=b.offset||be<=a.offset);
}
inline bool ShapePreflight(const ReservationShape& s,std::uint64_t actualFileBytes) noexcept {
    // These are supplied geometry, not a genuine allocation or finance original.
    if(!s.reservationSequence||!s.fileBytes||s.fileBytes!=actualFileBytes||
       !s.maximumRecordBytes||!s.records.bytes||!s.terminal.bytes||
       !s.failureTailBytes||s.failureTailBytes>s.terminal.bytes||
       s.nativeAttempts>UINT64_MAX-s.pureAttempts||
       !(s.nativeAttempts+s.pureAttempts))return false;
    const Interval file{0,s.fileBytes};
    return Within(s.records,file)&&Within(s.indexes,file)&&Within(s.terminal,file)&&
        Separate(s.records,s.indexes)&&Separate(s.records,s.terminal)&&
        Separate(s.indexes,s.terminal);
}
inline bool SlotLayout(const ReservationShape& r,const SlotShape& s) noexcept {
    std::uint64_t ce{},ie{},oe{},se{};
    if(!s.sequence||!s.operation||!s.abiVersion||!s.slot.bytes||
       !s.control.bytes||s.slot.bytes>r.maximumRecordBytes||
       !Within(s.slot,r.records)||!End(s.control,ce)||!End(s.input,ie)||
       !End(s.output,oe)||!End(s.slot,se))return false;
    return s.control.offset==s.slot.offset&&s.input.offset==ce&&
        s.output.offset==ie&&oe==se&&
        // This is only a dimensional bound. The destination closed operation
        // recipe separately requires the exact input/output role; writes carry
        // requested bytes in input, reads carry them in output.
        (s.requested<=s.input.bytes||s.requested<=s.output.bytes)&&
        s.sourceOffset<=UINT64_MAX-s.requested;
}
inline bool SameInterval(const Interval& a,const Interval& b) noexcept {
    return a.offset==b.offset&&a.bytes==b.bytes;
}
inline bool SameSlot(const SlotShape& a,const SlotShape& b) noexcept {
    return a.sequence==b.sequence&&a.predecessorSequence==b.predecessorSequence&&
        a.operation==b.operation&&a.abiVersion==b.abiVersion&&
        SameInterval(a.slot,b.slot)&&SameInterval(a.control,b.control)&&
        SameInterval(a.input,b.input)&&SameInterval(a.output,b.output)&&
        a.requested==b.requested&&a.sourceOffset==b.sourceOffset;
}
inline bool ClaimGeometry(const ReservationShape& r,std::uint64_t nextSequence,
    const SlotShape& source,const SlotShape* previous,ClaimGeometryOriginal& out) noexcept {
    // Input/output object disjointness precedes any output mutation.
    if(!StorageSeparate(&out,sizeof(out),&r,sizeof(r))||
       !StorageSeparate(&out,sizeof(out),&source,sizeof(source))||
       (previous&&!StorageSeparate(&out,sizeof(out),previous,sizeof(*previous)))||
       !ShapePreflight(r,r.fileBytes)||!SlotLayout(r,source)||
       source.sequence!=nextSequence||nextSequence>r.nativeAttempts+r.pureAttempts)return false;
    if(previous){
        std::uint64_t priorEnd{};
        if(previous==&source||!SlotLayout(r,*previous)||previous->sequence==UINT64_MAX||
           previous->sequence+1!=nextSequence||source.predecessorSequence!=previous->sequence||
           !End(previous->slot,priorEnd)||source.slot.offset!=priorEnd)return false;
    }else if(nextSequence!=1||source.predecessorSequence||source.slot.offset!=r.records.offset)return false;
    const ClaimGeometryOriginal value{&r,&source,previous,r,previous?*previous:SlotShape{},source};out=value;return true;
}
inline bool ClaimGeometryReturnMatches(const ClaimGeometryOriginal& c) noexcept {
    if(!c.reservationOriginal||!c.sourceOwnedSlotOriginal||
       !StorageSeparate(&c,sizeof(c),c.reservationOriginal,sizeof(*c.reservationOriginal))||
       !StorageSeparate(&c,sizeof(c),c.sourceOwnedSlotOriginal,sizeof(*c.sourceOwnedSlotOriginal))||
       (c.previousSlotOriginal&&!StorageSeparate(&c,sizeof(c),c.previousSlotOriginal,sizeof(*c.previousSlotOriginal)))||
       !SameSlot(c.value,*c.sourceOwnedSlotOriginal))return false;
    const auto& r=*c.reservationOriginal;const auto& v=c.reservationValue;
    if(r.reservationSequence!=v.reservationSequence||r.fileBytes!=v.fileBytes||
       r.maximumRecordBytes!=v.maximumRecordBytes||r.nativeAttempts!=v.nativeAttempts||
       r.pureAttempts!=v.pureAttempts||r.indexNodes!=v.indexNodes||
       r.failureTailBytes!=v.failureTailBytes||!SameInterval(r.records,v.records)||
       !SameInterval(r.indexes,v.indexes)||!SameInterval(r.terminal,v.terminal)||
       (c.previousSlotOriginal&&!SameSlot(*c.previousSlotOriginal,c.previousValue)))return false;
    ClaimGeometryOriginal reconstructed{};
    return ClaimGeometry(*c.reservationOriginal,c.value.sequence,*c.sourceOwnedSlotOriginal,
        c.previousSlotOriginal,reconstructed)&&SameSlot(c.value,reconstructed.value);
}
} // namespace template_creator_receipts

// UNCOMPILED native copy capture, supporting the selected copied-CODE producer.
// Actual producer/consumer/path/borrower authority is a separate gate relation.
#pragma once
#include "windows_captured_copy.hpp"
namespace template_creator {
class OriginalCopiedSourceCapture {
    NativeHeldFiles generated_;
    OriginalCapturedCopyComparison comparison_;
    const NativeHeldFiles* sourceOriginal_=nullptr;
    const wchar_t* generatedLiteralOriginal_=nullptr;
    std::array<wchar_t,32768> generatedLiteral_{};
    SIZE_T generatedLiteralUnits_=0;
    SIZE_T sourceOrdinal_=SIZE_MAX,generatedOrdinal_=SIZE_MAX;
    bool attempted_=false;
public:
    static SIZE_T CheckedSourceBytes(SIZE_T rows,SIZE_T bytes) {
        const SIZE_T nested=NativeHeldFiles::CheckedSourceBytes(1,rows,bytes);
        constexpr SIZE_T own=sizeof(OriginalCopiedSourceCapture)-sizeof(NativeHeldFiles);
        if(nested>SIZE_MAX-own)throw std::invalid_argument("copied source aggregate geometry");
        return own+nested;
    }
    static SIZE_T CheckedReadRows(SIZE_T rows,SIZE_T bytes) {
        (void)CheckedSourceBytes(rows,bytes);return rows;
    }
    OriginalCopiedSourceCapture(SIZE_T selectedReadRows,SIZE_T selectedBytes)
        :generated_(1,CheckedReadRows(selectedReadRows,selectedBytes),selectedBytes) {}
    OriginalCopiedSourceCapture(const OriginalCopiedSourceCapture&)=delete;
    OriginalCopiedSourceCapture& operator=(const OriginalCopiedSourceCapture&)=delete;
    bool Capture(const NativeHeldFiles& source,SIZE_T sourceOrdinal,const wchar_t* generatedLiteral) noexcept {
        if(attempted_)return false;attempted_=true;
        sourceOriginal_=&source;sourceOrdinal_=sourceOrdinal;generatedLiteralOriginal_=generatedLiteral;
        if(!generatedLiteral)return false;
        bool terminated=false;
        for(SIZE_T i=0;i<generatedLiteral_.size();++i) {
            generatedLiteral_[i]=generatedLiteral[i];
            if(!generatedLiteral[i]){generatedLiteralUnits_=i;terminated=true;break;}
        }
        if(!terminated || source.Failed())return false;
        const unsigned char* originalBytes=nullptr;SIZE_T originalExtent=0;
        if(!source.ExactCapturedSourceSpan(sourceOrdinal,originalBytes,originalExtent))return false;
        const auto& originalFile=source.PrivateFiles()[sourceOrdinal];
        // Native Capture copies the literal before opening it. Size/digest come
        // from THIS actual captured source, rather than a fresh JSON declaration.
        generatedOrdinal_=generated_.Capture(generatedLiteral_.data(),originalExtent,originalFile.actualSha256);
        const bool compared=comparison_.Observe(source,sourceOrdinal,generated_,generatedOrdinal_);
        return generatedOrdinal_==0 && !generated_.Failed() && compared &&
            comparison_.outcome==OriginalCopyOutcome::Equal;
    }
    const NativeHeldFiles& PrivateGenerated() const noexcept { return generated_; }
    const OriginalCapturedCopyComparison& PrivateComparison() const noexcept { return comparison_; }
    const NativeHeldFiles* PrivateSourceOriginal() const noexcept { return sourceOriginal_; }
    SIZE_T PrivateSourceOrdinal() const noexcept { return sourceOrdinal_; }
    bool DisposeIndependentlyEligible() noexcept { return generated_.Dispose(); }
    bool PhysicallyDisposed() const noexcept { return generated_.PhysicallyDisposed(); }
    SIZE_T ReservedSourceBytes() const noexcept {
        return sizeof(*this)-sizeof(NativeHeldFiles)+generated_.ReservedSourceBytes();
    }
};
// This capture owns no process or borrower by itself. The connected product gate
// must establish actual eligibility before disposal; equality grants no launch.
// Source and generated owners/backing remain resident through borrower closure.
}

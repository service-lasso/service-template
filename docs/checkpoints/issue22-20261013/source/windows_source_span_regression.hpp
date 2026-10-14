// UNCOMPILED/UNRUN actual native source-span regression scenarios.
// An admitted Windows harness must preown and retain this object, then supply
// four private regular single-link files containing EXACTLY abc, xyz, empty,
// and a separately produced abc copy. This fixture preparation is still UNRUN.
// This source does not create files, invoke itself or fabricate native outcomes.
#pragma once
#include "windows_copied_source_capture.hpp"
namespace template_creator {
struct OriginalSourceSpanRegression {
    NativeHeldFiles accepted{3,9,9};
    NativeHeldFiles changedSize{1,3,5};
    OriginalCopiedSourceCapture copied{4,4};
    OriginalCopiedSourceCapture nextConsumerCopy{4,4};
    bool attempted=false, captureChecks=false, changedSizeDenied=false;
    bool acceptedDisposalAttempted=false, changedDisposalAttempted=false;
    bool acceptedDisposed=false, changedDisposed=false;
    bool copiedDisposalAttempted=false,copiedDisposed=false;
    bool nextCopyAttempted=false,nextCopyCaptured=false,nextCopyDisposalAttempted=false,nextCopyDisposed=false;
    bool sequentialCopyChecks=false;
    std::array<OriginalCapturedCopyComparison,6> comparisons{};
    bool comparisonChecks=false;
    static std::array<unsigned char,32> Digest(const char* hex) noexcept {
        std::array<unsigned char,32> value{};
        for(SIZE_T i=0;i<value.size();++i) {
            const auto digit=[](char c) noexcept -> unsigned char {
                return static_cast<unsigned char>(c>='a'?c-'a'+10:c-'0');
            };
            value[i]=static_cast<unsigned char>((digit(hex[i*2])<<4)|digit(hex[i*2+1]));
        }
        return value;
    }
    bool Observe(const wchar_t* abcPath,const wchar_t* xyzPath,const wchar_t* emptyPath,
                 const wchar_t* copiedAbcPath) noexcept {
        if(attempted)return false;attempted=true;
        const auto abc=Digest("ba7816bf8f01cfea414140de5dae2223b00361a396177a9cb410ff61f20015ad");
        const auto xyz=Digest("3608bca1e44ea6c4d268eb6db02260269892c0b42b86bbf1e77a6fa16c3c9282");
        const auto empty=Digest("e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855");
        const SIZE_T first=accepted.Capture(abcPath,3,abc);
        const SIZE_T second=accepted.Capture(xyzPath,3,xyz);
        const SIZE_T third=accepted.Capture(emptyPath,0,empty);
        const unsigned char *a=nullptr,*b=nullptr,*e=nullptr;
        SIZE_T an=0,bn=0,en=0;
        bool clean=first==0 && second==1 && third==2 && !accepted.Failed() &&
            accepted.ExactCapturedSourceSpan(0,a,an) &&
            accepted.ExactCapturedSourceSpan(1,b,bn) &&
            accepted.ExactCapturedSourceSpan(2,e,en) &&
            an==3 && bn==3 && en==0 && memcmp(a,"abc",3)==0 && memcmp(b,"xyz",3)==0 &&
            a==accepted.PrivateReadBytes() && b==a+3 && e==b+3 &&
            accepted.RetainedReadByteCount()==6;
        SIZE_T nextRead=0;
        for(SIZE_T i=0;i<accepted.FileCount();++i) {
            const auto& file=accepted.PrivateFiles()[i];
            clean=clean && file.readBegin==nextRead && file.readEnd>file.readBegin &&
                file.readEnd<=accepted.ReadCount() && file.bodyExtent==(i==2?0:3);
            for(SIZE_T j=file.readBegin;j<file.readEnd;++j) {
                const auto& read=accepted.PrivateReads()[j];
                clean=clean && read.fileOrdinal==i && read.result && !read.originalError;
                if(read.growthProbe)clean=clean && j+1==file.readEnd && read.returned==0;
            }
            nextRead=file.readEnd;
        }
        captureChecks=clean && nextRead==accepted.ReadCount() && accepted.FileCount()==3;
        // A real original size mismatch must deny access to a clean source span;
        // the hash cannot substitute for the actual file-size observation.
        const SIZE_T bad=changedSize.Capture(abcPath,4,abc);
        const unsigned char* rejected=nullptr;SIZE_T rejectedSize=99;
        changedSizeDenied=bad==0 && changedSize.Failed() && changedSize.FileCount()==1 &&
            changedSize.PrivateFiles()[0].sizeResult &&
            changedSize.PrivateFiles()[0].actualSize.QuadPart==3 &&
            !changedSize.ExactCapturedSourceSpan(0,rejected,rejectedSize) &&
            !rejected && rejectedSize==0 && changedSize.ReadCount()==0;
        const bool sameObserved=comparisons[0].Observe(accepted,0,accepted,0);
        const bool adjacentObserved=comparisons[1].Observe(accepted,0,accepted,1);
        const bool emptyObserved=comparisons[2].Observe(accepted,2,accepted,2);
        const bool sizeObserved=comparisons[3].Observe(accepted,2,accepted,0);
        const bool failedObserved=comparisons[4].Observe(changedSize,0,accepted,0);
        const bool copyCaptured=copied.Capture(accepted,0,copiedAbcPath);
        const auto& copyOwner=copied.PrivateGenerated();
        const bool copyObserved=comparisons[5].Observe(accepted,0,copyOwner,0);
        comparisonChecks=sameObserved &&
            comparisons[0].outcome==OriginalCopyOutcome::Equal &&
            adjacentObserved &&
            comparisons[1].outcome==OriginalCopyOutcome::Different &&
            emptyObserved &&
            comparisons[2].outcome==OriginalCopyOutcome::Equal &&
            sizeObserved &&
            comparisons[3].outcome==OriginalCopyOutcome::Different &&
            !failedObserved &&
            comparisons[4].outcome==OriginalCopyOutcome::Unavailable &&
            comparisons[4].sourceOwner==&changedSize && comparisons[4].generatedOwner==&accepted &&
            copyCaptured && !copyOwner.Failed() && copyObserved &&
            comparisons[5].outcome==OriginalCopyOutcome::Equal &&
            comparisons[5].sourceOwner==&accepted && comparisons[5].generatedOwner==&copyOwner &&
            comparisons[5].sourceBytes==a && comparisons[5].generatedBytes==copyOwner.PrivateReadBytes() &&
            copied.PrivateSourceOriginal()==&accepted && copied.PrivateSourceOrdinal()==0 &&
            copied.PrivateComparison().outcome==OriginalCopyOutcome::Equal;
        // Independent once-close results remain resident even if a check failed.
        copiedDisposalAttempted=true;copiedDisposed=copied.DisposeIndependentlyEligible();
        // Distinct sequential consumers may acquire distinct originals. Do not
        // manufacture a second attempt after uncertain first-owner disposal.
        if(copyCaptured && copiedDisposed && copied.PhysicallyDisposed()) {
            nextCopyAttempted=true;
            nextCopyCaptured=nextConsumerCopy.Capture(accepted,0,copiedAbcPath);
            const auto& nextOwner=nextConsumerCopy.PrivateGenerated();
            sequentialCopyChecks=nextCopyCaptured && &nextOwner!=&copyOwner &&
                nextConsumerCopy.PrivateSourceOriginal()==&accepted &&
                nextConsumerCopy.PrivateSourceOrdinal()==0 &&
                nextConsumerCopy.PrivateComparison().outcome==OriginalCopyOutcome::Equal &&
                nextConsumerCopy.PrivateComparison().sourceBytes==a &&
                nextConsumerCopy.PrivateComparison().generatedOwner==&nextOwner &&
                nextConsumerCopy.PrivateComparison().generatedBytes==nextOwner.PrivateReadBytes();
        }
        // Even an unused preowned owner has an actual independent disposition
        // outcome; no returned comparison grants process-borrower eligibility.
        nextCopyDisposalAttempted=true;
        nextCopyDisposed=nextConsumerCopy.DisposeIndependentlyEligible();
        acceptedDisposalAttempted=true;acceptedDisposed=accepted.Dispose();
        changedDisposalAttempted=true;changedDisposed=changedSize.Dispose();
        return captureChecks && changedSizeDenied && comparisonChecks && sequentialCopyChecks &&
            acceptedDisposed && changedDisposed && copiedDisposed && nextCopyDisposed;
    }
};
// The harness retains all four owners and raw read/close observations through actual
// closure. No local stack return/destructor supplies disposal or native proof.
}

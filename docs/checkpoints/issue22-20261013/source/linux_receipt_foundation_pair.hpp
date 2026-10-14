// Native pair correspondence only. Authentic foundation acquisition is separate.
#pragma once
#ifndef _WIN32
#include "linux_upstream_subject_receiver.hpp"
namespace template_creator_linux {
class OriginalReceiptFoundationPair {
public:
    struct NativeOriginal {
        const IngressFd* referenceOriginal=nullptr;
        int descriptor=-1;
        struct stat status{};
        int result=-1,error=0;
        bool captureStarted=false,attempted=false,returned=false;
        IngressNativeClock clock{};
    };
private:
    const OriginalInitialProviderReceiver* receiver_=nullptr;
    const slcli_bq_provider_expectations* source_=nullptr;
    const OriginalDeadline* deadline_=nullptr;
    OriginalDeadlineSupervisor* supervisor_=nullptr;
    const OriginalInitialProviderReceiver::OfferOriginal *writerOffer_=nullptr,*readbackOffer_=nullptr;
    const IngressFd *writer_=nullptr,*readback_=nullptr;
    std::array<NativeOriginal,6> rows_{};
    IngressNativeClock finalClock_{};
    bool attempted_=false,captureReturned_=false,observed_=false,failed_=false,dispositionRequested_=false;
    // The synchronous startup owner calls capture and disposition on its creating
    // thread. This publication covers validation, clocks and the continuation
    // between native rows as well as the rows themselves; it is not a worker join.
    struct CaptureReturnOriginal {
        OriginalReceiptFoundationPair& owner;
        ~CaptureReturnOriginal() noexcept {owner.captureReturned_=true;}
    };
    bool Observe(size_t ordinal,const IngressFd& reference,bool flags) noexcept {
        auto& row=rows_[ordinal];if(row.captureStarted)return false;row.captureStarted=true;
        row.referenceOriginal=&reference;row.descriptor=reference.original;
        if(!row.clock.Before(deadline_,"foundation-pair-observation",false)||!supervisor_->BeginNativeCall())return false;
        row.attempted=true;
        row.result=flags?fcntl(row.descriptor,F_GETFL):fstat(row.descriptor,&row.status);
        if(row.result<0)row.error=errno;row.returned=true;
        supervisor_->OriginalNativeCallReturned();row.clock.After(deadline_,"foundation-pair-observation");
        if(!row.clock.clean||row.result<0)return false;
        if(ordinal&&row.clock.before.nanoseconds<rows_[ordinal-1].clock.after.nanoseconds)return false;
        if(!ordinal&&(row.clock.before.nanoseconds<writer_->statClock.after.nanoseconds||
            row.clock.before.nanoseconds<readback_->statClock.after.nanoseconds))return false;
        return true;
    }
    static bool Same(const struct stat& a,const struct stat& b) noexcept {
        return a.st_dev==b.st_dev&&a.st_ino==b.st_ino&&a.st_mode==b.st_mode&&
            a.st_uid==b.st_uid&&a.st_gid==b.st_gid&&a.st_size==b.st_size;
    }
public:
    OriginalReceiptFoundationPair(const OriginalInitialProviderReceiver& receiver,
        const slcli_bq_provider_expectations& source,const OriginalDeadline& deadline,
        OriginalDeadlineSupervisor& supervisor) noexcept:
        receiver_(&receiver),source_(&source),deadline_(&deadline),supervisor_(&supervisor) {}
    OriginalReceiptFoundationPair(const OriginalReceiptFoundationPair&)=delete;
    OriginalReceiptFoundationPair& operator=(const OriginalReceiptFoundationPair&)=delete;
    bool CaptureOriginal(uint32_t phase) noexcept {
        if(attempted_||dispositionRequested_)return false;attempted_=true;failed_=true;
        CaptureReturnOriginal captureReturn{*this};
        if(!receiver_||!source_||!deadline_||!supervisor_||supervisor_->OriginalDeadlineOwner()!=deadline_||
           !slcli_bq_provider_expectations_bootstrap_shape(source_,phase)||receiver_->OriginalOfferCount()!=3)return false;
        writerOffer_=receiver_->OriginalOffer(1);readbackOffer_=receiver_->OriginalOffer(2);
        if(!writerOffer_||!readbackOffer_||writerOffer_==readbackOffer_||
           !writerOffer_->rawOfferObserved||!readbackOffer_->rawOfferObserved||
           writerOffer_->rightsCount!=1||readbackOffer_->rightsCount!=1)return false;
        writer_=&writerOffer_->rights[0];readback_=&readbackOffer_->rights[0];
        if(writer_->original<0||readback_->original<0||writer_->original==readback_->original||
           !writer_->statReturned||writer_->statResult||!readback_->statReturned||readback_->statResult||
           writer_->closeAttempted||readback_->closeAttempted)return false;
        // Each result cell remains in this surviving owner before/through native effects.
        if(!Observe(0,*writer_,false)||!Observe(1,*readback_,false)||
           !Observe(2,*writer_,true)||!Observe(3,*readback_,true)||
           !Observe(4,*writer_,false)||!Observe(5,*readback_,false))return false;
        if(!S_ISREG(rows_[0].status.st_mode)||!Same(rows_[0].status,writer_->status)||
           !Same(rows_[1].status,readback_->status)||!Same(rows_[0].status,rows_[1].status)||
           !Same(rows_[0].status,rows_[4].status)||!Same(rows_[1].status,rows_[5].status)||
           rows_[0].status.st_size<0||static_cast<uint64_t>(rows_[0].status.st_size)!=source_->subjects[0].source_occurrence.native_file_bytes||
           (rows_[2].result&O_ACCMODE)!=O_RDWR||(rows_[3].result&O_ACCMODE)!=O_RDONLY||
           (rows_[2].result&O_PATH)||(rows_[3].result&O_PATH))return false;
        if(!finalClock_.Before(deadline_,"foundation-pair-final",false)||
           finalClock_.before.nanoseconds<rows_[5].clock.after.nanoseconds)return false;
        finalClock_.After(deadline_,"foundation-pair-final");if(!finalClock_.clean)return false;
        observed_=true;failed_=false;return true;
        // Native identity/rights correspondence is NOT independent entry100/source/finance proof.
    }
    bool NativeCallsReturnedOriginal() const noexcept {
        for(const auto& row:rows_)if(row.attempted&&!row.returned)return false;
        return true;
    }
    bool ObserveIndependentDispositionOriginal() noexcept {
        dispositionRequested_=true;
        return (!attempted_||captureReturned_)&&NativeCallsReturnedOriginal();
        // No reference is closed here. Receiver still owns both actual rights.
        // Once requested, capture cannot start; an attempted capture must have
        // returned through its actual synchronous issuing continuation first.
    }
    bool SourceDispositionEstablishedOriginal() const noexcept {
        return dispositionRequested_&&(!attempted_||captureReturned_)&&NativeCallsReturnedOriginal();
    }
    bool NativeCorrespondenceObserved() const noexcept{return observed_&&!failed_;}
    bool SameOriginals(const OriginalInitialProviderReceiver& receiver,
        const slcli_bq_provider_expectations& source,const OriginalDeadline& deadline,
        const OriginalDeadlineSupervisor& supervisor) const noexcept {
        return receiver_==&receiver&&source_==&source&&deadline_==&deadline&&supervisor_==&supervisor;
    }
    const slcli_bq_provider_expectations* SourceOriginal() const noexcept{return source_;}
    const OriginalInitialProviderReceiver* ReceiverOriginal() const noexcept{return receiver_;}
    const OriginalClockBoundary& FinalCorrespondenceClockOriginal() const noexcept{return finalClock_.after;}
    const NativeOriginal* FinalNativeIdentityOriginal(size_t member) const noexcept {
        return NativeCorrespondenceObserved()&&member<2?&rows_[4+member]:nullptr;
    }
    bool AcquiredFoundationOriginalsQualified() const noexcept {
        return false; // Genuine independent entry/reserve/source/alias/storage consumer remains unfinished.
    }
    size_t ReservedSourceBytes() const noexcept{return sizeof(*this);}
};
}
#endif

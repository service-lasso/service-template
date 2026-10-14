// UNCOMPILED retained Linux sink. Descriptor custody stays with the ingress
// owner. Physical prefix readback is independent of failed qualification.
#pragma once
#include <sys/stat.h>
#include "linux_held_file.hpp"
#include "linux_receipt_foundation_pair.hpp"
#include <unistd.h>
#include <fcntl.h>
#include <cerrno>
#include <cstring>
#include <memory>
#include <stdexcept>
#include <cstdint>
#include "linux_original_deadline.hpp"
#include "linux_deadline_supervisor.hpp"
namespace template_creator_bridge {class OriginalLinuxFinalOutput;}
namespace template_creator_linux {
enum class SinkCall { Stat, Flags, Seek, Write, Sync, Read, Eof };
struct SinkOriginalCall {
    SinkCall operation{};
    bool attempted = false, returnedObserved = false;
    size_t destinationOffset = 0, requested = 0;
    ssize_t returned = -1;
    int error = 0;
    unsigned char eofProbeByte = 0;
    struct stat metadata{};
};
class OriginalNativeSink {
    const OriginalLinuxHeldFile* inputOverlapOriginal_=nullptr;
    const OriginalReceiptFoundationPair* nativePairOverlapOriginal_=nullptr;
    size_t inputExclusionCount_=0;
    bool inputExclusionFailed_=false;
    friend class template_creator_bridge::OriginalLinuxFinalOutput;
    OriginalDeadline& deadline_;
    OriginalDeadlineSupervisor& supervisor_;
    int borrowed_ = -1;
    std::unique_ptr<unsigned char[]> copied_, readbacks_;
    std::unique_ptr<SinkOriginalCall[]> calls_;
    size_t bytesCapacity_ = 0, readbacksCapacity_=0,callsCapacity_ = 0, callCount_ = 0;
    size_t selectedBytes_=0,selectedCalls_=0;
    bool preownAttempted_=false,preowned_=false;
    std::exception_ptr preownException_;
    size_t copiedExtent_ = 0, writtenExtent_ = 0, readbackExtent_ = 0;
    bool adoptAttempted_ = false, adopted_ = false, failed_ = false;
    bool extentUnknown_ = false, prefixVerified_ = false, copiesSealed_ = false;
    const bool finalCaptureOnly_;
    bool finalWriteBlocked_=false;
    bool finalCaptureAdmitted_=false;
    SinkOriginalCall* Reserve(SinkCall operation) noexcept {
        if (callCount_ == callsCapacity_) { failed_ = true; return nullptr; }
        auto& row = calls_[callCount_++]; row.operation = operation; row.attempted = true; return &row;
    }
    bool BeginOriginalCall(SinkOriginalCall& row,bool independentlyEligible) noexcept {
        row.attempted=false;
        const bool admitted=independentlyEligible?supervisor_.BeginIndependentlyEligibleCleanupNativeCall():supervisor_.BeginNativeCall();
        if(!admitted){failed_=true;return false;}
        row.attempted=true;return true;
    }
    bool Seek(off_t offset, bool independentlyEligibleCapture) noexcept {
        auto* row = Reserve(SinkCall::Seek); if (!row) return false;
        row->destinationOffset = static_cast<size_t>(offset);
        if (!deadline_.Observe("sink-seek",false)) {
            failed_ = true;
            if (!independentlyEligibleCapture) { row->attempted = false; return false; }
        }
        if(!BeginOriginalCall(*row,independentlyEligibleCapture))return false;
        row->returned = lseek(borrowed_,offset,SEEK_SET);
        if (row->returned < 0) row->error = errno;
        row->returnedObserved = true;
        supervisor_.OriginalNativeCallReturned();
        if (!deadline_.Observe("sink-seek",true)) failed_ = true;
        if (row->returned != offset) { failed_ = true; return false; } return true;
    }
public:
    OriginalNativeSink(DeferredOriginalPreownTag,size_t selectedBytes,size_t selectedCalls,OriginalDeadline& originalDeadline,
            OriginalDeadlineSupervisor& originalSupervisor,bool finalCaptureOnly=false) noexcept
        :deadline_(originalDeadline),supervisor_(originalSupervisor),selectedBytes_(selectedBytes),selectedCalls_(selectedCalls),finalCaptureOnly_(finalCaptureOnly){}
    OriginalNativeSink(size_t selectedBytes,size_t selectedCalls,OriginalDeadline& originalDeadline,
            OriginalDeadlineSupervisor& originalSupervisor,bool finalCaptureOnly=false)
        :OriginalNativeSink(DeferredOriginalPreown,selectedBytes,selectedCalls,originalDeadline,originalSupervisor,finalCaptureOnly){
        if(!PreownOriginal()&&preownException_)std::rethrow_exception(preownException_);
    }
    bool PreownOriginal() noexcept {
        if(preownAttempted_)return false;preownAttempted_=true;
        try{
        if (supervisor_.OriginalDeadlineOwner()!=&deadline_ || !selectedBytes_ || selectedBytes_ > static_cast<uint64_t>(INT64_MAX) ||
            !selectedCalls_ || selectedCalls_ > SIZE_MAX/sizeof(SinkOriginalCall))
            throw std::invalid_argument("selected native sink capacity");
        size_t sourceBytes=sizeof(*this);
        const auto add=[&](size_t n) {
            if(n>SIZE_MAX-sourceBytes)return false;sourceBytes+=n;return true;
        };
        if(!add(selectedBytes_)||!add(selectedBytes_)||!add(selectedCalls_*sizeof(SinkOriginalCall)))
            throw std::invalid_argument("aggregate native sink backing");
        copied_=std::make_unique<unsigned char[]>(selectedBytes_);bytesCapacity_=selectedBytes_;
        readbacks_=std::make_unique<unsigned char[]>(selectedBytes_);readbacksCapacity_=selectedBytes_;
        calls_=std::make_unique<SinkOriginalCall[]>(selectedCalls_);callsCapacity_=selectedCalls_;
        preowned_=true;return true;
        }catch(...){preownException_=std::current_exception();failed_=true;return false;}
    }
    bool PreownedOriginal() const noexcept{return preowned_;}
    const std::exception_ptr& PreownExceptionOriginal() const noexcept{return preownException_;}
    size_t AllocatedCopiedCapacityOriginal() const noexcept{return bytesCapacity_;}
    size_t AllocatedReadbackCapacityOriginal() const noexcept{return readbacksCapacity_;}
    size_t AllocatedCallCapacityOriginal() const noexcept{return callsCapacity_;}
    OriginalNativeSink(const OriginalNativeSink&) = delete;
    OriginalNativeSink& operator=(const OriginalNativeSink&) = delete;
    bool AdoptOriginal(int actualIngressDescriptor) noexcept {
        if (!preowned_ || adoptAttempted_) return false; adoptAttempted_ = true; borrowed_ = actualIngressDescriptor;
        if (!deadline_.Observe("sink-fstat",false)) { failed_ = true; return false; }
        auto* metadata = Reserve(SinkCall::Stat); if (!metadata) return false;
        if(!BeginOriginalCall(*metadata,false))return false;
        metadata->returned = fstat(borrowed_,&metadata->metadata);
        if (metadata->returned < 0) metadata->error = errno;
        metadata->returnedObserved = true;
        supervisor_.OriginalNativeCallReturned();
        if (!deadline_.Observe("sink-fstat",true)) { failed_ = true; return false; }
        if (!deadline_.Observe("sink-flags",false)) { failed_ = true; return false; }
        auto* flags = Reserve(SinkCall::Flags); if (!flags) return false;
        if(!BeginOriginalCall(*flags,false))return false;
        flags->returned = fcntl(borrowed_,F_GETFL);
        if (flags->returned < 0) flags->error = errno;
        flags->returnedObserved = true;
        supervisor_.OriginalNativeCallReturned();
        if (!deadline_.Observe("sink-flags",true)) { failed_ = true; return false; }
        if (metadata->returned != 0 || !S_ISREG(metadata->metadata.st_mode) || metadata->metadata.st_size != 0 ||
            flags->returned < 0 || (flags->returned&O_ACCMODE) != O_RDWR || (flags->returned&O_APPEND)) {
            failed_ = true; return false;
        }
        adopted_ = Seek(0,false) && deadline_.QualificationClean(); return adopted_;
    }
    bool Append(const unsigned char* originalBytes, size_t extent) noexcept {
        if (copiesSealed_ || !adopted_ || (finalCaptureOnly_&& !finalCaptureAdmitted_) ||
            (finalCaptureOnly_?finalWriteBlocked_:failed_) || extentUnknown_ ||
            (!finalCaptureOnly_&&!deadline_.QualificationClean()) || (!originalBytes && extent) || extent > bytesCapacity_-copiedExtent_) {
            finalWriteBlocked_=failed_ = true; return false;
        }
        const size_t start = copiedExtent_;
        if (extent) memcpy(copied_.get()+start,originalBytes,extent);
        copiedExtent_ += extent; prefixVerified_ = false;
        size_t submitted = 0;
        while (submitted < extent) {
            auto* row = Reserve(SinkCall::Write); if (!row) {finalWriteBlocked_=true;return false;}
            row->destinationOffset = start+submitted;
            row->requested = extent-submitted > 65536 ? 65536 : extent-submitted;
            if (!deadline_.Observe("sink-write",false)) {
                failed_ = true;
                if(!finalCaptureOnly_){row->attempted=false;return false;}
            }
            if(!BeginOriginalCall(*row,finalCaptureOnly_)){finalWriteBlocked_=true;return false;}
            row->returned = write(borrowed_,copied_.get()+row->destinationOffset,row->requested);
            if (row->returned < 0) row->error = errno;
            row->returnedObserved = true;
            const bool extentShape=row->returned>=0&&static_cast<size_t>(row->returned)<=row->requested;
            if(extentShape){writtenExtent_+=static_cast<size_t>(row->returned);submitted+=static_cast<size_t>(row->returned);}
            supervisor_.OriginalNativeCallReturned();
            const bool clockClean = deadline_.Observe("sink-write",true);
            if (row->returned < 0) { finalWriteBlocked_=failed_ = true; return false; }
            if (static_cast<size_t>(row->returned) > row->requested) { finalWriteBlocked_=extentUnknown_ = failed_ = true; return false; }
            if (!clockClean) { failed_ = true;if(!finalCaptureOnly_)return false; }
            if (!row->returned) { finalWriteBlocked_=failed_ = true; return false; }
        }
        return true;
    }
    bool FlushReadbackOriginal() noexcept {
        if (!adopted_ || extentUnknown_) return false;
        prefixVerified_ = false;
        auto* sync = Reserve(SinkCall::Sync); if (!sync) return false;
        // Failure-prefix capture is independently eligible after completed
        // synchronous IO. Expiry is retained failure, not a reason to hide it.
        if (!deadline_.Observe("sink-fsync",false)) failed_ = true;
        if(!BeginOriginalCall(*sync,true))return false;
        sync->returned = fsync(borrowed_); if (sync->returned < 0) sync->error = errno;
        sync->returnedObserved = true;
        supervisor_.OriginalNativeCallReturned();
        if (!deadline_.Observe("sink-fsync",true)) failed_ = true;
        if (sync->returned != 0) { failed_ = true; return false; }
        if (!Seek(0,true)) return false;
        // Each readback occupies a distinct retained extent; earlier raw bytes
        // survive subsequent append/readback cycles and failures.
        const size_t cycleStart = readbackExtent_;
        if (writtenExtent_ > bytesCapacity_-readbackExtent_) { failed_ = true; return false; }
        size_t compared = 0;
        bool matched = true;
        while (compared < writtenExtent_) {
            auto* row = Reserve(SinkCall::Read); if (!row) return false;
            row->destinationOffset = readbackExtent_;
            row->requested = writtenExtent_-compared > 65536 ? 65536 : writtenExtent_-compared;
            if (!deadline_.Observe("sink-readback",false)) failed_ = true;
            if(!BeginOriginalCall(*row,true))return false;
            row->returned = read(borrowed_,readbacks_.get()+readbackExtent_,row->requested);
            if (row->returned < 0) row->error = errno;
            row->returnedObserved = true;
            const bool extentShape=row->returned>=0&&static_cast<size_t>(row->returned)<=row->requested;
            if(extentShape)readbackExtent_+=static_cast<size_t>(row->returned);
            supervisor_.OriginalNativeCallReturned();
            if (!deadline_.Observe("sink-readback",true)) failed_ = true;
            if (row->returned < 0) { failed_ = true; return false; }
            if (static_cast<size_t>(row->returned) > row->requested) { extentUnknown_ = failed_ = true; return false; }

            if (!row->returned) { failed_ = true; return false; }
            if (memcmp(copied_.get()+compared,readbacks_.get()+row->destinationOffset,static_cast<size_t>(row->returned))) matched = false;
            compared += static_cast<size_t>(row->returned);
        }
        auto* eof = Reserve(SinkCall::Eof); if (!eof) return false;
        eof->requested = 1; eof->destinationOffset = readbackExtent_;
        // Preserve even unexpected EOF-probe bytes in this attempt's row storage.
        if (!deadline_.Observe("sink-eof-probe",false)) failed_ = true;
        if(!BeginOriginalCall(*eof,true))return false;
        eof->returned = read(borrowed_,&eof->eofProbeByte,1); if (eof->returned < 0) eof->error = errno;
        eof->returnedObserved = true;
        supervisor_.OriginalNativeCallReturned();
        if (!deadline_.Observe("sink-eof-probe",true)) failed_ = true;
        if (eof->returned != 0) { failed_ = true; if (eof->returned > 1) extentUnknown_ = true; return false; }
        if (!Seek(static_cast<off_t>(writtenExtent_),true)) return false;
        prefixVerified_ = matched && compared == writtenExtent_ && readbackExtent_-cycleStart == writtenExtent_;
        if (!prefixVerified_) failed_ = true;
        return prefixVerified_;
    }
    bool Qualified() const noexcept { return adopted_ && !failed_ && !extentUnknown_ && deadline_.QualificationClean() && prefixVerified_ && copiedExtent_ == writtenExtent_; }
    bool PrefixReadbackVerified() const noexcept { return prefixVerified_; }
    bool SealOriginalCopies() noexcept {
        if(!adopted_||extentUnknown_||!AllAttemptedOriginalCallsReturned())return false;
        copiesSealed_=true;return true;
        // SAME native creator thread after final append. This source API seal
        // is not external native writer exclusion or worker quiescence proof.
    }
    int BorrowedOriginal() const noexcept{return borrowed_;}
    bool ExcludeCapturedInputOriginal(const OriginalLinuxHeldFile& input) noexcept {
        if(inputExclusionFailed_||inputExclusionCount_==SIZE_MAX||!adopted_||copiedExtent_||writtenExtent_||!input.CapturedNativeIdentity()){
            inputExclusionFailed_=finalWriteBlocked_=failed_=true;inputOverlapOriginal_=&input;return false;
        }
        const SinkOriginalCall* metadata=nullptr;
        for(size_t i=0;i<callCount_;++i)if(calls_[i].operation==SinkCall::Stat){
            if(metadata){inputExclusionFailed_=finalWriteBlocked_=failed_=true;inputOverlapOriginal_=&input;return false;}
            metadata=&calls_[i];
        }
        const auto& after=input.PrivateAfter();
        if(!metadata||!metadata->attempted||!metadata->returnedObserved||metadata->returned!=0||
           !after.returnedObserved||after.returned!=0||
           (metadata->metadata.st_dev==after.metadata.st_dev&&metadata->metadata.st_ino==after.metadata.st_ino)){
            inputExclusionFailed_=finalWriteBlocked_=failed_=true;inputOverlapOriginal_=&input;return false;
        }
        ++inputExclusionCount_;return true;
    }
    const OriginalDeadline& OriginalDeadlineOwner() const noexcept{return deadline_;}
    bool ExcludeNativeReceiptPairOriginal(const OriginalReceiptFoundationPair& pair) noexcept {
        const auto refuse=[&]() noexcept {
            nativePairOverlapOriginal_=&pair;inputExclusionFailed_=finalWriteBlocked_=failed_=true;return false;
        };
        const auto* receiver=pair.ReceiverOriginal();const auto* source=pair.SourceOriginal();
        if(inputExclusionFailed_||inputExclusionCount_==SIZE_MAX||!adopted_||copiedExtent_||writtenExtent_||
           !receiver||!source||!pair.SameOriginals(*receiver,*source,deadline_,supervisor_))return refuse();
        const SinkOriginalCall* metadata=nullptr;
        for(size_t i=0;i<callCount_;++i)if(calls_[i].operation==SinkCall::Stat){
            if(metadata)return refuse();metadata=&calls_[i];
        }
        if(!metadata||!metadata->attempted||!metadata->returnedObserved||metadata->returned!=0)return refuse();
        for(size_t member=0;member<2;++member){const auto* original=pair.FinalNativeIdentityOriginal(member);
            if(!original||!original->attempted||!original->returned||original->result!=0||
               !original->referenceOriginal||original->referenceOriginal->closeAttempted||
               (metadata->metadata.st_dev==original->status.st_dev&&metadata->metadata.st_ino==original->status.st_ino))return refuse();
        }
        ++inputExclusionCount_;return true; // Sampled own-output alias exclusion, not writer/currentness authority.
    }
    OriginalDeadlineSupervisor& OriginalSupervisorOwner() noexcept{return supervisor_;}
    bool AllAttemptedOriginalCallsReturned() const noexcept {
        for(size_t i=0;i<callCount_;++i)
            if(calls_[i].attempted&&!calls_[i].returnedObserved)return false;
        return true;
        // Same native owner after actual call return only. This source roster
        // predicate does not establish another worker's quiescence or join.
    }
    const SinkOriginalCall* PrivateOriginalCalls() const noexcept { return calls_.get(); }
    size_t PrivateOriginalCallCount() const noexcept { return callCount_; }
    const unsigned char* PrivateOriginalCopiedBytes() const noexcept { return copied_.get(); }
    const unsigned char* PrivateOriginalReadbackBytes() const noexcept { return readbacks_.get(); }
    size_t OriginalCopiedExtent() const noexcept { return copiedExtent_; }
    size_t OriginalWrittenExtent() const noexcept { return writtenExtent_; }
    size_t OriginalReadbackExtent() const noexcept { return readbackExtent_; }
    bool ReservedSourceBytes(size_t& bytes) const noexcept {
        bytes = sizeof(*this);
        const auto add = [&](size_t amount) noexcept { if (amount > SIZE_MAX-bytes) return false; bytes += amount; return true; };
        return add(bytesCapacity_) && add(readbacksCapacity_) && add(callsCapacity_*sizeof(SinkOriginalCall));
    }
};
}

// UNCOMPILED dedicated native lease custodian for source-created candidate
// files only. No sigaction, process-wide disposition, ambient tool lease,
// privilege change, implicit expiry renewal, detach, cancellation or kill.
#pragma once
#include "linux_private_candidate_file.hpp"
#include "linux_command_route.hpp"
#include <pthread.h>
#include <sys/signalfd.h>
#include <sys/eventfd.h>
#include <sys/syscall.h>
#include <poll.h>
#include <signal.h>
#include <atomic>
namespace template_creator_linux {
enum class LeaseOperation {
    ThreadSelf, ThreadTid, EmptyOwnSignalMask, AddOwnSignalMask, BlockOwnSignal, SignalFd,
    OriginalRecipient, OriginalSignal, SetRecipient, GetRecipient, SetSignal,
    GetSignal, Acquire, GetLease, FileStat, FileRead, Poll, SignalRead, RequestRead
};
struct LeaseCallOriginal {
    LeaseOperation operation=LeaseOperation::ThreadTid;
    size_t fileOrdinal=SIZE_MAX, offset=0, requested=0;
    int descriptor=-1, error=0; long returned=-1;
    bool attempted=false, beforeClockClean=false, afterClockClean=false;
    struct f_owner_ex recipient{};
    struct stat metadata{};
    std::array<struct pollfd,2> pollDescriptors{};
    struct signalfd_siginfo signal{};
    uint64_t requestBody=0;
    pthread_t originalThreadSelf{};
    sigset_t originalMask{}, requestedMask{};
};
struct LeaseFileOriginal {
    OriginalPrivateCandidateFile* source=nullptr;
    std::unique_ptr<unsigned char[]> readBytes;
    size_t readCapacity=0, retainedBytes=0;
    bool acquired=false, contentVerified=false, eof=false;
    bool unlockAttempted=false, unlockBeforeClock=false, unlockAfterClock=false;
    int unlockReturned=-1, unlockError=0;
};
struct LeaseRequestOriginal {
    bool attempted=false, stop=false, beforeClock=false, afterClock=false;
    uint64_t epoch=0, writtenBody=1;
    ssize_t returned=-1; int error=0;
};
struct LeaseJoinOriginal { bool attempted=false,beforeClock=false,afterClock=false; int returned=0; void* originalThreadReturn=nullptr; };
struct LeaseMainCallOriginal {
    bool attempted=false,beforeClock=false,afterClock=false,errorFromReturn=false;
    int returned=-1,error=0;
};
class OriginalLeaseOwner {
    OriginalCommandRoute& route_;
    OriginalDeadline& mainClock_;
    OriginalDeadline workerClock_;
    std::unique_ptr<LeaseFileOriginal[]> files_;
    std::unique_ptr<LeaseCallOriginal[]> calls_;
    std::unique_ptr<LeaseRequestOriginal[]> requests_;
    std::unique_ptr<LeaseJoinOriginal[]> joins_;
    LeaseRequestOriginal originalStopRequest_{};
    LeaseMainCallOriginal initializeAttributes_,configureStack_,queryStack_,queryGuard_;
    LeaseMainCallOriginal createWake_,createThread_,destroyAttributes_;
    size_t fileCount_=0, callCapacity_=0, callCount_=0, requestCapacity_=0, requestCount_=0;
    size_t joinCapacity_=0, joinCount_=0, selectedStackBytes_=0;
    std::atomic<size_t> publishedCallCount_{0};
    std::atomic<bool> failed_{false}, ready_{false}, finished_{false}, stopRequested_{false};
    std::atomic<uint64_t> requestedEpoch_{0}, completedEpoch_{0};
    pthread_t originalThread_{};
    pthread_attr_t originalAttributes_{};
    bool startAttempted_=false, attributesAttempted_=false, attributesInitialized_=false;
    int attributesReturned_=0, stackReturned_=0, guardReturned_=0, configuredStackReturned_=0;
    size_t originalGuardBytes_=0, configuredStackBytes_=0;
    int attributeDestroyReturned_=0, createReturned_=0;
    bool attributeDestroyAttempted_=false, attributesDestroyed_=false, threadCreated_=false, joined_=false, joinUnknown_=false;
    pthread_t workerSelf_{};
    long workerTid_=-1;
    sigset_t originalWorkerMask_{}, requestedWorkerMask_{};
    int emptyMaskReturned_=-1, emptyMaskError_=0, addMaskReturned_=-1, addMaskError_=0;
    int blockMaskReturned_=0;
    CandidateFdOriginal wake_, signal_;
    bool sourceCapacityExhausted_=false;
    template<class Call> bool MainCall(LeaseMainCallOriginal& row,const char* operation,
        Call call,bool errnoReturn=false,bool independentlyEligible=false) noexcept {
        row.beforeClock=mainClock_.Observe(operation,false);
        if (!row.beforeClock && !independentlyEligible) { failed_.store(true,std::memory_order_release); return false; }
        row.attempted=true; row.returned=call();
        if (errnoReturn) { if (row.returned<0) row.error=errno; }
        else { row.errorFromReturn=true; row.error=row.returned; }
        row.afterClock=mainClock_.Observe(operation,true);
        if (!row.beforeClock || !row.afterClock) failed_.store(true,std::memory_order_release);
        return row.afterClock;
    }
    LeaseCallOriginal* Reserve(LeaseOperation operation,size_t file=SIZE_MAX) noexcept {
        if (callCount_==callCapacity_) {
            sourceCapacityExhausted_=true; failed_.store(true,std::memory_order_release); return nullptr;
        }
        auto& row=calls_[callCount_++]; row.operation=operation; row.fileOrdinal=file;
        return &row;
    }
    bool Before(LeaseCallOriginal& row,const char* operation) noexcept {
        row.beforeClockClean=workerClock_.Observe(operation,false);
        if (!row.beforeClockClean || failed_.load(std::memory_order_acquire)) { failed_.store(true,std::memory_order_release); return false; }
        row.attempted=true; return true;
    }
    bool After(LeaseCallOriginal& row,const char* operation) noexcept {
        row.afterClockClean=workerClock_.Observe(operation,true);
        publishedCallCount_.store(callCount_,std::memory_order_release);
        if (!row.afterClockClean) failed_.store(true,std::memory_order_release);
        return row.afterClockClean;
    }
    bool Fcntl(LeaseOperation kind,size_t ordinal,int command,int argument) noexcept {
        auto* row=Reserve(kind,ordinal); if (!row) return false;
        row->descriptor=files_[ordinal].source->reader_.returned;
        if (!Before(*row,"private-lease-fcntl")) return false;
        row->returned=fcntl(row->descriptor,command,argument);
        if (row->returned<0) row->error=errno;
        // Actual lease acquisition precedes clock/result classification.
        if (command==F_SETLEASE && argument==F_RDLCK && row->returned==0) files_[ordinal].acquired=true;
        const bool after=After(*row,"private-lease-fcntl");
        const bool expected=command==F_GETLEASE?row->returned==F_RDLCK:
            command==F_GETSIG?row->returned==SIGIO:row->returned==0;
        if (!expected || !after) failed_.store(true,std::memory_order_release);
        return expected&&after;
    }
    bool CheckAllLeases() noexcept {
        bool all=true;
        for (size_t i=0; i<fileCount_; ++i) all=Fcntl(LeaseOperation::GetLease,i,F_GETLEASE,0)&&all;
        return all;
    }
    bool ReadOriginal(size_t ordinal) noexcept {
        auto& file=files_[ordinal]; auto& source=*file.source;
        auto* stat=Reserve(LeaseOperation::FileStat,ordinal); if (!stat) return false;
        stat->descriptor=source.reader_.returned;
        if (!Before(*stat,"private-lease-held-stat")) return false;
        stat->returned=fstat(stat->descriptor,&stat->metadata);
        if (stat->returned<0) stat->error=errno;
        const bool statAfter=After(*stat,"private-lease-held-stat");
        if (stat->returned || !statAfter || !S_ISREG(stat->metadata.st_mode) ||
            stat->metadata.st_dev!=source.bornStat_.st_dev || stat->metadata.st_ino!=source.bornStat_.st_ino ||
            stat->metadata.st_size<0 || static_cast<uintmax_t>(stat->metadata.st_size)!=source.bytes_) return false;
        while (!file.eof) {
            if (!Fcntl(LeaseOperation::GetLease,ordinal,F_GETLEASE,0)) return false;
            auto* row=Reserve(LeaseOperation::FileRead,ordinal); if (!row) return false;
            row->descriptor=source.reader_.returned; row->offset=file.retainedBytes;
            row->requested=file.readCapacity-file.retainedBytes;
            if (row->requested>65536) row->requested=65536;
            if (!row->requested || row->offset>static_cast<uintmax_t>(INT64_MAX)) return false;
            if (!Before(*row,"private-lease-held-read")) return false;
            row->returned=pread(row->descriptor,file.readBytes.get()+row->offset,row->requested,static_cast<off_t>(row->offset));
            if (row->returned<0) row->error=errno;
            const bool after=After(*row,"private-lease-held-read");
            if (row->returned>0 && static_cast<size_t>(row->returned)<=row->requested)
                file.retainedBytes+=static_cast<size_t>(row->returned);
            if (row->returned<0 || static_cast<uintmax_t>(row->returned)>row->requested || !after) return false;
            if (!row->returned) file.eof=true;
            if (file.retainedBytes>source.bytes_) return false;
            if (!Fcntl(LeaseOperation::GetLease,ordinal,F_GETLEASE,0)) return false;
        }
        file.contentVerified=file.retainedBytes==source.bytes_ &&
            (!source.bytes_ || memcmp(file.readBytes.get(),source.source_.get(),source.bytes_)==0);
        return file.contentVerified;
    }
    bool SetupWorker() noexcept {
        auto* self=Reserve(LeaseOperation::ThreadSelf);
        if (!self || !Before(*self,"private-lease-thread-self")) return false;
        workerSelf_=pthread_self(); self->originalThreadSelf=workerSelf_;
        if (!After(*self,"private-lease-thread-self")) return false;
        auto* tid=Reserve(LeaseOperation::ThreadTid); if (!tid || !Before(*tid,"private-lease-thread-tid")) return false;
        tid->returned=syscall(SYS_gettid); if (tid->returned<0) tid->error=errno;
        workerTid_=tid->returned;
        if (!After(*tid,"private-lease-thread-tid") || workerTid_<=0 || workerTid_>INT32_MAX) return false;
        // These mask operations affect THIS newly owned worker only. Its
        // original mask remains retained; creator/child masks are untouched.
        auto* empty=Reserve(LeaseOperation::EmptyOwnSignalMask);
        if (!empty || !Before(*empty,"private-lease-own-empty-mask")) return false;
        emptyMaskReturned_=sigemptyset(&requestedWorkerMask_); if (emptyMaskReturned_<0) emptyMaskError_=errno;
        empty->returned=emptyMaskReturned_; empty->error=emptyMaskError_;
        if (!After(*empty,"private-lease-own-empty-mask") || emptyMaskReturned_) return false;
        auto* add=Reserve(LeaseOperation::AddOwnSignalMask);
        if (!add || !Before(*add,"private-lease-own-add-mask")) return false;
        addMaskReturned_=sigaddset(&requestedWorkerMask_,SIGIO); if (addMaskReturned_<0) addMaskError_=errno;
        add->returned=addMaskReturned_; add->error=addMaskError_;
        if (!After(*add,"private-lease-own-add-mask") || addMaskReturned_) return false;
        if (emptyMaskReturned_ || addMaskReturned_) return false;
        auto* mask=Reserve(LeaseOperation::BlockOwnSignal);
        if (!mask || !Before(*mask,"private-lease-own-mask")) return false;
        mask->requestedMask=requestedWorkerMask_;
        blockMaskReturned_=pthread_sigmask(SIG_BLOCK,&requestedWorkerMask_,&originalWorkerMask_);
        mask->returned=blockMaskReturned_; // pthread result IS the error code.
        mask->error=blockMaskReturned_; mask->originalMask=originalWorkerMask_;
        if (!After(*mask,"private-lease-own-mask") || blockMaskReturned_) return false;
        auto* signal=Reserve(LeaseOperation::SignalFd);
        if (!signal || !Before(*signal,"private-lease-signalfd")) return false;
        signal_.attempted=true; signal_.returned=signalfd(-1,&requestedWorkerMask_,SFD_NONBLOCK|SFD_CLOEXEC);
        if (signal_.returned<0) signal_.error=errno;
        signal->returned=signal_.returned; signal->error=signal_.error;
        if (!After(*signal,"private-lease-signalfd") || signal_.returned<0) return false;
        for (size_t i=0; i<fileCount_; ++i) {
            auto* original=Reserve(LeaseOperation::OriginalRecipient,i); if (!original) return false;
            original->descriptor=files_[i].source->reader_.returned;
            if (!Before(*original,"private-lease-original-route")) return false;
            original->returned=fcntl(original->descriptor,F_GETOWN_EX,&original->recipient);
            if (original->returned<0) original->error=errno;
            if (!After(*original,"private-lease-original-route") || original->returned || original->recipient.pid!=0) return false;
            auto* originalSignal=Reserve(LeaseOperation::OriginalSignal,i); if (!originalSignal) return false;
            originalSignal->descriptor=original->descriptor;
            if (!Before(*originalSignal,"private-lease-original-signal")) return false;
            originalSignal->returned=fcntl(originalSignal->descriptor,F_GETSIG);
            if (originalSignal->returned<0) originalSignal->error=errno;
            if (!After(*originalSignal,"private-lease-original-signal") || originalSignal->returned!=0) return false;
            auto* set=Reserve(LeaseOperation::SetRecipient,i); if (!set) return false;
            set->descriptor=files_[i].source->reader_.returned;
            set->recipient.type=F_OWNER_TID; set->recipient.pid=static_cast<pid_t>(workerTid_);
            if (!Before(*set,"private-lease-route-own-thread")) return false;
            set->returned=fcntl(set->descriptor,F_SETOWN_EX,&set->recipient);
            if (set->returned<0) set->error=errno;
            if (!After(*set,"private-lease-route-own-thread") || set->returned) return false;
            auto* get=Reserve(LeaseOperation::GetRecipient,i); if (!get) return false;
            get->descriptor=set->descriptor;
            if (!Before(*get,"private-lease-read-own-route")) return false;
            get->returned=fcntl(get->descriptor,F_GETOWN_EX,&get->recipient);
            if (get->returned<0) get->error=errno;
            if (!After(*get,"private-lease-read-own-route") || get->returned ||
                get->recipient.type!=F_OWNER_TID || get->recipient.pid!=workerTid_) return false;
            if (!Fcntl(LeaseOperation::SetSignal,i,F_SETSIG,SIGIO) ||
                !Fcntl(LeaseOperation::GetSignal,i,F_GETSIG,0) ||
                !Fcntl(LeaseOperation::Acquire,i,F_SETLEASE,F_RDLCK)) return false;
            files_[i].acquired=true;
            if (!ReadOriginal(i)) return false;
        }
        return CheckAllLeases();
    }
    void RunWorker() noexcept {
        if (!SetupWorker()) { failed_.store(true,std::memory_order_release); FinishWorker(); return; }
        ready_.store(true,std::memory_order_release);
        while (!stopRequested_.load(std::memory_order_acquire)) {
            auto* row=Reserve(LeaseOperation::Poll);
            if (!row || !Before(*row,"private-lease-owned-channel-poll")) break;
            row->pollDescriptors[0]={signal_.returned,POLLIN,0};
            row->pollDescriptors[1]={wake_.returned,POLLIN,0};
            row->returned=poll(row->pollDescriptors.data(),2,-1);
            if (row->returned<0) row->error=errno;
            if (!After(*row,"private-lease-owned-channel-poll") || row->returned<0) break;
            if (row->pollDescriptors[0].revents) {
                auto* signal=Reserve(LeaseOperation::SignalRead);
                if (!signal || !Before(*signal,"private-lease-break-read")) break;
                signal->descriptor=signal_.returned; signal->requested=sizeof(signal->signal);
                signal->returned=read(signal_.returned,&signal->signal,sizeof(signal->signal));
                if (signal->returned<0) signal->error=errno;
                After(*signal,"private-lease-break-read");
                // Any delivered break/unknown shape denies qualification. No
                // automatic release/reacquire and no kernel timeout is ignored.
                failed_.store(true,std::memory_order_release);
            }
            if (row->pollDescriptors[1].revents) {
                auto* request=Reserve(LeaseOperation::RequestRead);
                if (!request || !Before(*request,"private-lease-request-read")) break;
                request->descriptor=wake_.returned; request->requested=sizeof(request->requestBody);
                request->returned=read(wake_.returned,&request->requestBody,sizeof(request->requestBody));
                if (request->returned<0) request->error=errno;
                const bool after=After(*request,"private-lease-request-read");
                if (!after || request->returned!=sizeof(request->requestBody)) { failed_.store(true,std::memory_order_release); break; }
                if (stopRequested_.load(std::memory_order_acquire)) break;
                const uint64_t epoch=requestedEpoch_.load(std::memory_order_acquire);
                if (!CheckAllLeases()) failed_.store(true,std::memory_order_release);
                completedEpoch_.store(epoch,std::memory_order_release);
            }
            if (failed_.load(std::memory_order_acquire)) break;
        }
        FinishWorker();
    }
    void FinishWorker() noexcept {
        // On early failure, do NOT release the leases while native commands
        // might still require input custody. Main retains their original FDs.
        // Blocked worker-only SIGIO is never restored into a default handler.
        publishedCallCount_.store(callCount_,std::memory_order_release);
        finished_.store(true,std::memory_order_release);
    }
    static void* ThreadMain(void* owner) noexcept {
        static_cast<OriginalLeaseOwner*>(owner)->RunWorker(); return nullptr;
    }
    bool NativeCommandsPermitDisposal() const noexcept {
        return !route_.AnyCommandAcquisitionAttempted() || route_.AllPhysicalCommandsDisposed();
    }
    bool WriteRequest(bool stop,uint64_t epoch) noexcept {
        if (!stop && requestCount_==requestCapacity_) { failed_.store(true,std::memory_order_release); return false; }
        auto& row=stop?originalStopRequest_:requests_[requestCount_++]; row.stop=stop; row.epoch=epoch;
        row.beforeClock=mainClock_.Observe("private-lease-request-write",false);
        if (!row.beforeClock && !stop) { failed_.store(true,std::memory_order_release); return false; }
        row.attempted=true; row.returned=write(wake_.returned,&row.writtenBody,sizeof(row.writtenBody));
        if (row.returned<0) row.error=errno;
        row.afterClock=mainClock_.Observe("private-lease-request-write",true);
        if (row.returned!=sizeof(row.writtenBody) || (!stop&&(!row.beforeClock||!row.afterClock))) {
            failed_.store(true,std::memory_order_release); return false;
        }
        return true;
    }
    ~OriginalLeaseOwner()=default; // Resident source owner: never implicit cleanup.
public:
    OriginalLeaseOwner(OriginalCommandRoute& route,OriginalDeadline& actualOrigin,
        OriginalPrivateCandidateFile* const* actualFiles,size_t fileCount,size_t callRows,
        size_t requestRows,size_t joinRows,size_t workerClockRows,size_t selectedStackBytes)
        : route_(route),mainClock_(actualOrigin),workerClock_(workerClockRows,
            actualOrigin.HasOriginalDeadline(),actualOrigin.OriginalAbsoluteNanoseconds()) {
        if (!actualFiles || !fileCount || fileCount>SIZE_MAX/sizeof(LeaseFileOriginal) ||
            !callRows || callRows>SIZE_MAX/sizeof(LeaseCallOriginal) || !requestRows ||
            requestRows>SIZE_MAX/sizeof(LeaseRequestOriginal) || !joinRows ||
            joinRows>SIZE_MAX/sizeof(LeaseJoinOriginal) || !selectedStackBytes)
            throw std::invalid_argument("precharged private lease worker geometry");
        files_=std::make_unique<LeaseFileOriginal[]>(fileCount);
        calls_=std::make_unique<LeaseCallOriginal[]>(callRows);
        requests_=std::make_unique<LeaseRequestOriginal[]>(requestRows);
        joins_=std::make_unique<LeaseJoinOriginal[]>(joinRows);
        for (size_t i=0; i<fileCount; ++i) {
            if (!actualFiles[i] || actualFiles[i]->SourceByteCount()==SIZE_MAX)
                throw std::invalid_argument("actual private candidate source owner");
            for (size_t j=0; j<i; ++j) if (actualFiles[i]==actualFiles[j])
                throw std::invalid_argument("distinct private candidate owner required");
            files_[i].source=actualFiles[i]; files_[i].readCapacity=actualFiles[i]->SourceByteCount()+1;
            files_[i].readBytes=std::make_unique<unsigned char[]>(files_[i].readCapacity);
        }
        fileCount_=fileCount; callCapacity_=callRows; requestCapacity_=requestRows;
        joinCapacity_=joinRows; selectedStackBytes_=selectedStackBytes;
    }
    OriginalLeaseOwner(const OriginalLeaseOwner&)=delete;
    OriginalLeaseOwner& operator=(const OriginalLeaseOwner&)=delete;
    bool Start() noexcept {
        if (startAttempted_ || route_.AnyCommandAcquisitionAttempted()) return false;
        startAttempted_=true;
        for (size_t i=0; i<fileCount_; ++i) if (!files_[i].source->LeaseEligibleOriginal()) {
            failed_.store(true,std::memory_order_release); return false;
        }
        for (size_t i=0; i<fileCount_; ++i) files_[i].source->leaseAttached_=true;
        // Every native setup return stays in its once-only original owner.
        const bool initializedClock=MainCall(initializeAttributes_,"private-lease-attribute-init",[&]() noexcept {
            attributesAttempted_=true; attributesReturned_=pthread_attr_init(&originalAttributes_);
            attributesInitialized_=attributesReturned_==0; return attributesReturned_;
        });
        if (!attributesInitialized_ || !initializedClock) { failed_.store(true,std::memory_order_release); return false; }
        const bool stackClock=MainCall(configureStack_,"private-lease-configure-stack",[&]() noexcept {
            stackReturned_=pthread_attr_setstacksize(&originalAttributes_,selectedStackBytes_); return stackReturned_;
        });
        if (!stackClock || stackReturned_) { failed_.store(true,std::memory_order_release); return false; }
        const bool stackQueryClock=MainCall(queryStack_,"private-lease-query-stack",[&]() noexcept {
            configuredStackReturned_=pthread_attr_getstacksize(&originalAttributes_,&configuredStackBytes_); return configuredStackReturned_;
        });
        if (!stackQueryClock || configuredStackReturned_) { failed_.store(true,std::memory_order_release); return false; }
        const bool guardClock=MainCall(queryGuard_,"private-lease-query-guard",[&]() noexcept {
            guardReturned_=pthread_attr_getguardsize(&originalAttributes_,&originalGuardBytes_); return guardReturned_;
        });
        if (!guardClock || guardReturned_ || configuredStackBytes_!=selectedStackBytes_) {
            failed_.store(true,std::memory_order_release); return false;
        }
        const bool wakeClock=MainCall(createWake_,"private-lease-create-owned-wake",[&]() noexcept {
            wake_.attempted=true; wake_.returned=eventfd(0,EFD_NONBLOCK|EFD_CLOEXEC); return wake_.returned;
        },true);
        wake_.error=createWake_.error;
        if (!wakeClock || wake_.returned<0) { failed_.store(true,std::memory_order_release); return false; }
        const bool createClock=MainCall(createThread_,"private-lease-create-owned-thread",[&]() noexcept {
            createReturned_=pthread_create(&originalThread_,&originalAttributes_,ThreadMain,this);
            threadCreated_=createReturned_==0; return createReturned_;
        });
        if (!threadCreated_) failed_.store(true,std::memory_order_release);
        MainCall(destroyAttributes_,"private-lease-eligible-attribute-destroy",[&]() noexcept {
            attributeDestroyAttempted_=true; attributeDestroyReturned_=pthread_attr_destroy(&originalAttributes_);
            attributesDestroyed_=attributeDestroyReturned_==0; return attributeDestroyReturned_;
        },false,true);
        if (!attributesDestroyed_) failed_.store(true,std::memory_order_release);
        return createClock&&threadCreated_&&!failed_.load(std::memory_order_acquire);
    }
    uint64_t RequestCheck() noexcept {
        if (!threadCreated_ || !ready_.load(std::memory_order_acquire) || failed_.load(std::memory_order_acquire) ||
            finished_.load(std::memory_order_acquire) || stopRequested_.load(std::memory_order_acquire)) return 0;
        const uint64_t previous=requestedEpoch_.load(std::memory_order_acquire);
        if (previous!=completedEpoch_.load(std::memory_order_acquire) || previous==UINT64_MAX) return 0;
        const uint64_t next=previous+1; requestedEpoch_.store(next,std::memory_order_release);
        return WriteRequest(false,next)?next:0;
    }
    bool QualifiedEpoch(uint64_t originalRequestedEpoch) const noexcept {
        return originalRequestedEpoch && ready_.load(std::memory_order_acquire) &&
            !failed_.load(std::memory_order_acquire) && !finished_.load(std::memory_order_acquire) &&
            !stopRequested_.load(std::memory_order_acquire) &&
            requestedEpoch_.load(std::memory_order_acquire)==originalRequestedEpoch &&
            completedEpoch_.load(std::memory_order_acquire)==originalRequestedEpoch;
    }
    bool RequestEligibleStop() noexcept {
        if (!NativeCommandsPermitDisposal()) return false;
        if (!threadCreated_ || finished_.load(std::memory_order_acquire)) return true;
        if (stopRequested_.exchange(true,std::memory_order_acq_rel)) return true;
        return WriteRequest(true,requestedEpoch_.load(std::memory_order_acquire));
    }
    bool PollEligibleJoinAndRelease() noexcept {
        if (!NativeCommandsPermitDisposal()) return false;
        if (threadCreated_ && !joined_) {
            if (!finished_.load(std::memory_order_acquire) || joinUnknown_ || joinCount_==joinCapacity_) return false;
            auto& row=joins_[joinCount_++]; row.attempted=true;
            row.beforeClock=mainClock_.Observe("private-lease-eligible-tryjoin",false);
            row.returned=pthread_tryjoin_np(originalThread_,&row.originalThreadReturn);
            row.afterClock=mainClock_.Observe("private-lease-eligible-tryjoin",true);
            if (!row.beforeClock || !row.afterClock) failed_.store(true,std::memory_order_release);
            if (!row.returned) {
                joined_=true; // Physical join survives a failed return-shape check.
                if (row.originalThreadReturn!=nullptr) failed_.store(true,std::memory_order_release);
            }
            else if (row.returned!=EBUSY) { joinUnknown_=true; failed_.store(true,std::memory_order_release); }
            if (!joined_) return false;
        }
        bool all=true;
        for (size_t i=0; i<fileCount_; ++i) {
            auto& file=files_[i];
            if (file.acquired && !file.unlockAttempted) {
                file.unlockBeforeClock=mainClock_.Observe("private-lease-eligible-unlock",false);
                file.unlockAttempted=true;
                file.unlockReturned=fcntl(file.source->reader_.returned,F_SETLEASE,F_UNLCK);
                if (file.unlockReturned<0) file.unlockError=errno;
                file.unlockAfterClock=mainClock_.Observe("private-lease-eligible-unlock",true);
            }
            const bool released=!file.acquired || (file.unlockAttempted&&file.unlockReturned==0);
            if (released) file.source->leaseAttached_=false;
            all=released&&all;
        }
        if (attributesInitialized_ && !attributesDestroyed_) {
            if (!attributeDestroyAttempted_) {
                MainCall(destroyAttributes_,"private-lease-eligible-attribute-destroy",[&]() noexcept {
                    attributeDestroyAttempted_=true; attributeDestroyReturned_=pthread_attr_destroy(&originalAttributes_);
                    attributesDestroyed_=attributeDestroyReturned_==0; return attributeDestroyReturned_;
                },false,true);
            }
            // An attempted failed destroy is uncertain: never retry it.
            all=attributesDestroyed_&&all;
        }
        const bool signalClosed=signal_.CloseOnce(mainClock_);
        const bool wakeClosed=wake_.CloseOnce(mainClock_);
        return all&&signalClosed&&wakeClosed&&(!threadCreated_||joined_);
    }
    bool Failed() const noexcept { return failed_.load(std::memory_order_acquire); }
    size_t PublishedCallCount() const noexcept { return publishedCallCount_.load(std::memory_order_acquire); }
    const LeaseCallOriginal* PrivateCalls() const noexcept { return calls_.get(); }
    const LeaseFileOriginal* PrivateFiles() const noexcept { return files_.get(); }
    bool PrivateFilesReadyForCapture() const noexcept {
        return ready_.load(std::memory_order_acquire)||finished_.load(std::memory_order_acquire);
    }
    const LeaseRequestOriginal* PrivateRequests() const noexcept { return requests_.get(); }
    size_t RequestCount() const noexcept { return requestCount_; }
    const LeaseRequestOriginal& PrivateStopRequest() const noexcept { return originalStopRequest_; }
    const LeaseJoinOriginal* PrivateJoins() const noexcept { return joins_.get(); }
    size_t JoinCount() const noexcept { return joinCount_; }
    bool ReservedSourceBytes(size_t& bytes) const noexcept {
        bytes=sizeof(*this);
        auto add=[&](size_t amount) noexcept { if (amount>SIZE_MAX-bytes) return false; bytes+=amount; return true; };
        if (!add(fileCount_*sizeof(LeaseFileOriginal)) || !add(callCapacity_*sizeof(LeaseCallOriginal)) ||
            !add(requestCapacity_*sizeof(LeaseRequestOriginal)) || !add(joinCapacity_*sizeof(LeaseJoinOriginal)) ||
            !add(workerClock_.ReservedSourceBytes()-sizeof(OriginalDeadline))) return false;
        for (size_t i=0; i<fileCount_; ++i) if (!add(files_[i].readCapacity)) return false;
        if (queryStack_.attempted && !configuredStackReturned_ && queryGuard_.attempted && !guardReturned_ && configuredStackBytes_) {
            if (!add(configuredStackBytes_) || !add(originalGuardBytes_)) return false;
        } else return false;
        return true; // TLS/kernel/libc/allocator and candidate source owners additional.
    }
};
// SIGIO coalescing cannot restore qualification: every signal/shape fails the
// whole owner, and each fresh gate epoch queries every retained actual lease.
// F_GETLEASE's break target (F_UNLCK) and actual timeout loss deny every later
// positive read. Directory/mount/loaded-image/caller authority remain separate.
}

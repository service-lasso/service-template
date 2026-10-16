// UNCOMPILED independent same-process sink readback, not external exit proof.
#pragma once
#include "linux_native_sink.hpp"
#include <pthread.h>
#include <atomic>
namespace template_creator_bridge {struct NativePlatformState;}
namespace template_creator_linux {
struct LinuxCollectorRead {SupervisorNativeOriginal call;};
class OriginalLinuxSinkCollector {
    friend struct template_creator_bridge::NativePlatformState;
    OriginalNativeSink& sink_;
    const OriginalDeadline& deadline_;
    OriginalDeadlineSupervisor& supervisor_;
    std::unique_ptr<unsigned char[]> bytes_;
    std::unique_ptr<LinuxCollectorRead[]> reads_;
    std::unique_ptr<SupervisorNativeOriginal[]> joins_;
    std::unique_ptr<void*[]> joinReturns_;
    size_t byteCapacity_=0,readCapacity_=0,joinCapacity_=0,joinReturnCapacity_=0,readCount_=0,joinCount_=0,extent_=0,expected_=0;
    size_t selectedBytes_=0,selectedReads_=0,selectedJoins_=0;
    bool preownAttempted_=false,preowned_=false;
    std::exception_ptr preownException_;
    const unsigned char* expectedBytes_=nullptr;
    int reader_=-1;pthread_t worker_{};pthread_attr_t attributes_{};
    struct stat originalMetadata_{},actualMetadata_{};
    SupervisorNativeOriginal duplicate_,originalStat_,readerStat_,attrInit_,attrStack_,stackQuery_,guardQuery_,
        create_,attrDestroy_,workerTid_,readerClose_,finalJoin_;
    void* finalJoinValue_=nullptr;bool finalJoinAdmissionAttempted_=false;
    size_t selectedStack_=0,actualStack_=0,actualGuard_=0;
    bool attempted_=false,attributesAcquired_=false,created_=false,joined_=false,joinUnknown_=false;
    bool readerAcquired_=false,readerUnknown_=false,matched_=false,eof_=false;
    std::atomic<bool> failed_{false},startPublished_{false};
    std::atomic<bool> returned_{false};
    ~OriginalLinuxSinkCollector()=default; // SAME main retains through actual join.
    bool Before(SupervisorNativeOriginal& row,const char* operation) noexcept {
        const bool clock=CaptureSupervisorClock(row.beforeClock,deadline_,operation,false);
        if(!clock)failed_=true;
        if(!supervisor_.BeginIndependentlyEligibleCleanupNativeCall()){failed_=true;return false;}
        row.attempted=true;return true;
    }
    void Returned(SupervisorNativeOriginal& row,long value,bool errnoStyle=true) noexcept {
        row.returned=value;if(errnoStyle&&value<0)row.error=errno;
        if(!errnoStyle&&value)row.error=static_cast<int>(value);
        row.returnedObserved=true;supervisor_.OriginalNativeCallReturned();
        if(!CaptureSupervisorClock(row.afterClock,deadline_,row.beforeClock.operation,true)||
            !row.beforeClock.shapeValid||!row.afterClock.shapeValid||row.afterClock.nanoseconds<row.beforeClock.nanoseconds)failed_=true;
    }
    void CloseReaderOnce() noexcept {
        if(!readerAcquired_||readerClose_.attempted||readerUnknown_)return;
        if(!Before(readerClose_,"sink-collector-close"))return;
        Returned(readerClose_,close(reader_));if(readerClose_.returned)failed_=true;
    }
    void Run() noexcept {
        if(!Before(workerTid_,"sink-collector-gettid"))return;
        Returned(workerTid_,syscall(SYS_gettid));
        if(workerTid_.returned<=0){failed_=true;CloseReaderOnce();return;}
        if(!Before(readerStat_,"sink-collector-fstat")){CloseReaderOnce();return;}
        Returned(readerStat_,fstat(reader_,&actualMetadata_));
        bool equal=readerStat_.returned==0&&originalStat_.returned==0&&
            actualMetadata_.st_dev==originalMetadata_.st_dev&&actualMetadata_.st_ino==originalMetadata_.st_ino;
        if(!equal){failed_=true;CloseReaderOnce();return;}
        while(extent_<expected_){
            if(readCount_==readCapacity_){failed_=true;break;}
            auto& row=reads_[readCount_++].call;row.destinationOffset=extent_;
            row.requested=expected_-extent_>65536?65536:expected_-extent_;
            if(!Before(row,"sink-collector-pread"))break;
            const auto value=pread(reader_,bytes_.get()+extent_,row.requested,static_cast<off_t>(extent_));
            // Raw errno precedes any clock or shape work.
            Returned(row,value);
            if(value<0){failed_=true;break;}
            if(static_cast<size_t>(value)>row.requested){readerUnknown_=failed_=true;break;}
            const size_t before=extent_;extent_+=static_cast<size_t>(value);
            if(!value){failed_=true;break;}
            if(memcmp(bytes_.get()+before,expectedBytes_+before,static_cast<size_t>(value)))equal=false;
        }
        if(extent_==expected_){
            if(readCount_==readCapacity_)failed_=true;
            else {
                auto& row=reads_[readCount_++].call;row.destinationOffset=extent_;row.requested=1;
                if(Before(row,"sink-collector-eof")){
                    Returned(row,pread(reader_,bytes_.get()+extent_,1,static_cast<off_t>(extent_)));
                    if(row.returned>=0&&row.returned<=1){extent_+=static_cast<size_t>(row.returned);eof_=row.returned==0;}
                    else {failed_=true;if(row.returned>1)readerUnknown_=true;}
                }
            }
        }
        matched_=equal&&eof_&&extent_==expected_;if(!matched_)failed_=true;
        CloseReaderOnce();
    }
    static void* ThreadMain(void* opaque) noexcept {
        auto& self=*static_cast<OriginalLinuxSinkCollector*>(opaque);
        // Source publication barrier: creator finishes create/attribute calls
        // and releases their one pending slot before worker enters native IO.
        // If creator remains pending, originals/worker stay resident and SAME
        // already-live deadline observers retain caller-notice responsibility.
        while(!self.startPublished_.load(std::memory_order_acquire)){}
        self.Run();
        self.returned_.store(true,std::memory_order_release);return nullptr;
    }
public:
    OriginalLinuxSinkCollector(DeferredOriginalPreownTag,OriginalNativeSink& sink,size_t bytes,size_t reads,size_t joins,size_t stack) noexcept
        :sink_(sink),deadline_(sink.OriginalDeadlineOwner()),supervisor_(sink.OriginalSupervisorOwner()),
         selectedBytes_(bytes),selectedReads_(reads),selectedJoins_(joins),selectedStack_(stack){}
    OriginalLinuxSinkCollector(OriginalNativeSink& sink,size_t bytes,size_t reads,size_t joins,size_t stack)
        :OriginalLinuxSinkCollector(DeferredOriginalPreown,sink,bytes,reads,joins,stack){
        if(!PreownOriginal()&&preownException_)std::rethrow_exception(preownException_);
    }
    bool PreownOriginal() noexcept {
        if(preownAttempted_)return false;preownAttempted_=true;
        const size_t bytes=selectedBytes_,reads=selectedReads_,joins=selectedJoins_,stack=selectedStack_;
        try{
        if(!bytes||bytes==SIZE_MAX||bytes>=static_cast<uint64_t>(INT64_MAX)||!reads||!joins||!stack||
           reads>SIZE_MAX/sizeof(LinuxCollectorRead)||joins>SIZE_MAX/sizeof(SupervisorNativeOriginal)||joins>SIZE_MAX/sizeof(void*))
            throw std::invalid_argument("original Linux sink collector geometry");
        // Bound the combined backing before the first allocation, not only
        // each separate multiplication or a later accounting observation.
        size_t sourceBytes=sizeof(*this);
        const auto add=[&](size_t amount){
            if(amount>SIZE_MAX-sourceBytes)return false;sourceBytes+=amount;return true;
        };
        if(!add(bytes+1)||!add(reads*sizeof(LinuxCollectorRead))||
           !add(joins*sizeof(SupervisorNativeOriginal))||!add(joins*sizeof(void*)))
            throw std::invalid_argument("aggregate original Linux sink collector backing");
        bytes_=std::make_unique<unsigned char[]>(bytes+1);byteCapacity_=bytes;
        reads_=std::make_unique<LinuxCollectorRead[]>(reads);readCapacity_=reads;
        joins_=std::make_unique<SupervisorNativeOriginal[]>(joins);joinCapacity_=joins;
        joinReturns_=std::make_unique<void*[]>(joins);joinReturnCapacity_=joins;
        preowned_=true;return true;
        }catch(...){preownException_=std::current_exception();failed_=true;return false;}
    }
    bool OutputBackingSeparateOriginal(const void* output,size_t bytes) const noexcept {
        return OriginalBackingStorageSeparate(output,bytes,this,1,sizeof(*this))&&
            OriginalBackingStorageSeparate(output,bytes,bytes_.get(),byteCapacity_)&&
            OriginalBackingStorageSeparate(output,bytes,reads_.get(),readCapacity_,sizeof(LinuxCollectorRead))&&
            OriginalBackingStorageSeparate(output,bytes,joins_.get(),joinCapacity_,sizeof(SupervisorNativeOriginal))&&
            OriginalBackingStorageSeparate(output,bytes,joinReturns_.get(),joinReturnCapacity_,sizeof(void*));
    }
    bool PreownedOriginal() const noexcept{return preowned_;}
    const std::exception_ptr& PreownExceptionOriginal() const noexcept{return preownException_;}
    size_t AllocatedByteCapacityOriginal() const noexcept{return bytes_?byteCapacity_+1:0;}
    size_t AllocatedReadCapacityOriginal() const noexcept{return readCapacity_;}
    size_t AllocatedJoinCapacityOriginal() const noexcept{return joinCapacity_;}
    size_t AllocatedJoinReturnCapacityOriginal() const noexcept{return joinReturnCapacity_;}
    OriginalLinuxSinkCollector(const OriginalLinuxSinkCollector&)=delete;
    OriginalLinuxSinkCollector& operator=(const OriginalLinuxSinkCollector&)=delete;
    bool Start() noexcept {
        if(!preowned_||attempted_)return false;attempted_=true;
        if(!sink_.SealOriginalCopies()||sink_.OriginalWrittenExtent()>byteCapacity_){failed_=true;return false;}
        expected_=sink_.OriginalWrittenExtent();expectedBytes_=sink_.PrivateOriginalCopiedBytes();
        if(!Before(originalStat_,"sink-collector-original-fstat"))return false;
        Returned(originalStat_,fstat(sink_.BorrowedOriginal(),&originalMetadata_));
        if(originalStat_.returned){failed_=true;return false;}
        if(!Before(duplicate_,"sink-collector-duplicate"))return false;
        Returned(duplicate_,fcntl(sink_.BorrowedOriginal(),F_DUPFD_CLOEXEC,0));
        reader_=static_cast<int>(duplicate_.returned);readerAcquired_=duplicate_.returned>=0;
        if(!readerAcquired_){failed_=true;return false;}
        if(!Before(attrInit_,"sink-collector-attr-init")){CloseReaderOnce();return false;}
        Returned(attrInit_,pthread_attr_init(&attributes_),false);attributesAcquired_=attrInit_.returned==0;
        if(!attributesAcquired_){CloseReaderOnce();return false;}
        if(Before(attrStack_,"sink-collector-attr-stack"))Returned(attrStack_,pthread_attr_setstacksize(&attributes_,selectedStack_),false);
        if(Before(stackQuery_,"sink-collector-stack-query"))Returned(stackQuery_,pthread_attr_getstacksize(&attributes_,&actualStack_),false);
        if(Before(guardQuery_,"sink-collector-guard-query"))Returned(guardQuery_,pthread_attr_getguardsize(&attributes_,&actualGuard_),false);
        if(attrStack_.returnedObserved&&!attrStack_.returned&&stackQuery_.returnedObserved&&!stackQuery_.returned&&
           guardQuery_.returnedObserved&&!guardQuery_.returned&&Before(create_,"sink-collector-thread-create")){
            // pthread_create publishes all immutable worker inputs. Creator
            // must not touch worker-owned fields until returned acquire/join.
            Returned(create_,pthread_create(&worker_,&attributes_,ThreadMain,this),false);created_=create_.returned==0;
        }
        if(Before(attrDestroy_,"sink-collector-attr-destroy"))Returned(attrDestroy_,pthread_attr_destroy(&attributes_),false);
        if(!created_){failed_=true;CloseReaderOnce();}
        else startPublished_.store(true,std::memory_order_release);
        return created_;
    }
    bool PollEligibleJoin() noexcept {
        if(created_&&!joined_){
            if(!returned_.load(std::memory_order_acquire)||joinUnknown_)return false;
            if(joinCount_==joinCapacity_){
                failed_=true; // Exhausted observation budget never qualifies.
                if(finalJoinAdmissionAttempted_)return false;
                finalJoinAdmissionAttempted_=true;
                // Worker published after Run, but may still execute native TLS
                // or runtime exit callbacks. Only actual join proves terminal.
                if(!Before(finalJoin_,"sink-collector-final-join"))return false;
                Returned(finalJoin_,pthread_join(worker_,&finalJoinValue_),false);
                if(finalJoin_.returned){joinUnknown_=true;return false;}
                joined_=true;if(finalJoinValue_)failed_=true;
            }
            if(!joined_){
                const size_t ordinal=joinCount_++;auto& row=joins_[ordinal];
                if(!Before(row,"sink-collector-tryjoin"))return false;
                Returned(row,pthread_tryjoin_np(worker_,&joinReturns_[ordinal]),false);
                if(!row.returned){joined_=true;if(joinReturns_[ordinal])failed_=true;}
                else if(row.returned!=EBUSY){joinUnknown_=failed_=true;}
            }
        }
        if(!created_||joined_){
            if(attributesAcquired_&&!attrDestroy_.attempted&&Before(attrDestroy_,"sink-collector-eligible-attr-destroy"))
                Returned(attrDestroy_,pthread_attr_destroy(&attributes_),false);
            CloseReaderOnce();
        }
        return PhysicalDisposed();
    }
    bool PhysicalDisposed() const noexcept {
        // Never inspect worker-written close/shape fields before actual join.
        if(created_&&!joined_)return false;
        return (!readerAcquired_||(readerClose_.returnedObserved&&!readerClose_.returned))&&
            !readerUnknown_&&(!attributesAcquired_||(attrDestroy_.returnedObserved&&!attrDestroy_.returned));
    }
    bool ObserveSelectedClosure() noexcept {
        // No second blocking native call competes with the single original
        // pending IO slot. Existing independent supervisor/caller remains live.
        // Protected None remains None; expiry/failure returns unresolved custody
        // without cancellation, and main must keep this owner/borrowers alive.
        // One finite observation. The source entry owns paced progression and
        // its separately precharged dispatch subdivision; None is not a timer.
        return PollEligibleJoin();
    }
    bool Qualified() const noexcept{return created_&&joined_&&matched_&&eof_&&!failed_&&PhysicalDisposed()&&sink_.Qualified();}
    bool ReservedSourceBytes(size_t& bytes) const noexcept {
        bytes=sizeof(*this);const auto add=[&](size_t amount){if(amount>SIZE_MAX-bytes)return false;bytes+=amount;return true;};
        if(!add(bytes_?byteCapacity_+1:0)||!add(readCapacity_*sizeof(LinuxCollectorRead))||
           !add(joinCapacity_*sizeof(SupervisorNativeOriginal))||!add(joinReturnCapacity_*sizeof(void*)))return false;
        if(attributesAcquired_){
            if(!stackQuery_.returnedObserved||stackQuery_.returned||!guardQuery_.returnedObserved||guardQuery_.returned)return false;
            if(!add(actualStack_)||!add(actualGuard_))return false;
        }
        return true; // Kernel/TLS/allocator/runtime and overlap still additional.
    }
    // Native main must not inspect worker-owned fields or destroy any original
    // before acquire plus actual join. No independent process/exit assertion.
};
}

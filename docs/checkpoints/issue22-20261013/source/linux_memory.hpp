// UNCOMPILED actual held-task maps and selected mapped-span byte custody.
// No physical/relocated equality, anonymous/JIT authority or atomic snapshot
// is inferred. This reader cannot admit a process or a library by pathname.
#pragma once
#include "linux_task_identity.hpp"
#include "linux_deadline_supervisor.hpp"
#include "linux_ingress.hpp"
#include "linux_driver.hpp"
namespace template_creator_linux {
struct LinuxMappedOriginal {
    uint64_t begin=0,end=0,fileOffset=0,deviceMajor=0,deviceMinor=0,inode=0;
    size_t rawLineOffset=0,rawLineBytes=0,rawNameOffset=0,rawNameBytes=0;
    char permissions[4]{};
    bool captureAttempted=false,captured=false,shapeFailure=false;
    size_t destinationOffset=0,retainedBytes=0;
    uint64_t capturedBegin=0,capturedEnd=0;
    size_t firstReadOrdinal=SIZE_MAX,readCount=0;
};
struct LinuxMemoryReadOriginal {
    uint64_t virtualAddress=0;size_t requested=0,destinationOffset=0;
    ssize_t returned=-1;int error=0;bool attempted=false;
    OriginalClockBoundary before{},after{};
};
struct LinuxMemoryThreadOriginal {
    long returned=-1;int error=0;bool attempted=false;
    OriginalClockBoundary before{},after{};
};
struct LinuxMemorySpanOriginal {
    size_t mappingOrdinal=SIZE_MAX;
    LinuxMappedOriginal captured;
};
class OriginalLinuxMappedMemory {
    HeldTaskIdentity& task_;
    OriginalDeadlineSupervisor& supervisor_;
    OriginalDeadline& deadline_;
    ProcFileObservation maps_;
    ProcOwnedFd memory_;
    const State* stoppedState_=nullptr;
    const Tracee* stoppedTracee_=nullptr;
    const Event* stoppedEvent_=nullptr;
    bool SameRetainedStop() const noexcept {
        if(!stoppedState_||!stoppedTracee_||!stoppedEvent_)return false;
        const auto& state=*stoppedState_;const auto& task=*stoppedTracee_;const auto& event=*stoppedEvent_;
        size_t ordinal=state.eventCount;
        for(size_t i=0;i<state.eventCount;++i)if(&state.events[i]==&event){ordinal=i;break;}
        if(ordinal==state.eventCount||!RetainedBirthAssociation(task,state)||task.terminal||task.supersededByExec||
           !event.waitAttempted||event.returnedTid!=task.tid||event.requestedTid!=task.tid||
           !WIFSTOPPED(event.status)||event.continued||!state.creatingThreadShapeValid)return false;
        for(size_t i=ordinal+1;i<state.eventCount;++i)
            if(state.events[i].waitAttempted&&state.events[i].requestedTid==task.tid)return false;
        return true;
    }
    IngressNativeClock memoryOpenClock_,memoryCloseClock_;
    std::unique_ptr<LinuxMappedOriginal[]> regions_;
    std::unique_ptr<LinuxMemoryReadOriginal[]> reads_;
    std::unique_ptr<LinuxMemoryThreadOriginal[]> threads_;
    std::unique_ptr<LinuxMemorySpanOriginal[]> spans_;
    size_t spanCapacity_=0,spanCount_=0;
    size_t threadCapacity_=0,threadCount_=0;
    std::unique_ptr<unsigned char[]> bytes_;
    size_t regionCapacity_=0,regionCount_=0,readCapacity_=0,readCount_=0,byteCapacity_=0,reservedExtent_=0;
    bool mapsAttempted_=false,mapsCaptured_=false,failed_=false;
    bool ObserveOriginalTracerThread() noexcept {
        if(!stoppedState_||threadCount_==threadCapacity_)return false;
        auto& row=threads_[threadCount_++];
        if(!CaptureSupervisorClock(row.before,deadline_,"memory-original-tracer-thread",false)||
           !supervisor_.BeginNativeCall())return false;
        row.attempted=true;row.returned=syscall(SYS_gettid);if(row.returned<0)row.error=errno;
        supervisor_.OriginalNativeCallReturned();
        const bool after=CaptureSupervisorClock(row.after,deadline_,"memory-original-tracer-thread",true);
        return after&&row.after.nanoseconds>=row.before.nanoseconds&&row.returned>0&&
            stoppedState_->creatingThreadShapeValid&&row.returned==stoppedState_->originalCreatingThread.result;
    }
    static bool Number(const unsigned char*& p,const unsigned char* end,unsigned radix,uint64_t& value) noexcept {
        const auto* start=p;value=0;
        while(p!=end){unsigned digit=255;
            if(*p>='0'&&*p<='9')digit=*p-'0';
            else if(*p>='a'&&*p<='f')digit=*p-'a'+10;
            else if(*p>='A'&&*p<='F')digit=*p-'A'+10;
            if(digit>=radix)break;
            if(value>(UINT64_MAX-digit)/radix)return false;value=value*radix+digit;++p;
        }return p!=start;
    }
    static bool Space(const unsigned char*& p,const unsigned char* end) noexcept {
        const auto* start=p;while(p!=end&&(*p==' '||*p=='\t'))++p;return p!=start;
    }
    bool ParseMaps() noexcept {
        const auto* base=maps_.bytes.get();const auto* p=base;const auto* end=base+maps_.used;
        while(p!=end){
            if(regionCount_==regionCapacity_)return false;
            auto& row=regions_[regionCount_++];const auto* line=p;
            const auto* lineEnd=p;while(lineEnd!=end&&*lineEnd!='\n')++lineEnd;
            row.rawLineOffset=line-base;row.rawLineBytes=lineEnd-line;
            if(!Number(p,lineEnd,16,row.begin)||p==lineEnd||*p++!='-'||!Number(p,lineEnd,16,row.end)||
               row.begin>=row.end||!Space(p,lineEnd)||lineEnd-p<4)return false;
            std::memcpy(row.permissions,p,4);p+=4;
            if((row.permissions[0]!='r'&&row.permissions[0]!='-')||
               (row.permissions[1]!='w'&&row.permissions[1]!='-')||
               (row.permissions[2]!='x'&&row.permissions[2]!='-')||
               (row.permissions[3]!='p'&&row.permissions[3]!='s')||!Space(p,lineEnd)||
               !Number(p,lineEnd,16,row.fileOffset)||!Space(p,lineEnd)||!Number(p,lineEnd,16,row.deviceMajor)||
               p==lineEnd||*p++!=':'||!Number(p,lineEnd,16,row.deviceMinor)||!Space(p,lineEnd)||
               !Number(p,lineEnd,10,row.inode))return false;
            if(p!=lineEnd&&!Space(p,lineEnd))return false;
            row.rawNameOffset=p-base;row.rawNameBytes=lineEnd-p;
            if(regionCount_>1&&regions_[regionCount_-2].end>row.begin)return false;
            p=lineEnd==end?end:lineEnd+1;
        }return regionCount_!=0;
    }
    ~OriginalLinuxMappedMemory()=default;
    friend struct std::default_delete<OriginalLinuxMappedMemory>;
public:
    static size_t CheckedSourceBacking(OriginalDeadlineSupervisor& supervisor,OriginalDeadline& origin,
            size_t mapsBytes,size_t mapsReads,size_t regions,size_t bytes,size_t reads,size_t spansPerRegion=5) {
        if(supervisor.OriginalDeadlineOwner()!=&origin)throw std::invalid_argument("same mapped-memory origin");
        return CheckedGeometrySourceBacking(mapsBytes,mapsReads,regions,bytes,reads,spansPerRegion);
    }
    static size_t CheckedGeometrySourceBacking(size_t mapsBytes,size_t mapsReads,size_t regions,size_t bytes,size_t reads,size_t spansPerRegion=5) {
        if(!mapsBytes||mapsBytes==SIZE_MAX||!mapsReads||
           mapsReads>SIZE_MAX/sizeof(ProcReadOriginal)||!regions||regions==SIZE_MAX||!reads||!bytes||
           regions>SIZE_MAX/sizeof(LinuxMappedOriginal)||reads>SIZE_MAX/sizeof(LinuxMemoryReadOriginal)||
            !spansPerRegion||regions>SIZE_MAX/spansPerRegion||regions*spansPerRegion==SIZE_MAX||
            regions*spansPerRegion>SIZE_MAX/sizeof(LinuxMemorySpanOriginal)||
            regions*spansPerRegion+1>SIZE_MAX/sizeof(LinuxMemoryThreadOriginal))
            throw std::invalid_argument("same-origin mapped-memory geometry");
        size_t total=sizeof(OriginalLinuxMappedMemory);
        const auto add=[&](size_t n) {
            if(n>SIZE_MAX-total)return false;total+=n;return true;
        };
        if(!add(mapsBytes+1)||!add(mapsReads*sizeof(ProcReadOriginal))||
           !add(regions*sizeof(LinuxMappedOriginal))||!add(reads*sizeof(LinuxMemoryReadOriginal))||
            !add(bytes)||!add(regions*spansPerRegion*sizeof(LinuxMemorySpanOriginal))||
            !add((regions*spansPerRegion+1)*sizeof(LinuxMemoryThreadOriginal)))
            throw std::invalid_argument("aggregate mapped-memory backing");
        return total;
    }
private:
    static size_t CheckedMapsBacking(OriginalDeadlineSupervisor& supervisor,OriginalDeadline& origin,
            size_t mapsBytes,size_t mapsReads,size_t regions,size_t bytes,size_t reads,size_t spansPerRegion) {
        (void)CheckedSourceBacking(supervisor,origin,mapsBytes,mapsReads,regions,bytes,reads,spansPerRegion);return mapsBytes;
    }
public:
    OriginalLinuxMappedMemory(HeldTaskIdentity& originalTask,OriginalDeadlineSupervisor& supervisor,
            OriginalDeadline& origin,size_t mapsBytes,size_t mapsReads,size_t regions,size_t bytes,size_t reads,size_t spansPerRegion=5)
        :task_(originalTask),supervisor_(supervisor),deadline_(origin),
          maps_(CheckedMapsBacking(supervisor,origin,mapsBytes,mapsReads,regions,bytes,reads,spansPerRegion),mapsReads),
         regionCapacity_(regions),readCapacity_(reads),byteCapacity_(bytes) {
        if(supervisor.OriginalDeadlineOwner()!=&origin||!regions||!reads||!bytes||
           regions>SIZE_MAX/sizeof(LinuxMappedOriginal)||reads>SIZE_MAX/sizeof(LinuxMemoryReadOriginal)||
           regions==SIZE_MAX||regions+1>SIZE_MAX/sizeof(LinuxMemoryThreadOriginal))
            throw std::invalid_argument("same-origin mapped-memory geometry");
        regions_=std::make_unique<LinuxMappedOriginal[]>(regions);reads_=std::make_unique<LinuxMemoryReadOriginal[]>(reads);
        bytes_=std::make_unique<unsigned char[]>(bytes);
        spanCapacity_=regions*spansPerRegion;spans_=std::make_unique<LinuxMemorySpanOriginal[]>(spanCapacity_);
        threadCapacity_=spanCapacity_+1;threads_=std::make_unique<LinuxMemoryThreadOriginal[]>(threadCapacity_);
    }
    OriginalLinuxMappedMemory(const OriginalLinuxMappedMemory&)=delete;
    OriginalLinuxMappedMemory& operator=(const OriginalLinuxMappedMemory&)=delete;
    bool CaptureMaps(const ProcTaskObservation& exactTaskObservation,const State& exactState,
            const Tracee& exactTracee,const Event& exactStoppedEvent) noexcept {
        if(mapsAttempted_)return false;mapsAttempted_=true;
        stoppedState_=&exactState;stoppedTracee_=&exactTracee;stoppedEvent_=&exactStoppedEvent;
        if(!SameRetainedStop()||exactTaskObservation.observedPid!=exactTracee.tid||
           exactTaskObservation.observedTracer!=exactState.originalCreatingThread.result||
           !task_.SameOriginal(exactTaskObservation)||task_.OriginalSupervisorOwner()!=&supervisor_||!task_.OriginalRootOwner()||
           task_.OriginalRootOwner()->OriginalDeadlineOwner()!=&deadline_||task_.OriginalDirectory().closeAttempted){failed_=true;return false;}
        if(!ObserveOriginalTracerThread()){failed_=true;return false;}
        const bool captured=maps_.Capture(task_.OriginalDirectory().original,"maps",deadline_,&supervisor_);
        mapsCaptured_=captured&&SameRetainedStop()&&ParseMaps();if(!mapsCaptured_)failed_=true;return mapsCaptured_;
    }
    bool CaptureWholeOwnedRegion(size_t ordinal) noexcept {
        if(ordinal>=regionCount_)return false;
        if(regions_[ordinal].captureAttempted)return false;
        const size_t before=spanCount_;
        const bool captured=CaptureOwnedMappedSpan(ordinal,regions_[ordinal].begin,regions_[ordinal].end);
        if(spanCount_==before+1&&spans_[before].mappingOrdinal==ordinal)regions_[ordinal]=spans_[before].captured;
        return captured;
    }
    bool CaptureOwnedMappedSpan(size_t ordinal,uint64_t begin,uint64_t end) noexcept {
        if(failed_||!mapsCaptured_||ordinal>=regionCount_)return false;
        if(!SameRetainedStop()){failed_=true;return false;}
        if(spanCount_==spanCapacity_){failed_=true;return false;}
        auto& span=spans_[spanCount_++];span.mappingOrdinal=ordinal;
        auto& region=span.captured;region=regions_[ordinal];region.captureAttempted=true;
        region.capturedBegin=begin;region.capturedEnd=end;
        for(size_t i=0;i+1<spanCount_;++i){const auto& prior=spans_[i].captured;
            if(prior.captureAttempted&&begin<prior.capturedEnd&&prior.capturedBegin<end){failed_=true;return false;}
        }
        if(begin>=end||begin<region.begin||end>region.end){region.shapeFailure=true;failed_=true;return false;}
        const uint64_t extent=end-begin;
        if(region.permissions[0]!='r'||extent>byteCapacity_-reservedExtent_||
           begin>static_cast<uint64_t>(std::numeric_limits<off_t>::max())||
           end-1>static_cast<uint64_t>(std::numeric_limits<off_t>::max())) {
            region.shapeFailure=true;failed_=true;return false;
        }
        region.destinationOffset=reservedExtent_;reservedExtent_+=static_cast<size_t>(extent);
        region.firstReadOrdinal=readCount_;
        if(!ObserveOriginalTracerThread()){failed_=true;return false;}
        if(!memory_.openAttempted) {
            if(!memoryOpenClock_.Before(&deadline_,"held-task-memory-open",false)) {failed_=true;return false;}
            if(!supervisor_.BeginNativeCall()) {memoryOpenClock_.nativeAttempted=false;failed_=true;return false;}
            memory_.openAttempted=true;
            memory_.openResult=openat(task_.OriginalDirectory().original,"mem",O_RDONLY|O_CLOEXEC|O_NOFOLLOW);
            memory_.original=memory_.openResult;if(memory_.openResult<0)memory_.openError=errno;
            supervisor_.OriginalNativeCallReturned();memoryOpenClock_.After(&deadline_,"held-task-memory-open");
        }
        if(memory_.original<0||memory_.closeAttempted||!memoryOpenClock_.clean){failed_=true;return false;}
        while(region.retainedBytes<extent) {
            if(!SameRetainedStop()){failed_=true;return false;}
            if(readCount_==readCapacity_){failed_=true;return false;}
            auto& row=reads_[readCount_++];++region.readCount;row.virtualAddress=begin+region.retainedBytes;
            row.destinationOffset=region.destinationOffset+region.retainedBytes;
            const uint64_t left=extent-region.retainedBytes;row.requested=left>65536?65536:static_cast<size_t>(left);
            if(!CaptureSupervisorClock(row.before,deadline_,"held-task-memory-pread",false)||!supervisor_.BeginNativeCall()) {failed_=true;return false;}
            row.attempted=true;row.returned=pread(memory_.original,bytes_.get()+row.destinationOffset,row.requested,static_cast<off_t>(row.virtualAddress));
            if(row.returned<0)row.error=errno;
            // Actual positive extent is retained BEFORE clock/shape decisions.
            const bool shape=row.returned>=0&&static_cast<uint64_t>(row.returned)<=row.requested;
            if(shape&&row.returned>0)region.retainedBytes+=static_cast<size_t>(row.returned);
            supervisor_.OriginalNativeCallReturned();
            const bool clockClean=CaptureSupervisorClock(row.after,deadline_,"held-task-memory-pread",true);
            if(!shape||row.returned<=0||!clockClean||row.after.nanoseconds<row.before.nanoseconds) {failed_=true;return false;}
        }
        region.captured=true;return true;
    }
    bool DisposeIndependentlyEligible() noexcept {
        // Same source-owned control thread calls after its synchronous native
        // calls returned. A failed/uncertain close is never retried.
        if(memory_.original>=0&&!memory_.closeAttempted) {
            if(!supervisor_.BeginIndependentlyEligibleCleanupNativeCall())return false;
            memoryCloseClock_.Before(&deadline_,"held-task-memory-once-close",true);
            // Do not enroll twice: this exact owner brackets the actual close,
            // then releases only after its raw return/error is retained.
            memory_.CloseOnce();
            supervisor_.OriginalNativeCallReturned();
            memoryCloseClock_.After(&deadline_,"held-task-memory-once-close");
        }
        const bool physical=(memory_.original<0||memory_.closed)&&
            (maps_.descriptor.original<0||maps_.descriptor.closed);
        if(!physical||(memory_.closeAttempted&&!memoryCloseClock_.clean))failed_=true;
        return physical;
    }
    const LinuxMemoryThreadOriginal* PrivateThreadOriginals() const noexcept{return threads_.get();}
    size_t ThreadOriginalCount() const noexcept{return threadCount_;}
    const State* OriginalStoppedState() const noexcept{return stoppedState_;}
    const Tracee* OriginalStoppedTracee() const noexcept{return stoppedTracee_;}
    const Event* OriginalStoppedEvent() const noexcept{return stoppedEvent_;}
    bool RetainedStopStillObserved() const noexcept{return SameRetainedStop();}
    const ProcFileObservation& OriginalMaps() const noexcept{return maps_;}
    const LinuxMappedOriginal* PrivateRegions() const noexcept{return regions_.get();}
    size_t RegionCount() const noexcept{return regionCount_;}
    const LinuxMemoryReadOriginal* PrivateReads() const noexcept{return reads_.get();}
    size_t ReadCount() const noexcept{return readCount_;}
    const unsigned char* PrivateBytes() const noexcept{return bytes_.get();}
    size_t ReservedByteExtent() const noexcept{return reservedExtent_;}
    const LinuxMemorySpanOriginal* PrivateSpans() const noexcept{return spans_.get();}
    size_t SpanCount() const noexcept{return spanCount_;}
    bool SameCapturedBytes(uint64_t address,const void* original,size_t extent) const noexcept {
        if(failed_||!mapsCaptured_||!SameRetainedStop()||(!original&&extent)||extent>UINT64_MAX-address)return false;
        const auto* expected=static_cast<const unsigned char*>(original);size_t compared=0;
        while(compared<extent){
            const uint64_t current=address+compared;const LinuxMappedOriginal* found=nullptr;
            for(size_t i=0;i<spanCount_;++i){const auto& row=spans_[i].captured;
                if(row.captured&&row.capturedBegin<=current&&current<row.capturedEnd){if(found)return false;found=&row;}
            }
            if(!found||found->capturedEnd<=found->capturedBegin||
               found->retainedBytes!=found->capturedEnd-found->capturedBegin)return false;
            const size_t offset=static_cast<size_t>(current-found->capturedBegin);
            const uint64_t available=found->capturedEnd-current;
            const size_t count=available<extent-compared?static_cast<size_t>(available):extent-compared;
            if(found->destinationOffset>byteCapacity_||offset>byteCapacity_-found->destinationOffset||
               count>byteCapacity_-found->destinationOffset-offset||
               std::memcmp(bytes_.get()+found->destinationOffset+offset,expected+compared,count))return false;
            compared+=count;
        }
        return true;
    }
    const ProcOwnedFd& OriginalMemoryDescriptor() const noexcept{return memory_;}
    bool ObserveCapturedZeroSpan(uint64_t address,size_t extent,bool& allZero) const noexcept {
        allZero=false;
        if(failed_||!mapsCaptured_||!SameRetainedStop()||extent>UINT64_MAX-address)return false;
        size_t compared=0;bool zero=true;
        while(compared<extent){
            const uint64_t current=address+compared;const LinuxMappedOriginal* found=nullptr;
            for(size_t i=0;i<spanCount_;++i){const auto& row=spans_[i].captured;
                if(row.captured&&row.capturedBegin<=current&&current<row.capturedEnd){if(found)return false;found=&row;}
            }
            if(!found||found->capturedEnd<=found->capturedBegin||
               found->retainedBytes!=found->capturedEnd-found->capturedBegin)return false;
            const size_t offset=static_cast<size_t>(current-found->capturedBegin);
            const uint64_t available=found->capturedEnd-current;
            const size_t count=available<extent-compared?static_cast<size_t>(available):extent-compared;
            if(found->destinationOffset>byteCapacity_||offset>byteCapacity_-found->destinationOffset||
               count>byteCapacity_-found->destinationOffset-offset)return false;
            for(size_t i=0;i<count;++i)if(bytes_[found->destinationOffset+offset+i])zero=false;
            compared+=count;
        }
        allZero=zero;return true; // Observation, not a kernel zero-fill contract.
    }
    bool Failed() const noexcept{return failed_;}
    bool ReservedSourceBytes(size_t& bytes) const noexcept {
        bytes=sizeof(*this);auto add=[&](size_t n){if(n>SIZE_MAX-bytes)return false;bytes+=n;return true;};
        return add(maps_.byteCapacity+1)&&add(maps_.readCapacity*sizeof(ProcReadOriginal))&&
            add(regionCapacity_*sizeof(LinuxMappedOriginal))&&add(readCapacity_*sizeof(LinuxMemoryReadOriginal))&&add(byteCapacity_)&&
            add(threadCapacity_*sizeof(LinuxMemoryThreadOriginal))&&add(spanCapacity_*sizeof(LinuxMemorySpanOriginal));
    }
    friend class OriginalLinuxPlatformState;
};
// Raw maps names are retained exactly: newline and " (deleted)" presentations
// are ambiguous and are NOT canonical file identity. Primary format/rights:
// https://man7.org/linux/man-pages/man5/proc_pid_maps.5.html
// https://man7.org/linux/man-pages/man5/proc_pid_mem.5.html
// Actual stopped-thread/process associations, mapping stability, loaded image
// role and physical/runtime fit remain complete native gate obligations.
}

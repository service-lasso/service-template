// UNCOMPILED source-owned raw member decoder. Package framing and authentic
// build/asset association are separate owners; this object grants neither.
#pragma once
#include <atomic>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <limits>
#include <memory>
#include <stdexcept>
#include <exception>
#include "zlib.h"
#pragma push_macro("local")
#include "zutil.h" // selected internal-layout prerequisites, including ZLIB_INTERNAL
#include "inftrees.h"
#include "inflate.h"
#pragma pop_macro("local")
#if ZLIB_VERNUM != 0x1310
#error Selected authentic zlib 1.3.1 headers are required
#endif
#if !defined(Z_SOLO) || !defined(NO_GZIP) || defined(BUILDFIXED) || defined(MAKEFIXED) || defined(ASMINF) || defined(ZLIB_DEBUG) || defined(PKZIP_BUG_WORKAROUND) || defined(INFLATE_ALLOW_INVALID_DISTANCE_TOOFAR_ARRR)
#error Selected private raw inflater configuration is required
#endif
namespace template_creator {
class OriginalRawMember {
public:
    struct Allocation {uInt items=0,size=0;std::size_t extent=0,offset=0;void* result=nullptr;bool attempted=false,freed=false;};
    struct Call {const Bytef* inputBefore=nullptr;Bytef* outputBefore=nullptr;const Bytef* inputAfter=nullptr;Bytef* outputAfter=nullptr;
        uInt inputAvailableBefore=0,outputAvailableBefore=0,inputAvailableAfter=0,outputAvailableAfter=0;
        int result=Z_STREAM_ERROR,dataType=0;const char* originalMessage=nullptr;bool attempted=false,returned=false;};
private:
    std::atomic_flag operation_=ATOMIC_FLAG_INIT;
    struct OperationRelease {std::atomic_flag& flag;~OperationRelease(){flag.clear(std::memory_order_release);}};
    z_stream stream_{},initializeBefore_{},initializeAfter_{},endBefore_{},endAfter_{};
    Allocation allocations_[2]{},firstOverflowAllocation_{};
    struct FreeCall {void* original=nullptr;std::size_t allocationOrdinal=SIZE_MAX;bool attempted=false,matched=false;};
    FreeCall frees_[2]{},firstOverflowFree_{};std::size_t freeCount_=0;
    std::size_t allocationCount_=0,arenaUsed_=0;
    static constexpr std::size_t Alignment=alignof(std::max_align_t);
    static constexpr std::size_t ArenaBytes=sizeof(inflate_state)+32768+2*Alignment;
    alignas(std::max_align_t) unsigned char arena_[ArenaBytes]{};
    std::unique_ptr<unsigned char[]> output_;
    std::unique_ptr<Call[]> calls_;
    const unsigned char* input_=nullptr;
    std::size_t inputExtent_=0,expected_=0,consumed_=0,produced_=0,callCount_=0,callCapacity_=0;
    int initializeResult_=Z_STREAM_ERROR,endResult_=Z_STREAM_ERROR;
    bool attempted_=false,initializeAttempted_=false,initializeReturned_=false,initialized_=false;
    bool endAttempted_=false,endReturned_=false,disposalAttempted_=false,failed_=false,complete_=false,shapeUnknown_=false;
    bool allocatorFailed_=false;
    bool preownAttempted_=false,preowned_=false;
    std::exception_ptr preownException_;
    static voidpf Allocate(voidpf opaque,uInt items,uInt size) noexcept {
        auto& self=*static_cast<OriginalRawMember*>(opaque);
        if(self.allocationCount_==2){
            if(!self.firstOverflowAllocation_.attempted){auto& row=self.firstOverflowAllocation_;row.attempted=true;row.items=items;row.size=size;}
            self.allocatorFailed_=true;return Z_NULL;
        }
        auto& row=self.allocations_[self.allocationCount_++];row.attempted=true;row.items=items;row.size=size;
        if(size&&items>std::numeric_limits<std::size_t>::max()/size){self.allocatorFailed_=true;return Z_NULL;}
        row.extent=static_cast<std::size_t>(items)*size;
        const auto remainder=self.arenaUsed_%Alignment;
        const auto padding=remainder?Alignment-remainder:0;
        if(padding>ArenaBytes-self.arenaUsed_){self.allocatorFailed_=true;return Z_NULL;}
        row.offset=self.arenaUsed_+padding;
        if(row.extent>ArenaBytes-row.offset){self.allocatorFailed_=true;return Z_NULL;}
        row.result=self.arena_+row.offset;self.arenaUsed_=row.offset+row.extent;return row.result;
    }
    static void Free(voidpf opaque,voidpf address) noexcept {
        auto& self=*static_cast<OriginalRawMember*>(opaque);
        if(self.freeCount_==2){
            if(!self.firstOverflowFree_.attempted){self.firstOverflowFree_.attempted=true;self.firstOverflowFree_.original=address;}
            self.allocatorFailed_=true;return;
        }
        auto& call=self.frees_[self.freeCount_++];call.attempted=true;call.original=address;
        for(std::size_t i=0;i<self.allocationCount_;++i){auto& row=self.allocations_[i];
            if(row.result==address&&address&&!row.freed){call.allocationOrdinal=i;call.matched=true;row.freed=true;return;}}
        self.allocatorFailed_=true; // no deallocation/reuse of uncertain backing
    }
    static std::size_t Geometry(std::size_t compressed,std::size_t expected) {
        const auto maximum=std::numeric_limits<std::size_t>::max();
        if(expected>maximum-sizeof(OriginalRawMember)-5||compressed>maximum-expected-5)throw std::invalid_argument("original member call geometry");
        const auto count=compressed+expected+5;
        if(count>(maximum-sizeof(OriginalRawMember)-expected-1)/sizeof(Call))throw std::invalid_argument("original member aggregate geometry");
        return count;
    }
public:
    static std::size_t ReservedSourceBytesFor(std::size_t compressed,std::size_t expected) {
        const auto count=Geometry(compressed,expected);
        return sizeof(OriginalRawMember)+expected+1+count*sizeof(Call);
    }
    OriginalRawMember(std::size_t compressedExtent,std::size_t expectedExtent)
        :inputExtent_(compressedExtent),expected_(expectedExtent) {
        stream_.zalloc=&Allocate;stream_.zfree=&Free;stream_.opaque=this;
    }
    bool PreownOriginal() noexcept {
        if(preownAttempted_||disposalAttempted_)return false;preownAttempted_=true;
        try {
            const auto count=Geometry(inputExtent_,expected_);
            output_=std::make_unique<unsigned char[]>(expected_+1);
            calls_=std::make_unique<Call[]>(count);callCapacity_=count;
            preowned_=true;return true;
        }catch(...){preownException_=std::current_exception();failed_=true;return false;}
    }
    const std::exception_ptr& PreownExceptionOriginal() const noexcept{return preownException_;}
    OriginalRawMember(const OriginalRawMember&)=delete;
    OriginalRawMember& operator=(const OriginalRawMember&)=delete;
    OriginalRawMember(OriginalRawMember&&)=delete;
    OriginalRawMember& operator=(OriginalRawMember&&)=delete;
    bool Capture(const unsigned char* original,std::size_t extent) noexcept {
        if(operation_.test_and_set(std::memory_order_acquire))return false;
        OperationRelease release{operation_};
        if(attempted_||disposalAttempted_)return false;attempted_=true;input_=original;
        if(!preowned_||(!original&&extent)||extent!=inputExtent_){failed_=true;return false;}
        initializeBefore_=stream_;initializeAttempted_=true;initializeResult_=inflateInit2(&stream_,-15);initializeAfter_=stream_;initializeReturned_=true;initialized_=initializeResult_==Z_OK;
        if(!initialized_){failed_=true;return false;}
        while(callCount_<callCapacity_){
            auto& row=calls_[callCount_++];
            const auto remainingInput=inputExtent_-consumed_;
            const auto remainingOutput=expected_+1-produced_;
            stream_.next_in=const_cast<Bytef*>(original?original+consumed_:nullptr);
            stream_.avail_in=static_cast<uInt>(remainingInput>65536?65536:remainingInput);
            stream_.next_out=output_.get()+produced_;
            stream_.avail_out=static_cast<uInt>(remainingOutput>65536?65536:remainingOutput);
            row.inputBefore=stream_.next_in;row.outputBefore=stream_.next_out;
            row.inputAvailableBefore=stream_.avail_in;row.outputAvailableBefore=stream_.avail_out;row.attempted=true;
            row.result=inflate(&stream_,Z_NO_FLUSH);row.returned=true;
            row.inputAfter=stream_.next_in;row.outputAfter=stream_.next_out;
            row.inputAvailableAfter=stream_.avail_in;row.outputAvailableAfter=stream_.avail_out;
            row.dataType=stream_.data_type;row.originalMessage=stream_.msg;
            if(row.inputAvailableAfter>row.inputAvailableBefore||row.outputAvailableAfter>row.outputAvailableBefore){shapeUnknown_=failed_=true;return false;}
            const auto inputDelta=row.inputAvailableBefore-row.inputAvailableAfter;
            const auto outputDelta=row.outputAvailableBefore-row.outputAvailableAfter;
            if(row.inputAfter!=(row.inputBefore?row.inputBefore+inputDelta:nullptr)||row.outputAfter!=row.outputBefore+outputDelta){shapeUnknown_=failed_=true;return false;}
            consumed_+=inputDelta;produced_+=outputDelta;
            if(produced_>expected_||allocatorFailed_){failed_=true;return false;}
            if(row.result==Z_STREAM_END){complete_=consumed_==inputExtent_&&produced_==expected_;failed_=!complete_;return complete_;}
            if(row.result!=Z_OK||(!inputDelta&&!outputDelta)){failed_=true;return false;}
        }
        failed_=true;return false;
    }
    bool DisposeDecoderEligible() noexcept {
        // Acquiring this source operation lock excludes the entire synchronous
        // Capture body, including allocator callbacks and all returned snapshots.
        if(operation_.test_and_set(std::memory_order_acquire))return false;
        OperationRelease release{operation_};
        disposalAttempted_=true; // absorbs future Capture even with no library state
        if(shapeUnknown_||(initializeAttempted_&&!initializeReturned_))return false;
        for(std::size_t i=0;i<callCount_;++i)if(calls_[i].attempted&&!calls_[i].returned)return false;
        if(!initialized_){
            for(std::size_t i=0;i<allocationCount_;++i)if(allocations_[i].result&&!allocations_[i].freed)return false;
            return !allocatorFailed_;
        }
        if(endAttempted_)return endReturned_&&endResult_==Z_OK&&!allocatorFailed_;
        endBefore_=stream_;endAttempted_=true;endResult_=inflateEnd(&stream_);endAfter_=stream_;endReturned_=true;
        if(endResult_!=Z_OK||allocatorFailed_)failed_=true;
        return endResult_==Z_OK&&!allocatorFailed_;
    }
    // Caller retains object/input/output and reads these views only at its
    // quiescent synchronous creator-thread cut. The source operation lock is
    // not native thread termination, a worker join, or output borrower release.
    bool CompleteOriginal() const noexcept{return complete_&&!failed_&&!shapeUnknown_;}
    const unsigned char* PrivateOutput() const noexcept{return output_.get();}
    std::size_t RetainedOutputExtent() const noexcept{return produced_;}
    std::size_t PrivateOutputCapacity() const noexcept{return output_?expected_+1:0;}
    const Call* PrivateCalls() const noexcept{return calls_.get();}
    std::size_t CallCount() const noexcept{return callCount_;}
    std::size_t CallCapacity() const noexcept{return calls_?callCapacity_:0;}
    const Allocation* PrivateAllocations() const noexcept{return allocations_;}
    std::size_t ReservedSourceBytes() const noexcept{return sizeof(*this)+PrivateOutputCapacity()+CallCapacity()*sizeof(Call);}
};
}

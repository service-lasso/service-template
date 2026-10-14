// UNCOMPILED original-deadline observations. No timeout is created for a
// command that has none; expiration denies further qualification effects.
#pragma once
#include <time.h>
#include <cstdint>
#include <memory>
#include <stdexcept>
#include <cerrno>
#include <exception>
namespace template_creator_linux {
inline bool OriginalBackingStorageSeparate(const void* output,size_t outputBytes,const void* backing,size_t count,size_t width=1) noexcept {
    if(!output||outputBytes>UINTPTR_MAX-reinterpret_cast<uintptr_t>(output)||
       (width&&count>SIZE_MAX/width))return false;
    const size_t bytes=count*width;if(!bytes)return true;if(!backing)return false;
    const auto a=reinterpret_cast<uintptr_t>(output),b=reinterpret_cast<uintptr_t>(backing);
    if(bytes>UINTPTR_MAX-b)return false;return a+outputBytes<=b||b+bytes<=a;
}
struct DeferredOriginalPreownTag {explicit constexpr DeferredOriginalPreownTag() noexcept=default;};
inline constexpr DeferredOriginalPreownTag DeferredOriginalPreown{};
struct OriginalClockBoundary {
    const char* operation = nullptr;
    bool after = false, attempted = false;
    int returned = -1, error = 0;
    struct timespec output{};
    uint64_t nanoseconds = 0;
    bool shapeValid = false, expired = false;
};
class OriginalDeadline {
    std::unique_ptr<OriginalClockBoundary[]> rows_;
    size_t capacity_ = 0, count_ = 0;
    bool hasDeadline_ = false, failed_ = false, expired_ = false;
    uint64_t originalAbsoluteNs_ = 0, latestNs_ = 0;
    size_t selectedRows_=0;
    bool preownAttempted_=false,preowned_=false;
    std::exception_ptr preownException_;
public:
    OriginalDeadline(DeferredOriginalPreownTag,size_t selectedBoundaries,bool hasOriginalDeadline,uint64_t originalAbsoluteNs) noexcept
        :hasDeadline_(hasOriginalDeadline),originalAbsoluteNs_(originalAbsoluteNs),selectedRows_(selectedBoundaries) {}
    OriginalDeadline(size_t selectedBoundaries, bool hasOriginalDeadline, uint64_t originalAbsoluteNs)
        :OriginalDeadline(DeferredOriginalPreown,selectedBoundaries,hasOriginalDeadline,originalAbsoluteNs) {
        if(!PreownOriginal())std::rethrow_exception(preownException_);
    }
    bool PreownOriginal() noexcept {
        if(preownAttempted_)return false;preownAttempted_=true;
        try {
            if(!selectedRows_||selectedRows_>(SIZE_MAX-sizeof(*this))/sizeof(OriginalClockBoundary)||
               (hasDeadline_&&!originalAbsoluteNs_))throw std::invalid_argument("original clock roster");
            rows_=std::make_unique<OriginalClockBoundary[]>(selectedRows_);capacity_=selectedRows_;
            preowned_=true;return true;
        }catch(...){preownException_=std::current_exception();failed_=true;return false;}
    }
    OriginalDeadline(const OriginalDeadline&) = delete;
    OriginalDeadline& operator=(const OriginalDeadline&) = delete;
    bool Observe(const char* operation, bool after) noexcept {
        if(!preowned_){failed_=true;return false;}
        if (count_ == capacity_) { failed_ = true; return false; }
        auto& row = rows_[count_++]; row.operation = operation; row.after = after; row.attempted = true;
        row.returned = clock_gettime(CLOCK_MONOTONIC,&row.output);
        if (row.returned != 0) {
            if (row.returned < 0) row.error = errno;
            failed_ = true; return false;
        }
        if (row.output.tv_sec < 0 || row.output.tv_nsec < 0 || row.output.tv_nsec >= 1000000000 ||
            static_cast<uint64_t>(row.output.tv_sec) > (UINT64_MAX-static_cast<uint64_t>(row.output.tv_nsec))/1000000000) {
            failed_ = true; return false;
        }
        row.nanoseconds = static_cast<uint64_t>(row.output.tv_sec)*1000000000+static_cast<uint64_t>(row.output.tv_nsec);
        row.shapeValid = true;
        if (count_ > 1 && row.nanoseconds < latestNs_) failed_ = true;
        latestNs_ = row.nanoseconds;
        row.expired = hasDeadline_ && row.nanoseconds >= originalAbsoluteNs_;
        expired_ |= row.expired;
        return !failed_ && !expired_;
    }
    bool QualificationClean() const noexcept { return preowned_ && !failed_ && !expired_; }
    bool PreownedOriginal() const noexcept{return preowned_;}
    const std::exception_ptr& PreownExceptionOriginal() const noexcept{return preownException_;}
    size_t OriginalCapacity() const noexcept{return rows_?capacity_:0;}
    bool HasOriginalDeadline() const noexcept { return hasDeadline_; }
    uint64_t OriginalAbsoluteNanoseconds() const noexcept { return originalAbsoluteNs_; }
    const OriginalClockBoundary* PrivateOriginals() const noexcept { return rows_.get(); }
    size_t OriginalCount() const noexcept { return count_; }
    size_t ReservedSourceBytes() const noexcept { return sizeof(*this)+capacity_*sizeof(OriginalClockBoundary); }
};
}

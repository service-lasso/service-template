#pragma once
#include <cstddef>
#include <cstdint>
#include <memory>
namespace template_creator {
inline bool OriginalRetainedStorageSeparate(const void* output,std::size_t bytes,const void* storage,std::size_t count,std::size_t width=1) noexcept {
    if(!output||bytes>UINTPTR_MAX-reinterpret_cast<std::uintptr_t>(output)||(width&&count>SIZE_MAX/width))return false;
    const auto extent=count*width;if(!extent)return true;if(!storage)return false;
    const auto a=reinterpret_cast<std::uintptr_t>(output),b=reinterpret_cast<std::uintptr_t>(storage);
    return extent<=UINTPTR_MAX-b&&(a+bytes<=b||b+extent<=a);
}
template<class T> bool OriginalRetainedArraySeparate(const void* output,std::size_t bytes,const std::unique_ptr<T[]>& storage,std::size_t capacity) noexcept {
    return OriginalRetainedStorageSeparate(output,bytes,storage.get(),storage?capacity:0,sizeof(T));
}
template<class T> bool OriginalRetainedOwnerSeparate(const void* output,std::size_t bytes,const std::unique_ptr<T>& owner) noexcept {
    return !owner||owner->OutputBackingSeparateOriginal(output,bytes);
}
}

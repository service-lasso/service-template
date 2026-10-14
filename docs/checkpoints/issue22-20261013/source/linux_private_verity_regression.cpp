// UNRUN native result assertions. Caller must supply actually executed originals;
// this file neither enables filesystem features nor manufactures native results.
#include "linux_private_verity.hpp"
namespace template_creator_linux {
bool VerifyActualPrivateVerityOriginal(const OriginalPrivateVerity& owner) noexcept {
    if(!owner.EnableNativeAttempted() || !owner.ContentSealedAndReadback() || owner.EnableReturned()!=0 ||
       owner.EnableError()!=0 || !owner.OriginalFile().Produced()) return false;
    const auto& input=owner.OriginalEnable();
    if(input.version!=1 || input.hash_algorithm!=FS_VERITY_HASH_ALG_SHA256 ||
       input.block_size<1024 || (input.block_size&(input.block_size-1)) ||
       input.salt_size || input.salt_ptr || input.sig_size || input.sig_ptr || input.__reserved1)
        return false;
    for(const auto value:input.__reserved2) if(value) return false;
    const size_t size=owner.OriginalFile().SourceByteCount();
    if(owner.RetainedExtent()!=size || !owner.ReadCount()) return false;
    size_t position=0;
    for(size_t i=0;i<owner.ReadCount();++i) {
        const auto& row=owner.PrivateReads()[i];
        if(!row.attempted || row.offset!=position || row.returned<0 || row.error ||
            static_cast<size_t>(row.returned)>row.requested) return false;
        const bool last=i+1==owner.ReadCount();
        if(last) {
            if(position!=size || row.requested!=1 || row.returned!=0) return false;
        } else {
            if(!row.requested || row.requested>65536 || row.returned==0 ||
               static_cast<size_t>(row.returned)>size-position) return false;
            position+=static_cast<size_t>(row.returned);
        }
    }
    return !size || std::memcmp(owner.PrivateObservedBytes(),
        owner.OriginalFile().PrivateSourceBytes(),size)==0;
}
// A failed actual ioctl is a retained unsupported/failed route. This assertion
// intentionally grants neither content seal nor permission to continue launch.
bool VerifyActualPrivateVerityDenied(const OriginalPrivateVerity& owner) noexcept {
    return owner.EnableNativeAttempted() && owner.EnableReturned()<0 && owner.EnableError()!=0 &&
        !owner.ContentSealedAndReadback() && owner.ReadCount()==0;
}
}

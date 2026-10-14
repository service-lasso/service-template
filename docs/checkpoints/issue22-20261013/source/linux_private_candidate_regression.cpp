// UNRUN assertions over actual private-producer results; no surrogate syscalls.
#include "linux_private_candidate_file.hpp"
namespace template_creator_linux {
bool VerifyActualPrivateCandidateProduced(const OriginalPrivateCandidateFile& owner) noexcept {
    if(!owner.Produced())return false;
    const auto& writer=owner.OriginalWriter();const auto& reader=owner.OriginalReader();
    const auto& birth=owner.OriginalBirthStat();const auto& reopened=owner.OriginalReaderStat();
    if(!writer.attempted || writer.returned<0 || writer.error ||
       !writer.closeAttempted || !writer.closed || writer.closeReturned || writer.closeError ||
       !reader.attempted || reader.returned<0 || reader.error || reader.closeAttempted ||
       !S_ISREG(birth.st_mode) || !S_ISREG(reopened.st_mode) || birth.st_size!=0 ||
       birth.st_dev!=reopened.st_dev || birth.st_ino!=reopened.st_ino || reopened.st_size<0 ||
       static_cast<uintmax_t>(reopened.st_size)!=owner.SourceByteCount() ||
       (birth.st_mode&07777)!=owner.OriginalCreateMode() ||
       (reopened.st_mode&07777)!=owner.OriginalCreateMode())return false;
    if(owner.OriginalCreateMode()!=0600 && owner.OriginalCreateMode()!=0700)return false;
    size_t position=0;
    for(size_t i=0;i<owner.WriteCount();++i) {
        const auto& row=owner.PrivateWrites()[i];
        if(!row.attempted || row.offset!=position || !row.requested || row.requested>65536 ||
            row.returned<=0 || row.error || static_cast<size_t>(row.returned)>row.requested ||
            static_cast<size_t>(row.returned)>owner.SourceByteCount()-position)return false;
        position+=static_cast<size_t>(row.returned);
    }
    return position==owner.SourceByteCount();
}
// Creation/mode/write-result assertions do not establish content seal, root
// exclusion or executable/loader/source authority; those have distinct joins.
}

// UNRUN assertions over actual admitted native sink outputs; no injected rows.
#include "linux_native_sink.hpp"
namespace template_creator_linux {
bool VerifyActualRepeatedSinkReadback(const OriginalNativeSink& owner) noexcept {
    if(!owner.Qualified()||!owner.OriginalWrittenExtent())return false;
    size_t actualWrites=0,actualReads=0,eofCalls=0,syncCalls=0;
    const auto* rows=owner.PrivateOriginalCalls();
    for(size_t i=0;i<owner.PrivateOriginalCallCount();++i){
        const auto& row=rows[i];if(!row.attempted||row.error||row.returned<0)return false;
        if(row.operation==SinkCall::Write){
            if(!row.requested||static_cast<size_t>(row.returned)>row.requested||
               row.destinationOffset!=actualWrites)return false;
            actualWrites+=static_cast<size_t>(row.returned);
        }else if(row.operation==SinkCall::Read){
            if(!row.requested||row.returned<=0||static_cast<size_t>(row.returned)>row.requested||
               row.destinationOffset!=actualReads)return false;
            actualReads+=static_cast<size_t>(row.returned);
        }else if(row.operation==SinkCall::Eof){if(row.returned)return false;++eofCalls;}
        else if(row.operation==SinkCall::Sync){if(row.returned)return false;++syncCalls;}
    }
    return eofCalls>=2&&syncCalls>=2&&actualWrites==owner.OriginalWrittenExtent()&&
        actualReads==owner.OriginalReadbackExtent()&&owner.OriginalCopiedExtent()==actualWrites;
}
}
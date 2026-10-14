// UNRUN source fixtures for the selected authentic linked inflater. These are
// not installed runtime, package provenance or native acceptance evidence.
#include "windows_original_raw_member.hpp"
namespace template_creator {
bool OriginalRawMemberSourceRegression() {
    // RFC1951 fixed-Huffman empty final block, followed by byte padding.
    const unsigned char empty[]={0x03,0x00};
    OriginalRawMember closedBeforeStart(sizeof(empty),0);
    if(!closedBeforeStart.DisposeDecoderEligible()||closedBeforeStart.Capture(empty,sizeof(empty))||
        closedBeforeStart.CallCount()!=0||closedBeforeStart.CompleteOriginal())return false;
    OriginalRawMember clean(sizeof(empty),0);
    if(clean.PrivateOutput()||clean.PrivateCalls()||clean.PrivateOutputCapacity()||clean.CallCapacity()||
        !clean.PreownOriginal()||clean.PreownOriginal()||!clean.Capture(empty,sizeof(empty))||!clean.CompleteOriginal()||clean.RetainedOutputExtent()!=0||
        !clean.DisposeDecoderEligible()||!clean.DisposeDecoderEligible()||clean.Capture(empty,sizeof(empty)))return false;
    for(std::size_t i=0;i<clean.CallCount();++i)
        if(!clean.PrivateCalls()[i].attempted||!clean.PrivateCalls()[i].returned)return false;
    const unsigned char trailing[]={0x03,0x00,0x00};
    OriginalRawMember excess(sizeof(trailing),0);
    if(!excess.PreownOriginal()||excess.Capture(trailing,sizeof(trailing))||excess.CompleteOriginal()||!excess.DisposeDecoderEligible())return false;
    const unsigned char invalid[]={0x07}; // reserved BTYPE3
    OriginalRawMember malformed(sizeof(invalid),0);
    if(!malformed.PreownOriginal()||malformed.Capture(invalid,sizeof(invalid))||malformed.CompleteOriginal()||!malformed.DisposeDecoderEligible())return false;
    OriginalRawMember invalidGeometry(SIZE_MAX,0);
    if(invalidGeometry.PreownOriginal()||!invalidGeometry.PreownExceptionOriginal()||
        invalidGeometry.PrivateOutput()||invalidGeometry.PrivateCalls()||
        invalidGeometry.ReservedSourceBytes()!=sizeof(invalidGeometry))return false;
    return true;
}
}
#ifdef TEMPLATE_RAW_MEMBER_SYNTHETIC_REGRESSION
int main(){return template_creator::OriginalRawMemberSourceRegression()?0:1;}
#endif

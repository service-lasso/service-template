// UNRUN callable source regressions, not an invoked native harness or authority.
#ifndef _WIN32
#include "linux_private_selection_startup.hpp"
namespace template_creator_bridge {
bool RegressionLinuxStartupClosureBeforeBegin(const OriginalLinuxSelectionStartupInput& original){
    auto* startup=new OriginalLinuxSelectionStartup();
    if(!startup->PreownOriginal(original)||startup->hostBeginAttempted||startup->captureAttempted)return false;
    // Real class closure on actual preowned, never-started owners. No supplied
    // descriptor is consumed, duplicated, closed or represented as authority.
    if(!startup->ObserveIndependentlyEligibleClosure()||!startup->dispositionRequested)return false;
    if(startup->BeginOriginal()||startup->PreownOriginal(original)||startup->hostBeginAttempted||startup->captureAttempted)
        return false;
    if(!startup->decoder||startup->decoder->DecodeOriginal()||startup->decoder->BeginDecodedOriginal())return false;
    return startup->ObserveIndependentlyEligibleClosure();
}
bool RegressionLinuxDecodedSelectionClosureBeforeInvoke(
    template_creator_linux::OriginalLinuxHeldFile& actualCapturedSelection,
    int originalProcRoot,const int* actualCapabilities,std::size_t count){
    if(!actualCapturedSelection.CapturedSourceBytes())return false;
    auto* decoder=new OriginalLinuxSelectionDecoder(actualCapturedSelection,originalProcRoot,actualCapabilities,count);
    // Actual full positive-format capture is required before exercising denial;
    // an invalid/uncaptured fixture cannot satisfy this regression accidentally.
    if(!decoder->DecodeOriginal()||!decoder->Decoded()||decoder->PrivateInvocation())return false;
    if(!decoder->ObservePrePlatformInputEligibility())return false;
    if(decoder->BeginDecodedOriginal()||decoder->PrivateInvocation())return false;
    if(!decoder->ObserveIndependentlyEligibleClosure())return false;
    // Decoder owns copies only. SAME original file/capabilities stay with their
    // real enclosing owner; this function does not dispose a borrowed input.
    return actualCapturedSelection.CapturedSourceBytes();
}
}
#endif

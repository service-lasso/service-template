// UNRUN supplied-byte decoder contract, not CPU/native/CALL/build proof.
#ifdef _WIN32
#include "windows_amd64_caller_boundary.hpp"
namespace template_creator::regression {
bool Amd64SelectedOpcodeExtensionsOriginal() {
    const unsigned char nop[]={0x0f,0x1f,0x00};
    const unsigned char reservedNop[]={0x0f,0x1f,0xc8};
    const unsigned char shift[]={0xc1,0xe0,0x01};
    const unsigned char unselectedShift[]={0xc1,0xf0,0x01};
    const unsigned char setccIgnoredReg[]={0x0f,0x94,0xc8};
    const unsigned char reservedTest[]={0xf7,0xc8,0,0,0,0};
    const unsigned char btRegister[]={0x4d,0x0f,0xa3,0xd3};
    const unsigned char btMemory[]={0x4d,0x0f,0xa3,0x13};
    const unsigned char btrRegister[]={0x49,0x0f,0xba,0xf2,0x00};
    const unsigned char baOtherExtension[]={0x49,0x0f,0xba,0xea,0x00};
    const unsigned char btrMemory[]={0x49,0x0f,0xba,0x32,0x00};
    const unsigned char btrTruncated[]={0x49,0x0f,0xba,0xf2};
    const unsigned char btTruncated[]={0x4d,0x0f,0xa3};
    const unsigned char btrExtendedGroup[]={0x4d,0x0f,0xba,0xf2,0x00};
    OriginalAmd64Instruction bt{},btBad{},btr{},baBad{},btrBad{},truncated{},btShort{},extendedGroup{};
    OriginalAmd64Instruction positive{},negative{},selectedShift{},negativeShift{},setcc{},negativeTest{};
    return OriginalAmd64CallerBoundary::DecodeSuppliedInstructionContract(nop,sizeof(nop),positive) && positive.length==3 &&
        !OriginalAmd64CallerBoundary::DecodeSuppliedInstructionContract(reservedNop,sizeof(reservedNop),negative) &&
        OriginalAmd64CallerBoundary::DecodeSuppliedInstructionContract(shift,sizeof(shift),selectedShift) && selectedShift.length==3 &&
        !OriginalAmd64CallerBoundary::DecodeSuppliedInstructionContract(unselectedShift,sizeof(unselectedShift),negativeShift) &&
        OriginalAmd64CallerBoundary::DecodeSuppliedInstructionContract(setccIgnoredReg,sizeof(setccIgnoredReg),setcc) && setcc.length==3 &&
        !OriginalAmd64CallerBoundary::DecodeSuppliedInstructionContract(reservedTest,sizeof(reservedTest),negativeTest) &&
        OriginalAmd64CallerBoundary::DecodeSuppliedInstructionContract(btRegister,sizeof(btRegister),bt) &&
        bt.length==4 && bt.flow==OriginalAmd64Instruction::Flow::Next && bt.registerOperand &&
        !OriginalAmd64CallerBoundary::DecodeSuppliedInstructionContract(btMemory,sizeof(btMemory),btBad) &&
        OriginalAmd64CallerBoundary::DecodeSuppliedInstructionContract(btrRegister,sizeof(btrRegister),btr) &&
        btr.length==5 && btr.flow==OriginalAmd64Instruction::Flow::Next && btr.registerOperand &&
        !OriginalAmd64CallerBoundary::DecodeSuppliedInstructionContract(baOtherExtension,sizeof(baOtherExtension),baBad) &&
        !OriginalAmd64CallerBoundary::DecodeSuppliedInstructionContract(btrMemory,sizeof(btrMemory),btrBad) &&
        !OriginalAmd64CallerBoundary::DecodeSuppliedInstructionContract(btrTruncated,sizeof(btrTruncated),truncated) &&
        !OriginalAmd64CallerBoundary::DecodeSuppliedInstructionContract(btTruncated,sizeof(btTruncated),btShort) &&
        !OriginalAmd64CallerBoundary::DecodeSuppliedInstructionContract(btrExtendedGroup,sizeof(btrExtendedGroup),extendedGroup);
}
}
#endif

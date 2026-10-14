// UNCOMPILED/UNRUN native scenarios. Admitted harness supplies its actual private
// drive-backed output directory and a separate actual private regular file.
// This object stays resident through all acquired/unknown original custody.
#pragma once
#include "windows_private_fixture_root.hpp"
namespace template_creator {
struct OriginalFixtureRootRegression {
    OriginalWindowsFixtureRoot accepted,wrongParent,invalidParent;
    bool attempted=false,acceptedReturned=false,wrongReturned=false,invalidReturned=false;
    bool captureChecks=false,acceptedDisposed=false,wrongDisposed=false,invalidDisposed=false;
    bool Observe(HANDLE actualPrivateDirectory,HANDLE actualPrivateRegularFile) noexcept {
        if(attempted)return false;attempted=true;
        acceptedReturned=accepted.Create(actualPrivateDirectory);
        wrongReturned=wrongParent.Create(actualPrivateRegularFile);
        invalidReturned=invalidParent.Create(INVALID_HANDLE_VALUE);
        captureChecks=acceptedReturned&&accepted.created&&accepted.createAttempted&&
            accepted.createReturned==0&&accepted.originalStatus.Status==0&&accepted.originalStatus.Information==2&&
            accepted.rootAcquired&&!accepted.rootUnknown&&accepted.environmentPathBound&&
            accepted.environmentAcquired&&accepted.environmentCloseAttempted&&accepted.environmentCloseReturned&&
            accepted.environmentIdentity.captured&&accepted.rootIdentity.captured&&
            accepted.environmentIdentity.id.VolumeSerialNumber==accepted.rootIdentity.id.VolumeSerialNumber&&
            memcmp(accepted.environmentIdentity.id.FileId.Identifier,accepted.rootIdentity.id.FileId.Identifier,
                sizeof(accepted.rootIdentity.id.FileId.Identifier))==0&&
            accepted.environmentPathUnits==accepted.dosPathReturned-4&&
            !wrongReturned&&wrongParent.parentAcquired&&wrongParent.parentIdentity.captured&&
            !wrongParent.parentIdentity.standard.Directory&&!wrongParent.createAttempted&&
            !wrongParent.rootAcquired&&!invalidReturned&&!invalidParent.parentAcquired&&
            !invalidParent.createAttempted&&!invalidParent.rootAcquired;
        // No product consumer is launched by this scenario. All real native
        // acquisition/query calls have returned before these independent closes.
        acceptedDisposed=accepted.DisposeAfterConsumersAndFixtureCleanup();
        wrongDisposed=wrongParent.DisposeAfterConsumersAndFixtureCleanup();
        invalidDisposed=invalidParent.DisposeAfterConsumersAndFixtureCleanup();
        return captureChecks&&acceptedDisposed&&accepted.RemovalSucceeded()&&
            wrongDisposed&&wrongParent.parentCloseAttempted&&wrongParent.parentCloseReturned&&
            !wrongParent.dispositionAttempted&&invalidDisposed&&!invalidParent.dispositionAttempted;
    }
};
// No constructed native rows, fake handle, injected success or process-borrower
// proof is used. Unsupported actual rights/path/filesystem produces failure;
// the fixture does not provision, retry, delete recursively or claim execution.
}

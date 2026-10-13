// UNRUN native scenario continuations, not fabricated-state native proof.
// The admitted harness must supply an actual source owner at the stated failed
// boundary. These functions perform the real eligible close and verify that a
// second cleanup request preserves its exact original result without retry.
#ifdef _WIN32
#include "windows_original_main.hpp"
namespace template_creator_bridge {
static bool CloseActualUnconnectedOriginal(OriginalWindowsNativeMain& main) noexcept {
    if(!main.beginAttempted || !main.duplicateReturned || !main.threadAcquired ||
        main.threadUnknown || main.connectAttempted || main.creatorStarted ||
        main.receiptStartAttempted || main.threadCloseAttempted) return false;
    const auto birthReturned=main.birthReturned;
    const auto birthError=main.birthError;
    const auto observations=main.observationCount;
    const auto exhausted=main.observationExhausted;
    const bool first=main.CloseUnconnectedThreadHandleEligible();
    const auto closeReturned=main.threadCloseReturned;
    const auto closeError=main.threadCloseError;
    const bool second=main.CloseUnconnectedThreadHandleEligible();
    return main.mainHandleDisposalAttempted && main.threadCloseAttempted &&
        first==(closeReturned!=FALSE) && second==first &&
        main.threadCloseReturned==closeReturned && main.threadCloseError==closeError &&
        main.birthReturned==birthReturned && main.birthError==birthError &&
        main.observationCount==observations && main.observationExhausted==exhausted;
}
bool ExerciseActualMainBirthFailureCleanup(OriginalWindowsNativeMain& main) noexcept {
    if(main.birthReturned || !main.beginAttempted || main.initialized) return false;
    return CloseActualUnconnectedOriginal(main);
}
bool ExerciseActualMainObservationExhaustionCleanup(OriginalWindowsNativeMain& main) noexcept {
    if(!main.observationExhausted || main.observationCount!=main.observationCapacity) return false;
    return CloseActualUnconnectedOriginal(main);
}
bool ExerciseActualReceiptNoWorkerCleanup(OriginalWindowsNativeMain& main) noexcept {
    if(!main.receipt || !main.receiptStartAttempted || !main.receipt->NoWorkerCreated() ||
        main.receiptStartReturned || main.receipt->Published() || main.threadCloseAttempted ||
        !main.host || main.receiptHostProgressOrdinal>=main.host->progressCount ||
        !main.host->progress[main.receiptHostProgressOrdinal].physicalDisposed) return false;
    const auto createError=main.receipt->createError;
    const bool first=main.CloseCreatingThreadHandleEligible();
    const auto returned=main.threadCloseReturned;
    const auto error=main.threadCloseError;
    const bool second=main.CloseCreatingThreadHandleEligible();
    const auto& outcome=main.PublishRetainedSourceOutcome();
    return main.threadCloseAttempted && first==(returned!=FALSE) && second==first &&
        main.threadCloseReturned==returned && main.threadCloseError==error &&
        main.receipt->createError==createError && main.receipt->NoWorkerCreated() &&
        !main.receipt->terminal && !main.receipt->Published() &&
        outcome.published && outcome.sourceQualificationFailed;
}
bool ExerciseActualReceiptUnknownCreationRetainsMain(OriginalWindowsNativeMain& main) noexcept {
    if(!main.receipt || !main.receiptStartAttempted || main.receiptStartReturned ||
        !main.receipt->createCompleted || !main.receipt->creationUnknown ||
        main.threadCloseAttempted || main.receipt->closeAttempted) return false;
    const auto handle=main.receipt->OriginalWorkerHandle();
    const auto id=main.receipt->createdThreadId;
    const auto error=main.receipt->createError;
    const bool closed=main.CloseCreatingThreadHandleEligible();
    const auto& outcome=main.PublishRetainedSourceOutcome();
    return !closed && !main.threadCloseAttempted && !outcome.published &&
        !main.receipt->NoWorkerCreated() && !main.receipt->PhysicallyDisposed() &&
        main.receipt->creationUnknown && !main.receipt->closeAttempted &&
        main.receipt->OriginalWorkerHandle()==handle && main.receipt->createdThreadId==id &&
        main.receipt->createError==error;
}
// No harness currently produces the original failed GetThreadTimes/CreateThread
// outcomes here. No scenario is credited until its actual admitted producer and
// native originals are supplied; changing owner fields is not such a producer.
}
#endif

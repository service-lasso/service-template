// UNCOMPILED source-owned Linux command progression. Gate admission and measured
// physical fit are separate requirements; this owner never manufactures either.
#pragma once
#include "linux_original_deadline.hpp"
#include "linux_driver.hpp"
#include "native_linux_command_plan.hpp"
#include <stdexcept>
namespace template_creator_linux {
struct OriginalRouteCommand {
    template_creator_bridge::LinuxOriginalCommand plan;
    std::unique_ptr<FiniteCommandDriver> driver;
    Gate* originalGate = nullptr;
    int originalEmptyStdin = -1;
    int originalDirectory = -1;
    bool bindingAttempted = false, bound = false, beginAttempted = false;
    bool beginReturned = false, advanceAttempted = false, advanced = false;
    DriverObservation last{};
    std::exception_ptr originalException;
};
class OriginalCommandRoute {
    std::unique_ptr<std::unique_ptr<OriginalRouteCommand>[]> commands_;
    size_t capacity_ = 0, current_ = 0, boundCount_ = 0;
    size_t selectedCommands_=0,bornCount_=0;
    bool preownAttempted_=false,preowned_=false;
    std::exception_ptr preownException_;
    bool sealed_ = false, failed_ = false, beginAttempted_ = false;
public:
    OriginalCommandRoute(DeferredOriginalPreownTag,size_t selectedCommands) noexcept:selectedCommands_(selectedCommands){}
    explicit OriginalCommandRoute(size_t selectedCommands):OriginalCommandRoute(DeferredOriginalPreown,selectedCommands){
        if(!PreownOriginal()&&preownException_)std::rethrow_exception(preownException_);
    }
    bool PreownOriginal() noexcept {
        if(preownAttempted_)return false;preownAttempted_=true;
        try{
            constexpr size_t rowBytes=sizeof(std::unique_ptr<OriginalRouteCommand>)+sizeof(OriginalRouteCommand);
            if(!selectedCommands_||selectedCommands_>(SIZE_MAX-sizeof(*this))/rowBytes)
                throw std::invalid_argument("selected original Linux command roster");
            commands_=std::make_unique<std::unique_ptr<OriginalRouteCommand>[]>(selectedCommands_);
            capacity_=selectedCommands_;
            for(size_t i=0;i<capacity_;++i){commands_[i]=std::make_unique<OriginalRouteCommand>();bornCount_=i+1;}
            preowned_=true;return true;
        }catch(...){preownException_=std::current_exception();failed_=true;return false;}
    }
    bool PreownedOriginal() const noexcept{return preowned_;}
    const std::exception_ptr& PreownExceptionOriginal() const noexcept{return preownException_;}
    size_t AllocatedCommandCapacityOriginal() const noexcept{return capacity_;}
    size_t BornCommandCountOriginal() const noexcept{return bornCount_;}
    const OriginalRouteCommand* AllocatedCommandOriginal(size_t i) const noexcept{return commands_&&i<capacity_?commands_[i].get():nullptr;}
    OriginalCommandRoute(const OriginalCommandRoute&) = delete;
    OriginalCommandRoute& operator=(const OriginalCommandRoute&) = delete;
    bool PreownSnapshot(size_t ordinal,PyObject* exactSnapshot) noexcept {
        if(!preowned_||sealed_||failed_||beginAttempted_||ordinal>=capacity_)return false;
        auto& row=*commands_[ordinal];
        if(row.bindingAttempted)return false;
        if(row.plan.attempted)return row.plan.SameSnapshot(exactSnapshot);
        if(!row.plan.CaptureSnapshot(exactSnapshot)){failed_=true;return false;}
        return true;
    }
    // Called before any command acquisition. The concrete session owns each
    // gate and stdin capability through every retained command and cleanup.
    bool Bind(size_t ordinal, PyObject* exactSnapshot, Gate& actualGate, int actualEmptyStdin, int actualDirectory) noexcept {
        if (!preowned_ || sealed_ || failed_ || ordinal != boundCount_ || ordinal >= capacity_) return false;
        auto& row = *commands_[ordinal];
        if (row.bindingAttempted) return false;
        row.bindingAttempted = true; row.originalGate = &actualGate;
        row.originalEmptyStdin = actualEmptyStdin;
        row.originalDirectory = actualDirectory;
        try {
            if (actualEmptyStdin < 0 || actualDirectory < 0 ||
                !(row.plan.attempted?row.plan.SameSnapshot(exactSnapshot):row.plan.CaptureSnapshot(exactSnapshot))) {
                failed_ = true; return false;
            }
            // Includes the complete lifecycle State/read stores. Allocation is
            // finished for ALL commands before Seal permits the first birth.
            row.driver = std::make_unique<FiniteCommandDriver>(actualGate);
            row.bound = true; ++boundCount_; return true;
        } catch (...) {
            row.originalException = std::current_exception(); failed_ = true;
            if (!PyErr_Occurred()) PyErr_SetString(PyExc_RuntimeError,"Linux route preownership failed");
            return false;
        }
    }
    bool Seal() noexcept {
        if (!preowned_ || sealed_ || failed_ || boundCount_ != capacity_) return false;
        sealed_ = true; return true;
    }
    bool BeginCurrent() noexcept {
        if (!sealed_ || failed_ || current_ >= capacity_) return false;
        auto& row = *commands_[current_];
        if (!row.bound || row.beginAttempted) return false;
        beginAttempted_ = row.beginAttempted = true;
        try {
            row.beginReturned = row.driver->Begin(row.plan.image.data(), row.plan.argv.data(),
                row.plan.environment.data(), row.plan.cwd.data(), row.originalEmptyStdin, row.originalDirectory);
            if (!row.beginReturned) failed_ = true;
            return row.beginReturned;
        } catch (...) { row.originalException = std::current_exception(); failed_ = true; return false; }
    }
    DriverObservation StepCurrent() noexcept {
        if (!sealed_ || current_ >= capacity_) return {};
        auto& row = *commands_[current_];
        if (!row.beginAttempted || !row.driver) return row.last;
        if (row.originalException) {
            row.last.nativeDisposed = row.driver->DisposeAlreadyEligible();
            return row.last;
        }
        try { row.last = row.driver->Step(); }
        catch (...) { if (!row.originalException) row.originalException = std::current_exception(); failed_ = true; }
        if (row.driver->PrivateOriginals().failed) failed_ = true;
        return row.last; // Failure does not suppress independently eligible Step.
    }
    bool Advance() noexcept {
        if (!sealed_ || failed_ || current_+1 >= capacity_) return false;
        auto& row = *commands_[current_]; row.advanceAttempted = true;
        if (!row.beginReturned || !row.driver) return false;
        const auto& native = row.driver->PrivateOriginals();
        if (!native.disposed || !native.terminal || native.failed || native.continuationDenied ||
            !native.setupValidated || row.originalException) return false;
        row.advanced = true; ++current_; return true;
    }
    bool AllPhysicalCommandsDisposed() const noexcept {
        // Seal is a qualification boundary. Failed precharge with no Begin
        // owns no child/setup resource; inspect each actual acquisition below
        // instead of demanding a successful seal for independent disposal.
        for (size_t i=0; i<bornCount_; ++i) {
            const auto& row = *commands_[i];
            // Unlaunched plans own only source memory; actual acquired native
            // setup resources require the driver's explicit disposal result.
            if (!row.beginAttempted) continue;
            if (!row.driver) return false;
            const auto& state = row.driver->PrivateOriginals();
            if (state.originalCloneResult > 0 || state.rootCreationUnknown) {
                if (!state.disposed) return false;
            } else if (!state.unlaunchedDisposalAttempted || !state.unlaunchedDisposalResult) return false;
        }
        return true;
    }
    bool QualificationClean() const noexcept {
        if (!sealed_ || failed_ || !beginAttempted_ || current_+1 != capacity_) return false;
        for (size_t i=0; i<bornCount_; ++i) {
            const auto& row = *commands_[i];
            if (!row.beginReturned || row.originalException || !row.driver) return false;
            const auto& state = row.driver->PrivateOriginals();
            if (state.failed || !state.terminal || !state.disposed || !state.setupValidated) return false;
        }
        return true;
    }
    const OriginalRouteCommand& PrivateCommand(size_t ordinal) const { return *commands_[ordinal]; }
    bool ReservedSourceBytes(size_t& bytes) const noexcept {
        bytes = sizeof(*this);
        auto add = [&](size_t amount) noexcept {
            if (amount > SIZE_MAX-bytes) return false;
            bytes += amount; return true;
        };
        if (!add(capacity_*sizeof(std::unique_ptr<OriginalRouteCommand>))) return false;
        for (size_t i=0; i<bornCount_; ++i) {
            if (!add(sizeof(OriginalRouteCommand))) return false;
            const auto& row = *commands_[i];
            if (row.driver) {
                size_t owned = 0;
                // Unknown actual page geometry remains unknown, including for
                // a preowned command not yet launched. Never a fit certificate.
                if (!row.driver->ReservedSourceBytes(owned) || !add(owned)) return false;
            }
        }
        return true; // Excludes Python referents, gate owners, allocator/kernel.
    }
    size_t CommandCount() const noexcept { return capacity_; }
    size_t CurrentOrdinal() const noexcept { return current_; }
    bool AnyCommandAcquisitionAttempted() const noexcept { return beginAttempted_; }
};
// Command exit codes remain originals for the selected caller contract. This
// route does not turn expected negative assertions into a new zero-exit rule.
}

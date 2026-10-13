// UNCOMPILED concrete all-command routing over ONE creating debugger thread.
// Native SessionCore owns this route; no external function table supplies gates.
#pragma once
#include "windows_driver.hpp"
#include "windows_session_gate.hpp"
namespace template_creator {
struct OriginalWindowsRouteRow {
    template_creator_bridge::WindowsOriginalCommand command;
    std::wstring mutableLaunch;
    std::unique_ptr<OriginalWindowsGate> gate;
    const State* nativeOriginal = nullptr;
    std::exception_ptr prechargeOriginal;
    bool prechargeAttempted = false, precharged = false, beginAttempted = false;
};
class OriginalWindowsCommandRoute;
class OriginalWindowsGateRouter final : public InputGate {
    friend class OriginalWindowsCommandRoute;
    OriginalWindowsRouteRow* rows_;
    SIZE_T count_, active_ = 0;
    explicit OriginalWindowsGateRouter(OriginalWindowsRouteRow* rows, SIZE_T count) : rows_(rows), count_(count) {}
    OriginalWindowsGate& Active() noexcept { return *rows_[active_].gate; }
public:
    bool BeforeCreate(const State& state,const wchar_t* image,const wchar_t* line,const wchar_t* cwd,
                      const wchar_t* environment,SIZE_T extent) override {
        return Active().BeforeCreate(state,image,line,cwd,environment,extent);
    }
    bool BeforeContinue(const DEBUG_EVENT& event,const State& state) override { return Active().BeforeContinue(event,state); }
    bool BeforeFailureContinue(const DEBUG_EVENT& event,const State& state) override { return Active().BeforeFailureContinue(event,state); }
    SavedEventDecision DecideSavedEvent(const Event& event,const State& state,bool failed) override {
        return Active().DecideSavedEvent(event,state,failed);
    }
    bool ValidateOwnedObserverConsumption(const Event& event,const State& state) override {
        return Active().ValidateOwnedObserverConsumption(event,state);
    }
    bool ValidateOwnedObserverCleanupConsumption(const Event& event,const State& state) override {
        return Active().ValidateOwnedObserverCleanupConsumption(event,state);
    }
    bool BeforeNativeDisposal(const State& state) override { return Active().BeforeNativeDisposal(state); }
    bool BeforeFailureDrain(const State& state) override { return Active().BeforeFailureDrain(state); }
    bool IsInitialDebuggerBreakpoint(const DEBUG_EVENT& event,const State& state) override { return Active().IsInitialDebuggerBreakpoint(event,state); }
};
class OriginalWindowsCommandRoute {
    std::unique_ptr<OriginalWindowsRouteRow[]> rows_;
    SIZE_T count_ = 0, active_ = 0;
    std::unique_ptr<OriginalWindowsGateRouter> router_;
    std::unique_ptr<FiniteCommandDriver> driver_;
    bool sealAttempted_ = false, sealed_ = false, failed_ = false;
    std::exception_ptr sealOriginal_;
public:
    explicit OriginalWindowsCommandRoute(SIZE_T completeCount) {
        if (!completeCount || completeCount > 65) throw std::invalid_argument("complete native Windows route");
        rows_ = std::make_unique<OriginalWindowsRouteRow[]>(completeCount); count_ = completeCount;
    }
    OriginalWindowsCommandRoute(const OriginalWindowsCommandRoute&) = delete;
    OriginalWindowsCommandRoute& operator=(const OriginalWindowsCommandRoute&) = delete;
    bool Precharge(SIZE_T ordinal, PyObject* exactSnapshot, OriginalPrivateIngress& ingress,
                   NativeHeldFiles& inputs, SIZE_T selectedCallerRows,
                   const GateImageGeometry* imageGeometry,SIZE_T imageRows,SIZE_T ntdllFile,
                   template_creator_bridge::OriginalWindowsPendingCalls& pendingCalls,
                   const OriginalWindowsFixtureRoot* fixtureRoot=nullptr) {
        if (sealAttempted_ || ordinal >= count_) return false;
        auto& row = rows_[ordinal];
        if (row.prechargeAttempted) return false; row.prechargeAttempted = true;
        if (!row.command.CaptureSnapshot(exactSnapshot)) { failed_ = true; return false; }
        // Construct the actual driver-required std::wstring BEFORE any bootstrap
        // effects. It stays with this exact command; Begin performs no conversion.
        try {
            row.mutableLaunch.assign(reinterpret_cast<const wchar_t*>(row.command.commandLine.data()),row.command.commandUnits);
            row.gate = std::make_unique<OriginalWindowsGate>(ingress,inputs,row.command,selectedCallerRows,imageGeometry,imageRows,ntdllFile,pendingCalls,this,fixtureRoot);
            if(!row.gate->PreownOriginal()){
                row.prechargeOriginal=row.gate->PreownExceptionOriginal();failed_=true;return false;
            }
        } catch (...) { row.prechargeOriginal = std::current_exception(); failed_ = true; throw; }
        if (row.gate->Failed()) { failed_ = true; return false; }
        row.precharged = true; return true;
    }
    bool Seal() {
        if (sealAttempted_) return false; sealAttempted_ = true;
        if (failed_) return false;
        for (SIZE_T i=0; i<count_; ++i) if (!rows_[i].precharged) { failed_ = true; return false; }
        // Both stores are complete before the first Begin. Router's selection is
        // private and can advance only together with this retained native owner.
        try {
            router_.reset(new OriginalWindowsGateRouter(rows_.get(),count_));
            driver_ = std::make_unique<FiniteCommandDriver>(*router_,count_);
        } catch (...) { sealOriginal_ = std::current_exception(); failed_ = true; throw; }
        sealed_ = true; return true;
    }
    bool Begin(SIZE_T ordinal, PyObject* exactSnapshot) {
        if (!sealed_ || failed_ || ordinal != active_ || ordinal >= count_) return false;
        auto& row = rows_[ordinal];
        if (row.beginAttempted || !row.command.SameSnapshot(exactSnapshot)) { failed_ = true; return false; }
        row.beginAttempted = true;
        const bool result = driver_->Begin(reinterpret_cast<const wchar_t*>(row.command.image.data()),row.mutableLaunch,
            reinterpret_cast<const wchar_t*>(row.command.cwd.data()),reinterpret_cast<const wchar_t*>(row.command.environment.data()),
            row.command.environmentUnits,ordinal == 0);
        row.nativeOriginal = &driver_->PrivateOriginals();
        if (!result) failed_ = true;
        return result;
    }
    DriverObservation Step(SIZE_T ordinal) {
        if (!sealed_ || ordinal != active_ || ordinal >= count_ || !rows_[ordinal].beginAttempted)
            throw std::invalid_argument("same active original Windows command required");
        auto observation = driver_->Step();
        rows_[ordinal].nativeOriginal = &driver_->PrivateOriginals();
        const auto& original=*rows_[ordinal].nativeOriginal;
        if(original.terminal && original.streams.NativeEof() &&
            !rows_[ordinal].gate->DisposeParserInputsAfterFullNativeSettlement(original))failed_=true;
        if (rows_[ordinal].nativeOriginal->stopped) failed_ = true;
        return observation; // Failure never suppresses eligible existing cleanup.
    }
    bool AdvanceDisposedOriginal() {
        if (!sealed_ || failed_ || active_+1 >= count_ || !rows_[active_].nativeOriginal ||
            !rows_[active_].nativeOriginal->disposed) return false;
        if (!driver_->NextCommand()) { failed_ = true; return false; }
        ++active_; router_->active_ = active_; return true;
    }
    const OriginalWindowsRouteRow& PrivateOriginal(SIZE_T ordinal) const { return rows_[ordinal]; }
    SIZE_T SelectedCount() const noexcept { return count_; }
    bool AllPhysicalCommandsDisposed() const noexcept {
        for (SIZE_T i=0; i<count_; ++i) {
            if (rows_[i].gate && !rows_[i].gate->ObserverModulesPhysicallyDisposed()) return false;
            if (rows_[i].beginAttempted && (!rows_[i].nativeOriginal ||
                (!rows_[i].nativeOriginal->disposed && !rows_[i].nativeOriginal->unlaunchedDisposalResult))) return false;
        }
        return true;
    }
    bool Failed() const noexcept { return failed_; }
    const std::exception_ptr& PrivateSealOriginal() const noexcept { return sealOriginal_; }
    bool ReservedSourceBytes(SIZE_T& bytes) const noexcept {
        bytes = sizeof(*this);
        const auto add = [&](SIZE_T amount) noexcept { if (amount > SIZE_MAX-bytes) return false; bytes += amount; return true; };
        if (!add(count_*sizeof(OriginalWindowsRouteRow))) return false;
        for (SIZE_T i=0; i<count_; ++i) {
            const auto& row = rows_[i];
            if (row.mutableLaunch.capacity() > SIZE_MAX/sizeof(wchar_t)-1 ||
                !add((row.mutableLaunch.capacity()+1)*sizeof(wchar_t))) return false;
            SIZE_T gateBytes = 0;
            if (row.gate && (!row.gate->ReservedSourceBytes(gateBytes) || !add(gateBytes))) return false;
        }
        SIZE_T driverBytes = 0;
        return (!router_ || add(sizeof(OriginalWindowsGateRouter))) &&
            (!driver_ || (driver_->ReservedSourceBytes(driverBytes) && add(driverBytes)));
    }
};
}

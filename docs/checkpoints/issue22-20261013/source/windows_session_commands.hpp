// UNCOMPILED concrete Windows command-session body. Ingress/input/sink/native
// host wiring remains separate source work; this is not a standalone issuer.
#pragma once
#include "windows_command_route.hpp"
namespace template_creator {
struct OriginalSessionCommand {
    PyObject* token = nullptr; // Exact strong native-owned identity, never fields.
    PyObject* snapshot = nullptr;
    bool beginAttempted = false, begun = false, bootstrapPending = false;
    bool qualificationFailed = false;
    DriverObservation latest{};
    std::exception_ptr nativeOriginal;
};
class OriginalWindowsSessionCommands {
    OriginalWindowsCommandRoute& route_;
    std::unique_ptr<OriginalSessionCommand[]> commands_;
    SIZE_T count_ = 0, active_ = 0;
    PyObject* bootstrapSnapshot_ = nullptr;
    bool failed_ = false, bindingAttempted_ = false, bound_ = false;
    std::exception_ptr bindingOriginal_;
    OriginalSessionCommand* Exact(PyObject* token, SIZE_T& ordinal) noexcept {
        if (!bound_ || !token) { PyErr_SetString(PyExc_ValueError,"bound original native command required"); return nullptr; }
        for (SIZE_T i=0; i<count_; ++i) if (commands_[i].token == token) { ordinal = i; return &commands_[i]; }
        PyErr_SetString(PyExc_ValueError,"exact retained native command token required"); return nullptr;
    }
    static PyObject* Error(const char* message) noexcept {
        if (!PyErr_Occurred()) PyErr_SetString(PyExc_RuntimeError,message); return nullptr;
    }
public:
    // Host supplies the exact precharged source bootstrap snapshot and selected
    // protected-command snapshots. All Python identities are allocated BEFORE
    // the first bootstrap effect; none contains a caller-controlled native PID.
    OriginalWindowsSessionCommands(OriginalWindowsCommandRoute& route, SIZE_T selectedCommands) : route_(route) {
        if (!selectedCommands || selectedCommands > 64 || route.SelectedCount() != selectedCommands+1)
            throw std::invalid_argument("complete native command roster capacity");
        commands_ = std::make_unique<OriginalSessionCommand[]>(selectedCommands); count_ = selectedCommands;
    }
    // Called only after this owner has entered the retained native registry.
    // A failure leaves every acquired Python original in its preowned row.
    bool BindSnapshots(PyObject* bootstrapSnapshot, PyObject* exactSnapshots) {
        if (bindingAttempted_) return false; bindingAttempted_ = true;
        if (!exactSnapshots || !PyTuple_CheckExact(exactSnapshots) || PyTuple_GET_SIZE(exactSnapshots) <= 0 ||
            static_cast<SIZE_T>(PyTuple_GET_SIZE(exactSnapshots)) != count_ || !bootstrapSnapshot) {
            failed_ = true; throw std::invalid_argument("complete native command snapshot roster");
        }
        try {
        bootstrapSnapshot_ = Py_NewRef(bootstrapSnapshot);
        for (SIZE_T i=0; i<count_; ++i) {
            auto& row = commands_[i];
            row.snapshot = Py_NewRef(PyTuple_GET_ITEM(exactSnapshots,static_cast<Py_ssize_t>(i)));
            row.token = PyObject_CallNoArgs(reinterpret_cast<PyObject*>(&PyBaseObject_Type));
            if (!row.token || PyErr_Occurred()) { failed_ = true; return false; }
            if (!route_.PrivateOriginal(i+1).command.SameSnapshot(row.snapshot)) {
                failed_ = true; throw std::invalid_argument("native route snapshot association");
            }
        }
        if (!route_.PrivateOriginal(0).command.SameSnapshot(bootstrapSnapshot_)) {
            failed_ = true; throw std::invalid_argument("native bootstrap snapshot association");
        }
        } catch (...) { bindingOriginal_ = std::current_exception(); failed_ = true; throw; }
        bound_ = true; return true;
    }
    OriginalWindowsSessionCommands(const OriginalWindowsSessionCommands&) = delete;
    OriginalWindowsSessionCommands& operator=(const OriginalWindowsSessionCommands&) = delete;
    PyObject* Preowned(PyObject* originalOrdinal) noexcept {
        if (!bound_) return Error("original command identities not bound");
        if (!originalOrdinal || !PyLong_CheckExact(originalOrdinal)) return Error("exact command ordinal required");
        const auto ordinal = PyLong_AsSize_t(originalOrdinal);
        if (PyErr_Occurred()) return nullptr;
        if (ordinal >= count_) return Error("original command ordinal outside selected route");
        return Py_NewRef(commands_[ordinal].token);
    }
    PyObject* Begin(PyObject* originalToken, PyObject* exactSnapshot) {
        SIZE_T ordinal = 0; auto* row = Exact(originalToken,ordinal); if (!row) return nullptr;
        if (!bound_ || failed_ || ordinal != active_ || row->beginAttempted || exactSnapshot != row->snapshot)
            return Error("same unstarted original command required");
        row->beginAttempted = true;
        try {
            if (ordinal == 0) {
                row->bootstrapPending = true;
                row->begun = route_.Begin(0,bootstrapSnapshot_);
            } else {
                row->begun = route_.AdvanceDisposedOriginal() && route_.Begin(ordinal+1,row->snapshot);
            }
        } catch (...) { if (!row->nativeOriginal) row->nativeOriginal = std::current_exception(); row->qualificationFailed = failed_ = true; throw; }
        if (!row->begun) { row->qualificationFailed = failed_ = true; return Error("original native launch failed"); }
        Py_RETURN_NONE;
    }
    PyObject* Step(PyObject* originalToken) {
        SIZE_T ordinal = 0; auto* row = Exact(originalToken,ordinal); if (!row) return nullptr;
        if (ordinal != active_ || !row->beginAttempted) return Error("same begun original command required");
        try {
            row->latest = route_.Step(row->bootstrapPending ? 0 : ordinal+1);
            if (row->bootstrapPending && row->latest.nativeDisposed) {
                if (route_.Failed() || !route_.AdvanceDisposedOriginal()) {
                    row->qualificationFailed = failed_ = true; return Error("original bootstrap qualification failed");
                }
                row->bootstrapPending = false;
                row->begun = route_.Begin(1,row->snapshot);
                row->latest = {}; // This command's terminal/EOF are not bootstrap's.
                if (!row->begun) { row->qualificationFailed = failed_ = true; return Error("original command launch after bootstrap failed"); }
            }
            if (route_.Failed()) row->qualificationFailed = failed_ = true;
        } catch (...) { if (!row->nativeOriginal) row->nativeOriginal = std::current_exception(); row->qualificationFailed = failed_ = true; throw; }
        Py_RETURN_NONE;
    }
    PyObject* Disposed(PyObject* originalToken) noexcept {
        SIZE_T ordinal = 0; auto* row = Exact(originalToken,ordinal); if (!row) return nullptr;
        if (!row->beginAttempted) Py_RETURN_TRUE; // This token owns no native launch.
        const auto* native = route_.PrivateOriginal(row->bootstrapPending?0:ordinal+1).nativeOriginal;
        if (!native) Py_RETURN_FALSE;
        // A clean bootstrap still has to progress to the protected command.
        // Failed launch cleanup can instead report its actual no-child disposal
        // without inventing bootstrap completion or starting another command.
        if (row->bootstrapPending && !route_.Failed()) Py_RETURN_FALSE;
        return PyBool_FromLong(native->disposed || native->unlaunchedDisposalResult);
    }
    PyObject* DisposeIndependentlyEligible(PyObject* originalToken) {
        SIZE_T ordinal = 0; auto* row = Exact(originalToken,ordinal); if (!row) return nullptr;
        if (!row->beginAttempted) Py_RETURN_NONE; // This token acquired no process.
        const SIZE_T nativeOrdinal = row->bootstrapPending ? 0 : ordinal+1;
        const auto* native = route_.PrivateOriginal(nativeOrdinal).nativeOriginal;
        if (native && native->disposed) Py_RETURN_NONE;
        if (ordinal != active_) return Error("same retained active cleanup owner required");
        // A finite observation/disposal step on the existing route only. Unlike
        // Step, this never advances from bootstrap to a new protected command.
        // Gate failure continuation policy remains the actual native boundary.
        row->qualificationFailed = failed_ = true;
        try { row->latest = route_.Step(nativeOrdinal); }
        catch (...) {
            if (!row->nativeOriginal) row->nativeOriginal = std::current_exception();
            throw;
        }
        Py_RETURN_NONE; // An eligible attempt is not a physical closure result.
    }
    PyObject* VerifyZeroResult(PyObject* originalToken) noexcept {
        SIZE_T ordinal = 0; auto* row = Exact(originalToken,ordinal); if (!row) return nullptr;
        const auto* native = route_.PrivateOriginal(ordinal+1).nativeOriginal;
        // This method serves the unchanged top-level positive test/generator
        // commands; expected-negative subprocess assertions execute inside them.
        if (failed_ || row->qualificationFailed || row->bootstrapPending || !native || !native->disposed ||
            native->stopped || !native->memberCount || native->members[0].code != 0)
            return Error("original protected command did not qualify");
        if (ordinal != active_) return Error("original command verification order");
        ++active_; Py_RETURN_NONE;
    }
    const OriginalSessionCommand& PrivateOriginal(SIZE_T ordinal) const { return commands_[ordinal]; }
    const OriginalSessionCommand* ExactPrivateOriginal(PyObject* token,SIZE_T& ordinal) noexcept {
        return Exact(token,ordinal);
    }
    const State* ExactPrivateNative(PyObject* token,SIZE_T& ordinal) noexcept {
        const auto* row = Exact(token,ordinal);
        if (!row) return nullptr;
        return route_.PrivateOriginal(row->bootstrapPending ? 0 : ordinal+1).nativeOriginal;
    }
    const State* PrivateBootstrapNative() const noexcept { return route_.PrivateOriginal(0).nativeOriginal; }
    bool AllPhysicalCommandsDisposed() const noexcept { return route_.AllPhysicalCommandsDisposed(); }
    bool QualificationComplete() const noexcept { return !failed_ && active_ == count_ && !route_.Failed(); }
    const std::exception_ptr& PrivateBindingOriginal() const noexcept { return bindingOriginal_; }
    SIZE_T ReservedSourceBytes() const noexcept {
        return sizeof(*this)+count_*sizeof(OriginalSessionCommand);
        // Strong Python originals/token allocations are additional whole-runtime
        // referents, not hidden inside this source-cell counter.
    }
};
}

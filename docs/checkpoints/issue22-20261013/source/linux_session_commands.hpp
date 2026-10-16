// UNCOMPILED exact-token Linux caller wiring. Concrete native gates/input/root
// admission and the resident platform/host still have to own this source body.
#pragma once
#include "linux_command_route.hpp"
#include "linux_native_sink.hpp"
namespace template_creator_linux {
struct LinuxSessionCommand {
    PyObject* originalToken = nullptr;
    PyObject* originalSnapshot = nullptr;
    bool beginAttempted = false, routeSelected = false, began = false, verified = false;
    DriverObservation latest{};
};
class OriginalLinuxSessionCommands {
    OriginalCommandRoute& route_;
    std::unique_ptr<LinuxSessionCommand[]> rows_;
    size_t count_ = 0, active_ = 0;
    bool bindingAttempted_ = false, bound_ = false, failed_ = false;
    bool preownAttempted_=false,preowned_=false;
    std::exception_ptr preownException_;
    LinuxSessionCommand* Exact(PyObject* token, size_t& ordinal) noexcept {
        if (bound_ && token)
            for (size_t i=0; i<count_; ++i)
                if (rows_[i].originalToken == token) { ordinal=i; return &rows_[i]; }
        Error("same retained Linux command token required"); return nullptr;
    }
    static PyObject* Error(const char* text) noexcept {
        if (!PyErr_Occurred()) PyErr_SetString(PyExc_RuntimeError,text);
        return nullptr;
    }
public:
    OriginalLinuxSessionCommands(DeferredOriginalPreownTag,OriginalCommandRoute& route) noexcept:route_(route){}
    explicit OriginalLinuxSessionCommands(OriginalCommandRoute& route):OriginalLinuxSessionCommands(DeferredOriginalPreown,route){
        if(!PreownOriginal()&&preownException_)std::rethrow_exception(preownException_);
    }
    bool PreownOriginal() noexcept {
        if(preownAttempted_)return false;preownAttempted_=true;
        try{
            const size_t selected=route_.CommandCount();
            if(!route_.PreownedOriginal()||!selected||selected>(SIZE_MAX-sizeof(*this))/sizeof(LinuxSessionCommand))
                throw std::invalid_argument("preowned Linux session command roster");
            rows_=std::make_unique<LinuxSessionCommand[]>(selected);count_=selected;
            preowned_=true;return true;
        }catch(...){preownException_=std::current_exception();failed_=true;return false;}
    }
    bool OutputBackingSeparateOriginal(const void* output,size_t bytes) const noexcept {
        return OriginalBackingStorageSeparate(output,bytes,this,1,sizeof(*this))&&
            OriginalBackingStorageSeparate(output,bytes,rows_.get(),count_,sizeof(LinuxSessionCommand));
    }
    bool PreownedOriginal() const noexcept{return preowned_;}
    const std::exception_ptr& PreownExceptionOriginal() const noexcept{return preownException_;}
    OriginalLinuxSessionCommands(const OriginalLinuxSessionCommands&) = delete;
    OriginalLinuxSessionCommands& operator=(const OriginalLinuxSessionCommands&) = delete;
    bool BindSnapshots(PyObject* snapshots) noexcept {
        if (!preowned_ || bindingAttempted_) return false;
        bindingAttempted_ = true;
        if (!snapshots || !PyTuple_CheckExact(snapshots) ||
            static_cast<size_t>(PyTuple_GET_SIZE(snapshots)) != count_) {
            failed_ = true; Error("exact Linux snapshot roster required"); return false;
        }
        for (size_t i=0; i<count_; ++i) {
            auto& row = rows_[i];
            row.originalSnapshot = Py_NewRef(PyTuple_GET_ITEM(snapshots,static_cast<Py_ssize_t>(i)));
            if (!route_.PrivateCommand(i).plan.SameSnapshot(row.originalSnapshot)) {
                failed_ = true; Error("Linux route snapshot association"); return false;
            }
            row.originalToken = PyObject_CallNoArgs(reinterpret_cast<PyObject*>(&PyBaseObject_Type));
            // Attach the actual returned reference BEFORE error classification.
            if (!row.originalToken || PyErr_Occurred()) { failed_ = true; return false; }
        }
        bound_ = true; return true;
    }
    PyObject* Preowned(PyObject* originalOrdinal) noexcept {
        if (!bound_ || !originalOrdinal || !PyLong_CheckExact(originalOrdinal))
            return Error("exact bound Linux command ordinal required");
        const size_t ordinal = PyLong_AsSize_t(originalOrdinal);
        if (PyErr_Occurred()) return nullptr;
        if (ordinal >= count_) return Error("Linux command ordinal outside original roster");
        return Py_NewRef(rows_[ordinal].originalToken);
    }
    PyObject* Begin(PyObject* token, PyObject* snapshot) noexcept {
        size_t ordinal=0; auto* row=Exact(token,ordinal); if (!row) return nullptr;
        if (failed_ || ordinal != active_ || row->beginAttempted || snapshot != row->originalSnapshot)
            return Error("same unstarted Linux command and snapshot required");
        row->beginAttempted = true;
        if (ordinal && !route_.Advance()) { failed_=true; return Error("preceding Linux command not qualified/disposed"); }
        row->routeSelected = true;
        row->began = route_.BeginCurrent();
        if (!row->began) { failed_=true; return Error("original Linux launch failed"); }
        Py_RETURN_NONE;
    }
    PyObject* Step(PyObject* token) noexcept {
        size_t ordinal=0; auto* row=Exact(token,ordinal); if (!row) return nullptr;
        if (!row->beginAttempted || !row->routeSelected || ordinal != active_) return Error("same active Linux command required");
        row->latest=route_.StepCurrent();
        const auto& native=route_.PrivateCommand(ordinal).driver->PrivateOriginals();
        if (native.failed || route_.PrivateCommand(ordinal).originalException) failed_=true;
        Py_RETURN_NONE; // Failed qualification can still progress existing cleanup.
    }
    PyObject* Disposed(PyObject* token) noexcept {
        size_t ordinal=0; auto* row=Exact(token,ordinal); if (!row) return nullptr;
        const auto& command=route_.PrivateCommand(ordinal);
        if (!row->beginAttempted || !row->routeSelected) Py_RETURN_TRUE; // No native launch/setup owned by this token.
        if (!command.driver) Py_RETURN_FALSE;
        const auto& native=command.driver->PrivateOriginals();
        return PyBool_FromLong(native.disposed || native.unlaunchedDisposalResult);
    }
    PyObject* DisposeIndependentlyEligible(PyObject* token) noexcept {
        size_t ordinal=0; auto* row=Exact(token,ordinal); if (!row) return nullptr;
        if (!row->beginAttempted || !row->routeSelected) Py_RETURN_NONE;
        if (ordinal != active_) return Error("same retained active Linux cleanup required");
        failed_=true; row->latest=route_.StepCurrent();
        Py_RETURN_NONE; // Never advances or launches a new command.
    }
    PyObject* VerifyZeroResult(PyObject* token) noexcept {
        size_t ordinal=0; auto* row=Exact(token,ordinal); if (!row) return nullptr;
        const auto& command=route_.PrivateCommand(ordinal);
        if (failed_ || ordinal != active_ || !row->began || row->verified || !command.driver)
            return Error("same clean Linux protected command required");
        const auto& native=command.driver->PrivateOriginals();
        if (!native.disposed || native.failed || !native.setupValidated || !native.traceeCount ||
            native.currentRootTracee >= native.traceeCount)
            return Error("Linux command native closure incomplete");
        const auto& root=native.tracees[native.currentRootTracee];
        if (!root.terminal || !WIFEXITED(root.terminalStatus) || WEXITSTATUS(root.terminalStatus) != 0)
            return Error("original Linux protected command result nonzero");
        row->verified=true; ++active_; Py_RETURN_NONE;
    }
    PyObject* CaptureOriginalCommand(PyObject* token,OriginalNativeSink& sink) noexcept {
        size_t ordinal=0;auto* row=Exact(token,ordinal);if(!row)return nullptr;
        const auto& command=route_.PrivateCommand(ordinal);
        struct Frame { uint64_t version,kind,commandOrdinal,extent; };
        const auto append=[&](uint64_t kind,const void* source,size_t extent) noexcept {
            const Frame frame{1,kind,static_cast<uint64_t>(ordinal),static_cast<uint64_t>(extent)};
            return sink.Append(reinterpret_cast<const unsigned char*>(&frame),sizeof(frame)) &&
                sink.Append(static_cast<const unsigned char*>(source),extent);
        };
        // Private same-ABI bytes are observations only. All owners/referents,
        // exceptions and native descriptors remain resident in this session.
        bool captured=append(0,row,sizeof(*row));
        if(captured&&command.driver) {
            const auto& state=command.driver->PrivateOriginals();
            captured=append(1,&state,sizeof(state));
            const auto stream=[&](const Reader& reader,uint64_t kind) noexcept {
                if(reader.used>Reader::ByteLimit||reader.discardRetained>Reader::ByteLimit||
                   reader.resultCount>EventLimit||reader.discardResultCount>EventLimit)return false;
                return append(kind,reader.bytes.get(),reader.used) &&
                    append(kind+1,reader.results.get(),reader.resultCount*sizeof(ReadResult)) &&
                    append(kind+2,reader.discard.get(),reader.discardRetained) &&
                    append(kind+3,reader.discardResults.get(),reader.discardResultCount*sizeof(ReadResult));
            };
            if(captured)captured=stream(state.stdoutStream,2);
            if(captured)captured=stream(state.stderrStream,6);
            if(captured)captured=stream(state.setupStream,10);
        }
        if(!captured){failed_=true;return Error("original Linux command capture failed");}
        Py_RETURN_NONE; // Prefix durability/readback remains a separate operation.
    }
    bool AllPhysicalCommandsDisposed() const noexcept { return route_.AllPhysicalCommandsDisposed(); }
    bool QualificationComplete() const noexcept {
        if(!bound_||failed_||!count_||active_!=count_||count_!=route_.CommandCount()||
           !route_.QualificationClean())return false;
        for(size_t i=0;i<count_;++i)
            if(!rows_[i].originalToken||!rows_[i].originalSnapshot||!rows_[i].beginAttempted||
               !rows_[i].routeSelected||!rows_[i].began||!rows_[i].verified)return false;
        return true;
    }
    const LinuxSessionCommand& PrivateOriginal(size_t ordinal) const { return rows_[ordinal]; }
    size_t ReservedSourceBytes() const noexcept { return sizeof(*this)+count_*sizeof(LinuxSessionCommand); }
    // Strong snapshot/token referents and Python error triples belong to the
    // resident platform/core and measured whole runtime, additional to cells.
};
}

// UNCOMPILED final original-reference retention and native prefix capture.
// Python referents remain live in the original interpreter; this record does
// not serialize their graphs or claim they survive interpreter destruction.
#ifdef _WIN32
#include "native_windows_platform.hpp"
#include <cstdint>
namespace template_creator_bridge {
PyObject* NativePlatformState::CaptureCreator(PyObject* originals,PyObject* failures) {
    if (creatorCaptureAttempted) {
        qualificationFailed = true;
        if (!PyErr_Occurred()) PyErr_SetString(PyExc_RuntimeError,"final creator capture already attempted");
        return nullptr;
    }
    creatorCaptureAttempted = true;
    // Retain both originals before any validation, conversion or sink effect.
    creatorOriginals = Py_XNewRef(originals); creatorFailures = Py_XNewRef(failures);
    if (!originals || !failures || !PyTuple_CheckExact(originals) || PyTuple_GET_SIZE(originals)!=4 ||
        !PyTuple_CheckExact(failures) || PyTuple_GET_SIZE(failures)!=3 ||
        PyTuple_GET_ITEM(originals,0)!=originalIngressIdentity) {
        qualificationFailed = true;
        if (!PyErr_Occurred()) PyErr_SetString(PyExc_ValueError,"exact private creator originals required");
        return nullptr;
    }
    auto* failureRows = PyTuple_GET_ITEM(failures,0);
    if (!PyTuple_CheckExact(failureRows)) {
        qualificationFailed = true;
        if (!PyErr_Occurred()) PyErr_SetString(PyExc_ValueError,"exact retained creator failure roster required");
        return nullptr;
    }
    const bool hasFailures = PyTuple_GET_SIZE(failureRows)!=0 ||
        PyTuple_GET_ITEM(failures,1)!=Py_None || PyTuple_GET_ITEM(failures,2)!=Py_None;
    qualificationFailed = qualificationFailed || hasFailures;
    const bool originalHeld=creatorHeldInputs&&creatorHeldInputs->captured&&
        PyTuple_GET_ITEM(originals,1)==creatorHeldInputs->tuple;
    const bool currentHeld=creatorHeldInputs&&creatorHeldInputs->CurrentFieldIsOriginal();
    qualificationFailed|=!originalHeld||!currentHeld;
    if(PyErr_Occurred())return nullptr; // Original lookup triple stays pending.
    struct FinalCreatorRecord {
        std::uint64_t version, kind, retainedOriginalGroups, retainedFailureRows;
        std::uint64_t priorCopiedBytes, priorWrittenBytes, failed;
    };
    const FinalCreatorRecord record{1,6,4,static_cast<std::uint64_t>(PyTuple_GET_SIZE(failureRows)),
        static_cast<std::uint64_t>(sink.CopiedOriginalBytes()),static_cast<std::uint64_t>(sink.WrittenOriginalBytes()),
        qualificationFailed ? 1u : 0u};
    // This is the LAST creator append. Strong originals and all native owner
    // referents remain resident while prefix readback/collector are in flight.
    creatorRecordAppended = sink.AppendOriginalBytes(&record,sizeof(record));
    creatorPrefixReadback = sink.FlushReadbackOriginal();
    // A failed write may still have a physically completed prefix. Start's
    // actual completion checks preserve unknown IO instead of reusing storage.
    creatorCollectorStarted = StartFinalCollector();
    if (creatorCollectorStarted) collector->ObserveSelectedClosure();
    if (!creatorRecordAppended || !creatorPrefixReadback || !creatorCollectorStarted || !CollectorQualified()) {
        qualificationFailed = true;
        if (!PyErr_Occurred()) PyErr_SetString(PyExc_RuntimeError,"final creator prefix capture or collector start failed");
        return nullptr;
    }
    Py_RETURN_NONE; // Collector terminal/join and host closure remain separate.
}
}
#endif

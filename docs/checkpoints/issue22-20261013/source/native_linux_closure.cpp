// UNCOMPILED final collector caller. Physical readback is not launch authority.
#ifndef _WIN32
#include "native_linux_platform.hpp"
namespace template_creator_bridge {
PyObject* NativePlatformState::VerifyClosure(){
    const auto fail=[&](const char* reason)->PyObject*{
        qualificationFailed=true;if(!PyErr_Occurred())PyErr_SetString(PyExc_RuntimeError,reason);return nullptr;
    };
    if(!creatorCaptureAttempted||!sinkPrepared)return fail("original Linux creator final capture absent");
    // Actual creator_entry calls this AFTER its last synchronous sink flush.
    // Future appends are source-closed; held descriptor/copies/observers stay live.
    if(!collectorStartAttempted){collectorStartAttempted=true;collectorStartReturned=collector.Start();}
    const bool disposed=collector.ObserveSelectedClosure();
    if(!disposed)Py_RETURN_FALSE; // Retained pending custody, never qualification.
    if(!collectorStartReturned||!collector.Qualified()||!creatorRecordAppended||!creatorPrefixReadback)
        return fail("original Linux final collector qualification failed");
    if(!commands.QualificationComplete()||!inputCapturesClean||qualificationFailed||!ExecutedOriginalCommandsQualified())
        return fail("original Linux command or input qualification incomplete");
    // This consumes completed command/input/collector outcomes only. Later
    // host joins and physical ingress disposal have their own main consumer.
    Py_RETURN_NONE;
}
}
#endif

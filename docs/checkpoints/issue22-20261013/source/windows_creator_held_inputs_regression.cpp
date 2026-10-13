// UNRUN/UNCOMPILED. Admitted harness supplies a fresh connected actual host,
// genuine nonempty selected steps, and resident observation storage.
#ifdef _WIN32
#include "windows_standalone_host.hpp"
#include "native_creator_held_inputs_regression.hpp"
namespace template_creator_bridge {
bool ConstructAndCheckOriginalHeldReplacement(OriginalWindowsStandaloneHost& host,
        OriginalHeldConstructionRegression& row,PyObject* actualSteps) {
    if(row.attempted||!host.wrapper||!host.platform||host.entryConstructionAttempted||
       host.creatorInvoked||!actualSteps||!PyTuple_CheckExact(actualSteps)||
       PyTuple_GET_SIZE(actualSteps)==0||PyErr_Occurred())return false;
    row.steps=Py_NewRef(actualSteps);row.attempted=true;
    row.constructionReturn=host.ConstructCreatorEntry(row.steps);
    row.returned=true; // Actual reference retained before error classification.
    if(!row.constructionReturn||PyErr_Occurred())return false;
    row.checked=OriginalHeldTupleReplacementRegression(row.original,row.constructionReturn);
    // No creator invocation follows mutation. The actual host/entry/observation
    // stay resident for separately admitted eligible cleanup and interpreter cut.
    return row.checked&&!host.creatorInvoked&&!PyErr_Occurred();
}
}
#endif

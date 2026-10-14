// UNRUN callable API for a separately admitted native harness. No main/run hook.
#pragma once
#include "native_creator_held_inputs.hpp"
namespace template_creator_bridge {
bool OriginalHeldTupleReplacementRegression(OriginalCreatorHeldInputs&,PyObject*);
#ifdef _WIN32
struct OriginalWindowsStandaloneHost;
struct OriginalHeldConstructionRegression {
    OriginalCreatorHeldInputs original;
    PyObject* steps=nullptr;
    PyObject* constructionReturn=nullptr;
    bool attempted=false,returned=false,checked=false;
};
bool ConstructAndCheckOriginalHeldReplacement(OriginalWindowsStandaloneHost&,
    OriginalHeldConstructionRegression&,PyObject*);
#endif
}

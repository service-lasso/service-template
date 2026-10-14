// UNRUN source identity regression; actual original entry comes from admitted
// TemplateCreator construction. Does not run creator/native qualification.
#include "native_creator_held_inputs_regression.hpp"
namespace template_creator_bridge {
bool OriginalHeldTupleReplacementRegression(OriginalCreatorHeldInputs& original,PyObject* actualConstructedEntry) {
    if(!original.Capture(actualConstructedEntry)||!original.tuple||
       PyTuple_GET_SIZE(original.tuple)==0||!original.CurrentFieldIsOriginal())return false;
    auto* first=PyTuple_GET_ITEM(original.tuple,0);
    if(!original.Contains(0,first))return false;
    // A distinct opaque member cannot become an original through field update.
    auto* replacementMember=PyObject_CallNoArgs(reinterpret_cast<PyObject*>(&PyBaseObject_Type));
    if(!replacementMember)return false;
    auto* replacement=PyTuple_Pack(1,replacementMember);
    if(!replacement){Py_DECREF(replacementMember);return false;}
    const int replaced=PyDict_SetItemString(original.dictionary,"_held",replacement);
    const bool denied=replaced==0&&!original.CurrentFieldIsOriginal()&&original.replacementObserved&&
        !original.Contains(0,replacementMember)&&original.Contains(0,first);
    // Restore the test-owned field; retained observation stays failed/changed.
    const int restored=PyDict_SetItemString(original.dictionary,"_held",original.tuple);
    const bool retained=restored==0&&original.CurrentFieldIsOriginal()&&original.replacementObserved&&
        original.Contains(0,first)&&original.tuple!=replacement;
    auto* empty=PyTuple_New(0);
    if(!empty){Py_DECREF(replacement);Py_DECREF(replacementMember);return false;}
    const int emptySet=PyDict_SetItemString(original.dictionary,"_held",empty);
    const bool emptyDenied=emptySet==0&&!original.CurrentFieldIsOriginal()&&original.Contains(0,first);
    const int emptyRestore=PyDict_SetItemString(original.dictionary,"_held",original.tuple);
    auto* differentDictionary=PyDict_New();
    bool dictionaryDenied=false,dictionaryRestored=false;
    if(differentDictionary&&PyDict_SetItemString(differentDictionary,"_held",original.tuple)==0){
        const int changed=PyObject_GenericSetDict(actualConstructedEntry,differentDictionary,nullptr);
        dictionaryDenied=changed==0&&!original.CurrentFieldIsOriginal()&&original.Contains(0,first);
        const int back=PyObject_GenericSetDict(actualConstructedEntry,original.dictionary,nullptr);
        dictionaryRestored=back==0&&original.CurrentFieldIsOriginal()&&original.replacementObserved;
    }
    Py_XDECREF(differentDictionary);Py_DECREF(empty);
    Py_DECREF(replacement);Py_DECREF(replacementMember);
    // Caller-preowned test cell retains its original entry/dict/tuple refs,
    // matching resident production ownership. Harness accounts/releases only
    // after admitted interpreter/native ownership settles; no early decref.
    return denied&&retained&&emptyDenied&&emptyRestore==0&&dictionaryDenied&&dictionaryRestored&&!PyErr_Occurred();
}
}

// UNCOMPILED actual creating-thread Linux host closure adapter. Resident entry,
// Python error triples and platform survive incomplete/unknown native custody.
#ifndef _WIN32
#include "native_linux_platform.hpp"
namespace template_creator_bridge {
bool NativeSessionCore::PrepareLinuxFinalPublicationInputs() noexcept {
    auto& row=hostClosure_;
    // Cached input disposal is only a pure fact. Every returned admission also
    // authorizes new main refcount/final IO progress, so actual owner checks
    // precede that cached success. Main diagnostic observations are not used.
    if(!OnOriginalPythonThread()||!OnCreatingNativeThread())return false;
    if(row.finalInputCutReturned)return true;
    if(row.finalInputCutOriginal)return false;
    row.finalInputCutAttempted=true;
    if(!row.pendingType&&!row.pendingValue&&!row.pendingTraceback&&PyErr_Occurred()){
        PyErr_Fetch(&row.pendingType,&row.pendingValue,&row.pendingTraceback);
        Py_XINCREF(row.pendingType);Py_XINCREF(row.pendingValue);Py_XINCREF(row.pendingTraceback);
        PyErr_Restore(row.pendingType,row.pendingValue,row.pendingTraceback);
    }
    try{if(platform_)row.finalInputCutReturned=platform_->PrepareFinalPublicationInputCut();}
    catch(...){row.finalInputCutOriginal=std::current_exception();row.qualificationFailed=true;}
    return row.finalInputCutReturned;
}
bool NativeSessionCore::FinishLinuxHostOriginals() noexcept {
    auto& row=hostClosure_;
    if(row.physicalDisposed||row.nativeException)return row.physicalDisposed;
    if(!OnOriginalPythonThread()||!OnCreatingNativeThread())return false;
    row.attempted=true;
    if(!row.pendingType&&!row.pendingValue&&!row.pendingTraceback&&PyErr_Occurred()){
        PyErr_Fetch(&row.pendingType,&row.pendingValue,&row.pendingTraceback);
        Py_XINCREF(row.pendingType);Py_XINCREF(row.pendingValue);Py_XINCREF(row.pendingTraceback);
        PyErr_Restore(row.pendingType,row.pendingValue,row.pendingTraceback);
    }
    try{
        if(!platform_)return false;
        row.preEntryDisposal=!creatorInvocation_.attempted;
        row.eligible=platform_->commands.AllPhysicalCommandsDisposed()&&
            platform_->sink.AllAttemptedOriginalCallsReturned()&&
            !platform_->pending.OriginalSupervisorOwner().HasPendingOriginalNativeCall();
        if(!row.eligible)return false;
        row.disposalReturned=platform_->ObservePhysicalHostClosureEligible();
        row.physicalDisposed=row.disposalReturned&&platform_->AllPhysicalOriginalsDisposed();
        if(row.physicalDisposed){
            row.linuxFinalHostConsumed=true;
            row.linuxFinalHostNormal=platform_->ConsumeSettledHostOriginals();
            row.qualificationFailed=!row.linuxFinalHostNormal||row.finalInputCutOriginal||
                row.pendingType||row.pendingValue||row.pendingTraceback||
                creatorInvocation_.nativeException||creatorInvocation_.pythonType||
                creatorInvocation_.pythonValue||creatorInvocation_.pythonTraceback;
        }
        // Exact owning-source results are consumed here after real joins; no
        // later external ACK or IO after supervisor stop is invented. Useful
        // EXEC/provider/actor gates remain independent command prerequisites.
    }catch(...){row.nativeException=std::current_exception();row.qualificationFailed=true;}
    return row.physicalDisposed;
}
}
#endif

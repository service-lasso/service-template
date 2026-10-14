// UNCOMPILED/UNRUN native regression requiring two actually admitted private
// regular single-link files: exact fixed CMD body and same-length changed body.
// This object creates neither file and fabricates no native results.
#pragma once
#include "windows_held_files.hpp"
namespace template_creator {
struct OriginalFixedStubRegression {
    NativeHeldFiles accepted{1,NativeHeldFiles::FixedNodeStubExtent()+1,NativeHeldFiles::FixedNodeStubExtent()+1};
    NativeHeldFiles changed{1,NativeHeldFiles::FixedNodeStubExtent()+1,NativeHeldFiles::FixedNodeStubExtent()+1};
    bool attempted=false,acceptedChecks=false,changedChecks=false;
    bool acceptedCloseAttempted=false,changedCloseAttempted=false,acceptedClosed=false,changedClosed=false;
    bool Observe(const wchar_t* exactPath,const wchar_t* changedPath) noexcept {
        if(attempted)return false;attempted=true;
        const SIZE_T good=accepted.CaptureFixedNodeStub(exactPath);
        const SIZE_T bad=changed.CaptureFixedNodeStub(changedPath);
        const unsigned char* body=nullptr;SIZE_T extent=0;
        acceptedChecks=good==0 && !accepted.Failed() && accepted.FileCount()==1 &&
            accepted.PrivateFiles()[0].fixedExpectedBodyOriginal==reinterpret_cast<const unsigned char*>(NativeHeldFiles::FixedNodeStubBody()) &&
            accepted.PrivateFiles()[0].fixedExpectedBodyCompared && accepted.PrivateFiles()[0].fixedExpectedBodyEqual &&
            accepted.ExactCapturedSourceSpan(0,body,extent) && extent==NativeHeldFiles::FixedNodeStubExtent() &&
            !memcmp(body,NativeHeldFiles::FixedNodeStubBody(),extent);
        body=nullptr;extent=99;
        changedChecks=bad==0 && changed.Failed() && changed.FileCount()==1 &&
            changed.PrivateFiles()[0].bytesCaptured && changed.PrivateFiles()[0].fixedExpectedBodyCompared &&
            !changed.PrivateFiles()[0].fixedExpectedBodyEqual &&
            changed.PrivateFiles()[0].bodyExtent==NativeHeldFiles::FixedNodeStubExtent() &&
            !changed.ExactCapturedSourceSpan(0,body,extent) && !body && extent==0;
        acceptedCloseAttempted=true;acceptedClosed=accepted.Dispose();
        changedCloseAttempted=true;changedClosed=changed.Dispose();
        return acceptedChecks&&changedChecks&&acceptedClosed&&changedClosed;
    }
};
}

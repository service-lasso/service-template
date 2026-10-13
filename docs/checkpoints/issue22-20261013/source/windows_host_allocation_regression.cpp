// UNRUN/UNCOMPILED. Only an admitted standalone TEST process that exclusively
// owns its CPython allocator/interpreter may call this regression. Never install
// this allocator in the product host, another interpreter, or a shared worker.
// A fresh precharged actual host and real test-owned root/tool/sink/evidence
// capabilities are required. No replacement platform/session/issuer is used.
#ifdef _WIN32
#include "windows_standalone_host.hpp"
namespace template_creator_bridge::regression {
enum class AllocationOperation { Malloc, Calloc, Realloc, Free };
struct AllocationOriginal {
    AllocationOperation operation{};
    void* input=nullptr;
    void* output=nullptr;
    size_t first=0,second=0;
    size_t allocationOrdinal=0;
    DWORD callbackThread=0;
    bool injected=false;
};
struct AllocationFailureObservation {
    SRWLOCK rosterLock=SRWLOCK_INIT;
    SRWLOCK callbackLock=SRWLOCK_INIT;
    PyMemAllocatorEx previous{};
    std::unique_ptr<AllocationOriginal[]> calls;
    size_t capacity=0,count=0,allocations=0,failOrdinal=0;
    bool overflow=false,installed=false,restored=false;
    bool connected=false,exactBoundary=false,transferred=false;
    bool selectedInjectionRetained=false;
    bool sessionDisposed=false,hostDisposed=false,failedQualification=false;
    PyObject* errorType=nullptr;
    PyObject* errorValue=nullptr;
    PyObject* errorTraceback=nullptr;
    explicit AllocationFailureObservation(size_t selectedCalls,size_t selectedFailureOrdinal)
        : calls(std::make_unique<AllocationOriginal[]>(selectedCalls)),capacity(selectedCalls),failOrdinal(selectedFailureOrdinal) {}
private:
    ~AllocationFailureObservation()=default; // Test custodian retains callback context, never stack/delete.
public:
    AllocationOriginal* Reserve(AllocationOperation operation,void* input,size_t first,size_t second,bool allocate) noexcept {
        AcquireSRWLockExclusive(&rosterLock);
        if (count==capacity) { overflow=true; ReleaseSRWLockExclusive(&rosterLock); return nullptr; }
        auto& row=calls[count++]; row.operation=operation; row.input=input; row.first=first; row.second=second;
        row.callbackThread=GetCurrentThreadId();
        if (allocate) { row.allocationOrdinal=++allocations; row.injected=row.allocationOrdinal==failOrdinal; }
        ReleaseSRWLockExclusive(&rosterLock);
        return &row;
    }
    static void* Malloc(void* context,size_t bytes) {
        auto& owner=*static_cast<AllocationFailureObservation*>(context);
        AcquireSRWLockShared(&owner.callbackLock);
        auto* row=owner.Reserve(AllocationOperation::Malloc,nullptr,bytes,0,bytes!=0);
        // Recording exhaustion remains a failed test, but must not silently
        // suppress an independent allocator/free operation of the interpreter.
        void* result=row && row->injected ? nullptr : owner.previous.malloc(owner.previous.ctx,bytes);
        if (row) row->output=result;
        ReleaseSRWLockShared(&owner.callbackLock); return result;
    }
    static void* Calloc(void* context,size_t count,size_t bytes) {
        auto& owner=*static_cast<AllocationFailureObservation*>(context);
        AcquireSRWLockShared(&owner.callbackLock);
        auto* row=owner.Reserve(AllocationOperation::Calloc,nullptr,count,bytes,count!=0 && bytes!=0);
        void* result=row && row->injected ? nullptr : owner.previous.calloc(owner.previous.ctx,count,bytes);
        if (row) row->output=result;
        ReleaseSRWLockShared(&owner.callbackLock); return result;
    }
    static void* Realloc(void* context,void* input,size_t bytes) {
        auto& owner=*static_cast<AllocationFailureObservation*>(context);
        AcquireSRWLockShared(&owner.callbackLock);
        auto* row=owner.Reserve(AllocationOperation::Realloc,input,bytes,0,bytes!=0);
        void* result=row && row->injected ? nullptr : owner.previous.realloc(owner.previous.ctx,input,bytes);
        if (row) row->output=result;
        ReleaseSRWLockShared(&owner.callbackLock); return result;
    }
    static void Free(void* context,void* input) {
        auto& owner=*static_cast<AllocationFailureObservation*>(context);
        AcquireSRWLockShared(&owner.callbackLock);
        owner.Reserve(AllocationOperation::Free,input,0,0,false);
        owner.previous.free(owner.previous.ctx,input);
        ReleaseSRWLockShared(&owner.callbackLock);
    }
};
// Selected failure ordinal is derived by the admitted harness from its actual
// installed CPython allocation route. Do not assume ordinal1/2 has a universal
// meaning. The predicate below requires the ACTUAL post-transfer boundary.
bool OriginalPostTransferAllocationFailure(OriginalWindowsStandaloneHost& host,
    const std::array<HANDLE,3>& actualCapabilities,HANDLE actualEvidenceRoot,
    const void* actualBody,DWORD actualBodyBytes,AllocationFailureObservation& row,bool requireWrapperBoundary) {
    if (host.connectAttempted || !host.core || !row.capacity || !row.failOrdinal || PyErr_Occurred()) return false;
    PyMem_GetAllocator(PYMEM_DOMAIN_OBJ,&row.previous);
    PyMemAllocatorEx injected{&row,AllocationFailureObservation::Malloc,AllocationFailureObservation::Calloc,
        AllocationFailureObservation::Realloc,AllocationFailureObservation::Free};
    row.installed=true;
    PyMem_SetAllocator(PYMEM_DOMAIN_OBJ,&injected);
    row.connected=host.Connect(actualCapabilities,actualEvidenceRoot,actualBody,actualBodyBytes);
    // Always restore before Python diagnostics, native cleanup or returning to
    // the harness. Connect is noexcept; no stack unwinding can leave it installed.
    // Wait for currently executing callbacks before restoration. The admitted
    // harness must also exclude other allocator users/threads from this entire
    // interval; this lock is not proof that no foreign thread cached a callback.
    // Context remains resident even on failed/exhausted/quiescence-unknown runs.
    AcquireSRWLockExclusive(&row.callbackLock);
    PyMem_SetAllocator(PYMEM_DOMAIN_OBJ,&row.previous);
    ReleaseSRWLockExclusive(&row.callbackLock);
    row.restored=true;
    size_t injectedCount=0;
    for (size_t i=0;i<row.count;++i) {
        const auto& original=row.calls[i];
        if (!original.injected) continue;
        ++injectedCount;
        const bool geometry=original.operation==AllocationOperation::Malloc ? !original.input && original.first!=0 && original.second==0 :
            original.operation==AllocationOperation::Calloc ? !original.input && original.first!=0 && original.second!=0 :
            original.operation==AllocationOperation::Realloc && original.first!=0 && original.second==0;
        row.selectedInjectionRetained=original.allocationOrdinal==row.failOrdinal && original.output==nullptr && geometry &&
            original.callbackThread==GetCurrentThreadId() && host.connectAttempted;
    }
    row.selectedInjectionRetained=row.selectedInjectionRetained && injectedCount==1 && row.allocations>=row.failOrdinal;
    row.transferred=host.transferred && host.platform && host.handoff.captured &&
        host.platform->transferredOriginals==host.handoff.OriginalCapabilities();
    row.exactBoundary=!row.connected && row.transferred && !host.wrapper &&
        PyErr_ExceptionMatches(PyExc_MemoryError) &&
        (requireWrapperBoundary ? host.platform->originalIngressIdentity!=nullptr && host.platform->ingressBound :
            host.platform->originalIngressIdentity==nullptr && host.core->PrivatePlatformPythonType()!=nullptr);
    PyErr_Fetch(&row.errorType,&row.errorValue,&row.errorTraceback);
    Py_XINCREF(row.errorType); Py_XINCREF(row.errorValue); Py_XINCREF(row.errorTraceback);
    PyErr_Restore(row.errorType,row.errorValue,row.errorTraceback);
    row.sessionDisposed=host.ObserveHostClose();
    row.hostDisposed=host.CloseHostOwnedEligible();
    row.failedQualification=host.core->PrivateHostClosure().qualificationFailed;
    return row.exactBoundary && row.selectedInjectionRetained && !row.overflow && row.restored && row.sessionDisposed && row.hostDisposed &&
        host.core->PrivateHostClosure().preEntryDisposal && row.failedQualification &&
        host.core->PrivateHostClosure().pendingValue==row.errorValue;
}
// A C++ factory throw BEFORE transfer remains a separate native allocation
// injection case; this CPython domain wrapper cannot manufacture that outcome.
}
#endif

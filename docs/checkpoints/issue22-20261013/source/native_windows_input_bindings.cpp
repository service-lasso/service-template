// UNCOMPILED physical input capture/binding. Layout/requirements are precharge
// selections, not trust grants or complete source/native dependency proof.
#ifdef _WIN32
#include "native_windows_platform.hpp"
#include "windows_standalone_host.hpp"
#include "windows_original_capabilities.hpp"
namespace template_creator_bridge {
static PyObject* InputError(NativePlatformState& state,const char* message) noexcept {
    state.qualificationFailed = true;
    if (!PyErr_Occurred()) PyErr_SetString(PyExc_ValueError,message); return nullptr;
}
PyObject* NativePlatformState::CaptureInputClasses(PyObject* ordinal,PyObject* step) {
    if(!ordinal||!PyLong_CheckExact(ordinal)||!adopted||!stepsBound||qualificationFailed||
       inputBindingAttempted||!stepsOriginal||!PyTuple_CheckExact(stepsOriginal))
        return InputError(*this,"original Windows class-view phase");
    const size_t index=PyLong_AsSize_t(ordinal);
    if(PyErr_Occurred()||index>=static_cast<size_t>(PyTuple_GET_SIZE(stepsOriginal))||
       PyTuple_GET_ITEM(stepsOriginal,index)!=step||!step||!PyTuple_Check(step)||PyTuple_GET_SIZE(step)!=5)
        return InputError(*this,"same original Windows class-view step");
    auto& owner=inputOwners[index+1]; // Route zero is the original bootstrap.
    if(owner.classViewAttempted||!owner.layoutBound)return InputError(*this,"original Windows class view already attempted");
    owner.classViewAttempted=true;
    auto* requirements=PyTuple_GET_ITEM(step,2);
    if(!requirements||!PyTuple_CheckExact(requirements))return InputError(*this,"original Windows class-view requirements");
    auto* result=PyTuple_New(PyTuple_GET_SIZE(requirements));if(!result)return nullptr;
    for(Py_ssize_t i=0;i<PyTuple_GET_SIZE(requirements);++i)PyTuple_SET_ITEM(result,i,Py_NewRef(Py_None));
    owner.classViewOriginal=Py_NewRef(result);
    return result; // Current Windows path retains independent complete-file capture.
}
bool NativePlatformState::BindOriginalBootstrap(PyObject* snapshot) {
    if (bootstrapBindingAttempted) return false; bootstrapBindingAttempted = true;
    bootstrapSnapshotOriginal = Py_XNewRef(snapshot);
    if (!snapshot || !PyTuple_Check(snapshot) || PyTuple_GET_SIZE(snapshot)!=5) {
        InputError(*this,"actual original bootstrap snapshot required"); return false;
    }
    return true;
}
bool NativePlatformState::PreownInputLayout(SIZE_T ordinal,SIZE_T callerRows,SIZE_T readRows,
    const template_creator::GateImageGeometry* geometry,SIZE_T imageRows,SIZE_T originalNtdllOrdinal) {
    if (inputBindingAttempted || ordinal>=route.SelectedCount()) return false;
    auto& owner = inputOwners[ordinal];
    if (owner.layoutAttempted) return false; owner.layoutAttempted = true;
    if (!callerRows || !readRows || readRows>65536 || !geometry || !imageRows || imageRows>SIZE_MAX/sizeof(template_creator::GateImageGeometry)) {
        InputError(*this,"selected original native observation layout required"); return false;
    }
    try {
        owner.geometry = std::make_unique<template_creator::GateImageGeometry[]>(imageRows);
        for (SIZE_T i=0;i<imageRows;++i) owner.geometry[i] = geometry[i];
    } catch (...) { owner.originalException = std::current_exception(); qualificationFailed = true; throw; }
    owner.imageRows = imageRows; owner.callerRows = callerRows; owner.readRows = readRows; owner.ntdllOrdinal = originalNtdllOrdinal;
    owner.layoutBound = true; return true;
}
static bool Requirements(PyObject* snapshot,PyObject*& requirements) noexcept {
    if (!snapshot || !PyTuple_Check(snapshot) || PyTuple_GET_SIZE(snapshot)!=5) return false;
    requirements = PyTuple_GET_ITEM(snapshot,4);
    return requirements && PyTuple_CheckExact(requirements) && PyTuple_GET_SIZE(requirements)>0 && PyTuple_GET_SIZE(requirements)<=1024;
}
static bool Requirement(PyObject* original,PyObject*& path,uint64_t& size,std::array<unsigned char,32>& hash) noexcept {
    if (!original || !PyTuple_Check(original) || PyTuple_GET_SIZE(original)!=4) return false;
    path = PyTuple_GET_ITEM(original,1); auto* digest = PyTuple_GET_ITEM(original,2); auto* extent = PyTuple_GET_ITEM(original,3);
    if (!path || !PyUnicode_CheckExact(path) || !digest || !PyUnicode_CheckExact(digest) || PyUnicode_GET_LENGTH(digest)!=64 ||
        !extent || !PyLong_CheckExact(extent)) return false;
    size = PyLong_AsUnsignedLongLong(extent);
    if (PyErr_Occurred() || size>1024ull*1024*1024) return false;
    for (Py_ssize_t i=0;i<64;++i) {
        const auto c = PyUnicode_READ_CHAR(digest,i);
        unsigned value = c>='0' && c<='9' ? c-'0' : c>='a' && c<='f' ? c-'a'+10 : c>='A' && c<='F' ? c-'A'+10 : 16;
        if (value==16) return false;
        if (!(i&1)) hash[static_cast<size_t>(i)/2] = static_cast<unsigned char>(value<<4);
        else hash[static_cast<size_t>(i)/2] |= static_cast<unsigned char>(value);
    }
    return true;
}
static bool Path(PyObject* original,std::array<wchar_t,32768>& destination) noexcept {
    SIZE_T units = 0; const auto length = PyUnicode_GET_LENGTH(original);
    for (Py_ssize_t i=0;i<length;++i) {
        auto scalar = PyUnicode_READ_CHAR(original,i);
        if (!scalar || scalar>0x10ffff || (scalar>=0xd800 && scalar<=0xdfff)) return false;
        const SIZE_T need = scalar>0xffff ? 2 : 1;
        if (units>destination.size()-1-need) return false; units += need;
    }
    SIZE_T offset = 0;
    for (Py_ssize_t i=0;i<length;++i) {
        auto scalar = PyUnicode_READ_CHAR(original,i);
        if (scalar<=0xffff) destination[offset++] = static_cast<wchar_t>(scalar);
        else { scalar-=0x10000; destination[offset++]=static_cast<wchar_t>(0xd800+(scalar>>10)); destination[offset++]=static_cast<wchar_t>(0xdc00+(scalar&1023)); }
    }
    destination[offset]=0; return offset>0;
}
PyObject* NativePlatformState::BindInputs(PyObject* inputs,PyObject* snapshots) {
    if (inputBindingAttempted) return InputError(*this,"original input binding already attempted");
    inputBindingAttempted = true; inputObjectsOriginal = Py_XNewRef(inputs); snapshotsOriginal = Py_XNewRef(snapshots);
    if (!sourceObjectsAssociated || !stepsBound || qualificationFailed || !bootstrapSnapshotOriginal || !inputs || !PyTuple_CheckExact(inputs) ||
        !snapshots || !PyTuple_CheckExact(snapshots) || PyTuple_GET_SIZE(snapshots)!=PyTuple_GET_SIZE(stepsOriginal) ||
        PyTuple_GET_SIZE(inputs)!=PyTuple_GET_SIZE(snapshots)) return InputError(*this,"complete original native input roster required");
    // Allocate ALL file owners before the first native open/capture. Any later
    // failure leaves original handles/errors reachable in these retained cells.
    for (SIZE_T ordinal=0;ordinal<route.SelectedCount();++ordinal) {
        auto& owner = inputOwners[ordinal];
        if (!owner.layoutBound) return InputError(*this,"native observation layout not preowned");
        owner.snapshot = Py_NewRef(ordinal ? PyTuple_GET_ITEM(snapshots,static_cast<Py_ssize_t>(ordinal-1)) : bootstrapSnapshotOriginal);
        PyObject* requirements = nullptr;
        if (!Requirements(owner.snapshot,requirements)) return InputError(*this,"original physical requirements absent");
        SIZE_T bytes = 0, reads = 0;
        for (Py_ssize_t i=0;i<PyTuple_GET_SIZE(requirements);++i) {
            PyObject* path = nullptr; uint64_t size = 0; std::array<unsigned char,32> digest{};
            if (!Requirement(PyTuple_GET_ITEM(requirements,i),path,size,digest)) return InputError(*this,"original physical requirement shape");
            const auto requiredReads = static_cast<SIZE_T>((size+65535)/65536+1);
            if (size+1>1024ull*1024*1024-bytes || requiredReads>65536-reads)
                return InputError(*this,"selected native capture capacity exhausted");
            bytes += static_cast<SIZE_T>(size)+1; reads += requiredReads;
        }
        if (owner.ntdllOrdinal>=static_cast<SIZE_T>(PyTuple_GET_SIZE(requirements))) return InputError(*this,"original native ntdll input association absent");
        if (reads>owner.readRows) return InputError(*this,"selected read roster below necessary minimum");
        // The necessary minimum is not a complete native short-read budget.
        // Allocate the actual separately selected finite roster, retaining every
        // short read; exhaustion is failed qualification, never assumed filling.
        try { owner.files = std::make_unique<template_creator::NativeHeldFiles>(static_cast<SIZE_T>(PyTuple_GET_SIZE(requirements)),owner.readRows,bytes); }
        catch (...) { owner.originalException = std::current_exception(); qualificationFailed = true; throw; }
    }
    for (SIZE_T ordinal=0;ordinal<route.SelectedCount();++ordinal) {
        auto& owner = inputOwners[ordinal]; owner.captureAttempted = true;
        PyObject* requirements = nullptr; Requirements(owner.snapshot,requirements);
        for (Py_ssize_t i=0;i<PyTuple_GET_SIZE(requirements);++i) {
            PyObject* path = nullptr; uint64_t size = 0; std::array<unsigned char,32> digest{};
            std::array<wchar_t,32768> literal{};
            if (!Requirement(PyTuple_GET_ITEM(requirements,i),path,size,digest) || !Path(path,literal) ||
                owner.files->Capture(literal.data(),size,digest)!=static_cast<SIZE_T>(i) ||
                owner.files->Failed() || !owner.files->PrivateFiles()[static_cast<SIZE_T>(i)].valid)
                return InputError(*this,"original native physical input capture failed");
        }
        if(ordinal) {
            // The unchanged generation/final argv runs relative to this exact
            // candidate root. Bootstrap cwd has its separate input selection.
            PyObject* command=PyTuple_GET_ITEM(owner.snapshot,0);
            if(!command || !PyTuple_Check(command) || PyTuple_GET_SIZE(command)!=6 ||
                !PyUnicode_CheckExact(PyTuple_GET_ITEM(command,2)) ||
                !Path(PyTuple_GET_ITEM(command,2),owner.directoryPath))
                return InputError(*this,"protected command cwd original shape");
            owner.directoryAttempted=true;
            owner.directoryReturned=CreateFileW(owner.directoryPath.data(),
                FILE_LIST_DIRECTORY|FILE_TRAVERSE|FILE_READ_ATTRIBUTES|READ_CONTROL|SYNCHRONIZE,
                FILE_SHARE_READ,nullptr,OPEN_EXISTING,
                FILE_FLAG_BACKUP_SEMANTICS,nullptr);
            if(owner.directoryReturned==INVALID_HANDLE_VALUE) {
                owner.directoryError=GetLastError();return InputError(*this,"protected cwd native open failed");
            }
            owner.directoryAcquired=owner.directoryReturned!=nullptr;
            if(!owner.directoryAcquired) {
                owner.directoryUnknown=true;return InputError(*this,"protected cwd native output unknown");
            }
            owner.directoryIdentity.Capture(owner.directoryReturned);
            const auto& expected=sourceTransferredIdentities[0];
            const auto& observed=owner.directoryIdentity;
            owner.directoryAssociated=observed.captured && expected.captured && observed.standard.Directory &&
                observed.id.VolumeSerialNumber==expected.id.VolumeSerialNumber &&
                memcmp(observed.id.FileId.Identifier,expected.id.FileId.Identifier,
                    sizeof(observed.id.FileId.Identifier))==0;
            if(!owner.directoryAssociated)return InputError(*this,"protected cwd differs from original candidate root");
        }
        try {
            if(!pendingCallsOriginal)return InputError(*this,"same host pending owner absent");
            if(ordinal&&!sourceCapabilitiesOriginal)return InputError(*this,"actual private fixture-root owner absent");
            if (!route.Precharge(ordinal,owner.snapshot,ingress,*owner.files,owner.callerRows,
                owner.geometry.get(),owner.imageRows,owner.ntdllOrdinal,*pendingCallsOriginal,
                ordinal?&sourceCapabilitiesOriginal->PrivateFixtureRoot():nullptr)) return InputError(*this,"native command/gate precharge failed");
        } catch (...) { owner.originalException = std::current_exception(); qualificationFailed = true; throw; }
    }
    if (!route.Seal() || !commands.BindSnapshots(bootstrapSnapshotOriginal,snapshots)) return InputError(*this,"native route binding failed");
    inputCapturesClean = true; // Physical capture only; LaunchAssociationsEstablished is distinct.
    if(!sourceHostOriginal || !sourceHostOriginal->StartOriginalNoticeForRoute(*this))
        return InputError(*this,"original None notice workers not started");
    Py_RETURN_NONE;
}
}
#endif

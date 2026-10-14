// UNCOMPILED private post-close destination. The caller supplies an actual
// separately admitted evidence-directory capability; no path/JSON/native-rights
// declaration substitutes for it. Never use the candidate input root here.
#pragma once
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#include <winternl.h>
#include <bcrypt.h>
#include "windows_native_sink.hpp"
namespace template_creator_bridge {
struct OriginalWindowsFinalOutput {
private:
    friend struct OriginalWindowsStandaloneHost;
    ~OriginalWindowsFinalOutput()=default;
public:
    using CreateFileNative=NTSTATUS (NTAPI*)(PHANDLE,ACCESS_MASK,POBJECT_ATTRIBUTES,PIO_STATUS_BLOCK,
        PLARGE_INTEGER,ULONG,ULONG,ULONG,ULONG,PVOID,ULONG);
    template_creator::OriginalWindowsSink sink;
    HANDLE borrowedRoot=nullptr, root=nullptr, file=nullptr;
    HANDLE duplicateSourceProcess=nullptr, duplicateTargetProcess=nullptr;
    HMODULE originalNtdll=nullptr;
    BOOL moduleReturned=FALSE, moduleCloseReturned=FALSE;
    DWORD moduleCloseError=0;
    bool moduleAcquired=false, moduleUnknown=false, moduleCloseAttempted=false;
    FARPROC originalCreateAddress=nullptr;
    FILE_ID_INFO rootIdentity{}, candidateIdentity{};
    FILE_STANDARD_INFO rootStandard{};
    BOOL duplicateReturned=FALSE, rootIdentityReturned=FALSE, candidateIdentityReturned=FALSE, rootStandardReturned=FALSE;
    DWORD duplicateError=0, rootIdentityError=0, candidateIdentityError=0, rootStandardError=0, moduleError=0, addressError=0;
    std::array<unsigned char,16> random{};
    std::array<wchar_t,64> name{};
    NTSTATUS randomReturned=0, createReturned=0;
    UNICODE_STRING originalName{};
    OBJECT_ATTRIBUTES originalAttributes{};
    IO_STATUS_BLOCK originalStatus{};
    bool createAttempted=false, adoptAttempted=false, adopted=false, creationUnknown=false;
    bool fileAcquired=false, rootAcquired=false;
    bool fileCloseAttempted=false, rootCloseAttempted=false;
    BOOL fileCloseReturned=FALSE, rootCloseReturned=FALSE;
    DWORD fileCloseError=0, rootCloseError=0;
    OriginalWindowsFinalOutput(SIZE_T originalBytes,SIZE_T readbackBytes,SIZE_T calls):sink(originalBytes,readbackBytes,calls) {}
    OriginalWindowsFinalOutput(const OriginalWindowsFinalOutput&)=delete;
    OriginalWindowsFinalOutput& operator=(const OriginalWindowsFinalOutput&)=delete;
    bool Create(HANDLE actualEvidenceRoot,HANDLE actualCandidateRoot) noexcept {
        if (adoptAttempted) return false; adoptAttempted=true; borrowedRoot=actualEvidenceRoot;
        if (!actualEvidenceRoot || actualEvidenceRoot==INVALID_HANDLE_VALUE ||
            !actualCandidateRoot || actualCandidateRoot==INVALID_HANDLE_VALUE) return false;
        duplicateSourceProcess=GetCurrentProcess(); duplicateTargetProcess=GetCurrentProcess();
        duplicateReturned=DuplicateHandle(duplicateSourceProcess,actualEvidenceRoot,duplicateTargetProcess,&root,0,FALSE,DUPLICATE_SAME_ACCESS);
        if (!duplicateReturned) { duplicateError=GetLastError(); creationUnknown=root!=nullptr; return false; }
        rootAcquired=root && root!=INVALID_HANDLE_VALUE;
        if (!rootAcquired) { creationUnknown=true; return false; }
        rootIdentityReturned=GetFileInformationByHandleEx(root,FileIdInfo,&rootIdentity,sizeof(rootIdentity));
        if (!rootIdentityReturned) rootIdentityError=GetLastError();
        candidateIdentityReturned=GetFileInformationByHandleEx(actualCandidateRoot,FileIdInfo,&candidateIdentity,sizeof(candidateIdentity));
        if (!candidateIdentityReturned) candidateIdentityError=GetLastError();
        rootStandardReturned=GetFileInformationByHandleEx(root,FileStandardInfo,&rootStandard,sizeof(rootStandard));
        if (!rootStandardReturned) rootStandardError=GetLastError();
        if (!rootIdentityReturned || !candidateIdentityReturned || !rootStandardReturned || !rootStandard.Directory ||
            (rootIdentity.VolumeSerialNumber==candidateIdentity.VolumeSerialNumber &&
             memcmp(rootIdentity.FileId.Identifier,candidateIdentity.FileId.Identifier,sizeof(rootIdentity.FileId.Identifier))==0)) return false;
        randomReturned=BCryptGenRandom(nullptr,random.data(),static_cast<ULONG>(random.size()),BCRYPT_USE_SYSTEM_PREFERRED_RNG);
        if (randomReturned!=0) return false;
        constexpr wchar_t prefix[]=L"creator-final-", hex[]=L"0123456789abcdef";
        SIZE_T used=0;
        for (;used<sizeof(prefix)/sizeof(wchar_t)-1;++used) name[used]=prefix[used];
        for (auto value:random) { name[used++]=hex[value>>4]; name[used++]=hex[value&15]; }
        name[used]=0;
        originalName.Buffer=name.data(); originalName.Length=static_cast<USHORT>(used*sizeof(wchar_t));
        originalName.MaximumLength=static_cast<USHORT>((used+1)*sizeof(wchar_t));
        originalAttributes.Length=sizeof(originalAttributes); originalAttributes.RootDirectory=root;
        originalAttributes.ObjectName=&originalName; originalAttributes.Attributes=0; // No inheritance/kernel/backup privilege.
        moduleReturned=GetModuleHandleExW(0,L"ntdll.dll",&originalNtdll);
        if (!moduleReturned) { moduleError=GetLastError(); moduleUnknown=originalNtdll!=nullptr; return false; }
        moduleAcquired=originalNtdll!=nullptr;
        if (!moduleAcquired) { moduleUnknown=true; return false; }
        originalCreateAddress=GetProcAddress(originalNtdll,"NtCreateFile");
        if (!originalCreateAddress) { addressError=GetLastError(); return false; }
        // FILE_CREATE=2, SYNCHRONOUS_IO_NONALERT=0x20,
        // NON_DIRECTORY_FILE=0x40, OPEN_REPARSE_POINT=0x200000.
        // Relative one-component creation stays beneath this held directory;
        // no absolute path reopen, overwrite, delete-on-close or collision retry.
        createAttempted=true;
        createReturned=reinterpret_cast<CreateFileNative>(originalCreateAddress)(&file,
            FILE_READ_DATA|FILE_WRITE_DATA|FILE_READ_ATTRIBUTES|SYNCHRONIZE,&originalAttributes,&originalStatus,
            nullptr,FILE_ATTRIBUTE_NORMAL,FILE_SHARE_READ,2,0x20|0x40|0x200000,nullptr,0);
        creationUnknown=createReturned==static_cast<NTSTATUS>(0x103) || originalStatus.Status==static_cast<NTSTATUS>(0x103) ||
            (createReturned!=0 && file!=nullptr) || (createReturned==0 && (!file || file==INVALID_HANDLE_VALUE || file==root));
        fileAcquired=createReturned==0 && file && file!=INVALID_HANDLE_VALUE && !creationUnknown;
        if (createReturned!=0 || creationUnknown || originalStatus.Status!=0 || originalStatus.Information!=2) return false;
        adopted=sink.AdoptBorrowedOriginal(file);
        return adopted; // Root/loader/rights authority remains a separate original association.
    }
    bool DisposeEligible() noexcept {
        bool all=!creationUnknown && !moduleUnknown && sink.CompletionKnown();
        if (fileAcquired && sink.CompletionKnown() && !creationUnknown && !fileCloseAttempted) {
            fileCloseAttempted=true; fileCloseReturned=CloseHandle(file);
            if (!fileCloseReturned) fileCloseError=GetLastError();
        }
        if (rootAcquired && !creationUnknown && !rootCloseAttempted) {
            rootCloseAttempted=true; rootCloseReturned=CloseHandle(root);
            if (!rootCloseReturned) rootCloseError=GetLastError();
        }
        if (moduleAcquired && !creationUnknown && !moduleCloseAttempted) {
            moduleCloseAttempted=true; moduleCloseReturned=FreeLibrary(originalNtdll);
            if (!moduleCloseReturned) moduleCloseError=GetLastError();
        }
        return all && (!fileAcquired || fileCloseReturned) && (!rootAcquired || rootCloseReturned) &&
            (!moduleAcquired || moduleCloseReturned);
    }
    // Keep this owner and its complete native outputs/copies resident through
    // independent receipt readback. No destructor deletes private evidence.
};
}

// UNCOMPILED source-owned private root acquisition. This is not prefix/path
// authority, a directory seal, or admission of its future generated children.
#pragma once
#include <windows.h>
#include <winternl.h>
#include <bcrypt.h>
#include "windows_identity.hpp"
namespace template_creator {
class OriginalWindowsFixtureRoot {
public:
    using NativeCreate=NTSTATUS (NTAPI*)(PHANDLE,ACCESS_MASK,POBJECT_ATTRIBUTES,
        PIO_STATUS_BLOCK,PLARGE_INTEGER,ULONG,ULONG,ULONG,ULONG,PVOID,ULONG);
    HANDLE borrowedParent=nullptr, parent=nullptr, root=nullptr;
    HANDLE duplicateSourceProcess=nullptr,duplicateTargetProcess=nullptr;
    BOOL parentDuplicateReturned=FALSE;
    DWORD parentDuplicateError=0;
    bool attempted=false,parentAcquired=false,parentUnknown=false;
    bool createAttempted=false,rootAcquired=false,rootUnknown=false;
    bool moduleAcquired=false,moduleUnknown=false;
    HMODULE module=nullptr;
    BOOL moduleReturned=FALSE;
    DWORD moduleError=0,addressError=0;
    FARPROC createAddress=nullptr;
    std::array<unsigned char,16> random{};
    std::array<wchar_t,64> component{};
    NTSTATUS randomReturned=0,createReturned=0;
    UNICODE_STRING originalName{};
    OBJECT_ATTRIBUTES originalAttributes{};
    IO_STATUS_BLOCK originalStatus{};
    FileIdentityObservation parentIdentity,rootIdentity;
    FileIdentityObservation environmentIdentity;
    HANDLE environmentOpened=INVALID_HANDLE_VALUE;
    DWORD environmentOpenError=0,environmentCloseError=0;
    bool environmentOpenAttempted=false,environmentAcquired=false,environmentUnknown=false;
    bool environmentCloseAttempted=false;
    BOOL environmentCloseReturned=FALSE;
    std::array<wchar_t,32768> dosPath{},environmentPath{};
    DWORD dosPathReturned=0,dosPathError=0;
    SIZE_T environmentPathUnits=0;
    bool dosPathAttempted=false,dosPathTooSmall=false,environmentPathBound=false;
    bool created=false;
    bool rootCloseAttempted=false,parentCloseAttempted=false,moduleCloseAttempted=false;
    BOOL rootCloseReturned=FALSE,parentCloseReturned=FALSE,moduleCloseReturned=FALSE;
    DWORD rootCloseError=0,parentCloseError=0,moduleCloseError=0;
    FILE_DISPOSITION_INFO originalDisposition{TRUE};
    bool dispositionAttempted=false;
    BOOL dispositionReturned=FALSE;
    DWORD dispositionError=0;
    OriginalWindowsFixtureRoot()=default;
    OriginalWindowsFixtureRoot(const OriginalWindowsFixtureRoot&)=delete;
    OriginalWindowsFixtureRoot& operator=(const OriginalWindowsFixtureRoot&)=delete;
    bool Create(HANDLE actualPrivateParent) noexcept {
        if(attempted)return false;attempted=true;borrowedParent=actualPrivateParent;
        if(!actualPrivateParent||actualPrivateParent==INVALID_HANDLE_VALUE)return false;
        duplicateSourceProcess=GetCurrentProcess();duplicateTargetProcess=GetCurrentProcess();
        parentDuplicateReturned=DuplicateHandle(duplicateSourceProcess,actualPrivateParent,
            duplicateTargetProcess,&parent,0,FALSE,DUPLICATE_SAME_ACCESS);
        if(!parentDuplicateReturned) {
            parentDuplicateError=GetLastError();parentUnknown=parent!=nullptr;return false;
        }
        parentAcquired=parent&&parent!=INVALID_HANDLE_VALUE;
        if(!parentAcquired){parentUnknown=true;return false;}
        parentIdentity.Capture(parent);
        if(!parentIdentity.captured||!parentIdentity.standard.Directory||
            (parentIdentity.basic.FileAttributes&FILE_ATTRIBUTE_REPARSE_POINT))return false;
        randomReturned=BCryptGenRandom(nullptr,random.data(),static_cast<ULONG>(random.size()),
            BCRYPT_USE_SYSTEM_PREFERRED_RNG);
        if(randomReturned!=0)return false;
        constexpr wchar_t prefix[]=L"creator-fixture-",hex[]=L"0123456789abcdef";
        SIZE_T used=0;
        for(;used<sizeof(prefix)/sizeof(wchar_t)-1;++used)component[used]=prefix[used];
        for(auto byte:random){component[used++]=hex[byte>>4];component[used++]=hex[byte&15];}
        component[used]=0;
        originalName.Buffer=component.data();
        originalName.Length=static_cast<USHORT>(used*sizeof(wchar_t));
        originalName.MaximumLength=static_cast<USHORT>((used+1)*sizeof(wchar_t));
        originalAttributes.Length=sizeof(originalAttributes);
        originalAttributes.RootDirectory=parent;
        originalAttributes.ObjectName=&originalName;
        moduleReturned=GetModuleHandleExW(0,L"ntdll.dll",&module);
        if(!moduleReturned){moduleError=GetLastError();moduleUnknown=module!=nullptr;return false;}
        moduleAcquired=module!=nullptr;
        if(!moduleAcquired){moduleUnknown=true;return false;}
        createAddress=GetProcAddress(module,"NtCreateFile");
        if(!createAddress){addressError=GetLastError();return false;}
        // Documented native constants: FILE_CREATE=2, DIRECTORY_FILE=1,
        // SYNCHRONOUS_IO_NONALERT=0x20. No overwrite, collision retry, inherited
        // handle, backup intent, new privilege or ambient absolute-path reopen.
        createAttempted=true;
        createReturned=reinterpret_cast<NativeCreate>(createAddress)(&root,
            FILE_LIST_DIRECTORY|FILE_TRAVERSE|FILE_READ_ATTRIBUTES|READ_CONTROL|SYNCHRONIZE|DELETE,
            &originalAttributes,&originalStatus,nullptr,FILE_ATTRIBUTE_DIRECTORY,
            FILE_SHARE_READ|FILE_SHARE_WRITE,2,1|0x20,nullptr,0);
        rootUnknown=createReturned==static_cast<NTSTATUS>(0x103)||
            originalStatus.Status==static_cast<NTSTATUS>(0x103)||
            (createReturned!=0&&root!=nullptr)||
            (createReturned==0&&(!root||root==INVALID_HANDLE_VALUE||root==parent));
        rootAcquired=createReturned==0&&!rootUnknown;
        if(!rootAcquired||originalStatus.Status!=0||originalStatus.Information!=2)return false;
        rootIdentity.Capture(root);
        created=rootIdentity.captured&&rootIdentity.standard.Directory&&
            !(rootIdentity.basic.FileAttributes&FILE_ATTRIBUTE_REPARSE_POINT);
        if(!created)return false;
        dosPathAttempted=true;
        dosPathReturned=GetFinalPathNameByHandleW(root,dosPath.data(),
            static_cast<DWORD>(dosPath.size()),FILE_NAME_NORMALIZED|VOLUME_NAME_DOS);
        if(!dosPathReturned){dosPathError=GetLastError();return false;}
        dosPathTooSmall=dosPathReturned>=dosPath.size();
        if(dosPathTooSmall)return false;
        // The unchanged Windows alias cases assert drive-rooted fixture paths.
        // Retain the native extended spelling separately; remove only its
        // documented extended DOS drive prefix, never normalize aliases.
        if(dosPathReturned<7||dosPath[0]!=L'\\'||dosPath[1]!=L'\\'||dosPath[2]!=L'?'||
            dosPath[3]!=L'\\'||!((dosPath[4]>=L'A'&&dosPath[4]<=L'Z')||
            (dosPath[4]>=L'a'&&dosPath[4]<=L'z'))||dosPath[5]!=L':'||dosPath[6]!=L'\\')return false;
        environmentPathUnits=dosPathReturned-4;
        for(SIZE_T i=0;i<environmentPathUnits;++i)environmentPath[i]=dosPath[i+4];
        environmentPath[environmentPathUnits]=0;
        environmentOpenAttempted=true;
        environmentOpened=CreateFileW(environmentPath.data(),FILE_READ_ATTRIBUTES|READ_CONTROL|SYNCHRONIZE,
            FILE_SHARE_READ|FILE_SHARE_WRITE|FILE_SHARE_DELETE,nullptr,OPEN_EXISTING,
            FILE_FLAG_BACKUP_SEMANTICS|FILE_FLAG_OPEN_REPARSE_POINT,nullptr);
        if(environmentOpened==INVALID_HANDLE_VALUE){environmentOpenError=GetLastError();return false;}
        environmentAcquired=environmentOpened!=nullptr;
        if(!environmentAcquired){environmentUnknown=true;return false;}
        environmentIdentity.Capture(environmentOpened);
        environmentPathBound=environmentIdentity.captured&&environmentIdentity.standard.Directory&&
            !(environmentIdentity.basic.FileAttributes&FILE_ATTRIBUTE_REPARSE_POINT)&&
            environmentIdentity.id.VolumeSerialNumber==rootIdentity.id.VolumeSerialNumber&&
            memcmp(environmentIdentity.id.FileId.Identifier,rootIdentity.id.FileId.Identifier,
                sizeof(rootIdentity.id.FileId.Identifier))==0;
        // Fully shared metadata observation cannot block another writer. Retain
        // every original and close this independent returned query capability
        // before any child can borrow the constructed environment.
        environmentCloseAttempted=true;environmentCloseReturned=CloseHandle(environmentOpened);
        if(!environmentCloseReturned)environmentCloseError=GetLastError();
        return environmentPathBound&&environmentCloseReturned;
    }
    bool DisposeAfterConsumersAndFixtureCleanup() noexcept {
        // Concrete session caller must establish the actual borrower/IO cut.
        // Pending/unknown creation can still own parent/module/output storage.
        if(rootUnknown)return false;
        if(rootAcquired&&!dispositionAttempted&&!rootCloseAttempted) {
            dispositionAttempted=true;
            dispositionReturned=SetFileInformationByHandle(root,FileDispositionInfo,
                &originalDisposition,sizeof(originalDisposition));
            if(!dispositionReturned)dispositionError=GetLastError();
        }
        // Ordinary empty-directory removal only, never recursive traversal or
        // deletion of another fixture. Failure stays qualification-failed even
        // while independently eligible handle/module disposition continues.
        if(rootAcquired&&!rootCloseAttempted) {
            rootCloseAttempted=true;rootCloseReturned=CloseHandle(root);
            if(!rootCloseReturned)rootCloseError=GetLastError();
        }
        if(parentAcquired&&!parentCloseAttempted) {
            parentCloseAttempted=true;parentCloseReturned=CloseHandle(parent);
            if(!parentCloseReturned)parentCloseError=GetLastError();
        }
        if(moduleAcquired&&!moduleCloseAttempted) {
            moduleCloseAttempted=true;moduleCloseReturned=FreeLibrary(module);
            if(!moduleCloseReturned)moduleCloseError=GetLastError();
        }
        return PhysicallyDisposed();
    }
    bool PhysicallyDisposed() const noexcept {
        return !rootUnknown&&!parentUnknown&&!moduleUnknown&&!environmentUnknown&&
            (!environmentAcquired||(environmentCloseAttempted&&environmentCloseReturned))&&
            (!rootAcquired||(rootCloseAttempted&&rootCloseReturned))&&
            (!parentAcquired||(parentCloseAttempted&&parentCloseReturned))&&
            (!moduleAcquired||(moduleCloseAttempted&&moduleCloseReturned));
    }
    bool RemovalSucceeded() const noexcept {
        return created&&dispositionAttempted&&dispositionReturned&&PhysicallyDisposed();
    }
    // Root/object owners are intentionally retained here until the concrete
    // session establishes consumer/IO and original fixture cleanup eligibility.
    // No destructor silently closes uncertain or borrowed originals.
    SIZE_T ReservedSourceBytes() const noexcept {return sizeof(*this);}
};
// Primary native contract, refreshed 11 October 2026:
// https://learn.microsoft.com/en-us/windows/win32/api/winternl/nf-winternl-ntcreatefile
// Authentic SDK/architecture, parent rights/ownership, real returned handles and
// pending native supervision remain qualification inputs. Source is UNRUN.
}

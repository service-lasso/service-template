// UNCOMPILED bounded metadata hold of an actual fixture name component.
// Reparse entry and traversed target are separate observations/owners.
#pragma once
#include <climits>
#include "windows_private_fixture_root.hpp"
namespace template_creator {
struct OriginalFixtureNameHold {
    const OriginalWindowsFixtureRoot* rootOriginal=nullptr;
    std::array<wchar_t,32768> path{};
    SIZE_T pathUnits=0;
    UNICODE_STRING name{};OBJECT_ATTRIBUTES attributes{};IO_STATUS_BLOCK status{};
    HANDLE original=nullptr;NTSTATUS returned=0;
    bool attempted=false,nativeAttempted=false,acquired=false,unknown=false;
    bool reparseEntry=false,directory=false,allowReparse=false,reparseObserved=false;
    FileIdentityObservation identity;
    bool closeAttempted=false;BOOL closeReturned=FALSE;DWORD closeError=0;
    bool Capture(const OriginalWindowsFixtureRoot& root,const wchar_t* selected,SIZE_T units,
        bool selectedDirectory,bool selectedReparseEntry,bool selectedAllowReparse=false) noexcept {
        if(attempted)return false;attempted=true;rootOriginal=&root;
        directory=selectedDirectory;reparseEntry=selectedReparseEntry;allowReparse=selectedAllowReparse;
        if(!selected || !units || units>=path.size() || units>(USHRT_MAX-sizeof(wchar_t))/sizeof(wchar_t) ||
            !root.created || root.rootUnknown || root.rootCloseAttempted || root.dispositionAttempted ||
            !root.moduleAcquired || root.moduleUnknown || root.moduleCloseAttempted || !root.createAddress)return false;
        SIZE_T begin=0;
        for(SIZE_T i=0;i<units;++i) {
            const auto c=selected[i];
            if(!c || c==L'/' || c==L':' || (!i&&c==L'\\'))return false;
            path[i]=c;
            if(c==L'\\') {
                const SIZE_T length=i-begin;
                if(!length || (length==1&&path[begin]==L'.') ||
                    (length==2&&path[begin]==L'.'&&path[begin+1]==L'.'))return false;
                begin=i+1;
            }
        }
        const SIZE_T length=units-begin;
        if(!length || (length==1&&path[begin]==L'.') ||
            (length==2&&path[begin]==L'.'&&path[begin+1]==L'.'))return false;
        pathUnits=units;name.Buffer=path.data();name.Length=static_cast<USHORT>(units*sizeof(wchar_t));
        name.MaximumLength=static_cast<USHORT>((units+1)*sizeof(wchar_t));
        attributes.Length=sizeof(attributes);attributes.RootDirectory=root.root;attributes.ObjectName=&name;
        nativeAttempted=true;
        returned=reinterpret_cast<OriginalWindowsFixtureRoot::NativeCreate>(root.createAddress)(
            &original,FILE_READ_ATTRIBUTES|READ_CONTROL|SYNCHRONIZE,&attributes,&status,nullptr,
            FILE_ATTRIBUTE_NORMAL,FILE_SHARE_READ,1,0x20|(directory?0:0x40)|(reparseEntry?0x00200000:0),nullptr,0);
        // Directory roles omit FILE_DIRECTORY_FILE when opening a reparse
        // entry: the documented compatible-options list is narrower. Actual
        // FileStandardInfo below validates the acquired object's directory kind.
        unknown=returned==static_cast<NTSTATUS>(0x103)||status.Status==static_cast<NTSTATUS>(0x103)||
            (returned<0&&original!=nullptr)||(returned>=0&&(!original||original==INVALID_HANDLE_VALUE));
        acquired=returned>=0&&!unknown;
        if(!acquired || status.Status<0)return false;
        identity.Capture(original);
        reparseObserved=identity.captured&&(identity.basic.FileAttributes&FILE_ATTRIBUTE_REPARSE_POINT)!=0;
        return identity.captured && static_cast<bool>(identity.standard.Directory)==directory &&
            (allowReparse || !reparseObserved);
    }
    bool CloseOnce() noexcept {
        if(unknown)return false;
        if(acquired&&!closeAttempted){closeAttempted=true;closeReturned=CloseHandle(original);if(!closeReturned)closeError=GetLastError();}
        return !acquired||(closeAttempted&&closeReturned);
    }
    bool PhysicallyDisposed() const noexcept{return !unknown&&(!acquired||(closeAttempted&&closeReturned));}
};
}

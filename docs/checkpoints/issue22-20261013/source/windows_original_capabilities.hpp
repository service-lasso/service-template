// UNCOMPILED source-owned native caller acquisition. Paths select originals;
// successful opens/identity observations do not grant source/runtime authority.
#pragma once
#include "windows_original_main.hpp"
#include "windows_private_fixture_root.hpp"
namespace template_creator_bridge {
struct OriginalWindowsCapabilityPaths {
    std::wstring candidateDirectory,toolFile,evidenceDirectory;
};
struct OriginalWindowsOpenedCapability {
    std::array<wchar_t,32768> path{};
    HANDLE returned=INVALID_HANDLE_VALUE;
    DWORD openError=0,closeError=0;
    DWORD requestedAccess=0,requestedSharing=0,requestedFlags=0,requestedDisposition=0;
    bool attempted=false,acquired=false,outputUnknown=false,valid=false,closeAttempted=false;
    BOOL closeReturned=FALSE;
    template_creator::FileIdentityObservation identity;
    void CloseOnce() noexcept {
        if(!acquired || closeAttempted || outputUnknown)return;
        closeAttempted=true;closeReturned=CloseHandle(returned);
        if(!closeReturned)closeError=GetLastError();
    }
};
class OriginalWindowsSourceCapabilities {
    ~OriginalWindowsSourceCapabilities()=default; // Resident through unknown custody.
    std::array<OriginalWindowsOpenedCapability,3> originals_{};
    template_creator::OriginalWindowsFixtureRoot fixtureRoot_;
    OriginalWindowsFinalOutput* privateSink_=nullptr;
    OriginalWindowsNativeMain* main_=nullptr;
    bool acquireAttempted_=false,acquired_=false,connectAttempted_=false,connectReturned_=false;
    bool handoffEntered_=false;
    bool sinkCreationAttempted_=false,sinkCreationReturned_=false,disposalAttempted_=false,disposalReturned_=false;
    static void CopyPath(const std::wstring& source,std::array<wchar_t,32768>& destination) {
        if(source.empty() || source.size()>=destination.size() || source.find(L'\0')!=std::wstring::npos)
            throw std::invalid_argument("bounded original native capability path");
        const bool drive=source.size()>=3 && ((source[0]>=L'A'&&source[0]<=L'Z') || (source[0]>=L'a'&&source[0]<=L'z')) &&
            source[1]==L':' && (source[2]==L'\\'||source[2]==L'/');
        const bool unc=source.size()>2 && source[0]==L'\\' && source[1]==L'\\';
        if(!drive&&!unc)throw std::invalid_argument("explicit absolute native capability selection");
        for(SIZE_T i=0;i<source.size();++i)destination[i]=source[i];
        destination[source.size()]=0;
    }
public:
    OriginalWindowsSourceCapabilities(const OriginalWindowsCapabilityPaths& paths,
        SIZE_T sinkBytes,SIZE_T sinkReadbackBytes,SIZE_T sinkCalls) {
        // Every copy and sink store exists before the first native acquisition.
        CopyPath(paths.candidateDirectory,originals_[0].path);
        CopyPath(paths.toolFile,originals_[1].path);
        CopyPath(paths.evidenceDirectory,originals_[2].path);
        privateSink_=new OriginalWindowsFinalOutput(sinkBytes,sinkReadbackBytes,sinkCalls);
    }
    OriginalWindowsSourceCapabilities(const OriginalWindowsSourceCapabilities&)=delete;
    OriginalWindowsSourceCapabilities& operator=(const OriginalWindowsSourceCapabilities&)=delete;
    bool AcquireOriginals() noexcept {
        if(acquireAttempted_ || disposalAttempted_)return false;acquireAttempted_=true;
        bool clean=true;
        for(SIZE_T i=0;i<originals_.size();++i) {
            auto& row=originals_[i];row.attempted=true;
            row.requestedAccess=i==1 ? GENERIC_READ|READ_CONTROL|FILE_EXECUTE :
                FILE_LIST_DIRECTORY|FILE_TRAVERSE|FILE_READ_ATTRIBUTES|READ_CONTROL|SYNCHRONIZE;
            if(i==2)row.requestedAccess|=FILE_ADD_FILE|FILE_ADD_SUBDIRECTORY;
            // Selected private output and fixture-root creation. The native
            // acquisition can deny either right; no ACL/privilege is changed.
            row.requestedSharing=FILE_SHARE_READ;
            row.requestedDisposition=OPEN_EXISTING;
            row.requestedFlags=FILE_FLAG_OPEN_REPARSE_POINT|(i==1?0:FILE_FLAG_BACKUP_SEMANTICS);
            row.returned=CreateFileW(row.path.data(),row.requestedAccess,row.requestedSharing,nullptr,row.requestedDisposition,
                row.requestedFlags,nullptr);
            if(row.returned==INVALID_HANDLE_VALUE) { row.openError=GetLastError();clean=false;continue; }
            row.acquired=row.returned!=nullptr;
            if(!row.acquired) { row.outputUnknown=true;clean=false;continue; }
            row.identity.Capture(row.returned);
            row.valid=row.identity.captured && !(row.identity.basic.FileAttributes&FILE_ATTRIBUTE_REPARSE_POINT) &&
                (static_cast<bool>(row.identity.standard.Directory)==(i!=1));
            if(!row.valid)clean=false;
        }
        if(!clean)return false;
        const auto& candidate=originals_[0].identity.id;
        const auto& evidence=originals_[2].identity.id;
        if(candidate.VolumeSerialNumber==evidence.VolumeSerialNumber &&
            memcmp(candidate.FileId.Identifier,evidence.FileId.Identifier,sizeof(candidate.FileId.Identifier))==0)return false;
        // SAME source-owned evidence parent, never the canonical candidate
        // input tree or ambient system temp directory. Root owner is preowned.
        if(!fixtureRoot_.Create(originals_[2].returned))return false;
        sinkCreationAttempted_=true;
        sinkCreationReturned_=privateSink_->Create(originals_[2].returned,originals_[0].returned);
        acquired_=sinkCreationReturned_;return acquired_;
    }
    bool ConnectOriginalMain(OriginalWindowsNativeMain& main,const void* originalBody,DWORD originalBodyBytes) noexcept {
        if(connectAttempted_ || disposalAttempted_ || !acquired_ || main.connectAttempted)return false;
        connectAttempted_=true;main_=&main;
        const std::array<HANDLE,3> rootToolSink{originals_[0].returned,originals_[1].returned,privateSink_->file};
        connectReturned_=main.ConnectOriginals(rootToolSink,originals_[2].returned,originalBody,originalBodyBytes);
        handoffEntered_=main.connectAttempted;
        return connectReturned_;
    }
    bool DisposeIndependentlyEligible() noexcept {
        if(disposalAttempted_)return disposalReturned_;
        // Connect can fail after partial transfer. Consume the SAME retained
        // host's physical row, never the Connect return value as closure proof.
        if(handoffEntered_) {
            if(!main_ || !main_->host || !main_->host->progressCount ||
                !main_->host->progress[main_->host->progressCount-1].physicalDisposed)return false;
        }
        disposalAttempted_=true;
        bool all=privateSink_->DisposeEligible();
        const bool fixtureDisposed=fixtureRoot_.DisposeAfterConsumersAndFixtureCleanup();
        if(!fixtureDisposed)all=false;
        for(auto& row:originals_) {
            row.CloseOnce();
            if(row.outputUnknown || (row.acquired&&(!row.closeAttempted||!row.closeReturned)))all=false;
        }
        disposalReturned_=all;return all;
    }
    const auto& PrivateOriginals() const noexcept {return originals_;}
    const OriginalWindowsFinalOutput& PrivateSink() const noexcept {return *privateSink_;}
    const template_creator::OriginalWindowsFixtureRoot& PrivateFixtureRoot() const noexcept {return fixtureRoot_;}
    bool AcquisitionReturned() const noexcept {return acquired_;}
    bool ConnectionReturned() const noexcept {return connectReturned_;}
    const OriginalWindowsNativeMain* ConnectedMainOriginal() const noexcept {return main_;}
    bool DisposalAttempted() const noexcept {return disposalAttempted_;}
    bool DisposalReturned() const noexcept {return disposalReturned_;}
    bool ReservedSourceBytes(SIZE_T& bytes) const noexcept {
        SIZE_T sinkBytes=0;
        if(!privateSink_->sink.ReservedSourceBytes(sinkBytes))return false;
        const auto fixed=sizeof(*this)+sizeof(OriginalWindowsFinalOutput)-sizeof(privateSink_->sink);
        if(sinkBytes>SIZE_MAX-fixed)return false;
        bytes=fixed+sinkBytes;return true;
    }
    // This constructor observes leaves and documented sharing only. Actual
    // ancestor/name/mount/security/source/tool/runtime/loaded-capacity joins
    // remain required by LaunchAssociationsEstablished, not inferred here.
    // Requested directory traversal/add-file and native tool execute rights:
    // https://learn.microsoft.com/en-us/windows/win32/fileio/file-access-rights-constants
    // (7 January 2021, refreshed 10 October 2026). No bypass privilege enabled.
};
}

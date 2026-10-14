// UNCOMPILED actual native directory query owner. This records collector-native
// membership, not the child's FindFirst/FindNext calls or installed ABI authority.
#pragma once
#include "windows_pending_call_owner.hpp"
#include "native_command_plan.hpp"
#include <cstring>
#include <cstddef>
namespace template_creator {
class OriginalFrameworkNativeDirectory {
public:
    struct Query {
        HANDLE handleInput=nullptr;
        const wchar_t* searchOriginal=nullptr;
        WIN32_FIND_DATAW data{};
        HANDLE handleReturned=INVALID_HANDLE_VALUE;
        BOOL nextReturned=FALSE;
        DWORD error=0;
        template_creator_bridge::OriginalWindowsPendingCall* pendingOriginal=nullptr;
        bool first=false,attempted=false,returned=false,directory=false,dot=false,nameTerminated=false;
        SIZE_T nameUnits=0;
    };
private:
    std::unique_ptr<wchar_t[]> search_;
    std::unique_ptr<Query[]> rows_;
    SIZE_T pathCapacity_=0,pathUnits_=0,capacity_=0,count_=0;
    const void* pathOwnerOriginal_=nullptr;
    const template_creator_bridge::WindowsOriginalCommand* commandOriginal_=nullptr;
    template_creator_bridge::OriginalWindowsPendingCalls* pendingOwnerOriginal_=nullptr;
    HANDLE directoryOriginal_=INVALID_HANDLE_VALUE;
    template_creator_bridge::OriginalWindowsPendingCall* closePendingOriginal_=nullptr;
    BOOL closeReturned_=FALSE;
    DWORD closeError_=0;
    bool copied_=false,attempted_=false,complete_=false,unknown_=false,closeAttempted_=false,closed_=false;
    bool Classify(Query& row) noexcept {
        constexpr SIZE_T cells=sizeof(row.data.cFileName)/sizeof(wchar_t);
        while(row.nameUnits<cells && row.data.cFileName[row.nameUnits])++row.nameUnits;
        row.nameTerminated=row.nameUnits<cells;
        if(!row.nameTerminated)return false;
        row.directory=(row.data.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY)!=0;
        row.dot=(row.nameUnits==1&&row.data.cFileName[0]==L'.') ||
            (row.nameUnits==2&&row.data.cFileName[0]==L'.'&&row.data.cFileName[1]==L'.');
        return true;
    }
public:
    OriginalFrameworkNativeDirectory(SIZE_T pathUnits,SIZE_T nativeRows):capacity_(nativeRows) {
        if(!pathUnits || pathUnits>SIZE_MAX-3 || !nativeRows || nativeRows>SIZE_MAX/sizeof(Query))
            throw std::invalid_argument("native framework directory geometry");
        pathCapacity_=pathUnits+3;
        if(pathCapacity_>(SIZE_MAX-sizeof(*this))/sizeof(wchar_t) ||
            nativeRows>(SIZE_MAX-sizeof(*this)-pathCapacity_*sizeof(wchar_t))/sizeof(Query))
            throw std::invalid_argument("native framework directory aggregate geometry");
        search_=std::make_unique<wchar_t[]>(pathCapacity_);
        rows_=std::make_unique<Query[]>(capacity_);
    }
    OriginalFrameworkNativeDirectory(const OriginalFrameworkNativeDirectory&)=delete;
    OriginalFrameworkNativeDirectory& operator=(const OriginalFrameworkNativeDirectory&)=delete;
    // The owning resolver supplies its actual fullpath/LongFile result and
    // retained source owner. This method does not fabricate that association.
    bool CopyOriginalDirectory(const void* pathOwner,const wchar_t* path,SIZE_T units) noexcept {
        if(copied_ || attempted_ || !pathOwner || !path || !units || units>pathCapacity_-3 || path[units])return false;
        for(SIZE_T i=0;i<units;++i)if(!path[i])return false;
        pathOwnerOriginal_=pathOwner;memcpy(search_.get(),path,units*sizeof(wchar_t));pathUnits_=units;
        if(search_[pathUnits_-1]!=L'\\'&&search_[pathUnits_-1]!=L'/')search_[pathUnits_++]=L'\\';
        search_[pathUnits_++]=L'*';search_[pathUnits_]=0;copied_=true;return true;
    }
    bool ObserveOriginal(template_creator_bridge::OriginalWindowsPendingCalls& pending,
            const template_creator_bridge::WindowsOriginalCommand& command) noexcept {
        if(attempted_)return false;attempted_=true;pendingOwnerOriginal_=&pending;commandOriginal_=&command;
        if(!copied_ || !command.copied || !command.snapshotOriginal)return false;
        while(count_<capacity_) {
            auto& row=rows_[count_++];row.first=count_==1;row.handleInput=directoryOriginal_;
            row.searchOriginal=search_.get();
            row.pendingOriginal=pending.BeginOriginal(this,&row,&row.data,sizeof(row.data),&command,
                offsetof(Query,data));
            if(!row.pendingOriginal)return false;
            row.attempted=true;
            if(row.first) {
                row.handleReturned=FindFirstFileExW(search_.get(),FindExInfoStandard,&row.data,
                    FindExSearchNameMatch,nullptr,0);
                if(row.handleReturned==INVALID_HANDLE_VALUE)row.error=GetLastError();
                row.returned=true;
                if(!pending.ReturnedOriginalHandle(*row.pendingOriginal,row.handleReturned,row.error,INVALID_HANDLE_VALUE,row.handleReturned==nullptr)){
                    unknown_=true;return false;
                }
                directoryOriginal_=row.handleReturned;
                if(directoryOriginal_==INVALID_HANDLE_VALUE)return false;
                if(!directoryOriginal_){unknown_=true;return false;}
            } else {
                row.nextReturned=FindNextFileW(directoryOriginal_,&row.data);
                if(!row.nextReturned)row.error=GetLastError();row.returned=true;
                if(!pending.ReturnedOriginalBoolean(*row.pendingOriginal,row.nextReturned,row.error)){
                    unknown_=true;return false;
                }
                if(!row.nextReturned){complete_=row.error==ERROR_NO_MORE_FILES;return complete_;}
            }
            if(!Classify(row))return false;
        }
        // No further query is made without a preowned raw result cell. The
        // handle remains eligible for independent closure if no call is pending.
        return false;
    }
    bool CloseIndependentlyEligible() noexcept {
        if(closed_)return true;
        if(unknown_ || !attempted_ || !pendingOwnerOriginal_ || !commandOriginal_)return false;
        if(directoryOriginal_==INVALID_HANDLE_VALUE){closed_=true;return true;}
        if(closeAttempted_)return false;
        closePendingOriginal_=pendingOwnerOriginal_->BeginOriginal(this,&directoryOriginal_,nullptr,0,
            commandOriginal_,sizeof(directoryOriginal_));
        if(!closePendingOriginal_)return false;
        closeAttempted_=true;closeReturned_=FindClose(directoryOriginal_);
        if(!closeReturned_)closeError_=GetLastError();
        if(!pendingOwnerOriginal_->ReturnedOriginalBoolean(*closePendingOriginal_,closeReturned_,closeError_)){
            unknown_=true;return false;
        }
        closed_=closeReturned_!=FALSE;return closed_;
    }
    bool CompleteOriginalMembership() const noexcept{return complete_&&!unknown_;}
    bool PhysicallyDisposed() const noexcept{return closed_&&!unknown_;}
    const Query* PrivateQueries() const noexcept{return rows_.get();}
    SIZE_T QueryCount() const noexcept{return count_;}
    const void* OriginalPathOwner() const noexcept{return pathOwnerOriginal_;}
    const wchar_t* OriginalSearch() const noexcept{return search_.get();}
    SIZE_T ReservedSourceBytes() const noexcept{return sizeof(*this)+pathCapacity_*sizeof(wchar_t)+capacity_*sizeof(Query);}
};
}

// UNCOMPILED retained native selection input. Content selects private source
// inputs; successful capture does not grant native/source/execution authority.
#pragma once
#include "windows_identity.hpp"
#include <memory>
#include <array>
#include <stdexcept>
namespace template_creator_bridge {
struct OriginalWindowsInvocation;
class OriginalWindowsSelectionDecoder;
struct OriginalSelectionRead {
    DWORD requested=0,actual=0,error=0;
    SIZE_T offset=0;
    BOOL returned=FALSE;
    bool attempted=false,eofAttempt=false;
};
class OriginalWindowsSelectionReader {
    friend struct OriginalWindowsInvocation;
    friend class OriginalWindowsSelectionDecoder;
    std::array<wchar_t,32768> path_{};
    std::unique_ptr<unsigned char[]> bytes_;
    std::unique_ptr<OriginalSelectionRead[]> reads_;
    SIZE_T byteCapacity_=0,rowCapacity_=0,rowCount_=0,retained_=0;
    HANDLE original_=INVALID_HANDLE_VALUE;
    DWORD openError_=0,closeError_=0;
    BOOL closeReturned_=FALSE;
    template_creator::FileIdentityObservation first_,after_,final_;
    bool attempted_=false,acquired_=false,creationUnknown_=false,ioUnknown_=false;
    bool failed_=false,captured_=false,closeAttempted_=false,finalValidationAttempted_=false,finalValid_=false;
    static bool SameHeldSelection(const template_creator::FileIdentityObservation& a,
        const template_creator::FileIdentityObservation& b) noexcept {
        return a.captured && b.captured && a.id.VolumeSerialNumber==b.id.VolumeSerialNumber &&
            memcmp(a.id.FileId.Identifier,b.id.FileId.Identifier,sizeof(a.id.FileId.Identifier))==0 &&
            a.basic.CreationTime.QuadPart==b.basic.CreationTime.QuadPart &&
            a.basic.LastWriteTime.QuadPart==b.basic.LastWriteTime.QuadPart &&
            a.basic.ChangeTime.QuadPart==b.basic.ChangeTime.QuadPart && a.basic.FileAttributes==b.basic.FileAttributes &&
            a.standard.EndOfFile.QuadPart==b.standard.EndOfFile.QuadPart &&
            a.standard.NumberOfLinks==b.standard.NumberOfLinks && a.standard.DeletePending==b.standard.DeletePending &&
            a.standard.Directory==b.standard.Directory && a.pathResult==b.pathResult &&
            memcmp(a.finalPath.data(),b.finalPath.data(),a.pathResult*sizeof(wchar_t))==0 &&
            a.securityRequired==b.securityRequired && a.securityRequired<=a.security.size() &&
            memcmp(a.security.data(),b.security.data(),a.securityRequired)==0;
    }
    ~OriginalWindowsSelectionReader()=default; // Held through actual consumers.
public:
    OriginalWindowsSelectionReader(const wchar_t* path,SIZE_T selectedBytes,SIZE_T selectedReadRows) {
        if(!path || !selectedBytes || selectedBytes==SIZE_MAX || !selectedReadRows ||
            selectedReadRows>SIZE_MAX/sizeof(OriginalSelectionRead))
            throw std::invalid_argument("precharged native selection reader");
        SIZE_T units=0;while(units<path_.size() && path[units])++units;
        if(!units || units==path_.size())throw std::invalid_argument("bounded private selection path");
        for(SIZE_T i=0;i<=units;++i)path_[i]=path[i];
        byteCapacity_=selectedBytes+1;rowCapacity_=selectedReadRows;
        bytes_=std::make_unique<unsigned char[]>(byteCapacity_);
        reads_=std::make_unique<OriginalSelectionRead[]>(rowCapacity_);
    }
    OriginalWindowsSelectionReader(const OriginalWindowsSelectionReader&)=delete;
    OriginalWindowsSelectionReader& operator=(const OriginalWindowsSelectionReader&)=delete;
    bool CaptureOriginal() noexcept {
        if(attempted_)return false;attempted_=true;
        original_=CreateFileW(path_.data(),GENERIC_READ|READ_CONTROL,FILE_SHARE_READ,nullptr,OPEN_EXISTING,
            FILE_FLAG_OPEN_REPARSE_POINT,nullptr);
        if(original_==INVALID_HANDLE_VALUE){openError_=GetLastError();failed_=true;return false;}
        acquired_=original_!=nullptr;
        if(!acquired_){creationUnknown_=true;failed_=true;return false;}
        first_.Capture(original_);
        if(!first_.captured || first_.standard.Directory ||
            (first_.basic.FileAttributes&FILE_ATTRIBUTE_REPARSE_POINT) || first_.standard.EndOfFile.QuadPart<0 ||
            static_cast<unsigned long long>(first_.standard.EndOfFile.QuadPart)>=byteCapacity_) {
            failed_=true;return false;
        }
        const SIZE_T expected=static_cast<SIZE_T>(first_.standard.EndOfFile.QuadPart);
        while(retained_<expected) {
            if(rowCount_==rowCapacity_){failed_=true;return false;}
            auto& row=reads_[rowCount_++];row.offset=retained_;
            row.requested=static_cast<DWORD>((expected-retained_)>65536?65536:expected-retained_);
            row.attempted=true;
            row.returned=ReadFile(original_,bytes_.get()+row.offset,row.requested,&row.actual,nullptr);
            if(!row.returned)row.error=GetLastError();
            if(row.actual>row.requested){ioUnknown_=true;failed_=true;return false;}
            retained_+=row.actual; // Actual affected prefix retained before interpretation.
            if(!row.returned){ioUnknown_=row.error==ERROR_IO_PENDING;failed_=true;return false;}
            if(!row.actual){failed_=true;return false;}
        }
        if(rowCount_==rowCapacity_){failed_=true;return false;}
        auto& eof=reads_[rowCount_++];eof.offset=retained_;eof.requested=1;eof.eofAttempt=true;eof.attempted=true;
        eof.returned=ReadFile(original_,bytes_.get()+retained_,1,&eof.actual,nullptr);
        if(!eof.returned)eof.error=GetLastError();
        if(eof.actual>1){ioUnknown_=true;failed_=true;return false;}
        retained_+=eof.actual;
        if(!eof.returned){ioUnknown_=eof.error==ERROR_IO_PENDING;failed_=true;return false;}
        if(eof.actual){failed_=true;return false;}
        after_.Capture(original_);
        captured_=SameHeldSelection(first_,after_);
        failed_|=!captured_;return captured_;
    }
private:
    // Called only by the native caller AFTER actual parser/command/input
    // consumers no longer borrow this held input. Method name is eligibility,
    // not a caller-provided Boolean grant; uncertain IO always retains owner.
    bool CloseAfterConsumers() noexcept {
        if(closeAttempted_)return closeReturned_!=FALSE;
        if(ioUnknown_ || creationUnknown_)return false;
        if(!acquired_)return true;
        finalValidationAttempted_=true;final_.Capture(original_);
        finalValid_=SameHeldSelection(first_,final_);failed_|=!finalValid_;
        // Final observation failure cannot suppress an independently eligible
        // once-close; actual qualification remains failed after physical close.
        closeAttempted_=true;closeReturned_=CloseHandle(original_);
        if(!closeReturned_){closeError_=GetLastError();failed_=true;}
        return closeReturned_!=FALSE;
    }
public:
    const unsigned char* OriginalBytes() const noexcept {return bytes_.get();}
    SIZE_T OriginalExtent() const noexcept {return retained_;}
    bool Captured() const noexcept {return captured_&&!failed_&&acquired_&&!closeAttempted_&&!ioUnknown_&&!creationUnknown_;}
    bool ClosedKnownOriginal() const noexcept {
        // Physical owner disposition only. Failed capture/final validation is
        // preserved separately and never converted to qualification success.
        return !creationUnknown_ && !ioUnknown_ &&
            (!acquired_ || (closeAttempted_ && closeReturned_));
    }
    bool QualifiedFinalInput() const noexcept {return captured_ && !failed_ && finalValid_ && closeAttempted_ && closeReturned_;}
    bool CompletionKnown() const noexcept {return !ioUnknown_&&!creationUnknown_;}
    const OriginalSelectionRead* OriginalReads() const noexcept {return reads_.get();}
    SIZE_T OriginalReadCount() const noexcept {return rowCount_;}
    HANDLE OriginalHandle() const noexcept {return original_;}
    bool ReservedSourceBytes(SIZE_T& bytes) const noexcept {
        if(byteCapacity_>SIZE_MAX-sizeof(*this))return false;
        bytes=sizeof(*this)+byteCapacity_;
        const SIZE_T rows=rowCapacity_*sizeof(OriginalSelectionRead);
        if(rows>SIZE_MAX-bytes)return false;bytes+=rows;return true;
    }
};
}
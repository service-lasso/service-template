// UNCOMPILED source-owned local constructor. No public handle enrollment,
// external issuer, process attach or changes to another worker's process.
#pragma once
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#include <bcrypt.h>
#include <array>
#include <cstdint>
#include <cstring>
#include "windows_pending_call_owner.hpp"
namespace template_creator_bridge {
struct OriginalWindowsHostHandoff;
struct OriginalHostWire {
    std::uint32_t version = 0, bytes = 0, pointerBytes = 0, phase = 0;
    std::array<std::uintptr_t,3> callerOriginals{};
};
struct OriginalHostChannel {
private:
    friend struct OriginalWindowsHostHandoff;
    friend class OriginalWindowsNoticeOwner;
    friend struct OriginalWindowsStandaloneHost;
    HANDLE server_ = INVALID_HANDLE_VALUE, client_ = INVALID_HANDLE_VALUE, caller_ = nullptr;
    HANDLE current_ = nullptr;
    OriginalWindowsPendingCalls* pendingCallsOriginal_ = nullptr;
    std::array<unsigned char,16> random_{};
    std::array<wchar_t,64> name_{};
    FILETIME born_{}, exited_{}, kernel_{}, user_{};
    NTSTATUS randomReturned_ = 0;
    BOOL callerDuplicateReturned_ = FALSE, timesReturned_ = FALSE, connectReturned_ = FALSE, clientModeReturned_ = FALSE;
    DWORD callerDuplicateError_ = 0, timesError_ = 0, serverError_ = 0, clientError_ = 0, connectError_ = 0, clientModeError_ = 0;
    bool attempted_ = false, constructed_ = false;
    bool serverOutputUnknown_=false,clientOutputUnknown_=false,callerOutputUnknown_=false;
    OriginalHostWire selectedWire_{};
    std::array<unsigned char,65536> selectedBody_{};
    DWORD selectedBodyBytes_ = 0;
    struct NativeWriteOriginal {
        HANDLE endpoint=nullptr;const void* bytes=nullptr;DWORD requested=0;
        DWORD actual=0;BOOL returned=FALSE;DWORD error=0;
        OriginalWindowsPendingCall* pending=nullptr;
        bool attempted=false,returnedObserved=false;
    };
    NativeWriteOriginal wireWriteOriginal_{},bodyWriteOriginal_{};
    bool WriteOriginal(NativeWriteOriginal& row,const void* bytes,DWORD extent,
                       BOOL& returned,DWORD& actual,DWORD& error) noexcept {
        if(row.attempted || !pendingCallsOriginal_ || !bytes || !extent)return false;
        row.endpoint=client_;row.bytes=bytes;row.requested=extent;
        // The native write borrows these exact channel-owned input bytes.
        row.pending=pendingCallsOriginal_->BeginOriginal(this,&row,const_cast<void*>(bytes),extent,
            nullptr,offsetof(NativeWriteOriginal,actual));
        if(!row.pending)return false;
        row.attempted=true;row.returned=WriteFile(row.endpoint,row.bytes,row.requested,&row.actual,nullptr);
        if(!row.returned)row.error=GetLastError();row.returnedObserved=true;
        returned=row.returned;error=row.error;
        const bool settled=pendingCallsOriginal_->ReturnedOriginal(*row.pending,row.returned,row.error,row.actual);
        if(settled)actual=row.actual;
        if(!settled)writeCompletionUnknown_=true;
        return settled && row.returned && row.actual==row.requested;
    }
    bool publicationAttempted_ = false, bodyWriteAttempted_ = false, writeCompletionUnknown_ = false;
    BOOL wireWriteReturned_ = FALSE, bodyWriteReturned_ = FALSE;
    DWORD wireWritten_ = 0, bodyWritten_ = 0, wireWriteError_ = 0, bodyWriteError_ = 0;
public:
    OriginalHostChannel() = default;
    OriginalHostChannel(const OriginalHostChannel&) = delete;
    OriginalHostChannel& operator=(const OriginalHostChannel&) = delete;
    bool Construct() noexcept {
        if (attempted_) return false; attempted_ = true;
        if(!pendingCallsOriginal_)return false;
        current_ = GetCurrentProcess();
        callerDuplicateReturned_ = DuplicateHandle(current_,current_,current_,&caller_,
            PROCESS_DUP_HANDLE|PROCESS_QUERY_LIMITED_INFORMATION|SYNCHRONIZE,FALSE,0);
        if (!callerDuplicateReturned_) {
            callerDuplicateError_ = GetLastError(); callerOutputUnknown_=caller_!=nullptr; return false;
        }
        if (!caller_ || caller_==INVALID_HANDLE_VALUE) { callerOutputUnknown_=true; return false; }
        timesReturned_ = GetProcessTimes(caller_,&born_,&exited_,&kernel_,&user_);
        if (!timesReturned_) { timesError_ = GetLastError(); return false; }
        randomReturned_ = BCryptGenRandom(nullptr,random_.data(),static_cast<ULONG>(random_.size()),BCRYPT_USE_SYSTEM_PREFERRED_RNG);
        if (randomReturned_!=0) return false;
        constexpr wchar_t prefix[] = L"\\\\.\\pipe\\template-creator-";
        constexpr wchar_t hex[] = L"0123456789abcdef";
        SIZE_T position = 0;
        for (;position<sizeof(prefix)/sizeof(wchar_t)-1;++position) name_[position] = prefix[position];
        for (auto value : random_) { name_[position++] = hex[value>>4]; name_[position++] = hex[value&15]; }
        name_[position] = 0;
        // Neither endpoint is inheritable. This constructor owns the only
        // source-created server reader/mode capability; no endpoint is public.
        server_ = CreateNamedPipeW(name_.data(),PIPE_ACCESS_DUPLEX|FILE_FLAG_FIRST_PIPE_INSTANCE,
            PIPE_TYPE_MESSAGE|PIPE_READMODE_MESSAGE|PIPE_NOWAIT,1,65536,65536,0,nullptr);
        if (server_==INVALID_HANDLE_VALUE) { serverError_ = GetLastError(); return false; }
        if (!server_) { serverOutputUnknown_=true; return false; }
        client_ = CreateFileW(name_.data(),GENERIC_READ|GENERIC_WRITE,0,nullptr,OPEN_EXISTING,0,nullptr);
        if (client_==INVALID_HANDLE_VALUE) { clientError_ = GetLastError(); return false; }
        if (!client_) { clientOutputUnknown_=true; return false; }
        connectReturned_ = ConnectNamedPipe(server_,nullptr);
        if (!connectReturned_) connectError_ = GetLastError();
        if (!connectReturned_ && connectError_!=ERROR_PIPE_CONNECTED) return false;
        DWORD mode = PIPE_READMODE_MESSAGE|PIPE_NOWAIT;
        clientModeReturned_ = SetNamedPipeHandleState(client_,&mode,nullptr,nullptr);
        if (!clientModeReturned_) { clientModeError_ = GetLastError(); return false; }
        constructed_ = true; return true;
    }
    bool PublishNativeOriginals(std::uint32_t phase,const std::array<HANDLE,3>& originals,
                                const void* originalBody,DWORD bodyBytes) noexcept {
        if (publicationAttempted_) return false; publicationAttempted_ = true;
        if (!constructed_ || phase>2 || !originalBody || !bodyBytes || bodyBytes>selectedBody_.size()) return false;
        selectedWire_.version = 1; selectedWire_.bytes = sizeof(selectedWire_);
        selectedWire_.pointerBytes = sizeof(void*); selectedWire_.phase = phase;
        for (SIZE_T i=0;i<originals.size();++i) selectedWire_.callerOriginals[i] = reinterpret_cast<std::uintptr_t>(originals[i]);
        selectedBodyBytes_ = bodyBytes;
        memcpy(selectedBody_.data(),originalBody,bodyBytes); // Preowned immutable copy.
        return WriteOriginal(wireWriteOriginal_,&selectedWire_,sizeof(selectedWire_),
            wireWriteReturned_,wireWritten_,wireWriteError_);
    }
    bool PublishSelectedBodyAfterHandoff(const OriginalWindowsHostHandoff& handoff) noexcept;
private:
    bool PublishSelectedBody() noexcept {
        if (bodyWriteAttempted_ || !wireWriteReturned_ || wireWritten_!=sizeof(selectedWire_) || writeCompletionUnknown_) return false;
        bodyWriteAttempted_ = true;
        return WriteOriginal(bodyWriteOriginal_,selectedBody_.data(),selectedBodyBytes_,
            bodyWriteReturned_,bodyWritten_,bodyWriteError_);
    }
    // Source construction/publication is no token/ACL/root/tool/installed grant.
};
}

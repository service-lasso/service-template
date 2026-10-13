// UNCOMPILED standalone-host ingress acquisition. Private wire values select
// actual caller handles for DuplicateHandle; they never constitute role grants.
// No public argv/ENV handle enrollment, source close, privilege change or attach.
#pragma once
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#include <array>
#include <cstdint>
#include "windows_host_channel.hpp"
namespace template_creator_bridge {
struct HostDuplicateOriginal {
    HANDLE source = nullptr, output = nullptr;
    BOOL returned = FALSE;
    DWORD error = 0;
    bool attempted = false, acquired = false, outputUnknown = false;
};
struct OriginalWindowsHostHandoff {
    HANDLE server = nullptr, caller = nullptr, currentProcess = nullptr;
    DWORD callerPid = 0, pidError = 0, heldCallerPid = 0, heldCallerPidError = 0;
    BOOL pidReturned = FALSE, timesReturned = FALSE, peekReturned = FALSE, readReturned = FALSE;
    DWORD timesError = 0, peekError = 0, readError = 0;
    FILETIME born{}, exited{}, kernel{}, user{};
    FILETIME constructorBirth{};
    DWORD callerWait = WAIT_FAILED, callerWaitError = 0;
    DWORD available = 0, messageBytes = 0, requested = 0, actual = 0;
    BOOL stateReturned = FALSE;
    DWORD pipeState = 0, stateError = 0;
    struct HeaderReadOriginal {
        HANDLE endpoint=nullptr;void* destination=nullptr;DWORD requested=0;
        OriginalWindowsPendingCall* pending=nullptr;
    } headerReadOriginal;
    OriginalHostWire wire{};
    std::array<HostDuplicateOriginal,3> duplicates{};
    bool attempted = false, callerAcquired = false, readAttempted = false;
    bool captured = false, unknownRead = false;
    const OriginalHostChannel* channelOriginal = nullptr;
    OriginalWindowsHostHandoff() = default;
    OriginalWindowsHostHandoff(const OriginalWindowsHostHandoff&) = delete;
    OriginalWindowsHostHandoff& operator=(const OriginalWindowsHostHandoff&) = delete;
    bool Capture(OriginalHostChannel& channel) noexcept {
        if (attempted) return false; attempted = true; channelOriginal = &channel;
        if (!channel.constructed_ || !channel.pendingCallsOriginal_) return false;
        if (!channel.wireWriteReturned_ || channel.wireWritten_!=sizeof(channel.selectedWire_) || channel.writeCompletionUnknown_) return false;
        server = channel.server_;
        // The source-owned constructor must supply the retained original caller
        // handle and its actual pre-launch birth observation. Never reopen a
        // numeric PID obtained from the pipe: death/reuse would change custody.
        caller = channel.caller_; constructorBirth = channel.born_;
        callerAcquired = caller && caller!=INVALID_HANDLE_VALUE;
        if (!callerAcquired) return false;
        heldCallerPid = GetProcessId(caller);
        if (!heldCallerPid) { heldCallerPidError = GetLastError(); return false; }
        pidReturned = GetNamedPipeClientProcessId(server,&callerPid);
        if (!pidReturned) { pidError = GetLastError(); return false; }
        timesReturned = GetProcessTimes(caller,&born,&exited,&kernel,&user);
        if (!timesReturned) timesError = GetLastError();
        callerWait = WaitForSingleObject(caller,0);
        if (callerWait==WAIT_FAILED) callerWaitError = GetLastError();
        if (!timesReturned || callerWait!=WAIT_TIMEOUT || callerPid!=heldCallerPid ||
            (!born.dwLowDateTime && !born.dwHighDateTime) ||
            born.dwLowDateTime!=constructorBirth.dwLowDateTime || born.dwHighDateTime!=constructorBirth.dwHighDateTime) return false;
        stateReturned = GetNamedPipeHandleStateW(server,&pipeState,nullptr,nullptr,nullptr,nullptr,0);
        if (!stateReturned) { stateError = GetLastError(); return false; }
        // The source constructor selects a message/nonblocking server. Its
        // exclusive reader/mode ownership must additionally be joined by the
        // actual host constructor; a queried bit alone is not such ownership.
        if (!(pipeState&PIPE_READMODE_MESSAGE) || !(pipeState&PIPE_NOWAIT)) return false;
        peekReturned = PeekNamedPipe(server,nullptr,0,nullptr,&available,&messageBytes);
        if (!peekReturned) { peekError = GetLastError(); return false; }
        if (messageBytes!=sizeof(wire) || available<messageBytes) return false;
        requested = sizeof(wire);
        headerReadOriginal.endpoint=server;headerReadOriginal.destination=&wire;headerReadOriginal.requested=requested;
        headerReadOriginal.pending=channel.pendingCallsOriginal_->BeginOriginal(this,&headerReadOriginal,&wire,requested,
            nullptr,offsetof(HeaderReadOriginal,pending));
        if(!headerReadOriginal.pending)return false;
        readAttempted = true;
        readReturned = ReadFile(headerReadOriginal.endpoint,headerReadOriginal.destination,headerReadOriginal.requested,&actual,nullptr);
        if (!readReturned) readError = GetLastError();
        unknownRead = !channel.pendingCallsOriginal_->ReturnedOriginal(*headerReadOriginal.pending,readReturned,readError,actual);
        if (unknownRead || !readReturned || actual!=requested || wire.version!=1 || wire.bytes!=sizeof(wire) ||
            wire.pointerBytes!=sizeof(void*) || wire.phase>2) return false;
        currentProcess = GetCurrentProcess(); // Retained pseudo-handle result.
        for (SIZE_T i=0;i<duplicates.size();++i) {
            auto& row = duplicates[i]; row.source = reinterpret_cast<HANDLE>(wire.callerOriginals[i]);
            row.attempted = true;
            row.returned = DuplicateHandle(caller,row.source,currentProcess,&row.output,0,FALSE,DUPLICATE_SAME_ACCESS);
            if (!row.returned) { row.error = GetLastError(); row.outputUnknown = row.output!=nullptr; return false; }
            row.acquired = row.output && row.output!=INVALID_HANDLE_VALUE;
            if (!row.acquired) { row.outputUnknown = true; return false; }
        }
        captured = true; return true;
    }
    std::array<void*,5> OriginalCapabilities() const noexcept {
        return {server,caller,duplicates[0].output,duplicates[1].output,duplicates[2].output};
    }
};
inline bool OriginalHostChannel::PublishSelectedBodyAfterHandoff(const OriginalWindowsHostHandoff& handoff) noexcept {
    if (handoff.channelOriginal!=this || !handoff.captured || handoff.unknownRead) return false;
    // Header has actually been consumed by THIS retained handoff before the
    // full selected body is written. No wire+maximum-body buffer overbooking.
    return PublishSelectedBody();
}
// Original pipe/process/duplicate handles and partial wire bytes stay in this
// host owner until exact transfer attachment or independently eligible cleanup.
// Actual token/ACL/root/source/role admission is still the concrete session's job.
}

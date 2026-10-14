// UNCOMPILED narrow saved-event observation using authentic selected SDK types.
// No numeric PEB offsets, cwd/env inference, target writes or source-input grant.
#pragma once
#include "windows_driver.hpp"
#include "windows_pending_call_owner.hpp"
#include "native_command_plan.hpp"
#include <winternl.h>
namespace template_creator {
struct OriginalProcessMachineQueryInput {
    HANDLE process=nullptr;
    FARPROC entry=nullptr;
    USHORT* processOutput=nullptr;
    USHORT* nativeOutput=nullptr;
};
struct OriginalProcessInformationQueryInput {
    HANDLE process=nullptr;
    FARPROC entry=nullptr;
    PROCESSINFOCLASS informationClass=ProcessBasicInformation;
    PVOID destination=nullptr;
    ULONG requested=0;
    PULONG returnedLengthOutput=nullptr;
};
struct OriginalParameterRead {
    HANDLE processOriginal=nullptr;
    const void* address=nullptr;
    SIZE_T requested=0,returned=0;
    BOOL result=FALSE;
    DWORD error=0;
    bool attempted=false,extentUnknown=false;
    template_creator_bridge::OriginalWindowsPendingCall* pendingOriginal=nullptr;
    bool Read(HANDLE process,const void* source,void* destination,SIZE_T bytes,
            template_creator_bridge::OriginalWindowsPendingCalls& pending,
            const template_creator_bridge::WindowsOriginalCommand& command) noexcept {
        if(attempted)return false;attempted=true;
        processOriginal=process;address=source;requested=bytes;
        pendingOriginal=pending.BeginOriginal(this,this,destination,bytes,&command,offsetof(OriginalParameterRead,returned));
        if(!pendingOriginal)return false;
        result=ReadProcessMemory(process,source,destination,bytes,&returned);
        if(!result)error=GetLastError();
        if(!pending.ReturnedOriginal(*pendingOriginal,result,error,returned)){extentUnknown=true;return false;}
        extentUnknown=returned>requested;
        return result && !extentUnknown && returned==requested;
    }
};
struct OriginalCommandLineObservation {
    using QueryProcess=NTSTATUS (NTAPI*)(HANDLE,PROCESSINFOCLASS,PVOID,ULONG,PULONG);
    using QueryMachine=BOOL (WINAPI*)(HANDLE,USHORT*,USHORT*);
    const State* stateOriginal=nullptr;
    const Event* eventOriginal=nullptr;
    template_creator_bridge::OriginalWindowsPendingCalls* pendingOwnerOriginal=nullptr;
    const template_creator_bridge::WindowsOriginalCommand* commandOriginal=nullptr;
    SIZE_T memberOrdinal=MemberLimit,processResource=ResourceLimit;
    HANDLE processOriginal=nullptr;
    FILETIME processBirth{};
    HMODULE ntdllOriginal=nullptr,kernelOriginal=nullptr;
    BOOL ntdllReturned=FALSE,kernelReturned=FALSE;
    DWORD ntdllError=0,kernelError=0,queryAddressError=0,machineAddressError=0;
    FARPROC queryAddress=nullptr,machineAddress=nullptr;
    bool ntdllAcquired=false,kernelAcquired=false,moduleOutputUnknown=false;
    bool ntdllCloseAttempted=false,kernelCloseAttempted=false;
    BOOL ntdllCloseReturned=FALSE,kernelCloseReturned=FALSE;
    DWORD ntdllCloseError=0,kernelCloseError=0;
    BOOL machineReturned=FALSE;
    DWORD machineError=0;
    USHORT processMachine=IMAGE_FILE_MACHINE_UNKNOWN,nativeMachine=IMAGE_FILE_MACHINE_UNKNOWN;
    OriginalProcessMachineQueryInput machineInput{};
    OriginalProcessInformationQueryInput queryInput{};
    template_creator_bridge::OriginalWindowsPendingCall* machinePendingOriginal=nullptr;
    template_creator_bridge::OriginalWindowsPendingCall* queryPendingOriginal=nullptr;
    PROCESS_BASIC_INFORMATION basic{};
    NTSTATUS queryReturned=0;
    ULONG queryLength=0;
    bool queryAttempted=false;
    PEB peb{};
    RTL_USER_PROCESS_PARAMETERS parameters{};
    OriginalParameterRead pebRead,parametersRead,commandRead;
    // UNICODE_STRING Length is a USHORT byte extent. No implicit remote NUL/EOF
    // is invented; only the exact returned extent is an observed command body.
    std::array<unsigned char,65536> commandBytes{};
    SIZE_T retainedCommandExtent=0;
    bool attempted=false,captured=false,failed=false;
    bool originalRootCompared=false,originalRootMatches=false;
    bool crtProjectionAttempted=false,crtProjectionComplete=false,crtProjectionFailed=false;
    SIZE_T crtProjectedWordCount=0,crtProjectionSourceOffset=0,crtProjectionSourceUnits=0;
    // Derived selected Node forms, only after SAME event image/FileId join.
    // These buffers are not evidence of target argv or producer authority.
    bool nodeProjectionAttempted=false,nodeImageAssociated=false;
    bool nodeSourceFormComplete=false,nodeUnsupportedForm=false;
    bool nodeScriptSeen=false,nodePreloadSeen=false,nodeEvalSeen=false;
    SIZE_T nodeImageSelectedFile=SIZE_MAX;
    std::array<wchar_t,32769> nodeScript{},nodePreload{},nodeEval{};
    SIZE_T nodeScriptUnits=0,nodePreloadUnits=0,nodeEvalUnits=0;
    const char* firstFailure=nullptr;
    bool Fail(const char* operation) noexcept {
        failed=true;if(!firstFailure)firstFailure=operation;return false;
    }
    static USHORT CompiledMachine() noexcept {
#if defined(_M_X64) || defined(__x86_64__)
        return IMAGE_FILE_MACHINE_AMD64;
#elif defined(_M_ARM64) || defined(__aarch64__)
        return IMAGE_FILE_MACHINE_ARM64;
#elif defined(_M_IX86) || defined(__i386__)
        return IMAGE_FILE_MACHINE_I386;
#else
        return IMAGE_FILE_MACHINE_UNKNOWN;
#endif
    }
    bool Capture(const Event& actual,const State& state,
            template_creator_bridge::OriginalWindowsPendingCalls& pending,
            const template_creator_bridge::WindowsOriginalCommand& command) noexcept {
        if(attempted)return false;attempted=true;
        stateOriginal=&state;eventOriginal=&actual;pendingOwnerOriginal=&pending;commandOriginal=&command;
        if(!command.copied || !command.snapshotOriginal)return Fail("command_line_original_command_missing");
        if(!state.eventCount || &actual!=&state.events[state.eventCount-1] ||
            actual.original.dwDebugEventCode!=CREATE_PROCESS_DEBUG_EVENT ||
            actual.continued || actual.disposition ||
            actual.createdMemberOrdinal>=state.memberCount || actual.createdProcessResource>=state.resourceCount)
            return Fail("command_line_not_original_stopped_create_occurrence");
        memberOrdinal=actual.createdMemberOrdinal;processResource=actual.createdProcessResource;
        const auto& member=state.members[memberOrdinal];
        const auto& process=state.resources[processResource];
        if(member.process!=processResource || member.pid!=actual.original.dwProcessId ||
            process.kind!=Kind::Process || !process.acquired || process.unknownOutput || process.attempted ||
            !process.duplicateAttempted || !process.duplicateResult ||
            process.duplicateSource!=actual.original.u.CreateProcessInfo.hProcess || !positive(member.birth))
            return Fail("command_line_process_not_same_retained_occurrence");
        processOriginal=process.original;processBirth=member.birth;
        ntdllReturned=GetModuleHandleExW(0,L"ntdll.dll",&ntdllOriginal);
        if(!ntdllReturned)ntdllError=GetLastError();
        ntdllAcquired=ntdllReturned && ntdllOriginal;
        moduleOutputUnknown=(!ntdllReturned && ntdllOriginal) || (ntdllReturned && !ntdllOriginal);
        if(!ntdllAcquired)return Fail("command_line_original_query_module_unavailable");
        kernelReturned=GetModuleHandleExW(0,L"kernel32.dll",&kernelOriginal);
        if(!kernelReturned)kernelError=GetLastError();
        kernelAcquired=kernelReturned && kernelOriginal;
        moduleOutputUnknown=moduleOutputUnknown || (!kernelReturned && kernelOriginal) || (kernelReturned && !kernelOriginal);
        if(!kernelAcquired)return Fail("command_line_original_machine_module_unavailable");
        queryAddress=GetProcAddress(ntdllOriginal,"NtQueryInformationProcess");
        if(!queryAddress)queryAddressError=GetLastError();
        machineAddress=GetProcAddress(kernelOriginal,"IsWow64Process2");
        if(!machineAddress)machineAddressError=GetLastError();
        if(!queryAddress || !machineAddress)return Fail("command_line_original_native_entry_unavailable");
        static_assert(offsetof(OriginalCommandLineObservation,nativeMachine)==
            offsetof(OriginalCommandLineObservation,processMachine)+sizeof(USHORT),"native machine output pair");
        machineInput={processOriginal,machineAddress,&processMachine,&nativeMachine};
        machinePendingOriginal=pending.BeginOriginal(this,&machineInput,&processMachine,2*sizeof(USHORT),
            &command,sizeof(machineInput));
        if(!machinePendingOriginal)return Fail("command_line_machine_query_pending_capacity_or_freeze");
        machineReturned=reinterpret_cast<QueryMachine>(machineAddress)(processOriginal,&processMachine,&nativeMachine);
        if(!machineReturned)machineError=GetLastError();
        if(!pending.ReturnedOriginalBoolean(*machinePendingOriginal,machineReturned,machineError))
            return Fail("command_line_machine_query_completion_unknown");
        if(!machineReturned)return Fail("command_line_original_machine_query_failed");
        const auto actualMachine=processMachine==IMAGE_FILE_MACHINE_UNKNOWN ? nativeMachine : processMachine;
        if(CompiledMachine()==IMAGE_FILE_MACHINE_UNKNOWN || actualMachine!=CompiledMachine())
            return Fail("command_line_selected_sdk_remote_machine_unmatched");
        queryAttempted=true;
        queryInput={processOriginal,queryAddress,ProcessBasicInformation,&basic,sizeof(basic),&queryLength};
        queryPendingOriginal=pending.BeginOriginal(this,&queryInput,&basic,sizeof(basic),&command,sizeof(queryInput));
        if(!queryPendingOriginal)return Fail("command_line_basic_query_pending_capacity_or_freeze");
        queryReturned=reinterpret_cast<QueryProcess>(queryAddress)(processOriginal,ProcessBasicInformation,
            &basic,sizeof(basic),&queryLength);
        if(!pending.ReturnedOriginalNtQuery(*queryPendingOriginal,queryReturned,queryLength))
            return Fail("command_line_basic_query_completion_unknown");
        if(queryReturned!=0 || queryLength!=sizeof(basic) || !basic.PebBaseAddress)
            return Fail("command_line_original_basic_query_failed_or_shape");
        if(!pebRead.Read(processOriginal,basic.PebBaseAddress,&peb,sizeof(peb),pending,command) || !peb.ProcessParameters)
            return Fail("command_line_original_peb_read_failed_or_shape");
        if(!parametersRead.Read(processOriginal,peb.ProcessParameters,&parameters,sizeof(parameters),pending,command))
            return Fail("command_line_original_parameters_read_failed_or_shape");
        const auto& line=parameters.CommandLine;
        if(line.Length>line.MaximumLength || line.Length%sizeof(wchar_t) || line.MaximumLength%sizeof(wchar_t) ||
            line.Length>commandBytes.size() || (line.Length && !line.Buffer))
            return Fail("command_line_original_unicode_extent_shape");
        if(line.Length) {
            const bool read=commandRead.Read(processOriginal,line.Buffer,commandBytes.data(),line.Length,pending,command);
            if(!commandRead.extentUnknown && commandRead.returned<=commandRead.requested)retainedCommandExtent=commandRead.returned;
            if(!read)return Fail("command_line_original_body_read_failed_or_shape");
        }
        captured=true;return true; // Native observation only, not consumed-input admission.
    }
    bool DisposeIndependentModules() noexcept {
        // These are independent creator module references. Pending read outputs
        // and the process duplicate remain with their original lifecycle owner;
        // freeing these references does not settle those native borrowers.
        if(ntdllAcquired && !ntdllCloseAttempted) {
            ntdllCloseAttempted=true;ntdllCloseReturned=FreeLibrary(ntdllOriginal);
            if(!ntdllCloseReturned)ntdllCloseError=GetLastError();
        }
        if(kernelAcquired && !kernelCloseAttempted) {
            kernelCloseAttempted=true;kernelCloseReturned=FreeLibrary(kernelOriginal);
            if(!kernelCloseReturned)kernelCloseError=GetLastError();
        }
        const bool all=!moduleOutputUnknown && (!ntdllAcquired || ntdllCloseReturned) &&
            (!kernelAcquired || kernelCloseReturned);
        if(!all)Fail("command_line_module_disposal_failed_or_unknown");
        return all;
    }
    bool ModulesPhysicallyDisposed() const noexcept {
        return !moduleOutputUnknown && (!ntdllAcquired || ntdllCloseReturned) && (!kernelAcquired || kernelCloseReturned);
    }
};
// Authentic selected SDK/version/architecture and loaded ntdll/kernel32 inputs
// remain ROOT dependencies. No undocumented reserved fields are interpreted.
}

// UNCOMPILED concrete Windows lifecycle gate over session-owned originals.
// This composes actual caller/file/image observations; full private admission,
// ACL/mapping exclusions and tool supply-chain associations remain session checks.
#pragma once
#include "windows_driver.hpp"
#include "windows_ingress.hpp"
#include "windows_held_files.hpp"
#include "windows_remote_exports.hpp"
#include "windows_command_line_observation.hpp"
#include "windows_crt_argument_projection.hpp"
#include "windows_node_source_projection.hpp"
#include "windows_tls_observation.hpp"
#include "windows_codeview_observation.hpp"
#include "windows_compiler_range_observation.hpp"
#include "windows_coreclr_entry_properties.hpp"
#include "windows_fixture_relative_file.hpp"
#include "windows_alias_code_guard.hpp"
#include "windows_helper_source_projection.hpp"
#include "windows_helper_code_guard.hpp"
#include "windows_parser_input_capture.hpp"
#include "windows_parser_runtime_association.hpp"
#include "windows_private_fixture_root.hpp"
#include "native_command_plan.hpp"
#include "windows_pending_call_owner.hpp"
#include "windows_coreclr_export.hpp"
#include "windows_coreclr_entry_inputs.hpp"
#include "windows_coreclr_controller_custody.hpp"
#include "windows_amd64_caller_boundary.hpp"
#include "windows_coreclr_caller_target.hpp"
#include "windows_coreclr_debug_cause.hpp"
#include "windows_coreclr_instruction_step.hpp"
#include "windows_coreclr_compiled_geometry.hpp"
#include "windows_coreclr_writer_bootstrap.hpp"
#if defined(TEMPLATE_NATIVE_CHAIN_REGRESSION)
#include "windows_chain_regression_receipt.hpp"
#endif
namespace template_creator {
struct GateCallerObservation {
    DWORD waitReturned = WAIT_FAILED, waitError = 0;
    BOOL timesReturned = FALSE;
    DWORD timesError = 0;
    FILETIME born{}, exited{}, kernel{}, user{};
};
struct GateImageGeometry { SIZE_T bytes = 0, regions = 0; };
struct GateImageObservation {
    FileIdentityObservation actualFile;
    LoadedImageObservation loaded;
    OriginalCoreClrInitializeExport coreClrInitialize;
    OriginalCoreClrInitializeExport coreClrErrorWriter;
    OriginalLoadedTlsObservation tlsOriginal;
    OriginalLoadedCodeView codeViewOriginal;
    OriginalCoreClrCompiledGeometry ownModuleGeometryOriginal;
    OriginalCompilerRangeObservation compilerRangesOriginal;
    OriginalCompilerTlsCandidates compilerTlsCandidatesOriginal;
    OriginalCompilerTlsInitializerCandidates compilerTlsInitializersOriginal;
    SIZE_T compilerArtifactCandidateOrdinal=SIZE_MAX,compilerArtifactIdentifierMatches=0;
    bool compilerArtifactCandidateAttempted=false;
    const State* stateOriginal=nullptr;
    const Event* loadOriginal=nullptr;
    const Event* unloadOriginal=nullptr;
    SIZE_T memberOrdinal=SIZE_MAX;
    FILETIME processBirth{};
    DWORD pid = 0;
    SIZE_T selectedFile = SIZE_MAX;
    bool attempted = false, associated = false;
    explicit GateImageObservation(GateImageGeometry geometry) : loaded(geometry.bytes, geometry.regions) {}
};
struct GateCoreClrInstruction {
    OriginalCoreClrThreadContext context;
    OriginalCoreClrDebugCause cause;
    OriginalCoreClrInstructionStep step;
    OriginalCompilerInstructionCandidate compilerNodeOriginal;
    const OriginalCoreClrContextChange* pairOriginal=nullptr;
    const OriginalCoreClrContextChange* movementOriginal=nullptr;
    const GateCoreClrInstruction* parentCallOriginal=nullptr;
    const GateCoreClrInstruction* returningCallOriginal=nullptr;
    const GateCoreClrInstruction* predecessorOriginal=nullptr;
};
inline bool OriginalChainRowAvailable(SIZE_T count,SIZE_T capacity) noexcept {return count<capacity;}
inline bool OriginalChainEventUsefulContinued(const Event* event) noexcept {
    return event&&event->decisionAttempted&&event->decisionReturned&&
        event->requestedDecision==SavedEventDecision::ConsumeOwnedObserverException&&
        event->continued&&event->continuationResult&&event->disposition==DBG_CONTINUE;
}
inline bool OriginalChainPredecessorContinued(const GateCoreClrInstruction& previous,
        const OriginalCoreClrThreadContext& context,const OriginalCoreClrContextChange* pair) noexcept {
    const auto* event=previous.context.stoppedOriginal;
    return previous.context.captured&&context.captured&&event&&
        event!=context.stoppedOriginal&&previous.context.stateOriginal==context.stateOriginal&&
        previous.context.threadCreateOriginal==context.threadCreateOriginal&&
        previous.context.commandOriginal==context.commandOriginal&&
        previous.context.pendingOwnerOriginal==context.pendingOwnerOriginal&&
        OriginalChainEventUsefulContinued(event)&&
        previous.movementOriginal&&previous.movementOriginal->effectiveVerified&&
        previous.movementOriginal==pair;
}
inline bool OriginalChainPredecessorContinued(const GateCoreClrInstruction& previous,
                                             const GateCoreClrInstruction& current) noexcept {
    return OriginalChainPredecessorContinued(previous,current.context,current.pairOriginal);
}
// These readonly predicates are consumed by the actual controller below. A
// supplied regression view can exercise rejection, but cannot acquire event,
// thread, context or native continuation authority through these predicates.
inline const char* OriginalChainNextStopFailure(const GateCoreClrInstruction& previous,
    const GateCoreClrInstruction* active,const CONTEXT& actual) noexcept {
#if defined(_M_AMD64) || defined(__x86_64__)
    if(!previous.step.predicted||previous.step.nextAddress!=actual.Rip)
        return "instruction_chain_previous_transition_missing";
    if(previous.step.instruction.flow==OriginalAmd64Instruction::Flow::Call){
        if(active!=&previous||previous.step.before.effective.Rsp<8||
            actual.Rsp!=previous.step.before.effective.Rsp-8)
            return "instruction_chain_actual_call_push_mismatch";
    }else if(previous.step.instruction.flow==OriginalAmd64Instruction::Flow::Return){
        if(!previous.step.returnStackCaptured||!previous.returningCallOriginal||
            active!=previous.returningCallOriginal||actual.Rsp!=previous.step.predictedReturnRsp)
            return "instruction_chain_actual_return_stack_mismatch";
    }
    return nullptr;
#else
    return "instruction_chain_architecture_unsupported";
#endif
}
inline bool OriginalChainReturnMatchesCall(const GateCoreClrInstruction& row,
    const GateCoreClrInstruction* call) noexcept {
#if defined(_M_AMD64) || defined(__x86_64__)
    return call&&call->step.predicted&&call->step.instruction.flow==OriginalAmd64Instruction::Flow::Call&&
        row.step.predicted&&row.step.instruction.flow==OriginalAmd64Instruction::Flow::Return&&
        row.step.returnStackCaptured&&call->step.before.effective.Rsp>=8&&
        row.step.before.effective.Rsp==call->step.before.effective.Rsp-8&&
        call->step.before.effective.Rip<=UINTPTR_MAX-call->step.instruction.length&&
        row.step.originalReturnTarget==row.step.revalidatedReturnTarget&&
        row.step.nextAddress==row.step.originalReturnTarget&&
        row.step.nextAddress==call->step.before.effective.Rip+call->step.instruction.length;
#else
    return false;
#endif
}
struct OriginalHostContractCallbackRange {
    enum class Result {Unobserved,Null,OutsideImage,NoSupportedRange,CodeMismatch,MatchedRange};
    const OriginalHostRuntimeContractObservation* contractOriginal=nullptr;
    const GateImageObservation* imageOriginal=nullptr;
    const Event* stoppedOriginal=nullptr;
    DWORD64 addressOriginal=0;
    OriginalAmd64RuntimeFunction functionOriginal{};
    SIZE_T functionOrdinal=SIZE_MAX;
    Result result=Result::Unobserved;
    bool functionStartObserved=false;
};
struct GateCoreClrInvocation {
    enum class Phase {Unused,AwaitWriterReturn,Walking,EntryCaptured,Failed};
    Phase phase=Phase::Unused;
    std::unique_ptr<GateCoreClrInstruction[]> instructionOriginals;
    SIZE_T instructionCapacity=0,instructionCount=0;
    const State* stateOriginal=nullptr;
    const Event* threadCreateOriginal=nullptr;
    const Event* writerEventOriginal=nullptr;
    const Event* continuationEventOriginal=nullptr;
    DWORD64 continuationCandidate=0,predictedAddress=0;
    const GateCoreClrInstruction* initializerCallOriginal=nullptr;
    const GateCoreClrInstruction* activeCallOriginal=nullptr;
    const GateCoreClrInstruction* entryPredecessorOriginal=nullptr;
    const Event* firstFailureOriginal=nullptr;
    OriginalCoreClrWriterBootstrap writerBootstrap,postWriterObservation;
    OriginalCompilerPostWriterSourceCandidate continuationSourceCandidateOriginal;
    const OriginalCoreClrContextChange* bootstrapMovementOriginal=nullptr;
    const OriginalCoreClrContextChange* entryMovementOriginal=nullptr;
    const OriginalCoreClrContextChange* postWriterMovementOriginal=nullptr;
    const OriginalCoreClrContextChange* entryPairOriginal=nullptr;
    OriginalCoreClrDebugCause entryCause;
    OriginalCoreClrThreadContext entryContext;
    std::unique_ptr<OriginalCoreClrEntryInputs> inputs;
    OriginalCoreClrEntryProperties propertiesOriginal;
    std::array<OriginalHostContractCallbackRange,4> hostCallbackRangesOriginal{};
    std::unique_ptr<OriginalAmd64CallerBoundary> callerBoundary;
    OriginalCoreClrCallerTarget callerTarget;
    static SIZE_T CheckedSourceBytes(const template_creator_bridge::OriginalWindowsObserverGeometry& geometry,SIZE_T instructions) {
        SIZE_T bytes=sizeof(GateCoreClrInvocation);
        const auto add=[&](SIZE_T value) {
            if(value>SIZE_MAX-bytes)throw std::invalid_argument("complete invocation aggregate geometry");
            bytes+=value;
        };
        add(OriginalCoreClrEntryInputs::CheckedSourceBytes(geometry.pointers,geometry.stringBytes,geometry.readRows));
        add(OriginalAmd64CallerBoundary::CheckedSourceBytes(geometry.readRows));
        if(!instructions || instructions>(SIZE_MAX-bytes)/sizeof(GateCoreClrInstruction))
            throw std::invalid_argument("derived complete instruction roster backing");
        return bytes+instructions*sizeof(GateCoreClrInstruction);
    }
    bool preownAttempted=false,preowned=false;
    std::exception_ptr preownExceptionOriginal;
    GateCoreClrInvocation()=default;
    bool PreownOriginal(const template_creator_bridge::OriginalWindowsObserverGeometry& geometry,SIZE_T instructions) noexcept {
        if(preownAttempted)return false;preownAttempted=true;
        try {
        (void)CheckedSourceBytes(geometry,instructions); // Whole nested graph before its first allocation.
        inputs=std::make_unique<OriginalCoreClrEntryInputs>(geometry.pointers,geometry.stringBytes,geometry.readRows);
        if(!inputs->PreownOriginal()){preownExceptionOriginal=inputs->preownExceptionOriginal;return false;}
        callerBoundary=std::make_unique<OriginalAmd64CallerBoundary>(geometry.readRows);
        if(!callerBoundary->PreownOriginal()){preownExceptionOriginal=callerBoundary->preownExceptionOriginal;return false;}
        instructionOriginals=std::make_unique<GateCoreClrInstruction[]>(instructions);instructionCapacity=instructions;
        preowned=true;return true;
        }catch(...){preownExceptionOriginal=std::current_exception();return false;}
    }
};
#if defined(TEMPLATE_NATIVE_CHAIN_REGRESSION)
bool ObserveOriginalChainRegression(const GateCoreClrInvocation&,const State&) noexcept;
#endif
class OriginalWindowsGate final : public InputGate {
#if defined(TEMPLATE_NATIVE_CHAIN_REGRESSION)
    bool chainRegressionRecorded_=false;
#endif
    std::unique_ptr<std::unique_ptr<GateCoreClrInvocation>[]> coreClrInvocations_;
    SIZE_T coreClrInvocationCapacity_=0,coreClrInvocationCount_=0;
    std::unique_ptr<OriginalCoreClrControllerCustody> coreClrCustody_;
    std::array<OriginalCoreClrCompiledGeometry,2> coreClrCompiledGeometry_;
    SIZE_T coreClrInstructionCapacity_=0,coreClrChangeMinimum_=0,hostpolicyCandidateOrdinal_=SIZE_MAX;
    static bool CandidateBaseName(const HeldNativeFile& file,const wchar_t* expected) noexcept {
        SIZE_T begin=0;
        for(SIZE_T i=0;i<file.originalPathChars;++i)
            if(file.originalPath[i]==L'/' || file.originalPath[i]==L'\\')begin=i+1;
        SIZE_T at=0;
        while(expected[at]) {
            if(begin+at>=file.originalPathChars)return false;
            wchar_t actual=file.originalPath[begin+at],wanted=expected[at];
            if(actual>=L'A' && actual<=L'Z')actual+=L'a'-L'A';
            if(wanted>=L'A' && wanted<=L'Z')wanted+=L'a'-L'A';
            if(actual!=wanted)return false;++at;
        }
        return begin+at==file.originalPathChars;
    }
    bool DeriveCompiledInstructionBacking() noexcept {
        for(SIZE_T i=0;i<inputs_.FileCount();++i) {
            if(!CandidateBaseName(inputs_.PrivateFiles()[i],L"hostpolicy.dll"))continue;
            if(hostpolicyCandidateOrdinal_!=SIZE_MAX)return Fail("compiled_hostpolicy_candidate_ambiguous");
            hostpolicyCandidateOrdinal_=i;
        }
        if(hostpolicyCandidateOrdinal_==SIZE_MAX || originalNtdllFile_==hostpolicyCandidateOrdinal_ ||
            !coreClrCompiledGeometry_[0].Derive(inputs_,hostpolicyCandidateOrdinal_) ||
            !coreClrCompiledGeometry_[1].Derive(inputs_,originalNtdllFile_))
            return Fail("compiled_instruction_backing_originals_missing");
        const SIZE_T caller=coreClrCompiledGeometry_[0].executableByteBound;
        const SIZE_T dispatcher=coreClrCompiledGeometry_[1].executableByteBound;
        if(dispatcher>SIZE_MAX-caller)return Fail("compiled_instruction_backing_sum_width");
        coreClrInstructionCapacity_=caller+dispatcher;
        return true; // Conservative storage only; no native/source/profile grant.
    }
    bool DeriveContextChangeBacking(const template_creator_bridge::OriginalWindowsObserverGeometry& geometry) noexcept {
        // One pair installation and one independently attempted disarm per T;
        // bootstrap, initializer resume, post-writer resume and at most J
        // contiguous instruction movements per I. Failed attempts consume their
        // original row and have no retry. Native terminal may leave disarm unused.
        if(coreClrInstructionCapacity_>SIZE_MAX-3 || geometry.threads>SIZE_MAX/2)
            return Fail("compiled_context_change_backing_width");
        const SIZE_T perInvocation=coreClrInstructionCapacity_+3;
        if(geometry.invocations>SIZE_MAX/perInvocation)
            return Fail("compiled_context_change_invocation_product_width");
        const SIZE_T invocations=geometry.invocations*perInvocation,threads=geometry.threads*2;
        if(invocations>SIZE_MAX-threads)return Fail("compiled_context_change_sum_width");
        coreClrChangeMinimum_=threads+invocations;
        if(geometry.readRows<coreClrChangeMinimum_)
            return Fail("original_context_change_roster_below_derived_minimum");
        return true; // Context-change rows only; not pending IO/read or whole fit.
    }
    bool FailChain(GateCoreClrInvocation& invocation,const Event& saved,const char* reason) noexcept {
        invocation.phase=GateCoreClrInvocation::Phase::Failed;
        if(!invocation.firstFailureOriginal)invocation.firstFailureOriginal=&saved;
        return Fail(reason);
    }
    SavedEventDecision PreserveForeignEventOriginal(const Event& saved,const State& state) {
        if(lifecycleStateOriginal_!=&state || !SameLifecycleCommandOriginal(state) ||
            !state.eventCount || &saved!=&state.events[state.eventCount-1] || saved.continued)
            return SavedEventDecision::Deny;
        // Perform the original precontinuation observations before recording the
        // absorbing chain failure. Its disposition remains the actual base policy,
        // never an owned exception consumption or a reconstructed handler trace.
        const auto decision=InputGate::DecideSavedEvent(saved,state,false);
        if(saved.original.dwDebugEventCode!=EXCEPTION_DEBUG_EVENT)return decision;
        for(SIZE_T i=0;i<coreClrInvocationCount_;++i) {
            auto& invocation=*coreClrInvocations_[i];
            if(invocation.stateOriginal!=&state || invocation.phase!=GateCoreClrInvocation::Phase::Walking ||
                !invocation.threadCreateOriginal ||
                !OriginalStoppedThreadOccurrenceFor(state,saved,*invocation.threadCreateOriginal))continue;
            FailChain(invocation,saved,"foreign_exception_interrupted_original_instruction_chain");
        }
        return decision;
    }
    const OriginalCommandLineObservation* NativeAmd64ProcessOriginal(const State& state,
            const Event& saved,const Event& born) const noexcept {
        if(!OriginalStoppedThreadOccurrenceFor(state,saved,born) ||
            born.createdThreadMemberOrdinal>=state.memberCount)return nullptr;
        const auto& member=state.members[born.createdThreadMemberOrdinal];
        if(member.process>=state.resourceCount)return nullptr;
        const auto& process=state.resources[member.process];
        if(process.kind!=Kind::Process || !process.acquired || process.unknownOutput || process.attempted)
            return nullptr;
        const OriginalCommandLineObservation* found=nullptr;
        for(SIZE_T i=0;i<commandCount_;++i) {
            const auto& original=commandLines_[i];
            if(original.stateOriginal!=&state || original.memberOrdinal!=born.createdThreadMemberOrdinal ||
                original.processResource!=member.process || original.processOriginal!=process.original ||
                !same_time(original.processBirth,member.birth))continue;
            if(found || original.pendingOwnerOriginal!=&pendingCallsOriginal_ || original.commandOriginal!=&command_ ||
                !original.eventOriginal || original.eventOriginal->original.dwDebugEventCode!=CREATE_PROCESS_DEBUG_EVENT ||
                original.eventOriginal->original.dwProcessId!=saved.original.dwProcessId ||
                original.eventOriginal->createdProcessResource!=member.process || !original.attempted ||
                !original.machineAddress || !original.machineReturned || original.moduleOutputUnknown ||
                !original.machinePendingOriginal ||
                !original.machinePendingOriginal->resultPublished.load(std::memory_order_acquire) ||
                original.machinePendingOriginal->operationOwner!=&original ||
                original.machinePendingOriginal->originalInput!=&original.machineInput ||
                original.machinePendingOriginal->commandOriginal!=&command_ ||
                original.machinePendingOriginal->resultShape!=template_creator_bridge::OriginalWindowsPendingResultShape::BooleanWithoutCount ||
                original.machinePendingOriginal->completionUnknown || original.machinePendingOriginal->actualObserved ||
                original.machinePendingOriginal->returned!=original.machineReturned ||
                original.machinePendingOriginal->error!=original.machineError ||
                original.machineInput.process!=process.original || original.machineInput.entry!=original.machineAddress ||
                original.machineInput.processOutput!=&original.processMachine || original.machineInput.nativeOutput!=&original.nativeMachine ||
                original.processMachine!=IMAGE_FILE_MACHINE_UNKNOWN || original.nativeMachine!=IMAGE_FILE_MACHINE_AMD64)
                return nullptr;
            found=&original;
        }
        return found;
    }
    const GateImageObservation* UniqueCurrentImage(const State& state,const Event& saved,SIZE_T selectedFile) noexcept {
        const GateImageObservation* found=nullptr;
        for(SIZE_T i=0;i<imageCount_;++i) {
            const auto& row=*images_[i];
            if(!row.associated || row.stateOriginal!=&state || row.unloadOriginal || row.selectedFile!=selectedFile ||
                row.pid!=saved.original.dwProcessId || row.memberOrdinal>=state.memberCount ||
                !same_time(row.processBirth,state.members[row.memberOrdinal].birth))continue;
            if(row.loaded.pendingOwnerOriginal!=&pendingCallsOriginal_ || row.loaded.commandOriginal!=&command_)
                return nullptr;
            if(found)return nullptr;found=&row;
        }return found;
    }
    const GateImageObservation* UniquePairImage(const State& state,const Event& saved,
            const OriginalCoreClrContextChange& pair) noexcept {
        const GateImageObservation* found=nullptr;
        for(SIZE_T i=0;i<imageCount_;++i) {
            const auto& row=*images_[i];
            if(!row.associated || row.stateOriginal!=&state || row.unloadOriginal ||
                row.pid!=saved.original.dwProcessId || row.memberOrdinal>=state.memberCount ||
                !same_time(row.processBirth,state.members[row.memberOrdinal].birth) ||
                row.coreClrInitialize.result!=OriginalCoreClrInitializeExport::Result::Direct ||
                row.coreClrErrorWriter.result!=OriginalCoreClrInitializeExport::Result::Direct ||
                reinterpret_cast<std::uintptr_t>(row.coreClrInitialize.address)!=pair.address)continue;
            if(row.loaded.pendingOwnerOriginal!=&pendingCallsOriginal_ || row.loaded.commandOriginal!=&command_)
                return nullptr;
            if(found)return nullptr;found=&row;
        }return found;
    }
    bool InstallOriginalCoreClrPairs(const State& state,const Event& saved) noexcept {
        if(!coreClrCustody_||!parserRuntime_||lifecycleStateOriginal_!=&state||!SameLifecycleCommandOriginal(state))
            return Fail("pair_install_original_owners_unbound");
        const auto code=saved.original.dwDebugEventCode;
        if(code!=LOAD_DLL_DEBUG_EVENT&&code!=CREATE_PROCESS_DEBUG_EVENT&&code!=CREATE_THREAD_DEBUG_EVENT)return true;
        const GateImageObservation* image=nullptr;
        for(SIZE_T i=0;i<imageCount_;++i){const auto& row=*images_[i];
            if(!row.associated||row.stateOriginal!=&state||row.unloadOriginal||
               row.pid!=saved.original.dwProcessId||!row.loadOriginal||row.memberOrdinal>=state.memberCount||
               !same_time(row.processBirth,state.members[row.memberOrdinal].birth))continue;
            const auto relation=parserRuntime_->OriginalCoreClrObservationAsset(state,*row.loadOriginal,row.selectedFile,row.loaded);
            if(relation==OriginalParserRuntimeAssociation::CoreClrObservationAsset::Unrelated)continue;
            if(relation!=OriginalParserRuntimeAssociation::CoreClrObservationAsset::Matched)
                return Fail("pair_install_selected_package_occurrence_unresolved");
            if(image)return Fail("pair_install_selected_image_ambiguous");image=&row;
        }
        if(!image)return true; // No original CoreCLR occurrence yet.
        if(image->coreClrInitialize.result!=OriginalCoreClrInitializeExport::Result::Direct||
           image->coreClrErrorWriter.result!=OriginalCoreClrInitializeExport::Result::Direct)
            return Fail("pair_install_original_direct_exports_unresolved");
        for(SIZE_T i=0;i<coreClrCustody_->PrivateThreadCount();++i){
            const auto& thread=coreClrCustody_->PrivateThreads()[i];
            if(thread.stateOriginal!=&state||!thread.createOriginal||
               thread.createOriginal->original.dwProcessId!=saved.original.dwProcessId||
               thread.exitThreadOriginal||thread.lastThreadProcessExitOriginal)continue;
            if(!OriginalStoppedThreadOccurrenceFor(state,saved,*thread.createOriginal))
                return Fail("pair_install_live_thread_occurrence_unresolved");
            if(thread.installationOriginal){
                if(thread.installationImageEventOriginal!=image->loadOriginal||thread.installationImageOriginal!=&image->loaded||
                   !thread.currentPairOriginal||!thread.currentPairOriginal->effectiveVerified||thread.disarmOriginal||
                   thread.currentPairOriginal->before.commandOriginal!=&command_||
                   thread.currentPairOriginal->before.pendingOwnerOriginal!=&pendingCallsOriginal_)
                    return Fail("pair_install_existing_generation_unresolved");
                continue;
            }
            auto* installation=coreClrCustody_->ReservePairInstallationOriginal(state,*thread.createOriginal,*image->loadOriginal,image->loaded);
            if(!installation||!installation->InstallPair(state,saved,*thread.createOriginal,
                reinterpret_cast<DWORD64>(image->coreClrInitialize.address),
                reinterpret_cast<DWORD64>(image->coreClrErrorWriter.address),pendingCallsOriginal_,command_))
                return Fail("pair_install_native_original_failed_or_unknown");
        }
        return true; // Observation ownership only; independent Consume remains closed.
    }
    bool FailurePairContinuationOriginal(const State& state,const DEBUG_EVENT& event) noexcept {
        if(!coreClrCustody_||!state.eventCount||&event!=&state.events[state.eventCount-1].original||
           state.events[state.eventCount-1].continued||!pendingCallsOriginal_.CommandNativeCallsSettled(&command_))return false;
        if(event.dwDebugEventCode==EXIT_THREAD_DEBUG_EVENT||event.dwDebugEventCode==EXIT_PROCESS_DEBUG_EVENT){
            const auto& saved=state.events[state.eventCount-1];
            const bool enrolled=coreClrCustody_->ObserveSavedEvent(state,saved);
            // Retain aggregate unknown/failure in the SAME custody. Only an
            // independently exact native terminal relation can remain eligible.
            if(!enrolled&&!coreClrCustody_->TerminalOriginalCoversPairBorrowers(state,saved))return false;
        }
        const bool debugException=event.dwDebugEventCode==EXCEPTION_DEBUG_EVENT&&
            event.u.Exception.ExceptionRecord.ExceptionCode==EXCEPTION_SINGLE_STEP;
        if(event.dwDebugEventCode!=EXCEPTION_DEBUG_EVENT&&
           !coreClrCustody_->DisarmStoppedPairsOriginal(state,state.events[state.eventCount-1],pendingCallsOriginal_,command_))return false;
        for(SIZE_T i=0;i<coreClrCustody_->PrivateThreadCount();++i){
            const auto& thread=coreClrCustody_->PrivateThreads()[i];
            if(thread.stateOriginal!=&state||!thread.createOriginal||
               thread.createOriginal->original.dwProcessId!=event.dwProcessId||!thread.installationOriginal||
               !thread.installationOriginal->setAttempted||thread.NativeTerminalContinued())continue;
            const auto& saved=state.events[state.eventCount-1];
            if((event.dwDebugEventCode==EXIT_THREAD_DEBUG_EVENT&&thread.exitThreadOriginal==&saved)||
               (event.dwDebugEventCode==EXIT_PROCESS_DEBUG_EVENT&&thread.lastThreadProcessExitOriginal==&saved))continue;
            if(thread.disarmOriginal&&thread.disarmOriginal->effectiveVerified)continue;
            if(!thread.currentPairOriginal||!thread.currentPairOriginal->effectiveVerified)return false;
            // The separate typed owned-#DB cleanup validates cause and restores
            // before consumption. Other failed stops must retain custody until
            // their concrete disarm path settles; no implicit live-slot resume.
            if(!debugException)return false;
        }
        return true;
    }
    bool CaptureWriterCandidate(const State& state,const Event& saved,const Event& born) noexcept {
        const auto* current=coreClrCustody_?coreClrCustody_->CurrentPairOriginal(state,born):nullptr;
        if(!current || !coreClrCustody_->OwnsChangeOriginal(*current) || !current->effectiveVerified)
            return Fail("writer_current_pair_original_missing");
        const auto* image=UniquePairImage(state,saved,*current);
        const auto* caller=UniqueCurrentImage(state,saved,hostpolicyCandidateOrdinal_);
        if(!image || !caller)return Fail("writer_current_image_originals_missing");
        GateCoreClrInvocation* post=nullptr;
        for(SIZE_T i=0;i<coreClrInvocationCount_;++i) {
            auto& prior=*coreClrInvocations_[i];
            if(prior.stateOriginal!=&state || prior.threadCreateOriginal!=&born)continue;
            if(prior.phase==GateCoreClrInvocation::Phase::AwaitWriterReturn || prior.phase==GateCoreClrInvocation::Phase::Walking)
                return FailChain(prior,saved,"writer_reentry_with_unresolved_original_chain");
            if(prior.phase==GateCoreClrInvocation::Phase::EntryCaptured && !prior.postWriterObservation.attempted) {
                if(post)return FailChain(prior,saved,"writer_post_call_owner_ambiguous");post=&prior;
            }
        }
        if(post) {
            auto& observation=post->postWriterObservation;
            if(!observation.Capture(state,saved,born,*current,image->coreClrErrorWriter,caller->loaded,
                coreClrCompiledGeometry_[0],pendingCallsOriginal_,command_) || observation.nonNullPrecallCandidate)
                return FailChain(*post,saved,"writer_post_call_source_or_reentry_unknown");
            auto* movement=coreClrCustody_->ReservePairMovementOriginal(state,born);post->postWriterMovementOriginal=movement;
            if(!movement || movement->precedingPairOriginal!=current || !movement->MovePair(state,saved,*current,
                observation.context,reinterpret_cast<std::uintptr_t>(image->coreClrErrorWriter.address),pendingCallsOriginal_,command_))
                return FailChain(*post,saved,"writer_post_call_resume_unknown");
            return true; // No initializer return/out-parameter/ACK is inferred.
        }
        if(coreClrInvocationCount_==coreClrInvocationCapacity_)return Fail("writer_invocation_roster_exhausted");
        auto& invocation=*coreClrInvocations_[coreClrInvocationCount_++];
        invocation.stateOriginal=&state;invocation.threadCreateOriginal=&born;invocation.writerEventOriginal=&saved;
        if(!invocation.writerBootstrap.Capture(state,saved,born,*current,image->coreClrErrorWriter,caller->loaded,
            coreClrCompiledGeometry_[0],pendingCallsOriginal_,command_) || !invocation.writerBootstrap.nonNullPrecallCandidate)
            return FailChain(invocation,saved,"writer_pre_call_candidate_unknown_or_null");
        invocation.continuationCandidate=invocation.writerBootstrap.returnCandidate;
        invocation.predictedAddress=invocation.continuationCandidate;
        invocation.phase=GateCoreClrInvocation::Phase::AwaitWriterReturn;
        auto* movement=coreClrCustody_->ReservePairMovementOriginal(state,born);invocation.bootstrapMovementOriginal=movement;
        if(!movement || movement->precedingPairOriginal!=current || !movement->MovePair(state,saved,*current,
            invocation.writerBootstrap.context,invocation.continuationCandidate,pendingCallsOriginal_,command_))
            return FailChain(invocation,saved,"writer_continuation_guard_unknown");
        return true; // Bootstrap is not an instruction chain or preceding CALL.
    }
    bool CaptureWriterContinuation(const State& state,const Event& saved,GateCoreClrInvocation& invocation) noexcept {
        const auto* current=coreClrCustody_ && invocation.threadCreateOriginal?
            coreClrCustody_->CurrentPairOriginal(state,*invocation.threadCreateOriginal):nullptr;
        if(invocation.phase!=GateCoreClrInvocation::Phase::AwaitWriterReturn || invocation.stateOriginal!=&state ||
            invocation.continuationEventOriginal || !invocation.writerEventOriginal ||
            invocation.writerEventOriginal==&saved || !OriginalChainEventUsefulContinued(invocation.writerEventOriginal) ||
            !invocation.writerBootstrap.captured || !invocation.writerBootstrap.nonNullPrecallCandidate ||
            !current || current!=invocation.bootstrapMovementOriginal || !current->effectiveVerified ||
            !current->movableEnabled || current->movableAddress!=invocation.continuationCandidate ||
            invocation.predictedAddress!=invocation.continuationCandidate)
            return FailChain(invocation,saved,"writer_continuation_original_transition_missing");
        // The first chain row itself captures the exact saved execution cause,
        // current complete context and instruction at the candidate. Merely
        // observing the writer/stack never publishes this transition.
        invocation.continuationEventOriginal=&saved;
        invocation.phase=GateCoreClrInvocation::Phase::Walking;
        return CapturePredictedChainStep(state,saved,invocation);
    }
    bool ObserveHostContractCallbackRangesOriginal(const State& state,const Event& saved,
            GateCoreClrInvocation& invocation) noexcept {
        const auto& contract=invocation.propertiesOriginal.hostContractOriginal;
        if(!contract.representationObserved || contract.inputsOriginal!=invocation.inputs.get() ||
            !invocation.inputs || invocation.inputs->stateOriginal!=&state || invocation.inputs->stoppedOriginal!=&saved)
            return false;
        const auto* image=UniqueCurrentImage(state,saved,hostpolicyCandidateOrdinal_);
        if(!image || !image->loaded.complete || image->loaded.failed || !image->loaded.nativeBase)return false;
        const DWORD64 addresses[]={reinterpret_cast<DWORD64>(contract.first.get_runtime_property),
            reinterpret_cast<DWORD64>(contract.first.bundle_probe),reinterpret_cast<DWORD64>(contract.first.pinvoke_override),
            reinterpret_cast<DWORD64>(contract.first.external_assembly_probe)};
        const auto base=reinterpret_cast<std::uintptr_t>(image->loaded.nativeBase);
        for(SIZE_T i=0;i<invocation.hostCallbackRangesOriginal.size();++i) {
            auto& row=invocation.hostCallbackRangesOriginal[i];
            if(row.result!=OriginalHostContractCallbackRange::Result::Unobserved)return false;
            row.contractOriginal=&contract;row.imageOriginal=image;row.stoppedOriginal=&saved;
            row.addressOriginal=addresses[i];
            if(!row.addressOriginal){row.result=OriginalHostContractCallbackRange::Result::Null;continue;}
            if(row.addressOriginal<base || row.addressOriginal-base>MAXDWORD ||
                row.addressOriginal-base>=image->loaded.imageSize) {
                row.result=OriginalHostContractCallbackRange::Result::OutsideImage;continue;
            }
            const DWORD rva=static_cast<DWORD>(row.addressOriginal-base);
            if(!coreClrCompiledGeometry_[0].ContainingRuntimeFunction(image->loaded,rva,
                    row.functionOriginal,row.functionOrdinal)) {
                row.result=OriginalHostContractCallbackRange::Result::NoSupportedRange;continue;
            }
            row.functionStartObserved=rva==row.functionOriginal.BeginAddress;
            const SIZE_T extent=SIZE_T(row.functionOriginal.EndAddress)-row.functionOriginal.BeginAddress;
            row.result=coreClrCompiledGeometry_[0].MatchesMappedCodeSpan(image->loaded,row.functionOriginal.BeginAddress,extent)?
                OriginalHostContractCallbackRange::Result::MatchedRange:OriginalHostContractCallbackRange::Result::CodeMismatch;
        }
        // Actual callback operands and retained load/code correspondence only.
        // Leaf/thunk/fixup forms remain recorded unresolved for the full profile.
        return true;
    }
    bool CapturePredictedEntry(const State& state,const Event& saved,GateCoreClrInvocation& invocation) noexcept {
        const auto* current=coreClrCustody_ && invocation.threadCreateOriginal?
            coreClrCustody_->CurrentPairOriginal(state,*invocation.threadCreateOriginal):nullptr;
        if(invocation.phase!=GateCoreClrInvocation::Phase::Walking || invocation.stateOriginal!=&state ||
            !current || !coreClrCustody_->OwnsChangeOriginal(*current) || !current->effectiveVerified ||
            current->movableEnabled || invocation.predictedAddress!=current->address ||
            !invocation.continuationEventOriginal || !invocation.activeCallOriginal ||
            !invocation.instructionCount || invocation.instructionCount>invocation.instructionCapacity)
            return FailChain(invocation,saved,"initializer_entry_before_complete_chain");
        const auto& previous=invocation.instructionOriginals[invocation.instructionCount-1];
        if(!OriginalChainEventUsefulContinued(previous.context.stoppedOriginal)||
           previous.movementOriginal!=current||!previous.step.predicted||previous.step.nextAddress!=invocation.predictedAddress)
            return FailChain(invocation,saved,"initializer_entry_previous_native_continue_missing");
        invocation.entryPredecessorOriginal=&previous;
        const auto* image=UniquePairImage(state,saved,*current);invocation.entryPairOriginal=current;
        if(!image || !invocation.entryContext.Capture(state,saved,pendingCallsOriginal_,command_,invocation.threadCreateOriginal) ||
            !invocation.entryCause.Observe(state,saved,invocation.entryContext,*current,current->slot) ||
            !invocation.inputs->CaptureOriginal(invocation.entryContext,image->coreClrInitialize,pendingCallsOriginal_,command_) ||
            !invocation.inputs->RevalidateOriginal() || !invocation.propertiesOriginal.Observe(*invocation.inputs))
            return FailChain(invocation,saved,"initializer_entry_original_capture");
#if defined(_M_AMD64) || defined(__x86_64__)
        if(!OriginalChainPredecessorContinued(previous,invocation.entryContext,current)||
           OriginalChainNextStopFailure(previous,invocation.activeCallOriginal,invocation.entryContext.effective))
            return FailChain(invocation,saved,"initializer_entry_actual_predecessor_mismatch");
        invocation.initializerCallOriginal=invocation.activeCallOriginal;
        const auto& call=invocation.initializerCallOriginal->step;
        const auto& before=call.before.effective;const auto& entry=invocation.entryContext.effective;
        if(!call.predicted || !call.callStackCaptured || call.instruction.flow!=OriginalAmd64Instruction::Flow::Call ||
            before.Rsp<8 || entry.Rsp!=before.Rsp-8 || before.Rip>UINTPTR_MAX-call.instruction.length ||
            invocation.inputs->returnAddress!=before.Rip+call.instruction.length ||
            entry.Rcx!=before.Rcx || entry.Rdx!=before.Rdx || static_cast<DWORD>(entry.R8)!=static_cast<DWORD>(before.R8) ||
            entry.R9!=before.R9 || invocation.inputs->valuesAddress!=call.callStack[4] ||
            invocation.inputs->hostHandleAddress!=call.callStack[5] || invocation.inputs->domainIdAddress!=call.callStack[6])
            return FailChain(invocation,saved,"initializer_actual_call_ABI_or_push_mismatch");
        if(!invocation.propertiesOriginal.hostContractOriginal.Capture(*invocation.inputs))
            return FailChain(invocation,saved,"initializer_original_host_contract_capture");
        if(!ObserveHostContractCallbackRangesOriginal(state,saved,invocation))
            return FailChain(invocation,saved,"initializer_host_contract_callback_occurrences");
        auto* movement=coreClrCustody_->ReservePairMovementOriginal(state,*invocation.threadCreateOriginal);
        invocation.entryMovementOriginal=movement;
        if(!movement || movement->precedingPairOriginal!=current || !movement->MovePair(state,saved,*current,
            invocation.entryContext,reinterpret_cast<std::uintptr_t>(image->coreClrErrorWriter.address),pendingCallsOriginal_,command_))
            return FailChain(invocation,saved,"initializer_owned_resume_unknown");
        invocation.phase=GateCoreClrInvocation::Phase::EntryCaptured;
        return true; // Source/property/CPU/exclusion validation still precedes Consume.
#else
        return FailChain(invocation,saved,"initializer_entry_architecture_unsupported");
#endif
    }
    bool CapturePredictedChainStep(const State& state,const Event& saved,GateCoreClrInvocation& invocation) noexcept {
        if(invocation.phase!=GateCoreClrInvocation::Phase::Walking || invocation.stateOriginal!=&state ||
            !invocation.threadCreateOriginal ||
            !OriginalStoppedThreadOccurrenceFor(state,saved,*invocation.threadCreateOriginal))
            return FailChain(invocation,saved,"instruction_chain_original_thread_missing");
        const auto* current=coreClrCustody_?coreClrCustody_->CurrentPairOriginal(state,*invocation.threadCreateOriginal):nullptr;
        if(!current || !coreClrCustody_->OwnsChangeOriginal(*current) || !current->effectiveVerified ||
            !current->movableEnabled || !OriginalChainRowAvailable(invocation.instructionCount,invocation.instructionCapacity))
            return FailChain(invocation,saved,"instruction_chain_current_pair_or_capacity");
        // Same finite row is attached before the first native context read.
        auto& row=invocation.instructionOriginals[invocation.instructionCount++];row.pairOriginal=current;
        if(!row.context.Capture(state,saved,pendingCallsOriginal_,command_,invocation.threadCreateOriginal) ||
            !row.cause.Observe(state,saved,row.context,*current,current->movableSlot))
            return FailChain(invocation,saved,"instruction_chain_cause_or_context");
#if defined(_M_AMD64) || defined(__x86_64__)
        if(row.context.effective.Rip!=invocation.predictedAddress)
            return FailChain(invocation,saved,"instruction_chain_unpredicted_stop");
        if(invocation.instructionCount==1&&parserRuntime_){
            const auto* caller=UniqueCurrentImage(state,saved,hostpolicyCandidateOrdinal_);
            const auto& sources=parserRuntime_->PrivateCoreClrPropertySources();
            if(caller&&sources.exactSourceObserved&&sources.filesOriginal==&inputs_)
                invocation.continuationSourceCandidateOriginal.Observe(caller->compilerRangesOriginal,
                    row.context.effective.Rip,sources.ordinals[0],sources.bodies[0],sources.extents[0]);
        }
        if(invocation.instructionCount>1){
            const auto& previous=invocation.instructionOriginals[invocation.instructionCount-2];
            if(!OriginalChainPredecessorContinued(previous,row))
                return FailChain(invocation,saved,"instruction_chain_previous_native_continue_missing");
            if(const auto* failure=OriginalChainNextStopFailure(previous,invocation.activeCallOriginal,row.context.effective))
                return FailChain(invocation,saved,failure);
            row.predecessorOriginal=&previous;
            if(previous.step.instruction.flow==OriginalAmd64Instruction::Flow::Return){
                invocation.activeCallOriginal=previous.returningCallOriginal->parentCallOriginal;
            }
        }
        row.parentCallOriginal=invocation.activeCallOriginal;
        for(SIZE_T i=0;i+1<invocation.instructionCount;++i) {
            const auto& prior=invocation.instructionOriginals[i];
            if(prior.context.captured && prior.context.effective.Rip==row.context.effective.Rip&&
               prior.parentCallOriginal==row.parentCallOriginal)
                return FailChain(invocation,saved,"instruction_chain_cycle_without_original_bound");
        }
        const GateImageObservation* selected=nullptr;const OriginalCoreClrCompiledGeometry* geometry=nullptr;
        for(SIZE_T i=0;i<imageCount_;++i) {
            const auto& image=*images_[i];
            if(!image.associated || image.stateOriginal!=&state || image.unloadOriginal ||
                image.pid!=saved.original.dwProcessId || image.memberOrdinal>=state.memberCount ||
                !same_time(image.processBirth,state.members[image.memberOrdinal].birth))continue;
            const auto base=reinterpret_cast<std::uintptr_t>(image.loaded.nativeBase);
            if(row.context.effective.Rip<base || row.context.effective.Rip-base>=image.loaded.imageSize)continue;
            const OriginalCoreClrCompiledGeometry* mapped=nullptr;
            for(const auto& candidate:coreClrCompiledGeometry_)
                if(candidate.derived && candidate.fileOrdinal==image.selectedFile)mapped=&candidate;
            if(!mapped || selected)return FailChain(invocation,saved,"instruction_chain_compiled_range_unknown_or_ambiguous");
            selected=&image;geometry=mapped;
        }
        if(!selected || !geometry || !row.step.Capture(row.cause,selected->loaded,*geometry))
            return FailChain(invocation,saved,"instruction_chain_native_instruction_capture");
        const auto* compilerGeometry=selected->compilerRangesOriginal.geometryOriginal;
        if(compilerGeometry&&compilerGeometry->filesOriginal==geometry->filesOriginal&&
           compilerGeometry->fileOriginal==geometry->fileOriginal&&compilerGeometry->bodyOriginal==geometry->bodyOriginal&&
           compilerGeometry->bodyExtent==geometry->bodyExtent&&selected->compilerRangesOriginal.imageOriginal==&selected->loaded)
            row.compilerNodeOriginal.Observe(selected->compilerRangesOriginal,row.step.instruction);
        if(row.step.instruction.flow==OriginalAmd64Instruction::Flow::Call) {
            invocation.activeCallOriginal=&row;
        }else if(row.step.instruction.flow==OriginalAmd64Instruction::Flow::Return){
            const auto* call=invocation.activeCallOriginal;
            if(!OriginalChainReturnMatchesCall(row,call))
                return FailChain(invocation,saved,"instruction_chain_return_without_original_call");
            row.returningCallOriginal=call;
        }
        // Cause was retained and classified BEFORE this method can clear owned
        // status. No original-install fallback after an unknown movement.
        auto* movement=coreClrCustody_->ReservePairMovementOriginal(state,*invocation.threadCreateOriginal);
        row.movementOriginal=movement;
        if(!movement || movement->precedingPairOriginal!=current ||
            !movement->MovePair(state,saved,*current,row.context,row.step.nextAddress,pendingCallsOriginal_,command_))
            return FailChain(invocation,saved,"instruction_chain_pair_movement_unknown");
        invocation.predictedAddress=row.step.nextAddress;
        return true; // Transition prepared only; saved-event Consume is independent.
#else
        return FailChain(invocation,saved,"instruction_chain_architecture_unsupported");
#endif
    }
    const State* lifecycleStateOriginal_=nullptr;
    const wchar_t* nativeImageOriginal_=nullptr;
    const wchar_t* nativeLineOriginal_=nullptr;
    const wchar_t* nativeCwdOriginal_=nullptr;
    const wchar_t* nativeEnvironmentOriginal_=nullptr;
    bool SameLifecycleCommandOriginal(const State& state) const noexcept {
        return command_.copied && nativeImageOriginal_ && nativeLineOriginal_ && nativeCwdOriginal_ && nativeEnvironmentOriginal_ &&
            nativeImageOriginal_==state.originalImage.data() && nativeLineOriginal_==state.originalCommand.data() &&
            nativeCwdOriginal_==state.originalCwd.data() && nativeEnvironmentOriginal_==state.originalEnvironment.data() &&
            state.imageChars==command_.imageUnits && state.commandChars==command_.commandUnits &&
            state.cwdChars==command_.cwdUnits && state.environmentChars==command_.environmentUnits;
    }
    bool BindLifecycleCommandOriginal(const State& state) noexcept {
        if(!SameLifecycleCommandOriginal(state) || (lifecycleStateOriginal_ && lifecycleStateOriginal_!=&state))return false;
        if(!lifecycleStateOriginal_)lifecycleStateOriginal_=&state;return true;
    }
    OriginalPrivateIngress& ingress_;
    NativeHeldFiles& inputs_;
    template_creator_bridge::WindowsOriginalCommand& command_;
    template_creator_bridge::OriginalWindowsPendingCalls& pendingCallsOriginal_;
    const void* const commandRouteOriginal_;
    const template_creator_bridge::OriginalWindowsNoticeOwner* noticeBodiesOriginal_=nullptr;
    std::unique_ptr<GateCallerObservation[]> callers_;
    std::unique_ptr<OriginalCommandLineObservation[]> commandLines_;
    std::unique_ptr<OriginalHelperSourceProjection[]> helperProjections_;
    SIZE_T commandCapacity_ = 0, commandCount_ = 0;
    std::array<wchar_t,32769> crtDerivedScratch_{};
    std::unique_ptr<std::unique_ptr<GateImageObservation>[]> images_;
    SIZE_T callerCapacity_ = 0, callerCount_ = 0, imageCapacity_ = 0, imageCount_ = 0;
    SIZE_T originalNtdllFile_ = SIZE_MAX;
    SIZE_T originalLaunchFile_ = SIZE_MAX;
    const OriginalWindowsFixtureRoot* fixtureRootOriginal_=nullptr;
    bool fixtureEnvironmentAssociated_=false;
    std::array<OriginalFixtureRelativeFile,2> canonicalAliasSources_;
    SIZE_T canonicalAliasSourceCount_=0;
    std::array<std::unique_ptr<OriginalAliasCodeGuard>,16> aliasCodeGuards_;
    std::array<SIZE_T,2> aliasCodeGuardCounts_{};
    std::array<OriginalFixtureRelativeFile,3> canonicalHelperSources_;
    SIZE_T canonicalHelperSourceCount_=0;
    std::array<std::unique_ptr<OriginalHelperCodeGuard>,12> helperCodeGuards_;
    std::array<SIZE_T,2> helperCodeGuardCounts_{};
    std::array<OriginalFixtureRelativeFile,2> canonicalParserSources_;
    SIZE_T canonicalParserSourceCount_=0;
    // Two immutable parser inputs for each actual helper occurrence. The
    // unchanged producer has two helpers, two invocation forms and three cases.
    std::array<std::unique_ptr<OriginalParserInputCapture>,24> parserInputs_;
    SIZE_T parserInputCount_=0;
    std::unique_ptr<OriginalParserRuntimeAssociation> parserRuntime_;
    bool IsUnchangedSuiteCommand() const noexcept {
        auto* snapshot=command_.snapshotOriginal;
        if(!snapshot || !PyTuple_Check(snapshot) || PyTuple_GET_SIZE(snapshot)!=5)return false;
        auto* command=PyTuple_GET_ITEM(snapshot,0);
        if(!command || !PyTuple_Check(command) || PyTuple_GET_SIZE(command)!=6)return false;
        auto* argv=PyTuple_GET_ITEM(command,1);
        if(!argv || !PyTuple_CheckExact(argv) || PyTuple_GET_SIZE(argv)!=4)return false;
        const char* expected[]={"--test","tests/template-contract.test.mjs","tests/development-candidate-workflow.test.mjs","tests/qualification-publication.test.mjs"};
        for(Py_ssize_t i=0;i<4;++i) {
            auto* value=PyTuple_GET_ITEM(argv,i);
            if(!value || !PyUnicode_CheckExact(value) || PyUnicode_GET_LENGTH(value)!=static_cast<Py_ssize_t>(strlen(expected[i])))return false;
            for(Py_ssize_t j=0;j<PyUnicode_GET_LENGTH(value);++j)
                if(PyUnicode_READ_CHAR(value,j)!=static_cast<unsigned char>(expected[i][j]))return false;
        }
        return true;
    }
    bool LiveFixtureEnvironment() noexcept {
        if(!fixtureRootOriginal_)return true; // Bootstrap has its original ENV.
        const auto& root=*fixtureRootOriginal_;
        if(!fixtureEnvironmentAssociated_||!root.created||!root.environmentPathBound||root.rootUnknown||
            root.rootCloseAttempted||root.dispositionAttempted)return Fail("private_fixture_root_lifetime_unavailable");
        return true;
    }
    bool AssociateFixtureEnvironment() noexcept {
        if(!fixtureRootOriginal_)return true;
        const auto& root=*fixtureRootOriginal_;
        SIZE_T offset=0;unsigned found=0;
        while(offset<command_.environmentUnits&&command_.environment[offset]) {
            const SIZE_T begin=offset;
            while(offset<command_.environmentUnits&&command_.environment[offset]&&command_.environment[offset]!=u'=')++offset;
            if(offset==command_.environmentUnits||command_.environment[offset]!=u'=')return false;
            const SIZE_T nameUnits=offset-begin;
            const auto named=[&](const wchar_t* expected,SIZE_T units) noexcept {
                if(nameUnits!=units)return false;
                for(SIZE_T i=0;i<units;++i) {
                    auto c=command_.environment[begin+i];if(c>=u'a'&&c<=u'z')c-=u'a'-u'A';
                    if(c!=expected[i])return false;
                }
                return true;
            };
            const SIZE_T value=++offset;
            while(offset<command_.environmentUnits&&command_.environment[offset])++offset;
            if(offset==command_.environmentUnits)return false;
            const unsigned bit=named(L"TEMP",4)?1:named(L"TMP",3)?2:0;
            if(bit) {
                if(found&bit)return false;
                if(offset-value!=root.environmentPathUnits||
                    memcmp(command_.environment.data()+value,root.environmentPath.data(),root.environmentPathUnits*sizeof(wchar_t)))return false;
                found|=bit;
            }
            ++offset;
        }
        fixtureEnvironmentAssociated_=found==3;
        return fixtureEnvironmentAssociated_;
    }
    std::array<bool, MemberLimit> initialBreakpointConsumed_{};
    bool failed_ = false;
    SIZE_T selectedCallerRowsOriginal_=0,selectedImageRowsOriginal_=0;
    const GateImageGeometry* selectedImageGeometryOriginal_=nullptr;
    bool preownAttempted_=false,preownReturned_=false,preowned_=false;
    std::exception_ptr preownExceptionOriginal_;
    const char* firstFailure_ = nullptr;
    bool Fail(const char* operation) noexcept {
        failed_ = true; if (!firstFailure_) firstFailure_ = operation; return false;
    }
    static bool SameFile(const FileIdentityObservation& a, const FileIdentityObservation& b) noexcept {
        return a.captured && b.captured && a.id.VolumeSerialNumber == b.id.VolumeSerialNumber &&
            memcmp(a.id.FileId.Identifier, b.id.FileId.Identifier, sizeof(a.id.FileId.Identifier)) == 0;
    }
    bool LiveInputs() noexcept {
        if (inputs_.Failed() || !inputs_.FileCount()) return Fail("selected_native_inputs_failed_or_absent");
        const auto* files = inputs_.PrivateFiles();
        for (SIZE_T i=0; i<inputs_.FileCount(); ++i)
            if (!files[i].valid || !files[i].original || files[i].closeAttempted)
                return Fail("selected_native_input_hold_lost_or_unknown");
        return true;
    }
    bool LiveOriginalCaller() noexcept {
        if (!ingress_.Captured()) return Fail("original_private_ingress_unbound");
        if (callerCount_ == callerCapacity_) return Fail("caller_observation_capacity_before_call");
        GateCallerObservation& row = callers_[callerCount_++];
        const HANDLE process = ingress_.OriginalCapabilities()[1].original;
        row.waitReturned = WaitForSingleObject(process, 0);
        if (row.waitReturned == WAIT_FAILED) row.waitError = GetLastError();
        row.timesReturned = GetProcessTimes(process, &row.born, &row.exited, &row.kernel, &row.user);
        if (!row.timesReturned) row.timesError = GetLastError();
        if (row.waitReturned != WAIT_TIMEOUT || !row.timesReturned ||
            !same_time(row.born, ingress_.OriginalCallerBirth()) || positive(row.exited))
            return Fail("original_caller_lifetime_changed_or_unknown");
        return true;
    }
    static SIZE_T MemberOrdinal(DWORD pid, const State& state) noexcept {
        for (SIZE_T i=0; i<state.memberCount; ++i) if (state.members[i].pid == pid) return i;
        return MemberLimit;
    }
    bool OwnedMember(DWORD pid, const State& state, SIZE_T& ordinal) noexcept {
        ordinal = MemberOrdinal(pid, state);
        if (ordinal == MemberLimit) return Fail("event_not_original_enrolled_member");
        const auto& member = state.members[ordinal];
        if (member.process >= state.resourceCount) return Fail("member_retained_process_missing");
        const auto& process = state.resources[member.process];
        if (!process.acquired || process.unknownOutput || process.attempted || !positive(member.birth))
            return Fail("member_retained_lifetime_unavailable");
        return true;
    }
    bool ObserveImage(const DEBUG_EVENT& event, const State& state, SIZE_T memberOrdinal) noexcept {
        if (imageCount_ == imageCapacity_) return Fail("image_observation_capacity_before_call");
        auto& row = *images_[imageCount_++]; row.attempted = true; row.pid = event.dwProcessId;
        if (!state.eventCount || &event != &state.events[state.eventCount-1].original)
            return Fail("image_observation_not_exact_saved_event");
        const Event& saved = state.events[state.eventCount-1];
        if(memberOrdinal>=state.memberCount)return Fail("image_original_member_missing");
        row.stateOriginal=&state;row.loadOriginal=&saved;row.memberOrdinal=memberOrdinal;
        row.processBirth=state.members[memberOrdinal].birth;
        if (saved.retainedImageResource >= state.resourceCount || saved.originalImageFileResource >= state.resourceCount)
            return Fail("saved_event_image_resource_association_missing");
        const Resource& retained = state.resources[saved.retainedImageResource];
        const Resource& acquiredOriginal = state.resources[saved.originalImageFileResource];
        if (retained.kind != Kind::Image || !retained.acquired || retained.unknownOutput || retained.attempted ||
            acquiredOriginal.kind != Kind::EventFile || retained.duplicateSource != acquiredOriginal.original ||
            !retained.duplicateAttempted || !retained.duplicateResult)
            return Fail("saved_event_original_image_duplicate_unavailable");
        HANDLE file = retained.original;
        const void* base = event.dwDebugEventCode == CREATE_PROCESS_DEBUG_EVENT ?
            event.u.CreateProcessInfo.lpBaseOfImage : event.u.LoadDll.lpBaseOfDll;
        if (!file || file == INVALID_HANDLE_VALUE || !base) return Fail("original_image_file_or_base_unavailable");
        row.actualFile.Capture(file);
        if (!row.actualFile.captured) return Fail("original_event_image_file_observation_failed");
        const auto* selected = inputs_.PrivateFiles();
        for (SIZE_T i=0; i<inputs_.FileCount(); ++i)
            if (selected[i].valid && !selected[i].closeAttempted && SameFile(selected[i].first, row.actualFile)) {
                row.selectedFile = i; break;
            }
        if (row.selectedFile == SIZE_MAX) return Fail("event_image_not_selected_original_file");
        // The root image must be the actual file selected for this exact
        // original command, rather than merely any image in its dependency set.
        // Descendant images keep their own selected-image association below.
        if (event.dwDebugEventCode == CREATE_PROCESS_DEBUG_EVENT && event.dwProcessId == state.root &&
            (originalLaunchFile_ >= inputs_.FileCount() ||
             !SameFile(selected[originalLaunchFile_].first, row.actualFile)))
            return Fail("root_event_image_differs_from_original_launch_file");
        const auto& member = state.members[memberOrdinal];
        const HANDLE process = state.resources[member.process].original;
        if (!row.loaded.Capture(process, base, member.birth,pendingCallsOriginal_,&command_))
            return Fail("original_loaded_image_observation_failed");
        // Retain even unsupported/malformed TLS outcomes for the independent
        // actor consumer. This observation never authorizes native continuation.
        row.tlsOriginal.Observe(row.loaded);
        row.codeViewOriginal.Observe(row.loaded);
        row.compilerArtifactCandidateAttempted=true;
        if(parserRuntime_&&row.codeViewOriginal.result==OriginalLoadedCodeView::Result::Rsds){
            const auto* artifacts=parserRuntime_->PrivateCompilerArtifacts();
            for(SIZE_T i=0;i<parserRuntime_->CompilerArtifactCount();++i){
                const auto& artifact=artifacts[i];const auto& info=artifact.OriginalInfoHeader();
                if(artifact.FilesOriginal()!=&inputs_||artifact.FileOrdinal()!=i||
                   !artifact.RequirementOriginal()||artifact.Outcome()!=OriginalHeldMsfSource::Result::Geometry||
                   !info.read||!info.supported||info.age!=row.codeViewOriginal.age||
                   info.guid!=row.codeViewOriginal.guid)continue;
                ++row.compilerArtifactIdentifierMatches;
                row.compilerArtifactCandidateOrdinal=i;
            }
            if(row.compilerArtifactIdentifierMatches!=1)row.compilerArtifactCandidateOrdinal=SIZE_MAX;
            if(row.compilerArtifactCandidateOrdinal!=SIZE_MAX){
                // The loaded own module can be CoreCLR or another selected
                // dependency, independently of the caller/dispatcher chain.
                // This inline owner never adds that module to instruction J.
                if(row.ownModuleGeometryOriginal.Derive(inputs_,row.selectedFile))
                    row.compilerRangesOriginal.Observe(artifacts[row.compilerArtifactCandidateOrdinal],
                        row.ownModuleGeometryOriginal,row.loaded);
                row.compilerTlsCandidatesOriginal.Observe(row.compilerRangesOriginal,row.tlsOriginal);
                row.compilerTlsInitializersOriginal.Observe(row.compilerRangesOriginal);
            }
        }
        row.associated = true;
        // Retain the projection for this exact captured image. A matching name
        // alone does not select CoreCLR or grant any thread continuation effect.
        row.coreClrInitialize.Observe(row.loaded);
        row.coreClrErrorWriter.Observe(row.loaded,OriginalCoreClrInitializeExport::Symbol::ErrorWriter);
        if(parserRuntime_ && !parserRuntime_->ObserveAsset(state,saved,memberOrdinal,row.selectedFile,row.actualFile,row.loaded))
            return Fail("parser_runtime_actual_asset_occurrence_join_failed");
        if(event.dwDebugEventCode==CREATE_PROCESS_DEBUG_EVENT && fixtureRootOriginal_) {
            for(SIZE_T i=0;i<commandCount_;++i)if(commandLines_[i].stateOriginal==&state&&commandLines_[i].eventOriginal==&saved) {
                helperProjections_[i].Observe(commandLines_[i],crtDerivedScratch_.data(),crtDerivedScratch_.size());
                bool test=false;SIZE_T fixtureUnits=0;
                if(OriginalHelperCodeGuard::Select(*fixtureRootOriginal_,helperProjections_[i],test,fixtureUnits)) {
                    const SIZE_T kind=test?0:1;
                    if(canonicalHelperSourceCount_!=3 || helperCodeGuardCounts_[kind]==6)
                        return Fail("copied_helper_consumer_capacity_or_source_unavailable");
                    const SIZE_T ordinal=kind*6+helperCodeGuardCounts_[kind]++;
                    if(!helperCodeGuards_[ordinal] || !helperCodeGuards_[ordinal]->Capture(state,saved,commandLines_[i],
                        helperProjections_[i],*fixtureRootOriginal_,inputs_,canonicalHelperSources_[kind].canonicalSourceOrdinal,
                        canonicalHelperSources_[2].canonicalSourceOrdinal))return Fail("copied_helper_code_capture_failed");
                    if(canonicalParserSourceCount_!=2 || parserInputCount_>parserInputs_.size()-2)
                        return Fail("copied_parser_input_capacity_or_source_unavailable");
                    for(SIZE_T role=0;role<2;++role) {
                        auto& parser=parserInputs_[parserInputCount_++];
                        if(!parser || !parser->Capture(state,saved,commandLines_[i],helperProjections_[i],
                            *fixtureRootOriginal_,inputs_,canonicalParserSources_[role].canonicalSourceOrdinal,
                            role?OriginalParserInputCapture::Role::Harness:OriginalParserInputCapture::Role::Service))
                            return Fail("copied_parser_input_capture_failed");
                    }
                }
                break; // Projection alone is not a PowerShell tool/image grant.
            }
        }
        if(event.dwDebugEventCode==CREATE_PROCESS_DEBUG_EVENT && fixtureRootOriginal_ &&
            originalLaunchFile_<inputs_.FileCount() && SameFile(row.actualFile,selected[originalLaunchFile_].first)) {
            // Exact saved CREATE already owns the command observation. Only
            // the SAME actually observed Node image supports this projection;
            // PowerShell/cmd retain their own unprojected native command body.
            for(SIZE_T i=0;i<commandCount_;++i) {
                auto& command=commandLines_[i];
                if(command.stateOriginal==&state && command.eventOriginal==&saved) {
                    command.nodeImageAssociated=true;command.nodeImageSelectedFile=row.selectedFile;
                    ProjectTemplateNodeSourceForm(command,crtDerivedScratch_.data(),crtDerivedScratch_.size());
                    SIZE_T kind=0,fixtureUnits=0;bool encoded=false;
                    if(OriginalAliasCodeGuard::Select(*fixtureRootOriginal_,command,kind,fixtureUnits,encoded)) {
                        if(canonicalAliasSourceCount_!=2 || aliasCodeGuardCounts_[kind]==8)
                            return Fail("copied_alias_consumer_capacity_or_source_unavailable");
                        const SIZE_T ordinal=kind*8+aliasCodeGuardCounts_[kind]++;
                        if(!aliasCodeGuards_[ordinal] || !aliasCodeGuards_[ordinal]->CaptureSelected(state,saved,command,
                            *fixtureRootOriginal_,inputs_,canonicalAliasSources_[kind].canonicalSourceOrdinal))
                            return Fail("copied_alias_consumer_capture_failed");
                    }
                    break;
                }
            }
        }
        return true;
    }
    bool ObserveImageUnload(const State& state,const Event& saved,SIZE_T member) noexcept {
        if(saved.original.dwDebugEventCode!=UNLOAD_DLL_DEBUG_EVENT || member>=state.memberCount ||
            !state.eventCount || &saved!=&state.events[state.eventCount-1])
            return Fail("unload_not_exact_saved_member_occurrence");
        GateImageObservation* found=nullptr;
        for(SIZE_T i=0;i<imageCount_;++i) {
            auto& image=*images_[i];
            if(!image.associated || image.stateOriginal!=&state || image.memberOrdinal!=member ||
                image.pid!=saved.original.dwProcessId ||
                !same_time(image.processBirth,state.members[member].birth) ||
                image.loaded.nativeBase!=saved.original.u.UnloadDll.lpBaseOfDll)continue;
            if(image.unloadOriginal==&saved)return true; // Same retained failure revisit.
            if(image.unloadOriginal)continue; // An earlier native image lifetime.
            if(found)return Fail("unload_current_image_occurrence_ambiguous");
            found=&image;
        }
        if(!found)return Fail("unload_current_image_occurrence_missing");
        found->unloadOriginal=&saved;
        bool chainInvalidated=false;
        if(coreClrCustody_)for(SIZE_T i=0;i<coreClrCustody_->PrivateThreadCount();++i){
            const auto& thread=coreClrCustody_->PrivateThreads()[i];
            if(thread.stateOriginal==&state&&thread.installationImageOriginal==&found->loaded&&
               thread.installationImageEventOriginal==found->loadOriginal&&thread.installationOriginal&&
               thread.installationOriginal->setAttempted&&!thread.NativeTerminalContinued()&&
               (!thread.disarmOriginal||!thread.disarmOriginal->effectiveVerified))chainInvalidated=true;
        }
        for(SIZE_T i=0;i<coreClrInvocationCount_;++i) {
            auto& invocation=*coreClrInvocations_[i];
            if(invocation.stateOriginal!=&state || !invocation.threadCreateOriginal ||
                invocation.threadCreateOriginal->original.dwProcessId!=saved.original.dwProcessId ||
                invocation.phase==GateCoreClrInvocation::Phase::Unused ||
                invocation.phase==GateCoreClrInvocation::Phase::Failed)continue;
            // The retained chain's code/profile observations cover these SAME
            // selected occurrences. Unload ends their currentness; retained
            // bytes and a later load at the same address cannot renew it.
            if(found->selectedFile==hostpolicyCandidateOrdinal_ || found->selectedFile==originalNtdllFile_ ||
                found->coreClrInitialize.result==OriginalCoreClrInitializeExport::Result::Direct) {
                invocation.phase=GateCoreClrInvocation::Phase::Failed;
                if(!invocation.firstFailureOriginal)invocation.firstFailureOriginal=&saved;
                chainInvalidated=true;
            }
        }
        // This records lifetime loss only. It neither clears a hardware slot
        // nor settles a slot borrower: actual disarm/continued native terminal
        // must independently settle the original custody rows.
        return !chainInvalidated || Fail("coreclr_chain_image_lifetime_ended");
    }
    bool ObserveOwnedEvent(const DEBUG_EVENT& event, const State& state, bool retainedFailure=false) noexcept {
        if(!BindLifecycleCommandOriginal(state))return Fail("controller_lifecycle_command_original_unbound");
        if(!state.eventCount || &event!=&state.events[state.eventCount-1].original)
            return Fail("controller_not_exact_saved_event");
        if(!coreClrCustody_ || !coreClrCustody_->ObserveSavedEvent(state,state.events[state.eventCount-1])) {
            if(!retainedFailure)return Fail("coreclr_controller_saved_thread_custody_unresolved");
            // Qualification remains failed. Retained original failure cleanup
            // may continue this exact event without granting a slot operation.
        }
        SIZE_T member = MemberLimit;
        if (event.dwDebugEventCode == CREATE_PROCESS_DEBUG_EVENT) {
            if (!state.eventCount || &event != &state.events[state.eventCount-1].original)
                return Fail("create_not_exact_saved_occurrence");
            const Event& saved = state.events[state.eventCount-1];
            if (saved.createdMemberOrdinal >= state.memberCount ||
                saved.createdProcessResource >= state.resourceCount)
                return Fail("create_retained_member_association_missing");
            member = saved.createdMemberOrdinal;
            if (state.members[member].process != saved.createdProcessResource)
                return Fail("create_retained_process_association_changed");
            // A revisited saved failure event reuses its exact original observation;
            // it must not acquire a new module/read owner or replace its error.
            for (SIZE_T i=0; i<commandCount_; ++i) {
                const auto& prior = commandLines_[i];
                if (prior.eventOriginal == &saved && prior.stateOriginal == &state)
                    return Fail("saved_create_gate_already_attempted_without_continuation");
            }
            if (commandCount_ == commandCapacity_) return Fail("command_observation_capacity_before_call");
            auto& command = commandLines_[commandCount_++];
            const bool observed = command.Capture(saved,state,pendingCallsOriginal_,command_);
            // Both calls have completed: independently eligible module references
            // are closed even when the original observation failed.
            const bool disposed = command.DisposeIndependentModules();
            if (!observed || !disposed) return Fail("original_command_line_observation_or_disposal_failed");
            command.crtProjectionAttempted = true;
            OriginalCrtArgumentCursor projection(command);
            OriginalCrtWord projectedWord;
            while(projection.Next(crtDerivedScratch_.data(),crtDerivedScratch_.size(),projectedWord)) {}
            command.crtProjectionComplete = projection.Valid() && projection.Finished() && !projection.Failed();
            command.crtProjectionFailed = !command.crtProjectionComplete;
            command.crtProjectedWordCount = projection.WordCount();
            command.crtProjectionSourceOffset = projection.SourceOffset();
            command.crtProjectionSourceUnits = projection.SourceUnits();
            // Pure CRT projection is not a claim that this particular consumer
            // uses that grammar. Its authentic Node/CRT or other-tool join is
            // separate; no projection result grants or denies continuation.
            if (event.dwProcessId == state.root) {
                command.originalRootCompared = true;
                command.originalRootMatches = command.retainedCommandExtent == command_.commandUnits*sizeof(wchar_t) &&
                    memcmp(command.commandBytes.data(),command_.commandLine.data(),command.retainedCommandExtent) == 0;
                // Retain comparison only: authentic selected startup semantics
                // must establish whether the remote representation is equivalent.
            }
        } else if (event.dwDebugEventCode == EXIT_PROCESS_DEBUG_EVENT) {
            if (!state.eventCount || &event != &state.events[state.eventCount-1].original)
                return Fail("exit_not_exact_saved_occurrence");
            const Event& saved = state.events[state.eventCount-1];
            member = saved.exitingMemberOrdinal;
            if (member >= state.memberCount || saved.exitingProcessResource >= state.resourceCount ||
                state.members[member].process != saved.exitingProcessResource ||
                state.members[member].pid != event.dwProcessId || !state.members[member].exitEvent)
                return Fail("exit_retained_member_association_missing");
            const SIZE_T createOrdinal = state.members[member].originalCreateEvent;
            if (createOrdinal >= state.eventCount-1 ||
                state.events[createOrdinal].original.dwDebugEventCode != CREATE_PROCESS_DEBUG_EVENT ||
                state.events[createOrdinal].createdMemberOrdinal != member ||
                state.events[createOrdinal].createdProcessResource != saved.exitingProcessResource)
                return Fail("exit_original_create_occurrence_missing");
            const Resource& process = state.resources[saved.exitingProcessResource];
            if (process.kind != Kind::Process || !process.acquired || process.unknownOutput ||
                process.attempted || !positive(state.members[member].birth))
                return Fail("exit_retained_process_original_unavailable");
            // Distinct per-consumer CODE holds may now settle. Other processes,
            // kernel shutdown, DATA and EOF keep their separate native owners.
            bool guards=true;
            for(SIZE_T kind=0;kind<2;++kind)for(SIZE_T i=0;i<aliasCodeGuardCounts_[kind];++i) {
                auto& guard=*aliasCodeGuards_[kind*8+i];
                if(guard.BelongsTo(state,state.events[createOrdinal]) && !guard.ObserveLastThreadExit(state,saved))guards=false;
            }
            if(!guards)Fail("copied_alias_code_disposal_failed");
            // Source-owned contract result joins this exact consumer boundary.
            // A pending/unsupported result never grants parser input release;
            // independently eligible full native cleanup remains available.
            const OriginalParserRuntimeAssociation::ConsumerContractResult* parserContract=nullptr;
            if(parserRuntime_){
                bool parserConsumer=false;
                for(SIZE_T i=0;i<parserInputCount_;++i)
                    if(parserInputs_[i]->BelongsTo(state,state.events[createOrdinal]))parserConsumer=true;
                if(parserConsumer){
                    const OriginalHelperSourceProjection* helperOriginal=nullptr;
                    for(SIZE_T i=0;i<commandCount_;++i)
                        if(commandLines_[i].stateOriginal==&state&&commandLines_[i].eventOriginal==&state.events[createOrdinal]){
                            helperOriginal=&helperProjections_[i];break;
                        }
                    const OriginalCoreClrEntryProperties* initializerOriginal=nullptr;
                    bool initializerAmbiguous=false;
                    for(SIZE_T i=0;i<coreClrInvocationCount_;++i) {
                        const auto& invocation=*coreClrInvocations_[i];
                        if(invocation.stateOriginal!=&state || invocation.phase!=GateCoreClrInvocation::Phase::EntryCaptured ||
                            !invocation.threadCreateOriginal ||
                            invocation.threadCreateOriginal->createdThreadMemberOrdinal!=member ||
                            invocation.threadCreateOriginal->createdThreadProcessResource!=saved.exitingProcessResource)continue;
                        if(initializerOriginal){initializerAmbiguous=true;break;}
                        initializerOriginal=&invocation.propertiesOriginal;
                    }
                    if(initializerAmbiguous){initializerOriginal=nullptr;Fail("parser_runtime_initializer_ambiguous");}
                    if(helperOriginal&&IsUnchangedSuiteCommand())
                        parserContract=parserRuntime_->ObserveConsumerContract(state,state.events[createOrdinal],saved,
                            *helperOriginal,initializerOriginal);
                    if(!parserContract)Fail("parser_runtime_consumer_result_retention_failed");
                }
            }
            for(SIZE_T kind=0;kind<2;++kind)for(SIZE_T i=0;i<helperCodeGuardCounts_[kind];++i) {
                auto& guard=*helperCodeGuards_[kind*6+i];
                if(guard.BelongsTo(state,state.events[createOrdinal])&&!guard.ObserveLastThreadExit(state,saved))guards=false;
            }
            for(SIZE_T i=0;i<parserInputCount_;++i) {
                auto& parser=*parserInputs_[i];
                if(parser.BelongsTo(state,state.events[createOrdinal]) && !parser.ObserveLastThreadExit(state,saved)) {
                    guards=false;Fail("parser_input_last_thread_association_failed");
                }
                if(parser.BelongsTo(state,state.events[createOrdinal])&&parserContract&&
                    parserRuntime_->AdmitsSynchronousRelease(*parserContract,state,state.events[createOrdinal],saved)&&
                    !parser.DisposeAfterSupportedSynchronousContract(*parserRuntime_,*parserContract)){
                    guards=false;Fail("parser_input_synchronous_disposal_failed");
                }
            }
            if(!guards){
                Fail("copied_code_disposal_failed");
                // Last-thread EXIT admits only the already-required kernel
                // shutdown continuation. Failed close qualification remains
                // absorbing; every independent guard above was still visited.
                return true;
            }
        } else if (!OwnedMember(event.dwProcessId, state, member)) return false;
        if (event.dwDebugEventCode == CREATE_PROCESS_DEBUG_EVENT || event.dwDebugEventCode == LOAD_DLL_DEBUG_EVENT)
            return ObserveImage(event, state, member);
        if(event.dwDebugEventCode==UNLOAD_DLL_DEBUG_EVENT)
            return ObserveImageUnload(state,state.events[state.eventCount-1],member);
        return true;
    }
public:
    OriginalWindowsGate(OriginalPrivateIngress& ingress, NativeHeldFiles& inputs,
                        template_creator_bridge::WindowsOriginalCommand& command,
                        SIZE_T selectedCallerRows, const GateImageGeometry* selectedImageGeometry,
                        SIZE_T selectedImageRows, SIZE_T selectedOriginalNtdllFile,
                        template_creator_bridge::OriginalWindowsPendingCalls& pendingCalls,
                        const void* commandRouteOriginal,
                        const OriginalWindowsFixtureRoot* fixtureRoot=nullptr)
        : ingress_(ingress), inputs_(inputs), command_(command), pendingCallsOriginal_(pendingCalls),
          commandRouteOriginal_(commandRouteOriginal),
          originalNtdllFile_(selectedOriginalNtdllFile),
          fixtureRootOriginal_(fixtureRoot),selectedCallerRowsOriginal_(selectedCallerRows),
          selectedImageRowsOriginal_(selectedImageRows),selectedImageGeometryOriginal_(selectedImageGeometry) {}
    bool PreownOriginal() noexcept {
        if(preownAttempted_)return false;preownAttempted_=true;
        struct Returned {bool& value;~Returned(){value=true;}} returned{preownReturned_};
        try {
        const SIZE_T selectedCallerRows=selectedCallerRowsOriginal_,selectedImageRows=selectedImageRowsOriginal_;
        const auto* selectedImageGeometry=selectedImageGeometryOriginal_;
        if (!commandRouteOriginal_ || !selectedCallerRows || !selectedImageRows || !selectedImageGeometry ||
            selectedCallerRows > SIZE_MAX/sizeof(GateCallerObservation) ||
            selectedImageRows > SIZE_MAX/sizeof(std::unique_ptr<GateImageObservation>))
            throw std::invalid_argument("complete original Windows gate capacities");
        // Same prelaunch source owner, constructed before instruction backing so
        // a complete genuine compiled graph can derive its charge from held metadata.
        // Missing inputs remain recorded UNKNOWN support, not a new launch gate.
        parserRuntime_=std::make_unique<OriginalParserRuntimeAssociation>(inputs_,command_.snapshotOriginal,selectedImageRows,command_);
        if(!parserRuntime_->PreownOriginal()){
            preownExceptionOriginal_=parserRuntime_->PreownExceptionOriginal();
            return Fail("original_parser_runtime_preown_failed");
        }
        parserRuntime_->ObserveAvailableSupportedSourceCase();
        parserRuntime_->ObserveAvailableHostConfiguration();
        parserRuntime_->ObserveAvailableVendorPackage();
        parserRuntime_->CompareAvailableVendorAssets();
        parserRuntime_->BindAvailableSourceRelationship();
        callers_ = std::make_unique<GateCallerObservation[]>(selectedCallerRows);
        callerCapacity_ = selectedCallerRows;
        images_ = std::make_unique<std::unique_ptr<GateImageObservation>[]>(selectedImageRows);
        imageCapacity_ = selectedImageRows;
        for (SIZE_T i=0; i<selectedImageRows; ++i){
            images_[i] = std::make_unique<GateImageObservation>(selectedImageGeometry[i]);
            if(!images_[i]->loaded.PreownOriginal()){
                preownExceptionOriginal_=images_[i]->loaded.preownExceptionOriginal;
                return Fail("original_loaded_image_preown_failed");
            }
        }
        const auto& observer=pendingCallsOriginal_.OriginalObserverGeometry();
        if(!observer.Complete() || observer.threads>ResourceLimit ||
            observer.invocations>SIZE_MAX/sizeof(std::unique_ptr<GateCoreClrInvocation>))
            throw std::invalid_argument("complete original observer gate geometry");
        coreClrCustody_=std::make_unique<OriginalCoreClrControllerCustody>(observer.threads,observer.readRows);
        if(!coreClrCustody_->PreownOriginal()){
            preownExceptionOriginal_=coreClrCustody_->preownExceptionOriginal;
            return Fail("original_controller_custody_preown_failed");
        }
        if(!DeriveCompiledInstructionBacking() || !DeriveContextChangeBacking(observer))return false;
        const SIZE_T invocationBytes=GateCoreClrInvocation::CheckedSourceBytes(observer,coreClrInstructionCapacity_);
        if(invocationBytes>SIZE_MAX-sizeof(std::unique_ptr<GateCoreClrInvocation>) ||
            observer.invocations>SIZE_MAX/(invocationBytes+sizeof(std::unique_ptr<GateCoreClrInvocation>)))
            throw std::invalid_argument("whole invocation instruction backing width");
        coreClrInvocations_=std::make_unique<std::unique_ptr<GateCoreClrInvocation>[]>(observer.invocations);
        coreClrInvocationCapacity_=observer.invocations;
        for(SIZE_T i=0;i<observer.invocations;++i){
            coreClrInvocations_[i]=std::make_unique<GateCoreClrInvocation>();
            if(!coreClrInvocations_[i]->PreownOriginal(observer,coreClrInstructionCapacity_)){
                preownExceptionOriginal_=coreClrInvocations_[i]->preownExceptionOriginal;
                return Fail("original_invocation_preown_failed");
            }
        }
        // Each CREATE consumes an image row and one member. Derive this separate
        // command-body store from those existing admitted capacities.
        commandCapacity_ = selectedImageRows < MemberLimit ? selectedImageRows : MemberLimit;
        commandLines_ = std::make_unique<OriginalCommandLineObservation[]>(commandCapacity_);
        helperProjections_=std::make_unique<OriginalHelperSourceProjection[]>(commandCapacity_);
        // No new open, normalization, case folding or path reconstruction: bind
        // the immutable launch literal to the literal used by actual Capture.
        // This is a source association, not proof of later named-path continuity.
        const auto* files = inputs_.PrivateFiles();
        for (SIZE_T i=0; i<inputs_.FileCount(); ++i) {
            if (files[i].valid && !files[i].closeAttempted &&
                files[i].originalPathChars == command_.imageUnits+1 &&
                memcmp(files[i].originalPath.data(), command_.image.data(),
                    (command_.imageUnits+1)*sizeof(wchar_t)) == 0) {
                originalLaunchFile_ = i; break;
            }
        }
        if (originalLaunchFile_ == SIZE_MAX) Fail("original_launch_literal_not_captured_native_input");
        if(!AssociateFixtureEnvironment())Fail("protected_environment_differs_from_original_private_root");
        if(!failed_ && fixtureRootOriginal_ && IsUnchangedSuiteCommand()) {
            for(SIZE_T i=0;i<canonicalAliasSources_.size();++i) {
                auto& source=canonicalAliasSources_[canonicalAliasSourceCount_++];
                const bool captured=source.CaptureCanonicalSource(ingress_,*fixtureRootOriginal_,inputs_,
                    i?OriginalFixtureRelativeFile::CanonicalScript::Publisher:OriginalFixtureRelativeFile::CanonicalScript::Workflow);
                // Only metadata used this query handle; the actual source byte
                // owner stays held. Every acquired query gets its own close.
                const bool disposed=source.DisposeIndependentlyEligible();
                if(!captured || !disposed){Fail("canonical_alias_producer_source_not_captured_or_disposed");break;}
            }
            if(!failed_)for(SIZE_T kind=0;kind<2;++kind) {
                const unsigned char* bytes=nullptr;SIZE_T extent=0;
                if(!inputs_.ExactCapturedSourceSpan(canonicalAliasSources_[kind].canonicalSourceOrdinal,bytes,extent) ||
                    extent>=NativeHeldFiles::ReadLimit){Fail("copied_alias_read_roster_geometry_unavailable");break;}
                // Every positive short read consumes at least one byte; reserve
                // all such returns plus the independent EOF/growth probe.
                for(SIZE_T i=0;i<8;++i)aliasCodeGuards_[kind*8+i]=
                    std::make_unique<OriginalAliasCodeGuard>(extent+1,extent+1);
            }
            if(!failed_)for(SIZE_T i=0;i<canonicalHelperSources_.size();++i) {
                auto& source=canonicalHelperSources_[canonicalHelperSourceCount_++];
                using Script=OriginalFixtureRelativeFile::CanonicalScript;
                const bool captured=source.CaptureCanonicalSource(ingress_,*fixtureRootOriginal_,inputs_,
                    i==0?Script::TestHelper:i==1?Script::VerifyHelper:Script::EchoRuntime);
                const bool disposed=source.DisposeIndependentlyEligible();
                if(!captured||!disposed){Fail("canonical_helper_source_not_captured_or_disposed");break;}
            }
            if(!failed_) {
                std::array<SIZE_T,3> extents{};
                for(SIZE_T i=0;i<3;++i) {
                    const unsigned char* bytes=nullptr;
                    if(!inputs_.ExactCapturedSourceSpan(canonicalHelperSources_[i].canonicalSourceOrdinal,bytes,extents[i]) ||
                        extents[i]>=NativeHeldFiles::ReadLimit){Fail("copied_helper_read_roster_geometry_unavailable");break;}
                }
                if(!failed_)for(SIZE_T kind=0;kind<2;++kind)for(SIZE_T i=0;i<6;++i)
                    helperCodeGuards_[kind*6+i]=std::make_unique<OriginalHelperCodeGuard>(extents[kind]+1,extents[kind]+1,
                        extents[2]+1,extents[2]+1,kind==0);
            }
            if(!failed_)for(SIZE_T role=0;role<2;++role) {
                auto& source=canonicalParserSources_[canonicalParserSourceCount_++];
                using Script=OriginalFixtureRelativeFile::CanonicalScript;
                const bool captured=source.CaptureCanonicalSource(ingress_,*fixtureRootOriginal_,inputs_,
                    role?Script::HarnessDocument:Script::ServiceDocument);
                const bool disposed=source.DisposeIndependentlyEligible();
                const unsigned char* bytes=nullptr;SIZE_T extent=0;
                if(!captured || !disposed || !inputs_.ExactCapturedSourceSpan(source.canonicalSourceOrdinal,bytes,extent) ||
                    extent>=NativeHeldFiles::ReadLimit) {Fail("canonical_parser_source_or_geometry_unavailable");break;}
                for(SIZE_T occurrence=0;occurrence<12;++occurrence)
                    parserInputs_[occurrence*2+role]=std::make_unique<OriginalParserInputCapture>(extent+1,extent+1);
            }
        }
        preowned_=!failed_;return preowned_;
        }catch(...){preownExceptionOriginal_=std::current_exception();return Fail("original_gate_preown_exception");}
    }
    const std::exception_ptr& PreownExceptionOriginal() const noexcept{return preownExceptionOriginal_;}
    OriginalWindowsGate(const OriginalWindowsGate&) = delete;
    OriginalWindowsGate& operator=(const OriginalWindowsGate&) = delete;
    bool BeforeCreate(const State& state, const wchar_t* image, const wchar_t* line, const wchar_t* cwd,
                      const wchar_t* environment, SIZE_T environmentChars) override {
        static_assert(sizeof(wchar_t) == sizeof(char16_t));
        if (!preowned_ || failed_ || !command_.copied || command_.shapeFailure || inputs_.Failed() ||
            !inputs_.FileCount() || originalNtdllFile_ >= inputs_.FileCount() ||
            originalLaunchFile_ >= inputs_.FileCount()) return Fail("original_native_command_inputs_unbound");
        if (!image || !line || !cwd || !environment || environmentChars != command_.environmentUnits ||
            memcmp(image, command_.image.data(), (command_.imageUnits+1)*sizeof(wchar_t)) ||
            memcmp(line, command_.commandLine.data(), (command_.commandUnits+1)*sizeof(wchar_t)) ||
            memcmp(cwd, command_.cwd.data(), (command_.cwdUnits+1)*sizeof(wchar_t)) ||
            memcmp(environment, command_.environment.data(), environmentChars*sizeof(wchar_t)))
            return Fail("native_launch_differs_from_retained_command_copy");
        if(!LiveFixtureEnvironment() || !LiveInputs() || !LiveOriginalCaller())return false;
        if(!pendingCallsOriginal_.OriginalNoticeWorkerBodiesMatch(&ingress_,commandRouteOriginal_))
            return Fail("original_product_notice_worker_bodies_or_native_birth_unbound");
        noticeBodiesOriginal_=pendingCallsOriginal_.OriginalNoticeOwner();
        nativeImageOriginal_=image;nativeLineOriginal_=line;nativeCwdOriginal_=cwd;nativeEnvironmentOriginal_=environment;
        return BindLifecycleCommandOriginal(state);
    }
    bool BeforeContinue(const DEBUG_EVENT& event, const State& state) override {
        if (failed_ || inputs_.Failed() || state.stopped || lifecycleStateOriginal_!=&state || !SameLifecycleCommandOriginal(state)) return false;
        return LiveFixtureEnvironment() && LiveInputs() && LiveOriginalCaller() && ObserveOwnedEvent(event, state) &&
            InstallOriginalCoreClrPairs(state,state.events[state.eventCount-1]);
    }
    bool BeforeFailureContinue(const DEBUG_EVENT& event, const State& state) override {
        // Qualification remains failed. Only the same already-owned native event
        // may continue; no next-command or new ingress admission is granted.
        return lifecycleStateOriginal_==&state && SameLifecycleCommandOriginal(state) &&
            LiveInputs() && LiveOriginalCaller() && FailurePairContinuationOriginal(state,event) &&
            ObserveOwnedEvent(event, state, true);
    }
    SavedEventDecision DecideSavedEvent(const Event& saved,const State& state,bool failed) override {
        if(!failed && !failed_) {
            const auto& event=saved.original;
            if(event.dwDebugEventCode!=EXCEPTION_DEBUG_EVENT || !event.u.Exception.dwFirstChance ||
                event.u.Exception.ExceptionRecord.ExceptionCode!=EXCEPTION_SINGLE_STEP)
                return PreserveForeignEventOriginal(saved,state);
            if(lifecycleStateOriginal_!=&state || !SameLifecycleCommandOriginal(state) ||
                !state.eventCount || &saved!=&state.events[state.eventCount-1] || saved.continued)
                return SavedEventDecision::Deny;
            const OriginalCoreClrThreadCustody* owner=nullptr;
            const auto address=reinterpret_cast<std::uintptr_t>(event.u.Exception.ExceptionRecord.ExceptionAddress);
            if(coreClrCustody_)for(SIZE_T i=0;i<coreClrCustody_->PrivateThreadCount();++i) {
                const auto& row=coreClrCustody_->PrivateThreads()[i];
                if(row.stateOriginal!=&state || !row.createOriginal || !row.currentPairOriginal ||
                    !OriginalStoppedThreadOccurrenceFor(state,saved,*row.createOriginal) ||
                    (row.disarmOriginal && row.disarmOriginal->effectiveVerified))continue;
                const auto* pair=row.currentPairOriginal;
                if(address!=pair->address && (!pair->movableEnabled || address!=pair->movableAddress))continue;
                if(owner)return SavedEventDecision::Deny;owner=&row;
            }
            if(!owner)return PreserveForeignEventOriginal(saved,state);
            if(!BeforeContinue(event,state))return SavedEventDecision::Deny;
            const auto* pair=coreClrCustody_->CurrentPairOriginal(state,*owner->createOriginal);
            if(!pair || pair!=owner->currentPairOriginal || !pair->effectiveVerified)
                return SavedEventDecision::Deny;
            const auto* image=UniquePairImage(state,saved,*pair);
            if(!image)return SavedEventDecision::Deny;
            GateCoreClrInvocation* active=nullptr;
            for(SIZE_T i=0;i<coreClrInvocationCount_;++i) {
                auto& invocation=*coreClrInvocations_[i];
                if(invocation.stateOriginal!=&state || invocation.threadCreateOriginal!=owner->createOriginal ||
                    (invocation.phase!=GateCoreClrInvocation::Phase::AwaitWriterReturn &&
                     invocation.phase!=GateCoreClrInvocation::Phase::Walking))continue;
                if(active)return SavedEventDecision::Deny;active=&invocation;
            }
            bool captured=false;
            if(address==pair->address) {
                // The permanent guard never admits an entry from bootstrap or
                // a return-slot candidate. Only the actual completed chain may
                // attempt the independent entry/context/argument capture.
                if(!active || active->phase!=GateCoreClrInvocation::Phase::Walking)
                    return Fail("initializer_entry_without_original_chain"),SavedEventDecision::Deny;
                captured=CapturePredictedEntry(state,saved,*active);
            } else if(address==reinterpret_cast<std::uintptr_t>(image->coreClrErrorWriter.address)) {
                captured=CaptureWriterCandidate(state,saved,*owner->createOriginal);
            } else if(active && address==active->predictedAddress) {
                captured=active->phase==GateCoreClrInvocation::Phase::AwaitWriterReturn?
                    CaptureWriterContinuation(state,saved,*active):CapturePredictedChainStep(state,saved,*active);
            } else return Fail("owned_stop_without_original_controller_transition"),SavedEventDecision::Deny;
            // This requests a disposition only. The separately dispatched
            // lifecycle validator remains default-closed until complete source,
            // CPU, compiled-currentness and own-actor authority is implemented.
            return captured?SavedEventDecision::ConsumeOwnedObserverException:SavedEventDecision::Deny;
        }
        if(lifecycleStateOriginal_!=&state || !SameLifecycleCommandOriginal(state) ||
            !state.eventCount || &saved!=&state.events[state.eventCount-1] || saved.continued)
            return SavedEventDecision::Deny;
        const auto& event=saved.original;
        if(event.dwDebugEventCode!=EXCEPTION_DEBUG_EVENT || !event.u.Exception.dwFirstChance ||
            event.u.Exception.ExceptionRecord.ExceptionCode!=EXCEPTION_SINGLE_STEP)
            return InputGate::DecideSavedEvent(saved,state,true);
        const OriginalCoreClrThreadCustody* candidate=nullptr;
        if(coreClrCustody_)for(SIZE_T i=0;i<coreClrCustody_->PrivateThreadCount();++i) {
            const auto& row=coreClrCustody_->PrivateThreads()[i];
            if(row.stateOriginal!=&state || !row.createOriginal || !row.currentPairOriginal ||
                !OriginalStoppedThreadOccurrenceFor(state,saved,*row.createOriginal))continue;
            if(row.disarmOriginal && row.disarmOriginal->effectiveVerified)continue;
            const auto* pair=row.currentPairOriginal;
            const auto address=reinterpret_cast<std::uintptr_t>(event.u.Exception.ExceptionRecord.ExceptionAddress);
            if(address!=pair->address && (!pair->movableEnabled || address!=pair->movableAddress))continue;
            if(candidate)return SavedEventDecision::Deny;candidate=&row;
        }
        if(!candidate)return InputGate::DecideSavedEvent(saved,state,true);
        // Candidate address merely chooses the retained cleanup attempt. Exact
        // cause/current generation and fresh completed restoration are checked
        // independently below and again by lifecycle's closed validator.
        if(!BeforeFailureContinue(event,state) || !coreClrCustody_->PrepareOwnedCleanupOriginal(
            state,saved,*candidate->createOriginal,pendingCallsOriginal_,command_))return SavedEventDecision::Deny;
        return SavedEventDecision::ConsumeOwnedObserverCleanupException;
    }
    bool ValidateOwnedObserverConsumption(const Event& saved,const State& state) override {
        if(failed_ || state.stopped || lifecycleStateOriginal_!=&state || !SameLifecycleCommandOriginal(state) ||
            !state.eventCount || &saved!=&state.events[state.eventCount-1] || saved.continued ||
            !coreClrCustody_ || !pendingCallsOriginal_.CommandNativeCallsSettled(&command_))return false;
        const OriginalCoreClrDebugCause* cause=nullptr;
        const OriginalCoreClrContextChange* movement=nullptr;
        bool ambiguous=false;
        const auto select=[&](const OriginalCoreClrDebugCause& candidate,
                const OriginalCoreClrContextChange* change) {
            if(candidate.stateOriginal!=&state || candidate.stoppedOriginal!=&saved)return;
            if(cause){ambiguous=true;return;}cause=&candidate;movement=change;
        };
        for(SIZE_T i=0;i<coreClrInvocationCount_;++i) {
            const auto& invocation=*coreClrInvocations_[i];
            if(invocation.stateOriginal!=&state)continue;
            if(invocation.entryCause.stoppedOriginal==&saved &&
                (!invocation.propertiesOriginal.representationObserved ||
                 !invocation.propertiesOriginal.hostContractOriginal.representationObserved ||
                 !invocation.propertiesOriginal.hostContractOriginal.first.context ||
                 !invocation.propertiesOriginal.hostContractOriginal.first.get_runtime_property ||
                 invocation.propertiesOriginal.hostContractOriginal.inputsOriginal!=invocation.inputs.get() ||
                 invocation.propertiesOriginal.inputsOriginal!=invocation.inputs.get()))return false;
            if(invocation.entryCause.stoppedOriginal==&saved)
                for(const auto& callback:invocation.hostCallbackRangesOriginal)
                    if(callback.result==OriginalHostContractCallbackRange::Result::Unobserved ||
                        callback.contractOriginal!=&invocation.propertiesOriginal.hostContractOriginal ||
                        callback.stoppedOriginal!=&saved ||
                        callback.imageOriginal!=UniqueCurrentImage(state,saved,hostpolicyCandidateOrdinal_))return false;
            select(invocation.writerBootstrap.cause,invocation.bootstrapMovementOriginal);
            select(invocation.postWriterObservation.cause,invocation.postWriterMovementOriginal);
            select(invocation.entryCause,invocation.entryMovementOriginal);
            for(SIZE_T j=0;j<invocation.instructionCount;++j)
                select(invocation.instructionOriginals[j].cause,invocation.instructionOriginals[j].movementOriginal);
        }
        if(ambiguous || !cause || !movement || !cause->ownedOnlyObserved || !cause->contextOriginal ||
            !cause->pairOriginal || !cause->contextOriginal->captured || !movement->effectiveVerified ||
            movement->kind!=OriginalCoreClrContextChange::Kind::MovePair ||
            movement->precedingPairOriginal!=cause->pairOriginal ||
            !coreClrCustody_->OwnsChangeOriginal(*cause->pairOriginal) ||
            !coreClrCustody_->OwnsChangeOriginal(*movement))return false;
        const auto* born=cause->contextOriginal->threadCreateOriginal;
        if(!born || !OriginalStoppedThreadOccurrenceFor(state,saved,*born) ||
            !NativeAmd64ProcessOriginal(state,saved,*born) ||
            coreClrCustody_->CurrentPairOriginal(state,*born)!=movement ||
            movement->before.stateOriginal!=&state || movement->before.stoppedOriginal!=&saved ||
            movement->before.threadCreateOriginal!=born || movement->after.stateOriginal!=&state ||
            movement->after.stoppedOriginal!=&saved || movement->after.threadCreateOriginal!=born ||
            !movement->setAttempted || !movement->setReturnedObserved || !movement->setReturned ||
            !movement->setPendingOriginal || movement->setPendingOriginal->commandOriginal!=&command_ ||
            !UniquePairImage(state,saved,*movement))return false;
#if defined(_M_AMD64) || defined(__x86_64__)
        if(!OriginalCoreClrContextChange::GeneralEqual(movement->before.effective,cause->contextOriginal->effective) ||
            !OriginalCoreClrContextChange::DebugEqual(movement->before.effective,cause->contextOriginal->effective) ||
            !OriginalCoreClrContextChange::GeneralEqual(movement->after.effective,movement->desired) ||
            !OriginalCoreClrContextChange::DebugEqual(movement->after.effective,movement->desired))return false;
#else
        return false;
#endif
        if(!parserRuntime_)return false;
        const auto& privateSource=parserRuntime_->PrivateCoreClrPropertySources();
        if(!privateSource.attempted || !privateSource.exactSourceObserved || privateSource.ambiguous ||
            privateSource.filesOriginal!=&inputs_)return false;
        for(SIZE_T source=0;source<privateSource.ordinals.size();++source)
            if(!privateSource.bodies[source] || !privateSource.extents[source] ||
                privateSource.ordinals[source]>=inputs_.FileCount())return false;
        // Concrete transition/source correspondence above is necessary, not sufficient.
        // The actual CPU/source/compiled-path and product-owned target actor
        // consumer is not yet implemented. No callback or physical outcome may
        // stand in for those independent original relationships.
        return false;
    }
    bool ValidateOwnedObserverCleanupConsumption(const Event& saved,const State& state) override {
        return (failed_ || state.stopped) && lifecycleStateOriginal_==&state && SameLifecycleCommandOriginal(state) &&
            coreClrCustody_ && coreClrCustody_->ValidateOwnedCleanupOriginal(state,saved,pendingCallsOriginal_,command_);
    }
    bool BeforeNativeDisposal(const State& state) override {
        // Same creating-thread serialization as pending Begin/Return. This
        // command is terminal; later route commands have distinct originals.
        const bool eligible=BindLifecycleCommandOriginal(state) && lifecycleStateOriginal_==&state && state.terminal &&
            pendingCallsOriginal_.CommandNativeCallsSettled(&command_) &&
            coreClrCustody_ && coreClrCustody_->SlotBorrowersSettled(state);
#if defined(TEMPLATE_NATIVE_CHAIN_REGRESSION)
        if(eligible&&!chainRegressionRecorded_){
          chainRegressionRecorded_=true;
          for(SIZE_T i=0;i<coreClrInvocationCount_;++i){
            const bool observed=ObserveOriginalChainRegression(*coreClrInvocations_[i],state);
            if(originalChainRegressionReceipt.observedInvocations==SIZE_MAX)originalChainRegressionReceipt.failed=true;
            else ++originalChainRegressionReceipt.observedInvocations;
            if(!observed)originalChainRegressionReceipt.failed=true;
          }
        }
#endif
        return eligible;
    }
    bool BeforeFailureDrain(const State& state) override {
        return lifecycleStateOriginal_==&state && SameLifecycleCommandOriginal(state) &&
            state.created && !state.disposed && LiveOriginalCaller();
    }
    bool IsInitialDebuggerBreakpoint(const DEBUG_EVENT& event, const State& state) override {
        if(lifecycleStateOriginal_!=&state || !SameLifecycleCommandOriginal(state))return false;
        if (event.dwDebugEventCode != EXCEPTION_DEBUG_EVENT ||
            event.u.Exception.ExceptionRecord.ExceptionCode != EXCEPTION_BREAKPOINT ||
            event.u.Exception.dwFirstChance != 1) return false;
        const SIZE_T member = MemberOrdinal(event.dwProcessId, state);
        if (member == MemberLimit || initialBreakpointConsumed_[member]) return false;
        for (SIZE_T i=0; i<imageCount_; ++i) {
            const auto& image = *images_[i];
            if (image.associated && image.pid == event.dwProcessId &&
                originalNtdllFile_ < inputs_.FileCount() &&
                SameFile(image.actualFile,inputs_.PrivateFiles()[originalNtdllFile_].first)) {
                const void* address = OriginalDbgBreakPoint(image.loaded);
                if (address && address == event.u.Exception.ExceptionRecord.ExceptionAddress) {
                    initialBreakpointConsumed_[member] = true; return true;
                }
            }
        }
        return false; // Never suppress an arbitrary application exception.
    }
    bool ReservedSourceBytes(SIZE_T& bytes) const noexcept {
        bytes = sizeof(*this);
        auto add = [&](SIZE_T amount) noexcept {
            if (amount > SIZE_MAX-bytes) return false;
            bytes += amount; return true;
        };
        if (!add(callerCapacity_*sizeof(GateCallerObservation)) ||
            !add(coreClrInvocationCapacity_*sizeof(std::unique_ptr<GateCoreClrInvocation>)) ||
            !add(imageCapacity_*sizeof(std::unique_ptr<GateImageObservation>)) ||
            !add(commandCapacity_*sizeof(OriginalCommandLineObservation)) ||
            !add(commandCapacity_*sizeof(OriginalHelperSourceProjection))) return false;
        for(SIZE_T i=0;i<coreClrInvocationCapacity_;++i)
            if(coreClrInvocations_[i]&&(!add(sizeof(GateCoreClrInvocation)) ||
                !add(coreClrInvocations_[i]->inputs?coreClrInvocations_[i]->inputs->ReservedSourceBytes():0) ||
                !add(coreClrInvocations_[i]->callerBoundary?coreClrInvocations_[i]->callerBoundary->ReservedSourceBytes():0) ||
                !add(coreClrInvocations_[i]->instructionCapacity*sizeof(GateCoreClrInstruction))))return false;
        for (SIZE_T i=0; i<imageCapacity_; ++i) {
            // sizeof(loaded) is already included in the enclosing row. The
            // native image observer's heap bytes/regions remain simultaneous.
            if (images_[i]&&(!add(sizeof(GateImageObservation)-sizeof(LoadedImageObservation)) ||
                !add(images_[i]->loaded.ReservedSourceBytes()))) return false;
        }
        for(const auto& guard:aliasCodeGuards_)if(guard&&!add(guard->ReservedSourceBytes()))return false;
        for(const auto& guard:helperCodeGuards_)if(guard&&!add(guard->ReservedSourceBytes()))return false;
        for(const auto& parser:parserInputs_)if(parser&&!add(parser->ReservedSourceBytes()))return false;
        if(parserRuntime_&&!add(parserRuntime_->ReservedSourceBytes()))return false;
        SIZE_T custodyBytes=0;
        if(!coreClrCustody_ || !coreClrCustody_->ReservedSourceBytes(custodyBytes) || !add(custodyBytes))return false;
        return true; // Source arithmetic only; NOT measured whole-runtime fit.
    }
    bool ObserverModulesPhysicallyDisposed() const noexcept {
        for(const auto& parser:parserInputs_)if(parser&&!parser->PhysicallyDisposed())return false;
        for(SIZE_T i=0;i<canonicalParserSourceCount_;++i)if(!canonicalParserSources_[i].PhysicallyDisposed())return false;
        for(const auto& guard:helperCodeGuards_)if(guard&&!guard->PhysicallyDisposed())return false;
        for(SIZE_T i=0;i<canonicalHelperSourceCount_;++i)if(!canonicalHelperSources_[i].PhysicallyDisposed())return false;
        for(const auto& guard:aliasCodeGuards_)if(guard&&!guard->PhysicallyDisposed())return false;
        for(SIZE_T i=0;i<canonicalAliasSourceCount_;++i)
            if(!canonicalAliasSources_[i].PhysicallyDisposed())return false;
        for (SIZE_T i=0; i<commandCount_; ++i)
            if (!commandLines_[i].ModulesPhysicallyDisposed()) return false;
        return true;
    }
    const OriginalCommandLineObservation& PrivateCommandLine(SIZE_T ordinal) const { return commandLines_[ordinal]; }
    SIZE_T CommandLineCount() const noexcept { return commandCount_; }
    const OriginalHelperSourceProjection& PrivateHelperProjection(SIZE_T ordinal) const noexcept{return helperProjections_[ordinal];}
    const GateCallerObservation* PrivateCallerRows() const noexcept { return callers_.get(); }
    SIZE_T CallerRowCount() const noexcept { return callerCount_; }
    const GateImageObservation& PrivateImage(SIZE_T ordinal) const { return *images_[ordinal]; }
    SIZE_T ImageRowCount() const noexcept { return imageCount_; }
    const OriginalCoreClrControllerCustody* PrivateCoreClrCustody() const noexcept{return coreClrCustody_.get();}
    bool CoreClrNativeOutputsSettledOriginal() const noexcept{return pendingCallsOriginal_.CommandNativeCallsSettled(&command_);}
    SIZE_T PrivateCoreClrInvocationCount() const noexcept{return coreClrInvocationCount_;}
    const GateCoreClrInvocation& PrivateCoreClrInvocation(SIZE_T i) const noexcept{return *coreClrInvocations_[i];}
    struct RetainedPreownView {
        const OriginalWindowsGate* gateOriginal=nullptr;
        const template_creator_bridge::WindowsOriginalCommand* commandOriginal=nullptr;
        const void* imageSlotsOriginal=nullptr;const void* invocationSlotsOriginal=nullptr;
        const std::exception_ptr* exceptionOriginal=nullptr;
        SIZE_T imageCapacity=0,imageObserved=0,invocationCapacity=0,invocationObserved=0;
        bool attempted=false,returned=false,preowned=false,failed=false;
    };
    bool OutputBackingSeparateOriginal(const void* output,std::size_t bytes) const noexcept {
        if(!OriginalRetainedStorageSeparate(output,bytes,this,1,sizeof(*this))||
           !OriginalRetainedStorageSeparate(output,bytes,&ingress_,1,sizeof(ingress_))||
           !OriginalRetainedStorageSeparate(output,bytes,&command_,1,sizeof(command_))||
           !inputs_.OutputBackingSeparateOriginal(output,bytes)||
           !pendingCallsOriginal_.OutputBackingSeparateOriginal(output,bytes)||
           !OriginalRetainedArraySeparate(output,bytes,callers_,callerCapacity_)||
           !OriginalRetainedArraySeparate(output,bytes,commandLines_,commandCapacity_)||
           !OriginalRetainedArraySeparate(output,bytes,helperProjections_,commandCapacity_)||
           !OriginalRetainedArraySeparate(output,bytes,images_,imageCapacity_)||
           !OriginalRetainedArraySeparate(output,bytes,coreClrInvocations_,coreClrInvocationCapacity_)||
           !OriginalRetainedOwnerSeparate(output,bytes,coreClrCustody_)||!OriginalRetainedOwnerSeparate(output,bytes,parserRuntime_))return false;
        if(images_)for(SIZE_T i=0;i<imageCapacity_;++i)if(images_[i]){
            const auto& image=*images_[i];
            if(!OriginalRetainedStorageSeparate(output,bytes,&image,1,sizeof(image))||
               !OriginalRetainedArraySeparate(output,bytes,image.loaded.bytes,image.loaded.byteCapacity)||
               !OriginalRetainedArraySeparate(output,bytes,image.loaded.regions,image.loaded.regionCapacity))return false;
        }
        if(coreClrInvocations_)for(SIZE_T i=0;i<coreClrInvocationCapacity_;++i)if(coreClrInvocations_[i]){
            const auto& invocation=*coreClrInvocations_[i];
            if(!OriginalRetainedStorageSeparate(output,bytes,&invocation,1,sizeof(invocation))||
               !OriginalRetainedArraySeparate(output,bytes,invocation.instructionOriginals,invocation.instructionCapacity)||
               !OriginalRetainedOwnerSeparate(output,bytes,invocation.inputs)||
               !OriginalRetainedOwnerSeparate(output,bytes,invocation.callerBoundary))return false;
        }
        for(const auto& owner:aliasCodeGuards_)if(!OriginalRetainedOwnerSeparate(output,bytes,owner))return false;
        for(const auto& owner:helperCodeGuards_)if(!OriginalRetainedOwnerSeparate(output,bytes,owner))return false;
        for(const auto& owner:parserInputs_)if(!OriginalRetainedOwnerSeparate(output,bytes,owner))return false;
        return true;
    }
    bool ObserveRetainedPreownViewOriginal(RetainedPreownView& out) const noexcept {
        if(!OutputBackingSeparateOriginal(&out,sizeof(out))||!preownAttempted_||!preownReturned_||!CoreClrNativeOutputsSettledOriginal()||
            imageCount_>imageCapacity_||coreClrInvocationCount_>coreClrInvocationCapacity_||
            (imageCapacity_&&!images_)||(coreClrInvocationCapacity_&&!coreClrInvocations_))return false;
        RetainedPreownView value{};value.gateOriginal=this;value.commandOriginal=&command_;
        value.imageSlotsOriginal=images_.get();value.invocationSlotsOriginal=coreClrInvocations_.get();
        value.exceptionOriginal=&preownExceptionOriginal_;value.imageCapacity=imageCapacity_;
        value.imageObserved=imageCount_;value.invocationCapacity=coreClrInvocationCapacity_;
        value.invocationObserved=coreClrInvocationCount_;value.attempted=preownAttempted_;
        value.returned=preownReturned_;value.preowned=preowned_;value.failed=failed_;out=value;return true;
    }
    const GateImageObservation* RetainedPreownImageOriginal(const RetainedPreownView& view,SIZE_T ordinal) const noexcept {
        if(!SameRetainedPreownViewOriginal(view)||ordinal>=imageCapacity_||!images_)return nullptr;
        return images_[ordinal].get();
    }
    const GateCoreClrInvocation* RetainedPreownInvocationOriginal(const RetainedPreownView& view,SIZE_T ordinal) const noexcept {
        if(!SameRetainedPreownViewOriginal(view)||ordinal>=coreClrInvocationCapacity_||!coreClrInvocations_)return nullptr;
        return coreClrInvocations_[ordinal].get();
    }
    bool SameRetainedPreownViewOriginal(const RetainedPreownView& view) const noexcept {
        RetainedPreownView actual{};if(!ObserveRetainedPreownViewOriginal(actual))return false;
        return view.gateOriginal==actual.gateOriginal&&view.commandOriginal==actual.commandOriginal&&
            view.imageSlotsOriginal==actual.imageSlotsOriginal&&view.invocationSlotsOriginal==actual.invocationSlotsOriginal&&
            view.exceptionOriginal==actual.exceptionOriginal&&view.imageCapacity==actual.imageCapacity&&
            view.imageObserved==actual.imageObserved&&view.invocationCapacity==actual.invocationCapacity&&
            view.invocationObserved==actual.invocationObserved&&view.attempted==actual.attempted&&
            view.returned==actual.returned&&view.preowned==actual.preowned&&view.failed==actual.failed;
    }
    bool Failed() const noexcept { return failed_; }
    bool InitialSourceAssociations(const OriginalPrivateIngress& ingress,const NativeHeldFiles& inputs,
        const template_creator_bridge::WindowsOriginalCommand& command,
        const OriginalWindowsFixtureRoot* root) const noexcept {
        if(&ingress!=&ingress_ || &inputs!=&inputs_ || &command!=&command_ ||
            root!=fixtureRootOriginal_ || failed_ || !command_.copied || command_.shapeFailure ||
            inputs_.Failed() || originalLaunchFile_>=inputs_.FileCount() ||
            originalNtdllFile_>=inputs_.FileCount())return false;
        if(root && (!fixtureEnvironmentAssociated_ || !root->created || !root->environmentPathBound ||
            root->rootUnknown || root->rootCloseAttempted || root->dispositionAttempted))return false;
        const auto* files=inputs_.PrivateFiles();
        for(SIZE_T i=0;i<inputs_.FileCount();++i)
            if(!files[i].valid || !files[i].original || files[i].closeAttempted)return false;
        return true; // Actual source references; not future consumer/native acceptance.
    }
    SIZE_T OriginalLaunchFileOrdinal() const noexcept { return originalLaunchFile_; }
    bool DisposeParserInputsAfterFullNativeSettlement(const State& state) noexcept {
        bool clean=true;
        for(SIZE_T i=0;i<parserInputCount_;++i)
            if(!parserInputs_[i]->DisposeAfterFullNativeSettlement(state))clean=false;
        return clean;
    }
    SIZE_T ParserInputCount() const noexcept{return parserInputCount_;}
    const OriginalParserRuntimeAssociation* PrivateParserRuntime() const noexcept{return parserRuntime_.get();}
    const OriginalParserInputCapture& PrivateParserInput(SIZE_T ordinal) const noexcept{return *parserInputs_[ordinal];}
    SIZE_T CanonicalParserSourceCount() const noexcept{return canonicalParserSourceCount_;}
    const OriginalFixtureRelativeFile& PrivateCanonicalParserSource(SIZE_T ordinal) const noexcept{return canonicalParserSources_[ordinal];}
    SIZE_T CanonicalAliasSourceCount() const noexcept {return canonicalAliasSourceCount_;}
    SIZE_T CanonicalHelperSourceCount() const noexcept{return canonicalHelperSourceCount_;}
    const OriginalFixtureRelativeFile& PrivateCanonicalHelperSource(SIZE_T ordinal) const noexcept{return canonicalHelperSources_[ordinal];}
    SIZE_T HelperCodeGuardCount(SIZE_T kind) const noexcept{return kind<2?helperCodeGuardCounts_[kind]:0;}
    const OriginalHelperCodeGuard& PrivateHelperCodeGuard(SIZE_T kind,SIZE_T ordinal) const noexcept{return *helperCodeGuards_[kind*6+ordinal];}
    SIZE_T AliasCodeGuardCount(SIZE_T kind) const noexcept{return kind<2?aliasCodeGuardCounts_[kind]:0;}
    const OriginalAliasCodeGuard& PrivateAliasCodeGuard(SIZE_T kind,SIZE_T ordinal) const noexcept{return *aliasCodeGuards_[kind*8+ordinal];}
    const OriginalFixtureRelativeFile& PrivateCanonicalAliasSource(SIZE_T ordinal) const noexcept {return canonicalAliasSources_[ordinal];}
    const char* FirstFailure() const noexcept { return firstFailure_; }
};
#if defined(TEMPLATE_NATIVE_CHAIN_REGRESSION)
bool ObserveOriginalWindowsRetainedBackingRegression(const OriginalWindowsGate&) noexcept;
#endif
// Session still must bind actual caller/root/tool rights and source admission,
// complete selected input membership, writer exclusion and actual supplied native
// tool/source provenance. No new signature issuer is inferred by this gate.
}

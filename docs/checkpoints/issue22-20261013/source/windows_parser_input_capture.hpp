// UNCOMPILED full copied parser-input acquisition. No early-release grant is
// inferred from DATA classification, exit code, or the CODE guard rule.
#pragma once
#include "windows_retained_backing.hpp"
#include "windows_helper_code_guard.hpp"
#include "windows_parser_runtime_association.hpp"
namespace template_creator {
class OriginalParserInputCapture {
    OriginalCopiedSourceCapture copy_;
    std::array<OriginalFixtureNameHold,2> directories_;
    SIZE_T directoryCount_=0;
    OriginalFixtureRelativeFile target_;
    std::array<wchar_t,32768> relative_{},literal_{};
    const State* stateOriginal_=nullptr;const Event* createOriginal_=nullptr;
    const Event* exitOriginal_=nullptr;
    const OriginalHelperSourceProjection* projectionOriginal_=nullptr;
    SIZE_T memberOrdinal_=SIZE_MAX,processOrdinal_=SIZE_MAX;
    FILETIME birthOriginal_{};
    bool attempted_=false,captured_=false,consumerEnrolled_=false,disposalAttempted_=false,disposalReturned_=false;
    bool unborrowedDisposalAttempted_=false,unborrowedDisposalReturned_=false;
public:
    enum class Role {Service,Harness};
    Role originalRole=Role::Service;
    static SIZE_T CheckedSourceBytes(SIZE_T rows,SIZE_T bytes) {
        const SIZE_T nested=OriginalCopiedSourceCapture::CheckedSourceBytes(rows,bytes);
        constexpr SIZE_T own=sizeof(OriginalParserInputCapture)-sizeof(OriginalCopiedSourceCapture);
        if(nested>SIZE_MAX-own)throw std::invalid_argument("parser capture aggregate geometry");
        return own+nested;
    }
    static SIZE_T CheckedReadRows(SIZE_T rows,SIZE_T bytes) {
        (void)CheckedSourceBytes(rows,bytes);return rows;
    }
    OriginalParserInputCapture(SIZE_T rows,SIZE_T bytes):copy_(CheckedReadRows(rows,bytes),bytes){}
    bool Capture(const State& state,const Event& create,const OriginalCommandLineObservation& command,
        const OriginalHelperSourceProjection& projection,const OriginalWindowsFixtureRoot& root,
        const NativeHeldFiles& source,SIZE_T sourceOrdinal,Role role) noexcept {
        if(attempted_)return false;attempted_=true;
        stateOriginal_=&state;createOriginal_=&create;projectionOriginal_=&projection;originalRole=role;
        bool test=false;SIZE_T fixtureUnits=0;
        if(!state.eventCount || &create!=&state.events[state.eventCount-1] ||
            create.original.dwDebugEventCode!=CREATE_PROCESS_DEBUG_EVENT || command.stateOriginal!=&state ||
            command.eventOriginal!=&create || projection.commandOriginal!=&command ||
            !OriginalHelperCodeGuard::Select(root,projection,test,fixtureUnits) ||
            create.createdMemberOrdinal>=state.memberCount || create.createdProcessResource>=state.resourceCount)return false;
        memberOrdinal_=create.createdMemberOrdinal;processOrdinal_=create.createdProcessResource;
        const auto& member=state.members[memberOrdinal_];const auto& process=state.resources[processOrdinal_];
        birthOriginal_=member.birth;
        if(member.process!=processOrdinal_ || !positive(birthOriginal_) || process.kind!=Kind::Process ||
            !process.acquired || process.unknownOutput || process.attempted)return false;
        consumerEnrolled_=true; // Before the first possible native acquisition.
        const wchar_t* tail=role==Role::Service?L"\\service.json":role==Role::Harness?L"\\verify\\service-harness.json":nullptr;
        if(!tail)return false;
        const SIZE_T relativeUnits=fixtureUnits+wcslen(tail);
        if(relativeUnits>=relative_.size() || root.environmentPathUnits+1+relativeUnits>=literal_.size())return false;
        const auto* fixture=projection.helperPath.data()+root.environmentPathUnits+1;
        memcpy(relative_.data(),fixture,fixtureUnits*sizeof(wchar_t));
        memcpy(relative_.data()+fixtureUnits,tail,(wcslen(tail)+1)*sizeof(wchar_t));
        memcpy(literal_.data(),root.environmentPath.data(),root.environmentPathUnits*sizeof(wchar_t));
        literal_[root.environmentPathUnits]=L'\\';
        memcpy(literal_.data()+root.environmentPathUnits+1,relative_.data(),(relativeUnits+1)*sizeof(wchar_t));
        if(!directories_[directoryCount_++].Capture(root,relative_.data(),fixtureUnits,true,true))return false;
        if(role==Role::Harness && !directories_[directoryCount_++].Capture(root,relative_.data(),fixtureUnits+7,true,true))return false;
        if(!copy_.Capture(source,sourceOrdinal,literal_.data()))return false;
        captured_=target_.Capture(root,relative_.data(),copy_);return captured_;
    }
    bool DisposeAfterFullNativeSettlement(const State& state) noexcept {
        if(!consumerEnrolled_)return DisposeBeforeConsumerAcquisitionEligible();
        if(!attempted_ || stateOriginal_!=&state || !state.terminal || !state.streams.NativeEof() ||
            memberOrdinal_>=state.memberCount || processOrdinal_>=state.resourceCount)return false;
        const auto& member=state.members[memberOrdinal_];
        if(!member.shutdown || !member.continuedExit || member.process!=processOrdinal_ ||
            !same_time(member.birth,birthOriginal_) || member.originalCreateEvent>=state.eventCount ||
            &state.events[member.originalCreateEvent]!=createOriginal_)return false;
        if(!disposalAttempted_) {
            disposalAttempted_=true;
            const bool target=target_.DisposeIndependentlyEligible();const bool copied=copy_.DisposeIndependentlyEligible();
            bool directories=true;
            for(SIZE_T i=0;i<directoryCount_;++i)if(!directories_[i].CloseOnce())directories=false;
            disposalReturned_=target&&copied&&directories;
        }
        return disposalReturned_; // Physical cleanup, not qualification.
    }
    bool ObserveLastThreadExit(const State& state,const Event& exit) noexcept {
        if(!consumerEnrolled_ || stateOriginal_!=&state || !state.eventCount ||
            &exit!=&state.events[state.eventCount-1] ||
            exit.original.dwDebugEventCode!=EXIT_PROCESS_DEBUG_EVENT ||
            exit.exitingMemberOrdinal!=memberOrdinal_ || exit.exitingProcessResource!=processOrdinal_ ||
            memberOrdinal_>=state.memberCount)return false;
        const auto& member=state.members[memberOrdinal_];
        if(member.originalCreateEvent>=state.eventCount-1 ||
            &state.events[member.originalCreateEvent]!=createOriginal_ ||
            !same_time(member.birth,birthOriginal_) || member.pid!=exit.original.dwProcessId)return false;
        if(exitOriginal_)return exitOriginal_==&exit;
        exitOriginal_=&exit;
        // Retain the actual last-thread boundary independently. This does not
        // infer synchronous provider disposal or authorize parser DATA release.
        // The runtime/build join must consume this SAME saved occurrence.
        return true;
    }
    bool BelongsTo(const State& state,const Event& create) const noexcept {
        return attempted_ && stateOriginal_==&state && createOriginal_==&create;
    }
    const Event* PrivateLastThreadExit() const noexcept{return exitOriginal_;}
    bool DisposeAfterSupportedSynchronousContract(const OriginalParserRuntimeAssociation& runtime,
        const OriginalParserRuntimeAssociation::ConsumerContractResult& result) noexcept {
        if(!captured_||!consumerEnrolled_||!stateOriginal_||!createOriginal_||!exitOriginal_||
            !runtime.AdmitsSynchronousRelease(result,*stateOriginal_,*createOriginal_,*exitOriginal_))return false;
        if(!disposalAttempted_){
            disposalAttempted_=true;
            const bool target=target_.DisposeIndependentlyEligible();
            const bool copied=copy_.DisposeIndependentlyEligible();
            bool directories=true;
            for(SIZE_T i=0;i<directoryCount_;++i)if(!directories_[i].CloseOnce())directories=false;
            disposalReturned_=target&&copied&&directories;
        }
        return disposalReturned_; // Physical result remains separate from contract qualification.
    }
    bool DisposeBeforeConsumerAcquisitionEligible() noexcept {
        if(!attempted_ || consumerEnrolled_)return false;
        if(!unborrowedDisposalAttempted_) {
            unborrowedDisposalAttempted_=true;
            // No native effect is permitted before enrollment in Capture.
            // Check actual owner cells too; absence of a valid member ordinal
            // alone never authorizes disposing an acquired or unknown original.
            unborrowedDisposalReturned_=!directoryCount_ && !target_.nativeAttempted &&
                !copy_.PrivateGenerated().FileCount() && !copy_.PrivateComparison().attempted;
        }
        return unborrowedDisposalReturned_; // Empty physical owner, failed capture.
    }
    bool PhysicallyDisposed() const noexcept{return !attempted_||(disposalAttempted_&&disposalReturned_) ||
        (unborrowedDisposalAttempted_&&unborrowedDisposalReturned_);}
    SIZE_T ReservedSourceBytes() const noexcept{return sizeof(*this)-sizeof(copy_)+copy_.ReservedSourceBytes();}
    const OriginalCopiedSourceCapture& PrivateCopy() const noexcept{return copy_;}
    bool Captured() const noexcept{return captured_;}
    // The normal-release consumer is connected, but its supported host/provider
    // result producer is still incomplete and cannot yet grant this branch.
public:
    bool OutputBackingSeparateOriginal(const void* output,std::size_t bytes) const noexcept {
        return OriginalRetainedStorageSeparate(output,bytes,this,1,sizeof(*this))&&copy_.OutputBackingSeparateOriginal(output,bytes);
    }
};
}

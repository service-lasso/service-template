// UNCOMPILED per-consumer copied CODE ownership. Paths select observations;
// full byte correspondence and native identities establish the file relation.
#pragma once
#include "windows_fixture_relative_file.hpp"
#include "windows_command_line_observation.hpp"
#include "windows_fixture_name_hold.hpp"
namespace template_creator {
class OriginalAliasCodeGuard {
    OriginalCopiedSourceCapture copy_;
    OriginalFixtureRelativeFile physical_,alias_;
    // Selected deepest CODE path is fixture/runtime/win32/echo-service.ps1:
    // one fixture + two physical + two alias directory entries + file link.
    std::array<OriginalFixtureNameHold,6> names_;
    SIZE_T nameCount_=0;
    const State* stateOriginal_=nullptr;
    const Event* createOriginal_=nullptr;
    const Event* exitOriginal_=nullptr;
    const OriginalCommandLineObservation* commandOriginal_=nullptr;
    SIZE_T memberOrdinal_=SIZE_MAX,processOrdinal_=SIZE_MAX;
    FILETIME birthOriginal_{};
    bool attempted_=false,associated_=false,releaseEligible_=false;
    bool releaseAttempted_=false,releaseReturned_=false;
    std::array<wchar_t,32768> physicalLiteral_{},physicalRelative_{},aliasRelative_{};
public:
    static SIZE_T CheckedSourceBytes(SIZE_T rows,SIZE_T bytes) {
        const SIZE_T nested=OriginalCopiedSourceCapture::CheckedSourceBytes(rows,bytes);
        constexpr SIZE_T own=sizeof(OriginalAliasCodeGuard)-sizeof(OriginalCopiedSourceCapture);
        if(nested>SIZE_MAX-own)throw std::invalid_argument("alias guard aggregate geometry");
        return own+nested;
    }
    static SIZE_T CheckedReadRows(SIZE_T rows,SIZE_T bytes) {
        (void)CheckedSourceBytes(rows,bytes);return rows;
    }
    OriginalAliasCodeGuard(SIZE_T rows,SIZE_T bytes):copy_(CheckedReadRows(rows,bytes),bytes){}
    OriginalAliasCodeGuard(const OriginalAliasCodeGuard&)=delete;
    OriginalAliasCodeGuard& operator=(const OriginalAliasCodeGuard&)=delete;
    // A selector for exactly the unchanged fixture's two alias directory forms.
    // Selection is informational until Capture joins bytes and held objects.
    static bool Select(const OriginalWindowsFixtureRoot& root,const OriginalCommandLineObservation& command,
        SIZE_T& scriptKind,SIZE_T& fixtureUnits,bool& encoded) noexcept {
        if(!command.nodeScriptSeen || !command.nodeSourceFormComplete || !root.environmentPathBound)return false;
        const auto* path=command.nodeScript.data();const SIZE_T units=command.nodeScriptUnits;
        SIZE_T start=root.environmentPathUnits;
        if(units>=command.nodeScript.size() || start>=root.environmentPath.size() || path[units])return false;
        if(units<=start || memcmp(path,root.environmentPath.data(),start*sizeof(wchar_t)) || path[start]!=L'\\')return false;
        ++start;const SIZE_T fixtureBegin=start;
        const wchar_t prefix[]=L"template-physical-cli-";
        constexpr SIZE_T prefixUnits=std::size(prefix)-1;
        if(units-start<prefixUnits || memcmp(path+start,prefix,prefixUnits*sizeof(wchar_t)))return false;
        while(start<units&&path[start]!=L'\\')++start;
        if(start==units || start==fixtureBegin+prefixUnits)return false;
        fixtureUnits=start-fixtureBegin;++start;
        const SIZE_T aliasBegin=start;while(start<units&&path[start]!=L'\\')++start;
        if(start==units)return false;
        const SIZE_T aliasUnits=start-aliasBegin;
        const wchar_t ordinary[]=L"alias",special[]=L"alias space # percent% é";
        encoded=aliasUnits==std::size(special)-1 && !memcmp(path+aliasBegin,special,aliasUnits*sizeof(wchar_t));
        if(!encoded && !(aliasUnits==5&&!memcmp(path+aliasBegin,ordinary,5*sizeof(wchar_t))))return false;
        ++start;
        const wchar_t* names[]={L"validate-development-candidate-workflow.mjs",L"workflow.mjs",L"publish-development-candidate.mjs",L"publisher.mjs"};
        for(SIZE_T i=0;i<4;++i)if(units-start==wcslen(names[i])&&!memcmp(path+start,names[i],(units-start)*sizeof(wchar_t))) {
            scriptKind=i/2;return true;
        }
        return false;
    }
    bool CaptureSelected(const State& state,const Event& create,const OriginalCommandLineObservation& command,
        const OriginalWindowsFixtureRoot& root,const NativeHeldFiles& source,SIZE_T sourceOrdinal) noexcept {
        SIZE_T kind=0,fixtureUnits=0;bool encoded=false;
        if(!command.nodeImageAssociated || !Select(root,command,kind,fixtureUnits,encoded))return false;
        const auto* relative=command.nodeScript.data()+root.environmentPathUnits+1;
        const wchar_t* physical=encoded?L"physical space # percent% é":L"physical";
        const wchar_t* name=kind?L"publish-development-candidate.mjs":L"validate-development-candidate-workflow.mjs";
        const SIZE_T relativeUnits=fixtureUnits+1+wcslen(physical)+1+wcslen(name);
        if(relativeUnits>=physicalRelative_.size() || root.environmentPathUnits+1+relativeUnits>=physicalLiteral_.size() ||
            command.nodeScriptUnits-root.environmentPathUnits-1>=aliasRelative_.size())return false;
        SIZE_T n=0;
        for(SIZE_T i=0;i<fixtureUnits;++i)physicalRelative_[n++]=relative[i];
        physicalRelative_[n++]=L'\\';
        for(SIZE_T i=0;physical[i];++i)physicalRelative_[n++]=physical[i];
        physicalRelative_[n++]=L'\\';for(SIZE_T i=0;name[i];++i)physicalRelative_[n++]=name[i];
        memcpy(physicalLiteral_.data(),root.environmentPath.data(),root.environmentPathUnits*sizeof(wchar_t));
        physicalLiteral_[root.environmentPathUnits]=L'\\';
        memcpy(physicalLiteral_.data()+root.environmentPathUnits+1,physicalRelative_.data(),(n+1)*sizeof(wchar_t));
        memcpy(aliasRelative_.data(),relative,(command.nodeScriptUnits-root.environmentPathUnits)*sizeof(wchar_t));
        return Capture(state,create,command,root,source,sourceOrdinal,physicalLiteral_.data(),physicalRelative_.data(),aliasRelative_.data());
    }
private:
    bool HoldSelectedNames(const OriginalWindowsFixtureRoot& root,const wchar_t* physicalRelative,const wchar_t* aliasRelative,bool aliasRole) noexcept {
        SIZE_T fixtureUnits=0;while(physicalRelative[fixtureUnits]&&physicalRelative[fixtureUnits]!=L'\\')++fixtureUnits;
        if(!physicalRelative[fixtureUnits])return false;
        // Hold the concrete fixture and both directories before copying or
        // following the alias. File-link fixtures also hold the link itself.
        // These are per-consumer holds; later cleanup follows this actual EXIT.
        SIZE_T physicalDirectoryUnits=wcslen(physicalRelative),aliasDirectoryUnits=wcslen(aliasRelative);
        while(physicalDirectoryUnits&&physicalRelative[physicalDirectoryUnits]!=L'\\')--physicalDirectoryUnits;
        while(aliasDirectoryUnits&&aliasRelative[aliasDirectoryUnits]!=L'\\')--aliasDirectoryUnits;
        if(physicalDirectoryUnits<=fixtureUnits || aliasDirectoryUnits<=fixtureUnits)return false;
        const wchar_t* aliasName=aliasRelative+aliasDirectoryUnits+1;
        const bool fileAlias=!wcscmp(aliasName,L"workflow.mjs")||!wcscmp(aliasName,L"publisher.mjs");
        // Ordinary directories are opened as entries, with actual captured
        // reparse attributes rejected. A target-only hold would not pin a
        // distinct directory entry. The selected alias alone allows reparses.
        bool names=names_[nameCount_++].Capture(root,physicalRelative,fixtureUnits,true,true);
        for(SIZE_T i=fixtureUnits+1;names&&i<=physicalDirectoryUnits;++i)if(physicalRelative[i]==L'\\') {
            if(nameCount_==names_.size())return false;
            names=names_[nameCount_++].Capture(root,physicalRelative,i,true,true);
        }
        for(SIZE_T i=fixtureUnits+1;names&&i<=aliasDirectoryUnits;++i)if(aliasRelative[i]==L'\\') {
            if(nameCount_==names_.size())return false;
            names=names_[nameCount_++].Capture(root,aliasRelative,i,true,true,aliasRole&&!fileAlias&&i==aliasDirectoryUnits);
        }
        if(names&&fileAlias) {
            if(nameCount_==names_.size())return false;
            names=names_[nameCount_++].Capture(root,aliasRelative,wcslen(aliasRelative),false,true,aliasRole);
        }
        return names;
    }
public:
    bool Capture(const State& state,const Event& create,const OriginalCommandLineObservation& command,
        const OriginalWindowsFixtureRoot& root,const NativeHeldFiles& source,SIZE_T sourceOrdinal,
        const wchar_t* physicalLiteral,const wchar_t* physicalRelative,const wchar_t* aliasRelative,bool aliasRole=true) noexcept {
        if(attempted_)return false;attempted_=true;
        stateOriginal_=&state;createOriginal_=&create;commandOriginal_=&command;
        if(!state.eventCount || &create!=&state.events[state.eventCount-1] ||
            create.original.dwDebugEventCode!=CREATE_PROCESS_DEBUG_EVENT ||
            command.stateOriginal!=&state || command.eventOriginal!=&create ||
            !command.captured || !command.crtProjectionComplete ||
            create.createdMemberOrdinal>=state.memberCount || create.createdProcessResource>=state.resourceCount)return false;
        memberOrdinal_=create.createdMemberOrdinal;processOrdinal_=create.createdProcessResource;
        const auto& member=state.members[memberOrdinal_];const auto& process=state.resources[processOrdinal_];
        birthOriginal_=member.birth;
        if(member.process!=processOrdinal_ || member.pid!=create.original.dwProcessId ||
            process.kind!=Kind::Process || !process.acquired || process.unknownOutput || process.attempted ||
            !positive(birthOriginal_))return false;
        if(!HoldSelectedNames(root,physicalRelative,aliasRelative,aliasRole))return false;
        if(!copy_.Capture(source,sourceOrdinal,physicalLiteral))return false;
        // Both metadata holds bind the actual copied file, including alias
        // traversal. No pathname prefix supplies the positive identity claim.
        const bool physical=physical_.Capture(root,physicalRelative,copy_);
        const bool alias=alias_.Capture(root,aliasRelative,copy_);
        associated_=physical&&alias;return associated_;
    }
    bool ObserveLastThreadExit(const State& state,const Event& exit) noexcept {
        if(!attempted_ || stateOriginal_!=&state || !state.eventCount ||
            &exit!=&state.events[state.eventCount-1] || exit.original.dwDebugEventCode!=EXIT_PROCESS_DEBUG_EVENT ||
            memberOrdinal_>=state.memberCount || processOrdinal_>=state.resourceCount ||
            exit.exitingMemberOrdinal!=memberOrdinal_ || exit.exitingProcessResource!=processOrdinal_)return false;
        const auto& member=state.members[memberOrdinal_];
        if(!member.exitEvent || member.process!=processOrdinal_ || !same_time(member.birth,birthOriginal_) ||
            member.originalCreateEvent>=state.eventCount-1 || &state.events[member.originalCreateEvent]!=createOriginal_ ||
            exit.original.dwProcessId!=createOriginal_->original.dwProcessId)return false;
        exitOriginal_=&exit;releaseEligible_=true;
        if(!releaseAttempted_) {
            releaseAttempted_=true;
            // Every independently eligible owner is visited despite another
            // close failure. Numeric uncertain handles are never retried.
            const bool alias=alias_.DisposeIndependentlyEligible();
            const bool physical=physical_.DisposeIndependentlyEligible();
            const bool copied=copy_.DisposeIndependentlyEligible();
            bool names=true;for(SIZE_T i=0;i<nameCount_;++i)if(!names_[i].CloseOnce())names=false;
            releaseReturned_=alias&&physical&&copied&&names;
        }
        return associated_&&releaseReturned_;
    }
    bool PhysicallyDisposed() const noexcept {
        return !attempted_ || (releaseEligible_&&releaseAttempted_&&releaseReturned_);
    }
    SIZE_T ReservedSourceBytes() const noexcept {
        return sizeof(*this)-sizeof(copy_)+copy_.ReservedSourceBytes();
    }
    const OriginalCopiedSourceCapture& PrivateCopy() const noexcept{return copy_;}
    const OriginalFixtureRelativeFile& PrivatePhysical() const noexcept{return physical_;}
    const OriginalFixtureRelativeFile& PrivateAlias() const noexcept{return alias_;}
    bool Associated() const noexcept{return associated_;}
    bool BelongsTo(const State& state,const Event& create) const noexcept {
        return attempted_&&stateOriginal_==&state&&createOriginal_==&create;
    }
    // This releases only this consumer's CODE holds after its actual last thread
    // event. Retained process duplicates, shutdown, streams and EOF are separate.
};
}

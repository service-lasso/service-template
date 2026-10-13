// UNCOMPILED actual root-relative file acquisition. This joins native objects,
// not producer authority: a reparse traversal can still leave the selected root.
#pragma once
#include <climits>
#include "windows_private_fixture_root.hpp"
#include "windows_copied_source_capture.hpp"
#include "windows_ingress.hpp"
namespace template_creator {
class OriginalFixtureRelativeFile {
public:
    const OriginalWindowsFixtureRoot* rootOriginal=nullptr;
    const wchar_t* selectedRelativeOriginal=nullptr;
    const OriginalCopiedSourceCapture* copyOriginal=nullptr;
    const OriginalPrivateIngress* canonicalIngressOriginal=nullptr;
    const NativeHeldFiles* canonicalInputsOriginal=nullptr;
    SIZE_T canonicalSourceOrdinal=SIZE_MAX;
    std::array<wchar_t,32768> relative{};
    SIZE_T relativeUnits=0;
    UNICODE_STRING name{};
    OBJECT_ATTRIBUTES attributes{};
    IO_STATUS_BLOCK status{};
    HANDLE returned=nullptr;
    NTSTATUS nativeReturned=0;
    bool attempted=false,nativeAttempted=false,acquired=false,unknown=false;
    bool identityCompared=false,sameCapturedFile=false;
    FileIdentityObservation identity;
    bool closeAttempted=false;
    BOOL closeReturned=FALSE;
    DWORD closeError=0;
    OriginalFixtureRelativeFile()=default;
    OriginalFixtureRelativeFile(const OriginalFixtureRelativeFile&)=delete;
    OriginalFixtureRelativeFile& operator=(const OriginalFixtureRelativeFile&)=delete;
    enum class CanonicalScript {Workflow,Publisher,TestHelper,VerifyHelper,EchoRuntime,ServiceDocument,HarnessDocument};
    CanonicalScript canonicalSelection=CanonicalScript::Workflow;
    bool CaptureCanonicalSource(const OriginalPrivateIngress& ingress,const OriginalWindowsFixtureRoot& moduleRoot,
                                const NativeHeldFiles& source,CanonicalScript selection) noexcept {
        if(attempted)return false;attempted=true;
        canonicalIngressOriginal=&ingress;canonicalInputsOriginal=&source;rootOriginal=&moduleRoot;canonicalSelection=selection;
        const auto& capability=ingress.OriginalCapabilities()[2];
        if(!ingress.Captured() || !capability.informationResult || capability.closeAttempted || capability.aliasesEarlier ||
            !capability.original || capability.original==INVALID_HANDLE_VALUE || source.Failed() ||
            !moduleRoot.created || moduleRoot.rootUnknown || moduleRoot.rootCloseAttempted ||
            !moduleRoot.moduleAcquired || moduleRoot.moduleUnknown || moduleRoot.moduleCloseAttempted || !moduleRoot.createAddress)return false;
        const wchar_t* literal=selection==CanonicalScript::Workflow?L"scripts\\validate-development-candidate-workflow.mjs":
            selection==CanonicalScript::Publisher?L"scripts\\publish-development-candidate.mjs":
            selection==CanonicalScript::TestHelper?L"scripts\\test.ps1":
            selection==CanonicalScript::VerifyHelper?L"scripts\\verify.ps1":
            selection==CanonicalScript::EchoRuntime?L"runtime\\win32\\echo-service.ps1":
            selection==CanonicalScript::ServiceDocument?L"service.json":
            selection==CanonicalScript::HarnessDocument?L"verify\\service-harness.json":nullptr;
        if(!literal)return false;
        for(;literal[relativeUnits];++relativeUnits)relative[relativeUnits]=literal[relativeUnits];
        name.Buffer=relative.data();name.Length=static_cast<USHORT>(relativeUnits*sizeof(wchar_t));
        name.MaximumLength=static_cast<USHORT>((relativeUnits+1)*sizeof(wchar_t));
        attributes.Length=sizeof(attributes);attributes.RootDirectory=capability.original;attributes.ObjectName=&name;
        nativeAttempted=true;
        nativeReturned=reinterpret_cast<OriginalWindowsFixtureRoot::NativeCreate>(moduleRoot.createAddress)(
            &returned,GENERIC_READ|READ_CONTROL|SYNCHRONIZE,&attributes,&status,nullptr,
            FILE_ATTRIBUTE_NORMAL,FILE_SHARE_READ,1,0x20|0x40,nullptr,0);
        unknown=nativeReturned==static_cast<NTSTATUS>(0x103) || status.Status==static_cast<NTSTATUS>(0x103) ||
            (nativeReturned<0&&returned!=nullptr) || (nativeReturned>=0&&(!returned||returned==INVALID_HANDLE_VALUE));
        acquired=nativeReturned>=0&&!unknown;
        if(!acquired || status.Status<0)return false;
        identity.Capture(returned);identityCompared=true;
        if(!identity.captured || identity.standard.Directory)return false;
        for(SIZE_T i=0;i<source.FileCount();++i) {
            const auto& candidate=source.PrivateFiles()[i];
            const unsigned char* body=nullptr;SIZE_T extent=0;
            if(candidate.valid && !candidate.closeAttempted && source.ExactCapturedSourceSpan(i,body,extent) &&
                candidate.first.id.VolumeSerialNumber==identity.id.VolumeSerialNumber &&
                !memcmp(candidate.first.id.FileId.Identifier,identity.id.FileId.Identifier,sizeof(identity.id.FileId.Identifier))) {
                canonicalSourceOrdinal=i;sameCapturedFile=true;break;
            }
        }
        return sameCapturedFile;
    }
    bool Capture(const OriginalWindowsFixtureRoot& root,const wchar_t* selected,
                 const OriginalCopiedSourceCapture& copy) noexcept {
        if(attempted)return false;attempted=true;rootOriginal=&root;selectedRelativeOriginal=selected;copyOriginal=&copy;
        const auto& generated=copy.PrivateGenerated();
        if(generated.Failed() || generated.FileCount()!=1 ||
            copy.PrivateComparison().outcome!=OriginalCopyOutcome::Equal)return false;
        const auto& generatedFile=generated.PrivateFiles()[0];
        if(!generatedFile.valid || !generatedFile.original || generatedFile.closeAttempted)return false;
        const auto& capturedCopy=generatedFile.first;
        if(!selected || !root.created || !root.rootAcquired || root.rootUnknown ||
            root.rootCloseAttempted || root.dispositionAttempted || !root.moduleAcquired ||
            root.moduleCloseAttempted || !root.createAddress || !capturedCopy.captured)return false;
        SIZE_T componentBegin=0;bool terminated=false;
        for(SIZE_T i=0;i<relative.size();++i) {
            const auto value=selected[i];
            if(value==L':' || value==L'/' || (i==0&&value==L'\\'))return false;
            relative[i]=value;
            if(!value || value==L'\\') {
                const auto length=i-componentBegin;
                if(!length || (length==1&&relative[componentBegin]==L'.') ||
                    (length==2&&relative[componentBegin]==L'.'&&relative[componentBegin+1]==L'.'))return false;
                componentBegin=i+1;
            }
            if(!value){relativeUnits=i;terminated=true;break;}
        }
        // UNICODE_STRING byte lengths are USHORT, independently of backing size.
        if(!terminated || relativeUnits>(USHRT_MAX-sizeof(wchar_t))/sizeof(wchar_t))return false;
        name.Buffer=relative.data();name.Length=static_cast<USHORT>(relativeUnits*sizeof(wchar_t));
        name.MaximumLength=static_cast<USHORT>((relativeUnits+1)*sizeof(wchar_t));
        attributes.Length=sizeof(attributes);attributes.RootDirectory=root.root;attributes.ObjectName=&name;
        nativeAttempted=true;
        nativeReturned=reinterpret_cast<OriginalWindowsFixtureRoot::NativeCreate>(root.createAddress)(
            &returned,GENERIC_READ|READ_CONTROL|SYNCHRONIZE,&attributes,&status,nullptr,
            FILE_ATTRIBUTE_NORMAL,FILE_SHARE_READ,1,0x20|0x40,nullptr,0);
        // FILE_OPEN; synchronous non-directory open. No namespace rewrite,
        // backup intent, privilege, collision fallback or borrowed-handle close.
        unknown=nativeReturned==static_cast<NTSTATUS>(0x103) || status.Status==static_cast<NTSTATUS>(0x103) ||
            (nativeReturned<0&&returned!=nullptr) ||
            (nativeReturned>=0&&(!returned||returned==INVALID_HANDLE_VALUE));
        acquired=nativeReturned>=0&&!unknown;
        if(!acquired || status.Status<0)return false;
        identity.Capture(returned);
        identityCompared=true;
        sameCapturedFile=identity.captured && !identity.standard.Directory &&
            identity.id.VolumeSerialNumber==capturedCopy.id.VolumeSerialNumber &&
            memcmp(identity.id.FileId.Identifier,capturedCopy.id.FileId.Identifier,
                sizeof(identity.id.FileId.Identifier))==0;
        return sameCapturedFile;
    }
    bool DisposeIndependentlyEligible() noexcept {
        if(unknown)return false;
        if(acquired&&!closeAttempted) {
            closeAttempted=true;closeReturned=CloseHandle(returned);
            if(!closeReturned)closeError=GetLastError();
        }
        return !acquired||(closeAttempted&&closeReturned);
    }
    bool PhysicallyDisposed() const noexcept {
        return !unknown && (!acquired || (closeAttempted&&closeReturned));
    }
    // Caller must retain rootOriginal through a pending native acquisition.
    // SAME file equality is not evidence of creation by the owned test family,
    // alias ancestry, actual consumer decoding or CODE borrower settlement.
};
}

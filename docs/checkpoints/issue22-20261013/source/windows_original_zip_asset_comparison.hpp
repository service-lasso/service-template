// UNCOMPILED exact package/member byte correspondence. It does not authenticate
// a vendor record, select a managed provider or grant a parser release.
#pragma once
#include "windows_retained_backing.hpp"
#include "windows_original_zip_directory.hpp"
#include "windows_original_raw_member.hpp"
#include "windows_original_zip_crc.hpp"
#include "windows_held_files.hpp"
namespace template_creator {
class OriginalZipHeldAssetComparison {
    const NativeHeldFiles& packageOwner_;
    const NativeHeldFiles& assetOwner_;
    const OriginalZipDirectory& directory_;
    SIZE_T packageOrdinal_=SIZE_MAX,assetOrdinal_=SIZE_MAX,memberOrdinal_=SIZE_MAX;
    const unsigned char* packageBody_=nullptr;SIZE_T packageExtent_=0;
    const unsigned char* assetBody_=nullptr;SIZE_T assetExtent_=0;
    const unsigned char* memberBody_=nullptr;SIZE_T memberExtent_=0;
    std::unique_ptr<OriginalRawMember> inflater_;
    OriginalZipCrc crc_;
    bool attempted_=false,ownersAssociated_=false,memberDecoded_=false,crcMatched_=false,bytesEqual_=false;
    bool failed_=false,decoderDisposeAttempted_=false,decoderDisposed_=false;
    bool preownAttempted_=false,preowned_=false;
    std::exception_ptr preownException_;
    static bool HeldSpan(const NativeHeldFiles& owner,SIZE_T ordinal,const unsigned char*& body,SIZE_T& extent) noexcept {
        if(owner.Failed()||ordinal>=owner.FileCount())return false;
        const auto& row=owner.PrivateFiles()[ordinal];
        return row.valid&&!row.closeAttempted&&!row.readCompletionUnknown&&owner.ExactCapturedSourceSpan(ordinal,body,extent);
    }
public:
    OriginalZipHeldAssetComparison(const NativeHeldFiles& packageOwner,SIZE_T packageOrdinal,
        const OriginalZipDirectory& directory,SIZE_T memberOrdinal,
        const NativeHeldFiles& assetOwner,SIZE_T assetOrdinal)
        :packageOwner_(packageOwner),assetOwner_(assetOwner),directory_(directory),
         packageOrdinal_(packageOrdinal),assetOrdinal_(assetOrdinal),memberOrdinal_(memberOrdinal) {}
    bool PreownOriginal() noexcept {
        if(preownAttempted_)return false;preownAttempted_=true;
        try {
        // Pure association and allocation precede member library effects and
        // the eventual native command launch. Original native owners stay live.
        if(!directory_.CompleteFramingOriginal()||!directory_.SupportedVersionOriginal()||memberOrdinal_>=directory_.MemberCount()||
            !HeldSpan(packageOwner_,packageOrdinal_,packageBody_,packageExtent_)||
            packageBody_!=directory_.PrivateOriginal()||packageExtent_!=directory_.OriginalExtent()||
            !HeldSpan(assetOwner_,assetOrdinal_,assetBody_,assetExtent_)){failed_=true;return false;}
        const auto& member=directory_.PrivateMembers()[memberOrdinal_];
        if(!member.centralObserved||!member.localObserved||!member.supportedVersion||member.uncompressed!=assetExtent_||
            member.compressed>SIZE_MAX||member.body>packageExtent_||member.compressed>packageExtent_-member.body){failed_=true;return false;}
        if(member.method==8){
            if(OriginalRawMember::ReservedSourceBytesFor(static_cast<SIZE_T>(member.compressed),assetExtent_)>SIZE_MAX-sizeof(*this))
                throw std::invalid_argument("original member comparison aggregate geometry");
            inflater_=std::make_unique<OriginalRawMember>(static_cast<SIZE_T>(member.compressed),assetExtent_);
            if(!inflater_->PreownOriginal()){
                if(inflater_->PreownExceptionOriginal())std::rethrow_exception(inflater_->PreownExceptionOriginal());
                failed_=true;return false;
            }
        }
        else if(member.method!=0){failed_=true;return false;}
        ownersAssociated_=true;preowned_=true;return true;
        }catch(...){preownException_=std::current_exception();failed_=true;return false;}
    }
    const std::exception_ptr& PreownExceptionOriginal() const noexcept{return preownException_;}
    OriginalZipHeldAssetComparison(const OriginalZipHeldAssetComparison&)=delete;
    OriginalZipHeldAssetComparison& operator=(const OriginalZipHeldAssetComparison&)=delete;
    bool CaptureMemberOriginal() noexcept {
        if(attempted_||decoderDisposeAttempted_)return false;attempted_=true;
        if(!preowned_||failed_||!ownersAssociated_){failed_=true;return false;}
        const unsigned char* samePackage=nullptr;SIZE_T packageExtent=0;
        const unsigned char* sameAsset=nullptr;SIZE_T assetExtent=0;
        if(!HeldSpan(packageOwner_,packageOrdinal_,samePackage,packageExtent)||samePackage!=packageBody_||packageExtent!=packageExtent_||
            !HeldSpan(assetOwner_,assetOrdinal_,sameAsset,assetExtent)||sameAsset!=assetBody_||assetExtent!=assetExtent_){failed_=true;return false;}
        const auto& member=directory_.PrivateMembers()[memberOrdinal_];
        const auto* compressed=packageBody_+member.body;
        if(inflater_){
            if(!inflater_->Capture(compressed,static_cast<SIZE_T>(member.compressed))){failed_=true;return false;}
            memberBody_=inflater_->PrivateOutput();memberExtent_=inflater_->RetainedOutputExtent();
        }else{memberBody_=compressed;memberExtent_=static_cast<SIZE_T>(member.compressed);}
        memberDecoded_=true;
        if(memberExtent_!=assetExtent_||!crc_.Capture(memberBody_,memberExtent_)){failed_=true;return false;}
        crcMatched_=crc_.CompleteOriginal()&&crc_.OriginalResult()==member.crc;
        bytesEqual_=memberExtent_==assetExtent_&&(!memberExtent_||!std::memcmp(memberBody_,assetBody_,memberExtent_));
        failed_=!crcMatched_||!bytesEqual_;return !failed_;
    }
    bool DisposeDecoderEligible() noexcept {
        decoderDisposeAttempted_=true;
        decoderDisposed_=!inflater_||inflater_->DisposeDecoderEligible();
        if(!decoderDisposed_)failed_=true;return decoderDisposed_;
    }
    bool BytesCorrespond() const noexcept{return attempted_&&ownersAssociated_&&memberDecoded_&&crcMatched_&&bytesEqual_&&!failed_;}
    SIZE_T OriginalAssetOrdinal() const noexcept{return assetOrdinal_;}
    SIZE_T OriginalPackageOrdinal() const noexcept{return packageOrdinal_;}
    const OriginalRawMember* PrivateInflater() const noexcept{return inflater_.get();}
    const OriginalZipCrc& PrivateCrc() const noexcept{return crc_;}
    SIZE_T ReservedSourceBytes() const noexcept{return sizeof(*this)+(inflater_?inflater_->ReservedSourceBytes():0);}
    // Caller retains package, directory, selected asset, this decoder and all
    // returned byte views at the actual quiescent source/native borrower cut.
    // Disposal here frees no original held input or output backing.
public:
    bool OutputBackingSeparateOriginal(const void* output,std::size_t bytes) const noexcept {
        return OriginalRetainedStorageSeparate(output,bytes,this,1,sizeof(*this))&&OriginalRetainedOwnerSeparate(output,bytes,inflater_);
    }
};
}

// UNCOMPILED private runtime/source owner joins. A structural join is not a
// vendor assertion, an admitted build, or a synchronous-disposal qualification.
#pragma once
#include "windows_retained_backing.hpp"
#include "windows_helper_source_projection.hpp"
#include "windows_held_files.hpp"
#include "windows_memory.hpp"
#include "windows_coreclr_entry_properties.hpp"
#include "windows_runtime_host_configuration.hpp"
#include "windows_framework_list_source.hpp"
#include "windows_portable_rid_source.hpp"
#include "windows_original_zip_directory.hpp"
#include "windows_original_zip_name.hpp"
#include "windows_original_zip_asset_comparison.hpp"
#include "windows_msf_source_owner.hpp"
#include "native_command_plan.hpp"
#include <exception>
namespace template_creator {
class OriginalParserRuntimeAssociation {
public:
    struct RuntimeEnvironmentSourceSpan {
        enum class Outcome {Unobserved,InputUnresolved,SourceAbsent,SourceEmpty,SourceNonempty};
        const template_creator_bridge::WindowsOriginalCommand* commandOriginal=nullptr;
        const char* sourceKey=nullptr;
        SIZE_T valueBegin=SIZE_MAX,valueUnits=0,matchingKeys=0;
        bool attempted=false,keyComparisonUnresolved=false;
        Outcome outcome=Outcome::Unobserved;
    };
    struct StartupHooksSourcePlan {
        enum class Outcome {Unobserved,InputUnresolved,SourceNeither,SourceConfigOnly,SourceEnvOnly,SourceEnvThenConfig};
        const template_creator_bridge::WindowsOriginalCommand* environmentOwnerOriginal=nullptr;
        OriginalRuntimeHostConfiguration::ConfiguredProperty appConfigOriginal;
        SIZE_T environmentValueBegin=SIZE_MAX,environmentValueUnits=0;
        SIZE_T matchingEnvironmentKeys=0;
        bool environmentKeyComparisonUnresolved=false;
        char16_t separator=u';';
        Outcome outcome=Outcome::Unobserved;
        bool attempted=false;
        // Ordered segments only. No actual child getenv return is fabricated.
    };
    struct RuntimeEnvironmentIntegerSource {
        RuntimeEnvironmentSourceSpan span;
        std::int32_t value=0;
        SIZE_T consumedUnits=0;
        bool attempted=false,sourceValueKnown=false,sourceOverflow=false,localeUnresolved=false;
        // Source predictions only; no actual CRT errno or getenv result.
    };
    struct AssetOccurrence {
        const State* stateOriginal=nullptr;
        const Event* eventOriginal=nullptr;
        const FileIdentityObservation* fileOriginal=nullptr;
        const LoadedImageObservation* loadedOriginal=nullptr;
        SIZE_T selectedOrdinal=SIZE_MAX,memberOrdinal=SIZE_MAX,processOrdinal=SIZE_MAX;
        FILETIME birthOriginal{};
        bool attempted=false,membershipObserved=false;
    };
    struct FrameworkEnvironmentSourceConsumer {
        enum class Outcome {Unobserved,EarlierStageIncomplete,PrereleaseChoiceUnresolved,
            EnvironmentInputUnresolved,RollComparisonUnresolved,CommandOverridesPending};
        const OriginalRuntimeHostConfiguration::FrameworkSettingsSource* frameworkOriginal=nullptr;
        const RuntimeEnvironmentSourceSpan* rollOriginal=nullptr;
        int rollForwardSource=-1;
        bool attempted=false,preferReleaseReached=false,preferReleaseKnown=false,preferRelease=false,rollAssigned=false;
        Outcome outcome=Outcome::Unobserved;
    };
    struct HelperHostOverrideSource {
        const OriginalHelperSourceProjection* helperOriginal=nullptr;
        const OriginalCommandLineObservation* commandOriginal=nullptr;
        SIZE_T firstUnrecognizedArgument=SIZE_MAX,sourceParsedHostOptions=0;
        bool attempted=false,conditionalApphostEmptyOptions=false;
        bool sourceAssumesRunningExecutableIncluded=false,actualArgumentOffsetAssociated=false;
        // Actual host mode/argv/build association remains independently unmet.
        bool actualHostModeAssociated=false;
    };
    struct VendorNameOccurrence {
        OriginalZipNameSelection central,local;bool sameDecoded=false;
    };
    struct ConsumerContractResult {
        enum class Cause {Unobserved,InvalidOccurrence,SourceRelationshipMissing,
            HostConfigurationMissing,RuntimeOccurrenceMissing,RuntimeOccurrenceAmbiguous,
            VendorCorrespondenceMissing,HostProviderResolutionUnimplemented,InitializerOccurrenceMissing,InitializerOccurrenceInvalid};
        const State* stateOriginal=nullptr;
        const Event* createOriginal=nullptr;
        const Event* exitOriginal=nullptr;
        const OriginalParserRuntimeAssociation* associationOriginal=nullptr;
        const OriginalHelperSourceProjection* helperCallerOriginal=nullptr;
        const OriginalCommandLineObservation* helperCommandOriginal=nullptr;
        const OriginalCoreClrEntryProperties* initializerPropertiesOriginal=nullptr;
        const Event* initializerEntryOriginal=nullptr;
        const Event* initializerThreadBirthOriginal=nullptr;
        SIZE_T initializerEntryOrdinal=SIZE_MAX;
        SIZE_T initializerThreadBirthOrdinal=SIZE_MAX;
        bool initializerUsefulContinuationObserved=false;
        struct AppPropertyCorrespondence {
            const OriginalCoreClrEntryProperties* initializerOriginal=nullptr;
            const OriginalRuntimeHostConfiguration* configurationOriginal=nullptr;
            OriginalRuntimeHostConfiguration::PropertyCursor cursorOriginal;
            OriginalRuntimeHostConfiguration::ConfiguredProperty firstUnresolvedSource;
            SIZE_T visited=0,matched=0,missing=0,ambiguous=0,different=0,unresolved=0;
            SIZE_T firstUnresolvedInitializerIndex=SIZE_MAX;
            bool attempted=false,traversalComplete=false;
        } appPropertyCorrespondence;
        struct ProviderFilePathCandidates {
            const OriginalCoreClrEntryProperties* initializerOriginal=nullptr;
            const NativeHeldFiles* filesOriginal=nullptr;
            SIZE_T propertyIndex=SIZE_MAX,segments=0,emptySegments=0,unique=0,missing=0,ambiguous=0,invalid=0;
            SIZE_T firstUnresolvedOffset=SIZE_MAX,firstUnresolvedExtent=0;
            bool attempted=false,propertyUnique=false,emptyValue=false,complete=false;
        };
        std::array<ProviderFilePathCandidates,3> providerFilePathCandidates;
        std::array<SIZE_T,4> occurrenceOrdinals{SIZE_MAX,SIZE_MAX,SIZE_MAX,SIZE_MAX};
        std::array<SIZE_T,4> vendorComparisonOrdinals{SIZE_MAX,SIZE_MAX,SIZE_MAX,SIZE_MAX};
        std::array<SIZE_T,2> configurationOrdinals{SIZE_MAX,SIZE_MAX};
        std::array<SIZE_T,2> configurationVendorOrdinals{SIZE_MAX,SIZE_MAX};
        SIZE_T memberOrdinal=SIZE_MAX;
        const OriginalRuntimeHostConfiguration* appConfigurationOriginal=nullptr;
        OriginalRuntimeHostConfiguration::RidGraphSourceDecision appRidGraphSource;
        OriginalRuntimeHostConfiguration::SetAppPathsSourceDecision appSetAppPathsSource;
        StartupHooksSourcePlan appStartupHooksSource;
        std::array<RuntimeEnvironmentSourceSpan,3> runtimeRollEnvironmentSources;
        std::array<RuntimeEnvironmentIntegerSource,2> runtimeRollIntegerSources;
        RuntimeEnvironmentSourceSpan runtimeRidEnvironmentSource;
        OriginalPortableRidSource portableRidLastSource,portableRidFirstUnresolved;
        SIZE_T portableRidSourceChoices=0,portableRidSourceUnresolved=0;
        bool portableRidSourceTraversalFinished=false;
        int sourceInitialDefaultRoll=-1;
        bool sourcePrereleaseChoiceKnown=false,sourceRollToPrerelease=false;
        OriginalRuntimeHostConfiguration::OrderedOptionSource runtimeDefaultSourceSeed;
        std::array<OriginalRuntimeHostConfiguration::OrderedOptionSource,2> runtimeOrderedSourceReplay;
        bool runtimeOrderedSourceReplayAttempted=false;
        FrameworkEnvironmentSourceConsumer frameworkEnvironmentSource;
        HelperHostOverrideSource helperHostOverrideSource;
        const std::array<OriginalRuntimeHostConfiguration::OrderedOptionSource,2>* appOrderedOptionsOriginal=nullptr;
        SIZE_T appReservedCollisionCount=0;
        bool appReservedInputUnresolved=false,appSourceDecisionsObserved=false;
        Cause cause=Cause::Unobserved;
        bool attempted=false;
        // No positive transition until the genuine selected host/provider and
        // synchronous control-flow relationship is implemented and consumed.
        bool normalSynchronousRelease=false;
    };
    struct SourceRelationship {
        // Exact retained tool/compiler input owner and original source record.
        // The containing NativePlatformState retains the Python originals;
        // this object never releases or substitutes their referents.
        PyObject* inputSnapshotOriginal=nullptr;
        PyObject* toolRoleOriginal=nullptr;
        const NativeHeldFiles* filesOriginal=nullptr;
        SIZE_T interpreterOrdinal=SIZE_MAX,buildRecordOrdinal=SIZE_MAX,sourceContractOrdinal=SIZE_MAX;
        const unsigned char* buildBytesOriginal=nullptr;
        const unsigned char* sourceBytesOriginal=nullptr;
        SIZE_T buildExtent=0,sourceExtent=0;
        bool attempted=false,retainedMembershipObserved=false;
        bool snapshotReferenceOwned=false,toolRoleReferenceOwned=false;
    };
    struct CoreClrPrivatePropertySourceCase {
        const NativeHeldFiles* filesOriginal=nullptr;
        // Entry/export/provider producer, original contract ABI, and owning map/context lock.
        std::array<SIZE_T,7> ordinals{SIZE_MAX,SIZE_MAX,SIZE_MAX,SIZE_MAX,SIZE_MAX,SIZE_MAX,SIZE_MAX};
        std::array<const unsigned char*,7> bodies{};
        std::array<SIZE_T,7> extents{};
        bool attempted=false,exactSourceObserved=false,ambiguous=false;
    };
    struct SupportedSourceCase {
        enum class Kind {Unknown,PowerShellExactRawFileContent};
        Kind kind=Kind::Unknown;
        std::array<SIZE_T,3> ordinals{SIZE_MAX,SIZE_MAX,SIZE_MAX};
        std::array<const unsigned char*,3> bodies{};
        std::array<SIZE_T,3> extents{};
        bool attempted=false,fullSourceCorrespondence=false;
    };
private:
    enum class HeldPathComparison {Equal,Different,Invalid};
    static HeldPathComparison ExactHeldPathUtf8(const HeldNativeFile& file,const unsigned char* bytes,SIZE_T extent) noexcept {
        static_assert(sizeof(wchar_t)==2);
        if(!bytes || !file.valid || !file.bytesCaptured || file.closeAttempted || file.readCompletionUnknown ||
            file.originalPathChars>=file.originalPath.size() || file.originalPath[file.originalPathChars])return HeldPathComparison::Invalid;
        SIZE_T used=0;
        for(SIZE_T i=0;i<file.originalPathChars;++i){
            std::uint32_t scalar=static_cast<std::uint16_t>(file.originalPath[i]);
            if(!scalar)return HeldPathComparison::Invalid;
            if(scalar>=0xd800 && scalar<=0xdbff){
                if(++i>=file.originalPathChars)return HeldPathComparison::Invalid;
                const std::uint32_t low=static_cast<std::uint16_t>(file.originalPath[i]);
                if(low<0xdc00 || low>0xdfff)return HeldPathComparison::Invalid;
                scalar=0x10000+((scalar-0xd800)<<10)+(low-0xdc00);
            }else if(scalar>=0xdc00 && scalar<=0xdfff)return HeldPathComparison::Invalid;
            unsigned char encoded[4]{};SIZE_T count=0;
            if(scalar<0x80){encoded[0]=static_cast<unsigned char>(scalar);count=1;}
            else if(scalar<0x800){encoded[0]=static_cast<unsigned char>(0xc0|(scalar>>6));encoded[1]=static_cast<unsigned char>(0x80|(scalar&63));count=2;}
            else if(scalar<0x10000){encoded[0]=static_cast<unsigned char>(0xe0|(scalar>>12));encoded[1]=static_cast<unsigned char>(0x80|((scalar>>6)&63));encoded[2]=static_cast<unsigned char>(0x80|(scalar&63));count=3;}
            else {encoded[0]=static_cast<unsigned char>(0xf0|(scalar>>18));encoded[1]=static_cast<unsigned char>(0x80|((scalar>>12)&63));encoded[2]=static_cast<unsigned char>(0x80|((scalar>>6)&63));encoded[3]=static_cast<unsigned char>(0x80|(scalar&63));count=4;}
            if(count>extent-used || memcmp(bytes+used,encoded,count))return HeldPathComparison::Different;
            used+=count;
        }
        return used==extent?HeldPathComparison::Equal:HeldPathComparison::Different;
    }
    const NativeHeldFiles& nativeInputsOriginal_;
    PyObject* snapshotOriginal_=nullptr;
    const template_creator_bridge::WindowsOriginalCommand* environmentOwnerOriginal_=nullptr;
    inline static constexpr SIZE_T SourceSizes[]={15151,24980,57701};
    inline static constexpr unsigned char SourceDigests[3][32]={
            {0x98,0x2b,0xc1,0x8d,0x10,0x53,0xda,0xf2,0x81,0x5b,0x04,0xf3,0x90,0x76,0xac,0xa0,0xbc,0xb8,0xa9,0x02,0xc3,0x29,0x81,0x76,0x08,0x36,0x29,0x5a,0xf5,0x73,0xd1,0xe8},
            {0xa4,0x39,0x97,0x99,0xcf,0xc6,0xfc,0xef,0x45,0xbe,0xec,0x5a,0x3d,0xe6,0x26,0xab,0x9e,0x40,0x2e,0x47,0x35,0x4e,0xad,0x4a,0x1f,0xf9,0x9a,0x3c,0x81,0xdd,0x9c,0x80},
            {0xbd,0x44,0x12,0x1e,0xf7,0xe4,0xa1,0x59,0x1e,0x1d,0x2d,0xb0,0xc1,0xbe,0xea,0x4f,0x08,0x0a,0x4f,0x69,0xec,0x62,0x23,0x62,0xd6,0x43,0x48,0x36,0x51,0x2c,0x95,0xfe}
        };
    SourceRelationship source_;
    SupportedSourceCase supportedSource_;
    CoreClrPrivatePropertySourceCase coreClrPropertySource_;
    std::unique_ptr<OriginalHeldMsfSource[]> compilerArtifacts_;
    std::unique_ptr<OriginalHeldSourceDigest[]> compilerSourceDigests_;
    SIZE_T compilerArtifactCount_=0,compilerArtifactDynamicBytes_=0;
    bool preownAttempted_=false,preowned_=false;
    std::exception_ptr preownExceptionOriginal_;
    std::unique_ptr<AssetOccurrence[]> occurrences_;
    SIZE_T capacity_=0,count_=0;
    std::unique_ptr<ConsumerContractResult[]> consumerResults_;
    SIZE_T consumerResultCount_=0;
    std::unique_ptr<OriginalRuntimeHostConfiguration> hostConfiguration_;
    std::unique_ptr<OriginalFrameworkListSource> frameworkListsSource_;
    std::array<SIZE_T,2> hostConfigurationOrdinals_{SIZE_MAX,SIZE_MAX};
    SIZE_T hostDevConfigurationOrdinal_=SIZE_MAX;
    bool hostDevConfigurationAmbiguous_=false;
    bool hostConfigurationAttempted_=false,hostConfigurationCorrespondence_=false;
    std::unique_ptr<OriginalZipDirectory> vendorDirectory_;
    std::unique_ptr<VendorNameOccurrence[]> vendorNames_;
    SIZE_T vendorNameCount_=0;
    SIZE_T vendorPackageOrdinal_=SIZE_MAX;
    bool vendorPackageAttempted_=false,vendorFramingCaptured_=false,vendorComparisonAttempted_=false;
    std::unique_ptr<std::unique_ptr<OriginalZipHeldAssetComparison>[]> vendorComparisons_;
    SIZE_T vendorComparisonCapacity_=0,vendorComparisonCount_=0,vendorComparisonBytes_=0;
    static bool BaseName(const HeldNativeFile& file,const wchar_t* literal) noexcept {
        if(!file.originalPathChars||file.originalPath[file.originalPathChars-1]!=L'\0')return false;
        const SIZE_T pathExtent=file.originalPathChars-1;
        SIZE_T begin=pathExtent;
        while(begin&&file.originalPath[begin-1]!=L'\\'&&file.originalPath[begin-1]!=L'/')--begin;
        SIZE_T i=0;
        while(literal[i]){
            if(begin+i==pathExtent)return false;
            wchar_t actual=file.originalPath[begin+i],expected=literal[i];
            if(actual>=L'A'&&actual<=L'Z')actual+=L'a'-L'A';
            if(actual!=expected)return false;++i;
        }
        return begin+i==pathExtent;
    }
    static bool MemberMatchesRelative(const OriginalZipNameSelection& selected,
        const wchar_t* relative,SIZE_T units) noexcept {
        if(!selected.CompleteOriginal())return false;
        OriginalZipNameCursor name(selected.SelectedOriginal(),selected.SelectedExtent(),selected.SelectedUtf8());
        SIZE_T i=0;std::uint32_t scalar=0;
        while(name.Next(scalar)){
            if(!scalar||scalar==L'\\'||i==units)return false;
            std::uint32_t actual=relative[i++];if(actual==L'\\')actual=L'/';
            if(actual>=0xd800&&actual<=0xdbff){
                if(i==units||relative[i]<0xdc00||relative[i]>0xdfff)return false;
                actual=0x10000+((actual-0xd800)<<10)+(relative[i++]-0xdc00);
            }else if(actual>=0xdc00&&actual<=0xdfff)return false;
            if(actual!=scalar)return false;
        }
        return name.CompleteOriginal()&&i==units;
    }
    static bool SameFile(const FileIdentityObservation& a,const FileIdentityObservation& b) noexcept {
        return a.captured && b.captured && a.id.VolumeSerialNumber==b.id.VolumeSerialNumber &&
            !memcmp(a.id.FileId.Identifier,b.id.FileId.Identifier,sizeof(a.id.FileId.Identifier));
    }
public:
    OriginalParserRuntimeAssociation(const NativeHeldFiles& inputs,PyObject* snapshot,SIZE_T originalImageCapacity,
        const template_creator_bridge::WindowsOriginalCommand& originalCommand)
        :nativeInputsOriginal_(inputs),snapshotOriginal_(snapshot),environmentOwnerOriginal_(&originalCommand),capacity_(originalImageCapacity) {}
    bool PreownOriginal() noexcept {
        if(preownAttempted_)return false;preownAttempted_=true;
        try {
        const SIZE_T originalImageCapacity=capacity_;
        const auto& inputs=nativeInputsOriginal_;
        if(!originalImageCapacity || originalImageCapacity>(SIZE_MAX-sizeof(*this))/sizeof(AssetOccurrence))
            throw std::invalid_argument("original runtime image roster geometry");
        capacity_=originalImageCapacity;
        if(originalImageCapacity>(SIZE_MAX-sizeof(*this)-capacity_*sizeof(AssetOccurrence))/sizeof(ConsumerContractResult))
            throw std::invalid_argument("original runtime consumer roster geometry");
        const SIZE_T baseCharge=sizeof(*this)+capacity_*sizeof(AssetOccurrence)+
            originalImageCapacity*sizeof(ConsumerContractResult);
        if(inputs.FileCount()>(SIZE_MAX-baseCharge)/(sizeof(OriginalHeldMsfSource)+sizeof(OriginalHeldSourceDigest)))
            throw std::invalid_argument("optional compiler artifact roster geometry");
        occurrences_=std::make_unique<AssetOccurrence[]>(originalImageCapacity);
        consumerResults_=std::make_unique<ConsumerContractResult[]>(originalImageCapacity);
        compilerArtifactCount_=inputs.FileCount();
        compilerArtifacts_=std::make_unique<OriginalHeldMsfSource[]>(inputs.FileCount());
        compilerSourceDigests_=std::make_unique<OriginalHeldSourceDigest[]>(inputs.FileCount());
        if(snapshotOriginal_&&PyTuple_CheckExact(snapshotOriginal_)&&PyTuple_GET_SIZE(snapshotOriginal_)==5){
            auto* requirements=PyTuple_GET_ITEM(snapshotOriginal_,4);
            if(requirements&&PyTuple_CheckExact(requirements)&&
               PyTuple_GET_SIZE(requirements)==static_cast<Py_ssize_t>(compilerArtifactCount_))
                for(SIZE_T i=0;i<compilerArtifactCount_;++i){
                    auto* row=PyTuple_GET_ITEM(requirements,static_cast<Py_ssize_t>(i));
                    if(!row||!PyTuple_CheckExact(row)||PyTuple_GET_SIZE(row)!=4)continue;
                    auto* role=PyTuple_GET_ITEM(row,0);
                    if(!role||!PyUnicode_CheckExact(role)||PyUnicode_CompareWithASCIIString(role,"compiler-input")!=0)continue;
                    auto& artifact=compilerArtifacts_[i];
                    const SIZE_T before=artifact.ReservedSourceBytes();
                    artifact.Observe(inputs,i,row,SIZE_MAX-ReservedSourceBytes()+before);
                    compilerArtifactDynamicBytes_+=artifact.ReservedSourceBytes()-before;
                    compilerSourceDigests_[i].Observe(inputs,i,row);
                }
        }
        for(SIZE_T i=0;i<compilerArtifactCount_;++i)
            compilerArtifacts_[i].ObserveSourceDigests(compilerSourceDigests_.get(),compilerArtifactCount_);
        if(snapshotOriginal_)Py_INCREF(snapshotOriginal_);
        preowned_=true;return true;
        }catch(...){
            preownExceptionOriginal_=std::current_exception();
            // Observe can throw after attaching a partial MSF allocation. Its
            // owner remains in this already-attached array; re-count its actual
            // requested subtotal rather than losing the exceptional prefix.
            compilerArtifactDynamicBytes_=0;
            if(compilerArtifacts_)for(SIZE_T i=0;i<compilerArtifactCount_;++i){
                const SIZE_T actual=compilerArtifacts_[i].ReservedSourceBytes();
                const SIZE_T dynamic=actual>=sizeof(OriginalHeldMsfSource)?actual-sizeof(OriginalHeldMsfSource):0;
                if(dynamic>SIZE_MAX-compilerArtifactDynamicBytes_){compilerArtifactDynamicBytes_=SIZE_MAX;break;}
                compilerArtifactDynamicBytes_+=dynamic;
            }
            return false;
        }
    }
    const std::exception_ptr& PreownExceptionOriginal() const noexcept{return preownExceptionOriginal_;}
    OriginalParserRuntimeAssociation(const OriginalParserRuntimeAssociation&)=delete;
    OriginalParserRuntimeAssociation& operator=(const OriginalParserRuntimeAssociation&)=delete;
    static HelperHostOverrideSource ObserveHelperHostOverrideSource(const OriginalHelperSourceProjection& helper) noexcept {
        HelperHostOverrideSource r;r.attempted=true;r.helperOriginal=&helper;r.commandOriginal=helper.commandOriginal;
        if(!helper.complete||!helper.commandOriginal||!helper.commandOriginal->captured)return r;
        // The exact completed helper projection requires first argument -NoLogo.
        // In selected apphost command_line.cpp parse_known_args, every known
        // host option has two hyphens, so this first word stops host parsing.
        // Later -File/-Command contents cannot re-enter that stopped loop.
        // Source args_include_running_executable selects argoff0/1. This
        // conditional case uses included argv0, not host-mode label as proof.
        r.sourceAssumesRunningExecutableIncluded=true;
        r.firstUnrecognizedArgument=1;r.conditionalApphostEmptyOptions=true;
        return r;
    }
    RuntimeEnvironmentSourceSpan ObserveRuntimeEnvironmentSource(const char* key) const noexcept {
        RuntimeEnvironmentSourceSpan r;r.attempted=true;r.sourceKey=key;
        r.commandOriginal=environmentOwnerOriginal_;r.outcome=RuntimeEnvironmentSourceSpan::Outcome::InputUnresolved;
        if(!key||!environmentOwnerOriginal_||!environmentOwnerOriginal_->copied||
            environmentOwnerOriginal_->snapshotOriginal!=snapshotOriginal_)return r;
        const auto& env=environmentOwnerOriginal_->environment;const SIZE_T units=environmentOwnerOriginal_->environmentUnits;
        if(units<2||units>env.size()||env[units-1]||env[units-2])return r;
        SIZE_T entry=0;
        while(entry<units-1&&env[entry]){
            SIZE_T end=entry;while(end<units&&env[end])++end;
            if(end>=units-1)return r;
            SIZE_T separator=entry;while(separator<end&&env[separator]!=u'=')++separator;
            if(separator==end)return r;
            bool matches=true,unknown=false;SIZE_T i=0;
            for(SIZE_T n=entry;n<separator;++n){
                const auto scalar=static_cast<unsigned>(env[n]);
                if(scalar>127){unknown=true;break;}
                if(!key[i]){matches=false;break;}
                const auto folded=scalar>='a'&&scalar<='z'?scalar-('a'-'A'):scalar;
                if(folded!=static_cast<unsigned char>(key[i++])){matches=false;break;}
            }
            if(unknown)r.keyComparisonUnresolved=true;
            else if(matches&&!key[i]){
                ++r.matchingKeys;
                if(r.matchingKeys==1){r.valueBegin=separator+1;r.valueUnits=end-separator-1;}
            }
            entry=end+1;
        }
        if((units==2?entry!=0:entry!=units-1)||r.keyComparisonUnresolved||r.matchingKeys>1)return r;
        using Outcome=RuntimeEnvironmentSourceSpan::Outcome;
        r.outcome=!r.matchingKeys?Outcome::SourceAbsent:r.valueUnits?Outcome::SourceNonempty:Outcome::SourceEmpty;
        // Original command spans only: child ENV/native getenv/xtoi are not
        // observed by this conditional source projection.
        return r;
    }
    static RuntimeEnvironmentIntegerSource ObserveRuntimeEnvironmentIntegerSource(
        const RuntimeEnvironmentSourceSpan& original) noexcept {
        RuntimeEnvironmentIntegerSource r;r.attempted=true;r.span=original;
        using Outcome=RuntimeEnvironmentSourceSpan::Outcome;
        if(original.outcome!=Outcome::SourceNonempty||!original.commandOriginal)return r;
        const auto& command=*original.commandOriginal;
        if(!command.copied||original.valueUnits>command.environmentUnits||
            original.valueBegin>command.environmentUnits-original.valueUnits||
            command.environmentUnits>command.environment.size())return r;
        const auto* text=command.environment.data()+original.valueBegin;const SIZE_T count=original.valueUnits;
        SIZE_T i=0;
        while(i<count&&(text[i]==u' '||text[i]==u'\t'))++i;
        if(i<count&&(text[i]>127||text[i]==u'\r'||text[i]==u'\n'||text[i]==u'\v'||text[i]==u'\f')){
            r.localeUnresolved=true;r.consumedUnits=i;return r;}
        bool negative=false;
        if(i<count&&(text[i]==u'+'||text[i]==u'-')){negative=text[i]==u'-';++i;}
        const std::uint32_t limit=negative?2147483648u:2147483647u;
        std::uint32_t magnitude=0;
        while(i<count&&text[i]>=u'0'&&text[i]<=u'9'){
            const auto digit=static_cast<std::uint32_t>(text[i]-u'0');
            if(magnitude>(limit-digit)/10)r.sourceOverflow=true;
            if(!r.sourceOverflow)magnitude=magnitude*10+digit;++i;
        }
        r.consumedUnits=i;
        if(i<count&&text[i]>127){r.localeUnresolved=true;return r;}
        r.value=r.sourceOverflow?(negative?INT32_MIN:INT32_MAX):
            negative?(magnitude==2147483648u?INT32_MIN:-static_cast<std::int32_t>(magnitude)):
                static_cast<std::int32_t>(magnitude);
        r.sourceValueKnown=true;return r;
    }
    static FrameworkEnvironmentSourceConsumer ObserveFrameworkEnvironmentSource(
        const OriginalRuntimeHostConfiguration::FrameworkSettingsSource& framework,
        const RuntimeEnvironmentSourceSpan& roll,bool prereleaseChoiceKnown,bool rollToPrerelease) noexcept {
        FrameworkEnvironmentSourceConsumer r;r.attempted=true;r.frameworkOriginal=&framework;r.rollOriginal=&roll;
        using Outcome=FrameworkEnvironmentSourceConsumer::Outcome;
        r.rollForwardSource=framework.effectiveRollForward;
        if(framework.nameVersion.outcome==OriginalFrameworkNameVersionSource::Outcome::SourceReturnedTrue){
            r.preferReleaseReached=true;
            const bool parsedPre=framework.nameVersion.versionSource.outcome==OriginalFrameworkVersionSource::Outcome::SourceParsed&&
                framework.nameVersion.versionSource.prerelease;
            if(parsedPre){r.preferReleaseKnown=true;}
            else if(prereleaseChoiceKnown){r.preferReleaseKnown=true;r.preferRelease=!rollToPrerelease;}
        }
        // Prefer-release precedes framework-specific settings. Its observation
        // survives a later settings failure, but that failure skips roll ENV.
        if(framework.outcome!=OriginalRuntimeHostConfiguration::FrameworkSettingsSource::Outcome::EnvironmentAndOverridesPending){
            r.outcome=Outcome::EarlierStageIncomplete;return r;}
        if(!r.preferReleaseKnown){r.outcome=Outcome::PrereleaseChoiceUnresolved;return r;}
        using Env=RuntimeEnvironmentSourceSpan::Outcome;
        if(roll.outcome==Env::SourceAbsent||roll.outcome==Env::SourceEmpty){r.outcome=Outcome::CommandOverridesPending;return r;}
        if(roll.outcome!=Env::SourceNonempty||!roll.commandOriginal){r.outcome=Outcome::EnvironmentInputUnresolved;return r;}
        const auto& command=*roll.commandOriginal;
        if(!command.copied||roll.valueUnits>command.environmentUnits||
            roll.valueBegin>command.environmentUnits-roll.valueUnits||command.environmentUnits>command.environment.size()){
            r.outcome=Outcome::EnvironmentInputUnresolved;return r;}
        const char* labels[]={"Disable","LatestPatch","Minor","LatestMinor","Major","LatestMajor"};
        for(int candidate=0;candidate<6;++candidate){
            SIZE_T n=0;while(labels[candidate][n])++n;
            if(n!=roll.valueUnits)continue;
            bool same=true;for(SIZE_T i=0;i<n;++i)
                if(command.environment[roll.valueBegin+i]!=static_cast<unsigned char>(labels[candidate][i])){same=false;break;}
            if(same){r.rollForwardSource=candidate;r.rollAssigned=true;r.outcome=Outcome::CommandOverridesPending;return r;}
        }
        // Exact canonical source case only; _wcsicmp/actual CRT locale and
        // unsupported spelling are not fabricated as native invalid returns.
        r.outcome=Outcome::RollComparisonUnresolved;return r;
    }
    StartupHooksSourcePlan ObserveStartupHooksSource(
        const OriginalRuntimeHostConfiguration::ConfiguredProperty& config) const noexcept {
        StartupHooksSourcePlan result;result.attempted=true;result.appConfigOriginal=config;
        result.environmentOwnerOriginal=environmentOwnerOriginal_;
        result.outcome=StartupHooksSourcePlan::Outcome::InputUnresolved;
        if(!environmentOwnerOriginal_||!environmentOwnerOriginal_->copied||
            environmentOwnerOriginal_->snapshotOriginal!=snapshotOriginal_)return result;
        const auto& original=*environmentOwnerOriginal_;
        const auto& env=original.environment;const SIZE_T units=original.environmentUnits;
        if(units<2||units>env.size()||env[units-1]||env[units-2])return result;
        const char* target="DOTNET_STARTUP_HOOKS";
        SIZE_T entry=0;
        while(entry<units-1&&env[entry]){
            SIZE_T end=entry;while(end<units&&env[end])++end;
            if(end>=units-1)return result;
            SIZE_T separator=entry;while(separator<end&&env[separator]!=u'=')++separator;
            if(separator==end)return result;
            bool matches=true,unknown=false;SIZE_T i=0;
            for(SIZE_T key=entry;key<separator;++key){
                const auto scalar=static_cast<std::uint32_t>(env[key]);
                if(scalar>127){unknown=true;break;}
                if(!target[i]){matches=false;break;}
                const auto folded=scalar>='a'&&scalar<='z'?scalar-('a'-'A'):scalar;
                if(folded!=static_cast<unsigned char>(target[i++])){matches=false;break;}
            }
            if(unknown)result.environmentKeyComparisonUnresolved=true;
            else if(matches&&!target[i]){
                ++result.matchingEnvironmentKeys;
                if(result.matchingEnvironmentKeys==1){
                    result.environmentValueBegin=separator+1;result.environmentValueUnits=end-separator-1;
                }
            }
            entry=end+1;
        }
        if((units==2?entry!=0:entry!=units-1)||result.environmentKeyComparisonUnresolved||
            result.matchingEnvironmentKeys>1)return result;
        using Config=OriginalRuntimeHostConfiguration::ConfiguredProperty::Outcome;
        const bool hasConfig=config.outcome==Config::String||config.outcome==Config::JsonLiteral;
        if(config.outcome!=Config::Absent&&!hasConfig)return result;
        if(hasConfig&&!config.jsonOriginal)return result;
        if(config.outcome==Config::JsonLiteral&&config.literalKind!=OriginalJson::Kind::True&&
            config.literalKind!=OriginalJson::Kind::False&&config.literalKind!=OriginalJson::Kind::Null)return result;
        const bool hasEnv=result.matchingEnvironmentKeys==1&&result.environmentValueUnits;
        using Outcome=StartupHooksSourcePlan::Outcome;
        result.outcome=hasEnv?(hasConfig?Outcome::SourceEnvThenConfig:Outcome::SourceEnvOnly):
            (hasConfig?Outcome::SourceConfigOnly:Outcome::SourceNeither);
        // Conditional source composition: actual inherited child ENV and the
        // two native getenv returns remain separate from these original spans.
        return result;
    }
    bool RetainSourceRelationship(PyObject* snapshot,PyObject* originalToolRole,const NativeHeldFiles& files,
        SIZE_T interpreter,SIZE_T buildRecord,SIZE_T sourceContract) noexcept {
        if(source_.attempted)return false;source_.attempted=true;
        source_.inputSnapshotOriginal=snapshot;source_.toolRoleOriginal=originalToolRole;source_.filesOriginal=&files;
        source_.interpreterOrdinal=interpreter;source_.buildRecordOrdinal=buildRecord;source_.sourceContractOrdinal=sourceContract;
        // Same original native/Python thread is an actual SessionCore caller
        // precondition. Own references before classification; a rejected supplied
        // relation remains resident with the other original failure inputs.
        if(snapshot){Py_INCREF(snapshot);source_.snapshotReferenceOwned=true;}
        if(originalToolRole){Py_INCREF(originalToolRole);source_.toolRoleReferenceOwned=true;}
        if(!snapshot || snapshot!=snapshotOriginal_ || &files!=&nativeInputsOriginal_ || !originalToolRole || files.Failed() || interpreter>=files.FileCount() ||
            buildRecord>=files.FileCount() || sourceContract>=files.FileCount())return false;
        const auto* originals=files.PrivateFiles();
        for(const SIZE_T ordinal:{interpreter,buildRecord,sourceContract})
            if(!originals[ordinal].valid || originals[ordinal].closeAttempted || originals[ordinal].readCompletionUnknown)return false;
        if(!files.ExactCapturedSourceSpan(buildRecord,source_.buildBytesOriginal,source_.buildExtent) ||
            !files.ExactCapturedSourceSpan(sourceContract,source_.sourceBytesOriginal,source_.sourceExtent))return false;
        source_.retainedMembershipObserved=true;
        return true; // Original retention only. No authenticity/semantic grant.
    }
private:
    bool ObserveCoreClrPrivatePropertySources(PyObject* requirements) noexcept {
        auto& observed=coreClrPropertySource_;
        if(observed.attempted)return observed.exactSourceObserved;
        observed.attempted=true;observed.filesOriginal=&nativeInputsOriginal_;
        constexpr SIZE_T sizes[]={7593,16338,13753,2638,38632,17299,35368};
        constexpr unsigned char digests[7][32]={
            {0x80,0x0e,0x19,0xe1,0x0f,0x0e,0x7b,0xeb,0x1a,0xef,0x87,0xc6,0xe5,0xf0,0xae,0x71,0x26,0x65,0x28,0x65,0xb4,0xc1,0xdc,0xed,0xe7,0x69,0xab,0xa8,0xcb,0x33,0x27,0x1b},
            {0x15,0x71,0x89,0x09,0x3a,0xf4,0xb9,0xa9,0x93,0x97,0xe4,0xb3,0xfe,0x4c,0xfd,0x37,0x7f,0xce,0xec,0x67,0x75,0x66,0x64,0x31,0x77,0xf0,0x8e,0x27,0xeb,0x0d,0x75,0xdb},
            {0xad,0x4f,0x32,0xc4,0x67,0x68,0xd6,0xc1,0xcd,0xc7,0x50,0xc4,0x8a,0xdd,0xe5,0xd1,0x10,0x67,0xa4,0x16,0x8b,0x04,0x73,0xed,0x42,0x6c,0xf2,0x42,0x1a,0xd7,0xf7,0x4b},
            {0xaa,0x1e,0xcb,0x63,0x2f,0x1c,0x2e,0xfa,0x9f,0x70,0x48,0x82,0xde,0x58,0xe9,0xd2,0xdf,0xcc,0xf9,0xd1,0x3a,0xe5,0x0f,0x10,0xaa,0xba,0xbb,0xa1,0x5f,0x4d,0x7b,0xa8},
            {0x75,0xdf,0xd5,0xff,0xee,0x58,0x63,0x53,0x80,0x96,0x63,0xab,0x5d,0x48,0xa2,0x70,0x7b,0x2f,0xfd,0x0d,0x9b,0xf4,0x99,0xac,0x2a,0xc6,0xd9,0x44,0x9e,0x5a,0x05,0xf8},
            {0xb8,0x60,0xe3,0x30,0x5a,0xce,0xf3,0x96,0xca,0xf7,0x0d,0x01,0x01,0x36,0x14,0x22,0xd7,0xed,0x84,0x5a,0xd0,0x37,0xe7,0x93,0xea,0x2a,0xfe,0x47,0xde,0x8d,0xf1,0x98},
            {0x49,0xf9,0xc0,0x3a,0x5f,0x43,0x38,0xa4,0xd5,0x6d,0xd8,0xff,0xe0,0x04,0x27,0x34,0x70,0x15,0xb5,0x0f,0xae,0x64,0xdc,0x08,0xff,0x2b,0x6f,0xa1,0xeb,0x38,0x8a,0x7d}
        };
        if(!requirements || !PyTuple_CheckExact(requirements) ||
            PyTuple_GET_SIZE(requirements)!=static_cast<Py_ssize_t>(nativeInputsOriginal_.FileCount()))return false;
        for(SIZE_T i=0;i<nativeInputsOriginal_.FileCount();++i) {
            const auto& file=nativeInputsOriginal_.PrivateFiles()[i];
            for(SIZE_T source=0;source<observed.ordinals.size();++source) {
                if(memcmp(file.actualSha256.data(),digests[source],32))continue;
                if(observed.ordinals[source]!=SIZE_MAX){observed.ambiguous=true;return false;}
                observed.ordinals[source]=i;
                auto* requirement=PyTuple_GET_ITEM(requirements,static_cast<Py_ssize_t>(i));
                if(!requirement || !PyTuple_CheckExact(requirement) || PyTuple_GET_SIZE(requirement)!=4)return false;
                auto* role=PyTuple_GET_ITEM(requirement,0);
                if(!role || !PyUnicode_CheckExact(role) || PyUnicode_CompareWithASCIIString(role,"compiler-input")!=0 ||
                    !file.valid || file.closeAttempted || file.readCompletionUnknown ||
                    !nativeInputsOriginal_.ExactCapturedSourceSpan(i,observed.bodies[source],observed.extents[source]) ||
                    observed.extents[source]!=sizes[source])return false;
            }
        }
        for(const auto ordinal:observed.ordinals)if(ordinal==SIZE_MAX)return false;
        if(observed.ambiguous)return false;
        observed.exactSourceObserved=true;return true; // Source semantics, never compiled/native authority.
    }
public:
    bool BindAvailableSourceRelationship() noexcept {
        // Reuse exact already-retained Requirement rows. These roles establish
        // only source-owned references/membership, never signed/native authority.
        if(source_.attempted||!snapshotOriginal_||!PyTuple_Check(snapshotOriginal_)||PyTuple_GET_SIZE(snapshotOriginal_)!=5||
            vendorPackageOrdinal_==SIZE_MAX||!supportedSource_.fullSourceCorrespondence)return false;
        auto* requirements=PyTuple_GET_ITEM(snapshotOriginal_,4);
        if(!requirements||!PyTuple_CheckExact(requirements)||
            PyTuple_GET_SIZE(requirements)!=static_cast<Py_ssize_t>(nativeInputsOriginal_.FileCount()))return false;
        ObserveCoreClrPrivatePropertySources(requirements);
        const auto role=[&](SIZE_T ordinal,const char* literal) noexcept {
            if(ordinal>=nativeInputsOriginal_.FileCount())return false;
            auto* row=PyTuple_GET_ITEM(requirements,static_cast<Py_ssize_t>(ordinal));
            if(!row||!PyTuple_Check(row)||PyTuple_GET_SIZE(row)!=4)return false;
            auto* value=PyTuple_GET_ITEM(row,0);
            return value&&PyUnicode_CheckExact(value)&&PyUnicode_CompareWithASCIIString(value,literal)==0;
        };
        if(!role(vendorPackageOrdinal_,"compiler-input"))return false;
        for(const auto ordinal:supportedSource_.ordinals)if(!role(ordinal,"compiler-input"))return false;
        SIZE_T interpreter=SIZE_MAX;
        for(SIZE_T i=0;i<nativeInputsOriginal_.FileCount();++i){
            if(!BaseName(nativeInputsOriginal_.PrivateFiles()[i],L"pwsh.exe")||!role(i,"interpreter"))continue;
            if(interpreter!=SIZE_MAX)return false;interpreter=i;
        }
        if(interpreter==SIZE_MAX)return false;
        return RetainSourceRelationship(snapshotOriginal_,PyTuple_GET_ITEM(requirements,static_cast<Py_ssize_t>(interpreter)),
            nativeInputsOriginal_,interpreter,vendorPackageOrdinal_,supportedSource_.ordinals[0]);
    }
    const CoreClrPrivatePropertySourceCase& PrivateCoreClrPropertySources() const noexcept {
        return coreClrPropertySource_;
    }
    const OriginalHeldMsfSource* PrivateCompilerArtifacts() const noexcept{return compilerArtifacts_.get();}
    SIZE_T CompilerArtifactCount() const noexcept{return compilerArtifactCount_;}
    const OriginalHeldSourceDigest* PrivateCompilerSourceDigests() const noexcept{return compilerSourceDigests_.get();}
    SIZE_T RetainedArrayCapacityOriginal() const noexcept{return capacity_;}
    enum class CoreClrObservationAsset {Unrelated,Unresolved,Matched};
    CoreClrObservationAsset OriginalCoreClrObservationAsset(const State& state,const Event& event,
            SIZE_T selectedOrdinal,const LoadedImageObservation& loaded) const noexcept {
        if(selectedOrdinal>=nativeInputsOriginal_.FileCount())return CoreClrObservationAsset::Unresolved;
        if(!BaseName(nativeInputsOriginal_.PrivateFiles()[selectedOrdinal],L"coreclr.dll"))
            return CoreClrObservationAsset::Unrelated;
        SIZE_T occurrences=0,comparisons=0;
        for(SIZE_T i=0;i<count_;++i){const auto& row=occurrences_[i];
            if(row.stateOriginal==&state&&row.eventOriginal==&event&&row.selectedOrdinal==selectedOrdinal&&
               row.loadedOriginal==&loaded&&row.membershipObserved)++occurrences;
        }
        if(occurrences!=1||vendorPackageOrdinal_==SIZE_MAX||!vendorFramingCaptured_||!vendorComparisonAttempted_)
            return CoreClrObservationAsset::Unresolved;
        for(SIZE_T i=0;i<vendorComparisonCount_;++i)
            if(vendorComparisons_[i]&&vendorComparisons_[i]->OriginalAssetOrdinal()==selectedOrdinal&&
               vendorComparisons_[i]->OriginalPackageOrdinal()==vendorPackageOrdinal_&&
               vendorComparisons_[i]->BytesCorrespond())++comparisons;
        return comparisons==1?CoreClrObservationAsset::Matched:CoreClrObservationAsset::Unresolved;
        // Exact observation asset only, not native build/ABI/provider authority.
    }
    bool ObserveAsset(const State& state,const Event& event,SIZE_T memberOrdinal,SIZE_T selectedOrdinal,
        const FileIdentityObservation& file,const LoadedImageObservation& loaded) noexcept {
        if(count_==capacity_)return false;
        auto& row=occurrences_[count_++];row.attempted=true;
        row.stateOriginal=&state;row.eventOriginal=&event;row.fileOriginal=&file;row.loadedOriginal=&loaded;
        row.selectedOrdinal=selectedOrdinal;row.memberOrdinal=memberOrdinal;
        if(!state.eventCount || &event!=&state.events[state.eventCount-1] ||
            (event.original.dwDebugEventCode!=CREATE_PROCESS_DEBUG_EVENT && event.original.dwDebugEventCode!=LOAD_DLL_DEBUG_EVENT) ||
            memberOrdinal>=state.memberCount || selectedOrdinal>=nativeInputsOriginal_.FileCount())return false;
        const auto& member=state.members[memberOrdinal];row.processOrdinal=member.process;row.birthOriginal=member.birth;
        if(member.pid!=event.original.dwProcessId || member.process>=state.resourceCount || !positive(member.birth))return false;
        const auto& process=state.resources[member.process];
        const auto& selected=nativeInputsOriginal_.PrivateFiles()[selectedOrdinal];
        if(process.kind!=Kind::Process || !process.acquired || process.unknownOutput || process.attempted ||
            !selected.valid || selected.closeAttempted || !SameFile(file,selected.first) || !loaded.complete)return false;
        row.membershipObserved=true;return true;
    }
    const ConsumerContractResult* ObserveConsumerContract(const State& state,const Event& create,
        const Event& exit,const OriginalHelperSourceProjection& helper,
        const OriginalCoreClrEntryProperties* initializer) noexcept {
        // Every admitted consumer has a CREATE image row, hence the existing
        // image capacity bounds this separate once-per-consumer EXIT roster.
        for(SIZE_T i=0;i<consumerResultCount_;++i)
            if(consumerResults_[i].stateOriginal==&state&&consumerResults_[i].createOriginal==&create)
                return consumerResults_[i].exitOriginal==&exit&&consumerResults_[i].helperCallerOriginal==&helper&&
                    consumerResults_[i].initializerPropertiesOriginal==initializer?
                    &consumerResults_[i]:nullptr;
        if(consumerResultCount_==capacity_)return nullptr;
        auto& result=consumerResults_[consumerResultCount_++];result.attempted=true;
        result.stateOriginal=&state;result.createOriginal=&create;result.exitOriginal=&exit;
        result.associationOriginal=this;result.cause=ConsumerContractResult::Cause::InvalidOccurrence;
        result.helperCallerOriginal=&helper;result.helperCommandOriginal=helper.commandOriginal;
        result.initializerPropertiesOriginal=initializer;
        if(!state.eventCount||&exit!=&state.events[state.eventCount-1]||
            exit.original.dwDebugEventCode!=EXIT_PROCESS_DEBUG_EVENT||
            create.original.dwDebugEventCode!=CREATE_PROCESS_DEBUG_EVENT||
            create.createdMemberOrdinal!=exit.exitingMemberOrdinal||
            create.createdProcessResource!=exit.exitingProcessResource||
            exit.exitingMemberOrdinal>=state.memberCount || exit.exitingProcessResource>=state.resourceCount)return &result;
        result.memberOrdinal=exit.exitingMemberOrdinal;
        const auto& member=state.members[result.memberOrdinal];
        if(!helper.complete||!helper.commandOriginal||!helper.commandOriginal->captured||
            helper.commandOriginal->stateOriginal!=&state||helper.commandOriginal->eventOriginal!=&create||
            helper.commandOriginal->memberOrdinal!=result.memberOrdinal||
            helper.commandOriginal->processResource!=exit.exitingProcessResource||
            !same_time(helper.commandOriginal->processBirth,member.birth))return &result;
        if(member.originalCreateEvent>=state.eventCount-1||
            &state.events[member.originalCreateEvent]!=&create||member.pid!=exit.original.dwProcessId)
            return &result;
        if(!initializer){result.cause=ConsumerContractResult::Cause::InitializerOccurrenceMissing;return &result;}
        result.cause=ConsumerContractResult::Cause::InitializerOccurrenceInvalid;
        const auto* entryInputs=initializer->inputsOriginal;
        if(!initializer->representationObserved || !entryInputs || !entryInputs->captured || !entryInputs->revalidated ||
            entryInputs->failed || entryInputs->stateOriginal!=&state || !entryInputs->stoppedOriginal ||
            !entryInputs->contextOriginal || !entryInputs->contextOriginal->captured ||
            entryInputs->contextOriginal->stateOriginal!=&state ||
            entryInputs->contextOriginal->stoppedOriginal!=entryInputs->stoppedOriginal ||
            !entryInputs->contextOriginal->threadCreateOriginal ||
            entryInputs->processOriginal!=state.resources[exit.exitingProcessResource].original ||
            !initializer->hostContractOriginal.representationObserved ||
            initializer->hostContractOriginal.inputsOriginal!=entryInputs || !entryInputs->commandOriginal ||
            !entryInputs->pendingOriginal || entryInputs->commandOriginal!=helper.commandOriginal->commandOriginal ||
            entryInputs->pendingOriginal!=helper.commandOriginal->pendingOwnerOriginal)return &result;
        result.initializerEntryOriginal=entryInputs->stoppedOriginal;
        result.initializerThreadBirthOriginal=entryInputs->contextOriginal->threadCreateOriginal;
        for(SIZE_T i=member.originalCreateEvent+1;i<state.eventCount-1;++i)
            if(&state.events[i]==result.initializerEntryOriginal){result.initializerEntryOrdinal=i;break;}
        if(result.initializerEntryOrdinal==SIZE_MAX)return &result;
        for(SIZE_T i=0;i<result.initializerEntryOrdinal;++i)
            if(&state.events[i]==result.initializerThreadBirthOriginal){result.initializerThreadBirthOrdinal=i;break;}
        if(result.initializerThreadBirthOrdinal==SIZE_MAX ||
            result.initializerThreadBirthOriginal->createdThreadMemberOrdinal!=result.memberOrdinal ||
            result.initializerThreadBirthOriginal->createdThreadProcessResource!=exit.exitingProcessResource ||
            result.initializerEntryOriginal->original.dwProcessId!=exit.original.dwProcessId ||
            result.initializerEntryOriginal->original.dwDebugEventCode!=EXCEPTION_DEBUG_EVENT ||
            result.initializerEntryOriginal->original.u.Exception.dwFirstChance!=1 ||
            result.initializerEntryOriginal->original.u.Exception.ExceptionRecord.ExceptionCode!=EXCEPTION_SINGLE_STEP ||
            result.initializerEntryOriginal->original.dwThreadId!=result.initializerThreadBirthOriginal->original.dwThreadId ||
            result.initializerEntryOriginal->requestedDecision!=SavedEventDecision::ConsumeOwnedObserverException ||
            !result.initializerEntryOriginal->decisionAttempted || !result.initializerEntryOriginal->decisionReturned ||
            !result.initializerEntryOriginal->continued || !result.initializerEntryOriginal->continuationResult ||
            result.initializerEntryOriginal->disposition!=DBG_CONTINUE)return &result;
        result.initializerUsefulContinuationObserved=true;
        constexpr SIZE_T providerRoles[]={0,4,5}; // TPA, app deps, framework deps.
        for(SIZE_T role=0;role<3;++role){
            auto& paths=result.providerFilePathCandidates[role];paths.attempted=true;
            paths.initializerOriginal=initializer;paths.filesOriginal=&nativeInputsOriginal_;
            const SIZE_T commonRole=providerRoles[role];
            paths.propertyIndex=initializer->firstCommonIndex[commonRole];
            if(initializer->commonMatches[commonRole]!=1 || paths.propertyIndex>=
                static_cast<SIZE_T>(entryInputs->propertyCount))continue;
            paths.propertyUnique=true;
            const auto& value=entryInputs->PrivateStrings()[2+static_cast<SIZE_T>(entryInputs->propertyCount)+paths.propertyIndex];
            const auto* bytes=entryInputs->PrivateStringBytes()+value.offset;
            const SIZE_T extent=value.extent-1; // Representation validated first NUL.
            if(!extent){paths.emptyValue=true;paths.complete=true;continue;}
            SIZE_T begin=0;
            for(SIZE_T end=0;end<=extent;++end){
                if(end!=extent && bytes[end]!=';')continue;
                const SIZE_T length=end-begin;++paths.segments;
                if(!length)++paths.emptySegments;
                else {
                    SIZE_T matches=0;bool invalid=false;
                    for(SIZE_T file=0;file<nativeInputsOriginal_.FileCount();++file){
                        const auto comparison=ExactHeldPathUtf8(nativeInputsOriginal_.PrivateFiles()[file],bytes+begin,length);
                        if(comparison==HeldPathComparison::Equal)++matches;
                        else if(comparison==HeldPathComparison::Invalid)invalid=true;
                    }
                    if(!invalid && matches==1)++paths.unique;
                    else {
                        if(invalid)++paths.invalid;else if(!matches)++paths.missing;else ++paths.ambiguous;
                        if(paths.firstUnresolvedOffset==SIZE_MAX){paths.firstUnresolvedOffset=value.offset+begin;paths.firstUnresolvedExtent=length;}
                    }
                }
                if(end==extent){paths.complete=true;break;}
                begin=end+1;
            }
        }
        // Exact path membership is a candidate relation only: no normalization,
        // probe choice, runtime load or identity continuity is inferred here.
        const wchar_t* names[]={L"pwsh.exe",L"hostfxr.dll",L"hostpolicy.dll",L"coreclr.dll"};
        for(SIZE_T role=0;role<4;++role){
            for(SIZE_T i=0;i<count_;++i){const auto& row=occurrences_[i];
                if(!row.membershipObserved||row.stateOriginal!=&state||row.memberOrdinal!=result.memberOrdinal||
                    row.processOrdinal!=exit.exitingProcessResource||!same_time(row.birthOriginal,member.birth)||
                    row.selectedOrdinal>=nativeInputsOriginal_.FileCount()||
                    !BaseName(nativeInputsOriginal_.PrivateFiles()[row.selectedOrdinal],names[role]))continue;
                if(result.occurrenceOrdinals[role]!=SIZE_MAX){
                    result.cause=ConsumerContractResult::Cause::RuntimeOccurrenceAmbiguous;return &result;}
                result.occurrenceOrdinals[role]=i;
            }
        }
        if(!source_.retainedMembershipObserved||!supportedSource_.fullSourceCorrespondence){
            result.cause=ConsumerContractResult::Cause::SourceRelationshipMissing;return &result;}
        if(!hostConfigurationCorrespondence_||!hostConfiguration_->LibraryAssetJoinComplete()){
            result.cause=ConsumerContractResult::Cause::HostConfigurationMissing;return &result;}
        result.configurationOrdinals=hostConfigurationOrdinals_;
        for(const auto ordinal:result.occurrenceOrdinals)if(ordinal==SIZE_MAX){
            result.cause=ConsumerContractResult::Cause::RuntimeOccurrenceMissing;return &result;}
        if(occurrences_[result.occurrenceOrdinals[0]].selectedOrdinal!=source_.interpreterOrdinal){
            result.cause=ConsumerContractResult::Cause::SourceRelationshipMissing;return &result;}
        for(SIZE_T role=0;role<4;++role){
            const auto asset=occurrences_[result.occurrenceOrdinals[role]].selectedOrdinal;
            for(SIZE_T i=0;i<vendorComparisonCount_;++i)
                if(vendorComparisons_[i]->OriginalAssetOrdinal()==asset&&
                    vendorComparisons_[i]->OriginalPackageOrdinal()==vendorPackageOrdinal_&&
                    vendorComparisons_[i]->BytesCorrespond())result.vendorComparisonOrdinals[role]=i;
            if(result.vendorComparisonOrdinals[role]==SIZE_MAX){
                result.cause=ConsumerContractResult::Cause::VendorCorrespondenceMissing;return &result;}
        }
        for(SIZE_T document=0;document<2;++document){
            for(SIZE_T i=0;i<vendorComparisonCount_;++i)
                if(vendorComparisons_[i]->OriginalAssetOrdinal()==result.configurationOrdinals[document]&&
                    vendorComparisons_[i]->OriginalPackageOrdinal()==vendorPackageOrdinal_&&
                    vendorComparisons_[i]->BytesCorrespond())result.configurationVendorOrdinals[document]=i;
            if(result.configurationVendorOrdinals[document]==SIZE_MAX){
                result.cause=ConsumerContractResult::Cause::VendorCorrespondenceMissing;return &result;}
        }
        // Join conditional app dev/main source decisions to THIS same consumer;
        // actual additional/framework properties, map enumeration and ENV are
        // not supplied by these app-source observations.
        result.appConfigurationOriginal=hostConfiguration_.get();
        auto& appProperties=result.appPropertyCorrespondence;
        appProperties.attempted=true;appProperties.initializerOriginal=initializer;
        appProperties.configurationOriginal=hostConfiguration_.get();
        hostConfiguration_->BeginPropertyCursor(appProperties.cursorOriginal);
        while(hostConfiguration_->NextPropertyOriginal(appProperties.cursorOriginal)){
            const auto& source=appProperties.cursorOriginal.last;
            if(appProperties.visited==SIZE_MAX)break;
            ++appProperties.visited;
            SIZE_T matches=0,index=SIZE_MAX;bool keyUnresolved=false;
            using Comparison=OriginalRuntimeHostConfiguration::PropertyTextComparison;
            const auto* spans=entryInputs->PrivateStrings();
            const auto* bytes=entryInputs->PrivateStringBytes();
            const SIZE_T count=static_cast<SIZE_T>(entryInputs->propertyCount);
            for(SIZE_T i=0;i<count;++i){
                const auto& key=spans[2+i];
                const auto comparison=source.jsonOriginal?
                    OriginalRuntimeHostConfiguration::CompareOriginalStringUtf8(*source.jsonOriginal,source.key,
                        bytes+key.offset,key.extent):Comparison::Unresolved;
                if(comparison==Comparison::Unresolved)keyUnresolved=true;
                else if(comparison==Comparison::Equal){++matches;index=i;}
            }
            bool unresolved=false;
            if(keyUnresolved){++appProperties.unresolved;unresolved=true;}
            else if(!matches){++appProperties.missing;unresolved=true;}
            else if(matches!=1){++appProperties.ambiguous;unresolved=true;}
            else {
                const auto& value=spans[2+count+index];
                const auto comparison=OriginalRuntimeHostConfiguration::CompareConfiguredPropertyUtf8(source,
                    bytes+value.offset,value.extent);
                if(comparison==Comparison::Equal)++appProperties.matched;
                else if(comparison==Comparison::Different){++appProperties.different;unresolved=true;}
                else {++appProperties.unresolved;unresolved=true;}
            }
            if(unresolved && appProperties.firstUnresolvedSource.outcome==
                OriginalRuntimeHostConfiguration::ConfiguredProperty::Outcome::Unobserved){
                appProperties.firstUnresolvedSource=source;appProperties.firstUnresolvedInitializerIndex=index;
            }
        }
        appProperties.traversalComplete=appProperties.cursorOriginal.outcome==
            OriginalRuntimeHostConfiguration::PropertyCursor::Outcome::Complete;
        // This records app source correspondence only. Framework/additional
        // properties, reserved substitutions and actual host writers remain
        // independent provider obligations, even when every app row matches.
        result.appRidGraphSource=hostConfiguration_->PrivateRidGraphSourceDecision();
        result.appSetAppPathsSource=hostConfiguration_->PrivateSetAppPathsSourceDecision();
        result.appStartupHooksSource=ObserveStartupHooksSource(hostConfiguration_->PrivateConfiguredProperties()[2]);
        result.helperHostOverrideSource=ObserveHelperHostOverrideSource(helper);
        result.runtimeRollEnvironmentSources={
            ObserveRuntimeEnvironmentSource("DOTNET_ROLL_FORWARD_ON_NO_CANDIDATE_FX"),
            ObserveRuntimeEnvironmentSource("DOTNET_ROLL_FORWARD_TO_PRERELEASE"),
            ObserveRuntimeEnvironmentSource("DOTNET_ROLL_FORWARD")};
        result.runtimeRollIntegerSources={
            ObserveRuntimeEnvironmentIntegerSource(result.runtimeRollEnvironmentSources[0]),
            ObserveRuntimeEnvironmentIntegerSource(result.runtimeRollEnvironmentSources[1])};
        using EnvOutcome=RuntimeEnvironmentSourceSpan::Outcome;
        const auto& legacySpan=result.runtimeRollEnvironmentSources[0];
        const auto& legacy=result.runtimeRollIntegerSources[0];
        if(legacySpan.outcome==EnvOutcome::SourceAbsent||legacySpan.outcome==EnvOutcome::SourceEmpty)
            result.sourceInitialDefaultRoll=2; // Minor before actual dev/main settings.
        else if(legacy.sourceValueKnown&&legacy.value>=0&&legacy.value<=2){
            const int mapping[]={1,2,4};result.sourceInitialDefaultRoll=mapping[legacy.value];
        }
        const auto& preSpan=result.runtimeRollEnvironmentSources[1];
        const auto& pre=result.runtimeRollIntegerSources[1];
        if(preSpan.outcome==EnvOutcome::SourceAbsent||preSpan.outcome==EnvOutcome::SourceEmpty)
            result.sourcePrereleaseChoiceKnown=true;
        else if(pre.sourceValueKnown){result.sourcePrereleaseChoiceKnown=true;result.sourceRollToPrerelease=pre.value==1;}
        if(result.sourceInitialDefaultRoll>=0){
            using Host=OriginalRuntimeHostConfiguration;using Ordered=Host::OrderedOptionSource;
            result.runtimeOrderedSourceReplayAttempted=true;
            auto& seed=result.runtimeDefaultSourceSeed;seed.attempted=true;
            seed.rollForwardSource=result.sourceInitialDefaultRoll;seed.hasDefaultApplySource=true;
            seed.applyPatchesSource=true;
            const auto* dev=hostConfiguration_->PrivateDevConfigSyntax();
            const auto& devOriginal=hostConfiguration_->PrivateDevConfigOriginal();
            unsigned mask=0;bool dependent=false,devFlowKnown=false;
            auto& first=result.runtimeOrderedSourceReplay[0];
            if(dev&&devOriginal.spanObserved){
                if(!devOriginal.syntaxObserved){
                    first=seed;first.jsonOriginal=dev;first.optionsOriginal=hostConfiguration_->PrivateDevOptionStage();
                    first.outcome=Ordered::Outcome::NoOp;devFlowKnown=true;
                }else {
                    first=Host::ObserveOrderedOptionSource(*dev,hostConfiguration_->PrivateDevOptionStage(),
                        true,mask,dependent,&seed);
                    devFlowKnown=first.outcome==Ordered::Outcome::NoOp||first.outcome==Ordered::Outcome::SourceReturnedTrue||
                        first.outcome==Ordered::Outcome::SourceReturnedFalse;
                }
            }
            if(devFlowKnown)result.runtimeOrderedSourceReplay[1]=Host::ObserveOrderedOptionSource(
                hostConfiguration_->PrivateRuntimeConfigSyntax(),hostConfiguration_->PrivateMainOptionStage(),
                false,mask,dependent,&first);
            result.frameworkEnvironmentSource=ObserveFrameworkEnvironmentSource(
                result.runtimeOrderedSourceReplay[1].frameworkSettings,result.runtimeRollEnvironmentSources[2],
                result.sourcePrereleaseChoiceKnown,result.sourceRollToPrerelease);
            if(frameworkListsSource_)frameworkListsSource_->Observe(*hostConfiguration_,seed,
                [&](const Host::FrameworkSettingsSource& settings) noexcept {
                    const auto env=ObserveFrameworkEnvironmentSource(settings,result.runtimeRollEnvironmentSources[2],
                        result.sourcePrereleaseChoiceKnown,result.sourceRollToPrerelease);
                    OriginalFrameworkListSource::Completion completion;
                    completion.originalEnvironment=&result.runtimeRollEnvironmentSources[2];
                    completion.originalHelperOptions=&result.helperHostOverrideSource;
                    completion.environmentOutcome=static_cast<unsigned>(env.outcome);
                    completion.effectiveRoll=env.rollForwardSource;
                    completion.preferReleaseKnown=env.preferReleaseKnown;completion.preferRelease=env.preferRelease;
                    // Conditional source apphost case only. Actual host mode,
                    // argv offset, child ENV and build association remain false.
                    completion.outcome=env.outcome==FrameworkEnvironmentSourceConsumer::Outcome::CommandOverridesPending&&
                        result.helperHostOverrideSource.conditionalApphostEmptyOptions?
                        OriginalFrameworkListSource::Completion::Outcome::ConditionalSourceReturnedTrue:
                        OriginalFrameworkListSource::Completion::Outcome::Unresolved;
                    return completion;
                });
            // Replay is conditional on original source ENV interpretation;
            // it does not observe native getenv or settle final framework overrides.
        }
        result.appOrderedOptionsOriginal=&hostConfiguration_->PrivateOrderedOptionsSource();
        const auto& reserved=hostConfiguration_->PrivateReservedPropertyDecision();
        result.appReservedCollisionCount=reserved.sourceCollisionCount;
        result.appReservedInputUnresolved=reserved.inputUnresolved;
        result.appSourceDecisionsObserved=reserved.attempted;
        result.runtimeRidEnvironmentSource=ObserveRuntimeEnvironmentSource("DOTNET_RUNTIME_ID");
        for(SIZE_T library=0;library<hostConfiguration_->LibraryMetadataOccurrenceCount();++library){
            for(SIZE_T type=0;type<3;++type){
                const auto choice=OriginalPortableRidSource::Observe(*hostConfiguration_,library,type,
                    result.runtimeRidEnvironmentSource);
                // Derived source cursor values are recomputable from the same
                // retained full JSON/ENV originals. This is not a roster of
                // actual native provider selections or getenv outputs.
                result.portableRidLastSource=choice;
                if(result.portableRidSourceChoices==SIZE_MAX)return &result;
                ++result.portableRidSourceChoices;
                using RidOutcome=OriginalPortableRidSource::Outcome;
                if(choice.outcome!=RidOutcome::EnvironmentRid&&choice.outcome!=RidOutcome::PortableRid&&
                   choice.outcome!=RidOutcome::NonRidFallback){
                    if(!result.portableRidSourceUnresolved)result.portableRidFirstUnresolved=choice;
                    ++result.portableRidSourceUnresolved;
                }
            }
        }
        result.portableRidSourceTraversalFinished=true;
        // Actual package coverage and host/provider selection remain distinct.
        // Preserve the concrete pending technique rather than treating native
        // LOAD_DLL membership as managed-assembly/provider selection.
        result.cause=ConsumerContractResult::Cause::HostProviderResolutionUnimplemented;
        return &result;
    }
    bool RecognizeExactPowerShellReaderSourceCase(const std::array<SIZE_T,3>& originalSourceOrdinals) noexcept {
        if(supportedSource_.attempted)return false;supportedSource_.attempted=true;
        supportedSource_.ordinals=originalSourceOrdinals;
        // Exact complete upstream originals retained beside this source unit.
        // These identify reviewed source semantics only, never a native build.
        if(nativeInputsOriginal_.Failed())return false;
        for(SIZE_T i=0;i<3;++i) {
            const SIZE_T ordinal=originalSourceOrdinals[i];
            if(ordinal>=nativeInputsOriginal_.FileCount())return false;
            const auto& file=nativeInputsOriginal_.PrivateFiles()[ordinal];
            if(!file.valid || file.closeAttempted || file.readCompletionUnknown ||
                !nativeInputsOriginal_.ExactCapturedSourceSpan(ordinal,supportedSource_.bodies[i],supportedSource_.extents[i]) ||
                supportedSource_.extents[i]!=SourceSizes[i] || memcmp(file.actualSha256.data(),SourceDigests[i],32))return false;
        }
        supportedSource_.fullSourceCorrespondence=true;
        supportedSource_.kind=SupportedSourceCase::Kind::PowerShellExactRawFileContent;
        // These three complete source bodies also match the current candidate's
        // claimed ea554a8f9c6085f54fb2828f7ea286c65c35599f originals byte-for-byte.
        // This identifies source semantics, never installed version/build.
        // Reviewed source case: synchronous Raw ReadToEnd; ProcessRecord finally
        // invokes CloseContent; reader Close invokes StreamReader/FileStream
        // disposal. Genuine selected provider/.NET/vendor build correspondence
        // and actual normal control flow remain independent, still required.
        return true;
    }
    bool ObserveAvailableSupportedSourceCase() noexcept {
        std::array<SIZE_T,3> selected{SIZE_MAX,SIZE_MAX,SIZE_MAX};
        for(SIZE_T source=0;source<3;++source)
            for(SIZE_T file=0;file<nativeInputsOriginal_.FileCount();++file) {
                const auto& original=nativeInputsOriginal_.PrivateFiles()[file];
                if(original.valid && !original.closeAttempted && original.bodyExtent==SourceSizes[source] &&
                    !memcmp(original.actualSha256.data(),SourceDigests[source],32)) {selected[source]=file;break;}
            }
        return RecognizeExactPowerShellReaderSourceCase(selected);
    }
    void ObserveAvailableHostConfiguration() {
        if(hostConfigurationAttempted_)return;hostConfigurationAttempted_=true;
        const wchar_t* names[]={L"pwsh.runtimeconfig.json",L"pwsh.deps.json"};
        // Filename selects candidates only, not authentic apphost/hostpolicy
        // consumption. Multiple selected candidates remain ambiguous/UNKNOWN.
        for(SIZE_T document=0;document<2;++document){
            for(SIZE_T i=0;i<nativeInputsOriginal_.FileCount();++i){
                const auto& file=nativeInputsOriginal_.PrivateFiles()[i];
                if(!BaseName(file,names[document]))continue;
                if(hostConfigurationOrdinals_[document]!=SIZE_MAX)return;
                hostConfigurationOrdinals_[document]=i;
            }
            if(hostConfigurationOrdinals_[document]==SIZE_MAX)return;
        }
        const unsigned char* bodies[2]{};SIZE_T extents[2]{};
        for(SIZE_T i=0;i<2;++i)
            if(!nativeInputsOriginal_.ExactCapturedSourceSpan(hostConfigurationOrdinals_[i],bodies[i],extents[i])||!extents[i])return;
        for(SIZE_T i=0;i<nativeInputsOriginal_.FileCount();++i){
            if(!BaseName(nativeInputsOriginal_.PrivateFiles()[i],L"pwsh.runtimeconfig.dev.json"))continue;
            if(hostDevConfigurationOrdinal_!=SIZE_MAX){hostDevConfigurationAmbiguous_=true;return;}
            hostDevConfigurationOrdinal_=i;
        }
        SIZE_T devExtent=0;const unsigned char* devBody=nullptr;
        if(hostDevConfigurationOrdinal_!=SIZE_MAX&&
            !nativeInputsOriginal_.ExactCapturedSourceSpan(hostDevConfigurationOrdinal_,devBody,devExtent))return;
        // Missing selection is UNSUPPLIED, not an observed nonexistent path.
        // A selected empty dev input still needs one token/frame failure cell.
        if(devExtent==SIZE_MAX)throw std::invalid_argument("original dev configuration token geometry");
        const SIZE_T devTokens=hostDevConfigurationOrdinal_==SIZE_MAX?0:devExtent+1;
        // At most one token/frame per original byte. Heap frames prevent
        // input nesting from recursively consuming an uncharged native stack.
        // Aggregate real allocator/runtime/body fit still requires qualification.
        if(OriginalRuntimeHostConfiguration::ReservedSourceBytesFor(extents[0],extents[1],devTokens)>
            SIZE_MAX-ReservedSourceBytes())
            throw std::invalid_argument("original runtime association aggregate geometry");
        SIZE_T depth=extents[0]>extents[1]?extents[0]:extents[1];if(devTokens>depth)depth=devTokens;
        hostConfiguration_=std::make_unique<OriginalRuntimeHostConfiguration>(nativeInputsOriginal_,
            extents[0],extents[1],depth,devTokens);
        if(!hostConfiguration_->PreownOriginal())std::rethrow_exception(hostConfiguration_->PreownExceptionOriginal());
        if(devTokens>SIZE_MAX-extents[0])throw std::invalid_argument("original framework list aggregate geometry");
        const SIZE_T frameworkRows=extents[0]+devTokens;
        if(OriginalFrameworkListSource::ReservedSourceBytesFor(frameworkRows)>SIZE_MAX-ReservedSourceBytes())
            throw std::invalid_argument("original framework list association geometry");
        frameworkListsSource_=std::make_unique<OriginalFrameworkListSource>(frameworkRows);
        const bool devSpan=devTokens?hostConfiguration_->CaptureOriginalDev(hostDevConfigurationOrdinal_):true;
        const bool mainSpans=hostConfiguration_->CaptureOriginals(hostConfigurationOrdinals_[0],hostConfigurationOrdinals_[1]);
        hostConfigurationCorrespondence_=mainSpans&&devSpan;
        hostConfiguration_->ReconcileOriginalLibraryOccurrences();
    }
    void ObserveAvailableVendorPackage() {
        if(vendorPackageAttempted_)return;vendorPackageAttempted_=true;
        // Candidate locator only. The existing exact creator/native-thread
        // caller owns this object and every held input through all consumers.
        // This spelling identifies the supported source case, not authenticity
        // or installed-version selection; no alternate tooling is requested.
        for(SIZE_T i=0;i<nativeInputsOriginal_.FileCount();++i){
            const auto& candidate=nativeInputsOriginal_.PrivateFiles()[i];
            if(!BaseName(candidate,L"PowerShell-7.6.3-win-x64.zip") &&
                !BaseName(candidate,L"PowerShell-7.6.5-win-x64.zip"))continue;
            if(vendorPackageOrdinal_!=SIZE_MAX)return;
            vendorPackageOrdinal_=i;
        }
        if(vendorPackageOrdinal_==SIZE_MAX||nativeInputsOriginal_.Failed())return;
        const auto& file=nativeInputsOriginal_.PrivateFiles()[vendorPackageOrdinal_];
        const unsigned char* body=nullptr;SIZE_T extent=0;
        if(!file.valid||file.closeAttempted||file.readCompletionUnknown||
            !nativeInputsOriginal_.ExactCapturedSourceSpan(vendorPackageOrdinal_,body,extent))return;
        // Allocate and attach before parsing; a partial framing result remains
        // resident. No extraction, reopening, or decoder worker is introduced.
        if(OriginalZipDirectory::ReservedSourceBytesFor(extent)>SIZE_MAX-ReservedSourceBytes())
            throw std::invalid_argument("original vendor directory aggregate geometry");
        vendorDirectory_=std::make_unique<OriginalZipDirectory>(extent);
        vendorFramingCaptured_=vendorDirectory_->Capture(body,extent);
        if(!vendorFramingCaptured_)return;
        const auto names=vendorDirectory_->MemberCount();
        if(names>(SIZE_MAX-ReservedSourceBytes())/sizeof(VendorNameOccurrence))
            throw std::invalid_argument("original vendor name geometry");
        vendorNames_=std::make_unique<VendorNameOccurrence[]>(names);vendorNameCount_=names;
        for(SIZE_T i=0;i<names;++i){
            const auto& member=vendorDirectory_->PrivateMembers()[i];auto& row=vendorNames_[i];
            const bool central=row.central.Capture(body,extent,member.name,member.nameBytes,member.extra,member.extraBytes,(member.flags&0x800)!=0);
            const bool local=row.local.Capture(body,extent,member.localName,member.nameBytes,member.localExtra,member.localExtraBytes,(member.flags&0x800)!=0);
            if(!central||!local)continue;
            OriginalZipNameCursor a(row.central.SelectedOriginal(),row.central.SelectedExtent(),row.central.SelectedUtf8());
            OriginalZipNameCursor b(row.local.SelectedOriginal(),row.local.SelectedExtent(),row.local.SelectedUtf8());
            std::uint32_t x=0,y=0;bool same=true;
            for(;;){const bool hasA=a.Next(x),hasB=b.Next(y);
                if(hasA!=hasB){same=false;break;}if(!hasA)break;if(x!=y){same=false;break;}}
            row.sameDecoded=same&&a.CompleteOriginal()&&b.CompleteOriginal();
        }
    }
    void CompareAvailableVendorAssets() {
        if(vendorComparisonAttempted_)return;vendorComparisonAttempted_=true;
        if(!vendorFramingCaptured_||vendorNameCount_!=vendorDirectory_->MemberCount()||
            (vendorNameCount_&&!vendorNames_))return;
        SIZE_T interpreter=SIZE_MAX;
        for(SIZE_T i=0;i<nativeInputsOriginal_.FileCount();++i){
            if(!BaseName(nativeInputsOriginal_.PrivateFiles()[i],L"pwsh.exe"))continue;
            if(interpreter!=SIZE_MAX)return;interpreter=i;
        }
        if(interpreter==SIZE_MAX)return;
        const auto& root=nativeInputsOriginal_.PrivateFiles()[interpreter];
        SIZE_T prefix=root.originalPathChars-1;
        while(prefix&&root.originalPath[prefix-1]!=L'\\'&&root.originalPath[prefix-1]!=L'/')--prefix;
        if(!prefix)return;
        const SIZE_T capacity=nativeInputsOriginal_.FileCount();
        if(capacity>(SIZE_MAX-ReservedSourceBytes())/sizeof(std::unique_ptr<OriginalZipHeldAssetComparison>))
            throw std::invalid_argument("original vendor asset roster geometry");
        vendorComparisons_=std::make_unique<std::unique_ptr<OriginalZipHeldAssetComparison>[]>(capacity);
        vendorComparisonCapacity_=capacity;
        for(SIZE_T asset=0;asset<capacity;++asset){
            const auto& file=nativeInputsOriginal_.PrivateFiles()[asset];
            if(asset==vendorPackageOrdinal_||!file.valid||file.closeAttempted||file.originalPathChars<=prefix+1||
                std::memcmp(file.originalPath.data(),root.originalPath.data(),prefix*sizeof(wchar_t)))continue;
            SIZE_T selected=SIZE_MAX;
            for(SIZE_T member=0;member<vendorDirectory_->MemberCount();++member)
                if(vendorNames_[member].sameDecoded&&MemberMatchesRelative(vendorNames_[member].central,
                    file.originalPath.data()+prefix,file.originalPathChars-prefix-1)){
                    if(selected!=SIZE_MAX){selected=SIZE_MAX;break;}selected=member;
                }
            if(selected==SIZE_MAX)continue;
            const auto& member=vendorDirectory_->PrivateMembers()[selected];
            SIZE_T charge=sizeof(OriginalZipHeldAssetComparison);
            if(member.method==8){
                const auto decode=OriginalRawMember::ReservedSourceBytesFor(static_cast<SIZE_T>(member.compressed),file.bodyExtent);
                if(decode>SIZE_MAX-charge)throw std::invalid_argument("original vendor comparison geometry");charge+=decode;
            }
            if(charge>SIZE_MAX-ReservedSourceBytes())throw std::invalid_argument("original vendor aggregate geometry");
            auto& comparison=vendorComparisons_[vendorComparisonCount_];
            comparison=std::make_unique<OriginalZipHeldAssetComparison>(nativeInputsOriginal_,vendorPackageOrdinal_,
                *vendorDirectory_,selected,nativeInputsOriginal_,asset);
            ++vendorComparisonCount_;
            if(!comparison->PreownOriginal()){
                vendorComparisonBytes_+=comparison->ReservedSourceBytes();
                if(comparison->PreownExceptionOriginal())std::rethrow_exception(comparison->PreownExceptionOriginal());
                throw std::invalid_argument("original vendor comparison preown refused");
            }
            vendorComparisonBytes_+=comparison->ReservedSourceBytes();
            // Same synchronous creator-thread call, no worker/callback escape.
            // Attach before library effects; End after Capture returns and keep
            // all body/result backing resident for native publication/consumers.
            comparison->CaptureMemberOriginal();comparison->DisposeDecoderEligible();
        }
    }
    const VendorNameOccurrence* PrivateVendorNames() const noexcept{return vendorNames_.get();}
    SIZE_T VendorNameCount() const noexcept{return vendorNameCount_;}
    SIZE_T VendorComparisonCount() const noexcept{return vendorComparisonCount_;}
    const OriginalZipHeldAssetComparison* PrivateVendorComparison(SIZE_T ordinal) const noexcept {
        return ordinal<vendorComparisonCount_?vendorComparisons_[ordinal].get():nullptr;
    }
    const OriginalZipDirectory* PrivateVendorDirectory() const noexcept{return vendorDirectory_.get();}
    SIZE_T PrivateVendorPackageOrdinal() const noexcept{return vendorPackageOrdinal_;}
    const OriginalRuntimeHostConfiguration* PrivateHostConfiguration() const noexcept{return hostConfiguration_.get();}
    const OriginalFrameworkListSource* PrivateFrameworkListsSource() const noexcept{return frameworkListsSource_.get();}
    const SourceRelationship& PrivateSourceRelationship() const noexcept{return source_;}
    const SupportedSourceCase& PrivateSupportedSourceCase() const noexcept{return supportedSource_;}
    const AssetOccurrence* PrivateOccurrences() const noexcept{return occurrences_.get();}
    SIZE_T OccurrenceCount() const noexcept{return count_;}
    const ConsumerContractResult* PrivateConsumerResults() const noexcept{return consumerResults_.get();}
    SIZE_T ConsumerResultCount() const noexcept{return consumerResultCount_;}
    bool AdmitsSynchronousRelease(const ConsumerContractResult& result,const State& state,
        const Event& create,const Event& exit) const noexcept {
        bool owned=false;
        for(SIZE_T i=0;i<consumerResultCount_;++i)if(&consumerResults_[i]==&result){owned=true;break;}
        return owned&&result.associationOriginal==this&&result.stateOriginal==&state&&
            result.createOriginal==&create&&result.exitOriginal==&exit&&result.attempted&&
            result.normalSynchronousRelease;
    }
    SIZE_T ReservedSourceBytes() const noexcept{
        SIZE_T total=sizeof(*this);
        const auto add=[&total](SIZE_T count,SIZE_T width) noexcept {
            if(width&&count>(SIZE_MAX-total)/width){total=SIZE_MAX;return;}
            total+=count*width;
        };
        add(capacity_,sizeof(AssetOccurrence));add(capacity_,sizeof(ConsumerContractResult));
        add(compilerArtifactCount_,sizeof(OriginalHeldMsfSource)+sizeof(OriginalHeldSourceDigest));
        add(compilerArtifactDynamicBytes_,1);
        add(hostConfiguration_?hostConfiguration_->ReservedSourceBytes():0,1);
        add(frameworkListsSource_?frameworkListsSource_->ReservedSourceBytes():0,1);
        add(vendorDirectory_?vendorDirectory_->ReservedSourceBytes():0,1);
        add(vendorComparisonCapacity_,sizeof(std::unique_ptr<OriginalZipHeldAssetComparison>));
        add(vendorComparisonBytes_,1);add(vendorNameCount_,sizeof(VendorNameOccurrence));
        return total;
    }
    // Deliberately no "Qualified" transition from version, role or file bytes.
    // The selected genuine vendor/build/source relationship and its supported
    // synchronous control-flow contract are separate admission/implementation
    // inputs. Their absence remains UNKNOWN, not successful disposal semantics.
    // NativePlatformState is resident through original host settlement; neither
    // a wrapper finalizer nor this object's destruction decrefs these originals.
public:
    bool OutputBackingSeparateOriginal(const void* output,std::size_t bytes) const noexcept {
        if(!OriginalRetainedStorageSeparate(output,bytes,this,1,sizeof(*this))||
           !OriginalRetainedArraySeparate(output,bytes,occurrences_,capacity_)||!OriginalRetainedArraySeparate(output,bytes,consumerResults_,capacity_)||
           !OriginalRetainedArraySeparate(output,bytes,compilerArtifacts_,compilerArtifactCount_)||!OriginalRetainedArraySeparate(output,bytes,compilerSourceDigests_,compilerArtifactCount_)||
           !OriginalRetainedOwnerSeparate(output,bytes,hostConfiguration_)||!OriginalRetainedOwnerSeparate(output,bytes,frameworkListsSource_)||
           !OriginalRetainedOwnerSeparate(output,bytes,vendorDirectory_)||!OriginalRetainedArraySeparate(output,bytes,vendorNames_,vendorNameCount_)||
           !OriginalRetainedArraySeparate(output,bytes,vendorComparisons_,vendorComparisonCapacity_))return false;
        if(compilerArtifacts_)for(SIZE_T i=0;i<compilerArtifactCount_;++i)if(!compilerArtifacts_[i].OutputBackingSeparateOriginal(output,bytes))return false;
        if(vendorComparisons_)for(SIZE_T i=0;i<vendorComparisonCapacity_;++i)if(!OriginalRetainedOwnerSeparate(output,bytes,vendorComparisons_[i]))return false;
        return true;
    }
};
}

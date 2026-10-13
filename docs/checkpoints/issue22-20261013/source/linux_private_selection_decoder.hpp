// UNCOMPILED private constructor format. Captured bytes are not authority.
#pragma once
#ifndef _WIN32
#include "linux_original_invocation.hpp"
#include "linux_held_file.hpp"
#include <climits>
namespace template_creator_bridge {
class OriginalLinuxSelectionDecoder {
    template_creator_linux::OriginalLinuxHeldFile& original_;
    OriginalLinuxPythonPaths python_;
    OriginalLinuxHostSelection host_{};
    OriginalLinuxRouteSelection route_;
    std::vector<std::vector<std::size_t>> reads_;
    std::vector<const std::size_t*> readPointers_;
    std::vector<std::size_t> counts_;
    std::vector<int> capabilities_;
    std::size_t cursor_=0,pythonUnits_=0,sessions_=0,routeBytes_=0,mainRows_=0;
    bool attempted_=false,decoded_=false,beginAttempted_=false;
    bool dispositionRequested_=false;
    std::exception_ptr originalException_;
    OriginalLinuxInvocation* invocation_=nullptr;
    ~OriginalLinuxSelectionDecoder()=default; // Retained through actual consumers.
    void Require(std::size_t count) const {
        if(cursor_>original_.RetainedByteCount()||count>original_.RetainedByteCount()-cursor_)
            throw std::invalid_argument("private Linux selection truncated field");
    }
    std::uint64_t UInt(unsigned bytes=8){
        Require(bytes);std::uint64_t value=0;
        for(unsigned i=0;i<bytes;++i)value|=std::uint64_t(original_.PrivateBody()[cursor_++])<<(8*i);
        return value;
    }
    std::size_t Size(){const auto n=UInt();if(n>SIZE_MAX)throw std::invalid_argument("private Linux native-size overflow");return static_cast<std::size_t>(n);}
    std::size_t Count(std::size_t minimum){
        const auto n=Size();Require(0);
        if(!minimum||n>(original_.RetainedByteCount()-cursor_)/minimum)
            throw std::invalid_argument("private Linux count exceeds original body");
        return n;
    }
    static std::uint32_t Scalar(const unsigned char* bytes,std::size_t extent,std::size_t& index){
        if(index==extent)throw std::invalid_argument("private UTF8 scalar missing");
        const auto first=bytes[index++];std::uint32_t cp=0;unsigned tails=0;std::uint32_t minimum=0;
        if(first<0x80)cp=first;
        else if(first>=0xc2&&first<=0xdf){cp=first&31;tails=1;minimum=0x80;}
        else if(first>=0xe0&&first<=0xef){cp=first&15;tails=2;minimum=0x800;}
        else if(first>=0xf0&&first<=0xf4){cp=first&7;tails=3;minimum=0x10000;}
        else throw std::invalid_argument("private UTF8 lead byte");
        for(unsigned i=0;i<tails;++i){
            if(index==extent||(bytes[index]&0xc0)!=0x80)throw std::invalid_argument("private UTF8 continuation");
            cp=(cp<<6)|(bytes[index++]&63);
        }
        if(!cp||cp<minimum||cp>0x10ffff||(cp>=0xd800&&cp<=0xdfff))
            throw std::invalid_argument("private UTF8 scalar value");
        return cp;
    }
    std::string Text(){
        const auto count=Count(1);Require(count);const auto start=cursor_;
        std::size_t index=0;while(index<count)Scalar(original_.PrivateBody()+start,count,index);
        cursor_+=count;
        return std::string(reinterpret_cast<const char*>(original_.PrivateBody()+start),count);
    }
    std::wstring WideText(){
        // Exact supported native ABI, not locale conversion or replacement.
        static_assert(sizeof(wchar_t)==4&&WCHAR_MAX>=0x10ffff);
        const auto count=Count(1);Require(count);const auto* bytes=original_.PrivateBody()+cursor_;
        std::size_t index=0,scalars=0;while(index<count){Scalar(bytes,count,index);++scalars;}
        if(scalars==SIZE_MAX||scalars+1>SIZE_MAX-pythonUnits_)
            throw std::invalid_argument("private Python path geometry");
        std::wstring result;result.resize(scalars);index=0;
        for(auto& unit:result)unit=static_cast<wchar_t>(Scalar(bytes,count,index));
        cursor_+=count;pythonUnits_+=scalars+1;return result;
    }
    void Requirements(std::size_t ordinal){
        const auto count=Count(36);auto& rows=route_.requirements[ordinal];rows.resize(count);
        for(auto& row:rows){
            const auto kind=UInt(4);
            if(kind==1){
                row.emplace<ImmutableFileRequirement>();auto& file=std::get<ImmutableFileRequirement>(row);
                file.role=Text();file.path=Text();Require(file.sha256.size());
                for(auto& byte:file.sha256)byte=original_.PrivateBody()[cursor_++];file.size=UInt();
            }else if(kind==5||kind==6){
                const auto extent=UInt();if(!extent)throw std::invalid_argument("positive native archive extent required");
                slcli_bq_expected_occurrence_selector writer{},reader{};
                writer.phase=static_cast<uint32_t>(UInt(4));writer.command=static_cast<uint32_t>(UInt(4));writer.requirement=static_cast<uint32_t>(UInt(4));
                reader.phase=static_cast<uint32_t>(UInt(4));reader.command=static_cast<uint32_t>(UInt(4));reader.requirement=static_cast<uint32_t>(UInt(4));
                const auto index=static_cast<std::size_t>(&row-rows.data());
                if(ordinal||index!=(kind==5?0:1)||writer.phase!=static_cast<uint32_t>(route_.phase)||
                   reader.phase!=writer.phase||writer.command||writer.requirement||reader.command||reader.requirement!=1)
                    throw std::invalid_argument("source-fixed native archive occurrence order required");
                if(kind==5)row=NativeArchiveFoundationPrerequisite{extent,writer,reader};
                else row=NativeArchiveReadbackPrerequisite{extent,writer,reader};
            }else throw std::invalid_argument("private Linux requirement kind");
        }
        if(!ordinal&&(rows.size()<2||!std::holds_alternative<NativeArchiveFoundationPrerequisite>(rows[0])||
           !std::holds_alternative<NativeArchiveReadbackPrerequisite>(rows[1])||
           RequirementNativeExtent(rows[0])!=RequirementNativeExtent(rows[1])))
            throw std::invalid_argument("complete native archive pair required");
        route_.observations[ordinal]=Size();route_.pollNanoseconds[ordinal]=UInt();
        const auto readCount=Count(8);if(readCount!=count)throw std::invalid_argument("private Linux input read subdivision");
        reads_[ordinal].resize(readCount);for(std::size_t i=0;i<readCount;++i){
            reads_[ordinal][i]=Size();if(!FileRequirement(rows[i])&&reads_[ordinal][i])
                throw std::invalid_argument("native archive has no immutable read subdivision");
        }
        counts_[ordinal]=count;readPointers_[ordinal]=reads_[ordinal].data();
    }
public:
    OriginalLinuxSelectionDecoder(template_creator_linux::OriginalLinuxHeldFile& original,
        int originalProcRoot,const int* actualCapabilities,std::size_t count,
        template_creator_linux::OriginalLinuxPendingHost* originalHost=nullptr):original_(original){
        if(originalProcRoot<0||!count||!actualCapabilities||count>SIZE_MAX/sizeof(int))
            throw std::invalid_argument("original Linux capability constructor");
        capabilities_.assign(actualCapabilities,actualCapabilities+count);
        host_.procRoot=originalProcRoot;host_.capabilities=capabilities_.data();host_.capabilityCount=count;
        if(originalHost){
            auto& pending=originalHost->OriginalPendingOwner();
            if(original.OriginalDeadlineOwner()!=&pending.OriginalDeadlineOwner()||
               original.OriginalSupervisorOwner()!=&pending.OriginalSupervisorOwner())
                throw std::invalid_argument("same original selection observer owners required");
            host_.preInterpreterHost=originalHost;host_.selectionOriginal=&original;
        }
        // These are borrowed original capabilities, never wire-selected FD grants.
        // Their actual ingress/caller/source associations remain native work.
    }
    OriginalLinuxSelectionDecoder(const OriginalLinuxSelectionDecoder&)=delete;
    OriginalLinuxSelectionDecoder& operator=(const OriginalLinuxSelectionDecoder&)=delete;
    bool DecodeOriginal() noexcept {
        if(dispositionRequested_||attempted_)return false;attempted_=true;
        try{
            if(!original_.CapturedSourceBytes())throw std::invalid_argument("same successfully captured Linux selection required");
            constexpr unsigned char magic[8]={'T','P','L','7','7','L','N','3'};Require(8);
            for(auto byte:magic)if(original_.PrivateBody()[cursor_++]!=byte)throw std::invalid_argument("private Linux selection marker");
            if(UInt(4)!=3)throw std::invalid_argument("private Linux selection version");
            const auto phase=UInt(4);if(phase>1)throw std::invalid_argument("private Linux protected phase");
            host_.phase=route_.phase=phase?OriginalPhase::FinalQualification:OriginalPhase::Generation;
            host_.commands=phase?2:1;
            python_.home=WideText();python_.program=WideText();python_.executable=WideText();python_.baseExecutable=WideText();
            python_.prefix=WideText();python_.execPrefix=WideText();python_.basePrefix=WideText();python_.baseExecPrefix=WideText();
            const auto modules=Count(8);python_.modulePaths.resize(modules);for(auto& path:python_.modulePaths)path=WideText();
            route_.node=Text();route_.root=Text();
            const auto environment=Count(16);route_.environment.resize(environment);
            for(auto& pair:route_.environment){pair.first=Text();pair.second=Text();}
            reads_.resize(host_.commands);readPointers_.resize(host_.commands);counts_.resize(host_.commands);
            for(std::size_t i=0;i<host_.commands;++i)Requirements(i);
            host_.inputReadRows=readPointers_.data();host_.inputCounts=counts_.data();
            host_.dispatchRows=Size();host_.hostObservations=Size();host_.sinkBytes=Size();host_.sinkCalls=Size();
            auto& g=host_.geometry;
            g.clockRows=Size();g.taskObservations=Size();g.statBytes=Size();g.statusBytes=Size();g.procReads=Size();
            g.fdinfoBytes=Size();g.supervisorStack=Size();g.supervisorJoins=Size();
            g.receiverStack=Size();g.receiverWaits=Size();g.receiverJoins=Size();
            g.mapsBytes=Size();g.mapsReads=Size();g.memoryRegions=Size();g.memoryBytes=Size();g.memoryReads=Size();
            const auto hasDeadline=UInt(1);if(hasDeadline>1)throw std::invalid_argument("private deadline shape");
            host_.hasDeadline=hasDeadline!=0;host_.absoluteNanoseconds=UInt();
            if(!host_.hasDeadline&&host_.absoluteNanoseconds)throw std::invalid_argument("absent original deadline has value");
            host_.callerStack=Size();host_.callerJoins=Size();host_.sinkOrdinal=Size();host_.sourceRootOrdinal=Size();
            host_.toolOrdinal=Size();host_.evidenceRootOrdinal=Size();
            sessions_=Size();routeBytes_=Size();mainRows_=Size();
            host_.bodyBytes=Count(1);Require(host_.bodyBytes);host_.body=original_.PrivateBody()+cursor_;cursor_+=host_.bodyBytes;
            if(cursor_!=original_.RetainedByteCount())throw std::invalid_argument("private Linux selection trailing bytes");
            if(!host_.bodyBytes||!sessions_||!routeBytes_||mainRows_<4||
               capabilities_.size()==SIZE_MAX||host_.sinkOrdinal>=capabilities_.size()+1||host_.sourceRootOrdinal>=capabilities_.size()+1||
               host_.toolOrdinal>=capabilities_.size()+1||host_.evidenceRootOrdinal>=capabilities_.size()+1)
                throw std::invalid_argument("private Linux constructor subdivision");
            decoded_=true;return true;
        }catch(...){originalException_=std::current_exception();return false;}
    }
    OriginalLinuxInvocation* BeginDecodedOriginal() noexcept {
        if(dispositionRequested_||beginAttempted_||!decoded_||!original_.CapturedSourceBytes())return nullptr;beginAttempted_=true;
        try{
            // Begin catches every exception after its resident owner is attached.
            // An escaping allocation failure therefore precedes all effects.
            invocation_=BeginOriginalLinuxInvocation(python_,pythonUnits_,sessions_,host_,route_,routeBytes_,mainRows_);
            return invocation_;
        }catch(...){if(!originalException_)originalException_=std::current_exception();return nullptr;}
    }
    bool ObserveIndependentlyEligibleClosure() noexcept {
        dispositionRequested_=true;
        if(!invocation_)return !beginAttempted_||bool(originalException_);
        return ObserveOriginalLinuxInvocationClosure(*invocation_);
        // Physical host only. Original selection/capability disposal and late
        // caller publication remain explicit enclosing-owner obligations.
    }
    const OriginalLinuxInvocation* PrivateInvocation() const noexcept{return invocation_;}
    bool ObserveFailedPreownInputEligibility(NativePlatformState* exactPlatform) noexcept {
        dispositionRequested_=true;
        return invocation_&&ObserveOriginalLinuxFailedPreownInputEligibility(*invocation_,exactPlatform);
    }
    bool ObservePrePlatformInputEligibility() noexcept {
        dispositionRequested_=true;
        return !invocation_||ObserveOriginalLinuxPrePlatformInputEligibility(*invocation_);
    }
    const std::exception_ptr& PrivateException() const noexcept{return originalException_;}
    bool Decoded() const noexcept{return decoded_;}
    bool InitialProviderReadRowsOriginal(std::size_t command,std::size_t requirement,std::size_t& rows) const noexcept {
        if(!decoded_||!original_.CapturedSourceBytes()||command>=reads_.size()||
           command>=route_.requirements.size()||requirement>=reads_[command].size()||
           reads_[command].size()!=route_.requirements[command].size())return false;
        if(!FileRequirement(route_.requirements[command][requirement]))return false;
        rows=reads_[command][requirement];return rows!=0;
    }
    bool InitialProviderNativePrerequisiteNoReadRowsOriginal(std::size_t command,std::size_t requirement) const noexcept {
        return decoded_&&original_.CapturedSourceBytes()&&command<reads_.size()&&
            command<route_.requirements.size()&&requirement<reads_[command].size()&&
            reads_[command].size()==route_.requirements[command].size()&&
            !FileRequirement(route_.requirements[command][requirement])&&reads_[command][requirement]==0;
    }
    bool InitialProviderOfferCountOriginal(std::size_t& count) const noexcept {
        count=0;
        if(!decoded_||!original_.CapturedSourceBytes()||host_.commands<1||host_.commands>route_.requirements.size())return false;
        std::size_t total=1; // Separate actual producer process REFERENCE.
        for(std::size_t command=0;command<host_.commands;++command){
            const auto requirements=route_.requirements[command].size();
            if(requirements>SIZE_MAX-total)return false;
            total+=requirements;
        }
        for(std::size_t command=host_.commands;command<route_.requirements.size();++command)
            if(!route_.requirements[command].empty())return false;
        count=total;return true; // Geometry only, no subject family/member/admission.
    }
    const OriginalLinuxRouteSelection* InitialProviderRouteOriginal() const noexcept {
        return decoded_&&original_.CapturedSourceBytes()?&route_:nullptr;
    }
    bool InitialProviderCommandCountOriginal(std::size_t& count) const noexcept {
        count=0;
        if(!decoded_||!original_.CapturedSourceBytes()||!host_.commands||host_.commands>route_.requirements.size())return false;
        count=host_.commands;return true;
    }
    bool ReservedDecodedSourceBytes(std::size_t& bytes) const noexcept {
        bytes=sizeof(*this);
        const auto add=[&](std::size_t count,std::size_t width) noexcept {
            if(width&&count>(SIZE_MAX-bytes)/width)return false;bytes+=count*width;return true;
        };
        const auto text=[&](const std::string& value) noexcept {
            return value.capacity()!=SIZE_MAX&&add(value.capacity()+1,sizeof(char));
        };
        const auto wide=[&](const std::wstring& value) noexcept {
            return value.capacity()!=SIZE_MAX&&add(value.capacity()+1,sizeof(wchar_t));
        };
        if(!add(reads_.capacity(),sizeof(std::vector<std::size_t>))||
           !add(readPointers_.capacity(),sizeof(const std::size_t*))||!add(counts_.capacity(),sizeof(std::size_t))||
           !add(capabilities_.capacity(),sizeof(int))||!add(python_.modulePaths.capacity(),sizeof(std::wstring))||
           !add(route_.environment.capacity(),sizeof(std::pair<std::string,std::string>)))return false;
        for(const auto& row:reads_)if(!add(row.capacity(),sizeof(std::size_t)))return false;
        if(!wide(python_.home)||!wide(python_.program)||!wide(python_.executable)||!wide(python_.baseExecutable)||
           !wide(python_.prefix)||!wide(python_.execPrefix)||!wide(python_.basePrefix)||!wide(python_.baseExecPrefix)||
           !text(route_.node)||!text(route_.root))return false;
        for(const auto& path:python_.modulePaths)if(!wide(path))return false;
        for(const auto& pair:route_.environment)if(!text(pair.first)||!text(pair.second))return false;
        for(const auto& rows:route_.requirements){
            if(!add(rows.capacity(),sizeof(OriginalLinuxRequirementSelection)))return false;
            for(const auto& row:rows)if(const auto* file=FileRequirement(row))
                if(!text(file->role)||!text(file->path))return false;
        }
        return true; // Conservative SSO count; held file/invocation/runtime/allocator/overlap additional.
    }
};
// The enclosing native caller retains this resident decoder even when Decode
// or Begin fails, keeping body/capability/configuration originals reachable.
OriginalLinuxSelectionDecoder* BeginOriginalLinuxCapturedSelection(
    template_creator_linux::OriginalLinuxHeldFile&,int originalProcRoot,
    const int* actualCapabilities,std::size_t capabilityCount,
    template_creator_linux::OriginalLinuxPendingHost* originalHost=nullptr);
}
#endif

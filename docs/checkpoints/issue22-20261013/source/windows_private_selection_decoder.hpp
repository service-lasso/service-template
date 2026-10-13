// UNCOMPILED private constructor-input decoder, not an authority/proof format.
// Native decoding precedes interpreter initialization: no JSON/import bootstrap
// is needed to discover the interpreter's own original path configuration.
#pragma once
#include "windows_original_invocation.hpp"
#include <limits>
namespace template_creator_bridge {
class OriginalWindowsSelectionDecoder {
    OriginalWindowsSelectionReader& original_;
    SIZE_T cursor_=0,stringUnits_=0;
    bool attempted_=false,decoded_=false,prechargeAttempted_=false;
    std::exception_ptr originalException_;
    OriginalPythonPaths python_;
    OriginalWindowsCapabilityPaths capabilities_;
    OriginalNativeRouteSelection route_,bootstrap_;
    OriginalWindowsHostSelection host_{};
    std::vector<OriginalNativeInputLayoutSelection> layouts_;
    SIZE_T mainObservations_=0,receiptPolls_=0,receiptStack_=0;
    SIZE_T bodyOffset_=0;
    DWORD bodyBytes_=0;
    OriginalWindowsInvocation* invocation_=nullptr;
    ~OriginalWindowsSelectionDecoder()=default; // All original copies resident.
    void Require(SIZE_T bytes) const {
        if(cursor_>original_.OriginalExtent() || bytes>original_.OriginalExtent()-cursor_)
            throw std::invalid_argument("private selection truncated field");
    }
    uint64_t Unsigned(unsigned bytes) {
        Require(bytes);uint64_t value=0;
        for(unsigned i=0;i<bytes;++i)value|=static_cast<uint64_t>(original_.OriginalBytes()[cursor_++])<<(8*i);
        return value;
    }
    SIZE_T Size() {
        const auto value=Unsigned(8);
        if(value>SIZE_MAX)throw std::invalid_argument("private selection native-size overflow");
        return static_cast<SIZE_T>(value);
    }
    SIZE_T Count(SIZE_T minimumWireBytes) {
        const auto value=Size();
        if(!minimumWireBytes || value>(original_.OriginalExtent()-cursor_)/minimumWireBytes)
            throw std::invalid_argument("private selection count exceeds actual body");
        return value;
    }
    std::wstring Text() {
        const SIZE_T units=Count(2);Require(units*2);
        if(units>=32768 || units+1>SIZE_MAX-stringUnits_)
            throw std::invalid_argument("private selection bounded UTF16 original");
        // Validate all scalar units before constructing the native string.
        const auto start=cursor_;
        bool high=false;
        for(SIZE_T i=0;i<units;++i) {
            const auto value=Unsigned(2);
            if(!value || (high && !(value>=0xdc00&&value<=0xdfff)) ||
                (!high && value>=0xdc00&&value<=0xdfff))
                throw std::invalid_argument("private selection invalid UTF16 scalar");
            high=!high && value>=0xd800&&value<=0xdbff;
        }
        if(high)throw std::invalid_argument("private selection incomplete UTF16 scalar");
        std::wstring result;result.resize(units);
        for(SIZE_T i=0;i<units;++i)result[i]=static_cast<wchar_t>(original_.OriginalBytes()[start+2*i] |
            (static_cast<unsigned>(original_.OriginalBytes()[start+2*i+1])<<8));
        stringUnits_+=units+1;return result;
    }
    void Environment(OriginalNativeRouteSelection& selected) {
        const SIZE_T count=Count(16);selected.environment.resize(count);
        for(auto& pair:selected.environment){pair.first=Text();pair.second=Text();}
    }
    void Requirements(OriginalNativeRouteSelection& selected,SIZE_T ordinal) {
        const SIZE_T count=Count(56);
        if(!count || count>1024)throw std::invalid_argument("private selected requirement roster");
        auto& rows=selected.requirements[ordinal];rows.resize(count);
        for(auto& row:rows) {
            row.role=Text();row.path=Text();Require(row.sha256.size());
            for(auto& byte:row.sha256)byte=original_.OriginalBytes()[cursor_++];
            row.size=Unsigned(8);
        }
        selected.observations[ordinal]=Unsigned(8);selected.pollNanoseconds[ordinal]=Unsigned(8);
    }
public:
    explicit OriginalWindowsSelectionDecoder(OriginalWindowsSelectionReader& original):original_(original) {}
    OriginalWindowsSelectionDecoder(const OriginalWindowsSelectionDecoder&)=delete;
    OriginalWindowsSelectionDecoder& operator=(const OriginalWindowsSelectionDecoder&)=delete;
    bool DecodeOriginal() noexcept {
        if(attempted_)return false;attempted_=true;
        try {
            if(!original_.Captured())throw std::invalid_argument("same captured private selection required");
            constexpr unsigned char magic[8]={'T','P','L','7','7','I','N','4'};Require(sizeof(magic));
            for(const auto byte:magic)if(original_.OriginalBytes()[cursor_++]!=byte)
                throw std::invalid_argument("private selection version marker");
            if(Unsigned(4)!=4)throw std::invalid_argument("private selection version");
            const auto phase=Unsigned(4);
            if(phase>1)throw std::invalid_argument("protected generation/final private selection phase");
            route_.phase=host_.phase=phase?OriginalPhase::FinalQualification:OriginalPhase::Generation;
            host_.protectedCommands=phase?2:1;
            python_.home=Text();python_.program=Text();python_.executable=Text();python_.baseExecutable=Text();
            python_.prefix=Text();python_.execPrefix=Text();python_.basePrefix=Text();python_.baseExecPrefix=Text();
            const SIZE_T modules=Count(10);if(!modules)throw std::invalid_argument("original interpreter module paths");
            python_.modulePaths.resize(modules);for(auto& path:python_.modulePaths)path=Text();
            capabilities_.candidateDirectory=Text();capabilities_.toolFile=Text();capabilities_.evidenceDirectory=Text();
            route_.node=Text();route_.root=Text();Environment(route_);
            for(SIZE_T i=0;i<host_.protectedCommands;++i)Requirements(route_,i);
            bootstrap_.phase=OriginalPhase::Generation; // Single five-field conversion only.
            bootstrap_.node=Text();bootstrap_.root=Text();Environment(bootstrap_);Requirements(bootstrap_,0);
            host_.dispatchRows=Size();host_.hostClosureObservations=Size();
            host_.sinkBytes=Size();host_.readbackBytes=Size();host_.sinkCalls=Size();
            host_.collectorBytes=Size();host_.collectorReads=Size();host_.collectorObservations=Size();
            host_.finalBytes=Size();host_.finalReadbackBytes=Size();host_.finalCalls=Size();
            host_.hostProgressObservations=Size();
            host_.finalCollectorBytes=Size();host_.finalCollectorReads=Size();host_.finalCollectorObservations=Size();
            host_.pendingCallRows=Size();
            if(!host_.pendingCallRows)throw std::invalid_argument("original pending call subdivision required");
            host_.observerGeometry.threads=Size();host_.observerGeometry.invocations=Size();
            host_.observerGeometry.pointers=Size();host_.observerGeometry.stringBytes=Size();host_.observerGeometry.readRows=Size();
            if(!host_.observerGeometry.Complete())throw std::invalid_argument("complete original observer subdivisions required");
            host_.noticeGeometry.noticeStack=Size();host_.noticeGeometry.receiverStack=Size();
            host_.noticeGeometry.noticeIoRows=Size();host_.noticeGeometry.receiverIoRows=Size();
            if(!host_.noticeGeometry.Complete())throw std::invalid_argument("complete original notice subdivisions required");
            host_.finalCollectorWaitMilliseconds=static_cast<DWORD>(Unsigned(4));
            host_.collectorWaitMilliseconds=static_cast<DWORD>(Unsigned(4));
            mainObservations_=Size();receiptPolls_=Size();receiptStack_=Size();
            const SIZE_T layouts=Count(32);
            if(layouts!=host_.protectedCommands+1)throw std::invalid_argument("complete native layout roster");
            layouts_.resize(layouts);
            for(auto& row:layouts_) {
                row.callerRows=Size();row.readRows=Size();row.ntdllOrdinal=Size();
                const SIZE_T images=Count(16);row.images.resize(images);
                for(auto& image:row.images){image.bytes=Size();image.regions=Size();}
            }
            bodyBytes_=static_cast<DWORD>(Unsigned(4));
            if(!bodyBytes_ || bodyBytes_>65536)throw std::invalid_argument("private ingress body representation");
            Require(bodyBytes_);bodyOffset_=cursor_;cursor_+=bodyBytes_;
            if(cursor_!=original_.OriginalExtent())throw std::invalid_argument("private selection trailing bytes");
            decoded_=true;return true;
        } catch(...) {originalException_=std::current_exception();return false;}
    }
    OriginalWindowsInvocation* PrechargeInvocationOriginal() noexcept {
        if(prechargeAttempted_ || !decoded_)return nullptr;prechargeAttempted_=true;
        try {
            invocation_=new OriginalWindowsInvocation(); // Attach before further allocation/effects.
            if(!invocation_->BindCapturedSelectionOriginal(original_) ||
                !invocation_->Precharge(python_,stringUnits_,host_,capabilities_,route_,stringUnits_,
                    bootstrap_,stringUnits_,layouts_,original_.OriginalBytes()+bodyOffset_,bodyBytes_,
                    mainObservations_,receiptPolls_,receiptStack_))return nullptr;
            return invocation_;
        } catch(...) {if(!originalException_)originalException_=std::current_exception();return nullptr;}
    }
    bool ObserveIndependentlyEligibleClosure() noexcept {
        if(invocation_)return invocation_->ObserveIndependentlyEligibleClosure();
        // Decode/precharge allocation failure before any invocation owns no
        // interpreter/channel/child consumer; retain the parser exception.
        return original_.CloseAfterConsumers();
    }
    const OriginalWindowsSelectionReader& OriginalInput() const noexcept {return original_;}
    const OriginalWindowsInvocation* PrivateInvocation() const noexcept {return invocation_;}
    // All selected capacities are still unqualified input. Count checks bound
    // native parsing allocations by actual encoded geometry; they neither
    // establish physical whole fit nor authenticate runtime/source/capabilities.
};
}

// UNCOMPILED private native constructor. Explicit geometry/capability selections
// are not authentic runtime/source/native admission. No ambient ENV discovery.
#ifndef _WIN32
#include "linux_private_selection_startup.hpp"
#include "bq_provider_entry.h"
#ifdef TEMPLATE_NATIVE_ROOT_MEMORY_REGRESSION
#include "linux_memory_regression_receipt.hpp"
#endif
#include <linux/futex.h>
#include <time.h>
#include <atomic>
#include <algorithm>
#include <climits>
#include <limits>
extern "C" {
extern const struct slcli_bq_provider_expectations slcli_bq_destination_expectations;
extern const struct slcli_bq_destination_retention_constraints slcli_bq_destination_retention_constraints;
}
namespace template_creator_bridge {
struct LinuxEntryProgress {
    bool attempted=false,returned=false,closureResult=false,delayAttempted=false,delayReturned=false;
    int delayResult=0,delayError=0;timespec requested{},remaining{};
};
struct LinuxOriginalEntry {
    int argc=0;char** argv=nullptr;
    std::array<char,SLCLI_BQ_PROVIDER_ENDPOINT_TEXT_BYTES> endpointText{};
    std::array<char,SLCLI_BQ_PROVIDER_COUNT_TEXT_BYTES> offerCountText{};
    OriginalLinuxSelectionStartupInput input;
    OriginalLinuxSelectionStartup* startup=nullptr;
    template_creator_linux::IngressFd untransferredEndpoint;
    bool endpointCustodyRetained=false,endpointTransferred=false;
    std::unique_ptr<LinuxEntryProgress[]> progress;
    size_t capacity=0,count=0;uint64_t cadence=0;
    std::exception_ptr originalException;
    bool accepted=false,settled=false,exhausted=false;
    int retentionWord=0;bool retainAttempted=false,retainReturned=false;
    long retainResult=0;int retainError=0;
    static size_t Decimal(const char* s,bool zero=false) {
        if(!s||!*s)throw std::invalid_argument("private decimal absent");
        size_t n=0;
        for(;*s;++s){if(*s<'0'||*s>'9'||n>(SIZE_MAX-size_t(*s-'0'))/10)
            throw std::invalid_argument("private decimal representation");n=n*10+size_t(*s-'0');}
        if(!zero&&!n)throw std::invalid_argument("private positive geometry");return n;
    }
    static unsigned Hex(char c){if(c>='0'&&c<='9')return c-'0';if(c>='a'&&c<='f')return c-'a'+10;
        if(c>='A'&&c<='F')return c-'A'+10;throw std::invalid_argument("private hex");}
    void Begin(int actualArgc,char** actualArgv) noexcept {
        argc=actualArgc;argv=actualArgv;
        try {
            // Fixed fields: path,digest,bytes,reads,progress,cadence-ns,
            // deadline-present,absolute-ns,proc-root,caller-stack,caller-joins,
            // sixteen PendingInputGeometry fields, ingress-body hex, then FDs.
            if(argc<32||!argv)throw std::invalid_argument("private Linux constructor arguments");
            // These native entry operands precede the historical private
            // selection operands. Syntax/count is neither birth nor financing.
            const auto copyOperand=[](const char* source,auto& destination) {
                if(!source)throw std::invalid_argument("initial provider operand absent");
                size_t n=0;while(n<destination.size()&&source[n])++n;
                if(n==destination.size())throw std::invalid_argument("initial provider operand extent");
                std::memcpy(destination.data(),source,n+1);return n+1;
            };
            const size_t endpointBytes=copyOperand(argv[SLCLI_BQ_PROVIDER_ENDPOINT_ARGUMENT],endpointText);
            const size_t countBytes=copyOperand(argv[SLCLI_BQ_PROVIDER_OFFER_COUNT_ARGUMENT],offerCountText);
            int32_t endpoint=-1;uint64_t offerCount=0;
            if(!slcli_bq_provider_endpoint_decode(&endpoint,endpointText.data(),endpointBytes)||
               !slcli_bq_provider_count_decode(&offerCount,offerCountText.data(),countBytes)||offerCount>SIZE_MAX)
                throw std::invalid_argument("initial provider endpoint/count representation");
            input.initialProviderEndpoint=endpoint;input.initialProviderOfferCount=static_cast<size_t>(offerCount);
            template_creator_linux::OriginalInitialProviderReceiver::CheckedSourceBacking(input.initialProviderOfferCount);
            // Preserve every prior operand index through a local view; the
            // retained argc/argv above still denotes the original native entry.
            auto* privateArgv=argv+(SLCLI_BQ_PROVIDER_PREVIOUS_ARGUMENT_START-1);
            const int privateArgc=argc-(SLCLI_BQ_PROVIDER_PREVIOUS_ARGUMENT_START-1);
            BeginSelectionOperands(privateArgc,privateArgv);
        }catch(...){originalException=std::current_exception();}
    }
    void BeginSelectionOperands(int argc,char** argv) {
            // Exact destination-local constraints only; neither linkage nor
            // these policy limits supply the independent initial BQ reserve.
            input.sourceExpectationsOriginal=&slcli_bq_destination_expectations;
            input.retentionConstraintsOriginal=&slcli_bq_destination_retention_constraints;
            if(!template_creator_linux::OriginalLinuxPrebirthRetentionPolicy::InitialContainingReservationQualifiedOriginal())
                throw std::invalid_argument("initial BQ source/containing reservation missing before selection allocations");
            input.selectionPath=argv[1];
            if(std::char_traits<char>::length(argv[2])!=64)throw std::invalid_argument("selection digest length");
            for(size_t i=0;i<32;++i)input.selectionSha256[i]=static_cast<unsigned char>((Hex(argv[2][2*i])<<4)|Hex(argv[2][2*i+1]));
            input.selectionBytes=Decimal(argv[3]);input.selectionReadRows=Decimal(argv[4]);
            capacity=Decimal(argv[5]);cadence=Decimal(argv[6]);
            if(capacity>SIZE_MAX/sizeof(LinuxEntryProgress)||cadence/1000000000ULL>uint64_t(std::numeric_limits<time_t>::max()))
                throw std::invalid_argument("entry storage/time representation");
            const size_t present=Decimal(argv[7],true);if(present>1)throw std::invalid_argument("deadline flag");
            input.hasDeadline=present!=0;input.absoluteNanoseconds=Decimal(argv[8],true);
            const size_t proc=Decimal(argv[9],true);if(proc>INT_MAX)throw std::invalid_argument("proc descriptor representation");
            input.procRoot=static_cast<int>(proc);input.callerStack=Decimal(argv[10]);input.callerJoins=Decimal(argv[11]);
            size_t* fields[]={&input.geometry.clockRows,&input.geometry.taskObservations,&input.geometry.statBytes,
                &input.geometry.statusBytes,&input.geometry.procReads,&input.geometry.fdinfoBytes,
                &input.geometry.supervisorStack,&input.geometry.supervisorJoins,&input.geometry.receiverStack,
                &input.geometry.receiverWaits,&input.geometry.receiverJoins,&input.geometry.mapsBytes,
                &input.geometry.mapsReads,&input.geometry.memoryRegions,&input.geometry.memoryBytes,&input.geometry.memoryReads};
            for(size_t i=0;i<16;++i)*fields[i]=Decimal(argv[12+i]);
            const size_t hexBytes=std::char_traits<char>::length(argv[28]);
            if(!hexBytes||(hexBytes&1)||hexBytes/2>template_creator_linux::OriginalPrivateIngress::BodyCapacity)
                throw std::invalid_argument("ingress body geometry");
            input.ingressBody.resize(hexBytes/2);
            for(size_t i=0;i<input.ingressBody.size();++i)input.ingressBody[i]=static_cast<unsigned char>((Hex(argv[28][2*i])<<4)|Hex(argv[28][2*i+1]));
            if(size_t(argc-29)>=template_creator_linux::OriginalPrivateIngress::FdCapacity)
                throw std::invalid_argument("capability subdivision");
            for(int i=29;i<argc;++i){const size_t fd=Decimal(argv[i],true);if(fd>INT_MAX)
                throw std::invalid_argument("capability descriptor representation");input.capabilities.push_back(static_cast<int>(fd));}
            if(input.initialProviderEndpoint==input.procRoot||
               std::find(input.capabilities.begin(),input.capabilities.end(),input.initialProviderEndpoint)!=input.capabilities.end())
                throw std::invalid_argument("initial provider descriptor has competing local custody");
            untransferredEndpoint.original=input.initialProviderEndpoint;endpointCustodyRetained=true;
            progress=std::make_unique<LinuxEntryProgress[]>(capacity);
            accepted=true;
            // Same destination image's reviewed constraint occurrence only.
            // No definition/default is supplied by this packet; linkage and
            // authentic native acquisition remain separate required inputs.
            input.sourceExpectationsOriginal=&slcli_bq_destination_expectations;
            startup=BeginOriginalLinuxSelectionStartup(input);
    }
    bool Observe() noexcept {
        if(endpointCustodyRetained&&!endpointTransferred){
            endpointTransferred=startup&&startup->initialProvider&&
                startup->initialProvider->OwnsOriginalEndpoint(untransferredEndpoint.original);
            if(!endpointTransferred){
                untransferredEndpoint.CloseOnce();
                if(!untransferredEndpoint.closed){exhausted=true;return false;}
            }
        }
        if(!startup)return true; // No native startup effect if initial allocation failed.
        if(count==capacity){exhausted=true;return false;}
        auto& row=progress[count++];row.attempted=true;
        row.closureResult=startup->ObserveIndependentlyEligibleClosure();row.returned=true;
        // Consume the same actual owner publication, not interpreter presence
        // or a declaration that physical native cleanup implies qualification.
        const bool localPublished=row.closureResult&&startup->PublishLocalSettlementOriginal().published;
        // SAME owning-source settlement permits natural return after actual
        // required local operations and borrowers settle. Qualification failure
        // remains a negative outcome; future external exit/EOF is observed later.
        // No external custodian or interpreter-heap copy is implied here.
        settled=localPublished;
        return settled;
    }
    void Delay() noexcept {
        auto& row=progress[count-1];row.requested.tv_sec=static_cast<time_t>(cadence/1000000000ULL);
        row.requested.tv_nsec=static_cast<long>(cadence%1000000000ULL);row.delayAttempted=true;
        row.delayResult=clock_nanosleep(CLOCK_MONOTONIC,0,&row.requested,&row.remaining);
        row.delayError=row.delayResult;row.delayReturned=true;
        if(row.delayResult)exhausted=true; // No retry of interrupted scheduling call.
    }
    bool ReservedEntrySourceBytes(size_t& bytes) const noexcept {
        bytes=sizeof(*this);
        const auto add=[&](size_t n,size_t width) noexcept {
            if(width&&n>(SIZE_MAX-bytes)/width)return false;bytes+=n*width;return true;
        };
        if(!add(capacity,sizeof(LinuxEntryProgress))||!add(input.ingressBody.capacity(),1)||
           !add(input.capabilities.capacity(),sizeof(int))||input.selectionPath.capacity()==SIZE_MAX||
           !add(input.selectionPath.capacity()+1,1))return false;
        // The actual startup copy, decoder, invocation, CRT argv/environment,
        // Python/runtime, native stacks/kernel and allocators are additional.
        return true;
    }
    [[noreturn]] void Retain() noexcept {
        retainAttempted=true;retainResult=syscall(SYS_futex,&retentionWord,FUTEX_WAIT_PRIVATE,0,nullptr,nullptr,0);
        if(retainResult<0)retainError=errno;retainReturned=true;
        // One original blocking result retained. An unexpected return does not
        // authorize exiting the original main thread or destroying borrowers.
        for(;;){std::atomic_signal_fence(std::memory_order_seq_cst);}
    }
};
static LinuxOriginalEntry* originalLinuxEntry=nullptr;
}
int main(int argc,char** argv){
    using namespace template_creator_bridge;
    try{originalLinuxEntry=new LinuxOriginalEntry();}catch(...){return 2;}
    originalLinuxEntry->Begin(argc,argv);
    while(!originalLinuxEntry->Observe()){
        if(originalLinuxEntry->exhausted)originalLinuxEntry->Retain();
        originalLinuxEntry->Delay();
    }
    const auto* owner=originalLinuxEntry->startup;
#ifdef TEMPLATE_NATIVE_ROOT_MEMORY_REGRESSION
    // Test outcome only, after SAME real local cleanup settlement. Product
    // qualification failure and the denied EXEC remain in original evidence.
    return owner&&owner->LocalSettlementRetainsStartupOriginals()&&!originalLinuxEntry->originalException&&
        template_creator_linux::ActualRootExecFiveSpanRegressionPassed()?0:2;
#else
    return owner&&owner->LocalSettlementRetainsStartupOriginals()&&
        !owner->localSettlement.qualificationFailed&&!originalLinuxEntry->originalException?0:2;
#endif
}
#endif

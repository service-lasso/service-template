// UNCOMPILED INITIAL_PROVIDER native receipt custody. No admission from framing.
#pragma once
#ifndef _WIN32
#include "linux_ingress.hpp"
#include "linux_deadline_supervisor.hpp"
#include "bq_subject_transfer.h"
#include "upstream-cli/bq_provider_expectations.h"
#include <poll.h>
#include <limits>
#include <memory>
#include <stdexcept>
#include <fcntl.h>
namespace template_creator_bridge { class OriginalLinuxSelectionStartup; struct NativePlatformState; }
namespace template_creator_linux {
class OriginalReceivedSubjectReadback;
class OriginalReceivedSourceUnitObservation;
class OriginalInitialProviderProcessAssociation;
class OriginalInitialProviderArchive;
class OriginalInitialProviderReceiver {
    friend struct InitialProviderReceiptSourceRegression;
    friend class template_creator_bridge::OriginalLinuxSelectionStartup;
    friend class OriginalInitialProviderArchive;
    friend bool VerifySurvivingPendingHostPreownRegressionOriginal();
    friend struct template_creator_bridge::NativePlatformState;
public:
    // Control alignment padding can carry extra rights. Retain every delivered
    // bounded descriptor before rejecting the one-rights OFFER shape.
    static constexpr size_t ControlBytes=CMSG_SPACE(sizeof(ucred))+CMSG_SPACE(sizeof(int));
    static constexpr size_t RightsCapacity=(ControlBytes-CMSG_LEN(0))/sizeof(int);
    struct CandidateRouteOriginal {
        uint64_t sequence=0,memberBytes=0;
        uint32_t phase=0,command=0,requirement=0;
    };
    struct OfferOriginal {
        struct AckOriginal {
            slcli_bq_subject_route route{};
            std::array<unsigned char,SLCLI_BQ_SUBJECT_RECORD_BYTES> payload{};
            pollfd readiness{};timespec timeout{};
            IngressNativeClock waitClock{},sendClock{};
            iovec vector{};msghdr message{};
            int waitResult=-1,waitError=0;ssize_t sendResult=-1;int sendError=0;
            bool started=false,hasTimeout=false,waitAttempted=false,waitReturned=false;
            bool sendAttempted=false,sendReturned=false,complete=false;
        } ack;
        slcli_bq_subject_route expected{},decoded{};
        CandidateRouteOriginal sourceRoute{};
        bool exactExpectedRequested=false;
        std::array<unsigned char,SLCLI_BQ_SUBJECT_RECORD_BYTES> payload{};
        alignas(cmsghdr) std::array<unsigned char,ControlBytes> control{};
        std::array<IngressFd,RightsCapacity> rights{};
        iovec vector{};msghdr message{};ucred credentials{};
        IngressNativeClock receiveClock{};
        IngressNativeClock rightsFlagsClock{};
        int rightsFlags=-1,rightsFlagsError=0;
        bool rightsFlagsAttempted=false,rightsFlagsReturned=false;
        ssize_t result=-1;int error=0;
        size_t rightsCount=0,rightsMessages=0,credentialMessages=0;
        bool reserved=false,attempted=false,returned=false;
        bool controlExtentUnknown=false,shapeFailed=false,routeDecoded=false;
        bool rawOfferObserved=false;
        pollfd readiness{};timespec readinessTimeout{};
        IngressNativeClock readinessClock{};
        const OriginalDeadlineSupervisor* readinessSupervisor=nullptr;
        int readinessResult=-1,readinessError=0;
        bool readinessStarted=false,readinessAttempted=false,readinessReturned=false;
        bool readinessHasTimeout=false,readabilityObserved=false;
    };
private:
    IngressFd endpoint_{};
    std::unique_ptr<OfferOriginal[]> offers_;
    size_t capacity_=0,count_=0,sourceBytes_=0;
    const OriginalDeadline* deadline_=nullptr;
    OriginalDeadlineSupervisor* supervisor_=nullptr;
    OriginalInitialProviderArchive* archiveOriginal_=nullptr;
    bool endpointAttached_=false,frozen_=false,failed_=false;
    uint64_t lastSequence_=0;
    const slcli_bq_provider_expectations* bootstrapExpectationsOriginal_=nullptr;
    uint32_t bootstrapPhaseOriginal_=UINT32_MAX;
    size_t bootstrapAcksCompleted_=0;
    slcli_bq_subject_route rejectedExpectedOriginal_{};
    bool rejectedExpectedObserved_=false;
    CandidateRouteOriginal rejectedSourceRoute_{};
    bool rejectedSourceObserved_=false;
    int endpointType_=0,endpointPassCredentials_=0;
    ucred endpointPeer_{};
    socklen_t endpointTypeBytes_=sizeof(endpointType_);
    socklen_t endpointPassBytes_=sizeof(endpointPassCredentials_);
    socklen_t endpointPeerBytes_=sizeof(endpointPeer_);
    std::array<int,3> endpointQueryResult_{{-1,-1,-1}},endpointQueryError_{};
    std::array<bool,3> endpointQueryAttempted_{},endpointQueryReturned_{};
    std::array<IngressNativeClock,3> endpointQueryClock_{};
    bool endpointObservationAttempted_=false,endpointShapeObserved_=false;
    ~OriginalInitialProviderReceiver()=default;
    void ObserveDescriptorOriginal(IngressFd& fd) noexcept {
        if(fd.statObservationAttempted)return;fd.statObservationAttempted=true;
        if(!supervisor_||supervisor_->OriginalDeadlineOwner()!=deadline_||
           !fd.statClock.Before(deadline_,"initial-provider-fstat",false)||!supervisor_->BeginNativeCall())return;
        fd.statAttempted=true;fd.statResult=fstat(fd.original,&fd.status);
        if(fd.statResult<0)fd.statErrno=errno;fd.statReturned=true;
        supervisor_->OriginalNativeCallReturned();fd.statClock.After(deadline_,"initial-provider-fstat");
    }
    bool ObserveEndpointOriginal() noexcept {
        if(endpointObservationAttempted_)return endpointShapeObserved_;
        endpointObservationAttempted_=true;
        ObserveDescriptorOriginal(endpoint_);
        if(!endpoint_.statAttempted||endpoint_.statResult!=0||!endpoint_.statClock.clean||
           !S_ISSOCK(endpoint_.status.st_mode))return false;
        void* outputs[3]={&endpointType_,&endpointPassCredentials_,&endpointPeer_};
        socklen_t* extents[3]={&endpointTypeBytes_,&endpointPassBytes_,&endpointPeerBytes_};
        const int options[3]={SO_TYPE,SO_PASSCRED,SO_PEERCRED};
        for(size_t i=0;i<3;++i){
            if(!endpointQueryClock_[i].Before(deadline_,"initial-provider-endpoint-query",false))return false;
            if(!supervisor_->BeginNativeCall())return false;
            endpointQueryAttempted_[i]=true;
            endpointQueryResult_[i]=getsockopt(endpoint_.original,SOL_SOCKET,options[i],outputs[i],extents[i]);
            if(endpointQueryResult_[i]<0)endpointQueryError_[i]=errno;
            endpointQueryReturned_[i]=true;
            supervisor_->OriginalNativeCallReturned();
            endpointQueryClock_[i].After(deadline_,"initial-provider-endpoint-query");
            if(endpointQueryResult_[i]!=0||!endpointQueryClock_[i].clean)return false;
        }
        endpointShapeObserved_=endpointTypeBytes_==sizeof(endpointType_)&&endpointType_==SOCK_SEQPACKET&&
            endpointPassBytes_==sizeof(endpointPassCredentials_)&&endpointPassCredentials_!=0&&
            endpointPeerBytes_==sizeof(endpointPeer_)&&endpointPeer_.pid>0;
        return endpointShapeObserved_; // Actual peer fields are still not admitted birth/image proof.
    }
public:
    static size_t CheckedSourceBacking(size_t sourceOfferCount) {
        if(!sourceOfferCount||sourceOfferCount>(SIZE_MAX-sizeof(OriginalInitialProviderReceiver))/sizeof(OfferOriginal))
            throw std::invalid_argument("initial provider complete offer geometry");
        return sizeof(OriginalInitialProviderReceiver)+sourceOfferCount*sizeof(OfferOriginal);
    }
    explicit OriginalInitialProviderReceiver(size_t sourceOfferCount,const OriginalDeadline* deadline,
        OriginalDeadlineSupervisor* supervisor=nullptr)
        :capacity_(sourceOfferCount),deadline_(deadline),supervisor_(supervisor) {
        // Containing entry must charge this derived source backing before
        // constructing this owner. This check is not that independent charge,
        // allocator/kernel residency or the complete EF52 numeric admission.
        sourceBytes_=CheckedSourceBacking(capacity_);
        offers_=std::make_unique<OfferOriginal[]>(capacity_);
    }
    OriginalInitialProviderReceiver(const OriginalInitialProviderReceiver&)=delete;
    OriginalInitialProviderReceiver& operator=(const OriginalInitialProviderReceiver&)=delete;
    // The actual inherited-endpoint birth/allowlist consumer must call this.
    // Attaching a number only transfers local custody; it authenticates nobody.
    bool AttachOriginalInheritedEndpoint(int original) noexcept {
        if(endpointAttached_||frozen_||original<0){failed_=true;return false;}
        endpoint_.original=original;endpointAttached_=true;return true;
    }
    bool ReceiveOfferOriginal(const slcli_bq_subject_route& sourceExpected,OriginalDeadlineSupervisor* supervisor=nullptr) noexcept {
        if(frozen_||failed_)return false;
        if(!slcli_bq_subject_route_equal(&sourceExpected,&sourceExpected)){
            rejectedExpectedOriginal_=sourceExpected;rejectedExpectedObserved_=true;failed_=true;return false;
        }
        if(sourceExpected.message!=SLCLI_BQ_SUBJECT_OFFER||sourceExpected.cut!=SLCLI_BQ_SUBJECT_INITIAL_PROVIDER||
           (count_==0?sourceExpected.role!=SLCLI_BQ_SUBJECT_REFERENCE:sourceExpected.role==SLCLI_BQ_SUBJECT_REFERENCE)){
            rejectedExpectedOriginal_=sourceExpected;rejectedExpectedObserved_=true;failed_=true;return false;
        }
        return ReceiveOriginal({sourceExpected.sequence,sourceExpected.member_bytes,sourceExpected.phase,
            sourceExpected.command,sourceExpected.requirement},&sourceExpected,supervisor);
    }
    bool ReceiveCandidateOriginal(const CandidateRouteOriginal& sourceRoute,OriginalDeadlineSupervisor* supervisor=nullptr) noexcept {
        return ReceiveOriginal(sourceRoute,nullptr,supervisor);
    }
    bool BindBootstrapOriginal(const slcli_bq_provider_expectations& source,uint32_t phase) noexcept {
        if(frozen_||failed_||count_||bootstrapExpectationsOriginal_||source.subject_count==SIZE_MAX||
           capacity_!=source.subject_count+1||!slcli_bq_provider_expectations_bootstrap_shape(&source,phase))return false;
        bootstrapExpectationsOriginal_=&source;bootstrapPhaseOriginal_=phase;return true;
        // Protocol geometry only. Complete archive/authentic acquisition remains independent.
    }
    bool AcknowledgeReadbackOriginal(const OriginalReceivedSubjectReadback&) noexcept;
    bool AcknowledgeSourceUnitOriginal(const OriginalReceivedSourceUnitObservation&) noexcept;
    bool AcknowledgeProcessReferenceOriginal(const OriginalInitialProviderProcessAssociation&) noexcept;
private:
    bool ArchiveOriginalsQualifiedAtCut(size_t) const noexcept;
    bool SendReadbackAckOriginal(size_t ordinal) noexcept {
        if(frozen_||failed_||ordinal>=count_||!supervisor_||
           supervisor_->OriginalDeadlineOwner()!=deadline_)return false;
        if(bootstrapExpectationsOriginal_&&ordinal<3){
            if(count_!=3||ordinal!=bootstrapAcksCompleted_||
               (ordinal&&!offers_[ordinal-1].ack.complete))return false;
        }else if(ordinal+1!=count_||(bootstrapExpectationsOriginal_&&bootstrapAcksCompleted_!=3))return false;
        if(!ArchiveOriginalsQualifiedAtCut(ordinal))return false;
        auto& offer=offers_[ordinal];auto& ack=offer.ack;
        if(!offer.rawOfferObserved||ack.started)return false;ack.started=true;
        ack.route=offer.decoded;ack.route.message=SLCLI_BQ_SUBJECT_ARCHIVE_ACK;
        if(!slcli_bq_subject_route_encode(ack.payload.data(),ack.payload.size(),&ack.route))return false;
        ack.readiness.fd=endpoint_.original;ack.readiness.events=POLLOUT;
        if(!ack.waitClock.Before(deadline_,"initial-provider-ack-writable",false))return false;
        ack.hasTimeout=deadline_->HasOriginalDeadline();
        if(ack.hasTimeout){
            const uint64_t now=ack.waitClock.before.nanoseconds,end=deadline_->OriginalAbsoluteNanoseconds();
            if(!ack.waitClock.before.shapeValid||now>=end)return false;
            const uint64_t remaining=end-now,seconds=remaining/1000000000;
            if(seconds>static_cast<uint64_t>(std::numeric_limits<time_t>::max()))return false;
            ack.timeout.tv_sec=static_cast<time_t>(seconds);ack.timeout.tv_nsec=static_cast<long>(remaining%1000000000);
        }
        if(!supervisor_->BeginNativeCall())return false;ack.waitAttempted=true;
        ack.waitResult=ppoll(&ack.readiness,1,ack.hasTimeout?&ack.timeout:nullptr,nullptr);
        if(ack.waitResult<0)ack.waitError=errno;ack.waitReturned=true;supervisor_->OriginalNativeCallReturned();
        ack.waitClock.After(deadline_,"initial-provider-ack-writable");
        if(ack.waitResult!=1||ack.readiness.revents!=POLLOUT||!ack.waitClock.clean)return false;
        ack.vector.iov_base=ack.payload.data();ack.vector.iov_len=ack.payload.size();
        ack.message.msg_iov=&ack.vector;ack.message.msg_iovlen=1;
        // No ancillary rights: readback ACK does not transfer or retire subjects.
        if(!ack.sendClock.Before(deadline_,"initial-provider-readback-ack",false)||!supervisor_->BeginNativeCall())return false;
        ack.sendAttempted=true;ack.sendResult=sendmsg(endpoint_.original,&ack.message,MSG_DONTWAIT|MSG_NOSIGNAL);
        if(ack.sendResult<0)ack.sendError=errno;ack.sendReturned=true;supervisor_->OriginalNativeCallReturned();
        ack.sendClock.After(deadline_,"initial-provider-readback-ack");
        ack.complete=ack.sendResult==static_cast<ssize_t>(ack.payload.size())&&ack.sendClock.clean;
        if(ack.complete&&bootstrapExpectationsOriginal_&&ordinal<3)++bootstrapAcksCompleted_;
        return ack.complete; // Actual ARCHIVE_ACK; no authority or last-borrower close.
    }
    static bool ReadabilityOriginal(const OfferOriginal& row) noexcept {
        return row.readinessAttempted&&row.readinessReturned&&row.readinessClock.clean&&
            row.readinessResult==1&&row.readiness.events==POLLIN&&row.readiness.revents==POLLIN;
    }
    bool WaitReadableOriginal(OfferOriginal& row,OriginalDeadlineSupervisor& supervisor) noexcept {
        row.readinessStarted=true;row.readinessSupervisor=&supervisor;
        if(!deadline_||supervisor.OriginalDeadlineOwner()!=deadline_)return false;
        row.readiness.fd=endpoint_.original;row.readiness.events=POLLIN;
        if(!row.readinessClock.Before(deadline_,"initial-provider-offer-readable",false))return false;
        row.readinessHasTimeout=deadline_->HasOriginalDeadline();
        if(row.readinessHasTimeout){
            const uint64_t now=row.readinessClock.before.nanoseconds;
            const uint64_t end=deadline_->OriginalAbsoluteNanoseconds();
            if(!row.readinessClock.before.shapeValid||now>=end)return false;
            const uint64_t remaining=end-now,seconds=remaining/1000000000;
            if(seconds>static_cast<uint64_t>(std::numeric_limits<time_t>::max()))return false;
            row.readinessTimeout.tv_sec=static_cast<time_t>(seconds);
            row.readinessTimeout.tv_nsec=static_cast<long>(remaining%1000000000);
        }
        if(!supervisor.BeginNativeCall())return false;
        row.readinessAttempted=true;
        row.readinessResult=ppoll(&row.readiness,1,row.readinessHasTimeout?&row.readinessTimeout:nullptr,nullptr);
        if(row.readinessResult<0)row.readinessError=errno;
        row.readinessReturned=true;supervisor.OriginalNativeCallReturned();
        row.readinessClock.After(deadline_,"initial-provider-offer-readable");
        row.readabilityObserved=ReadabilityOriginal(row);return row.readabilityObserved;
    }
    bool ReceiveOriginal(const CandidateRouteOriginal& sourceRoute,const slcli_bq_subject_route* exactExpected,
        OriginalDeadlineSupervisor* supervisor=nullptr) noexcept {
        if(frozen_||failed_)return false;
        const bool bootstrapPrefix=bootstrapExpectationsOriginal_&&count_<3;
        if(bootstrapExpectationsOriginal_){
            const auto& expected=*bootstrapExpectationsOriginal_;
            if(!slcli_bq_provider_expectations_bootstrap_shape(&expected,bootstrapPhaseOriginal_)||
               expected.subject_count==SIZE_MAX||capacity_!=expected.subject_count+1||
               sourceRoute.phase!=bootstrapPhaseOriginal_||
               (count_>=3&&bootstrapAcksCompleted_!=3)){
                rejectedSourceRoute_=sourceRoute;rejectedSourceObserved_=true;failed_=true;return false;
            }
            if(count_>0&&count_<3){const auto& row=expected.subjects[count_-1];
                if(sourceRoute.command!=row.command||sourceRoute.requirement!=row.requirement||
                   sourceRoute.memberBytes!=row.source_occurrence.native_file_bytes||
                   !offers_[count_-1].rawOfferObserved){
                    rejectedSourceRoute_=sourceRoute;rejectedSourceObserved_=true;failed_=true;return false;
                }
            }
        }
        if(!endpointAttached_||count_==capacity_||(count_&&!offers_[count_-1].ack.complete&&!bootstrapPrefix)||!sourceRoute.sequence||lastSequence_==UINT64_MAX||
           sourceRoute.sequence!=lastSequence_+1||sourceRoute.phase>2||
           (count_==0?(sourceRoute.memberBytes!=0||sourceRoute.command!=UINT32_MAX||sourceRoute.requirement!=UINT32_MAX):
               (sourceRoute.command==UINT32_MAX||sourceRoute.requirement==UINT32_MAX))){
            rejectedSourceRoute_=sourceRoute;rejectedSourceObserved_=true;
            if(exactExpected){rejectedExpectedOriginal_=*exactExpected;rejectedExpectedObserved_=true;}
            failed_=true;return false;
        }
        auto& row=offers_[count_++];row.reserved=true;row.sourceRoute=sourceRoute;
        if(exactExpected){row.expected=*exactExpected;row.exactExpectedRequested=true;}
        lastSequence_=sourceRoute.sequence; // Reservation consumes this use even on failure.
        if(!supervisor||supervisor!=supervisor_||supervisor->OriginalDeadlineOwner()!=deadline_){failed_=true;return false;}
        if(!ObserveEndpointOriginal()){failed_=true;return false;}
        if(!supervisor||!WaitReadableOriginal(row,*supervisor)){failed_=true;return false;}
        row.vector.iov_base=row.payload.data();row.vector.iov_len=row.payload.size();
        row.message.msg_iov=&row.vector;row.message.msg_iovlen=1;
        row.message.msg_control=row.control.data();row.message.msg_controllen=row.control.size();
        if(!row.receiveClock.Before(deadline_,"initial-provider-offer-recvmsg",false)){
            failed_=true;return false;
        }
        if(!supervisor->BeginNativeCall()){failed_=true;return false;}
        row.attempted=true;
        row.result=recvmsg(endpoint_.original,&row.message,MSG_CMSG_CLOEXEC|MSG_DONTWAIT);
        if(row.result<0)row.error=errno;row.returned=true;
        supervisor->OriginalNativeCallReturned();
        row.receiveClock.After(deadline_,"initial-provider-offer-recvmsg");
        if(row.result<0){failed_=true;return false;}
        CaptureAncillaryOriginal(row);
        if(!row.shapeFailed&&!row.controlExtentUnknown){
            row.routeDecoded=slcli_bq_subject_route_decode(&row.decoded,row.payload.data(),row.payload.size())!=0;
            const auto& candidate=row.decoded;
            row.rawOfferObserved=row.routeDecoded&&candidate.message==SLCLI_BQ_SUBJECT_OFFER&&
                candidate.cut==SLCLI_BQ_SUBJECT_INITIAL_PROVIDER&&candidate.sequence==row.sourceRoute.sequence&&
                candidate.phase==row.sourceRoute.phase&&candidate.command==row.sourceRoute.command&&
                candidate.requirement==row.sourceRoute.requirement&&candidate.member_bytes==row.sourceRoute.memberBytes&&
                (count_==1?candidate.role==SLCLI_BQ_SUBJECT_REFERENCE:candidate.role!=SLCLI_BQ_SUBJECT_REFERENCE)&&
                (!row.exactExpectedRequested||slcli_bq_subject_route_equal(&row.expected,&candidate))&&row.receiveClock.clean;
        }
        if(row.rawOfferObserved){
            auto& fd=row.rights[0];ObserveDescriptorOriginal(fd);
            if(!fd.statAttempted||fd.statResult!=0||!fd.statClock.clean)row.rawOfferObserved=false;
            if(row.rawOfferObserved&&row.rightsFlagsClock.Before(deadline_,"initial-provider-rights-flags",false)){
                if(!supervisor->BeginNativeCall()){failed_=true;return false;}
                row.rightsFlagsAttempted=true;row.rightsFlags=fcntl(fd.original,F_GETFD);
                if(row.rightsFlags<0)row.rightsFlagsError=errno;row.rightsFlagsReturned=true;
                supervisor->OriginalNativeCallReturned();
                row.rightsFlagsClock.After(deadline_,"initial-provider-rights-flags");
                row.rawOfferObserved=row.rightsFlags>=0&&(row.rightsFlags&FD_CLOEXEC)&&row.rightsFlagsClock.clean;
            }else row.rawOfferObserved=false;
        }
        if(!row.rawOfferObserved)failed_=true;
        return row.rawOfferObserved; // No peer/birth/image/member/native-subject authority.
    }
private:
    bool DisposeBeforeSubjectExposureOriginal() noexcept {
        // Only SAME startup before creator invocation can reach this path.
        // Actual acquired-subject borrowers require a separate later consumer.
        if(supervisor_&&supervisor_->HasPendingOriginalNativeCall())return false;
        if(endpoint_.statAttempted&&!endpoint_.statReturned)return false;
        for(size_t i=0;i<count_;++i)if(offers_[i].readinessAttempted&&!offers_[i].readinessReturned)return false;
        for(size_t i=0;i<3;++i)if(endpointQueryAttempted_[i]&&!endpointQueryReturned_[i])return false;
        for(size_t i=0;i<count_;++i){
            const auto& row=offers_[i];
            if((row.ack.waitAttempted&&!row.ack.waitReturned)||(row.ack.sendAttempted&&!row.ack.sendReturned))return false;
            if(row.controlExtentUnknown||(row.attempted&&!row.returned)||
               (row.rightsFlagsAttempted&&!row.rightsFlagsReturned))return false;
            for(size_t j=0;j<row.rightsCount;++j){
                const auto& fd=row.rights[j];
                if(fd.statAttempted&&!fd.statReturned)return false;
                if(fd.original>=0&&fd.original==endpoint_.original)return false;
                for(size_t prior=0;prior<=i;++prior){
                    const size_t limit=prior==i?j:offers_[prior].rightsCount;
                    for(size_t k=0;k<limit;++k)if(fd.original>=0&&fd.original==offers_[prior].rights[k].original)return false;
                }
            }
        }
        frozen_=true;
        return CloseIndependentOriginalsAfterBorrowersSettled();
    }
    static void CaptureAncillaryOriginal(OfferOriginal& row) noexcept {
        if(row.message.msg_controllen>row.control.size()){
            row.controlExtentUnknown=true;return;
        }
        size_t offset=0;
        while(offset<row.message.msg_controllen){
            if(row.message.msg_controllen-offset<sizeof(cmsghdr)){row.controlExtentUnknown=true;row.shapeFailed=true;break;}
            cmsghdr header{};std::memcpy(&header,row.control.data()+offset,sizeof(header));
            const size_t length=header.cmsg_len;
            if(length<CMSG_LEN(0)||length>row.message.msg_controllen-offset){row.controlExtentUnknown=true;row.shapeFailed=true;break;}
            const size_t bytes=length-CMSG_LEN(0);
            const auto* data=row.control.data()+offset+CMSG_LEN(0);
            if(header.cmsg_level==SOL_SOCKET&&header.cmsg_type==SCM_RIGHTS){
                if(row.rightsMessages++)row.shapeFailed=true;
                if(bytes%sizeof(int)){row.controlExtentUnknown=true;row.shapeFailed=true;}
                for(size_t index=0;index<bytes/sizeof(int);++index){
                    if(row.rightsCount==row.rights.size()){
                        row.controlExtentUnknown=true;row.shapeFailed=true;break;
                    }
                    auto& fd=row.rights[row.rightsCount++];
                    std::memcpy(&fd.original,data+index*sizeof(int),sizeof(int));
                    if(fd.original<0){row.controlExtentUnknown=true;row.shapeFailed=true;}
                }
            }else if(header.cmsg_level==SOL_SOCKET&&header.cmsg_type==SCM_CREDENTIALS&&bytes==sizeof(ucred)){
                if(row.credentialMessages++)row.shapeFailed=true;
                else std::memcpy(&row.credentials,data,sizeof(ucred));
            }else {row.controlExtentUnknown=true;row.shapeFailed=true;}
            const size_t step=CMSG_ALIGN(length);
            if(step<length){row.controlExtentUnknown=true;row.shapeFailed=true;break;}
            if(step>row.message.msg_controllen-offset){
                if(length!=row.message.msg_controllen-offset){row.controlExtentUnknown=true;row.shapeFailed=true;}
                break;
            }
            offset+=step;
        }
        if(row.message.msg_flags&(MSG_TRUNC|MSG_CTRUNC))row.shapeFailed=true;
        if(row.result!=static_cast<ssize_t>(row.payload.size())||row.rightsCount!=1||
           row.rightsMessages!=1||row.credentialMessages!=1)row.shapeFailed=true;
    }
    // Startup's private pre-exposure path checks actual returned native output
    // custody above. After subject exposure, concrete borrower settlement and
    // archive readback are still required; no public assertion/ACK reaches this.
    bool FreezeReceiptsOriginal() noexcept {frozen_=true;return !failed_;}
    void CloseDescriptorOriginal(IngressFd& fd) noexcept {
        if(fd.original<0||fd.closeAttempted)return;
        if(!supervisor_||supervisor_->OriginalDeadlineOwner()!=deadline_||
           !fd.closeClock.Before(deadline_,"initial-provider-once-close",true)||
           !supervisor_->BeginIndependentlyEligibleCleanupNativeCall())return;
        fd.closeAttempted=true;fd.closeResult=close(fd.original);
        if(fd.closeResult<0)fd.closeErrno=errno;else fd.closed=true;
        fd.closeReturned=true;supervisor_->OriginalNativeCallReturned();
        fd.closeClock.After(deadline_,"initial-provider-once-close");
    }
    bool CloseIndependentOriginalsAfterBorrowersSettled() noexcept {
        if(!frozen_)return false;
        for(size_t i=0;i<count_;++i)if(offers_[i].controlExtentUnknown)return false;
        bool closed=true;
        for(size_t i=0;i<count_;++i)for(size_t j=0;j<offers_[i].rightsCount;++j){
            auto& fd=offers_[i].rights[j];CloseDescriptorOriginal(fd);
            closed&=fd.original<0||fd.closed;
        }
        CloseDescriptorOriginal(endpoint_);closed&=!endpointAttached_||endpoint_.closed;
        return closed; // Physical close only; no ACK or qualification inferred.
    }
public:
    bool IndependentOriginalsClosedAtCutOriginal() const noexcept {
        if(!frozen_||(supervisor_&&supervisor_->HasPendingOriginalNativeCall())||
           (endpointAttached_&&!endpoint_.closed))return false;
        for(size_t i=0;i<count_;++i){
            const auto& row=offers_[i];
            if(row.controlExtentUnknown||(row.attempted&&!row.returned)||
               (row.ack.waitAttempted&&!row.ack.waitReturned)||(row.ack.sendAttempted&&!row.ack.sendReturned))return false;
            for(size_t j=0;j<row.rightsCount;++j)if(row.rights[j].original>=0&&!row.rights[j].closed)return false;
        }
        return true;
    }
    const OriginalInitialProviderArchive* OriginalArchiveOwner() const noexcept{return archiveOriginal_;}
    const OfferOriginal* OriginalOffer(size_t ordinal) const noexcept{return ordinal<count_?&offers_[ordinal]:nullptr;}
    size_t OriginalOfferCount() const noexcept{return count_;}
    size_t SourceBackingBytes() const noexcept{return sourceBytes_;}
    bool Failed() const noexcept{return failed_;}
    bool OwnsOriginalEndpoint(int original) const noexcept {
        return endpointAttached_&&endpoint_.original==original;
    }
    const ucred* OriginalObservedEndpointPeer() const noexcept {
        return endpointShapeObserved_?&endpointPeer_:nullptr;
    }
};
}
#endif

// Synthetic source regression of the SAME receiver preflight; UNRUN.
// No producer/endpoint/native receipt identities or admission are fabricated.
#ifndef _WIN32
#include "linux_upstream_subject_receiver.hpp"
#include "linux_initial_provider_archive.hpp"
namespace template_creator_linux {
struct InitialProviderReceiptSourceRegression {
    static bool MissingArchiveCannotQualify() {
        OriginalInitialProviderReceiver owner(1,nullptr);
        // Actual out-of-line receiver-to-archive consumer; no constructed
        // archive/finance/authentic producer original exists in this case.
        return !owner.ArchiveOriginalsQualifiedAtCut(0)&&!owner.archiveOriginal_&&
            !owner.offers_[0].ack.started&&!owner.offers_[0].ack.waitAttempted&&
            !owner.offers_[0].ack.sendAttempted&&owner.count_==0&&owner.lastSequence_==0;
    }
    static bool ArchiveReceivePrefixDisposition() {
        // Synthetic native-output shapes, not native observations/admission.
        // Copies below invoke the SAME fixed archive consumer predicate.
        OriginalInitialProviderReceiver::OfferOriginal complete{};
        complete.reserved=complete.attempted=complete.returned=complete.rawOfferObserved=complete.routeDecoded=true;
        complete.result=SLCLI_BQ_SUBJECT_RECORD_BYTES;
        complete.rightsCount=complete.rightsMessages=complete.credentialMessages=1;
        complete.readinessAttempted=complete.readinessReturned=complete.readabilityObserved=true;
        complete.readinessResult=1;complete.readiness.events=POLLIN;complete.readiness.revents=POLLIN;
        complete.readinessClock.clean=complete.receiveClock.clean=complete.rightsFlagsClock.clean=true;
        complete.rightsFlagsAttempted=complete.rightsFlagsReturned=true;complete.rightsFlags=FD_CLOEXEC;
        complete.rights[0].original=101;complete.rights[0].statAttempted=complete.rights[0].statReturned=true;
        complete.rights[0].statResult=0;complete.rights[0].statClock.clean=true;
        const auto clockShape=[](IngressNativeClock& clock){
            clock.before.attempted=clock.after.attempted=true;clock.before.returned=clock.after.returned=0;
            clock.before.shapeValid=clock.after.shapeValid=true;clock.before.nanoseconds=10;clock.after.nanoseconds=11;
        };
        clockShape(complete.readinessClock);clockShape(complete.receiveClock);
        clockShape(complete.rightsFlagsClock);clockShape(complete.rights[0].statClock);
        if(!OriginalInitialProviderArchive::ReceivePrefixRetainedOriginal(complete))return false;
        for(unsigned failure=0;failure<17;++failure){
            auto invalid=complete;
            switch(failure){
            case 0:invalid.returned=false;break;
            case 1:invalid.result=SLCLI_BQ_SUBJECT_RECORD_BYTES-1;break;
            case 2:invalid.controlExtentUnknown=true;break;
            case 3:invalid.rightsCount=2;break;
            case 4:invalid.message.msg_flags=MSG_CTRUNC;break;
            case 5:invalid.readinessReturned=false;break;
            case 6:invalid.receiveClock.clean=false;break;
            case 7:invalid.rightsFlagsReturned=false;break;
            case 8:invalid.rightsFlags=0;break;
            case 9:invalid.rights[0].statReturned=false;break;
            case 10:invalid.rights[0].statResult=-1;break;
            case 11:invalid.rights[0].closeAttempted=true;break;
            case 12:invalid.readinessResult=0;break;
            case 13:invalid.readiness.revents=POLLIN|POLLHUP;break;
            case 14:invalid.receiveClock.after.attempted=false;break;
            case 15:invalid.rightsFlagsClock.after.nanoseconds=9;break;
            case 16:invalid.rights[0].statClock.after.expired=true;break;
            }
            if(OriginalInitialProviderArchive::ReceivePrefixRetainedOriginal(invalid))return false;
        }
        return true;
    }
    static bool ArchiveChronologyDisposition() {
        // Synthetic scalar clock rows exercise the SAME production predicate;
        // these are not actual native clocks or authenticated archive cuts.
        OriginalClockBoundary complete{};complete.attempted=true;complete.returned=0;
        complete.shapeValid=true;complete.nanoseconds=100;
        if(!OriginalInitialProviderArchive::CutChronologyOriginal(complete,100)||
           !OriginalInitialProviderArchive::CutChronologyOriginal(complete,99)||
           OriginalInitialProviderArchive::CutChronologyOriginal(complete,101))return false;
        auto invalid=complete;invalid.attempted=false;
        if(OriginalInitialProviderArchive::CutChronologyOriginal(invalid,99))return false;
        invalid=complete;invalid.returned=-1;
        if(OriginalInitialProviderArchive::CutChronologyOriginal(invalid,99))return false;
        invalid=complete;invalid.shapeValid=false;
        if(OriginalInitialProviderArchive::CutChronologyOriginal(invalid,99))return false;
        invalid=complete;invalid.expired=true;
        return !OriginalInitialProviderArchive::CutChronologyOriginal(invalid,99);
    }
    static bool UnknownControlCannotDispose() {
        OriginalInitialProviderReceiver owner(1,nullptr);owner.count_=1;owner.frozen_=true;
        auto& row=owner.offers_[0];row.message.msg_controllen=row.control.size()+1;
        OriginalInitialProviderReceiver::CaptureAncillaryOriginal(row);
        return row.controlExtentUnknown&&!owner.CloseIndependentOriginalsAfterBorrowersSettled()&&
            !owner.endpoint_.closeAttempted&&!row.rights[0].closeAttempted;
    }
    static bool UnreturnedStatCannotDispose() {
        OriginalInitialProviderReceiver owner(1,nullptr);
        owner.endpoint_.statAttempted=true;
        return !owner.DisposeBeforeSubjectExposureOriginal()&&!owner.frozen_&&!owner.endpoint_.closeAttempted;
    }
    static bool UnwalkableControlCannotDispose() {
        OriginalInitialProviderReceiver owner(1,nullptr);owner.count_=1;
        auto& row=owner.offers_[0];row.message.msg_controllen=sizeof(cmsghdr)-1;
        OriginalInitialProviderReceiver::CaptureAncillaryOriginal(row);
        return row.shapeFailed&&row.controlExtentUnknown&&!owner.DisposeBeforeSubjectExposureOriginal()&&
            !owner.frozen_&&!owner.endpoint_.closeAttempted;
    }
    static bool DuplicateRightsCannotDispose() {
        OriginalInitialProviderReceiver owner(1,nullptr);owner.count_=1;
        owner.offers_[0].rightsCount=2;
        owner.offers_[0].rights[0].original=101;owner.offers_[0].rights[1].original=101;
        return !owner.DisposeBeforeSubjectExposureOriginal()&&!owner.frozen_&&
            !owner.endpoint_.closeAttempted&&!owner.offers_[0].rights[0].closeAttempted&&
            !owner.offers_[0].rights[1].closeAttempted;
    }
    static bool CapacityBeforeEffects() {
        OriginalInitialProviderReceiver owner(1,nullptr);owner.count_=1;owner.endpointAttached_=true;
        slcli_bq_subject_route route{};
        route.message=SLCLI_BQ_SUBJECT_OFFER;route.cut=SLCLI_BQ_SUBJECT_INITIAL_PROVIDER;
        route.sequence=1;route.member=1;route.role=SLCLI_BQ_SUBJECT_REFERENCE;
        route.command=UINT32_MAX;route.requirement=UINT32_MAX;
        return !owner.ReceiveOfferOriginal(route)&&owner.failed_&&owner.count_==1&&
            owner.lastSequence_==0&&!owner.endpointObservationAttempted_;
    }
    static bool AncillaryRetention(size_t rightsCount,int flags) {
        OriginalInitialProviderReceiver::OfferOriginal row{};
        row.result=SLCLI_BQ_SUBJECT_RECORD_BYTES;
        row.message.msg_flags=flags;
        cmsghdr rights{};rights.cmsg_level=SOL_SOCKET;rights.cmsg_type=SCM_RIGHTS;
        rights.cmsg_len=CMSG_LEN(rightsCount*sizeof(int));
        std::memcpy(row.control.data(),&rights,sizeof(rights));
        const int synthetic[2]={101,102};
        std::memcpy(row.control.data()+CMSG_LEN(0),synthetic,rightsCount*sizeof(int));
        const size_t next=CMSG_SPACE(rightsCount*sizeof(int));
        cmsghdr credentials{};credentials.cmsg_level=SOL_SOCKET;credentials.cmsg_type=SCM_CREDENTIALS;
        credentials.cmsg_len=CMSG_LEN(sizeof(ucred));
        std::memcpy(row.control.data()+next,&credentials,sizeof(credentials));
        const ucred syntheticCredentials{77,88,99};
        std::memcpy(row.control.data()+next+CMSG_LEN(0),&syntheticCredentials,sizeof(syntheticCredentials));
        row.message.msg_controllen=next+CMSG_SPACE(sizeof(ucred));
        OriginalInitialProviderReceiver::CaptureAncillaryOriginal(row);
        return row.rightsCount==rightsCount&&row.rightsMessages==1&&row.credentialMessages==1&&
            row.rights[0].original==101&&(rightsCount==1||row.rights[1].original==102)&&
            row.shapeFailed==(rightsCount!=1||flags!=0)&&!row.controlExtentUnknown&&
            !row.rights[0].statAttempted&&!row.rights[0].closeAttempted;
    }
    static bool InvalidExpectedRoleBeforeEffects(uint32_t role) {
        OriginalInitialProviderReceiver owner(1,nullptr);
        // Test-only inconsistent attachment with the retained negative sentinel:
        // a missing preflight would attempt fstat(-1), never a real upstream FD.
        owner.endpointAttached_=true;
        slcli_bq_subject_route route{};
        route.message=SLCLI_BQ_SUBJECT_OFFER;route.cut=SLCLI_BQ_SUBJECT_INITIAL_PROVIDER;
        route.sequence=1;route.member=1;route.role=role;
        const bool result=owner.ReceiveOfferOriginal(route);
        return !result&&owner.failed_&&owner.count_==0&&owner.lastSequence_==0&&
            owner.rejectedExpectedObserved_&&owner.rejectedExpectedOriginal_.role==role&&
            !owner.endpointObservationAttempted_&&!owner.endpoint_.statObservationAttempted&&
            !owner.offers_[0].reserved&&!owner.offers_[0].attempted;
    }
    static bool InvalidInitialStageBeforeEffects(unsigned variant) {
        OriginalInitialProviderReceiver owner(2,nullptr);owner.endpointAttached_=true;
        slcli_bq_subject_route route{};
        route.message=SLCLI_BQ_SUBJECT_OFFER;route.cut=SLCLI_BQ_SUBJECT_INITIAL_PROVIDER;
        route.sequence=1;route.member=1;route.role=SLCLI_BQ_SUBJECT_REFERENCE;
        route.command=UINT32_MAX;route.requirement=UINT32_MAX;
        if(variant==0)route.sequence=2;
        if(variant==1)route.phase=3;
        if(variant==2)route.command=0;
        if(variant==3)route.member_bytes=1;
        if(variant==4)route.role=SLCLI_BQ_SUBJECT_SOURCE;
        return !owner.ReceiveOfferOriginal(route)&&owner.failed_&&owner.count_==0&&
            owner.lastSequence_==0&&!owner.endpointObservationAttempted_&&
            !owner.offers_[0].reserved&&!owner.offers_[0].attempted;
    }
    static bool InvalidCandidateGeometryBeforeEffects(unsigned variant) {
        OriginalInitialProviderReceiver owner(2,nullptr);owner.endpointAttached_=true;
        OriginalInitialProviderReceiver::CandidateRouteOriginal source{1,0,0,UINT32_MAX,UINT32_MAX};
        if(variant==0)source.sequence=2;
        if(variant==1)source.phase=3;
        if(variant==2)source.command=0;
        if(variant==3)source.memberBytes=1;
        if(variant==4)source.requirement=0;
        return !owner.ReceiveCandidateOriginal(source)&&owner.failed_&&owner.count_==0&&
            owner.lastSequence_==0&&owner.rejectedSourceObserved_&&
            owner.rejectedSourceRoute_.sequence==source.sequence&&
            owner.rejectedSourceRoute_.phase==source.phase&&owner.rejectedSourceRoute_.command==source.command&&
            owner.rejectedSourceRoute_.requirement==source.requirement&&
            owner.rejectedSourceRoute_.memberBytes==source.memberBytes&&
            !owner.rejectedExpectedObserved_&&!owner.endpointObservationAttempted_&&
            !owner.offers_[0].reserved&&!owner.offers_[0].exactExpectedRequested;
    }
    static bool ReadabilityDisposition() {
        // Synthetic outputs exercise the SAME production predicate; UNRUN,
        // not an actual readable endpoint or successful native wait.
        OriginalInitialProviderReceiver::OfferOriginal row{};
        row.readinessAttempted=true;row.readinessReturned=true;
        row.readinessResult=1;row.readiness.events=POLLIN;row.readiness.revents=POLLIN;
        if(!OriginalInitialProviderReceiver::ReadabilityOriginal(row))return false;
        auto invalid=row;invalid.readinessReturned=false;
        if(OriginalInitialProviderReceiver::ReadabilityOriginal(invalid))return false;
        invalid=row;invalid.readinessResult=0;
        if(OriginalInitialProviderReceiver::ReadabilityOriginal(invalid))return false;
        invalid=row;invalid.readinessResult=-1;invalid.readinessError=EINTR;
        if(OriginalInitialProviderReceiver::ReadabilityOriginal(invalid))return false;
        invalid=row;invalid.readiness.revents=POLLIN|POLLHUP;
        if(OriginalInitialProviderReceiver::ReadabilityOriginal(invalid))return false;
        invalid=row;invalid.readiness.revents=POLLERR;
        if(OriginalInitialProviderReceiver::ReadabilityOriginal(invalid))return false;
        invalid=row;invalid.readinessClock.clean=false;
        return !OriginalInitialProviderReceiver::ReadabilityOriginal(invalid);
    }
    static bool Verify() {
        slcli_bq_subject_route valid{};
        valid.message=SLCLI_BQ_SUBJECT_OFFER;valid.cut=SLCLI_BQ_SUBJECT_INITIAL_PROVIDER;
        valid.sequence=1;valid.member=1;valid.role=SLCLI_BQ_SUBJECT_REFERENCE;
        return slcli_bq_subject_route_equal(&valid,&valid)&&
            InvalidExpectedRoleBeforeEffects(0)&&
            InvalidExpectedRoleBeforeEffects(SLCLI_BQ_SUBJECT_REFERENCE+1)&&
            InvalidInitialStageBeforeEffects(0)&&InvalidInitialStageBeforeEffects(1)&&
            InvalidInitialStageBeforeEffects(2)&&InvalidInitialStageBeforeEffects(3)&&
            InvalidInitialStageBeforeEffects(4)&&
            InvalidCandidateGeometryBeforeEffects(0)&&InvalidCandidateGeometryBeforeEffects(1)&&
            InvalidCandidateGeometryBeforeEffects(2)&&InvalidCandidateGeometryBeforeEffects(3)&&
            InvalidCandidateGeometryBeforeEffects(4)&&
            AncillaryRetention(1,0)&&AncillaryRetention(2,0)&&
            AncillaryRetention(1,MSG_TRUNC)&&AncillaryRetention(1,MSG_CTRUNC)&&
            UnknownControlCannotDispose()&&UnwalkableControlCannotDispose()&&UnreturnedStatCannotDispose()&&
            DuplicateRightsCannotDispose()&&CapacityBeforeEffects()&&ReadabilityDisposition()&&
            ArchiveChronologyDisposition()&&ArchiveReceivePrefixDisposition()&&MissingArchiveCannotQualify();
    }
};
bool VerifyInitialProviderReceiptPreflightSourceRegression() {
    return InitialProviderReceiptSourceRegression::Verify();
}
}
int main(){return template_creator_linux::VerifyInitialProviderReceiptPreflightSourceRegression()?0:1;}
#endif

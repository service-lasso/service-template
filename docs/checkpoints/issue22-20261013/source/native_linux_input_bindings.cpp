// UNCOMPILED physical correspondence against SAME retained snapshot requirements.
// No writer exclusion, loaded execution view or native role grant follows.
#ifndef _WIN32
#include "native_linux_platform.hpp"
#include "linux_session_gate.hpp"
#include "linux_receipt_foundation_pair.hpp"
namespace template_creator_bridge {
static PyObject* InputFailure(NativePlatformState& owner,const char* reason) noexcept {
    owner.qualificationFailed=true;
    if(!PyErr_Occurred())PyErr_SetString(PyExc_ValueError,reason);
    return nullptr;
}
bool NativePlatformState::PreownInputLayout(std::size_t ordinal,const std::size_t* reads,std::size_t count){
    if(!storagePreowned){qualificationFailed=true;return false;}
    if(inputBindingAttempted||ordinal>=route.CommandCount())return false;
    auto& owner=inputOwners[ordinal];
    if(owner.layoutAttempted)return false;owner.layoutAttempted=true;
    const auto* layoutPair=pendingHost.ReceiptFoundationPairOriginal();
    const auto* layoutReceiver=pendingHost.InitialProviderOriginal();
    const auto* layoutSource=layoutPair?layoutPair->SourceOriginal():nullptr;
    if(!layoutPair||!layoutReceiver||!layoutSource||
       !layoutPair->SameOriginals(*layoutReceiver,*layoutSource,pending.OriginalDeadlineOwner(),pending.OriginalSupervisorOwner())||
       !slcli_bq_provider_expectations_bootstrap_shape(layoutSource,static_cast<uint32_t>(originalPhase))||
       layoutSource->command_count!=route.CommandCount()||ordinal>=layoutSource->command_count||
       count!=layoutSource->requirements_per_command[ordinal]){
        InputFailure(*this,"complete original Linux source input-layout count");return false;
    }
    constexpr std::size_t layoutRowBytes=sizeof(std::size_t)+sizeof(LinuxInterpreterRequirementOriginal)+sizeof(LinuxReceivedSubjectAssociationOriginal)+
        sizeof(std::unique_ptr<template_creator_linux::OriginalLinuxHeldFile>);
    if(!reads||!count||count>(SIZE_MAX-sizeof(owner))/layoutRowBytes){
        InputFailure(*this,"original Linux input layout geometry");return false;
    }
    owner.count=count; // Geometry survives either partial allocation failure.
    try{
        owner.selectedReadRows=std::make_unique<std::size_t[]>(count);
        owner.interpreterObservations=std::make_unique<LinuxInterpreterRequirementOriginal[]>(count);
        owner.receivedSubjectAssociations=std::make_unique<LinuxReceivedSubjectAssociationOriginal[]>(count);
        owner.files=std::make_unique<std::unique_ptr<template_creator_linux::OriginalLinuxHeldFile>[]>(count);
        for(std::size_t i=0;i<count;++i){
            if(!reads[i]){
                const auto* pair=pendingHost.ReceiptFoundationPairOriginal();
                const auto* receiver=pendingHost.InitialProviderOriginal();
                const auto* source=pair?pair->SourceOriginal():nullptr;
                if(ordinal!=0||i>1||!pair||!receiver||!source||
                   !pair->SameOriginals(*receiver,*source,pending.OriginalDeadlineOwner(),pending.OriginalSupervisorOwner())||
                   !pair->NativeCorrespondenceObserved()||
                   !slcli_bq_provider_expectations_bootstrap_shape(source,static_cast<uint32_t>(originalPhase))||
                   source->subjects[i].command!=ordinal||source->subjects[i].requirement!=i||
                   source->subjects[i].source_occurrence.reader_class!=(i==0?
                       SLCLI_BQ_EXPECTED_ARCHIVE_FOUNDATION_PREREQUISITE:SLCLI_BQ_EXPECTED_ARCHIVE_READBACK_PREREQUISITE)){
                    InputFailure(*this,"original Linux native prerequisite zero-read association");return false;
                }
            }else if((ordinal==0&&i<2&&pendingHost.ReceiptFoundationPairOriginal())||
                     reads[i]>SIZE_MAX/sizeof(template_creator_linux::HeldFileCall)){
                InputFailure(*this,"original Linux read roster geometry");return false;
            }
            owner.selectedReadRows[i]=reads[i];
        }
    }catch(...){owner.originalException=std::current_exception();qualificationFailed=true;throw;}
    owner.layoutBound=true;return true;
}
static bool RequirementOriginal(PyObject* row,PyObject*& path,std::size_t& extent,
    std::array<unsigned char,32>& digest) noexcept {
    if(!row||!PyTuple_Check(row)||PyTuple_GET_SIZE(row)!=4)return false;
    path=PyTuple_GET_ITEM(row,1);auto* hash=PyTuple_GET_ITEM(row,2);auto* size=PyTuple_GET_ITEM(row,3);
    if(!path||!PyUnicode_CheckExact(path)||!hash||!PyUnicode_CheckExact(hash)||
       PyUnicode_GET_LENGTH(hash)!=64||!size||!PyLong_CheckExact(size))return false;
    const auto bytes=PyLong_AsUnsignedLongLong(size);
    if(PyErr_Occurred()||bytes>SIZE_MAX||bytes>UINT64_MAX/8)return false;extent=static_cast<std::size_t>(bytes);
    for(Py_ssize_t i=0;i<64;++i){
        const auto c=PyUnicode_READ_CHAR(hash,i);
        const unsigned value=c>='0'&&c<='9'?c-'0':c>='a'&&c<='f'?c-'a'+10:c>='A'&&c<='F'?c-'A'+10:16;
        if(value==16)return false;
        if(!(i&1))digest[static_cast<std::size_t>(i)/2]=static_cast<unsigned char>(value<<4);
        else digest[static_cast<std::size_t>(i)/2]|=static_cast<unsigned char>(value);
    }
    return true;
}
static bool NativeArchiveRequirementOriginal(LinuxReceivedSubjectAssociationOriginal& original,
    PyObject* row,const slcli_bq_expected_source_occurrence& source) noexcept {
    if(!row||original.nativeRequirementDictionaryOriginal)return false;
    original.requirementOriginal=row;
    original.nativeRequirementDictionaryOriginal=PyObject_GenericGetDict(row,nullptr);
    auto* dictionary=original.nativeRequirementDictionaryOriginal;
    if(!dictionary||PyErr_Occurred()||!PyDict_CheckExact(dictionary)||PyDict_Size(dictionary)!=3)return false;
    auto* extent=PyDict_GetItemString(dictionary,"native_extent_bytes");
    if(!extent||!PyLong_CheckExact(extent))return false;
    const auto bytes=PyLong_AsUnsignedLongLong(extent);
    if(PyErr_Occurred()||bytes!=source.native_file_bytes)return false;
    const auto selector=[&](const char* name,const slcli_bq_expected_occurrence_selector& expected) noexcept {
        auto* value=PyDict_GetItemString(dictionary,name);
        if(!value||!PyTuple_CheckExact(value)||PyTuple_GET_SIZE(value)!=3)return false;
        const uint32_t fields[]={expected.phase,expected.command,expected.requirement};
        for(size_t n=0;n<3;++n){auto* field=PyTuple_GET_ITEM(value,static_cast<Py_ssize_t>(n));
            if(!field||!PyLong_CheckExact(field))return false;
            const auto actual=PyLong_AsUnsignedLongLong(field);
            if(PyErr_Occurred()||actual!=fields[n])return false;
        }
        return true;
    };
    return selector("archive_foundation",source.archive_foundation)&&selector("archive_readback",source.archive_readback);
}
PyObject* NativePlatformState::CaptureInputClasses(PyObject* ordinal,PyObject* step){
    if(!ordinal||!PyLong_CheckExact(ordinal)||!adopted||!stepsBound||qualificationFailed||
       inputBindingAttempted||inputDisposalStarted||!originalSteps||!PyTuple_CheckExact(originalSteps))
        return InputFailure(*this,"original Linux class-view phase");
    const size_t index=PyLong_AsSize_t(ordinal);
    if(PyErr_Occurred()||index>=route.CommandCount()||index>=static_cast<size_t>(PyTuple_GET_SIZE(originalSteps))||
       PyTuple_GET_ITEM(originalSteps,index)!=step||!step||!PyTuple_Check(step)||PyTuple_GET_SIZE(step)!=5)
        return InputFailure(*this,"same original Linux class-view step");
    auto& owner=inputOwners[index];
    if(owner.classViewAttempted||!owner.layoutBound)return InputFailure(*this,"original Linux class view already attempted");
    owner.classViewAttempted=true;
    auto* requirements=PyTuple_GET_ITEM(step,2);
    const auto* receiver=pendingHost.InitialProviderOriginal();
    const auto* pair=pendingHost.ReceiptFoundationPairOriginal();
    const auto* pairSource=pair?pair->SourceOriginal():nullptr;
    const auto* archive=receiver?receiver->OriginalArchiveOwner():nullptr;
    if(!archive||!requirements||!PyTuple_CheckExact(requirements)||PyTuple_GET_SIZE(requirements)!=static_cast<Py_ssize_t>(owner.count))
        return InputFailure(*this,"complete original Linux class-view archive");
    PyObject* result=PyTuple_New(static_cast<Py_ssize_t>(owner.count));if(!result)return nullptr;
    owner.classViewOriginal=Py_NewRef(result); // Preserve every completed prefix on later failure.
    for(size_t i=0;i<owner.count;++i){
        if(owner.selectedReadRows[i]==0){
            if(index!=0||i>1||!pair||!receiver||!pairSource||
               !pair->SameOriginals(*receiver,*pairSource,pending.OriginalDeadlineOwner(),pending.OriginalSupervisorOwner())||
               !pair->NativeCorrespondenceObserved()||
               !slcli_bq_provider_expectations_bootstrap_shape(pairSource,static_cast<uint32_t>(originalPhase))){
                Py_DECREF(result);return InputFailure(*this,"same native Linux pair class-view occurrence");
            }
            owner.receiptFoundationPairOriginal=pair;
            const auto& occurrence=pairSource->subjects[i].source_occurrence;
            PyObject* value=Py_BuildValue("(IKKK)",occurrence.reader_class,
                static_cast<unsigned long long>(occurrence.native_file_bytes),0ULL,0ULL);
            if(!value){Py_DECREF(result);return nullptr;}
            PyTuple_SET_ITEM(result,static_cast<Py_ssize_t>(i),value);
            continue; // Native geometry is independently checked by typed Python capture.
        }
        const template_creator_linux::OriginalInitialProviderArchive::CutOriginal* found=nullptr;
        for(size_t n=1;n<archive->OriginalCutCount();++n){const auto* cut=archive->OriginalCut(n);
            if(!cut||!cut->offer||cut->offer->sourceRoute.phase!=static_cast<uint32_t>(originalPhase)||
               cut->offer->sourceRoute.command!=index||cut->offer->sourceRoute.requirement!=i)continue;
            if(found){Py_DECREF(result);return InputFailure(*this,"ambiguous Linux class-view occurrence");}found=cut;
        }
        if(!found||found->owner!=archive||!found->prefixRetained||bool(found->file)==bool(found->unit)){
            Py_DECREF(result);return InputFailure(*this,"missing Linux class-view occurrence");}
        PyObject* value=nullptr;
        if(found->unit){const auto& unit=*found->unit;const auto& subject=unit.subject_;
            const auto& occurrence=subject.source_occurrence;
            PyObject* path=nullptr;size_t extent=0;std::array<unsigned char,32> digest{};
            if(!RequirementOriginal(PyTuple_GET_ITEM(requirements,static_cast<Py_ssize_t>(i)),path,extent,digest)||
               subject.phase!=static_cast<uint32_t>(originalPhase)||subject.command!=index||subject.requirement!=i||
               occurrence.read_bytes!=extent||occurrence.native_file_bytes!=unit.source_.source_unit_bytes||
               occurrence.read_offset>occurrence.native_file_bytes||occurrence.read_bytes>occurrence.native_file_bytes-occurrence.read_offset||
               std::memcmp(subject.body_digest,digest.data(),digest.size())){
                Py_DECREF(result);return InputFailure(*this,"original Linux class-view requirement geometry mismatch");}
            if(!unit.observed_||unit.borrowReleased_||!unit.tableObserved_||unit.parsed_!=unit.recordCount_||
               !unit.associationRowsObserved_||unit.associationRowsCompared_!=unit.recordCount_||!unit.associationOriginal_||
               &unit.receiver_!=receiver||unit.offerOrdinal_!=found->ordinal||
               (occurrence.reader_class!=SLCLI_BQ_EXPECTED_UNIT_PREREQUISITE&&occurrence.reader_class!=SLCLI_BQ_EXPECTED_SP1_MEMBER_RANGE)||
               (occurrence.reader_class==SLCLI_BQ_EXPECTED_SP1_MEMBER_RANGE&&!unit.memberObserved_)){
                Py_DECREF(result);return InputFailure(*this,"unsettled Linux class-view source unit");}
            value=Py_BuildValue("(IKKK)",occurrence.reader_class,
                static_cast<unsigned long long>(unit.source_.source_unit_bytes),
                static_cast<unsigned long long>(occurrence.read_offset),static_cast<unsigned long long>(occurrence.read_bytes));
        }else value=Py_NewRef(Py_None);
        if(!value){Py_DECREF(result);return nullptr;}PyTuple_SET_ITEM(result,static_cast<Py_ssize_t>(i),value);
    }
    return result; // Retained native observation geometry, never source/member authority.
}
PyObject* NativePlatformState::BindInputs(PyObject* inputs,PyObject* snapshots){
    if(inputBindingAttempted||inputDisposalStarted)return InputFailure(*this,"original Linux input binding phase closed");
    inputBindingAttempted=true;originalInputs=Py_XNewRef(inputs);originalSnapshots=Py_XNewRef(snapshots);
    if(!adopted||!stepsBound||qualificationFailed||!inputs||!PyTuple_CheckExact(inputs)||
       !snapshots||!PyTuple_CheckExact(snapshots)||
       PyTuple_GET_SIZE(snapshots)!=static_cast<Py_ssize_t>(route.CommandCount())||
       PyTuple_GET_SIZE(inputs)!=PyTuple_GET_SIZE(snapshots))
        return InputFailure(*this,"complete original Linux snapshot roster required");
    if(!creatorHeldInputs||!creatorHeldInputs->inputsCaptured||inputs!=creatorHeldInputs->inputs||
       !creatorHeldInputs->CurrentFieldIsOriginal()||!originalSteps)
        return InputFailure(*this,"same retained entry Linux input tuple required");
    // All paths, bodies, read rows and file owner cells exist before first open.
    // The source tuple remains strongly retained; UTF-8 temporary copies below
    // are preparation allocations, not authentic source or whole-fit evidence.
    for(std::size_t ordinal=0;ordinal<route.CommandCount();++ordinal){
        auto& owner=inputOwners[ordinal];owner.snapshot=Py_NewRef(PyTuple_GET_ITEM(snapshots,ordinal));
        // SAME native host acquisition owner, never a Python-supplied pointer
        // or an authenticated-member result. Missing producer remains closed
        // at the independent useful consumer rather than fabricated here.
        owner.initialProviderOriginal=pendingHost.InitialProviderOriginal();
        if(!owner.layoutBound||!PyTuple_Check(owner.snapshot)||PyTuple_GET_SIZE(owner.snapshot)!=5)
            return InputFailure(*this,"original Linux input layout/snapshot absent");
        owner.commandInputsOriginal=Py_NewRef(PyTuple_GET_ITEM(inputs,ordinal));
        owner.commandInputsDictionary=PyObject_GenericGetDict(owner.commandInputsOriginal,nullptr);
        if(!owner.commandInputsDictionary||PyErr_Occurred()||!PyDict_CheckExact(owner.commandInputsDictionary))
            return InputFailure(*this,"original Linux command input dictionary absent");
        if(!owner.classViewAttempted||!owner.classViewOriginal||
           PyDict_GetItemString(owner.commandInputsDictionary,"_native_classes")!=owner.classViewOriginal)
            return InputFailure(*this,"same retained Linux input class-view result required");
        owner.capturedLookupAttempted=true;
        owner.capturedLookupResult=PyDict_GetItemStringRef(owner.commandInputsDictionary,"_captured",&owner.capturedFieldOriginal);
        owner.capturedLookupReturned=true;
        if(owner.capturedLookupResult<0||PyErr_Occurred())return InputFailure(*this,"original Linux snapshot lookup failed");
        owner.heldLookupAttempted=true;
        owner.heldLookupResult=PyDict_GetItemStringRef(owner.commandInputsDictionary,"held",&owner.heldFieldOriginal);
        owner.heldLookupReturned=true;
        if(owner.heldLookupResult<0||PyErr_Occurred())return InputFailure(*this,"original Linux held owner lookup failed");
        auto* selectedStep=PyTuple_GET_ITEM(originalSteps,ordinal);
        if(owner.capturedLookupResult!=1||owner.capturedFieldOriginal!=owner.snapshot||
           owner.heldLookupResult!=1||!creatorHeldInputs->Contains(ordinal,owner.heldFieldOriginal)||
           !selectedStep||!PyTuple_Check(selectedStep)||PyTuple_GET_SIZE(selectedStep)!=5||
           PyTuple_GET_ITEM(owner.snapshot,0)!=PyTuple_GET_ITEM(selectedStep,1))
            return InputFailure(*this,"same original Linux command/input/snapshot association required");
        auto* requirements=PyTuple_GET_ITEM(owner.snapshot,4);
        if(!requirements||requirements!=PyTuple_GET_ITEM(selectedStep,2)||!PyTuple_CheckExact(requirements)||
           PyTuple_GET_SIZE(requirements)!=static_cast<Py_ssize_t>(owner.count))
            return InputFailure(*this,"original Linux requirement/layout count mismatch");
        owner.sourceAssociated=true; // Source identity only; no execution-view grant.
        {
            auto* command=PyTuple_GET_ITEM(owner.snapshot,0);
            if(!command||!PyTuple_Check(command)||PyTuple_GET_SIZE(command)!=6)
                return InputFailure(*this,"original Linux cwd command shape");
            auto* cwd=PyTuple_GET_ITEM(command,2);
            if(!cwd||!PyUnicode_CheckExact(cwd))return InputFailure(*this,"original Linux cwd string");
            std::size_t bytes=0;
            for(Py_ssize_t i=0;i<PyUnicode_GET_LENGTH(cwd);++i){
                const auto c=PyUnicode_READ_CHAR(cwd,i);
                if(!c||c>0x10ffff||(c>=0xd800&&c<=0xdfff))return InputFailure(*this,"original Linux cwd scalar");
                const std::size_t width=c<128?1:c<2048?2:c<65536?3:4;
                // SAME capacity as the actual retained native command plan.
                if(width>32767-bytes)return InputFailure(*this,"original Linux cwd plan capacity");
                bytes+=width;
            }
            auto* encoded=PyUnicode_AsUTF8String(cwd);
            if(!encoded||PyErr_Occurred()){Py_XDECREF(encoded);return InputFailure(*this,"original Linux cwd encoding");}
            try{owner.cwd=std::make_unique<template_creator_linux::OriginalLinuxCommandDirectory>(
                PyBytes_AS_STRING(encoded),bytes,initialObjects,pending.OriginalDeadlineOwner(),pending.OriginalSupervisorOwner());}
            catch(...){owner.originalException=std::current_exception();qualificationFailed=true;Py_DECREF(encoded);throw;}
            Py_DECREF(encoded);
            if(!route.PreownSnapshot(ordinal,owner.snapshot))return InputFailure(*this,"original Linux route plan precharge");
            try{owner.gate=std::make_unique<OriginalLinuxSessionGate>(*this,owner,ordinal);}
            catch(...){owner.originalException=std::current_exception();qualificationFailed=true;throw;}
        }
        for(std::size_t i=0;i<owner.count;++i){
            if(owner.selectedReadRows[i]==0){
                const auto* pair=owner.receiptFoundationPairOriginal;
                const auto* source=pair?pair->SourceOriginal():nullptr;
                if(ordinal!=0||i>1||!pair||pair!=pendingHost.ReceiptFoundationPairOriginal()||
                   !source||!owner.initialProviderOriginal||
                   !pair->SameOriginals(*owner.initialProviderOriginal,*source,pending.OriginalDeadlineOwner(),pending.OriginalSupervisorOwner())||
                   !pair->NativeCorrespondenceObserved()||
                   !slcli_bq_provider_expectations_bootstrap_shape(source,static_cast<uint32_t>(originalPhase))||
                   !NativeArchiveRequirementOriginal(owner.receivedSubjectAssociations[i],
                       PyTuple_GET_ITEM(requirements,static_cast<Py_ssize_t>(i)),source->subjects[i].source_occurrence))
                    return InputFailure(*this,"same original Linux typed native requirement");
                owner.receivedSubjectAssociations[i].receiptFoundationPairOriginal=pair;
                continue; // Native references retain their receiver custody, no path/body owner.
            }
            PyObject* path=nullptr;std::size_t extent=0;std::array<unsigned char,32> digest{};
            if(!RequirementOriginal(PyTuple_GET_ITEM(requirements,i),path,extent,digest))
                return InputFailure(*this,"original Linux requirement shape");
            const auto* archive=owner.initialProviderOriginal?owner.initialProviderOriginal->OriginalArchiveOwner():nullptr;
            const template_creator_linux::OriginalInitialProviderArchive::CutOriginal* received=nullptr;
            if(!archive)return InputFailure(*this,"original Linux requirement archive absent before preown");
            for(size_t n=1;n<archive->OriginalCutCount();++n){const auto* cut=archive->OriginalCut(n);
                if(!cut||!cut->offer||cut->offer->sourceRoute.phase!=static_cast<uint32_t>(originalPhase)||
                   cut->offer->sourceRoute.command!=ordinal||cut->offer->sourceRoute.requirement!=i)continue;
                if(received)return InputFailure(*this,"original Linux requirement archive ambiguous before preown");
                received=cut;
            }
            if(!received||received->owner!=archive||!received->prefixRetained)
                return InputFailure(*this,"original Linux requirement archive incomplete before preown");
            auto mode=template_creator_linux::OriginalLinuxHeldFile::CaptureMode::CompleteFile;
            size_t nativeBytes=extent;
            if(received->unit){
                if(received->file||!received->unit->observed_||received->unit->borrowReleased_||
                   received->unit->source_.source_unit_bytes>SIZE_MAX)
                    return InputFailure(*this,"original Linux source-unit class unresolved before preown");
                owner.receivedSubjectAssociations[i].sourceUnit=received->unit;
                nativeBytes=static_cast<size_t>(received->unit->source_.source_unit_bytes);
                mode=template_creator_linux::OriginalLinuxHeldFile::CaptureMode::SourceUnitIdentity;
            }
            // Preflight exact scalar encoding before asking CPython for a copy.
            std::size_t pathBytes=0;
            for(Py_ssize_t c=0;c<PyUnicode_GET_LENGTH(path);++c){
                const auto scalar=PyUnicode_READ_CHAR(path,c);
                if(!scalar||scalar>0x10ffff||(scalar>=0xd800&&scalar<=0xdfff))
                    return InputFailure(*this,"original Linux path scalar/NUL");
                const std::size_t width=scalar<128?1:scalar<2048?2:scalar<65536?3:4;
                if(width>SIZE_MAX-pathBytes-1)return InputFailure(*this,"original Linux path overflow");pathBytes+=width;
            }
            if(!pathBytes||PyUnicode_READ_CHAR(path,0)!='/')return InputFailure(*this,"absolute original Linux input path required");
            auto* encoded=PyUnicode_AsEncodedString(path,"utf-8","strict");
            if(!encoded)return InputFailure(*this,"original Linux path encoding failed");
            try{
                owner.files[i]=std::make_unique<template_creator_linux::OriginalLinuxHeldFile>(
                    template_creator_linux::DeferredOriginalPreown,PyBytes_AS_STRING(encoded),pathBytes,nativeBytes,digest.data(),owner.selectedReadRows[i],
                    pending.OriginalDeadlineOwner(),pending.OriginalSupervisorOwner(),mode);
                if(!owner.files[i]->PreownStorageOriginal())std::rethrow_exception(owner.files[i]->PreownExceptionOriginal());
            }catch(...){owner.originalException=std::current_exception();qualificationFailed=true;Py_DECREF(encoded);throw;}
            Py_DECREF(encoded);
        }
    }
    // Preown candidate referents before ANY held-file native capture. Source
    // path selection is not kernel lookup or compiled-image authority.
    for(std::size_t ordinal=0;ordinal<route.CommandCount();++ordinal){
            auto& owner=inputOwners[ordinal];const auto& plan=route.PrivateCommand(ordinal).plan;
            for(std::size_t i=0;i<owner.count;++i)if(owner.files[i]&&
                !std::strcmp(owner.files[i]->PrivateOriginalPath(),plan.image.data())){
                if(owner.files[i]->OriginalCaptureMode()!=template_creator_linux::OriginalLinuxHeldFile::CaptureMode::CompleteFile)
                    return InputFailure(*this,"source-unit range cannot replace original executable");
                if(owner.kernelExeCandidate)continue; // Same first original requirement policy.
                const auto& original=*owner.files[i];
                try{owner.kernelExeCandidate=std::make_unique<template_creator_linux::OriginalLinuxHeldFile>(
                    template_creator_linux::DeferredOriginalPreown,original.PrivateOriginalPath(),original.OriginalPathBytes(),original.BodyCapacity(),
                    original.OriginalExpectedDigest().data(),original.OriginalReadCapacity(),
                    pending.OriginalDeadlineOwner(),pending.OriginalSupervisorOwner());
                    if(!owner.kernelExeCandidate->PreownStorageOriginal())std::rethrow_exception(owner.kernelExeCandidate->PreownExceptionOriginal());}
                catch(...){owner.originalException=std::current_exception();qualificationFailed=true;throw;}
                owner.kernelExeSourceOrdinal=i;
            }
            if(!owner.kernelExeCandidate)return InputFailure(*this,"original Linux kernel executable candidate geometry absent");
        }
    for(std::size_t ordinal=0;ordinal<route.CommandCount();++ordinal){
        auto& owner=inputOwners[ordinal];owner.captureAttempted=true;
        for(std::size_t i=0;i<owner.count;++i){
            if(!owner.selectedReadRows[i]){
                if(owner.files[i]||!owner.receivedSubjectAssociations[i].receiptFoundationPairOriginal)
                    return InputFailure(*this,"original Linux native requirement custody missing");
                continue;
            }
            if(!owner.files[i]||!owner.files[i]->Capture())
                return InputFailure(*this,"original Linux held input capture failed");
        }
        owner.captureClean=true;
        if(!owner.initialProviderOriginal||!owner.receivedSubjectAssociations)
            return InputFailure(*this,"original Linux acquired-subject containing graph absent");
        const auto* archive=owner.initialProviderOriginal->OriginalArchiveOwner();
        auto* requirements=PyTuple_GET_ITEM(owner.snapshot,4);
        for(std::size_t i=0;i<owner.count;++i){
            auto& association=owner.receivedSubjectAssociations[i];association.attempted=true;
            association.receiver=owner.initialProviderOriginal;association.archive=archive;
            association.command=ordinal;association.requirement=i;
            association.requirementOriginal=PyTuple_GET_ITEM(requirements,i);association.heldFile=owner.files[i].get();
            association.result=LinuxReceivedSubjectAssociationOriginal::Result::Missing;
            if(!owner.selectedReadRows[i]){
                const auto* pair=association.receiptFoundationPairOriginal;
                if(!pair||pair!=owner.receiptFoundationPairOriginal||!pair->NativeCorrespondenceObserved()||
                   owner.files[i]||!association.nativeRequirementDictionaryOriginal)
                    return InputFailure(*this,"original Linux native pair correspondence missing");
                if(!template_creator_linux::IngressNativeClock::Capture(association.finalClock,&pending.OriginalDeadlineOwner(),
                       "native-pair-requirement-correspondence",true)||!association.finalClock.shapeValid||
                   association.finalClock.expired||
                   association.finalClock.nanoseconds<pair->FinalCorrespondenceClockOriginal().nanoseconds)
                    return InputFailure(*this,"original Linux native pair requirement final clock unresolved");
                association.result=LinuxReceivedSubjectAssociationOriginal::Result::MatchedNativeArchiveCorrespondence;
                continue; // No body/readback/source/archive authority from correspondence.
            }
            if(!archive)return InputFailure(*this,"original Linux received archive absent");
            for(size_t cutOrdinal=1;cutOrdinal<archive->OriginalCutCount();++cutOrdinal){
                const auto* cut=archive->OriginalCut(cutOrdinal);
                if(!cut||!cut->offer||cut->offer->sourceRoute.phase!=static_cast<uint32_t>(originalPhase)||
                   cut->offer->sourceRoute.command!=ordinal||cut->offer->sourceRoute.requirement!=i)continue;
                if(association.cut){association.result=LinuxReceivedSubjectAssociationOriginal::Result::Ambiguous;
                    return InputFailure(*this,"original Linux received requirement ambiguous");}
                association.cut=cut;
            }
            const auto* cut=association.cut;
            if(!cut)return InputFailure(*this,"original Linux received requirement missing");
            association.result=LinuxReceivedSubjectAssociationOriginal::Result::Invalid;
            if(cut->owner!=archive||!cut->prefixRetained||(!cut->file&&!cut->unit)||(cut->file&&cut->unit)||
               (cut->file&&!cut->file->ReadbackObserved())||
               cut->offer!=owner.initialProviderOriginal->OriginalOffer(cut->ordinal)||cut->offer->rightsCount!=1||
               cut->offer->rights[0].closeAttempted||!cut->offer->rights[0].statReturned||
               cut->offer->rights[0].statResult!=0||!owner.files[i]->CapturedNativeIdentity())
                return InputFailure(*this,"original Linux received requirement native graph unresolved");
            const auto& received=cut->offer->rights[0].status;
            const auto& held=owner.files[i]->PrivateAfter().metadata;
            association.bytes=owner.files[i]->RetainedByteCount();
            const bool identity=received.st_dev==held.st_dev&&received.st_ino==held.st_ino&&
                received.st_mode==held.st_mode&&received.st_size==held.st_size&&
                received.st_mtim.tv_sec==held.st_mtim.tv_sec&&received.st_mtim.tv_nsec==held.st_mtim.tv_nsec&&
                received.st_ctim.tv_sec==held.st_ctim.tv_sec&&received.st_ctim.tv_nsec==held.st_ctim.tv_nsec;
            association.result=LinuxReceivedSubjectAssociationOriginal::Result::Different;
            const bool body=cut->unit?
                (association.sourceUnit==cut->unit&&cut->unit->observed_&&!cut->unit->borrowReleased_&&
                 owner.files[i]->OriginalCaptureMode()==template_creator_linux::OriginalLinuxHeldFile::CaptureMode::SourceUnitIdentity):
                (owner.files[i]->CapturedSourceBytes()&&association.bytes==cut->file->RetainedBytes()&&
                 (!association.bytes||!std::memcmp(owner.files[i]->PrivateBody(),cut->file->OriginalBody(),association.bytes)));
            if(!identity||!body)
                return InputFailure(*this,"original Linux received versus held requirement differs");
            if(!template_creator_linux::IngressNativeClock::Capture(association.finalClock,&pending.OriginalDeadlineOwner(),
                   "received-held-requirement-correspondence",true)||!association.finalClock.shapeValid||
               association.finalClock.expired||
               association.finalClock.nanoseconds<owner.files[i]->PrivateAfter().clock.after.nanoseconds||
               association.finalClock.nanoseconds<cut->finalClock.nanoseconds)
                return InputFailure(*this,"original Linux received requirement final clock unresolved");
            association.result=cut->unit?LinuxReceivedSubjectAssociationOriginal::Result::MatchedNativeUnitIdentity:
                LinuxReceivedSubjectAssociationOriginal::Result::Matched;
            // Exact physical correspondence, never producer/source authority.
        }
    }
    if(!initialObjects.CaptureOriginals())
        return InputFailure(*this,"original Linux initial objects capture failed");
    if(!finalOutput.CreateBeforeIngressClosure(initialObjects))
        return InputFailure(*this,"original Linux final output acquisition failed");
    {
        for(std::size_t ordinal=0;ordinal<route.CommandCount();++ordinal){
            auto& owner=inputOwners[ordinal];
            auto* command=PyTuple_GET_ITEM(owner.snapshot,0);
            auto* image=PyTuple_GET_ITEM(command,0);
            if(!image||!PyUnicode_CheckExact(image))return InputFailure(*this,"original Linux image string");
            auto* requirements=PyTuple_GET_ITEM(owner.snapshot,4);
            for(std::size_t i=0;i<owner.count;++i){
                if(!owner.selectedReadRows[i])continue;
                auto* path=PyTuple_GET_ITEM(PyTuple_GET_ITEM(requirements,i),1);
                const int comparison=PyUnicode_Compare(image,path);
                if(PyErr_Occurred())return InputFailure(*this,"original Linux image/path comparison");
                if(comparison)continue;
                if(!owner.files[i]||!initialObjects.SameCapturedTool(*owner.files[i]))
                    return InputFailure(*this,"original Linux launch file/tool object mismatch");
                if(owner.launchFileOrdinal==SIZE_MAX)owner.launchFileOrdinal=i;
            }
            owner.launchFileAssociated=owner.launchFileOrdinal!=SIZE_MAX;
            if(!owner.launchFileAssociated)return InputFailure(*this,"original Linux launch image input absent");
            for(std::size_t i=0;i<owner.count;++i){
                if(!owner.selectedReadRows[i])continue;
                auto* row=PyTuple_GET_ITEM(requirements,i);auto* role=PyTuple_GET_ITEM(row,0);
                if(!role||!PyUnicode_CheckExact(role))return InputFailure(*this,"original Linux requirement role shape");
                const int roleComparison=PyUnicode_CompareWithASCIIString(role,"interpreter");
                if(PyErr_Occurred())return InputFailure(*this,"original Linux interpreter role comparison");
                if(roleComparison==0){
                    if(owner.receivedSubjectAssociations[i].sourceUnit){
                        auto& observation=owner.interpreterObservations[i];
                        observation.requirementOriginal=row;observation.candidateOriginal=owner.files[i].get();
                        observation.launchOriginal=owner.files[owner.launchFileOrdinal].get();
                        observation.attempted=observation.returned=true;
                        observation.result=LinuxInterpreterCorrespondence::SourceRangeNotApplicable;
                        continue; // Explicit source-range class, not a fabricated ELF nonmatch.
                    }
                    const auto result=static_cast<OriginalLinuxSessionGate*>(owner.gate.get())->ObserveInterpreterRequirementOriginal(i,row);
                    if(result==LinuxInterpreterCorrespondence::Invalid||result==LinuxInterpreterCorrespondence::Unobserved)
                        return InputFailure(*this,"original Linux interpreter correspondence observation failed");
                }
            }
            try {
                if(!static_cast<OriginalLinuxSessionGate*>(owner.gate.get())->PrepareInterpreterMemoryBackingOriginal())
                    return InputFailure(*this,"original Linux interpreter memory precharge failed");
            } catch(...) {owner.originalException=std::current_exception();qualificationFailed=true;throw;}
        }
    }
    for(std::size_t ordinal=0;ordinal<route.CommandCount();++ordinal)
            if(!inputOwners[ordinal].cwd||!inputOwners[ordinal].cwd->Capture())
                return InputFailure(*this,"original Linux cwd/root object association failed");
    if(!protectedStdin.CaptureProtectedOriginal())
        return InputFailure(*this,"original Linux protected EOF stdin capture failed");
    {
        for(std::size_t ordinal=0;ordinal<route.CommandCount();++ordinal){
            auto& owner=inputOwners[ordinal];
            if(!owner.gate||!owner.cwd||!route.Bind(ordinal,owner.snapshot,*owner.gate,
                protectedStdin.OriginalReader(),owner.cwd->OriginalDescriptor()))
                return InputFailure(*this,"original Linux concrete route binding failed");
        }
        if(!route.Seal()||!commands.BindSnapshots(snapshots))
            return InputFailure(*this,"original Linux route seal/token binding failed");
    }
    inputCapturesClean=true;
    if(!RetainPrelaunchOriginalAssociations())
        return InputFailure(*this,"original Linux prelaunch input/caller/tool/root associations incomplete");
    Py_RETURN_NONE; // Physical binding only; concrete route/gate admission separate.
}
}
#endif

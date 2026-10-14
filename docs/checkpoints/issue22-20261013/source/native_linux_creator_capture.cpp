// UNCOMPILED retained final references and actual native prefix readback.
// Neither ABI records nor reference counts serialize Python referent graphs.
#ifndef _WIN32
#include "native_linux_platform.hpp"
#include "linux_session_gate.hpp"
namespace template_creator_bridge {
PyObject* NativePlatformState::CaptureCreator(PyObject* originals,PyObject* failures){
    const auto fail=[&](const char* reason)->PyObject*{
        qualificationFailed=true;if(!PyErr_Occurred())PyErr_SetString(PyExc_RuntimeError,reason);return nullptr;
    };
    if(creatorCaptureAttempted)return fail("original Linux final creator capture already attempted");
    creatorCaptureAttempted=true;
    creatorOriginals=Py_XNewRef(originals);creatorFailures=Py_XNewRef(failures);
    if(!sinkPrepared||!originals||!PyTuple_CheckExact(originals)||PyTuple_GET_SIZE(originals)!=4||
       PyTuple_GET_ITEM(originals,0)!=originalIngressIdentity||!failures||!PyTuple_CheckExact(failures)||
       PyTuple_GET_SIZE(failures)!=3)return fail("same Linux private creator original groups required");
    auto* rows=PyTuple_GET_ITEM(failures,0);
    if(!rows||!PyTuple_CheckExact(rows))return fail("original Linux retained failure roster required");
    qualificationFailed|=PyTuple_GET_SIZE(rows)!=0||PyTuple_GET_ITEM(failures,1)!=Py_None||PyTuple_GET_ITEM(failures,2)!=Py_None;
    const bool originalHeld=creatorHeldInputs&&creatorHeldInputs->captured&&
        PyTuple_GET_ITEM(originals,1)==creatorHeldInputs->tuple;
    const bool currentHeld=creatorHeldInputs&&creatorHeldInputs->CurrentFieldIsOriginal();
    qualificationFailed|=!originalHeld||!currentHeld;
    if(PyErr_Occurred())return nullptr;
    bool inputsAppended=true;
    const auto appendOriginal=[&](const void* bytes,std::size_t extent) noexcept {
        const bool actual=sink.Append(static_cast<const unsigned char*>(bytes),extent);
        inputsAppended=actual&&inputsAppended;
    };
    struct InitialRecord {std::uint64_t version,kind,rows;};
    const InitialRecord initialHeader{1,8,3};
    appendOriginal(&initialHeader,sizeof(initialHeader));
    appendOriginal(initialObjects.PrivateOriginals(),3*sizeof(template_creator_linux::InitialObjectStat));
    // Copy each actual native owner's inline originals AND separate referents.
    // Raw ABI snapshots alone never stand in for path/body/read storage.
    for(std::size_t ordinal=0;ordinal<route.CommandCount();++ordinal){
        const auto& owner=inputOwners[ordinal];
        struct PrelaunchRecord {std::uint64_t version,kind,commandOrdinal,extent;};
        const PrelaunchRecord prelaunchHeader{1,10,ordinal,sizeof(owner.prelaunchOriginal)};
        appendOriginal(&prelaunchHeader,sizeof(prelaunchHeader));
        appendOriginal(&owner.prelaunchOriginal,sizeof(owner.prelaunchOriginal));
        if(owner.interpreterObservations){
            const PrelaunchRecord interpreterHeader{1,11,ordinal,owner.count*sizeof(LinuxInterpreterRequirementOriginal)};
            appendOriginal(&interpreterHeader,sizeof(interpreterHeader));
            appendOriginal(owner.interpreterObservations.get(),owner.count*sizeof(LinuxInterpreterRequirementOriginal));
        }
        if(owner.receivedSubjectAssociations){
            if(owner.count>SIZE_MAX/sizeof(LinuxReceivedSubjectAssociationOriginal)){
                inputsAppended=false;qualificationFailed=true;
            }else{
                const auto extent=owner.count*sizeof(LinuxReceivedSubjectAssociationOriginal);
                const PrelaunchRecord receivedHeader{1,12,ordinal,extent};
                appendOriginal(&receivedHeader,sizeof(receivedHeader));
                appendOriginal(owner.receivedSubjectAssociations.get(),extent);
            }
        }
        if(owner.gate){
            const bool captured=static_cast<OriginalLinuxSessionGate*>(owner.gate.get())->CaptureOriginals(sink);
            inputsAppended=captured&&inputsAppended;
        }
        if(owner.cwd){
            const auto& cwd=*owner.cwd;
            struct DirectoryRecord {std::uint64_t version,kind,commandOrdinal,pathBytes;};
            const DirectoryRecord header{1,9,ordinal,cwd.OriginalPathBytes()+1};
            appendOriginal(&header,sizeof(header));
            appendOriginal(&cwd.PrivateOpen(),sizeof(cwd.PrivateOpen()));
            appendOriginal(&cwd.PrivateStat(),sizeof(cwd.PrivateStat()));
            appendOriginal(&cwd.PrivateClose(),sizeof(cwd.PrivateClose()));
            appendOriginal(&cwd.PrivateMetadata(),sizeof(cwd.PrivateMetadata()));
            appendOriginal(cwd.PrivateOriginalPath(),cwd.OriginalPathBytes()+1);
        }
        for(std::size_t i=0;i<owner.count;++i)if(owner.files&&owner.files[i]){
            const auto& file=*owner.files[i];
            const auto append=[&](const void* bytes,std::size_t extent) noexcept {
                const bool actual=sink.Append(static_cast<const unsigned char*>(bytes),extent);
                inputsAppended=actual&&inputsAppended;
            };
            struct InputRecord {std::uint64_t version,kind,commandOrdinal,fileOrdinal,pathBytes,bodyBytes,readRows;};
            const InputRecord header{1,7,ordinal,i,file.OriginalPathBytes()+1,file.RetainedByteCount(),file.ReadCount()};
            append(&header,sizeof(header));append(&file,sizeof(file));
            append(file.PrivateOriginalPath(),file.OriginalPathBytes()+1);
            if(file.RetainedByteCount()>file.BodyCapacity()||file.ReadCount()>SIZE_MAX/sizeof(template_creator_linux::HeldFileCall)){
                inputsAppended=false;qualificationFailed=true;continue;
            }
            append(file.PrivateBody(),file.RetainedByteCount());
            append(file.PrivateReads(),file.ReadCount()*sizeof(template_creator_linux::HeldFileCall));
        }
    }
    struct FinalOriginalRecord {
        std::uint64_t version,kind,groups,failureRows,copied,written,failed;
    };
    const FinalOriginalRecord record{1,6,4,static_cast<std::uint64_t>(PyTuple_GET_SIZE(rows)),
        sink.OriginalCopiedExtent(),sink.OriginalWrittenExtent(),qualificationFailed?1u:0u};
    creatorRecordAppended=sink.Append(reinterpret_cast<const unsigned char*>(&record),sizeof(record))&&inputsAppended;
    // Independent completed-prefix capture remains eligible after failed append.
    creatorPrefixReadback=sink.FlushReadbackOriginal();
    if(!creatorRecordAppended||!creatorPrefixReadback)return fail("original Linux creator prefix capture failed");
    Py_RETURN_NONE;
    // Actual independent final receiver/host/observer/disposition closure is
    // still required; this method grants neither qualification nor host return.
}
}
#endif

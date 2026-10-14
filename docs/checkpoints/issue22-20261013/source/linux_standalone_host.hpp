// UNCOMPILED resident actual Linux native caller. Source/private capabilities
// are supplied to this closed native constructor, never Python numeric handles.
#pragma once
#ifndef _WIN32
#include "native_session_registry.hpp"
#include "native_linux_platform.hpp"
#include <vector>
namespace template_creator_bridge {
struct OriginalLinuxHostSelection {
    OriginalPhase phase=OriginalPhase::Generation;
    std::size_t dispatchRows=0,hostObservations=0,commands=0,sinkBytes=0,sinkCalls=0;
    template_creator_linux::PendingInputGeometry geometry{};
    bool hasDeadline=false;std::uint64_t absoluteNanoseconds=0;
    std::size_t callerStack=0,callerJoins=0,sinkOrdinal=SIZE_MAX;
    std::size_t sourceRootOrdinal=SIZE_MAX,toolOrdinal=SIZE_MAX,evidenceRootOrdinal=SIZE_MAX;
    int procRoot=-1;
    const unsigned char* body=nullptr;std::size_t bodyBytes=0;
    const int* capabilities=nullptr;std::size_t capabilityCount=0;
    const std::size_t* const* inputReadRows=nullptr;
    const std::size_t* inputCounts=nullptr;
    template_creator_linux::OriginalLinuxPendingHost* preInterpreterHost=nullptr;
    template_creator_linux::OriginalLinuxHeldFile* selectionOriginal=nullptr;
};
class OriginalLinuxStandaloneHost {
    RetainedNativeRegistry& registry_;
    std::vector<unsigned char> bodyCopy_;
    std::vector<int> capabilityCopy_;
    std::vector<std::vector<std::size_t>> readCopy_;
    std::vector<const std::size_t*> readPointers_;
    std::vector<std::size_t> countsCopy_;
    ~OriginalLinuxStandaloneHost()=default; // Native main retains unresolved owner.
    void RetainError(PyObject*&,PyObject*&,PyObject*&) noexcept;
public:
    OriginalLinuxHostSelection originalSelection{};
    NativeSessionCore* core=nullptr;
    NativePlatformState* platform=nullptr;
    PyObject *wrapper=nullptr,*stepsOriginal=nullptr,*bridgeModule=nullptr,*bridgeClass=nullptr,*bridgeOriginal=nullptr;
    PyObject *creatorModule=nullptr,*creatorClass=nullptr,*phaseClass=nullptr,*phaseOriginal=nullptr,*entryOriginal=nullptr;
    PyObject *constructionType=nullptr,*constructionValue=nullptr,*constructionTraceback=nullptr;
    PyObject *connectType=nullptr,*connectValue=nullptr,*connectTraceback=nullptr;
    PyObject *invocationResult=nullptr;
    std::exception_ptr connectException;
    bool connectAttempted=false,selectionCopied=false,connected=false,constructionAttempted=false,invokeAttempted=false;
    explicit OriginalLinuxStandaloneHost(RetainedNativeRegistry& registry):registry_(registry){}
    OriginalLinuxStandaloneHost(const OriginalLinuxStandaloneHost&)=delete;
    OriginalLinuxStandaloneHost& operator=(const OriginalLinuxStandaloneHost&)=delete;
    bool Connect(const OriginalLinuxHostSelection&);
    PyObject* ConstructCreatorEntry(PyObject* actualSelectedSteps);
    PyObject* InvokeCreator();
    bool ObserveHostClose() noexcept{return core&&core->FinishLinuxHostOriginals();}
    bool ReservedStandaloneSourceBytes(std::size_t& bytes) const noexcept {
        bytes=sizeof(*this);
        const auto add=[&](std::size_t count,std::size_t width) noexcept {
            if(width&&count>(SIZE_MAX-bytes)/width)return false;bytes+=count*width;return true;
        };
        if(!add(bodyCopy_.capacity(),sizeof(unsigned char))||!add(capabilityCopy_.capacity(),sizeof(int))||
           !add(readCopy_.capacity(),sizeof(std::vector<std::size_t>))||
           !add(readPointers_.capacity(),sizeof(const std::size_t*))||!add(countsCopy_.capacity(),sizeof(std::size_t)))return false;
        for(const auto& row:readCopy_)if(!add(row.capacity(),sizeof(std::size_t)))return false;
        return true; // Registry/platform, allocator/Python/kernel/stack additional.
    }
    // Actual main/interpreter/final independent receipt and qualified Linux
    // launch gate are still separate bodies. No destructor/finalizer closes.
};
}
#endif

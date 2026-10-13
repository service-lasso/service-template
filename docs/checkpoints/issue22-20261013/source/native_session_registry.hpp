// UNCOMPILED native-session lifetime registry. No automatic resource disposal.
#pragma once
#include "native_session_core.hpp"
#include <stdexcept>
#include <array>
#ifndef _WIN32
#include "linux_pending_input_owner.hpp"
namespace template_creator_linux {class OriginalLinuxPendingHost;class OriginalLinuxHeldFile;}
#endif
namespace template_creator_bridge {
class OriginalWindowsPendingCalls;
struct SourceByteCount { std::size_t value = 0; bool overflow = false; };
class RetainedNativeRegistry {
    std::unique_ptr<NativeSessionCore*[]> sessions_;
    std::unique_ptr<NativePlatformState*[]> platforms_;
    std::size_t capacity_ = 0, count_ = 0;
    bool exhausted_ = false;
    // A custodian-owned collector registry remains resident while any original
    // native custody/evidence is unresolved. Public Python never creates it.
    ~RetainedNativeRegistry() = default;
public:
    explicit RetainedNativeRegistry(std::size_t completeSelectedSessions) {
        if (!completeSelectedSessions || completeSelectedSessions >
            (std::numeric_limits<std::size_t>::max() - sizeof(*this)) / (sizeof(NativeSessionCore*)+sizeof(NativePlatformState*)))
            throw std::invalid_argument("complete native session capacity");
        sessions_ = std::make_unique<NativeSessionCore*[]>(completeSelectedSessions);
        platforms_ = std::make_unique<NativePlatformState*[]>(completeSelectedSessions);
        capacity_ = completeSelectedSessions;
    }
    RetainedNativeRegistry(const RetainedNativeRegistry&) = delete;
    RetainedNativeRegistry& operator=(const RetainedNativeRegistry&) = delete;
    NativeSessionCore* PreownSession(std::size_t completeDispatchRows,std::size_t completeHostObservations) {
        if (exhausted_ || count_ == capacity_) { exhausted_ = true; return nullptr; }
        // Entire allocation precedes ingress/native acquisition. Publishing into
        // the already-owned slot is nonallocating and cannot orphan the new core.
        auto* original = new NativeSessionCore(completeDispatchRows,completeHostObservations);
        sessions_[count_++] = original;
        return original;
    }
    bool Owns(const NativeSessionCore* original) const noexcept {
        for (std::size_t i=0; i<count_; ++i) if (sessions_[i] == original) return true;
        return false;
    }
    const NativeSessionCore* const* PrivateOriginalSessions() const noexcept { return sessions_.get(); }
    std::size_t SessionCount() const noexcept { return count_; }
#ifdef _WIN32
    // Native-only capability transfer, never Python numeric/JSON construction.
    NativePlatformState* PreownWindowsPlatform(NativeSessionCore& original,
        OriginalPhase originalPhase,std::size_t protectedCommands,std::size_t sinkBytes,std::size_t readbackBytes,
        std::size_t sinkCalls,std::size_t collectorBytes,std::size_t collectorReads,
        std::size_t collectorObservations,unsigned long collectorWaitMilliseconds,const std::array<void*,5>& actualTransferredOriginals,
        OriginalWindowsPendingCalls& pendingCalls);
#endif
#ifndef _WIN32
    NativePlatformState* PreownLinuxPlatform(NativeSessionCore& original,OriginalPhase phase,
        std::size_t selectedCommands,std::size_t sinkBytes,std::size_t sinkCalls,
        const template_creator_linux::PendingInputGeometry& geometry,bool hasOriginalDeadline,
        std::uint64_t originalAbsoluteNanoseconds,std::size_t callerStack,std::size_t callerJoins,
        const unsigned char* originalBody,std::size_t originalBodyBytes,const int* originalCapabilities,
        std::size_t originalCapabilityCount,int actualProcRoot,std::size_t actualSinkOrdinal,
        std::size_t actualRootOrdinal,std::size_t actualToolOrdinal,std::size_t actualEvidenceOrdinal,
        const std::size_t* const* inputReadRows,const std::size_t* inputCounts,
        template_creator_linux::OriginalLinuxPendingHost* adoptedHost=nullptr,
        template_creator_linux::OriginalLinuxHeldFile* selectionOriginal=nullptr);
#endif
    SourceByteCount ReservedSourceBytes() const noexcept {
        std::size_t bytes = sizeof(*this) + capacity_*(sizeof(NativeSessionCore*)+sizeof(NativePlatformState*));
        for (std::size_t i=0; i<count_; ++i) {
            const auto selected = sessions_[i]->ReservedDispatchSourceBytes();
            if (selected > std::numeric_limits<std::size_t>::max() - bytes)
                return {bytes, true}; // An overflow is an explicit failed/unknown count.
            bytes += selected;
        }
        return {bytes, false}; // Platform allocations are separately counted by their native owner.
    }
};
// Native host integration must keep this registry AND its creating native thread
// alive across unresolved failure; keeping a pointer is not evidence of that
// native lifetime. Platform/core/sink/host closure is still being implemented.
}

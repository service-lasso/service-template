// UNRUN native harness receipt. This grants no product continuation.
#pragma once
#ifndef _WIN32
namespace template_creator_linux {
class OriginalLinuxMappedMemory;
struct Event;
struct OriginalLinuxElfInterpreterSource;
// The same root harness also consumes its actual pipe-source consumer checks;
// scalar negative close cases are source surrogates, not native close failures.
bool InvokeActualRootExecFiveSpanRegression(const OriginalLinuxMappedMemory&,bool ownPipeSourceChecks) noexcept;
bool ObserveActualRootExecReservedGateRegression(const Event&) noexcept;
bool ObserveActualRootExecContinuedRegression(const Event&) noexcept;
bool ExpectActualInterpreterFileSpansRegression() noexcept;
bool ObserveActualInterpreterFileSpansRegression(const OriginalLinuxMappedMemory&,
    const OriginalLinuxElfInterpreterSource&,unsigned long long bias) noexcept;
bool ActualRootExecFiveSpanRegressionPassed() noexcept;
}
#endif

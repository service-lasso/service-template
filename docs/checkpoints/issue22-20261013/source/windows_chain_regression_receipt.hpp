// Internal native ROOT harness receipt. UNRUN; no product admission authority.
#pragma once
#include <cstddef>
namespace template_creator {
struct OriginalChainRegressionReceipt {
    std::size_t observedInvocations=0;
    bool failed=false;
};
inline OriginalChainRegressionReceipt originalChainRegressionReceipt;
inline bool OriginalChainRegressionPassed() noexcept {
    return originalChainRegressionReceipt.observedInvocations!=0&&!originalChainRegressionReceipt.failed;
}
}

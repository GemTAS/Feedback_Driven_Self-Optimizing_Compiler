#pragma once
#include "pass.h"
#include <unordered_map>

class LoopUnrollingPass : public OptimizationPass {
public:
    // Profile data and unroll parameters set by FeedbackEngine
    std::unordered_map<std::string, int> hotLabels;
    int unrollFactor = 4;
    int hotThreshold = 10;

    std::string name() const override;
    void run(std::vector<IRInstr>& ir) override;
};

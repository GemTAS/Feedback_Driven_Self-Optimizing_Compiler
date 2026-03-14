#pragma once
#include "pass.h"
#include "feedback_engine.h"
#include "../runtime/profiler.h"
#include <vector>
#include <memory>

class PassManager {
public:
    void registerStandardPasses();
    void loadProfileAndRegister(const Profiler* profiler);
    void runAll(std::vector<IRInstr>& ir);

private:
    std::vector<std::unique_ptr<OptimizationPass>> passes;
    FeedbackEngine engine;
};

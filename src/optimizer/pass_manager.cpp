#include "pass_manager.h"
#include "constant_folding.h"
#include "dead_code.h"
#include "loop_unrolling.h"
#include "../utils/logger.h"

void PassManager::registerStandardPasses() {
    // First, let's load up the baseline optimizations that should happen to everything.
    passes.push_back(std::make_unique<ConstantFoldingPass>());
    passes.push_back(std::make_unique<DeadCodeEliminationPass>());
}

void PassManager::loadProfileAndRegister(const Profiler* profiler) {
    // Pass the raw data over to the feedback engine so it can think about what needs unrolling.
    engine.setProfiler(profiler);

    if (engine.hasProfileData()) {
        // If we actually have data, let's inject the dynamic loop unroller into our pipeline!
        auto loopPass = std::make_unique<LoopUnrollingPass>();
        engine.configureLoopUnrolling(*loopPass);
        passes.push_back(std::move(loopPass));
    }
}

void PassManager::runAll(std::vector<IRInstr>& ir) {
    Logger::info("=== OPTIMIZATION PIPELINE ===");
    Logger::info("IR size before: " + std::to_string(ir.size()) + " instructions");

    for (auto& pass : passes) {
        Logger::info("Running pass: " + pass->name());
        pass->run(ir);
        Logger::info("IR size after " + pass->name() + ": " + 
                     std::to_string(ir.size()) + " instructions");
    }

    Logger::info("=== PIPELINE COMPLETE ===");
    Logger::info("Final IR size: " + std::to_string(ir.size()) + " instructions");
}

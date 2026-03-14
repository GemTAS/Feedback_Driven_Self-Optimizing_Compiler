#include "feedback_engine.h"
#include "../utils/config.h"
#include "../utils/logger.h"

void FeedbackEngine::setProfiler(const Profiler* p) {
    profiler = p;
    if (profiler && profiler->hasData()) {
        Logger::info("FeedbackEngine: Linked to Profiler with " + 
                     std::to_string(profiler->getAllData().size()) + " entries.");
    } else {
        Logger::warn("FeedbackEngine: Linked to empty or null Profiler. Operating without feedback.");
    }
}

void FeedbackEngine::configureLoopUnrolling(LoopUnrollingPass& pass) {
    if (!profiler) return;

    pass.hotLabels = profiler->getAllData();
    pass.hotThreshold = Config::hotThreshold;
    pass.unrollFactor = Config::unrollFactor;

    // Here's the adaptive trick: if a loop is cooking at insane temperatures, 
    // let's just go all out and unroll it aggressively.
    for (const auto& p : pass.hotLabels) {
        if (p.second > Config::extremeHotThreshold) {
            pass.unrollFactor = Config::extremeUnrollFactor;
            Logger::info("FeedbackEngine: Extremely hot label detected (" + p.first + " = " + 
                         std::to_string(p.second) + "), increasing unroll factor to " + 
                         std::to_string(pass.unrollFactor));
            break;
        }
    }
}

bool FeedbackEngine::hasProfileData() const {
    return profiler != nullptr && profiler->hasData();
}

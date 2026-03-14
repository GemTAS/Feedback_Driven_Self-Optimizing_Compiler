#pragma once
#include "loop_unrolling.h"
#include "../runtime/profiler.h"
#include <string>

class FeedbackEngine {
public:
    const Profiler* profiler = nullptr;

    void setProfiler(const Profiler* p);
    void configureLoopUnrolling(LoopUnrollingPass& pass);
    bool hasProfileData() const;
};

#pragma once
#include "pass.h"

class DeadCodeEliminationPass : public OptimizationPass {
public:
    std::string name() const override;
    void run(std::vector<IRInstr>& ir) override;
};

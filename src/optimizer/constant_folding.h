#pragma once
#include "pass.h"

class ConstantFoldingPass : public OptimizationPass {
public:
    std::string name() const override;
    void run(std::vector<IRInstr>& ir) override;

private:
    static bool isNumber(const std::string& s);
};

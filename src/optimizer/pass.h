#pragma once
#include "../ir/ir_node.h"
#include <vector>
#include <string>

class OptimizationPass {
public:
    virtual ~OptimizationPass() = default;
    virtual std::string name() const = 0;
    virtual void run(std::vector<IRInstr>& ir) = 0;
};
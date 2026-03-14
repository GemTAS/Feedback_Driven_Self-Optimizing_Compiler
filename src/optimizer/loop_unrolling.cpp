#include "loop_unrolling.h"
#include "../utils/logger.h"
using namespace std;

string LoopUnrollingPass::name() const {
    return "LoopUnrolling";
}

void LoopUnrollingPass::run(vector<IRInstr>& ir) {
    int unrolled = 0;

    for (size_t i = 0; i < ir.size(); i++) {
        if (ir[i].op != "label") continue;

        // We snag the label's name right here since the IR builder stashes it in the 'label' field.
        string label = ir[i].label;

        // If this loop isn't blazing hot according to our profile, just ignore it.
        if (hotLabels[label] < hotThreshold) continue;

        // Let's hunt down the goto instruction that loops back up to this label.
        int back = -1;
        for (size_t j = i + 1; j < ir.size(); j++) {
            if (ir[j].op == "goto" && ir[j].res == label) {
                back = j;
                break;
            }
        }
        if (back == -1) continue;

        // We need to grab the entire loop body, which includes the conditions and jumps.
        vector<IRInstr> body;
        bool hasInnerLabels = false;
        for (size_t k = i + 1; k < (size_t)back; k++) {
            if (ir[k].op == "label") {
                hasInnerLabels = true;
                break;
            }
            body.push_back(ir[k]);
        }
        if (body.empty() || hasInnerLabels) continue;

        // Okay, let's start piecing together the new, unrolled IR.
        vector<IRInstr> newIR;

        // First, paste in everything that came before our loop.
        for (size_t k = 0; k <= i; k++) {
            newIR.push_back(ir[k]);
        }

        // Here's the core: we just blindly replicate the body a bunch of times.
        // The cool thing is that the original 'ifgoto' exit check is copied too,
        // so it'll cleanly bail out whenever the actual loop condition breaks.
        for (int u = 0; u < unrollFactor; u++) {
            for (auto inst : body) {
                newIR.push_back(inst);
            }
        }

        // And don't forget the back-edge jump to return to the top of our unrolled block.
        IRInstr backEdge;
        backEdge.op = "goto";
        backEdge.res = label;
        newIR.push_back(backEdge);

        // Finish up by copying the rest of the original IR structure.
        for (size_t k = back + 1; k < ir.size(); k++) {
            newIR.push_back(ir[k]);
        }

        ir = newIR;
        hotLabels[label] = 0; // We have to zero this out or we'll infinitely unroll it!
        i = 0;  // restart scan
        unrolled++;
    }

    Logger::info("[LoopUnrolling] Unrolled " + to_string(unrolled) + 
                 " hot loops (factor=" + to_string(unrollFactor) + ")");
}

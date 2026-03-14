#include "dead_code.h"
#include "../utils/logger.h"
#include <unordered_set>
#include <cctype>
using namespace std;

string DeadCodeEliminationPass::name() const {
    return "DeadCodeElimination";
}

void DeadCodeEliminationPass::run(vector<IRInstr>& ir) {
    // Step 1: Let's figure out what variables we actually care about (the ones being read).
    unordered_set<string> used;

    for (auto& i : ir) {
        if (!i.a1.empty() && isalpha(i.a1[0])) used.insert(i.a1);
        if (!i.a2.empty() && isalpha(i.a2[0])) used.insert(i.a2);
        if (i.op == "print") used.insert(i.a1);
    }

    // Step 2: Now we just sweep through and toss out any assignments writing to variables nobody reads.
    vector<IRInstr> cleaned;
    int eliminated = 0;

    for (auto& i : ir) {
        if (i.op == "assign" || i.op == "+" || i.op == "-" ||
            i.op == "*"      || i.op == "/") {
            if (used.count(i.res)) {
                cleaned.push_back(i);
            } else {
                eliminated++;
            }
        } else {
            cleaned.push_back(i);
        }
    }

    ir = cleaned;
    Logger::info("[DeadCodeElimination] Removed " + to_string(eliminated) + " dead instructions");
}

#include "constant_folding.h"
#include "../utils/logger.h"
#include <cctype>
using namespace std;

string ConstantFoldingPass::name() const {
    return "ConstantFolding";
}

bool ConstantFoldingPass::isNumber(const string& s) {
    return !s.empty() && isdigit(s[0]);
}

void ConstantFoldingPass::run(vector<IRInstr>& ir) {
    int folded = 0;

    for (auto &i : ir) {
        if (!isNumber(i.a1) || !isNumber(i.a2)) continue;

        int x = stoi(i.a1);
        int y = stoi(i.a2);
        int r;

        if      (i.op == "+") r = x + y;
        else if (i.op == "-") r = x - y;
        else if (i.op == "*") r = x * y;
        else if (i.op == "/") { if (y == 0) continue; r = x / y; }
        else continue;

        i.op = "assign";
        i.a1 = to_string(r);
        i.a2.clear();
        folded++;
    }

    Logger::info("[ConstantFolding] Folded " + to_string(folded) + " expressions");
}

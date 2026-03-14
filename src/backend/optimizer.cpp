#include "optimizer.h"
#include <fstream>
#include <unordered_map>
#include <unordered_set>
#include <cctype>
#include <sstream>
#include <iostream>
using namespace std;

/* ================= HELPERS ================= */

static bool isNumber(const std::string& s) {
    return !s.empty() && std::isdigit(s[0]);
}

/* ================= CONSTANT FOLD ================= */

void Optimizer::constantFold(vector<IRInstr>& ir) {
    for (auto &i : ir) {
        if (isNumber(i.a1) && isNumber(i.a2)) {

            int x = stoi(i.a1);
            int y = stoi(i.a2);
            int r;

            if (i.op == "+") r = x + y;
            else if (i.op == "-") r = x - y;
            else if (i.op == "*") r = x * y;
            else if (i.op == "/") r = x / y;
            else continue;

            i.op = "assign";
            i.a1 = to_string(r);
            i.a2.clear();
        }
    }
}

/* ================= DEAD CODE ================= */

void Optimizer::deadCodeElimination(std::vector<IRInstr>& ir) {

    std::unordered_set<std::string> used;

    for (auto& i : ir) {
        if (!i.a1.empty() && std::isalpha(i.a1[0])) used.insert(i.a1);
        if (!i.a2.empty() && std::isalpha(i.a2[0])) used.insert(i.a2);
        if (i.op == "print") used.insert(i.a1);
    }

    std::vector<IRInstr> cleaned;

    for (auto& i : ir) {
        if (i.op=="assign" || i.op=="+" || i.op=="-" || i.op=="*" || i.op=="/") {
            if (used.count(i.res))
                cleaned.push_back(i);
        }
        else
            cleaned.push_back(i);
    }

    ir = cleaned;
}

/* ================= PROFILE LOAD ================= */

void Optimizer::loadProfile(const std::string& file) {

    hot.clear();

    std::ifstream in(file);
    std::string label;
    int count;

    while (in >> label >> count)
        hot[label] = count;

    // DEBUG — prove profile loaded
    for (auto &p : hot)
        std::cout << p.first << " " << p.second << std::endl;
}

/* ================= HOT PATH OPT ================= */

void Optimizer::hotLoopUnroll(std::vector<IRInstr>& ir) {

    int UNROLL = 4;

    for (size_t i = 0; i < ir.size(); i++) {

        if (ir[i].op != "label") continue;

        string label = ir[i].res;

        if (hot[label] < 10) continue;

        int back = -1;
        for (size_t j = i + 1; j < ir.size(); j++) {
            if (ir[j].op == "goto" && ir[j].res == label) {
                back = j;
                break;
            }
        }

        if (back == -1) continue;

        vector<IRInstr> body;
        IRInstr exitCheck;

        for (size_t k = i + 1; k < back; k++) {
            if (ir[k].op == "ifgoto") {
                exitCheck = ir[k];
                break;
            }
            body.push_back(ir[k]);
        }

        if (body.empty()) continue;

        string remLabel = label + "_rem";

        vector<IRInstr> newIR;

        for (size_t k = 0; k <= i; k++)
            newIR.push_back(ir[k]);

        for (int u = 0; u < UNROLL; u++)
            for (auto inst : body)
                newIR.push_back(inst);

        IRInstr toRem = exitCheck;
        toRem.res = remLabel;
        newIR.push_back(toRem);

        IRInstr backFast;
        backFast.op = "goto";
        backFast.res = label;
        newIR.push_back(backFast);

        IRInstr remLab;
        remLab.op = "label";
        remLab.res = remLabel;
        newIR.push_back(remLab);

        for (auto inst : body)
            newIR.push_back(inst);

        newIR.push_back(exitCheck);

        IRInstr backRem;
        backRem.op = "goto";
        backRem.res = remLabel;
        newIR.push_back(backRem);

        for (size_t k = back + 1; k < ir.size(); k++)
            newIR.push_back(ir[k]);

        ir = newIR;
        i = 0;
    }
}
//All in one
void Optimizer::optimize(std::vector<IRInstr>& ir) {
    constantFold(ir);
    hotLoopUnroll(ir);
    deadCodeElimination(ir);
}

#include "ir_builder.h"
#include <stdexcept>
std::vector<IRInstr> IRBuilder::build(const std::vector<Stmt*>& prog) {
    std::vector<IRInstr> ir;
    for (auto s : prog)
        genStmt(s, ir);
    return ir;
}

void IRBuilder::genStmt(Stmt* s, std::vector<IRInstr>& ir) {

    if (auto w = dynamic_cast<WhileStmt*>(s)) {

        std::string start = newLabel();
        std::string end   = newLabel();

        // start label
        IRInstr lab;
        lab.op = "label";
        lab.label = start;

        ir.push_back(lab);

        // condition
        std::string cond = genExpr(w->condition, ir);

        // if cond == 0 goto end
        ir.push_back({"ifgoto", cond, "0", end});

        // body
        for (auto stmt : w->body)
            genStmt(stmt, ir);

        // jump back
        ir.push_back({"goto", "", "", start});

        // exit label
        IRInstr exitLab;
        exitLab.op = "label";
        exitLab.label = end;
        ir.push_back(exitLab);

    }

    else if (auto l = dynamic_cast<LetStmt*>(s)) {
        std::string v = genExpr(l->value, ir);
        ir.push_back({"assign", v, "", l->name});
    }

    else if (auto a = dynamic_cast<AssignStmt*>(s)) {
        std::string v = genExpr(a->value, ir);
        ir.push_back({"assign", v, "", a->name});
    }

    else if (auto p = dynamic_cast<PrintStmt*>(s)) {
        std::string v = genExpr(p->value, ir);
        ir.push_back({"print", v, "", ""});
    }
}

std::string IRBuilder::genExpr(Expr* e, std::vector<IRInstr>& ir) {

    if (auto n = dynamic_cast<NumberExpr*>(e))
        return std::to_string(n->value);

    if (auto v = dynamic_cast<VarExpr*>(e))
        return v->name;

    if (auto b = dynamic_cast<BinaryExpr*>(e)) {

        std::string l = genExpr(b->left, ir);
        std::string r = genExpr(b->right, ir);
        std::string t = newTemp();

        IRInstr inst;
        inst.op = b->op;
        inst.a1 = l;
        inst.a2 = r;
        inst.res = t;
        ir.push_back(inst);

        return t;
    }

    throw std::runtime_error("Unknown expression");
}

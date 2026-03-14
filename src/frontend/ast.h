#pragma once
#include <string>
#include <vector>

struct Expr {
    virtual ~Expr() = default;
};

struct NumberExpr : Expr {
    int value;
    NumberExpr(int v) : value(v) {}
};

struct VarExpr : Expr {
    std::string name;
    VarExpr(const std::string& n) : name(n) {}
};

struct BinaryExpr : Expr {
    std::string op;
    Expr* left;
    Expr* right;
    BinaryExpr(const std::string& o, Expr* l, Expr* r): op(o), left(l), right(r) {}

};

struct Stmt {
    virtual ~Stmt() = default;
};

struct LetStmt : Stmt {
    std::string name;
    Expr* value;
    LetStmt(const std::string& n, Expr* v) : name(n), value(v) {}
};

struct AssignStmt : Stmt {
    std::string name;
    Expr* value;
    AssignStmt(const std::string& n, Expr* v) : name(n), value(v) {}
};

struct PrintStmt : Stmt {
    Expr* value;
    PrintStmt(Expr* v) : value(v) {}
};

struct WhileStmt : Stmt {
    Expr* condition;
    std::vector<Stmt*> body;
    WhileStmt(Expr* c, const std::vector<Stmt*>& b) : condition(c), body(b) {}
};
struct IfGotoStmt : Stmt {
    std::string lhs, op, rhs, label;
};


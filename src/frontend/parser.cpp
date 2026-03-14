#include "parser.h"
#include <stdexcept>

Parser::Parser(const std::vector<Token>& t) : tokens(t) {}

Token Parser::peek() { return tokens[pos]; }
Token Parser::get() { return tokens[pos++]; }

bool Parser::match(TokenType t) {
    if (peek().type == t) {
        get();
        return true;
    }
    return false;
}

std::vector<Stmt*> Parser::parseProgram() {
    std::vector<Stmt*> stmts;
    while (peek().type != TokenType::END)
        stmts.push_back(parseStatement());
    return stmts;
}

Stmt* Parser::parseStatement() {
    if (peek().type == TokenType::LET) return parseLet();
    if (peek().type == TokenType::PRINT) return parsePrint();
    if (peek().type == TokenType::WHILE) return parseWhile();
    if (peek().type == TokenType::IDENTIFIER) return parseAssign();
    throw std::runtime_error("Unknown statement");
}

Stmt* Parser::parseLet() {
    get(); 
    std::string name = get().value;
    get(); 
    Expr* val = parseExpr();
    get(); 
    return new LetStmt(name, val);
}

Stmt* Parser::parseAssign() {
    std::string name = get().value;
    get();
    Expr* val = parseExpr();
    get();
    return new AssignStmt(name, val);
}

Stmt* Parser::parsePrint() {
    get();
    Expr* v = parseExpr();
    get();
    return new PrintStmt(v);
}

Stmt* Parser::parseWhile() {
    get();
    Expr* cond = parseExpr();
    get();
    std::vector<Stmt*> body;

    while (peek().type != TokenType::RBRACE)
        body.push_back(parseStatement());

    get();
    return new WhileStmt(cond, body);
}

Expr* Parser::parseExpr() {
    Expr* left = parseTerm();

    while (peek().type == TokenType::PLUS || peek().type == TokenType::MINUS) {
        std::string op = get().value;   // consume operator
        Expr* right = parseTerm();
        left = new BinaryExpr(op, left, right);
    }

    if (peek().type == TokenType::LT) {
        std::string op = get().value;   // consume <
        Expr* right = parseTerm();
        left = new BinaryExpr(op, left, right);
    }

    return left;
}

Expr* Parser::parseTerm() {
    Expr* left = parseFactor();

    while (peek().type == TokenType::MUL || peek().type == TokenType::DIV) {
        std::string op = get().value;   // consume operator
        Expr* right = parseFactor();
        left = new BinaryExpr(op, left, right);
    }

    return left;
}

Expr* Parser::parseFactor() {
    if (peek().type == TokenType::NUMBER)
        return new NumberExpr(std::stoi(get().value));
    if (peek().type == TokenType::IDENTIFIER)
        return new VarExpr(get().value);

    throw std::runtime_error("Invalid expression");
}

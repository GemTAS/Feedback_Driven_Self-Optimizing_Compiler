#include <iostream>
#include <fstream>
#include <sstream>
#include "frontend/lexer.h"
#include "frontend/parser.h"
#include "ir/ir_builder.h"
#include "frontend/ast.h"
#include "backend/codegen.h"
#include "runtime/profiler.h"
#include "utils/config.h"
#include "utils/logger.h"
#include "optimizer/pass_manager.h"

int main(int argc, char* argv[]) {
    // Let's make sure the user provided at least a test file to compile.    if (argc < 2) {
        std::cout << "Usage: ./compiler <file> [profile.txt] [-v]\n";
        return 1;
    }

    std::string profileFile = "profile.txt";
    if (argc >= 3) {
        std::string arg2 = argv[2];
        if (arg2 == "-v") Config::currentLogLevel = Config::LogLevel::DEBUG;
        else profileFile = arg2;
    }
    if (argc >= 4) {
        std::string arg3 = argv[3];
        if (arg3 == "-v") Config::currentLogLevel = Config::LogLevel::DEBUG;
    }

    }

    // Alright, time to read the source file into memory.
    std::ifstream file(argv[1]);
    if (!file.is_open()) {
        Logger::error(std::string("Failed to open file: ") + argv[1]);
        return 1;
    }
    }

    // We'll use a stringstream buffer to slurp the whole file.
    std::stringstream buffer;
    buffer << file.rdbuf();
    buffer << file.rdbuf();

    // Got it, now we have the source string.
    std::string source = buffer.str();
    Logger::info("Source loaded (" + std::to_string(source.size()) + " bytes)");

    // === FRONTEND ===
    // First up, let's break the raw text down into tokens.
    Lexer lexer(source);
    auto tokens = lexer.tokenize();
    for (auto& t : tokens) {
        Logger::debug(std::to_string((int)t.type) + " : " + t.value);
    }

    // === PARSER ===
    // Now we take those tokens and build an Abstract Syntax Tree (AST).
    Parser parser(tokens);
    auto program = parser.parseProgram();
    Logger::info("Parsed " + std::to_string(program.size()) + " statements successfully");

    // === IR BUILDER ===
    // Time to flatten the AST into our three-address code IR.
    IRBuilder builder;
    auto ir = builder.build(program);

    // === OPTIMIZER (Pass Manager Architecture) ===
    // Here comes the magic! Let's set up our optimizer passes.
    PassManager pm;
    pm.registerStandardPasses();                     // The basics: constant folding and dead code elimination.

    Profiler profiler;
    profiler.loadProfile(profileFile);
    pm.loadProfileAndRegister(&profiler);            // Now we hook up the feedback-driven loop unrolling pass using the profile.
    pm.runAll(ir);                                   // And let the passes do their thing!

    // === CODE GENERATION ===
    // Finally, we translate our optimized IR directly into workable C code.
    CodeGen gen;
    gen.emitC(ir, "output.c");

    // === PRINT OPTIMIZED IR ===
    // If we're debugging, let's dump the final IR so we can see what the optimizer did.
    if (Config::currentLogLevel <= Config::LogLevel::DEBUG) {
        Logger::debug("=== OPTIMIZED IR ===");
        for (auto &i : ir) {
            if (i.op == "label")
                Logger::debug(i.res + ":");
            else if (i.op == "goto")
                Logger::debug("GOTO " + i.res);
            else if (i.op == "ifgoto")
                Logger::debug("IF " + i.a1 + " == " + i.a2 + " GOTO " + i.res);
            else if (i.op == "assign")
                Logger::debug(i.res + " = " + i.a1);
            else if (i.op == "print")
                Logger::debug("print " + i.a1);
            else
                Logger::debug(i.res + " = " + i.a1 + " " + i.op + " " + i.a2);
        }
    }
    return 0;
}

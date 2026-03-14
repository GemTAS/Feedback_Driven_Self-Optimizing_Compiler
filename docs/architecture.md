# Feedback-Driven Self-Optimizing Compiler: Architecture

## High-Level Pipeline

The compiler follows a modular, 5-stage pipeline designed for profile-guided optimization (PGO).

**1. Frontend (Lexer & Parser)**
The `Lexer` converts the raw `.lang` source strings into tokens. The `Parser` consumes these tokens to construct an Abstract Syntax Tree (AST) representing statements like `LetVal`, `WhileLoop`, and `Print`.

**2. IR Builder**
The AST is traversed to generate a flat, three-address code Intermediate Representation (IR). The `IRBuilder` uses unique labels (e.g., `L0`, `L1`) and temporary variables (`t0`, `t1`) to break down complex expressions into simple `op` instructions suitable for optimization and C translation.

**3. Execution Profiler (The Feedback Loop)**
The compiler uses an external C language runtime (`src/backend/profiler_runtime.c`) to gather execution statistics. 
When an unoptimized version of the code is run, every jump to an IR `label` issues a `prof_hit("label_name")` call. Upon program exit, the runtime dumps a statistical map of label hits to `profile.txt`.

**4. Optimizer Engine (PassManager)**
The core contribution of this project is the `PassManager`. It operates on the generated IR before C codegen.
It is initialized with a generic `Profiler` wrapper object that reads `profile.txt`. The `PassManager` then sequentially applies `OptimizationPass` objects to the IR.

**5. Backend CodeGen**
The optimized IR is directly emitted as raw C code (`output.c`), which is then compiled using `gcc` into the final executable. The C code includes the profiler runtime header depending on whether it's an unoptimized or optimized build.

## The PassManager Module
The optimization engine uses a polymorphic approach:
*   `OptimizationPass` (Base Interface): Requires every pass to implement a `run(vector<IRInstr>&)` method.
*   `ConstantFoldingPass`: Folds static scalar arithmetic.
*   `DeadCodeEliminationPass`: Sweeps unused assignments.
*   `LoopUnrollingPass`: Replicates hot loop bodies to minimize branch overhead.

The `FeedbackEngine` bridges the gap between the `Profiler` runtime wrapper and the `LoopUnrollingPass`, dynamically injecting unroll factors based on the exact hotness of the labels found in the user's workload.

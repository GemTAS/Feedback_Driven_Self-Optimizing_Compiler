# Optimization Plan & Strategy

This document outlines the three primary optimization passes implemented in the compiler.

## 1. Constant Folding
**Goal:** Compute static expressions at compile-time to save runtime cycles.
**Mechanism:** The `ConstantFoldingPass` scans the IR for binary operators (`+`, `-`, `*`, `/`). If both operands (`a1` and `a2`) are literal numbers (verified via `isdigit`), the pass evaluates the math synchronously and converts the instruction into a simple `assign` operation containing the pre-computed result.

## 2. Dead Code Elimination (DCE)
**Goal:** Remove unused variables and assignments, shrinking the executable size and improving cache coherence.
**Mechanism:** The `DeadCodeEliminationPass` uses a two-pass liveness approach:
1.  **Sweep 1:** Iterate over the entire IR to populate an `unordered_set<string>` of every variable that is placed on the *right-hand side* of an expression or utilized in a `print` statement.
2.  **Sweep 2:** Iterate over the IR again. If an `assign` or mathematical instruction stores a result in a variable that is fundamentally missing from the "used" set, the instruction is discarded.

## 3. Feedback-Driven Loop Unrolling
**Goal:** Maximize pipeline throughput by eliminating branch instructions (`ifgoto`, `goto`) in high-frequency loops.
**Mechanism:** The `LoopUnrollingPass` is uniquely tied to the `Profiler` data. 
*   **Identification:** The pass scans the IR for `label` instructions and cross-references them with the label map provided by the `FeedbackEngine`. If a label's hit-count exceeds `Config::hotThreshold`, the loop is marked for unrolling.
*   **Replication:** The instructions comprising the loop body (between the `label` and the back-edge `goto`) are duplicated `unrollFactor` times sequentially into the IR.
*   **Adaptive Unrolling:** If the profiler identifies an *extremely* hot label (exceeding `Config::extremeHotThreshold`), the `FeedbackEngine` adaptively boosts the unroll factor for that specific pass instance, resulting in aggressive optimization without manual user intervention.

*Note: The unroller specifically skips unrolling complex nested loops, as duplicating internal labels creates standard C compilation conflicts.*

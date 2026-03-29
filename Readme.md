<b>FeedbackDrivenSelfOptimizingCompiler</b>

A custom compiler that uses runtime profiling to identify frequently executed code paths and applies optimizations such as constant folding and loop optimizations to improve performance.

===> (1.)Project Setup

  -> Required build tools installed:
    sudo apt update
    sudo apt install build-essential cmake

===> (2.)Build Instructions
(not needed since i have pushed build folder as well for now)
  cd ~/CompilerDesign/FeedbackDrivenSelfOptimizingCompiler
  mkdir -p build
  cd build
  cmake ..
  make

===> (3.)Running the Compiler
  -> Run the compiler on sample test programs:
    ./compiler ../tests/sample1.lang
    ./compiler ../tests/sample2.lang
  This will:
    Parse the source code
    Generate Intermediate Representation (IR)
    Apply optimizations
    Produce output.c

===> (4.)Compile and Execute Generated Code

  1.gcc output.c ../src/backend/profiler_runtime.c -I../src/backend -o program
  ./program

  2.gcc output.c ../src/backend/profiler_runtime.c -I../src/backend -o program_opt
  ./program_opt


===> (5.)View Profiling Data
  cat profile.txt
  This file contains execution frequency of labels, used for feedback-driven optimizations.

===> (6.)Rebuilding the Project if build issue
  cd ~/CompilerDesign/FeedbackDrivenSelfOptimizingCompiler
  rm -rf build
  mkdir build
  cd build
  cmake ..
  make

===> (7.)All in one full comparison
  cd /home/tas/CompilerDesign/FeedbackDrivenSelfOptimizingCompiler/build 
  && 
  rm -f profile.txt 
  && 
  ./compiler ../tests/benchmark/heavy_loop.lang 
  && 
  gcc output.c ../src/backend/profiler_runtime.c -I../src/backend -o heavy_loop_unopt 
  && 
  echo "=== Unoptimized built ===" 
  && 
  ./heavy_loop_unopt > /dev/null 
  && 
  echo "=== Profile generated ===" 
  && 
  cat profile.txt 
  && 
  echo "=== Now building optimized ===" 
  && 
  ./compiler ../tests/benchmark/heavy_loop.lang profile.txt 
  && 
  gcc output.c ../src/backend/profiler_runtime.c -I../src/backend -o heavy_loop_opt 
  && 
  echo "=== Ready ==="

===> (8.)Both Binaries Ready Final Result
  cd /home/tas/CompilerDesign/FeedbackDrivenSelfOptimizingCompiler/build 
  && 
  echo "--- Unoptimized (22 IR instructions, no loop unrolling) ---" 
  && 
  time ./heavy_loop_unopt > /dev/null && echo "" 
  && 
  echo "--- Optimized (64 IR instructions, loop unrolled x8) ---"
  && 
  time ./heavy_loop_opt > /dev/null
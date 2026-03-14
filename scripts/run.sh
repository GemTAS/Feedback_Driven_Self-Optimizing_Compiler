#!/bin/bash
set -e

if [ $# -lt 1 ]; then
    echo "Usage: ./scripts/run.sh <test_file.lang>"
    exit 1
fi

FILE=$1
BASENAME=$(basename "$FILE" .lang)
BUILD_DIR="build"

echo "=========================================="
echo "    Running FDO Compiler Pipeline     "
echo "=========================================="

echo "[1/3] Compiling without profiling feedback..."
# We have to nuke the old profile first, otherwise the baseline run might accidentally cheat!
rm -f profile.txt
cd $BUILD_DIR
./compiler ../$FILE profile.txt > /dev/null
gcc output.c ../src/backend/profiler_runtime.c -I../src/backend -o ${BASENAME}_unopt
cd ..

echo "[2/3] Running unoptimized binary to gather profile..."
# The C runtime profiler is linked in, so this execution will sneakily dump to profile.txt
./$BUILD_DIR/${BASENAME}_unopt > /dev/null

echo "[3/3] Re-compiling WITH feedback profile..."
cd $BUILD_DIR
./compiler ../$FILE ../profile.txt > /dev/null
gcc output.c ../src/backend/profiler_runtime.c -I../src/backend -o ${BASENAME}_opt
cd ..

echo "=========================================="
echo " Pipeline Complete! Executables built: "
echo "   - $BUILD_DIR/${BASENAME}_unopt (Baseline)"
echo "   - $BUILD_DIR/${BASENAME}_opt   (Optimized)"
echo "=========================================="

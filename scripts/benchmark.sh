#!/bin/bash
set -e

if [ $# -lt 1 ]; then
    echo "Usage: ./scripts/benchmark.sh <test_file.lang>"
    exit 1
fi

FILE=$1
BASENAME=$(basename "$FILE" .lang)

echo "=========================================="
echo "          Running Benchmark         "
echo "          $FILE"
echo "=========================================="

# 1. Warm up the FDO pipeline first so we're guaranteed to have fresh binaries to test against.
./scripts/run.sh $FILE

echo ""
echo "--- Unoptimized Performance (Baseline) ---"
time ./build/${BASENAME}_unopt > /dev/null

echo ""
echo "--- Profile-Guided Optimized Performance ---"
time ./build/${BASENAME}_opt > /dev/null

echo ""
echo "=========================================="
echo "Benchmark Complete."
echo "=========================================="

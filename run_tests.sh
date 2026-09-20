#!/bin/sh
# Runs every tests/valid*.bcs (expects "Parsing Successful")
# and tests/invalid*.bcs (expects "Syntax Error").
pass=0; fail=0
for f in tests/valid*.bcs;   do
    out=$(./bcs24 "$f")
    if [ "$out" = "Parsing Successful" ]; then pass=$((pass+1)); else fail=$((fail+1)); echo "FAIL: $f -> $out"; fi
done
for f in tests/invalid*.bcs; do
    out=$(./bcs24 "$f")
    if [ "$out" = "Syntax Error" ]; then pass=$((pass+1)); else fail=$((fail+1)); echo "FAIL: $f -> $out"; fi
done
echo "passed: $pass, failed: $fail"
[ "$fail" -eq 0 ]

#!/usr/bin/env bash
set -euo pipefail

if ! command -v pace2024verifier >/dev/null 2>&1; then
    echo "pace2024verifier is not installed or not on PATH."
    exit 1
fi

g++ ../src/main.cpp
truncate -s 0 crossing_counts.txt
mkdir -p medium_test_set/test

total_time=0.0
for i in `seq 1 60 `
do
    start=$(date +%s.%N)
    # our algo
    ./a.out medium_test_set/instances/$i.gr > medium_test_set/test/$i.sol
    duration=$(echo "$(date +%s.%N) - $start" | bc)
    python -c "print($duration)" >> crossing_counts.txt
    total_time=$(python -c "print($total_time + $duration)")
done

echo "Total time: $total_time seconds"

score=0.0
for i in `seq 1 60 `
do
    # provided solution
    pc_crossings=$(pace2024verifier -c medium_test_set/instances/$i.gr medium_test_set/solutions/$i.sol)
    # our algo
    ./a.out medium_test_set/instances/$i.gr > medium_test_set/test/$i.sol
    my_crossings=$(pace2024verifier -c medium_test_set/instances/$i.gr medium_test_set/test/$i.sol)
    ratio=$(python -c "print($pc_crossings/$my_crossings)")
    python -c "print($ratio)" >> crossing_counts.txt
    python -c "print($my_crossings)" >> crossing_counts.txt
    score=$(python -c "print($score + $ratio)")
done

echo "Total time: $total_time seconds" >> crossing_counts.txt
echo "Total score: $score" >> crossing_counts.txt
echo "Score: $score"

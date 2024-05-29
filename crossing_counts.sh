## Evaluate the score on the medium set

g++ src/main.cpp
truncate -s 0 crossing_counts.txt
truncate -s 0 sol.sol

opt="bmgr"

for i in `seq 1 60 `
do
    # provided solution
    pc_crossings=$(pace2024verifier -c medium_test_set/instances/$i.gr medium_test_set/solutions/$i.sol)
    # our algo
    ./a.out medium_test_set/instances/$i.gr sol.sol -${opt}
    my_crossings=$(pace2024verifier -c medium_test_set/instances/$i.gr sol.sol)
    python -c "print($my_crossings/$pc_crossings)" >> crossing_counts.txt
    python -c "print($my_crossings)" >> crossing_counts.txt
    truncate -s 0 sol.sol

done

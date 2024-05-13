## Evaluate the plus-value of recursion

g++ src/main.cpp
truncate -s 0 crossing_counts.txt

opt1="mg"
opt2="mgr"
echo $opt1 >> crossing_counts.txt
echo $opt2 >> crossing_counts.txt

# medium with greedy
start=$(date +%s.%N);
for i in `seq 1 60 `
do
    echo "medium set $i"
    # provided solution
    pc_crossings=$(pace2024verifier -c medium_test_set/instances/$i.gr medium_test_set/solutions/$i.sol)
    echo $pc_crossings
    # without recursion
    ./a.out medium_test_set/instances/$i.gr medium_test_set/${opt1}_sol/$i.sol -$opt1
    my_crossings=$(pace2024verifier -c medium_test_set/instances/$i.gr medium_test_set/${opt1}_sol/$i.sol)
    echo $my_crossings
    python -c "print($my_crossings/$pc_crossings)" >> crossing_counts.txt
    # our algo
    ./a.out medium_test_set/instances/$i.gr medium_test_set/${opt2}_sol/$i.sol -${opt2}
    my_crossings=$(pace2024verifier -c medium_test_set/instances/$i.gr medium_test_set/${opt2}_sol/$i.sol)
    echo $my_crossings
    python -c "print($my_crossings/$pc_crossings)" >> crossing_counts.txt
done
duration=$(echo "$(date +%s.%N) - $start" | bc)
printf "Execution time, medium: %.1f seconds" $duration
echo

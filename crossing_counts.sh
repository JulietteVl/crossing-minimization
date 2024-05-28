## Evaluate the plus-value of recursion

g++ src/main.cpp
truncate -s 0 crossing_counts.txt

opt1="bmg"
opt2="bmgr"
folder="medium_test_set"
echo $opt1 >> crossing_counts.txt
echo $opt2 >> crossing_counts.txt

# medium with greedy
start=$(date +%s.%N);
for i in `seq 1 60`
do
    echo "medium set $i"
    # provided solution
    pc_crossings=$(pace2024verifier -c $folder/instances/$i.gr $folder/solutions/$i.sol)
    echo $pc_crossings
    # without recursion
    ./a.out $folder/instances/$i.gr $folder/${opt1}_sol/$i.sol -$opt1
    my_crossings=$(pace2024verifier -c $folder/instances/$i.gr $folder/${opt1}_sol/$i.sol)
    # echo $my_crossings
    python -c "print($my_crossings/$pc_crossings)" >> crossing_counts.txt
    # our algo
    ./a.out $folder/instances/$i.gr $folder/${opt2}_sol/$i.sol -${opt2}
    my_crossings=$(pace2024verifier -c $folder/instances/$i.gr $folder/${opt2}_sol/$i.sol)
    # echo $my_crossings
    python -c "print($my_crossings/$pc_crossings)" >> crossing_counts.txt
done
duration=$(echo "$(date +%s.%N) - $start" | bc)
printf "Execution time, medium: %.1f seconds" $duration
echo

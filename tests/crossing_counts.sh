## Evaluate the score on the medium set

g++ ../src/main.cpp
truncate -s 0 crossing_counts.txt
mkdir medium_test_set/test

for i in `seq 1 60 `
do
    start=$(date +%s.%N);
    # our algo
    ./a.out medium_test_set/instances/$i.gr > medium_test_set/test/$i.sol
    duration=$(echo "$(date +%s.%N) - $start" | bc)
    python -c "print($duration)" >> crossing_counts.txt
done

for i in `seq 1 60 `
do
    # provided solution
    pc_crossings=$(pace2024verifier -c medium_test_set/instances/$i.gr medium_test_set/solutions/$i.sol)
    # our algo
    ./a.out medium_test_set/instances/$i.gr > medium_test_set/test/$i.sol
    my_crossings=$(pace2024verifier -c medium_test_set/instances/$i.gr medium_test_set/test/$i.sol)
    python -c "print($pc_crossings/$my_crossings)" >> crossing_counts.txt
    python -c "print($my_crossings)" >> crossing_counts.txt

done

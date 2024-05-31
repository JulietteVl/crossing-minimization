# 1-Sided Crossing minimization
## How to use
To use the heuristics, compile (i.e `g++ src/main.cpp`) and run:
```
path/to/excecutable input > output
```

## Requirements
No external library should be necessary.
Built with C++ 17.

## DOI
TBA

## LICENSE
[![License](http://img.shields.io/:license-mit-blue.svg?style=flat-square)](http://badges.mit-license.org)

## For developers
To run the tests locally, in the test folder run:
```
g++ -o test.out test_heuristics.cpp -lgtest -lgtest_main -pthread && ./test.out
```
This requires Google test.

To assess performance on the medium test set, in the default folder run:
```
./crossing_counts.sh && python crossing_counts_sort.py < crossing_counts.txt
```
This requires `numpy` and `pace2024-verifier` to be installed.

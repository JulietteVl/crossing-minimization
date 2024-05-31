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
https://zenodo.org/doi/10.5281/zenodo.11400797

## LICENSE
[![License](http://img.shields.io/:license-BSD--3--Clause-blue.svg?style=flat-square)](https://opensource.org/license/BSD-3-clause)

## For developers
To run the tests locally, run the following in the `tests` folder:
```
g++ -o test.out test_heuristics.cpp -lgtest -lgtest_main -pthread && ./test.out
```
This requires Google test.

To assess performance on the medium test set, run the following in the `tests` folder:
```
./crossing_counts.sh && python crossing_counts_sort.py < crossing_counts.txt
```
This requires `numpy` and `pace2024-verifier` to be installed.

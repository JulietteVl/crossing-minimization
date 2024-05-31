# 1-Sided Crossing minimization
[![DOI](http://img.shields.io/:DOI-10.5281/zenodo.11400797-blue)](https://zenodo.org/doi/10.5281/zenodo.11400797)
[![License](http://img.shields.io/:license-BSD--3--Clause-blue)](https://opensource.org/license/BSD-3-clause)
[![Tests](https://github.com/JulietteVl/crossing-minimization/actions/workflows/cmake-single-platform.yml/badge.svg)](https://github.com/JulietteVl/crossing-minimization/actions/workflows/cmake-single-platform.yml)

## How to use
To use the heuristics, compile (i.e `g++ src/main.cpp`) and run:
```
path/to/excecutable input > output
```

## Requirements
No external library should be necessary.
Built with C++ 17.

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

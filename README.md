# 1-Sided Crossing minimization

To use the heuristics, compile (i.e `g++ src/main.cpp`) and run:
```
path/to/excecutable input output
```
This currently gives the best result between median or barycenter + greedy.

To plot the degree distribution:
```
python path_to_script/graphs_statistics.py -f path_to_your_graph/filename.gr
```

To get the minimum and maximum degree of different graphs:
```
python path_to_script/graphs_statistics.py -p path_to_your_graph -r start end
```

To run the tests locally, in the test folder run:
"g++ -o test.out test_heuristics.cpp -lgtest -lgtest_main -pthread && ./test.out"

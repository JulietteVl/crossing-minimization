#include <iostream>
#include <fstream>
#include <sstream>
#include <bits/stdc++.h>
#include <unistd.h>

#include "graph.h"

using namespace std;

int pipeline(int argc, char **argv);

int main(int argc, char **argv)
{
    pipeline(argc, argv);
    return 0;
}

int pipeline(int argc, char **argv)
{
    // Check and process arguments
    if (argc < 3)
    {
        std::cout << "Usage: " << argv[0] << " <input_file_path> <output_file_path>\n";
        return 1;
    }

    string input_name = argv[1];
    string output_name = argv[2];

    // Open input file and get input
    Graph graph(input_name);

    // Apply some algorithm to the input - we keep the best result
    graph.compute_crossing_numbers();

    graph.median_ordering();
    graph.greedy_ordering();
    
    graph.barycenter_ordering();
    graph.greedy_ordering();

    vector<vector<int>> fixed({{1, 2}});
    vector<vector<int>> free({{10, 11}});
    Graph G2(graph, fixed, free);

    cout << graph.best_crossing_count << endl;
    vector<int> order = graph.get_best_order();

    // Output
    ofstream output_file(output_name);

    if (!output_file.is_open()) {
        std::cerr << "Failed to open the output file." << std::endl;
        return 1;
    }

    for (auto v: order)
    {
        output_file << v << endl;
    }

    output_file.close();
    return 0;
}

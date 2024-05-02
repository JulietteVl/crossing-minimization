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
    vector<int> order;

    // Open input file and get input
    Graph graph(input_name);

    // Apply some algorithm to the input - we keep the best result

    graph.median_ordering();
    graph.greedy_ordering();
    
    graph.barycenter_ordering();
    graph.greedy_ordering();

    // Test contraction
    // vector<vector<int>> fixed({{1, 2}});
    // vector<vector<int>> free({{10, 11}});
    // Graph G2(graph, fixed, free);
    // G2.median_ordering(); // useless to use global heuristics on local graph...
    // graph.assign_order(G2.get_best_order());

    // Recursive contraction
    order = graph.get_best_order();

    // vector<vector<int>> fixed;
    // vector<vector<int>> free(order.begin(), order.begin() + graph.n1 / 2);
    // Graph G2(graph, fixed, free);

    // int s1, s2;
    // for (int i = 2; i < log2(graph.n1); i++){
    //     free.resize(0);
    //     for (int j = 0; j < pow(2, i); j++){
    //         s1 = (j * graph.n1) / pow(2, i);
    //         s2 = ((j + 1) * graph.n1) / pow(2, i);
    //         vector<int> subgroup(order.begin() + s1, order.begin() + s2);
    //         free.push_back(subgroup);
    //     }
    //     Graph G2(graph, fixed, free);
    // }
    // order = G2.get_best_order();
    // graph.assign_order(order);

    // Output
    cout << graph.best_crossing_count << endl; // just to know
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

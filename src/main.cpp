#include <iostream>
#include <fstream>
#include <sstream>
#include <bits/stdc++.h>
#include <unistd.h>

#include "library.h"
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

    int opt;
    bool apply_greedy = false;
    bool apply_barycenter = false;
    bool apply_median = false;
    while ((opt = getopt(argc, argv, "bmg")) != -1) {
        switch (opt) {
            case 'b':
                apply_barycenter = true;
                break;
            case 'm':
                apply_median = true;
                break;
            case 'g':
                apply_greedy = true;
                break;
            default:
                break;
        }
    }

    // Open input file and get input
    Graph graph(input_name);
    int n0 = graph.n0;
    int n1 = graph.n1;
    int m = graph.m;
    vector<int> offset = graph.offset;              // size n0 + n1 + 2 (1-indexed as the nodes and we need 1 more)
    vector<pair<int, int>> edges = graph.edges;     // size 2m

    // Apply some algorithm to the input
    vector<int> crossings;
    vector<int> order(n1);
    if (apply_greedy){
        crossings = crosssing_numbers(n0, n1, m, edges);
        iota(order.begin(), order.end(), n0 + 1);
    }
    if (apply_median){
        order = median_ordering(n0, n1, offset, edges); // array of size n1
    }
    if (apply_barycenter){
        order = barycenter_ordering(n0, n1, offset, edges); // array of size n1
    }
    if (apply_greedy){
        order = greedy_ordering(n0, n1, order, crossings);
    }

    cout << crossing_count(n0, n1, m, edges, order) << endl;

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

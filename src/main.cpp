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

    int opt;
    bool recursive = false;
    bool b = false;
    bool m =false;
    bool g = false;
    while ((opt = getopt(argc, argv, "rbmg")) != -1) {
        switch (opt) {
            case 'r':
                recursive = true;
                break;
            case 'm':
                m = true;
                break;
            case 'b':
                b = true;
                break;
            case 'g':
                g = true;
                break;
            default:
                break;
        }
    }

    // Open input file and get input
    Graph graph(input_name);

    // Apply some algorithm to the input - we keep the best result

    if (m)
        graph.median_ordering();
        order = graph.get_best_order();
        graph.assign_order(order);
    if (b)
        graph.barycenter_ordering();
    if (g)
        graph.greedy_ordering();

    // Test contraction
    order = graph.get_best_order();
    // Recursive contraction
    if (recursive){

        vector<int> split_free({graph.n1 / 2});

        int s;
        for (int i = 1; i < log2(graph.n1) + 1; i++){
            split_free.resize(0);
            for (int j = 1; j < pow(2, i); j++){
                s = (j * graph.n1) / pow(2, i);
                split_free.push_back(s);
                }
            Graph G2(graph, split_free);
            G2.greedy_ordering();
            order = G2.get_best_order();
            graph.assign_order(order);
        }
    }


    // Output
    // cout << graph.best_crossing_count << endl; // just to know
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

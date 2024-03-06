#include <iostream>
#include <fstream>
#include <sstream>
#include <bits/stdc++.h>
#include <unistd.h>

#include "library.h"

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
    ifstream input_file(input_name);

    if (!input_file.is_open())
    {
        cout << "Failed to open the input file." << endl;
    }
    string line;
    int n0, n1, m, u, v;
    vector<int> offset;             // size n0 + n1 + 2 (1-indexed as the nodes and we need 1 more)
    vector<pair<int, int>> edges;   // size 2m

    while (getline(input_file, line))
    {
        // Skip comment lines
        if (line[0] == 'c')
        {
            continue;
        }

        // Parse p-line
        else if (line[0] == 'p')
        {
            std::stringstream ss(line);
            string p, ocr;
            ss >> p >> ocr >> n0 >> n1 >> m;
            offset.resize(n0 + n1 + 2, 0);
        }

        else
        {
            // Read the edges
            // TODO should I check for comments in the middle?
            istringstream iss(line);
            iss >> u >> v;
            edges.push_back(make_pair(u, v));
            edges.push_back(make_pair(v, u));
        }
    }

    // Compute offset array
    sort(edges.begin(), edges.end(), compare_first);
    int j = 0;  // offset array index. There will be whatever at 0 because the nodes are 1-indexed.
    for (int i = 0; i < 2 * m; i++)
    {
        u = edges[i].first;
        while (j < u){
            offset[++j] = i;
        }
    }
    offset[j + 1] = 2 * m;

    if (false){
        // print edges
        cout << "sorted edges:" << endl;
        for (int i = 0; i < 2 * m; i++){
            cout << edges[i].first << ' ' << edges[i].second << endl;
        }
         cout << endl;
    }

    if (false){
        // print offset array
        cout << "offset array:" << endl;
        for (int i = 1; i <= n0 + n1 + 1; i++){
            cout << offset[i] << " ";
        }
        cout << endl;
    }

    // Apply some algorithm to the input
    vector<int> crossings = crosssing_numbers(n0, n1, m, edges);
    vector<int> order(n1);
    if (apply_greedy){
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
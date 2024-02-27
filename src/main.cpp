#include <iostream>
#include <fstream>
#include <sstream>
#include <bits/stdc++.h>

#include "library.h"

using namespace std;

int main(int argc, char **argv)
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
    ifstream input_file(input_name);

    if (!input_file.is_open())
    {
        cout << "Failed to open the input file." << endl;
    }
    string line;
    int n0, n1, m, u, v;
    int *offset;
    pair<int, int> *edges;

    int i = 0;
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
            edges = new pair<int, int> [2 * m];
            offset = new int[n0 + n1 + 2];
        }

        else
        {
            // Read the edges
            // TODO should I check for comments in the middle?
            istringstream iss(line);
            iss >> u >> v;
            edges[i++] = make_pair(u, v);
            edges[i++] = make_pair(v, u);
        }
    }

    sort(edges, edges + 2 * m, compare_first);
    int j = 0;  // offset array index. There will be nothing at 0 because the nodes are 1-indexed.
    for (int i = 0; i < 2 * m; i++)
    {
        u = edges[i].first;
        while (j < u){
            offset[++j] = i;
        }
    }
    offset[j + 1] = 2 * m;

    // cout << "sorted edges:" << endl;
    // for (int i = 0; i < 2 * m; i++){
    //     cout << edges[i].first << ' ' << edges[i].second << endl;
    // }

    // cout << endl;

    cout << "offset array:" << endl;
    for (int i = 1; i <= n0 + n1 + 1; i++){
        cout << offset[i] << " ";
    }

    // cout << endl;

    // Apply some algorithm to the input
    // Barycenter heuristic
    pair<int, double>* average_position = new pair<int, double> [n1 + 1];
    for (int i = n0 + 1; i <= n0; i++)
    {
        double sum = 0;
        int count = 0;

        for (int j = offset[i]; j < offset[j + 1]; i++)
        {
            sum += edges[j].second;
            count++;
        }
        if (count > 0) average_position[i] = make_pair(i, sum / count);
        else average_position[i] = make_pair(i, 0);
    }

    sort(average_position + n0, average_position + n0, compare_second);

    // Output
    ofstream output_file(output_name);

    if (!output_file.is_open()) {
        std::cerr << "Failed to open the output file." << std::endl;
        return 1;
    }

    for (int i = n0 + 1; i <= n0 + n1; i++)
    {
        output_file << average_position[i].first << endl;
    }

    output_file.close();

    delete[] offset;
    delete[] edges;
    return 0;
}
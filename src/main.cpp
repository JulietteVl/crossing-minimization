#include <iostream>
#include <fstream>
#include <sstream>
#include <bits/stdc++.h>

using namespace std;

const int N = 100;

bool compareAverage(const pair<int, double> &a, const pair<int, double> &b)
{
    return a.second < b.second;
}

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
    pair<int, int> *edges = new pair<int, int> [m];
    int *offset = new int[n0 + n1];

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
            break;
        }

        else
        {
            // Read the edges
            int j = 0;  // offset array index. There will be nothing at 0 because the nodes are 1-indexed.
            for (int i = 0; i < m; i++)
            {
                // assumes that u are in order as in the instances.
                istringstream iss(line);
                iss >> u >> v;
                edges[m] = make_pair(u, v);
                while (j < u){
                    offset[j] = i;
                    j++;
                }
            }
        }
    }

    // Apply some algorithm to the input
    // Barycenter heuristic
    pair<int, double> averagePosition[N];
    for (int i = n0 + 1; i <= n0 + n1; i++)
    {
        double sum = 0;
        int count = 0;

        for (int neighbor = edges[offset[i]].second; neighbor < edges[offset[i]].second; i++)
        {
            sum += neighbor;
            count++;
        }

        averagePosition[i] = make_pair(i, sum / count);
    }

    sort(averagePosition + n0, averagePosition + n0 + n1, compareAverage);

    // Output
    ofstream output_file(output_name);

    if (!output_file.is_open()) {
        std::cerr << "Failed to open the output file." << std::endl;
        return 1;
    }

    for (int i = n0 + 1; i <= n0 + n1; i++)
    {
        output_file << averagePosition[i].first << endl;
    }

    output_file.close();
    delete[] edges;
    return 0;
}
#include <bits/stdc++.h>

using namespace std;

struct Graph{
    int n0, n1, m;
    vector<pair<int, int>> edges;   // size 2m, we store them in both directions.
    vector<int> offset;             // size n0 + n1 + 2 (1-indexed as the nodes and we need 1 more)
    vector<int> order;
    vector<int> crossings;
};

Graph input(string input_name)
{
    Graph graph;

    // Open input file and get input
    ifstream input_file(input_name);

    if (!input_file.is_open())
    {
        cout << "Failed to open the input file." << endl;
    }
    string line;
    int u, v;            

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
            ss >> p >> ocr >> graph.n0 >> graph.n1 >> graph.m;
            graph.offset.resize(graph.n0 + graph.n1 + 2, 0);
        }

        else
        {
            // Read the edges
            // TODO should I check for comments in the middle?
            istringstream iss(line);
            iss >> u >> v;
            graph.edges.push_back(make_pair(u, v));
            graph.edges.push_back(make_pair(v, u));
        }
    }

    // Compute offset array
    sort(graph.edges.begin(), graph.edges.end(), compare_first);
    int j = 0;  // offset array index. There will be whatever at 0 because the nodes are 1-indexed.
    for (int i = 0; i < 2 * graph.m; i++)
    {
        u = graph.edges[i].first;
        while (j < u){
            graph.offset[++j] = i;
        }
    }
    graph.offset[j + 1] = 2 * graph.m;

    return graph;
}

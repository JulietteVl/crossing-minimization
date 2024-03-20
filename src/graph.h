#include <bits/stdc++.h>
#include "helpers.h"

using namespace std;

class Graph{
    public:
        int n0, n1, m;
        int best_crossing_count = INT_MAX;
        vector<pair<int, int>> edges;   // size 2m, we store them in both directions.
        vector<int> offset;             // size n0 + n1 + 2 (1-indexed as the nodes and we need 1 more)
        vector<int> order;              // the one we are working on at the moment
        vector<int> best_order;
        vector<int> crossings;
    public:
        Graph(string input_name);
        ~Graph();
        void update_best();
        int crossing_count();
        void compute_crossing_numbers();
        void barycenter_ordering();
        void median_ordering();
        void greedy_ordering();
};

Graph::Graph(string input_name)
{
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

    // initialise order
    order.resize(n1);
    iota(order.begin(), order.end(), n0 + 1);
}
Graph::~Graph(){}

void Graph::update_best(){
    if (crossing_count() < best_crossing_count){
        vector<int> copy(order);
        best_order = order;
    }
}

int Graph::crossing_count(){
    edges.resize(m);                                        // remove duplicates
    vector<int> position = order_to_position(order, n0);    // position between 1 and n1!
    sort(edges.begin(), edges.end(), make_comparison(position, n0));    // sort lexicographically using the order.
    int po2 = next_power_of_2(n1);
    vector<int> tree(2 * po2, 0);
    int S = 0;
    for (auto e: edges){
        S += sum(tree, po2, position[e.second - n0 - 1] + 1, n1);   // any edge that happened earlier in the lexicographic order and has its other endpoint after crosses e 
                                                                    // (or is a duplicate of e)
        update(tree, po2, position[e.second - n0 - 1]);             // add the edge to the segment tree
    }
    return S;
}

void Graph::compute_crossing_numbers(){
    // quadratic in the number of edges
    crossings.resize(n1 * n1, 0);
    int u, v;
    for (int i = 0;  i < m; i++){
        for (int j = i + 1;  j < m; j++){
            if(i == 6 && j == 1){
                int a = 0;
            }
            u = edges[i].second - n0 - 1;   // offset to have the right indices,
            v = edges[j].second - n0 - 1;   // This is not the node number anymore

            if (u!=v && edges[i].first < edges[j].first){ // Note that i < j and therefore the fixed layer vertices are ordered
                crossings[n1 * v + u]++;
            }
        }
    }
}


void Graph::barycenter_ordering()
{
    // return the free vertices ordered by the average of their neighbours.
    vector<pair<int, double>> average_position(n1);
    order.resize(n1);
    for (int i = n0 + 1; i <= n0 + n1; i++)
    {
        double sum = 0;
        int count = 0;

        for (int j = offset[i]; j < offset[i + 1]; j++)
        {
            sum += edges[j].second;
            count++;
        }
        if (count > 0)
            average_position[i - n0 - 1] = make_pair(i, sum / count);
        else
            average_position[i - n0 - 1] = make_pair(i, 0);
    }
    if (false){
        cout << "average position:" << endl;
        for (int i = 0; i < n1; i++){
            cout << average_position[i].first << " " << average_position[i].second << endl;
        }
    }
    sort(average_position.begin(), average_position.end(), compare_second);
    for (int i = 0; i < n1; i++)
    {
        order[i] = average_position[i].first;
    }
    update_best();
}

void Graph::median_ordering()
{
    // return the free vertices ordered by the median of their neighbours.
    vector<pair<int, double>>median_position(n1);
    order.resize(n1);
    for (int i = n0 + 1; i <= n0 + n1; i++)
    {
        int middle = (offset[i] + offset[i + 1] - 1) / 2;
        if (offset[i] < offset[i + 1])
            median_position[i - n0 - 1] = make_pair(i, edges[middle].second);
        else
            median_position[i - n0 - 1] = make_pair(i, 0);
    }
    if (false){ // display medians
        cout << "median position:" << endl;
        for (int i = 0; i < n1; i++){
            cout << median_position[i].first << " " << median_position[i].second << endl;
        }
    }
    sort(median_position.begin(), median_position.end(), compare_second);
    for (int i = 0; i < n1; i++)
    {
        order[i] = median_position[i].first;
    }
    update_best();
}

void Graph::greedy_ordering(){
    int u, v;
    int temp, i = 0, j = 1; // we look at two vertices of consecutive positions i, j
    while(j < n1){
        u = order[i] - n0 - 1;
        v = order[j] - n0 - 1;
        if (crossings[n1 * u + v] > crossings[n1 * v + u]){
            temp = order[i];
            order[i] = order[j];
            order[j] = temp;
            if (i > 0){
                i--;            // The vertex i-1 could have been affected.
                j--;
            } 
        }
        else{
            i++;
            j++;
        }
    }

    update_best();
}
#include <bits/stdc++.h>
#include "helpers.h"

using namespace std;

class Graph{
    public:
        int n0, n1, m;
        int best_crossing_count = INT_MAX;
        vector<Edge> edges;             // size 2m, we store them in both directions. 
        vector<int> offset;             // size n0 + n1 + 2 (1-indexed as the nodes and we need 1 more)
        vector<int> crossings;
        vector<vector<int>> group;      // group[i]: original vertices contracted in i. Empty is there has been no contraction.
    private:                            // orders need to be reconstructed if we have merged vertices
        vector<int> order;              // the one we are working on at the moment
        vector<int> best_order;
    public:
        Graph(string input_name);
        Graph(Graph graph, vector<vector<int>> contract_fixed, vector<vector<int>> contract_free);
        ~Graph();
        vector<int> get_order(){return reconstruct_order(order);}
        vector<int> get_best_order(){return reconstruct_order(best_order);}
        int crossing_count();
        void compute_crossing_numbers();
        void barycenter_ordering();
        void median_ordering();
        void greedy_ordering();
    private:
        vector<int> reconstruct_order(vector<int> order_contracted);
        void make_offset();
        void update_best();
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
        }

        else
        {
            // Read the edges
            // TODO should I check for comments in the middle?
            istringstream iss(line);
            iss >> u >> v;
            edges.push_back(Edge(u, v));
            edges.push_back(Edge(v, u));
        }
    }

    // Compute offset array
    sort(edges.begin(), edges.end());
    make_offset();

    // initialise order
    order.resize(n1);
    iota(order.begin(), order.end(), n0 + 1);
}

// TODO This version only handles correctly merging vertices 1 - n0
Graph::Graph(Graph graph, vector<vector<int>> contract_fixed, vector<vector<int>> contract_free){
    n0 = graph.n0; n1 = graph.n1; m = graph.m;      // We will update this as we go
    vector<bool> is_grouped(n0 + n1 + 1, false);    // we need to keep the vertices that have not been contracted
    vector<int> replacement(n0 + n1 + 1);           // what vertex i is replaced by in the new graph

    vector<int> placeholder(0);
    group.push_back(placeholder);   // sticking to 1-indexing, sadly

    // get n0, n1
    for (auto group_: contract_fixed){
        n0 -= group_.size() - 1;
        for (int v: group_){ is_grouped[v] = true; }
    }
    for (auto group_: contract_free){
        n1 -= group.size() - 1 ;
        for (int v: group_){ is_grouped[v] = true; }
    }
    // handle the single vertices: put them in their own group
    for (int v = 1; v <= graph.n0; v++){
        if(!is_grouped[v]){
            contract_fixed.push_back({v});
        }  
    }
    for (int v = graph.n0 + 1; v <= graph.n0 + graph.n1; v++){
        if(!is_grouped[v]){
            contract_free.push_back({v});
        }
    }
    // If the grouped vertices were consecutive in the graph order, this preserves the order.
    // fixed layer:
    sort(contract_fixed.begin(), contract_fixed.end());
    // free layer:
    vector<int> position = order_to_position(graph.order, graph.n0);
    sort(contract_free.begin(), contract_free.end(), make_average_comparison(position, graph.n0));

    int group_number = 1;
    // handle the groups
    for (auto group_: contract_fixed){
        for (int v: group_){
            replacement[v] = group_number;
        }
        group.push_back(group_);
        group_number ++;
    }
    for (auto group_: contract_free){
        for (int v: group_){
            replacement[v] = group_number;
        }
        group.push_back(group_);
        group_number ++;
    }
    // rename edges
    vector<Edge> edges_temp;
    for (Edge e: graph.edges){
        Edge f(replacement[e.first], replacement[e.second]);
        edges_temp.push_back(f);
    }
    sort(edges_temp.begin(), edges_temp.end());
    for (int i = 0; i < edges_temp.size(); i++){
        if (edges_temp[i] == edges_temp[i + 1]){
            edges_temp[i + 1].weight += edges_temp[i].weight;
            edges_temp[i].weight = 0;
        }
    }
    for (Edge e: edges_temp){
        if (e.weight > 0){
            edges.push_back(e);
        }
    }
    m = edges.size() / 2;
    make_offset();

    // initialise order
    order.resize(n1);
    std::iota(order.begin(), order.end(), n0 + 1);
}

Graph::~Graph(){}

// assumes the edges are sorted
void Graph::make_offset(){
    offset.resize(n0 + n1 + 2, 0);
    int u;
    int j = 0;  // offset array index. There will be whatever at 0 because the nodes are 1-indexed.
    for (int i = 0; i < 2 * m; i++)
    {
        u = edges[i].first;
        while (j < u){
            offset[++j] = i;
        }
    }
    offset[j + 1] = 2 * m;
}

void Graph::update_best(){
    if (crossing_count() < best_crossing_count){
        vector<int> copy(order);
        best_order = order;
    }
}

vector<int> Graph::reconstruct_order(vector<int> order_contracted){
    if (group.empty()){     // nothing has been contracted
        return order_contracted;
    }
    vector<int> order_full;
    for (int v: order){
        for (int u: group[v]){
            order_full.push_back(u);
        }
    }
    return order_full;
}

// -------------------- Crossings --------------------

// uses the current order and not necessarily the best one.
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

// -------------------- Heuristics --------------------

void Graph::barycenter_ordering()
{
    // return the free vertices ordered by the average of their neighbours.
    vector<pair<double, int>> average_position(n1); // for sorting purposes, let us store the median and then the vertex number
    order.resize(n1);
    int count;
    for (int i = n0 + 1; i <= n0 + n1; i++)
    {
        double sum = 0;
        count = offset[i + 1] - offset[i];

        for (int j = offset[i]; j < offset[i + 1]; j++)
        {
            sum += edges[j].second;
        }
        if (count > 0)
            average_position[i - n0 - 1] = make_pair(sum / count, i);
        else
            average_position[i - n0 - 1] = make_pair(0, i);
    }
    sort(average_position.begin(), average_position.end());
    for (int i = 0; i < n1; i++)
    {
        order[i] = average_position[i].second;
    }
    update_best();
}

void Graph::median_ordering()
{
    // return the free vertices ordered by the median of their neighbours.
    vector<pair<double, int>>median_position(n1);   // for sorting purposes, let us store the median and then the vertex number
    order.resize(n1);
    for (int i = n0 + 1; i <= n0 + n1; i++)
    {
        int middle = (offset[i] + offset[i + 1] - 1) / 2;
        if (offset[i] == offset[i + 1])
            median_position[i - n0 - 1] = make_pair(0, i);
        else if ((offset[i + 1] - offset[i]) % 2 == 1)
            median_position[i - n0 - 1] = make_pair(edges[middle].second, i);
        else
            median_position[i - n0 - 1] = make_pair((edges[middle].second + edges[middle + 1].second)/2.0, i);
    }
    sort(median_position.begin(), median_position.end());
    for (int i = 0; i < n1; i++)
    {
        order[i] = median_position[i].second;
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
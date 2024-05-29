#include <bits/stdc++.h>
#include "helpers.h"

using namespace std;

class Graph{
    public:
        int m;
        long int n0, n1;
        long int best_crossing_count = INT_MAX;  // if the graph is contracted, this gives the crosssing count of the contracted vertices. currently edge weight is not taken into account
                                            // it seems useless to use anything but the crossing count in the original graph. TODO?
        vector<Edge> edges;             // size 2m, we store them in both directions. 
        vector<int> offset;             // size n0 + n1 + 2 (1-indexed as the nodes and we need 1 more)
        vector<int> crossings;
        vector<vector<int>> group;      // group[i]: original vertices contracted in i. Empty is there has been no contraction.
    protected:                          // orders need to be reconstructed if we have merged vertices
        vector<int> order;              // the one we are working on at the moment, in the contracted graph. 0 indexed
        vector<int> best_order;         // in the contracted graph. 0 indexed
        vector<vector<int>> adj;
    public:
        Graph();                        // expect std in
        Graph(int m);                   // empty graph
        Graph(string input_name);
        Graph(Graph graph, vector<vector<int>> contract_fixed, vector<vector<int>> contract_free);  // use node numbers
        Graph(Graph graph, vector<int> split_free);                        // split order (currently free only)
        ~Graph();
        Graph get_copy_without_duplicates();
        void assign_order(vector<int> external_order);
        vector<int> get_order(){return reconstruct_order(order);}                   // in the original graph
        vector<int> get_best_order(){return reconstruct_order(best_order);}         // in the original graph
        int crossing_count();                                                       // in the contracted graph
        void compute_crossing_numbers();                                            // in the contracted graph
        // global heuristics: they work on contracted vertices, but there is probably no point in not using them on the original graph only.
        void barycenter_ordering();
        void median_ordering();
        // local heuristic
        void greedy_ordering();
    private:
        void construct(Graph graph, vector<vector<int>> contract_fixed, vector<vector<int>> contract_free);
        vector<int> reconstruct_order(vector<int> order_contracted);
        void make_offset();
        void update_best();
};

Graph::Graph()
{
    string line;
    int u, v, count = 0;  

    while (getline(cin, line))
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
            istringstream iss(line);
            iss >> u >> v;
            edges.push_back(Edge(u, v));
            edges.push_back(Edge(v, u));
            count++;
            if (count >= m){
                break;
            }
        }
    }

    // Compute offset array
    sort(edges.begin(), edges.end());
    make_offset();

    // initialise order
    order.resize(n1);
    iota(order.begin(), order.end(), n0 + 1);
    update_best();
}

Graph::Graph(int m){
    m = m;
}

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
    update_best();
}

// The vertices grouped do not have to be consecutive in the order. the order of the vertices in the groups depend on the current order.
Graph::Graph(Graph graph, vector<vector<int>> contract_fixed, vector<vector<int>> contract_free){
    n0 = graph.n0; n1 = graph.n1;                   // We will update this as we go
    vector<bool> is_grouped(n0 + n1 + 1, false);    // we need to keep the vertices that have not been contracted

    // get n0, n1
    for (auto subgroup: contract_fixed){
        n0 -= subgroup.size() - 1;
        for (int v: subgroup){ is_grouped[v] = true; }
    }
    for (auto subgroup: contract_free){
        n1 -= subgroup.size() - 1 ;
        for (int v: subgroup){ is_grouped[v] = true; }
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

    construct(graph, contract_fixed, contract_free);
}

// The vertices grouped do not have to be consecutive in the order. the order of the vertices in the groups depend on the current order.
Graph::Graph(Graph graph, vector<int> split_free){
    // we currently do not contract fixed layer:
    split_free.push_back(graph.n1);
    vector<vector<int>> contract_fixed;
    for (int v = 1; v <= graph.n0; v++){
        contract_fixed.push_back({v});
    }
    vector<vector<int>> contract_free;
    int i1 = 0;
    int i2;
    for (int i2: split_free){
        if (i1 == i2){
            continue;
        }
        vector<int> subgroup(graph.best_order.begin() + i1, graph.best_order.begin() + i2);
        contract_free.push_back(subgroup);
        i1 = i2;  
    }

    construct(graph, contract_fixed, contract_free);
}

Graph::~Graph(){}

void Graph::construct(Graph graph, vector<vector<int>> contract_fixed, vector<vector<int>> contract_free){
    n0 = contract_fixed.size();
    n1 = contract_free.size();
    m = graph.m;

    vector<int> placeholder(0);
    group.push_back(placeholder);   // sticking to 1-indexing, sadly

    vector<int> replacement(graph.n0 + graph.n1 + 1);   // what vertex i is replaced by in the new graph
                                                        // with respect to the input graph - not the original one.
                                                        // used to make edges only
    int group_number = 1;
    // handle the groups
    for (auto subgroup: contract_fixed){
        for (int v: subgroup){
            replacement[v] = group_number;
        }
        if (graph.group.empty()){
            group.push_back(subgroup);
        }
        else{
            vector<int> subgroup2;
            for (int v: subgroup){
                for (int u: graph.group[v]){
                    subgroup2.push_back(u);
                }
            }
            group.push_back(subgroup2);
        }
        group_number ++;
    }
    for (auto subgroup: contract_free){
        for (int v: subgroup){
            replacement[v] = group_number;
        }
        if (graph.group.empty()){
            group.push_back(subgroup);
        }
        else{
            vector<int> subgroup2;
            for (int v: subgroup){
                for (int u: graph.group[v]){
                    subgroup2.push_back(u);
                }
            }
            group.push_back(subgroup2);
        }
        group_number ++;
    }
    // rename edges
    vector<Edge> edges_temp;
    for (Edge e: graph.edges){
        Edge f(replacement[e.first], replacement[e.second], e.weight);
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
    update_best();
}

Graph Graph::get_copy_without_duplicates(){
    if (adj.empty()){
        adj.resize(n0 + n1 + 1);
        for (Edge e: edges){
            adj[e.first].push_back(e.second);
        }
    }
    // we need the vertices to have been sorted with eg barycenter to proceed
    vector<vector<int>> contract_fixed;
    vector<vector<int>> contract_free;
    int i = 0;
    while (i < n1){
        int j = 1;
        vector<int> subgroup(1, order[i]);
        while (i + j < n1 && adj[order[i]] == adj[order[i + j]]){
            subgroup.push_back(order[i + j]);
            j++;
        }
        i+=j;
        contract_free.push_back(subgroup);
    }
    Graph G2(*this, contract_fixed, contract_free);
    return G2;
}

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
    int cc = crossing_count();
    if (cc < best_crossing_count){
        vector<int> copy(order);
        best_crossing_count = cc;
        best_order = copy;
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

// the order must be either in the same graph or in the original graph.
// Assumes that the vertices in the groups are consecutive in the given order
void Graph::assign_order(vector<int> external_order){
    if (n1 == external_order.size()) 
        order = external_order;
    else{
        vector<int> position = order_to_position(external_order);    // position between 1 and n1!
        vector<pair<int, int>> group_position;
        for (int i = n0 + 1; i <= n0 + n1; i++){
            int v = group[i][0] - n0 - 1;
            group_position.push_back(make_pair(position[v], i));
        }
        sort(group_position.begin(), group_position.end());
        for (int i = 0; i < n1; i++)
        {
            order[i] = group_position[i].second;
        }
    }
    update_best();
}

// -------------------- Crossings --------------------

// uses the current order and not necessarily the best one.
int Graph::crossing_count(){
    // This does not include intra vertex crossing count
    vector<Edge> edges_unique(edges.begin(), edges.begin() + m);                                       // remove duplicates
    vector<int> position = order_to_position(order, n0);    // position between 1 and n1!
    sort(edges_unique.begin(), edges_unique.end(), make_comparison(position, n0));    // sort lexicographically using the order.
    int po2 = next_power_of_2(n1);
    vector<int> tree(2 * po2, 0);
    int S = 0;
    for (auto e: edges_unique){
        S += sum(tree, po2, position[e.second - n0 - 1] + 1, n1);   // any edge that happened earlier in the lexicographic order and has its other endpoint after crosses e 
                                                                    // (or is a duplicate of e)
        update(tree, po2, position[e.second - n0 - 1]);             // add the edge to the segment tree
    }
    return S;
}

void Graph::compute_crossing_numbers(){
    // quadratic in the number of edges
    long int max_size = crossings.max_size();
    if (pow(n1, 2) > max_size){
        return;
    }
    crossings.resize(pow(n1, 2), 0);
    long int u, v;
    for (int i = 0;  i < m; i++){
        for (int j = i + 1;  j < m; j++){
            u = edges[i].second - n0 - 1;   // offset to have the right indices,
            v = edges[j].second - n0 - 1;   // This is not the node number anymore
            // Note that i < j and therefore the fixed layer vertices are ordered
            // So the two edges do not cause a conflict in order uv
            if (edges[i].first == edges[j].first || u == v){
                continue;
            }
            // the order vu result in a crossing
            else {
                crossings[n1 * v + u] += edges[i].weight * edges[j].weight;
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
        int count = 0;

        for (int j = offset[i]; j < offset[i + 1]; j++)
        {
            sum += edges[j].second * edges[j].weight;
            count += edges[j].weight;
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
    vector<pair<double, int>>median_position_2(n1);
    order.resize(n1);
    int n_edges;
    int middle;
    for (int i = n0 + 1; i <= n0 + n1; i++)
    {   
        if (!group.empty()){ // We will see each edges a constant number of time, so this is still linear.
            n_edges = 0;
            for (int j = offset[i]; j < offset[i + 1]; j++){
                n_edges += edges[j].weight;
            }
            int cw = 0; // cumulative weight
            // find middle:
            for (int j = offset[i]; j < offset[i + 1]; j++){
                cw += edges[j].weight;
                if (cw >= (n_edges + 1) / 2){ // adjust for odd numbers: if there are 5 edges, the middle is the 3rd one.
                    middle = j;
                    break;
                }
            }
        }
        else{ // no group
            n_edges = offset[i + 1] - offset[i];
            middle = (offset[i] + offset[i + 1] - 1) / 2;
        }

        if (n_edges == 0)
            median_position[i - n0 - 1] = make_pair(0, i);
        else
            median_position[i - n0 - 1] = make_pair(edges[middle].second, i);

        // median version 2, performs better on some but not all examples
        if (n_edges == 0)
            median_position_2[i - n0 - 1] = make_pair(0, i);
        else if ((n_edges % 2 == 1) || (edges[middle].weight > 1))
            median_position_2[i - n0 - 1] = make_pair(edges[middle].second, i);
        else{ // even number of edges AND edge of weight 1 (if weight > 1, the two original middle edges have been merged)
            median_position_2[i - n0 - 1] = make_pair((edges[middle].second + edges[middle + 1].second)/2.0, i);
        }  
    }
    sort(median_position.begin(), median_position.end());
    for (int i = 0; i < n1; i++)
    {
        order[i] = median_position[i].second;
    }
    update_best();

    sort(median_position_2.begin(), median_position_2.end());
    for (int i = 0; i < n1; i++)
    {
        order[i] = median_position_2[i].second;
    }
    update_best();
}

void Graph::greedy_ordering(){
    if (n1*n1 > crossings.max_size()){
        return;
    }
    if (crossings.empty()){
        compute_crossing_numbers();
    }
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
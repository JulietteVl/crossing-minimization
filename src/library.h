#include <bits/stdc++.h>
#include "helpers.h"

using namespace std;

int crossing_count(int n0, int n1, int m, vector<pair<int, int>> edges_to_copy, vector<int> order);
vector<int> crosssing_numbers(int n0, int n1, int m, vector<pair<int, int>> edges, vector<int> order);

vector<int> barycenter_ordering(int n0, int n1, vector<int> offset, vector<pair<int, int>> edges);
vector<int> median_ordering(int n0, int n1, vector<int> offset, vector<pair<int, int>> edges);
vector<int> greedy_ordering(int n0, int n1, vector<int> initial_order, vector<int> crossings);

int crossing_count(int n0, int n1, int m, vector<pair<int, int>> edges, vector<int> order){
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

// quadratic in the number of edges
vector<int> crosssing_numbers(int n0, int n1, int m, vector<pair<int, int>> edges){
    vector<int> crossings(n1 * n1, 0);
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
    return crossings;
}

vector<int> greedy_ordering(int n0, int n1, vector<int> initial_order, vector<int> crossings){
    int u, v;
    vector<int> order(initial_order);
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
    return order;
}

vector<int> barycenter_ordering(int n0, int n1, vector<int> offset, vector<pair<int, int>> edges)
{
    // return the free vertices ordered by the average of their neighbours.
    vector<pair<int, double>> average_position(n1);
    vector<int> order(n1);
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
    return order;
}

vector<int> median_ordering(int n0, int n1, vector<int> offset, vector<pair<int, int>> edges)
{
    // return the free vertices ordered by the median of their neighbours.
    vector<pair<int, double>>median_position(n1);
    vector<int> order(n1);
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
    return order;
}


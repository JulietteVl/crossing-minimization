#include <bits/stdc++.h>

using namespace std;

int *barycenter_ordering(int n0, int n1, int *offset, pair<int, int> *edges);
int *median_ordering(int n0, int n1, int *offset, pair<int, int> *edges);
int *greedy_ordering(int n0, int n1, int *offset, pair<int, int> *edges);
bool compare_first(const pair<int, double> &a, const pair<int, double> &b);
bool compare_second(const pair<int, double> &a, const pair<int, double> &b);

int *barycenter_ordering(int n0, int n1, int *offset, pair<int, int> *edges)
{
    // return the free vertices ordered by the average of their neighbours.
    pair<int, double> *average_position = new pair<int, double>[n1];
    int* order = new int[n1];
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
    // cout << "average position:" << endl;
    // for (int i = 0; i < n1; i++){
    //     cout << average_position[i].first << " " << average_position[i].second << endl;
    // }
    sort(average_position, average_position + n1, compare_second);
    for (int i = 0; i < n1; i++)
    {
        order[i] = average_position[i].first;
    }
    return order;
}

int *median_ordering(int n0, int n1, int *offset, pair<int, int> *edges)
{
    // return the free vertices ordered by the median of their neighbours.
    pair<int, double> *median_position = new pair<int, double>[n1];
    int* order = new int[n1];
    for (int i = n0 + 1; i <= n0 + n1; i++)
    {
        int middle = (offset[i] + offset[i + 1] - 1) / 2;
        if (offset[i] < offset[i + 1])
            median_position[i - n0 - 1] = make_pair(i, edges[middle].second);
        else
            median_position[i - n0 - 1] = make_pair(i, 0);
    }
    // cout << "median position:" << endl;
    // for (int i = 0; i < n1; i++){
    //     cout << median_position[i].first << " " << median_position[i].second << endl;
    // }
    sort(median_position, median_position + n1, compare_second);
    for (int i = 0; i < n1; i++)
    {
        order[i] = median_position[i].first;
    }
    return order;
}

bool compare_first(const pair<int, double> &a, const pair<int, double> &b)
{
    if (a.first == b.first)
    {
        return a.second < b.second;
    }
    return a.first < b.first;
}

bool compare_second(const pair<int, double> &a, const pair<int, double> &b)
{
    if (a.second == b.second){
        return a.first < b.first;
    }
    return a.second < b.second;
}
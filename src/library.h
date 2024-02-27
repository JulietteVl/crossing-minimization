#include <bits/stdc++.h>

using namespace std;

int *barycenter_ordering(int n0, int n1, int *offset, pair<int, int> *edges);
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
    return a.second < b.second;
}
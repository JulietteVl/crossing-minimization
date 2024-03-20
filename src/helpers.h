#include <bits/stdc++.h>

using namespace std;

// segement tree
vector<int> order_to_position(vector<int> order, int n0);
int next_power_of_2(int v);
void update(vector<int>& tree, int n, int k);
int sum(vector<int> tree, int n, int a, int b);

// comparisons
bool compare_first(pair<int, int> a, pair<int, int> b);
bool compare_second(pair<int, int> a, pair<int, int> b);
auto make_comparison(vector<int> order, int n0);





// segment tree
vector<int> order_to_position(vector<int> order, int n0){
    int n1 = order.size();
    vector<int> position(n1);
    for (int i = 0; i < n1; i++){
        position[order[i] - n0 - 1] = i + 1;
    }
    return position;
}

int next_power_of_2(int v){ // 32 bit value
    v--;
    v |= v >> 1;
    v |= v >> 2;
    v |= v >> 4;
    v |= v >> 8;
    v |= v >> 16;
    v++;
    return v;
}

void update(vector<int>& tree, int n, int k)
{
    // 1 <= k <= n, n power of 2
    k += n - 1;
    tree[k]++;
    for (k /= 2; k >= 1; k /= 2)
    {
        tree[k] = tree[2 * k] + tree[2 * k + 1];
    }
    return;
}

int sum(vector<int> tree, int n, int a, int b)
{
    // n power of 2
    a += n - 1;
    b += n - 1;
    int s = 0;
    while (a <= b)
    {
        if (a % 2 == 1)
            s += tree[a++];
        if (b % 2 == 0)
            s += tree[b--];
        a /= 2;
        b /= 2;
    }
    return s;
}

// comparisons
bool compare_first(pair<int, int> a, pair<int, int> b)
{
    if (a.first == b.first)
    {
        return a.second < b.second;
    }
    return a.first < b.first;
}

bool compare_second(pair<int, int> a, pair<int, int> b)
{
    if (a.second == b.second){
        return a.first < b.first;
    }
    return a.second < b.second;
}

auto make_comparison(vector<int> position, int n0){
    return [position, n0](pair<int, int> a, pair < int, int> b)
    {
        if (a.first == b.first)
        {
            return position[a.second - n0 - 1] < position[b.second - n0 - 1];
        }
        return a.first < b.first;
    };
}

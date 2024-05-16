#include <bits/stdc++.h>

using namespace std;


struct Edge{
    int first;
    int second;
    int weight = 1;

    Edge(){}
    Edge(int u, int v){
        first = u; second = v;
    }
    Edge(int u, int v, int weight){
        first = u; second = v;
        this->weight = weight;
    }
    ~Edge(){}

    friend bool operator< (const Edge lhs, const Edge rhs){
        if (lhs.first == rhs.first)
        {
            return lhs.second < rhs.second;
        }
        return lhs.first < rhs.first;
    }

    bool operator== (Edge e){
        return (this->first == e.first) && (this->second == e.second);
    }
};


// segement tree

vector<int> order_to_position(vector<int> order, int n0);
int next_power_of_2(int v);
void update(vector<int>& tree, int n, int k);
int sum(vector<int> tree, int n, int a, int b);

// comparison

auto make_comparison(vector<int> position, int n0);
auto make_average_comparison(vector<int> position, int n0);



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

// comparison

auto make_comparison(vector<int> position, int n0){
    return [position, n0](Edge a, Edge b)
    {
        if (a.first == b.first)
        {
            return position[a.second - n0 - 1] < position[b.second - n0 - 1];
        }
        return a.first < b.first;
    };
}

auto make_average_comparison(vector<int> position, int n0){
    return[position, n0](vector<int> a, vector<int> b)
    {
        int pos_a = 0, pos_b = 0;
        for (int v: a){
            pos_a += position[v - n0 - 1];   // size n1, 0 indexed     
        }
        pos_a /= a.size();
        for (int v: b){
            pos_b+= position[v - n0 - 1];   // size n1, 0 indexed     
        }
        pos_b /= b.size();
        return pos_a < pos_b;
    };
}
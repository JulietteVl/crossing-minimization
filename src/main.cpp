#include <iostream>
#include <bits/stdc++.h>
#include <algorithm>

using namespace std;

const int N = 100;

bool compareAverage(const pair<int, double>& a, const pair<int, double>& b) {
    return a.second < b.second;
}

int main() {
    string line;
    int n0, n1, m;
    vector<int> adj[N];
   

    while (getline(cin, line)) {
        // Skip comment lines
        if (line[0] == 'c') {
            continue;
        }

        // Parse p-line
        if (line[0] == 'p') {
            std::stringstream ss(line);
            string p, ocr;
            ss >> p >> ocr >> n0 >> n1 >> m;
            //cout << n0 << n1 << m;
            break;
        }
    }

    // Read the edges
    int x, y;
    for (int i = 0; i < m; i++) {
        cin >> x >> y;
        adj[x].push_back(y);
        adj[y].push_back(x);
    }

    // Barycenter heuristic
    pair<int, double> averagePosition[N];
    for (int i = n0 + 1 ; i <= n0 + n1; i++) {
        //cout << "i " << i << endl; 
        double sum = 0;
        int count = 0;
        
        for (int neighbor : adj[i]) {
            sum += neighbor;
            count++;
        }
        // cout << sum / count << endl;
        
        averagePosition[i] = make_pair(i, sum / count);
    }

    sort(averagePosition + n0, averagePosition + n0 + n1, compareAverage);
    
    for (int i = n0 + 1 ; i <= n0 + n1; i++) {
        cout << averagePosition[i].first << endl;
    }

    return 0;
}
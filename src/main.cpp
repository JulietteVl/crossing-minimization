#include <iostream>
#include <string>

using namespace std;

int main() {
    string line;
    int n0, n1, m;

    while (getline(cin, line)) {
        // Skip comment lines
        if (line[0] == 'c') {
            continue;
        }

        // Parse p-line
        if (line[0] == 'p') {
            string ocr;
            cin >> ocr >> n0 >> n1 >> m;
            break;
        }
    }

    // Read the edges
    for (int i = 0; i < m; i++) {
        int x, y;
        cin >> x >> y;

        // Process the edge (x, y)
        // ...

        // Print the edge (x, y)
        cout << x << " " << y << endl;
    }

    return 0;
}
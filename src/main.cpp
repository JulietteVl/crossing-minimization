#include "graph.h"

using namespace std;

int pipeline(int argc, char **argv);

int main(int argc, char **argv)
{
    pipeline(argc, argv);
    return 0;
}

vector<int> order;
void term(int signum)
{
    for (auto v: order)
    {
        cout << v << endl;
    }
    exit(0);
}

int pipeline(int argc, char **argv)
{
    // handle signal
    struct sigaction action;
    memset(&action, 0, sizeof(struct sigaction));
    action.sa_handler = term;
    sigaction(SIGTERM, &action, NULL);

    // Check and process arguments
    if (argc < 2)
    {
        std::cout << "Usage: " << argv[0] << " <input_file_path> > <output_file_path>\n";
        return 1;
    }

    string input_name = argv[1];

    // Open input file and get input
    Graph graph(input_name);

    // Apply some algorithm to the input - we keep the best result

    graph.median_ordering();
    order = graph.get_best_order();
    graph.assign_order(order);

    graph.barycenter_ordering();
    graph.greedy_ordering();

    // Test contraction
    order = graph.get_best_order();
    // Recursive contraction
    int s = graph.n1 / 2;
    vector<int> split_free({s});
    for (int i = 1; i < log2(graph.n1) + 1; i++)
    {
        split_free.resize(0);
        for (int j = 1; j < pow(2, i); j++)
        {
            s = (j * graph.n1) / pow(2, i);
            split_free.push_back(s);
        }
        Graph G2(graph, split_free);
        G2.greedy_ordering();
        order = G2.get_best_order();
        graph.assign_order(order);
    }

    // Output
    for (auto v: order)
    {
        cout << v << endl;
    }

    return 0;
}

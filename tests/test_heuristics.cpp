#include <gtest/gtest.h>
#include "../src/graph.h"

TEST(example, test_greedy)
{
    Graph graph("example_input.gr");
    graph.compute_crossing_numbers();
    graph.greedy_ordering();
    ASSERT_EQ(graph.crossing_count(), 3);
}

TEST(example, test_median)
{
    Graph graph("example_input.gr");
    graph.median_ordering();
    vector<int> order({9, 6, 10, 11, 7, 8});
    ASSERT_EQ(graph.get_order(), order);
}

TEST(example, test_barycenter)
{
    Graph graph("example_input.gr");
    graph.barycenter_ordering();
    vector<int> order({9, 6, 11, 10, 7, 8});
    ASSERT_EQ(graph.get_order(), order);
}

TEST(example, test_count)
{
    Graph graph("example_input.gr");
    graph.median_ordering();
    ASSERT_EQ(graph.crossing_count(), 4);
    graph.barycenter_ordering();
    ASSERT_EQ(graph.crossing_count(), 3);
}

TEST(example, test_contraction)
{
    Graph G1("example_input.gr");
    G1.barycenter_ordering();
    vector<vector<int>> fixed({{1, 2}});
    vector<vector<int>> free({{11, 10}});
    Graph G2(G1, fixed, free);
    ASSERT_EQ(G1.crossing_count(), 3);
}

TEST(example, test_median_contraction)
{
    Graph G1("example_input.gr");
    vector<vector<int>> fixed({{1, 2}});
    vector<vector<int>> free({{10, 11}});
    Graph G2(G1, fixed, free);
    G2.median_ordering();
    ASSERT_EQ(G2.crossing_count(), 0);
    G1.assign_order(G2.get_best_order());
    ASSERT_EQ(G1.crossing_count(), 5);
}

TEST(example, test_position)
{
    int n0 = 5;
    vector<int> order {9, 6, 11, 10, 7, 8};
    vector<int> position{2, 5, 6, 1, 4, 3}; // 1 indexed for the segment tree
    ASSERT_EQ(order_to_position(order, n0), position);
}

int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
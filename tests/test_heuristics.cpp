#include <gtest/gtest.h>
#include "../src/graph.h"

TEST(example, test_median)
{
    Graph graph("example_input.gr");
    graph.compute_crossing_numbers();
    graph.median_ordering();
    graph.greedy_ordering();
    ASSERT_EQ(graph.crossing_count(), 3);
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
#include "gtest/gtest.h"
#include "DijkstraPathRouter.h"
#include <string>
#include <vector>
#include <chrono>

TEST(DijkstraPathRouterTest, ShortestPathBasic) {
    CDijkstraPathRouter router;
    auto v0 = router.AddVertex(std::string("A"));
    auto v1 = router.AddVertex(std::string("B"));
    auto v2 = router.AddVertex(std::string("C"));
    EXPECT_TRUE(router.AddEdge(v0, v1, 1.0));
    EXPECT_TRUE(router.AddEdge(v1, v2, 2.0));
    EXPECT_TRUE(router.AddEdge(v0, v2, 4.0));
    auto deadline = std::chrono::steady_clock::now() + std::chrono::seconds(1);
    EXPECT_TRUE(router.Precompute(deadline));
    std::vector<CPathRouter::TVertexID> path;
    double totalWeight = router.FindShortestPath(v0, v2, path);
    EXPECT_DOUBLE_EQ(totalWeight, 3.0);
    ASSERT_EQ(path.size(), 3u);
    EXPECT_EQ(path[0], v0);
    EXPECT_EQ(path[1], v1);
    EXPECT_EQ(path[2], v2);
}

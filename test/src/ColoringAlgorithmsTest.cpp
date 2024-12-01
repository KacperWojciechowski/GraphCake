#include <gmock/gmock.h>
#include <Graphs/AdjMatrix.hpp>
#include <Graphs/Algorithm.hpp>
#include <Graphs/ColoringAlgorithms.hpp>
#include <gtest/gtest.h>

using namespace testing;
using namespace Graphs::Algorithm;

namespace Graphs
{
struct GreedyColoringTests : public testing::Test
{
    void SetUp()
    {
        result = std::make_shared<ColoringResult>();
        sut = std::make_unique<GreedyColoring<notVerbose>>(result);
    }

    std::shared_ptr<ColoringResult> result;
    std::unique_ptr<GreedyColoring<notVerbose>> sut;

    AdjMatrix createEmptyGraph()
    {
        return AdjMatrix{};
    }

    AdjMatrix createGraphWithOnlyOneNode()
    {
        AdjMatrix graph;
        graph.addNodes(1);
        return graph;
    }

    AdjMatrix createGraphWithChromaticNumber3()
    {
        AdjMatrix graph;
        graph.addNodes(9);
        graph.setEdge({1, 2});
        graph.setEdge({1, 6});
        graph.setEdge({2, 6});
        graph.setEdge({3, 4});
        graph.setEdge({3, 7});
        graph.setEdge({4, 7});
        graph.setEdge({5, 8});
        graph.setEdge({5, 9});
        graph.setEdge({6, 8});
        graph.setEdge({7, 9});
        graph.setEdge({8, 9});

        return graph;
    }
};

TEST_F(GreedyColoringTests, greedyColoringOnGraphWithChromaticNumber3Returns3)
{
    auto sampleGraph = createGraphWithChromaticNumber3();
    sut->operator()(sampleGraph);
    auto& [colorId, _] = *result;
    EXPECT_EQ(colorId, 3);
}

TEST_F(GreedyColoringTests, greedyColoringOnEmptyGraphReturnsMaxColor)
{
    auto sampleGraph = createEmptyGraph();
    sut->operator()(sampleGraph);
    auto& [colorId, _] = *result;
    EXPECT_EQ(colorId, std::numeric_limits<ColorId>::max());
}

TEST_F(GreedyColoringTests, greedyColoringOnGraphWithOneNodeReturnsMinimalColor)
{
    auto sampleGraph = createGraphWithOnlyOneNode();
    sut->operator()(sampleGraph);
    auto& [colorId, _] = *result;
    EXPECT_EQ(colorId, std::numeric_limits<ColorId>::min());
}
} // namespace Graphs

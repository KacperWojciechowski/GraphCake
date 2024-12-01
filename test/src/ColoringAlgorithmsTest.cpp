#include <gmock/gmock.h>
#include <Graphs/AdjList.hpp>
#include <Graphs/AdjMatrix.hpp>
#include <Graphs/Algorithm.hpp>
#include <Graphs/ColoringAlgorithms.hpp>
#include <gtest/gtest.h>

using namespace testing;
using namespace Graphs::Algorithm;

namespace Graphs
{
template <typename GraphType>
struct ColoringTests : public testing::Test
{
public:
    virtual GraphType createEmptyGraph()
    {
        return GraphType{};
    }

    virtual GraphType createGraphWithOnlyOneNode()
    {
        GraphType graph = {};
        graph.addNodes(1);
        return graph;
    }

    virtual GraphType createGraphWithChromaticNumber3()
    {
        GraphType graph = {};
        graph.addNodes(10);
        graph.setEdge({1, 2});

        graph.setEdge({2, 1});

        graph.setEdge({3, 4});

        graph.setEdge({4, 3});

        graph.setEdge({5, 6});
        graph.setEdge({5, 7});

        graph.setEdge({6, 5});
        graph.setEdge({6, 7});

        graph.setEdge({7, 5});
        graph.setEdge({7, 6});

        graph.setEdge({8, 9});
        graph.setEdge({8, 10});

        graph.setEdge({9, 8});
        graph.setEdge({9, 10});

        graph.setEdge({10, 8});
        graph.setEdge({10, 9});

        return graph;
    }

    virtual GraphType createGraphWithChromaticNumber4()
    {
        GraphType graph = {};
        graph.addNodes(5);
        graph.setEdge({2, 3});
        graph.setEdge({2, 4});
        graph.setEdge({2, 5});
        graph.setEdge({3, 2});
        graph.setEdge({3, 4});
        graph.setEdge({3, 5});
        graph.setEdge({4, 2});
        graph.setEdge({4, 3});
        graph.setEdge({4, 5});
        graph.setEdge({5, 2});
        graph.setEdge({5, 3});
        graph.setEdge({5, 4});

        return graph;
    }

    GraphType createGraphWithChromaticNumber5()
    {
        GraphType graph = {};
        graph.addNodes(8);
        graph.setEdge({1, 4});
        graph.setEdge({1, 5});
        graph.setEdge({1, 6});
        graph.setEdge({1, 7});
        graph.setEdge({1, 8});

        graph.setEdge({2, 4});
        graph.setEdge({2, 5});
        graph.setEdge({2, 6});
        graph.setEdge({2, 7});
        graph.setEdge({2, 8});

        graph.setEdge({3, 4});
        graph.setEdge({3, 5});
        graph.setEdge({3, 6});
        graph.setEdge({3, 7});
        graph.setEdge({3, 8});

        graph.setEdge({4, 1});
        graph.setEdge({4, 2});
        graph.setEdge({4, 3});
        graph.setEdge({4, 5});
        graph.setEdge({4, 6});
        graph.setEdge({4, 7});
        graph.setEdge({4, 8});

        graph.setEdge({5, 1});
        graph.setEdge({5, 2});
        graph.setEdge({5, 3});
        graph.setEdge({5, 4});
        graph.setEdge({5, 6});
        graph.setEdge({5, 7});
        graph.setEdge({5, 8});

        graph.setEdge({6, 1});
        graph.setEdge({6, 2});
        graph.setEdge({6, 3});
        graph.setEdge({6, 4});
        graph.setEdge({6, 5});
        graph.setEdge({6, 7});
        graph.setEdge({6, 8});

        graph.setEdge({7, 1});
        graph.setEdge({7, 2});
        graph.setEdge({7, 3});
        graph.setEdge({7, 4});
        graph.setEdge({7, 5});
        graph.setEdge({7, 6});
        graph.setEdge({7, 8});

        graph.setEdge({8, 1});
        graph.setEdge({8, 2});
        graph.setEdge({8, 3});
        graph.setEdge({8, 4});
        graph.setEdge({8, 5});
        graph.setEdge({8, 6});
        graph.setEdge({8, 7});

        return graph;
    }

    GraphType createGraphWithChromaticNumber6()
    {
        GraphType graph = {};
        graph.addNodes(6);
        graph.setEdge({1, 2});
        graph.setEdge({1, 3});
        graph.setEdge({1, 4});
        graph.setEdge({1, 5});
        graph.setEdge({1, 6});
        graph.setEdge({2, 1});
        graph.setEdge({2, 3});
        graph.setEdge({2, 4});
        graph.setEdge({2, 5});
        graph.setEdge({2, 6});
        graph.setEdge({3, 1});
        graph.setEdge({3, 2});
        graph.setEdge({3, 4});
        graph.setEdge({3, 5});
        graph.setEdge({3, 6});
        graph.setEdge({4, 1});
        graph.setEdge({4, 2});
        graph.setEdge({4, 3});
        graph.setEdge({4, 5});
        graph.setEdge({4, 6});
        graph.setEdge({5, 1});
        graph.setEdge({5, 2});
        graph.setEdge({5, 3});
        graph.setEdge({5, 4});
        graph.setEdge({5, 6});
        graph.setEdge({6, 1});
        graph.setEdge({6, 2});
        graph.setEdge({6, 3});
        graph.setEdge({6, 4});
        graph.setEdge({6, 5});
        return graph;
    }
};

template <typename GraphType>
struct GreedyColoringTests : public ColoringTests<GraphType>
{
    void SetUp()
    {
        result = std::make_shared<ColoringResult>();
        sut = std::make_unique<GreedyColoring<notVerbose>>(result);
    }

    using ColoringTests<GraphType>::createEmptyGraph;
    using ColoringTests<GraphType>::createGraphWithOnlyOneNode;
    using ColoringTests<GraphType>::createGraphWithChromaticNumber3;
    using ColoringTests<GraphType>::createGraphWithChromaticNumber4;
    using ColoringTests<GraphType>::createGraphWithChromaticNumber5;
    using ColoringTests<GraphType>::createGraphWithChromaticNumber6;

    std::shared_ptr<ColoringResult> result;
    std::unique_ptr<GreedyColoring<notVerbose>> sut;
};

using GraphTypes = ::testing::Types<AdjList, AdjMatrix>;

TYPED_TEST_SUITE(GreedyColoringTests, GraphTypes);

TYPED_TEST(GreedyColoringTests, greedyColoringOnGraphWithChromaticNumber3Returns3)
{
    auto sampleGraph = this->createGraphWithChromaticNumber3();
    this->sut->operator()(sampleGraph);
    auto& [colorId, _] = *this->result;
    EXPECT_EQ(colorId, 3);
}

TYPED_TEST(GreedyColoringTests, greedyColoringOnEmptyGraphReturnsMaxColor)
{
    auto sampleGraph = this->createEmptyGraph();
    this->sut->operator()(sampleGraph);
    auto& [colorId, _] = *this->result;
    EXPECT_EQ(colorId, std::numeric_limits<ColorId>::max());
}

TYPED_TEST(GreedyColoringTests, greedyColoringOnGraphWithOneNodeReturnsMinimalColor)
{
    auto sampleGraph = this->createGraphWithOnlyOneNode();
    this->sut->operator()(sampleGraph);
    auto& [colorId, _] = *this->result;
    EXPECT_EQ(colorId, std::numeric_limits<ColorId>::min());
}

TYPED_TEST(GreedyColoringTests, greedyColoringOnGraphWithChromaticNumber4Returns4)
{
    auto sampleGraph = this->createGraphWithChromaticNumber4();
    this->sut->operator()(sampleGraph);
    auto& [colorId, _] = *this->result;
    EXPECT_EQ(colorId, 4);
}

TYPED_TEST(GreedyColoringTests, greedyColoringOnGraphWithChromaticNumber5Returns5)
{
    auto sampleGraph = this->createGraphWithChromaticNumber5();
    this->sut->operator()(sampleGraph);
    auto& [colorId, _] = *this->result;
    EXPECT_EQ(colorId, 5);
}

TYPED_TEST(GreedyColoringTests, greedyColoringOnGraphWithChromaticNumber6Returns6)
{
    auto sampleGraph = this->createGraphWithChromaticNumber6();
    this->sut->operator()(sampleGraph);
    auto& [colorId, _] = *this->result;
    EXPECT_EQ(colorId, 6);
}
} // namespace Graphs

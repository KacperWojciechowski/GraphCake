#include <gmock/gmock.h>
#include <GraphRepresentationHelpers.hpp>
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
        graph.setEdges({
            {.source = 1,  .destination = 2},
            {.source = 3,  .destination = 4},
            {.source = 5,  .destination = 6},
            {.source = 5,  .destination = 7},
            {.source = 6,  .destination = 7},
            {.source = 8,  .destination = 9},
            {.source = 8, .destination = 10},
            {.source = 9, .destination = 10}
        });

        return graph;
    }

    virtual GraphType createGraphWithChromaticNumber4()
    {
        GraphType graph = {};
        graph.addNodes(5);
        graph.setEdges({
            {.source = 2, .destination = 3},
            {.source = 2, .destination = 4},
            {.source = 2, .destination = 5},
            {.source = 3, .destination = 4},
            {.source = 3, .destination = 5},
            {.source = 4, .destination = 5}
        });

        return graph;
    }

    GraphType createGraphWithChromaticNumber5()
    {
        GraphType graph = {};
        graph.addNodes(8);
        graph.setEdges({
            {.source = 1, .destination = 4},
            {.source = 1, .destination = 5},
            {.source = 1, .destination = 6},
            {.source = 1, .destination = 7},
            {.source = 1, .destination = 8},
            {.source = 2, .destination = 4},
            {.source = 2, .destination = 5},
            {.source = 2, .destination = 6},
            {.source = 2, .destination = 7},
            {.source = 2, .destination = 8},
            {.source = 3, .destination = 4},
            {.source = 3, .destination = 5},
            {.source = 3, .destination = 6},
            {.source = 3, .destination = 7},
            {.source = 3, .destination = 8},
            {.source = 4, .destination = 5},
            {.source = 4, .destination = 6},
            {.source = 4, .destination = 7},
            {.source = 4, .destination = 8},
            {.source = 5, .destination = 6},
            {.source = 5, .destination = 7},
            {.source = 5, .destination = 8},
            {.source = 6, .destination = 7},
            {.source = 6, .destination = 8},
            {.source = 7, .destination = 8}
        });

        return graph;
    }

    GraphType createGraphWithChromaticNumber6()
    {
        GraphType graph = {};
        graph.addNodes(6);
        graph.setEdges({
            {.source = 1, .destination = 2},
            {.source = 1, .destination = 3},
            {.source = 1, .destination = 4},
            {.source = 1, .destination = 5},
            {.source = 1, .destination = 6},
            {.source = 2, .destination = 3},
            {.source = 2, .destination = 4},
            {.source = 2, .destination = 5},
            {.source = 2, .destination = 6},
            {.source = 3, .destination = 4},
            {.source = 3, .destination = 5},
            {.source = 3, .destination = 6},
            {.source = 4, .destination = 5},
            {.source = 4, .destination = 6},
            {.source = 5, .destination = 6}
        });

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

TYPED_TEST_SUITE(GreedyColoringTests, GraphTypes);

TYPED_TEST(GreedyColoringTests, greedyColoringOnGraphWithChromaticNumber3Returns3)
{
    auto sampleGraph = this->createGraphWithChromaticNumber3();
    this->sut->operator()(sampleGraph);
    auto& [coloringValue, _] = *this->result;
    EXPECT_EQ(coloringValue, 3);
}

TYPED_TEST(GreedyColoringTests, greedyColoringOnEmptyGraphReturnsMinColor)
{
    auto sampleGraph = this->createEmptyGraph();
    this->sut->operator()(sampleGraph);
    auto& [coloringValue, _] = *this->result;
    EXPECT_EQ(coloringValue, std::numeric_limits<ColorId>::min());
}

TYPED_TEST(GreedyColoringTests, greedyColoringOnGraphWithOneNodeReturnsMinimalColor)
{
    auto sampleGraph = this->createGraphWithOnlyOneNode();
    this->sut->operator()(sampleGraph);
    auto& [coloringValue, _] = *this->result;
    EXPECT_EQ(coloringValue, std::numeric_limits<ColorId>::min());
}

TYPED_TEST(GreedyColoringTests, greedyColoringOnGraphWithChromaticNumber4Returns4)
{
    auto sampleGraph = this->createGraphWithChromaticNumber4();
    this->sut->operator()(sampleGraph);
    auto& [coloringValue, _] = *this->result;
    EXPECT_EQ(coloringValue, 4);
}

TYPED_TEST(GreedyColoringTests, greedyColoringOnGraphWithChromaticNumber5Returns5)
{
    auto sampleGraph = this->createGraphWithChromaticNumber5();
    this->sut->operator()(sampleGraph);
    auto& [coloringValue, _] = *this->result;
    EXPECT_EQ(coloringValue, 5);
}

TYPED_TEST(GreedyColoringTests, greedyColoringOnGraphWithChromaticNumber6Returns6)
{
    auto sampleGraph = this->createGraphWithChromaticNumber6();
    this->sut->operator()(sampleGraph);
    auto& [coloringValue, _] = *this->result;
    EXPECT_EQ(coloringValue, 6);
}
} // namespace Graphs

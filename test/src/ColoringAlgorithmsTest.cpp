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

    /*    1: 2 6
    2: 1 6
    3: 4 7
    4: 3 7
    5: 8 9
    6: 1 2 8
    7: 3 4 9
    8: 5 6 9
    9: 5 7 8*/

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

TEST_F(GreedyColoringTests, test)
{
    auto sampleGraph = createGraphWithChromaticNumber3();
    (*sut)(sampleGraph);
    EXPECT_EQ(result->)
}
} // namespace Graphs

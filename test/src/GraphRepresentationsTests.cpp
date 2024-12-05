#include <gmock/gmock.h>
#include <Graphs/AdjList.hpp>
#include <Graphs/AdjMatrix.hpp>
#include <Graphs/Graph.hpp>
#include <gtest/gtest.h>
#include <string>

using namespace testing;

namespace
{
constexpr uint32_t oneNode = 1;
constexpr uint32_t threeNodes = 3;
constexpr uint32_t fourNodes = 4;

constexpr Graphs::NodeId firstNodeId = 1;
constexpr Graphs::NodeId secondNodeId = 2;
constexpr Graphs::NodeId thirdNodeId = 3;
} // namespace

namespace Graphs
{
template <typename GraphType>
class GraphRepresentationsTests : public testing::Test
{
protected:
    GraphType sut;
};

using GraphTypes = ::testing::Types<AdjList, AdjMatrix>;

TYPED_TEST_SUITE(GraphRepresentationsTests, GraphTypes);

TYPED_TEST(GraphRepresentationsTests, constructorCreatesAnEmptyGraph)
{
    EXPECT_EQ(this->sut.nodesAmount(), 0);
}

TYPED_TEST(GraphRepresentationsTests, addingNodesIncreasesNodesAmount)
{
    this->sut.addNodes(oneNode);
    EXPECT_EQ(this->sut.nodesAmount(), 1);
    this->sut.addNodes(threeNodes);
    EXPECT_EQ(this->sut.nodesAmount(), 4);
}

TYPED_TEST(GraphRepresentationsTests, removingNodesDecreasesNodesAmount)
{
    this->sut.addNodes(fourNodes);
    this->sut.removeNode(thirdNodeId);
    EXPECT_EQ(this->sut.nodesAmount(), 3);
    this->sut.removeNode(firstNodeId);
    EXPECT_EQ(this->sut.nodesAmount(), 2);
}

TYPED_TEST(GraphRepresentationsTests, removingNodeWhichHasBeenAlreadyRemovedDoesNotThrowAndDoesNotRemoveAnyNodes)
{
    this->sut.addNodes(fourNodes);
    this->sut.removeNode(thirdNodeId);
    EXPECT_EQ(this->sut.nodesAmount(), 3);
    EXPECT_NO_THROW(this->sut.removeNode(thirdNodeId));
    EXPECT_EQ(this->sut.nodesAmount(), 3);
}

TYPED_TEST(GraphRepresentationsTests, removingNodeRemovesAllEdgesConnectedToIt)
{
    this->sut.addNodes(fourNodes);
    this->sut.setEdge({firstNodeId, thirdNodeId});
    this->sut.setEdge({thirdNodeId, firstNodeId});
    this->sut.removeNode(thirdNodeId);
    EXPECT_EQ(this->sut.nodeDegree(firstNodeId), 0);
}

TYPED_TEST(GraphRepresentationsTests, addingEdgeIncreasesNodeDegree)
{
    this->sut.addNodes(fourNodes);
    EXPECT_EQ(this->sut.nodeDegree(firstNodeId), 0);
    this->sut.setEdge({firstNodeId, thirdNodeId});
    EXPECT_EQ(this->sut.nodeDegree(firstNodeId), 1);
}

TYPED_TEST(GraphRepresentationsTests, removingEdgeDecreasesNodeDegree)
{
    this->sut.addNodes(fourNodes);
    this->sut.setEdge({firstNodeId, thirdNodeId});
    this->sut.setEdge({firstNodeId, secondNodeId});
    EXPECT_EQ(this->sut.nodeDegree(firstNodeId), 2);
    this->sut.removeEdge({firstNodeId, thirdNodeId});
    EXPECT_EQ(this->sut.nodeDegree(firstNodeId), 1);
}

TYPED_TEST(GraphRepresentationsTests, removingEdgeWhichHasBeenAlreadyRemovedDoesNotThrowAndDoesNotRemoveAnyEdges)
{
    this->sut.addNodes(fourNodes);
    this->sut.setEdge({firstNodeId, thirdNodeId});
    this->sut.setEdge({firstNodeId, secondNodeId});
    EXPECT_EQ(this->sut.nodeDegree(firstNodeId), 2);
    this->sut.removeEdge({firstNodeId, thirdNodeId});
    EXPECT_EQ(this->sut.nodeDegree(firstNodeId), 1);
    EXPECT_NO_THROW(this->sut.removeEdge({firstNodeId, thirdNodeId}));
    EXPECT_EQ(this->sut.nodeDegree(firstNodeId), 1);
}

TYPED_TEST(GraphRepresentationsTests, getNeighborsOfReturnsCorrectNeighbors)
{
    this->sut.addNodes(fourNodes);
    this->sut.setEdge({firstNodeId, thirdNodeId});
    this->sut.setEdge({firstNodeId, secondNodeId});
    EXPECT_THAT(this->sut.getNeighborsOf(firstNodeId), ElementsAre(secondNodeId, thirdNodeId));
}

TYPED_TEST(GraphRepresentationsTests, findEdgeReturnsCorrectEdgeInfo)
{
    this->sut.addNodes(fourNodes);
    this->sut.setEdge({firstNodeId, thirdNodeId, 1});
    EXPECT_EQ(this->sut.findEdge({firstNodeId, thirdNodeId}).source, firstNodeId);
    EXPECT_EQ(this->sut.findEdge({firstNodeId, thirdNodeId}).destination, thirdNodeId);
    EXPECT_EQ(this->sut.findEdge({firstNodeId, thirdNodeId}).weight, 1);
}

TYPED_TEST(GraphRepresentationsTests, findEdgeReturnsEmptyWeightIfEdgeDoesNotExist)
{
    this->sut.addNodes(fourNodes);
    this->sut.setEdge({firstNodeId, thirdNodeId});
    EXPECT_EQ(this->sut.findEdge({firstNodeId, secondNodeId}).weight, std::nullopt);
}

TYPED_TEST(GraphRepresentationsTests, setEdgeAppliesDefaultWeightWhenNoWeightIsGiven)
{
    this->sut.addNodes(fourNodes);
    this->sut.setEdge({firstNodeId, thirdNodeId, 1});
    EXPECT_EQ(this->sut.findEdge({firstNodeId, thirdNodeId}).weight, 1);
}

TYPED_TEST(GraphRepresentationsTests, setEdgeUpdatesWeightWhenEdgeAlreadyExists)
{
    this->sut.addNodes(fourNodes);
    this->sut.setEdge({firstNodeId, thirdNodeId, 5});
    EXPECT_EQ(this->sut.findEdge({firstNodeId, thirdNodeId}).weight, 5);
    this->sut.setEdge({firstNodeId, thirdNodeId, 10});
    EXPECT_EQ(this->sut.findEdge({firstNodeId, thirdNodeId}).weight, 10);
}

TYPED_TEST(GraphRepresentationsTests, canSetMultipleEdgesAtOnce)
{
    this->sut.addNodes(fourNodes);
    this->sut.setEdges({
        {firstNodeId,  thirdNodeId},
        {firstNodeId, secondNodeId}
    });
    EXPECT_EQ(this->sut.findEdge({firstNodeId, thirdNodeId}).weight, 1);
    EXPECT_EQ(this->sut.findEdge({firstNodeId, secondNodeId}).weight, 1);
}

TYPED_TEST(GraphRepresentationsTests, canSetMultipleEdgesAtOnceWithDefaultWeight)
{
    this->sut.addNodes(fourNodes);
    this->sut.setEdges({
        {firstNodeId,  thirdNodeId},
        {firstNodeId, secondNodeId}
    });
    EXPECT_EQ(this->sut.findEdge({firstNodeId, thirdNodeId}).weight, 1);
    EXPECT_EQ(this->sut.findEdge({firstNodeId, secondNodeId}).weight, 1);
}

TYPED_TEST(GraphRepresentationsTests, canUpdateMultipleEdges)
{
    this->sut.addNodes(fourNodes);
    this->sut.setEdges({
        {firstNodeId,  thirdNodeId,  5},
        {firstNodeId, secondNodeId, 10}
    });
    EXPECT_EQ(this->sut.findEdge({firstNodeId, thirdNodeId}).weight, 5);
    EXPECT_EQ(this->sut.findEdge({firstNodeId, secondNodeId}).weight, 10);
    this->sut.setEdges({
        {firstNodeId,  thirdNodeId, 10},
        {firstNodeId, secondNodeId,  5}
    });
    EXPECT_EQ(this->sut.findEdge({firstNodeId, thirdNodeId}).weight, 10);
    EXPECT_EQ(this->sut.findEdge({firstNodeId, secondNodeId}).weight, 5);
}

TYPED_TEST(GraphRepresentationsTests, settingEdgeBetweenNonexistantNodesDoesNotDoAnything)
{
    this->sut.addNodes(oneNode);
    EXPECT_NO_THROW(this->sut.setEdge({firstNodeId, thirdNodeId}));
    EXPECT_EQ(this->sut.nodeDegree(firstNodeId), 0);
}

TYPED_TEST(GraphRepresentationsTests, fetchingInformationForNonexistantEdgeDoesNotThrowAndReturnsEmptyWeight)
{
    this->sut.addNodes(oneNode);
    EXPECT_NO_THROW(this->sut.findEdge({firstNodeId, thirdNodeId}));
    EXPECT_EQ(this->sut.findEdge({firstNodeId, thirdNodeId}).weight, std::nullopt);
    EXPECT_EQ(this->sut.findEdge({firstNodeId, thirdNodeId}).source, firstNodeId);
    EXPECT_EQ(this->sut.findEdge({firstNodeId, thirdNodeId}).destination, thirdNodeId);
}

TYPED_TEST(GraphRepresentationsTests, nodesAfterRemovedNoteAreCorrectlyIdentifiedAfterRemoval)
{
    this->sut.addNodes(threeNodes);
    this->sut.setEdge({firstNodeId, secondNodeId});
    this->sut.setEdge({secondNodeId, thirdNodeId});
    this->sut.setEdge({thirdNodeId, firstNodeId});
    this->sut.removeNode(secondNodeId);
    EXPECT_EQ(this->sut.nodesAmount(), 2);
    auto searchedEdge = this->sut.findEdge({thirdNodeId, firstNodeId});
    EXPECT_TRUE(searchedEdge.weight.has_value());
    EXPECT_EQ(searchedEdge.weight.value(), 1);
}

TYPED_TEST(GraphRepresentationsTests, gettingNodesListOfGraphWithOnlyOneNodeHasOnlyOneElement)
{
    this->sut.addNodes(oneNode);
    auto nodes = this->sut.getNodeIds();
    EXPECT_THAT(nodes, ElementsAre(firstNodeId));
}

TYPED_TEST(GraphRepresentationsTests, spaceshipOperatorCorrectlyComparesGraphsBasedOnNodesCount)
{
    auto smallerGraph = this->sut;
    auto largerGraph = this->sut;

    this->sut.addNodes(oneNode);
    largerGraph.addNodes(fourNodes);

    EXPECT_THAT(this->sut > smallerGraph, true);
    EXPECT_THAT(this->sut == this->sut, true);
    EXPECT_THAT(this->sut < largerGraph, true);
    EXPECT_THAT(smallerGraph > largerGraph, false);
}

} // namespace Graphs

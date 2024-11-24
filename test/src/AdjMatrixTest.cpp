#include <gmock/gmock.h>
#include <Graphs/AdjMatrix.hpp>
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
class AdjMatrixTest : public testing::Test
{
protected:
    AdjMatrix sut;
};

TEST_F(AdjMatrixTest, constructorCreatesAnEmptyGraph)
{
    EXPECT_EQ(sut.nodesAmount(), 0);
}

TEST_F(AdjMatrixTest, addingNodesIncreasesNodesAmount)
{
    sut.addNodes(oneNode);
    EXPECT_EQ(sut.nodesAmount(), 1);
    sut.addNodes(threeNodes);
    EXPECT_EQ(sut.nodesAmount(), 4);
}

TEST_F(AdjMatrixTest, removingNodesDecreasesNodesAmount)
{
    sut.addNodes(fourNodes);
    sut.removeNode(thirdNodeId);
    EXPECT_EQ(sut.nodesAmount(), 3);
    sut.removeNode(firstNodeId);
    EXPECT_EQ(sut.nodesAmount(), 2);
}

TEST_F(AdjMatrixTest, removingNodeWhichHasBeenAlreadyRemovedDoesNotThrowAndDoesNotRemoveAnyNodes)
{
    sut.addNodes(fourNodes);
    sut.removeNode(thirdNodeId);
    EXPECT_EQ(sut.nodesAmount(), 3);
    EXPECT_NO_THROW(sut.removeNode(thirdNodeId));
    EXPECT_EQ(sut.nodesAmount(), 3);
}

TEST_F(AdjMatrixTest, removingNodeRemovesAllEdgesConnectedToIt)
{
    sut.addNodes(fourNodes);
    sut.setEdge({firstNodeId, thirdNodeId});
    sut.setEdge({thirdNodeId, firstNodeId});
    sut.removeNode(thirdNodeId);
    EXPECT_EQ(sut.nodeDegree(firstNodeId), 0);
}

TEST_F(AdjMatrixTest, addingEdgeIncreasesNodeDegree)
{
    sut.addNodes(fourNodes);
    EXPECT_EQ(sut.nodeDegree(firstNodeId), 0);
    sut.setEdge({firstNodeId, thirdNodeId});
    EXPECT_EQ(sut.nodeDegree(firstNodeId), 1);
}

TEST_F(AdjMatrixTest, removingEdgeDecreasesNodeDegree)
{
    sut.addNodes(fourNodes);
    sut.setEdge({firstNodeId, thirdNodeId});
    sut.setEdge({firstNodeId, secondNodeId});
    EXPECT_EQ(sut.nodeDegree(firstNodeId), 2);
    sut.removeEdge({firstNodeId, thirdNodeId});
    EXPECT_EQ(sut.nodeDegree(firstNodeId), 1);
}

TEST_F(AdjMatrixTest, removingEdgeWhichHasBeenAlreadyRemovedDoesNotThrowAndDoesNotRemoveAnyEdges)
{
    sut.addNodes(fourNodes);
    sut.setEdge({firstNodeId, thirdNodeId});
    sut.setEdge({firstNodeId, secondNodeId});
    EXPECT_EQ(sut.nodeDegree(firstNodeId), 2);
    sut.removeEdge({firstNodeId, thirdNodeId});
    EXPECT_EQ(sut.nodeDegree(firstNodeId), 1);
    EXPECT_NO_THROW(sut.removeEdge({firstNodeId, thirdNodeId}));
    EXPECT_EQ(sut.nodeDegree(firstNodeId), 1);
}

TEST_F(AdjMatrixTest, getNeighborsOfReturnsCorrectNeighbors)
{
    sut.addNodes(fourNodes);
    sut.setEdge({firstNodeId, thirdNodeId});
    sut.setEdge({firstNodeId, secondNodeId});
    EXPECT_THAT(sut.getNeighborsOf(firstNodeId), ElementsAre(secondNodeId, thirdNodeId));
}

TEST_F(AdjMatrixTest, findEdgeReturnsCorrectEdgeInfo)
{
    sut.addNodes(fourNodes);
    sut.setEdge({firstNodeId, thirdNodeId, 5});
    EXPECT_EQ(sut.findEdge({firstNodeId, thirdNodeId}).source, firstNodeId);
    EXPECT_EQ(sut.findEdge({firstNodeId, thirdNodeId}).destination, thirdNodeId);
    EXPECT_EQ(sut.findEdge({firstNodeId, thirdNodeId}).weight, 5);
}

TEST_F(AdjMatrixTest, findEdgeReturnsEmptyWeightIfEdgeDoesNotExist)
{
    sut.addNodes(fourNodes);
    sut.setEdge({firstNodeId, thirdNodeId, 5});
    EXPECT_EQ(sut.findEdge({firstNodeId, secondNodeId}).weight, std::nullopt);
}

TEST_F(AdjMatrixTest, setEdgeAppliesDefaultWeightWhenNoWeightIsGiven)
{
    sut.addNodes(fourNodes);
    sut.setEdge({firstNodeId, thirdNodeId});
    EXPECT_EQ(sut.findEdge({firstNodeId, thirdNodeId}).weight, 1);
}

TEST_F(AdjMatrixTest, setEdgeUpdatesWeightWhenEdgeAlreadyExists)
{
    sut.addNodes(fourNodes);
    sut.setEdge({firstNodeId, thirdNodeId, 5});
    EXPECT_EQ(sut.findEdge({firstNodeId, thirdNodeId}).weight, 5);
    sut.setEdge({firstNodeId, thirdNodeId, 10});
    EXPECT_EQ(sut.findEdge({firstNodeId, thirdNodeId}).weight, 10);
}

TEST_F(AdjMatrixTest, canSetMultipleEdgesAtOnce)
{
    sut.addNodes(fourNodes);
    sut.setEdges({
        {firstNodeId,  thirdNodeId,  5},
        {firstNodeId, secondNodeId, 10}
    });
    EXPECT_EQ(sut.findEdge({firstNodeId, thirdNodeId}).weight, 5);
    EXPECT_EQ(sut.findEdge({firstNodeId, secondNodeId}).weight, 10);
}

TEST_F(AdjMatrixTest, canSetMultipleEdgesAtOnceWithDefaultWeight)
{
    sut.addNodes(fourNodes);
    sut.setEdges({
        {firstNodeId,  thirdNodeId},
        {firstNodeId, secondNodeId}
    });
    EXPECT_EQ(sut.findEdge({firstNodeId, thirdNodeId}).weight, 1);
    EXPECT_EQ(sut.findEdge({firstNodeId, secondNodeId}).weight, 1);
}

TEST_F(AdjMatrixTest, canUpdateMultipleEdges)
{
    sut.addNodes(fourNodes);
    sut.setEdges({
        {firstNodeId,  thirdNodeId,  5},
        {firstNodeId, secondNodeId, 10}
    });
    EXPECT_EQ(sut.findEdge({firstNodeId, thirdNodeId}).weight, 5);
    EXPECT_EQ(sut.findEdge({firstNodeId, secondNodeId}).weight, 10);
    sut.setEdges({
        {firstNodeId,  thirdNodeId, 10},
        {firstNodeId, secondNodeId,  5}
    });
    EXPECT_EQ(sut.findEdge({firstNodeId, thirdNodeId}).weight, 10);
    EXPECT_EQ(sut.findEdge({firstNodeId, secondNodeId}).weight, 5);
}

TEST_F(AdjMatrixTest, settingEdgeBetweenNonexistantNodesDoesNotDoAnything)
{
    sut.addNodes(oneNode);
    EXPECT_NO_THROW(sut.setEdge({firstNodeId, thirdNodeId}));
    EXPECT_EQ(sut.nodeDegree(firstNodeId), 0);
}

TEST_F(AdjMatrixTest, fetchingInformationForNonexistantEdgeDoesNotThrowAndReturnsEmptyWeight)
{
    sut.addNodes(oneNode);
    EXPECT_NO_THROW(sut.findEdge({firstNodeId, thirdNodeId}));
    EXPECT_EQ(sut.findEdge({firstNodeId, thirdNodeId}).weight, std::nullopt);
    EXPECT_EQ(sut.findEdge({firstNodeId, thirdNodeId}).source, firstNodeId);
    EXPECT_EQ(sut.findEdge({firstNodeId, thirdNodeId}).destination, thirdNodeId);
}

TEST_F(AdjMatrixTest, nodesAfterRemovedNoteAreCorrectlyIdentifiedAfterRemoval)
{
    sut.addNodes(threeNodes);
    sut.setEdge({firstNodeId, secondNodeId, 1});
    sut.setEdge({secondNodeId, thirdNodeId, 2});
    sut.setEdge({thirdNodeId, firstNodeId, 3});
    sut.removeNode(secondNodeId);
    EXPECT_EQ(sut.nodesAmount(), 2);
    auto searchedEdge = sut.findEdge({thirdNodeId, firstNodeId});
    EXPECT_TRUE(searchedEdge.weight.has_value());
    EXPECT_EQ(searchedEdge.weight.value(), 3);
}

TEST_F(AdjMatrixTest, gettingNodesListOfGraphWithOnlyOneNodeHasOnlyOneElement)
{
    sut.addNodes(oneNode);
    auto nodes = sut.getNodeIds();
    EXPECT_THAT(nodes, ElementsAre(firstNodeId));
}
} // namespace Graphs

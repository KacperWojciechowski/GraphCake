#include <gmock/gmock.h>
#include <Graphs/AdjList.hpp>
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
class AdjListTest : public testing::Test
{
protected:
    AdjList sut;
};

TEST_F(AdjListTest, constructorCreatesAnEmptyGraph)
{
    EXPECT_EQ(sut.nodesAmount(), 0);
}

TEST_F(AdjListTest, addingNodesIncreasesNodesAmount)
{
    sut.addNodes(oneNode);
    EXPECT_EQ(sut.nodesAmount(), 1);
    sut.addNodes(threeNodes);
    EXPECT_EQ(sut.nodesAmount(), 4);
}

TEST_F(AdjListTest, removingNodesDecreasesNodesAmount)
{
    sut.addNodes(fourNodes);
    sut.removeNode(thirdNodeId);
    EXPECT_EQ(sut.nodesAmount(), 3);
    sut.removeNode(firstNodeId);
    EXPECT_EQ(sut.nodesAmount(), 2);
}

TEST_F(AdjListTest, removingNodeWhichHasBeenAlreadyRemovedDoesNotThrowAndDoesNotRemoveAnyNodes)
{
    sut.addNodes(fourNodes);
    sut.removeNode(thirdNodeId);
    EXPECT_EQ(sut.nodesAmount(), 3);
    EXPECT_NO_THROW(sut.removeNode(thirdNodeId));
    EXPECT_EQ(sut.nodesAmount(), 3);
}

TEST_F(AdjListTest, removingNodeRemovesAllEdgesConnectedToIt)
{
    sut.addNodes(fourNodes);
    sut.setEdge({firstNodeId, thirdNodeId});
    sut.setEdge({thirdNodeId, firstNodeId});
    sut.removeNode(thirdNodeId);
    EXPECT_EQ(sut.nodeDegree(firstNodeId), 0);
}

TEST_F(AdjListTest, addingEdgeIncreasesNodeDegree)
{
    sut.addNodes(fourNodes);
    EXPECT_EQ(sut.nodeDegree(firstNodeId), 0);
    sut.setEdge({firstNodeId, thirdNodeId});
    EXPECT_EQ(sut.nodeDegree(firstNodeId), 1);
}

TEST_F(AdjListTest, removingEdgeDecreasesNodeDegree)
{
    sut.addNodes(fourNodes);
    sut.setEdge({firstNodeId, thirdNodeId});
    sut.setEdge({firstNodeId, secondNodeId});
    EXPECT_EQ(sut.nodeDegree(firstNodeId), 2);
    sut.removeEdge({firstNodeId, thirdNodeId});
    EXPECT_EQ(sut.nodeDegree(firstNodeId), 1);
}

TEST_F(AdjListTest, removingEdgeWhichHasBeenAlreadyRemovedDoesNotThrowAndDoesNotRemoveAnyEdges)
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

TEST_F(AdjListTest, getNeighborsOfReturnsCorrectNeighbors)
{
    sut.addNodes(fourNodes);
    sut.setEdge({firstNodeId, thirdNodeId});
    sut.setEdge({firstNodeId, secondNodeId});
    EXPECT_THAT(sut.getNeighborsOf(firstNodeId), ElementsAre(secondNodeId, thirdNodeId));
}

TEST_F(AdjListTest, findEdgeReturnsCorrectEdgeInfo)
{
    sut.addNodes(fourNodes);
    sut.setEdge({firstNodeId, thirdNodeId, 1});
    EXPECT_EQ(sut.findEdge({firstNodeId, thirdNodeId}).source, firstNodeId);
    EXPECT_EQ(sut.findEdge({firstNodeId, thirdNodeId}).destination, thirdNodeId);
    EXPECT_EQ(sut.findEdge({firstNodeId, thirdNodeId}).weight, 1);
}

TEST_F(AdjListTest, findEdgeReturnsEmptyWeightIfEdgeDoesNotExist)
{
    sut.addNodes(fourNodes);
    sut.setEdge({firstNodeId, thirdNodeId});
    EXPECT_EQ(sut.findEdge({firstNodeId, secondNodeId}).weight, std::nullopt);
}

TEST_F(AdjListTest, setEdgeAppliesDefaultWeightWhenNoWeightIsGiven)
{
    sut.addNodes(fourNodes);
    sut.setEdge({firstNodeId, thirdNodeId, 1});
    EXPECT_EQ(sut.findEdge({firstNodeId, thirdNodeId}).weight, 1);
}

TEST_F(AdjListTest, setEdgeUpdatesWeightWhenEdgeAlreadyExists)
{
    sut.addNodes(fourNodes);
    sut.setEdge({firstNodeId, thirdNodeId, 5});
    EXPECT_EQ(sut.findEdge({firstNodeId, thirdNodeId}).weight, 5);
    sut.setEdge({firstNodeId, thirdNodeId, 10});
    EXPECT_EQ(sut.findEdge({firstNodeId, thirdNodeId}).weight, 10);
}

TEST_F(AdjListTest, canSetMultipleEdgesAtOnce)
{
    sut.addNodes(fourNodes);
    sut.setEdges({
        {firstNodeId,  thirdNodeId},
        {firstNodeId, secondNodeId}
    });
    EXPECT_EQ(sut.findEdge({firstNodeId, thirdNodeId}).weight, 1);
    EXPECT_EQ(sut.findEdge({firstNodeId, secondNodeId}).weight, 1);
}

TEST_F(AdjListTest, canSetMultipleEdgesAtOnceWithDefaultWeight)
{
    sut.addNodes(fourNodes);
    sut.setEdges({
        {firstNodeId,  thirdNodeId},
        {firstNodeId, secondNodeId}
    });
    EXPECT_EQ(sut.findEdge({firstNodeId, thirdNodeId}).weight, 1);
    EXPECT_EQ(sut.findEdge({firstNodeId, secondNodeId}).weight, 1);
}

TEST_F(AdjListTest, canUpdateMultipleEdges)
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

TEST_F(AdjListTest, settingEdgeBetweenNonexistantNodesDoesNotDoAnything)
{
    sut.addNodes(oneNode);
    EXPECT_NO_THROW(sut.setEdge({firstNodeId, thirdNodeId}));
    EXPECT_EQ(sut.nodeDegree(firstNodeId), 0);
}

TEST_F(AdjListTest, fetchingInformationForNonexistantEdgeDoesNotThrowAndReturnsEmptyWeight)
{
    sut.addNodes(oneNode);
    EXPECT_NO_THROW(sut.findEdge({firstNodeId, thirdNodeId}));
    EXPECT_EQ(sut.findEdge({firstNodeId, thirdNodeId}).weight, std::nullopt);
    EXPECT_EQ(sut.findEdge({firstNodeId, thirdNodeId}).source, firstNodeId);
    EXPECT_EQ(sut.findEdge({firstNodeId, thirdNodeId}).destination, thirdNodeId);
}

TEST_F(AdjListTest, nodesAfterRemovedNoteAreCorrectlyIdentifiedAfterRemoval)
{
    sut.addNodes(threeNodes);
    sut.setEdge({firstNodeId, secondNodeId});
    sut.setEdge({secondNodeId, thirdNodeId});
    sut.setEdge({thirdNodeId, firstNodeId});
    sut.removeNode(secondNodeId);
    EXPECT_EQ(sut.nodesAmount(), 2);
    auto searchedEdge = sut.findEdge({thirdNodeId, firstNodeId});
    EXPECT_TRUE(searchedEdge.weight.has_value());
    EXPECT_EQ(searchedEdge.weight.value(), 1);
}

} // namespace Graphs

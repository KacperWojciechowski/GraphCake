#include <gmock/gmock.h>
#include <Graphs/Deserializer.hpp>
#include <gtest/gtest.h>

using namespace testing;

namespace Graphs
{
template <typename T>
struct DeserializerTest : public testing::Test
{
    DeserializerTest() = default;

    Deserializer<T> sut = {};
};

using GraphTypes = ::testing::Types<AdjList, AdjMatrix>;

TYPED_TEST_SUITE(DeserializerTest, GraphTypes);

TYPED_TEST(DeserializerTest, canDeserializeLstFile)
{
    std::string fileContent = "1: 1 3\n2: 1\n3: 2 3\n";
    std::stringstream mockStream(fileContent);

    auto graph = this->sut.deserializeLstFile(mockStream);

    EXPECT_EQ(graph.nodesAmount(), 3);
    EXPECT_EQ(graph.findEdge({1, 1}).weight, 1);
    EXPECT_EQ(graph.findEdge({1, 2}).weight, std::nullopt);
    EXPECT_EQ(graph.findEdge({1, 3}).weight, 1);
    EXPECT_EQ(graph.findEdge({2, 1}).weight, 1);
    EXPECT_EQ(graph.findEdge({2, 2}).weight, std::nullopt);
    EXPECT_EQ(graph.findEdge({2, 3}).weight, std::nullopt);
    EXPECT_EQ(graph.findEdge({3, 1}).weight, std::nullopt);
    EXPECT_EQ(graph.findEdge({3, 2}).weight, 1);
    EXPECT_EQ(graph.findEdge({3, 3}).weight, 1);
}

TYPED_TEST(DeserializerTest, canDeserializeMatFile)
{
    std::string fileContent = "1 1 3\n2 1 0\n0 2 3\n";
    std::stringstream mockStream(fileContent);

    auto graph = this->sut.deserializeMatFile(mockStream);

    EXPECT_EQ(graph.nodesAmount(), 3);
    EXPECT_EQ(graph.findEdge({1, 1}).weight, 1);
    EXPECT_EQ(graph.findEdge({1, 2}).weight, 1);
    EXPECT_EQ(graph.findEdge({1, 3}).weight, 3);
    EXPECT_EQ(graph.findEdge({2, 1}).weight, 2);
    EXPECT_EQ(graph.findEdge({2, 2}).weight, 1);
    EXPECT_EQ(graph.findEdge({2, 3}).weight, std::nullopt);
    EXPECT_EQ(graph.findEdge({3, 1}).weight, std::nullopt);
    EXPECT_EQ(graph.findEdge({3, 2}).weight, 2);
    EXPECT_EQ(graph.findEdge({3, 3}).weight, 3);
}

TYPED_TEST(DeserializerTest, deserializingEmptyLstFileReturnsEmptyGraph)
{
    std::string fileContent = "";
    std::stringstream mockStream(fileContent);

    auto graph = this->sut.deserializeLstFile(mockStream);

    EXPECT_EQ(graph.nodesAmount(), 0);
}
} // namespace Graphs

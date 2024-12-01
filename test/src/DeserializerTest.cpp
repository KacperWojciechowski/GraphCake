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

    std::string makeSampleGraphMlFile()
    {
        return "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
               "<graphml xmlns=\"http://graphml.graphdrawing.org/xmlns\"\n"
               "   xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\"\n"
               "   xsi:schemaLocation=\"http://graphml.graphdrawing.org/xmlns\n"
               "   http://graphml.graphdrawing.org/xmlns/1.0/graphml.xsd\">\n"
               "   <graph id=\"Graph\" edgedefault=\"undirected\">\n"
               "       <node id=\"n0\"/>\n"
               "       <node id=\"n1\"/>\n"
               "       <node id=\"n2\"/>\n"
               "       <node id=\"n3\"/>\n"
               "       <node id=\"n4\"/>\n"
               "       <edge source=\"n0\" target=\"n1\"/>\n"
               "       <edge source=\"n1\" target=\"n0\"/>\n"
               "       <edge source=\"n2\" target=\"n3\"/>\n"
               "       <edge source=\"n3\" target=\"n2\"/>\n"
               "   </graph>\n"
               "</graphml>\n";
    }

    std::string makeSampleLstFile()
    {
        return "1: 1 3\n"
               "2: 1\n"
               "3: 2 3\n";
    }

    std::string makeSampleMatFile()
    {
        return "1 1 3\n"
               "2 1 0\n"
               "0 2 3\n";
    }

    std::string makeMalformedMatFile()
    {
        return "1 1 3\n"
               "2 1 0 2\n"
               "0 2 3\n";
    }

    Deserializer<T> sut = {};
};

using GraphTypes = ::testing::Types<AdjList, AdjMatrix>;

TYPED_TEST_SUITE(DeserializerTest, GraphTypes);

TYPED_TEST(DeserializerTest, canDeserializeLstFile)
{
    std::string fileContent = this->makeSampleLstFile();
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
    std::string fileContent = this->makeSampleMatFile();
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

TYPED_TEST(DeserializerTest, canDeserializeGraphMlFile)
{
    std::string fileContent = this->makeSampleGraphMlFile();
    std::stringstream mockStream(fileContent);

    auto graph = this->sut.deserializeGraphMlFile(mockStream);

    EXPECT_EQ(graph.nodesAmount(), 5);
    EXPECT_EQ(graph.findEdge({1, 2}).weight, 1);
    EXPECT_EQ(graph.findEdge({2, 1}).weight, 1);
    EXPECT_EQ(graph.findEdge({3, 4}).weight, 1);
    EXPECT_EQ(graph.findEdge({4, 3}).weight, 1);
}

TYPED_TEST(DeserializerTest, deserializingEmptyLstFileReturnsEmptyGraph)
{
    std::string fileContent = "";
    std::stringstream mockStream(fileContent);

    auto graph = this->sut.deserializeLstFile(mockStream);

    EXPECT_EQ(graph.nodesAmount(), 0);
}

TYPED_TEST(DeserializerTest, deserializingEmptyMatFileReturnsEmptyGraph)
{
    std::string fileContent = "";
    std::stringstream mockStream(fileContent);

    auto graph = this->sut.deserializeMatFile(mockStream);

    EXPECT_EQ(graph.nodesAmount(), 0);
}

TYPED_TEST(DeserializerTest, deserializingEmptyGraphMlFileReturnsEmptyGraph)
{
    std::string fileContent = "";
    std::stringstream mockStream(fileContent);

    auto graph = this->sut.deserializeGraphMlFile(mockStream);

    EXPECT_EQ(graph.nodesAmount(), 0);
}

TYPED_TEST(DeserializerTest, deserializingInvalidLstFileReturnsEmptyGraph)
{
    std::string fileContent = "invalid content";
    std::stringstream mockStream(fileContent);

    auto graph = this->sut.deserializeLstFile(mockStream);

    EXPECT_EQ(graph.nodesAmount(), 0);
}

TYPED_TEST(DeserializerTest, deserializingInvalidMatFileReturnsEmptyGraph)
{
    std::string fileContent = "invalid content";
    std::stringstream mockStream(fileContent);

    auto graph = this->sut.deserializeMatFile(mockStream);

    EXPECT_EQ(graph.nodesAmount(), 0);
}

TYPED_TEST(DeserializerTest, deserializingInvalidGraphMlFileReturnsEmptyGraph)
{
    std::string fileContent = "invalid content";
    std::stringstream mockStream(fileContent);

    auto graph = this->sut.deserializeGraphMlFile(mockStream);

    EXPECT_EQ(graph.nodesAmount(), 0);
}

TYPED_TEST(DeserializerTest, deserializingMalformedMatFileReturnsEmptyGraph)
{
    std::string fileContent = this->makeMalformedMatFile();
    std::stringstream mockStream(fileContent);

    auto graph = this->sut.deserializeMatFile(mockStream);

    EXPECT_EQ(graph.nodesAmount(), 0);
}

TYPED_TEST(DeserializerTest, deserializingLstFileWithNoValuesReturnsEmptyGraph)
{
    std::string fileContent = " ";
    std::stringstream mockStream(fileContent);

    auto graph = this->sut.deserializeLstFile(mockStream);

    EXPECT_EQ(graph.nodesAmount(), 0);
}

TYPED_TEST(DeserializerTest, deserializingMatFileWithNoValuesReturnsEmptyGraph)
{
    std::string fileContent = " ";
    std::stringstream mockStream(fileContent);

    auto graph = this->sut.deserializeMatFile(mockStream);

    EXPECT_EQ(graph.nodesAmount(), 0);
}

TYPED_TEST(DeserializerTest, deserializingGraphMlFileWithNoValuesReturnsEmptyGraph)
{
    std::string fileContent = " ";
    std::stringstream mockStream(fileContent);

    auto graph = this->sut.deserializeGraphMlFile(mockStream);

    EXPECT_EQ(graph.nodesAmount(), 0);
}
} // namespace Graphs

#include <gmock/gmock.h>
#include <Graphs/Deserializer.hpp>
#include <gtest/gtest.h>
#include <tuple>

using namespace testing;

namespace Graphs
{
template <typename GraphType>
struct DeserializerTest : public testing::Test
{
    DeserializerTest() = default;

    std::tuple<std::string, GraphType> makeSampleGraphMlFile()
    {
        auto streamContent = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
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
        GraphType graphRep = {};
        graphRep.addNodes(5);
        graphRep.setEdge({.source = 1, .destination = 2});
        graphRep.setEdge({.source = 2, .destination = 1});
        graphRep.setEdge({.source = 3, .destination = 4});
        graphRep.setEdge({.source = 4, .destination = 3});

        return std::tie(streamContent, graphRep);
    }

    std::tuple<std::string, GraphType> makeSampleLstFile()
    {
        auto streamContent = "1: 1 3\n"
                             "2: 1\n"
                             "3: 2 3\n";
        GraphType graphRep = {};
        graphRep.addNodes(3);
        graphRep.setEdge({.source = 1, .destination = 1});
        graphRep.setEdge({.source = 1, .destination = 3});
        graphRep.setEdge({.source = 2, .destination = 1});
        graphRep.setEdge({.source = 3, .destination = 2});
        graphRep.setEdge({.source = 3, .destination = 3});

        return std::tie(streamContent, graphRep);
    }

    std::tuple<std::string, GraphType> makeSampleMatFile()
    {
        auto streamContent = "1 1 3\n"
                             "2 1 0\n"
                             "0 2 3\n";

        GraphType graphRep = {};
        graphRep.addNodes(3);
        graphRep.setEdge({.source = 1, .destination = 1});
        graphRep.setEdge({.source = 1, .destination = 2});
        graphRep.setEdge({.source = 1, .destination = 3, .weight = 3});
        graphRep.setEdge({.source = 2, .destination = 1, .weight = 2});
        graphRep.setEdge({.source = 2, .destination = 2});
        graphRep.setEdge({.source = 3, .destination = 2, .weight = 2});
        graphRep.setEdge({.source = 3, .destination = 3, .weight = 3});

        return std::tie(streamContent, graphRep);
    }

    std::string makeMalformedMatFile()
    {
        return "1 1 3\n"
               "2 1 0 2\n"
               "0 2 3\n";
    }

    Deserializer<GraphType> sut = {};
};

using GraphTypes = ::testing::Types<AdjList, AdjMatrix>;

TYPED_TEST_SUITE(DeserializerTest, GraphTypes);

TYPED_TEST(DeserializerTest, canDeserializeLstFile)
{
    auto [fileContent, referenceGraph] = this->makeSampleLstFile();
    std::stringstream mockStream(fileContent);

    auto graph = this->sut.deserializeLstFile(mockStream);

    EXPECT_EQ(graph.operator<=>(referenceGraph), std::strong_ordering::equal);
}

TYPED_TEST(DeserializerTest, canDeserializeMatFile)
{
    auto [fileContent, referenceGraph] = this->makeSampleMatFile();
    std::stringstream mockStream(fileContent);

    auto graph = this->sut.deserializeMatFile(mockStream);

    EXPECT_EQ(graph, referenceGraph);
}

TYPED_TEST(DeserializerTest, canDeserializeGraphMlFile)
{
    auto [fileContent, referenceGraph] = this->makeSampleGraphMlFile();
    std::stringstream mockStream(fileContent);

    auto graph = this->sut.deserializeGraphMlFile(mockStream);

    EXPECT_EQ(graph, referenceGraph);
}

TYPED_TEST(DeserializerTest, deserializingEmptyLstFileReturnsEmptyGraph)
{
    std::string fileContent = "";
    std::stringstream mockStream(fileContent);

    auto graph = this->sut.deserializeLstFile(mockStream);

    EXPECT_EQ(graph, decltype(graph){});
}

TYPED_TEST(DeserializerTest, deserializingEmptyMatFileReturnsEmptyGraph)
{
    std::string fileContent = "";
    std::stringstream mockStream(fileContent);

    auto graph = this->sut.deserializeMatFile(mockStream);

    EXPECT_EQ(graph, decltype(graph){});
}

TYPED_TEST(DeserializerTest, deserializingEmptyGraphMlFileReturnsEmptyGraph)
{
    std::string fileContent = "";
    std::stringstream mockStream(fileContent);

    auto graph = this->sut.deserializeGraphMlFile(mockStream);

    EXPECT_EQ(graph, decltype(graph){});
}

TYPED_TEST(DeserializerTest, deserializingInvalidLstFileReturnsEmptyGraph)
{
    std::string fileContent = "invalid content";
    std::stringstream mockStream(fileContent);

    auto graph = this->sut.deserializeLstFile(mockStream);

    EXPECT_EQ(graph, decltype(graph){});
}

TYPED_TEST(DeserializerTest, deserializingInvalidMatFileReturnsEmptyGraph)
{
    std::string fileContent = "invalid content";
    std::stringstream mockStream(fileContent);

    auto graph = this->sut.deserializeMatFile(mockStream);

    EXPECT_EQ(graph, decltype(graph){});
}

TYPED_TEST(DeserializerTest, deserializingInvalidGraphMlFileReturnsEmptyGraph)
{
    std::string fileContent = "invalid content";
    std::stringstream mockStream(fileContent);

    auto graph = this->sut.deserializeGraphMlFile(mockStream);

    EXPECT_EQ(graph, decltype(graph){});
}

TYPED_TEST(DeserializerTest, deserializingMalformedMatFileReturnsEmptyGraph)
{
    std::string fileContent = this->makeMalformedMatFile();
    std::stringstream mockStream(fileContent);

    auto graph = this->sut.deserializeMatFile(mockStream);

    EXPECT_EQ(graph, decltype(graph){});
}

TYPED_TEST(DeserializerTest, deserializingLstFileWithNoValuesReturnsEmptyGraph)
{
    std::string fileContent = " ";
    std::stringstream mockStream(fileContent);

    auto graph = this->sut.deserializeLstFile(mockStream);

    EXPECT_EQ(graph, decltype(graph){});
}

TYPED_TEST(DeserializerTest, deserializingMatFileWithNoValuesReturnsEmptyGraph)
{
    std::string fileContent = " ";
    std::stringstream mockStream(fileContent);

    auto graph = this->sut.deserializeMatFile(mockStream);

    EXPECT_EQ(graph, decltype(graph){});
}

TYPED_TEST(DeserializerTest, deserializingGraphMlFileWithNoValuesReturnsEmptyGraph)
{
    std::string fileContent = " ";
    std::stringstream mockStream(fileContent);

    auto graph = this->sut.deserializeGraphMlFile(mockStream);

    EXPECT_EQ(graph, decltype(graph){});
}
} // namespace Graphs

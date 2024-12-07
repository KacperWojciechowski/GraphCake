#include <gmock/gmock.h>
#include <GraphRepresentationHelpers.hpp>
#include <Graphs/Deserializer.hpp>
#include <gtest/gtest.h>
#include <SerializationHelpers.hpp>

using namespace testing;

namespace Graphs
{
template <typename GraphType>
struct DeserializerTest : public testing::Test
{
    DeserializerTest() = default;

    std::string makeMalformedMatFile()
    {
        return "1 1 3\n"
               "2 1 0 2\n"
               "0 2 3\n";
    }

    SerializationHelper<GraphType> helper;

    using sut = Deserializer<GraphType>;
};

TYPED_TEST_SUITE(DeserializerTest, GraphTypes);

TYPED_TEST(DeserializerTest, canDeserializeLstFile)
{
    auto [fileContent, referenceGraph] = this->helper.makeSampleLstFile();
    std::stringstream mockStream(fileContent);

    auto graph = DeserializerTest<TypeParam>::sut::deserializeLstFile(mockStream);

    EXPECT_EQ(graph.operator<=>(referenceGraph), std::strong_ordering::equal);
}

TYPED_TEST(DeserializerTest, canDeserializeMatFile)
{
    auto [fileContent, referenceGraph] = this->helper.makeSampleMatFile();
    std::stringstream mockStream(fileContent);

    auto graph = DeserializerTest<TypeParam>::sut::deserializeMatFile(mockStream);

    EXPECT_EQ(graph, referenceGraph);
}

TYPED_TEST(DeserializerTest, canDeserializeGraphMlFile)
{
    auto [fileContent, referenceGraph] = this->helper.makeSampleGraphMlFile();
    std::stringstream mockStream(fileContent);

    auto graph = DeserializerTest<TypeParam>::sut::deserializeGraphMlFile(mockStream);

    EXPECT_EQ(graph, referenceGraph);
}

TYPED_TEST(DeserializerTest, deserializingEmptyLstFileReturnsEmptyGraph)
{
    auto [fileContent, referenceGraph] = this->helper.makeSampleEmptyFile();
    std::stringstream mockStream(fileContent);

    auto graph = DeserializerTest<TypeParam>::sut::deserializeLstFile(mockStream);

    EXPECT_EQ(graph, referenceGraph);
}

TYPED_TEST(DeserializerTest, deserializingEmptyMatFileReturnsEmptyGraph)
{
    auto [fileContent, referenceGraph] = this->helper.makeSampleEmptyFile();
    std::stringstream mockStream(fileContent);

    auto graph = DeserializerTest<TypeParam>::sut::deserializeMatFile(mockStream);

    EXPECT_EQ(graph, referenceGraph);
}

TYPED_TEST(DeserializerTest, deserializingEmptyGraphMlFileReturnsEmptyGraph)
{
    auto [fileContent, referenceGraph] = this->helper.makeSampleEmptyFile();
    std::stringstream mockStream(fileContent);

    auto graph = DeserializerTest<TypeParam>::sut::deserializeGraphMlFile(mockStream);

    EXPECT_EQ(graph, referenceGraph);
}

TYPED_TEST(DeserializerTest, deserializingInvalidLstFileReturnsEmptyGraph)
{
    std::string fileContent = "invalid content";
    std::stringstream mockStream(fileContent);

    auto graph = DeserializerTest<TypeParam>::sut::deserializeLstFile(mockStream);

    EXPECT_EQ(graph, decltype(graph){});
}

TYPED_TEST(DeserializerTest, deserializingInvalidMatFileReturnsEmptyGraph)
{
    std::string fileContent = "invalid content";
    std::stringstream mockStream(fileContent);

    auto graph = DeserializerTest<TypeParam>::sut::deserializeMatFile(mockStream);

    EXPECT_EQ(graph, decltype(graph){});
}

TYPED_TEST(DeserializerTest, deserializingInvalidGraphMlFileReturnsEmptyGraph)
{
    std::string fileContent = "invalid content";
    std::stringstream mockStream(fileContent);

    auto graph = DeserializerTest<TypeParam>::sut::deserializeGraphMlFile(mockStream);

    EXPECT_EQ(graph, decltype(graph){});
}

TYPED_TEST(DeserializerTest, deserializingMalformedMatFileReturnsEmptyGraph)
{
    std::string fileContent = this->makeMalformedMatFile();
    std::stringstream mockStream(fileContent);

    auto graph = DeserializerTest<TypeParam>::sut::deserializeMatFile(mockStream);

    EXPECT_EQ(graph, decltype(graph){});
}

TYPED_TEST(DeserializerTest, deserializingLstFileWithNoValuesReturnsEmptyGraph)
{
    std::string fileContent = " ";
    std::stringstream mockStream(fileContent);

    auto graph = DeserializerTest<TypeParam>::sut::deserializeLstFile(mockStream);

    EXPECT_EQ(graph, decltype(graph){});
}

TYPED_TEST(DeserializerTest, deserializingMatFileWithNoValuesReturnsEmptyGraph)
{
    std::string fileContent = " ";
    std::stringstream mockStream(fileContent);

    auto graph = DeserializerTest<TypeParam>::sut::deserializeMatFile(mockStream);

    EXPECT_EQ(graph, decltype(graph){});
}

TYPED_TEST(DeserializerTest, deserializingGraphMlFileWithNoValuesReturnsEmptyGraph)
{
    std::string fileContent = " ";
    std::stringstream mockStream(fileContent);

    auto graph = DeserializerTest<TypeParam>::sut::deserializeGraphMlFile(mockStream);

    EXPECT_EQ(graph, decltype(graph){});
}

TYPED_TEST(DeserializerTest, deserializingGraphMlFileWithissmatchedDirectionalityReturnEmptyGraph)
{
    auto [fileContent, referenceGraph] = this->helper.makeSampleDirectionalityMissmatchGraphMlFile();
    std::stringstream mockStream(fileContent);

    auto graph = DeserializerTest<TypeParam>::sut::deserializeGraphMlFile(mockStream);

    EXPECT_EQ(graph, referenceGraph);
}
} // namespace Graphs

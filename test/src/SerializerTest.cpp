#include <GraphRepresentationHelpers.hpp>
#include <Graphs/Serializer.hpp>
#include <gtest/gtest.h>
#include <SerializationHelpers.hpp>

using namespace testing;

namespace Graphs
{
template <typename GraphType>
struct SerializerTest : testing::Test
{
    SerializerTest() = default;

    SerializationHelper<GraphType> helper;
    using sut = Serializer;
};

TYPED_TEST_SUITE(SerializerTest, GraphTypes);

TYPED_TEST(SerializerTest, canSerializeLstFiles)
{
    auto [fileContent, referenceGraph] = this->helper.makeSampleLstFile();
    std::stringstream mockStream = {};

    SerializerTest<TypeParam>::sut::serializeLstFile(mockStream, referenceGraph);

    EXPECT_EQ(mockStream.str(), fileContent);
}

TYPED_TEST(SerializerTest, canSerializeMatFiles)
{
    auto [fileContent, referenceGraph] = this->helper.makeSampleMatFile();
    std::stringstream mockStream = {};

    SerializerTest<TypeParam>::sut::serializeMatFile(mockStream, referenceGraph);

    EXPECT_EQ(mockStream.str(), fileContent);
}

TYPED_TEST(SerializerTest, canSerializeGraphMlFiles)
{
    auto [fileContent, referenceGraph] = this->helper.makeSampleGraphMlFile();
    std::stringstream mockStream = {};

    SerializerTest<TypeParam>::sut::serializeGraphMlFile(mockStream, referenceGraph);

    EXPECT_EQ(mockStream.str(), fileContent);
}
} // namespace Graphs

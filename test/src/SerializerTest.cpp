#include <Graphs/AdjList.hpp>
#include <Graphs/AdjMatrix.hpp>
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

    using sut = Serializer;
};

using GraphTypes = ::testing::Types<AdjList, AdjMatrix>;

TYPED_TEST_SUITE(SerializerTest, GraphTypes);

TYPED_TEST(SerializerTest, canSerializeLstFiles)
{
    auto [fileContent, referenceGraph] = makeSampleLstFile<TypeParam>();
    std::stringstream mockStream = {};

    SerializerTest<TypeParam>::sut::serializeLstFile(mockStream, referenceGraph);

    EXPECT_EQ(mockStream.str(), fileContent);
}

TYPED_TEST(SerializerTest, canSerializeMatFiles)
{
    auto [fileContent, referenceGraph] = makeSampleMatFile<TypeParam>();
    std::stringstream mockStream = {};

    SerializerTest<TypeParam>::sut::serializeMatFile(mockStream, referenceGraph);

    EXPECT_EQ(mockStream.str(), fileContent);
}
} // namespace Graphs

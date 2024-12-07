#pragma once

#include <GraphRepresentationHelpers.hpp>
#include <string>
#include <tuple>

namespace Graphs
{
template <typename GraphType>
class SerializationHelper
{
public:
    std::tuple<std::string, GraphType> makeSampleGraphMlFile();
    std::tuple<std::string, GraphType> makeSampleDirectionalityMissmatchGraphMlFile();
    std::tuple<std::string, GraphType> makeSampleLstFile();
    std::tuple<std::string, GraphType> makeSampleMatFile();
    std::tuple<std::string, GraphType> makeSampleEmptyFile();
};

template class SerializationHelper<AdjList<GraphDirectionality::undirected>>;
template class SerializationHelper<AdjList<GraphDirectionality::directed>>;
template class SerializationHelper<AdjMatrix<GraphDirectionality::undirected>>;
template class SerializationHelper<AdjMatrix<GraphDirectionality::directed>>;
} // namespace Graphs

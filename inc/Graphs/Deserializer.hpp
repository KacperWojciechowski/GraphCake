#pragma once

#include <fstream>
#include <Graphs/AdjList.hpp>
#include <Graphs/AdjMatrix.hpp>
#include <type_traits>

namespace Graphs
{
class Graph;

template <typename GraphType, typename Guard = std::enable_if_t<std::is_base_of_v<Graph, GraphType>>>
class Deserializer
{
public:
    static GraphType deserializeLstFile(std::istream& filePath);
    static GraphType deserializeMatFile(std::istream& filePath);
    // static GraphType deserializeGraphMlFile(std::istream& filePath);
};

template class Deserializer<AdjList>;
template class Deserializer<AdjMatrix>;
} // namespace Graphs

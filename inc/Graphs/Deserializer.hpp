#pragma once

#include <Graphs/AdjList.hpp>
#include <Graphs/AdjMatrix.hpp>
#include <iosfwd>
#include <type_traits>

namespace Graphs
{
class Graph;

template <typename GraphType, typename Guard = std::enable_if_t<std::is_base_of_v<Graph, GraphType>>>
class Deserializer
{
public:
    static GraphType deserializeLstFile(std::istream&);
    static GraphType deserializeMatFile(std::istream&);
    static GraphType deserializeGraphMlFile(std::istream&);
};

template class Deserializer<AdjList>;
template class Deserializer<AdjMatrix>;
} // namespace Graphs

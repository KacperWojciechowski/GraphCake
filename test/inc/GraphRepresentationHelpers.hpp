#pragma once

#include <Graphs/AdjList.hpp>
#include <Graphs/AdjMatrix.hpp>
#include <gtest/gtest.h>

namespace Graphs
{
using GraphTypes = ::testing::Types<AdjList<GraphDirectionality::undirected>,
                                    AdjList<GraphDirectionality::directed>,
                                    AdjMatrix<GraphDirectionality::undirected>,
                                    AdjMatrix<GraphDirectionality::directed>>;

using DirectedGraphTypes
    = ::testing::Types<AdjList<GraphDirectionality::directed>, AdjMatrix<GraphDirectionality::directed>>;

using UndirectedGraphTypes
    = ::testing::Types<AdjList<GraphDirectionality::undirected>, AdjMatrix<GraphDirectionality::undirected>>;
} // namespace Graphs

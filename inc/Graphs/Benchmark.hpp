#pragma once

#include <cstdint>
#include <Graphs/Algorithm.hpp>
#include <Graphs/Graph.hpp>
#include <iosfwd>

namespace Graphs
{
class Graph;

namespace Algorithm
{
class Benchmark
{
public:
    Benchmark() {}
    Benchmark(Benchmark&) = delete;
    Benchmark(Benchmark&&) = delete;

    void run(Graphs::Graph& graph, std::string identifier, std::ostream& out, AlgorithmsCollection algorithms);

    ~Benchmark() {}
};
} // namespace Algorithm
} // namespace Graphs

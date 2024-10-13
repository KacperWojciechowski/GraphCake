#pragma once

#include <cstdint>
#include <fstream>
#include <Graphs/Graph.hpp>

namespace Graphs
{
class Graph;

class Benchmark
{
public:
    enum class Mode
    {
        append = 0,
        overwrite
    };

    Benchmark() {}
    Benchmark(Benchmark&) = delete;
    Benchmark(Benchmark&&) = delete;

    void run(Graphs::Graph& graph,
             std::string identifier,
             std::string file_path,
             uint16_t iterations,
             Mode mode,
             bool bench_log,
             bool alg_log);
    void color_benchmark(Graphs::Graph& graph,
                         std::string identifier,
                         uint16_t iterations,
                         std::fstream& file,
                         bool bench_log,
                         bool alg_log);

    ~Benchmark() {}
};
} // namespace Graphs

#include <Graphs/Benchmark.hpp>
#include <iostream>

void Graphs::Benchmark::run(Graphs::Graph& graph,
                            std::string identifier,
                            std::string file_path,
                            uint16_t iterations,
                            Mode mode,
                            bool bench_log,
                            bool alg_log)
{
    std::fstream file;
    switch (mode)
    {
    case Mode::append:
        file.open(file_path, std::ios_base::out | std::ios_base::app);
        break;
    case Mode::overwrite:
        file.open(file_path, std::ios_base::out);
        break;
    }
    if (file.good())
    {
        this->color_benchmark(graph, identifier, iterations, file, bench_log, alg_log);
    }
    else
    {
        std::cout << "Error opening the benchmark file" << std::endl;
    }
    if (bench_log)
    {
        std::cout << "Benchmark of " << identifier << " done" << std::endl;
    }
    file.close();
}

void Graphs::Benchmark::color_benchmark(
    Graphs::Graph& graph, std::string identifier, uint16_t iterations, std::fstream& file, bool bench_log, bool alg_log)
{
    for (uint16_t i = 0; i < iterations; i++)
    {
        file << identifier << ";";
        file << i << ";";

        // file << graph.greedy_coloring(alg_log) << ";";
        // file << graph.lf_coloring(alg_log) << ";";
        // file << graph.sl_coloring(alg_log) << std::endl;
        if (bench_log)
        {
            std::cout << "    Iteration " << i + 1 << " done" << std::endl;
        }
    }
}

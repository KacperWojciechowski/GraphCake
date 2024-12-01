#include <chrono>
#include <Graphs/Benchmark.hpp>
#include <iostream>
#include <stdexcept>

namespace Graphs::Algorithm
{
void Benchmark::run(Graphs::Graph& graph, std::string identifier, std::ostream& out, AlgorithmsCollection algorithms)
{
    if (out.good())
    {
        out << std::format("Benchmark of {} started\n", identifier);

        for (auto& algorithm : algorithms)
        {
            out << std::format("Running {} algorithm\n", algorithm->getName());

            auto start = std::chrono::system_clock::now();
            algorithm->operator()(graph);
            auto end = std::chrono::system_clock::now();

            std::chrono::duration<double> elapsed = end - start;
            auto elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(elapsed);

            out << std::format("Time spent on {} algorithm: {} ms\n", algorithm->getName(), elapsed_ms.count());
        }

        out << std::format("Benchmark of {} done\n", identifier);
    }
    else
    {
        throw std::runtime_error("Error with benchmark out stream");
    }
}
} // namespace Graphs::Algorithm

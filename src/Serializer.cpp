#include <Graphs/Graph.hpp>
#include <Graphs/SerializationFormats.hpp>
#include <Graphs/Serializer.hpp>
#include <sstream>

namespace
{
std::string processGraphIntoLstRepresentation(const Graphs::Graph& graph)
{
    std::stringstream out = {};

    for (auto node : graph.getNodeIds())
    {
        out << node << ": ";
        auto neighbors = graph.getNeighborsOf(node);
        for (auto idx = 0; auto neighbor : neighbors)
        {
            if (graph.findEdge({node, neighbor}).weight.has_value())
            {
                out << graph;
                if (idx++ < neighbors.size() - 1)
                {
                    out << " ";
                }
            }
        }
        out << "\n";
    }
    return out.str();
}
} // namespace

namespace Graphs
{
void Serializer::serializeLstFile(std::ostream& file, const Graph& graph)
{
    if (not file.good())
    {
        throw std::runtime_error("[Serializer] Error accessing file");
    }
    file << processGraphIntoLstRepresentation(graph);
    file.flush();
}
} // namespace Graphs

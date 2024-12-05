#include <algorithm>
#include <Graphs/Graph.hpp>
#include <Graphs/SerializationFormats.hpp>
#include <Graphs/Serializer.hpp>
#include <sstream>

namespace
{
std::string makeGraphMlHeader()
{
    return "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
           "  <graphml xmlns=\"http://graphml.graphdrawing.org/xmlns\"\n"
           "    xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\"\n"
           "    xsi:schemaLocation=\"http://graphml.graphdrawing.org/xmlns\n"
           "    http://graphml.graphdrawing.org/xmlns/1.0/graphml.xsd\">\n";
}

std::string makeGraphMlClosing()
{
    return "  </graph>\n</graphml>";
}

std::string processGraphIntoLstRepresentation(const Graphs::Graph& graph)
{
    std::stringstream out = {};

    for (auto node : graph.getNodeIds())
    {
        out << std::format("{}:", node);
        auto neighbors = graph.getOutgoingNeighborsOf(node);
        for (auto idx = 0; auto neighbor : neighbors)
        {
            if (idx == 0)
            {
                out << " ";
            }

            out << neighbor;

            if (idx++ < neighbors.size() - 1)
            {
                out << " ";
            }
        }
        out << "\n";
    }
    return out.str();
}

std::string processGraphIntoMatRepresentation(const Graphs::Graph& graph)
{
    std::stringstream out = {};

    auto nodeIds = graph.getNodeIds();
    for (auto srcNode : nodeIds)
    {
        for (auto idx = 0; auto tgtNode : nodeIds)
        {
            out << graph.findEdge({srcNode, tgtNode}).weight.value_or(0);
            if (idx++ < nodeIds.size() - 1)
            {
                out << " ";
            }
        }
        out << "\n";
    }
    return out.str();
}

std::string processGraphIntoGraphMlRepresentation(const Graphs::Graph& graph)
{
    std::stringstream out = {};
    out << makeGraphMlHeader();
    out << "  <graph id=\"Graph\" edgedefault=\"undirected\">\n";
    for (auto& node : graph.getNodeIds())
    {
        out << std::format("    <node id=\"n{}\"/>\n", node - 1);
    }
    for (auto& node : graph.getNodeIds())
    {
        for (auto& neighbor : graph.getOutgoingNeighborsOf(node))
        {
            out << std::format("    <edge source=\"n{}\" target=\"n{}\"/>\n", node - 1, neighbor - 1);
        }
    }
    out << makeGraphMlClosing();
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

void Serializer::serializeMatFile(std::ostream& file, const Graph& graph)
{
    if (not file.good())
    {
        throw std::runtime_error("[Serializer] Error accessing file");
    }
    file << processGraphIntoMatRepresentation(graph);
    file.flush();
}

void Serializer::serializeGraphMlFile(std::ostream& file, const Graph& graph)
{
    if (not file.good())
    {
        throw std::runtime_error("[Serializer] Error accessing file");
    }
    file << processGraphIntoGraphMlRepresentation(graph);
    file.flush();
}
} // namespace Graphs

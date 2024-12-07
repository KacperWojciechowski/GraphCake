#include <SerializationHelpers.hpp>

namespace
{
std::string getSampleDirectedGraphMlContent()
{
    return "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
           "  <graphml xmlns=\"http://graphml.graphdrawing.org/xmlns\"\n"
           "    xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\"\n"
           "    xsi:schemaLocation=\"http://graphml.graphdrawing.org/xmlns\n"
           "    http://graphml.graphdrawing.org/xmlns/1.0/graphml.xsd\">\n"
           "  <graph id=\"Graph\" edgedefault=\"directed\">\n"
           "    <node id=\"n0\"/>\n"
           "    <node id=\"n1\"/>\n"
           "    <node id=\"n2\"/>\n"
           "    <node id=\"n3\"/>\n"
           "    <node id=\"n4\"/>\n"
           "    <edge source=\"n0\" target=\"n1\"/>\n"
           "    <edge source=\"n1\" target=\"n0\"/>\n"
           "    <edge source=\"n2\" target=\"n3\"/>\n"
           "    <edge source=\"n3\" target=\"n2\"/>\n"
           "  </graph>\n"
           "</graphml>";
}

std::string getSampleUndirectedGraphMlContent()
{
    return "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
           "  <graphml xmlns=\"http://graphml.graphdrawing.org/xmlns\"\n"
           "    xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\"\n"
           "    xsi:schemaLocation=\"http://graphml.graphdrawing.org/xmlns\n"
           "    http://graphml.graphdrawing.org/xmlns/1.0/graphml.xsd\">\n"
           "  <graph id=\"Graph\" edgedefault=\"undirected\">\n"
           "    <node id=\"n0\"/>\n"
           "    <node id=\"n1\"/>\n"
           "    <node id=\"n2\"/>\n"
           "    <node id=\"n3\"/>\n"
           "    <node id=\"n4\"/>\n"
           "    <edge source=\"n0\" target=\"n1\"/>\n"
           "    <edge source=\"n1\" target=\"n0\"/>\n"
           "    <edge source=\"n2\" target=\"n3\"/>\n"
           "    <edge source=\"n3\" target=\"n2\"/>\n"
           "  </graph>\n"
           "</graphml>";
}
} // namespace

namespace Graphs
{

template <typename GraphType>
std::tuple<std::string, GraphType> SerializationHelper<GraphType>::makeSampleGraphMlFile()
{
    std::string streamContent = {};
    if constexpr (GraphType::Directionality == GraphDirectionality::directed)
    {
        streamContent = getSampleDirectedGraphMlContent();
    }
    else
    {
        streamContent = getSampleUndirectedGraphMlContent();
    }
    GraphType graphRep = {};
    graphRep.addNodes(5);
    graphRep.setEdge({.source = 1, .destination = 2});
    graphRep.setEdge({.source = 2, .destination = 1});
    graphRep.setEdge({.source = 3, .destination = 4});
    graphRep.setEdge({.source = 4, .destination = 3});

    return std::tie(streamContent, graphRep);
}

template <typename GraphType>
std::tuple<std::string, GraphType> SerializationHelper<GraphType>::makeSampleDirectionalityMissmatchGraphMlFile()
{
    std::string streamContent = {};
    if constexpr (GraphType::Directionality == GraphDirectionality::directed)
    {
        streamContent = getSampleUndirectedGraphMlContent();
    }
    else
    {
        streamContent = getSampleDirectedGraphMlContent();
    }
    GraphType graphRep = {};
    return std::tie(streamContent, graphRep);
}

template <typename GraphType>
std::tuple<std::string, GraphType> SerializationHelper<GraphType>::makeSampleLstFile()
{
    auto streamContent = "1: 1 2 3\n"
                         "2: 1\n"
                         "3: 1 3\n";
    GraphType graphRep = {};
    graphRep.addNodes(3);
    graphRep.setEdge({.source = 1, .destination = 1});
    graphRep.setEdge({.source = 1, .destination = 2});
    graphRep.setEdge({.source = 1, .destination = 3});
    graphRep.setEdge({.source = 2, .destination = 1});
    graphRep.setEdge({.source = 3, .destination = 1});
    graphRep.setEdge({.source = 3, .destination = 3});

    return std::tie(streamContent, graphRep);
}

template <typename GraphType>
std::tuple<std::string, GraphType> SerializationHelper<GraphType>::makeSampleMatFile()
{
    auto streamContent = "1 2 3\n"
                         "2 1 0\n"
                         "3 0 3\n";

    GraphType graphRep = {};
    graphRep.addNodes(3);
    graphRep.setEdge({.source = 1, .destination = 1});
    graphRep.setEdge({.source = 1, .destination = 2, .weight = 2});
    graphRep.setEdge({.source = 1, .destination = 3, .weight = 3});
    graphRep.setEdge({.source = 2, .destination = 1, .weight = 2});
    graphRep.setEdge({.source = 2, .destination = 2});
    graphRep.setEdge({.source = 3, .destination = 1, .weight = 3});
    graphRep.setEdge({.source = 3, .destination = 3, .weight = 3});

    return std::tie(streamContent, graphRep);
}

template <typename GraphType>
std::tuple<std::string, GraphType> SerializationHelper<GraphType>::makeSampleEmptyFile()
{
    return {"", GraphType{}};
}
} // namespace Graphs

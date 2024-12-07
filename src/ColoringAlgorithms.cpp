#include <algorithm>
#include <format>
#include <Graphs/Algorithm.hpp>
#include <Graphs/ColoringAlgorithms.hpp>
#include <ranges>
#include <stdexcept>

namespace Graphs::Algorithm
{
namespace
{
void ostreamDeleter(std::ostream* out)
{
    if (out != &std::cout)
    {
        delete out;
    }
}

template <bool isVerbose, class... Args>
void log(std::ostream& outStream, std::string formatString, Args... args)
{
    if constexpr (not isVerbose)
    {
        return;
    }
    else
    {
        if constexpr (sizeof...(args) == 0)
        {
            outStream << formatString;
        }
        else
        {
            outStream << std::vformat(formatString, std::make_format_args(args...));
        }
    }
}

template <bool isVerbose>
void printPermutationOfNodes(std::ostream& out, const Permutation& nodes)
{
    if (nodes.empty())
    {
        log<isVerbose>(out, "Empty permutation\n");
        return;
    }

    auto distance = std::ranges::distance(nodes);
    for (const auto& nodeId : nodes | std::views::take(distance - 1))
    {
        log<isVerbose>(out, "{}, ", nodeId);
    }
    log<isVerbose>(out, "{}\n", nodes.back());
}

ColorId colorPredicate(const ColoringInfo& info)
{
    auto& [_, color] = info;
    return color;
}

ColorId findAvailableColorForCurrentNode(const Graph& graph, ColoringVector& coloring, NodeId currentNode)
{
    auto isNeighbor = [&graph, &currentNode](auto neighbor) {
        auto neighbors = graph.getNeighborsOf(currentNode);
        auto& [neighborId, _] = neighbor;

        return std::ranges::find(neighbors, neighborId) != neighbors.end();
    };

    auto neighborsColoringView = coloring | std::views::filter(isNeighbor);
    std::size_t neighborsCount = std::ranges::distance(neighborsColoringView);

    for (const auto& color : std::views::iota(0u, neighborsCount))
    {
        if (std::ranges::find(neighborsColoringView, color, colorPredicate) == neighborsColoringView.end())
        {
            return color;
        }
    }
    return neighborsCount + 1;
}

ColoringVector createColoringTable(const Permutation& nodes)
{
    ColoringVector coloring = {};
    coloring.reserve(nodes.size());
    for (const auto& nodeId : nodes)
    {
        coloring.emplace_back(nodeId, std::numeric_limits<ColorId>::max());
    }
    return coloring;
}

ColoringResult resizeAndInitializeResultStructure(const Permutation& nodes)
{
    auto [colorId, coloringVector] = ColoringResult{};
    coloringVector = createColoringTable(nodes);
    colorId = std::numeric_limits<ColorId>::min();
    return std::tie(colorId, coloringVector);
}
} // namespace

template <bool isVerbose>
void GreedyColoring<isVerbose>::operator()(const Graphs::Graph& graph)
{
    log<isVerbose>(*outStream, "Greedy coloring graph with {} nodes\n", graph.nodesAmount());
    auto nodes = graph.getNodeIds();

    log<isVerbose>(*outStream, "Generated permutation of nodes: ");
    printPermutationOfNodes<isVerbose>(*outStream, nodes);

    log<isVerbose>(*outStream, "{}\n", nodes.size());
    *result = resizeAndInitializeResultStructure(nodes);
    auto& [maxColor, coloring] = *result;

    for (auto& elem : coloring)
    {
        log<isVerbose>(*outStream, "Node: {}, Color: {}\n", elem.first, elem.second);
    }

    if (nodes.empty())
    {
        log<isVerbose>(*outStream, "Graph is empty, coloring is not possible\n");
        return;
    }

    auto& [frontNodeId, frontNodeColor] = coloring.front();
    frontNodeColor = 0u;

    log<isVerbose>(*outStream, "Coloring node {} with color {}\n", frontNodeId, frontNodeColor);

    for (auto& [nodeId, nodeColor] : coloring | std::views::drop(1))
    {
        auto color = findAvailableColorForCurrentNode(graph, coloring, nodeId);

        nodeColor = color;
        log<isVerbose>(*outStream, "Coloring node {} with color {}\n", nodeId, nodeColor);
    }

    auto [_, maxColorId] = std::ranges::max(coloring, std::ranges::less{}, colorPredicate);
    maxColor = maxColorId;

    log<isVerbose>(*outStream, "Greedy coloring completed\n");
}

template <bool isVerbose>
GreedyColoring<isVerbose>::GreedyColoring(std::shared_ptr<ColoringResult> resultContainer, std::ostream& out)
    : result(std::move(resultContainer)), outStream{&out, ostreamDeleter}
{
    if (not result)
    {
        log<isVerbose>(*outStream, "Coloring result cannot be null");
        throw std::invalid_argument{"Coloring result cannot be null"};
    }
}

template <bool isVerbose>
std::string GreedyColoring<isVerbose>::getName()
{
    return "Greedy coloring";
}

template class GreedyColoring<verbose>;
template class GreedyColoring<notVerbose>;

} // namespace Graphs::Algorithm

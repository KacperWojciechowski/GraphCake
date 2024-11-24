#include <algorithm>
#include <format>
#include <Graphs/Algorithm.hpp>
#include <Graphs/ColoringAlgorithms.hpp>
#include <memory>
#include <random>
#include <ranges>
#include <stdexcept>

namespace Graphs::Algorithm
{
namespace
{
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
    coloring.resize(nodes.size());
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
    colorId = std::numeric_limits<ColorId>::max();
    return std::tie(colorId, coloringVector);
}
} // namespace

template <bool isVerbose>
template <class... Args>
void GreedyColoring<isVerbose>::log(std::string formatString, Args... args) const
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
void GreedyColoring<isVerbose>::operator()(const Graphs::Graph& graph)
{
    log("Greedy coloring graph with {} nodes\n", graph.nodesAmount());

    auto nodes = graph.getNodeIds();

    log("Generated permutation of nodes: ");
    for (const auto& nodeId : nodes)
    {
        log("{}, ", nodeId);
    }
    log("\n");

    *result = resizeAndInitializeResultStructure(nodes);
    auto& [maxColor, coloring] = *result;
    auto& [frontNodeId, frontNodeColor] = coloring.front();
    frontNodeColor = 0;

    log("Coloring node {} with color {}\n", frontNodeId, frontNodeColor);

    for (auto& [nodeId, nodeColor] : coloring | std::views::drop(1))
    {
        auto color = findAvailableColorForCurrentNode(graph, coloring, nodeId);

        nodeColor = color;
        log("Coloring node {} with color {}\n", nodeId, nodeColor);
    }

    auto [_, maxColorId] = std::ranges::max(coloring, std::ranges::less{}, colorPredicate);
    maxColor = maxColorId;

    log("Greedy coloring completed\n");
}

template <bool isVerbose>
GreedyColoring<isVerbose>::GreedyColoring(std::shared_ptr<ColoringResult> resultContainer, std::ostream& out)
    : result(std::move(resultContainer)), outStream{out}
{
    if (not result)
    {
        log("Coloring result cannot be null");
        throw std::invalid_argument{"Coloring result cannot be null"};
    }
}

template class GreedyColoring<verbose>;
template class GreedyColoring<notVerbose>;

} // namespace Graphs::Algorithm

#include <format>
#include <Graphs/Algorithm.hpp>
#include <Graphs/ColoringAlgorithms.hpp>
#include <memory>
#include <random>
#include <ranges>

namespace Graphs::Algorithm
{
namespace
{
Permutation prepareNodePermutationForGreedyColoring(const Graph& graph)
{
    auto nodeIds = graph.getNodeIds();
    std::shuffle(nodeIds.begin(), nodeIds.end(), std::random_device{});
    return nodeIds;
}

ColorId
findAvailableColorForCurrentNode(const Graph& graph, ColoringResult& result, ColoringVector::iterator currentNode)
{
    auto [_, coloring] = result;
    auto isNeighbor = [&graph, &currentNode](auto neighbor) {
        static auto neighbors = graph.getNeighborsOf(currentNode->first);

        return std::ranges::find(neighbors, neighbor.first) != neighbors.end();
    };

    auto neighborsColoringView
        = std::ranges::subrange(coloring.begin(), coloring.end()) | std::views::filter(isNeighbor);
    std::size_t neighborsCount = std::ranges::distance(neighborsColoringView);

    auto colorFilter = [](const auto& coloring) {
        return coloring.second;
    };

    for (const auto& color : std::views::iota(0u, neighborsCount))
    {
        if (std::ranges::find(neighborsColoringView, color, colorFilter) == neighborsColoringView.end())
        {
            return color;
        }
    }
    return neighborsCount + 1;
}

ColoringResult resizeAndInitializeResultStructure(const Permutation& nodes)
{
    auto result = ColoringResult{};
    result.resize(nodes.size());
    for (std::size_t i = 0; i < nodes.size(); i++)
    {
        result[i].first = nodes[i];
        result[i].second = std::numeric_limits<ColorId>::max();
    }
    return result;
}
} // namespace

template <>
template <class... Args, class T, Verbose<verbose, T>>
void GreedyColoring<verbose>::log(std::string formatString, Args... args) const
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

std::vector<std::pair<NodeId, ColorId>> createColoringTable(const Permutation& nodes)
{
    std::vector<std::pair<NodeId, ColorId>> coloring;
    for (const auto& nodeId : nodes)
    {
        coloring.emplace_back(nodeId, std::numeric_limits<ColorId>::max());
    }
    return coloring;
}

template <>
void GreedyColoring<verbose>::operator()(const Graphs::Graph& graph)
{
    log("Greedy coloring graph with {} nodes\n", graph.nodesAmount());

    auto nodes = prepareNodePermutationForGreedyColoring(graph);

    log("Generated permutation of nodes: ");
    for (const auto& nodeId : nodes)
    {
        log("{}, ", nodeId);
    }
    log("\n");

    *result = resizeAndInitializeResultStructure(nodes);
    result->front().second = 0;

    log("Coloring node {} with color {}\n", result->front().first, result->front().second);

    for (auto currentNode = ++result->begin(); currentNode != result->end(); currentNode++)
    {
        auto color = findAvailableColorForCurrentNode(graph, *result, currentNode);

        currentNode->second = color;
        log("Coloring node {} with color {}\n", currentNode->first, currentNode->second);
    }

    log("Greedy coloring completed\n");
}

template <>
void GreedyColoring<notVerbose>::operator()(const Graphs::Graph& graph)
{
    auto nodes = prepareNodePermutationForGreedyColoring(graph);
    *result = resizeAndInitializeResultStructure(nodes);
    result->front().second = 0;

    for (auto currentNode = ++result->begin(); currentNode != result->end(); currentNode++)
    {
        auto color = findAvailableColorForCurrentNode(graph, *result, currentNode);

        currentNode->second = color;
    }
}
} // namespace Graphs::Algorithm

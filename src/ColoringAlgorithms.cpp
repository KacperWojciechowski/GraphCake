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

std::vector<NodeId> generateLfCompliantPermutationOfNodes(const Graph& graph, const Permutation& nodes)
{
    std::vector<NodeId> permutatedNodes = {};
    permutatedNodes.reserve(nodes.size());
    auto maxDeg = graph.graphDegree();
    for (auto degree = maxDeg; degree > 0; --degree)
    {
        auto nodesWithDegree
            = nodes | std::views::filter([degree, &graph](auto nodeId) {
                  return std::max(graph.getOutgoingDegree(nodeId), graph.getIncommingDegree(nodeId)) == degree;
              });
        std::ranges::copy(nodesWithDegree, std::back_inserter(permutatedNodes));
    }

    return permutatedNodes;
}

std::vector<NodeId> generateSlCompliantPermutationOfNodes(const Graph& graph, const Permutation& nodes)
{
    std::vector<NodeId> permutatedNodes = {};
    permutatedNodes.reserve(nodes.size());

    using DegreeInfo = std::pair<NodeId, uint32_t>;
    auto adjustedDegrees = std::vector<DegreeInfo>{nodes.size()};

    for (auto& [nodeId, degree] : adjustedDegrees)
    {
        degree = std::max(graph.getOutgoingDegree(nodeId), graph.getIncommingDegree(nodeId));
    }

    for (std::size_t i = 0; i < nodes.size(); ++i)
    {
        auto& [minDegreeNodeId, foundDegree]
            = *std::ranges::min_element(adjustedDegrees, [](const auto& lhs, const auto& rhs) {
                  return lhs.second < rhs.second;
              });

        permutatedNodes.push_back(minDegreeNodeId);
        foundDegree = std::numeric_limits<uint32_t>::max();

        auto neighbors = graph.getNeighborsOf(minDegreeNodeId);
        for (auto& [nodeId, degree] : adjustedDegrees)
        {
            if (std::ranges::find(neighbors, nodeId) != neighbors.end())
            {
                degree--;
            }
        }
    }

    std::reverse(permutatedNodes.begin(), permutatedNodes.end());
    return permutatedNodes;
}

ColoringResult resizeAndInitializeResultStructure(const Permutation& nodes)
{
    auto [colorId, coloringVector] = ColoringResult{};
    coloringVector = createColoringTable(nodes);
    colorId = std::numeric_limits<ColorId>::min();
    return std::tie(colorId, coloringVector);
}

template <bool isVerbose>
ColoringResult performCoreColoring(std::ostream& outStream, const Graph& graph, const Permutation& permutation)
{
    log<isVerbose>(outStream, "Generated permutation of nodes: ");
    printPermutationOfNodes<isVerbose>(outStream, permutation);

    ColoringResult result = {};
    log<isVerbose>(*outStream, "{}\n", permutation.size());
    result = resizeAndInitializeResultStructure(nodes);

    auto& [maxColor, coloring] = result;
    auto& [frontNodeId, frontNodeColor] = coloring.front();
    frontNodeColor = 0u;

    log<isVerbose>(*outStream, "Coloring node {} with color {}\n", frontNodeId, frontNodeColor);

    for (auto& [nodeId, nodeColor] : coloring | std::views::drop(1))
    {
        nodeColor = findAvailableColorForCurrentNode(graph, coloring, nodeId);
        log<isVerbose>(*outStream, "Coloring node {} with color {}\n", nodeId, nodeColor);
    }

    auto [_, maxColorId] = std::ranges::max(coloring, std::ranges::less{}, colorPredicate);
    maxColor = maxColorId;

    return result;
}
} // namespace

template <bool isVerbose>
void GreedyColoring<isVerbose>::operator()(const Graphs::Graph& graph)
{
    if (nodes.empty())
    {
        log<isVerbose>(*outStream, "Graph is empty, coloring is not possible\n");
        return;
    }

    log<isVerbose>(*outStream, "Greedy coloring graph with {} nodes\n", graph.nodesAmount());
    auto& nodes = graph.getNodeIds();

    *result = performCoreColoring(*outStream, graph, nodes);

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

template <bool isVerbose>
void LfColoring<isVerbose>::operator()(const Graphs::Graph&)
{
    if (nodes.empty())
    {
        log<isVerbose>(*outStream, "Graph is empty, coloring is not possible\n");
        return;
    }

    log<isVerbose>(*outStream, "LF coloring graph with {} nodes\n", graph.nodesAmount());
    auto& nodes = graph.getNodeIds();

    auto permutatedNodes = generateLfCompliantPermutationOfNodes(nodes);
    *result = performCoreColoring(*outStream, graph, permutatedNodes);

    log<isVerbose>(*outStream, "LF coloring completed\n");
}

template <bool isVerbose>
LfColoring<isVerbose>::LfColoring(std::shared_ptr<ColoringResult> resultContainer, std::ostream& out)
    : result(std::move(resultContainer)), outStream{&out, ostreamDeleter}
{
    if (not result)
    {
        log<isVerbose>(*outStream, "Coloring result cannot be null");
        throw std::invalid_argument{"Coloring result cannot be null"};
    }
}

template <bool isVerbose>
std::string LfColoring<isVerbose>::getName()
{
    return "LF coloring";
}

template class LfColoring<verbose>;
template class LfColoring<notVerbose>;

template <bool isVerbose>
void SlColoring<isVerbose>::operator()(const Graphs::Graph&)
{
    if (nodes.empty())
    {
        log<isVerbose>(*outStream, "Graph is empty, coloring is not possible\n");
        return;
    }

    log<isVerbose>(*outStream, "SL coloring graph with {} nodes\n", graph.nodesAmount());
    auto& nodes = graph.getNodeIds();

    auto permutatedNodes = generateSlCompliantPermutationOfNodes(nodes);
    *result = performCoreColoring(*outStream, graph, permutatedNodes);

    log<isVerbose>(*outStream, "SL coloring completed\n");
}

template <bool isVerbose>
SlColoring<isVerbose>::SlColoring(std::shared_ptr<ColoringResult> resultContainer, std::ostream& out)
    : result(std::move(resultContainer)), outStream{&out, ostreamDeleter}
{
    if (not result)
    {
        log<isVerbose>(*outStream, "Coloring result cannot be null");
        throw std::invalid_argument{"Coloring result cannot be null"};
    }
}

template <bool isVerbose>
std::string SlColoring<isVerbose>::getName()
{
    return "SL coloring";
}
} // namespace Graphs::Algorithm

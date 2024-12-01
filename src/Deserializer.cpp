#include <filesystem>
#include <Graphs/Deserializer.hpp>
#include <Graphs/Graph.hpp>
#include <iostream>
#include <regex>
#include <vector>

namespace
{
enum class FileType
{
    LST,
    MAT,
    GRAPHML
};

std::vector<uint32_t> parseLstLine(const std::string& line)
{
    std::vector<uint32_t> neighbors = {};
    std::stringstream stream(line);
    std::string value = {};

    while (std::getline(stream, value, ' '))
    {
        if (value.empty())
        {
            continue;
        }
        neighbors.emplace_back(std::stoul(value));
    }
    return neighbors;
}

std::vector<Graphs::WeightType> parseMatLine(const std::string& line)
{
    std::vector<Graphs::WeightType> weights = {};
    std::stringstream stream(line);
    std::string value = {};

    while (std::getline(stream, value, ' '))
    {
        if (value.empty())
        {
            continue;
        }
        weights.emplace_back(std::stoi(value));
    }
    return weights;
}
} // namespace

template <typename GraphType, FileType Ft>
class ContentIntoGraphRepresentationParser
{
public:
    static GraphType parse(const std::string&)
    {
        return GraphType{};
    }
};

template <typename GraphType>
class ContentIntoGraphRepresentationParser<GraphType, FileType::MAT>
{
public:
    static GraphType parse(const std::string& content)
    {
        if (content.empty())
        {
            return GraphType{};
        }

        using regItr = std::sregex_iterator;
        std::regex matLineRegex("([0-9 ]+)");
        std::vector<std::vector<Graphs::WeightType>> weights;

        for (auto itr = regItr(content.begin(), content.end(), matLineRegex); itr != regItr(); ++itr)
        {
            if (itr->size() < 1)
            {
                std::cerr << "[Deserializer] Invalid line format";
                continue;
            }
            std::ssub_match match = (*itr)[1];
            weights.emplace_back(parseMatLine(match.str()));
        }

        GraphType graph;
        graph.addNodes(weights.size());
        for (uint32_t nodeId = 1; nodeId <= weights.size(); nodeId++)
        {
            for (uint32_t neighborId = 1; neighborId <= weights[nodeId - 1].size(); neighborId++)
            {
                if (weights[nodeId - 1][neighborId - 1] != 0)
                {
                    graph.setEdge({nodeId, neighborId, weights[nodeId - 1][neighborId - 1]});
                }
            }
        }
        return graph;
    }
};

template <typename GraphType>
class ContentIntoGraphRepresentationParser<GraphType, FileType::LST>
{
public:
    static GraphType parse(const std::string& content)
    {
        if (content.empty())
        {
            return GraphType{};
        }

        using regItr = std::sregex_iterator;
        std::regex nodeRegex("[0-9]:([0-9 ]+)");
        std::vector<std::vector<Graphs::NodeId>> nodes;

        for (auto itr = regItr(content.begin(), content.end(), nodeRegex); itr != regItr(); ++itr)
        {
            if (itr->size() < 2)
            {
                std::cerr << "[Deserializer] Invalid line format";
                continue;
            }
            std::ssub_match match = (*itr)[1];
            nodes.emplace_back(parseLstLine(match.str()));
        }

        GraphType graph = {};
        graph.addNodes(nodes.size());
        for (uint32_t nodeId = 1; nodeId <= nodes.size(); nodeId++)
        {
            for (auto neighbor : nodes[nodeId - 1])
            {
                graph.setEdge({nodeId, neighbor});
            }
        }
        return graph;
    }
};

namespace Graphs
{
template <typename GraphType, typename Guard>
GraphType Deserializer<GraphType, Guard>::deserializeLstFile(std::istream& file)
{
    if (not file.good())
    {
        std::cerr << "[Deserializer] Error when accessing file";
        return GraphType{};
    }
    auto fileContent = std::string(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>());
    return ContentIntoGraphRepresentationParser<GraphType, FileType::LST>::parse(fileContent);
}

template <typename GraphType, typename Guard>
GraphType Deserializer<GraphType, Guard>::deserializeMatFile(std::istream& file)
{
    if (not file.good())
    {
        std::cerr << "[Deserializer] Error when accessing file";
        return GraphType{};
    }
    auto fileContent = std::string(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>());
    return ContentIntoGraphRepresentationParser<GraphType, FileType::MAT>::parse(fileContent);
}
} // namespace Graphs

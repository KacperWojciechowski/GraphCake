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
        std::regex matLineRegex("([0-9 ]+)[^a-zA-Z]");
        std::vector<std::vector<Graphs::WeightType>> weights;

        for (auto itr = regItr(content.begin(), content.end(), matLineRegex); itr != regItr(); ++itr)
        {
            std::ssub_match match = (*itr)[1];
            weights.emplace_back(parseMatLine(match.str()));
        }

        for (const auto& line : weights)
        {
            if (line.size() != weights.size())
            {
                std::cerr << "[Deserializer] Invalid matrix proportions.\n "
                             "Make sure each row contains the same number of values.\n";
                return GraphType{};
            }
        }

        GraphType graph = {};
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

template <typename GraphType>
class ContentIntoGraphRepresentationParser<GraphType, FileType::GRAPHML>
{
public:
    static GraphType parse(const std::string& content)
    {
        if (content.empty())
        {
            return GraphType{};
        }

        using regItr = std::sregex_iterator;
        std::regex nodeRegex("<node id=\"n([0-9]+)\"/>");
        std::regex edgeRegex("<edge source=\"n([0-9]+)\" target=\"n([0-9]+)\"/>");
        std::vector<Graphs::EdgeInfo> nodes;
        Graphs::NodeId nodesCount = 0;

        bool incrementIds = false;

        for (auto itr = regItr(content.begin(), content.end(), nodeRegex); itr != regItr(); ++itr)
        {
            std::ssub_match match = (*itr)[1];
            Graphs::NodeId id = std::stoul(match.str());

            if (id == 0)
            {
                incrementIds = true;
            }
            nodesCount = std::max(nodesCount, incrementIds ? id + 1 : id);
        }

        for (auto itr = regItr(content.begin(), content.end(), edgeRegex); itr != regItr(); ++itr)
        {
            if (itr->size() < 3)
            {
                std::cerr << "[Deserializer] Invalid line format";
                continue;
            }
            std::ssub_match sourceNode = (*itr)[1];
            std::ssub_match destinationNode = (*itr)[2];

            Graphs::NodeId srcNodeId = std::stoul(sourceNode.str());
            Graphs::NodeId destNodeId = std::stoul(destinationNode.str());

            // clang-format off
            nodes.push_back({.source = incrementIds ? srcNodeId + 1 : srcNodeId, 
                             .destination = incrementIds ? destNodeId + 1 : destNodeId});
            // clang-format on
        }

        GraphType graph = {};
        graph.addNodes(nodesCount);
        graph.setEdges(nodes);

        return graph;
    }
};

std::string readFileContent(std::istream& file)
{
    return std::string(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>());
}
}; // namespace

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
    return ContentIntoGraphRepresentationParser<GraphType, FileType::LST>::parse(readFileContent(file));
}

template <typename GraphType, typename Guard>
GraphType Deserializer<GraphType, Guard>::deserializeMatFile(std::istream& file)
{
    if (not file.good())
    {
        std::cerr << "[Deserializer] Error when accessing file";
        return GraphType{};
    }
    return ContentIntoGraphRepresentationParser<GraphType, FileType::MAT>::parse(readFileContent(file));
}

template <typename GraphType, typename Guard>
GraphType Deserializer<GraphType, Guard>::deserializeGraphMlFile(std::istream& file)
{
    if (not file.good())
    {
        std::cerr << "[Deserializer] Error when accessing file";
        return GraphType{};
    }
    return ContentIntoGraphRepresentationParser<GraphType, FileType::GRAPHML>::parse(readFileContent(file));
}
} // namespace Graphs

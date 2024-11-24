// this
#include <Graphs/AdjList.hpp>
#include <Graphs/AdjMatrix.hpp>

// libraries
#include <algorithm>
#include <cassert>
#include <filesystem>
#include <fstream>
#include <ranges>
#include <regex>
#include <sstream>
#include <string>

namespace Graphs
{
namespace
{
/*uint32_t findNodesCount(const std::string& fileContent, const std::regex& nodeRegex)
{
    std::smatch match;
    uint32_t nodesCount = 0;
    auto it = fileContent.cbegin();
    while (std::regex_search(it, fileContent.cend(), match, nodeRegex))
    {
        nodesCount++;
        it = match.suffix().first;
    }
    return nodesCount;
}*/

/*struct EdgeParsingResult
{
    bool result;
    EdgeInfo edge;
    std::string::const_iterator nextItr;
};

EdgeParsingResult findEdgeInFileContent(const std::string& fileContent,
                                        const std::regex& edgeRegex,
                                        const std::string::const_iterator itr)
{
    std::smatch match;
    if (std::regex_search(itr, fileContent.cend(), match, edgeRegex))
    {
        constexpr uint32_t defaultWeight = 1;
        auto source = static_cast<uint32_t>(std::stoi(match[1].str()));
        auto target = static_cast<uint32_t>(std::stoi(match[2].str()));

        return {
            true, {source, target, defaultWeight},
             match.suffix().first
        };
    }
    return {false, {}, itr};
}*/
} // namespace

void AdjMatrix::resizeMatrixToFitNodes(uint32_t nodesCount)
{
    assert(nodesCount > matrix.size());

    auto nodesAmountDiff = nodesCount - matrix.size();
    auto maxNodeItr = nodeIndexMapping.rbegin();
    auto maxNodeId = maxNodeItr != nodeIndexMapping.rend() ? maxNodeItr->first : 0;

    for (uint32_t i = 0; i < nodesAmountDiff; i++)
    {
        nodeIndexMapping.insert(std::make_pair(maxNodeId + i + 1, matrix.size() + i));
    }

    matrix.resize(nodesCount);
    for (auto& row : matrix)
    {
        row.resize(nodesCount);
    }
}

/*void AdjMatrix::buildFromMatFile(const std::string& filePath) {
    std::ifstream file(filePath);
    if (not file.good())
    {
        throw std::runtime_error("Error opening file");
    }

    auto parseLine = [](const auto& line) {
        Row row;
        std::stringstream stream(line);
        std::string value;

        while (std::getline(stream, value, ' '))
        {
            row.emplace_back(std::stoi(value));
        }

        return row;
    };

    NodeId nodeId = 0;
    while (not file.eof())
    {
        std::string line;
        std::getline(file, line);

        if (line.empty())
        {
            continue;
        }
        matrix.emplace_back(parseLine(line));
        nodeIndexMapping.insert(std::make_pair(nodeId++, matrix.size() - 1));
    }
}*/

/*void AdjMatrix::buildFromGraphMLFile(const std::string& filePath) {
    std::ifstream file(filePath);

    if (not file.good())
    {
        throw std::runtime_error("Error opening file");
    }

    std::regex nodeRegex("<node id=\"n([0-9]*)\"/>");
    std::regex edgeRegex("<edge source=\"n([0-9]*)\" target=\"n([0-9]*)\"\\/>");

    auto fileContent = std::string(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>());

    auto nodesCount = findNodesCount(fileContent, nodeRegex);
    resizeMatrixToFitNodes(nodesCount);

    auto itr = fileContent.cbegin();
    while (itr != fileContent.cend())
    {
        auto [result, edge, nextItr] = findEdgeInFileContent(fileContent, edgeRegex, itr);

        if (not result)
        {
            break;
        }

        if (edge.weight.has_value())
        {
            matrix[edge.source][edge.destination] = edge.weight.value();
        }
        itr = nextItr;
    }
}

AdjMatrix::AdjMatrix(std::string filePath) {
    std::filesystem::path path(filePath);
    const auto& extension = path.extension().string();
    assert(extension == ".mat" or extension == ".GRAPHML");

    if (extension == ".mat")
    {
        buildFromMatFile(filePath);
    }
    else if (extension == ".GRAPHML")
    {
        buildFromGraphMLFile(filePath);
    }
}*/

AdjMatrix::AdjMatrix(const Graph& other)
{
    resizeMatrixToFitNodes(other.nodesAmount());

    for (uint32_t i = 1; i <= matrix.size(); i++)
    {
        for (uint32_t j = 1; j <= matrix[i].size(); j++)
        {
            matrix[i - 1][j - 1] = other.findEdge({i + 1, j + 1}).weight.value_or(0);
        }
    }
}

uint32_t AdjMatrix::nodeDegree(NodeId node) const
{
    if (node > matrix.size())
    {
        return 0;
    }

    auto nodeIndex = nodeIndexMapping.find(node);
    if (nodeIndex == nodeIndexMapping.end())
    {
        return 0;
    }

    auto& [_, index] = *nodeIndex;

    uint32_t degree = 0;
    for (auto& elem : matrix[index])
    {
        if (elem != 0)
        {
            degree++;
        }
    }
    return degree;
}

/*void AdjMatrix::saveGraphML(std::string file_path) {
    // opening output file
    std::ofstream file(file_path);

    // header
    file << "<?xml version=\"1.0\"";
    file << " encoding=\"UTF-8\"?>\n";

    // xml schema
    file << "<graphml xmlns=";
    file << "\"http://graphml.graphdrawing.org/xmlns\"\n";
    file << "	xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\"\n";
    file << "	xsi:schemaLocation=\"http://graphml.graphdrawing.org/xmlns\n";
    file << "	http://graphml.graphdrawing.org/xmlns/1.0/graphml.xsd\">\n";

    // graph type data
    file << " <graph id=";
    file << "\"" + graph_name + "\"";
    file << " edgedefault=";
    file << "\"" + graph_type + "\">\n";

    // nodes data
    for (uint32_t i = 0; i < this->nodes_amount; i++) // Tworzy wiersze nod�w <node id="n0"/>
    {
        file << "	<node id=\"n" << i;
        file << "\"/>\n";
    }

    // edge data
    for (uint32_t i = 0; i < this->nodes_amount; i++)
    {
        for (uint32_t j = 0; j < this->nodes_amount; j++)
        {
            if (this->matrix[i][j] == 1)
            {
                file << "	<edge source=\"n" << i;
                file << "\" target=\"n" << j;
                file << "\"/>\n";
            }
        }
    }
    // close the markers and the file
    file << " </graph>\n";
    file << "</graphml>";
    file.close();
}*/

/*	Function calculates the Estrada index for the graph
        given the equation:

        index = sum(sqrt(deg(u) + deg(v) - 2)/(deg(v)^2)

        Params:
        None

        Return:
        value of Estrada index
*/
/*float AdjMatrix::estrada_index() {
    uint32_t* degrees = new uint32_t[this->nodes_amount];

    // calculating degrees
    for (uint32_t i = 0; i < this->nodes_amount; i++)
    {
        degrees[i] = 0;
        for (uint32_t j = 0; j < this->nodes_amount; j++)
        {
            if (this->matrix[i][j] == 1)
            {
                degrees[i]++;
            }
        }
    }

    float index = 0;

    // calculating the index
    for (uint32_t i = 0; i < this->nodes_amount; i++)
    {
        for (uint32_t j = 0; j < this->nodes_amount; j++)
        {
            if (this->matrix[i][j] == 1) // �apie s�siada
            {
                float root = std::sqrt(static_cast<float>(degrees[i] + degrees[j] - 2) / (pow(degrees[i], 2)));
                index += root;
            }
        }
    }

    // free allocated memory
    delete[] degrees;

    // return Estrada index value
    return index;
}*/

void AdjMatrix::setEdge(const EdgeInfo& edge)
{
    auto sourceNodeMapping = nodeIndexMapping.find(edge.source);
    auto destinationNodeMapping = nodeIndexMapping.find(edge.destination);
    if (sourceNodeMapping == nodeIndexMapping.end() or destinationNodeMapping == nodeIndexMapping.end())
    {
        return;
    }
    auto& [_, sourceNodeIndex] = *sourceNodeMapping;
    auto& [_, destinationNodeIndex] = *destinationNodeMapping;

    this->matrix[sourceNodeIndex][destinationNodeIndex] = edge.weight.has_value() ? edge.weight.value() : 1;
}

void AdjMatrix::addNodes(uint32_t nodesCount)
{
    resizeMatrixToFitNodes(matrix.size() + nodesCount);
}

void AdjMatrix::removeEdge(const EdgeInfo& edge)
{
    auto sourceNodeMapping = nodeIndexMapping.find(edge.source);
    auto destinationNodeMapping = nodeIndexMapping.find(edge.destination);
    if (sourceNodeMapping == nodeIndexMapping.end() or destinationNodeMapping == nodeIndexMapping.end())
    {
        return;
    }
    auto& [_, sourceNodeIndex] = *sourceNodeMapping;
    auto& [_, destinationNodeIndex] = *destinationNodeMapping;
    matrix[sourceNodeIndex][destinationNodeIndex] = 0;
}

void AdjMatrix::removeNode(NodeId node)
{
    auto nodeItr = nodeIndexMapping.find(node);
    if (nodeItr == nodeIndexMapping.end())
    {
        return;
    }

    auto& [_, nodeIndex] = *nodeItr;
    for (auto& [_, index] : std::ranges::subrange(std::next(nodeItr, 1), nodeIndexMapping.end()))
    {
        if (index > nodeIndex)
        {
            index--;
        }
    }

    matrix.erase(matrix.begin() + nodeIndex);
    for (auto& row : matrix)
    {
        row.erase(row.begin() + nodeIndex);
    }

    nodeIndexMapping.erase(nodeItr);
}

std::string AdjMatrix::show() const
{
    std::stringstream out;
    out << std::format("\nNodes amount = {}\n[\n", matrix.size());
    for (uint32_t i = 0; i < matrix.size(); i++)
    {
        for (uint32_t j = 0; j < matrix[i].size(); j++)
        {
            out << std::format("{}, ", this->matrix[i][j]);
        }
        out << "\n";
    }
    out << "]\n";
    return out.str();
}

/*void AdjMatrix::print_throughtput() {
    std::cout << "[" << std::endl;
    for (uint32_t i = 0; i < this->nodes_amount; i++)
    {
        for (uint32_t j = 0; j < this->nodes_amount; j++)
        {
            std::cout << this->throughtput[i][j] << ", ";
        }
        std::cout << std::endl;
    }
    std::cout << "]" << std::endl;
}*/

uint32_t AdjMatrix::nodesAmount() const
{
    return matrix.size();
}

EdgeInfo AdjMatrix::findEdge(const EdgeInfo& edge) const
{
    auto sourceIterator = nodeIndexMapping.find(edge.source);
    auto destinationIterator = nodeIndexMapping.find(edge.destination);

    if (sourceIterator == nodeIndexMapping.end() or destinationIterator == nodeIndexMapping.end())
    {
        return {edge.source, edge.destination, std::nullopt};
    }
    const auto& [_, sourceIndex] = *sourceIterator;
    const auto& [_, destinationIndex] = *destinationIterator;

    auto weight = matrix[sourceIndex][destinationIndex];
    return {edge.source, edge.destination, weight != 0 ? std::make_optional(weight) : std::nullopt};
}

std::vector<NodeId> AdjMatrix::getNodeIds() const
{
    std::vector<NodeId> nodeIds = {};
    for (const auto& [node, _] : nodeIndexMapping)
    {
        nodeIds.push_back(node);
    }
    return nodeIds;
}

std::vector<NodeId> AdjMatrix::getNeighborsOf(NodeId node) const
{
    auto nodeIndex = nodeIndexMapping.find(node);
    if (nodeIndex == nodeIndexMapping.end())
    {
        return {};
    }

    auto mappingFinder = [this](auto target) {
        return std::ranges::find_if(nodeIndexMapping, [target](const auto& elem) {
            auto& [_, index] = elem;
            return index == target;
        });
    };

    const auto& [_, index] = *nodeIndex;

    std::vector<NodeId> neighbors = {};
    for (uint32_t i = 0; i < matrix[index].size(); i++)
    {
        if (matrix[index][i] != 0)
        {
            const auto& [nodeId, _] = *mappingFinder(i);
            neighbors.push_back(nodeId);
        }
    }
    for (uint32_t i = 0; i < matrix.size(); i++)
    {
        if (matrix[i][index] != 0 and neighbors.end() == std::ranges::find(neighbors, i))
        {
            const auto& [nodeId, _] = *mappingFinder(i);
            neighbors.push_back(nodeId);
        }
    }
    std::ranges::sort(neighbors);
    return neighbors;
}

/*void AdjMatrix::change_to_line_graph() {
    // gather all the edges from the adjacency matrix of initial graph
    std::vector<coord> edges;
    for (uint32_t i = 0; i < this->nodes_amount; i++)
    {
        for (uint32_t j = i; j < this->nodes_amount; j++)
        {
            if (this->matrix[i][j] == 1)
            {
                edges.push_back({i, j});
            }
        }
    }

    // create sufficient matrix for the line graph and fill it with zeros
    uint32_t size = static_cast<uint32_t>(edges.size());
    uint32_t** m = new uint32_t*[size];
    for (uint32_t i = 0; i < size; i++)
    {
        m[i] = new uint32_t[size];
        for (uint32_t j = 0; j < size; j++)
        {
            m[i][j] = 0;
        }
    }

    coord coordinates;
    uint32_t index;

    // fill the line graph
    for (uint32_t i = 0; i < size; i++)
    {
        // load coordinates of the edge
        coordinates = edges[i];

        // search though the matrix for common vertices between edges
        // in the same row of the initial adjacency matrix
        for (uint32_t k = coordinates.x; k < this->nodes_amount; k++)
        {
            // if common vertex was found, calculate the line graph vertex index
            // and mark the connection
            if (this->matrix[coordinates.x][k] == 1 && k != coordinates.y)
            {
                index = find_index(edges, coordinates.x, k);
                m[i][index] = 1;
                m[index][i] = 1;
            }
        }
        // search through the matrix for common vertices between edges
        // in the same column of the initial adjacency matrix
        for (uint32_t k = coordinates.x; k < coordinates.y; k++)
        {
            // if common vertex was found, calculate the line graph vertex index
            // and mark the connection
            if (this->matrix[k][coordinates.y] == 1 && k != coordinates.x)
            {
                index = find_index(edges, k, coordinates.y);
                m[i][index] = 1;
                m[index][i] = 1;
            }
        }
    }

    // free previous matrix memory allocation
    for (uint32_t i = 0; i < this->nodes_amount; i++)
    {
        delete[] this->matrix[i];
    }
    delete[] this->matrix;

    // overwrite the matrix with the pointer to line graph's adjacency matrix
    // and save new size of the graph
    this->matrix = m;
    this->nodes_amount = size;
}*/

/*int32_t AdjMatrix::belman_ford(uint32_t vertex, bool log) {
    // set default values for the distances and previous vertices
    for (uint32_t i = 0; i < this->nodes_amount; i++)
    {
        this->distance[i] = 0x7FFFFFFF;
        this->prev_node[i].clear();
    }

    // set zero as a distance for the starting vertex
    this->distance[vertex] = 0;

    bool change_found;

    auto start = std::chrono::system_clock::now();

    // search through the graph multiple times to find shortest paths
    for (uint32_t i = 0; i < this->nodes_amount; i++)
    {
        // assume change in distance was not found
        change_found = false;
        for (uint32_t x = 0; x < this->nodes_amount; x++)
        {
            for (uint32_t j = 0; j < this->nodes_amount; j++)
            {
                // if change in distance between connected vertices was found, save said
                // information
                if (this->distance[j] > this->distance[x] + this->matrix[x][j] && this->matrix[x][j] != 0)
                {
                    // remember that change was found, to continue search
                    change_found = true;
                    this->distance[j] = this->distance[x] + this->matrix[x][j];
                    this->prev_node[j].clear();
                    this->prev_node[j].push_back(x);
                }
            }
            // if log flag is set, print informations about distance and previous
            // nodes
            if (log)
            {
                // distance information
                std::cout << "Distances: " << std::endl;
                for (uint32_t j = 0; j < this->nodes_amount; j++)
                {
                    std::cout << this->distance[j] << ", ";
                }
                // previous vertices information
                std::cout << std::endl << "Previous: " << std::endl;
                for (uint32_t j = 0; j < this->nodes_amount; j++)
                {
                    for (auto itr = this->prev_node[j].begin(); itr != this->prev_node[j].end(); itr++)
                    {
                        std::cout << *itr << ", ";
                    }
                    std::cout << std::endl;
                }
                std::cout << std::endl;
            }
        }
        // if change was not found, optimize the algorithm by stopping the search
        if (!change_found)
        {
            break;
        }
    }

    // calculate how long the algorithm was running
    auto end = std::chrono::system_clock::now();
    auto diff = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

    // if log flag is set, print the duration
    if (log)
    {
        std::cout << "Duration: " << diff.count() << " us" << std::endl;
    }
    return diff.count();
}*/

/*int32_t AdjMatrix::throughtput_belman_ford(uint32_t searched_throughtput, uint32_t vertex, bool log) {
    // set default values for the distances and previous vertices
    for (uint32_t i = 0; i < this->nodes_amount; i++)
    {
        this->distance[i] = 0x7FFFFFFF;
        this->prev_node[i].clear();
    }

    // set zero as a distance for the starting vertex
    this->distance[vertex] = 0;

    bool change_found;

    auto start = std::chrono::system_clock::now();
    // search through the graph multiple times to find shortest paths
    for (uint32_t i = 0; i < this->nodes_amount; i++)
    {
        // assume change in distance was not found
        change_found = false;
        for (uint32_t x = 0; x < this->nodes_amount; x++)
        {
            for (uint32_t j = 0; j < this->nodes_amount; j++)
            {
                // if change in distance between connected vertices was found while
                // fulfilling the throughtput requirement, save said information
                if (this->distance[j] > this->distance[x] + this->matrix[x][j]
                    && this->throughtput[x][j] >= searched_throughtput)
                {
                    // remember the change was found to continue search
                    change_found = true;
                    this->distance[j] = this->distance[x] + this->matrix[x][j];
                    this->prev_node[j].clear();
                    this->prev_node[j].push_back(x);
                }
                // if a path of the same distance was found, fulfilling the throughtput
                // requirement, and is not already saved, save it in prev_node vector.
                else if (this->distance[j] == this->distance[x] + this->matrix[x][j]
                         && this->throughtput[x][j] >= searched_throughtput
                         && std::find(this->prev_node[j].begin(), this->prev_node[j].end(), x)
                                == this->prev_node[j].end())
                {
                    // remember the change was found to continue search
                    change_found = true;
                    this->prev_node[j].push_back(x);
                }
            }
            // if log flag is set, print informations about distance and previous
            // nodes
            if (log)
            {
                // distance information
                std::cout << "Distances: " << std::endl;
                for (uint32_t j = 0; j < this->nodes_amount; j++)
                {
                    std::cout << this->distance[j] << ", ";
                }
                // previous vertices information
                std::cout << std::endl << "Previous: " << std::endl;
                for (uint32_t j = 0; j < this->nodes_amount; j++)
                {
                    for (auto itr = this->prev_node[j].begin(); itr != this->prev_node[j].end(); itr++)
                    {
                        std::cout << *itr << ", ";
                    }
                    std::cout << std::endl;
                }
                std::cout << std::endl;
            }
        }
        // if change was not found, optimize the algorithm by stopping the search
        if (!change_found)
        {
            break;
        }
    }

    // calculate how long the algorithm was running
    auto end = std::chrono::system_clock::now();
    auto diff = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

    // if log flag is set, print the duration
    if (log)
    {
        std::cout << "Duration: " << diff.count() << " us" << std::endl;
    }

    return diff.count();
}*/

/*uint32_t AdjMatrix::find_index(std::vector<coord>& edges, uint32_t x, uint32_t y) {
    uint32_t size = static_cast<uint32_t>(edges.size());

    uint32_t ret = 0xFFFFFFFF;

    // if coordinates match the source and destination of given edge,
    // it means that this edge is the line graph's vertex that we're looking for
    for (uint32_t i = 0; i < size; i++)
    {
        if (edges[i].x == x && edges[i].y == y)
        {
            ret = i;
        }
    }
    return ret;
}*/
} // namespace Graphs

/*void Data::generate_weighted_graph(std::string file_path, uint32_t nodes_amount) {
    std::ofstream file(file_path);

    srand(time(NULL));

    if (file.good())
    {
        // generate a graph of given size with random weights in the
        // range of <0;n>. Zero means no connection between vertices.
        for (uint32_t i = 0; i < nodes_amount; i++)
        {
            for (uint32_t j = 0; j < nodes_amount; j++)
            {
                if (i != j)
                {
                    file << (rand() % nodes_amount) * (rand() % 6 <= 3 ? 1 : 0);
                }
                else
                {
                    file << 0;
                }
                if (j < nodes_amount - 1)
                {
                    file << " ";
                }
            }
            if (i < nodes_amount - 1)
            {
                file << std::endl;
            }
        }
    }
    file.close();
}

void Data::generate_throughtput(std::string input_file_path, std::string output_file_path, uint32_t nodes_amount) {
    std::ofstream o_file(output_file_path);
    std::ifstream i_file(input_file_path);

    srand(time(NULL));

    uint32_t value;

    if (i_file.good())
    {
        // generate a matrix of throughtput for given graph file
        for (uint32_t i = 0; i < nodes_amount; i++)
        {
            for (uint32_t j = 0; j < nodes_amount; j++)
            {
                i_file >> value;
                if (value != 0)
                {
                    o_file << rand() % 9 + 1;
                }
                else
                {
                    o_file << 0;
                }
                if (j < nodes_amount - 1)
                {
                    o_file << " ";
                }
            }
            if (i < nodes_amount - 1)
            {
                o_file << std::endl;
            }
        }
    }
    o_file.close();
    i_file.close();
}*/

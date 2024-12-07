#include <algorithm>
#include <Graphs/AdjMatrix.hpp>
#include <ranges>
#include <sstream>

namespace Graphs
{
template <GraphDirectionality directionality>
void AdjMatrix<directionality>::resizeMatrixToFitNodes(uint32_t nodesCount)
{
    if (nodesCount > matrix.size())
    {
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
}

template <GraphDirectionality directionality>
GraphDirectionality AdjMatrix<directionality>::getDirectionality() const
{
    return Directionality;
}

template <GraphDirectionality directionality>
AdjMatrix<directionality>::AdjMatrix(const Graph& other)
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

template <GraphDirectionality directionality>
uint32_t AdjMatrix<directionality>::getOutgoingDegree(NodeId nodeId) const
{
    auto nodeMapping = nodeIndexMapping.find(nodeId);
    if (nodeMapping == nodeIndexMapping.end())
    {
        return 0;
    }

    auto [_, nodeIdx] = *nodeMapping;
    uint32_t degree = 0;

    for (auto edge : matrix[nodeIdx])
    {
        if (edge != 0)
        {
            ++degree;
        }
    }
    return degree;
}

template <GraphDirectionality directionality>
uint32_t AdjMatrix<directionality>::getIncommingDegree(NodeId nodeId) const
{
    auto nodeMapping = nodeIndexMapping.find(nodeId);
    if (nodeMapping == nodeIndexMapping.end())
    {
        return 0;
    }

    auto [_, nodeIdx] = *nodeMapping;
    uint32_t degree = 0;

    for (auto row : matrix)
    {
        if (row[nodeIdx] != 0)
        {
            ++degree;
        }
    }

    return degree;
}

template <GraphDirectionality directionality>
void AdjMatrix<directionality>::reset()
{
    matrix.clear();
    nodeIndexMapping.clear();
}

template <GraphDirectionality directionality>
std::vector<EdgeInfo> AdjMatrix<directionality>::getEdges() const
{
    auto findMapping = [this](auto matrixIdx) {
        auto [nodeId, _] = *(std::ranges::find_if(nodeIndexMapping, [matrixIdx](auto elem) {
            auto& [_, index] = elem;
            return index == matrixIdx;
        }));
        return nodeId;
    };

    std::vector<EdgeInfo> edges = {};
    for (uint32_t rowIdx = 0; auto row : matrix)
    {
        for (uint32_t edgeIdx = 0; auto edgeWeight : row)
        {
            if (edgeWeight != 0)
            {
                edges.push_back(
                    {.source = findMapping(rowIdx), .destination = findMapping(edgeIdx), .weight = edgeWeight});
            }
        }
    }
    return edges;
}

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

template <GraphDirectionality directionality>
void AdjMatrix<directionality>::setEdge(const EdgeInfo& edge)
{
    auto sourceNodeMapping = nodeIndexMapping.find(edge.source);
    auto destinationNodeMapping = nodeIndexMapping.find(edge.destination);
    if (sourceNodeMapping == nodeIndexMapping.end() or destinationNodeMapping == nodeIndexMapping.end())
    {
        return;
    }
    auto& [_, sourceNodeIndex] = *sourceNodeMapping;
    auto& [_, destinationNodeIndex] = *destinationNodeMapping;

    this->matrix[sourceNodeIndex][destinationNodeIndex] = edge.weight.value_or(1);

    if constexpr (directionality == GraphDirectionality::undirected)
    {
        this->matrix[destinationNodeIndex][sourceNodeIndex] = edge.weight.value_or(1);
    }
}

template <GraphDirectionality directionality>
void AdjMatrix<directionality>::addNodes(uint32_t nodesCount)
{
    resizeMatrixToFitNodes(matrix.size() + nodesCount);
}

template <GraphDirectionality directionality>
void AdjMatrix<directionality>::removeEdge(const EdgeInfo& edge)
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

    if constexpr (directionality == GraphDirectionality::undirected)
    {
        matrix[destinationNodeIndex][sourceNodeIndex] = 0;
    }
}

template <GraphDirectionality directionality>
void AdjMatrix<directionality>::removeNode(NodeId node)
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

template <GraphDirectionality directionality>
std::string AdjMatrix<directionality>::show() const
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

template <GraphDirectionality directionality>
uint32_t AdjMatrix<directionality>::nodesAmount() const
{
    return matrix.size();
}

template <GraphDirectionality directionality>
EdgeInfo AdjMatrix<directionality>::findEdge(const EdgeInfo& edge) const
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

template <GraphDirectionality directionality>
std::vector<NodeId> AdjMatrix<directionality>::getNodeIds() const
{
    std::vector<NodeId> nodeIds = {};
    for (const auto& [node, _] : nodeIndexMapping)
    {
        nodeIds.push_back(node);
    }
    return nodeIds;
}

template <GraphDirectionality directionality>
std::vector<NodeId> AdjMatrix<directionality>::getOutgoingNeighborsOf(NodeId node) const
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
    std::ranges::sort(neighbors);
    return neighbors;
}

template <GraphDirectionality directionality>
std::vector<NodeId> AdjMatrix<directionality>::getIncommingNeighborsOf(NodeId node) const
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
    for (uint32_t i = 0; i < matrix.size(); i++)
    {
        auto [mapping, _] = *mappingFinder(i);
        if (matrix[i][index] != 0)
        {
            const auto& [nodeId, _] = *mappingFinder(i);
            neighbors.push_back(nodeId);
        }
    }
    std::ranges::sort(neighbors);
    return neighbors;
}

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

} // namespace Graphs

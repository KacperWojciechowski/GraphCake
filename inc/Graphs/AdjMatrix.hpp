#pragma once

#include <cstdint>
#include <Graphs/Graph.hpp>
#include <map>
#include <string>
#include <vector>

/*namespace Data
{
void generate_weighted_graph(std::string file_path, unsigned int nodes_amount);
void generate_throughtput(std::string input_file_path, std::string output_file_path, unsigned int nodes_amount);
}*/ // namespace Data

namespace Graphs
{
class AdjMatrix : public Graph
{
public:
    AdjMatrix() = default;
    AdjMatrix(const Graph&);

    AdjMatrix(AdjMatrix&) = default;
    AdjMatrix(AdjMatrix&&) = default;

    uint32_t nodesAmount() const override;
    uint32_t nodeDegree(NodeId) const override;
    EdgeInfo findEdge(const EdgeInfo&) const override;

    void setEdge(const EdgeInfo&) override;
    void addNodes(uint32_t) override;
    void removeNode(NodeId) override;
    void removeEdge(const EdgeInfo&) override;
    std::vector<NodeId> getNodeIds() const override;
    std::vector<NodeId> getNeighborsOf(NodeId) const override;

    virtual ~AdjMatrix() = default;

private:
    std::string show() const override;
    void buildFromMatFile(const std::string&);
    void buildFromGraphMLFile(const std::string&);
    void resizeMatrixToFitNodes(uint32_t);

    std::map<NodeId, uint32_t> nodeIndexMapping;

    using Row = std::vector<int32_t>;
    std::vector<Row> matrix;
};
} // namespace Graphs

#pragma once

#include <cstdint>
#include <Graphs/Graph.hpp>
#include <map>
#include <string>
#include <vector>

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
    void resizeMatrixToFitNodes(uint32_t);

    using Row = std::vector<WeightType>;

    std::map<NodeId, uint32_t> nodeIndexMapping = {};
    std::vector<Row> matrix = {};
};
} // namespace Graphs

#pragma once

#include <cstdint>
#include <Graphs/Graph.hpp>
#include <map>
#include <string>
#include <vector>

namespace Graphs
{
template <GraphDirectionality directionality>
class AdjMatrix : public Graph
{
public:
    AdjMatrix() = default;
    AdjMatrix(const Graph&);

    AdjMatrix(AdjMatrix&) = default;
    AdjMatrix(AdjMatrix&&) = default;

    uint32_t nodesAmount() const override;
    uint32_t getIncommingDegree(NodeId) const override;
    uint32_t getOutgoingDegree(NodeId) const override;
    EdgeInfo findEdge(const EdgeInfo&) const override;

    void setEdge(const EdgeInfo&) override;
    void addNodes(uint32_t) override;
    void removeNode(NodeId) override;
    void removeEdge(const EdgeInfo&) override;
    std::vector<NodeId> getNodeIds() const override;
    std::vector<NodeId> getOutgoingNeighborsOf(NodeId) const override;
    std::vector<NodeId> getIncommingNeighborsOf(NodeId) const override;
    std::vector<EdgeInfo> getEdges() const override;

    void reset() override;

    GraphDirectionality getDirectionality() const override;

    virtual ~AdjMatrix() = default;

private:
    std::string show() const override;
    void resizeMatrixToFitNodes(uint32_t);

    using Row = std::vector<WeightType>;

    std::map<NodeId, uint32_t> nodeIndexMapping = {};
    std::vector<Row> matrix = {};
};

template class AdjMatrix<GraphDirectionality::undirected>;
template class AdjMatrix<GraphDirectionality::directed>;
} // namespace Graphs

#pragma once

#include <Graphs/Graph.hpp>
#include <map>
#include <string>

namespace Graphs
{
template <GraphDirectionality directionality>
class AdjList : public Graph
{
public:
    AdjList() = default;
    AdjList(const Graph&);
    AdjList(AdjList&) = default;
    AdjList(AdjList&&) = default;

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

    virtual ~AdjList() = default;

    static constexpr GraphDirectionality Directionality = directionality;

private:
    std::string show() const override;

    using Neighbors = std::vector<EdgeInfo>;

    void removeNeighborFromRange(Neighbors&, EdgeInfo);
    void addNeighborAndSortRange(Neighbors&, EdgeInfo);

    std::vector<Neighbors> nodes;
    std::map<NodeId, uint32_t> nodeMap;
};
template class AdjList<GraphDirectionality::undirected>;
template class AdjList<GraphDirectionality::directed>;
} // namespace Graphs

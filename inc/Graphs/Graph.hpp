#pragma once

#include <cstdint>
#include <iostream>
#include <optional>
#include <vector>

namespace Graphs
{
using NodeId = uint32_t;

struct EdgeInfo
{
    NodeId source;
    NodeId destination;
    std::optional<int32_t> weight;
};

class GraphReader
{
public:
    friend std::ostream& operator<<(std::ostream& os, const GraphReader& g)
    {
        os << g.show();
        return os;
    }
    virtual EdgeInfo findEdge(const EdgeInfo&) const = 0;
    virtual uint32_t nodesAmount() const = 0;
    virtual uint32_t nodeDegree(NodeId) const = 0;
    virtual std::vector<NodeId> getNodeIds() const = 0;
    virtual std::vector<NodeId> getNeighborsOf(NodeId) const = 0;

    virtual ~GraphReader() = default;

protected:
    virtual std::string show() const = 0;
};

class GraphWriter
{
public:
    virtual void setEdge(const EdgeInfo&) = 0;
    virtual void setEdges(const std::vector<EdgeInfo>& edges)
    {
        for (const auto& edge : edges)
        {
            setEdge(edge);
        }
    }
    virtual void addNodes(uint32_t) = 0;
    virtual void removeNode(NodeId) = 0;
    virtual void removeEdge(const EdgeInfo&) = 0;
};

class Graph : public GraphReader,
              public GraphWriter
{
public:
    virtual ~Graph() = default;
};
} // namespace Graphs

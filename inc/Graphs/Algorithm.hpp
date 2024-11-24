#pragma once

#include <Graphs/Graph.hpp>

namespace Graphs::Algorithm
{
class AlgorithmFunctor
{
public:
    virtual std::string getName() = 0;
    virtual void operator()(const Graphs::Graph&) = 0;
    virtual ~AlgorithmFunctor() = default;
};

using AlgorithmsCollection = std::vector<AlgorithmFunctor*>;
using Permutation = std::vector<NodeId>;

template <bool isVerbose, class T>
using Verbose = typename std::enable_if_t<isVerbose, T>*;

template <bool isVerbose, class T>
using NotVerbose = Verbose<not isVerbose, T>;

constexpr bool verbose = true;
constexpr bool notVerbose = false;
} // namespace Graphs::Algorithm

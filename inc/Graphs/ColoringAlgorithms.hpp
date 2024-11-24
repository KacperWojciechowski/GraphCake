#pragma once

#include <cstdint>
#include <Graphs/Algorithm.hpp>
#include <iosfwd>
#include <memory>
#include <tuple>
#include <vector>

namespace Graphs::Algorithm
{
using ColorId = uint32_t;
using ColoringInfo = std::pair<NodeId, ColorId>;
using ColoringVector = std::vector<ColoringInfo>;
using ColoringResult = std::tuple<ColorId, ColoringVector>;

template <bool isVerbose>
class GreedyColoring : public AlgorithmFunctor
{
public:
    explicit GreedyColoring(std::shared_ptr<ColoringResult> resultContainer, std::ostream& out = std::cout);

    GreedyColoring() = delete;
    GreedyColoring(const GreedyColoring&) = delete;
    GreedyColoring(GreedyColoring&&) = delete;

    GreedyColoring& operator=(const GreedyColoring&) = delete;
    GreedyColoring& operator=(GreedyColoring&&) = delete;

    void operator()(const Graphs::Graph&) override;

private:
    template <class... Args>
    void log(std::string, Args...) const;

    std::shared_ptr<ColoringResult> result = {};
    [[maybe_unused]] std::ostream& outStream;
};
} // namespace Graphs::Algorithm

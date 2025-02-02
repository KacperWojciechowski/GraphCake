#pragma once

#include <cstdint>
#include <functional>
#include <Graphs/Algorithm.hpp>
#include <Graphs/Graph.hpp>
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

    std::string getName() override;

    void operator()(const Graphs::Graph&) override;

private:
    std::shared_ptr<ColoringResult> result = {};
    [[maybe_unused]] std::unique_ptr<std::ostream, std::function<void(std::ostream*)>> outStream = {};
};

template <bool isVerbose>
class LfColoring : public AlgorithmFunctor
{
public:
    explicit LfColoring(std::shared_ptr<ColoringResult> resultContainer, std::ostream& out = std::cout);

    LfColoring() = delete;
    LfColoring(const LfColoring&) = delete;
    LfColoring(LfColoring&&) = delete;

    LfColoring& operator=(const LfColoring&) = delete;
    LfColoring& operator=(LfColoring&&) = delete;

    std::string getName() override;

    void operator()(const Graphs::Graph&) override;

private:
    std::shared_ptr<ColoringResult> result = {};
    [[maybe_unused]] std::unique_ptr<std::ostream, std::function<void(std::ostream*)>> outStream = {};
};
} // namespace Graphs::Algorithm

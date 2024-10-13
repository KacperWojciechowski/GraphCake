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
    template <class T = void, Verbose<isVerbose, T> = nullptr>
    GreedyColoring(std::shared_ptr<ColoringResult> resultContainer, std::ostream& out = std::cout)
        : result(std::move(resultContainer)), outStream{out}
    {
        if (not result)
        {
            log("Coloring result cannot be null");
            throw std::invalid_argument{"Coloring result cannot be null"};
        }
    }

    template <class T = void, NotVerbose<isVerbose, T> = nullptr>
    GreedyColoring(std::shared_ptr<ColoringResult> resultContainer)
        : result(std::move(resultContainer)), outStream(std::cout /*unused*/)
    {
        if (not result)
        {
            throw std::invalid_argument{"Coloring result cannot be null"};
        }
    }

    void operator()(const Graphs::Graph&) override;

private:
    template <class... Args, class T = void, Verbose<isVerbose, T> = nullptr>
    void log(std::string, Args...) const;

    std::shared_ptr<ColoringResult> result = {};
    std::ostream& outStream;
};
} // namespace Graphs::Algorithm

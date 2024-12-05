#pragma once

#include <iosfwd>
#include <type_traits>

namespace Graphs
{
class Graph;

class Serializer
{
public:
    static void serializeLstFile(std::ostream&, const Graph&);
    static void serializeMatFile(std::ostream&, const Graph&);
    static void serializeGraphMlFile(std::ostream&, const Graph&);
};
} // namespace Graphs

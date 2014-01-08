#ifndef __BOOSTGRAPH2OGDFCONVERTER_HPP__
#define __BOOSTGRAPH2OGDFCONVERTER_HPP__

#include <utility>
#include <memory>

namespace ogdf
{
	class Graph;
	class GraphAttributes;
}

class DependencyGraph;

class BoostGraph2OGDFConverter
{
public:
	using up_graph = std::unique_ptr<ogdf::Graph>;
	using up_graph_attributes = std::unique_ptr<ogdf::GraphAttributes>;
	using type_result = std::pair<up_graph, up_graph_attributes>;

	static type_result convert(const DependencyGraph& graph);
};

#endif
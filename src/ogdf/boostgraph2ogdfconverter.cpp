#include "boostgraph2ogdfconverter.hpp"

#include <ogdf/basic/Graph.h>
#include <ogdf/basic/GraphAttributes.h>

#include <dependencygraph.hpp>

BoostGraph2OGDFConverter::type_result BoostGraph2OGDFConverter::convert(const DependencyGraph& graph)
{
	up_graph G { new ogdf::Graph };
	up_graph_attributes GA { new ogdf::GraphAttributes(*G,
		ogdf::GraphAttributes::nodeGraphics | ogdf::GraphAttributes::edgeGraphics |
		ogdf::GraphAttributes::nodeLabel | ogdf::GraphAttributes::nodeColor | 
		ogdf::GraphAttributes::edgeColor | ogdf::GraphAttributes::edgeStyle | 
		ogdf::GraphAttributes::nodeStyle | ogdf::GraphAttributes::nodeTemplate) };

	auto vertices = graph.vertices();

	std::map<DependencyGraph::vertex_descriptor, ogdf::node> m_vertexMap;

	std::for_each(vertices.first, vertices.second, [&G, &GA, &graph, &m_vertexMap](DependencyGraph::vertex_descriptor v)
	{
		ogdf::node n = G->newNode();
		GA->labelNode(n) = graph(v)->filePath().c_str();
		m_vertexMap.insert(std::make_pair(v, n));
	});

	auto edges = graph.edges();
	std::for_each(edges.first, edges.second, [&G, &graph, &m_vertexMap](DependencyGraph::edge_descriptor e)
	{
		auto it = m_vertexMap.find(graph.source(e));
		assert(it != m_vertexMap.end());
		ogdf::node from = it->second;

		it = m_vertexMap.find(graph.target(e));
		assert(it != m_vertexMap.end());
		ogdf::node to = it->second;

		G->newEdge(from, to);
	});

	ogdf::node v;
	forall_nodes(v, *G)
	{
		GA->height(v) = 30.0;
		GA->width(v) = 7 * GA->labelNode(v).length();
	}

	return {std::move(G), std::move(GA)};
}
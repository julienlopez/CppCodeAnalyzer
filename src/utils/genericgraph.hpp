#ifndef __GENERICGRAPH_HPP__
#define __GENERICGRAPH_HPP__

#include <boost/graph/adjacency_list.hpp>

#include <boost/graph/graphviz.hpp>
#include <boost/graph/breadth_first_search.hpp>

template <class NODE>
class GenericGraph
{
public:
	using type_node = NODE;
	using Graph = typename boost::adjacency_list<boost::listS, boost::vecS, boost::bidirectionalS, type_node>;

	using vertex_descriptor = typename boost::graph_traits<Graph>::vertex_descriptor;
	using vertex_iterator = typename boost::graph_traits<Graph>::vertex_iterator;
	using type_pair_vertex_iterator = std::pair<vertex_iterator, vertex_iterator>;

	using edge_descriptor = typename boost::graph_traits<Graph>::edge_descriptor;
	using edge_iterator = typename boost::graph_traits<Graph>::edge_iterator;
	using type_pair_edge_iterator = std::pair<edge_iterator, edge_iterator>;

	GenericGraph() = default;

	template<class LABEL_WRITER>
	void print(std::ostream& o, LABEL_WRITER writer) const
	{
	    boost::write_graphviz(o, d_graph, writer);
	}

	vertex_descriptor addNode(type_node node)
	{
		auto v = add_vertex(d_graph);
		d_graph[v] = std::move(node);
		return v;
	}

	edge_descriptor addEdge(vertex_descriptor from, vertex_descriptor to)
	{
		return add_edge(from, to, d_graph).first;
	}

	const type_node& operator()(vertex_descriptor v) const
	{
		return d_graph[v];
	}

	type_pair_vertex_iterator vertices() const
	{
		return boost::vertices(d_graph);
	}

	std::vector<vertex_descriptor> parents(vertex_descriptor v) const
	{
		std::vector<vertex_descriptor> res;
		in_edge_iterator i, i_end;
		std::tie(i, i_end) = in_edges(v, d_graph);
		for(; i != i_end; ++i)
		{
			res.push_back(boost::source(*i, d_graph));
		}
		return res;
	}

	bool areLinked(vertex_descriptor v1, vertex_descriptor v2) const
	{
		bool found = false;
		custom_bfs_visitor<GenericGraph> vis(v2, found);
	  	breadth_first_search(d_graph, v1, visitor(vis));
	  	return found;
	}

	std::size_t countVertices() const
	{
		auto its = vertices();
		return std::distance(its.first, its.second);
	}

	std::size_t countEdges() const
	{
		auto its = edges();
		return std::distance(its.first, its.second);
	}

	type_pair_edge_iterator edges() const
	{
		return boost::edges(d_graph);
	}

	vertex_descriptor source(edge_descriptor e) const
	{
		return boost::source(e, d_graph);
	}

	vertex_descriptor target(edge_descriptor e) const
	{
		return boost::target(e, d_graph);
	}

private:

	using in_edge_iterator = typename boost::graph_traits<Graph>::in_edge_iterator;

	Graph d_graph;

	template<class GRAPH>
	class custom_bfs_visitor : public boost::default_bfs_visitor
	{
	public:
		typedef typename GRAPH::vertex_descriptor vertex_descriptor;
		custom_bfs_visitor(vertex_descriptor toFind, bool& found): m_toFind(toFind), m_found(found)
		{}
	 
		template<typename Vertex, typename Graph>
		void discover_vertex(Vertex u, const Graph&) const
	 	{
	    	if(u == m_toFind) m_found = true;
		}

	private:
		vertex_descriptor m_toFind;
		bool& m_found;
	};

};

#endif
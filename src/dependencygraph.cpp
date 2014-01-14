#include "dependencygraph.hpp"

#include <fstream>

#include <boost/filesystem.hpp>

#include <boost/graph/graphviz.hpp>
#include <boost/graph/breadth_first_search.hpp>

#include <file/filefactory.hpp>

#include <utils/stringhelper.hpp>

void DependencyGraph::print(std::ostream& o) const
{
    boost::write_graphviz(o, d_graph, label_writer(*this));
}

DependencyGraph::vertex_descriptor DependencyGraph::addNode(const std::string& node)
{
    auto its = vertices();
    auto i_v = std::find_if(its.first, its.second, [&node, this](DependencyGraph::vertex_descriptor v){ return StringHelper::endsWith(d_graph[v]->filePath(), node); });
	
	if(i_v != its.second) 
	return *i_v;

	auto v = add_vertex(d_graph);
	d_graph[v] = FileFactory::createFile(boost::filesystem::exists(node)?"Modifiable":"External", node);
	return v;
}

DependencyGraph::edge_descriptor DependencyGraph::addEdge(const std::string& from, const std::string& to)
{
	auto v1 = addNode(from);
	auto v2 = addNode(to);
	
	return add_edge(v1, v2, d_graph).first;

}

const DependencyGraph::type_node& DependencyGraph::operator()(vertex_descriptor v) const
{
	return d_graph[v];
}

DependencyGraph::type_pair_vertex_iterator DependencyGraph::vertices() const
{
	return boost::vertices(d_graph);
}

std::list<DependencyGraph::vertex_descriptor> DependencyGraph::parents(vertex_descriptor v) const
{
	std::list<DependencyGraph::vertex_descriptor> res;
	in_edge_iterator i, i_end;
	std::tie(i, i_end) = in_edges(v, d_graph);
	for(; i != i_end; ++i)
	{
		res.push_back(boost::source(*i, d_graph));
	}
	return res;
}

namespace {
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
}

bool DependencyGraph::areLinked(vertex_descriptor v1, vertex_descriptor v2) const
{
	bool found = false;
	custom_bfs_visitor<DependencyGraph> vis(v2, found);
  	breadth_first_search(d_graph, v1, visitor(vis));
  	return found;
}

std::size_t DependencyGraph::countVertices() const
{
	auto its = vertices();
	return std::distance(its.first, its.second);
}

std::size_t DependencyGraph::countEdges() const
{
	auto its = edges();
	return std::distance(its.first, its.second);
}

DependencyGraph::type_pair_edge_iterator DependencyGraph::edges() const
{
	return boost::edges(d_graph);
}

DependencyGraph::vertex_descriptor DependencyGraph::source(edge_descriptor e) const
{
	return boost::source(e, d_graph);
}

DependencyGraph::vertex_descriptor DependencyGraph::target(edge_descriptor e) const
{
	return boost::target(e, d_graph);
}

DependencyGraph::label_writer::label_writer(const DependencyGraph & sr) : d_sr(sr) {
}
void DependencyGraph::label_writer::operator()(std::ostream& out, const boost::graph_traits<Graph>::vertex_descriptor& v) const {
	out << "[label=\"" << createLabel(v) << "\"]";
}

std::string DependencyGraph::label_writer::createLabel(const boost::graph_traits<Graph>::vertex_descriptor& v) const {
	return d_sr.d_graph[v]->filePath();
}

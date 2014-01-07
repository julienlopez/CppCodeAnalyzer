#include "dependencygraph.hpp"

#include <fstream>
#include <boost/graph/graphviz.hpp>
#include <boost/graph/breadth_first_search.hpp>

DependencyGraph::DependencyGraph()
{}

void DependencyGraph::print(std::ostream& o) const {
    boost::write_graphviz(o, d_reseau, label_writer(*this));
}

void DependencyGraph::addNoeud(const DependencyGraph::type_node& node) {
    boost::graph_traits<Reseau>::vertex_descriptor v;
    type_map_vertex_descriptor::iterator i_v;
    i_v = d_mid2vertex.find(node);
	if(i_v ==d_mid2vertex.end()) {
		v = add_vertex(d_reseau);
		d_reseau[v] = node;
		d_mid2vertex[node] = v;
	}
}

void DependencyGraph::addLien(const DependencyGraph::type_node& from, const DependencyGraph::type_node& to) {
	addNoeud(from);
	addNoeud(to);
	type_map_vertex_descriptor::iterator ifrom = d_mid2vertex.find(from);
	if(ifrom != d_mid2vertex.end()) {
		type_map_vertex_descriptor::iterator ito = d_mid2vertex.find(to);
		if(ito!=d_mid2vertex.end()) {
		    if(d_link.find(std::make_pair(from,to))==d_link.end()) {
		    	boost::graph_traits<Reseau>::vertex_descriptor v1 = ifrom->second;
		    	boost::graph_traits<Reseau>::vertex_descriptor v2 = ito->second;
		    	add_edge(v1,v2,d_reseau);
		    	d_link.insert(std::make_pair(from,to));
		    }
		    return;
		}
	}
}

const DependencyGraph::type_node& DependencyGraph::operator()(vertex_descriptor v) const
{
	return d_reseau[v];
}

DependencyGraph::type_pair_vertex_iterator DependencyGraph::vertices() const
{
	return boost::vertices(d_reseau);
}

std::list<DependencyGraph::vertex_descriptor> DependencyGraph::parents(vertex_descriptor v) const
{
	std::list<DependencyGraph::vertex_descriptor> res;
	in_edge_iterator i, i_end;
	std::tie(i, i_end) = in_edges(v, d_reseau);
	for(; i != i_end; ++i)
	{
		res.push_back(boost::source(*i, d_reseau));
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
  	breadth_first_search(d_reseau, v1, visitor(vis));
  	return found;
}

std::size_t DependencyGraph::count() const
{
	auto its = vertices();
	return std::distance(its.first, its.second);
}

DependencyGraph::label_writer::label_writer(const DependencyGraph & sr) : d_sr(sr) {
}
void DependencyGraph::label_writer::operator()(std::ostream& out, const boost::graph_traits<Reseau>::vertex_descriptor& v) const {
	out << "[label=\"" << createLabel(v) << "\"]";
}

std::string DependencyGraph::label_writer::createLabel(const boost::graph_traits<Reseau>::vertex_descriptor& v) const {
	return d_sr.d_reseau[v].generic_string();
}

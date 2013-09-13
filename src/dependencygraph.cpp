#include "dependencygraph.hpp"

#include <fstream>
#include <boost/graph/graphviz.hpp>
#include <boost/graph/breadth_first_search.hpp>

DependencyGraph::DependencyGraph()
{}

void DependencyGraph::print(std::ostream& o) const {
    boost::write_graphviz(o, d_reseau, label_writer(*this));
}

void DependencyGraph::addNoeud(const std::string& noeud) {
    boost::graph_traits<Reseau>::vertex_descriptor v;
    type_map_vertex_descriptor::iterator i_v;
    i_v = d_mid2vertex.find(noeud);
	if(i_v ==d_mid2vertex.end()) {
		v = add_vertex(d_reseau);
		d_reseau[v] = noeud;
		d_mid2vertex[noeud] = v;
	}
}

void DependencyGraph::addLien(const std::string& depart, const std::string& arrivee) {
	addNoeud(depart);
	addNoeud(arrivee);
	type_map_vertex_descriptor::iterator ifrom = d_mid2vertex.find(depart);
	if(ifrom != d_mid2vertex.end()) {
		type_map_vertex_descriptor::iterator ito = d_mid2vertex.find(arrivee);
		if(ito!=d_mid2vertex.end()) {
		    if(d_link.find(std::make_pair(depart,arrivee))==d_link.end()) {
		    	boost::graph_traits<Reseau>::vertex_descriptor v1 = ifrom->second;
		    	boost::graph_traits<Reseau>::vertex_descriptor v2 = ito->second;
		    	add_edge(v1,v2,d_reseau);
		    	d_link.insert(std::make_pair(depart,arrivee));
		    }
		    return;
		}
	}
}

std::string DependencyGraph::operator()(vertex_descriptor v) const
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

DependencyGraph::label_writer::label_writer(const DependencyGraph & sr) : d_sr(sr) {
}
void DependencyGraph::label_writer::operator()(std::ostream& out, const boost::graph_traits<Reseau>::vertex_descriptor& v) const {
	out << "[label=\"" << createLabel(v) << "\"";

/*
	if(attr.type() == OPERATION) {
		out << " shape=diamond";
	}
	std::string color = nodeType2color(attr.type());
	if(!color.empty()) {
		out << " fillcolor=\"" << color << "\" style=filled";
	}
*/

	out << "]";
}

std::string DependencyGraph::label_writer::createLabel(const boost::graph_traits<Reseau>::vertex_descriptor& v) const {
	return d_sr.d_reseau[v];;
}

//void DependencyGraph::remplaceVertex(vertex_iterator i) {
//	typedef boost::graph_traits<Reseau>::adjacency_iterator adj_iter;
//	adj_iter it_aval, it_begin, it_end;
//	boost::tie(it_begin, it_end) = adjacent_vertices(*i, d_reseau);
//	it_aval = it_begin;
//	assert((++it_begin) == it_end);
//
//	typedef Reseau::inv_adjacency_iterator inv_adj_iter;
//	inv_adj_iter it_amont, it_begin2, it_end2;
//	boost::tie(it_begin2, it_end2) = inv_adjacent_vertices(*i, d_reseau);
//	it_amont = it_begin2;
//	assert((++it_begin2) == it_end2);
//
//	add_edge(*it_amont, *it_aval, d_reseau);
//	d_link.insert(std::make_pair(d_reseau[*it_amont], d_reseau[*it_aval]));
//
//	std::string str = d_reseau[*i];
//	boost::clear_vertex(*i, d_reseau);
//	boost::remove_vertex(*i, d_reseau);
//	d_mid2vertex.erase(str);
//}

#ifndef __DEPENDENCYGRAPH_HPP__
#define __DEPENDENCYGRAPH_HPP__

#include <string>
#include <boost/graph/adjacency_list.hpp>
#include <boost/filesystem/path.hpp>

class DependencyGraph
{
public:
  using type_noeud = boost::filesystem::path;
  using Reseau = boost::adjacency_list<boost::listS, boost::vecS, boost::bidirectionalS, type_noeud>;
  using vertex_descriptor = boost::graph_traits<Reseau>::vertex_descriptor;
  using type_map_vertex_descriptor = std::map<type_noeud, vertex_descriptor>;
  using vertex_iterator = boost::graph_traits<Reseau>::vertex_iterator;
  using type_pair_vertex_iterator = std::pair<vertex_iterator, vertex_iterator>;

	DependencyGraph();

	void print(std::ostream& o) const;

  void addNoeud(const type_noeud& noeud);

  void addLien(const type_noeud& depart, const type_noeud& arrivee);

  const type_noeud& operator()(vertex_descriptor v) const;

  type_pair_vertex_iterator vertices() const;

  std::list<vertex_descriptor> parents(vertex_descriptor v) const;

  bool areLinked(vertex_descriptor v1, vertex_descriptor v2) const;

private:

  using in_edge_iterator = boost::graph_traits<Reseau>::in_edge_iterator;

  Reseau d_reseau;
  type_map_vertex_descriptor d_mid2vertex;
  class label_writer 
  {
    public:
			label_writer(const DependencyGraph & sr);
			void operator()(std::ostream& out, const boost::graph_traits<Reseau>::vertex_descriptor& v) const;

       private:
			const DependencyGraph &d_sr;
			std::string createLabel(const boost::graph_traits<Reseau>::vertex_descriptor& v) const;
  };

  using type_linkPost = std::set<std::pair<type_noeud, type_noeud>>;
  type_linkPost d_link;
};

#endif // __DEPENDENCYGRAPH_HPP__

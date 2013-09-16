#ifndef __DEPENDENCYGRAPH_HPP__
#define __DEPENDENCYGRAPH_HPP__

#include <string>
#include <boost/graph/adjacency_list.hpp>

class DependencyGraph
{
public:
  typedef std::string Noeud;
  typedef boost::adjacency_list<boost::listS, boost::vecS, boost::bidirectionalS, std::string> Reseau;
  typedef boost::graph_traits<Reseau>::vertex_descriptor vertex_descriptor;
  typedef std::map<std::string, vertex_descriptor> type_map_vertex_descriptor;
  typedef boost::graph_traits<Reseau>::vertex_iterator vertex_iterator;
  typedef std::pair<vertex_iterator, vertex_iterator> type_pair_vertex_iterator;

	DependencyGraph();

	void print(std::ostream& o) const;

  void addNoeud(const std::string& noeud);

  void addLien(const std::string& depart, const std::string& arrivee);

  std::string operator()(vertex_descriptor v) const;

  type_pair_vertex_iterator vertices() const;

  std::list<vertex_descriptor> parents(vertex_descriptor v) const;

  bool areLinked(vertex_descriptor v1, vertex_descriptor v2) const;

private:

  typedef boost::graph_traits<Reseau>::in_edge_iterator in_edge_iterator;

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

  typedef std::set<std::pair<std::string, std::string> > type_linkPost;
  type_linkPost d_link;

};

#endif // __DEPENDENCYGRAPH_HPP__

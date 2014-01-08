#ifndef __DEPENDENCYGRAPH_HPP__
#define __DEPENDENCYGRAPH_HPP__

#include <string>
#include <boost/graph/adjacency_list.hpp>
#include <boost/filesystem/path.hpp>

class DependencyGraph
{
public:
  using type_node = boost::filesystem::path;
  using Reseau = boost::adjacency_list<boost::listS, boost::vecS, boost::bidirectionalS, type_node>;

  using vertex_descriptor = boost::graph_traits<Reseau>::vertex_descriptor;
  using vertex_iterator = boost::graph_traits<Reseau>::vertex_iterator;
  using type_pair_vertex_iterator = std::pair<vertex_iterator, vertex_iterator>;
  using type_map_vertex_descriptor = std::map<type_node, vertex_descriptor>;

  using edge_descriptor = boost::graph_traits<Reseau>::edge_descriptor;
  using edge_iterator = boost::graph_traits<Reseau>::edge_iterator;
  using type_pair_edge_iterator = std::pair<edge_iterator, edge_iterator>;

	DependencyGraph();

	void print(std::ostream& o) const;

  void addNoeud(const type_node& node);

  void addLien(const type_node& from, const type_node& to);

  const type_node& operator()(vertex_descriptor v) const;

  type_pair_vertex_iterator vertices() const;

  std::list<vertex_descriptor> parents(vertex_descriptor v) const;

  bool areLinked(vertex_descriptor v1, vertex_descriptor v2) const;

  std::size_t countVertices() const;

  std::size_t countEdges() const;

  type_pair_edge_iterator edges() const;

  vertex_descriptor source(edge_descriptor e) const;

  vertex_descriptor target(edge_descriptor e) const;

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

  using type_linkPost = std::set<std::pair<type_node, type_node>>;
  type_linkPost d_link;
};

#endif // __DEPENDENCYGRAPH_HPP__

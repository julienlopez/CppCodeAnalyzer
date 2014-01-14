#ifndef __DEPENDENCYGRAPH_HPP__
#define __DEPENDENCYGRAPH_HPP__

#include <string>
#include <memory>

#include <boost/graph/adjacency_list.hpp>

#include <file/ifile.hpp>

class DependencyGraph
{
public:
  using type_node = std::unique_ptr<iFile>;
  using Reseau = boost::adjacency_list<boost::listS, boost::vecS, boost::bidirectionalS, type_node>;

  using vertex_descriptor = boost::graph_traits<Reseau>::vertex_descriptor;
  using vertex_iterator = boost::graph_traits<Reseau>::vertex_iterator;
  using type_pair_vertex_iterator = std::pair<vertex_iterator, vertex_iterator>;

  using edge_descriptor = boost::graph_traits<Reseau>::edge_descriptor;
  using edge_iterator = boost::graph_traits<Reseau>::edge_iterator;
  using type_pair_edge_iterator = std::pair<edge_iterator, edge_iterator>;

	DependencyGraph();

	void print(std::ostream& o) const;

  vertex_descriptor addNoeud(const std::string& node);

  edge_descriptor addLien(const std::string& from, const std::string& to);

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

  class label_writer 
  {
    public:
			label_writer(const DependencyGraph & sr);
			void operator()(std::ostream& out, const boost::graph_traits<Reseau>::vertex_descriptor& v) const;

       private:
			const DependencyGraph &d_sr;
			std::string createLabel(const boost::graph_traits<Reseau>::vertex_descriptor& v) const;
  };
};

#endif // __DEPENDENCYGRAPH_HPP__

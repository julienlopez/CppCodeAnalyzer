#ifndef __DEPENDENCYGRAPH_HPP__
#define __DEPENDENCYGRAPH_HPP__

#include <memory>

#include <file/ifile.hpp>

#include <utils/genericgraph.hpp>

class DependencyGraph : public GenericGraph<std::unique_ptr<iFile>>
{
	using Base = GenericGraph<std::unique_ptr<iFile>>;

public:
	DependencyGraph() = default;

	void print(std::ostream& o) const;

	vertex_descriptor addNode(const std::string& node);

	edge_descriptor addEdge(const std::string& from, const std::string& to);

private:

	class label_writer 
	{
		public:
		label_writer(const DependencyGraph & sr);
		void operator()(std::ostream& out, const boost::graph_traits<Graph>::vertex_descriptor& v) const;

		private:
		const DependencyGraph &d_sr;
		std::string createLabel(const boost::graph_traits<Graph>::vertex_descriptor& v) const;
	};
};

#endif // __DEPENDENCYGRAPH_HPP__

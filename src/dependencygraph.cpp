#include "dependencygraph.hpp"

#include <boost/filesystem.hpp>

#include <file/filefactory.hpp>

#include <utils/stringhelper.hpp>

void DependencyGraph::print(std::ostream& o) const
{
    Base::print(o, label_writer(*this));
}

DependencyGraph::vertex_descriptor DependencyGraph::addNode(const std::string& node)
{
    auto its = vertices();
    auto i_v = std::find_if(its.first, its.second, [&node, this](DependencyGraph::vertex_descriptor v){ 
    	return StringHelper::endsWith((*this)(v)->filePath(), node); 
    });
	
	if(i_v != its.second) 
	return *i_v;

	return Base::addNode(FileFactory::createFile(boost::filesystem::exists(node)?"Modifiable":"External", node));
}

DependencyGraph::edge_descriptor DependencyGraph::addEdge(const std::string& from, const std::string& to)
{
	auto n1 = addNode(from);
	auto n2 = addNode(to);
	return Base::addEdge(n1, n2);
}

DependencyGraph::label_writer::label_writer(const DependencyGraph & sr) : d_sr(sr) {
}
void DependencyGraph::label_writer::operator()(std::ostream& out, const boost::graph_traits<Graph>::vertex_descriptor& v) const {
	out << "[label=\"" << createLabel(v) << "\"]";
}

std::string DependencyGraph::label_writer::createLabel(const boost::graph_traits<Graph>::vertex_descriptor& v) const {
	return d_sr(v)->filePath();
}

#ifndef __GRAPHCONSTRUCTOR_HPP__
#define __GRAPHCONSTRUCTOR_HPP__

#include "stringhelper.hpp"

#include <stdexcept>
#include <boost/filesystem.hpp>

class DependencyGraph;

class GraphConstructor
{
	public:
		static StringHelper::type_vector_string s_extensions;

		static void buildGraph(DependencyGraph& graph, const boost::filesystem::path& p) throw(std::exception);

	private:
		static void browseDirectory(DependencyGraph& graph, const boost::filesystem::path& p) throw(std::exception);
		static void analyseFile(DependencyGraph& graph, const boost::filesystem::path& p) throw(std::exception);
};

#endif // __GRAPHCONSTRUCTOR_HPP__

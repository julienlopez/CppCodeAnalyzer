#ifndef __PACKAGEDEPENDENCYBUILDER_HPP__
#define __PACKAGEDEPENDENCYBUILDER_HPP__

#include <string>

// #includ <boost/filesystem.hpp>

class DependencyGraph;
class PackageDependencyGraph;

namespace boost {
	namespace filesystem {
		class path;
	}
}

class PackageDependencyBuilder
{
	public:
		static void buildGraph(PackageDependencyGraph& result, const DependencyGraph& graph, const boost::filesystem::path& path);

	private:
		static std::string formatPathToPackage(std::string str, const boost::filesystem::path& path);

		static bool isHeader(const std::string& path);
		static bool isSource(const std::string& path);

		static std::string removeExtension(std::string str);
};

#endif
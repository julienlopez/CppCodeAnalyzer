#include "packagedependencybuilder.hpp"
#include <dependencygraph.hpp>
#include "packagedependencygraph.hpp"

#include <utils/stringhelper.hpp>

#include <boost/filesystem.hpp>

void PackageDependencyBuilder::buildGraph(PackageDependencyGraph& result, const DependencyGraph& graph, const boost::filesystem::path& path)
{
	auto p = graph.vertices();
	for(auto it1 = p.first; it1 != p.second; ++it1)
	{
		auto it2 = it1;
		++it2;
		for(; it2 != p.second; ++it2)
		{
			std::string str1 = formatPathToPackage(graph(*it1)->filePath(), boost::filesystem::absolute(path));
			std::string str2 = formatPathToPackage(graph(*it2)->filePath(), boost::filesystem::absolute(path));
			if(removeExtension(str1) == removeExtension(str2) && isHeader(str1)^isHeader(str2) && isSource(str1)^isSource(str2))
			{
				result.addNode(removeExtension(str1));
			}
		}
	}
}

std::string PackageDependencyBuilder::formatPathToPackage(std::string str, const boost::filesystem::path& path)
{
	if(StringHelper::startsWith(str, path.generic_string()))
	{
		str.erase(0, path.generic_string().size());
		if(!str.empty() && str[0] == '/')
			str.erase(0, 1);
	}
	return str;
}

bool PackageDependencyBuilder::isHeader(const std::string& path)
{
	return StringHelper::endsWith(path, ".hpp");
}

bool PackageDependencyBuilder::isSource(const std::string& path)
{
	return StringHelper::endsWith(path, ".cpp");
}

std::string PackageDependencyBuilder::removeExtension(std::string str)
{
	auto pos = str.rfind('.');
	if(pos >= 0 && pos < str.size())
		return str.erase(pos);
	return str;
}
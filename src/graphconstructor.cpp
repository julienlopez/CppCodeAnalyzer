#include "graphconstructor.hpp"
#include "dependencygraph.hpp"

#include <file/ifile.hpp>

StringHelper::type_vector_string GraphConstructor::s_extensions = {"h", "hpp", "h++", "c", "cc", "cpp"};

GraphConstructor::type_vector_path GraphConstructor::s_includePaths;

/*
GraphConstructor::ParsingError::ParsingError(const std::string& mess): Exception(mess)
{}

GraphConstructor::ParsingError::ParsingError(std::string&& mess): Exception(std::move(mess))
{}

GraphConstructor::FileNotFound::FileNotFound(const std::string& mess): Exception(mess)
{}

GraphConstructor::FileNotFound::FileNotFound(std::string&& mess): Exception(std::move(mess))
{}
*/

void GraphConstructor::buildGraph(DependencyGraph& graph, const boost::filesystem::path& p, const StringHelper::type_vector_string& includePaths)
{
	s_includePaths.resize(includePaths.size());
	std::transform(includePaths.begin(), includePaths.end(), s_includePaths.begin(), [](const std::string& includePath){ return boost::filesystem::absolute(includePath); });
	browseDirectory(graph, p);
}

void GraphConstructor::browseDirectory(DependencyGraph& graph, const boost::filesystem::path& p)
{
	if(!boost::filesystem::exists(p)) throw std::invalid_argument("the specified directory doesn't exist: " + p.generic_string());

	if(!boost::filesystem::is_directory(p)) throw std::invalid_argument("the given path is not a directory: " + p.generic_string());

	boost::filesystem::directory_iterator it_end;
	for(boost::filesystem::directory_iterator it(p); it != it_end; ++it)
	{
		if(boost::filesystem::is_directory(*it))
			browseDirectory(graph, *it);
		else
			analyseFile(graph, *it);
	}
}

void GraphConstructor::analyseFile(DependencyGraph& graph, const boost::filesystem::path& p)
{
	assert(boost::filesystem::exists(p));
	boost::filesystem::path dir = p.parent_path();

	std::string ext = p.extension().generic_string();
	if(ext.length() > 0 && ext.front() == '.') ext.erase(0, 1);
	if(std::find(s_extensions.begin(), s_extensions.end(), ext) == s_extensions.end()) return;

	std::string fileName = p.generic_string();
	if(StringHelper::startsWith(fileName, "./")) fileName.erase(0, 2);

	auto v = graph.addNoeud(boost::filesystem::absolute(fileName).generic_string());
	const auto& file = graph(v);
	assert(file->isModifiable());

	auto lines = file->getLinesByType(Line::Type::Preprocessor);
	for(const auto& l : lines)
	{
		std::string line = l.content();
        if(!StringHelper::startsWith(line, "#include")) continue;
        line.erase(0, 8);

        cleanUpLine(line);

		if(!(StringHelper::startsWith(line, "\"") && StringHelper::endsWith(line, "\"")) && !(StringHelper::startsWith(line, "<") && StringHelper::endsWith(line, ">")))
			// throw ParsingError("invalid line: `" + line + "` in " + p.generic_string());
			std::cerr << "invalid line: `" << line << "` in " << p.generic_string() << std::endl;

		line.erase(0, 1);
		line.erase(line.length()-1, 1);

		boost::filesystem::path path(line);
		if(!boost::filesystem::exists(path))
		{
			boost::filesystem::path localPath = dir / path;
			if(boost::filesystem::exists(localPath))
			{
				path = boost::filesystem::absolute(localPath);
			}
			else
			{
				localPath = findFileInIncludeDirs(path);
				if(localPath.empty()) {
					//throw FileNotFound("unable to find included file: " + path.generic_string());
					std::cerr << "unable to find " << path << std::endl;
				}
				else
				{
					path = localPath;
				}
			}
		}
		std::string newFile = path.generic_string();
		if(StringHelper::startsWith(newFile, "./")) newFile.erase(0, 2);
		graph.addLien(newFile, fileName);
	}
}

void GraphConstructor::cleanUpLine(std::string& line)
{
	std::size_t pos = line.find("//");
	if(pos != std::string::npos)
		line.erase(pos);
	line = StringHelper::trim(line);
}

boost::filesystem::path GraphConstructor::findFileInIncludeDirs(const boost::filesystem::path& file)
{
	for(const auto& path : s_includePaths)
	{
		boost::filesystem::path p = path / file;
		if(boost::filesystem::exists(p))
			return p;
	}
	return boost::filesystem::path();
}

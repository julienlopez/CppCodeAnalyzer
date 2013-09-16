#include "graphconstructor.hpp"
#include "dependencygraph.hpp"

#include <fstream>

using namespace std;

StringHelper::type_vector_string GraphConstructor::s_extensions = {"h", "hpp", "h++", "c", "cc", "cpp"};

GraphConstructor::ParsingError::ParsingError(const std::string& message): std::exception(), m_message(message)
{}

const char* GraphConstructor::ParsingError::what() const noexcept
{
	return m_message.c_str();
}

void GraphConstructor::buildGraph(DependencyGraph& graph, const boost::filesystem::path& p)
{
	browseDirectory(graph, p);
}

void GraphConstructor::browseDirectory(DependencyGraph& graph, const boost::filesystem::path& p)
{
	std::cout << "GraphConstructor::browseDirectory(" << p.generic_string() << ")" << std::endl;

	if(!boost::filesystem::exists(p)) throw std::invalid_argument("le dossier spécifié n'éxiste pas");

	if(!boost::filesystem::is_directory(p)) throw std::invalid_argument("le chemin spécifié n'est pas un dossier: " + p.generic_string());

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
	ifstream f(p.generic_string().c_str(), ios::in);
	if(!f) throw std::invalid_argument("Impossible d'ouvrir le fichier " + p.generic_string());

	std::string line;
	while(std::getline(f, line))
	{
        if(!StringHelper::startsWith(line, "#include ")) continue;
        line.erase(0, 9);

        cleanUpLine(line);

		if(!(StringHelper::startsWith(line, "\"") && StringHelper::endsWith(line, "\"")) && !(StringHelper::startsWith(line, "<") && StringHelper::endsWith(line, ">")))
			throw ParsingError("ligne invalide: " + line);

		line.erase(0, 1);
		line.erase(line.length()-1, 1);

		boost::filesystem::path path(line);
		if(!boost::filesystem::exists(path))
		{
			path = boost::filesystem::path(dir.generic_string() + "/" + path.generic_string());
			if(!boost::filesystem::exists(path))
			{
				cerr << "toujours pas trouvé: " << path.generic_string() << endl;
				path = boost::filesystem::path(line);
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
#include "graphconstructor.hpp"
#include "dependencygraph.hpp"

#include <fstream>

using namespace std;

StringHelper::type_vector_string GraphConstructor::s_extensions = {"h", "hpp", "h++", "c", "cc", "cpp"};

void GraphConstructor::buildGraph(DependencyGraph& graph, const boost::filesystem::path& p) throw(std::exception)
{
	browseDirectory(graph, p);
}

void GraphConstructor::browseDirectory(DependencyGraph& graph, const boost::filesystem::path& p) throw(std::exception)
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

void GraphConstructor::analyseFile(DependencyGraph& graph, const boost::filesystem::path& p) throw(std::exception)
{
	assert(boost::filesystem::exists(p));

	if(std::find_if(s_extensions.begin(), s_extensions.end(), [&](const std::string& extension)
					{
						return StringHelper::endsWith(p.generic_string(), "."+extension);
					} ) == s_extensions.end())
		return;

	std::string fileName = p.generic_string();
	if(StringHelper::startsWith(fileName, "./")) fileName.erase(0, 2);
	cout << p << " : " << fileName << endl;
	ifstream f(p.generic_string().c_str(), ios::in);
	if(!f) throw std::invalid_argument("Impossible d'ouvrir le fichier " + p.generic_string());

	std::string line;
	while(std::getline(f, line))
	{
        if(!StringHelper::startsWith(line, "#include ")) continue;
        line.erase(0, 9);
//		std::cout << line << std::endl;
		if(!(StringHelper::startsWith(line, "\"") && StringHelper::endsWith(line, "\"")) && !(StringHelper::startsWith(line, "<") && StringHelper::endsWith(line, ">")))
			throw("ligne invalide: " + line);
//		std::cout << line << std::endl;
		line.erase(0, 1);
		line.erase(line.length()-1, 1);
		std::cout << line << std::endl;
		graph.addLien(line, fileName);
	}
}

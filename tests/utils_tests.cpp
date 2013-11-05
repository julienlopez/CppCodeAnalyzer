#include "utils_tests.hpp"

#include <fstream>

void utils_tests::createEmptyFile(boost::filesystem::path dir, const std::string& fileName)
{
	dir /= boost::filesystem::path(fileName);
	std::ofstream ofs(dir.generic_string().c_str());
	ofs << "\n";
}

void utils_tests::createFile(boost::filesystem::path dir, const std::string& fileName, const std::list<std::string>& includes)
{
	dir /= boost::filesystem::path(fileName);
	std::ofstream ofs(dir.generic_string().c_str());
	for(const auto& file : includes)
	{
		ofs << "#include \"" << file << "\"\n";
	}
}

void utils_tests::createFileWithContent(boost::filesystem::path dir, const std::string& fileName, const std::string& text)
{
	dir /= boost::filesystem::path(fileName);
	std::ofstream ofs(dir.generic_string().c_str());
	ofs << text;
}

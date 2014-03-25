#include "utils_tests.hpp"

#include <fstream>

#include <boost/filesystem.hpp>

void utils_tests::createEmptyFile(boost::filesystem::path dir, const std::string& fileName)
{
	dir /= boost::filesystem::path(fileName);
	std::ofstream ofs(dir.generic_string());
	ofs << "\n";
}

void utils_tests::createFile(boost::filesystem::path dir, const std::string& fileName, const std::list<std::string>& includes)
{
	dir /= boost::filesystem::path(fileName);
	std::ofstream ofs(dir.generic_string());
	for(const auto& file : includes)
	{
		ofs << "#include \"" << file << "\"\n";
	}
}

void utils_tests::createFileWithContent(boost::filesystem::path dir, const std::string& fileName, const std::string& text)
{
	dir /= boost::filesystem::path(fileName);
	std::ofstream ofs(dir.generic_string());
	ofs << text;
}

std::string utils_tests::readFileAsAString(boost::filesystem::path dir, const std::string& fileName)
{
	dir /= boost::filesystem::path(fileName);
	std::ifstream ifs(dir.generic_string());
	std::string res;
	std::string line;
	while(std::getline(ifs, line))
		res += line + '\n';
	return res;
}

void utils_tests::GTestWithFilesBase::SetUp()
{
	m_dir_base = boost::filesystem::path("base");
	ASSERT_TRUE(boost::filesystem::create_directory(m_dir_base)) << "unable to create directory";
}

void utils_tests::GTestWithFilesBase::TearDown()
{
	boost::filesystem::remove_all(m_dir_base);
}
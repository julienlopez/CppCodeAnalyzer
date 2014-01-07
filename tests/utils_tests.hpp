#ifndef __UTILSTESTS_HPP__
#define __UTILSTESTS_HPP__

#include <boost/filesystem/path.hpp>

namespace utils_tests
{
	/**
	* \brief create an empty file in the directory dir with name fileName
	* \param dir the path of the directory where the file is to be created.
	* \param fileName the name of the file to be created.
	*/
	void createEmptyFile(boost::filesystem::path dir, const std::string& fileName);

	/**
	* \brief create a file in the directory dir with name fileName, with lines to include the given list of files in includes.
	* \param dir the path of the directory where the file is to be created.
	* \param fileName the name of the file to be created.
	* \param includes the differents files to include in the file.
	*/
	void createFile(boost::filesystem::path dir, const std::string& fileName, const std::list<std::string>& includes);

	/**
	* \brief create a file in the directory dir with name fileName, with text as content.
	* \param dir the path of the directory where the file is to be created.
	* \param fileName the name of the file to be created.
	* \param text the content of the file.
	*/
	void createFileWithContent(boost::filesystem::path dir, const std::string& fileName, const std::string& text);
	
	/**
	* \brief read the content of a file and puts it in a string.
	*/
	std::string readFileAsAString(boost::filesystem::path dir, const std::string& fileName);
}

#endif
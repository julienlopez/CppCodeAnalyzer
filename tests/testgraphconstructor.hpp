#ifndef __TESTGRAPHCONSTRUCTOR_HPP__
#define __TESTGRAPHCONSTRUCTOR_HPP__

#include <dependencygraph.hpp>

#include <cppunit/extensions/HelperMacros.h>

#include <boost/filesystem.hpp>

/**
* \brief unit test for the graph construction.
*/
class TestGraphConstructor : public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE(TestGraphConstructor);
	CPPUNIT_TEST(testOneFolderThreeFilesNoIclusion);
	CPPUNIT_TEST(testOneFolderThreeFiles);
	CPPUNIT_TEST(testOneFolderOneSubFolderThreeFiles);
	CPPUNIT_TEST(testIncludeLineInvalid);
	CPPUNIT_TEST(testSplitFoldersOneFileInEach);
	CPPUNIT_TEST_SUITE_END();

public:
	TestGraphConstructor() = default;

	void setUp();
	void tearDown();

	void testOneFolderThreeFilesNoIclusion();
	void testOneFolderThreeFiles();
	void testOneFolderOneSubFolderThreeFiles();

	void testIncludeLineInvalid();

	/**
	* Test the result of analyzing the following setup:
	* test of dir
	* dir
	*  - include
	*     - header.hpp
	*  - src
	*     - main.cpp
	*/
	void testSplitFoldersOneFileInEach();

private:
	boost::filesystem::path m_dir_base;

	/**
	* \brief create an empty file in the directory dir with name fileName
	* \param dir the path of the directory where the file is to be created.
	* \param fileName the name of the file to be created.
	*/
	static void createEmptyFile(boost::filesystem::path dir, const std::string& fileName);

	/**
	* \brief create a file in the directory dir with name fileName, with lines to include the given list of files in includes.
	* \param dir the path of the directory where the file is to be created.
	* \param fileName the name of the file to be created.
	* \param includes the differents files to include in the file.
	*/
	static void createFile(boost::filesystem::path dir, const std::string& fileName, const std::list<std::string>& includes);

	/**
	* \brief create a file in the directory dir with name fileName, with text as content.
	* \param dir the path of the directory where the file is to be created.
	* \param fileName the name of the file to be created.
	* \param text the content of the file.
	*/
	static void createFileWithContent(boost::filesystem::path dir, const std::string& fileName, const std::string& text);
};

#endif
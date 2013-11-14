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
	//CPPUNIT_TEST(testIncludeLineInvalid);
	CPPUNIT_TEST(testSplitFoldersOneFileInEach);
	CPPUNIT_TEST_SUITE_END();

public:
	TestGraphConstructor() = default;

	void setUp();
	void tearDown();

	void testOneFolderThreeFilesNoIclusion();
	void testOneFolderThreeFiles();
	void testOneFolderOneSubFolderThreeFiles();

	//void testIncludeLineInvalid();

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
};

#endif
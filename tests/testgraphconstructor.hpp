#ifndef __TESTGRAPHCONSTRUCTOR_HPP__
#define __TESTGRAPHCONSTRUCTOR_HPP__

#include <dependencygraph.hpp>

#include <cppunit/extensions/HelperMacros.h>

#include <boost/filesystem.hpp>

class TestGraphConstructor : public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE(TestGraphConstructor);
	CPPUNIT_TEST(testOneFolderThreeFilesNoIclusion);
	CPPUNIT_TEST(testOneFolderThreeFiles);
	CPPUNIT_TEST(testOneFolderOneSubFolderThreeFiles);
	CPPUNIT_TEST_SUITE_END();

public:
	TestGraphConstructor() = default;

	void setUp();
	void tearDown();

	void testOneFolderThreeFilesNoIclusion();
	void testOneFolderThreeFiles();
	void testOneFolderOneSubFolderThreeFiles();

private:
	boost::filesystem::path m_dir_base;

	static void createEmptyFile(boost::filesystem::path dir, const std::string& fileName);
	static void createFile(boost::filesystem::path dir, const std::string& fileName, const std::list<std::string>& includes);
};

#endif
#ifndef __TESTHEADERSORTER_HPP__
#define __TESTHEADERSORTER_HPP__

#include <cppunit/extensions/HelperMacros.h>

#include <boost/filesystem.hpp>

class TestHeaderSorter : public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE(TestHeaderSorter);
	CPPUNIT_TEST(testFileWithNoInclude);
	CPPUNIT_TEST(testSortingTwoSimpleIncludeLines);
	CPPUNIT_TEST_SUITE_END();

public:
	TestHeaderSorter() = default;

	void setUp();
	void tearDown();

	void testFileWithNoInclude();
	void testSortingTwoSimpleIncludeLines();

private:
	boost::filesystem::path m_dir_base;
};

#endif
#ifndef __TESTFILE_HPP__
#define __TESTFILE_HPP__

#include <cppunit/extensions/HelperMacros.h>

#include <boost/filesystem/path.hpp>

class TestFile : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(TestFile);
    CPPUNIT_TEST(testThrowOnInvalidFile);
	CPPUNIT_TEST(testParseFileWithOneLine);
	CPPUNIT_TEST(testParseFileWithOneCommentLine);
	CPPUNIT_TEST(testParseFileWithOnePreprocessorLine);
	CPPUNIT_TEST(testParseFileWithOneLineOfEach);
	CPPUNIT_TEST(testParseFileWithOneMultiLineComment);
	CPPUNIT_TEST(testCommentedIncludeLine);
	CPPUNIT_TEST(testMultiLineCommentedIncludeLine);
    CPPUNIT_TEST_SUITE_END();

public:
	TestFile() = default;

	void setUp();
	void tearDown();

	void testThrowOnInvalidFile();
	void testParseFileWithOneLine();
	void testParseFileWithOneCommentLine();
	void testParseFileWithOnePreprocessorLine();
	void testParseFileWithOneLineOfEach();
	void testParseFileWithOneMultiLineComment();

	/**
	* test the following case:
	* <pre>
	* #include "a.h"
	* //#include "b.h"
	* #include "c.h"
	* </pre>
	*/
	void testCommentedIncludeLine();

	/**
	* test the following case:
	* <pre>
	* #include "a.h"
	* / *
	* #include "b.h"
	* #include "c.h"
	* * /
	* #include "d.h"
	* </pre>
	*/
	void testMultiLineCommentedIncludeLine();

private:
	boost::filesystem::path m_dir_base;
};

#endif
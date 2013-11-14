#ifndef __TESTREDONDANTHEADERANALYZER_HPP__
#define __TESTREDONDANTHEADERANALYZER_HPP__

#include <cppunit/extensions/HelperMacros.h>

class TestRedondantHeaderAnalyzer : public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE(TestRedondantHeaderAnalyzer);
	CPPUNIT_TEST(testStraightInclusionOfThreeFiles);
	CPPUNIT_TEST(testYShapedInclusionOfThreeFiles);
	CPPUNIT_TEST(testDiamondShapedInclusionOfFourFiles);
	CPPUNIT_TEST(testDiamondShapedInclusionOfFourFilesAndBottomAlsoIncludeTop);
	CPPUNIT_TEST(testStraightInclusionOfThreeFilesWithTheBottomOneAlsoIncludingTop);
	CPPUNIT_TEST(testStraightInclusionOfFourFilesWithTheBottomOneAlsoIncludingTop);
	CPPUNIT_TEST_SUITE_END();

public:
	TestRedondantHeaderAnalyzer() = default;

	/**
	* main.cpp -> header1.hpp -> header2.hpp <br />
	* should not find any problem
	*/
	void testStraightInclusionOfThreeFiles();

	/**
	* main.cpp -> header1.hpp and main.cpp -> header2.hpp <br />
	* should not find any problem
	*/
	void testYShapedInclusionOfThreeFiles();

	/**
	* main.cpp -> header1.hpp, main.cpp -> header2.hpp, header1.hpp -> global.hpp and header2.hpp -> global.hpp <br />
	* should not find any problem
	*/
	void testDiamondShapedInclusionOfFourFiles();

	/**
	* main.cpp -> header1.hpp, main.cpp -> header2.hpp, header1.hpp -> global.hpp, header2.hpp -> global.hpp and main.cpp -> global.hpp <br />
	* => should find main.cpp including global.hpp as a problem
	*/
	void testDiamondShapedInclusionOfFourFilesAndBottomAlsoIncludeTop();

	/**
	* main.cpp -> header1.hpp -> header2.hpp and main.cpp -> header2.hpp <br />
	* should find main.cpp including header2.hpp as a problem
	*/
	void testStraightInclusionOfThreeFilesWithTheBottomOneAlsoIncludingTop();

	/**
	* main.cpp -> header1.hpp -> header2.hpp -> header3.hpp and main.cpp -> header3.hpp <br />
	* should find main.cpp including header3.hpp as a problem
	*/
	void testStraightInclusionOfFourFilesWithTheBottomOneAlsoIncludingTop();
};

#endif
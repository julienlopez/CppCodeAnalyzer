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
	CPPUNIT_TEST_SUITE_END();

public:
	TestRedondantHeaderAnalyzer() = default;

	void testStraightInclusionOfThreeFiles();
	void testYShapedInclusionOfThreeFiles();
	void testDiamondShapedInclusionOfFourFiles();
	void testDiamondShapedInclusionOfFourFilesAndBottomAlsoIncludeTop();
};

#endif
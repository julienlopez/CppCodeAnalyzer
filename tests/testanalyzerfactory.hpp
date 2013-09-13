#ifndef __TESTANALYZERFACTORY_HPP__
#define __TESTANALYZERFACTORY_HPP__

#include <cppunit/extensions/HelperMacros.h>

class TestAnalyzerFactory : public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE(TestAnalyzerFactory);
	CPPUNIT_TEST(testCreationInvalidAnalyzer);
	CPPUNIT_TEST(testCreationRedondantHeaderAnalyzer);
	CPPUNIT_TEST_SUITE_END();

public:
	TestAnalyzerFactory() = default;

	void testCreationInvalidAnalyzer();
	void testCreationRedondantHeaderAnalyzer();
};

#endif

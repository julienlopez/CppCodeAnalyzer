#ifndef __TESTDEPENDENCYGRAPH_HPP__
#define __TESTDEPENDENCYGRAPH_HPP__

#include <cppunit/extensions/HelperMacros.h>

class TestDependencyGraph : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(TestDependencyGraph);
    CPPUNIT_TEST(testPrint);
    CPPUNIT_TEST_SUITE_END();

public:
	TestDependencyGraph() = default;

	void testPrint();
};

#endif

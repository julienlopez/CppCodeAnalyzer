#ifndef __TESTSTRINGHELPER_HPP__
#define __TESTSTRINGHELPER_HPP__

#include <cppunit/extensions/HelperMacros.h>

class TestStringHelper : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(TestStringHelper);
    CPPUNIT_TEST(testStartsWith);
    CPPUNIT_TEST(testEndsWith);
    CPPUNIT_TEST(testJoin);
	CPPUNIT_TEST(testContains);
	CPPUNIT_TEST(testTrim);
    CPPUNIT_TEST_SUITE_END();

public:
	TestStringHelper() = default;

	void testStartsWith();
	void testEndsWith();
	void testJoin();
	void testContains();
	void testTrim();
};

#endif
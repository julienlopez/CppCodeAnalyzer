#include "teststringhelper.hpp"

#include <utils/stringhelper.hpp>

CPPUNIT_TEST_SUITE_REGISTRATION(TestStringHelper);

void TestStringHelper::testStartsWith()
{
	std::string str = "Just a test";
	CPPUNIT_ASSERT(StringHelper::startsWith(str, "Jus"));
	CPPUNIT_ASSERT(!StringHelper::startsWith(str, "No"));
}

void TestStringHelper::testEndsWith()
{
	std::string str = "Just a test";
	CPPUNIT_ASSERT(StringHelper::endsWith(str, "est"));
	CPPUNIT_ASSERT(!StringHelper::endsWith(str, "te"));
}

void TestStringHelper::testJoin()
{
	StringHelper::type_vector_string strl {{"a"}, {"bc"}, {"d"}};
	std::string res = StringHelper::join(strl);
	CPPUNIT_ASSERT_EQUAL(std::string("a bc d"), res);
	res = StringHelper::join(strl, "");
	CPPUNIT_ASSERT_EQUAL(std::string("abcd"), res);
}
void TestStringHelper::testContains()
{
	std::string str = "Just a test";
	CPPUNIT_ASSERT(StringHelper::contains(str, "t a te"));
	CPPUNIT_ASSERT(!StringHelper::contains(str, "tre"));
}

void TestStringHelper::testTrim()
{
	std::string str("  \tto trim \t");
	std::string res = StringHelper::trim(str);
	CPPUNIT_ASSERT_EQUAL(std::string("to trim"), res);
	str = "nothing to trim";
	res = StringHelper::trim(str);
	CPPUNIT_ASSERT_EQUAL(str, res);
}

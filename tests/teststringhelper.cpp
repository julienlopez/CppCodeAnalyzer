#include "teststringhelper.hpp"

#include <utils/stringhelper.hpp>

CPPUNIT_TEST_SUITE_REGISTRATION(TestStringHelper);

TestStringHelper::TestStringHelper()
{}

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
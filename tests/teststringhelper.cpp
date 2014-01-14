#include <gtest/gtest.h>

#include <utils/stringhelper.hpp>

TEST(TestStringHelper, testStartsWith)
{
	std::string str = "Just a test";
	ASSERT_TRUE(StringHelper::startsWith(str, "Jus"));
	ASSERT_TRUE(!StringHelper::startsWith(str, "No"));
}

TEST(TestStringHelper, testEndsWith)
{
	std::string str = "Just a test";
	ASSERT_TRUE(StringHelper::endsWith(str, "est"));
	ASSERT_TRUE(!StringHelper::endsWith(str, "te"));
}

TEST(TestStringHelper, testJoin)
{
	StringHelper::type_vector_string strl {{"a"}, {"bc"}, {"d"}};
	std::string res = StringHelper::join(strl);
	ASSERT_EQ(std::string("a bc d"), res);
	res = StringHelper::join(strl, "");
	ASSERT_EQ(std::string("abcd"), res);
}

TEST(TestStringHelper, testContains)
{
	std::string str = "Just a test";
	ASSERT_TRUE(StringHelper::contains(str, "t a te"));
	ASSERT_TRUE(!StringHelper::contains(str, "tre"));
}

TEST(TestStringHelper, testTrim)
{
	std::string str("  \tto trim \t");
	std::string res = StringHelper::trim(str);
	ASSERT_EQ(std::string("to trim"), res);
	str = "nothing to trim";
	res = StringHelper::trim(str);
	ASSERT_EQ(str, res);
}

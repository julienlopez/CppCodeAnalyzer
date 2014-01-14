#include "testfile.hpp"
#include "utils_tests.hpp"

#include <file/modifiablefile.hpp>
#include <utils/stringhelper.hpp>

#include <boost/filesystem.hpp>

void TestFile::SetUp()
{
	m_dir_base = boost::filesystem::path("base");
	ASSERT_TRUE(boost::filesystem::create_directory(m_dir_base)) << "unable to create directory";
}

void TestFile::TearDown()
{
	boost::filesystem::remove_all(m_dir_base);
}

TEST_F(TestFile, testThrowOnInvalidFile)
{
	ASSERT_THROW(ModifiableFile f("invalide/path.txt"), std::invalid_argument);
}

TEST_F(TestFile, testParseFileWithOneLine)
{
	std::string line = "int add();";
	utils_tests::createFileWithContent(m_dir_base, "main.cpp", line);
	boost::filesystem::path file = m_dir_base / "main.cpp";
	ModifiableFile f(file.generic_string());
	ASSERT_EQ((std::size_t)1, f.count());
	ASSERT_EQ((std::size_t)0, f.count(Line::Type::Comment));
	ASSERT_EQ((std::size_t)0, f.count(Line::Type::Preprocessor));
	ASSERT_EQ((std::size_t)1, f.count(Line::Type::Other));
	ASSERT_TRUE(Line::Type::Other == f.at(0).type());
	ASSERT_EQ(line, f.at(0).content());
}

TEST_F(TestFile, testParseFileWithOneCommentLine)
{
	std::string line = "//int add();";
	utils_tests::createFileWithContent(m_dir_base, "main.cpp", line);
	boost::filesystem::path file = m_dir_base / "main.cpp";
	ModifiableFile f(file.generic_string());
	ASSERT_EQ((std::size_t)1, f.count());
	ASSERT_EQ((std::size_t)1, f.count(Line::Type::Comment));
	ASSERT_EQ((std::size_t)0, f.count(Line::Type::Preprocessor));
	ASSERT_EQ((std::size_t)0, f.count(Line::Type::Other));
	ASSERT_TRUE(Line::Type::Comment == f.at(0).type());
	ASSERT_EQ(line, f.at(0).content());
}

TEST_F(TestFile, testParseFileWithOnePreprocessorLine)
{
	std::string line = "#include <file.h>";
	utils_tests::createFileWithContent(m_dir_base, "main.cpp", line);
	boost::filesystem::path file = m_dir_base / "main.cpp";
	ModifiableFile f(file.generic_string());
	ASSERT_EQ((std::size_t)1, f.count());
	ASSERT_EQ((std::size_t)0, f.count(Line::Type::Comment));
	ASSERT_EQ((std::size_t)1, f.count(Line::Type::Preprocessor));
	ASSERT_EQ((std::size_t)0, f.count(Line::Type::Other));
	ASSERT_TRUE(Line::Type::Preprocessor == f.at(0).type());
	ASSERT_EQ(line, f.at(0).content());
}

TEST_F(TestFile, testParseFileWithOneLineOfEach)
{
	std::map<Line::Type, std::string> lines = { 
												{Line::Type::Preprocessor, "#include <file.h>"}, 
												{Line::Type::Comment, "//int add();"}, 
												{Line::Type::Other, "int add();"} 
											  };
	utils_tests::createFileWithContent(m_dir_base, "main.cpp", lines[Line::Type::Preprocessor] + "\n" + lines[Line::Type::Comment] + "\n" + lines[Line::Type::Other]);
	boost::filesystem::path file = m_dir_base / "main.cpp";
	ModifiableFile f(file.generic_string());
	ASSERT_EQ((std::size_t)3, f.count());
	ASSERT_EQ((std::size_t)1, f.count(Line::Type::Comment));
	ASSERT_EQ((std::size_t)1, f.count(Line::Type::Preprocessor));
	ASSERT_EQ((std::size_t)1, f.count(Line::Type::Other));

	ASSERT_TRUE(Line::Type::Preprocessor == f.at(0).type());
	ASSERT_EQ(lines[Line::Type::Preprocessor], f.at(0).content());

	ASSERT_TRUE(Line::Type::Comment == f.at(1).type());
	ASSERT_EQ(lines[Line::Type::Comment], f.at(1).content());

	ASSERT_TRUE(Line::Type::Other == f.at(2).type());
	ASSERT_EQ(lines[Line::Type::Other], f.at(2).content());
}

TEST_F(TestFile, testParseFileWithOneMultiLineComment)
{
	std::string line1 = "/*";
	std::string line2 = "some text";
	std::string line3 = "*/";
	utils_tests::createFileWithContent(m_dir_base, "main.cpp", line1 + "\n" + line2 + "\n" + line3);
	boost::filesystem::path file = m_dir_base / "main.cpp";
	ModifiableFile f(file.generic_string());
	ASSERT_EQ((std::size_t)3, f.count());
	ASSERT_EQ((std::size_t)3, f.count(Line::Type::Comment));
	ASSERT_EQ((std::size_t)0, f.count(Line::Type::Preprocessor));
	ASSERT_EQ((std::size_t)0, f.count(Line::Type::Other));

	ASSERT_TRUE(Line::Type::Comment == f.at(0).type());
	ASSERT_EQ(line1, f.at(0).content());

	ASSERT_TRUE(Line::Type::Comment == f.at(1).type());
	ASSERT_EQ(line2, f.at(1).content());

	ASSERT_TRUE(Line::Type::Comment == f.at(2).type());
	ASSERT_EQ(line3, f.at(2).content());
}

/**
* test the following case:
* <pre>
* #include "a.h"
* //#include "b.h"
* #include "c.h"
* </pre>
*/
TEST_F(TestFile, testCommentedIncludeLine)
{
	std::string line1 = "#include \"a.h\"";
	std::string line2 = "//#include \"b.h\"";
	std::string line3 = "#include \"c.h\"";
	utils_tests::createFileWithContent(m_dir_base, "main.cpp", line1 + "\n" + line2 + "\n" + line3);
	boost::filesystem::path file = m_dir_base / "main.cpp";
	ModifiableFile f(file.generic_string());
	ASSERT_EQ((std::size_t)3, f.count());
	ASSERT_EQ((std::size_t)1, f.count(Line::Type::Comment));
	ASSERT_EQ((std::size_t)2, f.count(Line::Type::Preprocessor));
	ASSERT_EQ((std::size_t)0, f.count(Line::Type::Other));

	ASSERT_TRUE(Line::Type::Preprocessor == f.at(0).type());
	ASSERT_EQ(line1, f.at(0).content());

	ASSERT_TRUE(Line::Type::Comment == f.at(1).type());
	ASSERT_EQ(line2, f.at(1).content());

	ASSERT_TRUE(Line::Type::Preprocessor == f.at(2).type());
	ASSERT_EQ(line3, f.at(2).content());
}

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
TEST_F(TestFile, testMultiLineCommentedIncludeLine)
{
	StringHelper::type_vector_string lines = { {"#include \"a.h\""}, {"/*"}, {"#include \"b.h\""}, {"#include \"c.h\""}, {"*/"}, {"#include \"d.h\""} };
	utils_tests::createFileWithContent(m_dir_base, "main.cpp", StringHelper::join(lines, "\n"));
	boost::filesystem::path file = m_dir_base / "main.cpp";
	ModifiableFile f(file.generic_string());
	ASSERT_EQ((std::size_t)6, f.count());
	ASSERT_EQ((std::size_t)4, f.count(Line::Type::Comment));
	ASSERT_EQ((std::size_t)2, f.count(Line::Type::Preprocessor));
	ASSERT_EQ((std::size_t)0, f.count(Line::Type::Other));

	ASSERT_TRUE(Line::Type::Preprocessor == f.at(0).type());
	ASSERT_EQ(lines.at(0), f.at(0).content());

	ASSERT_TRUE(Line::Type::Comment == f.at(1).type());
	ASSERT_EQ(lines.at(1), f.at(1).content());

	ASSERT_TRUE(Line::Type::Comment == f.at(2).type());
	ASSERT_EQ(lines.at(2), f.at(2).content());

	ASSERT_TRUE(Line::Type::Comment == f.at(3).type());
	ASSERT_EQ(lines.at(3), f.at(3).content());

	ASSERT_TRUE(Line::Type::Comment == f.at(4).type());
	ASSERT_EQ(lines.at(4), f.at(4).content());

	ASSERT_TRUE(Line::Type::Preprocessor == f.at(5).type());
	ASSERT_EQ(lines.at(5), f.at(5).content());
}

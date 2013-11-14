#include "testfile.hpp"
#include "utils_tests.hpp"

#include <file.hpp>
#include <utils/stringhelper.hpp>

#include <boost/filesystem.hpp>

CPPUNIT_TEST_SUITE_REGISTRATION(TestFile);

void TestFile::setUp()
{
	m_dir_base = boost::filesystem::path("base");
	CPPUNIT_ASSERT_MESSAGE("unable to create directory", boost::filesystem::create_directory(m_dir_base));
}

void TestFile::tearDown()
{
	boost::filesystem::remove_all(m_dir_base);
}

void TestFile::testThrowOnInvalidFile()
{
	CPPUNIT_ASSERT_THROW(File f("invalide/path.txt"), std::invalid_argument);
}

void TestFile::testParseFileWithOneLine()
{
	std::string line = "int add();";
	utils_tests::createFileWithContent(m_dir_base, "main.cpp", line);
	boost::filesystem::path file = m_dir_base / "main.cpp";
	File f(file.generic_string());
	CPPUNIT_ASSERT_EQUAL((std::size_t)1, f.count());
	CPPUNIT_ASSERT_EQUAL((std::size_t)0, f.count(Line::Type::Comment));
	CPPUNIT_ASSERT_EQUAL((std::size_t)0, f.count(Line::Type::Preprocessor));
	CPPUNIT_ASSERT_EQUAL((std::size_t)1, f.count(Line::Type::Other));
	CPPUNIT_ASSERT(Line::Type::Other == f.at(0).type());
	CPPUNIT_ASSERT_EQUAL(line, f.at(0).content());
}

void TestFile::testParseFileWithOneCommentLine()
{
	std::string line = "//int add();";
	utils_tests::createFileWithContent(m_dir_base, "main.cpp", line);
	boost::filesystem::path file = m_dir_base / "main.cpp";
	File f(file.generic_string());
	CPPUNIT_ASSERT_EQUAL((std::size_t)1, f.count());
	CPPUNIT_ASSERT_EQUAL((std::size_t)1, f.count(Line::Type::Comment));
	CPPUNIT_ASSERT_EQUAL((std::size_t)0, f.count(Line::Type::Preprocessor));
	CPPUNIT_ASSERT_EQUAL((std::size_t)0, f.count(Line::Type::Other));
	CPPUNIT_ASSERT(Line::Type::Comment == f.at(0).type());
	CPPUNIT_ASSERT_EQUAL(line, f.at(0).content());
}

void TestFile::testParseFileWithOnePreprocessorLine()
{
	std::string line = "#include <file.h>";
	utils_tests::createFileWithContent(m_dir_base, "main.cpp", line);
	boost::filesystem::path file = m_dir_base / "main.cpp";
	File f(file.generic_string());
	CPPUNIT_ASSERT_EQUAL((std::size_t)1, f.count());
	CPPUNIT_ASSERT_EQUAL((std::size_t)0, f.count(Line::Type::Comment));
	CPPUNIT_ASSERT_EQUAL((std::size_t)1, f.count(Line::Type::Preprocessor));
	CPPUNIT_ASSERT_EQUAL((std::size_t)0, f.count(Line::Type::Other));
	CPPUNIT_ASSERT(Line::Type::Preprocessor == f.at(0).type());
	CPPUNIT_ASSERT_EQUAL(line, f.at(0).content());
}
void TestFile::testParseFileWithOneLineOfEach()
{
	std::map<Line::Type, std::string> lines = { 
												{Line::Type::Preprocessor, "#include <file.h>"}, 
												{Line::Type::Comment, "//int add();"}, 
												{Line::Type::Other, "int add();"} 
											  };
	utils_tests::createFileWithContent(m_dir_base, "main.cpp", lines[Line::Type::Preprocessor] + "\n" + lines[Line::Type::Comment] + "\n" + lines[Line::Type::Other]);
	boost::filesystem::path file = m_dir_base / "main.cpp";
	File f(file.generic_string());
	CPPUNIT_ASSERT_EQUAL((std::size_t)3, f.count());
	CPPUNIT_ASSERT_EQUAL((std::size_t)1, f.count(Line::Type::Comment));
	CPPUNIT_ASSERT_EQUAL((std::size_t)1, f.count(Line::Type::Preprocessor));
	CPPUNIT_ASSERT_EQUAL((std::size_t)1, f.count(Line::Type::Other));

	CPPUNIT_ASSERT(Line::Type::Preprocessor == f.at(0).type());
	CPPUNIT_ASSERT_EQUAL(lines[Line::Type::Preprocessor], f.at(0).content());

	CPPUNIT_ASSERT(Line::Type::Comment == f.at(1).type());
	CPPUNIT_ASSERT_EQUAL(lines[Line::Type::Comment], f.at(1).content());

	CPPUNIT_ASSERT(Line::Type::Other == f.at(2).type());
	CPPUNIT_ASSERT_EQUAL(lines[Line::Type::Other], f.at(2).content());
}

void TestFile::testParseFileWithOneMultiLineComment()
{
	std::string line1 = "/*";
	std::string line2 = "some text";
	std::string line3 = "*/";
	utils_tests::createFileWithContent(m_dir_base, "main.cpp", line1 + "\n" + line2 + "\n" + line3);
	boost::filesystem::path file = m_dir_base / "main.cpp";
	File f(file.generic_string());
	CPPUNIT_ASSERT_EQUAL((std::size_t)3, f.count());
	CPPUNIT_ASSERT_EQUAL((std::size_t)3, f.count(Line::Type::Comment));
	CPPUNIT_ASSERT_EQUAL((std::size_t)0, f.count(Line::Type::Preprocessor));
	CPPUNIT_ASSERT_EQUAL((std::size_t)0, f.count(Line::Type::Other));

	CPPUNIT_ASSERT(Line::Type::Comment == f.at(0).type());
	CPPUNIT_ASSERT_EQUAL(line1, f.at(0).content());

	CPPUNIT_ASSERT(Line::Type::Comment == f.at(1).type());
	CPPUNIT_ASSERT_EQUAL(line2, f.at(1).content());

	CPPUNIT_ASSERT(Line::Type::Comment == f.at(2).type());
	CPPUNIT_ASSERT_EQUAL(line3, f.at(2).content());
}

void TestFile::testCommentedIncludeLine()
{
	std::string line1 = "#include \"a.h\"";
	std::string line2 = "//#include \"b.h\"";
	std::string line3 = "#include \"c.h\"";
	utils_tests::createFileWithContent(m_dir_base, "main.cpp", line1 + "\n" + line2 + "\n" + line3);
	boost::filesystem::path file = m_dir_base / "main.cpp";
	File f(file.generic_string());
	CPPUNIT_ASSERT_EQUAL((std::size_t)3, f.count());
	CPPUNIT_ASSERT_EQUAL((std::size_t)1, f.count(Line::Type::Comment));
	CPPUNIT_ASSERT_EQUAL((std::size_t)2, f.count(Line::Type::Preprocessor));
	CPPUNIT_ASSERT_EQUAL((std::size_t)0, f.count(Line::Type::Other));

	CPPUNIT_ASSERT(Line::Type::Preprocessor == f.at(0).type());
	CPPUNIT_ASSERT_EQUAL(line1, f.at(0).content());

	CPPUNIT_ASSERT(Line::Type::Comment == f.at(1).type());
	CPPUNIT_ASSERT_EQUAL(line2, f.at(1).content());

	CPPUNIT_ASSERT(Line::Type::Preprocessor == f.at(2).type());
	CPPUNIT_ASSERT_EQUAL(line3, f.at(2).content());
}

void TestFile::testMultiLineCommentedIncludeLine()
{
	StringHelper::type_vector_string lines = { {"#include \"a.h\""}, {"/*"}, {"#include \"b.h\""}, {"#include \"c.h\""}, {"*/"}, {"#include \"d.h\""} };
	utils_tests::createFileWithContent(m_dir_base, "main.cpp", StringHelper::join(lines, "\n"));
	boost::filesystem::path file = m_dir_base / "main.cpp";
	File f(file.generic_string());
	CPPUNIT_ASSERT_EQUAL((std::size_t)6, f.count());
	CPPUNIT_ASSERT_EQUAL((std::size_t)4, f.count(Line::Type::Comment));
	CPPUNIT_ASSERT_EQUAL((std::size_t)2, f.count(Line::Type::Preprocessor));
	CPPUNIT_ASSERT_EQUAL((std::size_t)0, f.count(Line::Type::Other));

	CPPUNIT_ASSERT(Line::Type::Preprocessor == f.at(0).type());
	CPPUNIT_ASSERT_EQUAL(lines.at(0), f.at(0).content());

	CPPUNIT_ASSERT(Line::Type::Comment == f.at(1).type());
	CPPUNIT_ASSERT_EQUAL(lines.at(1), f.at(1).content());

	CPPUNIT_ASSERT(Line::Type::Comment == f.at(2).type());
	CPPUNIT_ASSERT_EQUAL(lines.at(2), f.at(2).content());

	CPPUNIT_ASSERT(Line::Type::Comment == f.at(3).type());
	CPPUNIT_ASSERT_EQUAL(lines.at(3), f.at(3).content());

	CPPUNIT_ASSERT(Line::Type::Comment == f.at(4).type());
	CPPUNIT_ASSERT_EQUAL(lines.at(4), f.at(4).content());

	CPPUNIT_ASSERT(Line::Type::Preprocessor == f.at(5).type());
	CPPUNIT_ASSERT_EQUAL(lines.at(5), f.at(5).content());
}

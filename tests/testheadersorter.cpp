#include "testheadersorter.hpp"
#include "utils_tests.hpp"

#include <dependencygraph.hpp>
#include <graphconstructor.hpp>
#include <modifier/imodifier.hpp>

CPPUNIT_TEST_SUITE_REGISTRATION(TestHeaderSorter);

void TestHeaderSorter::setUp()
{
	m_dir_base = boost::filesystem::path("base");
	CPPUNIT_ASSERT_MESSAGE("unable to create directory", boost::filesystem::create_directory(m_dir_base));
}

void TestHeaderSorter::tearDown()
{
	boost::filesystem::remove_all(m_dir_base);
}

void TestHeaderSorter::testFileWithNoInclude()
{
	std::string fileContent = "class Test{\npublic:\nTest();\n};\n";
	utils_tests::createFileWithContent(m_dir_base, "main.cpp", fileContent);

	DependencyGraph graph;
	CPPUNIT_ASSERT_NO_THROW(GraphConstructor::buildGraph(graph, m_dir_base));

	std::unique_ptr<iModifier> modifier(ModifierFactory::instance().create("HeaderSorter", graph));
	CPPUNIT_ASSERT(modifier);
	CPPUNIT_ASSERT(modifier->isRegistered());
	modifier->process();

	std::ostringstream oss;
	modifier->printReport(oss);
	CPPUNIT_ASSERT_EQUAL(std::string("Nothing has been changed"), oss.str());
	CPPUNIT_ASSERT_EQUAL(fileContent, utils_tests::readFileAsAString(m_dir_base, "main.cpp"));
}

void TestHeaderSorter::testSortingTwoSimpleIncludeLines()
{
	std::string fileContent = "#include <string>\n#include<iostream>\n\nclass Test{\npublic:\nTest();\n};\n";
	utils_tests::createFileWithContent(m_dir_base, "main.cpp", fileContent);

	DependencyGraph graph;
	CPPUNIT_ASSERT_NO_THROW(GraphConstructor::buildGraph(graph, m_dir_base));

	std::unique_ptr<iModifier> modifier(ModifierFactory::instance().create("HeaderSorter", graph));
	CPPUNIT_ASSERT(modifier);
	CPPUNIT_ASSERT(modifier->isRegistered());
	modifier->process();

	std::ostringstream oss;
	modifier->printReport(oss);
	// CPPUNIT_ASSERT_EQUAL(std::string("File main.cpp has been modified"), oss.str());
	CPPUNIT_ASSERT_EQUAL(std::string("#include <iostream>\n#include <string>\n\nclass Test{\npublic:\nTest();\n};\n"), 
							utils_tests::readFileAsAString(m_dir_base, "main.cpp"));
}
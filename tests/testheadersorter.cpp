#include "testheadersorter.hpp"
#include "utils_tests.hpp"

#include <dependencygraph.hpp>
#include <graphconstructor.hpp>
#include <modifier/imodifier.hpp>

void TestHeaderSorter::SetUp()
{
	m_dir_base = boost::filesystem::path("base");
	ASSERT_TRUE(boost::filesystem::create_directory(m_dir_base)) << "unable to create directory";
}

void TestHeaderSorter::TearDown()
{
	boost::filesystem::remove_all(m_dir_base);
}

TEST_F(TestHeaderSorter, testFileWithNoInclude)
{
	std::string fileContent = "class Test{\npublic:\nTest();\n};\n";
	utils_tests::createFileWithContent(m_dir_base, "main.cpp", fileContent);

	DependencyGraph graph;
	ASSERT_NO_THROW(GraphConstructor::buildGraph(graph, m_dir_base));

	std::unique_ptr<iModifier> modifier(ModifierFactory::instance().create("HeaderSorter", graph));
	ASSERT_TRUE((bool)modifier);
	ASSERT_TRUE(modifier->isRegistered());
	modifier->process();

	std::ostringstream oss;
	modifier->printReport(oss);
	ASSERT_EQ(std::string("Nothing has been changed"), oss.str());
	ASSERT_EQ(fileContent, utils_tests::readFileAsAString(m_dir_base, "main.cpp"));
}

TEST_F(TestHeaderSorter, testSortingTwoSimpleIncludeLines)
{
	std::string fileContent = "#include <string>\n#include<iostream>\n\nclass Test{\npublic:\nTest();\n};\n";
	utils_tests::createFileWithContent(m_dir_base, "main.cpp", fileContent);

	DependencyGraph graph;
	ASSERT_NO_THROW(GraphConstructor::buildGraph(graph, m_dir_base));

	std::unique_ptr<iModifier> modifier(ModifierFactory::instance().create("HeaderSorter", graph));
	ASSERT_TRUE((bool)modifier);
	ASSERT_TRUE(modifier->isRegistered());
	modifier->process();

	std::ostringstream oss;
	modifier->printReport(oss);
	// ASSERT_EQ(std::string("File main.cpp has been modified"), oss.str());
	ASSERT_EQ(std::string("#include <iostream>\n#include <string>\n\nclass Test{\npublic:\nTest();\n};\n"), 
							utils_tests::readFileAsAString(m_dir_base, "main.cpp"));
}
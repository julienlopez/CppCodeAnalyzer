#include "utils_tests.hpp"

#include <dependencygraph.hpp>
#include <graphconstructor.hpp>
#include <packagedependency/packagedependencygraph.hpp>
#include <packagedependency/packagedependencybuilder.hpp>

struct TestPackageDependencyBuilder : public utils_tests::GTestWithFilesBase
{};

TEST_F(TestPackageDependencyBuilder, ConvertTwoFilesInOneClass)
{
	utils_tests::createFile(m_dir_base, "test.cpp", {{"test.hpp"}});
	utils_tests::createEmptyFile(m_dir_base, "test.hpp");

	DependencyGraph g;
	GraphConstructor::buildGraph(g, m_dir_base);
	ASSERT_EQ(2, g.countVertices());

	PackageDependencyGraph pg;
	PackageDependencyBuilder::buildGraph(pg, g, m_dir_base);
	ASSERT_EQ(1, pg.countVertices());
	ASSERT_EQ("test", pg(*g.vertices().first));
}

TEST_F(TestPackageDependencyBuilder, ConvertThreeFilesInOneClass)
{
	utils_tests::createFile(m_dir_base, "test.cpp", {{"test.hpp"}});
	utils_tests::createEmptyFile(m_dir_base, "test.hpp");
	utils_tests::createFile(m_dir_base, "main.cpp", {{"test.hpp"}});

	DependencyGraph g;
	GraphConstructor::buildGraph(g, m_dir_base);
	ASSERT_EQ(3, g.countVertices());

	PackageDependencyGraph pg;
	PackageDependencyBuilder::buildGraph(pg, g, m_dir_base);
	ASSERT_EQ(1, pg.countVertices());
	ASSERT_EQ("test", pg(*g.vertices().first));
}

TEST_F(TestPackageDependencyBuilder, ConvertFiveFilesInTwoClasses)
{
	utils_tests::createFile(m_dir_base, "test.cpp", {{"test.hpp"}});
	utils_tests::createEmptyFile(m_dir_base, "test.hpp");
	utils_tests::createFile(m_dir_base, "main.cpp", {{"test.hpp"}, {"class1.hpp"}});
	utils_tests::createFile(m_dir_base, "class1.cpp", {{"class1.hpp"}});
	utils_tests::createEmptyFile(m_dir_base, "class1.hpp");

	DependencyGraph g;
	GraphConstructor::buildGraph(g, m_dir_base);
	ASSERT_EQ(5, g.countVertices());

	PackageDependencyGraph pg;
	PackageDependencyBuilder::buildGraph(pg, g, m_dir_base);
	ASSERT_EQ(2, pg.countVertices());
}
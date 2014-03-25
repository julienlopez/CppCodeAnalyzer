#include "testgraphconstructor.hpp"
#include "utils_tests.hpp"

#include <dependencygraph.hpp>
#include <graphconstructor.hpp>

#include <fstream>

namespace {
	class CompareGraphVertexByFilePath
	{
	public:
		CompareGraphVertexByFilePath(const DependencyGraph& graph, const std::string& path): m_graph(graph), m_path(path)
		{}

		bool operator()(const DependencyGraph::vertex_descriptor v) const
		{
			return StringHelper::endsWith(m_graph(v)->filePath(), m_path) || StringHelper::endsWith(m_path, m_graph(v)->filePath());
		}

	private:
		const DependencyGraph& m_graph;
		const std::string& m_path;
	};
}

TEST_F(TestGraphConstructor, testOneFolderThreeFilesNoIclusion)
{
	utils_tests::createEmptyFile(m_dir_base, "main.cpp");
	utils_tests::createEmptyFile(m_dir_base, "test.cpp");
	utils_tests::createEmptyFile(m_dir_base, "foo.cpp");

	DependencyGraph graph;
	ASSERT_NO_THROW(GraphConstructor::buildGraph(graph, m_dir_base));

	ASSERT_EQ((std::size_t)3, graph.countVertices());

	DependencyGraph::vertex_iterator i, i_end;
	std::tie(i, i_end) = graph.vertices();
	DependencyGraph::vertex_iterator i_test_cpp = std::find_if(i, i_end, CompareGraphVertexByFilePath(graph, "base/test.cpp"));
	ASSERT_TRUE(i_test_cpp != i_end);
	DependencyGraph::vertex_iterator i_main_cpp = std::find_if(i, i_end, CompareGraphVertexByFilePath(graph, "base/main.cpp"));
	ASSERT_TRUE(i_main_cpp != i_end);
	DependencyGraph::vertex_iterator i_foo_cpp = std::find_if(i, i_end, CompareGraphVertexByFilePath(graph, "base/foo.cpp"));
	ASSERT_TRUE(i_foo_cpp != i_end);
}

TEST_F(TestGraphConstructor, testOneFolderThreeFiles)
{
	utils_tests::createFile(m_dir_base, "main.cpp", {{"test.hpp"}});
	utils_tests::createFile(m_dir_base, "test.cpp", {{"test.hpp"}});
	utils_tests::createEmptyFile(m_dir_base, "test.hpp");

	DependencyGraph graph;
	ASSERT_NO_THROW(GraphConstructor::buildGraph(graph, m_dir_base));

	ASSERT_EQ((std::size_t)3, graph.countVertices());

	DependencyGraph::vertex_iterator i, i_end;
	std::tie(i, i_end) = graph.vertices();
	DependencyGraph::vertex_iterator i_test_hpp = std::find_if(i, i_end, CompareGraphVertexByFilePath(graph, "base/test.hpp"));
	ASSERT_TRUE(i_test_hpp != i_end);
	DependencyGraph::vertex_iterator i_test_cpp = std::find_if(i, i_end, CompareGraphVertexByFilePath(graph, "base/test.cpp"));
	ASSERT_TRUE(i_test_cpp != i_end);
	DependencyGraph::vertex_iterator i_main_cpp = std::find_if(i, i_end, CompareGraphVertexByFilePath(graph, "base/main.cpp"));
	ASSERT_TRUE(i_main_cpp != i_end);
	ASSERT_TRUE(graph.areLinked(*i_test_hpp, *i_main_cpp));
	ASSERT_TRUE(graph.areLinked(*i_test_hpp, *i_test_cpp));
}

TEST_F(TestGraphConstructor, testOneFolderOneSubFolderThreeFiles)
{
	boost::filesystem::path subdir = m_dir_base;
	subdir /= "subdir";
	ASSERT_TRUE(boost::filesystem::create_directory(subdir)) << "unable to create directory";
	utils_tests::createFile(m_dir_base, "main.cpp", {{"subdir/test.hpp"}});
	utils_tests::createFile(subdir, "test.cpp", {{"test.hpp"}});
	utils_tests::createEmptyFile(subdir, "test.hpp");

	DependencyGraph graph;
	ASSERT_NO_THROW(GraphConstructor::buildGraph(graph, m_dir_base));

	ASSERT_EQ((std::size_t)3, graph.countVertices());

	DependencyGraph::vertex_iterator i, i_end;
	std::tie(i, i_end) = graph.vertices();
	DependencyGraph::vertex_iterator i_test_hpp = std::find_if(i, i_end, CompareGraphVertexByFilePath(graph, "base/subdir/test.hpp"));
	ASSERT_TRUE(i_test_hpp != i_end);
	DependencyGraph::vertex_iterator i_test_cpp = std::find_if(i, i_end, CompareGraphVertexByFilePath(graph, "base/subdir/test.cpp"));
	ASSERT_TRUE(i_test_cpp != i_end);
	DependencyGraph::vertex_iterator i_main_cpp = std::find_if(i, i_end, CompareGraphVertexByFilePath(graph, "base/main.cpp"));
	ASSERT_TRUE(i_main_cpp != i_end);
	ASSERT_TRUE(graph.areLinked(*i_test_hpp, *i_main_cpp));
	ASSERT_TRUE(graph.areLinked(*i_test_hpp, *i_test_cpp));
}

/*
TEST_F(TestGraphConstructor, testIncludeLineInvalid)
{
	utils_tests::createFileWithContent(m_dir_base, "main.cpp", "#include blah.hpp");
	DependencyGraph graph;
	try 
	{
		GraphConstructor::buildGraph(graph, m_dir_base);
		CPPUNIT_FAIL("no exception was thrown");
	}
	catch(GraphConstructor::ParsingError& e)
	{
		ASSERT_EQ(std::string("invalid line: blah.hpp"), std::string(e.what()));
	}
	catch(...) 
	{
		CPPUNIT_FAIL("an exception was thrown, but not of type GraphConstructor::ParsingError");
	}
}
*/

/**
* Test the result of analyzing the following setup:
* test of dir
* dir
*  - include
*     - header.hpp
*  - src
*     - main.cpp
*/
TEST_F(TestGraphConstructor, testSplitFoldersOneFileInEach)
{
	boost::filesystem::path subdirInclude = m_dir_base / "include";
	ASSERT_TRUE(boost::filesystem::create_directory(subdirInclude)) << "unable to create directory";
	boost::filesystem::path subdirSrc = m_dir_base / "src";
	ASSERT_TRUE(boost::filesystem::create_directory(subdirSrc)) << "unable to create directory";

	utils_tests::createEmptyFile(subdirInclude, "header.hpp");
	utils_tests::createFile(subdirSrc, "main.cpp", {{"header.hpp"}});

	DependencyGraph graph;
	ASSERT_NO_THROW(GraphConstructor::buildGraph(graph, m_dir_base, {{subdirInclude.generic_string()}}));

	ASSERT_EQ((std::size_t)2, graph.countVertices());

	DependencyGraph::vertex_iterator i, i_end;
	std::tie(i, i_end) = graph.vertices();
	DependencyGraph::vertex_iterator i_header_hpp = std::find_if(i, i_end, CompareGraphVertexByFilePath(graph, "base/include/header.hpp"));
	ASSERT_TRUE(i_header_hpp != i_end);
	DependencyGraph::vertex_iterator i_main_cpp = std::find_if(i, i_end, CompareGraphVertexByFilePath(graph, "base/src/main.cpp"));
	ASSERT_TRUE(i_main_cpp != i_end);

	ASSERT_TRUE(graph.areLinked(*i_header_hpp, *i_main_cpp));
}

TEST_F(TestGraphConstructor, testBothIncludeWithOrWithoutSpaceIsOk)
{
	utils_tests::createFileWithContent(m_dir_base, "main.cpp", "#include \"one.h\"\n#include\"two.h\"\n");
	utils_tests::createEmptyFile(m_dir_base, "one.h");
	utils_tests::createEmptyFile(m_dir_base, "two.h");

	DependencyGraph graph;
	ASSERT_NO_THROW(GraphConstructor::buildGraph(graph, m_dir_base));
	ASSERT_EQ((std::size_t)3, graph.countVertices());
}

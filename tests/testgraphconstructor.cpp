#include "testgraphconstructor.hpp"
#include "utils_tests.hpp"

#include <graphconstructor.hpp>

#include <fstream>

CPPUNIT_TEST_SUITE_REGISTRATION(TestGraphConstructor);

void TestGraphConstructor::setUp()
{
	m_dir_base = boost::filesystem::path("base");
	CPPUNIT_ASSERT_MESSAGE("unable to create directory", boost::filesystem::create_directory(m_dir_base));
}

void TestGraphConstructor::tearDown()
{
	boost::filesystem::remove_all(m_dir_base);
}

namespace {
	class CompareGraphVertexByFilePath
	{
	public:
		CompareGraphVertexByFilePath(const DependencyGraph& graph, const std::string& path): m_graph(graph), m_path(path)
		{}

		bool operator()(const DependencyGraph::vertex_descriptor v) const
		{
			return StringHelper::endsWith(m_graph(v), m_path) || StringHelper::endsWith(m_path, m_graph(v));
		}

	private:
		const DependencyGraph& m_graph;
		const std::string& m_path;
	};
}

void TestGraphConstructor::testOneFolderThreeFilesNoIclusion()
{
	utils_tests::createEmptyFile(m_dir_base, "main.cpp");
	utils_tests::createEmptyFile(m_dir_base, "test.cpp");
	utils_tests::createEmptyFile(m_dir_base, "foo.cpp");

	DependencyGraph graph;
	CPPUNIT_ASSERT_NO_THROW(GraphConstructor::buildGraph(graph, m_dir_base));

	DependencyGraph::vertex_iterator i, i_end;
	std::tie(i, i_end) = graph.vertices();
	CPPUNIT_ASSERT_EQUAL(3, (int)std::distance(i, i_end));

	DependencyGraph::vertex_iterator i_test_cpp = std::find_if(i, i_end, CompareGraphVertexByFilePath(graph, "base/test.cpp"));
	CPPUNIT_ASSERT(i_test_cpp != i_end);
	DependencyGraph::vertex_iterator i_main_cpp = std::find_if(i, i_end, CompareGraphVertexByFilePath(graph, "base/main.cpp"));
	CPPUNIT_ASSERT(i_main_cpp != i_end);
	DependencyGraph::vertex_iterator i_foo_cpp = std::find_if(i, i_end, CompareGraphVertexByFilePath(graph, "base/foo.cpp"));
	CPPUNIT_ASSERT(i_foo_cpp != i_end);
}

void TestGraphConstructor::testOneFolderThreeFiles()
{
	utils_tests::createFile(m_dir_base, "main.cpp", {{"test.hpp"}});
	utils_tests::createFile(m_dir_base, "test.cpp", {{"test.hpp"}});
	utils_tests::createEmptyFile(m_dir_base, "test.hpp");

	DependencyGraph graph;
	CPPUNIT_ASSERT_NO_THROW(GraphConstructor::buildGraph(graph, m_dir_base));

	DependencyGraph::vertex_iterator i, i_end;
	std::tie(i, i_end) = graph.vertices();
	CPPUNIT_ASSERT_EQUAL(3, (int)std::distance(i, i_end));
	DependencyGraph::vertex_iterator i_test_hpp = std::find_if(i, i_end, CompareGraphVertexByFilePath(graph, "base/test.hpp"));
	CPPUNIT_ASSERT(i_test_hpp != i_end);
	DependencyGraph::vertex_iterator i_test_cpp = std::find_if(i, i_end, CompareGraphVertexByFilePath(graph, "base/test.cpp"));
	CPPUNIT_ASSERT(i_test_cpp != i_end);
	DependencyGraph::vertex_iterator i_main_cpp = std::find_if(i, i_end, CompareGraphVertexByFilePath(graph, "base/main.cpp"));
	CPPUNIT_ASSERT(i_main_cpp != i_end);
	CPPUNIT_ASSERT(graph.areLinked(*i_test_hpp, *i_main_cpp));
	CPPUNIT_ASSERT(graph.areLinked(*i_test_hpp, *i_test_cpp));
}

void TestGraphConstructor::testOneFolderOneSubFolderThreeFiles()
{
	boost::filesystem::path subdir = m_dir_base;
	subdir /= "subdir";
	CPPUNIT_ASSERT_MESSAGE("unable to create directory", boost::filesystem::create_directory(subdir));
	utils_tests::createFile(m_dir_base, "main.cpp", {{"subdir/test.hpp"}});
	utils_tests::createFile(subdir, "test.cpp", {{"test.hpp"}});
	utils_tests::createEmptyFile(subdir, "test.hpp");

	DependencyGraph graph;
	CPPUNIT_ASSERT_NO_THROW(GraphConstructor::buildGraph(graph, m_dir_base));

	DependencyGraph::vertex_iterator i, i_end;
	std::tie(i, i_end) = graph.vertices();
	CPPUNIT_ASSERT_EQUAL(3, (int)std::distance(i, i_end));
	DependencyGraph::vertex_iterator i_test_hpp = std::find_if(i, i_end, CompareGraphVertexByFilePath(graph, "base/subdir/test.hpp"));
	CPPUNIT_ASSERT(i_test_hpp != i_end);
	DependencyGraph::vertex_iterator i_test_cpp = std::find_if(i, i_end, CompareGraphVertexByFilePath(graph, "base/subdir/test.cpp"));
	CPPUNIT_ASSERT(i_test_cpp != i_end);
	DependencyGraph::vertex_iterator i_main_cpp = std::find_if(i, i_end, CompareGraphVertexByFilePath(graph, "base/main.cpp"));
	CPPUNIT_ASSERT(i_main_cpp != i_end);
	CPPUNIT_ASSERT(graph.areLinked(*i_test_hpp, *i_main_cpp));
	CPPUNIT_ASSERT(graph.areLinked(*i_test_hpp, *i_test_cpp));
}

void TestGraphConstructor::testIncludeLineInvalid()
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
		CPPUNIT_ASSERT_EQUAL(std::string("invalid line: blah.hpp"), std::string(e.what()));
	}
	catch(...) 
	{
		CPPUNIT_FAIL("an exception was thrown, but not of type GraphConstructor::ParsingError");
	}
}

void TestGraphConstructor::testSplitFoldersOneFileInEach()
{
	boost::filesystem::path subdirInclude = m_dir_base / "include";
	CPPUNIT_ASSERT_MESSAGE("unable to create directory", boost::filesystem::create_directory(subdirInclude));
	boost::filesystem::path subdirSrc = m_dir_base = "src";
	CPPUNIT_ASSERT_MESSAGE("unable to create directory", boost::filesystem::create_directory(subdirSrc));

	utils_tests::createEmptyFile(subdirInclude, "header.hpp");
	utils_tests::createFile(subdirSrc, "main.cpp", {{"header.hpp"}});

	DependencyGraph graph;
	CPPUNIT_ASSERT_NO_THROW(GraphConstructor::buildGraph(graph, m_dir_base, {{"include"}}));

	DependencyGraph::vertex_iterator i, i_end;
	std::tie(i, i_end) = graph.vertices();
	CPPUNIT_ASSERT_EQUAL(2, (int)std::distance(i, i_end));

	DependencyGraph::vertex_iterator i_header_hpp = std::find_if(i, i_end, CompareGraphVertexByFilePath(graph, "base/include/header.hpp"));
	CPPUNIT_ASSERT(i_header_hpp != i_end);
	DependencyGraph::vertex_iterator i_main_cpp = std::find_if(i, i_end, CompareGraphVertexByFilePath(graph, "base/src/main.cpp"));
	CPPUNIT_ASSERT(i_main_cpp != i_end);

	CPPUNIT_ASSERT(graph.areLinked(*i_header_hpp, *i_main_cpp));
}

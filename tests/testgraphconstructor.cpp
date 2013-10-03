#include "testgraphconstructor.hpp"

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
			return m_graph(v) == m_path;
		}

	private:
		const DependencyGraph& m_graph;
		const std::string& m_path;
	};
}

void TestGraphConstructor::testOneFolderThreeFilesNoIclusion()
{
	createEmptyFile(m_dir_base, "main.cpp");
	createEmptyFile(m_dir_base, "test.cpp");
	createEmptyFile(m_dir_base, "foo.cpp");

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
	createFile(m_dir_base, "main.cpp", {{"test.hpp"}});
	createFile(m_dir_base, "test.cpp", {{"test.hpp"}});
	createEmptyFile(m_dir_base, "test.hpp");

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
	createFile(m_dir_base, "main.cpp", {{"subdir/test.hpp"}});
	createFile(subdir, "test.cpp", {{"test.hpp"}});
	createEmptyFile(subdir, "test.hpp");

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
	createFileWithContent(m_dir_base, "main.cpp", "#include blah.hpp");
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
	boost::filesystem::path subdirInclude = m_dir_base;
	subdirInclude /= "include";
	CPPUNIT_ASSERT_MESSAGE("unable to create directory", boost::filesystem::create_directory(subdirInclude));
	boost::filesystem::path subdirSrc = m_dir_base;
	subdirSrc /= "src";
	CPPUNIT_ASSERT_MESSAGE("unable to create directory", boost::filesystem::create_directory(subdirSrc));

	createEmptyFile(subdirInclude, "header.hpp");
	createFile(subdirSrc, "main.cpp", {{"header.hpp"}});

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

void TestGraphConstructor::createEmptyFile(boost::filesystem::path dir, const std::string& fileName)
{
	dir /= boost::filesystem::path(fileName);
	std::ofstream ofs(dir.generic_string().c_str());
	ofs << "\n";
}

void TestGraphConstructor::createFile(boost::filesystem::path dir, const std::string& fileName, const std::list<std::string>& includes)
{
	dir /= boost::filesystem::path(fileName);
	std::ofstream ofs(dir.generic_string().c_str());
	for(const auto& file : includes)
	{
		ofs << "#include \"" << file << "\"\n";
	}
}

void TestGraphConstructor::createFileWithContent(boost::filesystem::path dir, const std::string& fileName, const std::string& text)
{
	dir /= boost::filesystem::path(fileName);
	std::ofstream ofs(dir.generic_string().c_str());
	ofs << text;
}

#include "testdependencygraph.hpp"

#include <dependencygraph.hpp>

CPPUNIT_TEST_SUITE_REGISTRATION(TestDependencyGraph);

void TestDependencyGraph::testPrint()
{
	DependencyGraph graph;
	graph.addLien("A1", "A2");
	std::ostringstream oss;
	graph.print(oss);
	CPPUNIT_ASSERT_EQUAL(std::string("digraph G {\n0[label=\"A1\"];\n1[label=\"A2\"];\n0->1 ;\n}\n"), oss.str());
}
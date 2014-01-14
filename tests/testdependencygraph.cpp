#include <gtest/gtest.h>

#include <dependencygraph.hpp>

TEST(TestDependencyGraph, testPrint)
{
	DependencyGraph graph;
	graph.addEdge("A1", "A2");
	std::ostringstream oss;
	graph.print(oss);
	ASSERT_EQ(std::string("digraph G {\n0[label=\"A1\"];\n1[label=\"A2\"];\n0->1 ;\n}\n"), oss.str());
}
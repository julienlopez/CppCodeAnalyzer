#include <gtest/gtest.h>

#include <dependencygraph.hpp>
#include <analyzer/analyzerfactory.hpp>
#include <analyzer/ianalyzer.hpp>

TEST(TestAnalyzerFactory, testCreationInvalidAnalyzer)
{
	DependencyGraph graph;
	std::string id("invalid");
	ASSERT_THROW(AnalyzerFactory::createAnalyzer("invalid", graph), AnalyzerFactory::type_factory::error_policy::Exception);
	try {
		AnalyzerFactory::createAnalyzer(id, graph);
	}
	catch(AnalyzerFactory::type_factory::error_policy::Exception& e)
	{
		ASSERT_EQ(std::string("Unknown type identifier passed to the factory."), std::string(e.what()));
		ASSERT_EQ(id, e.getId());
	}
}

TEST(TestAnalyzerFactory, testCreationRedondantHeaderAnalyzer)
{
	DependencyGraph graph;
	std::unique_ptr<iAnalyzer> analyzer(AnalyzerFactory::createAnalyzer("RedondantHeaderAnalyzer", graph));
	ASSERT_TRUE((bool)analyzer);
	ASSERT_TRUE(analyzer->isRegistered());
}

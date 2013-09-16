#include "testanalyzerfactory.hpp"

#include <dependencygraph.hpp>
#include <analyzer/analyzerfactory.hpp>
#include <analyzer/ianalyzer.hpp>

CPPUNIT_TEST_SUITE_REGISTRATION(TestAnalyzerFactory);

void TestAnalyzerFactory::testCreationInvalidAnalyzer()
{
	DependencyGraph graph;
	std::string id("invalid");
	CPPUNIT_ASSERT_THROW(AnalyzerFactory::createAnalyzer("invalid", graph), AnalyzerFactory::type_factory::error_policy::Exception);
	try {
		AnalyzerFactory::createAnalyzer(id, graph);
	}
	catch(AnalyzerFactory::type_factory::error_policy::Exception& e)
	{
		CPPUNIT_ASSERT_EQUAL(std::string("Unknown type identifier passed to the factory."), std::string(e.what()));
		CPPUNIT_ASSERT_EQUAL(id, e.getId());
	}
}

void TestAnalyzerFactory::testCreationRedondantHeaderAnalyzer()
{
	DependencyGraph graph;
	std::unique_ptr<iAnalyzer> analyzer(AnalyzerFactory::createAnalyzer("RedondantHeaderAnalyzer", graph));
	CPPUNIT_ASSERT(analyzer);
	CPPUNIT_ASSERT(analyzer->isRegistered());
}

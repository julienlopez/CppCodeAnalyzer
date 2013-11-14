#include "testredondantheaderanalyzer.hpp"

#include <dependencygraph.hpp>
#include <analyzer/analyzerfactory.hpp>
#include <analyzer/ianalyzer.hpp>

#include <sstream>

CPPUNIT_TEST_SUITE_REGISTRATION(TestRedondantHeaderAnalyzer);

void TestRedondantHeaderAnalyzer::testStraightInclusionOfThreeFiles()
{
	DependencyGraph graph;
	graph.addLien("header1.hpp", "main.cpp");
	graph.addLien("header2.hpp", "header1.hpp");

	std::unique_ptr<iAnalyzer> analyzer(AnalyzerFactory::createAnalyzer("RedondantHeaderAnalyzer", graph));
	CPPUNIT_ASSERT(analyzer);
	CPPUNIT_ASSERT(analyzer->isRegistered());
	analyzer->analyze();

	std::stringstream ioss;
	analyzer->printReport(ioss);

	std::string res("Redondant header analysis report:\nNothing detected\n");
	CPPUNIT_ASSERT_EQUAL(res, ioss.str());
}
void TestRedondantHeaderAnalyzer::testYShapedInclusionOfThreeFiles()
{
	DependencyGraph graph;
	graph.addLien("header1.hpp", "main.cpp");
	graph.addLien("header2.hpp", "main.cpp");

	std::unique_ptr<iAnalyzer> analyzer(AnalyzerFactory::createAnalyzer("RedondantHeaderAnalyzer", graph));
	CPPUNIT_ASSERT(analyzer);
	CPPUNIT_ASSERT(analyzer->isRegistered());
	analyzer->analyze();

	std::stringstream ioss;
	analyzer->printReport(ioss);

	std::string res("Redondant header analysis report:\nNothing detected\n");
	CPPUNIT_ASSERT_EQUAL(res, ioss.str());
}

void TestRedondantHeaderAnalyzer::testDiamondShapedInclusionOfFourFiles()
{
	DependencyGraph graph;
	graph.addLien("header1.hpp", "main.cpp");
	graph.addLien("header2.hpp", "main.cpp");
	graph.addLien("global.hpp", "header1.hpp");
	graph.addLien("global.hpp", "header2.hpp");

	std::unique_ptr<iAnalyzer> analyzer(AnalyzerFactory::createAnalyzer("RedondantHeaderAnalyzer", graph));
	CPPUNIT_ASSERT(analyzer);
	CPPUNIT_ASSERT(analyzer->isRegistered());
	analyzer->analyze();

	std::stringstream ioss;
	analyzer->printReport(ioss);

	std::string res("Redondant header analysis report:\nNothing detected\n");
	CPPUNIT_ASSERT_EQUAL(res, ioss.str());
}

void TestRedondantHeaderAnalyzer::testDiamondShapedInclusionOfFourFilesAndBottomAlsoIncludeTop()
{
	DependencyGraph graph;
	graph.addLien("header1.hpp", "main.cpp");
	graph.addLien("header2.hpp", "main.cpp");
	graph.addLien("global.hpp", "header1.hpp");
	graph.addLien("global.hpp", "header2.hpp");
	graph.addLien("global.hpp", "main.cpp");

	std::unique_ptr<iAnalyzer> analyzer(AnalyzerFactory::createAnalyzer("RedondantHeaderAnalyzer", graph));
	CPPUNIT_ASSERT(analyzer);
	CPPUNIT_ASSERT(analyzer->isRegistered());
	analyzer->analyze();

	std::stringstream ioss;
	analyzer->printReport(ioss);

	std::string res("Redondant header analysis report:\nIn file \"main.cpp\": redondant inclusion of \"global.hpp\". It is already included by \"header1.hpp\"\n");
	res += "In file \"main.cpp\": redondant inclusion of \"global.hpp\". It is already included by \"header2.hpp\"\n";
	CPPUNIT_ASSERT_EQUAL(res, ioss.str());
}

void TestRedondantHeaderAnalyzer::testStraightInclusionOfThreeFilesWithTheBottomOneAlsoIncludingTop()
{
	DependencyGraph graph;
	graph.addLien("header1.hpp", "main.cpp");
	graph.addLien("header2.hpp", "header1.hpp");
	graph.addLien("header2.hpp", "main.cpp");

	std::unique_ptr<iAnalyzer> analyzer(AnalyzerFactory::createAnalyzer("RedondantHeaderAnalyzer", graph));
	CPPUNIT_ASSERT(analyzer);
	CPPUNIT_ASSERT(analyzer->isRegistered());
	analyzer->analyze();

	std::stringstream ioss;
	analyzer->printReport(ioss);

	std::string res("Redondant header analysis report:\nIn file \"main.cpp\": redondant inclusion of \"header2.hpp\". It is already included by \"header1.hpp\"\n");
	CPPUNIT_ASSERT_EQUAL(res, ioss.str());
}

	/**
	* main.cpp -> header1.hpp -> header2.hpp -> header3.hpp and main.cpp -> header3.hpp <br />
	* should find main.cpp including header3.hpp as a problem
	*/
void TestRedondantHeaderAnalyzer::testStraightInclusionOfFourFilesWithTheBottomOneAlsoIncludingTop()
{
	DependencyGraph graph;
	graph.addLien("header1.hpp", "main.cpp");
	graph.addLien("header2.hpp", "header1.hpp");
	graph.addLien("header3.hpp", "header2.hpp");
	graph.addLien("header3.hpp", "main.cpp");

	std::unique_ptr<iAnalyzer> analyzer(AnalyzerFactory::createAnalyzer("RedondantHeaderAnalyzer", graph));
	CPPUNIT_ASSERT(analyzer);
	CPPUNIT_ASSERT(analyzer->isRegistered());
	analyzer->analyze();

	std::stringstream ioss;
	analyzer->printReport(ioss);

	std::string res("Redondant header analysis report:\nIn file \"main.cpp\": redondant inclusion of \"header3.hpp\". It is already included by \"header1.hpp\"\n");
	CPPUNIT_ASSERT_EQUAL(res, ioss.str());
}

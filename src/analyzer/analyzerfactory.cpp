#include "analyzerfactory.hpp"
#include "ianalyzer.hpp"

iAnalyzer* AnalyzerFactory::createAnalyzer(const std::string& analyzerName, const DependencyGraph& graph)
{
	return type_factory::instance().create(analyzerName, graph);
}
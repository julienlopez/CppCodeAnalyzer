#include "ianalyzer.hpp"

iAnalyzer::iAnalyzer(const DependencyGraph& g): m_graph(g)
{}

const DependencyGraph& iAnalyzer::graph() const
{
	return m_graph;
}

void iAnalyzer::analyze()
{
	do_analyze();
}

void iAnalyzer::printReport(std::ostream& out) const
{
	do_printReport(out);
}

bool iAnalyzer::isRegistered() const
{
	return do_isRegistered();	
}
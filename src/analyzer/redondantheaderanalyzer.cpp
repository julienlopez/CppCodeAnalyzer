#include "redondantheaderanalyzer.hpp"

const std::string RedondantHeaderAnalyzer::s_name = "RedondantHeaderAnalyzer";

RedondantHeaderAnalyzer::RedondantHeaderAnalyzer(const DependencyGraph& g) noexcept: CRTP_Analyzer<RedondantHeaderAnalyzer>(g)
{}

void RedondantHeaderAnalyzer::do_analyze()
{
	const DependencyGraph& g = graph();
	DependencyGraph::vertex_iterator i, i_end;
	std::tie(i, i_end) = g.vertices();
	for(; i != i_end; ++i)
	{
		analyseVertex(g, i);
	}
}

void RedondantHeaderAnalyzer::do_printReport(std::ostream& out) const
{
	out << "Redondant header analysis report:" << std::endl << std::endl;
}

void RedondantHeaderAnalyzer::analyseVertex(const DependencyGraph& g, DependencyGraph::vertex_iterator it)
{
	std::cerr << g(*it) << " inclue : " << std::endl;
	std::list<DependencyGraph::vertex_descriptor> parents = g.parents(*it);
	for(const DependencyGraph::vertex_descriptor& d : parents)
	{
		std::cerr << g(d) << std::endl;
		for(const DependencyGraph::vertex_descriptor& d2 : parents)
		{
			if(d == d2) continue;
			if(g.areLinked(d, d2))
			{
				std::cerr << "include redondant: " << g(d) << " et " << g(d2) << std::endl;
			}
		}
	}
	std::cerr << std::endl;
}
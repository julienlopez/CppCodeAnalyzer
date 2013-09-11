#include "redondantheaderanalyzer.hpp"

const std::string RedondantHeaderAnalyzer::s_name = "RedondantHeaderAnalyzer";

RedondantHeaderAnalyzer::RedondantHeaderAnalyzer(const DependencyGraph& g) noexcept: CRTP_Analyzer<RedondantHeaderAnalyzer>(g)
{}

void RedondantHeaderAnalyzer::do_analyze()
{
	m_datas.clear();
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
	const DependencyGraph& g = graph();
	out << "Redondant header analysis report:" << std::endl;
	for(const ReportData& data : m_datas)
	{
		out << "In file " << g(data.m_sourceFile) << ": redondant inclusion of " << g(data.m_redondantHeader) << 
				". It is already included by " << g(data.m_headerAlreadyIncludingIt) << std::endl;
	}
}

void RedondantHeaderAnalyzer::analyseVertex(const DependencyGraph& g, DependencyGraph::vertex_iterator it)
{
	std::list<DependencyGraph::vertex_descriptor> parents = g.parents(*it);
	for(const DependencyGraph::vertex_descriptor& d : parents)
	{
		for(const DependencyGraph::vertex_descriptor& d2 : parents)
		{
			if(d == d2) continue;
			if(g.areLinked(d, d2))
				m_datas.push_back(ReportData(*it, d, d2));
		}
	}
}
#ifndef __REDONDANTHEADERANALYZER_HPP__
#define __REDONDANTHEADERANALYZER_HPP__

#include "crtp_analyzer.hpp"
#include <dependencygraph.hpp>

class RedondantHeaderAnalyzer : public CRTP_Analyzer<RedondantHeaderAnalyzer>
{
public:
	static const std::string s_name;

	virtual ~RedondantHeaderAnalyzer() noexcept = default;

	RedondantHeaderAnalyzer() = delete;

	RedondantHeaderAnalyzer(const DependencyGraph& g) noexcept;

private:
	virtual void do_analyze() override;

	virtual void do_printReport(std::ostream& out) const override;

	void analyseVertex(const DependencyGraph& g, DependencyGraph::vertex_iterator it);

	struct ReportData
	{
		typedef DependencyGraph::vertex_descriptor vertex_descriptor;
		vertex_descriptor m_sourceFile;
		vertex_descriptor m_redondantHeader;
		vertex_descriptor m_headerAlreadyIncludingIt;

		ReportData(vertex_descriptor sourceFile, vertex_descriptor redondantHeader, vertex_descriptor headerAlreadyIncludingIt):
			m_sourceFile(sourceFile), m_redondantHeader(redondantHeader), m_headerAlreadyIncludingIt(headerAlreadyIncludingIt)
		{}
	};

	std::list<ReportData> m_datas;
};

#endif
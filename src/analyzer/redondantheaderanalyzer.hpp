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
};

#endif
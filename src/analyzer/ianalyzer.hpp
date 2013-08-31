#ifndef __IANALYZER_HPP__
#define __IANALYZER_HPP__

#include <iostream>

class DependencyGraph;

class iAnalyzer
{
public:
	virtual ~iAnalyzer() = default;

	iAnalyzer& operator=(const iAnalyzer&) = delete;
	iAnalyzer& operator=(iAnalyzer&&) = delete;

	virtual const DependencyGraph& graph() const final;

	void analyze();

	void printReport(std::ostream& out) const;

	bool isRegistered() const;

protected:
	iAnalyzer() = delete;
	iAnalyzer(const DependencyGraph& graph);
	iAnalyzer(const iAnalyzer&) = default;
	iAnalyzer(iAnalyzer&&) = default;

private:
	virtual void do_analyze() = 0;

	virtual void do_printReport(std::ostream& out) const = 0;

	virtual bool do_isRegistered() const = 0;

	const DependencyGraph& m_graph;
};

#endif
#ifndef __HEADER_SORTER_HPP__
#define __HEADER_SORTER_HPP__

#include "crtp_modifier.hpp"

#include "dependencygraph.hpp"

class HeaderSorter : public CRTP_Modifier<HeaderSorter>
{
public:
	static const std::string s_name;

	HeaderSorter(DependencyGraph& g);

private:
	virtual void do_process();
	virtual void do_printReport(std::ostream&) const;

	void processFile(DependencyGraph::vertex_descriptor it);

};

#endif
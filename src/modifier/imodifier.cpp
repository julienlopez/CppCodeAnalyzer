#include "imodifier.hpp"

iModifier::iModifier(DependencyGraph& g): m_graph(g)
{}

DependencyGraph& iModifier::graph()
{
	return m_graph;
}

void iModifier::process()
{
	do_process();
}

void iModifier::printReport(std::ostream& out) const
{
	do_printReport(out);
}

bool iModifier::isRegistered() const
{
	return do_isRegistered();	
}

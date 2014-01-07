#ifndef __IMODIFIER_HPP__
#define __IMODIFIER_HPP__

#include <iosfwd>

#include <utils/abstractfactory.hpp>

class DependencyGraph;

class iModifier
{
public:
	virtual ~iModifier() = default;

	iModifier& operator=(const iModifier&) = delete;
	iModifier& operator=(iModifier&&) = delete;

	virtual DependencyGraph& graph() final;

	void process();

	void printReport(std::ostream& out) const;

	bool isRegistered() const;

protected:
	iModifier() = delete;
	iModifier(DependencyGraph& g);
	iModifier(const iModifier&) = default;
	iModifier(iModifier&&) = default;

private:
	virtual void do_process() = 0;

	virtual void do_printReport(std::ostream& out) const = 0;

	virtual bool do_isRegistered() const = 0;

	DependencyGraph& m_graph;
};

using ModifierFactory = utils::AbstractFactory<iModifier, std::string, DependencyGraph&, iModifier*(*)(DependencyGraph&)>;

#endif
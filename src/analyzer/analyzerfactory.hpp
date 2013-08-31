#ifndef __ANALYZERFACTORY_HPP__
#define __ANALYZERFACTORY_HPP__

#include <utils/noninstanciable.hpp>
#include <utils/abstractfactory.hpp>

class iAnalyzer;
class DependencyGraph;

class AnalyzerFactory : public utils::noninstanciable
{
public:
	typedef utils::AbstractFactory<iAnalyzer, std::string, iAnalyzer*(*)(const DependencyGraph&)> type_factory;

	static iAnalyzer* createAnalyzer(const std::string& analyzerName, const DependencyGraph& graph);
};

#endif
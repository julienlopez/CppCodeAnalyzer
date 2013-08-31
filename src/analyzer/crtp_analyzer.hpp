#ifndef __CRTP_ANALYZER_HPP__
#define __CRTP_ANALYZER_HPP__

#include "ianalyzer.hpp"
#include "analyzerfactory.hpp"

template<class T> class CRTP_Analyzer : public iAnalyzer
{
protected:
    template<typename... Args> CRTP_Analyzer(Args&&... args): iAnalyzer(std::forward<Args>(args)...)
    {}

public:
	static const bool s_isRegistered;

	virtual bool do_isRegistered() const override final
	{
		return s_isRegistered;
	}
};

namespace 
{
	template<class T> iAnalyzer* createAnalyzer(const DependencyGraph& g)
	{ 
		return new T(g); 
	}
}

template<class T> const bool CRTP_Analyzer<T>::s_isRegistered = AnalyzerFactory::type_factory::instance().registerProduct(T::s_name, &createAnalyzer<T>);

#endif
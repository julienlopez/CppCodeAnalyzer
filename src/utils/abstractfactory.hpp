#ifndef __ABSTRACTFACTORY_HPP__
#define __ABSTRACTFACTORY_HPP__

#include "singleton.hpp"
#include "defaultfactoryerrorpolicy.hpp"

#include <map>

class DependencyGraph;

BEGIN_NAMESPACE_UTILS

template<	class AbstractProduct,
			class IdentifierType,
			class ProductCreator = AbstractProduct*(*)(),
			template<typename, class> class FactoryErrorPolicy = DefaultFactoryErrorPolicy
			>
class AbstractFactory : public FactoryErrorPolicy<IdentifierType, AbstractProduct>, public Singleton<AbstractFactory<AbstractProduct, IdentifierType, ProductCreator, FactoryErrorPolicy> >
{
public:
    typedef FactoryErrorPolicy<IdentifierType, AbstractProduct> error_policy;

    AbstractProduct* create(typename const_parameter_trait<IdentifierType>::type id)
	{
		typename AssocMap::const_iterator i = m_associations.find(id);
        if(i != m_associations.end()) return (i->second)();
        return error_policy::onUnknownType(id);
	}

    AbstractProduct* create(typename const_parameter_trait<IdentifierType>::type id, const DependencyGraph& graph)
	{
		typename AssocMap::const_iterator i = m_associations.find(id);
        if(i != m_associations.end()) return (i->second)(graph);
        return error_policy::onUnknownType(id);
	}
	
    bool registerProduct(typename const_parameter_trait<IdentifierType>::type id, ProductCreator creator)
	{
        return m_associations.insert(typename AssocMap::value_type(id, creator)).second;
	}
	
    bool unregisterProduct(typename const_parameter_trait<IdentifierType>::type id)
	{
		return m_associations.erase(id) == 1;
	}
	
private:
    typedef std::map<IdentifierType, ProductCreator> AssocMap;
	AssocMap m_associations;
};

END_NAMESPACE_UTILS

#endif

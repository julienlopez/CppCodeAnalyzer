#ifndef __DEFAULTFACTORYERRORPOLICY__
#define __DEFAULTFACTORYERRORPOLICY__

#include <utils/parametertrait.hpp>

#include <stdexcept>

BEGIN_NAMESPACE_UTILS

template<class IdentifierType, class ProductType> class DefaultFactoryErrorPolicy
{
public:
	class Exception : public std::exception
	{
	public:
        Exception(typename const_parameter_trait<IdentifierType>::type id) : std::exception(), m_unknownId(id)
		{}
		
		virtual const char* what()
		{
			return "Type inconnu passé à la factory.";
		}
		
        typename const_parameter_trait<IdentifierType>::type getId() const
		{
			return m_unknownId;
		}
		
	private:
		IdentifierType m_unknownId;
	};
	
protected:
    static ProductType* onUnknownType(typename const_parameter_trait<IdentifierType>::type id)
	{
		throw Exception(id);
	}
};

END_NAMESPACE_UTILS

#endif

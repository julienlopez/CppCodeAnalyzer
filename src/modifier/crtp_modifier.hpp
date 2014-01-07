#ifndef __CRTP_MODIFIER_HPP__
#define __CRTP_MODIFIER_HPP__

#include "imodifier.hpp"

template<class T> class CRTP_Modifier : public iModifier
{
protected:
    template<typename... Args> CRTP_Modifier(Args&&... args): iModifier(std::forward<Args>(args)...)
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
	template<class T> iModifier* createModifier(DependencyGraph& g)
	{ 
		return new T(g); 
	}
}

template<class T> const bool CRTP_Modifier<T>::s_isRegistered = ModifierFactory::instance().registerProduct(T::s_name, &createModifier<T>);

#endif
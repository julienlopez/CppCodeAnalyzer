/*
     Copyright (C) 2001-2011 Dynalogic

*/

#ifndef COMPARATOR_H
#define COMPARATOR_H

#include <utils/parametertrait.hpp>

BEGIN_NAMESPACE_UTILS

/**
 * \brief A basic utility class to allow for easy setting up comporation classes.
 */
template <typename T, typename PARAM = T> class comparator 
{
public:
	using type_param = typename const_parameter_trait<PARAM>::type;

protected:
	comparator(type_param t): d_t(t) 
	{}

	type_param get() const 
	{
		return d_t;
	}
private:
	PARAM d_t;
};


END_NAMESPACE_UTILS

#endif //COMPARATOR_H

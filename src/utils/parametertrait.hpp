#ifndef __PARAMETERTRAIT_HPP__
#define __PARAMETERTRAIT_HPP__

#include <utils/utils.hpp>

#include <type_traits>

BEGIN_NAMESPACE_UTILS

template<class T> struct parameter_trait {
    typedef typename std::conditional<std::is_scalar<T>::value, T, typename std::add_lvalue_reference<T>::type>::type type;
};

template<class T> struct const_parameter_trait {
    typedef typename parameter_trait<typename std::add_const<T>::type>::type type;
};

END_NAMESPACE_UTILS


#endif

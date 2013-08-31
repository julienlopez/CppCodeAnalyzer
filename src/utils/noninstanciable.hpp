#ifndef NONINSTANCIABLE_HPP
#define NONINSTANCIABLE_HPP

#include "utils.hpp"

BEGIN_NAMESPACE_UTILS

/**
  * \brief Defines a non-instanciable class.
  */
class noninstanciable
{
    noninstanciable() = delete;
    noninstanciable(const noninstanciable&) = delete;
    noninstanciable(noninstanciable&&) = delete;

    noninstanciable& operator=(const noninstanciable&) = delete;
    noninstanciable& operator=(noninstanciable&&) = delete;

    ~noninstanciable() = delete;
};

END_NAMESPACE_UTILS

#endif // NONINSTANCIABLE_HPP

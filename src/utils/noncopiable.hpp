#ifndef NONCOPIABLE_HPP
#define NONCOPIABLE_HPP

#include "utils.hpp"

BEGIN_NAMESPACE_UTILS

/**
  * \brief Defines a non-copiable class.
  */
class noncopiable
{
protected:
    noncopiable() = default;

    noncopiable(const noncopiable&) = delete;
    noncopiable(noncopiable&&) = delete;

    noncopiable& operator=(const noncopiable&) = delete;
    noncopiable& operator=(noncopiable&&) = delete;
};

END_NAMESPACE_UTILS

#endif // NONCOPIABLE_HPP

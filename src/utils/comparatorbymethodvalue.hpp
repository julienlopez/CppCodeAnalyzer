/*
     Copyright (C) 2001-2011 Dynalogic

*/

#ifndef COMPARATORBYMETHODVALUE_HPP
#define COMPARATORBYMETHODVALUE_HPP

#include <utils/comparator.hpp>

#include <functional>

BEGIN_NAMESPACE_UTILS

template<typename CLASS, typename T> class comparator_by_method_value : public comparator<CLASS, T>
{
    using base = comparator<CLASS, T>;

public:
    using type_callback = std::function<T (const CLASS&)>;

    comparator_by_method_value(const type_callback& callback, typename base::type_param value): base(value), m_callback(callback)
    {}

    bool operator()(const CLASS& p) const
    {
        return m_callback(p) == base::get();
    }

private:
    type_callback m_callback;
};


END_NAMESPACE_UTILS

#endif //COMPARATORBYMETHODVALUE_HPP

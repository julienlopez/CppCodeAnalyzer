#ifndef SINGLETON_HPP
#define SINGLETON_HPP

#include <utils/noncopiable.hpp>

BEGIN_NAMESPACE_UTILS

template<class T> class Singleton : public noncopiable {
public:
    static T& instance() {
        static T i;
        return i;
    }

protected:
    Singleton() {}
};

END_NAMESPACE_UTILS

#endif // SINGLETON_HPP

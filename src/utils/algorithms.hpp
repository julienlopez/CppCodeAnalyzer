#ifndef __ALGORITHMS_HPP__
#define __ALGORITHMS_HPP__

#include <utils/utils.hpp>

BEGIN_NAMESPACE_UTILS

/**
* slide a selection (begining at f and ending at l) to the position p.
* IT models a RandomAccessIterator
*/
template<class IT>
auto slide(IT f, IT l, IT p) -> std::pair<IT, IT>
{
	if(p < f) return {p, std::rotate(p, f, l)};
	if(l < p) return {std::rotate(f, l, p), p};
	return {f, l};
}

template <class ArgType, class Pred>
std::function<bool(ArgType)> negate_predicat(Pred &pred)
{
	return [&pred](ArgType t) { return !pred(t); };
}

/**
* move all the elements for which the predicat pred returns true to the position target.
* IT models BidirectionalIterator
* S models UnaryPredicate
*/
template<class IT, class PREDICAT>
auto gather(IT begin, IT end, IT target, PREDICAT pred) -> std::pair<IT, IT>
{
	using parameter_type = typename std::iterator_traits<IT>::value_type;
	return 
	{
		std::stable_partition(begin, target, negate_predicat<parameter_type, PREDICAT>(pred)),
		std::stable_partition(target, end, pred)
	};
}

END_NAMESPACE_UTILS

#endif
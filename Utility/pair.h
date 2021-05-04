#ifndef PHI__define_guard__Utility__pair_h
#define PHI__define_guard__Utility__pair_h

#include "../define.h"

namespace phi {

template<typename First, typename Second = First> struct pair {
	First first;
	Second second;

	pair() = default;

	// template<typename First_>
	// pair(First_&& first_): first(Forward<First_>(first_)) {}

	template<typename First_, typename Second_>
	pair(First_&& first_, Second_&& second_):
		first(Forward<First_>(first_)), second(Forward<Second_>(second_)) {}

	template<typename Pair>
	pair(const Pair& p): first(p.first), second(p.second) {}

	pair& operator=(const pair& p) {
		if (this == &p) { return *this; }
		this->first = p.first;
		this->second = p.second;
		return *this;
	}
};

template<typename First, typename Second>
bool operator==(const pair<First, Second>& x, const pair<First, Second>& y) {
	if (&x == &y) { return true; }
	return x.first == y.first && x.first == y.first;
}

template<typename First, typename Second>
bool operator!=(const pair<First, Second>& x, const pair<First, Second>& y) {
	return !(x == y);
}

struct DefaultPairFullComparer_ {
	DefaultFullComparer full_cmper;

	template<typename Pair> int operator()(const Pair& x, Pair& y) const {
		return this->full_cmper(x.first, y.first);
	}
};

using DefaultPairFullComparer =
	AutoImplementFullComparer<DefaultPairFullComparer_>;

}

#endif
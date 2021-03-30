#ifndef PHI__define_guard__Utility__pair_h
#define PHI__define_guard__Utility__pair_h

#include "../define.h"

namespace phi {

template<typename First, typename Second = First> struct pair {
	First first;
	Second second;

	pair() = default;

	template<typename First_, typename Second_>
	pair(First_&& first_, Second_&& second_):
		first(Forward<First_>(first_)), second(Forward<Second_>(second_)) {}
};

}

#endif
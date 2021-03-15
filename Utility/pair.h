#ifndef PHI__define_guard__Utility__pair_h
#define PHI__define_guard__Utility__pair_h

#include "../define.h"

namespace phi {

template<typename T1, typename T2 = T1> struct pair {
	T1 first;
	T2 second;

	pair(){};

	template<typename Y1, typename Y2>
	pair(Y1&& first_, Y2&& second_):
		first(Forward<Y1>(first_)), second(Forward<Y2>(second_)) {}
};

}

#endif
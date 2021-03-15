#ifndef PHI__define_guard__Utility__triple_h
#define PHI__define_guard__Utility__triple_h

#include "../define.h"

namespace phi {

template<typename T1, typename T2 = T1, typename T3 = T2> struct triple {
	T1 first;
	T2 second;
	T3 third;

	triple() {}

	template<typename Y1, typename Y2, typename Y3>
	triple(Y1&& first_, Y2&& second_, Y3&& third_):
		first(Forward<Y1>(first_)), second(Forward<Y2>(second_)),
		third(Forward<Y3>(third_)) {}
};

}

#endif
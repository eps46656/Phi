#ifndef PHI__define_guard__Utility__triple_h
#define PHI__define_guard__Utility__triple_h

#include "../define.h"

namespace phi {

template<typename First, typename Second = First, typename Third = Second>
struct triple {
	First first;
	Second second;
	Third third;

	triple() = default;

	template<typename First_, typename Second_, typename Third_>
	triple(First_&& first_, Second_&& second_, Third_&& third_):
		first(Forward<First_>(first_)), second(Forward<Second_>(second_)),
		third(Forward<Third_>(third_)) {}
};

}

#endif
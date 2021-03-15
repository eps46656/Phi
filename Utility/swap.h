#ifndef PHI__define_guard__Utility__swap_h
#define PHI__define_guard__Utility__swap_h

#include "../define.h"

namespace phi {

template<typename T> void DefaultSwap(T& x, T& y) {
	if (&x == &y) { return; }
	T temp(Move(x));
	x = Move(y);
	y = Move(temp);
}

template<typename T> void Swap(T& x, T& y) { DefaultSwap(x, y); }

struct DefaultSwapper {
public:
	template<typename T> void operator()(T& x, T& y) const { Swap(x, y); }
};

}

#endif
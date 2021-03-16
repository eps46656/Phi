#ifndef PHI__define_guard__Utility__swap_h
#define PHI__define_guard__Utility__swap_h

#include "../define.h"

namespace phi {

template<typename X, typename Y> void DefaultSwap(X&& x, Y&& y) {
	if (&x == &y) { return; }
	auto temp(Move(x));
	x = Move(y);
	y = Move(temp);
}

template<typename X, typename Y> void Swap(X&& x, Y&& y) { DefaultSwap(x, y); }

struct DefaultSwapper {
public:
	template<typename X, typename Y> void operator()(X&& x, Y&& y) const {
		Swap(x, y);
	}
};

}

#endif
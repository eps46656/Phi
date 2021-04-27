#ifndef PHI__define_guard__Utility__Random_h
#define PHI__define_guard__Utility__Random_h

#include <cstdlib>
#include <ctime>
#include "../define.h"
#include "memory.h"
#include "swap.h"

namespace phi {

inline void srand(long int seed) { srand48(seed); }

inline long int srand() {
	time_t t;
	long int seed((long int)time(&t));
	srand(seed);
	return seed;
}

inline unsigned long int rand() { return lrand48(); }

inline unsigned int rand(unsigned int lower, unsigned int upper) {
	return rand() % (upper - lower) + lower;
}

template<typename Src, typename Swapper = DefaultSwapper>
void shuffle(Src& src, size_t size, size_t swap_times = 0,
			 Swapper swapper = Swapper()) {
	if (swap_times == 0) { swap_times = size * 2; }

	while (swap_times != 0) {
		size_t a(rand(0, size));
		size_t b(rand(0, size));
		if (a == b) { continue; }
		swapper(src[a], src[b]);
		--swap_times;
	}
}

}

#endif
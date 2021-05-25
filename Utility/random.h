#ifndef PHI__define_guard__Utility__Random_h
#define PHI__define_guard__Utility__Random_h

#include <cstdlib>
#include <ctime>
#include "../define.h"
#include "memory.h"
#include "swap.h"

namespace phi {

inline long int SetRandom(long int seed) {
	srand48(seed);
	return seed;
}

inline long int SetRandom() {
	time_t t;
	long int seed((long int)time(&t));
	SetRandom(seed);
	return seed;
}

inline unsigned long int Random() { return lrand48(); }

inline unsigned int Random(unsigned int lower, unsigned int upper) {
	return Random() % (upper - lower) + lower;
}

template<typename Src>
void Shuffle(Src& src, size_t size, size_t swap_times = 0) {
	if (swap_times == 0) { swap_times = size * 2; }

	do {
		size_t a(Random(0, size));
		size_t b(Random(0, size));
		if (a == b) { continue; }
		UncheckedSwap(src[a], src[b]);
		--swap_times;
	} while (swap_times != 0);
}

}

#endif
#ifndef PHI__define_guard__Utility__Random_h
#define PHI__define_guard__Utility__Random_h

#include <cstdlib>
#include <ctime>
#include "../define.h"
#include "memory.h"
#include "swap.h"

namespace phi {

inline bool rand_init_() {
	time_t t;
	std::srand((unsigned int)time(&t));
	return true;
}

inline int rand() {
	static bool init(rand_init_());
	return std::rand();
}

template<typename Src, typename Swapper = DefaultSwapper>
void shuffle(Src& src, size_t size, size_t swap_times = 0,
			 Swapper swapper = Swapper()) {
	if (swap_times == 0) { swap_times = size * 2; }

	while (swap_times != 0) {
		size_t a(rand() % size);
		size_t b(rand() % size);
		if (a == b) { continue; }
		swapper(src[a], src[b]);
		--swap_times;
	}
}

}

#endif
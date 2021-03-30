#ifndef PHI__define_guard__Container__Array_cuh
#define PHI__define_guard__Container__Array_cuh

#include "../define.h"

namespace phi {
namespace cntr {

template<typename T, size_t N> struct Array {
	static_assert(N != 0, "N needs larger than 0");

	T value[N];

#///////////////////////////////////////////////////////////////////////////////

	template<typename... Args> static Array Make(Args&&... args) {
		Array r;
		phi::Assign<N>(r, Forward<Args>(args)...);
		return r;
	}

#///////////////////////////////////////////////////////////////////////////////

	Array() {}

	template<typename Src> Array(Src&& src) { *this = src; }

	~Array() {}

	operator T*() { return this->value; }
	operator const T*() const { return this->value; }

#///////////////////////////////////////////////////////////////////////////////

	template<typename Src> Array& operator=(Src&& src) {
		Copy<N>(this->value, Forward<Src>(src));
		return *this;
	}

	template<typename... Args> void Assign(Args&&... args) {
		phi::Assign<N>(this->value, Forward<Args>(args)...);
	}

#///////////////////////////////////////////////////////////////////////////////

	T& operator[](size_t index) { return this->value[index]; }
	const T& operator[](size index) const { return this->value[index]; }
};

}
}

#endif
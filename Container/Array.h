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
		PHI::Assign<N>(r, Forward<Args>(args)...);
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
		PHI::Assign<N>(this->value, Forward<Args>(args)...);
	}

#///////////////////////////////////////////////////////////////////////////////

	template<typename Index> T& operator[](Index&& index) & {
		return this->value[Forward<Index>(index)];
	}

	template<typename Index> const T& operator[](Index&& index) const& {
		return this->value[Forward<Index>(index)];
	}

	template<typename Index> T&& operator[](Index&& index) && {
		return Move(this->value[Forward<Index>(index)]);
	}
};

}
}

#endif
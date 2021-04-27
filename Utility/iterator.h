#ifndef PHI__define_guard__Utility__iterator_h
#define PHI__define_guard__Utility__iterator_h

#include "../define.h"

namespace phi {
namespace iterator {

namespace Type {

struct Forward {};
struct Bidirectional: public Forward {};
struct RandomAccess: public Bidirectional {};

}

template<typename Iterator> struct trait {
	using Type = typename Iterator::Type;
	using Value = typename Iterator::Value;
	using Ref = typename Iterator::Ref;
	using Ptr = typename Iterator::Ptr;
	using Diff = typename Iterator::Diff;
};

template<typename T> struct trait<T*> {
	using Type = Type::RandomAccess;
	using Value = T;
	using Ptr = T*;
	using Ref = T&;
	using Diff = diff_t;
};

template<typename Iterator> using GetType = typename trait<Iterator>::Type;
template<typename Iterator> using GetValue = typename trait<Iterator>::Value;
template<typename Iterator> using GetRef = typename trait<Iterator>::Ref;
template<typename Iterator> using GetDiff = typename trait<Iterator>::Diff;

template<typename Iterator> void base_on_forward() {
	static_assert(
		base_on<typename iterator::trait<Iterator>::Type, Type::Forward>::value,
		"iteretor is not based on forward iterator");
}

template<typename Iterator> void base_on_bidirectional() {
	static_assert(base_on<typename iterator::trait<Iterator>::Type,
						  Type::Bidirectional>::value,
				  "iteretor is not based on bidirectional iterator");
}

template<typename Iterator> void base_on_random_access() {
	static_assert(base_on<typename iterator::trait<Iterator>::Type,
						  Type::RandomAccess>::value,
				  "iteretor is not based on random access iterator");
}

}
}

#endif
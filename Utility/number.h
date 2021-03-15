#ifndef PHI__define_guard__Utility__number_h
#define PHI__define_guard__Utility__number_h

#include "../define.h"

namespace phi {

template<typename Int> struct add_unsigned { using type = Int; };

template<> struct add_unsigned<int> { using type = unsigned int; };
template<> struct add_unsigned<long int> { using type = unsigned long int; };
template<> struct add_unsigned<long long int> {
	using type = unsigned long long int;
};

template<typename Int> struct is_signed {
	static constexpr bool value =
		!is_same<Int, typename add_unsigned<Int>::type>::value;
};

template<typename Int> struct is_unsigned {
	static constexpr bool value =
		is_same<Int, typename add_unsigned<Int>::type>::value;
};

template<typename Int> Int GCD(Int x, Int y) {
	static_assert(is_same<Int, int>::value ||
					  is_same<Int, unsigned int>::value ||
					  is_same<Int, long int>::value ||
					  is_same<Int, unsigned long int>::value ||
					  is_same<Int, long long int>::value ||
					  is_same<Int, unsigned long long int>::value,
				  "not int");

	using UnsignedInt = typename add_unsigned<Int>::type;

	UnsignedInt a(x < 0 ? -x : x);
	UnsignedInt b(y < 0 ? -y : y);

	while ((a %= b) && (b %= a)) {}

	return a + b;
}

}

#endif
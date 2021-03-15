#ifndef PHI__define_guard__Utility__operator_h
#define PHI__define_guard__Utility__operator_h

#include "../define.h"

namespace phi {
namespace op {

template<typename T> struct neg {
	auto operator()(const T& x) const { return -x; }
};

#///////////////////////////////////////////////////////////////////////////////

template<typename X, typename Y = X> struct eq {
	bool operator()(const X& x, const Y& y) const { return x == y; }
	bool operator()(const Y& y, const X& x) const { return y == x; }
};

template<typename T> struct eq<T, T> {
	bool operator()(const T& x, const T& y) const { return x == y; }
};

#///////////////////////////////////////////////////////////////////////////////

template<typename X, typename Y = X> struct ne {
	bool operator()(const X& x, const Y& y) const { return x != y; }
	bool operator()(const Y& y, const X& x) const { return y != x; }
};

template<typename T> struct ne<T, T> {
	bool operator()(const T& x, const T& y) const { return x != y; }
};

#///////////////////////////////////////////////////////////////////////////////

template<typename X, typename Y = X> struct lt {
	bool operator()(const X& x, const Y& y) const { return x < y; }
	bool operator()(const Y& y, const X& x) const { return y < x; }
};

template<typename T> struct lt<T, T> {
	bool operator()(const T& x, const T& y) const { return x < y; }
};

#///////////////////////////////////////////////////////////////////////////////

template<typename X, typename Y = X> struct le {
	bool operator()(const X& x, const Y& y) const { return x <= y; }
	bool operator()(const Y& y, const X& x) const { return y <= x; }
};

template<typename T> struct le<T, T> {
	bool operator()(const T& x, const T& y) const { return x <= y; }
};

#///////////////////////////////////////////////////////////////////////////////

template<typename X, typename Y = X> struct gt {
	bool operator()(const X& x, const Y& y) const { return x > y; }
	bool operator()(const Y& y, const X& x) const { return y > x; }
};

template<typename T> struct gt<T, T> {
	bool operator()(const T& x, const T& y) const { return x > y; }
};

#///////////////////////////////////////////////////////////////////////////////

template<typename X, typename Y = X> struct ge {
	bool operator()(const X& x, const Y& y) const { return x >= y; }
	bool operator()(const Y& y, const X& x) const { return y >= x; }
};

template<typename T> struct ge<T, T> {
	bool operator()(const T& x, const T& y) const { return x >= y; }
};

#///////////////////////////////////////////////////////////////////////////////

template<typename T> struct log_not {
	bool operator()(const T& x) const { return !x; }
};

#///////////////////////////////////////////////////////////////////////////////

template<typename X, typename Y = X> struct log_or {
	bool operator()(const X& x, const Y& y) const { return x || y; }
	bool operator()(const Y& y, const X& x) const { return y || x; }
};

template<typename T> struct log_or<T, T> {
	bool operator()(const T& x, const T& y) const { return x || y; }
};

#///////////////////////////////////////////////////////////////////////////////

template<typename X, typename Y = X> struct log_and {
	bool operator()(const X& x, const Y& y) const { return x && y; }
	bool operator()(const Y& y, const X& x) const { return y && x; }
};

template<typename T> struct log_and<T, T> {
	bool operator()(const T& x, const T& y) const { return x && y; }
};

#///////////////////////////////////////////////////////////////////////////////

template<typename X, typename Y = X> struct log_xor {
	bool operator()(const X& x, const Y& y) const { return x ^ y; }
	bool operator()(const Y& y, const X& x) const { return y ^ x; }
};

template<typename T> struct log_xor<T, T> {
	bool operator()(const T& x, const T& y) const { return x ^ y; }
};

#///////////////////////////////////////////////////////////////////////////////

template<typename T> struct ptr {
	T& operator()(T& x) const { return &x; }
};

template<typename T> struct ref {
	T& operator()(T* x) const { return *x; }
};

#///////////////////////////////////////////////////////////////////////////////

template<typename Src, typename Dst> struct assign {
	auto operator()(Dst&& dst, const Src&& src) const { return dst = src; }
};

template<typename Src, typename Dst = Src> struct copy {
	Dst operator()(const Src& src) const { return static_cast<Dst>(src); }
};

template<typename Src, typename Dst> struct move {
	auto operator()(Dst& dst, Src& src) const { return dst = Move(src); }
};

#///////////////////////////////////////////////////////////////////////////////

template<typename T> struct DefaultCreator {
	T operator()() { return T(); }
};

template<typename T> struct DefaultDeleter {
	void operator()(T& x) const { Delete(&x); }
	void operator()(T* begin, T* end) const { Delete(begin, end); }
};

template<typename Iterator, typename T = decltype(*phi::declval<Iterator>()),
		 typename Ref = ref<T>, typename Compare = lt<T>>
struct IteratorCompare {
	bool operator()(const Iterator& x, const Iterator& y, Ref ref = Ref(),
					Compare compare = Compare()) {
		return compare(ref(x), ref(y));
	}
};

}
}

#endif
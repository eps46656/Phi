#ifndef PHI__define_guard__Utility__memory_op_h
#define PHI__define_guard__Utility__memory_op_h

#include "../define.h"
#include "memory.h"
#include "compare.h"
#include "iterator.h"

namespace phi {

template<typename T, typename... Args> T* New(Args&&... args) {
	return new (Malloc(sizeof(T))) T(Forward<Args>(args)...);
}

#///////////////////////////////////////////////////////////////////////////////

template<typename T> void Destroy(T* ptr) { ptr->~T(); }

template<typename T> void Destroy(void* ptr) { static_cast<T*>(ptr)->~T(); }

template<typename Iterator> void Destroy(Iterator begin, Iterator end) {
	using T = remove_reference_t<decltype(*begin)>;
	for (; begin != end; ++begin) { begin->~T(); }
}

template<typename Src> void Destroy(size_t size, Src&& src) {
	using T = remove_reference_t<decltype(src[0])>;
	for (size_t i(0); i != size; ++i) { src[i].~T(); }
}

#///////////////////////////////////////////////////////////////////////////////

template<typename T> void Delete(T* ptr) {
	if (ptr) {
		ptr->~T();
		Free(ptr);
	}
}

template<typename T> void Delete(size_t size, T* ptr) {
	for (size_t i(0); i != size; ++i) { ptr[i].~T(); }
	Free(ptr);
}

template<typename T> void Delete(T* begin, T* end) {
	if (begin == end) { return; }
	for (; begin != end; ++begin) { begin->~T(); }
	Free(begin);
}

#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////

template<typename T> void Swap(T& x, T& y) {
	if (&x == &y) { return; }
	T temp(Move(x));
	x = Move(y);
	y = Move(temp);
}

template<typename T> void UncheckedSwap(T& x, T& y) {
	T temp(Move(x));
	x = Move(y);
	y = Move(temp);
}

#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////

template<typename Dst, typename Value>
void Fill(size_t size, Dst&& dst, const Value& value) {
	for (size_t i(0); i != size; ++i) { dst[i] = value; }
}

#///////////////////////////////////////////////////////////////////////////////

template<size_t i, size_t size> struct Fill_ {
	template<typename Dst, typename T>
	static void F(Dst&& dst, const T& value) {
		Forward<Dst>(dst)[i] = value;
		Fill_<i + 1, size>()(Forward<Dst>(dst), value);
	}
};

template<size_t size> struct Fill_<size, size> {
	template<typename Dst, typename Value>
	static void F(Dst&& dst, const Value& value) {}
};

template<size_t size, typename Dst, typename Value>
void Fill(Dst&& dst, const Value& value) {
	Fill_<0, size>::F(Forward<Dst>(dst), value);
}

#///////////////////////////////////////////////////////////////////////////////

template<typename ForwardIterator, typename Value>
void Fill(ForwardIterator begin, ForwardIterator end, const Value& value) {
	for (; begin != end; ++begin) { *begin = value; }
}

#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////

template<typename Dst, typename Src>
void Copy(size_t size, Dst&& dst, Src&& src) {
	for (size_t i(0); i != size; ++i) { dst[i] = src[i]; }
}

template<typename Dst, typename Src>
void CopyForward(size_t size, Dst&& dst, Src&& src) {
	for (size_t i(0); i != size; ++i) { dst[i] = src[i]; }
}

template<typename Dst, typename Src>
void CopyBackward(size_t size, Dst&& dst, Src&& src) {
	while (size) {
		--size;
		dst[size] = src[size];
	}
}

template<size_t i, size_t size> struct Copy_ {
	template<typename Dst, typename Src> static void F(Dst&& dst, Src&& src) {
		Forward<Dst>(dst)[i] = Forward<Src>(src)[i];
		Copy_<i + 1, size>::F(Forward<Dst>(dst), Forward<Src>(src));
	}
};

template<size_t size> struct Copy_<size, size> {
	template<typename Dst, typename Src> static void F(Dst&& dst, Src&& src) {}
};

template<size_t size, typename Dst, typename Src>
void Copy(Dst&& dst, Src&& src) {
	Copy_<0, size>::F(Forward<Dst>(dst), Forward<Src>(src));
}

#///////////////////////////////////////////////////////////////////////////////

template<typename Dst, typename Src> void Move(Dst dst, Src src, size_t size) {
	for (; size; ++dst, ++src, --size) { *dst = move(*src); }
}

template<typename Dst, typename Src>
void Move(size_t size, Dst& dst, Src& src) {
	for (size_t i(0); i != size; ++i) { dst[i] = Move(src[i]); }
}

template<typename Dst, typename Src>
void MoveForward(size_t size, Dst& dst, Src& src) {
	for (size_t i(0); i != size; ++i) { dst[i] = Move(src[i]); }
}

template<typename Dst, typename Src>
void MoveBackward(size_t size, Dst& dst, Src& src) {
	while (size) {
		--size;
		dst[size] = Move(src[size]);
	}
}

#///////////////////////////////////////////////////////////////////////////////

template<size_t index, size_t size> struct Move_ {
	template<typename Dst, typename Src> static void F(Dst& dst, Src& src) {
		dst[index] = Move(src[index]);
		Move_<index + 1, size>::F(dst, src);
	}
};

template<size_t index_size> struct Move_<index_size, index_size> {
	template<typename Dst, typename Src> static void F(Dst& dst, Src& src) {}
};

template<size_t size, typename Dst, typename Src>
void Move(Dst& dst, Src& src) {
	Move_<0, size>::F(dst, src);
}

#///////////////////////////////////////////////////////////////////////////////

template<typename X, typename Y, typename EqualComparer = DefaultEqualComparer>
bool Equal(size_t size, X& x, Y& y, EqualComparer eq_cmper = EqualComparer()) {
	for (size_t i(0); i != size; ++i) {
		if (!eq_cmper(x[i], y[i])) { return false; }
	}

	return true;
}

inline void MemcpyForward(size_t size, void* dst, const void* src) {
	for (size_t i(0); i != size; ++i) {
		static_cast<char*>(dst)[i] = static_cast<const char*>(src)[i];
	}
}

inline void MemcpyBackward(size_t size, void* dst, const void* src) {
	while (size) {
		--size;
		static_cast<char*>(dst)[size] = static_cast<const char*>(src)[size];
	}
}

inline void Memcpy(size_t size, void* dst, const void* src) {
	if (dst == src) { return; }
	if (static_cast<const char*>(src) < static_cast<char*>(dst) &&
		static_cast<char*>(dst) < static_cast<const char*>(src) + size) {
		MemcpyBackward(size, dst, src);
	} else {
		MemcpyForward(size, dst, src);
	}
}

inline bool Memcmp(size_t size, const void* x, const void* y) {
	for (size_t i(0); i != size; ++i) {
		if (static_cast<const char*>(x)[i] != static_cast<const char*>(y)[i]) {
			return false;
		}
	}

	return true;
}

#///////////////////////////////////////////////////////////////////////////////

template<size_t index, size_t size> struct Memcpy_ {
	static void F(void* dst, const void* src) {
		static_cast<char*>(dst)[index] = static_cast<const char*>(src)[index];
		Memcpy_<index + 1, size>::F(dst, src);
	}
};

template<size_t index_size> struct Memcpy_<index_size, index_size> {
	static void F(void* dst, const void* src) {}
};

template<size_t size> void Memcpy(void* dst, const void* src) {
	Memcpy_<0, size>::F(dst, src);
}

#///////////////////////////////////////////////////////////////////////////////

template<size_t i, size_t size> struct Init_ {
	template<typename Dst, typename X, typename... Args>
	static void F(Dst* dst, X&& x, Args&&... args) {
		new (dst + i) Dst(Forward<X>(x));
		Init_<i + 1, size>::F(dst, Forward<Args>(args)...);
	}
};

template<size_t size> struct Init_<size, size> {
	template<typename Dst> static void F(Dst* dst) {}
};

template<size_t size, typename Dst, typename... Args>
void Init(Dst* dst, Args&&... args) {
	static_assert(size == sizeof...(args), "size error");
	Init_<0, size>::F(dst, Forward<Args>(args)...);
}

#///////////////////////////////////////////////////////////////////////////////

template<size_t i, size_t size> struct Assign_ {
	template<typename Dst, typename Value>
	static void F(Dst&& dst, Value&& value) {
		dst[i] = Forward<Value>(value);
		Assign_<i + 1, size>::F(dst, Forward<Value>(value));
	}
};

template<size_t size> struct Assign_<size, size> {
	template<typename Dst, typename Value>
	static void F(Dst&& dst, Value&& value) {}
};

template<size_t size, typename Dst, typename Value>
void Assign(Dst&& dst, Value&& value) {
	Assign_<0, size>::F(dst, Forward<Value>(value));
}

#///////////////////////////////////////////////////////////////////////////////

template<size_t i, size_t size> struct AssignVector_ {
	template<typename Dst, typename X, typename... Args>
	static void F(Dst&& dst, X&& x, Args&&... args) {
		dst[i] = Forward<X>(x);
		Assign_<i + 1, size>::F(dst, Forward<Args>(args)...);
	}
};

template<size_t size> struct AssignVector_<size, size> {
	template<typename Dst> static void F(Dst&& dst) {}
};

template<size_t size, typename Dst, typename... Args>
void AssignVector(Dst&& dst, Args&&... args) {
	static_assert(size == sizeof...(args), "size error");
	Assign_<0, size>::F(dst, Forward<Args>(args)...);
}

#///////////////////////////////////////////////////////////////////////////////

template<size_t i, size_t j, size_t col_dim, size_t row_dim, size_t align>
struct Assign2DVector_ {
	template<typename Dst, typename X, typename... Args>
	static void F(Dst&& dst, X&& x, Args&&... args) {
		Forward<Dst>(dst)[align * i + j] = Forward<X>(x);
		Assign2DVector_<i, j + 1, col_dim, row_dim, align>::F(
			Forward<Dst>(dst), Forward<Args>(args)...);
	}
};

template<size_t i, size_t col_dim, size_t row_dim, size_t align>
struct Assign2DVector_<i, row_dim, col_dim, row_dim, align> {
	template<typename Dst, typename... Args>
	static void F(Dst&& dst, Args&&... args) {
		Assign2DVector_<i + 1, 0, col_dim, row_dim, align>::F(
			Forward<Dst>(dst), Forward<Args>(args)...);
	}
};

template<size_t col_dim, size_t row_dim, size_t align>
struct Assign2DVector_<col_dim, 0, col_dim, row_dim, align> {
	template<typename Dst> static void F(Dst&& dst) {}
};

template<size_t col_size, size_t row_size, size_t align, typename Dst,
		 typename... Args>
void Assign2DVector(Dst&& dst, Args&&... args) {
	static_assert(col_size * row_size == sizeof...(args), "size error");
	Assign2DVector_<0, 0, col_size, row_size, align>::F(Forward<Dst>(dst),
														Forward<Args>(args)...);
}

#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////

template<typename Src> void Reverse(size_t lower, size_t upper, Src& src) {
	if (upper <= lower) { return; }
	for (--upper; lower < upper; ++lower, --upper) {
		Swap(src[lower], src[upper]);
	}
}

template<typename Src> void Reverse(size_t size, Src& src) {
	Reverse(0, size, src);
}

template<typename BidirectionalIterator>
void Reverse(BidirectionalIterator begin, BidirectionalIterator end) {
	if ((begin == end) || (begin == --end)) { return; }
	do { Swap(*begin, *end); } while ((++begin != end) && (begin != --end));
}

}

#endif

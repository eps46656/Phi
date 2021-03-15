#ifndef PHI__define_guard__define_h
#define PHI__define_guard__define_h

#include <assert.h>
#include <iostream>

#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////

#define PHI__debug_flag true

#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////

#if PHI__debug_flag
	#define PHI__debug if constexpr (true)
	#define PHI__debug_if(x) if (x)

	#define PHI__throw__(type, func, desc)                                     \
		{                                                                      \
			std::cout << __LINE__ << " " << type << "::" << func << "\n\t"     \
					  << desc << "\n";                                         \
			assert(false);                                                     \
		}
#else
	#define PHI__debug if constexpr (false)
	#define PHI__debug_if(x) if (false)

	#define PHI__throw__(type, func, desc) ;
#endif

#define PHI__throw_(type, func, desc) PHI__throw__(#type, func, desc)
#define PHI__throw(type, func, desc) PHI__throw_(type, func, desc)

#define PHI__offset(type, member) size_t(&((type*)0)->member)

#define PHI__eps (1e-8)
#define PHI__inf (1e256 * 1e256 * 1e256 * 1e256)
#define PHI__pi (3.14159265358979323)

#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////

namespace phi {

using nullptr_t = decltype(nullptr);
using uint = unsigned int;
using size_t = decltype(sizeof(char));
using diff_t = decltype((char*)(0) - (char*)(0));
using dim_t = unsigned int;
using id_t = unsigned int;
using double_t = double;
using hash_t = unsigned int;

#define PHI__ptr_addr(ptr) (reinterpret_cast<size_t>(ptr))
#define PHI__void_ptr_addr(ptr)                                                \
	(reinterpret_cast<size_t>(static_cast<void*>(ptr)))

#///////////////////////////////////////////////////////////////////////////////

template<typename T> struct identity { using type = T; };
template<typename T> using ideneity_t = typename identity<T>::type;

#///////////////////////////////////////////////////////////////////////////////

template<typename T> inline T declval() { return T(); }

#///////////////////////////////////////////////////////////////////////////////

template<typename T> struct remove_reference { using type = T; };
template<typename T> struct remove_reference<T&> { using type = T; };
template<typename T> struct remove_reference<T&&> { using type = T; };
template<typename T>
using remove_reference_t = typename remove_reference<T>::type;

#///////////////////////////////////////////////////////////////////////////////

template<typename T> struct add_lvalue_reference { using type = T&; };
template<typename T>
using add_lvalue_reference_t = typename add_lvalue_reference<T>::type;

template<typename T> struct add_rvalue_reference { using type = T&&; };
template<typename T>
using add_rvalue_reference_t = typename add_rvalue_reference<T>::type;

#///////////////////////////////////////////////////////////////////////////////

template<typename T> remove_reference_t<T>&& Move(T&& x) {
	return static_cast<remove_reference_t<T>&&>(x);
}

template<typename T> T&& Forward(remove_reference_t<T>& x) {
	return static_cast<T&&>(x);
}

#///////////////////////////////////////////////////////////////////////////////

template<typename T, typename Y> struct is_same {
	static constexpr bool value = false;
};

template<typename T> struct is_same<T, T> {
	static constexpr bool value = true;
};

#///////////////////////////////////////////////////////////////////////////////

template<typename T> struct is_const { static constexpr bool value = false; };

template<typename T> struct is_const<const T> {
	static constexpr bool value = true;
};

#///////////////////////////////////////////////////////////////////////////////

template<typename T> struct add_const { using type = const T; };

#///////////////////////////////////////////////////////////////////////////////

template<typename T> struct remove_const { using type = T; };
template<typename T> struct remove_const<const T> { using type = T; };

#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////

template<typename T> struct remove_reference_and_const {
	using type =
		typename remove_const<typename remove_reference<T>::type>::type;
};

#///////////////////////////////////////////////////////////////////////////////

template<typename BoolReturnedFunctor> struct ReverseBoolReturnedFunctor {
private:
	BoolReturnedFunctor f_;

public:
	template<typename... Args>
	ReverseBoolReturnedFunctor(Args&&... args): f_(Forward<Args>(args)...) {}

	template<typename... Args> bool operator()(Args&&... args) {
		return !this->f_(Forward<Args>(args)...);
	}

	template<typename... Args> bool operator()(Args&&... args) const {
		return !this->f_(Forward<Args>(args)...);
	}
};

#///////////////////////////////////////////////////////////////////////////////

template<typename F, typename... Functors> struct CombinedFunctor {
private:
	F f_;
	CombinedFunctor<Functors...> functors_;

public:
	CombinedFunctor() {}

	template<typename FInit, typename... Args>
	CombinedFunctor(FInit&& f_init, Args&&... args):
		f_(f_init), functors_(Forward<Args>(args)...) {}

	template<typename... Args> auto operator()(Args&&... args) {
		return this->f_(this->functors_(Forward<Args>(args)...));
	}

	template<typename... Args> auto operator()(Args&&... args) const {
		return this->f_(this->functors_(Forward<Args>(args)...));
	}
};

template<typename F> struct CombinedFunctor<F> {
private:
	F f_;

public:
	CombinedFunctor() {}

	template<typename FInit> CombinedFunctor(FInit&& f_init): f_(f_init) {}

	template<typename... Args> auto operator()(Args&&... args) {
		return this->f_(Forward<Args>(args)...);
	}

	template<typename... Args> auto operator()(Args&&... args) const {
		return this->f_(Forward<Args>(args)...);
	}
};

}

#endif

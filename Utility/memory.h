#ifndef PHI__define_guard__Utility__memory_h
#define PHI__define_guard__Utility__memory_h

#include "../define.h"
#include <new>

namespace phi {

template<typename T> T* Malloc(size_t size) {
	return reinterpret_cast<T*>(new char[sizeof(T) * size]);
}

inline void* Malloc(size_t size) { return Malloc<char>(size); }

inline void Free(void* ptr) { delete[](char*) ptr; }

}

#endif

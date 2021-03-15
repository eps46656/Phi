#ifndef PHI__define_guard__Container__ReverseIterator_h
#define PHI__define_guard__Container__ReverseIterator_h

#include "../define.h"

namespace phi {
namespace cntr {

template<typename Iterator> struct ReverseIterator: public Iterator {
	ReverseIterator(const Iterator& iterator): Iterator(iterator) {}

#///////////////////////////////////////////////////////////////////////////////

	auto operator++() { return --this->iterator_; }
	auto operator--() { return ++this->iterator_; }

	auto operator++() const { return --this->iterator_; }
	auto operator--() const { return ++this->iterator_; }
};

}
}

#endif

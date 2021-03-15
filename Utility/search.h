#ifndef PHI__define_guard__Utility__search_h
#define PHI__define_guard__Utility__search_h

#include "compare.h"
#include "pair.h"

namespace phi {

template<typename Src, typename Index,
		 typename EqualComparer = DefaultEqualComparer>
size_t LinearSearch(size_t lower, size_t upper, Src& src, const Index& index,
					EqualComparer eq_cmper = EqualComparer()) {
	for (; lower < upper; ++lower) {
		if (eq_cmper(src[lower], index)) { return lower; }
	}

	return upper;
}

template<typename Src, typename Index,
		 typename EqualComparer = DefaultEqualComparer>
size_t LinearSearch(size_t size, Src& src, const Index& index,
					EqualComparer eq_cmper = EqualComparer()) {
	return LinearSearch(size, src, index, eq_cmper);
}

#///////////////////////////////////////////////////////////////////////////////

template<typename ForwardIterator, typename Index,
		 typename EqualComparer = DefaultEqualComparer>
ForwardIterator LinearSearch(ForwardIterator begin, ForwardIterator end,
							 const Index& index,
							 EqualComparer eq_cmper = EqualComparer()) {
	while (begin != end && !eq_cmper(*begin, index)) { ++begin; }
	return begin;
}

template<typename ForwardIterator, typename Index,
		 typename EqualComparer = DefaultEqualComparer>
pair<size_t, ForwardIterator>
LinearSearch(size_t size, ForwardIterator begin, const Index& index,
			 EqualComparer eq_cmper = EqualComparer()) {
	size_t i(0);

	while (i != size && !eq_cmper(*begin, index)) {
		++begin;
		++i;
	}

	return pair<size_t, ForwardIterator>(size, begin);
}

#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////

template<typename Src, typename Index,
		 typename FullComparer = DefaultFullComparer>
size_t BinarySearch(size_t lower, size_t upper, Src& src, const Index& index,
					FullComparer full_cmper = FullComparer()) {
	size_t not_find(upper);

	while (lower != upper) {
		size_t i((lower + upper) / 2);

		switch (full_cmper(index, src[i])) {
			case -1: upper = i; break;
			case 1: lower = i + 1; break;
			case 0: return i;
		}
	}

	return not_find;
}

template<typename Src, typename Index,
		 typename FullComparer = DefaultFullComparer>
size_t BinarySearch(size_t size, Src& src, const Index& index,
					FullComparer full_cmper = FullComparer()) {
	return BinarySearch(0, size, src, index, full_cmper);
}

#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////

template<typename Src, typename Index,
		 typename EqualComparer = DefaultEqualComparer>
bool ContainLinear(size_t lower, size_t upper, Src& src, const Index& index,
				   EqualComparer eq_cmper = EqualComparer()) {
	return upper != LinearSearch(lower, upper, src, index, eq_cmper);
}

template<typename Src, typename Index,
		 typename EqualComparer = DefaultEqualComparer>
bool ContainLinear(size_t size, Src& src, const Index& index,
				   EqualComparer eq_cmper = EqualComparer()) {
	return ContainLinear(0, size, src, index, eq_cmper);
}

#///////////////////////////////////////////////////////////////////////////////

template<typename Iterator, typename Index,
		 typename EqualComparer = DefaultEqualComparer>
bool ContainLinear(Iterator begin, Iterator end, const Index& index,
				   EqualComparer eq_cmper = EqualComparer()) {
	return end != LinearSearch(begin, end, index, eq_cmper);
}

#///////////////////////////////////////////////////////////////////////////////

template<typename Src, typename Index,
		 typename EqualComparer = DefaultEqualComparer>
bool ContainBinary(size_t lower, size_t upper, Src& src, const Index& index,
				   EqualComparer eq_cmper = EqualComparer()) {
	return upper != BinarySearch(lower, upper, src, index, eq_cmper);
}

template<typename Src, typename Index,
		 typename EqualComparer = DefaultEqualComparer>
bool ContainBinary(size_t size, Src& src, const Index& index,
				   EqualComparer eq_cmper = EqualComparer()) {
	return ContainBinary(0, size, src, index, eq_cmper);
}

}

#endif
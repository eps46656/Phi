#ifndef PHI__define_guard__Utility__search_h
#define PHI__define_guard__Utility__search_h

#include "compare.h"
#include "pair.h"
#include "iterator.h"

namespace phi {

template<typename ForwardIterator, typename Index,
		 typename EqualComparer = DefaultEqualComparer>
ForwardIterator LinearSearch(ForwardIterator begin, ForwardIterator end,
							 const Index& index,
							 EqualComparer eq_cmper = EqualComparer()) {
	while (begin != end && !eq_cmper.eq(*begin, index)) { ++begin; }
	return begin;
}

template<typename ForwardIterator, typename Index,
		 typename EqualComparer = DefaultEqualComparer>
pair<size_t, ForwardIterator>
LinearSearch(size_t size, ForwardIterator begin, const Index& index,
			 EqualComparer eq_cmper = EqualComparer()) {
	size_t i(0);

	while (i != size && !eq_cmper.eq(*begin, index)) {
		++begin;
		++i;
	}

	return pair<size_t, ForwardIterator>(size, begin);
}

#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////

template<typename RandomAccessIterator, typename Index,
		 typename FullComparer = DefaultFullComparer>
typename iterator::trait<RandomAccessIterator>::Diff
BinarySearch(RandomAccessIterator begin, RandomAccessIterator end,
			 const Index& index, FullComparer full_cmper = FullComparer()) {
	using Diff = typename iterator::trait<RandomAccessIterator>::Diff;

	RandomAccessIterator not_find(end);

	while (begin < end) {
		RandomAccessIterator mid(begin + (end - begin) / Diff(2));

		switch (full_cmper(index, *mid)) {
			case -1: end = mid; break;
			case 1: begin = mid + Diff(1); break;
			case 0: return mid;
		}
	}

	return not_find;
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
		 typename FullComparer = DefaultFullComparer>
bool ContainBinary(size_t lower, size_t upper, Src& src, const Index& index,
				   FullComparer full_cmper = FullComparer()) {
	return upper != BinarySearch(lower, upper, src, index, full_cmper);
}

template<typename Src, typename Index,
		 typename FullComparer = DefaultFullComparer>
bool ContainBinary(size_t size, Src& src, const Index& index,
				   FullComparer full_cmper = FullComparer()) {
	return ContainBinary(0, size, src, index, full_cmper);
}

template<typename RandomAccessIterator, typename Index,
		 typename FullComparer = DefaultFullComparer>
bool ContainBinary(RandomAccessIterator begin, RandomAccessIterator end,
				   const Index& index,
				   FullComparer full_cmper = FullComparer()) {
	return end != BinarySearch(begin, end, index, full_cmper);
}

}

#endif
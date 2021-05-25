#ifndef PHI__define_guard__Utility__search_h
#define PHI__define_guard__Utility__search_h

#include "compare.h"
#include "pair.h"
#include "iterator.h"

namespace phi {

/*
 * Returns the distance from begin to end
*/

template<typename Iterator>
typename iterator::trait<Iterator>::Diff
Distance(iterator::Type::Forward iterator_type, Iterator begin, Iterator end) {
	typename iterator::trait<Iterator>::Diff r(0);
	for (; begin != end; ++begin) { ++r; }
	return r;
}

template<typename Iterator>
typename iterator::trait<Iterator>::Diff
Distance(iterator::Type::RandomAccess iterator_type, Iterator begin,
		 Iterator end) {
	return end - begin;
}

template<typename Iterator> auto Distance(Iterator begin, Iterator end) {
	return Distance(typename iterator::trait<Iterator>::Type(), begin, end);
}

#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////

/*
 * Return the iterator of minimum element in [begin, end).
 * Using eq_cmper to compare two elements.
 * If there are two or more elements equal, undefined which index will return.
*/

template<typename ForwardIterator,
		 typename LessThanComparer = DefaultLessThanComparer>
ForwardIterator Min(ForwardIterator begin, ForwardIterator end,
					LessThanComparer&& lt_cmper = LessThanComparer()) {
	if (begin == end) { return end; }

	ForwardIterator r(begin);

	while (++begin != end) {
		if (lt_cmper.lt(*begin, *r)) { r = begin; }
	}

	return r;
}

/*
 * Return the iterator of maximum element in [begin, end).
 * Using eq_cmper to compare two elements.
 * If there are two or more elements equal, undefined which index will return.
*/
template<typename ForwardIterator,
		 typename LessThanComparer = DefaultLessThanComparer>
ForwardIterator Max(ForwardIterator begin, ForwardIterator end,
					LessThanComparer&& lt_cmper = LessThanComparer()) {
	if (begin == end) { return end; }

	ForwardIterator r(begin);

	while (++begin != end) {
		if (lt_cmper.lt(*r, *begin)) { r = begin; }
	}

	return r;
}

#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////

/*
 * Count how many elements in [begin, end) let the output of func be true.
*/
template<typename ForwardIterator, typename F>
size_t Count(ForwardIterator begin, ForwardIterator end, F&& func) {
	size_t r(0);
	for (; begin != end; ++begin) { r += func(*begin); }
	return r;
}

template<typename ForwardIterator, typename Target,
		 typename EqualComparer = DefaultEqualComparer>
size_t CountEqual(ForwardIterator begin, ForwardIterator end,
				  const Target& target,
				  const EqualComparer& eq_cmper = EqualComparer()) {
	size_t r(0);
	for (; begin != end; ++begin) { r += eq_cmper.eq(*begin, target); }
	return r;
}

#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////

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
#ifndef PHI__define_guard__Utility__sort_h
#define PHI__define_guard__Utility__sort_h

#include "memory.h"
#include "compare.h"
#include "swap.h"
#include "pair.h"
#include "triple.h"
#include "search.h"
#include "iterator.h"
#include "heap.h"

namespace phi {

/*
Returns the number of iterators in [begin, end)
*/

template<typename Iterator>
auto Distance(iterator::Type::Forward iterator_type, Iterator begin,
			  Iterator end) {
	typename iterator::trait<Iterator>::Diff r(0);
	for (; begin != end; ++begin) { ++r; }
	return r;
}

template<typename Iterator>
auto Distance(iterator::Type::RandomAccess iterator_type, Iterator begin,
			  Iterator end) {
	return end - begin;
}

template<typename Iterator> auto Distance(Iterator begin, Iterator end) {
	return Distance(begin, end, typename iterator::trait<Iterator>::Type());
}

#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////

template<typename ForwardIterator, typename Target,
		 typename EqualComparer = DefaultEqualComparer>
size_t Count(ForwardIterator begin, ForwardIterator end, const Target& target,
			 const EqualComparer& eq_cmper = EqualComparer()) {
	size_t r(0);

	for (; begin != end; ++begin) {
		if (eq_cmper.eq(*begin, target)) { ++r; }
	}

	return r;
}

#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////

/*
Return the index of minimum element in [begin, end). Uses lt_cmper
Uses eq_cmper to compare two elements. If there are two or more elements equal,
undefined which index will return.
*/

template<typename ForwardIterator,
		 typename LessThanComparer = DefaultLessThanComparer>
ForwardIterator Min(ForwardIterator begin, ForwardIterator end,
					const LessThanComparer& lt_cmper = LessThanComparer()) {
	if (begin == end) { return end; }

	ForwardIterator r(begin);

	while (++begin != end) {
		if (lt_cmper.lt(*begin, *r)) { r = begin; }
	}

	return r;
}

template<typename ForwardIterator,
		 typename LessThanComparer = DefaultLessThanComparer>
ForwardIterator Max(ForwardIterator begin, ForwardIterator end,
					const LessThanComparer& lt_cmper = LessThanComparer()) {
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
Returns pivot_index.
Partitions [src[lower], src[upper]) into two section [lower, pivot_index) and
[pivot_index, upper).
Make [src[lower], src[pivot_index]) <= pivot <= [src[pivot_index], src[upper]).
Uses lt_cmper to compare two elements. Uses swapper to swap elemenets.
*/

template<typename Src, typename Pivot,
		 typename LessThanComparer = DefaultLessThanComparer,
		 typename Swapper = DefaultSwapper>
size_t Partition(size_t lower, size_t upper, Src& src, const Pivot& pivot,
				 const LessThanComparer& lt_cmper = LessThanComparer(),
				 const Swapper& swapper = Swapper()) {
	if (lower == upper) { return lower; }

	size_t a(lower);
	size_t b(upper - 1);

	while (a != b) {
		while (!lt_cmper.lt(pivot, src[a])) {
			if (++a == b) { return lt_cmper.lt(src[a], pivot) ? a + 1 : a; }
		}

		while (!lt_cmper.lt(src[b], pivot)) {
			if (a == --b) { return a; }
		}

		swapper(src[a], src[b]);
		if (--b < ++a) { return a; }
	}

	return lt_cmper.lt(src[a], pivot) ? a + 1 : a;
}

/*
Returns pivot_index.
Partitions [src[0], src[size]) into two section [0, pivot_index) and
[pivot_index, size).
Make [src[0], src[pivot_index]) <= pivot <= [src[pivot_index], src[size]).
Uses lt_cmper to compare two elements. Uses swapper to swap elemenets.
*/

template<typename Src, typename Pivot,
		 typename LessThanComparer = DefaultLessThanComparer,
		 typename Swapper = DefaultSwapper>
pair<size_t, size_t>
Partition(size_t size, Src& src, const Pivot& pivot,
		  const LessThanComparer& lt_cmper = LessThanComparer(),
		  const Swapper& swapper = Swapper()) {
	return Partition(0, size, src, pivot, lt_cmper, swapper);
}

#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////

template<typename ForwardIterator, typename Pivot,
		 typename LessThanComparer = DefaultLessThanComparer,
		 typename Swapper = DefaultSwapper>
ForwardIterator Partition(iterator::Type::Forward iterator_type,
						  ForwardIterator begin, ForwardIterator end,
						  const Pivot& pivot,
						  const LessThanComparer& lt_cmper = LessThanComparer(),
						  const Swapper& swapper = Swapper()) {
	if (begin == end) { return begin; }

	while (!lt_cmper.lt(pivot, *begin)) {
		if (++begin == end) { return end; }
	}

	for (ForwardIterator i(begin); ++i != end;) {
		if (lt_cmper.lt(*i, pivot)) {
			swapper(*begin, *i);
			++begin;
		}
	}

	return begin;
}

template<typename BidirectionalIterator, typename Pivot,
		 typename LessThanComparer = DefaultLessThanComparer,
		 typename Swapper = DefaultSwapper>
BidirectionalIterator
Partition(iterator::Type::Bidirectional iterator_type,
		  BidirectionalIterator begin, BidirectionalIterator end,
		  const Pivot& pivot,
		  const LessThanComparer& lt_cmper = LessThanComparer(),
		  const Swapper& swapper = Swapper()) {
	if (begin == end) { return begin; }

	while (begin != --end) {
		while (!lt_cmper.lt(pivot, *begin)) {
			if (++begin == end) {
				if (lt_cmper.lt(*begin, pivot)) { ++begin; }
				return begin;
			}
		}

		while (!lt_cmper.lt(*end, pivot)) {
			if (begin == --end) { return begin; }
		}

		swapper(*begin, *end);
		if (++begin == end) { return begin; }
	}

	if (lt_cmper.lt(*begin, pivot)) { ++begin; }
	return begin;
}

template<typename Iterator, typename Pivot,
		 typename LessThanComparer = DefaultLessThanComparer,
		 typename Swapper = DefaultSwapper>
Iterator Partition(Iterator begin, Iterator end, const Pivot& pivot,
				   const LessThanComparer& lt_cmper = LessThanComparer(),
				   const Swapper& swapper = Swapper()) {
	return Partition(typename iterator::trait<Iterator>::Type(), begin, end,
					 pivot, lt_cmper, swapper);
}

template<typename RandomAccessIterator, typename LessThanComparer,
		 typename Swapper>
RandomAccessIterator
UnrestrictedPartition(RandomAccessIterator begin, RandomAccessIterator end,
					  typename iterator::trait<RandomAccessIterator>::Ref pivot,
					  const LessThanComparer& lt_cmper,
					  const Swapper& swapper) {
	for (;;) {
		while (lt_cmper.lt(*begin, pivot)) { ++begin; }
		do { --end; } while (lt_cmper.lt(pivot, *end));
		if (!(begin < end)) { return begin; }
		swapper(*begin, *end);
		++begin;
	}
}

#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////

template<typename BidirectionalIterator, typename LessThanComparer,
		 typename Swapper>
void HeadTailSwap(BidirectionalIterator begin, BidirectionalIterator end,
				  const LessThanComparer& lt_cmper = DefaultLessThanComparer(),
				  const Swapper& swapper = DefaultSwapper()) {
	if (begin == end) { return; }

	while (begin != --end) {
		if (lt_cmper.lt(*end, *begin)) { swapper(*begin, *end); }
		if (++begin == end) { break; }
	}
}

#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////

template<typename BidirectionalIterator,
		 typename LessThanComparer = DefaultLessThanComparer>
void LinearInsert(BidirectionalIterator begin, BidirectionalIterator end,
				  const LessThanComparer& lt_cmper = LessThanComparer()) {
	if (begin == end || begin == --end) { return; }

	BidirectionalIterator end_prev(end);
	--end_prev;

	if (!lt_cmper.lt(*end, *end_prev)) { return; }

	typename iterator::trait<BidirectionalIterator>::Value value(Move(*end));

	*end = Move(*end_prev);

	while (begin != (end = end_prev)) {
		--end_prev;
		if (!lt_cmper.lt(value, *end_prev)) { break; }
		*end = Move(*end_prev);
	}

	*end = Move(value);
}

template<typename BidirectionalIterator,
		 typename LessThanComparer = DefaultLessThanComparer>
void UnrestrictedLinearInsert(
	BidirectionalIterator last,
	const LessThanComparer& lt_cmper = LessThanComparer()) {
	BidirectionalIterator last_prev(last);
	--last_prev;

	if (!lt_cmper.lt(*last, *last_prev)) { return; }

	typename iterator::trait<BidirectionalIterator>::Value value(Move(*last));

	do {
		*last = Move(*last_prev);
		last = last_prev;
		--last_prev;
	} while (lt_cmper.lt(value, *last_prev));

	*last = Move(value);
}

template<typename BidirectionalIterator,
		 typename LessThanComparer = DefaultLessThanComparer>
void UnrestrictedInsertionSort(
	BidirectionalIterator begin, BidirectionalIterator end,
	const LessThanComparer& lt_cmper = LessThanComparer()) {
	for (; begin != end; ++begin) { UnrestrictedLinearInsert(begin, lt_cmper); }
}

/*
Insertion sorts [begin, end). Uses lt_cmper to compare two elements.
Uses swapper to swap elemenets.
*/

template<typename BidirectionalIterator,
		 typename LessThanComparer = DefaultLessThanComparer>
void InsertionSort(BidirectionalIterator begin, BidirectionalIterator end,
				   const LessThanComparer& lt_cmper = LessThanComparer()) {
	if (begin == end) { return; }
	DefaultSwapper()(*begin, *Min(begin, end, lt_cmper));
	UnrestrictedInsertionSort(++begin, end, lt_cmper);
}

#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////

template<typename ForwardIterator,
		 typename LessThanComparer = DefaultLessThanComparer,
		 typename Swapper = DefaultSwapper>
void BubbleSort(ForwardIterator begin, ForwardIterator end,
				const LessThanComparer& lt_cmper = LessThanComparer(),
				const Swapper& swapper = Swapper()) {
	if (begin == end) { return; }

	ForwardIterator begin_next(begin);
	if (++begin_next == end) { return; }

	ForwardIterator i(begin);
	ForwardIterator i_next(begin_next);
	ForwardIterator last_swap(i);

	for (;;) {
		if (i_next == end) {
			if (begin == (end = last_swap) || (i_next = begin_next) == end) {
				return;
			}

			i = last_swap = begin;
		} else {
			if (lt_cmper.lt(*i_next, *i)) {
				swapper(*i, *i_next);
				last_swap = i_next;
			}

			i = i_next;
			++i_next;
		}
	}
}

#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////

/*
Selection sorts [begin, end). Uses lt_cmper to compare two elements.
Uses swapper to swap elemenets.
*/

template<typename ForwardIterator,
		 typename LessThanComparer = DefaultLessThanComparer,
		 typename Swapper = DefaultSwapper>
void SelectionSort(ForwardIterator begin, ForwardIterator end,
				   const LessThanComparer& lt_cmper = LessThanComparer(),
				   const Swapper& swapper = Swapper()) {
	for (; begin != end; ++begin) {
		swapper(*begin, *Min(begin, end, lt_cmper));
	}
}

#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////

template<typename RandomAccessIterator,
		 typename LessThanComparer = DefaultLessThanComparer,
		 typename Swapper = DefaultSwapper>
void QuickSort(RandomAccessIterator begin, RandomAccessIterator end,
			   const LessThanComparer& lt_cmper = LessThanComparer(),
			   const Swapper& swapper = Swapper()) {
	using Diff = typename iterator::trait<RandomAccessIterator>::Diff;

	Diff diff(end - begin);

	while (Diff(2) < diff) {
		if (Diff(3) < diff) { swapper(*begin, begin[(diff / 2)]); }

		RandomAccessIterator begin_next(begin + 1);
		RandomAccessIterator end_prev(end - 1);

		if (lt_cmper.lt(*end_prev, *begin_next)) {
			swapper(*begin_next, *end_prev);
		}

		if (lt_cmper.lt(*begin, *begin_next)) {
			swapper(*begin, *begin_next);
		} else if (lt_cmper.lt(*end_prev, *begin)) {
			swapper(*begin, *end_prev);
		}

		RandomAccessIterator p(
			Partition(begin + 2, end_prev, *begin, lt_cmper, swapper));
		RandomAccessIterator p_prev(p - 1);

		swapper(*begin, *p_prev);

		Diff a_diff(p_prev - begin);
		Diff b_diff(end - p);

		if (a_diff < b_diff) {
			QuickSort(begin, p_prev, lt_cmper, swapper);
			begin = p;
			diff = b_diff;
		} else {
			QuickSort(p, end, lt_cmper, swapper);
			end = p_prev;
			diff = a_diff;
		}
	}

	if (diff == Diff(2) && lt_cmper.lt(*(--end), *begin)) {
		swapper(*begin, *end);
	}
}

#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////

template<typename RandomAccessIterator,
		 typename LessThanComparer = DefaultLessThanComparer,
		 typename Swapper = DefaultSwapper>
void HeapSort(RandomAccessIterator begin, RandomAccessIterator end,
			  const LessThanComparer& lt_cmper = LessThanComparer(),
			  const Swapper& swapper = Swapper()) {
	using Value = typename iterator::trait<RandomAccessIterator>::Value;
	using Diff = typename iterator::trait<RandomAccessIterator>::Diff;

	Diff diff(end - begin);

	if (diff < Diff(2)) { return; }

	MakeHeap(begin, end, lt_cmper);

	do {
		Value value(Move(*begin));
		--diff;
		HeapifyWithHole(begin, Diff(0), diff, begin[diff], lt_cmper);
		begin[diff] = Move(value);
	} while (Diff(0) < diff);
}

#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////

#define PHI__insertion_sort_threshold (32)

template<typename RandomAccessIterator, typename LessThanComparer,
		 typename Swapper>
void IntroSort_(
	typename iterator::trait<RandomAccessIterator>::Diff depth_limit,
	RandomAccessIterator begin, RandomAccessIterator end,
	const LessThanComparer& lt_cmper, const Swapper& swapper) {
	using Diff = typename iterator::trait<RandomAccessIterator>::Diff;

	while (depth_limit != 0 && PHI__insertion_sort_threshold <= end - begin) {
		depth_limit /= Diff(2);

		RandomAccessIterator begin_next(begin + 1);
		RandomAccessIterator end_prev(end - 1);

		if (lt_cmper.lt(*end_prev, *begin_next)) {
			swapper(*begin_next, *end_prev);
		}

		if (lt_cmper.lt(*begin, *begin_next)) {
			swapper(*begin, *begin_next);
		} else if (lt_cmper.lt(*end_prev, *begin)) {
			swapper(*begin, *end_prev);
		}

		RandomAccessIterator p(
			Partition(begin + 2, end_prev, *begin, lt_cmper, swapper));
		RandomAccessIterator p_prev(p - 1);

		swapper(*begin, *p_prev);

		IntroSort_(depth_limit, p, end, lt_cmper, swapper);
		end = p_prev;
	}

	HeapSort(begin, end, lt_cmper);
}

template<typename RandomAccessIterator,
		 typename LessThanComparer = DefaultLessThanComparer,
		 typename Swapper = DefaultSwapper>
void IntroSort(RandomAccessIterator begin, RandomAccessIterator end,
			   const LessThanComparer& lt_cmper = LessThanComparer(),
			   const Swapper& swapper = Swapper()) {
	IntroSort_(end - begin, begin, end, lt_cmper, swapper);
}

#undef PHI__insertion_sort_threshold

#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////

template<typename RandomAccessIterator,
		 typename LessThanComparer = DefaultLessThanComparer,
		 typename Swapper = DefaultSwapper>
void Nth(typename iterator::trait<RandomAccessIterator>::Diff n,
		 RandomAccessIterator begin, RandomAccessIterator end,
		 const LessThanComparer& lt_cmper = LessThanComparer(),
		 const Swapper& swapper = Swapper()) {
	using Diff = typename iterator::trait<RandomAccessIterator>::Diff;

	Diff diff(end - begin);

	if (diff <= n) { return; }

	for (;;) {
		if (Diff(3) < diff) { swapper(*begin, begin[(diff / 2)]); }

		RandomAccessIterator begin_next(begin + 1);
		RandomAccessIterator end_prev(end - 1);

		if (lt_cmper.lt(*end_prev, *begin_next)) {
			swapper(*begin_next, *end_prev);
		}

		if (lt_cmper.lt(*begin, *begin_next)) {
			swapper(*begin, *begin_next);
		} else if (lt_cmper.lt(*end_prev, *begin)) {
			swapper(*begin, *end_prev);
		}

		RandomAccessIterator p(
			Partition(begin + 2, end_prev, *begin, lt_cmper, swapper));
		RandomAccessIterator p_prev(p - 1);

		swapper(*begin, *p_prev);

		Diff a_diff(p_prev - begin);
		Diff b_diff(end - p);

		if (a_diff < n) {
			n -= a_diff + 1;
			begin = p;
			diff = b_diff;
			continue;
		}

		if (n < a_diff) {
			end = p_prev;
			diff = a_diff;
			continue;
		}

		if (a_diff == n) { return; }
	}
}

#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////

#define PHI__insertion_sort_threshold (64)

template<typename RandomAccessIterator, typename LessThanComparer,
		 typename Swapper>
void Sort_a_(typename iterator::trait<RandomAccessIterator>::Diff limit,
			 RandomAccessIterator begin, RandomAccessIterator end,
			 const LessThanComparer& lt_cmper, const Swapper& swapper) {
	using Diff = typename iterator::trait<RandomAccessIterator>::Diff;

	Diff diff(end - begin);

	if (diff <= Diff(PHI__insertion_sort_threshold)) {
		InsertionSort(begin, end, lt_cmper);
		return;
	}

	if (limit == 0) {
		swapper(*begin, *Min(begin, end, lt_cmper));
		++begin;
		RandomAccessIterator p(end - Diff(PHI__insertion_sort_threshold));
		HeapSort(begin, p, lt_cmper);
		UnrestrictedInsertionSort(p, end, lt_cmper);
		return;
	}

	limit /= Diff(2);

	RandomAccessIterator begin_next(begin);
	++begin_next;
	RandomAccessIterator end_prev(end);
	--end_prev;

	if (lt_cmper.lt(*end_prev, *begin_next)) {
		swapper(*begin_next, *end_prev);
	}

	if (lt_cmper.lt(*begin, *begin_next)) {
		swapper(*begin, *begin_next);
	} else if (lt_cmper.lt(*end_prev, *begin)) {
		swapper(*begin, *end_prev);
	}

	RandomAccessIterator p(UnrestrictedPartition(begin + Diff(2), end_prev,
												 *begin, lt_cmper, swapper));
	RandomAccessIterator p_prev(p);
	--p_prev;

	swapper(*begin, *p_prev);
	Sort_a_(limit, begin, p_prev, lt_cmper, swapper);
	Sort_b_(limit, p, end, lt_cmper, swapper);
}

template<typename RandomAccessIterator, typename LessThanComparer,
		 typename Swapper>
void Sort_b_(typename iterator::trait<RandomAccessIterator>::Diff limit,
			 RandomAccessIterator begin, RandomAccessIterator end,
			 const LessThanComparer& lt_cmper, const Swapper& swapper) {
	using Diff = typename iterator::trait<RandomAccessIterator>::Diff;

	for (;;) {
		Diff diff(end - begin);

		if (diff <= Diff(PHI__insertion_sort_threshold)) {
			UnrestrictedInsertionSort(begin, end, lt_cmper);
			return;
		}

		if (limit == 0) {
			RandomAccessIterator p(end - Diff(PHI__insertion_sort_threshold));
			HeapSort(begin, p, lt_cmper);
			UnrestrictedInsertionSort(p, end, lt_cmper);
			return;
		}

		limit /= Diff(2);

		RandomAccessIterator begin_next(begin);
		++begin_next;
		RandomAccessIterator end_prev(end);
		--end_prev;

		if (lt_cmper.lt(*end_prev, *begin_next)) {
			swapper(*begin_next, *end_prev);
		}

		if (lt_cmper.lt(*begin, *begin_next)) {
			swapper(*begin, *begin_next);
		} else if (lt_cmper.lt(*end_prev, *begin)) {
			swapper(*begin, *end_prev);
		}

		RandomAccessIterator p(UnrestrictedPartition(
			begin + Diff(2), end_prev, *begin, lt_cmper, swapper));
		RandomAccessIterator p_prev(p);
		--p_prev;

		swapper(*begin, *p_prev);
		Sort_b_(limit, begin, p_prev, lt_cmper, swapper);
		begin = p;
	}
}

template<typename RandomAccessIterator,
		 typename LessThanComparer = DefaultLessThanComparer,
		 typename Swapper = DefaultSwapper>
void Sort(RandomAccessIterator begin, RandomAccessIterator end,
		  const LessThanComparer& lt_cmper = LessThanComparer(),
		  const Swapper& swapper = Swapper()) {
	using Diff = typename iterator::trait<RandomAccessIterator>::Diff;

	if (!(begin < end)) { return; }
	Sort_a_((end - begin) * Diff(4), begin, end, lt_cmper, swapper);
}

#undef PHI__insertion_sort_threshold

}

#endif
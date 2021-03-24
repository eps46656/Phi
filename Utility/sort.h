#ifndef PHI__define_guard__Utility__sort_h
#define PHI__define_guard__Utility__sort_h

#include "memory.h"
#include "compare.h"
#include "swap.h"
#include "pair.h"
#include "triple.h"
#include "search.h"
#include "random.h"

namespace phi {

/*
Returns the number of iterators in [begin, end)
*/

template<typename ForwardIterator>
size_t Distance(ForwardIterator begin, ForwardIterator end) {
	size_t r(0);

	while (begin != end) {
		++begin;
		++r;
	}

	return r;
}

#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////

/*
Returns the number of elements equaling to target in range
[src[lower], src[upper]). Uses eq_cmper to compare two elements.
*/

template<typename Src, typename Target,
		 typename EqualComparer = DefaultEqualComparer>
size_t Count(size_t lower, size_t upper, const Src& src, const Target& target,
			 const EqualComparer& eq_cmper = EqualComparer()) {
	size_t r(0);

	for (size_t i(lower); i != upper; ++i) {
		if (eq_cmper.eq(src[i], target)) { ++r; }
	}

	return r;
}

/*
Returns the number of elements equaling to target in range
[src[0], src[size]). Uses eq_cmper to compare two elements.
*/

template<typename Src, typename Target,
		 typename EqualComparer = DefaultEqualComparer>
size_t Count(size_t size, const Src& src, const Target& target,
			 const EqualComparer& eq_cmper = EqualComparer()) {
	return Count(0, size, src, target, eq_cmper);
}

/*
Returns the number of elements equaling to target in range
[begin, end). Uses eq_cmper to compare two elements.
*/

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
Return the index of minimum element in [src[lower], src[upper]). Uses lt_cmper
Uses eq_cmper to compare two elements. If there are two or more elements equal,
undefined which index will return.
*/

template<typename Src, typename LessThanComparer = DefaultLessThanComparer>
size_t Min(size_t lower, size_t upper, Src& src,
		   const LessThanComparer& lt_cmper = LessThanComparer()) {
	if (lower == upper) { return upper; }

	size_t r(lower);

	while (++lower != upper) {
		if (lt_cmper.lt(src[lower], src[r])) { r = lower; }
	}

	return r;
}

/*
Return the index of minimum element in [src[0], src[size]). Uses lt_cmper
Uses eq_cmper to compare two elements. If there are two or more elements equal,
undefined which index will return.
*/

template<typename Src, typename LessThanComparer = DefaultLessThanComparer>
size_t Min(size_t size, Src& src,
		   const LessThanComparer& lt_cmper = LessThanComparer()) {
	return Min(0, size, src, lt_cmper);
}

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

#///////////////////////////////////////////////////////////////////////////////

/*
Return the index of maximum element in [src[lower], src[upper]). Uses lt_cmper
Uses eq_cmper to compare two elements. If there are two or more elements equal,
undefined which index will return.
*/

template<typename Src, typename LessThanComparer = DefaultLessThanComparer>
size_t Max(size_t lower, size_t upper, Src& src,
		   const LessThanComparer& lt_cmper = LessThanComparer()) {
	if (lower == upper) { return upper; }

	size_t r(lower);

	while (++lower != upper) {
		if (lt_cmper.lt(src[r], src[lower])) { r = lower; }
	}

	return r;
}

/*
Return the index of maximum element in [src[0], src[size]). Uses lt_cmper
Uses eq_cmper to compare two elements. If there are two or more elements equal,
undefined which index will return.
*/

template<typename Src, typename LessThanComparer = DefaultLessThanComparer>
size_t Max(size_t size, Src& src,
		   const LessThanComparer& lt_cmper = LessThanComparer()) {
	return Max(0, size, src, lt_cmper);
}

/*
Return the index of maximum element in [begin, end). Uses lt_cmper
Uses eq_cmper to compare two elements. If there are two or more elements equal,
undefined which index will return.
*/

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
size_t TwoWayPartition(size_t lower, size_t upper, Src& src, const Pivot& pivot,
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
TwoWayPartition(size_t size, Src& src, const Pivot& pivot,
				const LessThanComparer& lt_cmper = LessThanComparer(),
				const Swapper& swapper = Swapper()) {
	return TwoWayPartition(0, size, src, pivot, lt_cmper, swapper);
}

#///////////////////////////////////////////////////////////////////////////////

/*
Returns pivot_lt_index and pivot_gt_index.
Partitions [src[lower], src[upper]) into three section [lower, pivot_lt_index),
[pivot_lt_index, pivot_gt_index) and [pivot_gt_index, upper).
Make [src[lower], src[pivot_lt_index]) < [pivot_lt_index, pivot_gt_index) == 
pivot < [pivot_gt_index, upper).
Uses lt_cmper to compare two elements. Uses swapper to swap elemenets.
*/

template<typename Src, typename Pivot,
		 typename FullComparer = DefaultFullComparer,
		 typename Swapper = DefaultSwapper>
pair<size_t, size_t>
ThreeWayPartition(size_t lower, size_t upper, Src& src, const Pivot& pivot,
				  const FullComparer& full_cmper = FullComparer(),
				  const Swapper& swapper = Swapper()) {
#define PHI__cmp(x) (full_cmper(x, pivot))
	size_t size(upper - lower);

	if (size == 0) { return pair<size_t, size_t>(lower, lower); }

	if (size == 1) {
		switch (PHI__cmp(src[lower])) {
			case -1: return pair<size_t, size_t>(upper, upper);
			case 0: return pair<size_t, size_t>(lower, upper);
			case 1: return pair<size_t, size_t>(lower, lower);
		}
	}

	size_t a(lower);
	size_t b(lower);
	size_t c(upper - 1);

	int cmp_b;
	int cmp_c;

	while (b != c) {
		while ((cmp_b = PHI__cmp(src[b])) == 0) {
			if (++b == c) { goto b_eq_c; }
		}

		if (cmp_b == -1) {
			swapper(src[a], src[b]);
			++a;
			++b;
			continue;
		}

		while ((cmp_c = PHI__cmp(src[c])) == 1) {
			if (b == --c) { goto b_eq_c; }
		}

		// cmp_b == 1

		swapper(src[b], src[c]);

		if (cmp_c == -1) {
			swapper(src[a], src[b]);
			++a;
		}

		if (--c < ++b) { return pair<size_t, size_t>(a, b); }
	}

b_eq_c:;

	cmp_b = PHI__cmp(src[b]);

	if (cmp_b == -1) {
		swapper(src[a], src[b]);
		return pair<size_t, size_t>(a + 1, b + 1);
	}

	if (cmp_b == 0) { return pair<size_t, size_t>(a, b + 1); }

	// cmp_b == 1

	return pair<size_t, size_t>(a, b);

#undef PHI__cmp
}

/*
Returns pivot_lt_index and pivot_gt_index.
Partitions [src[0], src[size]) into three section [0, pivot_lt_index),
[pivot_lt_index, pivot_gt_index) and [pivot_gt_index, size).
Make [src[0], src[pivot_lt_index]) < [pivot_lt_index, pivot_gt_index) == 
pivot < [pivot_gt_index, size).
Uses lt_cmper to compare two elements. Uses swapper to swap elemenets.
*/

template<typename Src, typename Pivot,
		 typename FullComparer = DefaultFullComparer,
		 typename Swapper = DefaultSwapper>
pair<size_t, size_t>
ThreeWayPartition(size_t size, Src& src, const Pivot& pivot,
				  const FullComparer& full_cmper = FullComparer(),
				  const Swapper& swapper = Swapper()) {
	return ThreeWayPartition(0, size, src, pivot, full_cmper, swapper);
}

#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////

template<typename Src, typename LessThanComparer, typename Swapper>
void HeadTailSwap(size_t lower, size_t upper, Src& src,
				  const LessThanComparer& lt_cmper = DefaultLessThanComparer(),
				  const Swapper& swapper = DefaultSwapper()) {
	for (; lower < --upper; ++lower) {
		if (lt_cmper.lt(src[upper], src[lower])) {
			swapper(src[lower], src[upper]);
		}
	}
}

template<typename Src, typename LessThanComparer, typename Swapper>
void HeadTailSwap(size_t size, Src& src,
				  const LessThanComparer& lt_cmper = DefaultLessThanComparer(),
				  const Swapper& swapper = DefaultSwapper()) {
	HeadTailSwap(0, size, src, lt_cmper, swapper);
}

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

/*
Insertion sorts [src[lower], src[upper]). Uses lt_cmper to compare two elements.
Uses swapper to swap elemenets.
*/

template<typename Src, typename LessThanComparer = DefaultLessThanComparer>
void InsertionSort(size_t lower, size_t upper, Src& src,
				   const LessThanComparer& lt_cmper = LessThanComparer()) {
	if (upper - lower < 2) { return; }

	size_t i(upper - 1);

	do {
		if (!lt_cmper.lt(src[i], src[i - 1])) {
			--i;
			continue;
		}

		auto temp(Move(src[i - 1]));
		src[i - 1] = Move(src[i]);

		size_t j(i);

		for (; j != upper - 1 && lt_cmper.lt(src[j + 1], temp); ++j) {
			src[j] = Move(src[j + 1]);
		}

		src[j] = Move(temp);
		--i;
	} while (i != lower);
}

/*
Insertion sorts [src[0], src[size]). Uses lt_cmper to compare two elements.
Uses swapper to swap elemenets.
*/

template<typename Src, typename LessThanComparer = DefaultLessThanComparer>
void InsertionSort(size_t size, Src& src,
				   const LessThanComparer& lt_cmper = LessThanComparer()) {
	InsertionSort(0, size, src, lt_cmper);
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

	BidirectionalIterator i(end);
	--i;

	while (i != begin) {
		BidirectionalIterator i_prev(i);
		--i_prev;

		if (lt_cmper.lt(*i, *i_prev)) {
			auto temp(*i_prev);
			*i_prev = Move(*i);

			BidirectionalIterator j(i);

			for (;;) {
				BidirectionalIterator j_next(j);
				if (++j_next == end) { break; }
				if (!lt_cmper.lt(*j_next, temp)) { break; }
				*j = Move(*j_next);
				j = j_next;
			}

			*j = Move(temp);
		}

		i = i_prev;
	}
}

#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////

/*
Bubble sorts [src[lower], src[upper]). Uses lt_cmper to compare two elements.
Uses swapper to swap elemenets.
*/

template<typename Src, typename LessThanComparer = DefaultLessThanComparer,
		 typename Swapper = DefaultSwapper>
void BubbleSort(size_t lower, size_t upper, Src& src,
				const LessThanComparer& lt_cmper = LessThanComparer(),
				const Swapper& swapper = Swapper()) {
	for (; 1 < upper - lower; --upper) {
		for (size_t i(lower); i != upper - 1; ++i) {
			if (lt_cmper.lt(src[i + 1], src[i])) {
				swapper(src[i], src[i + 1]);
			}
		}
	}
}

/*
Bubble sorts [src[0], src[size]). Uses lt_cmper to compare two elements.
Uses swapper to swap elemenets.
*/

template<typename Src, typename LessThanComparer = DefaultLessThanComparer,
		 typename Swapper = DefaultSwapper>
void BubbleSort(size_t size, Src& src,
				const LessThanComparer& lt_cmper = LessThanComparer(),
				const Swapper& swapper = Swapper()) {
	BubbleSort(0, size, src, lt_cmper, swapper);
}

/*
Bubble sorts [begin, end). Uses lt_cmper to compare two elements.
Uses swapper to swap elemenets.
*/

template<typename ForwardIterator,
		 typename LessThanComparer = DefaultLessThanComparer,
		 typename Swapper = DefaultSwapper>
void BubbleSort(ForwardIterator begin, ForwardIterator end,
				const LessThanComparer& lt_cmper = LessThanComparer(),
				const Swapper& swapper = Swapper()) {
	if (begin == end) { return; }

	ForwardIterator i(begin);
	ForwardIterator next_i(i);
	++next_i;

	while (begin != end) {
		if (next_i == end) {
			end = i;
			next_i = begin;
		} else if (lt_cmper.lt(*next_i, *i)) {
			swapper(*i, *next_i);
		}

		i = next_i;
		++next_i;
	}
}

/*
template<typename Bidirectioanl,
		 typename LessThanComparer = DefaultLessThanComparer,
		 typename Swapper = DefaultSwapper>
void BubbleSort(Bidirectioanl begin, Bidirectioanl end,
				const LessThanComparer& lt_cmper = LessThanComparer(),
				const Swapper& swapper = Swapper()) {
	if (begin == end) { return; }

	HeadTailSwap(begin, end, lt_cmper, swapper);

	do {
		Bidirectioanl i(begin);
		Bidirectioanl i_next(begin);
		++i_next;
		Bidirectioanl last_swap(i);

		while (i_next != end) {
			if (lt_cmper.lt(*i_next, *i)) {
				swapper(*i_next, *i);
				last_swap = i_next;
			}

			i = i_next;
			++i_next;
		}

		end = last_swap;
	} while (begin != end);
}
*/

#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////

/*
Selection sorts [src[lower], src[upper]). Uses lt_cmper to compare two elements.
Uses swapper to swap elemenets.
*/

template<typename Src, typename LessThanComparer = DefaultLessThanComparer,
		 typename Swapper = DefaultSwapper>
void SelectionSort(size_t lower, size_t upper, Src& src,
				   const LessThanComparer& lt_cmper = LessThanComparer(),
				   const Swapper& swapper = Swapper()) {
	for (; lower != upper; ++lower) {
		swapper(src[lower], src[Min(lower, upper, src, lt_cmper)]);
	}
}

/*
Selection sorts [src[0], src[size]). Uses lt_cmper to compare two elements.
Uses swapper to swap elemenets.
*/

template<typename Src, typename LessThanComparer = DefaultLessThanComparer,
		 typename Swapper = DefaultSwapper>
void SelectionSort(size_t size, Src& src,
				   const LessThanComparer& lt_cmper = LessThanComparer(),
				   const Swapper& swapper = Swapper()) {
	SelectionSort(0, size, src, lt_cmper, swapper);
}

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

template<typename Src, typename LessThanComparer = DefaultLessThanComparer,
		 typename Swapper = DefaultSwapper>
void TwoWayQuickSort(size_t lower, size_t upper, Src& src,
					 const LessThanComparer& lt_cmper = LessThanComparer(),
					 const Swapper& swapper = Swapper()) {
	while (2 < upper - lower) {
		if (3 < upper - lower) {
			swapper(src[lower], src[(lower + upper) / 2]);
		}

		if (lt_cmper.lt(src[upper - 1], src[lower + 1])) {
			swapper(src[lower + 1], src[upper - 1]);
		}

		if (lt_cmper.lt(src[lower], src[lower + 1])) {
			swapper(src[lower], src[lower + 1]);
		} else if (lt_cmper.lt(src[upper - 1], src[lower])) {
			swapper(src[lower], src[upper - 1]);
		}

		size_t p(TwoWayPartition(lower + 2, upper - 1, src, src[lower],
								 lt_cmper, swapper));

		swapper(src[lower], src[--p]);

		bool a_size(p - lower);
		bool b_size(upper - p);

		if (a_size < b_size) {
			TwoWayQuickSort(lower, p, src, lt_cmper, swapper);
			lower = p + 1;
		} else {
			TwoWayQuickSort(p + 1, upper, src, lt_cmper, swapper);
			upper = p;
		}
	}

	if ((upper - lower == 2) && lt_cmper.lt(src[upper - 1], src[lower])) {
		swapper(src[lower], src[upper - 1]);
	}
}

template<typename Src, typename LessThanComparer = DefaultLessThanComparer,
		 typename Swapper = DefaultSwapper>
void TwoWayQuickSort(size_t size, Src& src,
					 const LessThanComparer& lt_cmper = LessThanComparer(),
					 const Swapper& swapper = Swapper()) {
	TwoWayQuickSort(0, size, src, lt_cmper, swapper);
}

#///////////////////////////////////////////////////////////////////////////////

template<typename Src, typename FullComparer = DefaultFullComparer,
		 typename Swapper = DefaultSwapper>
void ThreeWayQuickSort(size_t lower, size_t upper, Src& src,
					   const FullComparer& full_cmper = FullComparer(),
					   const Swapper& swapper = Swapper()) {
	while (2 < upper - lower) {
		if (3 < upper - lower) {
			swapper(src[lower], src[(lower + upper) / 2]);
		}

		if (full_cmper.lt(src[upper - 1], src[lower + 1])) {
			swapper(src[lower + 1], src[upper - 1]);
		}

		if (full_cmper.lt(src[lower], src[lower + 1])) {
			swapper(src[lower], src[lower + 1]);
		} else if (full_cmper.lt(src[upper - 1], src[lower])) {
			swapper(src[lower], src[upper - 1]);
		}

		pair<size_t, size_t> p(ThreeWayPartition(
			lower + 2, upper - 1, src, src[lower], full_cmper, swapper));

		swapper(src[lower], src[--p.first]);

		bool a_size(p.first - lower);
		bool b_size(upper - p.second);

		if (a_size < b_size) {
			ThreeWayQuickSort(lower, p.first, src, full_cmper, swapper);
			lower = p.second;
		} else {
			ThreeWayQuickSort(p.second, upper, src, full_cmper, swapper);
			upper = p.first;
		}
	}

	if ((upper - lower == 2) && full_cmper.lt(src[upper - 1], src[lower])) {
		swapper(src[lower], src[upper - 1]);
	}
}

template<typename Src, typename FullComparer = DefaultFullComparer,
		 typename Swapper = DefaultSwapper>
void ThreeWayQuickSort(size_t size, Src& src,
					   const FullComparer& full_cmper = FullComparer(),
					   const Swapper& swapper = Swapper()) {
	ThreeWayQuickSort(0, size, src, full_cmper, swapper);
}

#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////

template<typename Src, typename LessThanComparer = DefaultLessThanComparer,
		 typename T, typename Swapper = DefaultSwapper>
void Heapify(size_t lower, size_t upper, size_t i, Src& src, T& value,
			 const LessThanComparer& lt_cmper = LessThanComparer(),
			 const Swapper& swapper = Swapper()) {
	for (;;) {
		size_t c(lower + (i - lower) * 2 + 1);

		if (upper <= c) { break; }
		if (c + 1 < upper && lt_cmper.lt(src[c], src[c + 1])) { ++c; }

		if (!lt_cmper.lt(value, src[c])) { break; }

		src[i] = Move(src[c]);
		i = c;
	}

	src[i] = Move(value);
}

template<typename Src, typename LessThanComparer = DefaultLessThanComparer,
		 typename Swapper = DefaultSwapper>
void HeapSort(size_t lower, size_t upper, Src& src,
			  const LessThanComparer& lt_cmper = LessThanComparer(),
			  const Swapper& swapper = Swapper()) {
	size_t size(upper - lower);

	if (size < 2) { return; }

	for (size_t i(size / 2); i != 0;) {
		--i;

		size_t p(lower + i);
		size_t c(lower + i * 2 + 1);

		if (c + 1 < upper && lt_cmper.lt(src[c], src[c + 1])) { ++c; }
		if (!lt_cmper.lt(src[p], src[c])) { continue; }

		auto temp(Move(src[p]));
		src[p] = Move(src[c]);

		Heapify(lower, upper, c, src, temp, lt_cmper, swapper);
	}

	for (; 1 < upper - lower; --upper) {
		auto temp(Move(src[lower]));
		Heapify(lower, upper, lower, src, src[upper - 1], lt_cmper, swapper);
		src[upper - 1] = Move(temp);
	}
}

template<typename Src, typename LessThanComparer = DefaultLessThanComparer,
		 typename Swapper = DefaultSwapper>
void HeapSort(size_t size, Src& src,
			  const LessThanComparer& lt_cmper = LessThanComparer(),
			  const Swapper& swapper = Swapper()) {
	HeapSort(0, size, src, lt_cmper, swapper);
}

#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////

#define PHI__intro_sort_depth_limit (32)
#define PHI__heap_sort_threshold (32)

template<typename Src, typename LessThanComparer, typename Swapper>
void TwoWayIntroSort_(size_t depth_limit, size_t lower, size_t upper, Src& src,
					  const LessThanComparer& lt_cmper,
					  const Swapper& swapper) {
	if ((depth_limit == 0) | (upper - lower) < PHI__heap_sort_threshold) {
		HeapSort(lower, upper, src, lt_cmper);
		return;
	}

	while (PHI__heap_sort_threshold < upper - lower) {
		swapper(src[lower], src[(lower + upper) / 2]);

		if (lt_cmper.lt(src[upper - 1], src[lower + 1])) {
			swapper(src[lower + 1], src[upper - 1]);
		}

		if (lt_cmper.lt(src[lower], src[lower + 1])) {
			swapper(src[lower], src[lower + 1]);
		} else if (lt_cmper.lt(src[upper - 1], src[lower])) {
			swapper(src[lower], src[upper - 1]);
		}

		size_t p(TwoWayPartition(lower + 2, upper - 1, src, src[lower],
								 lt_cmper, swapper));

		swapper(src[lower], src[--p]);

		bool a_size(p - lower);
		bool b_size(upper - p - 1);

		if (a_size < b_size) {
			TwoWayIntroSort_(depth_limit - 1, lower, p, src, lt_cmper, swapper);
			lower = p + 1;
		} else {
			TwoWayIntroSort_(depth_limit - 1, p + 1, upper, src, lt_cmper,
							 swapper);
			upper = p;
		}
	}

	HeapSort(lower, upper, src, lt_cmper);
}

template<typename Src, typename LessThanComparer = DefaultLessThanComparer,
		 typename Swapper = DefaultSwapper>
void TwoWayIntroSort(size_t lower, size_t upper, Src& src,
					 const LessThanComparer& lt_cmper = LessThanComparer(),
					 const Swapper& swapper = Swapper()) {
	TwoWayIntroSort_(PHI__intro_sort_depth_limit, lower, upper, src, lt_cmper,
					 swapper);
}

template<typename Src, typename LessThanComparer = DefaultLessThanComparer,
		 typename Swapper = DefaultSwapper>
void TwoWayIntroSort(size_t size, Src& src,
					 const LessThanComparer& lt_cmper = LessThanComparer(),
					 const Swapper& swapper = Swapper()) {
	TwoWayIntroSort(0, size, src, lt_cmper, swapper);
}

#///////////////////////////////////////////////////////////////////////////////

template<typename Src, typename FullComparer, typename Swapper,
		 typename MakePivotAtLowerFunc>
void ThreeWayIntroSort_(size_t depth_limit, size_t lower, size_t upper,
						Src& src, const FullComparer& full_cmper,
						const Swapper& swapper) {
	if ((depth_limit == 0) || (upper - lower < PHI__heap_sort_threshold)) {
		HeapSort(lower, upper, src, full_cmper, swapper);
		return;
	}

	while (PHI__heap_sort_threshold < upper - lower) {
		swapper(src[lower], src[(lower + upper) / 2]);

		if (full_cmper.lt(src[upper - 1], src[lower + 1])) {
			swapper(src[lower + 1], src[upper - 1]);
		}

		if (full_cmper.lt(src[lower], src[lower + 1])) {
			swapper(src[lower], src[lower + 1]);
		} else if (full_cmper.lt(src[upper - 1], src[lower])) {
			swapper(src[lower], src[upper - 1]);
		}

		pair<size_t, size_t> p(ThreeWayPartition(
			lower + 2, upper - 1, src, src[lower], full_cmper, swapper));

		swapper(src[lower], src[--p.first]);

		bool a_size(p.first - lower);
		bool b_size(upper - p.second);

		if (a_size < b_size) {
			ThreeWayIntroSort_(depth_limit - 1, lower, p.first, src, full_cmper,
							   swapper);
			lower = p.second;
		} else {
			ThreeWayIntroSort_(depth_limit - 1, p.second, upper, src,
							   full_cmper, swapper);
			upper = p.first;
		}
	}

	HeapSort(lower, upper, src, full_cmper, swapper);
}

template<typename Src, typename FullComparer = DefaultFullComparer,
		 typename Swapper = DefaultSwapper>
void ThreeWayIntroSort(size_t lower, size_t upper, Src& src,
					   const FullComparer& full_cmper = FullComparer(),
					   const Swapper& swapper = Swapper()) {
	ThreeWayIntroSort_(PHI__intro_sort_depth_limit, lower, upper, src,
					   full_cmper, swapper);
}

template<typename Src, typename FullComparer = DefaultFullComparer,
		 typename Swapper = DefaultSwapper>
void ThreeWayIntroSort(size_t size, Src& src,
					   const FullComparer& full_cmper = FullComparer(),
					   const Swapper& swapper = Swapper()) {
	ThreeWayIntroSort(0, size, src, full_cmper, swapper);
}

#undef PHI__intro_sort_depth_limit
#undef PHI__heap_sort_threshold

}

#endif
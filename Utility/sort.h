#ifndef PHI__define_guard__Utility__sort_h
#define PHI__define_guard__Utility__sort_h

#include "memory.h"
#include "compare.h"
#include "swap.h"
#include "pair.h"
#include "triple.h"
#include "search.h"

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
			 EqualComparer eq_cmper = EqualComparer()) {
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
			 EqualComparer eq_cmper = EqualComparer()) {
	return Count(0, size, src, target, eq_cmper);
}

/*
Returns the number of elements equaling to target in range
[begin, end). Uses eq_cmper to compare two elements.
*/

template<typename ForwardIterator, typename Target,
		 typename EqualComparer = DefaultEqualComparer>
size_t Count(ForwardIterator begin, ForwardIterator end, const Target& target,
			 EqualComparer eq_cmper = EqualComparer()) {
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
		   LessThanComparer lt_cmper = LessThanComparer()) {
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
		   LessThanComparer lt_cmper = LessThanComparer()) {
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
					LessThanComparer lt_cmper = LessThanComparer()) {
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
		   LessThanComparer lt_cmper = LessThanComparer()) {
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
		   LessThanComparer lt_cmper = LessThanComparer()) {
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
					LessThanComparer lt_cmper = LessThanComparer()) {
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
size_t TwoWayPartition(size_t lower, size_t upper, Src& src, Pivot& pivot,
					   LessThanComparer lt_cmper = LessThanComparer(),
					   Swapper swapper = Swapper()) {
	if (lower == upper) { return lower; }

	size_t a(lower);
	size_t b(upper - 1);

	while (a != b) {
		while (!lt_cmper.lt(pivot, src[a])) {
			if (++a == b) { goto a_eq_b; }
		}

		while (!lt_cmper.lt(src[b], pivot)) {
			if (a == --b) { goto a_eq_b; }
		}

		swapper(src[a], src[b]);
		if (--b < ++a) { return a; }
	}

a_eq_b:;

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
TwoWayPartition(size_t size, Src& src, Pivot& pivot,
				LessThanComparer lt_cmper = LessThanComparer(),
				Swapper swapper = Swapper()) {
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
pair<size_t, size_t> ThreeWayPartition(size_t lower, size_t upper, Src& src,
									   Pivot& pivot,
									   FullComparer full_cmper = FullComparer(),
									   Swapper swapper = Swapper()) {
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
pair<size_t, size_t> ThreeWayPartition(size_t size, Src& src, Pivot& pivot,
									   FullComparer full_cmper = FullComparer(),
									   Swapper swapper = Swapper()) {
	return ThreeWayPartition(0, size, src, pivot, full_cmper, swapper);
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
				   LessThanComparer lt_cmper = LessThanComparer()) {
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
				   LessThanComparer lt_cmper = LessThanComparer()) {
	InsertionSort(0, size, src, lt_cmper);
}

/*
Insertion sorts [begin, end). Uses lt_cmper to compare two elements.
Uses swapper to swap elemenets.
*/

template<typename BidirectionalIterator,
		 typename LessThanComparer = DefaultLessThanComparer>
void InsertionSort(BidirectionalIterator begin, BidirectionalIterator end,
				   LessThanComparer lt_cmper = LessThanComparer()) {
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
				LessThanComparer lt_cmper = LessThanComparer(),
				Swapper swapper = Swapper()) {
	if (upper - lower < 2) { return; }

	while (--upper != lower) {
		for (size_t i(lower); i != upper; ++i) {
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
				LessThanComparer lt_cmper = LessThanComparer(),
				Swapper swapper = Swapper()) {
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
				LessThanComparer lt_cmper = LessThanComparer(),
				Swapper swapper = Swapper()) {
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
				   LessThanComparer lt_cmper = LessThanComparer(),
				   Swapper swapper = Swapper()) {
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
				   LessThanComparer lt_cmper = LessThanComparer(),
				   Swapper swapper = Swapper()) {
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
				   LessThanComparer lt_cmper = LessThanComparer(),
				   Swapper swapper = Swapper()) {
	for (; begin != end; ++begin) {
		swapper(*begin, *Min(begin, end, lt_cmper));
	}
}

#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////

struct DefaultMakePivotAtLowerFunc {
	template<typename Src, typename LessThanComparer = DefaultLessThanComparer,
			 typename Swapper>
	void operator()(size_t lower, size_t upper, Src& src,
					LessThanComparer lt_cmper, Swapper swapper) {}
};

template<typename Src, typename LessThanComparer = DefaultLessThanComparer,
		 typename Swapper = DefaultSwapper,
		 typename MakePivotAtLowerFunc = DefaultMakePivotAtLowerFunc>
void TwoWayQuickSort(
	size_t lower, size_t upper, Src& src,
	LessThanComparer lt_cmper = LessThanComparer(), Swapper swapper = Swapper(),
	MakePivotAtLowerFunc make_pivot_at_lower = MakePivotAtLowerFunc()) {
	using T = decltype(src[lower]);

	if (upper - lower < 2) { return; }

	make_pivot_at_lower(lower, upper, src, lt_cmper, swapper);

	T& pivot(src[lower]);

	size_t p(TwoWayPartition(lower + 1, upper, src, pivot, lt_cmper, swapper));

	swapper(pivot, src[p - 1]);

	bool a_size(p - 1 - lower);
	bool b_size(upper - p);

#define PHI__threshold_size_to_use_insertion_sort 32
#define PHI__quick_sort_a                                                      \
	TwoWayQuickSort(lower, p - 1, src, lt_cmper, swapper, make_pivot_at_lower)
#define PHI__quick_sort_b                                                      \
	TwoWayQuickSort(p, upper, src, lt_cmper, swapper, make_pivot_at_lower)
#define PHI__insertion_sort_a                                                  \
	InsertionSort(lower, p - 1, src, lt_cmper, swapper)
#define PHI__insertion_sort_b InsertionSort(p, upper, src, lt_cmper, swapper)

	if (a_size <= PHI__threshold_size_to_use_insertion_sort) {
		PHI__insertion_sort_a;

		if (b_size <= PHI__threshold_size_to_use_insertion_sort) {
			PHI__insertion_sort_b;
		} else {
			PHI__quick_sort_b;
		}
	} else if (b_size <= PHI__threshold_size_to_use_insertion_sort) {
		PHI__insertion_sort_b;
		PHI__quick_sort_a;
	} else if (a_size < b_size) {
		PHI__quick_sort_a;
		PHI__quick_sort_b;
	} else {
		PHI__quick_sort_b;
		PHI__quick_sort_a;
	}

#undef PHI__threshold_size_to_use_insertion_sort
#undef PHI__quick_sort_a
#undef PHI__quick_sort_b
#undef PHI__insertion_sort_a
#undef PHI__insertion_sort_b
}

template<typename Src, typename LessThanComparer = DefaultLessThanComparer,
		 typename Swapper = DefaultSwapper,
		 typename MakePivotAtLowerFunc = DefaultMakePivotAtLowerFunc>
void TwoWayQuickSort(
	size_t size, Src& src, LessThanComparer lt_cmper = LessThanComparer(),
	Swapper swapper = Swapper(),
	MakePivotAtLowerFunc make_pivot_at_lower = MakePivotAtLowerFunc()) {
	TwoWayQuickSort(0, size, src, lt_cmper, swapper, make_pivot_at_lower);
}

#///////////////////////////////////////////////////////////////////////////////

template<typename Src, typename FullComparer = DefaultFullComparer,
		 typename Swapper = DefaultSwapper,
		 typename MakePivotAtLowerFunc = DefaultMakePivotAtLowerFunc>
void ThreeWayQuickSort(
	size_t lower, size_t upper, Src& src,
	FullComparer full_cmper = FullComparer(), Swapper swapper = Swapper(),
	MakePivotAtLowerFunc make_pivot_at_lower = MakePivotAtLowerFunc()) {
	using T = decltype(src[lower]);

	if (upper - lower < 2) { return; }

	make_pivot_at_lower(lower, upper, src, full_cmper, swapper);

	T& pivot(src[lower]);

	pair<size_t, size_t> p(
		ThreeWayPartition(lower + 1, upper, src, pivot, full_cmper, swapper));

	swapper(pivot, src[--p.first]);

	bool a_size(p.first - lower);
	bool b_size(upper - p.second);

#define PHI__threshold_size_to_use_insertion_sort 32
#define PHI__quick_sort_a                                                      \
	ThreeWayQuickSort(lower, p.first, src, full_cmper, swapper,                \
					  make_pivot_at_lower);
#define PHI__quick_sort_b                                                      \
	ThreeWayQuickSort(p.second, upper, src, full_cmper, swapper,               \
					  make_pivot_at_lower)
#define PHI__insertion_sort_a                                                  \
	InsertionSort(lower, p.first, src, full_cmper, swapper)
#define PHI__insertion_sort_b                                                  \
	InsertionSort(p.second, upper, src, full_cmper, swapper)

	if (a_size <= PHI__threshold_size_to_use_insertion_sort) {
		PHI__insertion_sort_a;

		if (b_size <= PHI__threshold_size_to_use_insertion_sort) {
			PHI__insertion_sort_b;
		} else {
			PHI__quick_sort_b;
		}
	} else if (b_size <= PHI__threshold_size_to_use_insertion_sort) {
		PHI__insertion_sort_b;
		PHI__quick_sort_a;
	} else if (a_size < b_size) {
		PHI__quick_sort_a;
		PHI__quick_sort_b;
	} else {
		PHI__quick_sort_b;
		PHI__quick_sort_a;
	}

#undef PHI__threshold_size_to_use_insertion_sort
#undef PHI__quick_sort_a
#undef PHI__quick_sort_b
#undef PHI__insertion_sort_a
#undef PHI__insertion_sort_b
}

template<typename Src, typename FullComparer = DefaultFullComparer,
		 typename Swapper = DefaultSwapper,
		 typename MakePivotAtLowerFunc = DefaultMakePivotAtLowerFunc>
void ThreeWayQuickSort(
	size_t size, Src& src, FullComparer full_cmper = FullComparer(),
	Swapper swapper = Swapper(),
	MakePivotAtLowerFunc make_pivot_at_lower = MakePivotAtLowerFunc()) {
	ThreeWayQuickSort(0, size, src, full_cmper, swapper, make_pivot_at_lower);
}
}

#endif
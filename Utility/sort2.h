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
#include "../Container/Vector.h"

namespace phi {

template<typename ForwardIterator>
cntr::Vector<pair<size_t, ForwardIterator>>
SchwartzianTransform(ForwardIterator begin, ForwardIterator end) {
	cntr::Vector<pair<size_t, ForwardIterator>> r;
	r.Reserve(Distance(begin, end));
	for (size_t i(0); begin != end; ++begin, ++i) { r.Push(i, begin); }
	return r;
}

template<typename ForwardIterator>
void InverseSchwartzianTransform(
	cntr::Vector<pair<size_t, ForwardIterator>>& v) {
	using Value = typename iterator::trait<ForwardIterator>::Value;

	for (size_t i(0); i != v.size(); ++i) {
		if (i == v[i].first) { continue; }

		size_t temp_i(i);
		Value temp(Move(*v[i].second));
		size_t hole(i);

		do {
			size_t next_hole = v[hole].first;
			*v[hole].second = Move(*v[next_hole].second);
			v[hole].first = hole;
			hole = next_hole;
		} while (v[hole].first != temp_i);

		*v[hole].second = Move(temp);
		v[hole].first = hole;
	}
}

template<typename ForwardIterator> struct SchwartzianAdapter {
	typename iterator::trait<ForwardIterator>::Ref
	operator()(pair<size_t, ForwardIterator>& x) {
		return *x.second;
	}

	typename iterator::trait<ForwardIterator>::Ref
	operator()(pair<size_t, ForwardIterator>& x) const {
		return *x.second;
	}
};

template<typename ForwardIterator, typename FullComparer = DefaultFullComparer>
using SchwartzianComparer =
	FullComparerAdapter<FullComparer, SchwartzianAdapter<ForwardIterator>>;

#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////

template<typename Value, typename LessThanComparer = DefaultLessThanComparer,
		 typename Swapper = DefaultSwapper>
void SwapIfDisorder(Value& x, Value& y,
					LessThanComparer&& lt_cmper = DefaultLessThanComparer(),
					Swapper&& swapper = DefaultSwapper()) {
	if (lt_cmper.lt(y, x)) { swapper(x, y); }
}

template<typename ForwardIterator,
		 typename LessThanComparer = DefaultLessThanComparer,
		 typename Swapper = DefaultSwapper>
void MakeMinAtBegin(ForwardIterator begin, ForwardIterator end,
					LessThanComparer&& lt_cmper = LessThanComparer(),
					Swapper&& swapper = Swapper()) {
	if (begin == end) { return; }
	ForwardIterator i(Min(begin, end, lt_cmper));
	if (i != begin) { swapper(*begin, *i); }
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
						  LessThanComparer&& lt_cmper = LessThanComparer(),
						  Swapper&& swapper = Swapper()) {
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
		  const Pivot& pivot, LessThanComparer&& lt_cmper = LessThanComparer(),
		  Swapper&& swapper = Swapper()) {
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
				   LessThanComparer&& lt_cmper = LessThanComparer(),
				   Swapper&& swapper = Swapper()) {
	return Partition(typename iterator::trait<Iterator>::Type(), begin, end,
					 pivot, lt_cmper, swapper);
}

template<typename RandomAccessIterator, typename LessThanComparer,
		 typename Swapper>
RandomAccessIterator
UnrestrictedPartition(RandomAccessIterator begin, RandomAccessIterator end,
					  typename iterator::trait<RandomAccessIterator>::Ref pivot,
					  LessThanComparer&& lt_cmper, Swapper&& swapper) {
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
				  LessThanComparer&& lt_cmper = DefaultLessThanComparer(),
				  Swapper&& swapper = DefaultSwapper()) {
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
				  LessThanComparer&& lt_cmper = LessThanComparer()) {
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
void UnrestrictedLinearInsertWithHole(
	BidirectionalIterator hole,
	typename iterator::trait<BidirectionalIterator>::Ref value,
	LessThanComparer&& lt_cmper = LessThanComparer()) {
	BidirectionalIterator hole_prev(hole);
	--hole_prev;

	while (lt_cmper.lt(value, *hole_prev)) {
		*hole = Move(*hole_prev);
		hole = hole_prev;
		--hole_prev;
	}

	*hole = Move(value);
}

template<typename BidirectionalIterator,
		 typename LessThanComparer = DefaultLessThanComparer>
void UnrestrictedLinearInsertWithValue(
	BidirectionalIterator last,
	typename iterator::trait<BidirectionalIterator>::Ref value,
	LessThanComparer&& lt_cmper = LessThanComparer()) {
	if (!lt_cmper.lt(value, *last)) { return; }
	typename iterator::trait<BidirectionalIterator>::Value temp(Move(*last));
	UnrestrictedLinearInsertWithHole(last, value, lt_cmper);
	value = Move(temp);
}

template<typename BidirectionalIterator,
		 typename LessThanComparer = DefaultLessThanComparer>
void LinearInsertWithValue(
	BidirectionalIterator begin, BidirectionalIterator end,
	typename iterator::trait<BidirectionalIterator>::Ref value,
	LessThanComparer&& lt_cmper = LessThanComparer()) {
	if (begin == end) { return; }

	--end;

	if (!lt_cmper.lt(value, *begin)) {
		UnrestrictedLinearInsertWithValue(end, value, lt_cmper);
		return;
	}

	typename iterator::trait<BidirectionalIterator>::Value temp(Move(value));

	value = Move(*end);

	for (BidirectionalIterator end_prev(end); begin != end; end = end_prev) {
		--end_prev;
		*end = Move(*end_prev);
	}

	*begin = Move(temp);
}

template<typename BidirectionalIterator,
		 typename LessThanComparer = DefaultLessThanComparer>
void UnrestrictedLinearInsert(
	BidirectionalIterator last,
	LessThanComparer&& lt_cmper = LessThanComparer()) {
	BidirectionalIterator last_prev(last);
	--last_prev;

	if (!lt_cmper.lt(*last, *last_prev)) { return; }

	typename iterator::trait<BidirectionalIterator>::Value value(Move(*last));

	*last = Move(*last_prev);

	UnrestrictedLinearInsertWithHole(last_prev, value, lt_cmper);
}

template<typename BidirectionalIterator,
		 typename LessThanComparer = DefaultLessThanComparer>
void UnrestrictedInsertionSort(
	BidirectionalIterator begin, BidirectionalIterator end,
	LessThanComparer&& lt_cmper = LessThanComparer()) {
	for (; begin != end; ++begin) { UnrestrictedLinearInsert(begin, lt_cmper); }
}

/*
Insertion sorts [begin, end). Uses lt_cmper to compare two elements.
Uses swapper to swap elemenets.
*/

template<typename BidirectionalIterator,
		 typename LessThanComparer = DefaultLessThanComparer>
void InsertionSort(BidirectionalIterator begin, BidirectionalIterator end,
				   LessThanComparer&& lt_cmper = LessThanComparer()) {
	if (begin == end) { return; }
	SwapIfDisorder(*begin, *Min(begin, end, lt_cmper), lt_cmper);
	UnrestrictedInsertionSort(++begin, end, lt_cmper);
}

#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////

template<typename BidirectionalIterator,
		 typename LessThanComparer = DefaultLessThanComparer>
void BubblePush(BidirectionalIterator begin, BidirectionalIterator end,
				LessThanComparer&& lt_cmper = LessThanComparer()) {
	if (begin == end || begin == --end) { return; }

	BidirectionalIterator end_prev(end);

	do {
		--end_prev;

		if (lt_cmper.lt(*end, *end_prev)) {
			typename iterator::trait<BidirectionalIterator>::Value temp(
				Move(*end));

			do {
				*end = Move(*end_prev);

				if (end_prev == begin) {
					*begin = Move(temp);
					return;
				}

				end = end_prev;
				--end_prev;
			} while (lt_cmper.lt(temp, *end_prev));

			*end = Move(temp);
		}

		end = end_prev;
	} while (begin != end);
}

template<typename ForwardIterator,
		 typename LessThanComparer = DefaultLessThanComparer,
		 typename Swapper = DefaultSwapper>
void BubbleSort(ForwardIterator begin, ForwardIterator end,
				LessThanComparer&& lt_cmper = LessThanComparer(),
				Swapper&& swapper = Swapper()) {
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
				   LessThanComparer&& lt_cmper = LessThanComparer(),
				   Swapper&& swapper = Swapper()) {
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
			   LessThanComparer&& lt_cmper = LessThanComparer(),
			   Swapper&& swapper = Swapper()) {
	using Diff = typename iterator::trait<RandomAccessIterator>::Diff;

	for (;;) {
		Diff diff(end - begin);

		if (diff <= Diff(8)) {
			InsertionSort(begin, end, lt_cmper);
			return;
		}

		RandomAccessIterator begin_next(begin + Diff(1));
		RandomAccessIterator end_prev(end - Diff(1));

		swapper(*begin_next, *(begin + diff / 2));

		SwapIfDisorder(*begin_next, *end_prev);

		if (lt_cmper.lt(*begin_next, *begin)) {
			swapper(*begin, *begin_next);
			SwapIfDisorder(*begin_next, *end_prev);
		}

		RandomAccessIterator p(UnrestrictedPartition(
			begin + Diff(2), end_prev, *(begin + Diff(1)), lt_cmper, swapper));
		RandomAccessIterator p_prev(p - Diff(1));

		swapper(*begin_next, *p_prev);
		QuickSort(p, end, lt_cmper, swapper);
		end = p_prev;
	}
}

#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////

template<typename RandomAccessIterator,
		 typename LessThanComparer = DefaultLessThanComparer,
		 typename Swapper = DefaultSwapper>
void HeapSort(RandomAccessIterator begin, RandomAccessIterator end,
			  LessThanComparer&& lt_cmper = LessThanComparer(),
			  Swapper&& swapper = Swapper()) {
	using Value = typename iterator::trait<RandomAccessIterator>::Value;
	using Diff = typename iterator::trait<RandomAccessIterator>::Diff;

	Diff diff(end - begin);

	if (diff < Diff(2)) { return; }

	MakeHeap(begin, end, lt_cmper);

	do {
		--diff;
		Value value(Move(*begin));
		HeapifyWithHole(begin, Diff(0), diff, begin[diff], lt_cmper);
		begin[diff] = Move(value);
	} while (Diff(0) < diff);
}

#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////

template<typename RandomAccessIterator, typename LessThanComparer,
		 typename Swapper>
void IntroSort_(
	typename iterator::trait<RandomAccessIterator>::Diff depth_limit,
	RandomAccessIterator begin, RandomAccessIterator end,
	LessThanComparer&& lt_cmper, Swapper&& swapper) {
	using Diff = typename iterator::trait<RandomAccessIterator>::Diff;

	while (depth_limit != Diff(0) && Diff(8) <= end - begin) {
		depth_limit /= Diff(2);

		RandomAccessIterator begin_next(begin + Diff(1));
		RandomAccessIterator end_prev(end - Diff(1));

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
		RandomAccessIterator p_prev(p - Diff(1));

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
			   LessThanComparer&& lt_cmper = LessThanComparer(),
			   Swapper&& swapper = Swapper()) {
	IntroSort_(end - begin, begin, end, lt_cmper, swapper);
}

#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////

/*
template<typename RandomAccessIterator,
		 typename LessThanComparer = DefaultLessThanComparer,
		 typename Swapper = DefaultSwapper>
void Nth(typename iterator::trait<RandomAccessIterator>::Diff n,
		 RandomAccessIterator begin, RandomAccessIterator end,
		 LessThanComparer&& lt_cmper = LessThanComparer(),
		 Swapper&& swapper = Swapper()) {
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
			Partition(begin + Diff(2), end_prev, *begin, lt_cmper, swapper));
		RandomAccessIterator p_prev(p - Diff(1));

		swapper(*begin, *p_prev);

		Diff a_diff(p_prev - begin);
		Diff b_diff(end - p);

		if (a_diff < n) {
			n -= a_diff + Diff(1);
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
*/

#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////

#define PHI__insertion_sort_threshold (32)
#define PHI__n (2)

template<typename RandomAccessIterator, typename LessThanComparer,
		 typename Swapper>
void Sort_Restricted_SortedBegin_(
	typename iterator::trait<RandomAccessIterator>::Diff depth_limit,
	RandomAccessIterator begin, RandomAccessIterator end,
	LessThanComparer&& lt_cmper, Swapper&& swapper) {
	using Diff = typename iterator::trait<RandomAccessIterator>::Diff;

	Diff diff(end - begin);

	if (diff <= Diff(PHI__insertion_sort_threshold)) {
		InsertionSort(begin, end, lt_cmper);
		return;
	}

	if (depth_limit == Diff(0)) {
		HeapSort(begin, end, lt_cmper);
		return;
	}

	depth_limit /= Diff(2);

	MakeMinAtBegin(end - Diff(PHI__n), end, lt_cmper, swapper);
	LinearInsertWithValue(begin, begin + Diff(PHI__n), *(begin + Diff(PHI__n)),
						  lt_cmper);
	LinearInsertWithValue(begin, begin + Diff(PHI__n + 1),
						  *(end - Diff(PHI__n)), lt_cmper);
	for (size_t i(1); i != PHI__n; ++i) {
		UnrestrictedLinearInsertWithValue(begin + Diff(PHI__n),
										  *(end - Diff(PHI__n - i)), lt_cmper);
	}

	RandomAccessIterator p(
		UnrestrictedPartition(begin + Diff(PHI__n + 1), end - Diff(PHI__n),
							  *(begin + Diff(PHI__n)), lt_cmper, swapper));

	swapper(*(begin + Diff(PHI__n)), *(p - Diff(1)));

	Sort_Restricted_SortedBegin_(depth_limit, begin, p - Diff(1), lt_cmper,
								 swapper);
	Sort_Unrestricted_UnsortedBegin_(depth_limit, p, end, lt_cmper, swapper);
}

template<typename RandomAccessIterator, typename LessThanComparer,
		 typename Swapper>
void Sort_Restricted_UnsortedBegin_(
	typename iterator::trait<RandomAccessIterator>::Diff depth_limit,
	RandomAccessIterator begin, RandomAccessIterator end,
	LessThanComparer&& lt_cmper, Swapper&& swapper) {
	using Diff = typename iterator::trait<RandomAccessIterator>::Diff;

	Diff diff(end - begin);

	if (diff <= Diff(PHI__insertion_sort_threshold)) {
		InsertionSort(begin, end, lt_cmper);
		return;
	}

	if (depth_limit == Diff(0)) {
		HeapSort(begin, end, lt_cmper);
		return;
	}

	depth_limit /= Diff(2);

	MakeMinAtBegin(begin, begin + Diff(PHI__n + 1), lt_cmper, swapper);
	MakeMinAtBegin(end - Diff(PHI__n), end, lt_cmper, swapper);
	SwapIfDisorder(*begin, *(end - Diff(PHI__n)), lt_cmper, swapper);
	UnrestrictedInsertionSort(begin + Diff(1), begin + Diff(PHI__n + 1),
							  lt_cmper);

	for (size_t i(0); i != PHI__n; ++i) {
		UnrestrictedLinearInsertWithValue(begin + Diff(PHI__n),
										  *(end - Diff(PHI__n - i)), lt_cmper);
	}

	RandomAccessIterator p(
		UnrestrictedPartition(begin + Diff(PHI__n + 1), end - Diff(PHI__n),
							  *(begin + Diff(PHI__n)), lt_cmper, swapper));

	swapper(*(begin + Diff(PHI__n)), *(p - Diff(1)));

	Sort_Restricted_SortedBegin_(depth_limit, begin, p - Diff(1), lt_cmper,
								 swapper);
	Sort_Unrestricted_UnsortedBegin_(depth_limit, p, end, lt_cmper, swapper);
}

template<typename RandomAccessIterator, typename LessThanComparer,
		 typename Swapper>
void Sort_Unrestricted_SortedBegin_(
	typename iterator::trait<RandomAccessIterator>::Diff depth_limit,
	RandomAccessIterator begin, RandomAccessIterator end,
	LessThanComparer&& lt_cmper, Swapper&& swapper) {
	using Diff = typename iterator::trait<RandomAccessIterator>::Diff;

	Diff diff(end - begin);

	if (diff <= Diff(PHI__insertion_sort_threshold)) {
		UnrestrictedInsertionSort(begin, end, lt_cmper);
		return;
	}

	if (depth_limit == Diff(0)) {
		HeapSort(begin, end, lt_cmper);
		return;
	}

	depth_limit /= Diff(2);

	MakeMinAtBegin(end - Diff(PHI__n), end, lt_cmper, swapper);
	LinearInsertWithValue(begin, begin + Diff(PHI__n), *(begin + Diff(PHI__n)),
						  lt_cmper);
	LinearInsertWithValue(begin, begin + Diff(PHI__n + 1),
						  *(end - Diff(PHI__n)), lt_cmper);
	for (size_t i(1); i != PHI__n; ++i) {
		UnrestrictedLinearInsertWithValue(begin + Diff(PHI__n),
										  *(end - Diff(PHI__n - i)), lt_cmper);
	}

	RandomAccessIterator p(
		UnrestrictedPartition(begin + Diff(PHI__n + 1), end - Diff(PHI__n),
							  *(begin + Diff(PHI__n)), lt_cmper, swapper));

	swapper(*(begin + Diff(PHI__n)), *(p - Diff(1)));

	Sort_Unrestricted_SortedBegin_(depth_limit, begin, p - Diff(1), lt_cmper,
								   swapper);
	Sort_Unrestricted_UnsortedBegin_(depth_limit, p, end, lt_cmper, swapper);
}

template<typename RandomAccessIterator, typename LessThanComparer,
		 typename Swapper>
void Sort_Unrestricted_UnsortedBegin_(
	typename iterator::trait<RandomAccessIterator>::Diff depth_limit,
	RandomAccessIterator begin, RandomAccessIterator end,
	LessThanComparer&& lt_cmper, Swapper&& swapper) {
	using Diff = typename iterator::trait<RandomAccessIterator>::Diff;

	for (;;) {
		Diff diff(end - begin);

		if (diff <= Diff(PHI__insertion_sort_threshold)) {
			UnrestrictedInsertionSort(begin, end, lt_cmper);
			return;
		}

		if (depth_limit == Diff(0)) {
			HeapSort(begin, end, lt_cmper);
			return;
		}

		depth_limit /= Diff(2);

		MakeMinAtBegin(begin, begin + Diff(PHI__n + 1), lt_cmper, swapper);
		MakeMinAtBegin(end - Diff(PHI__n), end, lt_cmper, swapper);
		SwapIfDisorder(*begin, *(end - Diff(PHI__n)), lt_cmper, swapper);
		UnrestrictedInsertionSort(begin + Diff(1), begin + Diff(PHI__n + 1),
								  lt_cmper);

		for (size_t i(0); i != PHI__n; ++i) {
			UnrestrictedLinearInsertWithValue(
				begin + Diff(PHI__n), *(end - Diff(PHI__n - i)), lt_cmper);
		}

		RandomAccessIterator p(
			UnrestrictedPartition(begin + Diff(PHI__n + 1), end - Diff(PHI__n),
								  *(begin + Diff(PHI__n)), lt_cmper, swapper));

		swapper(*(begin + Diff(PHI__n)), *(p - Diff(1)));

		Sort_Unrestricted_SortedBegin_(depth_limit, begin, p - Diff(1),
									   lt_cmper, swapper);
		begin = p;
	}
}

template<typename RandomAccessIterator,
		 typename LessThanComparer = DefaultLessThanComparer,
		 typename Swapper = DefaultSwapper>
void Sort(RandomAccessIterator begin, RandomAccessIterator end,
		  LessThanComparer&& lt_cmper = LessThanComparer(),
		  Swapper&& swapper = Swapper()) {
	if (!(begin < end)) { return; }
	Sort_Restricted_UnsortedBegin_(end - begin, begin, end, lt_cmper, swapper);
}

#undef PHI__insertion_sort_threshold
#undef PHI__n
}

#endif
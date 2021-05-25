#ifndef PHI__define_guard__Utility__sort_h
#define PHI__define_guard__Utility__sort_h

#include "memory_op.h"
#include "compare.h"
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

template<typename BidirectionalIterator,
		 typename LessThanComparer = DefaultLessThanComparer>
void ConvolutionSwapIfDisorder_(
	iterator::Type::Bidirectional iterator_type, BidirectionalIterator begin,
	BidirectionalIterator end,
	LessThanComparer&& lt_cmper = LessThanComparer()) {
	if (begin == end || begin == --end) { return; }

	do {
		if (lt_cmper.lt(*end, *begin)) { UncheckedSwap(*begin, *end); }
	} while (++begin != end && begin == --end);
}

template<typename RandomAccessIterator,
		 typename LessThanComparer = DefaultLessThanComparer>
void ConvolutionSwapIfDisorder_(
	iterator::Type::RandomAccess iterator_type, RandomAccessIterator begin,
	RandomAccessIterator end,
	LessThanComparer&& lt_cmper = LessThanComparer()) {
	if (!(begin < end)) { return; }

	--end;

	while (begin < end) {
		if (lt_cmper.lt(*end, *begin)) { UncheckedSwap(*begin, *end); }
		++begin;
		--end;
	}
}

template<typename Iterator, typename LessThanComparer = DefaultLessThanComparer>
void ConvolutionSwapIfDisorder(
	Iterator begin, Iterator end,
	LessThanComparer&& lt_cmper = LessThanComparer()) {
	ConvolutionSwapIfDisorder_(typename iterator::trait<Iterator>::Type(),
							   begin, end, lt_cmper);
}

#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////

template<typename BidirectionalIterator,
		 typename LessThanComparer = DefaultLessThanComparer>
void BubblePush(BidirectionalIterator begin, BidirectionalIterator end,
				LessThanComparer&& lt_cmper = LessThanComparer()) {
	using Value = typename iterator::trait<BidirectionalIterator>::Value;

	if (begin == end || begin == --end) { return; }

	BidirectionalIterator end_prev(end);

	do {
		--end_prev;

		if (lt_cmper.lt(*end, *begin)) { UncheckedSwap(*begin, *end); }

		if (lt_cmper.lt(*end, *end_prev)) {
			if (begin == end_prev) { UncheckedSwap(*begin, *end); }

			Value temp(Move(*end));

			do {
				*end = Move(*end_prev);
				end = end_prev;
				--end_prev;
			} while (lt_cmper.lt(temp, *end_prev));

			*end = Move(temp);
		}

		end = end_prev;
	} while (begin != end);
}

template<typename BidirectionalIterator,
		 typename LessThanComparer = DefaultLessThanComparer>
BidirectionalIterator
BubblePushReturnOrderedEnd(BidirectionalIterator begin,
						   BidirectionalIterator end,
						   LessThanComparer&& lt_cmper = LessThanComparer()) {
	using Value = typename iterator::trait<BidirectionalIterator>::Value;

	if (begin == end) { return end; }

	BidirectionalIterator r(end);

	if (begin == --end) { return r; }

	r = end;

	BidirectionalIterator end_prev(end);

	do {
		--end_prev;

		if (lt_cmper.lt(*end, *begin)) {
			UncheckedSwap(*begin, *end);
			r = end;
		}

		if (lt_cmper.lt(*end, *end_prev)) {
			if (begin == end_prev) {
				UncheckedSwap(*begin, *end);
				return ++end;
			}

			Value temp(Move(*end));

			do {
				*end = Move(*end_prev);
				end = end_prev;
				--end_prev;
			} while (lt_cmper.lt(temp, *end_prev));

			*end = Move(temp);

			r = end;
		}

		end = end_prev;
	} while (begin != end);

	return ++r;
}

#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////

template<typename BidirectionalIterator,
		 typename LessThanComparer = DefaultLessThanComparer>
void UnrestrictedLinearPushWithHole(
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
void UnrestrictedLinearPush(
	BidirectionalIterator last,
	typename iterator::trait<BidirectionalIterator>::Ref value,
	LessThanComparer&& lt_cmper = LessThanComparer()) {
	if (!lt_cmper.lt(value, *last)) { return; }
	typename iterator::trait<BidirectionalIterator>::Value temp(Move(*last));
	UnrestrictedLinearPushWithHole(last, value, lt_cmper);
	value = Move(temp);
}

template<typename BidirectionalIterator,
		 typename LessThanComparer = DefaultLessThanComparer>
void UnrestrictedLinearPush(BidirectionalIterator last,
							LessThanComparer&& lt_cmper = LessThanComparer()) {
	BidirectionalIterator last_prev(last);
	--last_prev;

	if (!lt_cmper.lt(*last, *last_prev)) { return; }

	typename iterator::trait<BidirectionalIterator>::Value value(Move(*last));

	*last = Move(*last_prev);

	UnrestrictedLinearPushWithHole(last_prev, value, lt_cmper);
}

#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////

template<typename RandomAccessIterator, typename LessThanComparer>
RandomAccessIterator
UnrestrictedPartition(RandomAccessIterator begin, RandomAccessIterator end,
					  typename iterator::trait<RandomAccessIterator>::Ref pivot,
					  LessThanComparer&& lt_cmper) {
	for (;;) {
		while (lt_cmper.lt(*begin, pivot)) { ++begin; }
		do { --end; } while (lt_cmper.lt(pivot, *end));
		if (!(begin < end)) { return begin; }
		UncheckedSwap(*begin, *end);
		++begin;
	}
}

#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////

template<typename BidirectionalIterator,
		 typename LessThanComparer = DefaultLessThanComparer>
void UnrestrictedInsertionSort(
	BidirectionalIterator begin, BidirectionalIterator end,
	LessThanComparer&& lt_cmper = LessThanComparer()) {
	for (; begin != end; ++begin) { UnrestrictedLinearPush(begin, lt_cmper); }
}

/*
 * Insertion sort [begin, end). Use lt_cmper to compare two elements.
*/

template<typename BidirectionalIterator,
		 typename LessThanComparer = DefaultLessThanComparer>
void InsertionSort(BidirectionalIterator begin, BidirectionalIterator end,
				   LessThanComparer&& lt_cmper = LessThanComparer()) {
	if (begin == end) { return; }
	begin = BubblePushReturnOrderedEnd(begin, end, lt_cmper);
	UnrestrictedInsertionSort(begin, end, lt_cmper);
}

#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////

template<typename ForwardIterator,
		 typename LessThanComparer = DefaultLessThanComparer>
void BubbleSort(ForwardIterator begin, ForwardIterator end,
				LessThanComparer&& lt_cmper = LessThanComparer()) {
	while (begin != end) {
		begin = BubblePushReturnOrderedEnd(begin, end, lt_cmper);
	}
}

#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////

/*
 * Selection sort [begin, end). Use lt_cmper to compare two elements.
*/

template<typename ForwardIterator,
		 typename LessThanComparer = DefaultLessThanComparer>
void SelectionSort(ForwardIterator begin, ForwardIterator end,
				   LessThanComparer&& lt_cmper = LessThanComparer()) {
	for (;;) {
		ForwardIterator begin_next(begin);
		if (++begin_next == end) { break; }
		ForwardIterator min(Min(begin_next, end, lt_cmper));
		if (lt_cmper.lt(*min, *begin)) { UncheckedSwap(*begin, *min); }
		begin = begin_next;
	}
}

#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////

template<typename RandomAccessIterator,
		 typename LessThanComparer = DefaultLessThanComparer>
void QuickSort(RandomAccessIterator begin, RandomAccessIterator end,
			   LessThanComparer&& lt_cmper = LessThanComparer()) {
	using Value = typename iterator::trait<RandomAccessIterator>::Value;
	using Ref = typename iterator::trait<RandomAccessIterator>::Ref;
	using Diff = typename iterator::trait<RandomAccessIterator>::Diff;

	for (;;) {
		Diff diff(end - begin);

		if (diff <= Diff(8)) {
			InsertionSort(begin, end, lt_cmper);
			return;
		}

		Ref mid(*(begin + diff / Diff(2)));

		if (lt_cmper.lt(mid, *begin)) { UncheckedSwap(*begin, mid); }

		if (lt_cmper.lt(*(end - Diff(1)), mid)) {
			Value temp(Move(*(end - Diff(1))));
			*(end - Diff(1)) = Move(mid);

			if (lt_cmper.lt(temp, *begin)) {
				mid = Move(*begin);
				*begin = Move(temp);
			} else {
				mid = Move(temp);
			}
		}

		UncheckedSwap(*(begin + Diff(1)), mid);

		RandomAccessIterator p(UnrestrictedPartition(
			begin + Diff(2), end - Diff(1), *(begin + Diff(1)), lt_cmper));

		Swap(*(begin + Diff(1)), *(p - Diff(1)));

		QuickSort(p, end, lt_cmper);
		end = p - Diff(1);
	}
}

#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////

template<typename RandomAccessIterator,
		 typename LessThanComparer = DefaultLessThanComparer>
void HeapSort(RandomAccessIterator begin, RandomAccessIterator end,
			  LessThanComparer&& lt_cmper = LessThanComparer()) {
	typename iterator::trait<RandomAccessIterator>::Diff diff(end - begin);
	heap::Make(begin, diff, lt_cmper);
	heap::Sort(begin, diff, lt_cmper);
}

#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////

template<typename RandomAccessIterator, typename LessThanComparer>
void IntroSort_(
	typename iterator::trait<RandomAccessIterator>::Diff depth_limit,
	RandomAccessIterator begin, RandomAccessIterator end,
	LessThanComparer&& lt_cmper) {
	using Value = typename iterator::trait<RandomAccessIterator>::Value;
	using Ref = typename iterator::trait<RandomAccessIterator>::Ref;
	using Diff = typename iterator::trait<RandomAccessIterator>::Diff;

	while (depth_limit != Diff(0)) {
		depth_limit /= Diff(2);

		Diff diff(end - begin);

		if (diff <= Diff(8)) { break; }

		Ref mid(*(begin + diff / Diff(2)));

		if (lt_cmper.lt(mid, *begin)) { UncheckedSwap(*begin, mid); }

		if (lt_cmper.lt(*(end - Diff(1)), mid)) {
			Value temp(Move(*(end - Diff(1))));
			*(end - Diff(1)) = Move(mid);

			if (lt_cmper.lt(temp, *begin)) {
				mid = Move(*begin);
				*begin = Move(temp);
			} else {
				mid = Move(temp);
			}
		}

		UncheckedSwap(*(begin + Diff(1)), mid);

		RandomAccessIterator p(UnrestrictedPartition(
			begin + Diff(2), end - Diff(1), *(begin + Diff(1)), lt_cmper));

		Swap(*(begin + Diff(1)), *(p - Diff(1)));

		IntroSort_(depth_limit, p, end, lt_cmper);
		end = p - Diff(1);
	}

	HeapSort(begin, end, lt_cmper);
}

template<typename RandomAccessIterator,
		 typename LessThanComparer = DefaultLessThanComparer>
void IntroSort(RandomAccessIterator begin, RandomAccessIterator end,
			   LessThanComparer&& lt_cmper = LessThanComparer()) {
	IntroSort_(end - begin, begin, end, lt_cmper);
}

#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////

int log2int_(unsigned int num) {
#if (defined(__GNUC__) || defined(__GNUG__)) && true
	return 8 * sizeof(unsigned int) - __builtin_clz(num);
#else
	int r(0);

	{
		int a(bool(num >> 32));
		num >>= 32 * a;
		r += 32 * a;
	}

	{
		int a(bool(num >> 16));
		num >>= 16 * a;
		r += 16 * a;
	}

	{
		int a(bool(num >> 8));
		num >>= 8 * a;
		r += 8 * a;
	}

	{
		int a(bool(num >> 4));
		num >>= 4 * a;
		r += 4 * a;
	}

	{
		int a(bool(num >> 2));
		num >>= 2 * a;
		r += 2 * a;
	}

	{
		int a(bool(num >> 1));
		num >>= 1 * a;
		r += 1 * a;
	}

	return r;
#endif
}

#define PHI__insertion_sort_threshold (32)

template<typename Num> int log2int(Num num) {
	return log2int_((unsigned long long int)(num));
}

template<typename Diff> Diff SortGetN_(Diff diff) {
	Diff n(diff / Diff(1024 * 2));
	return (n < Diff(3)) ? Diff(3) : (n | Diff(1));
}

template<typename Diff> Diff SortGetM_(Diff diff) {
	return SortGetN_(diff) / Diff(2);
}

template<bool begin_is_heap, typename RandomAccessIterator,
		 typename LessThanComparer>
void Sort_(typename iterator::trait<RandomAccessIterator>::Diff depth_limit,
		   RandomAccessIterator begin, RandomAccessIterator end,
		   LessThanComparer&& lt_cmper) {
	using Value = typename iterator::trait<RandomAccessIterator>::Value;
	using Ref = typename iterator::trait<RandomAccessIterator>::Ref;
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

		Diff m(SortGetM_(diff));

		if (m == Diff(1)) {
			Sort_n_is_three_(depth_limit, begin, end, lt_cmper);
			return;
		}

		depth_limit /= Diff(2);

		if constexpr (!begin_is_heap) { heap::Make(begin, m, lt_cmper); }

		Ref mid(*(begin + diff / Diff(2)));

		if (lt_cmper.lt(mid, *begin)) {
			Value temp(Move(*begin));
			begin[heap::PushDownwardWithValue(begin, m, Diff(0), mid,
											  lt_cmper)] = Move(mid);
			mid = Move(temp);
		}

		for (RandomAccessIterator i(end - m); i != end; ++i) {
			if (!lt_cmper.lt(*i, mid)) { continue; }

			if (lt_cmper.lt(*i, *begin)) {
				Value temp(Move(*begin));
				begin[heap::PushDownwardWithValue(begin, m, Diff(0), *i,
												  lt_cmper)] = Move(*i);
				*i = Move(mid);
				mid = Move(temp);
			} else {
				Swap(*i, mid);
			}
		}

		UncheckedSwap(*(begin + m), mid);

		RandomAccessIterator p(UnrestrictedPartition(
			begin + m + Diff(1), end - m, *(begin + m), lt_cmper));

		Swap(*(begin + m), *(p - Diff(1)));

		if constexpr (begin_is_heap) {
			Sort_<false>(depth_limit, p, end, lt_cmper);
			end = p - Diff(1);
		} else {
			Sort_<true>(depth_limit, begin, p - Diff(1), lt_cmper);
			begin = p;
		}
	}
}

template<typename RandomAccessIterator, typename LessThanComparer>
void Sort_n_is_three_(
	typename iterator::trait<RandomAccessIterator>::Diff depth_limit,
	RandomAccessIterator begin, RandomAccessIterator end,
	LessThanComparer&& lt_cmper) {
	using Value = typename iterator::trait<RandomAccessIterator>::Value;
	using Ref = typename iterator::trait<RandomAccessIterator>::Ref;
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

		Ref mid(*(begin + diff / Diff(2)));

		if (lt_cmper.lt(mid, *begin)) { UncheckedSwap(*begin, mid); }

		if (lt_cmper.lt(*(end - Diff(1)), mid)) {
			Value temp(Move(*(end - Diff(1))));
			*(end - Diff(1)) = Move(mid);

			if (lt_cmper.lt(temp, *begin)) {
				mid = Move(*begin);
				*begin = Move(temp);
			} else {
				mid = Move(temp);
			}
		}

		UncheckedSwap(*(begin + Diff(1)), mid);

		RandomAccessIterator p(UnrestrictedPartition(
			begin + Diff(2), end - Diff(1), *(begin + Diff(1)), lt_cmper));

		Swap(*(begin + Diff(1)), *(p - Diff(1)));

		Sort_n_is_three_(depth_limit, p, end, lt_cmper);
		end = p - Diff(1);
	}
}

template<typename RandomAccessIterator,
		 typename LessThanComparer = DefaultLessThanComparer>
void Sort(RandomAccessIterator begin, RandomAccessIterator end,
		  LessThanComparer&& lt_cmper = LessThanComparer()) {
	if (!(begin < end)) { return; }
	ConvolutionSwapIfDisorder(begin, end, lt_cmper);
	begin = BubblePushReturnOrderedEnd(begin, end, lt_cmper);
	if (!(begin < end)) { return; }
	Sort_<false>(end - begin, begin, end, lt_cmper);
}

#undef PHI__insertion_sort_threshold

}

#endif
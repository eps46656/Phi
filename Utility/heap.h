#ifndef PHI__define_guard__Utility__heap_h
#define PHI__define_guard__Utility__heap_h

#include "compare.h"
#include "iterator.h"

namespace phi {
namespace heap {

#define PHI__has_single_child_parent(end_index) (end_index % Diff(2) == Diff(0))

#define PHI__get_p_index(index) ((index - Diff(1)) / Diff(2))

#define PHI__get_l_index(index) (index * Diff(2) + Diff(1))
#define PHI__get_r_index(index) (index * Diff(2) + Diff(2))

#define PHI__has_l(index, end_index) (index < end_index / Diff(2))
#define PHI__has_r(index, end_index)                                           \
	(index < PHI__get_end_p_index_which_has_r(end_index))

#define PHI__get_end_index_which_has_p(end_index)

#define PHI__get_end_p_index_which_has_r(end_index)                            \
	((end_index - Diff(1)) / Diff(2))

#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////

/*
 * [0, end_index-1] is heap with hole
 * hole_index is in [0, end_index-1]
 * push the hole to 0
*/
template<typename RandomAccessIterator>
void PushUpward(
	RandomAccessIterator begin,
	typename iterator::trait<RandomAccessIterator>::Diff hole_index) {
	using Diff = typename iterator::trait<RandomAccessIterator>::Diff;

	PHI__debug_if(hole_index < Diff(0)) { PHI__throw("invalid index"); }

	while (hole_index != Diff(0)) {
		Diff target_index(PHI__get_p_index(hole_index));
		begin[hole_index] = Move(begin[target_index]);
		hole_index = target_index;
	}
}

/*
 * [0, end_index-1] is heap with hole
 * hole_index is in [0, end_index-1]
 * find the position to put value to let [begin, end) be heap
 * return the index of position
*/
template<typename RandomAccessIterator, typename LessThanComparer>
typename iterator::trait<RandomAccessIterator>::Diff PushUpwardWithValue(
	RandomAccessIterator begin,
	typename iterator::trait<RandomAccessIterator>::Diff hole_index,
	typename iterator::trait<RandomAccessIterator>::Ref value,
	LessThanComparer&& lt_cmper = LessThanComparer()) {
	using Diff = typename iterator::trait<RandomAccessIterator>::Diff;

	PHI__debug_if(hole_index < Diff(0)) { PHI__throw("invalid index"); }

	if (hole_index == Diff(0)) { return Diff(0); }

	if (lt_cmper.lt(*begin, value)) {
		PushUpward(begin, hole_index);
		return Diff(0);
	}

	for (;;) {
		Diff target_index(PHI__get_p_index(hole_index));
		if (!lt_cmper.lt(begin[target_index], value)) { return hole_index; }
		begin[hole_index] = Move(begin[target_index]);
		hole_index = target_index;
	}

	return Diff(0);
}

#///////////////////////////////////////////////////////////////////////////////

/*
 * [0, end_index-1] is heap with hole
 * hole_index is in [0, end_index-1]
 * push the hole to 0
*/
template<typename RandomAccessIterator, typename LessThanComparer>
typename iterator::trait<RandomAccessIterator>::Diff
PushDownward(RandomAccessIterator begin,
			 typename iterator::trait<RandomAccessIterator>::Diff end_index,
			 typename iterator::trait<RandomAccessIterator>::Diff hole_index,
			 LessThanComparer&& lt_cmper = LessThanComparer()) {
	using Diff = typename iterator::trait<RandomAccessIterator>::Diff;

	PHI__debug_if(hole_index < Diff(0) || end_index <= hole_index) {
		PHI__throw("invalid index");
	}

	for (Diff upper_index(PHI__get_end_p_index_which_has_r(end_index));
		 hole_index < upper_index;) {
		Diff l_index(PHI__get_l_index(hole_index));
		Diff r_index(PHI__get_r_index(hole_index));
		Diff target_index(l_index +
						  Diff(lt_cmper.lt(begin[l_index], begin[r_index])));
		begin[hole_index] = Move(begin[target_index]);
		hole_index = target_index;
	}

	if (!PHI__has_l(hole_index, end_index)) { return hole_index; }

	Diff target_index(PHI__get_l_index(hole_index));
	begin[hole_index] = Move(begin[target_index]);
	return target_index;
}

/*
 * [0, end_index-1] is heap with hole
 * hole_index is in [0, end_index-1]
 * find a position top down to put value to let [begin, end-1] be heap
 * return the index of position
*/
template<typename RandomAccessIterator, typename LessThanComparer>
typename iterator::trait<RandomAccessIterator>::Diff PushDownwardWithValue(
	RandomAccessIterator begin,
	typename iterator::trait<RandomAccessIterator>::Diff end_index,
	typename iterator::trait<RandomAccessIterator>::Diff hole_index,
	typename iterator::trait<RandomAccessIterator>::Ref value,
	LessThanComparer&& lt_cmper = LessThanComparer()) {
	using Diff = typename iterator::trait<RandomAccessIterator>::Diff;

	PHI__debug_if(hole_index < Diff(0) || end_index <= hole_index) {
		PHI__throw("invalid index");
	}

	if (end_index == Diff(0)) { return hole_index; }

	for (Diff upper_index(PHI__get_end_p_index_which_has_r(end_index));
		 hole_index < upper_index;) {
		Diff l_index(PHI__get_l_index(hole_index));
		Diff r_index(PHI__get_r_index(hole_index));
		Diff target_index(l_index +
						  Diff(lt_cmper.lt(begin[l_index], begin[r_index])));
		if (!lt_cmper.lt(value, begin[target_index])) { return hole_index; }
		begin[hole_index] = Move(begin[target_index]);
		hole_index = target_index;
	}

	if (!PHI__has_l(hole_index, end_index)) { return hole_index; }

	Diff target_index(PHI__get_l_index(hole_index));
	if (!lt_cmper.lt(value, begin[target_index])) { return hole_index; }
	begin[hole_index] = Move(begin[target_index]);
	return target_index;
}

#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////

/*
 * check begin[0, end_index-1] is heap
*/
template<typename RandomAccessIterator,
		 typename LessThanComparer = DefaultLessThanComparer>
bool IsHeap(RandomAccessIterator begin,
			typename iterator::trait<RandomAccessIterator>::Diff end_index,
			LessThanComparer&& lt_cmper = LessThanComparer()) {
	using Diff = typename iterator::trait<RandomAccessIterator>::Diff;

	if (end_index < Diff(2)) { return true; }
	--end_index;

	do {
		Diff p_index(PHI__get_p_index(end_index));
		if (lt_cmper.lt(begin[p_index], begin[end_index])) { return false; }
	} while (--end_index != Diff(0));

	return true;
}

#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////

/*
 * [0, end_index-1] is heap
 * move the value at begin to end_index-1
 * adjust [0, end_index-2] to let it be heap
*/
template<typename RandomAccessIterator,
		 typename LessThanComparer = DefaultLessThanComparer>
void Push(RandomAccessIterator begin,
		  typename iterator::trait<RandomAccessIterator>::Diff end_index,
		  LessThanComparer&& lt_cmper = LessThanComparer()) {
	using Value = typename iterator::trait<RandomAccessIterator>::Value;
	using Diff = typename iterator::trait<RandomAccessIterator>::Diff;

	PHI__debug_if(end_index == Diff(0)) { PHI__throw("end_index == Diff(0)"); }

	if (end_index < Diff(2)) { return; }
	--end_index;

	Diff p_index(PHI__get_p_index(end_index));

	if (!lt_cmper.lt(begin[p_index], begin[end_index])) { return; }

	Value temp(Move(begin[end_index]));
	begin[end_index] = Move(begin[p_index]);
	begin[PushUpwardWithValue(begin, p_index, temp, lt_cmper)] = Move(temp);
}

/*
 * [0, end_index-1] is heap
 * move the value at begin to end_index-1
 * adjust [0, end_index-2] to let it be heap
*/
template<typename RandomAccessIterator,
		 typename LessThanComparer = DefaultLessThanComparer>
void Pop(RandomAccessIterator begin,
		 typename iterator::trait<RandomAccessIterator>::Diff end_index,
		 LessThanComparer&& lt_cmper = LessThanComparer()) {
	using Value = typename iterator::trait<RandomAccessIterator>::Value;
	using Diff = typename iterator::trait<RandomAccessIterator>::Diff;

	if (end_index < Diff(2)) { return; }
	--end_index;

	Value temp(Move(*begin));
	begin[PushUpwardWithValue(
		begin, PushDownward(begin, end_index, Diff(0), lt_cmper),
		begin[end_index], lt_cmper)] = Move(begin[end_index]);
	begin[end_index] = Move(temp);
}

#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////

/*
 * let begin[0, end_index-1] becomes heap
*/
template<typename RandomAccessIterator,
		 typename LessThanComparer = DefaultLessThanComparer>
void Make(RandomAccessIterator begin,
		  typename iterator::trait<RandomAccessIterator>::Diff end_index,
		  LessThanComparer&& lt_cmper = LessThanComparer()) {
	using Value = typename iterator::trait<RandomAccessIterator>::Value;
	using Diff = typename iterator::trait<RandomAccessIterator>::Diff;

	if (end_index < Diff(2)) { return; }

	for (Diff index(1); index != end_index; ++index) {
		Diff p_index(PHI__get_p_index(index));
		if (!lt_cmper.lt(begin[p_index], begin[index])) { continue; }
		Value temp(Move(begin[index]));
		begin[index] = Move(begin[p_index]);
		begin[PushUpwardWithValue(begin, p_index, temp, lt_cmper)] = Move(temp);
	}

	/*Diff index(PHI__get_end_p_index_which_has_r(end_index));

	if (PHI__has_single_child_parent(end_index)) {
		Diff target_index(end_index - Diff(1));

		if (lt_cmper.lt(begin[index], begin[target_index])) {
			Value temp(Move(begin[index]));
			begin[index] = Move(begin[target_index]);
			begin[target_index] = Move(temp);
		}
	}

	while (index != Diff(0)) {
		--index;
		Diff l_index(PHI__get_l_index(index));
		Diff r_index(PHI__get_r_index(index));
		Diff target_index(l_index +
						  Diff(lt_cmper.lt(begin[l_index], begin[r_index])));

		if (lt_cmper.lt(begin[index], begin[target_index])) {
			Value temp(Move(begin[index]));
			begin[index] = Move(begin[target_index]);
			begin[PushDownwardWithValue(begin, end_index, target_index, temp,
										lt_cmper)] = Move(temp);
		}
	}*/
}

#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////

/*
 * begin[0, end_index-1] is heap
 * sort begin[0, end_index-1]
*/
template<typename RandomAccessIterator,
		 typename LessThanComparer = DefaultLessThanComparer>
void Sort(RandomAccessIterator begin,
		  typename iterator::trait<RandomAccessIterator>::Diff end_index,
		  LessThanComparer&& lt_cmper = LessThanComparer()) {
	using Value = typename iterator::trait<RandomAccessIterator>::Value;
	using Diff = typename iterator::trait<RandomAccessIterator>::Diff;
	for (; end_index != Diff(1); --end_index) {
		Pop(begin, end_index, lt_cmper);
	}
}

#undef PHI__has_single_child_parent

#undef PHI__get_p_index

#undef PHI__get_l_index
#undef PHI__get_r_index

#undef PHI__has_l
#undef PHI__has_r

#undef PHI__get_end_p_index_which_has_r

}
}

#endif
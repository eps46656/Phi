#ifndef PHI__define_guard__Utility__heap_h
#define PHI__define_guard__Utility__heap_h

#include "compare.h"
#include "iterator.h"

namespace phi {

template<typename RandomAccessIterator,
		 typename LessThanComparer = DefaultLessThanComparer>
bool IsHeap(RandomAccessIterator begin, RandomAccessIterator end,
			const LessThanComparer& lt_cmper = LessThanComparer()) {
	using Diff = typename iterator::trait<RandomAccessIterator>::Diff;
	Diff diff(end - begin);

	if (diff < Diff(2)) { return true; }

	Diff i(diff / 2 - 1);

	Diff child_index(i * 2 + 1);

	if (i < (diff - 1) / 2 &&
		lt_cmper.lt(begin[child_index], begin[child_index + 1])) {
		++child_index;
	}

	if (lt_cmper.lt(begin[i], begin[child_index])) { return false; }

	while (i != Diff(0)) {
		child_index = --i * 2 + 1;

		if (lt_cmper.lt(begin[child_index], begin[child_index + 1])) {
			++child_index;
		}

		if (lt_cmper.lt(begin[i], begin[child_index])) { return false; }
	}

	return true;
}

template<typename RandomAccessIterator,
		 typename LessThanComparer = DefaultLessThanComparer>
void HeapifyWithHole(
	RandomAccessIterator begin,
	typename iterator::trait<RandomAccessIterator>::Diff hole_index,
	typename iterator::trait<RandomAccessIterator>::Diff end_index,
	typename iterator::trait<RandomAccessIterator>::Ref value,
	const LessThanComparer& lt_cmper = LessThanComparer()) {
	using Diff = typename iterator::trait<RandomAccessIterator>::Diff;

	Diff second_child_index_upper((end_index - Diff(1)) / Diff(2));

	while (hole_index < second_child_index_upper) {
		Diff first_child_index(hole_index * Diff(2) + Diff(1));
		Diff second_child_index(hole_index * Diff(2) + Diff(2));

		if (lt_cmper.lt(begin[first_child_index], begin[second_child_index])) {
			begin[hole_index] = Move(begin[second_child_index]);
			hole_index = second_child_index;
		} else {
			begin[hole_index] = Move(begin[first_child_index]);
			hole_index = first_child_index;
		}
	}

	if ((end_index % Diff(2) == Diff(0)) &&
		(hole_index == end_index / Diff(2) - Diff(1))) {
		Diff first_child_index(hole_index * Diff(2) + Diff(1));
		begin[hole_index] = Move(begin[first_child_index]);
		hole_index = first_child_index;
	}

	while (hole_index != Diff(0)) {
		Diff parent_index((hole_index - Diff(1)) / Diff(2));

		if (!lt_cmper.lt(begin[parent_index], value)) { break; }

		begin[hole_index] = Move(begin[parent_index]);
		hole_index = parent_index;
	}

	begin[hole_index] = Move(value);
}

template<typename RandomAccessIterator,
		 typename LessThanComparer = DefaultLessThanComparer>
void MakeHeap(RandomAccessIterator begin, RandomAccessIterator end,
			  const LessThanComparer& lt_cmper = LessThanComparer()) {
	using Value = typename iterator::trait<RandomAccessIterator>::Value;
	using Diff = typename iterator::trait<RandomAccessIterator>::Diff;

	Diff diff(end - begin);

	if (diff < Diff(2)) { return; }

	if (diff % Diff(2) == Diff(0)) {
		Diff parent_index(diff / Diff(2) - Diff(1));
		Diff child_index(diff - Diff(1));

		if (lt_cmper.lt(begin[parent_index], begin[child_index])) {
			Value value(Move(begin[parent_index]));
			begin[parent_index] = Move(begin[child_index]);

			HeapifyWithHole(begin, child_index, diff, value, lt_cmper);
		}

		if (parent_index == Diff(0)) { return; }
	}

	for (Diff parent_index((diff - Diff(3)) / Diff(2));; --parent_index) {
		Diff child_index(parent_index * Diff(2) + Diff(1));

		if (lt_cmper.lt(begin[child_index], begin[child_index + Diff(1)])) {
			++child_index;
		}

		if (lt_cmper.lt(begin[parent_index], begin[child_index])) {
			Value value(Move(begin[parent_index]));
			begin[parent_index] = Move(begin[child_index]);

			HeapifyWithHole(begin, child_index, diff, value, lt_cmper);
		}

		if (parent_index == Diff(0)) { return; }
	}
}

}

#endif
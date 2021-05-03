#ifndef PHI__define_guard__Utility__sudoku_h
#define PHI__define_guard__Utility__sudoku_h

#include "memory_op.h"
#include "dancing_link.h"
#include "index_arranger.h"
#include "combination.h"

namespace phi {

template<size_t rank> class Sudoku {
public:
	static constexpr size_t size = rank * rank;
	static constexpr size_t size_sq = size * size;

	struct Block {
		size_t value;
		bool candidate[size];

		bool is_certain() const;
		size_t candidate_num() const;

		void set_value(size_t value);
		bool set_candidate_false(size_t i);

		Block();
	};

	struct Receiver {
		cntr::Vector<size_t> solution;

		template<typename ForwardIterator>
		bool operator()(ForwardIterator begin, ForwardIterator end);
	};

#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////

	static size_t GetIndex(size_t col_index, size_t row_index);
	static pair<size_t, size_t> GetColRowIndex(size_t index);
	static size_t GetBoxIndex(size_t index);
	static size_t GetBoxIndex(size_t col_index, size_t row_index);

	static const size_t* GetSameBoxIndex(size_t b_col_index,
										 size_t b_row_index);
	static const size_t* GetSameBoxIndex(size_t index);

	static const size_t* GetSameColIndex(size_t col_index);
	static const size_t* GetSameRowIndex(size_t col_index);

	static char get_char(size_t value);

	static size_t Eliminate(cntr::Vector<size_t>& queue, Block* block,
							size_t index);
	static size_t Eliminate(Block* blocks);

	static void PointSSS(size_t& certain_num, cntr::Vector<size_t>& queue,
						 Block* block);
	static void LineSSS(cntr::Vector<size_t>& queue, Block* block);
	static void MonoSSS(cntr::Vector<size_t>& queue, Block* block);
	static void MonoSSS(cntr::Vector<size_t>& queue, Block* block,
						const size_t* index);

	static void EnSet(cntr::Set<size_t>& set, const bool* candidate);

#///////////////////////////////////////////////////////////////////////////////

	size_t certain_num() const;

	void set_value(size_t col_index, size_t row_index, size_t value);

#///////////////////////////////////////////////////////////////////////////////

	Sudoku();

	void Print() const;

	void Solve();

private:
	size_t certain_num_;
	Block blocks_[size * size];
};

#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////

template<size_t rank>
template<typename ForwardIterator>
bool Sudoku<rank>::Receiver::operator()(ForwardIterator begin,
										ForwardIterator end) {
	for (; begin != end; ++begin) { this->solution.Push(*begin); }
	return false;
}

#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////

/*

value: 0 ~ size - 1    size             size + k
mean : certain         two candidate    k+2 cnadidate

*/

template<size_t rank> bool Sudoku<rank>::Block::is_certain() const {
	return this->value < size;
}

template<size_t rank> size_t Sudoku<rank>::Block::candidate_num() const {
	return this->value < size ? 0 : this->value - size + 2;
}

template<size_t rank> void Sudoku<rank>::Block::set_value(size_t value) {
	this->value = value;
	for (size_t i(0); i != size; ++i) { this->candidate[i] = i == value; }
}

template<size_t rank> bool Sudoku<rank>::Block::set_candidate_false(size_t i) {
	if (!this->candidate[i]) { return false; }
	this->candidate[i] = false;

	if (this->value == size) {
		this->value = 0;
		while (!this->candidate[this->value]) { ++this->value; }
		return true;
	}

	--this->value;
	return false;
}

template<size_t rank> Sudoku<rank>::Block::Block(): value(size - 2 + size) {
	Assign<size>(this->candidate, true);
}

#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////

template<size_t rank>
size_t Sudoku<rank>::GetIndex(size_t col_index, size_t row_index) {
	return size * col_index + row_index;
}

template<size_t rank>
pair<size_t, size_t> Sudoku<rank>::GetColRowIndex(size_t index) {
	size_t col_index(index / size);
	return pair<size_t, size_t>(col_index, index - size * col_index);
}

template<size_t rank> size_t Sudoku<rank>::GetBoxIndex(size_t index) {
	size_t col_index(index / size);
	return col_index / rank * rank + (index - size * col_index) / rank;
}

template<size_t rank>
size_t Sudoku<rank>::GetBoxIndex(size_t col_index, size_t row_index) {
	return col_index / rank * rank + row_index / rank;
}

template<size_t rank>
const size_t* Sudoku<rank>::GetSameBoxIndex(size_t b_index) {
	size_t b_col_index(b_index / rank);
	return GetSameBoxIndex(b_col_index, b_index - rank * b_col_index);
}

template<size_t rank>
const size_t* Sudoku<rank>::GetSameBoxIndex(size_t b_col_index,
											size_t b_row_index) {
	static bool initialized(false);
	static size_t value[rank][rank][size];

	if (!initialized) {
		for (size_t i(0); i != rank; ++i) {
			for (size_t j(0); j != rank; ++j) {
				size_t base(size * rank * i + rank * j);

				for (size_t k(0); k != rank; ++k) {
					for (size_t l(0); l != rank; ++l) {
						value[i][j][rank * k + l] =
							size * (i * rank + l) + (j * rank + k);
					}
				}
			}
		}

		initialized = true;
	}

	return value[b_col_index][b_row_index];
}

template<size_t rank>
const size_t* Sudoku<rank>::GetSameColIndex(size_t col_index) {
	static bool initialized(false);
	static size_t value[size][size];

	if (!initialized) {
		for (size_t i(0); i != size; ++i) {
			for (size_t j(0); j != size; ++j) { value[i][j] = GetIndex(i, j); }
		}

		initialized = true;
	}

	return value[col_index];
}

template<size_t rank>
const size_t* Sudoku<rank>::GetSameRowIndex(size_t row_index) {
	static bool initialized(false);
	static size_t value[size][size];

	if (!initialized) {
		for (size_t i(0); i != size; ++i) {
			for (size_t j(0); j != size; ++j) { value[j][i] = GetIndex(i, j); }
		}

		initialized = true;
	}

	return value[row_index];
}

template<size_t rank> char Sudoku<rank>::get_char(size_t value) {
	if (value < 10) { return '0' + value; }
	if (value < 10 + 26) { return 'A' + value; }
	return ' ';
}

#///////////////////////////////////////////////////////////////////////////////

template<size_t rank>
void Sudoku<rank>::PointSSS(size_t& certain_num, cntr::Vector<size_t>& queue,
							Block* blocks) {
	while (!queue.empty()) {
		size_t index(queue.back());
		queue.Pop();
		++certain_num;

		size_t col_index(index / size);
		size_t row_index(index - col_index * size);

		size_t value(blocks[index].value);

		// same col
		for (size_t row_index_a(0); row_index_a != size; ++row_index_a) {
			if (row_index_a == row_index) { continue; }
			size_t index_a(GetIndex(col_index, row_index_a));
			if (blocks[index_a].set_candidate_false(value)) {
				queue.Push(index_a);
			}
		}

		// same row
		for (size_t col_index_a(0); col_index_a != size; ++col_index_a) {
			if (col_index_a == col_index) { continue; }
			size_t index_a(size * col_index_a + row_index);
			if (blocks[index_a].set_candidate_false(value)) {
				queue.Push(index_a);
			}
		}

		const size_t* same_box_index(
			GetSameBoxIndex(col_index / rank, row_index / rank));

		// same box
		for (size_t i(0); i != size; ++i) {
			size_t index_a(same_box_index[i]);
			if (index_a == index) { continue; }
			if (blocks[index_a].set_candidate_false(value)) {
				queue.Push(index_a);
			}
		}

		if (queue.empty()) { certain_num += LineSSS(queue, blocks); }
	}
}

template<size_t rank>
void Sudoku<rank>::LineSSS(cntr::Vector<size_t>& queue, Block* blocks) {
	for (size_t b_index(0); b_index != size; ++b_index) {
		size_t b_col_index(b_index / rank);
		size_t b_row_index(b_index - rank * b_col_index);

		// PHI__print_value(b_index);
		// PHI__interrupt;

		const size_t* same_box_index(GetSameBoxIndex(b_col_index, b_row_index));

		bool set;
		size_t col_index;
		size_t row_index;

		for (size_t num(0); num != size; ++num) {
			set = false;

			for (size_t i(0); i != size; ++i) {
				if (blocks[same_box_index[i]].value == num) { goto next_num; }
				if (!blocks[same_box_index[i]].candidate[num]) { continue; }

				pair<size_t, size_t> col_row(GetColRowIndex(same_box_index[i]));

				if (set) {
					if (col_index != col_row.first) { col_index = size; }
					if (row_index != col_row.second) { row_index = size; }

					if (col_index == size && row_index == size) {
						goto next_num;
					}
				} else {
					col_index = col_row.first;
					row_index = col_row.second;
					set = true;
				}
			}

			const size_t* index_a(col_index == size
									  ? GetSameRowIndex(row_index)
									  : GetSameColIndex(col_index));

			for (size_t i(0); i != size; ++i) {
				if (GetBoxIndex(index_a[i]) == b_index) { continue; }

				if (blocks[index_a[i]].set_candidate_false(num)) {
					queue.Push(index_a[i]);
				}
			}

		next_num:;
		}
	}
}

template<size_t rank>
void Sudoku<rank>::MonoSSS(cntr::Vector<size_t>& queue, Block* blocks) {
	// same col
	for (size_t col_index(0); col_index != size; ++col_index) {
		MonoSSS(queue, blocks, GetSameColIndex(col_index));
		if (certain_num == size_sq) { return; }
	}

	for (size_t row_index(0); row_index != size; ++row_index) {
		MonoSSS(queue, blocks, GetSameRowIndex(row_index));
		if (certain_num == size_sq) { return; }
	}

	for (size_t box_index(0); box_index != size; ++box_index) {
		MonoSSS(queue, blocks, GetSameBoxIndex(box_index));
		if (certain_num == size_sq) { return; }
	}
}

template<size_t rank>
void Sudoku<rank>::MonoSSS(cntr::Vector<size_t>& queue, Block* blocks,
						   const size_t* indexs) {
	size_t candidate_num[size] = { 0 };
	size_t uncertain_num(0);

	for (size_t i(0); i != size; ++i) {
		if ((candidate_num[i] = indexs[i].candidate_num()) != 1) {
			++uncertain_num;
		}
	}

	if (uncertain_num == 0) { return; }

	if (uncertain_num == 1) {
		PHI__print_value("error");
		PHI__interrupt;
	}

	for (size_t num(2); num != uncertain_num; ++num) {
		size_t indexs_a[size];
		size_t indexs_a_size(0);

		for (size_t i(0); i != size; ++i) {
			if (1 < candidate_num[i] && candidate_num[i] <= num) {
				indexs_a[indexs_a_size] = i;
				++indexs_a_size;
			}
		}

		if (indexs_a_size < num) { continue; }

		cntr::Set<size_t> candidate_set;

		for (IntegerCombination c(indexs_a_size, num); !c.is_end();
			 candidate_set.Clear()) {
			for (size_t i(0); i != indexs_a_size; ++i) {
				EnSet(candidate_set, blocks[indexs[indexs_a[c[i]]]].cnadidate);

				if (num < candidate_set.size()) {
					c.next(i);
					goto next_combination;
				}
			}

			size_t i(0);
			size_t j(0);

			for (; i != size; ++i) {
				if (j != indexs_a_size && i == indexs_a[c[j]]) {
					++j;
					continue;
				}

				{
					auto candidate_set_iter(candidate_set.first_iterator());
					auto candidate_set_end(candidate_set.null_iterator());

					for (; candidate_set_iter != candidate_set_end;
						 ++candidate_set_iter) {
						if (blocks[indexs[indexs_a[c[i]]]].set_candidate_false(
								*candidate_set_iter)) {
							queue.Push(indexs[indexs_a[c[i]]]);
						}
					}
				}
			}

			c.next();

		next_combination:;
		}
	}
}

template<size_t rank>
void Sudoku<rank>::EnSet(cntr::Set<size_t>& set, const bool* candidate) {
	for (size_t i(0); i != size; ++i) {
		if (candidate[i]) { set.Insert(i); }
	}
}

/*
template<size_t rank>
size_t Sudoku<rank>::Eliminate(cntr::Vector<size_t>& quene, Block* blocks,
							   size_t index) {
	size_t certain_num(0);

	for (;;) {
		size_t col_index(index / size);
		size_t row_index(index - col_index * size);

		size_t value(blocks[index].value);

		// same col
		for (size_t row_index_a(0); row_index_a != size; ++row_index_a) {
			if (row_index_a == row_index) { continue; }
			size_t index_a(GetIndex(col_index, row_index_a));
			if (blocks[index_a].set_candidate_false(value)) {
				judge_queue[judge_queue_size] = index_a;
				++judge_queue_size;
				++certain_num;
			}
		}

		// same row
		for (size_t col_index_a(0); col_index_a != size; ++col_index_a) {
			if (col_index_a == col_index) { continue; }
			size_t index_a(size * col_index_a + row_index);
			if (blocks[index_a].set_candidate_false(value)) {
				judge_queue[judge_queue_size] = index_a;
				++judge_queue_size;
				++certain_num;
			}
		}

		const size_t* same_box_index(
			GetSameBoxIndex(col_index / rank, row_index / rank));

		// same box
		for (size_t i(0); i != size; ++i) {
			size_t index_a(same_box_index[i]);
			if (index_a == index) { continue; }
			if (blocks[index_a].set_candidate_false(value)) {
				judge_queue[judge_queue_size] = index_a;
				++judge_queue_size;
				++certain_num;
			}
		}

		if (judge_queue_size == 0) { break; }
		index = judge_queue[--judge_queue_size];
	}

	return certain_num;
}*/

template<size_t rank> size_t Sudoku<rank>::Eliminate(Block* blocks) {
	size_t certain_num(0);

	for (size_t i(0); i != size * size; ++i) {
		if (blocks[i].is_certain()) { certain_num += Eliminate(blocks, i); }
	}

	return certain_num;
}

#///////////////////////////////////////////////////////////////////////////////

template<size_t rank> size_t Sudoku<rank>::certain_num() const {
	return this->certain_num_;
}

template<size_t rank>
void Sudoku<rank>::set_value(size_t col_index, size_t row_index, size_t value) {
	size_t index(GetIndex(col_index, row_index));
	if (this->blocks_[index].is_certain()) { return; }
	this->blocks_[index].set_value(value);

	cntr::Vector<size_t> queue;
	queue.Reserve(size_sq);
	queue.Push(index);

	this->certain_num_ += CertainSSS(queue, this->blocks_);
}

#///////////////////////////////////////////////////////////////////////////////

template<size_t rank> Sudoku<rank>::Sudoku(): certain_num_(0) {}

#///////////////////////////////////////////////////////////////////////////////

template<size_t rank> void Sudoku<rank>::Print() const {
	for (size_t i(0); i != size; ++i) {
		for (size_t j(0); j != size; ++j) {
			if (this->blocks_[size * i + j].is_certain()) {
				std::cout << get_char(this->blocks_[size * i + j].value + 1)
						  << ' ';
			} else {
				std::cout << "  ";
			}
		}

		std::cout << '\n';
	}
}

template<size_t rank> void Sudoku<rank>::Solve() {
	if (this->certain_num_ == size_sq) { return; }

	// state index:
	//     size * index * num : num at index
	// cst index:
	//                       size * index            : index used
	//         [0              , size * size)
	//     size * size     + size * col + num        : num in col
	//         [size * size    , size * size * 2)
	//     size * size * 2 + size * row + num        : num in row
	//         [size * size * 2, size * size * 3)
	//     size * size * 3 + size * box + num        : num in box
	//         [size * size * 3, size * size * 4)

	cntr::Vector<pair<size_t, size_t>> hot;
	IndexArranger state_mapper;
	IndexArranger cst_mapper;

	for (size_t index(0); index != size * size; ++index) {
		if (this->blocks_[index].is_certain()) { continue; }

		for (size_t num(0); num != size; ++num) {
			if (!this->blocks_[index].candidate[num]) { continue; }

			size_t state_id(size * index + num);

			size_t col_index(index / size);
			size_t row_index(index - size * col_index);

			size_t used_cst(size * index);
			size_t col_cst(size * size + size * col_index + num);
			size_t row_cst(size * size * 2 + size * row_index + num);
			size_t box_cst(size * size * 3 +
						   size * GetBoxIndex(col_index, row_index) + num);

			hot.Push(state_id, used_cst);
			hot.Push(state_id, col_cst);
			hot.Push(state_id, row_cst);
			hot.Push(state_id, box_cst);

			state_mapper.Add(state_id);
			cst_mapper.Add(used_cst);
			cst_mapper.Add(col_cst);
			cst_mapper.Add(row_cst);
			cst_mapper.Add(box_cst);
		}
	}

	DancingLink dlx(state_mapper.size(), cst_mapper.size(), 0);

	for (size_t i(0); i != hot.size(); ++i) {
		dlx.AddHot(state_mapper.forward(hot[i].first),
				   cst_mapper.forward(hot[i].second));
	}

	Receiver receiver;
	dlx.Solve(receiver);

	auto iter(receiver.solution.first_iterator());
	auto end(receiver.solution.null_iterator());

	for (; iter != end; ++iter) {
		size_t state_id(state_mapper.backward(*iter));

		size_t index(state_id / size);
		size_t num(state_id - index * size);

		this->blocks_[index].set_value(num);
	}

	this->certain_num_ = size_sq;
}
}

#endif
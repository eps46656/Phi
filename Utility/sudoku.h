#ifndef PHI__define_guard__Utility__sudoku_h
#define PHI__define_guard__Utility__sudoku_h

#include "memory_op.h"
#include "exact_cover.h"

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

	static size_t GetIndex(size_t col_index, size_t row_index);
	static pair<size_t, size_t> GetColRow(size_t index);
	static pair<size_t, size_t> GetRegion(size_t index);
	static pair<size_t, size_t> GetRegion(size_t col_index, size_t row_index);

	static const size_t* GetSameRegionIndex(size_t r_col_index,
											size_t r_row_index);

	static size_t Eliminate(Block* block, size_t index);
	static size_t Eliminate(Block* blocks);

	static size_t OnlyOne(Block* blocks);

	static char get_char(size_t value);

#///////////////////////////////////////////////////////////////////////////////

	void set_value(size_t col_index, size_t row_index, size_t value);

	Sudoku();

	void Print() const;

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
	if (!this->candidate[i]) { return this->is_certain(); }
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
pair<size_t, size_t> Sudoku<rank>::GetColRow(size_t index) {
	size_t col_index(index / size);
	return pair<size_t, size_t>(col_index, index - size * col_index);
}

template<size_t rank>
pair<size_t, size_t> Sudoku<rank>::GetRegion(size_t index) {
	size_t col_index(index / size);
	return pair<size_t, size_t>(col_index / rank,
								(index - size * col_index) / rank);
}

template<size_t rank>
pair<size_t, size_t> Sudoku<rank>::GetRegion(size_t col_index,
											 size_t row_index) {
	return pair<size_t, size_t>(col_index / rank, row_index / rank);
}

template<size_t rank>
const size_t* Sudoku<rank>::GetSameRegionIndex(size_t r_col_index,
											   size_t r_row_index) {
	static bool initialized(false);
	static size_t value[rank][rank][size];

	if (!initialized) {
		for (size_t r_col_index(0); r_col_index != rank; ++r_col_index) {
			for (size_t r_row_index(0); r_row_index != rank; ++r_row_index) {
				size_t b_col_index(rank * r_col_index);
				size_t b_row_index(rank * r_row_index);

				for (size_t i(0); i != rank; ++i) {
					for (size_t j(0); j != rank; ++j) {
						value[r_col_index][r_row_index][rank * i + j] =
							size * (b_col_index + i) + b_row_index + j;
					}
				}
			}
		}

		initialized = true;
	}

	return value[r_col_index][r_row_index];
}

#///////////////////////////////////////////////////////////////////////////////

template<size_t rank>
size_t Sudoku<rank>::Eliminate(Block* blocks, size_t index) {
	size_t judge_queue[size];
	size_t judge_queue_size(0);
	size_t certain_size(1);

	for (;;) {
		size_t col_index(index / size);
		size_t row_index(index - col_index * size);
		size_t r_col_index(col_index / rank);
		size_t r_row_index(row_index / rank);
		size_t b_col_index(r_col_index * rank);
		size_t b_row_index(r_row_index * rank);

		size_t value(blocks[index].value);

		// same col
		for (size_t row_index_a(0); row_index_a != size; ++row_index_a) {
			if (row_index_a == row_index) { continue; }
			size_t index_a(size * col_index + row_index_a);
			if (blocks[index_a]->set_candidate_false(value)) {
				judge_queue[judge_queue_size] = index_a;
				++judge_queue_size;
				++certain_size;
			}
		}

		// same row
		for (size_t col_index_a(0); col_index_a != size; ++col_index_a) {
			if (col_index_a == col_index) { continue; }
			size_t index_a(size * col_index_a + row_index);
			if (blocks[index_a]->set_candidate_false(value)) {
				judge_queue[judge_queue_size] = index_a;
				++judge_queue_size;
				++certain_size;
			}
		}

		const size_t* same_region_index(
			GetSameRegionIndex(r_col_index, r_row_index));

		// same region
		for (size_t i(0); i != size; ++i) {
			size_t index_a(same_region_index[i]);
			if (index_a == index) { continue; }
			if (blocks[index_a]->set_candidate_false(value)) {
				judge_queue[judge_queue_size] = index_a;
				++judge_queue_size;
				++certain_size;
			}
		}

		if (judge_queue_size == 0) { break; }
		index = judge_queue[--judge_queue_size];
	}

	return certain_size;
}

template<size_t rank> size_t Sudoku<rank>::Eliminate(Block* blocks) {
	size_t judge_queue[size];
	size_t judge_queue_size(0);
	size_t certain_size(0);

	for (size_t i(0); i != size * size; ++i) {
		Block* block(blocks[i]);
		if (block->is_certain()) {
			judge_queue[judge_queue_size] = i;
			++judge_queue_size;
			++certain_size;
		}
	}

	for (size_t i(0); i != judge_queue_size; ++i) {
		certain_size += Eliminate(blocks, judge_queue[i]);
	}

	return certain_size;
}

#///////////////////////////////////////////////////////////////////////////////

template<size_t rank> size_t Sudoku<rank>::OnlyOne(Block* blocks) {
	size_t certain_size(0);

	// same col
	for (size_t col_index(0); col_index != size; ++col_index) {
		for (size_t num(0); num != size; ++num) {
			size_t a(size * size);

			for (size_t row_index(0); row_index != size; ++i) {
				size_t index(GetIndex(col_index, row_index));

				if (blocks[index].value == num) { goto next_num; }

				if (blocks[index].is_candidate(num)) {
					if (a != size * size) { goto next_num; }
					a = index;
				}
			}

			blocks[a].set_value(num);
			certain_size += 1 + Eliminate(blocks, a);

		next_num:;
		}
	}

	// same row
	for (size_t row_index(0); row_index != size; ++row_index) {
		for (size_t num(0); num != size; ++num) {
			size_t a(size * size);

			for (size_t col_index(0); col_index != size; ++i) {
				size_t index(GetIndex(col_index, col_index));

				if (blocks[index].value == num) { goto next_num; }

				if (blocks[index].is_candidate(num)) {
					if (a != size * size) { goto next_num; }
					a = index;
				}
			}

			blocks[a].set_value(num);
			certain_size += 1 + Eliminate(blocks, a);

		next_num:;
		}
	}

	// same region
	for (size_t r_col_index(0); r_col_index != rank; ++r_col_index) {
		for (size_t r_row_index(0); r_row_index != rank; ++r_row_index) {
			const size_t* r_index(GetSameRegionIndex(r_col_index, r_row_index));

			for (size_t num(0); num != size; ++num) {
				size_t a(size * size);

				for (size_t i(0); i != size; ++i) {
					size_t index(r_index[i]);

					if (blocks[index].value == num) { goto next_num; }

					if (blocks[index].is_candidate(num)) {
						if (a != size * size) { goto next_num; }
						a = index;
					}
				}

				blocks[a].set_value(num);
				certain_size += 1 + Eliminate(blocks, a);

			next_num:;
			}
		}
	}

	return certain_size;
}

#///////////////////////////////////////////////////////////////////////////////

template<size_t rank> char Sudoku<rank>::get_char(size_t value) {
	if (value < 10) { return '0' + value; }
	if (value < 10 + 26) { return 'A' + value; }
	return '*';
}

template<size_t rank>
void Sudoku<rank>::set_value(size_t col_index, size_t row_index, size_t value) {
	size_t index(GetIndex(col_index, row_index));
	this->blocks_[index].set_value(value);
	Eliminate(this->blocks_, index);
}

template<size_t rank> Sudoku<rank>::Sudoku() {}

template<size_t rank> void Sudoku<rank>::Print() const {
	for (size_t i(0); i != size; ++i) {
		for (size_t j(0); j != size; ++j) {
			Block* block(this->blocks_[size * i + j]);

			if (block->is_certain()) {
				std::cout << get_char(block->value);
			} else {
				std::cout << 'x';
			}

			std::cout << ' ';
		}

		std::cout << '\n';
	}
}
}

#endif
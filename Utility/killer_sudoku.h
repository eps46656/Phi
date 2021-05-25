#ifndef PHI__define_guard__Utility__killer_sudoku_h
#define PHI__define_guard__Utility__killer_sudoku_h

#include "memory_op.h"
#include "dancing_link.h"
#include "index_arranger.h"

namespace phi {

void FindPermutation(cntr::Vector<cntr::Vector<size_t>>& dst, size_t sum,
					 cntr::Vector<cntr::Vector<size_t>> candidate) {
	cntr::Vector<size_t> i;
	i.Reserve(candidate.size());
	while (i.size() != candidate.size()) { i.Push(0); }

	for (;;) {
		size_t j(0);
		size_t sum_(0);

		PHI__print_value(i[0]);
		PHI__print_value(i[1]);
		PHI__print_value(i[2]);

		while ((j != i.size()) && ((sum_ += candidate[j][i[j]]) <= sum)) {
			++j;
		}

		PHI__print_value(sum_);

		if (j == 0) { break; }

		if (j == i.size()) {
			if (sum_ == sum) {
				dst.Push();
				dst.back().Reserve(i.size());

				for (j = 0; j != i.size(); ++j) {
					dst.back().Push(candidate[j][i[j]]);
				}
			}
		}

		for (;;) {
			--j;

			if (i[j] != candidate[i[j]].size()) {
				++i[j];
				break;
			}

			if (j == 0) { return; }
		}

		for (++j; j != i.size(); ++j) { i[j] = 0; }

		PHI__interrupt;
	}
}

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

	static size_t GetBoxIndex(size_t col_index, size_t row_index);

	static const size_t* GetSameColIndexs(size_t col_index);
	static const size_t* GetSameRowIndexs(size_t col_index);

	static const size_t* GetSameBoxIndexs(size_t box_col_index,
										  size_t box_row_index);

	static void Eliminate(size_t& certain_num, cntr::Vector<size_t>& queue,
						  Block* blocks);
	static void Eliminate(cntr::Vector<size_t>& queue, Block* blocks,
						  const size_t* indexs, size_t except_index,
						  size_t num);

	static void Check(const Block* blocks, const size_t* indexs);

#///////////////////////////////////////////////////////////////////////////////

	size_t certain_num() const;

	const Block* blocks() const;

	void set_value(size_t col_index, size_t row_index, size_t value);

#///////////////////////////////////////////////////////////////////////////////

	Sudoku();

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
size_t Sudoku<rank>::GetBoxIndex(size_t col_index, size_t row_index) {
	return col_index / rank * rank + row_index / rank;
}

template<size_t rank>
const size_t* Sudoku<rank>::GetSameColIndexs(size_t col_index) {
	static bool initialized(false);
	static size_t value[size][size];

	if (!initialized) {
		for (size_t i(0); i != size; ++i) {
			for (size_t j(0); j != size; ++j) { value[i][j] = size * i + j; }
		}

		initialized = true;
	}

	return value[col_index];
}

template<size_t rank>
const size_t* Sudoku<rank>::GetSameRowIndexs(size_t row_index) {
	static bool initialized(false);
	static size_t value[size][size];

	if (!initialized) {
		for (size_t i(0); i != size; ++i) {
			for (size_t j(0); j != size; ++j) { value[j][i] = size * i + j; }
		}

		initialized = true;
	}

	return value[row_index];
}

template<size_t rank>
const size_t* Sudoku<rank>::GetSameBoxIndexs(size_t box_col_index,
											 size_t box_row_index) {
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

	return value[box_col_index][box_row_index];
}

#///////////////////////////////////////////////////////////////////////////////

template<size_t rank>
void Sudoku<rank>::Eliminate(size_t& certain_num, cntr::Vector<size_t>& queue,
							 Block* blocks) {
	while (!queue.empty()) {
		size_t index(queue.back());
		queue.Pop();
		++certain_num;

		size_t col_index(index / size);
		size_t row_index(index - size * col_index);

		size_t box_col_index(col_index / rank);
		size_t box_row_index(row_index / rank);

		size_t num(blocks[index].value);

		Eliminate(queue, blocks, GetSameColIndexs(col_index), index, num);
		Eliminate(queue, blocks, GetSameRowIndexs(row_index), index, num);
		Eliminate(queue, blocks, GetSameBoxIndexs(box_col_index, box_row_index),
				  index, num);
	}
}

template<size_t rank>
void Sudoku<rank>::Eliminate(cntr::Vector<size_t>& queue, Block* blocks,
							 const size_t* indexs, size_t except_index,
							 size_t num) {
	for (size_t i(0); i != size; ++i) {
		if (indexs[i] != except_index &&
			blocks[indexs[i]].set_candidate_false(num)) {
			queue.Push(indexs[i]);
		}
	}
}

#///////////////////////////////////////////////////////////////////////////////

template<size_t rank> size_t Sudoku<rank>::certain_num() const {
	return this->certain_num_;
}

template<size_t rank>
const typename Sudoku<rank>::Block* Sudoku<rank>::blocks() const {
	return this->blocks_;
}

template<size_t rank>
void Sudoku<rank>::set_value(size_t col_index, size_t row_index, size_t value) {
	size_t index(size * col_index + row_index);
	if (this->blocks_[index].is_certain()) { return; }
	this->blocks_[index].set_value(value);

	cntr::Vector<size_t> queue;
	queue.Reserve(size_sq);
	queue.Push(index);

	Eliminate(this->certain_num_, queue, this->blocks_);
}

#///////////////////////////////////////////////////////////////////////////////

template<size_t rank> Sudoku<rank>::Sudoku(): certain_num_(0) {}

#///////////////////////////////////////////////////////////////////////////////

/*
void PrintState(size_t state_id) {
	size_t size(9);

	size_t index(state_id / size);
	size_t num(state_id - size * index);

	size_t col_index(index / size);
	size_t row_index(index - size * col_index);

	std::cout << "fill " << num << " at (" << col_index << ", " << row_index
			  << ")\n";
}

void PrintCst(size_t cst) {
	size_t size(9);

	std::cout << "\t";

	if (cst < size * size) {
		size_t index(cst / size);
		size_t col_index(index / size);
		size_t row_index(index - size * col_index);
		std::cout << "(" << col_index << ", " << row_index << ") used\n";
		return;
	}

	if ((cst -= size * size) < size * size) {
		size_t col_index(cst / size);
		size_t num(cst - size * col_index);
		std::cout << "col " << col_index << " used " << num << "\n";
		return;
	}

	if ((cst -= size * size) < size * size) {
		size_t row_index(cst / size);
		size_t num(cst - size * row_index);
		std::cout << "row " << row_index << " used " << num << "\n";
		return;
	}

	if ((cst -= size * size) < size * size) {
		size_t box_index(cst / size);
		size_t num(cst - size * box_index);
		std::cout << "box " << box_index << " used " << num << "\n";
		return;
	}

	std::cout << "error\n";
}*/

template<size_t rank> void Sudoku<rank>::Solve() {
	if (this->certain_num_ == size_sq) { return; }

	// state id:
	//     fill nume at index:
	//         id expression: size * index + num
	//         range        : [0, size * size)
	//
	// constraint id:
	//     cell has num:
	//         id expression: index
	//         range        : [0, size * size)
	//
	//     col has num:
	//         id expression: size * size + size * col_index + num
	//         range        : [size * size, size * size * 2)
	//
	//     row has num:
	//         id expression: size * size * 2 + size * row_index + num
	//         range        : [size * size * 2, size * size * 3)
	//
	//     box has num:
	//         id expression: size* size * 3 + size* box_index + num
	//         range        : [size * size * 3, size * size * 4)
	//
	// size * size constraints totally

	cntr::Vector<pair<size_t, size_t>> hot;
	IndexArranger state_mapper;
	IndexArranger cst_mapper;

	for (size_t index(0); index != size * size; ++index) {
		if (this->blocks_[index].is_certain()) { continue; }

		for (size_t num(0); num != size; ++num) {
			if (!this->blocks_[index].candidate[num]) { continue; }

			size_t col_index(index / size);
			size_t row_index(index - size * col_index);

			size_t state_id(size * index + num);

			size_t used_cst(index);
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
		size_t a(state_mapper.forward(hot[i].first));
		dlx.AddHot(a, cst_mapper.forward(hot[i].second));
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

	for (size_t i(0); i != size; ++i) {
		Check(this->blocks_, GetSameColIndexs(i));
		Check(this->blocks_, GetSameRowIndexs(i));
		Check(this->blocks_, GetSameBoxIndexs(i / rank, i - i / rank * rank));
	}
}

template<size_t rank>
void Sudoku<rank>::Check(const Block* blocks, const size_t* indexs) {
	bool ok[size] = { false };

	Assign<size>(ok, false);

	for (size_t i(0); i != size; ++i) {
		if (!blocks[indexs[i]].is_certain()) {
			PHI__interrupt;
			continue;
		}

		if (ok[blocks[indexs[i]].value]) {
			PHI__interrupt;
			continue;
		}

		ok[blocks[indexs[i]].value] = true;
	}
}

}

#endif
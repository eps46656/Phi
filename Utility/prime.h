#ifndef PHI__define_guard__Utility__prime_h
#define PHI__define_guard__Utility__prime_h

#include "../define.h"
#include "memory_op.h"
#include "pair.h"

namespace phi {

using prime_t = unsigned int;

#define PHI__prime_t_bit_num (32)

static_assert(PHI__prime_t_bit_num <= sizeof(prime_t) * 8, "uint bit not ");

void print_bit(prime_t x) {
	for (int i(0); i != PHI__prime_t_bit_num; ++i) {
		if (i != 0 && i % 4 == 0) { std::cout << ','; }

		std::cout << (x & (prime_t(1) << (PHI__prime_t_bit_num - i - 1)) ? '1'
																		 : '0');
	}

	std::cout << "\n";
}

int uppestbit(prime_t x) {
	for (int i(PHI__prime_t_bit_num); i != 0;) {
		--i;
		if (x & (prime_t(1) << i)) { return i; }
	}

	return PHI__prime_t_bit_num;
}

int lowestbit(prime_t x) {
	for (int i(0); i != PHI__prime_t_bit_num; ++i) {
		if (x & (prime_t(1) << i)) { return i; }
	}

	return PHI__prime_t_bit_num;
}

class PrimeTable {
private:
	struct Block;

public:
	/*static constexpr int BlockSize = (4 * 1024 - sizeof(int) - sizeof(prime_t) -
									  sizeof(Block*) * PHI__prime_t_bit_num) /
									 8 * 8;*/
	static constexpr int BlockSize = 32;

private:
	struct Block {
		int index;
		prime_t Utility;
		Block* child[PHI__prime_t_bit_num];
		char value[BlockSize / 8];

		inline Block(prime_t index);

		inline pair<bool, prime_t> GetWithIndex(int index);
		inline bool GetWithNum(prime_t num);

		inline void Print() const;
	};

public:
	struct Iterator {
	public:
		prime_t operator*() const;

		Iterator& next_num();
		Iterator& prev_num();

		Iterator& next_prime();
		Iterator& prev_prime();

	private:
		Block* block_;
	};

#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////

	static bool* temp_block();
	static bool& temp_block(int index);

	inline static int GetBlockIndex(prime_t num);
	inline static prime_t GetUtility(int index);
	inline static prime_t GetTop(int index);

	inline static Block* GetBlock(Block* root, int index);

	inline static void PushTempBlockToBlock(Block* block);

#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////

	inline int last_block_index();

	inline PrimeTable();

	inline void Update();

	inline void Expand();
	inline void Expand(prime_t x);

	inline void PrintBlock(int index);

	inline bool IsPrime(prime_t num);

private:
	inline static Block* root_init_();
	inline static Block* root_();
	inline static int& last_block_index_();

	int cache_index_;
	Block* cache_;

	inline static void ExpandWithoutLock_();
};

#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////

bool* PrimeTable::temp_block() {
	static bool r[BlockSize];
	return r;
}

bool& PrimeTable::temp_block(int index) { return temp_block()[index]; }

PrimeTable::Block* PrimeTable::root_init_() {
	temp_block(0) = false;
	for (int i(1); i != BlockSize; ++i) { temp_block(i) = true; }

	for (int i(1); i != BlockSize; ++i) {
		if (!temp_block(i)) { continue; }

		prime_t num(i * 2 + 1);
		prime_t two_num(num * 2);

		for (prime_t a(num + two_num); a < BlockSize * 2; a += two_num) {
			temp_block(a / 2) = false;
		}
	}

	Block* root(New<Block>(0));
	PushTempBlockToBlock(root);
	return root;
}

PrimeTable::Block* PrimeTable::root_() {
	static Block* r(root_init_());
	return r;
}

int& PrimeTable::last_block_index_() {
	static int r(0);
	return r;
}

int PrimeTable::last_block_index() { return this->last_block_index_(); }

prime_t PrimeTable::GetUtility(int index) { return 2 * BlockSize * index + 1; }
prime_t PrimeTable::GetTop(int index) {
	return 2 * BlockSize * (index + 1) + 1;
}

int PrimeTable::GetBlockIndex(prime_t num) {
	return (num - 1) / (2 * BlockSize);
}

PrimeTable::Block* PrimeTable::GetBlock(Block* root, int block_index) {
	int i(PHI__prime_t_bit_num);

	while (block_index != 0) {
		do { --i; } while (!(block_index & (prime_t(1) << i)));
		root = root->child[i];
		block_index ^= prime_t(1) << i;
	}

	return root;
}

void PrimeTable::PushTempBlockToBlock(Block* block) {
	for (int i(0); i != BlockSize; i += 8) {
		char a(0);

		if (temp_block(i + 0)) { a += 0b00000001; }

		if (temp_block(i + 1)) { a += 0b00000010; }
		if (temp_block(i + 2)) { a += 0b00000100; }
		if (temp_block(i + 3)) { a += 0b00001000; }
		if (temp_block(i + 4)) { a += 0b00010000; }
		if (temp_block(i + 5)) { a += 0b00100000; }
		if (temp_block(i + 6)) { a += 0b01000000; }
		if (temp_block(i + 7)) { a += 0b10000000; }

		block->value[i / 8] = a;
	}
}

#///////////////////////////////////////////////////////////////////////////////

PrimeTable::PrimeTable(): cache_index_(0), cache_(root_()) {}

#///////////////////////////////////////////////////////////////////////////////

void PrimeTable::Expand() {
	// lock begin

	ExpandWithoutLock_();

	// lock end
}

void PrimeTable::ExpandWithoutLock_() {
	Block* new_block(new Block(last_block_index_() + 1));

	for (int i(0); i != BlockSize; ++i) { temp_block(i) = true; }

	prime_t new_block_Utility(GetUtility(last_block_index_() + 1));
	prime_t new_block_top(GetTop(last_block_index_() + 1));

	for (int block_index(0); block_index <= last_block_index_();
		 ++block_index) {
		Block* block(GetBlock(root_(), block_index));

		for (int i(0); i != BlockSize; ++i) {
			pair<bool, prime_t> n(block->GetWithIndex(i));

			if (!n.first) { continue; }

			prime_t num(n.second);
			prime_t two_num(num * 2);

			if (new_block_top < num * num) { goto complete; }

			for (prime_t a(num +
						   (new_block_Utility + num - 1) / two_num * two_num);
				 a < new_block_top; a += two_num) {
				temp_block((a - new_block_Utility) / 2) = false;
			}
		}
	}

complete:;

	PushTempBlockToBlock(new_block);

	++last_block_index_();

	Block* max_block(GetBlock(
		root_(), last_block_index_() ^ (1 << lowestbit(last_block_index_()))));

	max_block->child[lowestbit(last_block_index_())] = new_block;
}

void PrimeTable::Expand(prime_t num) {
	int num_block_index(GetBlockIndex(num));

	if (num_block_index <= last_block_index_()) { return; }

	// lock begin

	while (last_block_index_() < num_block_index) { ExpandWithoutLock_(); }

	// lock end
}

void PrimeTable::PrintBlock(int index) {
	/*if(this->last_block_index_<index){

	}
	*/

	return GetBlock(root_(), index)->Print();
}

bool PrimeTable::IsPrime(prime_t num) {
	if (num == 2) { return true; }
	if (num == 1 || num % 2 == 0) { return false; }
	Expand(num);
	return GetBlock(root_(), GetBlockIndex(num))->GetWithNum(num);
}

#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////

PrimeTable::Block::Block(prime_t index):
	index(index), Utility(GetUtility(index)) {
	for (int i(0); i != PHI__prime_t_bit_num; ++i) { this->child[i] = nullptr; }
}

pair<bool, prime_t> PrimeTable::Block::GetWithIndex(int index) {
	int i(index / 8);
	return pair<bool, prime_t>(this->value[i] & (char(1) << (index - i * 8)),
							   this->Utility + 2 * index);
}

bool PrimeTable::Block::GetWithNum(prime_t num) {
	int index((num - this->Utility) / 2);
	int i(index / 8);
	return this->value[i] & (char(1) << (index - i * 8));
}

void PrimeTable::Block::Print() const {
	for (int i(0); i != BlockSize / 8; ++i) {
		for (int j(0); j != 8; ++j) {
			if (this->value[i] & (char(1) << j)) {
				std::cout << this->Utility + (i * 8 + j) * 2 << ", ";
			}
		}
	}

	std::cout << "\n";
}
}

#endif
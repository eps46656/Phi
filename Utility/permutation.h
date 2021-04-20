#ifndef PHI__define_guard__Utility__Permutation_h
#define PHI__define_guard__Utility__Permutation_h

#include "memory_op.h"
#include "compare.h"

namespace phi {

template<typename LessThanComparer> struct prev_permutation_cmper_ {
	const LessThanComparer& lt_cmper;

	prev_permutation_cmper_(const LessThanComparer& lt_cmper):
		lt_cmper(lt_cmper) {}

	template<typename X, typename Y> bool lt(X& x, Y& y) const {
		return this->lt_cmper.lt(y, x);
	}
};

#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////

template<typename Src, typename LessThanComparer = DefaultLessThanComparer,
		 typename Swapper = DefaultSwapper>
bool next_permutation(size_t lower, size_t upper, Src& src,
					  const LessThanComparer& lt_cmper = LessThanComparer(),
					  const Swapper& swapper = Swapper()) {
	if (upper <= lower) { return false; }
	size_t n(upper - lower);
	if (n < 2) { return false; }

	size_t i(upper - 1);

	for (; !lt_cmper.lt(src[i - 1], src[i]); --i) {
		if (i - 1 == lower) { return false; }
	}

	size_t j(upper - 1);
	while (!lt_cmper.lt(src[i - 1], src[j])) { --j; }
	swapper(src[i - 1], src[j]);
	Reverse(i, upper, src, swapper);

	return true;
}

template<typename Src, typename LessThanComparer = DefaultLessThanComparer,
		 typename Swapper = DefaultSwapper>
bool next_permutation(size_t size, Src& src,
					  const LessThanComparer& lt_cmper = LessThanComparer(),
					  const Swapper& swapper = Swapper()) {
	return next_permutation(0, size, src, lt_cmper, swapper);
}

template<typename BidirectionalIterator,
		 typename LessThanComparer = DefaultLessThanComparer,
		 typename Swapper = DefaultSwapper>
bool next_permutation(BidirectionalIterator begin, BidirectionalIterator end,
					  const LessThanComparer& lt_cmper = LessThanComparer(),
					  const Swapper& swapper = Swapper()) {
	if (begin == end) { return false; }

	BidirectionalIterator i(end);
	if (begin == --i) { return false; }

	BidirectionalIterator i_prev(i);
	--i_prev;

	BidirectionalIterator j(i);

	for (;;) {
		if (i == begin) { return false; }
		if (lt_cmper.lt(*i_prev, *i)) { break; }
		i = i_prev;
		--i_prev;
	}

	while (!lt_cmper.lt(*i_prev, *j)) { --j; }
	swapper(*i_prev, *j);
	Reverse(i, end, swapper);

	return true;
}

#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////

template<typename Src, typename LessThanComparer = DefaultLessThanComparer,
		 typename Swapper = DefaultSwapper>
bool prev_permutation(size_t n, Src&& src,
					  const LessThanComparer& lt_cmper = LessThanComparer(),
					  const Swapper& swapper = Swapper()) {
	return next_permutation(
		n, src, prev_permutation_cmper_<LessThanComparer>(lt_cmper), swapper);
}

template<typename Src, typename LessThanComparer = DefaultLessThanComparer,
		 typename Swapper = DefaultSwapper>
bool prev_permutation(size_t lower, size_t upper, Src&& src,
					  const LessThanComparer& lt_cmper = LessThanComparer(),
					  const Swapper& swapper = Swapper()) {
	return next_permutation(lower, upper, src,
							prev_permutation_cmper_<LessThanComparer>(lt_cmper),
							swapper);
}

template<typename BidirectionalIterator,
		 typename LessThanComparer = DefaultLessThanComparer,
		 typename Swapper = DefaultSwapper>
bool prev_permutation(BidirectionalIterator begin, BidirectionalIterator end,
					  const LessThanComparer& lt_cmper = LessThanComparer(),
					  const Swapper& swapper = Swapper()) {
	return next_permutation(begin, end,
							prev_permutation_cmper_<LessThanComparer>(lt_cmper),
							swapper);
}

#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////

class PermutationGenerator {
public:
	static constexpr size_t max_n = 32;

	size_t n() const;
	size_t operator[](size_t index) const;

	void set(size_t n);
	template<typename Src> void set(size_t n, const Src& indexes);
	void set_at_first();
	void set_at_last();

	PermutationGenerator(size_t n);

	template<typename Indexes>
	PermutationGenerator(size_t n, const Indexes&& indexes);

	bool next();
	bool prev();

private:
	size_t n_;
	char data_[max_n];
};

#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////

size_t PermutationGenerator::n() const { return this->n_; }
size_t PermutationGenerator::operator[](size_t index) const {
	PHI__debug_if(this->n_ <= index) {
		PHI__throw(PermutationGenerator, __func__, "index error");
	}

	return this->data_[index];
}

void PermutationGenerator::set(size_t n) {
	PHI__debug_if(max_n < n) {
		PHI__throw(PermutationGenerator, __func__, "n error");
	}

	this->n_ = n;
	for (size_t i(0); i != n; ++i) { this->data_[i] = i; }
}

template<typename Indexes>
void PermutationGenerator::set(size_t n, const Indexes& indexes) {
	PHI__debug_if(max_n < n) {
		PHI__throw(PermutationGenerator, __func__, "n error");
	}

	if (n == 0) {
		this->n_ = n;
		return;
	}

	char new_data[max_n];
	bool has_used[max_n] = { false };

	for (size_t i(0); i != n; ++i) {
		size_t index(indexes[i]);

		if (n <= index || has_used[index]) {
			PHI__throw(PermutationGenerator, __func__, "index error");
		}

		has_used[index] = true;
		new_data[i] = index;
	}

	this->n_ = n;
	Copy<max_n>(this->data_, new_data);
}

#///////////////////////////////////////////////////////////////////////////////

PermutationGenerator::PermutationGenerator(size_t n) { this->set(n); }

template<typename Indexes>
PermutationGenerator::PermutationGenerator(size_t n, const Indexes&& indexes) {
	this->set(n, indexes);
}

#///////////////////////////////////////////////////////////////////////////////

bool PermutationGenerator::next() {
	return next_permutation(this->n_, this->data_);
}

bool PermutationGenerator::prev() {
	return prev_permutation(this->n_, this->data_);
}

#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////

class IntegerHeapPermutation {
public:
	static constexpr size_t max_n = 32;

	static bool Next(size_t* indexes, size_t n, size_t* data);
	static bool Prev(size_t* indexes, size_t n, size_t* data);

	template<typename Receiver>
	static void GenerateAll(size_t n, const Receiver& receiver);

#///////////////////////////////////////////////////////////////////////////////

	size_t n() const;
	size_t operator[](size_t index) const;
	const size_t* data() const;

	template<typename... Args> void set(size_t n, Args&&... args);

	void set_first();
	void set_last();
	void reset();

	void is_end() const;

#///////////////////////////////////////////////////////////////////////////////

	IntegerHeapPermutation(size_t n);
	operator bool() const;

#///////////////////////////////////////////////////////////////////////////////

	IntegerHeapPermutation& operator++();
	IntegerHeapPermutation& operator--();

private:
	template<typename Receiver>
	static bool GenerateAll_even_(size_t m, size_t n, size_t* data,
								  const Receiver& receiver);

	template<typename Receiver>
	static bool GenerateAll_odd_(size_t m, size_t n, size_t* data,
								 const Receiver& receiver);

#///////////////////////////////////////////////////////////////////////////////

	size_t n_;
	size_t data_[max_n];
	size_t indexes_[max_n];
	bool is_null_;
};

#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////

bool IntegerHeapPermutation::Next(size_t* indexes, size_t n, size_t* data) {
	size_t i(0);

	while (indexes[i] == i + 1) {
		indexes[i] = 0;
		if (++i == n - 1) { return false; }
	}

	if (i % 2 == 0) {
		Swap(data[indexes[i]], data[i + 1]);
	} else {
		Swap(data[0], data[i + 1]);
	}

	++indexes[i];
	return true;
}

bool IntegerHeapPermutation::Prev(size_t* indexes, size_t n, size_t* data) {
	size_t i(0);

	while (indexes[i] == 0) {
		indexes[i] = i + 1;
		if (++i == n - 1) { return false; }
	}

	--indexes[i];

	if (i % 2 == 0) {
		Swap(data[indexes[i]], data[i + 1]);
	} else {
		Swap(data[0], data[i + 1]);
	}

	return true;
}

/*

indexes bit number | state number
1                  | 2
2                  | 2 * 3 = 3! = 6
3                  | 2 * 3 * 4 = 4! = 24
n-1                | n!

a "n permutation" has n! state, onlyt nead n-1 bit number indexes

*/

template<typename Receiver>
void IntegerHeapPermutation::GenerateAll(size_t n, const Receiver& receiver) {
	size_t data[max_n];

	for (size_t i(0); i != n; ++i) { data[i] = i; }

	if (n % 2 == 0) {
		GenerateAll_even_(n, n, data, receiver);
	} else {
		GenerateAll_odd_(n, n, data, receiver);
	}
}

template<typename Receiver>
bool IntegerHeapPermutation::GenerateAll_even_(size_t m, size_t n, size_t* data,
											   const Receiver& receiver) {
	if (!GenerateAll_odd_(m - 1, n, data, receiver)) { return false; }

	for (size_t i(0); i != m - 1; ++i) {
		Swap(data[i], data[m - 1]);
		if (!GenerateAll_odd_(m - 1, n, data, receiver)) { return false; }
	}

	return true;
}

template<typename Receiver>
bool IntegerHeapPermutation::GenerateAll_odd_(size_t m, size_t n, size_t* data,
											  const Receiver& receiver) {
	if (m == 1) { return receiver(n, static_cast<const size_t*>(data)); }

	if (!GenerateAll_even_(m - 1, n, data, receiver)) { return false; }

	for (size_t i(0); i != m - 1; ++i) {
		Swap(data[0], data[m - 1]);
		if (!GenerateAll_even_(m - 1, n, data, receiver)) { return false; }
	}

	return true;
}

#///////////////////////////////////////////////////////////////////////////////

size_t IntegerHeapPermutation::n() const { return this->n_; }

size_t IntegerHeapPermutation::operator[](size_t index) const {
	PHI__debug_if(this->n_ <= index) {
		PHI__throw(IntegerHeapPermutation, "operator[]", "index error");
	}

	return this->data_[index];
}

const size_t* IntegerHeapPermutation::data() const { return this->data_; }

#///////////////////////////////////////////////////////////////////////////////

IntegerHeapPermutation::IntegerHeapPermutation(size_t n): n_(n) {
	this->set_first();
}

IntegerHeapPermutation::operator bool() const { return !this->is_null_; }

#///////////////////////////////////////////////////////////////////////////////

void IntegerHeapPermutation::set_first() {
	if (this->n_ == 0) {
		this->is_null_ = true;
		return;
	}

	for (size_t i(0); i != this->n_; ++i) {
		this->data_[i] = i;
		this->indexes_[i] = 0;
	}

	this->is_null_ = false;
}

void IntegerHeapPermutation::set_last() {
	if (this->n_ == 0) {
		this->is_null_ = true;
		return;
	}

	for (size_t i(0); i != this->n_; ++i) { this->indexes_[i] = i + 1; }

	if (this->n_ % 2 == 0) {
		for (size_t i(0); i != this->n_ - 1; ++i) { this->data_[i] = i + 1; }
		this->data_[this->n_ - 1] = 0;
	} else {
		this->data_[0] = this->n_ - 1;

		if (this->n_ != 1) {
			for (size_t i(1); i != this->n_ - 1; ++i) { this->data_[i] = i; }
			this->data_[this->n_ - 1] = 0;
		}
	}

	this->is_null_ = false;
}

void IntegerHeapPermutation::reset() { this->set_first(); }

#///////////////////////////////////////////////////////////////////////////////

IntegerHeapPermutation& IntegerHeapPermutation::operator++() {
	if (this->is_null_) {
		this->set_first();
	} else {
		this->is_null_ = !Next(this->indexes_, this->n_, this->data_);
	}

	return *this;
}

IntegerHeapPermutation& IntegerHeapPermutation::operator--() {
	if (this->is_null_) {
		this->set_last();
	} else {
		this->is_null_ = !Prev(this->indexes_, this->n_, this->data_);
	}

	return *this;
}

#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////

#if false

TODO
generalize to other type
base on IntegerHeapPermutation, just adapt index
custom T and Swapper

template<typename Swapper = DefaultSwapper> class HeapPermutation {
public:
	static constexpr size_t max_n = 32;

	template<typename Data, typename Swapper = DefaultSwapper>
	static bool Next(size_t& n, size_t* index, Data& data,
					 const Swapper& swapper = Swapper());

	template<typename Data, typename Receiver,
			 typename Swapper = DefaultSwapper>
	static void GenerateAll(size_t size, Data& data, const Receiver& receiver,
							const Swapper& swapper = Swapper());

	#///////////////////////////////////////////////////////////////////////////////

	size_t size() const;
	size_t operator[](size_t index) const;
	Swapper& swapper();
	const Swapper& swapper() const;

	void set_size(size_t size);
	void reset();

	void is_end() const;

	#///////////////////////////////////////////////////////////////////////////////

	template<typename... Args> HeapPermutation(size_t n, Args&&... args);

	operator bool() const;

	#///////////////////////////////////////////////////////////////////////////////

	HeapPermutation& operator++();
	HeapPermutation& operator--();

private:
	template<typename Data, typename Receiver>
	static void GenerateAll_even_(size_t m, size_t n, Data& data,
								  const Receiver& receiver,
								  const Swapper& swapper);

	template<typename Data, typename Receiver>
	static void GenerateAll_odd_(size_t m, size_t n, Data& data,
								 const Receiver& receiver,
								 const Swapper& swapper);

	#///////////////////////////////////////////////////////////////////////////////

	size_t n_;
	unsigned char data_[max_n];
	bool is_end_;

	Swapper swapper_;

	size_t m_;
	size_t indexes_[max_n];
};

	#///////////////////////////////////////////////////////////////////////////////
	#///////////////////////////////////////////////////////////////////////////////
	#///////////////////////////////////////////////////////////////////////////////

template<typename Swapper>
template<typename Data>
bool HeapPermutation<Swapper>::Next(size_t& m, size_t* indexes, Data& data,
									const Swapper& swapper) {
	while (indexes[m] == m + 1) {
		indexes[m] = 0;
		if (++m == size - 1) { return false; }
	}

	if (m % 2 == 0) {
		swapper(data[indexes[m]], data[m + 1]);
	} else {
		swapper(data[0], data[m + 1]);
	}

	++indexes[m];
	m = 0;

	return true;
}

template<typename Swapper>
template<typename Data, typename Receiver, typename Swapper>
void HeapPermutation::GenerateAll(size_t n, Data& data,
								  const Receiver& receiver,
								  const Swapper& swapper) {
	if (n % 2 == 0) {
		GenerateAll_even_(n, n, data, receiver, swapper);
	} else {
		GenerateAll_odd_(n, n, data, receiver, swapper);
	}
}

template<typename Swapper>
template<typename Data, typename Receiver, typename Swapper>
void HeapPermutation<Swapper>::GenerateAll_even_(size_t m, size_t n, Data& data,
												 const Receiver& receiver,
												 const Swapper& swapper) {
	GenerateAll_odd_(m - 1, n, data, receiver, swapper);

	for (size_t i(0); i != m - 1; ++i) {
		swapper(data[i], data[m - 1]);
		GenerateAll_odd_(m - 1, n, data, receiver, swapper);
	}
}

template<typename Swapper>
template<typename Data, typename Receiver, typename Swapper>
void HeapPermutation<Swapper>::GenerateAll_odd_(size_t m, size_t n, Data& data,
												const Receiver& receiver,
												const Swapper& swapper) {
	if (m == 1) {
		receiver(n, data);
		return;
	}

	GenerateAll_even_(m - 1, n, data, receiver, swapper);

	for (size_t i(0); i != m - 1; ++i) {
		swapper(data[0], data[m - 1]);
		GenerateAll_even_(m - 1, n, data, receiver, swapper);
	}
}

	#///////////////////////////////////////////////////////////////////////////////

template<typename Swapper> size_t HeapPermutation<Swapper>::n() const {
	return this->n_;
}

template<typename Swapper>
size_t HeapPermutation<Swapper>::operator[](size_t index) const {
	return this->data_[index];
}

template<typename Swapper> Swapper& HeapPermutation<Swapper>::swapper() {
	return this->swapper_;
}
template<typename Swapper>
const Swapper& HeapPermutation<Swapper>::swapper() const {
	return this->swapper_;
}

	#///////////////////////////////////////////////////////////////////////////////

template<typename Swapper>
template<typename... Args>
HeapPermutation<Swapper>::HeapPermutation(size_t n, Args&&... args):
	n_(n), swapper_(Forward<Args>(args)...) {
	this->reset();
}

template<typename Swapper> HeapPermutation<Swapper>::operator bool() const {
	return !this->is_end_;
}

	#///////////////////////////////////////////////////////////////////////////////

template<typename Swapper> void HeapPermutation<Swapper>::reset() {
	for (size_t i(0); i != this->n_; ++i) {
		this->data_[i] = i;
		this->indexes_[i] = 0;
	}

	this->is_end_ = this->m_ == 0;
}

	#///////////////////////////////////////////////////////////////////////////////

template<typename Swapper>
HeapPermutation<Swapper>& HeapPermutation<Swapper>::operator++() {
	if (!this->is_end_) {
		this->is_end_ =
			Next(this->m_, this->indexes_, this->data_, DefaultSwapper());
	}

	return *this;
}

#endif
}

#endif
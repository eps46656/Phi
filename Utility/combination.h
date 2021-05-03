#ifndef PHI__define_guard__Utility__combination_h
#define PHI__define_guard__Utility__combination_h

#include "../define.h"

namespace phi {

class IntegerCombination {
public:
	static constexpr size_t max_n = 32;

	template<typename Receiver>
	inline static void GenerateAll(size_t n, size_t k,
								   const Receiver& receiver);

	inline static bool Next(size_t n, size_t k, size_t* data);
	inline static bool Next(size_t n, size_t k, size_t* data, size_t index);

#///////////////////////////////////////////////////////////////////////////////

	inline bool is_end() const;

#///////////////////////////////////////////////////////////////////////////////

	inline IntegerCombination(size_t k, size_t n);

#///////////////////////////////////////////////////////////////////////////////

	inline void reset();

	inline void next();
	inline void next(size_t index);

#///////////////////////////////////////////////////////////////////////////////

	size_t operator[](size_t index) const;

private:
	template<typename Receiver>
	static bool GenerateAll_(size_t data_i, size_t* data, size_t lower,
							 size_t n, size_t k, const Receiver& receiver);

	size_t n_;
	size_t k_;
	size_t data_[max_n];
	bool is_end_;
};

#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////

template<typename Receiver>
void IntegerCombination::GenerateAll(size_t n, size_t k,
									 const Receiver& receiver) {
	size_t data[max_n];
	GenerateAll_(0, data, 0, n, k, receiver);
}

template<typename Receiver>
bool IntegerCombination::GenerateAll_(size_t data_i, size_t* data, size_t lower,
									  size_t n, size_t k,
									  const Receiver& receiver) {
	if (data_i == k) {
		return receiver(static_cast<const size_t*>(data),
						static_cast<const size_t*>(data + k));
	}

	size_t upper(n - k + data_i + 1);

	for (size_t i(lower); i != upper; ++i) {
		data[data_i] = i;
		if (!GenerateAll_(data_i + 1, data, i + 1, n, k, receiver)) {
			return false;
		}
	}

	return true;
}

bool IntegerCombination::Next(size_t n, size_t k, size_t* data) {
	Next(n, k, data, k - 1);
}

bool IntegerCombination::Next(size_t n, size_t k, size_t* data, size_t index) {
	if (data[0] == n - k) { return false; }

	while (data[index] == n - k + index) { --index; }
	++data[index];
	while (++index != k) { data[index] = data[index - 1] + 1; }

	return true;
}

#///////////////////////////////////////////////////////////////////////////////

bool IntegerCombination::is_end() const { return this->is_end_; }

#///////////////////////////////////////////////////////////////////////////////

IntegerCombination::IntegerCombination(size_t n, size_t k): n_(n), k_(k) {
	this->reset();
}

#///////////////////////////////////////////////////////////////////////////////

void IntegerCombination::reset() {
	for (size_t i(0); i != this->k_; ++i) { this->data_[i] = i; }
	this->is_end_ = false;
}

#///////////////////////////////////////////////////////////////////////////////

void IntegerCombination::next() {
	this->is_end_ = !Next(this->n_, this->k_, this->data_);
}

void IntegerCombination::next(size_t index) {
	this->is_end_ = !Next(this->n_, this->k_, this->data_, index);
}

#///////////////////////////////////////////////////////////////////////////////

size_t IntegerCombination::operator[](size_t index) const {
	return this->data_[index];
}

}

#endif
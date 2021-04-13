#ifndef PHI__define_guard__Utility__combination_h
#define PHI__define_guard__Utility__combination_h

#include "../define.h"

namespace phi {

class IntegerCombination {
public:
	static constexpr size_t max_n = 32;

	template<typename Receiver>
	static void GenerateAll(size_t k, size_t n, const Receiver& receiver);

	static bool Next(size_t k, size_t n, size_t* data);

private:
	template<typename Receiver>
	static bool GenerateAll_(size_t data_i, size_t* data, size_t lower,
							 size_t k, size_t n, const Receiver& receiver);

	size_t n_;
	size_t k_;
	size_t data[max_n];
	size_t indexes[max_n];
	bool is_end;
};

#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////

template<typename Receiver>
void IntegerCombination::GenerateAll(size_t k, size_t n,
									 const Receiver& receiver) {
	size_t data[max_n];
	GenerateAll_(0, data, 0, k, n, receiver);
}

template<typename Receiver>
bool IntegerCombination::GenerateAll_(size_t data_i, size_t* data, size_t lower,
									  size_t k, size_t n,
									  const Receiver& receiver) {
	if (data_i == k) { return receiver(k, data); }

	size_t upper(n - k + data_i + 1);

	for (size_t i(lower); i != upper; ++i) {
		data[data_i] = i;
		if (!GenerateAll_(data_i + 1, data, i + 1, k, n, receiver)) {
			return false;
		}
	}

	return true;
}

bool IntegerCombination::Next(size_t k, size_t n, size_t* data) {
	if (data[0] == n - k) { return false; }

	size_t i(k - 1);
	while (data[i] == n - k + i) { --i; }
	++data[i];
	while (++i != k) { data[i] = data[i - 1] + 1; }

	return true;
}

}

#endif
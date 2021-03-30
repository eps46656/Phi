#ifndef PHI__define_guard__Container__Map_h
#define PHI__define_guard__Container__Map_h

#include "../Utility/pair.h"
#include "../Utility/compare.h"
#include "Set.h"

namespace phi {
namespace cntr {

template<typename Index, typename Value, typename FullComparer>
struct MapFullComparer_ {
	const FullComparer& full_cmper;

	MapFullComparer_(const FullComparer& full_cmper): full_cmper(full_cmper) {}

	int operator()(const pair<Index, Value>& x,
				   const pair<Index, Value>& y) const {
		return this->full_cmper(x.first, y.first);
	}

	int operator()(const pair<Index, Value>& x, const Index& y) const {
		return this->full_cmper(x.first, y);
	}

	int operator()(const Index& x, const pair<Index, Value>& y) const {
		return this->full_cmper(x, y.first);
	}
};

template<typename Index, typename Value, typename FullComparer>
using MapFullComparer =
	AutoImplementFullComparer<MapFullComparer_<Index, Value, FullComparer>>;

template<typename Index, typename Value,
		 typename FullComparer = DefaultFullComparer>
using Map =
	Set<pair<Index, Value>, MapFullComparer<Index, Value, FullComparer>>;

}
}

#endif
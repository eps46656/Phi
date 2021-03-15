#ifndef PHI__define_guard__Container__Map_h
#define PHI__define_guard__Container__Map_h

#include "../Utility/pair.h"
#include "RedBlackTree.h"

namespace phi {
namespace cntr {

template<typename Index, typename Value, typename FullComparer>
struct MapFullComparer {
	FullComparer cmp;

	bool operator()(const Index& x, const pair<Index, Value>& y) const {
		return this->cmp(x, y.first);
	}

	bool operator()(const pair<Index, Value>& x, const Index& y) const {
		return this->cmp(x.first, y);
	}

	bool operator()(const pair<Index, Value>& x,
					const pair<Index, Value>& y) const {
		return this->cmp(x.first, y.first);
	}
};

template<typename Index, typename Value,
		 typename FullComparer = DefaultFullComparer>
using Map = RedBlackTree<pair<Index, Value>,
						 MapFullComparer<Index, Value, FullComparer>>;

}
}

#endif
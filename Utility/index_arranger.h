#ifndef PHI__define_guard__Utility__index_arranger_h
#define PHI__define_guard__Utility__index_arranger_h

#include "search.h"
#include "sort2.h"
#include "../Container/Vector.h"

namespace phi {

class IndexArranger {
public:
	inline size_t size() const;

	size_t forward(size_t index) const;
	size_t backward(size_t index) const;

#///////////////////////////////////////////////////////////////////////////////

	inline void Add(size_t index);

private:
	cntr::Vector<size_t> index_;
};

#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////

size_t IndexArranger::size() const { return this->index_.size(); }

size_t IndexArranger::forward(size_t index) const {
	return BinarySearch(this->index_.first_iterator(),
						this->index_.null_iterator(), index) -
		   this->index_.first_iterator();
}

size_t IndexArranger::backward(size_t index) const {
	return this->index_[index];
}

#///////////////////////////////////////////////////////////////////////////////

void IndexArranger::Add(size_t index) {
	if (ContainBinary(this->index_.first_iterator(),
					  this->index_.null_iterator(), index)) {
		return;
	}

	this->index_.Push(index);
	LinearInsert(this->index_.first_iterator(), this->index_.null_iterator());
}
}

#endif
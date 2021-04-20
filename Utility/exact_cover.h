
#ifndef PHI__define_guard__Utility__exact_cover_h
#define PHI__define_guard__Utility__exact_cover_h

#include "search.h"
#include "../Container/Vector.h"
#include "../Container/DoublyNode.h"

namespace phi {
namespace exact_cover {

struct Option: public cntr::DoublyNode {
	cntr::Vector<size_t> necessary_constraint;
	cntr::Vector<size_t> optional_constraint;

	Option* prev_cover;

	inline static bool Conflict(const cntr::Vector<size_t>& a,
								const cntr::Vector<size_t>& b);
	inline static bool Conflict(const Option* a, const Option* b);

	bool is_covered() const;

	void cover();
	void uncover();
};

bool Option::Conflict(const cntr::Vector<size_t>& a_,
					  const cntr::Vector<size_t>& b_) {
	if (a_.empty() || b_.empty() || a_.back() < b_.front() ||
		b_.back() < a_.front()) {
		return false;
	}

	const cntr::Vector<size_t>* a;
	const cntr::Vector<size_t>* b;

	if (a_.size() < b_.size()) {
		a = &a_;
		b = &b_;
	} else {
		a = &b_;
		b = &a_;
	}

	size_t lower(0);

	for (size_t i(0); i != a->size(); ++i) {
		size_t upper(b->size() - 1);

		while (lower < upper) {
			size_t mid((lower + upper) / 2);

			if ((*a)[i] < (*b)[mid]) {
				upper = mid;
			} else if ((*b)[mid] < (*a)[i]) {
				lower = mid + 1;
			} else {
				return true;
			}
		}

		if (lower == upper) {
			if ((*a)[i] < (*b)[lower]) { continue; }
			return (*a)[i] == (*b)[lower];
		}
	}

	return false;
}

bool Option::Conflict(const Option* a, const Option* b) {
	return Conflict(a->necessary_constraint, b->necessary_constraint) ||
		   Conflict(a->optional_constraint, b->optional_constraint);
}

bool Option::is_covered() const {
	return static_cast<Option*>(this->prev_)->next_ != this;
}

void Option::cover() { Link_(this->prev_, this->next_); }

void Option::uncover() {
	if (this->is_covered()) {
		static_cast<Option*>(this->prev_)->next_ = this;
		static_cast<Option*>(this->next_)->prev_ = this;
	}
}

bool Solve_(size_t depth, cntr::Vector<cntr::Vector<Option*>>& solution_dst,
			size_t necessary_constraint_size, Option* option_begin,
			cntr::DoublyNode* options);

cntr::Vector<cntr::Vector<Option*>> Solve(size_t necessary_constraint_size,
										  cntr::DoublyNode* options) {
	cntr::Vector<cntr::Vector<Option*>> solution_dst;
	solution_dst.Push();

	Solve_(0, solution_dst, necessary_constraint_size,
		   static_cast<Option*>(options->next()), options);

	solution_dst.Pop();

	return solution_dst;
}

bool Solve_(size_t depth, cntr::Vector<cntr::Vector<Option*>>& solution_dst,
			size_t necessary_constraint_size, Option* option_begin,
			cntr::DoublyNode* options) {
	if (necessary_constraint_size == 0) {
		solution_dst.Push(solution_dst.back());
		return false;
	}

	if (solution_dst.back().size() < depth + 1) {
		solution_dst.back().Push(nullptr);
	}

	for (Option* i(option_begin); i != options;
		 i = static_cast<Option*>(i->next())) {
		Option* cover(nullptr);

		for (Option* j(static_cast<Option*>(options->next())); j != options;
			 j = static_cast<Option*>(j->next())) {
			if (i != j && Option::Conflict(i, j)) {
				j->cover();
				j->prev_cover = cover;
				cover = j;
			}
		}

		i->cover();

		solution_dst.back()[depth] = i;

		Option* i_next(static_cast<Option*>(i->next()));

		Solve_(depth + 1, solution_dst,
			   necessary_constraint_size - i->necessary_constraint.size(),
			   i_next, options);

		i->uncover();

		while (cover != nullptr) {
			cover->uncover();
			cover = cover->prev_cover;
		}
	}

	return false;
}

}
}

#endif
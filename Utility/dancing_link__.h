#ifndef PHI__define_guard__Utility__dancing_linking_h
#define PHI__define_guard__Utility__dancing_linking_h

#include "sort2.h"
#include "../Container/Vector.h"
#include "../Container/Map.h"
#include "../Container/Pool.h"
#include <iostream>

namespace phi {
namespace dancing_link {

#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////

template<typename State, typename Constraint, typename StateFullComparer,
		 typename ConstraintFullComparer>
class DancingLink {
public:
	class Node {
	public:
		size_t col_index;
		size_t row_index;
		Node* col_header;
		Node* row_header;
		Node* u;
		Node* d;
		Node* l;
		Node* r;

		inline Node(size_t col_index, size_t row_index);

		inline bool is_root() const;
		inline bool is_col_header() const;
		inline bool is_row_header() const;
		inline bool is_normal() const;

		inline bool is_sole_ud() const;
		inline bool is_sole_lr() const;

		inline void PushU(Node* node);
		inline void PushD(Node* node);
		inline void PushL(Node* node);
		inline void PushR(Node* node);

		inline void PopUD();
		inline void PopLR();
		inline void Pop();

		inline void CoverUD();
		inline void CoverLR();

		inline void UnCoverUD();
		inline void UnCoverLR();

	protected:
		inline void LinkUD_(Node* x, Node* y);
		inline void LinkLR_(Node* x, Node* y);
	};

#///////////////////////////////////////////////////////////////////////////////

	struct StateAgent {
		State state;

		cntr::Set<Constraint> nec_cst;
		cntr::Set<Constraint> opt_cst;
	};

	struct StateAgentFullComparer_ {
		StateFullComparer state_full_cmper;

		template<typename StateFullComparerInitializeArg>
		StateAgentFullComparer_(
			StateFullComparerInitializeArg&& state_full_cmper_init_arg);

		int operator()(const StateAgent& a, const StateAgent& b) const;
	};

	using StateAgentFullComparer =
		AutoImplementFullComparer<StateAgentFullComparer_>;

#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////

	size_t state_num() const;

	size_t nec_cst_num() const;
	size_t opt_cst_num() const;
	size_t cst_num() const;

	StateFullComparer& state_full_cmper();
	const StateFullComparer& state_full_cmper() const;

	ConstraintFullComparer& cst_full_cmper();
	const ConstraintFullComparer& cst_full_cmper() const;

	void cst_full_cmper();
	void cst_full_cmper() const;

	bool solve_ready() const;

#///////////////////////////////////////////////////////////////////////////////

	template<typename StateFullComparerInitializeArg,
			 typename ConstraintFullComparerInitializeArg>
	DancingLink(StateFullComparerInitializeArg&& state_full_cmper_init_arg,
				ConstraintFullComparerInitializeArg&& cst_full_cmper_init_arg);

	template<typename NecCstCntrIterator, typename OptCstCntrIterator>
	void AddState(const State& state, NecCstCntrIterator nec_cst_begin,
				  NecCstCntrIterator nec_cst_end,
				  OptCstCntrIterator opt_nec_begin,
				  OptCstCntrIterator opt_nec_end);

	void SomeAmazingAlgToOptimizeIndex();

	void Build();
	void Release();
	template<typename Receiver> void Solve(Receiver& receiver);

	// private:
	cntr::Map<State, size_t, StateAgentFullComparer> state_index_;
	cntr::Map<Constraint, size_t, ConstraintFullComparer> nec_cst_index_;
	cntr::Map<Constraint, size_t, ConstraintFullComparer> opt_cst_index_;

	Node* root_;
	cntr::Pool<sizeof(Node)> pool_;
	bool solve_ready_;
};

#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////

template<typename State, typename Constraint, typename StateFullComparer,
		 typename ConstraintFullComparer>
void DancingLink<State, Constraint, StateFullComparer,
				 ConstraintFullComparer>::Node::LinkUD_(Node* x, Node* y) {
	x->d = y;
	y->u = x;
}

template<typename State, typename Constraint, typename StateFullComparer,
		 typename ConstraintFullComparer>
void DancingLink<State, Constraint, StateFullComparer,
				 ConstraintFullComparer>::Node::LinkLR_(Node* x, Node* y) {
	x->r = y;
	y->l = x;
}

#///////////////////////////////////////////////////////////////////////////////

template<typename State, typename Constraint, typename StateFullComparer,
		 typename ConstraintFullComparer>
bool DancingLink<State, Constraint, StateFullComparer,
				 ConstraintFullComparer>::Node::is_root() const {
	return this->is_col_header() && this->is_row_header();
}

template<typename State, typename Constraint, typename StateFullComparer,
		 typename ConstraintFullComparer>
bool DancingLink<State, Constraint, StateFullComparer,
				 ConstraintFullComparer>::Node::is_col_header() const {
	return this == this->col_header;
}

template<typename State, typename Constraint, typename StateFullComparer,
		 typename ConstraintFullComparer>
bool DancingLink<State, Constraint, StateFullComparer,
				 ConstraintFullComparer>::Node::is_row_header() const {
	return this == this->row_header;
}

template<typename State, typename Constraint, typename StateFullComparer,
		 typename ConstraintFullComparer>
bool DancingLink<State, Constraint, StateFullComparer,
				 ConstraintFullComparer>::Node::is_normal() const {
	return !this->is_col_header() && !this->is_row_header();
}

#///////////////////////////////////////////////////////////////////////////////

template<typename State, typename Constraint, typename StateFullComparer,
		 typename ConstraintFullComparer>
bool DancingLink<State, Constraint, StateFullComparer,
				 ConstraintFullComparer>::Node::is_sole_ud() const {
	return this->u == this;
}

template<typename State, typename Constraint, typename StateFullComparer,
		 typename ConstraintFullComparer>
bool DancingLink<State, Constraint, StateFullComparer,
				 ConstraintFullComparer>::Node::is_sole_lr() const {
	return this->l == this;
}

#///////////////////////////////////////////////////////////////////////////////

template<typename State, typename Constraint, typename StateFullComparer,
		 typename ConstraintFullComparer>
DancingLink<State, Constraint, StateFullComparer,
			ConstraintFullComparer>::Node::Node(size_t col_index,
												size_t row_index):
	col_index(col_index),
	row_index(row_index), u(this), d(this), l(this), r(this) {}

#///////////////////////////////////////////////////////////////////////////////

template<typename State, typename Constraint, typename StateFullComparer,
		 typename ConstraintFullComparer>
void DancingLink<State, Constraint, StateFullComparer,
				 ConstraintFullComparer>::Node::PushU(Node* node) {
	this->PopUD();
	LinkUD_(this->u, node);
	LinkUD_(node, this);
}

template<typename State, typename Constraint, typename StateFullComparer,
		 typename ConstraintFullComparer>
void DancingLink<State, Constraint, StateFullComparer,
				 ConstraintFullComparer>::Node::PushD(Node* node) {
	this->PopUD();
	LinkUD_(node, this->d);
	LinkUD_(this, node);
}

template<typename State, typename Constraint, typename StateFullComparer,
		 typename ConstraintFullComparer>
void DancingLink<State, Constraint, StateFullComparer,
				 ConstraintFullComparer>::Node::PushL(Node* node) {
	this->PopLR();
	LinkLR_(this->l, node);
	LinkLR_(node, this);
}

template<typename State, typename Constraint, typename StateFullComparer,
		 typename ConstraintFullComparer>
void DancingLink<State, Constraint, StateFullComparer,
				 ConstraintFullComparer>::Node::PushR(Node* node) {
	this->PopLR();
	LinkLR_(node, this->r);
	LinkLR_(this, node);
}

#///////////////////////////////////////////////////////////////////////////////

template<typename State, typename Constraint, typename StateFullComparer,
		 typename ConstraintFullComparer>
void DancingLink<State, Constraint, StateFullComparer,
				 ConstraintFullComparer>::Node::PopUD() {
	LinkUD_(this->u, this->d);
	this->u = this->d = nullptr;
}

template<typename State, typename Constraint, typename StateFullComparer,
		 typename ConstraintFullComparer>
void DancingLink<State, Constraint, StateFullComparer,
				 ConstraintFullComparer>::Node::PopLR() {
	LinkUD_(this->l, this->r);
	this->l = this->r = nullptr;
}

template<typename State, typename Constraint, typename StateFullComparer,
		 typename ConstraintFullComparer>
void DancingLink<State, Constraint, StateFullComparer,
				 ConstraintFullComparer>::Node::Pop() {
	this->PopUD();
	this->PopLR();
}

#///////////////////////////////////////////////////////////////////////////////

template<typename State, typename Constraint, typename StateFullComparer,
		 typename ConstraintFullComparer>
void DancingLink<State, Constraint, StateFullComparer,
				 ConstraintFullComparer>::Node::CoverUD() {
	LinkUD_(this->u, this->d);
}

template<typename State, typename Constraint, typename StateFullComparer,
		 typename ConstraintFullComparer>
void DancingLink<State, Constraint, StateFullComparer,
				 ConstraintFullComparer>::Node::CoverLR() {
	LinkLR_(this->l, this->r);
}

#///////////////////////////////////////////////////////////////////////////////

template<typename State, typename Constraint, typename StateFullComparer,
		 typename ConstraintFullComparer>
void DancingLink<State, Constraint, StateFullComparer,
				 ConstraintFullComparer>::Node::UnCoverUD() {
	LinkUD_(this->u, this);
	LinkUD_(this, this->d);
}

template<typename State, typename Constraint, typename StateFullComparer,
		 typename ConstraintFullComparer>
void DancingLink<State, Constraint, StateFullComparer,
				 ConstraintFullComparer>::Node::UnCoverLR() {
	LinkLR_(this->l, this);
	LinkLR_(this, this->r);
}

#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////

template<typename State, typename Constraint, typename StateFullComparer,
		 typename ConstraintFullComparer>
template<typename StateFullComparerInitializeArg>
DancingLink<State, Constraint, StateFullComparer, ConstraintFullComparer>::
	StateAgentFullComparer_::StateAgentFullComparer_(
		StateFullComparerInitializeArg&& state_full_cmper_init_arg):
	state_full_cmper(
		Forward<StateFullComparerInitializeArg>(state_full_cmper_init_arg)) {}

template<typename State, typename Constraint, typename StateFullComparer,
		 typename ConstraintFullComparer>
int DancingLink<State, Constraint, StateFullComparer, ConstraintFullComparer>::
	StateAgentFullComparer_::operator()(const StateAgent& a,
										const StateAgent& b) const {
	return this->state_full_cmper(a.state, b.state);
}

#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////

template<typename State, typename Constraint, typename StateFullComparer,
		 typename ConstraintFullComparer>
size_t DancingLink<State, Constraint, StateFullComparer,
				   ConstraintFullComparer>::state_num() const {
	return this->state_index.size();
}

template<typename State, typename Constraint, typename StateFullComparer,
		 typename ConstraintFullComparer>
size_t DancingLink<State, Constraint, StateFullComparer,
				   ConstraintFullComparer>::nec_cst_num() const {
	return this->nec_cst.size();
}

template<typename State, typename Constraint, typename StateFullComparer,
		 typename ConstraintFullComparer>
size_t DancingLink<State, Constraint, StateFullComparer,
				   ConstraintFullComparer>::opt_cst_num() const {
	return this->opt_cst.size();
}

template<typename State, typename Constraint, typename StateFullComparer,
		 typename ConstraintFullComparer>
size_t DancingLink<State, Constraint, StateFullComparer,
				   ConstraintFullComparer>::cst_num() const {
	return this->nec_cst_num() + this->opt_cst_num();
}

template<typename State, typename Constraint, typename StateFullComparer,
		 typename ConstraintFullComparer>
StateFullComparer& DancingLink<State, Constraint, StateFullComparer,
							   ConstraintFullComparer>::state_full_cmper() {
	return this->state_index_.full_cmper();
}

template<typename State, typename Constraint, typename StateFullComparer,
		 typename ConstraintFullComparer>
const StateFullComparer&
DancingLink<State, Constraint, StateFullComparer,
			ConstraintFullComparer>::state_full_cmper() const {
	return this->state_index_.full_cmper();
}

template<typename State, typename Constraint, typename StateFullComparer,
		 typename ConstraintFullComparer>
ConstraintFullComparer& DancingLink<State, Constraint, StateFullComparer,
									ConstraintFullComparer>::cst_full_cmper() {
	return this->nec_cst_index_.full_cmper();
}

template<typename State, typename Constraint, typename StateFullComparer,
		 typename ConstraintFullComparer>
const ConstraintFullComparer&
DancingLink<State, Constraint, StateFullComparer,
			ConstraintFullComparer>::cst_full_cmper() const {
	return this->nec_cst_index_.full_cmper();
}

template<typename State, typename Constraint, typename StateFullComparer,
		 typename ConstraintFullComparer>
bool DancingLink<State, Constraint, StateFullComparer,
				 ConstraintFullComparer>::solve_ready() const {
	return this->solve_ready_;
}

#///////////////////////////////////////////////////////////////////////////////

template<typename State, typename Constraint, typename StateFullComparer,
		 typename ConstraintFullComparer>
template<typename StateFullComparerInitializeArg,
		 typename ConstraintFullComparerInitializeArg>
DancingLink<State, Constraint, StateFullComparer, ConstraintFullComparer>::
	DancingLink(StateFullComparerInitializeArg&& state_full_cmper_init_arg,
				ConstraintFullComparerInitializeArg&& cst_full_cmper_init_arg):
	state_index_(
		Forward<StateFullComparerInitializeArg>(state_full_cmper_init_arg)),
	nec_cst_index_(
		Forward<ConstraintFullComparerInitializeArg>(cst_full_cmper_init_arg)),
	opt_cst_index_(
		Forward<ConstraintFullComparerInitializeArg>(cst_full_cmper_init_arg)),
	root_(nullptr), is_ready_to_solve(false) {
	this->state_agent_full_cmper_.state_full_cmper = &this->state_full_cmper_;
}

#///////////////////////////////////////////////////////////////////////////////

template<typename State, typename Constraint, typename StateFullComparer,
		 typename ConstraintFullComparer>
template<typename NecCstCntrIterator, typename OptCstCntrIterator>
void DancingLink<State, Constraint, StateFullComparer, ConstraintFullComparer>::
	AddState(const State& state, NecCstCntrIterator nec_cst_begin,
			 NecCstCntrIterator nec_cst_end, OptCstCntrIterator opt_cst_begin,
			 OptCstCntrIterator opt_cst_end) {
	bool state_duplicate(
		!this->state_to_index_->Insert(state, this->state_to_index->size())
			 .second);

	PHI__debug_if(state_duplicate) {
		PHI__throw(, __func__, "state duplicate");
	}

	StateInfo& state_info(this->state_info_.Insert(state).first.second);

	for (; nec_cst_begin != nec_cst_end; ++nec_cst_begin) {
		this->nec_cst_index.Insert(*nec_cst_begin);
		state_info.nec_cst.Insert(*nec_cst_begin);
	}

	for (; opt_cst_begin != opt_cst_end; ++opt_cst_begin) {
		this->opt_cst_index_.Insert(*opt_cst_begin);
		state_info.opt_cst.Insert(*opt_cst_begin);
	}

	this->solve_ready_ = false;
}

#///////////////////////////////////////////////////////////////////////////////

template<typename State, typename Constraint, typename StateFullComparer,
		 typename ConstraintFullComparer>
void DancingLink<State, Constraint, StateFullComparer,
				 ConstraintFullComparer>::SomeAmazingAlgToOptimizeIndex() {
	// no
}

#///////////////////////////////////////////////////////////////////////////////

template<typename State, typename Constraint, typename StateFullComparer,
		 typename ConstraintFullComparer>
void DancingLink<State, Constraint, StateFullComparer,
				 ConstraintFullComparer>::Build() {
	if (this->solve_ready_) { return; }

	this->SomeAmazingAlgToOptimizeIndex();

	size_t col_size(this->state_index.size());
	size_t row_size_a(this->nec_cst_index.size());
	size_t row_size_b(this->opt_cst_index.size());
	size_t row_size(row_size_a + row_size_b);

	PHI__debug_if(col_size == 0 || row_size == 0) {
		PHI__throw(, __func__, "error");
	}

	this->root_ = new (this->pool_.Pop()) Node(col_size, row_size);
	root->col_header = root;
	root->row_header = root;

	for (size_t col_index(0); col_index != col_size; ++col_index) {
		Node* node(new Node(col_index, col_size));
		node->col_header = node;
		node->row_header = this->root_;
		this->root_->PushU(node);
	}

	for (size_t row_index(0); row_index != row_size_a; ++row_index) {
		Node* node(new Node(row_size, row_index));
		node->col_header = this->root_;
		node->row_header = node;
		this->root_->PushL(node);
	}

	Node* opt_cst_begin_row_header(this->root_->l);

	for (size_t row_index(row_size_b); row_index != row_size; ++row_index) {
		Node* node(new Node(row_size, row_index));
		node->col_header = this->root_;
		node->row_header = node;
		this->root_->PushL(node);
	}

	opt_cst_begin_row_header = opt_cst_begin_row_header->r;

	cntr::Vector<size_t> cst_temp;
	auto state_iter(this->state_index_.first_iterator());
	Node* col_header(this->root_->d);

	for (size_t col_index(0); col_index != col_size;
		 ++col_index, ++state_iter) {
		{
			Node* row_header(this->root_->r);

			auto iter(state_iter->second.nec_cst.first_iterator());
			auto end(state_iter->second.nec_cst.null_iterator());

			Sort(iter, end);

			for (; iter != end; row_header = row_header->r) {
				if (*iter == row_header->row_index) {
					Node* node(new (this->pool_.Pop()) Node(
						col_header->col_index, row_header->row_index));
					node->col_header = col_header;
					node->col_header = row_header;
					col_header->PushL(node);
					row_header->PushR(node);
					++iter;
				}
			}
		}

		{ Node* row_header(opt_cst_begin_row_header); }

		Node* row_header(root->r);
	}
}

template<typename State, typename Constraint, typename StateFullComparer,
		 typename ConstraintFullComparer>
void DancingLink<State, Constraint, StateFullComparer,
				 ConstraintFullComparer>::Release() {
	while (!this->root_->is_sole_up()) {
		Node* col(this->root_->d);

		while (!col->is_sole_lr()) {
			Node* i(col->r);
			i->Pop();
			this->pool_->Push(i);
		}

		col->Pop();
		this->pool_->Push(col);
	}

	while (!this->roor->is_sole_lr()) {
		Node* i(this->root_->r);
		i->Pop();
		this->pool_->Push(i);
	}

	this->pool_->Push(this->root_);
	this->root_ = nullptr;

	this->solve_ready_ = false;
}

template<typename State, typename Constraint, typename StateFullComparer,
		 typename ConstraintFullComparer>
template<typename Receiver>
void DancingLink<State, Constraint, StateFullComparer,
				 ConstraintFullComparer>::Solve(Receiver& receiver) {
	this->Build();
}

#///////////////////////////////////////////////////////////////////////////////
}
}

#endif
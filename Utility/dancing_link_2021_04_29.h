#ifndef PHI__define_guard__Utility__dancing_linking_h
#define PHI__define_guard__Utility__dancing_linking_h

#include "sort2.h"
#include "../Container/Vector.h"
#include "../Container/Set.h"
#include "../Container/Map.h"
#include "../Container/Pool.h"
#include <iostream>

#define PHI__throw__local(desc) PHI__throw(DancingLink, __func__, desc);

namespace phi {

template<typename NecOptJudger> class DancingLink {
public:
	struct State {
		size_t state_id;

		cntr::Set<size_t> nec_cst;
		cntr::Set<size_t> opt_cst;
	};

	struct StateFullComparer_ {
		int operator()(const State& a, const State& b) const;
		int operator()(const State& a, size_t b) const;
		int operator()(size_t a, const State& b) const;
		int operator()(size_t a, size_t b) const;
	};

	using StateFullComparer = AutoImplementFullComparer<StateFullComparer_>;

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

		Node(size_t col_index, size_t row_index);

		bool is_root() const;
		bool is_col_header() const;
		bool is_row_header() const;
		bool is_normal() const;

		bool is_sole_ud() const;
		bool is_sole_lr() const;

		void PushU(Node* node);
		void PushD(Node* node);
		void PushL(Node* node);
		void PushR(Node* node);

		void PopUD();
		void PopLR();
		void Pop();

		void CoverUD();
		void CoverLR();

		void UnCoverUD();
		void UnCoverLR();

	protected:
		void LinkUD_(Node* x, Node* y);
		void LinkLR_(Node* x, Node* y);
	};

#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////

	size_t state_size() const;
	size_t nec_cst_size() const;
	size_t opt_cst_size() const;

	NecOptJudger& nec_opt_judger();
	const NecOptJudger& nec_opt_judger() const;

#///////////////////////////////////////////////////////////////////////////////

	template<typename... NecOptJudgerConstructArgs>
	DancingLink(NecOptJudgerConstructArgs&&... nec_opt_judger_construct_args);

	~DancingLink();

#///////////////////////////////////////////////////////////////////////////////

	void AddHot(size_t state_index, size_t cst_index);
	void AddNecHot(size_t state_index, size_t cst_index);
	void AddOptHot(size_t state_index, size_t cst_index);

#///////////////////////////////////////////////////////////////////////////////

	void Build();
	void Release();

	template<typename Receiver> void Solve(Receiver& receiver);

#///////////////////////////////////////////////////////////////////////////////

	void Print() const;

	static void Print(Node* root);

private:
	cntr::Set<State, StateFullComparer> state_;
	cntr::Set<size_t> nec_cst_;
	cntr::Set<size_t> opt_cst_;

	NecOptJudger nec_opt_judger_;

	bool solve_ready_;

	Node* root_;

	cntr::UncountedPool<sizeof(Node)> pool_;

	template<typename Receiver>
	bool Solve_(cntr::Vector<size_t>& dst, Node* col_header_i,
				size_t nec_cst_size, Receiver& receiver);
};

#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////

template<typename NecOptJudger>
int DancingLink<NecOptJudger>::StateFullComparer_::operator()(
	const State& a, const State& b) const {
	return this->operator()(a.state_id, b.state_id);
}

template<typename NecOptJudger>
int DancingLink<NecOptJudger>::StateFullComparer_::operator()(const State& a,
															  size_t b) const {
	return this->operator()(a.state_id, b);
}

template<typename NecOptJudger>
int DancingLink<NecOptJudger>::StateFullComparer_::operator()(
	size_t a, const State& b) const {
	return this->operator()(a, b.state_id);
}

template<typename NecOptJudger>
int DancingLink<NecOptJudger>::StateFullComparer_::operator()(size_t a,
															  size_t b) const {
	if (a < b) { return -1; }
	if (b < a) { return 1; }
	return 0;
}

#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////

template<typename NecOptJudger>
void DancingLink<NecOptJudger>::Node::LinkUD_(Node* x, Node* y) {
	x->d = y;
	y->u = x;
}

template<typename NecOptJudger>
void DancingLink<NecOptJudger>::Node::LinkLR_(Node* x, Node* y) {
	x->r = y;
	y->l = x;
}

template<typename NecOptJudger>
bool DancingLink<NecOptJudger>::Node::is_root() const {
	return this->is_col_header() && this->is_row_header();
}

template<typename NecOptJudger>
bool DancingLink<NecOptJudger>::Node::is_col_header() const {
	return this == this->col_header;
}

template<typename NecOptJudger>
bool DancingLink<NecOptJudger>::Node::is_row_header() const {
	return this == this->row_header;
}

template<typename NecOptJudger>
bool DancingLink<NecOptJudger>::Node::is_normal() const {
	return !this->is_col_header() && !this->is_row_header();
}

template<typename NecOptJudger>
bool DancingLink<NecOptJudger>::Node::is_sole_ud() const {
	return this->u == this;
}

template<typename NecOptJudger>
bool DancingLink<NecOptJudger>::Node::is_sole_lr() const {
	return this->l == this;
}

template<typename NecOptJudger>
DancingLink<NecOptJudger>::Node::Node(size_t col_index, size_t row_index):
	col_index(col_index), row_index(row_index), u(this), d(this), l(this),
	r(this) {}

template<typename NecOptJudger>
void DancingLink<NecOptJudger>::Node::PushU(Node* node) {
	node->PopUD();
	LinkUD_(this->u, node);
	LinkUD_(node, this);
}

template<typename NecOptJudger>
void DancingLink<NecOptJudger>::Node::PushD(Node* node) {
	node->PopUD();
	LinkUD_(node, this->d);
	LinkUD_(this, node);
}

template<typename NecOptJudger>
void DancingLink<NecOptJudger>::Node::PushL(Node* node) {
	node->PopLR();
	LinkLR_(this->l, node);
	LinkLR_(node, this);
}

template<typename NecOptJudger>
void DancingLink<NecOptJudger>::Node::PushR(Node* node) {
	node->PopLR();
	LinkLR_(node, this->r);
	LinkLR_(this, node);
}

template<typename NecOptJudger> void DancingLink<NecOptJudger>::Node::PopUD() {
	LinkUD_(this->u, this->d);
	this->u = this->d = this;
}

template<typename NecOptJudger> void DancingLink<NecOptJudger>::Node::PopLR() {
	LinkLR_(this->l, this->r);
	this->l = this->r = this;
}

template<typename NecOptJudger> void DancingLink<NecOptJudger>::Node::Pop() {
	this->PopUD();
	this->PopLR();
}

template<typename NecOptJudger>
void DancingLink<NecOptJudger>::Node::CoverUD() {
	LinkUD_(this->u, this->d);
}

template<typename NecOptJudger>
void DancingLink<NecOptJudger>::Node::CoverLR() {
	LinkLR_(this->l, this->r);
}

template<typename NecOptJudger>
void DancingLink<NecOptJudger>::Node::UnCoverUD() {
	LinkUD_(this->u, this);
	LinkUD_(this, this->d);
}

template<typename NecOptJudger>
void DancingLink<NecOptJudger>::Node::UnCoverLR() {
	LinkLR_(this->l, this);
	LinkLR_(this, this->r);
}

#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////

template<typename NecOptJudger>
size_t DancingLink<NecOptJudger>::state_size() const {
	return this->data_.size();
}

template<typename NecOptJudger>
size_t DancingLink<NecOptJudger>::nec_cst_size() const {
	return this->nec_cst_.size();
}

template<typename NecOptJudger>
size_t DancingLink<NecOptJudger>::opt_cst_size() const {
	return this->opt_cst_.size();
}

template<typename NecOptJudger>
NecOptJudger& DancingLink<NecOptJudger>::nec_opt_judger() {
	return this->nec_opt_judger_;
}

template<typename NecOptJudger>
const NecOptJudger& DancingLink<NecOptJudger>::nec_opt_judger() const {
	return this->nec_opt_judger_;
}

#///////////////////////////////////////////////////////////////////////////////

template<typename NecOptJudger>
template<typename... NecOptJudgerConstructArgs>
DancingLink<NecOptJudger>::DancingLink(
	NecOptJudgerConstructArgs&&... nec_opt_judger_construct_args):
	nec_opt_judger_(
		Forward<NecOptJudgerConstructArgs>(nec_opt_judger_construct_args)...),
	root_(nullptr), solve_ready_(false) {}

template<typename NecOptJudger> DancingLink<NecOptJudger>::~DancingLink() {
	this->Release();
}

#///////////////////////////////////////////////////////////////////////////////

template<typename NecOptJudger>
void DancingLink<NecOptJudger>::AddHot(size_t state_id, size_t cst_id) {
	auto state_iter(this->state_iter.Find(state_id));

	if (state_iter == this->state_iter.end()) {
		state_iter = this->state_iter.Insert(state_id).first;
		state_iter->second.state_id = state_id;
	}

	if (this->nec_opt_judger_(cst_id)) {
		state_iter->second.nec_cst.Insert(cst_id);
		this->nec_cst_.Insert(cst_id);
	} else {
		state_iter->second.opt_cst.Insert(cst_id);
		this->opt_cst_.Insert(cst_id);
	}
}

template<typename NecOptJudger>
void DancingLink<NecOptJudger>::AddNecHot(size_t state_id, size_t cst_id) {
	PHI__debug_if(!this->nec_opt_judger_(cst_id)) {
		PHI__throw__local("cst type error");
	}

	auto state_iter(this->state_iter.Find(state_id));

	if (state_iter == this->state_iter.end()) {
		state_iter = this->state_iter.Insert(state_id).first;
		state_iter->second.state_id = state_id;
	}

	state_iter->second.nec_cst.Insert(cst_id);
	this->nec_cst_.Insert(cst_id);
}

template<typename NecOptJudger>
void DancingLink<NecOptJudger>::AddOptHot(size_t state_id, size_t cst_id) {
	PHI__debug_if(this->nec_opt_judger_(cst_id)) {
		PHI__throw__local("cst type error");
	}

	auto state_iter(this->state_iter.Find(state_id));

	if (state_iter == this->state_iter.end()) {
		state_iter = this->state_iter.Insert(state_id).first;
		state_iter->second.state_id = state_id;
	}

	state_iter->second.opt_cst.Insert(cst_id);
	this->opt_cst_.Insert(cst_id);
}

#///////////////////////////////////////////////////////////////////////////////

template<typename NecOptJudger> void DancingLink<NecOptJudger>::Build() {
	if (this->solve_ready_) { return; }
	this->Release();

	size_t node_count(0);

	PHI__interrupt;

	size_t col_size(this->data_.size());
	size_t row_size(this->nec_cst_.size() + this->opt_cst_.size());

	this->root_ = new (this->pool_.Pop()) Node(col_size, row_size);
	this->root_->col_header = this->root_;
	this->root_->row_header = this->root_;

	++node_count;

	for (size_t col_index(0); col_index != this->state_size_; ++col_index) {
		Node* node(new (this->pool_.Pop()) Node(col_index, row_size));
		node->col_header = node;
		node->row_header = this->root_;
		this->root_->PushU(node);
		++node_count;
	}

	{
		auto nec_cst_iter(this->nec_cst_.first_iterator());
		auto nec_cst_end(this->nec_cst_.null_iterator());

		// for(;nec_cst_iter != nec_cst_end;)

		for (size_t row_index(0); row_index != row_size; ++row_index) {
			Node* node(new (this->pool_.Pop()) Node(col_size, row_index));
			node->col_header = this->root_;
			node->row_header = node;
			this->root_->PushL(node);
			++node_count;
		}
	}

	Node* col_header(this->root_->d);

	auto state_iter(this->data_.first_iterator());
	auto state_end(this->data_.end_iterator());

	for (; state_iter != state_end; col_header = col_header->d, ++state_iter) {
		Node* row_header(this->root_->r);

		for (size_t i(0); i != state_iter->second.size();
			 row_header = row_header->r) {
			if (row_header->row_index == state_iter->second[i]) {
				Node* node(new Node(state_iter->first, state_iter->second[i]));
				node->col_header = col_header;
				node->row_header = row_header;
				col_header->PushL(node);
				row_header->PushU(node);
				++i;
				++node_count;
			}
		}
	}

	PHI__print_value(node_count);

	this->solve_ready_ = true;
}

template<typename NecOptJudger> void DancingLink<NecOptJudger>::Release() {
	if (this->root_ == nullptr) { return; }

	size_t node_count(0);

	while (!this->root_->is_sole_ud()) {
		Node* col_header(this->root_->d);

		while (!col_header->is_sole_lr()) {
			Node* i(col_header->r);
			i->Pop();
			this->pool_.Push(i);
			++node_count;
		}

		col_header->Pop();
		this->pool_.Push(col_header);
		++node_count;
	}

	while (!this->root_->is_sole_lr()) {
		Node* row_header(this->root_->r);
		row_header->Pop();
		this->pool_.Push(row_header);
		++node_count;
	}

	this->pool_.Push(this->root_);
	this->root_ = nullptr;
	++node_count;

	this->solve_ready_ = false;

	PHI__print_value(node_count);
}

#///////////////////////////////////////////////////////////////////////////////

template<typename NecOptJudger>
template<typename Receiver>
void DancingLink<NecOptJudger>::Solve(Receiver& receiver) {
	this->Build();
	cntr::Vector<size_t> dst;
	Solve_(dst, this->root_->d, this->nec_cst_size_, receiver);
}

template<typename NecOptJudger>
template<typename Receiver>
bool DancingLink<NecOptJudger>::Solve_(cntr::Vector<size_t>& dst,
									   Node* col_header_i, size_t nec_cst_size,
									   Receiver& receiver) {
	if (nec_cst_size == 0) {
		return receiver(dst.first_const_iterator(), dst.null_const_iterator());
	}

	for (; col_header_i != this->root_; col_header_i = col_header_i->d) {
		for (Node* row(col_header_i->r); row != col_header_i; row = row->r) {
			if (row->row_index < this->nec_cst_size_) { --nec_cst_size; }
			Node* i(row->d);

			for (; !i->is_row_header(); i = i->d) {
				for (Node* j(i->r); j != i; j = j->r) { j->CoverUD(); }
			}

			i->CoverLR();
		}

		dst.Push(col_header_i->col_index);
		bool solve_continue(
			Solve_(dst, col_header_i->d, nec_cst_size, receiver));
		dst.Pop();

		for (Node* row(col_header_i->l); row != col_header_i; row = row->l) {
			if (row->row_index < this->nec_cst_size_) { ++nec_cst_size; }
			Node* i(row->row_header);
			i->UnCoverLR();

			while ((i = i->u) != row) {
				for (Node* j(i->l); j != i; j = j->l) { j->UnCoverUD(); }
			}
		}

		if (!solve_continue) { return false; }
	}

	return true;
}

#///////////////////////////////////////////////////////////////////////////////

template<typename NecOptJudger> void DancingLink<NecOptJudger>::Print() const {
	Print(this->root_);
}

template<typename NecOptJudger>
void DancingLink<NecOptJudger>::Print(Node* root) {
	size_t col_size(root->u->col_index + 1);
	size_t row_size(root->l->row_index + 1);

	::printf("    ");
	for (size_t i(0); i != row_size; ++i) { ::printf("%4ld", i); }
	::printf("\n");

	Node* col_header(root->d);

	for (; col_header != root; col_header = col_header->d) {
		::printf("%4ld", col_header->col_index);
		Node* node(col_header->r);

		size_t row_index(0);

		for (; node != col_header; ++row_index) {
			if (row_index == node->row_index) {
				::printf("   O");
				node = node->r;
			} else {
				::printf("   -");
			}
		}

		for (; row_index != row_size; ++row_index) { ::printf("   -"); }

		::printf("\n");
	}
}

}

#endif
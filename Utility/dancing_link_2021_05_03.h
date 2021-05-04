#ifndef PHI__define_guard__Utility__dancing_linking_h
#define PHI__define_guard__Utility__dancing_linking_h

#include "sort2.h"
#include "../Container/Vector.h"
#include "../Container/Set.h"
#include "../Container/Pool.h"
#include <iostream>

#define PHI__throw__local(desc) PHI__throw(DancingLink, __func__, desc);

namespace phi {

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
#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////

	size_t state_num() const;
	size_t nec_cst_num() const;
	size_t opt_cst_num() const;
	size_t cst_num() const;

#///////////////////////////////////////////////////////////////////////////////

	DancingLink(size_t state_num, size_t nec_cst_num, size_t opt_cst_num);

	~DancingLink();

#///////////////////////////////////////////////////////////////////////////////

	void AddHot(size_t state_index, size_t cst_index);

#///////////////////////////////////////////////////////////////////////////////

	void Build();
	void Release();

	template<typename Receiver> void Solve(Receiver& receiver);

#///////////////////////////////////////////////////////////////////////////////

	void Print() const;

	static void Print(Node* root);

private:
	size_t state_num_;
	size_t nec_cst_num_;
	size_t opt_cst_num_;

	cntr::Vector<cntr::Vector<size_t>> data_;

	bool solve_ready_;

	Node* root_;

	cntr::UncountedPool<sizeof(Node)> pool_;

	void EnPool_(Node* node);

	void PopState_(Node* col);

	void ChoosePop_(Node* node, size_t& nec_cst_num);
	void ChooseCover_(Node* node);
	void ChooseUnCover_(Node* node);

	void ChooseCoverD_(Node* node);
	void ChooseUnCoverD_(Node* node);

	void Optimize_();

	template<typename Receiver>
	bool Solve_(cntr::Vector<size_t>& dst, Node* col_header_i,
				size_t nec_cst_num, Receiver& receiver);
};

#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////

void DancingLink::Node::LinkUD_(Node* x, Node* y) {
	x->d = y;
	y->u = x;
}

void DancingLink::Node::LinkLR_(Node* x, Node* y) {
	x->r = y;
	y->l = x;
}

bool DancingLink::Node::is_root() const {
	return this->is_col_header() && this->is_row_header();
}
bool DancingLink::Node::is_col_header() const {
	return this == this->col_header;
}
bool DancingLink::Node::is_row_header() const {
	return this == this->row_header;
}
bool DancingLink::Node::is_normal() const {
	return !this->is_col_header() && !this->is_row_header();
}

bool DancingLink::Node::is_sole_ud() const { return this->u == this; }
bool DancingLink::Node::is_sole_lr() const { return this->l == this; }

DancingLink::Node::Node(size_t col_index, size_t row_index):
	col_index(col_index), row_index(row_index), u(this), d(this), l(this),
	r(this) {}

void DancingLink::Node::PushU(Node* node) {
	node->PopUD();
	LinkUD_(this->u, node);
	LinkUD_(node, this);
}

void DancingLink::Node::PushD(Node* node) {
	node->PopUD();
	LinkUD_(node, this->d);
	LinkUD_(this, node);
}

void DancingLink::Node::PushL(Node* node) {
	node->PopLR();
	LinkLR_(this->l, node);
	LinkLR_(node, this);
}

void DancingLink::Node::PushR(Node* node) {
	node->PopLR();
	LinkLR_(node, this->r);
	LinkLR_(this, node);
}

void DancingLink::Node::PopUD() {
	LinkUD_(this->u, this->d);
	this->u = this->d = this;
}

void DancingLink::Node::PopLR() {
	LinkLR_(this->l, this->r);
	this->l = this->r = this;
}

void DancingLink::Node::Pop() {
	this->PopUD();
	this->PopLR();
}

void DancingLink::Node::CoverUD() { LinkUD_(this->u, this->d); }
void DancingLink::Node::CoverLR() { LinkLR_(this->l, this->r); }

void DancingLink::Node::UnCoverUD() {
	LinkUD_(this->u, this);
	LinkUD_(this, this->d);
}

void DancingLink::Node::UnCoverLR() {
	LinkLR_(this->l, this);
	LinkLR_(this, this->r);
}

#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////

size_t DancingLink::state_num() const { return this->state_num_; }
size_t DancingLink::nec_cst_num() const { return this->nec_cst_num_; }
size_t DancingLink::opt_cst_num() const { return this->opt_cst_num_; }

#///////////////////////////////////////////////////////////////////////////////

DancingLink::DancingLink(size_t state_num, size_t nec_cst_num,
						 size_t opt_cst_num):
	state_num_(state_num),
	nec_cst_num_(nec_cst_num), opt_cst_num_(opt_cst_num), root_(nullptr),
	solve_ready_(false) {
	for (size_t i(0); i != state_num; ++i) { this->data_.Push(); }
}

DancingLink::~DancingLink() { this->Release(); }

#///////////////////////////////////////////////////////////////////////////////

void DancingLink::AddHot(size_t state_index, size_t cst_index) {
	PHI__debug_if(this->state_num_ <= state_index ||
				  (this->nec_cst_num_ + this->opt_cst_num_) <= cst_index) {
		PHI__throw__local("error");
	}

	cntr::Vector<size_t>& cst(this->data_[state_index]);

	auto begin(cst.first_iterator());

	if (!ContainBinary(begin, cst.null_iterator(), cst_index)) {
		cst.Push(cst_index);
		LinearInsert(begin, cst.null_iterator());
	}
}

#///////////////////////////////////////////////////////////////////////////////

void DancingLink::Build() {
	if (this->solve_ready_) { return; }
	this->Release();

	size_t col_num(this->state_num_);
	size_t row_num(this->nec_cst_num_ + this->opt_cst_num_);

	this->root_ = new (this->pool_.Pop()) Node(col_num, row_num);
	this->root_->col_header = this->root_;
	this->root_->row_header = this->root_;

	for (size_t col_index(0); col_index != this->state_num_; ++col_index) {
		Node* node(new (this->pool_.Pop()) Node(col_index, row_num));
		node->col_header = node;
		node->row_header = this->root_;
		this->root_->PushU(node);
	}

	for (size_t row_index(0); row_index != row_num; ++row_index) {
		Node* node(new (this->pool_.Pop()) Node(col_num, row_index));
		node->col_header = this->root_;
		node->row_header = node;
		this->root_->PushL(node);
	}

	Node* col_header(this->root_->d);

	for (size_t col_index(0); col_index != col_num;
		 ++col_index, col_header = col_header->d) {
		Node* row_header(this->root_->r);

		for (size_t i(0); i != this->data_[col_index].size();
			 row_header = row_header->r) {
			if (row_header->row_index == this->data_[col_index][i]) {
				Node* node(new Node(col_index, this->data_[col_index][i]));
				node->col_header = col_header;
				node->row_header = row_header;
				col_header->PushL(node);
				row_header->PushU(node);
				++i;
			}
		}
	}

	this->solve_ready_ = true;
}

void DancingLink::Release() {
	if (this->root_ == nullptr) { return; }

	while (!this->root_->is_sole_ud()) {
		Node* col_header(this->root_->d);

		while (!col_header->is_sole_lr()) {
			Node* i(col_header->r);
			i->Pop();
			this->pool_.Push(i);
		}

		col_header->Pop();
		this->pool_.Push(col_header);
	}

	while (!this->root_->is_sole_lr()) {
		Node* row_header(this->root_->r);
		row_header->Pop();
		this->pool_.Push(row_header);
	}

	this->pool_.Push(this->root_);
	this->root_ = nullptr;

	this->solve_ready_ = false;
}

#///////////////////////////////////////////////////////////////////////////////

void DancingLink::Optimize_() {
	cntr::Vector<Node*> ccc;

	Node* row_header(this->root_->r);

	for (; row_header != this->root_; ++row_header) {}
}

void DancingLink::EnPool_(Node* node) {
	node->Pop();
	this->pool_.Push(node);
}

void DancingLink::PopState_(Node* col) {
	col = col->col_header;
	while (!col->is_sole_lr()) { this->EnPool_(col->r); }
	this->EnPool_(col);
}

void DancingLink::ChoosePop_(Node* node, size_t& nec_cst_num) {
	node = node->col_header;

	while (!node->is_sole_lr()) {
		Node* row(node->r);
		if (row->row_index < this->nec_cst_num_) { --nec_cst_num; }
		while (row->d != row->row_header) { PopState_(row->d); }
		while (row->u != row->row_header) { PopState_(row->u); }
		this->EnPool_(row->u);
		this->EnPool_(row);
	}

	this->EnPool_(node);
}
/*
void DancingLink::ChooseCoverD_(Node* node, size_t& nec_cst_num) {
	size_t nec_cst_num(0);

	node = node->col_header;

	for (Node* row(node->r); row != node; row = row->r) {
		if (row->row_index < this->nec_cst_num_) { --nec_cst_num; }
		Node* i(row->d);

		for (; !i->is_row_header(); i = i->d) {
			for (Node* j(i->r); j != i; j = j->r) { j->CoverUD(); }
		}

		i->CoverLR();
	}
}*/

template<typename Receiver> void DancingLink::Solve(Receiver& receiver) {
	this->Build();

	size_t nec_cst_num(this->nec_cst_num_);

	cntr::Vector<size_t> dst;

	/*cntr::Vector<Node*> covered_col;

	{
		cntr::Vector<Node*> vvv;
		cntr::Set<size_t> sss;

		Node* row_header(this->root_->r);

		for (; row_header != this->root_; ++row_header) {
			Node* i(row_header->d);

			for (; i != row_header; i = i->d) {
				vvv.Push(i);

				for (Node* j(i->col_header->r);j!=i->col_header;j = j->r){

				}
			}

			if (vvv.size() == 0) { return; }

			if (vvv.size() == 1) { this->ChoosePop_(ccc[0], nec_cst_num); }
		}
	}*/

	Solve_(dst, this->root_->d, nec_cst_num, receiver);
}

template<typename Receiver>
bool DancingLink::Solve_(cntr::Vector<size_t>& dst, Node* col_header_i,
						 size_t nec_cst_num, Receiver& receiver) {
	/*if (4 <= dst.size()) {
		PHI__print_value(dst[dst.size() - 4]);
		PHI__print_value(dst[dst.size() - 3]);
		PHI__print_value(dst[dst.size() - 2]);
		PHI__print_value(dst[dst.size() - 1]);
		PHI__interrupt;
	}*/

	if (nec_cst_num == 0) {
		return receiver(dst.first_const_iterator(), dst.null_const_iterator());
	}

	for (; col_header_i != this->root_; col_header_i = col_header_i->d) {
		bool determine_fail(false);

		for (Node* row(col_header_i->r); row != col_header_i; row = row->r) {
			if (row->row_index < this->nec_cst_num_) { --nec_cst_num; }
			Node* i(row->d);

			for (; !i->is_row_header(); i = i->d) {
				for (Node* j(i->r); j != i; j = j->r) {
					if (j->u == j->d) { determine_fail = true; }
					j->CoverUD();
				}
			}

			i->CoverLR();
		}

		dst.Push(col_header_i->col_index);
		bool solve_continue(determine_fail ? true
										   : Solve_(dst, col_header_i->d,
													nec_cst_num, receiver));
		dst.Pop();

		for (Node* row(col_header_i->l); row != col_header_i; row = row->l) {
			if (row->row_index < this->nec_cst_num_) { ++nec_cst_num; }
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

void DancingLink::Print() const { Print(this->root_); }

void DancingLink::Print(Node* root) {
	size_t col_num(root->u->col_index + 1);
	size_t row_num(root->l->row_index + 1);

	::printf("    ");
	for (size_t i(0); i != row_num; ++i) { ::printf("%4ld", i); }
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

		for (; row_index != row_num; ++row_index) { ::printf("   -"); }

		::printf("\n");
	}
}

}

#undef PHI__throw__local

#endif
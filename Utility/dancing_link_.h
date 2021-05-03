#ifndef PHI__define_guard__Utility__dancing_linking_h
#define PHI__define_guard__Utility__dancing_linking_h

#include "sort2.h"
#include "../Container/Vector.h"
#include "../Container/Map.h"
#include <iostream>

namespace phi {
namespace dancing_link {

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

void Node::LinkUD_(Node* x, Node* y) {
	x->d = y;
	y->u = x;
}

void Node::LinkLR_(Node* x, Node* y) {
	x->r = y;
	y->l = x;
}

bool Node::is_root() const {
	return this->is_col_header() && this->is_row_header();
}
bool Node::is_col_header() const { return this == this->col_header; }
bool Node::is_row_header() const { return this == this->row_header; }
bool Node::is_normal() const {
	return !this->is_col_header() && !this->is_row_header();
}

bool Node::is_sole_ud() const { return this->u == this; }
bool Node::is_sole_lr() const { return this->l == this; }

Node::Node(size_t col_index, size_t row_index):
	col_index(col_index), row_index(row_index), u(this), d(this), l(this),
	r(this) {}

void Node::PushU(Node* node) {
	LinkUD_(this->u, node);
	LinkUD_(node, this);
}

void Node::PushD(Node* node) {
	LinkUD_(node, this->d);
	LinkUD_(this, node);
}

void Node::PushL(Node* node) {
	LinkLR_(this->l, node);
	LinkLR_(node, this);
}

void Node::PushR(Node* node) {
	LinkLR_(node, this->r);
	LinkLR_(this, node);
}

void Node::PopUD() {
	LinkUD_(this->u, this->d);
	this->u = this->d = nullptr;
}

void Node::PopLR() {
	LinkUD_(this->l, this->r);
	this->l = this->r = nullptr;
}

void Node::Pop() {
	this->PopUD();
	this->PopLR();
}

void Node::CoverUD() { LinkUD_(this->u, this->d); }
void Node::CoverLR() { LinkLR_(this->l, this->r); }

void Node::UnCoverUD() {
	LinkUD_(this->u, this);
	LinkUD_(this, this->d);
}

void Node::UnCoverLR() {
	LinkLR_(this->l, this);
	LinkLR_(this, this->r);
}

#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////

template<typename ID> class DancingLink {
public:
	cntr::Vector<cntr::Vector<ID>> nec_cst;
	cntr::Map<ID, size_t> nec_cst_index;

	cntr::Vector<cntr::Vector<ID>> opt_cst;
	cntr::Map<ID, size_t> opt_cst_index;

	size_t col_size;
	size_t row_size;
	size_t nec_cst_size;
	size_t opt_sct_size;

	Node* root;
};

#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////

inline Node*
ConstructDancingLink(const cntr::Vector<cntr::Vector<bool>>& incidence_matrix);

Node* ConstructDancingLink(
	const cntr::Vector<cntr::Vector<bool>>& incidence_matrix) {
	Node* root(new Node(-1, -1));
	root->col_header = root;
	root->row_header = root;

	size_t col_size(incidence_matrix.size());
	size_t row_size(incidence_matrix[0].size());

	for (size_t col_index(0); col_index != col_size; ++col_index) {
		Node* node(new Node(col_index, -1));
		node->col_header = node;
		node->row_header = root;
		root->PushU(node);
	}

	for (size_t row_index(0); row_index != row_size; ++row_index) {
		Node* node(new Node(-1, row_index));
		node->col_header = root;
		node->row_header = node;
		root->PushL(node);
	}

	Node* col_header(root->d);

	for (size_t col_index(0); col_index != col_size;
		 ++col_index, col_header = col_header->d) {
		Node* row_header(root->r);

		for (size_t row_index(0); row_index != row_size;
			 ++row_index, row_header = row_header->r) {
			if (incidence_matrix[col_index][row_index]) {
				Node* node(new Node(col_index, row_index));
				node->col_header = col_header;
				node->row_header = row_header;
				col_header->PushL(node);
				row_header->PushU(node);
			}
		}
	}

	return root;
}

void PrintDancingLink(Node* root) {
	Node* col_header(root->d);

	for (size_t i(0); i != 10; ++i) { std::cout << '|'; }
	std::cout << '\n';

	for (; !col_header->is_row_header(); col_header = col_header->d) {
		Node* node(col_header->r);
		std::cout << col_header->col_index << " : ";

		for (size_t row_index(0); !node->is_col_header(); ++row_index) {
			if (row_index == node->row_index) {
				std::cout << 'O';
				node = node->r;
			} else {
				std::cout << '-';
			}
		}

		std::cout << '\n';
	}
}

template<typename Receiver>
bool Solve_(cntr::Vector<Node*>& dst, Node* root, Node* col_header_i,
			const Receiver& receiver);

template<typename Receiver> void Solve(Node* root, const Receiver& receiver) {
	cntr::Vector<size_t> dst;
	Solve_(dst, root->d, root, receiver);
}

template<typename Receiver>
bool Solve_(cntr::Vector<size_t>& dst, Node* col_header_i, Node* root,
			const Receiver& receiver) {
	if (root->is_sole_lr()) {
		return receiver(static_cast<const cntr::Vector<size_t>&>(dst));
	}

	if (col_header_i == root) { return true; }

	for (; col_header_i != root; col_header_i = col_header_i->d) {
		for (Node* row(col_header_i->r); row != col_header_i; row = row->r) {
			Node* i(row->d);

			for (; !i->is_row_header(); i = i->d) {
				for (Node* j(i->r); j != i; j = j->r) { j->CoverUD(); }
			}

			i->CoverLR();
		}

		dst.Push(col_header_i->col_index);
		bool continue_solve(Solve_(dst, col_header_i->d, root, receiver));

		for (Node* row(col_header_i->l); row != col_header_i; row = row->l) {
			Node* i(row->row_header);
			i->UnCoverLR();

			while ((i = i->u) != row) {
				for (Node* j(i->l); j != i; j = j->l) { j->UnCoverUD(); }
			}
		}

		if (!continue_solve) { return false; }

		dst.Pop();
	}

	return true;
}

#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////

void ConstructDancingLink(DancingLink<size_t>* dst,
						  cntr::Vector<cntr::Vector<size_t>>& nec_cst,
						  cntr::Vector<cntr::Vector<size_t>>& opt_cst) {
	PHI__debug_if(nec_cst.size() != opt_cst.size()) {
		PHI__throw(, __func__, "row size mismatch");
	}

	dst->nec_cst = Move(nec_cst);
	dst->opt_cst = Move(opt_cst);

	size_t col_size(dst->col_size = dst->nec_cst.size());

	dst->nec_cst_index.Clear();

	for (size_t i(0); i != col_size; ++i) {
		Sort(dst->nec_cst[i].first_iterator(), dst->nec_cst[i].null_iterator());

		for (size_t j(0); j != dst->nec_cst[i].size(); ++j) {
			dst->nec_cst_index.Insert(dst->nec_cst[i][j], 0);
		}
	}

	{
		auto iter(dst->nec_cst_index.first_iterator());

		for (size_t i(0); i != dst->nec_cst_index.size(); ++iter, ++i) {
			iter->second = i;
		}
	}

	size_t opt_cst_begin_index(dst->nec_cst_index.size());

	dst->opt_cst_index.Clear();

	for (size_t i(0); i != col_size; ++i) {
		Sort(dst->nec_cst[i].first_iterator(), dst->nec_cst[i].null_iterator());

		for (size_t j(0); j != dst->opt_cst[i].size(); ++j) {
			PHI__debug_if(dst->nec_cst_index.Contain(dst->opt_cst[i][j])) {
				PHI__throw(, __func__, "constraint duplicate");
			}

			dst->opt_cst_index.Insert(dst->opt_cst[i][j], 0);
		}
	}

	{
		auto iter(dst->opt_cst_index.first_iterator());

		for (size_t i(0); i != dst->opt_cst_index.size(); ++iter, ++i) {
			iter->second = opt_cst_begin_index + i;
		}
	}

	size_t row_size(dst->row_size =
						dst->nec_cst_index.size() + dst->opt_cst_index.size());

	dst->root = new Node(col_size, row_size);
	dst->root->col_header = dst->root;
	dst->root->row_header = dst->root;

	PHI__interrupt;

	for (size_t col_index(0); col_index != col_size; ++col_index) {
		Node* node(new Node(col_index, row_size));
		node->col_header = node;
		node->row_header = dst->root;
		dst->root->PushU(node);
	}

	for (size_t row_index(0); row_index != row_size; ++row_index) {
		Node* node(new Node(col_size, row_index));
		node->col_header = dst->root;
		node->row_header = node;
		dst->root->PushL(node);
	}

	PHI__interrupt;

	Node* col_header(dst->root->d);

	for (size_t col_index(0); col_index != col_size;
		 ++col_index, col_header = col_header->d) {
		Node* row_header(dst->root->r);

		for (size_t i(0); i != dst->nec_cst[col_index].size(); ++i) {
			dst->nec_cst[col_index][i];

			PHI__interrupt;

			size_t row_index(
				dst->nec_cst_index.Find(dst->nec_cst[col_index][i])->second);

			PHI__interrupt;

			while (row_header->row_index != row_index) {
				row_header = row_header->r;
			}

			Node* node(new Node(col_index, row_index));
			node->col_header = col_header;
			node->row_header = row_header;
			col_header->PushL(node);
			row_header->PushU(node);

			row_header = row_header->r;
		}
	}
}

#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////

}
}

#endif
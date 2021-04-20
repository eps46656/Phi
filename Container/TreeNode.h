#ifndef PHI__define_guard__Container__TreeNode_cuh
#define PHI__define_guard__Container__TreeNode_cuh

#include "../define.h"

#define PHI__throw__local(desc) PHI__throw(cntr::TreeNode, __func__, desc);

namespace phi {
namespace cntr {

struct TreeNode {
	inline TreeNode* p();
	inline TreeNode* l();
	inline TreeNode* r();

	inline const TreeNode* p() const;
	inline const TreeNode* l() const;
	inline const TreeNode* r() const;

#///////////////////////////////////////////////////////////////////////////////

	inline TreeNode* most_p();
	inline TreeNode* most_l();
	inline TreeNode* most_r();

	inline const TreeNode* most_p() const;
	inline const TreeNode* most_l() const;
	inline const TreeNode* most_r() const;

#///////////////////////////////////////////////////////////////////////////////

	inline bool sole() const;

#///////////////////////////////////////////////////////////////////////////////

	inline TreeNode();

#///////////////////////////////////////////////////////////////////////////////

	inline TreeNode* next();
	inline const TreeNode* next() const;

	inline TreeNode* prev();
	inline const TreeNode* prev() const;

	inline TreeNode* root();
	inline const TreeNode* root() const;

#///////////////////////////////////////////////////////////////////////////////

	inline void RotateL();
	inline void RotateR();

	inline static void Swap(TreeNode* x, TreeNode* y);

#///////////////////////////////////////////////////////////////////////////////

	inline void InsertL(TreeNode* n);
	inline void InsertR(TreeNode* n);

#///////////////////////////////////////////////////////////////////////////////

	template<typename FullComparer>
	TreeNode* Find(const FullComparer& full_cmper);

	template<typename FullComparer>
	const TreeNode* Find(const FullComparer& full_cmper) const;

#///////////////////////////////////////////////////////////////////////////////

	inline TreeNode* EndRelease();

#///////////////////////////////////////////////////////////////////////////////

	inline void Check() const;

protected:
	TreeNode* p_;
	TreeNode* l_;
	TreeNode* r_;
};

#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////

TreeNode* TreeNode::p() { return this->p_; }
TreeNode* TreeNode::l() { return this->l_; }
TreeNode* TreeNode::r() { return this->r_; }

const TreeNode* TreeNode::p() const { return this->p_; }
const TreeNode* TreeNode::l() const { return this->l_; }
const TreeNode* TreeNode::r() const { return this->r_; }

#///////////////////////////////////////////////////////////////////////////////

TreeNode* TreeNode::most_p() {
	TreeNode* n(this);
	while (n->p_ != nullptr) { n = n->p_; }
	return n;
}

const TreeNode* TreeNode::most_p() const {
	const TreeNode* n(this);
	while (n->p_ != nullptr) { n = n->p_; }
	return n;
}

TreeNode* TreeNode::most_l() {
	TreeNode* n(this);
	while (n->l_ != nullptr) { n = n->l_; }
	return n;
}

const TreeNode* TreeNode::most_l() const {
	const TreeNode* n(this);
	while (n->l_ != nullptr) { n = n->l_; }
	return n;
}

TreeNode* TreeNode::most_r() {
	TreeNode* n(this);
	while (n->r_ != nullptr) { n = n->r_; }
	return n;
}

const TreeNode* TreeNode::most_r() const {
	const TreeNode* n(this);
	while (n->r_ != nullptr) { n = n->r_; }
	return n;
}

#///////////////////////////////////////////////////////////////////////////////

bool TreeNode::sole() const {
	return this->p_ == nullptr && this->l_ == nullptr && this->r_ == nullptr;
}

#///////////////////////////////////////////////////////////////////////////////

TreeNode::TreeNode(): p_(nullptr), l_(nullptr), r_(nullptr) {}

#///////////////////////////////////////////////////////////////////////////////

TreeNode* TreeNode::next() {
	if (this->r_ != nullptr) {
		TreeNode* n(this->r_);
		while (n->l_ != nullptr) { n = n->l_; }
		return n;
	}

	TreeNode* n(this);
	TreeNode* p(this->p_);

	for (; p != nullptr; p = (n = p)->p_) {
		if (n == p->l_) { return p; }
	}

	return nullptr;
}

const TreeNode* TreeNode::next() const {
	return const_cast<TreeNode*>(this)->next();
}

TreeNode* TreeNode::prev() {
	if (this->l_ != nullptr) {
		TreeNode* n(this->l_);
		while (n->r_ != nullptr) { n = n->r_; }
		return n;
	}

	TreeNode* n(this);
	TreeNode* p(this->p_);

	for (; p != nullptr; p = (n = p)->p_) {
		if (n == p->r_) { return p; }
	}

	return nullptr;
}

const TreeNode* TreeNode::prev() const {
	return const_cast<TreeNode*>(this)->prev();
}

TreeNode* TreeNode::root() {
	TreeNode* r(this);
	while (r->p_ != nullptr) { r = r->p_; }
	return r;
}

const TreeNode* TreeNode::root() const {
	return const_cast<TreeNode*>(this)->root();
}

void TreeNode::RotateL() {
	TreeNode* p(this->r_->p_ = this->p_);
	TreeNode* r(this->p_ = this->r_);

	if ((this->r_ = r->l_) != nullptr) { this->r_->p_ = this; }

	r->l_ = this;

	if (p != nullptr) {
		if (this == p->l_) {
			p->l_ = r;
		} else {
			p->r_ = r;
		}
	}
}

void TreeNode::RotateR() {
	TreeNode* p(this->l_->p_ = this->p_);
	TreeNode* l(this->p_ = this->l_);

	if ((this->l_ = l->r_) != nullptr) { this->l_->p_ = this; }

	l->r_ = this;

	if (p != nullptr) {
		if (this == p->l_) {
			p->l_ = l;
		} else {
			p->r_ = l;
		}
	}
}

void TreeNode::Swap(TreeNode* x, TreeNode* y) {
	PHI__debug_if(x == nullptr || y == nullptr) {
		PHI__throw__local("nullptr");
	}

	if (x->p_ == y) {
		TreeNode* temp(x);
		x = y;
		y = temp;
	}

	if (x == y->p_) {
		TreeNode* yl(y->l_);
		TreeNode* yr(y->r_);

		if ((y->p_ = x->p_) != nullptr) {
			if (x == x->p_->l_) {
				y->p_->l_ = y;
			} else {
				y->p_->r_ = y;
			}
		}

		if (x->l_ == (x->p_ = y)) {
			if ((y->r_ = (y->l_ = x)->r_) != nullptr) { y->r_->p_ = y; }
		} else {
			if ((y->l_ = (y->r_ = x)->l_) != nullptr) { y->l_->p_ = y; }
		}

		if ((x->l_ = yl) != nullptr) { yl->p_ = x; }
		if ((x->r_ = yr) != nullptr) { yr->p_ = x; }

		return;
	}

	TreeNode* xp(x->p_);
	TreeNode* xl(x->l_);
	TreeNode* xr(x->r_);

	TreeNode* yp(y->p_);
	TreeNode* yl(y->l_);
	TreeNode* yr(y->r_);

	if ((x->p_ = yp) != nullptr) {
		if (y == yp->l_) {
			yp->l_ = x;
		} else {
			yp->r_ = x;
		}
	}

	if ((y->p_ = xp) != nullptr) {
		if (x == xp->l_) {
			xp->l_ = y;
		} else {
			xp->r_ = y;
		}
	}

	if ((x->l_ = yl) != nullptr) { yl->p_ = x; }
	if ((x->r_ = yr) != nullptr) { yr->p_ = x; }

	if ((y->l_ = xl) != nullptr) { xl->p_ = y; }
	if ((y->r_ = xr) != nullptr) { xr->p_ = y; }
}

#///////////////////////////////////////////////////////////////////////////////

void TreeNode::InsertL(TreeNode* node) {
	PHI__debug_if(this->l_ != nullptr) {
		PHI__throw__local("this->l_ is not nullptr");
	}

	PHI__debug_if(!node->sole()) { PHI__throw__local("node is not sole"); }

	node->p_ = this;
	this->l_ = node;
}

void TreeNode::InsertR(TreeNode* node) {
	PHI__debug_if(this->r_ != nullptr) {
		PHI__throw__local("this->r_ is not nullptr");
	}

	PHI__debug_if(!node->sole()) { PHI__throw__local("node is not sole"); }

	this->r_ = node;
	node->p_ = this;
}

#///////////////////////////////////////////////////////////////////////////////

/*
If this has one child, this function replace this with child in the tree and
release this, return the child.

If this has no child, this function release this, return nullptr.
*/
TreeNode* TreeNode::EndRelease() {
	PHI__debug_if(this->l_ != nullptr && this->r_ != nullptr) {
		PHI__throw__local("this is not an end");
	}

	TreeNode* c;

	if (this->l_ != nullptr) {
		c = this->l_;
		this->l_ = nullptr;
	} else if (this->r_ != nullptr) {
		c = this->r_;
		this->r_ = nullptr;
	} else {
		if (this->p_ != nullptr) {
			if (this == this->p_->l_) {
				this->p_->l_ = nullptr;
			} else {
				this->p_->r_ = nullptr;
			}

			this->p_ = nullptr;
		}

		return nullptr;
	}

	c->p_ = this->p_;

	if (this->p_ != nullptr) {
		if (this == this->p_->l_) {
			this->p_->l_ = c;
		} else {
			this->p_->r_ = c;
		}

		this->p_ = nullptr;
	}

	return c;
}

#///////////////////////////////////////////////////////////////////////////////

void TreeNode::Check() const {
	if (this->p_ != nullptr) {
		if ((this == this->p_->l_) == (this == this->p_->r_)) {
			std::cout << "link error\n";
		}
	}

	this->l_->Check();
	this->r_->Check();
}

}
}

#undef PHI__throw__local

#endif
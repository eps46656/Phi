#ifndef PHI__define_guard__Container__RedBlackTreeNode_cuh
#define PHI__define_guard__Container__RedBlackTreeNode_cuh

#include "../define.h"
#include "TreeNode.h"

#define PHI__throw__local(desc)                                                \
	PHI__throw(cntr::RedBlackTreeNode, __func__, desc);

#define PHI__rbtn_ptr(x) static_cast<RedBlackTreeNode*>(x)
#define PHI__const_rbtn_ptr(x) static_cast<const RedBlackTreeNode*>(x)

namespace phi {
namespace cntr {

struct RedBlackTreeNode: public TreeNode {
	static constexpr bool black = false;
	static constexpr bool red = true;

#///////////////////////////////////////////////////////////////////////////////

	inline RedBlackTreeNode* p();
	inline RedBlackTreeNode* l();
	inline RedBlackTreeNode* r();

	inline const RedBlackTreeNode* p() const;
	inline const RedBlackTreeNode* l() const;
	inline const RedBlackTreeNode* r() const;

	inline bool color() const;

#///////////////////////////////////////////////////////////////////////////////

	inline RedBlackTreeNode* most_p();
	inline const RedBlackTreeNode* most_p() const;
	inline RedBlackTreeNode* most_l();
	inline const RedBlackTreeNode* most_l() const;
	inline RedBlackTreeNode* most_r();
	inline const RedBlackTreeNode* most_r() const;

#///////////////////////////////////////////////////////////////////////////////

	inline RedBlackTreeNode();
	inline virtual ~RedBlackTreeNode();

#///////////////////////////////////////////////////////////////////////////////

	inline RedBlackTreeNode* next();
	inline const RedBlackTreeNode* next() const;

	inline RedBlackTreeNode* prev();
	inline const RedBlackTreeNode* prev() const;

	inline RedBlackTreeNode* root();
	inline const RedBlackTreeNode* root() const;

#///////////////////////////////////////////////////////////////////////////////

	inline void InsertL(RedBlackTreeNode* n);
	inline void InsertR(RedBlackTreeNode* n);

	inline void Release();
	inline void ReleaseAll();

#///////////////////////////////////////////////////////////////////////////////

	inline size_t Check() const;

protected:
	bool color_;

	inline void InsertFix_();
	inline void ReleaseFix_();
	inline static void ReleaseAll_(RedBlackTreeNode* n);
};

#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////

RedBlackTreeNode* RedBlackTreeNode::p() { return PHI__rbtn_ptr(this->p_); }
RedBlackTreeNode* RedBlackTreeNode::l() { return PHI__rbtn_ptr(this->l_); }
RedBlackTreeNode* RedBlackTreeNode::r() { return PHI__rbtn_ptr(this->r_); }

const RedBlackTreeNode* RedBlackTreeNode::p() const {
	return PHI__rbtn_ptr(this->p_);
}
const RedBlackTreeNode* RedBlackTreeNode::l() const {
	return PHI__rbtn_ptr(this->l_);
}
const RedBlackTreeNode* RedBlackTreeNode::r() const {
	return PHI__rbtn_ptr(this->r_);
}

bool RedBlackTreeNode::color() const { return this->color_; }

#///////////////////////////////////////////////////////////////////////////////

RedBlackTreeNode* RedBlackTreeNode::most_p() {
	return PHI__rbtn_ptr(this->TreeNode::most_p());
}
RedBlackTreeNode* RedBlackTreeNode::most_l() {
	return PHI__rbtn_ptr(this->TreeNode::most_l());
}
RedBlackTreeNode* RedBlackTreeNode::most_r() {
	return PHI__rbtn_ptr(this->TreeNode::most_r());
}

const RedBlackTreeNode* RedBlackTreeNode::most_p() const {
	return PHI__const_rbtn_ptr(this->TreeNode::most_p());
}
const RedBlackTreeNode* RedBlackTreeNode::most_l() const {
	return PHI__const_rbtn_ptr(this->TreeNode::most_l());
}
const RedBlackTreeNode* RedBlackTreeNode::most_r() const {
	return PHI__const_rbtn_ptr(this->TreeNode::most_r());
}

#///////////////////////////////////////////////////////////////////////////////

RedBlackTreeNode::RedBlackTreeNode(): color_(black) {}

RedBlackTreeNode::~RedBlackTreeNode() { this->Release(); }

#///////////////////////////////////////////////////////////////////////////////

RedBlackTreeNode* RedBlackTreeNode::next() {
	return PHI__rbtn_ptr(this->TreeNode::next());
}

const RedBlackTreeNode* RedBlackTreeNode::next() const {
	return PHI__const_rbtn_ptr(this->TreeNode::next());
}

RedBlackTreeNode* RedBlackTreeNode::prev() {
	return PHI__rbtn_ptr(this->TreeNode::prev());
}

const RedBlackTreeNode* RedBlackTreeNode::prev() const {
	return PHI__const_rbtn_ptr(this->TreeNode::prev());
}

RedBlackTreeNode* RedBlackTreeNode::root() {
	return PHI__rbtn_ptr(this->TreeNode::root());
}

const RedBlackTreeNode* RedBlackTreeNode::root() const {
	return PHI__const_rbtn_ptr(this->TreeNode::root());
}

#///////////////////////////////////////////////////////////////////////////////

void RedBlackTreeNode::InsertL(RedBlackTreeNode* n) {
	this->TreeNode::InsertL(n);
	n->color_ = red;
	if (this->color_ == red) { n->InsertFix_(); }
}

void RedBlackTreeNode::InsertR(RedBlackTreeNode* n) {
	this->TreeNode::InsertR(n);
	n->color_ = red;
	if (this->color_ == red) { n->InsertFix_(); }
}

void RedBlackTreeNode::InsertFix_() {
	RedBlackTreeNode* n(this);
	RedBlackTreeNode* p;
	RedBlackTreeNode* g;
	RedBlackTreeNode* u;

	while ((p = PHI__rbtn_ptr(n->p_)) != nullptr && p->color_ == red) {
		if (p == (g = PHI__rbtn_ptr(p->p_))->l_) {
			if (n == p->r_) {
				p->RotateL();
				p = PHI__rbtn_ptr((n = p)->p_);
			}

			p->color_ = black;
			g->color_ = red;

			if ((u = PHI__rbtn_ptr(g->r_)) == nullptr || u->color_ == black) {
				g->RotateR();
				return;
			}
		} else {
			if (n == p->l_) {
				p->RotateR();
				p = PHI__rbtn_ptr((n = p)->p_);
			}

			p->color_ = black;
			g->color_ = red;

			if ((u = PHI__rbtn_ptr(g->l_)) == nullptr || u->color_ == black) {
				g->RotateL();
				return;
			}
		}

		u->color_ = black;
		if ((n = g)->p_ == nullptr) {
			n->color_ = black;
			break;
		}
	}
}

#///////////////////////////////////////////////////////////////////////////////

void RedBlackTreeNode::Release() {
	if (this->p_ == nullptr && this->l_ == nullptr && this->r_ == nullptr) {
		return;
	}

	RedBlackTreeNode* n(this);
	RedBlackTreeNode* m;

	if (n->l_ != nullptr && n->r_ != nullptr) {
		m = PHI__rbtn_ptr(n->l_->most_r());

		Swap(n, m);

		if (n->color_ != m->color_) {
			n->color_ = !n->color_;
			m->color_ = !m->color_;
		}
	}

	if (n->color_ == black) {
		if ((m = PHI__rbtn_ptr(n->l_)) != nullptr ||
			(m = PHI__rbtn_ptr(n->r_)) != nullptr) {
			if ((m->p_ = n->p_) != nullptr) {
				if (n == PHI__rbtn_ptr(m->p_)->l_) {
					PHI__rbtn_ptr(m->p_)->l_ = m;
				} else {
					PHI__rbtn_ptr(m->p_)->r_ = m;
				}
			}

			if (m->color_ == black) {
				m->ReleaseFix_();
			} else {
				m->color_ = black;
			}
		} else {
			n->ReleaseFix_();

			if (n == PHI__rbtn_ptr(n->p_)->l_) {
				PHI__rbtn_ptr(n->p_)->l_ = nullptr;
			} else {
				PHI__rbtn_ptr(n->p_)->r_ = nullptr;
			}
		}
	} else {
		if ((m = PHI__rbtn_ptr(n->l_)) != nullptr ||
			(m = PHI__rbtn_ptr(n->r_)) != nullptr) {
			if (n == PHI__rbtn_ptr(m->p_ = n->p_)->l_) {
				PHI__rbtn_ptr(m->p_)->l_ = m;
			} else {
				PHI__rbtn_ptr(m->p_)->r_ = m;
			}
		} else {
			if (n == PHI__rbtn_ptr(n->p_)->l_) {
				PHI__rbtn_ptr(n->p_)->l_ = nullptr;
			} else {
				PHI__rbtn_ptr(n->p_)->r_ = nullptr;
			}
		}
	}

	n->p_ = n->l_ = n->r_ = nullptr;
	n->color_ = black;
}

void RedBlackTreeNode::ReleaseFix_() {
	RedBlackTreeNode* n(this);
	RedBlackTreeNode* p;
	RedBlackTreeNode* s;

	while ((p = PHI__rbtn_ptr(n->p_)) != nullptr) {
		if (n == p->l_) {
			if ((s = PHI__rbtn_ptr(p->r_))->color_ == red) {
				p->color_ = red;
				s->color_ = black;
				p->RotateL();
				s = PHI__rbtn_ptr(p->r_);
			}

			RedBlackTreeNode* sr(PHI__rbtn_ptr(s->r_));

			if (sr != nullptr && sr->color_ == red) {
				s->color_ = p->color_;
				p->color_ = sr->color_ = black;
				p->RotateL();
				return;
			}

			RedBlackTreeNode* sl(PHI__rbtn_ptr(s->l_));

			if (sl != nullptr && sl->color_ == red) {
				sl->color_ = p->color_;
				p->color_ = black;
				s->RotateR();
				p->RotateL();
				return;
			}
		} else {
			if ((s = PHI__rbtn_ptr(p->l_))->color_ == red) {
				p->color_ = red;
				s->color_ = black;
				p->RotateR();
				s = PHI__rbtn_ptr(p->l_);
			}

			RedBlackTreeNode* sl(PHI__rbtn_ptr(s->l_));

			if (sl != nullptr && sl->color_ == red) {
				s->color_ = p->color_;
				p->color_ = sl->color_ = black;
				p->RotateR();
				return;
			}

			RedBlackTreeNode* sr(PHI__rbtn_ptr(s->r_));

			if (sr != nullptr && sr->color_ == red) {
				sr->color_ = p->color_;
				p->color_ = black;
				s->RotateL();
				p->RotateR();
				return;
			}
		}

		if (p->color_ == red) {
			p->color_ = black;
			s->color_ = red;
			return;
		}

		s->color_ = red;
		n = p;
	}
}

void RedBlackTreeNode::ReleaseAll() { ReleaseAll_(this->root()); }

void RedBlackTreeNode::ReleaseAll_(RedBlackTreeNode* n) {
	n->p_ = nullptr;

	if (n->l_ != nullptr) {
		ReleaseAll_(PHI__rbtn_ptr(n->l_));
		n->l_ = nullptr;
	}

	if (n->r_ != nullptr) {
		ReleaseAll_(PHI__rbtn_ptr(n->r_));
		n->r_ = nullptr;
	}

	n->color_ = black;
}

#///////////////////////////////////////////////////////////////////////////////

size_t RedBlackTreeNode::Check() const {
	if (this->p_ != nullptr) {
		if ((this == PHI__const_rbtn_ptr(this->p_)->l_) ==
			(this == PHI__const_rbtn_ptr(this->p_)->r_)) {
			std::cout << "link error\n";
		}

		if (this->color_ == red && PHI__rbtn_ptr(this->p_)->color_ == red) {
			std::cout << "color error\n";
		}
	}

	size_t l_bh(this->l_ == nullptr ? 0 : PHI__rbtn_ptr(this->l_)->Check());
	size_t r_bh(this->r_ == nullptr ? 0 : PHI__rbtn_ptr(this->r_)->Check());

	if (l_bh != r_bh) { std::cout << "bh error\n"; }

	return this->color_ == black ? l_bh + 1 : l_bh;
}

}
}

#undef PHI__throw__local
#undef PHI__rbtn_ptr
#undef PHI__const_rbtn_ptr

#endif
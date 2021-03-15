#ifndef PHI__define_guard__Container__RedBlackTreeNode_cuh
#define PHI__define_guard__Container__RedBlackTreeNode_cuh

#include "../define.h"

#define PHI__throw__local(desc)                                                \
	PHI__throw(cntr::RedBlackTreeNode, __func__, desc);

namespace phi {
namespace cntr {

struct RedBlackTreeNode {
	static constexpr bool black = false;
	static constexpr bool red = true;

#///////////////////////////////////////////////////////////////////////////////

	inline RedBlackTreeNode* p() const;
	inline RedBlackTreeNode* l() const;
	inline RedBlackTreeNode* r() const;
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

	inline void RotateL();
	inline void RotateR();

	inline static void Swap(RedBlackTreeNode* x, RedBlackTreeNode* y);

#///////////////////////////////////////////////////////////////////////////////

	inline void InsertL(RedBlackTreeNode* n);
	inline void InsertR(RedBlackTreeNode* n);
	inline void InsertFix();

#///////////////////////////////////////////////////////////////////////////////

	inline void Release();
	inline void ReleaseFix();

#///////////////////////////////////////////////////////////////////////////////

	inline size_t Check() const;

protected:
	RedBlackTreeNode* p_;
	RedBlackTreeNode* l_;
	RedBlackTreeNode* r_;
	bool color_;
};

#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////

RedBlackTreeNode* RedBlackTreeNode::p() const { return this->p_; }
RedBlackTreeNode* RedBlackTreeNode::l() const { return this->l_; }
RedBlackTreeNode* RedBlackTreeNode::r() const { return this->r_; }
bool RedBlackTreeNode::color() const { return this->color_; }

#///////////////////////////////////////////////////////////////////////////////

RedBlackTreeNode* RedBlackTreeNode::most_p() {
	RedBlackTreeNode* n(this);
	while (n->p_ != nullptr) { n = n->p_; }
	return n;
}

const RedBlackTreeNode* RedBlackTreeNode::most_p() const {
	const RedBlackTreeNode* n(this);
	while (n->p_ != nullptr) { n = n->p_; }
	return n;
}

RedBlackTreeNode* RedBlackTreeNode::most_l() {
	RedBlackTreeNode* n(this);
	while (n->l_ != nullptr) { n = n->l_; }
	return n;
}

const RedBlackTreeNode* RedBlackTreeNode::most_l() const {
	const RedBlackTreeNode* n(this);
	while (n->l_ != nullptr) { n = n->l_; }
	return n;
}

RedBlackTreeNode* RedBlackTreeNode::most_r() {
	RedBlackTreeNode* n(this);
	while (n->r_ != nullptr) { n = n->r_; }
	return n;
}

const RedBlackTreeNode* RedBlackTreeNode::most_r() const {
	const RedBlackTreeNode* n(this);
	while (n->r_ != nullptr) { n = n->r_; }
	return n;
}

#///////////////////////////////////////////////////////////////////////////////

RedBlackTreeNode::RedBlackTreeNode():
	p_(nullptr), l_(nullptr), r_(nullptr), color_(black) {}

RedBlackTreeNode::~RedBlackTreeNode() { this->Release(); }

#///////////////////////////////////////////////////////////////////////////////

RedBlackTreeNode* RedBlackTreeNode::next() {
	if (this->r_ != nullptr) {
		RedBlackTreeNode* n(this->r_);
		while (n->l_ != nullptr) { n = n->l_; }
		return n;
	}

	RedBlackTreeNode* n(this);
	RedBlackTreeNode* p(this->p_);

	for (; p != nullptr; p = (n = p)->p_) {
		if (n == p->l_) { return p; }
	}

	return nullptr;
}

const RedBlackTreeNode* RedBlackTreeNode::next() const {
	return const_cast<RedBlackTreeNode*>(this)->next();
}

RedBlackTreeNode* RedBlackTreeNode::prev() {
	if (this->l_ != nullptr) {
		RedBlackTreeNode* n(this->l_);
		while (n->r_ != nullptr) { n = n->r_; }
		return n;
	}

	RedBlackTreeNode* n(this);
	RedBlackTreeNode* p(this->p_);

	for (; p != nullptr; p = (n = p)->p_) {
		if (n == p->r_) { return p; }
	}

	return nullptr;
}

const RedBlackTreeNode* RedBlackTreeNode::prev() const {
	return const_cast<RedBlackTreeNode*>(this)->prev();
}

RedBlackTreeNode* RedBlackTreeNode::root() {
	RedBlackTreeNode* r(this);
	while (r->p_ != nullptr) { r = r->p_; }
	return r;
}

const RedBlackTreeNode* RedBlackTreeNode::root() const {
	return const_cast<RedBlackTreeNode*>(this)->root();
}

void RedBlackTreeNode::RotateL() {
	RedBlackTreeNode* p(this->r_->p_ = this->p_);
	RedBlackTreeNode* r(this->p_ = this->r_);

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

void RedBlackTreeNode::RotateR() {
	RedBlackTreeNode* p(this->l_->p_ = this->p_);
	RedBlackTreeNode* l(this->p_ = this->l_);

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

void RedBlackTreeNode::Swap(RedBlackTreeNode* x, RedBlackTreeNode* y) {
	PHI__debug_if(!x || !y) { PHI__throw__local("nullptr"); }

	if (x->p_ == y) {
		RedBlackTreeNode* temp(x);
		x = y;
		y = temp;
	}

	if (x == y->p_) {
		RedBlackTreeNode* yl(y->l_);
		RedBlackTreeNode* yr(y->r_);

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

	RedBlackTreeNode* xp(x->p_);
	RedBlackTreeNode* xl(x->l_);
	RedBlackTreeNode* xr(x->r_);

	RedBlackTreeNode* yp(y->p_);
	RedBlackTreeNode* yl(y->l_);
	RedBlackTreeNode* yr(y->r_);

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

void RedBlackTreeNode::InsertL(RedBlackTreeNode* node) {
	PHI__debug_if(this->l_) { PHI__throw__local("this->l_ exist"); }
	((node->p_ = this)->l_ = node)->color_ = red;
	if (this->color_ == red) { node->InsertFix(); }
}

void RedBlackTreeNode::InsertR(RedBlackTreeNode* node) {
	PHI__debug_if(this->r_) { PHI__throw__local("this->r_ exist"); }
	((node->p_ = this)->r_ = node)->color_ = red;
	if (this->color_ == red) { node->InsertFix(); }
}

void RedBlackTreeNode::InsertFix() {
	RedBlackTreeNode* n(this);
	RedBlackTreeNode* p;
	RedBlackTreeNode* g;
	RedBlackTreeNode* u;

	while ((p = n->p_) != nullptr && p->color_ == red) {
		if (p == (g = p->p_)->l_) {
			if (n == p->r_) {
				p->RotateL();
				p = (n = p)->p_;
			}

			p->color_ = black;
			g->color_ = red;

			if ((u = g->r_) == nullptr || u->color_ == black) {
				g->RotateR();
				return;
			}
		} else {
			if (n == p->l_) {
				p->RotateR();
				p = (n = p)->p_;
			}

			p->color_ = black;
			g->color_ = red;

			if ((u = g->l_) == nullptr || u->color_ == black) {
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
		m = m->l_->most_r();

		Swap(n, m);

		if (n->color_ != m->color_) {
			n->color_ = !n->color_;
			m->color_ = !m->color_;
		}
	}

	if (n->color_ == black) {
		if ((m = n->l_) != nullptr || (m = n->r_) != nullptr) {
			if ((m->p_ = n->p_) != nullptr) {
				if (n == m->p_->l_) {
					m->p_->l_ = m;
				} else {
					m->p_->r_ = m;
				}
			}

			if (m->color_ == black) {
				m->ReleaseFix();
			} else {
				m->color_ = black;
			}
		} else {
			n->ReleaseFix();

			if (n == n->p_->l_) {
				n->p_->l_ = nullptr;
			} else {
				n->p_->r_ = nullptr;
			}
		}
	} else {
		if ((m = n->l_) != nullptr || (m = n->r_) != nullptr) {
			if (n == (m->p_ = n->p_)->l_) {
				m->p_->l_ = m;
			} else {
				m->p_->r_ = m;
			}
		} else {
			if (n == n->p_->l_) {
				n->p_->l_ = nullptr;
			} else {
				n->p_->r_ = nullptr;
			}
		}
	}

	n->p_ = n->l_ = n->r_ = nullptr;
	n->color_ = black;
}

void RedBlackTreeNode::ReleaseFix() {
	RedBlackTreeNode* n(this);
	RedBlackTreeNode* p;
	RedBlackTreeNode* s;

	while ((p = n->p_) != nullptr) {
		if (n == p->l_) {
			if ((s = p->r_)->color_ == red) {
				p->color_ = red;
				s->color_ = black;
				p->RotateL();
				s = p->r_;
			}

			RedBlackTreeNode* sr(s->r_);

			if (sr != nullptr && sr->color_ == red) {
				s->color_ = p->color_;
				p->color_ = sr->color_ = black;
				p->RotateL();
				return;
			}

			RedBlackTreeNode* sl(s->l_);

			if (sl != nullptr && sl->color_ == red) {
				sl->color_ = p->color_;
				p->color_ = black;
				s->RotateR();
				p->RotateL();
				return;
			}
		} else {
			if ((s = p->l_)->color_ == red) {
				p->color_ = red;
				s->color_ = black;
				p->RotateR();
				s = p->l_;
			}

			RedBlackTreeNode* sl(s->l_);

			if (sl != nullptr && sl->color_ == red) {
				s->color_ = p->color_;
				p->color_ = sl->color_ = black;
				p->RotateR();
				return;
			}

			RedBlackTreeNode* sr(s->r_);

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

#///////////////////////////////////////////////////////////////////////////////

size_t RedBlackTreeNode::Check() const {
	if (this->p_ != nullptr) {
		if ((this == this->p_->l_) == (this == this->p_->r_)) {
			std::cout << "link error\n";
		}

		if (this->color_ == red && this->p_->color_ == red) {
			std::cout << "color error\n";
		}
	}

	size_t l_bh(this->l_ == nullptr ? 0 : this->l_->Check());
	size_t r_bh(this->r_ == nullptr ? 0 : this->r_->Check());

	if (l_bh != r_bh) { std::cout << "bh error\n"; }

	return this->color_ == black ? l_bh + 1 : l_bh;
}

}
}

#undef PHI__throw__local

#endif
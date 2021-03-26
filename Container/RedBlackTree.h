#ifndef PHI__define_guard__Container__RedBlackTree_h
#define PHI__define_guard__Container__RedBlackTree_h

#include "../define.h"
#include "../Utility/memory.h"
#include "../Utility/pair.h"
#include "../Utility/compare.h"
#include "RedBlackTreeNode.h"
#include "Pool.h"

#define PHI__throw__local(desc) PHI__throw(cntr::RedBlackTree, __func__, desc);

namespace phi {
namespace cntr {

template<typename T, typename FullComparer = DefaultFullComparer>
class RedBlackTree {
public:
	struct Node: private RedBlackTreeNode {
		friend class RedBlackTree;

	public:
		T value;

		template<typename... Args> Node(Args&&... args);

	private:
		void set_p(Node* p);
		void set_l(Node* l);
		void set_r(Node* r);
		void set_color(bool color);
	};

#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////

	class Iterator;
	class ConstIterator;

	class Iterator {
		friend class RedBlackTree;

	public:
		Iterator(const Iterator& iter);

		Iterator& operator=(const Iterator& iter);

		bool operator==(const Iterator& iter) const;
		bool operator!=(const Iterator& iter) const;
		bool operator==(const ConstIterator& const_iter) const;
		bool operator!=(const ConstIterator& const_iter) const;

		T& operator*() const;
		T* operator->() const;

		Iterator& operator++();
		Iterator& operator--();

	private:
		RedBlackTree* rbt_;
		Node* node_;

		Iterator(RedBlackTree* rbt, Node* node);
	};

	class ConstIterator {
		friend class RedBlackTree;

	public:
		ConstIterator(const Iterator& iter);
		ConstIterator(const ConstIterator& const_iter);

		ConstIterator& operator=(const Iterator& iter);
		ConstIterator& operator=(const ConstIterator& const_iter);

		bool operator==(const Iterator& iter) const;
		bool operator!=(const Iterator& iter) const;
		bool operator==(const ConstIterator& iter) const;
		bool operator!=(const ConstIterator& iter) const;

		const T& operator*() const;
		const T* operator->() const;

		ConstIterator& operator++();
		ConstIterator& operator--();

	private:
		const RedBlackTree* rbt_;
		const Node* node_;

		ConstIterator(const RedBlackTree* rbt, const Node* node);
	};

#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////

	static constexpr bool black = RedBlackTreeNode::black;
	static constexpr bool red = RedBlackTreeNode::red;

	size_t size() const;
	bool empty() const;

	const FullComparer& full_comparer();

#///////////////////////////////////////////////////////////////////////////////

	Iterator first_iterator();
	Iterator last_iterator();
	Iterator null_iterator();

	ConstIterator first_iterator() const;
	ConstIterator last_iterator() const;
	ConstIterator null_iterator() const;

	ConstIterator first_const_iterator();
	ConstIterator last_const_iterator();
	ConstIterator null_const_iterator();

#///////////////////////////////////////////////////////////////////////////////

	RedBlackTree(const FullComparer& full_cmper = FullComparer());
	RedBlackTree(const RedBlackTree& rbt);
	RedBlackTree(RedBlackTree&& rbt);

	~RedBlackTree();

#///////////////////////////////////////////////////////////////////////////////

	template<typename... Args> static RedBlackTree Make(Args&&... args);

#///////////////////////////////////////////////////////////////////////////////

	RedBlackTree& operator=(const RedBlackTree& rbt);
	RedBlackTree& operator=(RedBlackTree&& rbt);

#///////////////////////////////////////////////////////////////////////////////

	template<typename Index> bool Contain(const Index& index) const;

#///////////////////////////////////////////////////////////////////////////////

	template<typename Index> Iterator Find(const Index& index);
	template<typename Index> ConstIterator Find(const Index& index) const;

#///////////////////////////////////////////////////////////////////////////////

	pair<Node*, bool> InsertNode(Node* node);
	template<typename... Args> bool Insert(Args&&... args);

#///////////////////////////////////////////////////////////////////////////////

	void Release(const Iterator& iter);

	Iterator Erase(const Iterator& iter);
	template<typename Index> bool FindErase(const Index& index);

	void Clear();

	void Check() const { this->root_->Check(); }

private:
	size_t size_;
	FullComparer full_cmper_;
	Node* root_;

	UncountedPool<sizeof(Node)> pool_;

	Node* first_node_() const;
	Node* last_node_() const;

	template<typename X, typename... Args>
	static void Make_(RedBlackTree& rct, X&& x, Args&&... args);

	static void Make_(RedBlackTree& rct);

	static void Copy_(Node* n, const Node* m);
	void CopyWithPool_(Node* n, const Node* m);

	void Assign_(Node* n, const Node* m);

	void EnPool_(Node* n);

	template<typename Index> Node* Find_(const Index& index) const;
	bool Insert_(Node* n);
	void Release_(Node* n);

	static void Destruct_(Node* n);
};

#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////

template<typename T, typename FullComparer>
template<typename... Args>
RedBlackTree<T, FullComparer>::Node::Node(Args&&... args):
	value(Forward<Args>(args)...) {}

#///////////////////////////////////////////////////////////////////////////////

template<typename T, typename FullComparer>
void RedBlackTree<T, FullComparer>::Node::set_p(Node* p) {
	this->p_ = p;
}

template<typename T, typename FullComparer>
void RedBlackTree<T, FullComparer>::Node::set_l(Node* l) {
	this->l_ = l;
}

template<typename T, typename FullComparer>
void RedBlackTree<T, FullComparer>::Node::set_r(Node* r) {
	this->r_ = r;
}

template<typename T, typename FullComparer>
void RedBlackTree<T, FullComparer>::Node::set_color(bool color) {
	this->color_ = color;
}

#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////

template<typename T, typename FullComparer>
size_t RedBlackTree<T, FullComparer>::size() const {
	return this->size_;
}

template<typename T, typename FullComparer>
bool RedBlackTree<T, FullComparer>::empty() const {
	return this->size_ == 0;
}

template<typename T, typename FullComparer>
const FullComparer& RedBlackTree<T, FullComparer>::full_comparer() {
	return this->full_cmper_;
}

#///////////////////////////////////////////////////////////////////////////////

template<typename T, typename FullComparer>
typename RedBlackTree<T, FullComparer>::Node*
RedBlackTree<T, FullComparer>::first_node_() const {
	return this->root_ == nullptr ? nullptr
								  : static_cast<Node*>(this->root_->most_l());
}

template<typename T, typename FullComparer>
typename RedBlackTree<T, FullComparer>::Node*
RedBlackTree<T, FullComparer>::last_node_() const {
	return this->root_ == nullptr ? nullptr
								  : static_cast<Node*>(this->root_->most_r());
}

#///////////////////////////////////////////////////////////////////////////////

template<typename T, typename FullComparer>
typename RedBlackTree<T, FullComparer>::Iterator
RedBlackTree<T, FullComparer>::first_iterator() {
	return Iterator(this, this->first_node_());
}

template<typename T, typename FullComparer>
typename RedBlackTree<T, FullComparer>::Iterator
RedBlackTree<T, FullComparer>::last_iterator() {
	return Iterator(this, this->last_node_());
}

template<typename T, typename FullComparer>
typename RedBlackTree<T, FullComparer>::Iterator
RedBlackTree<T, FullComparer>::null_iterator() {
	return Iterator(this, nullptr);
}

template<typename T, typename FullComparer>
typename RedBlackTree<T, FullComparer>::ConstIterator
RedBlackTree<T, FullComparer>::first_iterator() const {
	return ConstIterator(this, this->first_node_());
}

template<typename T, typename FullComparer>
typename RedBlackTree<T, FullComparer>::ConstIterator
RedBlackTree<T, FullComparer>::last_iterator() const {
	return ConstIterator(this, this->last_node_());
}

template<typename T, typename FullComparer>
typename RedBlackTree<T, FullComparer>::ConstIterator
RedBlackTree<T, FullComparer>::null_iterator() const {
	return ConstIterator(this, nullptr);
}

template<typename T, typename FullComparer>
typename RedBlackTree<T, FullComparer>::ConstIterator
RedBlackTree<T, FullComparer>::first_const_iterator() {
	return ConstIterator(this, this->first_node_());
}

template<typename T, typename FullComparer>
typename RedBlackTree<T, FullComparer>::ConstIterator
RedBlackTree<T, FullComparer>::last_const_iterator() {
	return ConstIterator(this, this->last_node_());
}

template<typename T, typename FullComparer>
typename RedBlackTree<T, FullComparer>::ConstIterator
RedBlackTree<T, FullComparer>::null_const_iterator() {
	return ConstIterator(this, nullptr);
}

#///////////////////////////////////////////////////////////////////////////////

template<typename T, typename FullComparer>
RedBlackTree<T, FullComparer>::RedBlackTree(const FullComparer& full_cmper):
	size_(0), full_cmper_(full_cmper), root_(nullptr) {}

template<typename T, typename FullComparer>
RedBlackTree<T, FullComparer>::RedBlackTree(const RedBlackTree& rbt):
	size_(rbt.size_), root_(nullptr) {
	if (rbt.root_ != nullptr) {
		this->Copy_(this->root_ = New<Node>(rbt.root_->value), rbt.root_);
	}
}

template<typename T, typename FullComparer>
void RedBlackTree<T, FullComparer>::Copy_(Node* n, const Node* m) {
	const Node* ml(static_cast<const Node*>(m->l()));
	const Node* mr(static_cast<const Node*>(m->r()));

	if (ml != nullptr) {
		Node* nl(New<Node>(ml->value));
		n->set_l(nl);
		nl->set_p(n);
		nl->set_color(ml->color());

		Copy_(nl, ml);
	}

	if (mr != nullptr) {
		Node* nr(New<Node>(mr->value));
		n->set_r(nr);
		nr->set_p(n);
		nr->set_color(mr->color());

		Copy_(nr, mr);
	}
}

template<typename T, typename FullComparer>
void RedBlackTree<T, FullComparer>::CopyWithPool_(Node* n, const Node* m) {
	const Node* ml(static_cast<const Node*>(m->l()));
	const Node* mr(static_cast<const Node*>(m->r()));

	if (ml != nullptr) {
		Node* nl(static_cast<Node*>(this->pool_.Pop()));
		n->set_l(nl);
		nl->set_p(n);
		nl->set_l(nullptr);
		nl->set_r(nullptr);
		nl->set_color(ml->color());
		new (&nl->value) T(ml->value);

		if (this->pool_.empty()) {
			Copy_(nl, ml);
		} else {
			this->CopyWithPool_(nl, ml);
		}
	}

	if (mr != nullptr) {
		Node* nr(static_cast<Node*>(this->pool_.Pop()));
		n->set_r(nr);
		nr->set_p(n);
		nr->set_l(nullptr);
		nr->set_r(nullptr);
		nr->set_color(mr->color());
		new (&nr->value) T(mr->value);

		if (this->pool_.empty()) {
			Copy_(nr, mr);
		} else {
			this->CopyWithPool_(nr, mr);
		}
	}
}

template<typename T, typename FullComparer>
RedBlackTree<T, FullComparer>::RedBlackTree(RedBlackTree&& rbt):
	size_(rbt.size_), root_(rbt.root_) {
	rbt.size_ = 0;
	rbt.root_ = nullptr;
	rbt.pool_.TrensferTo(this->pool_);
}

template<typename T, typename FullComparer>
RedBlackTree<T, FullComparer>::~RedBlackTree() {
	this->size_ = 0;

	if (this->root_ != nullptr) {
		Destruct_(this->root_);
		this->root_ = nullptr;
	}
}

template<typename T, typename FullComparer>
void RedBlackTree<T, FullComparer>::Destruct_(Node* n) {
	Node* nl(static_cast<Node*>(n->l()));
	Node* nr(static_cast<Node*>(n->r()));
	if (nl != nullptr) { Destruct_(nl); }
	if (nr != nullptr) { Destruct_(nr); }
	n->value.~T();
	Free(n);
}

#///////////////////////////////////////////////////////////////////////////////

template<typename T, typename FullComparer>
template<typename... Args>
RedBlackTree<T, FullComparer>
RedBlackTree<T, FullComparer>::Make(Args&&... args) {
	RedBlackTree rbt;
	Make_(rbt, Forward<Args>(args)...);
}

template<typename T, typename FullComparer>
template<typename X, typename... Args>
void RedBlackTree<T, FullComparer>::Make_(RedBlackTree& rbt, X&& x,
										  Args&&... args) {
	rbt.Insert(Forward<X>(x));
	Make_(rbt, Forward<Args>(args)...);
}

template<typename T, typename FullComparer>
void RedBlackTree<T, FullComparer>::Make_(RedBlackTree& rbt) {}

#///////////////////////////////////////////////////////////////////////////////

#define PHI__left_bias_threshold 16

#define PHI__EnPool_nl                                                         \
	this->EnPool_(nl);                                                         \
	n->set_l(nullptr);

#define PHI__EnPool_nr                                                         \
	this->EnPool_(nr);                                                         \
	n->set_r(nullptr);

template<typename T, typename FullComparer>
RedBlackTree<T, FullComparer>&
RedBlackTree<T, FullComparer>::operator=(const RedBlackTree& rbt) {
	if (rbt.root_ == nullptr) {
		this->Clear();
		return *this;
	}

	this->size_ = rbt.size_;

	if (this->root_ == nullptr) {
		this->root_ = New<Node>(rbt.root_->value);

		if (this->pool_.empty()) {
			this->Copy_(this->root_, rbt.root_);
		} else {
			this->CopyWithPool_(this->root_, rbt.root_);
		}
	} else {
		this->root_->value = rbt.root_->value;
		this->Assign_(this->root_, rbt.root_);
	}

	return *this;
}

template<typename T, typename FullComparer>
void RedBlackTree<T, FullComparer>::Assign_(Node* n, const Node* m) {
	Node* nl(static_cast<Node*>(n->l()));
	Node* nr(static_cast<Node*>(n->r()));

	const Node* ml(static_cast<const Node*>(m->l()));
	const Node* mr(static_cast<const Node*>(m->r()));

	if (ml == nullptr) {
		if (mr == nullptr) {
			if (nl != nullptr) { PHI__EnPool_nl; }
			if (nr != nullptr) { PHI__EnPool_nr; }
		} else {
			if (nl == nullptr) {
				if (nr == nullptr) {
					nr = static_cast<Node*>(this->pool_.Pop());
					n->set_r(nr);
					nr->set_p(n);
					nr->set_l(nullptr);
					nr->set_r(nullptr);
					nr->set_color(mr->color());
					new (&nr->value) T(mr->value);

					if (this->pool_.empty()) {
						Copy_(nr, mr);
					} else {
						this->CopyWithPool_(nr, mr);
					}
				} else {
					nr->set_color(mr->color());
					nr->value = mr->value;
					this->Assign_(nr, mr);
				}
			} else {
				if (nr == nullptr) {
					n->set_r(nr = nl);
					n->set_l(nl = nullptr);
					nr->set_color(mr->color());
					nr->value = mr->value;
				} else {
					PHI__EnPool_nl;
					nr->set_color(mr->color());
					nr->value = mr->value;
				}

				this->Assign_(nr, mr);
			}
		}
	} else {
		if (nl == nullptr) {
			if (nr == nullptr) {
				nl = static_cast<Node*>(this->pool_.Pop());
				n->set_l(nl);
				nl->set_p(n);
				nl->set_l(nullptr);
				nl->set_r(nullptr);
				nl->set_color(ml->color());
				new (&nl->value) T(ml->value);

				if (this->pool_.empty()) {
					Copy_(nl, ml);
				} else {
					this->CopyWithPool_(nl, ml);
				}
			} else {
				n->set_l(nl = nr);
				n->set_r(nr = nullptr);

				nl->set_color(ml->color());
				nl->value = ml->value;

				this->Assign_(nl, ml);
			}
		} else {
			nl->set_color(ml->color());
			nl->value = ml->value;
			this->Assign_(nl, ml);
		}

		if (mr == nullptr) {
			if (nr != nullptr) { PHI__EnPool_nr; }
		} else {
			if (nr == nullptr) {
				nr = static_cast<Node*>(this->pool_.Pop());
				n->set_r(nr);
				nr->set_p(n);
				nr->set_l(nullptr);
				nr->set_r(nullptr);
				nr->set_color(mr->color());
				new (&nr->value) T(mr->value);

				if (this->pool_.empty()) {
					Copy_(nr, mr);
				} else {
					this->CopyWithPool_(nr, mr);
				}
			} else {
				nr->set_color(mr->color());
				nr->value = mr->value;
				this->Assign_(nr, mr);
			}
		}
	}
}

template<typename T, typename FullComparer>
void RedBlackTree<T, FullComparer>::EnPool_(Node* n) {
	Node* nl(static_cast<Node*>(n->l()));
	Node* nr(static_cast<Node*>(n->r()));
	if (nl != nullptr) { this->EnPool_(nl); }
	if (nr != nullptr) { this->EnPool_(nr); }
	n->value.~T();
	this->pool_.Push(n);
}

#undef PHI__EnPool_nl
#undef PHI__EnPool_nr

#///////////////////////////////////////////////////////////////////////////////

template<typename T, typename FullComparer>
template<typename Index>
bool RedBlackTree<T, FullComparer>::Contain(const Index& index) const {
	return this->Find_(index) != nullptr;
}

#///////////////////////////////////////////////////////////////////////////////

template<typename T, typename FullComparer>
template<typename Index>
typename RedBlackTree<T, FullComparer>::Node*
RedBlackTree<T, FullComparer>::Find_(const Index& index) const {
	for (Node* node(this->root_); node != nullptr;) {
		switch (this->full_cmper_(index, node->value)) {
			case -1: node = static_cast<Node*>(node->l()); break;
			case 1: node = static_cast<Node*>(node->r()); break;
			case 0: return node;
		}
	}

	return nullptr;
}

template<typename T, typename FullComparer>
template<typename Index>
typename RedBlackTree<T, FullComparer>::Iterator
RedBlackTree<T, FullComparer>::Find(const Index& index) {
	return Iterator(this, this->Find_(index));
}

template<typename T, typename FullComparer>
template<typename Index>
typename RedBlackTree<T, FullComparer>::ConstIterator
RedBlackTree<T, FullComparer>::Find(const Index& index) const {
	return ConstIterator(this, this->Find_(index));
}

#///////////////////////////////////////////////////////////////////////////////

template<typename T, typename FullComparer>
bool RedBlackTree<T, FullComparer>::Insert_(Node* node) {
	if (this->size_ == 0) {
		++this->size_;
		this->root_ = node;
		return true;
	}

	for (Node* n(this->root_);;) {
		switch (this->full_cmper_(node->value, n->value)) {
			case -1:
				if (n->l() == nullptr) {
					n->InsertL(node);
					goto insert_complete;
				}

				n = static_cast<Node*>(n->l());
				break;
			case 1:
				if (n->r() == nullptr) {
					n->InsertR(node);
					goto insert_complete;
				}

				n = static_cast<Node*>(n->r());
				break;
			case 0: return false;
		}
	}

insert_complete:;

	++this->size_;
	this->root_ = static_cast<Node*>(this->root_->most_p());

	return true;
}

template<typename T, typename FullComparer>
pair<typename RedBlackTree<T, FullComparer>::Node*, bool>
RedBlackTree<T, FullComparer>::InsertNode(Node* node) {
	PHI__debug_if(!node->sole()) { PHI__throw__local("node is not sole"); }
	return pair<Node*, bool>(node, this->Insert_(node));
}

template<typename T, typename FullComparer>
template<typename... Args>
bool RedBlackTree<T, FullComparer>::Insert(Args&&... args) {
	Node* node(new (this->pool_.Pop()) Node(Forward<Args>(args)...));
	if (this->Insert_(node)) { return true; }
	this->pool_.Push(node);
	return false;
}

#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////

template<typename T, typename FullComparer>
void RedBlackTree<T, FullComparer>::Release_(Node* node) {
	if (--this->size_ == 0) {
		this->root_ = nullptr;
	} else {
		if (this->root_ == node) {
			if (this->root_->l() != nullptr) {
				this->root_ = static_cast<Node*>(this->root_->l());
			} else {
				this->root_ = static_cast<Node*>(this->root_->r());
			}
		}

		node->Release();
		this->root_ = static_cast<Node*>(this->root_->most_p());
	}
}

template<typename T, typename FullComparer>
void RedBlackTree<T, FullComparer>::Release(const Iterator& iter) {
	PHI__debug_if(this != iter.rbt_) { PHI__throw__local("iter error"); }
	if (iter.node_ != nullptr) { this->Release_(iter.node_); }
}

template<typename T, typename FullComparer>
typename RedBlackTree<T, FullComparer>::Iterator
RedBlackTree<T, FullComparer>::Erase(const Iterator& iter) {
	PHI__debug_if(this != iter.rbt_) { PHI__throw__local("iter error"); }
	if (iter.node_ == nullptr) { return Iterator(this, nullptr); }

	Node* next_node(static_cast<Node*>(iter.node_->next()));
	this->Release_(iter.node_);
	this->pool_.Push(iter.node_);
	return Iterator(this, next_node);
}

template<typename T, typename FullComparer>
template<typename Index>
bool RedBlackTree<T, FullComparer>::FindErase(const Index& index) {
	Node* node(this->Find_(index));
	if (node == nullptr) { return false; }
	this->Release_(node);
	this->pool_.Push(node);
	return true;
}

template<typename T, typename FullComparer>
void RedBlackTree<T, FullComparer>::Clear() {
	if (this->root_ == nullptr) { return; }
	this->size_ = 0;
	this->EnPool_(this->root_);
	this->root_ = nullptr;
}

#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////

template<typename T, typename FullComparer>
RedBlackTree<T, FullComparer>::Iterator::Iterator(const Iterator& iter):
	rbt_(iter.rbt_), node_(iter.node_) {}

template<typename T, typename FullComparer>
RedBlackTree<T, FullComparer>::Iterator::Iterator(RedBlackTree* rbt,
												  Node* node):
	rbt_(rbt),
	node_(node) {}

#///////////////////////////////////////////////////////////////////////////////

template<typename T, typename FullComparer>
typename RedBlackTree<T, FullComparer>::Iterator&
RedBlackTree<T, FullComparer>::Iterator::operator=(const Iterator& iter) {
	this->rbt_ = iter.rbt_;
	this->node_ = iter.node_;
	return *this;
}

#///////////////////////////////////////////////////////////////////////////////

template<typename T, typename FullComparer>
bool RedBlackTree<T, FullComparer>::Iterator::operator==(
	const Iterator& iter) const {
	return this->node_ == iter.node_ && this->rbt_ == iter.rbt_;
}

template<typename T, typename FullComparer>
bool RedBlackTree<T, FullComparer>::Iterator::operator!=(
	const Iterator& iter) const {
	return this->node_ != iter.node_ || this->rbt_ != iter.rbt_;
}

template<typename T, typename FullComparer>
bool RedBlackTree<T, FullComparer>::Iterator::operator==(
	const ConstIterator& const_iter) const {
	return this->node_ == const_iter.node_ && this->rbt_ == const_iter.rbt_;
}

template<typename T, typename FullComparer>
bool RedBlackTree<T, FullComparer>::Iterator::operator!=(
	const ConstIterator& const_iter) const {
	return this->node_ != const_iter.node_ || this->rbt_ != const_iter.rbt_;
}

#///////////////////////////////////////////////////////////////////////////////

template<typename T, typename FullComparer>
T& RedBlackTree<T, FullComparer>::Iterator::operator*() const {
	return this->node_->value;
}

template<typename T, typename FullComparer>
T* RedBlackTree<T, FullComparer>::Iterator::operator->() const {
	return &this->node_->value;
}

#///////////////////////////////////////////////////////////////////////////////

template<typename T, typename FullComparer>
typename RedBlackTree<T, FullComparer>::Iterator&
RedBlackTree<T, FullComparer>::Iterator::operator++() {
	PHI__debug_if(this->rbt_ == nullptr) { PHI__throw__local("iter error"); }

	this->node_ = this->node_ == nullptr
					  ? this->rbt_->first_node_()
					  : static_cast<Node*>(this->node_->next());

	return *this;
}

template<typename T, typename FullComparer>
typename RedBlackTree<T, FullComparer>::Iterator&
RedBlackTree<T, FullComparer>::Iterator::operator--() {
	PHI__debug_if(this->rbt_ == nullptr) { PHI__throw__local("iter error"); }

	this->node_ = this->node_ == nullptr
					  ? this->rbt_->last_node_()
					  : static_cast<Node*>(this->node_->prev());

	return *this;
}

#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////

template<typename T, typename FullComparer>
RedBlackTree<T, FullComparer>::ConstIterator::ConstIterator(
	const Iterator& iter):
	rbt_(iter.rbt_),
	node_(iter.node_) {}

template<typename T, typename FullComparer>
RedBlackTree<T, FullComparer>::ConstIterator::ConstIterator(
	const ConstIterator& cont_iter):
	rbt_(cont_iter.rbt_),
	node_(cont_iter.node_) {}

template<typename T, typename FullComparer>
RedBlackTree<T, FullComparer>::ConstIterator::ConstIterator(
	const RedBlackTree* rbt, const Node* node):
	rbt_(rbt),
	node_(node) {}

#///////////////////////////////////////////////////////////////////////////////

template<typename T, typename FullComparer>
typename RedBlackTree<T, FullComparer>::ConstIterator&
RedBlackTree<T, FullComparer>::ConstIterator::operator=(const Iterator& iter) {
	this->rbt_ = iter.rbt_;
	this->node_ = iter.node_;
	return *this;
}

template<typename T, typename FullComparer>
typename RedBlackTree<T, FullComparer>::ConstIterator&
RedBlackTree<T, FullComparer>::ConstIterator::operator=(
	const ConstIterator& const_iter) {
	this->rbt_ = const_iter.rbt_;
	this->node_ = const_iter.node_;
	return *this;
}

#///////////////////////////////////////////////////////////////////////////////

template<typename T, typename FullComparer>
bool RedBlackTree<T, FullComparer>::ConstIterator::operator==(
	const Iterator& iter) const {
	return this->node_ == iter.node_ && this->rbt_ == iter.rbt_;
}

template<typename T, typename FullComparer>
bool RedBlackTree<T, FullComparer>::ConstIterator::operator!=(
	const Iterator& iter) const {
	return this->node_ != iter.node_ || this->rbt_ != iter.rbt_;
}

template<typename T, typename FullComparer>
bool RedBlackTree<T, FullComparer>::ConstIterator::operator==(
	const ConstIterator& const_iter) const {
	return this->node_ == const_iter.node_ && this->rbt_ == const_iter.rbt_;
}

template<typename T, typename FullComparer>
bool RedBlackTree<T, FullComparer>::ConstIterator::operator!=(
	const ConstIterator& const_iter) const {
	return this->node_ != const_iter.node_ || this->rbt_ != const_iter.rbt_;
}

#///////////////////////////////////////////////////////////////////////////////

template<typename T, typename FullComparer>
const T& RedBlackTree<T, FullComparer>::ConstIterator::operator*() const {
	PHI__debug_if(this->rbt_ == nullptr) { PHI__throw__local("iter error"); }
	return this->node_->value;
}

template<typename T, typename FullComparer>
const T* RedBlackTree<T, FullComparer>::ConstIterator::operator->() const {
	PHI__debug_if(this->rbt_ == nullptr) { PHI__throw__local("iter error"); }
	return &this->node_->value;
}

#///////////////////////////////////////////////////////////////////////////////

template<typename T, typename FullComparer>
typename RedBlackTree<T, FullComparer>::ConstIterator&
RedBlackTree<T, FullComparer>::ConstIterator::operator++() {
	PHI__debug_if(this->rbt_ == nullptr) { PHI__throw__local("iter error"); }

	this->node_ = this->node_ == nullptr
					  ? this->rbt_->first_node_()
					  : static_cast<Node*>(this->node_->prev());

	return *this;
}

template<typename T, typename FullComparer>
typename RedBlackTree<T, FullComparer>::ConstIterator&
RedBlackTree<T, FullComparer>::ConstIterator::operator--() {
	PHI__debug_if(this->rbt_ == nullptr) { PHI__throw__local("iter error"); }

	this->node_ = this->node_ == nullptr
					  ? this->rbt_->last_node_()
					  : static_cast<Node*>(this->node_->prev());

	return *this;
}

}
}

#undef PHI__throw__local

#endif
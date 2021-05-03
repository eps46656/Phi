#ifndef PHI__define_guard__Container__Set_h
#define PHI__define_guard__Container__Set_h

#include "../Utility/memory_op.h"
#include "../Utility/pair.h"
#include "../Utility/compare.h"
#include "RedBlackTree.h"
#include "Pool.h"

#define PHI__throw__local(desc) PHI__throw(cntr::Set, __func__, desc);

namespace phi {
namespace cntr {

template<typename T, typename FullComparer = DefaultFullComparer> class Set {
public:
	struct Node;
	struct RedBlackTreeFullComparer_;
	class RBT;
	class Iterator;
	class ConstIterator;

#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////

	struct Node: private RedBlackTreeNode {
		friend class Set;

		T value;

		template<typename... Args>
		Node(Args&&... args): value(Forward<Args>(args)...) {}
	};

#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////

	struct RedBlackTreeFullComparer_ {
		FullComparer full_cmper;

		template<typename... FullComparerConstructArgs>
		RedBlackTreeFullComparer_(
			FullComparerConstructArgs&&... full_cmper_construct_args):
			full_cmper(Forward<FullComparerConstructArgs>(
				full_cmper_construct_args)...) {}

		template<typename Value>
		RedBlackTreeFullComparer_& operator=(Value&& value) {
			this->full_cmper = Forward<Value>(value);
		}

		template<typename X, typename Y>
		int operator()(const X& x, const Y& y) const {
			constexpr bool x_is_rbtn(
				is_same<typename remove_reference_and_const<X>::type,
						RedBlackTreeNode*>::value);
			constexpr bool y_is_rbtn(
				is_same<typename remove_reference_and_const<Y>::type,
						RedBlackTreeNode*>::value);

			if constexpr (x_is_rbtn && y_is_rbtn) {
				return this->full_cmper(static_cast<Node*>(x)->value,
										static_cast<Node*>(y)->value);
			} else if constexpr (x_is_rbtn) {
				return this->full_cmper(static_cast<Node*>(x)->value, y);
			} else if constexpr (y_is_rbtn) {
				return this->full_cmper(x, static_cast<Node*>(y)->value);
			}
		}
	};

	using RedBlackTreeFullComparer =
		AutoImplementFullComparer<RedBlackTreeFullComparer_>;

#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////

	class RBT: public cntr::RedBlackTree<RedBlackTreeFullComparer> {
		friend class Set;
	};

#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////

	class Iterator {
		friend class Set;

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
		Set* set_;
		Node* node_;

		Iterator(Set* set, Node* node);
	};

	class ConstIterator {
		friend class Set;

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
		const Set* set_;
		const Node* node_;

		ConstIterator(const Set* set, const Node* node);
	};

#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////

	size_t size() const;
	bool empty() const;

	FullComparer& full_cmper();
	const FullComparer& full_cmper() const;

#///////////////////////////////////////////////////////////////////////////////

	Iterator first_iterator();
	Iterator last_iterator();
	Iterator null_iterator();

	ConstIterator first_iterator() const;
	ConstIterator last_iterator() const;
	ConstIterator null_iterator() const;

	ConstIterator first_const_iterator() const;
	ConstIterator last_const_iterator() const;
	ConstIterator null_const_iterator() const;

#///////////////////////////////////////////////////////////////////////////////

	template<typename... FullComparerConstructArgs>
	Set(FullComparerConstructArgs&&... full_cmper_construct_args);
	Set(const Set& set);
	Set(Set&& set);

	~Set();

#///////////////////////////////////////////////////////////////////////////////

	template<typename... Args> static Set Make(Args&&... args);

#///////////////////////////////////////////////////////////////////////////////

	Set& operator=(const Set& set);
	Set& operator=(Set&& set);

#///////////////////////////////////////////////////////////////////////////////

	bool operator==(const Set& set) const;

#///////////////////////////////////////////////////////////////////////////////

	template<typename Index> bool Contain(const Index& index) const;

#///////////////////////////////////////////////////////////////////////////////

	template<typename Index> Iterator Find(const Index& index);
	template<typename Index> ConstIterator Find(const Index& index) const;

#///////////////////////////////////////////////////////////////////////////////

	template<typename... Args> pair<Iterator, bool> Insert(Args&&... args);

#///////////////////////////////////////////////////////////////////////////////

	void Release(const Iterator& iter);

	Iterator Erase(const Iterator& iter);
	template<typename Index> bool FindErase(const Index& index);

	void Clear();

	void Check() const { this->rbt_.root_->Check(); }

private:
	RBT rbt_;
	UncountedPool<sizeof(Node)> pool_;

	Node* first_node_() const;
	Node* last_node_() const;

	template<typename X, typename... Args>
	static void Make_(Set& set, X&& x, Args&&... args);

	static void Make_(Set& set);

	static void Copy_(Node* n, const Node* m);
	void CopyWithPool_(Node* n, const Node* m);

	void Assign_(Node* n, const Node* m);

	void EnPool_(Node* n);

	static void Destruct_(Node* n);
};

#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////

template<typename T, typename FullComparer>
size_t Set<T, FullComparer>::size() const {
	return this->rbt_.size();
}

template<typename T, typename FullComparer>
bool Set<T, FullComparer>::empty() const {
	return this->rbt_.empty();
}

template<typename T, typename FullComparer>
FullComparer& Set<T, FullComparer>::full_cmper() {
	return this->rbt_.full_cmper().full_cmper;
}

template<typename T, typename FullComparer>
const FullComparer& Set<T, FullComparer>::full_cmper() const {
	return this->rbt_.full_cmper().full_cmper;
}

#///////////////////////////////////////////////////////////////////////////////

template<typename T, typename FullComparer>
typename Set<T, FullComparer>::Node* Set<T, FullComparer>::first_node_() const {
	return static_cast<Node*>(this->rbt_.first_node_());
}

template<typename T, typename FullComparer>
typename Set<T, FullComparer>::Node* Set<T, FullComparer>::last_node_() const {
	return static_cast<Node*>(this->rbt_.last_node_());
}

#///////////////////////////////////////////////////////////////////////////////

template<typename T, typename FullComparer>
typename Set<T, FullComparer>::Iterator Set<T, FullComparer>::first_iterator() {
	return Iterator(this, static_cast<Node*>(this->rbt_.first_node()));
}

template<typename T, typename FullComparer>
typename Set<T, FullComparer>::Iterator Set<T, FullComparer>::last_iterator() {
	return Iterator(this, static_cast<Node*>(this->rbt_.last_node()));
}

template<typename T, typename FullComparer>
typename Set<T, FullComparer>::Iterator Set<T, FullComparer>::null_iterator() {
	return Iterator(this, nullptr);
}

template<typename T, typename FullComparer>
typename Set<T, FullComparer>::ConstIterator
Set<T, FullComparer>::first_iterator() const {
	return ConstIterator(
		this, static_cast<const Node*>(this->rbt_.first_const_node()));
}

template<typename T, typename FullComparer>
typename Set<T, FullComparer>::ConstIterator
Set<T, FullComparer>::last_iterator() const {
	return ConstIterator(
		this, static_cast<const Node*>(this->rbt_.last_const_node()));
}

template<typename T, typename FullComparer>
typename Set<T, FullComparer>::ConstIterator
Set<T, FullComparer>::null_iterator() const {
	return ConstIterator(this, nullptr);
}

template<typename T, typename FullComparer>
typename Set<T, FullComparer>::ConstIterator
Set<T, FullComparer>::first_const_iterator() const {
	return ConstIterator(
		this, static_cast<const Node*>(this->rbt_.first_const_node()));
}

template<typename T, typename FullComparer>
typename Set<T, FullComparer>::ConstIterator
Set<T, FullComparer>::last_const_iterator() const {
	return ConstIterator(
		this, static_cast<const Node*>(this->rbt_.last_const_node()));
}

template<typename T, typename FullComparer>
typename Set<T, FullComparer>::ConstIterator
Set<T, FullComparer>::null_const_iterator() const {
	return ConstIterator(this, nullptr);
}

#///////////////////////////////////////////////////////////////////////////////

template<typename T, typename FullComparer>
template<typename... FullComparerConstructArgs>
Set<T, FullComparer>::Set(
	FullComparerConstructArgs&&... full_cmper_construct_args):
	rbt_(Forward<FullComparerConstructArgs>(full_cmper_construct_args)...) {}

template<typename T, typename FullComparer>
Set<T, FullComparer>::Set(const Set& set) {
	this->rbt_.size_ = set.rbt_.size_;
	if (set.rbt_.root_ != nullptr) {
		this->Copy_(this->rbt_.root_ = New<Node>(set.rbt_.root_->value),
					set.rbt_.root_);
	}
}

template<typename T, typename FullComparer>
Set<T, FullComparer>::Set(Set&& set):
	rbt_(Move(set.rbt_)), pool_(Move(set.pool_)) {}

template<typename T, typename FullComparer> Set<T, FullComparer>::~Set() {
	this->rbt_.size_ = 0;

	if (this->rbt_.root_ != nullptr) {
		Destruct_(static_cast<Node*>(this->rbt_.root_));
		this->rbt_.root_ = nullptr;
	}
}

#///////////////////////////////////////////////////////////////////////////////

template<typename T, typename FullComparer>
void Set<T, FullComparer>::Copy_(Node* n, const Node* m) {
	const Node* ml(static_cast<const Node*>(m->l_));
	const Node* mr(static_cast<const Node*>(m->r_));

	if (ml != nullptr) {
		Node* nl(New<Node>(ml->value));
		n->l_ = nl;
		nl->p_ = n;
		nl->color_ = ml->color_;

		Copy_(nl, ml);
	}

	if (mr != nullptr) {
		Node* nr(New<Node>(mr->value));
		n->r_ = nr;
		nr->p_ = n;
		nr->color_ = mr->color_;

		Copy_(nr, mr);
	}
}

template<typename T, typename FullComparer>
void Set<T, FullComparer>::CopyWithPool_(Node* n, const Node* m) {
	const Node* ml(static_cast<const Node*>(m->l_));
	const Node* mr(static_cast<const Node*>(m->r_));

	if (ml != nullptr) {
		Node* nl(static_cast<Node*>(this->pool_.Pop()));
		n->l_ = nl;
		nl->p_ = n;
		nl->l_ = nullptr;
		nl->r_ = nullptr;
		nl->color_ = ml->color_;
		new (&nl->value) T(ml->value);

		if (this->pool_.empty()) {
			Copy_(nl, ml);
		} else {
			this->CopyWithPool_(nl, ml);
		}
	}

	if (mr != nullptr) {
		Node* nr(static_cast<Node*>(this->pool_.Pop()));
		n->r_ = nr;
		nr->p_ = n;
		nr->l_ = nullptr;
		nr->r_ = nullptr;
		nr->color_ = mr->color_;
		new (&nr->value) T(mr->value);

		if (this->pool_.empty()) {
			Copy_(nr, mr);
		} else {
			this->CopyWithPool_(nr, mr);
		}
	}
}

template<typename T, typename FullComparer>
void Set<T, FullComparer>::Destruct_(Node* n) {
	Node* nl(static_cast<Node*>(n->l_));
	Node* nr(static_cast<Node*>(n->r_));
	if (nl != nullptr) { Destruct_(nl); }
	if (nr != nullptr) { Destruct_(nr); }
	n->value.~T();
	Free(n);
}

#///////////////////////////////////////////////////////////////////////////////

template<typename T, typename FullComparer>
template<typename... Args>
Set<T, FullComparer> Set<T, FullComparer>::Make(Args&&... args) {
	Set set;
	Make_(set, Forward<Args>(args)...);
}

template<typename T, typename FullComparer>
template<typename X, typename... Args>
void Set<T, FullComparer>::Make_(Set& set, X&& x, Args&&... args) {
	set.rbt_.Insert(Forward<X>(x));
	Make_(set, Forward<Args>(args)...);
}

template<typename T, typename FullComparer>
void Set<T, FullComparer>::Make_(Set& set) {}

#///////////////////////////////////////////////////////////////////////////////

#define PHI__EnPool_nl                                                         \
	this->EnPool_(nl);                                                         \
	n->l_ = nullptr;

#define PHI__EnPool_nr                                                         \
	this->EnPool_(nr);                                                         \
	n->r_ = nullptr;

template<typename T, typename FullComparer>
Set<T, FullComparer>& Set<T, FullComparer>::operator=(const Set& set) {
	this->rbt_.full_cmper_ = set.rbt_.full_cmper_;

	if (set.rbt_.root_ == nullptr) {
		this->Clear();
		return *this;
	}

	this->rbt_.size_ = set.rbt_.size_;

	if (this->rbt_.root_ == nullptr) {
		this->rbt_.root_ = new (this->pool_.Pop())
			Node(static_cast<Node*>(set.rbt_.root_)->value);

		if (this->pool_.empty()) {
			this->Copy_(static_cast<Node*>(this->rbt_.root_),
						static_cast<Node*>(set.rbt_.root_));
		} else {
			this->CopyWithPool_(static_cast<Node*>(this->rbt_.root_),
								static_cast<Node*>(set.rbt_.root_));
		}
	} else {
		static_cast<Node*>(this->rbt_.root_)->value =
			static_cast<Node*>(set.rbt_.root_)->value;
		this->Assign_(static_cast<Node*>(this->rbt_.root_),
					  static_cast<Node*>(set.rbt_.root_));
	}

	return *this;
}

template<typename T, typename FullComparer>
void Set<T, FullComparer>::Assign_(Node* n, const Node* m) {
	Node* nl(static_cast<Node*>(n->l_));
	Node* nr(static_cast<Node*>(n->r_));

	const Node* ml(static_cast<const Node*>(m->l_));
	const Node* mr(static_cast<const Node*>(m->r_));

	if (ml == nullptr) {
		if (mr == nullptr) {
			if (nl != nullptr) { PHI__EnPool_nl; }
			if (nr != nullptr) { PHI__EnPool_nr; }
		} else {
			if (nl == nullptr) {
				if (nr == nullptr) {
					nr = static_cast<Node*>(this->pool_.Pop());
					n->r_ = nr;
					nr->p_ = n;
					nr->l_ = nullptr;
					nr->r_ = nullptr;
					nr->color_ = mr->color_;
					new (&nr->value) T(mr->value);

					if (this->pool_.empty()) {
						Copy_(nr, mr);
					} else {
						this->CopyWithPool_(nr, mr);
					}
				} else {
					nr->color_ = mr->color_;
					nr->value = mr->value;
					this->Assign_(nr, mr);
				}
			} else {
				if (nr == nullptr) {
					n->r_ = nr = nl;
					n->l_ = nl = nullptr;
					nr->color_ = mr->color_;
					nr->value = mr->value;
				} else {
					PHI__EnPool_nl;
					nr->color_ = mr->color_;
					nr->value = mr->value;
				}

				this->Assign_(nr, mr);
			}
		}
	} else {
		if (nl == nullptr) {
			if (nr == nullptr) {
				nl = static_cast<Node*>(this->pool_.Pop());
				n->l_ = nl;
				nl->p_ = n;
				nl->l_ = nullptr;
				nl->r_ = nullptr;
				nl->color_ = ml->color_;
				new (&nl->value) T(ml->value);

				if (this->pool_.empty()) {
					Copy_(nl, ml);
				} else {
					this->CopyWithPool_(nl, ml);
				}
			} else {
				n->l_ = nl = nr;
				n->r_ = nr = nullptr;

				nl->color_ = ml->color_;
				nl->value = ml->value;

				this->Assign_(nl, ml);
			}
		} else {
			nl->color_ = ml->color_;
			nl->value = ml->value;
			this->Assign_(nl, ml);
		}

		if (mr == nullptr) {
			if (nr != nullptr) { PHI__EnPool_nr; }
		} else {
			if (nr == nullptr) {
				nr = static_cast<Node*>(this->pool_.Pop());
				n->r_ = nr;
				nr->p_ = n;
				nr->l_ = nullptr;
				nr->r_ = nullptr;
				nr->color_ = mr->color_;
				new (&nr->value) T(mr->value);

				if (this->pool_.empty()) {
					Copy_(nr, mr);
				} else {
					this->CopyWithPool_(nr, mr);
				}
			} else {
				nr->color_ = mr->color_;
				nr->value = mr->value;
				this->Assign_(nr, mr);
			}
		}
	}
}

template<typename T, typename FullComparer>
void Set<T, FullComparer>::EnPool_(Node* n) {
	Node* nl(static_cast<Node*>(n->l_));
	Node* nr(static_cast<Node*>(n->r_));
	if (nl != nullptr) { this->EnPool_(nl); }
	if (nr != nullptr) { this->EnPool_(nr); }
	n->value.~T();
	this->pool_.Push(n);
}

#undef PHI__EnPool_nl
#undef PHI__EnPool_nr

#///////////////////////////////////////////////////////////////////////////////

template<typename T, typename FullComparer>
bool Set<T, FullComparer>::operator==(const Set& set) const {
	return this->rbt_ == set.rbt_;
}

#///////////////////////////////////////////////////////////////////////////////

template<typename T, typename FullComparer>
template<typename Index>
bool Set<T, FullComparer>::Contain(const Index& index) const {
	return this->rbt_.Contain(index);
}

#///////////////////////////////////////////////////////////////////////////////

template<typename T, typename FullComparer>
template<typename Index>
typename Set<T, FullComparer>::Iterator
Set<T, FullComparer>::Find(const Index& index) {
	return Iterator(this, static_cast<Node*>(this->rbt_.Find_(index)));
}

template<typename T, typename FullComparer>
template<typename Index>
typename Set<T, FullComparer>::ConstIterator
Set<T, FullComparer>::Find(const Index& index) const {
	return ConstIterator(this,
						 static_cast<const Node*>(this->rbt_.Find_(index)));
}

#///////////////////////////////////////////////////////////////////////////////

template<typename T, typename FullComparer>
template<typename... Args>
pair<typename Set<T, FullComparer>::Iterator, bool>
Set<T, FullComparer>::Insert(Args&&... args) {
	Node* node(new (this->pool_.Pop()) Node(Forward<Args>(args)...));
	Node* n(static_cast<Node*>(this->rbt_.Insert(node)));

	if (node == n) {
		return pair<Iterator, bool>(Iterator(this, static_cast<Node*>(node)),
									true);
	}

	this->pool_.Push(node);
	return pair<Iterator, bool>(Iterator(this, n), false);
}

#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////

template<typename T, typename FullComparer>
void Set<T, FullComparer>::Release(const Iterator& iter) {
	PHI__debug_if(this != iter.rbt_) { PHI__throw__local("iter error"); }
	if (iter.node_ != nullptr) { this->rbt_.Release_(iter.node_); }
}

template<typename T, typename FullComparer>
typename Set<T, FullComparer>::Iterator
Set<T, FullComparer>::Erase(const Iterator& iter) {
	PHI__debug_if(this != iter.rbt_) { PHI__throw__local("iter error"); }
	if (iter.node_ == nullptr) { return Iterator(this, nullptr); }

	Node* next_node(static_cast<Node*>(iter.node_->next()));
	this->rbt_.Release_(iter.node_);
	this->pool_.Push(iter.node_);
	return Iterator(this, next_node);
}

template<typename T, typename FullComparer>
template<typename Index>
bool Set<T, FullComparer>::FindErase(const Index& index) {
	Node* node(static_cast<Node*>(this->rbt_.Find_(index)));
	if (node == nullptr) { return false; }
	this->rbt_.Release_(node);
	this->pool_.Push(node);
	return true;
}

template<typename T, typename FullComparer> void Set<T, FullComparer>::Clear() {
	if (this->rbt_.root_ == nullptr) { return; }
	this->rbt_.size_ = 0;
	this->EnPool_(static_cast<Node*>(this->rbt_.root_));
	this->rbt_.root_ = nullptr;
}

#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////

template<typename T, typename FullComparer>
Set<T, FullComparer>::Iterator::Iterator(const Iterator& iter):
	set_(iter.set_), node_(iter.node_) {}

template<typename T, typename FullComparer>
Set<T, FullComparer>::Iterator::Iterator(Set* set, Node* node):
	set_(set), node_(node) {}

#///////////////////////////////////////////////////////////////////////////////

template<typename T, typename FullComparer>
typename Set<T, FullComparer>::Iterator&
Set<T, FullComparer>::Iterator::operator=(const Iterator& iter) {
	this->set_ = iter.set_;
	this->node_ = iter.node_;
	return *this;
}

#///////////////////////////////////////////////////////////////////////////////

template<typename T, typename FullComparer>
bool Set<T, FullComparer>::Iterator::operator==(const Iterator& iter) const {
	return this->node_ == iter.node_ && this->set_ == iter.set_;
}

template<typename T, typename FullComparer>
bool Set<T, FullComparer>::Iterator::operator!=(const Iterator& iter) const {
	return this->node_ != iter.node_ || this->set_ != iter.set_;
}

template<typename T, typename FullComparer>
bool Set<T, FullComparer>::Iterator::operator==(
	const ConstIterator& const_iter) const {
	return this->node_ == const_iter.node_ && this->set_ == const_iter.set_;
}

template<typename T, typename FullComparer>
bool Set<T, FullComparer>::Iterator::operator!=(
	const ConstIterator& const_iter) const {
	return this->node_ != const_iter.node_ || this->set_ != const_iter.set_;
}

#///////////////////////////////////////////////////////////////////////////////

template<typename T, typename FullComparer>
T& Set<T, FullComparer>::Iterator::operator*() const {
	return this->node_->value;
}

template<typename T, typename FullComparer>
T* Set<T, FullComparer>::Iterator::operator->() const {
	return &this->node_->value;
}

#///////////////////////////////////////////////////////////////////////////////

template<typename T, typename FullComparer>
typename Set<T, FullComparer>::Iterator&
Set<T, FullComparer>::Iterator::operator++() {
	PHI__debug_if(this->set_ == nullptr) { PHI__throw__local("iter error"); }

	this->node_ = this->node_ == nullptr
					  ? this->set_->first_node_()
					  : static_cast<Node*>(this->node_->next());

	return *this;
}

template<typename T, typename FullComparer>
typename Set<T, FullComparer>::Iterator&
Set<T, FullComparer>::Iterator::operator--() {
	PHI__debug_if(this->set_ == nullptr) { PHI__throw__local("iter error"); }

	this->node_ = this->node_ == nullptr
					  ? this->set_->last_node_()
					  : static_cast<Node*>(this->node_->prev());

	return *this;
}

#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////

template<typename T, typename FullComparer>
Set<T, FullComparer>::ConstIterator::ConstIterator(const Iterator& iter):
	set_(iter.set_), node_(iter.node_) {}

template<typename T, typename FullComparer>
Set<T, FullComparer>::ConstIterator::ConstIterator(
	const ConstIterator& cont_iter):
	set_(cont_iter.set_),
	node_(cont_iter.node_) {}

template<typename T, typename FullComparer>
Set<T, FullComparer>::ConstIterator::ConstIterator(const Set* set,
												   const Node* node):
	set_(set),
	node_(node) {}

#///////////////////////////////////////////////////////////////////////////////

template<typename T, typename FullComparer>
typename Set<T, FullComparer>::ConstIterator&
Set<T, FullComparer>::ConstIterator::operator=(const Iterator& iter) {
	this->set_ = iter.set_;
	this->node_ = iter.node_;
	return *this;
}

template<typename T, typename FullComparer>
typename Set<T, FullComparer>::ConstIterator&
Set<T, FullComparer>::ConstIterator::operator=(
	const ConstIterator& const_iter) {
	this->set_ = const_iter.set_;
	this->node_ = const_iter.node_;
	return *this;
}

#///////////////////////////////////////////////////////////////////////////////

template<typename T, typename FullComparer>
bool Set<T, FullComparer>::ConstIterator::operator==(
	const Iterator& iter) const {
	return this->node_ == iter.node_ && this->set_ == iter.set_;
}

template<typename T, typename FullComparer>
bool Set<T, FullComparer>::ConstIterator::operator!=(
	const Iterator& iter) const {
	return this->node_ != iter.node_ || this->set_ != iter.set_;
}

template<typename T, typename FullComparer>
bool Set<T, FullComparer>::ConstIterator::operator==(
	const ConstIterator& const_iter) const {
	return this->node_ == const_iter.node_ && this->set_ == const_iter.set_;
}

template<typename T, typename FullComparer>
bool Set<T, FullComparer>::ConstIterator::operator!=(
	const ConstIterator& const_iter) const {
	return this->node_ != const_iter.node_ || this->set_ != const_iter.set_;
}

#///////////////////////////////////////////////////////////////////////////////

template<typename T, typename FullComparer>
const T& Set<T, FullComparer>::ConstIterator::operator*() const {
	PHI__debug_if(this->set_ == nullptr) { PHI__throw__local("iter error"); }
	return this->node_->value;
}

template<typename T, typename FullComparer>
const T* Set<T, FullComparer>::ConstIterator::operator->() const {
	PHI__debug_if(this->set_ == nullptr) { PHI__throw__local("iter error"); }
	return &this->node_->value;
}

#///////////////////////////////////////////////////////////////////////////////

template<typename T, typename FullComparer>
typename Set<T, FullComparer>::ConstIterator&
Set<T, FullComparer>::ConstIterator::operator++() {
	PHI__debug_if(this->set_ == nullptr) { PHI__throw__local("iter error"); }

	this->node_ = this->node_ == nullptr
					  ? this->set_->first_node_()
					  : static_cast<const Node*>(this->node_->next());

	return *this;
}

template<typename T, typename FullComparer>
typename Set<T, FullComparer>::ConstIterator&
Set<T, FullComparer>::ConstIterator::operator--() {
	PHI__debug_if(this->set_ == nullptr) { PHI__throw__local("iter error"); }

	this->node_ = this->node_ == nullptr
					  ? this->set_->last_node_()
					  : static_cast<const Node*>(this->node_->prev());

	return *this;
}

}
}

#undef PHI__throw__local

#endif
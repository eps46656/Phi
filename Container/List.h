#ifndef PHI__define_guard__Container__List_h
#define PHI__define_guard__Container__List_h

#include "../Utility/memory_op.h"
#include "DoublyNode.h"
#include "Pool.h"

#define PHI__throw__local(desc) PHI__throw(cntr::List, __func__, desc)

namespace phi {
namespace cntr {

template<typename T> class List {
public:
	struct Node;

	struct Node: public DoublyNode {
		T value;

		template<typename... Args> Node(Args&&... args);

		void PushNextAll(DoublyNode* node);
		void PushNextAllExcept(DoublyNode* node);
		void PushPrevAll(DoublyNode* node);
		void PushPrevAllExcept(DoublyNode* node);
	};

#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////

	struct Iterator;
	struct ConstIterator;

	struct Iterator {
	public:
		friend class List;

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
		List* list_;
		Node* node_;

		Iterator(List* list, DoublyNode* node);
	};

	struct ConstIterator {
	public:
		friend class List;

		ConstIterator(const Iterator& iter);
		ConstIterator(const ConstIterator& iter);

		ConstIterator& operator=(const Iterator& iter);
		ConstIterator& operator=(const ConstIterator& const_iter);

		bool operator==(const Iterator& iter) const;
		bool operator!=(const Iterator& iter) const;
		bool operator==(const ConstIterator& const_iter) const;
		bool operator!=(const ConstIterator& const_iter) const;

		const T& operator*() const;
		const T* operator->() const;

		ConstIterator& operator++();
		ConstIterator& operator--();

	private:
		const List* list_;
		const Node* node_;

		ConstIterator(const List* list, const DoublyNode* node);
	};

#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////

	size_t size() const;
	bool empty() const;

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

	List();
	List(const List& list);
	List(List&& list);

	~List();

#///////////////////////////////////////////////////////////////////////////////

	List& operator=(const List& list);
	List& operator=(List&& list);

#///////////////////////////////////////////////////////////////////////////////

	T& front();
	const T& front() const;

	T& back();
	const T& back() const;

#///////////////////////////////////////////////////////////////////////////////

	template<typename... Args> List& PushFront(Args&&... args);
	template<typename... Args> List& PushBack(Args&&... args);

	List& PushNodeFront(Node* node);
	List& PushNodeBack(Node* node);

	List& PushListFront(List& list);
	List& PushListBack(List& list);

#///////////////////////////////////////////////////////////////////////////////

	List& PopFront();
	List& PopBack();
	List& Pop(const Iterator& iter);

private:
	size_t size_;
	DoublyNode node_;
	UncountedPool<sizeof(Node)> pool_;
};

#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////

template<typename T>
template<typename... Args>
List<T>::Node::Node(Args&&... args): value(Forward<Args>(args)...) {}

#///////////////////////////////////////////////////////////////////////////////

template<typename T> void List<T>::Node::PushNextAll(DoublyNode* node) {
	this->DoublyNode::PushNextAll(node);
}

template<typename T> void List<T>::Node::PushNextAllExcept(DoublyNode* node) {
	this->DoublyNode::PushNextAllExcept(node);
}

template<typename T> void List<T>::Node::PushPrevAll(DoublyNode* node) {
	this->DoublyNode::PushPrevAll(node);
}

template<typename T> void List<T>::Node::PushPrevAllExcept(DoublyNode* node) {
	this->DoublyNode::PushPrevAllExcept(node);
}

#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////

template<typename T> size_t List<T>::size() const { return this->size_; }
template<typename T> bool List<T>::empty() const { return this->size_ == 0; }

#///////////////////////////////////////////////////////////////////////////////

template<typename T> typename List<T>::Iterator List<T>::first_iterator() {
	return Iterator(this, this->node_.next());
}
template<typename T> typename List<T>::Iterator List<T>::last_iterator() {
	return Iterator(this, this->node_.prev());
}
template<typename T> typename List<T>::Iterator List<T>::null_iterator() {
	return Iterator(this, &this->node_);
}

template<typename T>
typename List<T>::ConstIterator List<T>::first_iterator() const {
	return ConstIterator(this, this->node_.next());
}
template<typename T>
typename List<T>::ConstIterator List<T>::last_iterator() const {
	return ConstIterator(this, this->node_.prev());
}
template<typename T>
typename List<T>::ConstIterator List<T>::null_iterator() const {
	return ConstIterator(this, &this->node_);
}

template<typename T>
typename List<T>::ConstIterator List<T>::first_const_iterator() {
	return ConstIterator(this, this->node_.next());
}
template<typename T>
typename List<T>::ConstIterator List<T>::last_const_iterator() {
	return ConstIterator(this, this->node_.prev());
}
template<typename T>
typename List<T>::ConstIterator List<T>::null_const_iterator() {
	return ConstIterator(this, &this->node_);
}

#///////////////////////////////////////////////////////////////////////////////

template<typename T> List<T>::List(): size_(0) {}

template<typename T> List<T>::List(const List<T>& list): size_(list.size_) {
	for (Node* i(list.node_.next()); i != list.node_;
		 i = static_cast<Node*>(i->next())) {
		this->node_.PushPrev(New<Node>(i->value));
	}
}

template<typename T> List<T>::List(List<T>&& list): size_(list.size_) {
	list.size_ = 0;
	Node::Swap(this->node_, list.node_);
	this->pool_.PushPrevAllExcept(&list.pool_);
}

template<typename T> List<T>::~List() {
	this->size_ = 0;
	while (!this->node_.sole()) { Delete(this->node_.prev()); }
}

#///////////////////////////////////////////////////////////////////////////////

template<typename T> List<T>& List<T>::operator=(const List& list) {
	Node* i(static_cast<Node*>(this->node_.next()));
	Node* j(static_cast<Node*>(list.node_.next()));

	if (this->size_ < list.size_) {
		for (; i != &this->node_; i = static_cast<Node*>(i->next()),
								  j = static_cast<Node*>(j->next())) {
			i->value = j->value;
		}

		for (; !this->pool_.empty() && j != &list.node_;
			 j = static_cast<Node*>(j->next())) {
			this->node_.PushPrev(new (this->pool_.Pop()) Node(j->value));
		}

		for (; j != &list.node_; j = static_cast<Node*>(j->next())) {
			this->node_.PushPrev(New<Node>(j->value));
		}
	} else {
		for (; j != &list.node_; i = static_cast<Node*>(i->next()),
								 j = static_cast<Node*>(j->next())) {
			i->value = j->value;
		}

		Node* node;

		while ((node = static_cast<Node*>(this->node_.prev())) != i) {
			node->value.~T();
			this->pool_.PushPrev(node->Pop());
		}
	}

	this->size_ = list.size_;

	return *this;
}

template<typename T> List<T>& List<T>::operator=(List&& list) {
	this->size_ = list.size_;

	while (!this->node_.sole()) {
		Node* i(static_cast<Node*>(this->node_.prev()));
		i->value.~T();
		this->pool_.PushPrev(i);
	}

	Node::Swap(this->node_, list.node_);
	list.pool_.TtansferTo(this->pool_);

	return *this;
}

#///////////////////////////////////////////////////////////////////////////////

template<typename T> T& List<T>::front() {
	PHI__debug_if(this->size_ == 0) { PHI__throw__local("index error"); }
	return static_cast<Node*>(this->node_.next())->value;
}

template<typename T> const T& List<T>::front() const {
	PHI__debug_if(this->size_ == 0) { PHI__throw__local("index error"); }
	return static_cast<Node*>(this->node_.next())->value;
}

template<typename T> T& List<T>::back() {
	PHI__debug_if(this->size_ == 0) { PHI__throw__local("index error"); }
	return static_cast<Node*>(this->end_->prev())->value;
}

template<typename T> const T& List<T>::back() const {
	PHI__debug_if(this->size_ == 0) { PHI__throw__local("index error"); }
	return static_cast<Node*>(this->end_->prev())->value;
}

#///////////////////////////////////////////////////////////////////////////////

template<typename T>
template<typename... Args>
List<T>& List<T>::PushFront(Args&&... args) {
	++this->size_;
	this->node_.PushNext(new (this->pool_.Pop()) Node(Forward<Args>(args)...));
	return *this;
}
template<typename T>
template<typename... Args>
List<T>& List<T>::PushBack(Args&&... args) {
	++this->size_;
	this->node_.PushPrev(new (this->pool_.Pop()) Node(Forward<Args>(args)...));
	return *this;
}

template<typename T> List<T>& List<T>::PushNodeFront(Node* node) {
	PHI__debug_if(!node->sole()) { PHI__throw__local("node error"); }
	++this->size_;
	this->node_.PushNext(node);
	return *this;
}
template<typename T> List<T>& List<T>::PushNodeBack(Node* node) {
	PHI__debug_if(!node->sole()) { PHI__throw__local("node error"); }
	++this->size_;
	this->node_.PushPrev(node);
	return *this;
}

template<typename T> List<T>& List<T>::PushListFront(List<T>& list) {
	this->size_ += list.size_;
	list.size_ = 0;
	this->node_.PushNextAllExcept(&list.node_);
	return *this;
}
template<typename T> List<T>& List<T>::PushListBack(List<T>& list) {
	this->size_ += list.size_;
	list.size_ = 0;
	this->node_.PushPrevAllExcept(&list.node_);
	return *this;
}

#///////////////////////////////////////////////////////////////////////////////

template<typename T> List<T>& List<T>::PopFront() {
	PHI__debug_if(!this->size_) { PHI__throw__local("size error"); }
	--this->size_;
	Node* node(static_cast<Node*>(this->node_.next()->Pop()));
	node->value.~T();
	this->pool_.Push(node);
	return *this;
}

template<typename T> List<T>& List<T>::PopBack() {
	PHI__debug_if(!this->size_) { PHI__throw__local("size error"); }
	--this->size_;
	Node* node(static_cast<Node*>(this->node_.prev()->Pop()));
	node->value.~T();
	this->pool_.Push(node);
	return *this;
}

template<typename T> List<T>& List<T>::Pop(const Iterator& iter) {
	PHI__debug_if(this != iter.list_ || iter.node_ == nullptr ||
				  iter.node_ == &this->node_) {
		PHI__throw__local("iter error");
	}

	--this->size_;
	static_cast<Node*>(iter.node_->Pop())->value.~T();
	this->pool_.Push(iter.node_);
	const_cast<Iterator&>(iter).node_ = nullptr;
	return *this;
}

#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////

template<typename T>
List<T>::Iterator::Iterator(const Iterator& iter):
	list_(iter.list_), node_(iter.node_) {}

template<typename T>
List<T>::Iterator::Iterator(List<T>* list, DoublyNode* node):
	list_(list), node_(static_cast<Node*>(node)) {}

#///////////////////////////////////////////////////////////////////////////////

template<typename T>
typename List<T>::Iterator& List<T>::Iterator::operator=(const Iterator& iter) {
	this->list_ = iter.list_;
	this->node_ = iter.node_;
	return *this;
}

#///////////////////////////////////////////////////////////////////////////////

template<typename T>
bool List<T>::Iterator::operator==(const Iterator& iter) const {
	return this->node_ == iter.node_ && this->list_ == iter.list_;
}
template<typename T>
bool List<T>::Iterator::operator!=(const Iterator& iter) const {
	return this->node_ != iter.node_ || this->list_ != iter.list_;
}

template<typename T>
bool List<T>::Iterator::operator==(const ConstIterator& const_iter) const {
	return this->node_ == const_iter.node_ && this->list_ == const_iter.list_;
}
template<typename T>
bool List<T>::Iterator::operator!=(const ConstIterator& const_iter) const {
	return this->node_ != const_iter.node_ || this->list_ != const_iter.list_;
}

#///////////////////////////////////////////////////////////////////////////////

template<typename T> T& List<T>::Iterator::operator*() const {
	return this->node_->value;
}
template<typename T> T* List<T>::Iterator::operator->() const {
	return &this->node_->value;
}

#///////////////////////////////////////////////////////////////////////////////

template<typename T>
typename List<T>::Iterator& List<T>::Iterator::operator++() {
	this->node_ = static_cast<Node*>(this->node_->next());
	return *this;
}

template<typename T>
typename List<T>::Iterator& List<T>::Iterator::operator--() {
	this->node_ = static_cast<Node*>(this->node_->prev());
	return *this;
}

#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////

template<typename T>
List<T>::ConstIterator::ConstIterator(const Iterator& iter):
	list_(iter.list_), node_(iter.node_) {}

template<typename T>
List<T>::ConstIterator::ConstIterator(const ConstIterator& const_iter):
	list_(const_iter.list_), node_(const_iter.node_) {}

template<typename T>
List<T>::ConstIterator::ConstIterator(const List<T>* list,
									  const DoublyNode* node):
	list_(list),
	node_(static_cast<const Node*>(node)) {}

#///////////////////////////////////////////////////////////////////////////////

template<typename T>
typename List<T>::ConstIterator&
List<T>::ConstIterator::operator=(const Iterator& iter) {
	this->list_ = iter.list_;
	this->node_ = iter.node_;
	return *this;
}

template<typename T>
typename List<T>::ConstIterator&
List<T>::ConstIterator::operator=(const ConstIterator& const_iter) {
	this->list_ = const_iter.list_;
	this->node_ = const_iter.node_;
	return *this;
}

#///////////////////////////////////////////////////////////////////////////////

template<typename T>
bool List<T>::ConstIterator::operator==(const List<T>::Iterator& iter) const {
	return this->node_ == iter.node_ && this->list_ == iter.list_;
}
template<typename T>
bool List<T>::ConstIterator::operator!=(const List<T>::Iterator& iter) const {
	return this->node_ != iter.node_ || this->list_ != iter.list_;
}

template<typename T>
bool List<T>::ConstIterator::operator==(
	const List<T>::ConstIterator& const_iter) const {
	return this->node_ == const_iter.node_ && this->list_ == const_iter.list_;
}
template<typename T>
bool List<T>::ConstIterator::operator!=(
	const List<T>::ConstIterator& const_iter) const {
	return this->node_ != const_iter.node_ || this->list_ != const_iter.list_;
}

#///////////////////////////////////////////////////////////////////////////////

template<typename T> const T& List<T>::ConstIterator::operator*() const {
	return this->node_->value;
}
template<typename T> const T* List<T>::ConstIterator::operator->() const {
	return &this->node_->value;
}

#///////////////////////////////////////////////////////////////////////////////

template<typename T>
typename List<T>::ConstIterator& List<T>::ConstIterator::operator++() {
	this->node_ = static_cast<const Node*>(this->node_->next());
	return *this;
}

template<typename T>
typename List<T>::ConstIterator& List<T>::ConstIterator::operator--() {
	this->node_ = static_cast<const Node*>(this->node_->prev());
	return *this;
}

}
}

#undef PHI__throw__local

#endif
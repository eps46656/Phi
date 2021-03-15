#ifndef PHI__define_guard__Container__DoublyNode_h
#define PHI__define_guard__Container__DoublyNode_h

#include "../define.h"
#include "../Utility/swap.h"

namespace phi {
namespace cntr {

struct DoublyNode {
	inline DoublyNode* prev() const;
	inline DoublyNode* next() const;

	inline bool sole() const;
	inline bool Contain(const DoublyNode* node) const;

#///////////////////////////////////////////////////////////////////////////////

	inline DoublyNode();
	inline virtual ~DoublyNode();

#///////////////////////////////////////////////////////////////////////////////

	inline void PushPrev(DoublyNode* node);
	inline void PushPrevNew(void* node_memory);

	inline void PushNext(DoublyNode* node);
	inline void PushNextNew(void* node_memory);

	inline DoublyNode* Pop();

	inline void Replace(DoublyNode* node);

	inline void ReverseAll();

	inline static void Swap(DoublyNode& x, DoublyNode& y);

protected:
	inline void PushPrevAll(DoublyNode* node);
	inline void PushPrevAllExcept(DoublyNode* node);

	inline void PushNextAll(DoublyNode* node);
	inline void PushNextAllExcept(DoublyNode* node);

private:
	DoublyNode* prev_;
	DoublyNode* next_;

	inline static void Link_(DoublyNode* x, DoublyNode* y);
};

#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////

DoublyNode* DoublyNode::prev() const { return this->prev_; }
DoublyNode* DoublyNode::next() const { return this->next_; }

bool DoublyNode::sole() const { return this->prev_ == this; }

bool DoublyNode::Contain(const DoublyNode* node) const {
	if (node == this) { return true; }

	for (DoublyNode* i(this->next_); i != this; i = i->next_) {
		if (i == node) { return true; }
	}

	return false;
}

#///////////////////////////////////////////////////////////////////////////////

DoublyNode::DoublyNode(): prev_(this), next_(this) {}
DoublyNode::~DoublyNode() { this->Pop(); }

#///////////////////////////////////////////////////////////////////////////////

void DoublyNode::Link_(DoublyNode* x, DoublyNode* y) {
	x->next_ = y;
	y->prev_ = x;
}

#///////////////////////////////////////////////////////////////////////////////

void DoublyNode::PushPrev(DoublyNode* node) {
	if (node == this || node == this->prev_) { return; }
	Link_(node->prev_, node->next_);
	Link_(this->prev_, node);
	Link_(node, this);
}

void DoublyNode::PushPrevNew(void* node_memory) {
	DoublyNode* node(static_cast<DoublyNode*>(node_memory));
	Link_(this->prev_, node);
	Link_(node, this);
}

void DoublyNode::PushPrevAll(DoublyNode* node) {
	Link_(this->prev_, node->next_);
	Link_(node, this);
}

void DoublyNode::PushPrevAllExcept(DoublyNode* node) {
	if (node->sole()) { return; }
	Link_(this->prev_, node->next_);
	Link_(node->prev_, this);
	node->prev_ = node->next_ = node;
}

void DoublyNode::PushNext(DoublyNode* node) {
	if (node == this || node == this->next_) { return; }
	Link_(node->prev_, node->next_);
	Link_(node, this->next_);
	Link_(this, node);
}

void DoublyNode::PushNextNew(void* node_block) {
	DoublyNode* node(static_cast<DoublyNode*>(node_block));
	Link_(node, this->next_);
	Link_(this, node);
}

void DoublyNode::PushNextAll(DoublyNode* node) {
	Link_(node->prev_, this->next_);
	Link_(this, node);
}

void DoublyNode::PushNextAllExcept(DoublyNode* node) {
	if (node->sole()) { return; }
	Link_(node->prev_, this->next_);
	Link_(this, node->next_);
	node->prev_ = node->next_ = node;
}

#///////////////////////////////////////////////////////////////////////////////

DoublyNode* DoublyNode::Pop() {
	if (this->sole()) { return this; }
	Link_(this->prev_, this->next_);
	this->prev_ = this->next_ = this;
	return this;
}

void DoublyNode::Replace(DoublyNode* node) {
	if (node == this) { return; }
	Link_(node->prev_, node->next_);
	Link_(this->prev_, node);
	Link_(node, this->next_);
	this->prev_ = this->next_ = this;
}

void DoublyNode::ReverseAll() {
	phi::Swap(this->prev_, this->next_);

	for (DoublyNode* i(this->next_); i != this; i = i->next_) {
		phi::Swap(i->prev_, i->next_);
	}
}

void DoublyNode::Swap(DoublyNode& x, DoublyNode& y) {
	if (&x == &y) { return; }

	DoublyNode* x_prev_(x.prev_);
	DoublyNode* y_prev_(y.prev_);

	if (x_prev_ == &y) {
		Link_(y_prev_, &x);
		Link_(&y, x.next_);
		Link_(&x, &y);
	} else if (y_prev_ == &x) {
		Link_(x_prev_, &y);
		Link_(&x, y.next_);
		Link_(&y, &x);
	} else {
		DoublyNode* x_next_(x.next_);
		DoublyNode* y_next_(y.next_);

		Link_(x_prev_, &y);
		Link_(&y, x_next_);
		Link_(y_prev_, &x);
		Link_(&x, y_next_);
	}
}

}
}

#endif

#ifndef PHI__define_guard__Container__RedBlackTree_h
#define PHI__define_guard__Container__RedBlackTree_h

#include "RedBlackTreeNode.h"

#define PHI__throw__local(desc) PHI__throw(cntr::RedBlackTree, __func__, desc);

namespace phi {
namespace cntr {

template<typename FullComparer> class RedBlackTree {
public:
	static constexpr bool black = RedBlackTreeNode::black;
	static constexpr bool red = RedBlackTreeNode::red;

	size_t size() const;
	bool empty() const;

	FullComparer& full_cmper();
	const FullComparer& full_cmper() const;

#///////////////////////////////////////////////////////////////////////////////

	RedBlackTreeNode* first_node();
	RedBlackTreeNode* last_node();

	const RedBlackTreeNode* first_node() const;
	const RedBlackTreeNode* last_node() const;

	const RedBlackTreeNode* first_const_node() const;
	const RedBlackTreeNode* last_const_node() const;

#///////////////////////////////////////////////////////////////////////////////

	template<typename... FullComparerConstructArgs>
	RedBlackTree(FullComparerConstructArgs&&... full_cmper_construct_args);

	RedBlackTree(RedBlackTree&& rbt);

	~RedBlackTree();

#///////////////////////////////////////////////////////////////////////////////

	template<typename... RedBlackTreeNodes>
	static RedBlackTree Make(RedBlackTreeNodes... nodes);

#///////////////////////////////////////////////////////////////////////////////

	RedBlackTree& operator=(RedBlackTree&& rbt);

#///////////////////////////////////////////////////////////////////////////////

	bool operator==(const RedBlackTree& rbt) const;

#///////////////////////////////////////////////////////////////////////////////

	template<typename Index> bool Contain(const Index& index) const;

#///////////////////////////////////////////////////////////////////////////////

	template<typename Index> RedBlackTreeNode* Find(const Index& index);
	template<typename Index>
	const RedBlackTreeNode* Find(const Index& index) const;

#///////////////////////////////////////////////////////////////////////////////

	bool Insert(RedBlackTreeNode* node);

#///////////////////////////////////////////////////////////////////////////////

	void Release(RedBlackTreeNode* node);
	template<typename Index> bool FindRelease(const Index& index);

	void ReleaseAll();

	void Check() const;

protected:
	size_t size_;
	RedBlackTreeNode* root_;

	FullComparer full_cmper_;

	RedBlackTreeNode* first_node_() const;
	RedBlackTreeNode* last_node_() const;

	template<typename... RedBlackTreeNodes>
	static void Make_(RedBlackTree& rbt, RedBlackTreeNode* node,
					  RedBlackTreeNodes... nodes);

	static void Make_(RedBlackTree& rbt);

	template<typename Index> RedBlackTreeNode* Find_(const Index& index) const;
	bool Insert_(RedBlackTreeNode* node);
	void Release_(RedBlackTreeNode* node);

	static void Clear_(RedBlackTreeNode* node);
};

#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////

template<typename FullComparer>
size_t RedBlackTree<FullComparer>::size() const {
	return this->size_;
}

template<typename FullComparer> bool RedBlackTree<FullComparer>::empty() const {
	return this->size_ == 0;
}

template<typename FullComparer>
FullComparer& RedBlackTree<FullComparer>::full_cmper() {
	return this->full_cmper_;
}

template<typename FullComparer>
const FullComparer& RedBlackTree<FullComparer>::full_cmper() const {
	return this->full_cmper_;
}

#///////////////////////////////////////////////////////////////////////////////

template<typename FullComparer>
RedBlackTreeNode* RedBlackTree<FullComparer>::first_node_() const {
	return this->root_ == nullptr
			   ? nullptr
			   : static_cast<RedBlackTreeNode*>(this->root_->most_l());
}

template<typename FullComparer>
RedBlackTreeNode* RedBlackTree<FullComparer>::last_node_() const {
	return this->root_ == nullptr
			   ? nullptr
			   : static_cast<RedBlackTreeNode*>(this->root_->most_r());
}

#///////////////////////////////////////////////////////////////////////////////

template<typename FullComparer>
RedBlackTreeNode* RedBlackTree<FullComparer>::first_node() {
	return this->first_node_();
}

template<typename FullComparer>
RedBlackTreeNode* RedBlackTree<FullComparer>::last_node() {
	return this->last_node_();
}

template<typename FullComparer>
const RedBlackTreeNode* RedBlackTree<FullComparer>::first_node() const {
	return this->first_node_();
}

template<typename FullComparer>
const RedBlackTreeNode* RedBlackTree<FullComparer>::last_node() const {
	return this->last_node_();
}

template<typename FullComparer>
const RedBlackTreeNode* RedBlackTree<FullComparer>::first_const_node() const {
	return this->first_node_();
}

template<typename FullComparer>
const RedBlackTreeNode* RedBlackTree<FullComparer>::last_const_node() const {
	return this->last_node_();
}

#///////////////////////////////////////////////////////////////////////////////

template<typename FullComparer>
template<typename... FullComparerConstructArgs>
RedBlackTree<FullComparer>::RedBlackTree(
	FullComparerConstructArgs&&... full_cmper_construct_args):
	size_(0),
	root_(nullptr), full_cmper_(Forward<FullComparerConstructArgs>(
						full_cmper_construct_args)...) {}

template<typename FullComparer>
RedBlackTree<FullComparer>::RedBlackTree(RedBlackTree&& rbt):
	size_(rbt.size_), root_(rbt.root_), full_cmper_(Move(rbt.full_cmper_)) {
	rbt.size_ = 0;
	rbt.root_ = nullptr;
}

template<typename FullComparer> RedBlackTree<FullComparer>::~RedBlackTree() {
	this->ReleaseAll();
}

#///////////////////////////////////////////////////////////////////////////////

template<typename FullComparer>
template<typename... RedBlackTreeNodes>
RedBlackTree<FullComparer>
RedBlackTree<FullComparer>::Make(RedBlackTreeNodes... nodes) {
	RedBlackTree rbt;
	Make_(rbt, nodes...);
}

template<typename FullComparer>
template<typename... RedBlackTreeNodes>
void RedBlackTree<FullComparer>::Make_(RedBlackTree& rbt,
									   RedBlackTreeNode* node,
									   RedBlackTreeNodes... nodes) {
	rbt.Insert(node);
	Make_(rbt, nodes...);
}

template<typename FullComparer>
void RedBlackTree<FullComparer>::Make_(RedBlackTree& rbt) {}

#///////////////////////////////////////////////////////////////////////////////

template<typename FullComparer>
bool RedBlackTree<FullComparer>::operator==(const RedBlackTree& rbt) const {
	if (this == &rbt) { return true; }
	if (this->size_ != rbt.size_) { return false; }

	const RedBlackTreeNode* n(this->first_node());
	const RedBlackTreeNode* m(rbt.first_node());

	for (; n; n = n->next(), m = m->next()) {
		if (this->full_cmper_(n, m) != 0) { return false; }
	}

	return true;
}

#///////////////////////////////////////////////////////////////////////////////

template<typename FullComparer>
template<typename Index>
bool RedBlackTree<FullComparer>::Contain(const Index& index) const {
	return this->Find_(index) != nullptr;
}

#///////////////////////////////////////////////////////////////////////////////

template<typename FullComparer>
template<typename Index>
RedBlackTreeNode* RedBlackTree<FullComparer>::Find_(const Index& index) const {
	for (RedBlackTreeNode* node(this->root_); node != nullptr;) {
		switch (this->full_cmper_(index, node)) {
			case -1: node = static_cast<RedBlackTreeNode*>(node->l()); break;
			case 1: node = static_cast<RedBlackTreeNode*>(node->r()); break;
			case 0: return node;
		}
	}

	return nullptr;
}

template<typename FullComparer>
template<typename Index>
RedBlackTreeNode* RedBlackTree<FullComparer>::Find(const Index& index) {
	return this->Find_(index);
}

template<typename FullComparer>
template<typename Index>
const RedBlackTreeNode*
RedBlackTree<FullComparer>::Find(const Index& index) const {
	return this->Find_(index);
}

#///////////////////////////////////////////////////////////////////////////////

template<typename FullComparer>
bool RedBlackTree<FullComparer>::Insert_(RedBlackTreeNode* node) {
	if (this->size_ == 0) {
		++this->size_;
		this->root_ = node;

		return true;
	}

	for (RedBlackTreeNode* n(this->root_);;) {
		switch (this->full_cmper_(node, n)) {
			case -1:
				if (n->l() == nullptr) {
					n->InsertL(node);
					goto insert_complete;
				}

				n = static_cast<RedBlackTreeNode*>(n->l());
				break;
			case 1:
				if (n->r() == nullptr) {
					n->InsertR(node);
					goto insert_complete;
				}

				n = static_cast<RedBlackTreeNode*>(n->r());
				break;
			case 0: return false;
		}
	}

insert_complete:;

	++this->size_;
	this->root_ = static_cast<RedBlackTreeNode*>(this->root_->most_p());

	return true;
}

template<typename FullComparer>
bool RedBlackTree<FullComparer>::Insert(RedBlackTreeNode* node) {
	PHI__debug_if(!node->sole()) { PHI__throw__local("node is not sole"); }
	return this->Insert_(node);
}

#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////

template<typename FullComparer>
void RedBlackTree<FullComparer>::Release_(RedBlackTreeNode* node) {
	if (--this->size_ == 0) {
		this->root_ = nullptr;
	} else {
		if (this->root_ == node) {
			if (this->root_->l() != nullptr) {
				this->root_ = static_cast<RedBlackTreeNode*>(this->root_->l());
			} else {
				this->root_ = static_cast<RedBlackTreeNode*>(this->root_->r());
			}
		}

		node->Release();
		this->root_ = static_cast<RedBlackTreeNode*>(this->root_->most_p());
	}
}

template<typename FullComparer>
void RedBlackTree<FullComparer>::Release(RedBlackTreeNode* node) {
	PHI__debug_if(this->root_ != node->root()) {
		PHI__throw__local("node is not in this tree");
	}

	this->Release_(node);
}

template<typename FullComparer>
template<typename Index>
bool RedBlackTree<FullComparer>::FindRelease(const Index& index) {
	RedBlackTreeNode* node(this->Find_(index));
	if (node == nullptr) { return false; }
	this->Release_(node);
	return true;
}

template<typename FullComparer> void RedBlackTree<FullComparer>::ReleaseAll() {
	if (this->root_ == nullptr) { return; }
	this->size_ = 0;
	this->root_->ReleaseAll();
	this->root_ = nullptr;
}

template<typename FullComparer> void RedBlackTree<FullComparer>::Check() const {
	this->root_->Check();
}

}
}

#undef PHI__throw__local

#endif
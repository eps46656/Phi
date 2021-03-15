#ifndef PHI__define_guard__Container__ChainingHashTable_h
#define PHI__define_guard__Container__ChainingHashTable_h

#include "../Utility/compare.h"
#include "DoublyNode.h"
#include "Pool.h"

#define PHI__throw__local(desc)                                                \
	PHI__throw(cntr::ChainingHashTable, __func__, desc);

namespace phi {
namespace cntr {

template<bool judge, typename T> struct DefaultHashFunction_case_a_ {
	static hash_t F(const T& value) { return 0; }
};

template<typename T> struct DefaultHashFunction_case_a_<true, T> {
	static hash_t F(const T& value) {
		hash_t r(0);
		const char* x(reinterpret_cast<const char*>(&value));

		for (size_t i(0); i != sizeof(T); ++i) {
			reinterpret_cast<char*>(&r)[i] +=
				reinterpret_cast<const hash_t&>(x[i]);
		}

		return r;
	}
};

template<bool judge, typename T> struct DefaultHashFunction_case_b_ {
	static hash_t F(const T& value) { return 0; }
};

template<typename T> struct DefaultHashFunction_case_b_<false, T> {
	static hash_t F(const T& value) {
		hash_t r(0);
		const char* x(reinterpret_cast<const char*>(&value));

		for (size_t i(0); i <= sizeof(T) - sizeof(hash_t); ++i) {
			r += reinterpret_cast<const hash_t&>(x[i]);
		}

		return r;
	}
};

template<typename T> hash_t DefaultHashFunction(const T& value) {
	// static hash_t r_init(Random() % UnsignedIntMax<hash_t>());

	/*
	hash_t r(r_init);
	const char* x(reinterpret_cast<const char*>(&value));

	if constexpr (sizeof(T) <= sizeof(hash_t)) {
		for (size_t i(0); i != sizeof(T); ++i) {
			reinterpret_cast<char*>(&r)[i] ^=
				reinterpret_cast<const hash_t&>(x[i]);
		}
	} else {
		for (size_t i(0); i <= sizeof(T) - sizeof(hash_t); ++i) {
			std::cout << "i : " << i << "\n";
			r ^= reinterpret_cast<const hash_t&>(x[i]);
		}
	}

	return r;*/

	return DefaultHashFunction_case_a_<sizeof(T) <= sizeof(hash_t), T>::F(
			   value) +
		   DefaultHashFunction_case_b_<sizeof(T) <= sizeof(hash_t), T>::F(
			   value);
}

#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////

struct DefaultHasher {
	template<typename T> hash_t operator()(const T& value) const {
		return DefaultHashFunction(value);
	}
};

#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////

template<typename T, typename Hasher = DefaultHasher,
		 typename EqualComparer = DefaultEqualComparer>
class ChainingHashTable {
public:
	struct BucketNode: public DoublyNode {
		size_t num;

		BucketNode();
	};

	struct Node: public DoublyNode {
		hash_t hash_code;
		T value;

		template<typename... Args> Node(Args&&... args);
	};

#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////

	class Iterator;
	class ConstIterator;

	class Iterator {
	public:
		friend class ChainingHashTable;

		Iterator(const Iterator& iter);

		Iterator& operator=(const Iterator& iter);

		bool operator==(const Iterator& iter) const;
		bool operator!=(const Iterator& iter) const;
		bool operator==(const ConstIterator& const_iter) const;
		bool operator!=(const ConstIterator& const_iter) const;

		T& operator*() const;
		T* operator->() const;

		Iterator& operator--();
		Iterator& operator++();

	private:
		ChainingHashTable* cht_;
		Node* node_;

		Iterator(ChainingHashTable* cht, Node* node);
	};

	class ConstIterator {
	public:
		friend class ChainingHashTable;

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

		ConstIterator& operator--();
		ConstIterator& operator++();

	private:
		const ChainingHashTable* cht_;
		const Node* node_;

		ConstIterator(const ChainingHashTable* cht, const Node* node);
	};

#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////

	size_t size() const;
	bool empty() const;

	size_t bucket_size() const;

	size_t eval_num() const;
	double eval_num_over_size() const;

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

	ChainingHashTable(Hasher hasher = Hasher(),
					  EqualComparer eq_cmper = EqualComparer());

#///////////////////////////////////////////////////////////////////////////////

	static size_t CompressHashCode(hash_t hash_code, size_t bucket_size);
	static size_t BucketSizeShouldSet(size_t min_bucket_size);

#///////////////////////////////////////////////////////////////////////////////

	void Restruct();

	template<typename Key> bool Contain(const Key& key) const;

	template<typename Key> Iterator Find(const Key& key);
	template<typename Key> ConstIterator Find(const Key& key) const;

	template<typename... Args> void Insert(Args&&... args);

	void Erase(const Iterator& iter);
	Iterator EraseThenReturnNextIterator(const Iterator& iter);
	Iterator EraseThenReturnPrevIterator(const Iterator& iter);

	template<typename Key> bool FindErase(const Key& key);

	bool IsValid(const Node* node) const;

private:
	size_t bucket_size_;
	BucketNode* bucket_;
	size_t size_;

	size_t eval_num_;

	Hasher hasher_;
	EqualComparer eq_cmper_;

	UncountedPool<sizeof(Node)> pool_;

	size_t first_node_index_;
	size_t last_node_index_;

	Node* first_node_() const;
	Node* last_node_() const;

	Node* prev_node_(const Node* node) const;
	Node* next_node_(const Node* node) const;

	void ReCalcEvalNum_();

	template<typename Key>
	Node* Find_(BucketNode* bucket_node, const Key& key) const;

	void Erase_(size_t index, Node* node);
};

#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////

template<typename T, typename Hasher, typename EqualComparer>
ChainingHashTable<T, Hasher, EqualComparer>::BucketNode::BucketNode(): num(0) {}

#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////

template<typename T, typename Hasher, typename EqualComparer>
template<typename... Args>
ChainingHashTable<T, Hasher, EqualComparer>::Node::Node(Args&&... args):
	value(Forward<Args>(args)...) {}

#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////

template<typename T, typename Hasher, typename EqualComparer>
size_t ChainingHashTable<T, Hasher, EqualComparer>::size() const {
	return this->size_;
}

template<typename T, typename Hasher, typename EqualComparer>
bool ChainingHashTable<T, Hasher, EqualComparer>::empty() const {
	return this->size_ == 0;
}

template<typename T, typename Hasher, typename EqualComparer>
size_t ChainingHashTable<T, Hasher, EqualComparer>::bucket_size() const {
	return this->bucket_size_;
}

template<typename T, typename Hasher, typename EqualComparer>
size_t ChainingHashTable<T, Hasher, EqualComparer>::eval_num() const {
	return this->eval_num_;
}

template<typename T, typename Hasher, typename EqualComparer>
double ChainingHashTable<T, Hasher, EqualComparer>::eval_num_over_size() const {
	return this->size_ == 0 ? 0 : double(this->eval_num_) / this->size_;
}

#///////////////////////////////////////////////////////////////////////////////

template<typename T, typename Hasher, typename EqualComparer>
typename ChainingHashTable<T, Hasher, EqualComparer>::Node*
ChainingHashTable<T, Hasher, EqualComparer>::first_node_() const {
	return this->size_ == 0
			   ? nullptr
			   : static_cast<Node*>(
					 this->bucket_[this->first_node_index_].next());
}

template<typename T, typename Hasher, typename EqualComparer>
typename ChainingHashTable<T, Hasher, EqualComparer>::Node*
ChainingHashTable<T, Hasher, EqualComparer>::last_node_() const {
	return this->size_ == 0 ? nullptr
							: static_cast<Node*>(
								  this->bucket_[this->last_node_index_].prev());
}

template<typename T, typename Hasher, typename EqualComparer>
typename ChainingHashTable<T, Hasher, EqualComparer>::Node*
ChainingHashTable<T, Hasher, EqualComparer>::next_node_(
	const Node* node) const {
	if (node == nullptr) { return this->first_node_(); }

	DoublyNode* d_node(node->next());
	size_t d_node_addr(PHI__void_ptr_addr(d_node));
	size_t bucket_addr(PHI__void_ptr_addr(this->bucket_));

	if (d_node_addr < bucket_addr) { return static_cast<Node*>(d_node); }

	size_t bucket_index((d_node_addr - bucket_addr) / sizeof(BucketNode));

	if (this->bucket_size_ <= bucket_index) {
		return static_cast<Node*>(d_node);
	}

	if (bucket_index == this->last_node_index_) { return nullptr; }

	for (size_t i(bucket_index + 1); i != this->last_node_index_; ++i) {
		if (this->bucket_[i].num != 0) {
			return static_cast<Node*>(this->bucket_[i].next());
		}
	}

	return static_cast<Node*>(this->bucket_[this->last_node_index_].next());
}

template<typename T, typename Hasher, typename EqualComparer>
typename ChainingHashTable<T, Hasher, EqualComparer>::Node*
ChainingHashTable<T, Hasher, EqualComparer>::prev_node_(
	const Node* node) const {
	if (node == nullptr) { return this->last_node_(); }

	DoublyNode* d_node(node->prev());
	size_t d_node_addr(PHI__void_ptr_addr(d_node));
	size_t bucket_addr(PHI__void_ptr_addr(this->bucket_));

	if (d_node_addr < bucket_addr) { return static_cast<Node*>(d_node); }

	size_t bucket_index((d_node_addr - bucket_addr) / sizeof(BucketNode));

	if (this->bucket_size_ <= bucket_index) {
		return static_cast<Node*>(d_node);
	}

	if (bucket_index == this->first_node_index_) { return nullptr; }

	for (size_t i(bucket_index - 1); i != this->first_node_index_; --i) {
		if (this->bucket_[i].num != 0) {
			return static_cast<Node*>(this->bucket_[i].prev());
		}
	}

	return static_cast<Node*>(this->bucket_[this->first_node_index_].prev());
}

#///////////////////////////////////////////////////////////////////////////////

template<typename T, typename Hasher, typename EqualComparer>
typename ChainingHashTable<T, Hasher, EqualComparer>::Iterator
ChainingHashTable<T, Hasher, EqualComparer>::first_iterator() {
	return Iterator(this, this->first_node_());
}

template<typename T, typename Hasher, typename EqualComparer>
typename ChainingHashTable<T, Hasher, EqualComparer>::Iterator
ChainingHashTable<T, Hasher, EqualComparer>::last_iterator() {
	return Iterator(this, this->last_node_());
}

template<typename T, typename Hasher, typename EqualComparer>
typename ChainingHashTable<T, Hasher, EqualComparer>::Iterator
ChainingHashTable<T, Hasher, EqualComparer>::null_iterator() {
	return Iterator(this, nullptr);
}

template<typename T, typename Hasher, typename EqualComparer>
typename ChainingHashTable<T, Hasher, EqualComparer>::ConstIterator
ChainingHashTable<T, Hasher, EqualComparer>::first_iterator() const {
	return ConstIterator(this, this->first_node_());
}

template<typename T, typename Hasher, typename EqualComparer>
typename ChainingHashTable<T, Hasher, EqualComparer>::ConstIterator
ChainingHashTable<T, Hasher, EqualComparer>::last_iterator() const {
	return ConstIterator(this, this->last_node_());
}

template<typename T, typename Hasher, typename EqualComparer>
typename ChainingHashTable<T, Hasher, EqualComparer>::ConstIterator
ChainingHashTable<T, Hasher, EqualComparer>::null_iterator() const {
	return ConstIterator(this, nullptr);
}

template<typename T, typename Hasher, typename EqualComparer>
typename ChainingHashTable<T, Hasher, EqualComparer>::ConstIterator
ChainingHashTable<T, Hasher, EqualComparer>::first_const_iterator() {
	return ConstIterator(this, this->first_node_());
}

template<typename T, typename Hasher, typename EqualComparer>
typename ChainingHashTable<T, Hasher, EqualComparer>::ConstIterator
ChainingHashTable<T, Hasher, EqualComparer>::last_const_iterator() {
	return ConstIterator(this, this->last_node_());
}

template<typename T, typename Hasher, typename EqualComparer>
typename ChainingHashTable<T, Hasher, EqualComparer>::ConstIterator
ChainingHashTable<T, Hasher, EqualComparer>::null_const_iterator() {
	return ConstIterator(this, nullptr);
}

#///////////////////////////////////////////////////////////////////////////////

#define PHI__default_bucket_size (101)

template<typename T, typename Hasher, typename EqualComparer>
ChainingHashTable<T, Hasher, EqualComparer>::ChainingHashTable(
	Hasher hasher, EqualComparer eq_cmper):
	bucket_size_(PHI__default_bucket_size),
	bucket_(Malloc<BucketNode>(PHI__default_bucket_size)), size_(0),
	eval_num_(0), hasher_(hasher), eq_cmper_(eq_cmper) {
	for (size_t i(0); i != this->bucket_size_; ++i) {
		new (this->bucket_ + i) BucketNode();
	}
}

#///////////////////////////////////////////////////////////////////////////////

template<typename T, typename Hasher, typename EqualComparer>
size_t ChainingHashTable<T, Hasher, EqualComparer>::CompressHashCode(
	hash_t hash_code, size_t bucket_size) {
	size_t r(hash_code);

	/*
	size_t prime(5053);

	if (this->bucket_size_ < prime) { r %= prime; }
	*/

	return r % bucket_size;
}

#///////////////////////////////////////////////////////////////////////////////

template<typename T, typename Hasher, typename EqualComparer>
void ChainingHashTable<T, Hasher, EqualComparer>::ReCalcEvalNum_() {
	this->eval_num_ = 0;

	for (size_t i(0); i != this->bucket_size_; ++i) {
		this->eval_num_ +=
			this->bucket_[i].num * (this->bucket_[i].num + 1) / 2;
	}
}

#///////////////////////////////////////////////////////////////////////////////

inline bool is_prime(size_t odd) {
	for (size_t i(3);; i += 2) {
		size_t a(odd / i);
		if (odd == a * i) { return false; }
		if (a < i) { break; }
	}

	return true;
}

template<typename T, typename Hasher, typename EqualComparer>
size_t
ChainingHashTable<T, Hasher, EqualComparer>::BucketSizeShouldSet(size_t size) {
	// size_t r(size * 2);
	// if (r % 2 == 0) { ++r; }
	size_t r(size * 2 + 1); // r must be odd
	while (!is_prime(r)) { r += 2; }
	return r;
}

#///////////////////////////////////////////////////////////////////////////////

template<typename T, typename Hasher, typename EqualComparer>
void ChainingHashTable<T, Hasher, EqualComparer>::Restruct() {
	size_t new_bucket_size(BucketSizeShouldSet(
		this->size_ < this->bucket_size_ ? this->bucket_size_ : this->size_));

	BucketNode* new_bucket(Malloc<BucketNode>(new_bucket_size));

	for (size_t i(0); i != new_bucket_size; ++i) {
		new (new_bucket + i) BucketNode();
	}

	size_t i(0);

	for (; i != this->bucket_size_; ++i) {
		BucketNode* bucket_node(this->bucket_ + i);

		if (!bucket_node->sole()) {
			Node* node(static_cast<Node*>(bucket_node->next()->Pop()));
			size_t index(CompressHashCode(node->hash_code, new_bucket_size));

			++new_bucket[index].num;
			new_bucket[index].PushPrev(node);

			this->first_node_index_ = index;
			this->last_node_index_ = index;

			break;
		}
	}

	for (; i != this->bucket_size_; ++i) {
		BucketNode* bucket_node(this->bucket_ + i);

		while (!bucket_node->sole()) {
			Node* node(static_cast<Node*>(bucket_node->next()->Pop()));
			size_t index(CompressHashCode(node->hash_code, new_bucket_size));

			++new_bucket[index].num;
			new_bucket[index].PushPrev(node);

			if (index < this->first_node_index_) {
				this->first_node_index_ = index;
			} else if (this->last_node_index_ < index) {
				this->last_node_index_ = index;
			}
		}
	}

	this->bucket_size_ = new_bucket_size;

	Delete(this->bucket_);
	this->bucket_ = new_bucket;

	this->ReCalcEvalNum_();
}

#///////////////////////////////////////////////////////////////////////////////

template<typename T, typename Hasher, typename EqualComparer>
template<typename Key>
typename ChainingHashTable<T, Hasher, EqualComparer>::Node*
ChainingHashTable<T, Hasher, EqualComparer>::Find_(BucketNode* bucket_node,
												   const Key& key) const {
	for (Node* i(static_cast<Node*>(bucket_node->next()));
		 static_cast<void*>(i) != static_cast<void*>(bucket_node);
		 i = static_cast<Node*>(i->next())) {
		if (this->eq_cmper_.eq(i->value, key)) { return i; }
	}

	return nullptr;
}

#///////////////////////////////////////////////////////////////////////////////

template<typename T, typename Hasher, typename EqualComparer>
template<typename Key>
bool ChainingHashTable<T, Hasher, EqualComparer>::Contain(
	const Key& key) const {
	return this->size_ != 0 &&
		   this->Find_(this->bucket_ +
						   this->CompressHashCode(
							   this->hasher_(static_cast<const T&>(key)),
							   this->bucket_size_),
					   key) != nullptr;
}

#///////////////////////////////////////////////////////////////////////////////

template<typename T, typename Hasher, typename EqualComparer>
template<typename Key>
typename ChainingHashTable<T, Hasher, EqualComparer>::Iterator
ChainingHashTable<T, Hasher, EqualComparer>::Find(const Key& key) {
	size_t index(CompressHashCode(this->hasher_(static_cast<const T&>(key)),
								  this->bucket_size_));

	return Iterator(this, this->Find_(index, key));
}

template<typename T, typename Hasher, typename EqualComparer>
template<typename Key>
typename ChainingHashTable<T, Hasher, EqualComparer>::ConstIterator
ChainingHashTable<T, Hasher, EqualComparer>::Find(const Key& key) const {
	size_t index(CompressHashCode(this->hasher_(static_cast<const T&>(key)),
								  this->bucket_size_));

	return ConstIterator(this, this->Find_(index, key));
}

#///////////////////////////////////////////////////////////////////////////////

#define PHI__restruct_threshold 2

template<typename T, typename Hasher, typename EqualComparer>
template<typename... Args>
void ChainingHashTable<T, Hasher, EqualComparer>::Insert(Args&&... args) {
	Node* node(new (this->pool_.Pop()) Node(Forward<Args>(args)...));

	size_t index(this->CompressHashCode(
		node->hash_code = this->hasher_(node->value), this->bucket_size_));

	BucketNode* bucket_node(this->bucket_ + index);

	if (this->size_ != 0 && this->Find_(bucket_node, node->value) != nullptr) {
		node->value.~T();
		this->pool_.Push(node);
		return;
	}

	if (this->size_ == 0) {
		this->first_node_index_ = index;
		this->last_node_index_ = index;
	} else if (index < this->first_node_index_) {
		this->first_node_index_ = index;
	} else if (this->last_node_index_ < index) {
		this->last_node_index_ = index;
	}

	++this->size_;

	this->eval_num_ += bucket_node->num + 1;
	// - n(n+1)/2 + (n+1)(n+2)/2
	// = n + 1

	++bucket_node->num;
	bucket_node->PushPrev(node);

	// if (PHI__restruct_threshold <= this->eval_num_ / this->size_)
	if (PHI__restruct_threshold * this->size_ <= this->eval_num_) {
		this->Restruct();
	}
}

#///////////////////////////////////////////////////////////////////////////////

template<typename T, typename Hasher, typename EqualComparer>
void ChainingHashTable<T, Hasher, EqualComparer>::Erase_(size_t index,
														 Node* node) {
	node->~Node();
	this->pool_.Push(node);

	// this->eval_num_ -=
	// 	this->bucket_[index].num ^ 2 - (this->bucket_[index].num - 1) ^ 2;

	this->eval_num_ -= this->bucket_[index].num;
	// - n(n+1)/2 + (n-1)n/2
	// = - n

	if (--this->size_ == 0 || --this->bucket_[index].num != 0) { return; }

	if (this->size_ == 1) {
		if (index == this->first_node_index_) {
			this->first_node_index_ = this->last_node_index_;
		} else {
			this->last_node_index_ = this->first_node_index_;
		}
	} else if (index == this->first_node_index_) {
		do {
			++this->first_node_index_;
		} while (this->first_node_index_ != this->last_node_index_ &&
				 this->bucket_[this->first_node_index_].num == 0);
	} else if (index == this->last_node_index_) {
		do {
			--this->last_node_index_;
		} while (this->first_node_index_ != this->last_node_index_ &&
				 this->bucket_[this->last_node_index_].num == 0);
	}
}

template<typename T, typename Hasher, typename EqualComparer>
void ChainingHashTable<T, Hasher, EqualComparer>::Erase(const Iterator& iter) {
	PHI__debug_if(this != iter.cht_) { PHI__throw__local("iter error"); }

	if (iter.node_ != nullptr) {
		this->Erase_(
			CompressHashCode(iter.node_->hash_code, this->bucket_size_),
			iter.node_);
	}
}

template<typename T, typename Hasher, typename EqualComparer>
typename ChainingHashTable<T, Hasher, EqualComparer>::Iterator
ChainingHashTable<T, Hasher, EqualComparer>::EraseThenReturnNextIterator(
	const Iterator& iter) {
	PHI__debug_if(this != iter.cht_) { PHI__throw__local("iter error"); }
	Iterator r(this, this->next_node_(iter.node_));

	if (iter.node_ != nullptr) {
		this->Erase_(
			CompressHashCode(iter.node_->hash_code, this->bucket_size_),
			iter.node_);
	}

	return r;
}

template<typename T, typename Hasher, typename EqualComparer>
typename ChainingHashTable<T, Hasher, EqualComparer>::Iterator
ChainingHashTable<T, Hasher, EqualComparer>::EraseThenReturnPrevIterator(
	const Iterator& iter) {
	PHI__debug_if(this != iter.cht_) { PHI__throw__local("iter error"); }
	Iterator r(this, this->prev_node_(iter.node_));

	if (iter.node_ != nullptr) {
		this->Erase_(
			CompressHashCode(iter.node_->hash_code, this->bucket_size_),
			iter.node_);
	}

	return r;
}

template<typename T, typename Hasher, typename EqualComparer>
template<typename Key>
bool ChainingHashTable<T, Hasher, EqualComparer>::FindErase(const Key& key) {
	if (this->size_ == 0) { return false; }

	size_t index(CompressHashCode(this->hasher_(static_cast<const T&>(key)),
								  this->bucket_size_));
	Node* node(this->Find_(this->bucket_ + index, key));

	if (node == nullptr) { return false; }

	this->Erase_(index, node);
	return true;
}

#///////////////////////////////////////////////////////////////////////////////

template<typename T, typename Hasher, typename EqualComparer>
bool ChainingHashTable<T, Hasher, EqualComparer>::IsValid(
	const Node* node) const {
	if (this == nullptr) { return false; }
	if (node == nullptr) { return true; }

	size_t index(CompressHahsCode(node->hash_code, this->bucket_size_));

	for (Node* i(static_cast<Node*>(this->bucket_[index].next()));
		 i != this->bucket_ + index; i = static_cast<Node*>(i->next())) {
		if (i == node) { return true; }
	}

	return false;
}

#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////

template<typename T, typename Hasher, typename EqualComparer>
ChainingHashTable<T, Hasher, EqualComparer>::Iterator::Iterator(
	const Iterator& iter):
	cht_(iter.cht_),
	node_(iter.node_) {}

template<typename T, typename Hasher, typename EqualComparer>
ChainingHashTable<T, Hasher, EqualComparer>::Iterator::Iterator(
	ChainingHashTable* cht, Node* node):
	cht_(cht),
	node_(node) {}

#///////////////////////////////////////////////////////////////////////////////

template<typename T, typename Hasher, typename EqualComparer>
typename ChainingHashTable<T, Hasher, EqualComparer>::Iterator&
ChainingHashTable<T, Hasher, EqualComparer>::Iterator::operator=(
	const Iterator& iter) {
	this->cht_ = iter.cht_;
	this->node_ = iter.node_;
	return *this;
}

#///////////////////////////////////////////////////////////////////////////////

template<typename T, typename Hasher, typename EqualComparer>
bool ChainingHashTable<T, Hasher, EqualComparer>::Iterator::operator==(
	const Iterator& iter) const {
	return this->node_ == iter.node_ && this->cht_ == iter.cht_;
}

template<typename T, typename Hasher, typename EqualComparer>
bool ChainingHashTable<T, Hasher, EqualComparer>::Iterator::operator!=(
	const Iterator& iter) const {
	return this->node_ != iter.node_ || this->cht_ != iter.cht_;
}

template<typename T, typename Hasher, typename EqualComparer>
bool ChainingHashTable<T, Hasher, EqualComparer>::Iterator::operator==(
	const ConstIterator& const_iter) const {
	return this->node_ == const_iter.node_ && this->cht_ == const_iter.cht_;
}

template<typename T, typename Hasher, typename EqualComparer>
bool ChainingHashTable<T, Hasher, EqualComparer>::Iterator::operator!=(
	const ConstIterator& const_iter) const {
	return this->node_ != const_iter.node_ || this->cht_ != const_iter.cht_;
}

#///////////////////////////////////////////////////////////////////////////////

template<typename T, typename Hasher, typename EqualComparer>
T& ChainingHashTable<T, Hasher, EqualComparer>::Iterator::operator*() const {
	return this->node_->value;
}

template<typename T, typename Hasher, typename EqualComparer>
T* ChainingHashTable<T, Hasher, EqualComparer>::Iterator::operator->() const {
	return &this->node_->value;
}

#///////////////////////////////////////////////////////////////////////////////

template<typename T, typename Hasher, typename EqualComparer>
typename ChainingHashTable<T, Hasher, EqualComparer>::Iterator&
ChainingHashTable<T, Hasher, EqualComparer>::Iterator::operator--() {
	this->node_ = this->cht_->prev_node_(this->node_);
	return *this;
}

template<typename T, typename Hasher, typename EqualComparer>
typename ChainingHashTable<T, Hasher, EqualComparer>::Iterator&
ChainingHashTable<T, Hasher, EqualComparer>::Iterator::operator++() {
	this->node_ = this->cht_->next_node_(this->node_);
	return *this;
}

#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////

template<typename T, typename Hasher, typename EqualComparer>
ChainingHashTable<T, Hasher, EqualComparer>::ConstIterator::ConstIterator(
	const Iterator& iter):
	cht_(iter.cht_),
	node_(iter.node_) {}

template<typename T, typename Hasher, typename EqualComparer>
ChainingHashTable<T, Hasher, EqualComparer>::ConstIterator::ConstIterator(
	const ConstIterator& cont_iter):
	cht_(cont_iter.cht_),
	node_(cont_iter.node_) {}

template<typename T, typename Hasher, typename EqualComparer>
ChainingHashTable<T, Hasher, EqualComparer>::ConstIterator::ConstIterator(
	const ChainingHashTable* cht, const Node* node):
	cht_(cht),
	node_(node) {}

#///////////////////////////////////////////////////////////////////////////////

template<typename T, typename Hasher, typename EqualComparer>
typename ChainingHashTable<T, Hasher, EqualComparer>::ConstIterator&
ChainingHashTable<T, Hasher, EqualComparer>::ConstIterator::operator=(
	const Iterator& iter) {
	this->cht_ = iter.cht_;
	this->node_ = iter.node_;
	return *this;
}

template<typename T, typename Hasher, typename EqualComparer>
typename ChainingHashTable<T, Hasher, EqualComparer>::ConstIterator&
ChainingHashTable<T, Hasher, EqualComparer>::ConstIterator::operator=(
	const ConstIterator& const_iter) {
	this->cht_ = const_iter.cht_;
	this->node_ = const_iter.node_;
	return *this;
}

#///////////////////////////////////////////////////////////////////////////////

template<typename T, typename Hasher, typename EqualComparer>
bool ChainingHashTable<T, Hasher, EqualComparer>::ConstIterator::operator==(
	const Iterator& iter) const {
	return this->node_ == iter.node_ && this->cht_ == iter.cht_;
}

template<typename T, typename Hasher, typename EqualComparer>
bool ChainingHashTable<T, Hasher, EqualComparer>::ConstIterator::operator!=(
	const Iterator& iter) const {
	return this->node_ != iter.node_ || this->cht_ != iter.cht_;
}

template<typename T, typename Hasher, typename EqualComparer>
bool ChainingHashTable<T, Hasher, EqualComparer>::ConstIterator::operator==(
	const ConstIterator& const_iter) const {
	return this->node_ == const_iter.node_ && this->cht_ == const_iter.cht_;
}

template<typename T, typename Hasher, typename EqualComparer>
bool ChainingHashTable<T, Hasher, EqualComparer>::ConstIterator::operator!=(
	const ConstIterator& const_iter) const {
	return this->node_ != const_iter.node_ || this->cht_ != const_iter.cht_;
}

#///////////////////////////////////////////////////////////////////////////////

template<typename T, typename Hasher, typename EqualComparer>
const T&
ChainingHashTable<T, Hasher, EqualComparer>::ConstIterator::operator*() const {
	return this->node_->value;
}

template<typename T, typename Hasher, typename EqualComparer>
const T*
ChainingHashTable<T, Hasher, EqualComparer>::ConstIterator::operator->() const {
	return &this->node_->value;
}

#///////////////////////////////////////////////////////////////////////////////

template<typename T, typename Hasher, typename EqualComparer>
typename ChainingHashTable<T, Hasher, EqualComparer>::ConstIterator&
ChainingHashTable<T, Hasher, EqualComparer>::ConstIterator::operator--() {
	this->node_ = this->cht_->prev_node_(this->node_);
	return *this;
}

template<typename T, typename Hasher, typename EqualComparer>
typename ChainingHashTable<T, Hasher, EqualComparer>::ConstIterator&
ChainingHashTable<T, Hasher, EqualComparer>::ConstIterator::operator++() {
	this->node_ = this->cht_->next_node_(this->node_);
	return *this;
}

}
}

#undef PHI__throw__local

#endif
#ifndef PHI__define_guard__Container__Vector_h
#define PHI__define_guard__Container__Vector_h

#include "../Utility/memory.h"
#include "../Utility/compare.h"
#include "../Utility/swap.h"
#include "../Utility/iterator.h"

#define PHI__throw__local(desc) PHI__throw(cntr::Vector<T>, __func__, desc)

namespace phi {
namespace cntr {

template<typename T> class Vector {
public:
	struct Iterator;
	struct ConstIterator;

	class Iterator {
		friend class Vector;

	public:
		using Type = iterator::Type::RandomAccess;
		using Value = T;
		using Ref = T&;
		using Ptr = T*;
		using Diff = diff_t;

		Iterator(const Iterator& iter);

		operator size_t() const;

		Iterator& operator=(const Iterator& iter);

		bool operator<(const Iterator& iter) const;
		bool operator<(const ConstIterator& iter) const;

		bool operator==(const Iterator& iter) const;
		bool operator!=(const Iterator& iter) const;
		bool operator==(const ConstIterator& const_iter) const;
		bool operator!=(const ConstIterator& const_iter) const;

		Ref operator*() const;
		Ptr operator->() const;

		Iterator& operator++();
		Iterator& operator--();

		Iterator& operator+=(Diff step);
		Iterator& operator-=(Diff step);

		Iterator operator+(Diff step) const;
		Iterator operator-(Diff step) const;
		Diff operator-(const Iterator& iter) const;

		Ref operator[](Diff index) const;

	private:
		Vector* vector_;
		size_t index_;

		Iterator(Vector* vector, size_t index);
	};

	class ConstIterator {
		friend class Vector;

	public:
		using Type = iterator::Type::RandomAccess;
		using Value = T;
		using Ref = const T&;
		using Ptr = const T*;
		using Diff = diff_t;

		ConstIterator(const Iterator& iter);
		ConstIterator(const ConstIterator& const_iter);

		operator size_t() const;

		ConstIterator& operator=(const Iterator& iter);
		ConstIterator& operator=(const ConstIterator& iter);

		bool operator<(const Iterator& iter) const;
		bool operator<(const ConstIterator& iter) const;

		bool operator==(const Iterator& iter) const;
		bool operator!=(const Iterator& iter) const;
		bool operator==(const ConstIterator& const_iter) const;
		bool operator!=(const ConstIterator& const_iter) const;

		const T& operator*() const;
		const T* operator->() const;

		ConstIterator& operator++();
		ConstIterator& operator--();

		ConstIterator& operator+=(Diff step);
		ConstIterator& operator-=(Diff step);

		ConstIterator operator+(Diff step) const;
		ConstIterator operator-(Diff step) const;
		Diff operator-(const ConstIterator& const_iterator) const;

		Ref operator[](Diff index) const;

	private:
		const Vector* vector_;
		size_t index_;

		ConstIterator(const Vector* vector, size_t index);
	};

#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////

	size_t size() const;
	size_t capacity() const;
	bool empty() const;

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

	Vector();
	Vector(const Vector& vector);
	Vector(Vector&& vector);
	template<typename BidirectioanlIterator>
	Vector(BidirectioanlIterator begin, BidirectioanlIterator end);

	~Vector();

#///////////////////////////////////////////////////////////////////////////////

	template<typename... Args> static Vector Make(Args&&... args);

#///////////////////////////////////////////////////////////////////////////////

	Vector& operator=(const Vector& vector);
	Vector& operator=(Vector&& vector);

#///////////////////////////////////////////////////////////////////////////////

	template<typename Y> bool operator==(const Vector<Y>& vector) const;
	template<typename Y> bool operator!=(const Vector<Y>& vector) const;

#///////////////////////////////////////////////////////////////////////////////

	T& front();
	const T& front() const;

	T& back();
	const T& back() const;

#///////////////////////////////////////////////////////////////////////////////

	T& operator[](size_t index);
	const T& operator[](size_t index) const;

	T& at(size_t index);
	const T& at(size_t index) const;

#///////////////////////////////////////////////////////////////////////////////

	template<typename... Args> T& Push(Args&&... args);

	template<typename BidirectionalIterator>
	void PushIterator(BidirectionalIterator begin, BidirectionalIterator end);

#///////////////////////////////////////////////////////////////////////////////

	template<typename... Args> void Insert(size_t index, Args&&... args);

	template<typename BidirectionalIterator>
	void Insert(size_t index, BidirectionalIterator begin,
				BidirectionalIterator end);

#///////////////////////////////////////////////////////////////////////////////

	void Pop();
	void Pop(size_t size);

#///////////////////////////////////////////////////////////////////////////////

	void Erase(size_t index);
	void Erase(size_t begin, size_t end);

	template<typename Index, typename EqualComparer = DefaultEqualComparer>
	size_t FindErase(const Index& index,
					 EqualComparer eq_cmper = EqualComparer());

	T* Release();

	void Clear();

#///////////////////////////////////////////////////////////////////////////////

	void Reserve(size_t capacity);

#///////////////////////////////////////////////////////////////////////////////

	static size_t CapacityShouldAlloc(size_t required_capacity);
	static void Swap(Vector& x, Vector& y);

protected:
	size_t size_;
	size_t capacity_;
	T* data_;

	template<size_t i> void Make_();

	template<size_t i, typename X, typename... Args>
	void Make_(X&& x, Args&&... args);

	size_t index_pos_diff_(size_t index, diff_t diff) const;
	size_t index_neg_diff_(size_t index, diff_t diff) const;
};

#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////

template<typename T> size_t Vector<T>::size() const { return this->size_; }
template<typename T> size_t Vector<T>::capacity() const {
	return this->capacity_;
}

template<typename T> bool Vector<T>::empty() const { return this->size_ == 0; }

#///////////////////////////////////////////////////////////////////////////////

template<typename T> typename Vector<T>::Iterator Vector<T>::first_iterator() {
	return Iterator(this, 0);
}
template<typename T> typename Vector<T>::Iterator Vector<T>::last_iterator() {
	return Iterator(this, this->size_ - 1);
}
template<typename T> typename Vector<T>::Iterator Vector<T>::null_iterator() {
	return Iterator(this, this->size_);
}

template<typename T>
typename Vector<T>::ConstIterator Vector<T>::first_iterator() const {
	return ConstIterator(this, 0);
}
template<typename T>
typename Vector<T>::ConstIterator Vector<T>::last_iterator() const {
	return ConstIterator(this, this->size_ - 1);
}
template<typename T>
typename Vector<T>::ConstIterator Vector<T>::null_iterator() const {
	return ConstIterator(this, this->size_);
}

template<typename T>
typename Vector<T>::ConstIterator Vector<T>::first_const_iterator() {
	return ConstIterator(this, 0);
}
template<typename T>
typename Vector<T>::ConstIterator Vector<T>::last_const_iterator() {
	return ConstIterator(this, this->size_ - 1);
}
template<typename T>
typename Vector<T>::ConstIterator Vector<T>::null_const_iterator() {
	return ConstIterator(this, this->size_);
}

#///////////////////////////////////////////////////////////////////////////////

template<typename T>
Vector<T>::Vector(): size_(0), capacity_(0), data_(nullptr) {}

template<typename T>
Vector<T>::Vector(const Vector& vector):
	size_(vector.size_), capacity_(vector.size_),
	data_(Malloc<T>(this->capacity_)) {
	for (size_t i(0); i != this->size_; ++i) {
		new (this->data_ + i) T(vector.data_[i]);
	}
}

template<typename T>
Vector<T>::Vector(Vector&& vector):
	size_(vector.size_), capacity_(vector.capacity_), data_(vector.data_) {
	vector.size_ = vector.capacity_ = 0;
	vector.data_ = nullptr;
}

template<typename T>
template<typename BidirectioanlIterator>
Vector<T>::Vector(BidirectioanlIterator begin, BidirectioanlIterator end):
	size_(Distance(begin, end)), capacity_(CapacityShouldAlloc(this->size_)),
	data_(this->capacity_ ? Malloc<T>(this->capacity_) : nullptr) {
	for (T* i(this->data_); begin != end; ++begin, ++i) { new (i) T(*begin); }
}

template<typename T> Vector<T>::~Vector() {
	for (size_t i(0); i != this->size_; ++i) { this->data_[i].~T(); }
	Free(this->data_);

	this->size_ = 0;
	this->capacity_ = 0;
	this->data_ = nullptr;
}

#///////////////////////////////////////////////////////////////////////////////

template<typename T>
template<typename... Args>
Vector<T> Vector<T>::Make(Args&&... args) {
	Vector r;
	r.data_ =
		Malloc<T>(r.capacity_ = CapacityShouldAlloc(r.size_ = sizeof...(args)));
	r.Make_<0>(Forward<Args>(args)...);

	return r;
}

template<typename T> template<size_t i> void Vector<T>::Make_() {}

template<typename T>
template<size_t i, typename X, typename... Args>
void Vector<T>::Make_(X&& x, Args&&... args) {
	new (this->data_ + i) T(Forward<X>(x));
	this->Make_<i + 1>(Forward<Args>(args)...);
}

#///////////////////////////////////////////////////////////////////////////////

template<typename T> Vector<T>& Vector<T>::operator=(const Vector& vector) {
	if (this == &vector) { return *this; }

	if (this->capacity_ < vector.size_) {
		for (size_t i(0); i != this->size_; ++i) { this->data_[i].~T(); }
		Free(this->data_);
		this->data_ = Malloc<T>(this->size_ = this->capacity_ = vector.size_);

		for (size_t i(0); i != this->size_; ++i) {
			new (this->data_ + i) T(vector.data_[i]);
		}

		return *this;
	}

	size_t i(0);

	if (this->size_ < vector.size_) {
		for (; i != this->size_; ++i) { this->data_[i] = vector.data_[i]; }

		for (; i != vector.size_; ++i) {
			new (this->data_ + i) T(vector.data_[i]);
		}
	} else {
		for (; i != vector.size_; ++i) { this->data_[i] = vector.data_[i]; }
		for (; i != this->size_; ++i) { this->data_[i].~T(); }
	}

	this->size_ = vector.size_;

	return *this;
}

template<typename T> Vector<T>& Vector<T>::operator=(Vector&& vector) {
	if (this == &vector) { return *this; }

	for (size_t i(0); i != this->size_; ++i) { this->data_[i].~T(); }

	this->size_ = vector.size_;
	vector.size_ = 0;
	phi::Swap(this->capacity_, vector.capacity_);
	phi::Swap(this->data_, vector.data_);

	return *this;
}

#///////////////////////////////////////////////////////////////////////////////

template<typename T>
template<typename Y>
bool Vector<T>::operator==(const Vector<Y>& vector) const {
	if (this == &vector) { return true; }
	if (this->size_ != vector.size_) { return false; }

	for (size_t i(0); i != this->size_; ++i) {
		if (this->data_[i] != vector.data_[i]) { return false; }
	}

	return true;
}

template<typename T>
template<typename Y>
bool Vector<T>::operator!=(const Vector<Y>& vector) const {
	return !(this->operator==(vector));
}

#///////////////////////////////////////////////////////////////////////////////

template<typename T> T& Vector<T>::front() {
	PHI__debug_if(this->size_ == 0) { PHI__throw__local("index error"); }
	return *this->data_;
}

template<typename T> const T& Vector<T>::front() const {
	PHI__debug_if(this->size_ == 0) { PHI__throw__local("index error"); }
	return *this->data_;
}

template<typename T> T& Vector<T>::back() {
	PHI__debug_if(this->size_ == 0) { PHI__throw__local("index error"); }
	return this->data_[this->size_ - 1];
}

template<typename T> const T& Vector<T>::back() const {
	PHI__debug_if(this->size_ == 0) { PHI__throw__local("index error"); }
	return this->data_[this->size_ - 1];
}

#///////////////////////////////////////////////////////////////////////////////

template<typename T> T& Vector<T>::operator[](size_t index) {
	return this->data_[index];
}

template<typename T> const T& Vector<T>::operator[](size_t index) const {
	return this->data_[index];
}

template<typename T> T& Vector<T>::at(size_t index) {
	PHI__debug_if(this->size_ <= index) { PHI__throw__local("index error"); }
	return this->data_[index];
}

template<typename T> const T& Vector<T>::at(size_t index) const {
	PHI__debug_if(this->size_ <= index) { PHI__throw__local("index error"); }
	return this->data_[index];
}

#///////////////////////////////////////////////////////////////////////////////

template<typename T>
template<typename... Args>
T& Vector<T>::Push(Args&&... args) {
	if (this->size_ != this->capacity_) {
		return *(new (this->data_ + (++this->size_) - 1)
					 T(Forward<Args>(args)...));
	}

	T* data(Malloc<T>(this->capacity_ = CapacityShouldAlloc(this->size_ + 1)));
	T* r(new (data + this->size_) T(Forward<Args>(args)...));

	for (size_t i(0); i != this->size_; ++i) {
		new (data + i) T(Move(this->data_[i]));
		this->data_[i].~T();
	}

	Free(this->data_);
	this->data_ = data;
	++this->size_;

	return *r;
}

template<typename T>
template<typename BidirectionalIterator>
void Vector<T>::PushIterator(BidirectionalIterator begin,
							 BidirectionalIterator end) {
	size_t d_size(Distance(begin, end));

	if (d_size == 0) { return; }

	this->Reserve(this->size_ + d_size);

	for (size_t i(this->size_); i != this->size_ + d_size; ++i, ++begin) {
		new (this->data_ + i) T(*begin);
	}

	this->size_ += d_size;
}

#///////////////////////////////////////////////////////////////////////////////

template<typename T>
template<typename... Args>
void Vector<T>::Insert(size_t index, Args&&... args) {
	PHI__debug_if(!this->valid_(index)) { PHI__throw__local("index error"); }

	if (index == this->size_) {
		this->PushBack(Forward<Args>(args)...);
		return this->data_ + this->size_ - 1;
	}

	if (this->size_ < this->capacity_) {
		new (this->data_ + this->size_) T(Move(this->data_ + this->size_ - 1));

		for (size_t i(this->size_ - 1); i != index; --i) {
			this->data_[i] = Move(this->data_[i - 1]);
		}

		++this->size_;
		this->data_[index].~T();
		new (this->data_ + index) T(Forward<Args>(args)...);

		return this->data_ + index;
	}

	T* data(
		Malloc<T>(this->capacity_ = CapacityShouldAlloc(this->capacity_ + 1)));

	size_t i(0);

	for (; i != index; ++i) {
		new (data + i) T(Move(this->data_[i]));
		this->data_[i].~T();
	}

	new (data + i) T(Forward<Args>(args)...);

	for (; i != this->size_; ++i) {
		new (data + i + 1) T(Move(this->data_[i]));
		this->data_[i].~T();
	}

	++this->size_;
	Free(this->data_);
	this->data_ = data;

	return this->data_ + index;
}

template<typename T>
template<typename BidirectionalIterator>
void Vector<T>::Insert(size_t index, BidirectionalIterator begin,
					   BidirectionalIterator end) {
	PHI__debug_if(this->size_ < index) { PHI__throw__local("index error"); }

	if (index == this->size_) {
		this->PushIterator(begin, end);
		return;
	}

	size_t d_size(Distance(begin, end));
	if (d_size == 0) { return; }

	// this->size_ : 1 ~ inf
	// index : 0 ~ this->size_ - 1 = 0 ~ inf
	// d_size : 1 ~ inf

	if (this->capacity_ < this->size_ + d_size) {
		T* data(Malloc<T>(this->capacity_ =
							  CapacityShouldAlloc(this->size_ + d_size)));

		for (size_t i(0); i != index; ++i) {
			new (data + i) T(Move(this->data_[i]));
			this->data_[i].~T();
		}

		for (size_t i(0); i != d_size; ++i, ++begin) {
			new (data + index + i) T(*begin);
		}

		for (size_t i(index); i != this->size_; ++i) {
			new (data + d_size + i) T(Move(this->data_[i]));
			this->data_[i].~T();
		}

		this->size_ += d_size;
		Free(this->data_);
		this->data_ = data;

		return;
	}

	/*
	origin : --------------------
	case 1 : ---------------//////////-----
	case 2 : -----//////////---------------
	*/

	// this->size_ + d_size : 1 ~ inf

	if (this->size_ < index + d_size) {
		// origin : --------------------
		// case 1 : ---------------//////////-----

		// i : index ~ this->size_ - 1
		for (size_t i(index); i != this->size_; ++i) {
			new (this->data_ + i + d_size) T(Move(this->data_[i]));
		}

		// i : index ~ this->size_ - 1
		for (size_t i(index); i != this->size_; ++i, ++begin) {
			this->data_[i] = *begin;
		}

		// i : this->size_ ~ index + d_size - 1
		for (size_t i(this->size_); i != index + d_size; ++i, ++begin) {
			new (this->data_ + i) T(*begin);
		}

		this->size_ += d_size;

		return;
	}

	// origin : --------------------
	// case 2 : -----//////////---------------

	// i : this->size_ - d_size ~ this->size_ - 1
	for (size_t i(this->size_ - d_size); i != this->size_; ++i) {
		new (this->data_ + i + d_size) T(Move(this->data_[i]));
	}

	// i = this->size_ - d_size - 1 ~ index
	for (size_t i(this->size_ - d_size - 1); index <= i; --i) {
		this->data_[i + d_size] = Move(this->data_[i]);
	}

	// i = index ~ index + d_size - 1
	for (size_t i(index); i != index + d_size; ++i, ++begin) {
		this->data_[i] = *begin;
	}

	this->size_ += d_size;
}

#///////////////////////////////////////////////////////////////////////////////

template<typename T> void Vector<T>::Pop() {
	PHI__debug_if(this->size_ == 0) { PHI__throw__local("index error"); }
	--this->size_;
	this->data_[this->size_].~T();
}

template<typename T> void Vector<T>::Pop(size_t size) {
	PHI__debug_if(this->size_ < size) { PHI__throw__local("size error"); }

	for (size_t i(this->size_ - size); i != this->size_; ++i) {
		this->data_[i].~T();
	}

	this->size_ -= size;
}

#///////////////////////////////////////////////////////////////////////////////

template<typename T> void Vector<T>::Erase(size_t index) {
	PHI__debug_if(this->size_ <= index) { PHI__throw__local("index error"); }

	--this->size_;

	for (size_t i(index); i != this->size_; ++i) {
		this->data_[i] = Move(this->data_[i + 1]);
	}

	this->data_[this->size_].~T();
}

template<typename T> void Vector<T>::Erase(size_t begin, size_t end) {
	PHI__debug_if(this->size_ <= begin || this->size_ < end || end < begin) {
		PHI__throw__local("index error");
	}

	size_t d_size(end - begin);
	if (d_size == 0) { return; }

	for (size_t i(begin); i != this->size_ - d_size; ++i) {
		this->data_[i] = Move(this->data_[i + d_size]);
	}

	for (size_t i(this->size_ - d_size); i != this->size_; ++i) {
		this->data_[i].~T();
	}

	this->size_ -= d_size;
}

template<typename T>
template<typename Index, typename EqualComparer>
size_t Vector<T>::FindErase(const Index& index, EqualComparer eq_cmper) {
	for (size_t i(0); i != this->size_; ++i) {
		if (eq_cmper(this->data_[i], index)) {
			this->Erase(i);
			return i;
		}
	}

	return this->size_;
}

template<typename T> T* Vector<T>::Release() {
	T* old_data(this->data_);

	this->size_ = this->capacity_ = 0;
	this->data_ = nullptr;

	return old_data;
}

template<typename T> void Vector<T>::Clear() {
	for (size_t i(0); i != this->size_; ++i) { this->data_[i].~T(); }
	this->size_ = 0;
}

#///////////////////////////////////////////////////////////////////////////////

template<typename T> void Vector<T>::Reserve(size_t capacity) {
	if (capacity <= this->capacity_) { return; }

	T* data(Malloc<T>(this->capacity_ = CapacityShouldAlloc(capacity)));

	for (size_t i(0); i != this->size_; ++i) {
		new (data + i) T(Move(this->data_[i]));
		this->data_[i].~T();
	}

	Free(this->data_);
	this->data_ = data;
}

#///////////////////////////////////////////////////////////////////////////////

template<typename T>
size_t Vector<T>::CapacityShouldAlloc(size_t required_capacity) {
	return required_capacity * 2;
}

template<typename T> void Vector<T>::Swap(Vector& x, Vector& y) {
	phi::Swap(x.size_, y.size_);
	phi::Swap(x.capacity_, y.capacity_);
	phi::Swap(x.data_, y.data_);
}

#///////////////////////////////////////////////////////////////////////////////

template<typename T>
size_t Vector<T>::index_pos_diff_(size_t index, diff_t diff) const {
	if (diff < 0) {
		return this->size_ - (this->size_ - index - diff) % (this->size_ + 1);
	}

	if (0 < diff) { return (index + diff) % (this->size_ + 1); }

	return index;
}

template<typename T>
size_t Vector<T>::index_neg_diff_(size_t index, diff_t diff) const {
	if (diff < 0) { return (index - diff) % (this->size_ + 1); }

	if (0 < diff) {
		return this->size_ - (this->size_ - index + diff) % (this->size_ + 1);
	}

	return index;
}

#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////

template<typename T>
Vector<T>::Iterator::Iterator(const Iterator& iter):
	vector_(iter.vector_), index_(iter.index_) {}

template<typename T>
Vector<T>::Iterator::Iterator(Vector* vector, size_t index):
	vector_(vector), index_(index) {}

#///////////////////////////////////////////////////////////////////////////////

template<typename T> Vector<T>::Iterator::operator size_t() const {
	return this->index_;
}

#///////////////////////////////////////////////////////////////////////////////

template<typename T>
typename Vector<T>::Iterator&
Vector<T>::Iterator::operator=(const Iterator& iter) {
	this->vector_ = iter.vector_;
	this->index_ = iter.index_;
	return *this;
}

#///////////////////////////////////////////////////////////////////////////////

template<typename T>
bool Vector<T>::Iterator::operator<(const Iterator& iter) const {
	return this->vector_ == iter.vector_ && this->index_ < iter.index_;
}

template<typename T>
bool Vector<T>::Iterator::operator<(const ConstIterator& const_iter) const {
	return this->vector_ == const_iter.vector_ &&
		   this->index_ < const_iter.index_;
}

template<typename T>
bool Vector<T>::Iterator::operator==(const Iterator& iter) const {
	return this->vector_ == iter.vector_ && this->index_ == iter.index_;
}

template<typename T>
bool Vector<T>::Iterator::operator!=(const Iterator& iter) const {
	return this->vector_ != iter.vector_ || this->index_ != iter.index_;
}

template<typename T>
bool Vector<T>::Iterator::operator==(const ConstIterator& const_iter) const {
	return this->vector_ == const_iter.vector_ &&
		   this->index_ == const_iter.index_;
}

template<typename T>
bool Vector<T>::Iterator::operator!=(const ConstIterator& const_iter) const {
	return this->vector_ != const_iter.vector_ ||
		   this->index_ != const_iter.index_;
}

#///////////////////////////////////////////////////////////////////////////////

template<typename T> T& Vector<T>::Iterator::operator*() const {
	return this->vector_->data_[this->index_];
}

template<typename T> T* Vector<T>::Iterator::operator->() const {
	return this->vector_->data_ + this->index_;
}

#///////////////////////////////////////////////////////////////////////////////

template<typename T>
typename Vector<T>::Iterator& Vector<T>::Iterator::operator++() {
	this->index_ = this->index_ == this->vector_->size_ ? 0 : this->index_ + 1;
	return *this;
}

template<typename T>
typename Vector<T>::Iterator& Vector<T>::Iterator::operator--() {
	this->index_ = this->index_ == 0 ? this->vector_->size_ : this->index_ - 1;
	return *this;
}

#///////////////////////////////////////////////////////////////////////////////

template<typename T>
typename Vector<T>::Iterator& Vector<T>::Iterator::operator+=(Diff diff) {
	this->index_ = this->vector->index_pos_diff_(this->index_, diff);
	return *this;
}

template<typename T>
typename Vector<T>::Iterator& Vector<T>::Iterator::operator-=(Diff diff) {
	this->index_ = this->vector->index_neg_diff_(this->index_, diff);
	return *this;
}

#///////////////////////////////////////////////////////////////////////////////

template<typename T>
typename Vector<T>::Iterator Vector<T>::Iterator::operator+(Diff diff) const {
	return Iterator(this->vector_,
					this->vector_->index_pos_diff_(this->index_, diff));
}

template<typename T>
typename Vector<T>::Iterator Vector<T>::Iterator::operator-(Diff diff) const {
	return Iterator(this->vector_,
					this->vector_->index_neg_diff_(this->index_, diff));
}

template<typename T>
typename Vector<T>::Iterator::Diff
Vector<T>::Iterator::operator-(const Iterator& iter) const {
	return this->index_ - iter.index_;
}

#///////////////////////////////////////////////////////////////////////////////

template<typename T>
typename Vector<T>::Iterator::Ref
Vector<T>::Iterator::operator[](Diff index) const {
	return this->vector_
		->data_[this->vector_->index_pos_diff_(this->index_, index)];
}

#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////

template<typename T>
Vector<T>::ConstIterator::ConstIterator(const Iterator& iter):
	vector_(iter.vector_), index_(iter.index_) {}

template<typename T>
Vector<T>::ConstIterator::ConstIterator(const ConstIterator& const_iter):
	vector_(const_iter.vector_), index_(const_iter.index_) {}

template<typename T>
Vector<T>::ConstIterator::ConstIterator(const Vector* vector, size_t index):
	vector_(vector), index_(index) {}

#///////////////////////////////////////////////////////////////////////////////

template<typename T> Vector<T>::ConstIterator::operator size_t() const {
	return this->index_;
}

#///////////////////////////////////////////////////////////////////////////////

template<typename T>
typename Vector<T>::ConstIterator&
Vector<T>::ConstIterator::operator=(const Iterator& iter) {
	this->vector_ = iter.vector_;
	this->index_ = iter.index_;
	return *this;
}

template<typename T>
typename Vector<T>::ConstIterator&
Vector<T>::ConstIterator::operator=(const ConstIterator& const_iter) {
	this->vector_ = const_iter.vector_;
	this->index_ = const_iter.index_;
	return *this;
}

#///////////////////////////////////////////////////////////////////////////////

template<typename T>
bool Vector<T>::ConstIterator::operator<(const Iterator& iter) const {
	return this->vector_ == iter.vector_ && this->index_ < iter.index_;
}

template<typename T>
bool Vector<T>::ConstIterator::operator<(
	const ConstIterator& const_iter) const {
	return this->vector_ == const_iter.vector_ &&
		   this->index_ < const_iter.index_;
}

template<typename T>
bool Vector<T>::ConstIterator::operator==(const Iterator& iter) const {
	return this->vector_ == iter.vector_ && this->index_ == iter.index_;
}

template<typename T>
bool Vector<T>::ConstIterator::operator!=(const Iterator& iter) const {
	return this->vector_ != iter.vector_ || this->index_ != iter.index_;
}

template<typename T>
bool Vector<T>::ConstIterator::operator==(
	const ConstIterator& const_iter) const {
	return this->vector_ == const_iter.vector_ &&
		   this->index_ == const_iter.index_;
}

template<typename T>
bool Vector<T>::ConstIterator::operator!=(
	const ConstIterator& const_iter) const {
	return this->vector_ != const_iter.vector_ ||
		   this->index_ != const_iter.index_;
}

#///////////////////////////////////////////////////////////////////////////////

template<typename T> const T& Vector<T>::ConstIterator::operator*() const {
	return this->vector_->data_[this->index_];
}

template<typename T> const T* Vector<T>::ConstIterator::operator->() const {
	return this->vector_->data_ + this->index_;
}

#///////////////////////////////////////////////////////////////////////////////

template<typename T>
typename Vector<T>::ConstIterator& Vector<T>::ConstIterator::operator++() {
	this->index_ = this->index_ == this->vector_->size_ ? 0 : this->index_ + 1;
	return *this;
}

template<typename T>
typename Vector<T>::ConstIterator& Vector<T>::ConstIterator::operator--() {
	this->index_ == this->index_ == 0 ? this->vector_->size_ : this->index_ - 1;
	return *this;
}

#///////////////////////////////////////////////////////////////////////////////

template<typename T>
typename Vector<T>::ConstIterator&
Vector<T>::ConstIterator::operator+=(Diff diff) {
	this->index_ = this->vector_->index_pos_diff_(this->index, diff);
	return *this;
}

template<typename T>
typename Vector<T>::ConstIterator&
Vector<T>::ConstIterator::operator-=(Diff diff) {
	this->index_ = this->vector_->index_neg_diff_(this->index, diff);
	return *this;
}

#///////////////////////////////////////////////////////////////////////////////

template<typename T>
typename Vector<T>::ConstIterator
Vector<T>::ConstIterator::operator+(Diff diff) const {
	return ConstIterator(this->vector_,
						 this->vector_->index_pos_diff_(this->index_, diff));
}

template<typename T>
typename Vector<T>::ConstIterator
Vector<T>::ConstIterator::operator-(Diff diff) const {
	return ConstIterator(this->vector_,
						 this->vector_->index_neg_diff_(this->index_, -diff));
}

template<typename T>
typename Vector<T>::ConstIterator::Diff
Vector<T>::ConstIterator::operator-(const ConstIterator& const_iter) const {
	return this->index_ - const_iter.index_;
}

#///////////////////////////////////////////////////////////////////////////////

template<typename T>
typename Vector<T>::ConstIterator::Ref
Vector<T>::ConstIterator::operator[](Diff index) const {
	return this->vector_
		->data_[this->vector->index_pos_diff_(this->index_, index)];
}

}
}

#undef PHI__throw__local

#endif
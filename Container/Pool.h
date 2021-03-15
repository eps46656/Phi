#ifndef PHI__define_guard__Container__Pool_h
#define PHI__define_guard__Container__Pool_h

#include "DoublyNode.h"

namespace phi {
namespace cntr {

template<size_t BlockSize> class UncountedPool {
public:
	static_assert(sizeof(DoublyNode) <= BlockSize, "BlockSize too small");

#///////////////////////////////////////////////////////////////////////////////

	bool empty() const;

#///////////////////////////////////////////////////////////////////////////////

	~UncountedPool();

#///////////////////////////////////////////////////////////////////////////////

	void Push(void* block);
	void* Pop();

	void Clear();
	void Clear(size_t size);

	void TransferTo(UncountedPool<BlockSize>& uncounted_pool);

private:
	DoublyNode node_;
};

#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////

template<size_t BlockSize> class Pool: private UncountedPool<BlockSize> {
public:
	bool empty() const;
	size_t size() const;

#///////////////////////////////////////////////////////////////////////////////

	Pool();

#///////////////////////////////////////////////////////////////////////////////

	void Push(void* block);
	void* Pop();

	void Clear();
	void Clear(size_t size);

	void TransferTo(UncountedPool<BlockSize>& uncounted_pool);
	void TransferTo(Pool<BlockSize>& pool);

	void Reserve(size_t size);

private:
	size_t size_;
	DoublyNode node_;
};

#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////

template<size_t BlockSize> bool UncountedPool<BlockSize>::empty() const {
	return this->node_.sole();
}

#///////////////////////////////////////////////////////////////////////////////

template<size_t BlockSize> UncountedPool<BlockSize>::~UncountedPool() {
	this->Clear();
}

#///////////////////////////////////////////////////////////////////////////////

template<size_t BlockSize> void UncountedPool<BlockSize>::Push(void* block) {
	this->node_.PushPrevNew(block);
}

template<size_t BlockSize> void* UncountedPool<BlockSize>::Pop() {
	if (this->node_.sole()) { return Malloc(BlockSize); }
	return this->node_.prev()->Pop();
}

template<size_t BlockSize> void UncountedPool<BlockSize>::Clear() {
	while (!this->node_.sole()) { Free(this->node_.prev()->Pop()); }
}

template<size_t BlockSize>
void UncountedPool<BlockSize>::TransferTo(UncountedPool<BlockSize>& pool) {
	pool.node_.PushPrevAllExcept(&this->node_);
}

#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////

template<size_t BlockSize> bool Pool<BlockSize>::empty() const {
	return this->UncountedPool<BlockSize>::empty();
}

template<size_t BlockSize> size_t Pool<BlockSize>::size() const {
	return this->size_;
}

#///////////////////////////////////////////////////////////////////////////////

template<size_t BlockSize> Pool<BlockSize>::Pool(): size_(0) {}

#///////////////////////////////////////////////////////////////////////////////

template<size_t BlockSize> void Pool<BlockSize>::Push(void* block) {
	++this->size_;
	this->UncountedPool<BlockSize>::Push(block);
}

template<size_t BlockSize> void* Pool<BlockSize>::Pop() {
	if (this->size_ != 0) { --this->size_; }
	return this->UncountedPool<BlockSize>::Pop();
}

template<size_t BlockSize> void Pool<BlockSize>::Clear() {
	this->size_ = 0;
	this->UncountedPool<BlockSize>::Clear();
}

template<size_t BlockSize> void Pool<BlockSize>::Clear(size_t size) {
	if (this->size_ < size) {
		this->UncountedPool<BlockSize>::Clear();
	} else {
		this->size_ -= size;
		for (; size != 0; --size) { Free(this->Pop()); }
	}
}

template<size_t BlockSize>
void Pool<BlockSize>::TransferTo(UncountedPool<BlockSize>& uncounted_pool) {
	this->size_ = 0;
	this->UncountedPool<BlockSize>::TransferTo(uncounted_pool);
}

template<size_t BlockSize>
void Pool<BlockSize>::TransferTo(Pool<BlockSize>& pool) {
	pool.size_ += this->size_;
	this->size_ = 0;
	this->UncountedPool<BlockSize>::TransferTo(pool);
}

template<size_t BlockSize> void Pool<BlockSize>::Reserve(size_t size) {
	if (size <= this->size_) { return; }

	for (size_t i(this->size_); i != size; ++i) {
		this->UncountedPool<BlockSize>::Push(Malloc(BlockSize));
	}

	this->size_ = size;
}

}
}

#endif
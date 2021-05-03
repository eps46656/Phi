#ifndef PHI__define_guard__Utility__key_arranger_h
#define PHI__define_guard__Utility__key_arranger_h

#include "../define.h"
#include "../Container/Vector.h"
#include "../Container/Map.h"

namespace phi {

template<typename Key, typename FullComparer> struct KeyArranger {
public:
	template<typename Key_> size_t key_to_index(const Key_& key) const;
	const Key& index_to_key(size_t index) const;

#///////////////////////////////////////////////////////////////////////////////

	template<typename... FullComparerConstructArgs>
	KeyArranger(FullComparerConstructArgs&&... full_cmper_construct_args);

#///////////////////////////////////////////////////////////////////////////////

	template<typename Key_> void Insert(const Key_& key);

	void Arrange();

private:
	cntr::Map<Key, size_t, FullComparer> map_;
	cntr::Vector<Key> vec_;
};

#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////

template<typename Key, typename FullComparer>
template<typename Key_>
size_t KeyArranger<Key, FullComparer>::key_to_index(const Key_& key) const {
	auto iter(this->map_.Find(key));
	return iter == this->map_.null_iterator() ? this->map_.size()
											  : iter->second;
}

template<typename Key, typename FullComparer>
const Key& KeyArranger<Key, FullComparer>::index_to_key(size_t index) const {
	PHI__debug_if(this->vec_.size() <= index) {
		PHI__throw(, __func__, "index error");
	}

	return this->vec_[index];
}

#///////////////////////////////////////////////////////////////////////////////

template<typename Key, typename FullComparer>
template<typename... FullComparerConstructArgs>
KeyArranger<Key, FullComparer>::KeyArranger(
	FullComparerConstructArgs&&... full_cmper_construct_args):
	map_(Forward<FullComparerConstructArgs>(full_cmper_construct_args)...) {}

#///////////////////////////////////////////////////////////////////////////////

template<typename Key, typename FullComparer>
template<typename Key_>
void KeyArranger<Key, FullComparer>::Insert(const Key_& key) {
	this->map_.Insert(key);
}

template<typename Key, typename FullComparer>
void KeyArranger<Key, FullComparer>::Arrange() {
	this->vec_.Clear();
	this->vec_.Reserve(this->map_.size());

	auto iter(this->map_.first_iterator());
	auto end(this->map_.null_iterator());

	for (; iter != end; ++iter) {
		iter->second = this->vec_.size();
		this->vec_.Push(iter->first);
	}
}

}

#endif
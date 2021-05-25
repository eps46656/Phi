
#ifndef PHI__define_guard__Utility__exact_cover_h
#define PHI__define_guard__Utility__exact_cover_h

#include "search.h"
#include "../Container/Vector.h"

#include "../Container/List.h"
#include "../Container/Set.h"

namespace phi {

class ExactCover {
public:
	struct State;
	struct Effect;

	struct State: public cntr::DoublyNode {
		friend class ExactCover;

	public:
		cntr::Set<Effect*> eff;
		cntr::Set<State*> conflict;

		inline State();

		void Cover();
		void UnCover();

	private:
		ExactCover* ec_;
		size_t nec_eff_num_;
	};

	struct Effect {
		friend class ExactCover;

	public:
		bool is_nec;
		cntr::Set<State*> state;

		inline Effect();

	private:
		ExactCover* ec_;
	};

#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////

	inline ExactCover();

#///////////////////////////////////////////////////////////////////////////////

	inline const cntr::Set<State*>& all_state() const;
	inline const cntr::Set<Effect*>& all_eff() const;

	inline void AddState(State* state);
	inline void AddEffect(Effect* eff, bool is_nec);
	inline void AddHot(State* state, Effect* eff);

#///////////////////////////////////////////////////////////////////////////////

	template<typename Receiver> void Solve(const Receiver& receiver) const;

private:
	cntr::Set<State*> all_state_;
	cntr::Set<Effect*> all_eff_;

	size_t nec_eff_num_;

	template<typename Receiver>
	void Solve_(cntr::List<State*>& solution, size_t nec_eff_num,
				State* state_begin, cntr::DoublyNode* state_end,
				const Receiver& receiver) const;
};

#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////

ExactCover::State::State(): ec_(nullptr), nec_eff_num_(0) {}

void ExactCover::State::Cover() { Link_(this->prev_, this->next_); }
void ExactCover::State::UnCover() {
	Link_(this->prev_, this);
	Link_(this, this->next_);
}

#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////

ExactCover::Effect::Effect(): ec_(nullptr) {}

#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////

ExactCover::ExactCover(): nec_eff_num_(0) {}

#///////////////////////////////////////////////////////////////////////////////

const cntr::Set<ExactCover::State*>& ExactCover::all_state() const {
	return this->all_state_;
}

const cntr::Set<ExactCover::Effect*>& ExactCover::all_eff() const {
	return this->all_eff_;
}

#///////////////////////////////////////////////////////////////////////////////

void ExactCover::AddState(State* state) {
	PHI__debug_if(state->ec_ != nullptr) { PHI__throw("error"); }

	this->all_state_.Insert(state);
	state->ec_ = this;
	state->eff.Clear();
	state->conflict.Clear();
}

void ExactCover::AddEffect(Effect* eff, bool is_nec) {
	PHI__debug_if(eff->ec_ != nullptr) { PHI__throw("error"); }

	if (this == eff->ec_) { return; }

	this->all_eff_.Insert(eff);
	eff->ec_ = this;
	eff->is_nec = is_nec;
	eff->state.Clear();

	if (is_nec) { ++this->nec_eff_num_; }
}

void ExactCover::AddHot(State* state, Effect* eff) {
	PHI__debug_if(this != state->ec_ || this != eff->ec_) {
		PHI__throw("error");
	}

	if (!state->eff.Insert(eff).second) { return; }

	{
		auto iter(eff->state.first_iterator());

		for (auto end(eff->state.null_iterator()); iter != end; ++iter) {
			(*iter)->conflict.Insert(state);
			state->conflict.Insert(*iter);
		}
	}

	eff->state.Insert(state);

	if (eff->is_nec) { ++state->nec_eff_num_; }
}

#///////////////////////////////////////////////////////////////////////////////

template<typename Receiver>
void ExactCover::Solve(const Receiver& receiver) const {
	cntr::List<State*> solution;

	cntr::DoublyNode state_end;

	{
		auto iter(this->all_state_.first_iterator());
		auto end(this->all_state_.null_iterator());

		for (; iter != end; ++iter) { state_end.PushPrev(*iter); }
	}

	Solve_(solution, this->nec_eff_num_, static_cast<State*>(state_end.next()),
		   &state_end, receiver);
}

template<typename Receiver>
void ExactCover::Solve_(cntr::List<State*>& solution, size_t nec_eff_num,
						State* state_iter, cntr::DoublyNode* state_end,
						const Receiver& receiver) const {
	if (nec_eff_num == 0) {
		receiver(
			static_cast<const cntr::List<State*>&>(solution).first_iterator(),
			static_cast<const cntr::List<State*>&>(solution).null_iterator());
		return;
	}

	for (; state_iter != state_end;
		 state_iter = static_cast<State*>(state_iter->next())) {
		{
			auto iter(state_iter->conflict.first_iterator());
			auto end(state_iter->conflict.null_iterator());

			for (; iter != end; ++iter) { (*iter)->Cover(); }
		}

		solution.PushBack(state_iter);
		Solve_(solution, nec_eff_num - state_iter->nec_eff_num_,
			   static_cast<State*>(state_iter->next()), state_end, receiver);
		solution.PopBack();

		{
			auto iter(state_iter->conflict.first_iterator());
			auto end(state_iter->conflict.null_iterator());

			for (; iter != end; ++iter) { (*iter)->UnCover(); }
		}
	}
}

}

#endif
#ifndef PHI__define_guard__graf_search_h
#define PHI__define_guard__graf_search_h

#include "../Container/List.h"
#include "../Container/RedBlackTree.h"

namespace phi {

namespace a_star_search_utility {

struct PosOrder: public cntr::RedBlackTreeNode {};
struct ScoreOrder: public cntr::RedBlackTreeNode {};

template<typename Node> struct Agent: public PosOrder, public ScoreOrder {
	Node* node;
	Agent* parent;
	size_t dist_to_parent;
	size_t g_score;
	size_t f_score;
};

#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////

template<typename Node, typename NodeComparer> struct PosComparer_ {
	const NodeComparer& node_cmper;

	PosComparer_(const NodeComparer& node_cmper): node_cmper(node_cmper) {}

	inline int operator()(const cntr::RedBlackTreeNode* x,
						  const cntr::RedBlackTreeNode* y) const {
		const Agent<Node>* a(
			static_cast<const Agent<Node>*>(static_cast<const PosOrder*>(x)));
		const Agent<Node>* b(
			static_cast<const Agent<Node>*>(static_cast<const PosOrder*>(y)));

		return this->node_cmper(a->node, b->node);
	}

#///////////////////////////////////////////////////////////////////////////////

	inline int operator()(const cntr::RedBlackTreeNode* rbtn,
						  const Node* node) const {
		return this->node_cmper(
			static_cast<const Agent<Node>*>(static_cast<const PosOrder*>(rbtn))
				->node,
			node);
	}

	inline int operator()(const Node* node,
						  const cntr::RedBlackTreeNode* rbtn) const {
		return this->node_cmper(node, static_cast<const Agent<Node>*>(
										  static_cast<const PosOrder*>(rbtn))
										  ->node);
	}
};

template<typename Node, typename NodeComparer>
using PosComparer = AutoImplementFullComparer<PosComparer_<Node, NodeComparer>>;

#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////

template<typename Node, typename NodeComparer> struct ScoreComparer_ {
	const NodeComparer& node_cmper;

#///////////////////////////////////////////////////////////////////////////////

	ScoreComparer_(const NodeComparer& node_cmper): node_cmper(node_cmper) {}

#///////////////////////////////////////////////////////////////////////////////

	inline int operator()(const cntr::RedBlackTreeNode* x,
						  const cntr::RedBlackTreeNode* y) const {
		const Agent<Node>* a(
			static_cast<const Agent<Node>*>(static_cast<const ScoreOrder*>(x)));
		const Agent<Node>* b(
			static_cast<const Agent<Node>*>(static_cast<const ScoreOrder*>(y)));

		if (a->f_score < b->f_score) { return -1; }
		if (a->f_score > b->f_score) { return 1; }
		if (a->g_score > b->g_score) { return -1; }
		if (a->g_score < b->g_score) { return 1; }
		return this->node_cmper(a->node, b->node);
	}
};

template<typename Node, typename NodeComparer>
using ScoreComparer =
	AutoImplementFullComparer<ScoreComparer_<Node, NodeComparer>>;

#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////

template<typename Node, typename NodeComparer>
class PosOrderSet: public cntr::RedBlackTree<PosComparer<Node, NodeComparer>> {
public:
	using RBT = cntr::RedBlackTree<PosComparer<Node, NodeComparer>>;

	PosOrderSet(const PosComparer<Node, NodeComparer>& pos_cmper):
		RBT(pos_cmper) {}

	Agent<Node>* FindAgent(Agent<Node>* agent) {
		this->RBT::Insert(static_cast<PosOrder*>(agent));
	}

	void Insert(Agent<Node>* agent) {
		this->RBT::Insert(static_cast<PosOrder*>(agent));
	}

	void Release(Agent<Node>* agent) {
		this->RBT::Release(static_cast<PosOrder*>(agent));
	}
};

#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////

template<typename Node, typename NodeComparer>
class ScoreOrderSet:
	public cntr::RedBlackTree<ScoreComparer<Node, NodeComparer>> {
public:
	using RBT = cntr::RedBlackTree<ScoreComparer<Node, NodeComparer>>;

	ScoreOrderSet(const ScoreComparer<Node, NodeComparer>& score_cmper):
		RBT(score_cmper) {}

	Agent<Node>* FindAgent(Agent<Node>* agent) {
		this->RBT::Find(static_cast<ScoreOrder*>(agent));
	}

	void Insert(Agent<Node>* agent) {
		this->RBT::Insert(static_cast<ScoreOrder*>(agent));
	}

	void Release(Agent<Node>* agent) {
		this->RBT::Release(static_cast<ScoreOrder*>(agent));
	}

	Agent<Node>* Pop() {
		cntr::RedBlackTreeNode* n(this->first_node());
		this->RBT::Release(n);
		return static_cast<Agent<Node>*>(static_cast<ScoreOrder*>(n));
	}
};

}

#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////

template<typename Node, typename NodeComparer, typename NextNodeGenerator,
		 typename NodeReleaser, typename Heuristic>
cntr::List<Node*>
AStartSearch(Node* begin, Node* end, const NodeComparer& node_cmper,
			 const NextNodeGenerator& next_node_generator,
			 const NodeReleaser& node_releaser, const Heuristic& heuristic) {
	using PosOrder = a_star_search_utility::PosOrder;
	using ScoreOrder = a_star_search_utility::ScoreOrder;

	using Agent = a_star_search_utility::Agent<Node>;

	using PosComparer = a_star_search_utility::PosComparer<Node, NodeComparer>;
	using ScoreComparer =
		a_star_search_utility::ScoreComparer<Node, NodeComparer>;

	using PosOrderSet = a_star_search_utility::PosOrderSet<Node, NodeComparer>;
	using ScoreOrderSet =
		a_star_search_utility::ScoreOrderSet<Node, NodeComparer>;

	cntr::List<Node*> path;

	if (node_cmper(begin, end) == 0) {
		path.PushBack(begin);
		return path;
	}

	cntr::List<pair<Node*, size_t>> next_nodes;

	PosComparer pos_cmper(node_cmper);
	PosOrderSet pos_order_close_set(pos_cmper);
	PosOrderSet pos_order_open_set(pos_cmper);

	ScoreComparer score_cmper(node_cmper);
	ScoreOrderSet score_order_close_set(score_cmper);
	ScoreOrderSet score_order_open_set(score_cmper);

	Agent* begin_agent(New<Agent>());
	begin_agent->node = begin;
	begin_agent->parent = nullptr;
	begin_agent->dist_to_parent = 0;
	begin_agent->g_score = 0;
	begin_agent->f_score = heuristic(begin, end);

	pos_order_open_set.Insert(begin_agent);
	score_order_open_set.Insert(begin_agent);

	while (!pos_order_open_set.empty()) {
#if false
		if (pos_order_open_set.size() != score_order_open_set.size()) {
			std::cout
				<< "pos_order_open_set.size() != score_order_open_set.size()";
			exit(1);
		}

		if (pos_order_close_set.size() != score_order_close_set.size()) {
			std::cout
				<< "pos_order_close_set.size() != score_order_close_set.size()";
			exit(1);
		}
#endif

		Agent* agent(score_order_open_set.Pop());
		pos_order_open_set.Release(agent);
		pos_order_close_set.Insert(agent);
		score_order_close_set.Insert(agent);

		next_node_generator(next_nodes, agent->node);

		while (!next_nodes.empty()) {
			Node* next_node(next_nodes.front().first);
			size_t next_node_dist(next_nodes.front().second);
			next_nodes.PopFront();

			if (node_cmper(next_node, end) == 0) {
				node_releaser(next_node);
				path.PushBack(end);

				for (; agent != nullptr; agent = agent->parent) {
					pos_order_close_set.Release(agent);
					score_order_close_set.Release(agent);

					path.PushFront(agent->node);
				}

				goto search_complete;
			}

			if (pos_order_close_set.Contain(next_node)) {
				node_releaser(next_node);
				continue;
			}

			size_t g_score(agent->g_score + next_node_dist);
			Agent* prev_agent(static_cast<Agent*>(
				static_cast<PosOrder*>(pos_order_open_set.Find(next_node))));

			if (prev_agent != nullptr) {
				if (prev_agent->g_score <= g_score) {
					node_releaser(next_node);
					continue;
				}

				pos_order_open_set.Release(prev_agent);
				score_order_open_set.Release(prev_agent);
				node_releaser(prev_agent->node);
			}

			Agent* next_agent(New<Agent>());
			next_agent->node = next_node;
			next_agent->parent = agent;
			next_agent->dist_to_parent = next_node_dist;
			next_agent->g_score = g_score;
			next_agent->f_score = g_score + heuristic(next_node, end);

			pos_order_open_set.Insert(next_agent);
			score_order_open_set.Insert(next_agent);
		}
	}

search_complete:;

#if false

	std::cout << "score_order_close_set.size() " << score_order_close_set.size()
			  << "\n";
	std::cout << "score_order_open_set.size() " << score_order_open_set.size()
			  << "\n";

	#define MIN_X -10
	#define MIN_Y -10

	#define MAX_X 100
	#define MAX_Y 50

	for (int y(MAX_Y); MIN_Y <= y; --y) {
		for (int x(MIN_X); x <= MAX_X; ++x) {
			Node node(x, y);

			if (pos_order_close_set.Find(&node) != nullptr) {
				std::cout << "#";
			} else if (pos_order_open_set.Find(&node) != nullptr) {
				std::cout << "+";
			} else {
				std::cout << "O";
			}
		}

		std::cout << "\n";
	}

#endif

	while (!next_nodes.empty()) {
		node_releaser(next_nodes.back().first);
		next_nodes.PopBack();
	}

	while (!score_order_close_set.empty()) {
		node_releaser(score_order_close_set.Pop()->node);
	}

	while (!score_order_open_set.empty()) {
		node_releaser(score_order_open_set.Pop()->node);
	}

	return path;
}

}

#endif
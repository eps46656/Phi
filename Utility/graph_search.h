#ifndef PHI__define_guard__graf_search_h
#define PHI__define_guard__graf_search_h

#include "pair.h"
#include "../Container/Vector.h"
#include "../Container/List.h"
#include "../Container/RedBlackTree.h"

namespace phi {

namespace a_star_search_utility {

template<typename Node> struct PathNode {
	Node* node;
	PathNode* parent;
	size_t dist_to_parent;
	size_t g_score;
	size_t h_score;
};

#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////

template<typename Node, typename NodeComparer, typename Heuristic>
struct SetComparer {
	const NodeComparer& node_cmper;

#///////////////////////////////////////////////////////////////////////////////

	SetComparer(const NodeComparer& node_cmper): node_cmper(node_cmper) {}

#///////////////////////////////////////////////////////////////////////////////

	int operator()(const PathNode<Node>* x, const PathNode<Node>* y) const {
		size_t x_f_score(x->g_score + x->h_score);
		size_t y_f_score(y->g_score + y->h_score);
		if (x_f_score < y_f_score) { return -1; }
		if (x_f_score > y_f_score) { return 1; }
		return this->node_cmper(x->node, y->node);
	}

	int operator()(const PathNode<Node>* path_node, const Node* node) const {
		return this->node_cmper(path_node->node, node);
	}

	int operator()(const Node* node, const PathNode<Node>* path_node) const {
		return this->node_cmper(node, path_node->node);
	}

#///////////////////////////////////////////////////////////////////////////////

	template<typename X, typename Y> bool eq(X&& x, Y&& y) const {
		return this->operator()(x, y) == 0;
	}

	template<typename X, typename Y> bool qe(X&& x, Y&& y) const {
		return this->operator()(x, y) != 0;
	}

	template<typename X, typename Y> bool lt(X&& x, Y&& y) const {
		return this->operator()(x, y) == -1;
	}

	template<typename X, typename Y> bool gt(X&& x, Y&& y) const {
		return this->operator()(x, y) == 1;
	}

	template<typename X, typename Y> bool le(X&& x, Y&& y) const {
		return this->operator()(x, y) != 1;
	}

	template<typename X, typename Y> bool ge(X&& x, Y&& y) const {
		return this->operator()(x, y) != -1;
	}
};

#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////
#///////////////////////////////////////////////////////////////////////////////

template<typename Node, typename NodeComparer, typename Heuristic>
class Set:
	public cntr::RedBlackTree<PathNode<Node>*,
							  SetComparer<Node, NodeComparer, Heuristic>> {
public:
	Set(const SetComparer<Node, NodeComparer, Heuristic>& cmper):
		cntr::RedBlackTree<PathNode<Node>*,
						   SetComparer<Node, NodeComparer, Heuristic>>(cmper) {}

	PathNode<Node>* Pop() {
		auto iter(this->first_iterator());
		this->Release(iter);
		return *iter;
	}
};

}

template<typename Node, typename NodeComparer, typename NextNodeGenerator,
		 typename Heuristic>
cntr::List<Node*> AStartSearch(Node* begin, Node* end,
							   const NodeComparer& node_cmper,
							   const NextNodeGenerator& next_node_generator,
							   const Heuristic& heuristic) {
	using PathNode = a_star_search_utility::PathNode<Node>;
	using SetComparer =
		a_star_search_utility::SetComparer<Node, NodeComparer, Heuristic>;
	using Set = a_star_search_utility::Set<Node, NodeComparer, Heuristic>;

	SetComparer set_cmper(node_cmper);

	cntr::List<Node*> path;
	cntr::List<pair<Node*, size_t>> next_nodes;

	Set open_set(set_cmper);
	Set close_set(set_cmper);

	if (begin == end) {
		path.PushBack(begin);
		return path;
	}

	PathNode* begin_path_node = New<PathNode>();
	begin_path_node->node = begin;
	begin_path_node->parent = nullptr;
	begin_path_node->dist_to_parent = 0;
	begin_path_node->g_score = 0;
	begin_path_node->h_score = heuristic(begin, end);

	open_set.Insert(begin_path_node);

	while (!open_set.empty()) {
		PathNode* path_node(open_set.Pop());
		close_set.Insert(path_node);

		next_node_generator(next_nodes, path_node->node);

		while (!next_nodes.empty()) {
			Node* next_node(next_nodes.front().first);
			size_t next_node_dist(next_nodes.front().second);
			next_nodes.PopFront();

			if (node_cmper(next_node, end) == 0) {
				path.PushBack(end);

				for (; path_node->node != begin;
					 path_node = path_node->parent) {
					close_set.FindErase(path_node);
					path.PushFront(path_node->node);
				}

				close_set.FindErase(begin);
				path.PushFront(begin);

				goto search_end;
			}

			if (close_set.Contain(next_node)) { continue; }

			size_t g_score(path_node->g_score + next_node_dist);
			auto prev(open_set.Find(next_node));

			if (prev != open_set.null_iterator()) {
				if ((*prev)->g_score <= g_score) {
					continue; // next next_node;
				}

				open_set.Erase(prev);
			}

			PathNode* next_path_node(New<PathNode>());
			next_path_node->node = next_node;
			next_path_node->parent = path_node;
			next_path_node->dist_to_parent = next_node_dist;
			next_path_node->g_score = g_score;
			next_path_node->h_score = heuristic(next_node, end);

			open_set.Insert(next_path_node);
		}
	}

search_end:;

	while (!close_set.empty()) {
		PathNode* path_node(close_set.Pop());
		Delete(path_node->node);
	}

	while (!open_set.empty()) {
		PathNode* path_node(open_set.Pop());
		Delete(path_node->node);
	}

	return path;
}

/*
template<typename Node, typename AdjNodeGenerator, typename Heuristic>
cntr::List<Node> AStartSearch(Node begin, Node end,
							  const AdjNodeGenerator& adj_node_generator,
							  const Heuristic& heuristic) {
	using PathNode = a_star_search_utility::PathNode<Node>;

	cntr::List<Node> route;
	route.PushBack(begin);

	cntr::List<PathNode*> all_path_node;
	cntr::RedBlackTree<PathNode*> open_set;
	cntr::RedBlackTree<PathNode*> close_set;

	while (!open_set.empty()) {
		PathNode* path_node(open_set.Pop());

		cntr::Vector<pair<Node, size_t>> adjs(
			adj_node_generator(path_node->node));

		for (size_t i(0); i != adjs.size(); ++i) {
			Node& adj_node(adjs[i].first);
			size_t parent_length(adjs[i].second);
			size_t acc_length(path_node.acc_length + parent_length);

			{
				auto iter(open_set.find(adj_node));

				if (iter != open_set.end()) {
					if (acc_length <= (*iter)->acc_length) {}
				}
			}

			if () {
			} else if (close_set.Contains(adj_node)) {
			}
		}

		PathNode path_node =
			New<PathNode>(node, node_generator(*node), heuristic(*node),

			);

		all_path_node.Push(path_node);
	}
}*/
}

#endif
#pragma once
#include <iostream>
#include <vector>

template <typename T>
class Tree : public std::exception {
protected:
	struct Node;
	using NodePtr = std::shared_ptr<Node>;
	using NodeWeakPtr = std::weak_ptr<Node>;
public:
	class Position;
	using PositionList = std::vector<Position>;
protected:
	class Node {
		T data_;
		NodeWeakPtr parent_;//순환 문제를 해결하기 위해 parent를 weak_ptr로 참조한다.
		std::vector<NodePtr> children_;
	public:
		Node(const T& data) :data_(data) {};//루트 노드
		Node(const T& data, NodePtr parent) :data_(data), parent_(parent) {};//루트가 아닌 일반 노드
		~Node() { std::cout << "foo!" << std::endl; }
		void InsertChild(NodePtr input_node) { children_.push_back(input_node); }
		void DeleteChild(NodePtr delete_node) {
			unsigned int index = 0;
			for (size_t i = 0; i < children_.size(); i++) {
				if (children_[i] == delete_node) {
					index = i;
					break;
				}
			}
			children_.erase(children_.begin() + index);
		}
		friend Position;
		friend class Tree;
	};
public:
	class Position {
	private:
		NodePtr pointed_node_;
	public:
		Position(NodePtr position = nullptr) :pointed_node_(position) {};
		T& operator*() { return pointed_node_->data_; };
		PositionList children() const { return pointed_node_->children_; }
		Position parent() const { return pointed_node_->parent; }; //부모의 노드
		NodePtr get() const { return pointed_node_; }
		bool IsRoot() const { return pointed_node_->parent == nullptr ? true : false; }; //지금 가리키고 있는 노드가 루트인가?
		bool IsNull() const { return pointed_node_ == nullptr; }
		bool IsExternal() const { return pointed_node_->left_child == nullptr && pointed_node_->right_child == nullptr ? true : false; }; //지금 가리키고 있는 노드가 외부 노드인가?
		friend class Tree;
	};
public:
	Tree() :size_(0) , root_(nullptr){ }
	~Tree() {}
	size_t size() const { return size_; }
	bool IsEmpty() const { return size_ == 0; }
	Position root() const { return Position(root_); }
	void AddRoot(const T& data);
	void ExpandNode(Position external_node, const T& data);
	T EraseNode(Position position);
	PositionList PositionsByPreOrder() const {
		PositionList position_vector;
		PreOrder(root_, position_vector);
		return position_vector;
	}//트리를 전위 순회하면서 모든 노드의 Position을 list 컨테이너에 저장하여 반환한다.
	Position Search(const T& data) const;//트리를 전위 순회하면서 원소를 찾는다. 원소를 찾으면 해당 원소의 Position을 반환하고 없으면 nullptr을 반환한다.
	void PrintByPreOrder() const;//전위 순회하면서 각 노드가 가진 원소를 출력한다.
	void PrintChildren(const T& data) const;
	int DepthOfNode(const T& data) const;
protected:
	void PreOrder(NodePtr ptr, PositionList& position_vector) const;//트리를 전위 순회하는 내부 편의 함수이다.
private:
	size_t size_;
	NodePtr root_;
};
#include "Tree.hpp"
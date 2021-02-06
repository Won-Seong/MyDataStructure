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
		NodeWeakPtr parent_;//��ȯ ������ �ذ��ϱ� ���� parent�� weak_ptr�� �����Ѵ�.
		std::vector<NodePtr> children_;
	public:
		Node(const T& data) :data_(data) {};//��Ʈ ���
		Node(const T& data, NodePtr parent) :data_(data), parent_(parent) {};//��Ʈ�� �ƴ� �Ϲ� ���
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
		Position parent() const { return pointed_node_->parent; }; //�θ��� ���
		NodePtr get() const { return pointed_node_; }
		bool IsRoot() const { return pointed_node_->parent == nullptr ? true : false; }; //���� ����Ű�� �ִ� ��尡 ��Ʈ�ΰ�?
		bool IsNull() const { return pointed_node_ == nullptr; }
		bool IsExternal() const { return pointed_node_->left_child == nullptr && pointed_node_->right_child == nullptr ? true : false; }; //���� ����Ű�� �ִ� ��尡 �ܺ� ����ΰ�?
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
	}//Ʈ���� ���� ��ȸ�ϸ鼭 ��� ����� Position�� list �����̳ʿ� �����Ͽ� ��ȯ�Ѵ�.
	Position Search(const T& data) const;//Ʈ���� ���� ��ȸ�ϸ鼭 ���Ҹ� ã�´�. ���Ҹ� ã���� �ش� ������ Position�� ��ȯ�ϰ� ������ nullptr�� ��ȯ�Ѵ�.
	void PrintByPreOrder() const;//���� ��ȸ�ϸ鼭 �� ��尡 ���� ���Ҹ� ����Ѵ�.
	void PrintChildren(const T& data) const;
	int DepthOfNode(const T& data) const;
protected:
	void PreOrder(NodePtr ptr, PositionList& position_vector) const;//Ʈ���� ���� ��ȸ�ϴ� ���� ���� �Լ��̴�.
private:
	size_t size_;
	NodePtr root_;
};
#include "Tree.hpp"
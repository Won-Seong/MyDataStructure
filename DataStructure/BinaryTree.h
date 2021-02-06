#pragma once
#include <vector>

template <typename T>
class BinaryTree : public std::exception {
protected:
	struct Node;
	using NodePtr = std::shared_ptr<Node>;
	using NodeWeakPtr = std::weak_ptr<Node>;
public:
	class Position;
	using SimplePosition = BinaryTree<T>::Position;
	using PositionList = std::vector<Position>;
	using SimplePositionList = BinaryTree<T>::PositionList;
protected:
	class Node {
	public:
		T data_; //���� �ִ� ������
		NodeWeakPtr parent_; //�θ��� ���, ��ȯ ������ �ذ��ϱ� ���Ͽ� weak_ptr�� ����
		NodePtr left_child_; //���� �ڽ��� ���
		NodePtr right_child_; //������ �ڽ��� ���
	public:
		Node(const T& data) :data_(data), left_child_(nullptr), right_child_(nullptr) {};//��尡 ��Ʈ�� ����� ������
		Node(NodePtr parent , const T& data = T()) :data_(data), left_child_(nullptr), right_child_(nullptr), parent_(parent) {};//�Ϲ� ����� ������
		Node(const Node&) = delete;
		Node(Node&&) = delete;
		Node& operator=(const Node&) = delete;
		Node& operator=(Node&&) = delete;
		//Node�� ���԰� ������ Position ��ü�� �����ϱ� ������ ����� �̵� ������, �����ڸ� ����
		~Node() { }
		friend Position;
		const T& get_data() const { return data_; }
		T& get_data() { return data_; }
	};
public:
	class Position {
	private:
		NodePtr pointed_node_;
	public://������ �� �Ҹ���
		Position(NodePtr pos = nullptr) :pointed_node_(pos) {};
		Position(Position&& input_position) noexcept { pointed_node_ = input_position.pointed_node_; input_position.pointed_node_ = nullptr; };
		Position(const Position& input_position) { pointed_node_ = input_position.pointed_node_; }
		Position& operator=(const Position& input_position) { pointed_node_ = input_position.pointed_node_; return *this; }
		Position& operator=(Position&& input_position) noexcept { pointed_node_ = input_position.pointed_node_; input_position.pointed_node_ = nullptr; return *this; }
		~Position() {}
	public:
		NodePtr& operator*() { return pointed_node_; };//Reading and Writing
		const NodePtr& operator*() const { return pointed_node_; }//Only Reading
		Position left() const { return Position(pointed_node_->left_child_); }; //���� �ڽ�
		Position right() const { return Position(pointed_node_->right_child_); }; //������ �ڽ�
		Position parent() const { return Position(pointed_node_->parent_.lock()); }; //�θ��� ���
		void SetLeftChildren(const T& left_value) { 
			if (left().IsNull()) pointed_node_->left_child_ = std::make_shared<Node>(pointed_node_);
			left().operator*()->data_ = left_value;
		}
		void SetRightChildren(const T& right_value) { 
			if (right().IsNull()) pointed_node_->right_child_ = std::make_shared<Node>(pointed_node_);
			right().operator*()->data_ = right_value;
		}
		T DeleteNode() { T temp_data{ pointed_node_->data_ }; pointed_node_.reset(); return temp_data; }
		const T& get_data() const { return pointed_node_->data_; }
		bool IsRoot() const { return pointed_node_->parent == nullptr; }; //���� ����Ű�� �ִ� ��尡 ��Ʈ�ΰ�?
		bool IsNull() const { return pointed_node_ == nullptr; }
		bool IsExternal() const { return pointed_node_->left_child_ == nullptr && pointed_node_->right_child_ == nullptr; }; //���� ����Ű�� �ִ� ��尡 �ܺ� ����ΰ�?
		bool operator==(Position position) const { return pointed_node_ == position.pointed_node_; }
		friend class BinaryTree<T>;
	};
public:
	BinaryTree() :size_(0) ,root_(nullptr){ };
	BinaryTree(const BinaryTree&) = delete;
	BinaryTree(BinaryTree&&) = delete;
	BinaryTree& operator=(const BinaryTree&) = delete;
	BinaryTree& operator=(BinaryTree&&) = delete;
	//ȥ���� ���ϱ� ���� �̵� �� ���� ������, ������ ����
	~BinaryTree() { Clear(); };
	//�Ҹ��ڿ��� ��� ��带 delete
public:
	size_t get_size() const { return size_; };
	bool IsEmpty() const { return size_ == 0; };
	SimplePosition get_root() const { return Position(root_); };
	SimplePosition Search(const T& data);//Ʈ���� ���� ��ȸ�ϸ鼭 ���Ҹ� ã�´�. ���Ҹ� ã���� �ش� ������ Position�� ��ȯ�ϰ� ������ nullptr�� ��ȯ�Ѵ�.
	PositionList PositionsByPreOrder() const {
		PositionList pos_list;
		PreOrder(root_, pos_list);
		return PositionList(pos_list);
	}//Ʈ���� ���� ��ȸ�ϸ鼭 ��� ����� Position�� list �����̳ʿ� �����Ͽ� ��ȯ�Ѵ�.
	PositionList PositionsByPostOrder() const {
		PositionList pos_list;
		PostOrder(root_, pos_list);
		return PositionList(pos_list);
	}//Ʈ���� ���� ��ȸ�ϸ鼭 ��� ����� Position�� list �����̳ʿ� �����Ͽ� ��ȯ�Ѵ�.
	PositionList PositionsByInOrder() const {
		PositionList pos_list;
		InOrder(root_, pos_list);
		return PositionList(pos_list);
	}//Ʈ���� ���� ��ȸ�ϸ鼭 ��� ����� Position�� list �����̳ʿ� �����Ͽ� ��ȯ�Ѵ�.
	void AddRoot(const T& data);//Root�� �߰��Ѵ�. Root�� �̹� �����Ѵٸ� �ƹ��͵� ���� �ʴ´�.
	void ExpandExternal(const Position& position);//�ܺ� ��带 ���� ���� �����Ѵ�. �ڽ� ������ T()�� ���´�.
	void SetLeftChildrenOfPosition(Position position, const T& left_value);
	void SetRightChildrenOfPosition(Position position, const T& right_value);
	//���� ����� �ڽ� ����� ���� �����Ѵ�. ���ڷ� �־��� existent_value�� �������� ���� �� Error.
	SimplePosition RemoveAboveExternal(const Position& position);
	//�ܺ� ���� ���� �θ� �����Ѵ�. ���� �θ� ��尡 �ܺ� ����� ������ ��ü�ȴ�.
	void PrintByPreOrder() const;//���� ��ȸ�ϸ鼭 �� ��尡 ���� ���Ҹ� ����Ѵ�.
	void PrintByPostOrder() const;//���� ��ȸ�ϸ鼭 �� ��尡 ���� ���Ҹ� ����Ѵ�.
	void PrintByInOrder() const;//���� ��ȸ�ϸ鼭 �� ��尡 ���� ���Ҹ� ����Ѵ�.
	void Clear();//��� ��带 �����Ѵ�.
	SimplePositionList VectorOfSubTree(const Position& position);
protected:
	void PreOrder(Position pointed_node, PositionList& position_list) const;//Ʈ���� ���� ��ȸ�ϴ� ���� ���� �Լ��̴�.
	void PostOrder(Position pointed_node, PositionList& position_list)const;//Ʈ���� ���� ��ȸ�ϴ� ���� ���� �Լ��̴�.
	void InOrder(Position pointed_node, PositionList& position_list) const;//Ʈ���� ���� ��ȸ�ϴ� ���� ���� �Լ��̴�.
private:
	NodePtr root_;
	size_t size_;//����� ����.
};
#include "BinaryTree.hpp"

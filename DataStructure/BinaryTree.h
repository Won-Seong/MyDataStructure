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
		T data_; //갖고 있는 데이터
		NodeWeakPtr parent_; //부모의 노드, 순환 문제를 해결하기 위하여 weak_ptr로 참조
		NodePtr left_child_; //왼쪽 자식의 노드
		NodePtr right_child_; //오른쪽 자식의 노드
	public:
		Node(const T& data) :data_(data), left_child_(nullptr), right_child_(nullptr) {};//노드가 루트일 경우의 생성자
		Node(NodePtr parent , const T& data = T()) :data_(data), left_child_(nullptr), right_child_(nullptr), parent_(parent) {};//일반 노드의 생성자
		Node(const Node&) = delete;
		Node(Node&&) = delete;
		Node& operator=(const Node&) = delete;
		Node& operator=(Node&&) = delete;
		//Node의 삽입과 삭제는 Position 객체로 수행하기 때문에 복사와 이동 생성자, 연산자를 삭제
		~Node() { }
		friend Position;
		const T& get_data() const { return data_; }
		T& get_data() { return data_; }
	};
public:
	class Position {
	private:
		NodePtr pointed_node_;
	public://생성자 및 소멸자
		Position(NodePtr pos = nullptr) :pointed_node_(pos) {};
		Position(Position&& input_position) noexcept { pointed_node_ = input_position.pointed_node_; input_position.pointed_node_ = nullptr; };
		Position(const Position& input_position) { pointed_node_ = input_position.pointed_node_; }
		Position& operator=(const Position& input_position) { pointed_node_ = input_position.pointed_node_; return *this; }
		Position& operator=(Position&& input_position) noexcept { pointed_node_ = input_position.pointed_node_; input_position.pointed_node_ = nullptr; return *this; }
		~Position() {}
	public:
		NodePtr& operator*() { return pointed_node_; };//Reading and Writing
		const NodePtr& operator*() const { return pointed_node_; }//Only Reading
		Position left() const { return Position(pointed_node_->left_child_); }; //왼쪽 자식
		Position right() const { return Position(pointed_node_->right_child_); }; //오른쪽 자식
		Position parent() const { return Position(pointed_node_->parent_.lock()); }; //부모의 노드
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
		bool IsRoot() const { return pointed_node_->parent == nullptr; }; //지금 가리키고 있는 노드가 루트인가?
		bool IsNull() const { return pointed_node_ == nullptr; }
		bool IsExternal() const { return pointed_node_->left_child_ == nullptr && pointed_node_->right_child_ == nullptr; }; //지금 가리키고 있는 노드가 외부 노드인가?
		bool operator==(Position position) const { return pointed_node_ == position.pointed_node_; }
		friend class BinaryTree<T>;
	};
public:
	BinaryTree() :size_(0) ,root_(nullptr){ };
	BinaryTree(const BinaryTree&) = delete;
	BinaryTree(BinaryTree&&) = delete;
	BinaryTree& operator=(const BinaryTree&) = delete;
	BinaryTree& operator=(BinaryTree&&) = delete;
	//혼잡을 피하기 위해 이동 및 복사 생성자, 연산자 삭제
	~BinaryTree() { Clear(); };
	//소멸자에서 모든 노드를 delete
public:
	size_t get_size() const { return size_; };
	bool IsEmpty() const { return size_ == 0; };
	SimplePosition get_root() const { return Position(root_); };
	SimplePosition Search(const T& data);//트리를 전위 순회하면서 원소를 찾는다. 원소를 찾으면 해당 원소의 Position을 반환하고 없으면 nullptr을 반환한다.
	PositionList PositionsByPreOrder() const {
		PositionList pos_list;
		PreOrder(root_, pos_list);
		return PositionList(pos_list);
	}//트리를 전위 순회하면서 모든 노드의 Position을 list 컨테이너에 저장하여 반환한다.
	PositionList PositionsByPostOrder() const {
		PositionList pos_list;
		PostOrder(root_, pos_list);
		return PositionList(pos_list);
	}//트리를 후위 순회하면서 모든 노드의 Position을 list 컨테이너에 저장하여 반환한다.
	PositionList PositionsByInOrder() const {
		PositionList pos_list;
		InOrder(root_, pos_list);
		return PositionList(pos_list);
	}//트리를 중위 순회하면서 모든 노드의 Position을 list 컨테이너에 저장하여 반환한다.
	void AddRoot(const T& data);//Root를 추가한다. Root가 이미 존재한다면 아무것도 하지 않는다.
	void ExpandExternal(const Position& position);//외부 노드를 내부 노드로 변경한다. 자식 노드들은 T()를 갖는다.
	void SetLeftChildrenOfPosition(Position position, const T& left_value);
	void SetRightChildrenOfPosition(Position position, const T& right_value);
	//내부 노드의 자식 노드의 값을 변경한다. 인자로 넣었던 existent_value가 존재하지 않을 시 Error.
	SimplePosition RemoveAboveExternal(const Position& position);
	//외부 노드와 그의 부모를 삭제한다. 그의 부모 노드가 외부 노드의 형제로 대체된다.
	void PrintByPreOrder() const;//전위 순회하면서 각 노드가 가진 원소를 출력한다.
	void PrintByPostOrder() const;//후위 순회하면서 각 노드가 가진 원소를 출력한다.
	void PrintByInOrder() const;//중위 순회하면서 각 노드가 가진 원소를 출력한다.
	void Clear();//모든 노드를 삭제한다.
	SimplePositionList VectorOfSubTree(const Position& position);
protected:
	void PreOrder(Position pointed_node, PositionList& position_list) const;//트리를 전위 순회하는 내부 편의 함수이다.
	void PostOrder(Position pointed_node, PositionList& position_list)const;//트리를 후위 순회하는 내부 편의 함수이다.
	void InOrder(Position pointed_node, PositionList& position_list) const;//트리를 중위 순회하는 내부 편의 함수이다.
private:
	NodePtr root_;
	size_t size_;//노드의 개수.
};
#include "BinaryTree.hpp"

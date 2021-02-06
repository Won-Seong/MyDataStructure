#pragma once

template <typename T>
class CircularLinkedList : public std::exception {
protected:
	struct Node;
	using NodePtr = std::shared_ptr<Node>;
private:
	struct Node {
		T data;
		NodePtr next_node;
		Node(const T& data) :data(data), next_node(nullptr) {};
		~Node() {}
	};
public:
	CircularLinkedList() :size_(0), cursor_(nullptr) {};//일반 생성자
	CircularLinkedList(const CircularLinkedList&) = delete;//복사 생성자는 지원하지 않는다.
	CircularLinkedList(CircularLinkedList&& input_list) {
		size_ = input_list.size_;
		cursor_ = input_list.cursor_;
		input_list.cursor_ = nullptr;
		input_list.size_ = 0;
	}//이동 생성자
	CircularLinkedList& operator=(const CircularLinkedList&) = delete;//대입 연산자는 지원하지 않는다.
	CircularLinkedList& operator=(CircularLinkedList&& input_list) {
		size_ = input_list.size_;
		cursor_ = input_list.cursor_;
		input_list.cursor_ = nullptr;
		input_list.size_ = 0;
		return *this;
	}//대입 이동 연산자
	~CircularLinkedList() {
		Clear();
	}
public:
	void Add(const T& data);//커서의 다음에 노드를 삽입한다. O(1)
	const T& Cursor() const;//커서에 의해 참조되는 원소를 반환한다. O(1)
	const T& CursorNext() const;//커서의 다음 노드의 원소를 반환한다. O(1)
	void Advance();//리스트의 다음 노드로 커서를 이동시킨다. O(1)
	int get_size() const;
	T Remove();//커서의 다음 노드를 삭제한다. O(1)
	void Print() const;//모든 원소를 출력한다.
	void Clear();//모든 노드를 삭제한다.
	bool Empty() const;//비어 있으면 true를 반환한다.
private:
	NodePtr cursor_;//리스트의 어느 한 위치를 가리키는 커서.
	size_t size_;
};
#include "CircularLinkedList.hpp"
#pragma once

template <typename T>
class SinglyLinkedList : public std::exception {
protected:
	struct Node;
	using NodePtr = std::shared_ptr<Node>;
	NodePtr head_;
	size_t size_;
private:
	struct Node {
		T data;
		NodePtr next_node;
		Node(const T& data) :data(data), next_node(nullptr) {};
		~Node() {}
	};
public:
	SinglyLinkedList() :size_(0), head_(nullptr) {};//일반 생성자
	SinglyLinkedList(const SinglyLinkedList& input_list) = delete;//복사 생성자는 지원하지 않는다.
	SinglyLinkedList(SinglyLinkedList&& list) {
		size_ = list.size_;
		head_ = list.head_;
		list.head_ = nullptr;
		list.size_ = 0;
	}//이동 생성자, head를 이동.
	SinglyLinkedList& operator=(const SinglyLinkedList& list) = delete;//대입 연산자는 지원하지 않는다.
	SinglyLinkedList& operator=(SinglyLinkedList&& list) {
		size_ = list.size_;
		head_ = list.head_;
		list.head_ = nullptr;
		list.size_ = 0;
		return *this;
	}//대입 이동 연산자
	~SinglyLinkedList() {
		Clear();
	}
public:
	void Clear();
	void PushBack(const T& data);//원소를 추가한다. O(1)
	void Insert(int idx, const T& data);//원소를 중간에 삽입한다. O(1)
	size_t get_size() const;
	const T& get_front() const;
	T Erase(int idx);//원소를 제거한다. O(1)
	T At(int idx) const;//중간에 있는 원소를 참조한다. O(1)
	void Print() const;//모든 원소를 출력한다.
	bool Empty() const { return size_ == 0; }
public:
	T& operator[](int index) const;
};
#include "SinglyLinkedList.hpp"
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
	SinglyLinkedList() :size_(0), head_(nullptr) {};//�Ϲ� ������
	SinglyLinkedList(const SinglyLinkedList& input_list) = delete;//���� �����ڴ� �������� �ʴ´�.
	SinglyLinkedList(SinglyLinkedList&& list) {
		size_ = list.size_;
		head_ = list.head_;
		list.head_ = nullptr;
		list.size_ = 0;
	}//�̵� ������, head�� �̵�.
	SinglyLinkedList& operator=(const SinglyLinkedList& list) = delete;//���� �����ڴ� �������� �ʴ´�.
	SinglyLinkedList& operator=(SinglyLinkedList&& list) {
		size_ = list.size_;
		head_ = list.head_;
		list.head_ = nullptr;
		list.size_ = 0;
		return *this;
	}//���� �̵� ������
	~SinglyLinkedList() {
		Clear();
	}
public:
	void Clear();
	void PushBack(const T& data);//���Ҹ� �߰��Ѵ�. O(1)
	void Insert(int idx, const T& data);//���Ҹ� �߰��� �����Ѵ�. O(1)
	size_t get_size() const;
	const T& get_front() const;
	T Erase(int idx);//���Ҹ� �����Ѵ�. O(1)
	T At(int idx) const;//�߰��� �ִ� ���Ҹ� �����Ѵ�. O(1)
	void Print() const;//��� ���Ҹ� ����Ѵ�.
	bool Empty() const { return size_ == 0; }
public:
	T& operator[](int index) const;
};
#include "SinglyLinkedList.hpp"
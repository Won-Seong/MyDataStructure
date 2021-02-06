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
	CircularLinkedList() :size_(0), cursor_(nullptr) {};//�Ϲ� ������
	CircularLinkedList(const CircularLinkedList&) = delete;//���� �����ڴ� �������� �ʴ´�.
	CircularLinkedList(CircularLinkedList&& input_list) {
		size_ = input_list.size_;
		cursor_ = input_list.cursor_;
		input_list.cursor_ = nullptr;
		input_list.size_ = 0;
	}//�̵� ������
	CircularLinkedList& operator=(const CircularLinkedList&) = delete;//���� �����ڴ� �������� �ʴ´�.
	CircularLinkedList& operator=(CircularLinkedList&& input_list) {
		size_ = input_list.size_;
		cursor_ = input_list.cursor_;
		input_list.cursor_ = nullptr;
		input_list.size_ = 0;
		return *this;
	}//���� �̵� ������
	~CircularLinkedList() {
		Clear();
	}
public:
	void Add(const T& data);//Ŀ���� ������ ��带 �����Ѵ�. O(1)
	const T& Cursor() const;//Ŀ���� ���� �����Ǵ� ���Ҹ� ��ȯ�Ѵ�. O(1)
	const T& CursorNext() const;//Ŀ���� ���� ����� ���Ҹ� ��ȯ�Ѵ�. O(1)
	void Advance();//����Ʈ�� ���� ���� Ŀ���� �̵���Ų��. O(1)
	int get_size() const;
	T Remove();//Ŀ���� ���� ��带 �����Ѵ�. O(1)
	void Print() const;//��� ���Ҹ� ����Ѵ�.
	void Clear();//��� ��带 �����Ѵ�.
	bool Empty() const;//��� ������ true�� ��ȯ�Ѵ�.
private:
	NodePtr cursor_;//����Ʈ�� ��� �� ��ġ�� ����Ű�� Ŀ��.
	size_t size_;
};
#include "CircularLinkedList.hpp"
#pragma once

template <typename T>
class DoubleLinkedList : public std::exception {
protected:
	struct Node;
	using NodePtr = std::shared_ptr<Node>;
private:
	struct Node {
		T data;
		NodePtr prev;
		NodePtr next;
	public:
		Node(const T& data = T()) :data(data) {
			prev = nullptr;
			next = nullptr;
		}//���ڰ� ������ header Ȥ�� trailer.
		Node(const T& data, NodePtr prev, NodePtr next) :data(data), prev(prev), next(next) {}
		~Node() {}
	};
public:
	DoubleLinkedList() : size_(0) {
		header_ = std::make_shared<Node>();
		trailer_ = std::make_shared<Node>();
		header_->next = trailer_;
		trailer_->prev = header_;
	}
	DoubleLinkedList(DoubleLinkedList&& list) :size_(list.size_) {
		header_ = list.header_;
		trailer_ = list.trailer_;
		list.size_ = 0;
		list.header_ = std::make_shared<Node>();
		list.trailer_ = std::make_shared<Node>();
		list.header_->next = trailer_;
		list.trailer_->prev = header_;
	}//�̵� ������
	DoubleLinkedList& operator=(DoubleLinkedList&& list) {
		size_ = list.size_;
		header_ = list.header_;
		trailer_ = list.trailer_;
		list.size_ = 0;
		list.header_ = std::make_shared<Node>();
		list.trailer_ = std::make_shared<Node>();
		list.header_->next = trailer_;
		list.trailer_->prev = header_;
		return *this;
	}//�̵� ���� ������
	DoubleLinkedList(const DoubleLinkedList& list) = delete;
	DoubleLinkedList& operator=(const DoubleLinkedList& list) = delete;
	~DoubleLinkedList() {
		Clear();
		header_.reset();
		trailer_.reset();
	}
public:
	void Clear();//��� ��带 �����.
	void PushBack(const T& data);//Ʈ���Ϸ��� �տ� ��带 �߰��Ѵ�. O(1)
	void PushFront(const T& data);//����� �ڿ� ��带 �߰��Ѵ�. O(1)
	void Insert(const int& idx, const T& data);//�߰��� ���Ҹ� �߰��Ѵ�. O(1)
	void Set(const int& idx, const T& data);//Ư�� �ε����� ���Ҹ� �����Ѵ�.
	T PopBack();//Ʈ���Ϸ� �տ� ��带 �����Ѵ�. O(1)
	T PopFront();//��� ���� ��带 �����Ѵ�. O(1)
	const T& Front() const;//O(1)
	const T& Back() const;//O(1)
	T Erase(const int& idx);//�߰��� ���Ҹ� �����Ѵ�. O(1)
	T& At(const int& idx) const;//���Ҹ� �����Ѵ�. O(1)
	int get_size() const;
	void Print() const;
	bool Empty() const { return size_ == 0; }
public:
	T& operator[](const int& idx) const;//O(1)
private:
	size_t size_;
	NodePtr header_; //front
	NodePtr trailer_; //back
};
#include "DoublyLinkedList.hpp"
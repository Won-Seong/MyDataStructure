#pragma once

template <typename T>
class Queue : public std::exception {
private:
	struct Node;//Node�� Queue�� ���� Ŭ�����̴�.
	using NodePtr = std::shared_ptr<Node>;
protected:
	struct Node {
		T data;//���� Data�� ���� �ִ� ���� Node�̴�.
		NodePtr next_node;
		Node(const T& data) :data(data), next_node(nullptr) {};
		~Node() { }
	};
public://Maintenance Block
	Queue() :size_(0), front_(nullptr), back_(nullptr) {};//������
	Queue(const Queue& input_queue) = delete;//�������� ���ϱ� ���� ���� �����ڸ� �����Ͽ���.
	Queue(Queue&& input_queue) {
		size_ = input_queue.size_;
		front_ = std::move(input_queue.front_);
		back_ = std::move(input_queue.back_);
		input_queue.size_ = 0;
	}//�̵� ������
	Queue& operator=(const Queue& input_queue) = delete;//�������� ���ϱ� ���� ���� �����ڸ� �����Ͽ���.
	Queue& operator=(Queue&& input_queue) {
		size_ = input_queue.size_;
		front_ = std::move(input_queue.front_);
		back_ = std::move(input_queue.back_);
		input_queue.size_ = 0;
		return *this;
	}//�̵� ���� ������
	~Queue() {
		Clear();
	}
public://Fucntion Block
	void Clear();//��� ���Ҹ� ����� �Լ��̴�.
	void Push(const T& data);//O(1)
	int get_size() const;
	T Pop();//O(1)
	T Front() const;//O(1)
	void Print() const;//��� ���Ҹ� ����ϴ� �Լ��̴�.
private:
	NodePtr front_;//���� ����Ų��.
	NodePtr back_;//�ڸ� ����Ų��.
	size_t size_;//Queue�� ���� �ִ� Node�� ����.
};
#include "Queue.hpp"
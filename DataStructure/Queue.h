#pragma once

template <typename T>
class Queue : public std::exception {
private:
	struct Node;//Node는 Queue의 내부 클래스이다.
	using NodePtr = std::shared_ptr<Node>;
protected:
	struct Node {
		T data;//실제 Data를 갖고 있는 것은 Node이다.
		NodePtr next_node;
		Node(const T& data) :data(data), next_node(nullptr) {};
		~Node() { }
	};
public://Maintenance Block
	Queue() :size_(0), front_(nullptr), back_(nullptr) {};//생성자
	Queue(const Queue& input_queue) = delete;//복잡함을 피하기 위해 복사 생성자를 삭제하였다.
	Queue(Queue&& input_queue) {
		size_ = input_queue.size_;
		front_ = std::move(input_queue.front_);
		back_ = std::move(input_queue.back_);
		input_queue.size_ = 0;
	}//이동 생성자
	Queue& operator=(const Queue& input_queue) = delete;//복잡함을 피하기 위해 대입 연산자를 삭제하였다.
	Queue& operator=(Queue&& input_queue) {
		size_ = input_queue.size_;
		front_ = std::move(input_queue.front_);
		back_ = std::move(input_queue.back_);
		input_queue.size_ = 0;
		return *this;
	}//이동 대입 연산자
	~Queue() {
		Clear();
	}
public://Fucntion Block
	void Clear();//모든 원소를 지우는 함수이다.
	void Push(const T& data);//O(1)
	int get_size() const;
	T Pop();//O(1)
	T Front() const;//O(1)
	void Print() const;//모든 원소를 출력하는 함수이다.
private:
	NodePtr front_;//앞을 가리킨다.
	NodePtr back_;//뒤를 가리킨다.
	size_t size_;//Queue가 갖고 있는 Node의 개수.
};
#include "Queue.hpp"
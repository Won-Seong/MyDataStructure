#pragma once

template <typename T>
class Stack : public std::exception {
private:
	using UniquePtr = std::unique_ptr < T[] > ;
	UniquePtr data_;
	size_t capacity_;
	size_t size_;
public://�������� ���
	Stack(size_t capacity = 1) :capacity_(capacity), size_(0) {
		data_ = std::make_unique<T[]>(capacity_);
	}
	Stack(const Stack& stack) {
		size_ = stack.size_;
		capacity_ = stack.capacity_;
		data_ = std::make_unique<T[]>(capacity_);
		for (int i = 0; i < size_; i++)
			data_[i] = stack.data_[i];
	}//���� ������
	Stack(Stack&& stack) {
		size_ = stack.size_;
		capacity_ = stack.capacity_;
		data_ = std::move(stack.data_);
		stack.size_ = 0;
		stack.capacity_ = 1;
	}//�̵� ������
	Stack& operator=(const Stack& stack) {
		size_ = stack.size_;
		capacity_ = stack.capacity_;
		data_.reset();
		data_ = std::make_unique<T[]>(capacity_);
		for (int i = 0; i < size_; i++)
			data_[i] = stack.data_[i];
		return *this;
	}//���� ������
	Stack& operator=(Stack&& stack) {
		size_ = stack.size_;
		capacity_ = stack.capacity_;
		data_.reset();
		data_ = std::move(stack.data_);
		stack.size_ = 0;
		stack.capacity_ = 1;
		return *this;
	}//���� �̵� ������
	~Stack() {
		data_.reset();
	}
private:
	void SizeCheck();//�迭�� ũ�⸦ �˻��ϴ� ���� �Լ��̴�.
public:
	void Reserve(size_t size);//������ �����ϴ� �Լ��̴�. ������ ������ ������ ���� ������ �� ��� �ø���.
	T& Push(T data);//O(1)
	T Pop();//���� ���� �ִ� ���Ҹ� Stack ������ ������ �� ��ȯ�Ѵ�. O(1)
	int get_size() const;
	int get_capacity() const;
	bool Empty() const;//�迭�� ��� ������ true�� ��ȯ�Ѵ�.
	T Top() const;//���� ���� �ִ� ���Ҹ� ��ȯ�Ѵ�. O(1)
	void Print() const;//Stack�� ��� ���Ҹ� ����Ѵ�. 
};
#include "Stack.hpp"
#pragma once

template <typename T>
class Stack : public std::exception {
private:
	using UniquePtr = std::unique_ptr < T[] > ;
	UniquePtr data_;
	size_t capacity_;
	size_t size_;
public://유지보수 블록
	Stack(size_t capacity = 1) :capacity_(capacity), size_(0) {
		data_ = std::make_unique<T[]>(capacity_);
	}
	Stack(const Stack& stack) {
		size_ = stack.size_;
		capacity_ = stack.capacity_;
		data_ = std::make_unique<T[]>(capacity_);
		for (int i = 0; i < size_; i++)
			data_[i] = stack.data_[i];
	}//복사 생성자
	Stack(Stack&& stack) {
		size_ = stack.size_;
		capacity_ = stack.capacity_;
		data_ = std::move(stack.data_);
		stack.size_ = 0;
		stack.capacity_ = 1;
	}//이동 생성자
	Stack& operator=(const Stack& stack) {
		size_ = stack.size_;
		capacity_ = stack.capacity_;
		data_.reset();
		data_ = std::make_unique<T[]>(capacity_);
		for (int i = 0; i < size_; i++)
			data_[i] = stack.data_[i];
		return *this;
	}//대입 연산자
	Stack& operator=(Stack&& stack) {
		size_ = stack.size_;
		capacity_ = stack.capacity_;
		data_.reset();
		data_ = std::move(stack.data_);
		stack.size_ = 0;
		stack.capacity_ = 1;
		return *this;
	}//대입 이동 연산자
	~Stack() {
		data_.reset();
	}
private:
	void SizeCheck();//배열의 크기를 검사하는 내부 함수이다.
public:
	void Reserve(size_t size);//공간을 예약하는 함수이다. 공간이 부족할 때마다 여유 공간을 두 배로 늘린다.
	T& Push(T data);//O(1)
	T Pop();//가장 위에 있는 원소를 Stack 내에서 제거한 후 반환한다. O(1)
	int get_size() const;
	int get_capacity() const;
	bool Empty() const;//배열이 비어 있으면 true를 반환한다.
	T Top() const;//가장 위에 있는 원소를 반환한다. O(1)
	void Print() const;//Stack의 모든 원소를 출력한다. 
};
#include "Stack.hpp"
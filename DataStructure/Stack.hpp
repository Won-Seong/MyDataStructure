#pragma once
#include <iostream>
#include "Stack.h"

template<typename T>
void Stack<T>::SizeCheck()
{
	if (size_ >= capacity_) {
		capacity_ *= 2;
		UniquePtr temp = std::make_unique<T[]>(capacity_);
		for (int i = 0; i < size_; i++)
			temp[i] = data_[i];
		data_.reset();
		data_ = std::move(temp);
	}
}

template<typename T>
void Stack<T>::Reserve(size_t size_)
{
	if (capacity_ >= size_)
		return;//size가 예약할 공간보다 이미 큰 경우에는 아무것도 하지 않는다.
	capacity_ = size_;
	UniquePtr temp = std::make_unique<T[]>(capacity_);
	for (int i = 0; i < size_; i++)
		temp[i] = data_[i];
	data_ = std::move(temp);
}

template<typename T>
T& Stack<T>::Push(T data)
{
	SizeCheck();
	data_[size_++] = data;
	return data_[size_];
}

template<typename T>
T Stack<T>::Pop()
{
	if (Empty())
		throw std::length_error("No element!");
	return data_[--size_];
}

template<typename T>
int Stack<T>::get_size() const
{
	return size_;
}

template<typename T>
int Stack<T>::get_capacity() const
{
	return capacity_;
}

template<typename T>
bool Stack<T>::Empty() const
{
	return size_ == 0;
}

template<typename T>
T Stack<T>::Top() const
{
	if (Empty())
		throw std::length_error("No element!");
	return data_[size_ - 1];
}

template<typename T>
void Stack<T>::Print() const
{
	std::cout << "| ";
	for (int i = 0; i < size_; i++)
		std::cout << data_[i] << " | ";
	std::cout << std::endl;
}

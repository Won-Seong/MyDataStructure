#pragma once
#include <iostream>
#include "Vector.h"

template<typename T>
void Vector<T>::CapacityCheck()
{
	if (size_ >= capacity_) {
		capacity_ *= 2;
		std::unique_ptr<T[]> temp = std::make_unique<T[]>(capacity_);
		for (int i = 0; i < size_; i++)
			temp[i] = element_ptr_[i];
		element_ptr_ = std::move(temp);
	}
}

template<typename T>
void Vector<T>::Reserve(const size_t& input_capacity)
{
	if (capacity_ >= input_capacity) return;
	capacity_ = input_capacity;
	std::unique_ptr<T[]> temp = std::move(element_ptr_);
	element_ptr_ = std::make_unique<T[]>(capacity_);
	for (int i = 0; i < size_; i++)
		element_ptr_[i] = temp[i];
}

template<typename T>
void Vector<T>::PushBack(const T& data)
{
	CapacityCheck();
	element_ptr_[size_++] = data;
}

template<typename T>
T Vector<T>::PopBack()
{
	if (size_ <= 0) throw std::length_error("No element!");
	return element_ptr_[--size_];
}

template<typename T>
void Vector<T>::Set(const unsigned int& index, const T& data)
{
	if (index < 0 || index >= size_) throw std::out_of_range("Index error!");
	T temp_data = element_ptr_[index];
	element_ptr_[index] = data;
}

template<typename T>
void Vector<T>::Insert(const unsigned int& index, const T& data)
{
	if (index < 0 || index >= size_) throw std::out_of_range("Index error!");
	CapacityCheck();
	for (int i = size_; i != index; i--) {
		element_ptr_[i] = element_ptr_[i - 1];
	}
	element_ptr_[index] = data;
	size_++;
}

template<typename T>
T Vector<T>::Erase(const unsigned int& index)
{
	if (index < 0 || index >= size_) throw std::out_of_range("Index error!");
	T temp_data = element_ptr_[index];
	for (int i = index; i < size_ - 1; i++)
		element_ptr_[i] = element_ptr_[i + 1];
	size_--;
	return temp_data;
}

template<typename T>
const T& Vector<T>::At(const unsigned int& index) const
{
	if (index < 0 || index >= size_) throw std::out_of_range("Index error!");
	return element_ptr_[index];
}

template<typename T>
void Vector<T>::Print() const
{
	if (size_ == 0) {
		std::cout << "No element!" << std::endl;
		return;
	}
	std::cout << "| ";
	for (int i = 0; i < size_; i++)
		std::cout << element_ptr_[i] << " | ";
	std::cout << std::endl;
}

template<typename T>
inline void Vector<T>::Clear()
{
	capacity_ = 1;
	size_ = 0;
	element_ptr_ = std::make_unique<T[]>(capacity_);
}

template<typename T>
bool Vector<T>::operator==(const Vector& other_vector) const
{
	if (size_ != other_vector.size_) return false;
	bool flag = true;
	for (int i = 0; i < size_; i++) {
		if (element_ptr_[i] != other_vector.element_ptr_[i]) {
			flag = false;
			break;
		}
	}
	return flag;
}

template<typename T>
T& Vector<T>::operator[](const unsigned int& index) const
{
	if (index < 0 || index >= size_) throw std::out_of_range("Index error!");
	return element_ptr_[index];
}
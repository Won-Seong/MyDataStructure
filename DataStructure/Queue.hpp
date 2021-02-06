#pragma once
#include <iostream>
#include "Queue.h"

template<typename T>
void Queue<T>::Clear()
{
	if (size_ == 0)
		return;//원소가 없으면 아무것도 하지 않는다.
	NodePtr temp_for_iter = front_;
	NodePtr temp;
	while (temp_for_iter != back_) {
		temp = temp_for_iter;
		temp_for_iter = temp_for_iter->next_node;
		temp.reset();
	}
	temp_for_iter.reset();
	front_.reset();
	back_.reset();
	size_ = 0;
}

template<typename T>
void Queue<T>::Push(const T& data)
{
	if (front_ == nullptr) {
		front_ = std::make_shared<Node>(data);
		back_ = front_;
		size_ = 1;
	}
	else if (back_ == front_) {
		back_ = std::make_shared<Node>(data);
		front_->next_node = back_;
		size_++;
	}
	else {
		NodePtr temp = back_;
		back_ = std::make_shared<Node>(data);
		temp->next_node = back_;
		size_++;
	}
}

template<typename T>
int Queue<T>::get_size() const
{
	return size_;
}

template<typename T>
T Queue<T>::Pop()
{
	if (size_ == 0)
		throw std::length_error("No element!");
	T temp_data = front_->data;
	front_ = front_->next_node;
	size_--;
	return temp_data;
}

template<typename T>
T Queue<T>::Front() const
{
	if (size_ == 0)
		throw std::length_error("No element!");
	return front_->data;
}

template<typename T>
void Queue<T>::Print() const
{
	if (size_ == 0) {
		std::cout << "No element!" << std::endl;
		return;
	}
	NodePtr temp_for_iter = front_;
	unsigned int idx = 0;
	while (temp_for_iter != back_) {
		std::cout << "Index " << idx << " : " << temp_for_iter->data << '\n';
		temp_for_iter = temp_for_iter->next_node;
		idx++;
	}
	std::cout << "Index " << idx << " : " << temp_for_iter->data << '\n';
}
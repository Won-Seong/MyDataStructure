#pragma once
#include <iostream>
#include "CircularLinkedList.h"

template<typename T>
void CircularLinkedList<T>::Add(const T& data)
{
	NodePtr temp = std::make_shared<Node>(data);
	if (cursor_ == nullptr) {
		temp->next_node = temp;
		cursor_ = temp;
	}
	else {
		temp->next_node = cursor_->next_node;
		cursor_->next_node = temp;
	}
	size_++;
}

template<typename T>
const T& CircularLinkedList<T>::Cursor() const
{
	if (cursor_ == nullptr) throw std::length_error("No element!");
	return cursor_->data;
}

template<typename T>
const T& CircularLinkedList<T>::CursorNext() const
{
	if (cursor_ == nullptr) throw std::length_error("No element!");
	return cursor_->next_node->data;
}

template<typename T>
void CircularLinkedList<T>::Advance()
{
	if (cursor_ == nullptr) throw std::length_error("No element!");
	cursor_ = cursor_->next_node;
}

template<typename T>
int CircularLinkedList<T>::get_size() const
{
	return size_;
}

template<typename T>
T CircularLinkedList<T>::Remove()
{
	if (cursor_ == nullptr) throw std::length_error("No element!");
	NodePtr temp = cursor_->next_node;
	T temp_data = temp->data;
	if (temp == cursor_)
		cursor_.reset();
	else
	{
		cursor_->next_node = temp->next_node;
		temp.reset();
	}
	size_--;
	return temp_data;
}

template<typename T>
void CircularLinkedList<T>::Print() const
{
	if (cursor_ == nullptr) {
		std::cout << "There is no element!" << std::endl;
		return;
	}
	NodePtr temp = cursor_;
	std::cout << "| ";
	while (temp->next_node != cursor_) {
		std::cout << temp->data << " | ";
		temp = temp->next_node;
	}
	std::cout << temp->data << " | " << std::endl;
}

template<typename T>
void CircularLinkedList<T>::Clear()
{
	while (!Empty())
		Remove();
	size_ = 0;
}

template<typename T>
bool CircularLinkedList<T>::Empty() const
{
	return cursor_ == nullptr ? true : false;
}

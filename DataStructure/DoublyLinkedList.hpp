#pragma once
#include <iostream>
#include "DoublyLinkedList.h"

template<typename T>
void DoubleLinkedList<T>::Clear()
{
	if (size_ == 0)
		return;//원소가 없으면 아무것도 하지 않는다.
	NodePtr temp_ptr = header_->next;
	NodePtr ptr_for_delete;
	while (temp_ptr->next != nullptr) {
		ptr_for_delete = temp_ptr;
		temp_ptr = temp_ptr->next;
		ptr_for_delete.reset();
	}
	size_ = 0;
}

template<typename T>
void DoubleLinkedList<T>::PushBack(const T& data)
{
	NodePtr temp = trailer_->prev;
	NodePtr new_node = std::make_shared<Node>(data, temp, trailer_);
	temp->next = new_node;
	trailer_->prev = new_node;
	size_++;
}

template<typename T>
void DoubleLinkedList<T>::PushFront(const T& data)
{
	NodePtr temp = header_->next;
	NodePtr new_node = std::make_shared<Node>(data, header_, temp);
	temp->prev = new_node;
	header_->next = new_node;
	size_++;
}

template<typename T>
void DoubleLinkedList<T>::Insert(const int& idx, const T& data)
{
	if (idx < 0 || idx > size_)
		throw std::out_of_range("Invalid index!");
	if (idx >= (size_ / 2)) {
		NodePtr temp_ptr = trailer_->prev;
		for (int i = 0; i < (size_ - idx - 1); i++)
			temp_ptr = temp_ptr->prev;
		NodePtr temp_prev_ptr = temp_ptr->prev;
		NodePtr new_node = std::make_shared<Node>(data,temp_prev_ptr , temp_ptr);
		temp_prev_ptr->next = new_node;
		temp_ptr->prev = new_node;
	}
	else {
		NodePtr temp_ptr = header_->next;
		for (int i = 0; i < idx; i++)
			temp_ptr = temp_ptr->next;
		NodePtr temp_prev_ptr = temp_ptr->prev;
		NodePtr new_node = std::make_shared<Node>(data, temp_prev_ptr, temp_ptr);
		temp_prev_ptr->next = new_node;
		temp_ptr->prev = new_node;
	}
	size_++;
}

template<typename T>
inline void DoubleLinkedList<T>::Set(const int& idx, const T& data)
{
	if (idx < 0 || idx > size_)
		throw std::out_of_range("Invalid index!");
	if (idx >= (size_ / 2)) {
		NodePtr temp_ptr = trailer_->prev;
		for (int i = 0; i < (size_ - idx - 1); i++)
			temp_ptr = temp_ptr->prev;
		temp_ptr->data = data;
	}
	else {
		NodePtr temp_ptr = header_->next;
		for (int i = 0; i < idx; i++)
			temp_ptr = temp_ptr->next;
		temp_ptr->data = data;
	}
}

template<typename T>
T DoubleLinkedList<T>::PopBack()
{
	if (size_ == 0)
		throw std::out_of_range("No Element!");
	NodePtr temp_ptr = trailer_->prev;
	T temp_data = temp_ptr->data;
	temp_ptr->prev->next = trailer_;
	trailer_->prev = temp_ptr->prev;
	temp_ptr.reset();
	size_--;
	return temp_data;
}

template<typename T>
T DoubleLinkedList<T>::PopFront()
{
	if (size_ == 0)
		throw std::length_error("No Element!");
	NodePtr temp_ptr = header_->next;
	T temp_data = temp_ptr->data;
	temp_ptr->next->prev = header_;
	header_->next = temp_ptr->next;
	temp_ptr.reset();
	size_--;
	return temp_data;
}

template<typename T>
const T& DoubleLinkedList<T>::Front() const
{
	return header_->next->data;
}

template<typename T>
const T& DoubleLinkedList<T>::Back() const
{
	return trailer_->prev->data;
}

template<typename T>
T DoubleLinkedList<T>::Erase(const int& idx)
{
	if (idx < 0 || idx >= size_)
		throw std::out_of_range("Invalid index!");
	else if (size_ == 0)
		throw std::length_error("No element!");
	if (idx >= (size_ / 2)) {
		NodePtr temp_ptr = trailer_->prev;
		for (int i = 0; i < (size_ - idx - 1); i++)
			temp_ptr = temp_ptr->prev;
		T temp_data = temp_ptr->data;
		temp_ptr->prev->next = temp_ptr->next;
		temp_ptr->next->prev = temp_ptr->prev;
		temp_ptr.reset();
		size_--;
		return temp_data;
	}
	else {
		NodePtr temp_ptr = header_->next;
		for (int i = 0; i < idx; i++)
			temp_ptr = temp_ptr->next;
		T temp_data = temp_ptr->data;
		temp_ptr->prev->next = temp_ptr->next;
		temp_ptr->next->prev = temp_ptr->prev;
		temp_ptr.reset();
		size_--;
		return temp_data;
	}
}

template<typename T>
const T& DoubleLinkedList<T>::At(const int& idx) const
{
	if (idx < 0 || idx >= size_)
		throw std::out_of_range("Invalid index!");
	else if (size_ == 0)
		throw std::length_error("No element!");
	if (idx >= (size_ / 2)) {
		NodePtr temp_ptr = trailer_->prev;
		for (int i = 0; i < (size_ - idx - 1); i++)
			temp_ptr = temp_ptr->prev;
		return temp_ptr->data;
	}
	else {
		NodePtr temp_ptr = header_->next;
		for (int i = 0; i < idx; i++)
			temp_ptr = temp_ptr->next;
		return temp_ptr->data;
	}
}

template<typename T>
T& DoubleLinkedList<T>::operator[](const int& idx) const
{
	if (idx < 0 || idx >= size_)
		throw std::out_of_range("Invalid index!");
	else if (size_ == 0)
		throw std::length_error("No element!");

	if (idx >= (size_ / 2)) {
		NodePtr temp_ptr = trailer_->prev;
		for (int i = 0; i < (size_ - idx - 1); i++)
			temp_ptr = temp_ptr->prev;
		return temp_ptr->data;
	}
	else {
		NodePtr temp_ptr = header_->next;
		for (int i = 0; i < idx; i++)
			temp_ptr = temp_ptr->next;
		return temp_ptr->data;
	}
}

template<typename T>
int DoubleLinkedList<T>::get_size() const
{
	return size_;
}

template<typename T>
void DoubleLinkedList<T>::Print() const
{
	if (size_ == 0) {
		std::cout << "No element!" << std::endl;
		return;
	}
	NodePtr temp_ptr = header_->next;
	std::cout << "| ";
	while (temp_ptr->next != nullptr) {
		std::cout << temp_ptr->data << " | ";
		temp_ptr = temp_ptr->next;
	}
	std::cout << std::endl;
}

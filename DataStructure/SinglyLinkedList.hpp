#pragma once
#include <iostream>

template<typename T>
void SinglyLinkedList<T>::Clear()
{
	if (size_ == 0)
		return;
	NodePtr temp_for_iter = head_;
	NodePtr temp;
	while (temp_for_iter->next_node != nullptr) {
		temp = temp_for_iter;
		temp_for_iter = temp_for_iter->next_node;
		temp.reset();
	}
	temp_for_iter.reset();
	head_.reset();
	size_ = 0;
}

template<typename T>
void SinglyLinkedList<T>::PushBack(const T& data)
{
	if (head_ == nullptr) {
		head_ = std::make_shared<Node>(data);
		size_ = 1;
	}
	else {
		NodePtr temp = std::make_shared<Node>(data);
		NodePtr temp_for_iter = head_;
		while (temp_for_iter->next_node != nullptr)
			temp_for_iter = temp_for_iter->next_node;
		temp_for_iter->next_node = temp;
		size_++;
	}
}

template<typename T>
void SinglyLinkedList<T>::Insert(int idx, const T& data)
{
	if (idx < 0 || idx > size_)
		throw std::out_of_range("Out of range!");
	NodePtr temp = std::make_shared<Node>(data);
	NodePtr temp_for_iter = head_;
	for (int i = 0; i < (idx - 1); i++)
		temp_for_iter = temp_for_iter->next_node;
	NodePtr temp_for_link = temp_for_iter;
	temp_for_iter = temp_for_iter->next_node;
	temp->next_node = temp_for_iter;
	temp_for_link->next_node = temp;
	size_++;
}

template<typename T>
size_t SinglyLinkedList<T>::get_size() const
{
	return size_;
}

template<typename T>
const T& SinglyLinkedList<T>::get_front() const
{
	return head_->data;
}

template<typename T>
T SinglyLinkedList<T>::Erase(int idx)
{
	if (idx < 0 || idx >= size_)
		throw std::out_of_range("Out of range!");
	NodePtr temp_for_iter = head_;
	for (int i = 0; i < (idx - 1); i++)
		temp_for_iter = temp_for_iter->next_node;
	NodePtr temp_for_link = temp_for_iter;
	temp_for_iter = temp_for_iter->next_node;
	temp_for_link->next_node = temp_for_iter->next_node;
	T temp_data = temp_for_iter->data;
	temp_for_iter.reset();
	size_--;
	return temp_data;
}

template<typename T>
T SinglyLinkedList<T>::At(int idx) const
{
	if (idx < 0 || idx >= size_)
		throw std::out_of_range("Out of range!");
	NodePtr temp = head_;
	for (int i = 0; i < idx; i++)
		temp = temp->next_node;
	return temp->data;
}

template<typename T>
T& SinglyLinkedList<T>::operator[](int idx) const
{
	if (idx < 0 || idx >= size_)
		throw std::out_of_range("Out of range!");
	NodePtr temp = head_;
	for (int i = 0; i < idx; i++)
		temp = temp->next_node;
	return temp->data;
}

template<typename T>
void SinglyLinkedList<T>::Print() const
{
	NodePtr temp_for_iter = head_;
	int idx = 0;
	std::cout << "| ";
	while (temp_for_iter->next_node != nullptr) {
		std::cout << temp_for_iter->data << " | ";
		temp_for_iter = temp_for_iter->next_node;
		idx++;
	}
	std::cout << temp_for_iter->data << " | " << std::endl;
}
#pragma once
#include <iostream>
#include "BinaryTree.h"

template<typename T>
typename BinaryTree<T>::Position BinaryTree<T>::Search(const T& data)
{
	PositionList pos_list = PositionsByPreOrder();
	for (const auto& itr : pos_list) {
		if (itr.get_data() == data)
			return itr;
	}
	std::cout << "There isn't a node has such data!" << std::endl;
	return Position();
}

template<typename T>
void BinaryTree<T>::AddRoot(const T& data)
{
	if (size_ > 0) {
		std::cout << "There is a root already!" << std::endl;
	}
	else {
		root_ = std::make_shared<Node>(data);
		size_ = 1;
	}
}

template<typename T>
void BinaryTree<T>::ExpandExternal(const Position& position)
{
	NodePtr ptr = position.pointed_node_;
	ptr->left_child_ = std::make_shared<Node>(ptr);
	ptr->right_child_ = std::make_shared<Node>(ptr);
	size_ += 2;
}

template<typename T>
void BinaryTree<T>::SetLeftChildrenOfPosition(Position position, const T& left_value)
{
	if (position.IsNull()) throw std::exception("There is no such value");
	position.SetLeftChildren(left_value);
}

template<typename T>
void BinaryTree<T>::SetRightChildrenOfPosition(Position position, const T& right_value)
{
	if (position.IsNull()) throw std::exception("There is no such value");
	position.SetRightChildren(right_value);
}

template<typename T>
typename BinaryTree<T>::Position BinaryTree<T>::RemoveAboveExternal(const Position& position)
{
	NodeWeakPtr target_node = position.operator*();
	NodeWeakPtr parent = target_node.lock()->parent_;
	NodeWeakPtr sibling;
	if (target_node.lock() == parent.lock()) sibling = parent.lock()->right_child_;
	else sibling = parent.lock()->left_child_;
	if (parent.lock() == root_) {
		root_ = sibling.lock();
		//sibling.parent_.reset();
	}
	else {
		NodeWeakPtr grand_parent = parent.lock()->parent_;
		if (sibling.lock()) {
			if (parent.lock() == grand_parent.lock()->left_child_) grand_parent.lock()->left_child_.reset();
			else grand_parent.lock()->right_child_.reset();
			return Position();
		}
		if (parent.lock() == grand_parent.lock()->left_child_) grand_parent.lock()->left_child_ = sibling.lock();
		else grand_parent.lock()->right_child_ = sibling.lock();
		sibling.lock()->parent_ = grand_parent;
	}
	size_ -= 2;
	return Position(sibling.lock());
}

template<typename T>
void BinaryTree<T>::PrintByPreOrder() const
{
	std::cout << "ByPreOrder" << std::endl;
	PositionList position_list = PositionsByPreOrder();
	for (const auto& itr : position_list)
		std::cout << itr.get_data() << ' ';
	std::cout << std::endl;
}

template<typename T>
void BinaryTree<T>::PrintByPostOrder() const
{
	std::cout << "ByPostOrder" << std::endl;
	PositionList position_list = PositionsByPostOrder();
	for (const auto& itr : position_list)
		std::cout << itr.get_data() << ' ';
	std::cout << std::endl;
}

template<typename T>
void BinaryTree<T>::PrintByInOrder() const
{
	std::cout << "ByInOrder" << std::endl;
	PositionList position_list = PositionsByInOrder();
	for (const auto& itr : position_list)
		std::cout << itr.get_data() << ' ';
	std::cout << std::endl;
}

template<typename T>
void BinaryTree<T>::Clear()
{
	if (size_ == 0) return;
	PositionList position_list;
	PreOrder(root_, position_list);
	for (auto& itr : position_list)
		itr.DeleteNode();
	size_ = 0;
}

template<typename T>
typename BinaryTree<T>::PositionList BinaryTree<T>::VectorOfSubTree(const Position& position)
{
	PositionList pos_list;
	PreOrder(position.pointed_node_, pos_list);
	return pos_list;
}

template<typename T>
void BinaryTree<T>::PreOrder(Position pointed_node, PositionList& position_list) const
{
	position_list.push_back(pointed_node);
	if (!pointed_node.left().IsNull())
		PreOrder(pointed_node.left(), position_list);
	if (!pointed_node.right().IsNull())
		PreOrder(pointed_node.right(), position_list);
}

template<typename T>
void BinaryTree<T>::PostOrder(Position pointed_node, PositionList& position_list) const
{
	if (!pointed_node.left().IsNull())
		PostOrder(pointed_node.left(), position_list);
	if (!pointed_node.right().IsNull())
		PostOrder(pointed_node.right(), position_list);
	position_list.push_back(Position(pointed_node));
}

template<typename T>
void BinaryTree<T>::InOrder(Position pointed_node, PositionList& pos_list) const
{
	if (!pointed_node.left().IsNull())
		InOrder(pointed_node.left(), pos_list);
	pos_list.push_back(Position(pointed_node));
	if (!pointed_node.right().IsNull())
		InOrder(pointed_node.right(), pos_list);
}
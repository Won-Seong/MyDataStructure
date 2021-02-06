#pragma once
#include "Tree.h"

template<typename T>
void Tree<T>::AddRoot(const T& data)
{
	if (size_ != 0) {
		std::cout << "There is a root already!" << std::endl;
	}
	else {
		root_ = std::make_shared<Node>(data);
		size_ = 1;
	}
}

template<typename T>
void Tree<T>::ExpandNode(Position external_node, const T& data)
{
	if (external_node.IsNull()) {
		std::cout << "There is no such node!" << std::endl;
		return;
	}
	NodePtr temp = std::make_shared<Node>(data, external_node.get());
	external_node.get()->InsertChild(temp);
	size_++;
}

template<typename T>
T Tree<T>::EraseNode(Position position)
{
	if (position.IsNull()) throw std::invalid_argument("There is no such node!");
	Position parent_position{ position.parent() };
	parent_position.get()->DeleteChild(position.get());

	for (const auto& itr : position.get()->children) {
		parent_position.get()->InsertChild(itr);
		itr->parent = parent_position.get();
	}

	position.get()->children.clear();
	T temp_data = position.get()->data;
	position.get().reset();
	size_--;
	return temp_data;
}

template<typename T>
inline typename Tree<T>::Position Tree<T>::Search(const T& data) const
{
	PositionList position_vector = PositionsByPreOrder();
	for (auto& itr : position_vector) {
		if (itr.operator*() == data)
			return itr;
	}
	return Position();
}

template<typename T>
void Tree<T>::PrintByPreOrder() const
{
	PositionList position_vec = PositionsByPreOrder();
	for (auto& itr : position_vec)
		std::cout << itr.operator*() << std::endl;
}

template<typename T>
void Tree<T>::PrintChildren(const T& data) const
{
	Position position(Search(data));
	if (position.get() == nullptr || position.get()->children.empty()) {
		std::cout << '0' << std::endl;
		return;
	}
	for (const auto& itr : position.get()->children)
		std::cout << itr->data << ' ';
	std::cout << std::endl;
}

template<typename T>
int Tree<T>::DepthOfNode(const T& data) const
{
	Position position(Search(data));
	int index = 0;
	while (position.get() != root_) {
		position.get() = position.get()->parent;
		index++;
	}
	return index;
}

template<typename T>
void Tree<T>::PreOrder(NodePtr ptr, PositionList& position_vector) const
{
	position_vector.push_back(Position(ptr));
	for (const auto& itr : ptr->children_) {
		PreOrder(itr, position_vector);
	}
}

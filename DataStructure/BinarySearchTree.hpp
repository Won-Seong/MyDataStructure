#pragma once
#include "BinarySearchTree.h"

template<typename T>
typename BinarySearchTree<T>::Iterator& BinarySearchTree<T>::Iterator::operator++()
{
	TreePosition temp_position = position_of_entry_.right();
	if (!temp_position.IsExternal()) {
		do { position_of_entry_ = temp_position; temp_position = temp_position.left(); } while (!temp_position.IsExternal());
	}
	else {
		temp_position = position_of_entry_.parent();
		while (position_of_entry_ == temp_position.right()) {
			position_of_entry_ = temp_position; temp_position = temp_position.parent();
		}
		position_of_entry_ = temp_position;
	}
	return *this;

}

template<typename T>
typename BinarySearchTree<T>::Iterator BinarySearchTree<T>::Find(const K& key)
{
	TreePosition position = Finder(key, root());//가상 루트부터 검색
	if (!position.IsExternal()) return Iterator(position);//찾았다면...
	else return End();//찾지 못했다면...
}

template<typename T>
typename BinarySearchTree<T>::Iterator BinarySearchTree<T>::Insert(const K& key, const V& value)
{
	TreePosition position = Inserter(key, value); return Iterator(position);
}

template<typename T>
void BinarySearchTree<T>::Erase(const K& key)
{
	TreePosition position = Finder(key, root());
	if (position.IsExternal())
		throw std::invalid_argument("Erase of nonexistent");
	Eraser(position);
}

template<typename T>
void BinarySearchTree<T>::Erase(const Iterator& iterator)
{
	Eraser(iterator.position_of_entry_);
}

template<typename T>
typename BinarySearchTree<T>::TreePosition BinarySearchTree<T>::Finder(const K& key, const TreePosition& position)
{
	if (position.IsExternal()) return position;//외부 노드라면 찾지 못한 것
	if (key < position.operator*()->get_data().get_key()) return Finder(key, position.left());//찾으려는 값이 더 작다면 왼쪽으로
	else if (key > position.operator*()->get_data().get_key()) return Finder(key, position.right());//찾으려는 값이 더 크다면 오른쪽으로
	else return position;//찾으려는 값과 position에 있는 값이 같다면 반환
}

template<typename T>
typename BinarySearchTree<T>::TreePosition BinarySearchTree<T>::Inserter(const K& key, const V& value)
{
	TreePosition position = Finder(key, root());//가상 루트부터 탐색
	while (!position.IsExternal())//찾은 position이 External이 아니라면
		position = Finder(key, position.right());//External까지 가기 위해 더 찾는다.
	tree_.ExpandExternal(position);//새로운 내부 노드를 트리에 추가
	position.operator*()->get_data().set_key(key); position.operator*()->get_data().set_value(value);//엔트리 설정
	number_of_entries_++;//사이즈 증가
	return position;//삽입된 위치를 반환한다.
}

template<typename T>
typename BinarySearchTree<T>::TreePosition BinarySearchTree<T>::Eraser(TreePosition& position)
{
	TreePosition temp_position;
	if (position.left().IsExternal()) temp_position = position.left();
	else if (position.right().IsExternal()) temp_position = position.right();
	else {
		temp_position = position.right();
		do { temp_position = temp_position.left(); } while (!temp_position.IsExternal());
		TreePosition parent_of_temp_position = temp_position.parent();
		position->set_key(parent_of_temp_position->key()); position->set_value(parent_of_temp_position->value());
	}
	number_of_entries_--;
	return tree_.RemoveAboveExternal(temp_position);
}
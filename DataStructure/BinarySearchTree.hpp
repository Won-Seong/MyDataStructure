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
	TreePosition position = Finder(key, root());//���� ��Ʈ���� �˻�
	if (!position.IsExternal()) return Iterator(position);//ã�Ҵٸ�...
	else return End();//ã�� ���ߴٸ�...
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
	if (position.IsExternal()) return position;//�ܺ� ����� ã�� ���� ��
	if (key < position.operator*()->get_data().get_key()) return Finder(key, position.left());//ã������ ���� �� �۴ٸ� ��������
	else if (key > position.operator*()->get_data().get_key()) return Finder(key, position.right());//ã������ ���� �� ũ�ٸ� ����������
	else return position;//ã������ ���� position�� �ִ� ���� ���ٸ� ��ȯ
}

template<typename T>
typename BinarySearchTree<T>::TreePosition BinarySearchTree<T>::Inserter(const K& key, const V& value)
{
	TreePosition position = Finder(key, root());//���� ��Ʈ���� Ž��
	while (!position.IsExternal())//ã�� position�� External�� �ƴ϶��
		position = Finder(key, position.right());//External���� ���� ���� �� ã�´�.
	tree_.ExpandExternal(position);//���ο� ���� ��带 Ʈ���� �߰�
	position.operator*()->get_data().set_key(key); position.operator*()->get_data().set_value(value);//��Ʈ�� ����
	number_of_entries_++;//������ ����
	return position;//���Ե� ��ġ�� ��ȯ�Ѵ�.
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
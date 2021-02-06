#pragma once
#include <list>
#include <vector>
#include "BinaryTree.h"

template <typename K, typename V>
class Entry {
public:
	using Key = K;
	using Value = V;
public:
	Entry(const K& key = K(), const V& value = V()) :key_(key), value_(value) {};
	~Entry() {};
	const K& get_key() const { return key_; }
	const V& get_value() const { return value_; }
	void set_key(const K& key) { key_ = key; }
	void set_value(const V& value) { value_ = value; }
private:
	K key_;
	V value_;
};

template <typename K, typename V>
std::ostream& operator<<(std::ostream& os, const Entry<K,V>& entry) {
	os << "Key : " << entry.get_key() << "\t Value : " << entry.get_value() << std::endl;
	return os;
}

template <typename T>
class BinarySearchTree {
public:
	using K = typename T::Key;//Entry의...
	using V = typename T::Value;//Entry의...
	class Iterator;
public:
	BinarySearchTree() :tree_(), number_of_entries_(0) { tree_.AddRoot(T()); tree_.ExpandExternal(tree_.get_root()); }
	int get_size() const { return number_of_entries_; }
	bool IsEmpty() const { return tree_.IsEmpty(); }
	typename BinarySearchTree<T>::Iterator Begin() {
		TreePosition temp_position = root();
		while (!temp_position.IsExternal()) temp_position = temp_position.left();
		return Iterator(temp_position.parent());
	}
	typename BinarySearchTree<T>::Iterator End() { return Iterator(tree_.get_root()); }
	//InOrder이기 때문에 Begin은 가장 왼쪽에 있는 노드를, End는 슈퍼 루트를 반환한다.
	typename BinarySearchTree<T>::Iterator Find(const K& key);//키가 key인 엔트리를 찾는다.
	typename BinarySearchTree<T>::Iterator Insert(const K& key, const V& value);//키가 key인 엔트리를 찾아 value 삽입
	void Erase(const K& key);//key 엔트리를 찾아 삭제.
	void Erase(const Iterator& iterator);//iterator가 가리키는 엔트리 삭제
protected:
	using BinaryTree = BinaryTree<T>;
	using TreePosition = typename BinaryTree::Position;
	typename BinarySearchTree<T>::TreePosition root() const { return tree_.get_root().left(); };
	typename BinarySearchTree<T>::TreePosition Finder(const K& key, const TreePosition& position);
	typename BinarySearchTree<T>::TreePosition Inserter(const K& key, const V& value);
	typename BinarySearchTree<T>::TreePosition Eraser(TreePosition& position);
	typename BinarySearchTree<T>::TreePosition Restructure(const TreePosition& position);
private:
	BinaryTree tree_;
	int number_of_entries_;
public:
	class Iterator {
	private:
		TreePosition position_of_entry_;
	public:
		Iterator(const TreePosition& input_position) :position_of_entry_(input_position) {}
		Iterator(const Iterator& iterator) = delete;
		Iterator(Iterator&& iterator) { position_of_entry_ = iterator.position_of_entry_; iterator.position_of_entry_ = nullptr; }
		Iterator& operator=(const Iterator& iterator) = delete;
		Iterator& operator=(Iterator&& iterator) { position_of_entry_ = iterator.position_of_entry_; iterator.position_of_entry_ = nullptr; }
		const T& operator*() const { return position_of_entry_.operator*()->get_data(); }//읽기(R) 전용
		T& operator*() { return position_of_entry_.operator*()->get_data(); }//읽기와 쓰기(R/W)
		bool operator==(const Iterator& other_iterator) const { return position_of_entry_ == other_iterator.position_of_entry_; }
		typename BinarySearchTree<T>::Iterator& operator++();
		friend class BinarySearchTree;
	};
};
#include "BinarySearchTree.hpp"
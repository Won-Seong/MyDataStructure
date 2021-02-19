#pragma once
#include <iostream>
#include <vector>

template <typename V>
class TwoFourTree : std::exception {
private:
	class Entry;
	class Node;
public:
	using EntryPtr = std::unique_ptr<Entry>;
	using NodePtr = std::unique_ptr<Node>;
	using EntryList = std::vector<Entry>;
	using NodePtrList = std::vector<NodePtr>;
private:
	class Entry {
	public:
		Entry(const int& key, const V& value) : key_(key), value_(value) {}
		~Entry() {};
		void set_key(const int& key) { key_ = key; }
		void set_value(const V& value) { value_ = value; }
		int get_key() const { return key_; }
		V get_value() const { return value_; }
	private:
		int key_;
		V value_;
	};
private:
	class Node {
	public:
		Node() :entries_number(0) {
			entries_.reserve(5);
			children_.reserve(5);
		};
		~Node() {} //추후
		void PushEntry(const Entry& entry) { entries_.push_back(entry); }
		void PushChild(const Node& node) { children_.push_back(std::make_unique<Node>(node)); }
	private:
		EntryList entries_;
		NodePtrList children_;
		int entries_size_;
	};
public:
	TwoFourTree() : nodes_size(1) , all_entries_size(0) {
		root_ = std::make_unique<Node>;
	}
	~TwoFourTree() {} //추후
	void Insert(const Entry& entry);
	EntryPtr Search(const int& key) const;
	void Print() const;
private:
	NodePtr root_;
	int	all_entries_size;
	int nodes_size;
};
#include "(2,4)Tree.hpp"


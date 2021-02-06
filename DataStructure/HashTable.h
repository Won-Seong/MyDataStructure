#pragma once
#include <iostream>
#include <vector>

struct HashFunction {
	unsigned int operator()(unsigned int key, const size_t& size_of_Hash) { return key % size_of_Hash; }
};//Key�� Int�� �� Key ���� Hash�� ũ��� ���� ���� ��ȯ

struct DoubleHashFuction {
	const static int number = 17;
	unsigned int operator()(unsigned int key) { return (number - (key % number)); }
};

template <typename V, typename H, typename HH>
class HashTable {
private:
	struct Entry {
	public:
		enum class STATUS {
			AVAILABLE,
			OCCUPIED,
			UNOCCUPIED
		};
	public:
		Entry(const unsigned int& key = int(), const V& value = V(), STATUS status = STATUS::AVAILABLE) :key_(key), value_(value),status_(status) {};
		Entry& operator=(const Entry& entry) {
			key_ = entry.key_; value_ = entry.value_; status_ = entry.status_;
			return *this;
		}
		const unsigned int& get_key() const { return key_; }
		const V& get_value() const { return value_; }
		const STATUS& get_status() const { return status_; }
		void set_key(const unsigned int& key) { key_ = key; }
		void set_value(const int& value) { value_ = value; }
		void set_status(STATUS status) { status_ = status; }
		bool operator==(const Entry& entry) {
			return key_ == entry.get_key() && value_ == entry.get_value();
		}
	private:
		STATUS status_;
		unsigned int key_;
		V value_;
	};
public:
	using BucketArray = std::vector<Entry>;
public:
	HashTable(const unsigned int& capacity = 100) :number_of_entries_(0), bucket_array(capacity) {};//capacity��ŭ�� ũ�⸦ ���� �迭�� �����ȴ�.
	~HashTable() { bucket_array.clear(); }
	size_t get_size() const { return number_of_entries_; }
	bool IsEmpty() const { return number_of_entries_ == 0; }
	void Insert(const unsigned int& key, const V& value);//Entry�� ���� ������� �̿��Ͽ� �����Ѵ�.
	void DoubleHashingInsert(const unsigned int& key, const V& value);//DoubleHashing ����� �̿��Ͽ� �����Ѵ�.
	const V& Find(const unsigned int& key);//DoubleHashing ����� �̿��Ͽ� ã�´�. 
	void Print() const;//�迭�� ��� ���� ���
private:
	size_t number_of_entries_;//��Ʈ���� ��
	H hash_comparator_;//�ؽ� ����
	HH hash_comparator_double_;
	BucketArray bucket_array;//�迭
};
#include "HashTable.hpp"
#pragma once
#include <iostream>
#include <vector>

struct HashFunction {
	unsigned int operator()(unsigned int key, const size_t& size_of_Hash) { return key % size_of_Hash; }
};//Key가 Int일 때 Key 값을 Hash의 크기로 나눈 값을 반환

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
	HashTable(const unsigned int& capacity = 100) :number_of_entries_(0), bucket_array(capacity) {};//capacity만큼의 크기를 갖는 배열이 생성된다.
	~HashTable() { bucket_array.clear(); }
	size_t get_size() const { return number_of_entries_; }
	bool IsEmpty() const { return number_of_entries_ == 0; }
	void Insert(const unsigned int& key, const V& value);//Entry를 선형 조사법을 이용하여 삽입한다.
	void DoubleHashingInsert(const unsigned int& key, const V& value);//DoubleHashing 방법을 이용하여 삽입한다.
	const V& Find(const unsigned int& key);//DoubleHashing 방법을 이용하여 찾는다. 
	void Print() const;//배열의 모든 값을 출력
private:
	size_t number_of_entries_;//엔트리의 수
	H hash_comparator_;//해시 비교자
	HH hash_comparator_double_;
	BucketArray bucket_array;//배열
};
#include "HashTable.hpp"
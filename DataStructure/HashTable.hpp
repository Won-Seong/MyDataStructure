#pragma once
#include "HashTable.h"

template<typename V, typename H, typename HH>
void HashTable<V, H, HH>::Insert(const unsigned int& key, const V& value)
{
	unsigned int index = hash_comparator_(key, bucket_array.capacity());
	while (bucket_array[index].get_status() == Entry::STATUS::OCCUPIED ) { index++; }
	bucket_array[index] = Entry{ index, value , Entry::STATUS::OCCUPIED };
	number_of_entries_++;
}//충돌 관리 기법으로 선형 조사를 사용

template<typename V, typename H, typename HH>
void HashTable<V, H, HH>::DoubleHashingInsert(const unsigned int& key, const V& value)
{
	int index = hash_comparator_(key, bucket_array.capacity());
	for (int i = 1; ; i++) {
		if (bucket_array[index].get_status() != Entry::STATUS::OCCUPIED)
			break;
		index = (hash_comparator_(key, bucket_array.capacity()) + i * hash_comparator_double_(key)) % bucket_array.capacity();
	}
	bucket_array[index] = Entry(index, value, Entry::STATUS::OCCUPIED);
	number_of_entries_++;
}

template<typename V, typename H, typename HH>
const V& HashTable<V, H, HH>::Find(const unsigned int& key)
{
	unsigned int index = hash_comparator_(key, bucket_array.capacity());
	for (int i = 1; ; i++) {
		if (bucket_array[index].get_key() == key)
			return i;
		else if (bucket_array[index].get_status() == Entry::STATUS::AVAILABLE)
			return -i;
		index = (hash_comparator_(key, bucket_array.capacity()) + i * hash_comparator_double_(key)) % bucket_array.capacity();
	}
}

template<typename V, typename H, typename HH>
inline void HashTable<V, H, HH>::Print() const
{
	for (const auto& itr : bucket_array) {
		if(itr.get_status() == Entry::STATUS::OCCUPIED)
			std::cout << "Key : "<< itr.get_key() << " \t|" << " Value : " << itr.get_value() << std::endl;
	}
	std::cout << std::endl;
}
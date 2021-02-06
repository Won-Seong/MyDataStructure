#pragma once
#include <iostream>
#include "Heap.h"

template<typename T>
void Heap<T>::Insert(const T& value)
{
	array_of_heap_.push_back(value);
	Position current_position = last();
	while (!IsRoot(current_position)) {
		if (*current_position < *parent(current_position)) {
			Swap(current_position, parent(current_position));
			current_position = parent(current_position);
		}
		else break;
	}//UpHeapBubbling
}

template<typename T>
T Heap<T>::RemoveMin()
{
	if (size() > 1) {
		Swap(root(), last());
		T temp = *last();//root에 있던 원소를 복사
		array_of_heap_.pop_back();//root에 있던 원소를 제거
		Position current_position = root();
		while (HasLeft(current_position)) {
			Position small_child = left(current_position);
			if (HasRight(current_position) && *left(current_position) > *right(current_position))
				small_child = right(current_position);
			if (*current_position > *small_child) {
				Swap(current_position, small_child);
				current_position = small_child;
			}
			else break;
		}
		return temp;
	}
	else if (size() == 1) {
		T temp = *last();
		array_of_heap_.pop_back();
		return temp;
	}//root만 있을 경우
	else {
		throw std::length_error("There is no element!");
	}//원소가 없을 경우
}

template<typename T>
void Heap<T>::Swap(const Position& position_left, const Position& position_right)
{
	T temp = *position_left;
	*position_left = *position_right;
	*position_right = std::forward<T>(temp);
}

struct Int {
	int value;
	Int() = default;
	Int(int value) :value(value) {};
	bool operator>(Int to_compare) { return value > to_compare.value ? false : true; }
	bool operator<(Int to_compare) { return value < to_compare.value ? false : true; }
	friend std::ostream& operator<<(std::ostream& os, Int data);
};//최소 힙을 최대 힙으로 바꾸기 위한 Wrapper , 최대 힙을 이용하고 싶다면 int 대신 이 구조체를 넣자

std::ostream& operator<<(std::ostream& os, Int data) {
	os << data.value;
	return os;
}
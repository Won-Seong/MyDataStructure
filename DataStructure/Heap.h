#pragma once
#include <vector>

template <typename T>
class Heap :public std::exception {
private:
	std::vector<T> array_of_heap_;//공간은 벡터로 할당
public:
	using Position = typename std::vector<T>::iterator ;//Position은 Vector 공간의 반복자
protected://내부 편의 함수
	Position position(int index) { return array_of_heap_.begin() + index; }//인덱스를 받아 벡터의 반복자를 반환
	int index(const Position& position) const { return static_cast<int>(position - array_of_heap_.begin()); }//벡터의 반복자를 받아 인덱스 반환
protected://Position 관련 내부 편의 함수
	Position left(const Position& input_position) { return position(2 * index(input_position)); }//모든 Node의 left_child는 2 * index
	Position right(const Position& input_position) { return position(2 * index(input_position) + 1); }//모든 Node의 right_child는 2 * index + 1
	Position parent(const Position& input_position) { return position(index(input_position) / 2); }//모든 Node의 Parent는 index / 2
	bool HasLeft(const Position& input_position) const { return static_cast<size_t>(2 * index(input_position)) <= size(); }//만약 해당 Node가 left_child를 갖고 있다면 Size는 그보다 클 것이다
	bool HasRight(const Position& input_position) const { return static_cast<size_t>(2 * index(input_position) + 1) <= size(); }//만약 해당 Node가 right_child를 갖고 있다면 Size는 그보다 클 것이다
	bool IsRoot(const Position& input_position) const { return index(input_position) == 1; }
	Position root() { return position(1); }
	Position last() { return position(size()); }
public:
	Heap() : array_of_heap_{ T() } {};//Heap의 인덱스는 1부터 시작
	Heap(const Heap& heap) { array_of_heap_ = heap.array_of_heap_; }
	Heap(Heap&& heap) noexcept { array_of_heap_.swap(heap.array_of_heap_); heap.array_of_heap_.push_back(T()); }
	Heap& operator=(const Heap& heap) { array_of_heap_ = heap.array_of_heap_; }
	Heap& operator=(Heap&& heap) noexcept {
		array_of_heap_.clear(); array_of_heap_.push_back(T());
		array_of_heap_.swap(heap.array_of_heap_);
	}
	~Heap() { Clear(); }
public:
	int size() const { return static_cast<int>(array_of_heap_.size() - 1); }//Heap의 인덱스는 1부터 시작이기 때문에 1을 감한다
	T min() const { if (root()) return *root(); else throw std::length_error("No element!"); }
	bool Empty() const { return size() == 0; }
	void Swap(const Position& position_left, const Position& position_right);//두 Position의 원소를 Swap
	void Insert(const T& value);//원소를 삽입한 후 up-heap bubbling
	void Clear() { array_of_heap_.clear(); array_of_heap_.push_back(T()); };
	T RemoveMin();//가장 작은 원소를 삭제하고 down-heap bubbling 한 후 반환, 제일 작은 원소가 무조건 root로 간다
};
#include "Heap.hpp"
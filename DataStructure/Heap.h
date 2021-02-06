#pragma once
#include <vector>

template <typename T>
class Heap :public std::exception {
private:
	std::vector<T> array_of_heap_;//������ ���ͷ� �Ҵ�
public:
	using Position = typename std::vector<T>::iterator ;//Position�� Vector ������ �ݺ���
protected://���� ���� �Լ�
	Position position(int index) { return array_of_heap_.begin() + index; }//�ε����� �޾� ������ �ݺ��ڸ� ��ȯ
	int index(const Position& position) const { return static_cast<int>(position - array_of_heap_.begin()); }//������ �ݺ��ڸ� �޾� �ε��� ��ȯ
protected://Position ���� ���� ���� �Լ�
	Position left(const Position& input_position) { return position(2 * index(input_position)); }//��� Node�� left_child�� 2 * index
	Position right(const Position& input_position) { return position(2 * index(input_position) + 1); }//��� Node�� right_child�� 2 * index + 1
	Position parent(const Position& input_position) { return position(index(input_position) / 2); }//��� Node�� Parent�� index / 2
	bool HasLeft(const Position& input_position) const { return static_cast<size_t>(2 * index(input_position)) <= size(); }//���� �ش� Node�� left_child�� ���� �ִٸ� Size�� �׺��� Ŭ ���̴�
	bool HasRight(const Position& input_position) const { return static_cast<size_t>(2 * index(input_position) + 1) <= size(); }//���� �ش� Node�� right_child�� ���� �ִٸ� Size�� �׺��� Ŭ ���̴�
	bool IsRoot(const Position& input_position) const { return index(input_position) == 1; }
	Position root() { return position(1); }
	Position last() { return position(size()); }
public:
	Heap() : array_of_heap_{ T() } {};//Heap�� �ε����� 1���� ����
	Heap(const Heap& heap) { array_of_heap_ = heap.array_of_heap_; }
	Heap(Heap&& heap) noexcept { array_of_heap_.swap(heap.array_of_heap_); heap.array_of_heap_.push_back(T()); }
	Heap& operator=(const Heap& heap) { array_of_heap_ = heap.array_of_heap_; }
	Heap& operator=(Heap&& heap) noexcept {
		array_of_heap_.clear(); array_of_heap_.push_back(T());
		array_of_heap_.swap(heap.array_of_heap_);
	}
	~Heap() { Clear(); }
public:
	int size() const { return static_cast<int>(array_of_heap_.size() - 1); }//Heap�� �ε����� 1���� �����̱� ������ 1�� ���Ѵ�
	T min() const { if (root()) return *root(); else throw std::length_error("No element!"); }
	bool Empty() const { return size() == 0; }
	void Swap(const Position& position_left, const Position& position_right);//�� Position�� ���Ҹ� Swap
	void Insert(const T& value);//���Ҹ� ������ �� up-heap bubbling
	void Clear() { array_of_heap_.clear(); array_of_heap_.push_back(T()); };
	T RemoveMin();//���� ���� ���Ҹ� �����ϰ� down-heap bubbling �� �� ��ȯ, ���� ���� ���Ұ� ������ root�� ����
};
#include "Heap.hpp"
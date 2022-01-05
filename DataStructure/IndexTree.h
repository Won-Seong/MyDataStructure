#pragma once
#include <iostream>
#include <vector>
#include <cmath>

class IndexTree : public std::exception {
	using Vector = std::vector<long long>;
private:
	size_t root() const { return 1; }
	size_t left_child(size_t index) const { return index * 2; }
	bool left_child_existed(size_t index) const { return index * 2 <= Capacity() - 1; }
	size_t right_child(size_t index) const { return index * 2 + 1; }
	bool right_child_existed(size_t index) const { return index * 2 + 1 <= Capacity() - 1; }
	size_t parent(size_t index) const { return index / 2; }
	bool IsLeaf(size_t index) const { return index * 2 >= Capacity() - 1; }
public:
	void CreateTree(const Vector& vector);
	long long RangeSum(size_t left, size_t right) const;
	void UpdateValue(size_t index, const  long long& value);
public:
	IndexTree(size_t size) :size_(size) { 

		for (size_t i = 0; ; i++)
			if (std::pow(2, i) > size_) {
				vector_.resize( static_cast<size_t>(2 * std::pow(2,i) + 1 ) ); break;
			}

	}
	~IndexTree() { vector_.clear(); }
	void PrintInOrder() const;
	void Print() const {
		for (const auto& itr : vector_)
			std::cout << itr << ' ';
		std::cout << std::endl;
	};
	size_t Size() const { return size_; }
	size_t Capacity() const { return vector_.size(); }
private:
	size_t LeafStart() const { return (Capacity() - 1) / 2; }//Leaf의 시작점
	long long CreateSumUtility(size_t index);
	void InOrder(size_t index, Vector& vector) const;
private:
	Vector vector_;//IndexTree를 구성하는 배열
	size_t size_;
};
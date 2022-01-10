#pragma once
#include <iostream>
#include <vector>
#include <set>

class UnionFind : public std::exception {
	using Set = std::set<int>;
	using Vector = std::vector<int>;
	using IndexVector = std::vector< size_t >;
	using PairVector = std::vector< std::pair<size_t, int> >;
	using Matrix = std::vector< Vector >;
	using PairMatrix = std::vector< PairVector >;
public:
	void Insert(const size_t& left, const size_t& right, const int& value);
	void SetUnionVector();
	size_t Union(size_t left, size_t right);
	size_t Find(size_t index);//경로 압축 때문에 const가 아님
public:
	UnionFind(const size_t& size) :size_(size) { 
		vector_.resize(size_); 
	}
private:
	size_t size_;
	IndexVector union_vector_;
	PairMatrix vector_;
};
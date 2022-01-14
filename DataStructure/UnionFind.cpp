#include "UnionFind.h"

void UnionFind::Insert(const size_t& left, const size_t& right, const int& value)
{
	vector_[left].push_back(std::make_pair(right, value)), vector_[right].push_back(std::make_pair(left, value));
}

void UnionFind::SetUnionVector()
{
	union_vector_.clear();
	union_vector_.resize(size_);
	for (size_t i = 0; i < size_; i++)
	{
		union_vector_[i] = i;
	}
}

size_t UnionFind::Union(size_t left, size_t right)
{	
	union_vector_[Find(left)] = Find(right);
	return left;
}

size_t UnionFind::Find(size_t index) 
{
	IndexVector temp_vector;
	while (index != union_vector_[index])
		temp_vector.push_back(index) , index = union_vector_[index];
	for (const auto& itr : temp_vector)
		union_vector_[itr] = index;	
	return index;
}

size_t UnionFind::FindRecursive(size_t index)
{
	if (index == union_vector_[index]) return index;
	else return union_vector_[index] = FindRecursive(union_vector_[index]);
}

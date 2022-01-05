#include "IndexTree.h"

void IndexTree::CreateTree(const Vector& vector)
{
	size_t index{ root() };

	//먼저 Leaf node를 구성하는 과정

	while (index < Capacity())
		index *= 2;
	index /= 4;
	for (const auto& itr : vector)
	{
		vector_[index++] = itr;
	}
	index = root();
	vector_[root()] = CreateSumUtility(index);
}

long long IndexTree::RangeSum(size_t left, size_t right) const
{
	long long sum{ 0 };
	left = LeafStart() + left - 1;
	right = LeafStart() + right - 1;

	while (left <= right) {
		if ((left % 2) == 1) sum += vector_[left];
		if ((right % 2) == 0) sum += vector_[right];

		left = (left + 1) / 2;
		right = (right - 1) / 2;
		
	}
	return sum;
}

void IndexTree::UpdateValue(size_t index, const long long& value)
{
	index = LeafStart() + index - 1;
	long long last{ vector_[index] };
	vector_[index] = value;

	while (index > root()) {
		index = parent(index);
		vector_[index] = vector_[index] - last + value;
	}
}

void IndexTree::PrintInOrder() const
{
	Vector vector;
	vector.reserve(size_);
	InOrder(root(), vector);
	for (const auto& itr : vector)
		std::cout << itr << '\n';
}

long long IndexTree::CreateSumUtility(size_t index)
{
	if (IsLeaf(index)) return vector_[index];
	return vector_[index] = CreateSumUtility(index * 2) + CreateSumUtility(index * 2 + 1);
}

void IndexTree::InOrder(size_t index, Vector& vector) const
{
	if(left_child_existed(index))
		InOrder(left_child(index), vector);
	vector.push_back(vector_[index]);
	if(right_child_existed(index))
		InOrder(right_child(index), vector);
}

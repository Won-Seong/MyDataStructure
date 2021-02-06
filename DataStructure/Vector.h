#pragma once

template <typename T>
class Vector : public std::exception {
private:
	using UniquePtr = std::unique_ptr<T[]>;
	UniquePtr element_ptr_;//�迭�� ����ų ������
	size_t capacity_;//�迭�� �뷮
	size_t size_;//������ ����
public:
	Vector(const size_t& capacity = 1) :capacity_(capacity), size_(0)
	{
		element_ptr_ = std::make_unique<T[]>(capacity_);
	}//������ , �뷮 ���� ����
	Vector(const Vector& vec) {
		size_ = vec.size_;
		capacity_ = vec.capacity_;
		element_ptr_ = std::make_unique<T[]>(capacity_);
		for (int i = 0; i < size_; i++)
			element_ptr_[i] = vec.element_ptr_[i];
	}//���� ������
	Vector(Vector&& vec) noexcept {
		size_ = vec.size_;
		capacity_ = vec.capacity_;
		element_ptr_ = vec.element_ptr_;
		vec.capacity_ = 1;
		vec.size_ = 0;
		vec.element_ptr_ = std::make_unique<T[]>(vec.capacity_);
	}//�̵� ������ , ���� X
	~Vector() {
		Clear();
	}//�Ҹ��� , �̵� �����ڸ� ���� nullptr���� �ƴ��� �˻��Ѵ�.
	Vector& operator=(const Vector& vec) {
		capacity_ = vec.capacity_;
		size_ = vec.size_;
		element_ptr_ = std::make_unique<T[]>(capacity_);
		for (int i = 0; i < size_; i++)
			element_ptr_[i] = vec.element_ptr_[i];
		return *this;
	}//���� ������ , ���� ����
	Vector& operator=(Vector&& vec) {
		capacity_ = vec.capacity_;
		size_ = vec.size_;
		element_ptr_ = vec.element_ptr_;
		vec.size_ = 0;
		vec.capacity_ = 1;
		vec.element_ptr_ = std::make_unique<T[]>(vec.capacity);
		return *this;
	}//�̵� ���� ������
private:
	void CapacityCheck();//�迭�� �뷮�� �˻��ϴ� ���� �Լ�. �迭�� ũ�Ⱑ �뷮�� �Ѿ���� �� ���� �뷮�� �� ��� �ٽ� �Ҵ��Ѵ�.
public:
	void Reserve(const size_t& input_size);//�뷮�� �����Ѵ�.
	void PushBack(const T& data);//�� �ڿ� ���Ҹ� �����Ѵ�. O(1)
	T PopBack();//�� ���� ���Ҹ� �����Ѵ�. O(1)
	void Set(const unsigned int& index, const T& data);//�߰��� �ִ� ���Ҹ� �ٸ� ���ҷ� �ٲ۴�.
	void Insert(const unsigned int& index, const T& data);//�߰��� ���Ҹ� �����Ѵ�. O(n)
	T Erase(const unsigned int& index);//�߰��� �ִ� ���Ҹ� �����Ѵ�. O(n) 
	const T& At(const unsigned int& index) const;//�����͸� �����ؼ� ��ȯ�Ѵ�. O(1) , ���Ҹ� �����ؼ� ��ȯ�ϱ� ������ ���� ���� �Ұ���.
	bool Empty() const { return size_ == 0 ? true : false; };
	size_t get_size() const { return size_; };
	size_t get_capacity() const { return capacity_; };
	void Print() const;//��ü ���� ���
	void Clear();
public:
	bool operator==(const Vector& other_vector) const;//�� ���Ͱ� ���� ������?
	T& operator[](const unsigned int& index) const;//�����͸� �����Ͽ� ��ȯ�Ѵ�. O(1) , �����ڸ� ��ȯ�ϱ� ������ ���� ���� ����.
};
#include "Vector.hpp"

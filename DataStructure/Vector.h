#pragma once

template <typename T>
class Vector : public std::exception {
private:
	using UniquePtr = std::unique_ptr<T[]>;
	UniquePtr element_ptr_;//배열을 가리킬 포인터
	size_t capacity_;//배열의 용량
	size_t size_;//원소의 개수
public:
	Vector(const size_t& capacity = 1) :capacity_(capacity), size_(0)
	{
		element_ptr_ = std::make_unique<T[]>(capacity_);
	}//생성자 , 용량 지정 가능
	Vector(const Vector& vec) {
		size_ = vec.size_;
		capacity_ = vec.capacity_;
		element_ptr_ = std::make_unique<T[]>(capacity_);
		for (int i = 0; i < size_; i++)
			element_ptr_[i] = vec.element_ptr_[i];
	}//복사 생성자
	Vector(Vector&& vec) noexcept {
		size_ = vec.size_;
		capacity_ = vec.capacity_;
		element_ptr_ = vec.element_ptr_;
		vec.capacity_ = 1;
		vec.size_ = 0;
		vec.element_ptr_ = std::make_unique<T[]>(vec.capacity_);
	}//이동 생성자 , 예외 X
	~Vector() {
		Clear();
	}//소멸자 , 이동 생성자를 위해 nullptr인지 아닌지 검사한다.
	Vector& operator=(const Vector& vec) {
		capacity_ = vec.capacity_;
		size_ = vec.size_;
		element_ptr_ = std::make_unique<T[]>(capacity_);
		for (int i = 0; i < size_; i++)
			element_ptr_[i] = vec.element_ptr_[i];
		return *this;
	}//대입 연산자 , 깊은 복사
	Vector& operator=(Vector&& vec) {
		capacity_ = vec.capacity_;
		size_ = vec.size_;
		element_ptr_ = vec.element_ptr_;
		vec.size_ = 0;
		vec.capacity_ = 1;
		vec.element_ptr_ = std::make_unique<T[]>(vec.capacity);
		return *this;
	}//이동 대입 연산자
private:
	void CapacityCheck();//배열의 용량을 검사하는 내부 함수. 배열의 크기가 용량을 넘어서려고 할 때에 용량을 두 배로 다시 할당한다.
public:
	void Reserve(const size_t& input_size);//용량을 예약한다.
	void PushBack(const T& data);//맨 뒤에 원소를 삽입한다. O(1)
	T PopBack();//맨 뒤의 원소를 제거한다. O(1)
	void Set(const unsigned int& index, const T& data);//중간에 있는 원소를 다른 원소로 바꾼다.
	void Insert(const unsigned int& index, const T& data);//중간에 원소를 삽입한다. O(n)
	T Erase(const unsigned int& index);//중간에 있는 원소를 제거한다. O(n) 
	const T& At(const unsigned int& index) const;//데이터를 복사해서 반환한다. O(1) , 원소를 복사해서 반환하기 때문에 원본 수정 불가능.
	bool Empty() const { return size_ == 0 ? true : false; };
	size_t get_size() const { return size_; };
	size_t get_capacity() const { return capacity_; };
	void Print() const;//전체 원소 출력
	void Clear();
public:
	bool operator==(const Vector& other_vector) const;//두 벡터가 서로 같은가?
	T& operator[](const unsigned int& index) const;//데이터를 참조하여 반환한다. O(1) , 참조자를 반환하기 때문에 원소 수정 가능.
};
#include "Vector.hpp"

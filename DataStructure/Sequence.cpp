//Sequence
#include <iostream>

template <typename T>
class Sequence : public std::exception {
private:
	struct Node {
		T data;
		Node* prev;
		Node* next;
		Node(const T& data = NULL) :data(data), prev(nullptr), next(nullptr) {};
		Node(const T& data, Node& prev, Node& next) :data(data), prev(&prev), next(&next) {};
		~Node() {};
	};
	typedef Node* node_ptr;
public:
	struct Iterator {
		node_ptr node;
		int index;
	public:
		Iterator(const node_ptr& input_node, const int& index) : index(index), node(input_node) {};
		Iterator& operator=(const Iterator& iter) { node = iter.node; index = iter.index; }
		Iterator& operator++() {
			if (node->next == nullptr) throw std::length_error("No next element!");
			node = node->next;
			index++;
			return *this;
		}
		Iterator operator++(int) {
			if (node->next == nullptr) throw std::length_error("No next element!");
			Iterator iter(node, index);
			node = node->next;
			index++;
			return iter;
		}
		Iterator operator--() {
			if (node->prev == nullptr) throw std::length_error("No prev element!");
			node = node->prev;
			index--;
			return *this;
		}
		Iterator operator--(int) {
			if (node->prev == nullptr) throw std::length_error("No next element!");
			Iterator iter(node, index);
			node = node->prev;
			index--;
			return iter;
		}
		bool operator==(const Iterator& iter) const { return node == iter.node && index == iter.index; }
		bool operator!=(const Iterator& iter) const { return !(node == iter.node && index && iter.index); }
		node_ptr operator*() const { return node; }
		~Iterator() {};
	};//이는 각 노드를 순회할 반복자이다.
private:
	node_ptr* array_of_nodes_;//노드를 가리키는 포인터를 담는 배열이다.
	node_ptr header_;//배열의 가장 앞
	node_ptr trailer_;//배열의 가장 뒤
	int size_;//원소의 수
	int capacity_;//배열의 용량
private:
	void SizeCheck();//배열의 크기를 검사하는 내부 함수
public:
	Sequence(const int& capacity = 1) :capacity_(capacity), size_(0) {
		array_of_nodes_ = new Node * [capacity];
		header_ = new Node;
		trailer_ = new Node;
		header_->next = trailer_;
		trailer_->prev = header_;
		//처음 생성했을 때 헤더는 트레일러를, 트레일러는 헤더를 가리키고 있다.
	};
	Sequence(const Sequence& sequence) = delete;
	Sequence(Sequence&& sequence) = delete;
	Sequence& operator=(const Sequence& sequence) = delete;
	Sequence& operator=(Sequence&& sequence) = delete;
	~Sequence() { Clear(); }
public:
	void InsertFront(const T& data);//배열의 가장 맨 앞에 삽입
	void InsertBack(const T& data);//배열의 가장 맨 뒤에 삽입
	int get_size() const { return size_; };
	int get_capacity() const { return capacity_; };
	T At(int index) const { return array_of_nodes_[index]->data; }
	void Clear();
	void PrintByVector() const;//배열을 이용하여 프린트
	void PrintByNode() const;//링크를 이용하여 프린트
	void InsertByVector(const T& data, int index);//배열을 이용하여 삽입
	void InsertByNode(const T& data, int index);//링크를 이용하여 삽입
	T Erase(int index);//배열을 이용하여 제거
public://반복자 관련 함수
	Iterator Begin() const {
		return Iterator(header_->next, 0);
	}
	Iterator End() const {
		return Iterator(trailer_->prev, (size_ - 1));
	}
	Iterator AtIndex(int index) const {
		Iterator iter = Begin();
		for (int j = 0; j < index; j++) ++iter;
		return iter;
	};
	int IndexOf(const Iterator& iter) const;
};

template<typename T>
void Sequence<T>::SizeCheck()
{
	if (size_ >= capacity_) {
		capacity_ *= 2;
		node_ptr* temp = new Node * [capacity_];
		for (int i = 0; i < size_; i++)
			temp[i] = array_of_nodes_[i];
		delete[] array_of_nodes_;
		array_of_nodes_ = temp;
	}
}

template<typename T>
void Sequence<T>::InsertFront(const T& data)
{
	SizeCheck();
	node_ptr temp = new Node(data);
	temp->next = header_->next;
	temp->prev = header_;
	header_->next->prev = temp;
	header_->next = temp;
	for (int i = size_; i > 0; i--) {
		array_of_nodes_[i] = array_of_nodes_[i - 1];
	}
	array_of_nodes_[0] = temp;
	size_++;
}

template<typename T>
void Sequence<T>::InsertBack(const T& data)
{
	SizeCheck();
	node_ptr temp = new Node(data);
	temp->prev = trailer_->prev;
	temp->next = trailer_;
	trailer_->prev->next = temp;
	trailer_->prev = temp;
	array_of_nodes_[size_++] = temp;
}

template<typename T>
void Sequence<T>::PrintByVector() const
{
	if (size_ == 0) {
		std::cout << "No element!" << std::endl;
		return;
	}
	std::cout << "| ";
	for (int i = 0; i < size_; i++)
		std::cout << array_of_nodes_[i]->data << " | ";
	std::cout << std::endl;
}

template<typename T>
void Sequence<T>::PrintByNode() const
{
	Iterator iter = Begin();
	std::cout << "| ";
	while (iter != End()) {
		std::cout << (*iter)->data << " | ";
		++iter;
	}
	std::cout << std::endl;
}

template<typename T>
void Sequence<T>::Clear()
{
	if (size_ == 0) return;
	for (int i = 0; i < size_; i++) {
		delete array_of_nodes_[i];
	}
	delete[] array_of_nodes_;
	size_ = 0;
}

template<typename T>
void Sequence<T>::InsertByVector(const T& data, int index)
{
	if (index < 0 || index >= size_) throw std::length_error("Index Error");
	SizeCheck();
	for (int i = size_; i > index; i--) {
		array_of_nodes_[i] = array_of_nodes_[i - 1];
	}
	array_of_nodes_[index] = new Node(data, *array_of_nodes_[index - 1], *array_of_nodes_[index + 1]);
	array_of_nodes_[index - 1]->next = array_of_nodes_[index];
	array_of_nodes_[index + 1]->prev = array_of_nodes_[index];
	size_++;
}

template<typename T>
void Sequence<T>::InsertByNode(const T& data, int index)
{
	Iterator iter = AtIndex(index);
	Iterator iter_prev((*iter)->prev, iter.index - 1);
	node_ptr temp = new Node(data);
	(*iter_prev)->next = temp;
	temp->prev = (*iter_prev);
	(*iter)->prev = temp;
	temp->next = (*iter);
}

template<typename T>
T Sequence<T>::Erase(int index)
{
	if (index < 0 || index >= size_) throw std::length_error("Index Error");
	T temp = array_of_nodes_[index]->data;
	for (int i = index; i < size_ - 1; i++) {
		array_of_nodes_[i] = array_of_nodes_[i + 1];
	}
	size_--;
	return temp;
}

template<typename T>
int Sequence<T>::IndexOf(const Iterator& iter) const
{
	{
		Iterator temp_iter = Begin();
		int j = 0;
		while (temp_iter != iter) {
			++temp_iter; ++j;
		}
		return j;
	}
}

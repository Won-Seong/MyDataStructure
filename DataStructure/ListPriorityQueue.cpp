//ListPriorityQueue
#include <iostream>
#include <list>

template <typename T,typename COMPARATOR>
class ListPriorityQueue : public std::exception {
private:
	std::list<T> list_;
	COMPARATOR isLess;
protected:
	typedef	std::list<T> ElementList;
public:
	struct Position {
	private:
		typename ElementList::iterator PositionOfIterator;
	public:
		const T& operator*() { return *Position; }
		friend class ListPriorityQueue;
	};
public:
	ListPriorityQueue() = default;
	ListPriorityQueue(const ListPriorityQueue& input_queue) { list_ = input_queue.list_; }
	ListPriorityQueue(ListPriorityQueue&& move_queue) { list_ = std::move(move_queue.list_); move_queue.list_.clear(); }
	ListPriorityQueue& operator=(const ListPriorityQueue& input_queue) { list_ = input_queue.list_; return *this; }
	ListPriorityQueue& operator=(const ListPriorityQueue&& move_queue) { list_ = std::move(move_queue.list_); move_queue.list_.clear(); }
	~ListPriorityQueue() { list_.clear(); }
public:
	size_t Size() const { return list_.size(); }
	bool IsEmpty() const { return list_.empty(); }
	Position Insert(const T& data);//������ �� �켱������ ����Ͽ� �����Ѵ�
	const T& Min() const { return list_.front(); }
	T RemoveMin();
	T Remove(const Position& input_position);//���ϴ� ��ġ�� �ִ� ���Ҹ� ����
};

template<typename T, typename COMPARATOR>
typename ListPriorityQueue<T,COMPARATOR>::Position ListPriorityQueue<T, COMPARATOR>::Insert(const T& data)
{
	typename ElementList::iterator iter = list_.begin();
	while (iter != list_.end() && !isLess(data, *iter)) ++iter;//���� �ƴϸ鼭 �켱������ �� ���� ���Ұ� ���� ��� �� ĭ�� �δ�
	list_.insert(iter, data);
	Position pos; 
	pos.PositionOfIterator = --iter;
	return pos;
}

template<typename T, typename COMPARATOR>
T ListPriorityQueue<T, COMPARATOR>::RemoveMin()
{
	T temp = list_.front();
	list_.pop_front();//�켱 ������ �̹� ���ĵǾ� ������ �տ� �ִ� ���Ҹ� �����Ѵ�
	return temp;
}

template<typename T, typename COMPARATOR>
T ListPriorityQueue<T, COMPARATOR>::Remove(const Position& input_position)
{
	list_.erase(input_position.Position);
}

struct Compare {
	bool operator()(int a, int b) { return a < b; }
};


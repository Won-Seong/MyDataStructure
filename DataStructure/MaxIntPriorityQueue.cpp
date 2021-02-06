//MaxIntPriorityQueue
#include <iostream>
#include <vector>

class MaxIntPriorityQueue : public std::exception {
protected:
	std::vector<int> space_;
public:
	typedef std::vector<int>::iterator Position;
protected:
	Position GetPosition(int index) { return space_.begin() + index; }
	Position GetPosition(size_t size_index) { return space_.begin() + size_index; }
	int Index(const Position& position) const { return static_cast<int>(position - space_.begin()); }
public:
	Position left(const Position& input_position) { return GetPosition(2 * Index(input_position)); }
	Position right(const Position& input_position) { return GetPosition(2 * Index(input_position) + 1); }
	Position parent(const Position& input_position) { return GetPosition(Index(input_position) / 2); }
	bool HasLeft(const Position& input_position) const { return 2 * Index(input_position) <= size(); }
	bool HasRight(const Position& input_position) const { return 2 * Index(input_position) + 1 <= size(); }
	bool IsRoot(const Position& input_position) const { return Index(input_position) == 1; }
	Position root() { return GetPosition(1); }
	Position last() { return GetPosition(size()); }
public:
	MaxIntPriorityQueue() : space_{ -1 } {};
	~MaxIntPriorityQueue() { space_.clear(); }
public:
	size_t size() const { return space_.size() - 1; }
	void Clear() { space_.clear(); }
	bool Empty() const { return size() == 0; }
	int Top() { if (size() > 0) return *root(); else throw std::length_error("No element!"); }
	void Insert(const int& index);
	void Swap(const Position& left, const Position& right);
	int Pop();
	void Print() const;
};

void MaxIntPriorityQueue::Insert(const int& index)
{
	space_.push_back(index);
	Position current_position = last();
	while (!IsRoot(current_position)) {
		if (*current_position > * parent(current_position)) {
			Swap(current_position, parent(current_position));
			current_position = parent(current_position);
		}
		else break;
	}//UpHeapBubbling
}

void MaxIntPriorityQueue::Swap(const Position& left, const Position& right)
{
	int temp = *left;
	*left = *right;
	*right = std::forward<int>(temp);
}

int MaxIntPriorityQueue::Pop()
{
	if (size() > 1) {
		Swap(root(), last());
		int temp = *last();
		space_.pop_back();
		Position position_to_change = root();
		while (HasLeft(position_to_change)) {
			Position big_child = left(position_to_change);
			if (HasRight(position_to_change) && *left(position_to_change) < *right(position_to_change))
				big_child = right(position_to_change);
			if (*position_to_change < *big_child) {
				Swap(position_to_change, big_child);
				position_to_change = big_child;
			}
			else break;
		}//DownHeapBubbling
		return temp;
	}
	else if (size() == 1) {
		int temp = *last();
		space_.pop_back();
		return temp;
	}//root만 존재할 때
	else {
		throw std::length_error("No element!");
	}//원소가 존재하지 않을 때
}

void MaxIntPriorityQueue::Print() const
{
	if (size() == 0) throw std::length_error("No element!");
	for (int i = 1; i < space_.size(); i++)
		std::cout << space_[i] << ' ';
	std::cout << std::endl;
}


#pragma once
#include <iostream>
#include <vector>

class Sculpture;//������ Ŭ����
class Trail;//��å�� Ŭ����
class AdjListGraph;//���� ����Ʈ ��� �׷���
class PriorityQueue;//�켱 ���� ť(�� ���)

class Sculpture {
private:
	const unsigned int number_;//������ ��ȣ(����Ű, ����), �� �� �������� �ٲ��� �ʴ´�.
public://������ �� �Ҹ���
	Sculpture(const unsigned int& number) :number_(number) {};
public://getter and setter
	unsigned int get_number() const { return number_; }
};//������ Ŭ����

class Trail {
private:
	unsigned int one_;//��å�� ���� �� ����� �������� ��ȣ
	unsigned int two_;//��å�� �ݴ��� ����� �������� ��ȣ
	unsigned int cost_;//��å�� ������ ���
public://������, �Ҹ���
	Trail(const  unsigned int& one, const  unsigned int& two, const  unsigned int& cost) : one_(one), two_(two), cost_(cost) {};
public://getter and setter
	unsigned int get_one() const { return one_; }
	unsigned int get_two() const { return two_; }
	unsigned int get_cost() const { return cost_; }
	void set_one(const unsigned int& one) { one_ = one; }
	void set_two(const unsigned int& two) { two_ = two; }
	void set_cost(const unsigned int& cost) { cost_ = cost; }
};//��å�� Ŭ����

template <typename T>
class MyPriorityQueue : public std::exception {
private:
	class MyHeap {
	private:
		std::vector<T> vector_;
	private:
		using Position = typename std::vector<T>::iterator;//���� �迭�� ������ Heap���� �� ������ ������ Position(������ �ݺ���)�� �̿��Ͽ� �̷������. 
	private://���� ���� �Լ�
		Position position(int index) { return vector_.begin() + index; }//�ε����� �޾� ������ �ݺ��ڸ� ��ȯ
		int index(const Position& position) const { return static_cast<int>(position - vector_.begin()); }//������ �ݺ��ڸ� �޾� �ε��� ��ȯ
	private://Position ���� ���� ���� �Լ�
		Position left(const Position& input_position) { return position(2 * index(input_position)); }//��� Node�� left_child�� 2 * index
		Position right(const Position& input_position) { return position(2 * index(input_position) + 1); }//��� Node�� right_child�� 2 * index + 1
		Position parent(const Position& input_position) { return position(index(input_position) / 2); }//��� Node�� Parent�� index / 2
		bool HasLeft(const Position& input_position) const { return static_cast<size_t>(2 * index(input_position)) <= size(); }//���� �ش� Node�� left_child�� ���� �ִٸ� Size�� �׺��� Ŭ ���̴�
		bool HasRight(const Position& input_position) const { return static_cast<size_t>(2 * index(input_position) + 1) <= size(); }//���� �ش� Node�� right_child�� ���� �ִٸ� Size�� �׺��� Ŭ ���̴�
		bool IsRoot(const Position& input_position) const { return index(input_position) == 1; }
		Position root() { return position(1); }//Heap�� �ε��� 1���� �����ϹǷ�.
		Position last() { return position(size()); }
	public://������, �Ҹ���
		MyHeap() : vector_{ T() } {};//Heap�� �ε����� 1���� ����
	public:
		int size() const { return static_cast<int>(vector_.size() - 1); }//Heap�� �ε����� 1���� �����̱� ������ 1�� ���Ѵ�
		T min() { return root().operator*(); }//Min heap���� ��Ʈ�� �ִ� ���Ұ� ���� ���� �����̴�.
		bool Empty() const { return size() == 0; }
		void Swap(const Position& position_left, const Position& position_right) {
			T temp = *position_left;
			*position_left = *position_right;
			*position_right = temp;
		};//�� Position�� ���Ҹ� �ٲ۴�. 
		void Insert(const T& value) {
			vector_.push_back(value);//���� ���� �������� ���Ҹ� �����Ѵ�.
			Position current_position = last();
			while (!IsRoot(current_position)) {//��Ʈ���� �ö�Դٸ� �� �̻� �ö� �� �����Ƿ� �׸��Ѵ�.
				if (*current_position < *parent(current_position)) {//������ ���Ұ� ������ �θ𺸴� ���� �����ΰ�?
					Swap(current_position, parent(current_position));//�׷��ٸ� �θ�� ��ġ�� �ٲ۴�.
					current_position = parent(current_position);
				}
				else break;//�θ𺸴� ���� �ʴٸ� �׸��Ѵ�.
			}//UpHeapBubbling
		};//���Ҹ� ������ �� up-heap bubbling
		T RemoveMin() {
			if (size() > 1) {
				Swap(root(), last());//�����ϱ� ���� ���� ��Ʈ�� ������ ������ ��ġ�� �ٲ۴�.
				T temp = *last();//������ ����, �� ��Ʈ�� �־��� ���Ҹ� �ӽ÷� �����Ѵ�.
				vector_.pop_back();//������ ����, �� ��ǻ� ��Ʈ�� �־��� ���Ҹ� �����Ѵ�.
				Position current_position = root();//��Ʈ���� �����Ѵ�.
				while (HasLeft(current_position)) {//���� ������ ���� �ڽ��� �����ϴ°�?
					Position small_child = left(current_position);
					if (HasRight(current_position) && *left(current_position) > *right(current_position))//���� ������ �ڽĵ� �����Ѵٸ�, ���� �ڽİ� ������ �ڽ� �� ���� �� ������?
						small_child = right(current_position);//�������� �� �۴ٸ� ������ ���Ҹ� ����.
					if (*current_position > *small_child) {//���� �� �� �� ���� ���Ҹ� ���� �ڽĺ��� �� �۴ٸ�,
						Swap(current_position, small_child);//���� ��ġ�� �ٲ۴�. �̷ν� �� ���� �ڽ��� ���� �ö󰣴�.
						current_position = small_child;
					}
					else break;//�ڽĺ��� ���� �ʴٸ� �׸��Ѵ�.
				}
				return temp;//�ӽ÷� ������ ���Ҹ� ��ȯ.
			}
			else if (size() == 1) {
				T temp = *last();
				vector_.pop_back();
				return temp;
			}//���� ��Ʈ�ۿ� ���ٸ� ��Ʈ�� ���Ҹ� ��ȯ�� �� ����.
			else {
				throw std::length_error("There is no element!");
			}//���Ұ� ���� ���.
		};//���� ���� ���Ҹ� �����ϰ� down-heap bubbling �� �� ��ȯ�Ѵ�. ��������� Heap �� ���� ���� ���Ұ� ��Ʈ�� �ְ� �ȴ�.
	};
public://������, �Ҹ���
	MyPriorityQueue() {};
public:
	void Push(const T& element) {
		heap_.Insert(element);
	}//�켱 ���� ť�� ���Ҹ� �����Ѵ�.
	T Front()  {
		return heap_.min();
	}//���� �տ� �ִ� ���Ҹ� ��ȯ�Ѵ�.
	T Pop() {
		return heap_.RemoveMin();
	}//���� �տ� �ִ� ���Ҹ� ������ �� ��ȯ�Ѵ�.
	bool IsEmpty() const { return heap_.Empty(); };
	size_t get_size() const { return heap_.size(); }
private:
	MyHeap heap_;
};//�켱 ���� ť(������ ����)

class AdjListGraph {
private:
	class Vertex;
	class Edge;
public:
	using VertexList = std::vector<Vertex>;//Vertex�� ���� ������� ��� �迭.
	using EdgeList = std::vector< std::vector<Edge > >;//Edge�� �迭�� ���� ������� ��� �迭. 
	//�� VertexList�� EdgeList�� Index�� �����Ǿ�, i ��° vertex�� �����ϴ� edge�� ����Ʈ�� EdgeList[i]�̴�.
private:
	class Vertex {
	private:
		Sculpture sculpture_;//Vertex�� �������� ���ҷ� ���´�.
	public://������, �Ҹ���
		Vertex(const unsigned int& number) :sculpture_(number){ };
	public://getter and setter
		unsigned int get_number() const { return sculpture_.get_number(); }
	};//Vertex Ŭ����

	class Edge {
	private:
		Trail trail_;//Edge�� ��å�θ� ���ҷ� ���´�. 
		//��� Vertex�� ����Ǿ� �ִ���, �� ���� ���� ��å�� ���ҿ� ����Ǿ� �ִ�.
	public://������, �Ҹ���
		Edge() : trail_(unsigned int(), unsigned int(), unsigned int()) {};
		Edge(const  unsigned int& one, const  unsigned int& two, const  unsigned int& cost) : trail_(one, two, cost) {};
	public://getter and setter
		/*bool operator<(const Edge& edge) const {
			if (trail_.get_cost() != edge.trail_.get_cost())
				return trail_.get_cost() < edge.trail_.get_cost();
			else
				return trail_.get_two() < edge.trail_.get_cost();
		}
		bool operator>(const Edge& edge) const {
			if (trail_.get_cost() != edge.trail_.get_cost())
				return trail_.get_cost() > edge.trail_.get_cost();
			else
				return trail_.get_two() > edge.trail_.get_cost();
		}*/
		unsigned int get_one() const { return trail_.get_one(); }
		unsigned int get_two() const { return trail_.get_two(); }
		unsigned int get_cost() const { return trail_.get_cost(); }
		void set(const  unsigned int& one, const  unsigned int& two, const  unsigned int& cost) {
			trail_.set_one(one);
			trail_.set_two(two);
			trail_.set_cost(cost);
		}
	};

public://������, �Ҹ���
	AdjListGraph(const unsigned int& number) {
		vertice_.reserve(number);
		edges_.resize(number , std::vector<Edge>() );
		for (int j = 1; j <= number; j++) 
			vertice_.push_back(j);
	}//Vertex�� �� n�� ���ڷ� �޾�, 1 ~ n���� ���ڸ� ���� Vertice�� �����Ѵ�. i�� n���� ũ�ٰ� ����.
public:
	size_t SizeOfVertex() const { return vertice_.size(); }//Vertice�� �󸶳� �ִ°�?
	size_t SizeOfEdge() const { return edges_.size(); }//Edge�� �󸶳� �ִ°�?
	void InsertEdge(const unsigned int& one, const unsigned int& two, const unsigned int& cost){
		edges_[one - 1].push_back(Edge(one, two, cost));
		edges_[two - 1].push_back(Edge(two, one, cost));
	};//Edge�� �����Ѵ�. �ϳ��� Edge�� ���� ������ �̿� ������ �� Vertex�� ����Ʈ�� ��� ����ȴ�.
	void PrimPrint(const unsigned int& start) const {
		VertexList temp_vertice;//�ּ� ���� Ʈ���� �߰��Ǵ� Vertex�� ������ ����ϱ� ���� �ӽ÷� �����ϴ� �迭.
		unsigned int cost = Prim(start, temp_vertice);
		std::cout << cost << ' ';
		for (const auto& itr : temp_vertice)
			std::cout << itr.get_number() << ' ';
		std::cout << std::endl;
	}//Prim's algorithm�� ����� �޾�, �̸� ����ϴ� �Լ�.
private:
	unsigned int Prim(const unsigned int& start, VertexList& vertice) const {
		//Vertex�� ���¸� �����ϰ� ����ϱ� ���� enum class ����
		enum class STATUS {
			UNSEEN,TREE, FRINGE
		};

		//Prim's algorithm ���� �ּ� ����ġ�� ���� Edge�� �����ϰ� ����ϱ� ���� ����ü
		struct Entry {
			unsigned int vertex_number;//vertex number��
			unsigned int weight;//�� vertex���� ���� ���� �ɸ��� ����ġ
			Entry() :vertex_number(int()) , weight(int()) {};
			Entry(const unsigned int& number, const unsigned int& weight) :vertex_number(number), weight(weight) {};
			//�켱 ���� ť���� ��� �񱳸� �ϱ� ���� ������
			bool operator<(const Entry& entry) const {
				if (weight != entry.weight)
					return weight < entry.weight;
				else
					return vertex_number < entry.vertex_number;
			}
			bool operator>(const Entry& entry) const {
				if (weight != entry.weight)
					return weight > entry.weight;
				else
					return vertex_number > entry.vertex_number;
			}
		};

		std::vector<STATUS> status(SizeOfVertex(), STATUS::UNSEEN);//�� Vertex�� �����Ǵ� ���¸� �����ϴ� �迭. i��° Vertex�� ���´� status[i - 1]�� ����Ǿ� �ִ�.
		MyPriorityQueue<Entry> table;//Prim's algorithm ������ �ٽ� �ڷ� ����. �켱 ���� ť�� [Vertex number, �ش� Vertex���� ���� ���� �ʿ��� ����]�� ��´�.
		unsigned int sum{ 0 };//����� �����ϱ� ���� ����

		status[start - 1] = STATUS::TREE;//���� Vertex�� ���¸� TREE�� ��ȯ�Ѵ�.
		vertice.push_back(start);//���� Vertex��, Vertex�� �߰��� ������ ����ϴ� �迭�� �����Ѵ�.

		for (const auto& itr : edges_[start - 1]) {
			if( status[itr.get_one() - 1] == STATUS::TREE)
				table.Push( Entry(itr.get_two(), itr.get_cost()) );
			else
				table.Push(Entry(itr.get_one(), itr.get_cost()));
		}//���� Vertex�� ������ ������ ������ �켱 ���� ť�� ��´�. 

		do
		{
			while (!table.IsEmpty() && status[table.Front().vertex_number - 1] == STATUS::TREE)//���� �켱 ���� ť�� ���� �տ� �ִ� ���Ұ� �̹� �ּ� ���� Ʈ���� �ִ� Vertex�� �������
				table.Pop();//�����Ѵ�.
			if (table.IsEmpty()) break;//��� �����ߴµ� �켱 ���� ť�� �� �̻� ���� �� ���ٸ� �����Ѵ�.

			vertice.push_back( table.Front().vertex_number );//�ּ� ���� Ʈ���� �߰��� ����(�ش� Vertex�� ���� ���� �ʿ��� ������ ���� ����)�� Vertex number�� �迭�� �����Ѵ�.

			status[table.Front().vertex_number - 1] = STATUS::TREE;//�ּ� ���� Ʈ���� �߰��� ������ ���¸� TREE�� ��ȯ�Ѵ�.
			sum += table.Pop().weight;//�� ������ �����Ѵ�.

			for (const auto& itr : edges_[vertice.back().get_number() - 1]) {//�ּ� ���� Ʈ���� �� �߰��� Vertex�� ���� Edge ����Ʈ�� ��ȸ�Ѵ�.
				if (status[itr.get_two() - 1] != STATUS::TREE) {
					table.Push( Entry( itr.get_two() , itr.get_cost() ));
					status[itr.get_two() - 1] = STATUS::FRINGE;
				}//�ּ� ���� Ʈ���� �� �߰��� Vertex�� ������ ������ ������ �켱 ���� ť�� ��´�.
			}
		} while (!table.IsEmpty());//�켱 ���� ť�� �� ������ �ݺ��Ѵ�.
		return sum;//�� ������ ��ȯ�Ѵ�.
	}//Prim's algorithm
private:
	VertexList vertice_;//1 ~ n���� Vertex�� ����Ǿ� �ִ� �迭
	EdgeList edges_;//1 ~ n���� �� Vertex�� ����Ǿ� �ִ� Edge ������ ����Ǿ� �ִ� �迭
};//���� ����Ʈ ��� �׷���
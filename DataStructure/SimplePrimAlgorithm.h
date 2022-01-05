#pragma once
#include <iostream>
#include <vector>

class Sculpture;//조형물 클래스
class Trail;//산책로 클래스
class AdjListGraph;//인접 리스트 기반 그래프
class PriorityQueue;//우선 순위 큐(힙 기반)

class Sculpture {
private:
	const unsigned int number_;//조형물 번호(기준키, 유일), 한 번 정해지면 바뀌지 않는다.
public://생성자 및 소멸자
	Sculpture(const unsigned int& number) :number_(number) {};
public://getter and setter
	unsigned int get_number() const { return number_; }
};//조형물 클래스

class Trail {
private:
	unsigned int one_;//산책로 한쪽 편에 연결된 조형물의 번호
	unsigned int two_;//산책로 반대편에 연결된 조형물의 번호
	unsigned int cost_;//산책로 디자인 비용
public://생성자, 소멸자
	Trail(const  unsigned int& one, const  unsigned int& two, const  unsigned int& cost) : one_(one), two_(two), cost_(cost) {};
public://getter and setter
	unsigned int get_one() const { return one_; }
	unsigned int get_two() const { return two_; }
	unsigned int get_cost() const { return cost_; }
	void set_one(const unsigned int& one) { one_ = one; }
	void set_two(const unsigned int& two) { two_ = two; }
	void set_cost(const unsigned int& cost) { cost_ = cost; }
};//산책로 클래스

template <typename T>
class MyPriorityQueue : public std::exception {
private:
	class MyHeap {
	private:
		std::vector<T> vector_;
	private:
		using Position = typename std::vector<T>::iterator;//선형 배열로 구현한 Heap에서 각 원소의 접근은 Position(벡터의 반복자)을 이용하여 이루어진다. 
	private://내부 편의 함수
		Position position(int index) { return vector_.begin() + index; }//인덱스를 받아 벡터의 반복자를 반환
		int index(const Position& position) const { return static_cast<int>(position - vector_.begin()); }//벡터의 반복자를 받아 인덱스 반환
	private://Position 관련 내부 편의 함수
		Position left(const Position& input_position) { return position(2 * index(input_position)); }//모든 Node의 left_child는 2 * index
		Position right(const Position& input_position) { return position(2 * index(input_position) + 1); }//모든 Node의 right_child는 2 * index + 1
		Position parent(const Position& input_position) { return position(index(input_position) / 2); }//모든 Node의 Parent는 index / 2
		bool HasLeft(const Position& input_position) const { return static_cast<size_t>(2 * index(input_position)) <= size(); }//만약 해당 Node가 left_child를 갖고 있다면 Size는 그보다 클 것이다
		bool HasRight(const Position& input_position) const { return static_cast<size_t>(2 * index(input_position) + 1) <= size(); }//만약 해당 Node가 right_child를 갖고 있다면 Size는 그보다 클 것이다
		bool IsRoot(const Position& input_position) const { return index(input_position) == 1; }
		Position root() { return position(1); }//Heap은 인덱스 1부터 시작하므로.
		Position last() { return position(size()); }
	public://생성자, 소멸자
		MyHeap() : vector_{ T() } {};//Heap의 인덱스는 1부터 시작
	public:
		int size() const { return static_cast<int>(vector_.size() - 1); }//Heap의 인덱스는 1부터 시작이기 때문에 1을 감한다
		T min() { return root().operator*(); }//Min heap으로 루트에 있는 원소가 가장 작은 원소이다.
		bool Empty() const { return size() == 0; }
		void Swap(const Position& position_left, const Position& position_right) {
			T temp = *position_left;
			*position_left = *position_right;
			*position_right = temp;
		};//두 Position의 원소를 바꾼다. 
		void Insert(const T& value) {
			vector_.push_back(value);//먼저 제일 마지막에 원소를 삽입한다.
			Position current_position = last();
			while (!IsRoot(current_position)) {//루트까지 올라왔다면 더 이상 올라갈 수 없으므로 그만한다.
				if (*current_position < *parent(current_position)) {//삽입한 원소가 현재의 부모보다 작은 숫자인가?
					Swap(current_position, parent(current_position));//그렇다면 부모와 위치를 바꾼다.
					current_position = parent(current_position);
				}
				else break;//부모보다 작지 않다면 그만한다.
			}//UpHeapBubbling
		};//원소를 삽입한 후 up-heap bubbling
		T RemoveMin() {
			if (size() > 1) {
				Swap(root(), last());//삭제하기 위해 먼저 루트와 마지막 원소의 위치를 바꾼다.
				T temp = *last();//마지막 원소, 즉 루트에 있었던 원소를 임시로 저장한다.
				vector_.pop_back();//마지막 원소, 즉 사실상 루트에 있었던 원소를 제거한다.
				Position current_position = root();//루트부터 시작한다.
				while (HasLeft(current_position)) {//현재 원소의 왼쪽 자식이 존재하는가?
					Position small_child = left(current_position);
					if (HasRight(current_position) && *left(current_position) > *right(current_position))//만약 오른쪽 자식도 존재한다면, 왼쪽 자식과 오른쪽 자식 중 누가 더 작은가?
						small_child = right(current_position);//오른쪽이 더 작다면 오른쪽 원소를 선택.
					if (*current_position > *small_child) {//만약 둘 중 더 작은 원소를 가진 자식보다 더 작다면,
						Swap(current_position, small_child);//둘의 위치를 바꾼다. 이로써 더 작은 자식이 위로 올라간다.
						current_position = small_child;
					}
					else break;//자식보다 작지 않다면 그만한다.
				}
				return temp;//임시로 저장한 원소를 반환.
			}
			else if (size() == 1) {
				T temp = *last();
				vector_.pop_back();
				return temp;
			}//만약 루트밖에 없다면 루트의 원소를 반환한 후 종료.
			else {
				throw std::length_error("There is no element!");
			}//원소가 없을 경우.
		};//가장 작은 원소를 삭제하고 down-heap bubbling 한 후 반환한다. 결과적으로 Heap 내 가장 작은 원소가 루트에 있게 된다.
	};
public://생성자, 소멸자
	MyPriorityQueue() {};
public:
	void Push(const T& element) {
		heap_.Insert(element);
	}//우선 순위 큐에 원소를 삽입한다.
	T Front()  {
		return heap_.min();
	}//가장 앞에 있는 원소를 반환한다.
	T Pop() {
		return heap_.RemoveMin();
	}//가장 앞에 있는 원소를 제거한 후 반환한다.
	bool IsEmpty() const { return heap_.Empty(); };
	size_t get_size() const { return heap_.size(); }
private:
	MyHeap heap_;
};//우선 순위 큐(힙으로 구현)

class AdjListGraph {
private:
	class Vertex;
	class Edge;
public:
	using VertexList = std::vector<Vertex>;//Vertex를 숫자 순서대로 담는 배열.
	using EdgeList = std::vector< std::vector<Edge > >;//Edge의 배열을 숫자 순서대로 담는 배열. 
	//위 VertexList와 EdgeList의 Index는 대응되어, i 번째 vertex와 인접하는 edge의 리스트는 EdgeList[i]이다.
private:
	class Vertex {
	private:
		Sculpture sculpture_;//Vertex는 조형물을 원소로 갖는다.
	public://생성자, 소멸자
		Vertex(const unsigned int& number) :sculpture_(number){ };
	public://getter and setter
		unsigned int get_number() const { return sculpture_.get_number(); }
	};//Vertex 클래스

	class Edge {
	private:
		Trail trail_;//Edge는 산책로를 원소로 갖는다. 
		//어느 Vertex와 연결되어 있는지, 그 여부 역시 산책로 원소에 저장되어 있다.
	public://생성자, 소멸자
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

public://생성자, 소멸자
	AdjListGraph(const unsigned int& number) {
		vertice_.reserve(number);
		edges_.resize(number , std::vector<Edge>() );
		for (int j = 1; j <= number; j++) 
			vertice_.push_back(j);
	}//Vertex의 수 n를 인자로 받아, 1 ~ n까지 숫자를 갖는 Vertice를 생성한다. i는 n보다 크다고 가정.
public:
	size_t SizeOfVertex() const { return vertice_.size(); }//Vertice가 얼마나 있는가?
	size_t SizeOfEdge() const { return edges_.size(); }//Edge가 얼마나 있는가?
	void InsertEdge(const unsigned int& one, const unsigned int& two, const unsigned int& cost){
		edges_[one - 1].push_back(Edge(one, two, cost));
		edges_[two - 1].push_back(Edge(two, one, cost));
	};//Edge를 삽입한다. 하나의 Edge에 대한 정보는 이와 인접한 두 Vertex의 리스트에 모두 저장된다.
	void PrimPrint(const unsigned int& start) const {
		VertexList temp_vertice;//최소 신장 트리에 추가되는 Vertex의 순서를 기억하기 위해 임시로 선언하는 배열.
		unsigned int cost = Prim(start, temp_vertice);
		std::cout << cost << ' ';
		for (const auto& itr : temp_vertice)
			std::cout << itr.get_number() << ' ';
		std::cout << std::endl;
	}//Prim's algorithm의 결과를 받아, 이를 출력하는 함수.
private:
	unsigned int Prim(const unsigned int& start, VertexList& vertice) const {
		//Vertex의 상태를 간편하게 기억하기 위한 enum class 선언
		enum class STATUS {
			UNSEEN,TREE, FRINGE
		};

		//Prim's algorithm 도중 최소 가중치를 갖는 Edge를 간편하게 기억하기 위한 구조체
		struct Entry {
			unsigned int vertex_number;//vertex number와
			unsigned int weight;//그 vertex까지 가는 데에 걸리는 가중치
			Entry() :vertex_number(int()) , weight(int()) {};
			Entry(const unsigned int& number, const unsigned int& weight) :vertex_number(number), weight(weight) {};
			//우선 순위 큐에서 대소 비교를 하기 위한 연산자
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

		std::vector<STATUS> status(SizeOfVertex(), STATUS::UNSEEN);//각 Vertex와 대응되는 상태를 저장하는 배열. i번째 Vertex의 상태는 status[i - 1]에 저장되어 있다.
		MyPriorityQueue<Entry> table;//Prim's algorithm 과정의 핵심 자료 구조. 우선 순위 큐에 [Vertex number, 해당 Vertex까지 가는 데에 필요한 가중]을 담는다.
		unsigned int sum{ 0 };//비용을 저장하기 위한 변수

		status[start - 1] = STATUS::TREE;//시작 Vertex의 상태를 TREE로 변환한다.
		vertice.push_back(start);//시작 Vertex를, Vertex가 추가된 순서를 기억하는 배열에 저장한다.

		for (const auto& itr : edges_[start - 1]) {
			if( status[itr.get_one() - 1] == STATUS::TREE)
				table.Push( Entry(itr.get_two(), itr.get_cost()) );
			else
				table.Push(Entry(itr.get_one(), itr.get_cost()));
		}//시작 Vertex와 인접한 간선의 정보를 우선 순위 큐에 담는다. 

		do
		{
			while (!table.IsEmpty() && status[table.Front().vertex_number - 1] == STATUS::TREE)//만약 우선 순위 큐의 가장 앞에 있는 원소가 이미 최소 신장 트리에 있는 Vertex의 정보라면
				table.Pop();//제거한다.
			if (table.IsEmpty()) break;//모두 제거했는데 우선 순위 큐에 더 이상 남은 게 없다면 종료한다.

			vertice.push_back( table.Front().vertex_number );//최소 신장 트리에 추가될 원소(해당 Vertex로 가는 데에 필요한 가중이 가장 작은)의 Vertex number를 배열에 저장한다.

			status[table.Front().vertex_number - 1] = STATUS::TREE;//최소 신장 트리에 추가될 원소의 상태를 TREE로 변환한다.
			sum += table.Pop().weight;//총 가중을 갱신한다.

			for (const auto& itr : edges_[vertice.back().get_number() - 1]) {//최소 신장 트리에 막 추가된 Vertex의 인접 Edge 리스트를 순회한다.
				if (status[itr.get_two() - 1] != STATUS::TREE) {
					table.Push( Entry( itr.get_two() , itr.get_cost() ));
					status[itr.get_two() - 1] = STATUS::FRINGE;
				}//최소 신장 트리에 막 추가된 Vertex와 인접한 간선의 정보를 우선 순위 큐에 담는다.
			}
		} while (!table.IsEmpty());//우선 순위 큐가 빌 때까지 반복한다.
		return sum;//총 가중을 반환한다.
	}//Prim's algorithm
private:
	VertexList vertice_;//1 ~ n까지 Vertex가 저장되어 있는 배열
	EdgeList edges_;//1 ~ n까지 각 Vertex와 연결되어 있는 Edge 정보가 저장되어 있는 배열
};//인접 리스트 기반 그래프
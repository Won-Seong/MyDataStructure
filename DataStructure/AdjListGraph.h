#pragma once
#include <iostream>
#include <vector>
#include <set>

template <typename VT, typename ET>
class AdjListGraph : std::exception {
private:
	class Vertex;
	class Edge;
	using VertexPtr = std::shared_ptr<Vertex>;
	using VertexList = std::vector<VertexPtr>;
	using VertexWeakPtr = std::weak_ptr<Vertex>;
	using EdgePtr = std::shared_ptr<Edge>;
	using EdgeList = std::vector<EdgePtr>;
	using EdgeWeakPtr = std::weak_ptr<Edge>;
	using IncidentEdgeList = std::vector< EdgePtr >;
	using IncidentEdgeListPtr = std::shared_ptr< IncidentEdgeList >;
	using IncidentEdgeListWeakPtr = std::weak_ptr< IncidentEdgeList >;
	enum class STATUS {
		UNVISITED,
		VISITED,
		DISCOVERY,
		BACK
	};
private:
	class Vertex {
	private:
		VT vertex_data_;
		int vertex_index_;//이 정점이 몇 번째로 생성되었는가?
		IncidentEdgeListPtr incident_edge_list_;
		STATUS vertex_status_;
	public:
		Vertex(const int& index, const VT& data_ = VT()) :vertex_index_(index), vertex_data_(data_), vertex_status_(STATUS::UNVISITED) { incident_edge_list_ = std::make_shared<IncidentEdgeList>(); }
		~Vertex() { };
		VT& operator*() { return vertex_data_; }
		const VT& operator*() const { return vertex_data_; }
		const int& get_index() const { return vertex_index_; }
		void set_status(STATUS status) { vertex_status_ = status; }
		STATUS get_status() const { return vertex_status_; }
		IncidentEdgeListPtr get_incident_edge_list() const { return incident_edge_list_; }
		friend class AdjListGraph;
	};
	class Edge {
	private:
		ET edge_data_;
		int edge_index_;//이 간선이 몇 번째로 생성되었는가?
		VertexWeakPtr left_vertex_;
		IncidentEdgeListWeakPtr left_vertex_incident_edge_list_;
		VertexWeakPtr right_vertex_;
		IncidentEdgeListWeakPtr right_vertex_incident_edge_list_;
		STATUS edge_status_;
	public:
		Edge(const int& index, VertexPtr left, VertexPtr right, const ET& data_ = ET()) : edge_index_(index), left_vertex_(left), right_vertex_(right), edge_data_(data_), edge_status_(STATUS::UNVISITED) {
			left_vertex_incident_edge_list_ = left_vertex_.lock()->get_incident_edge_list();
			right_vertex_incident_edge_list_ = right_vertex_.lock()->get_incident_edge_list();
		}
		~Edge() {};
		ET& operator*() { return edge_data_; }
		const ET& operator*() const { return edge_data_; }
		const int& get_index() const { return edge_index_; }
		VertexWeakPtr get_left_vertex() const { return left_vertex_; }
		VertexWeakPtr get_right_vertex() const { return right_vertex_; }
		IncidentEdgeListWeakPtr get_incident_edge_list_of_left() const { return left_vertex_incident_edge_list_; }
		IncidentEdgeListWeakPtr get_incident_edge_list_of_right() const { return right_vertex_incident_edge_list_; }
		void set_status(STATUS status) { edge_status_ = status; }
		STATUS get_status() const { return edge_status_; }
		friend class AdjListGraph;
	};
	struct VertexComp {
		bool operator()(const VertexPtr left, const VertexPtr right) const {
			return **left < **right;
		}
	};//VertexPtr를 이용하여 정점을 비교할 때 쓰이는 Functor
public:
	AdjListGraph() :graph_vertex_index_(0) , graph_edge_index_(0) {};
	size_t VerticesSize() const { return vertices_.size(); }
	size_t EdgeSize() const { return edges_.size(); }
	VertexList Vertices() const { return vertices_; }
	EdgeList Edges() const { return edges_; }
	bool Empty() const { return VerticesSize() == 0 && EdgeSize() == 0; }
	void Print() const;
	void Clear() { 
		vertices_.clear(); edges_.clear();
		graph_edge_index_ = 0; graph_vertex_index_ = 0;
	};
public:
	void InsertVertex(const VT& data);
	VertexPtr SearchVertex(const VT& data) const;
	void PrintIncidentEdgeOfVertex(VertexPtr vertex) const;
	void InsertEdge(VertexPtr left, VertexPtr right, const ET& data);
	EdgePtr SearchEdge(const ET& data) const;
	VT EraseVertex(VertexPtr vertex);
	ET EraseEdge(EdgePtr edge);
	void PrintByDfs(VertexPtr start_vertex) const;//DFS를 이용하여 출력
	std::vector<VT> DfsFunction(const VertexPtr start_vertex) const;//간단한 DFS 함수
	void PrintByBfs(VertexPtr start_vertex) const;//BFS를 이용하여 출력
	std::vector<VT> BfsFunction(const VertexPtr start_vertex) const;//간단한 BFS 함수
private:
	IncidentEdgeListPtr IncidentEdgeOfVertex(VertexPtr vertex) const;
	VertexList IncidentVerticesOfVertex(VertexPtr vertex) const;
	VertexList IncidentVerticesOfVertexOrdered(VertexPtr vertex) const;//정점의 수대로 정렬된 리스트
	void DfsUtility(std::vector<VT>& data_list, VertexPtr current_vertex) const;//재귀를 이용한 DFS 유틸리티
	void BfsUtility(int& level,std::vector<VT>& data_list, VertexList current_vertices_list) const;//재귀를 이용한 BFS 유틸리티
private:
	VertexList vertices_;
	EdgeList edges_;
	int graph_vertex_index_;
	int graph_edge_index_;
public:
	template <typename G>
	friend class DFS;
};
#include "AdjListGraph.hpp"
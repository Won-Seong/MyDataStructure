#pragma once
#include "AdjListGraph.h"
#include <algorithm>

template<typename VT, typename ET>
inline void AdjListGraph<VT, ET>::Print() const
{
	std::cout << "\n======Graph Print======" << std::endl;
	std::cout << "Vertex" << std::endl;
	if (VerticesSize() == 0) { std::cout << "There is no vertex!" << std::endl; }
	else {
		for (const auto& itr : vertices_) {
			std::cout << "Index : " << itr->get_index() << "\t Data : " << **itr << std::endl;
		}
	}
	std::cout << std::endl;
	if (EdgeSize() == 0) { std::cout << "There is no edge!" << std::endl; }
	else {
		std::cout << "Edge" << std::endl;
		for (const auto& itr : edges_) {
			std::cout << "Index : " << itr->get_index() << "\t Data : " << **itr << std::endl;
		}
	}
	std::cout << "======Graph Print======\n" <<std::endl;
}

template<typename VT, typename ET>
inline void AdjListGraph<VT, ET>::InsertVertex(const VT& data)
{
	if (SearchVertex(data) != nullptr) throw std::invalid_argument("There is already such vertex!");
	VertexPtr temp = std::make_shared<Vertex>(graph_vertex_index_++,data);	
	vertices_.push_back(std::move(temp));
}

template<typename VT, typename ET>
inline void AdjListGraph<VT, ET>::InsertEdge(VertexPtr left, VertexPtr right , const ET& data)
{
	if (left == nullptr) throw std::invalid_argument("There is no such left vertex!");
	else if (right == nullptr) throw std::invalid_argument("There is no such right vertex!");
	EdgePtr temp = std::make_shared<Edge>(graph_edge_index_++, left, right, data);
	left->get_incident_edge_list()->push_back(temp);
	right->get_incident_edge_list()->push_back(temp);
	edges_.push_back(std::move(temp));
}

template<typename VT, typename ET>
inline typename AdjListGraph<VT,ET>::VertexPtr AdjListGraph<VT, ET>::SearchVertex(const VT& data) const
{
	for (const auto& itr : vertices_) {
		if (**itr == data) { return itr; }
	}
	return VertexPtr();
}

template<typename VT, typename ET>
inline VT AdjListGraph<VT, ET>::EraseVertex(VertexPtr vertex)
{
	if (vertex == nullptr) throw std::invalid_argument("There is no such vertex!");
	IncidentEdgeList temp{ vertex->get_incident_edge_list().operator*() };
	for (auto& itr : temp)
		EraseEdge(itr);
	VT temp_data{ **vertex };
	vertices_.erase(std::remove(vertices_.begin(), vertices_.end(), vertex), vertices_.end());
	return temp_data;
}

template<typename VT, typename ET>
inline ET AdjListGraph<VT, ET>::EraseEdge(EdgePtr edge)
{
	if (edge == nullptr) throw std::invalid_argument("There is no such edge!");
	//해당 edge에 인접한 Vertex들의 IncidentEdgeList
	IncidentEdgeListPtr left{ edge->get_incident_edge_list_of_left() };
	IncidentEdgeListPtr right{ edge->get_incident_edge_list_of_right() };
	//Vertex의 IncidentEdgeList에서 해당 edge를 지우는 과정
	left->erase(std::remove(left.operator*().begin(), left.operator*().end(), edge),left->end());
	right->erase(std::remove(right.operator*().begin(), right.operator*().end(), edge),right->end());
	//해당 edge를 Graph의 edge 목록에서 삭제
	ET temp{ **edge };
	edges_.erase(std::remove(edges_.begin(), edges_.end(), edge),edges_.end());
	return temp;
}

template<typename VT, typename ET>
inline std::vector<VT> AdjListGraph<VT, ET>::DfsFunction(const VertexPtr start_vertex) const
{
	std::vector<VT> VT_list;
	for (auto& itr : vertices_) {
		itr->set_status(STATUS::UNVISITED);
	}
	DfsUtility(VT_list, start_vertex);
	return VT_list;
}

template<typename VT, typename ET>
inline typename AdjListGraph<VT,ET>::VertexList AdjListGraph<VT, ET>::IncidentVerticesOfVertexOrdered(VertexPtr vertex) const
{
	std::set<VertexPtr,VertexComp> incident_vertices_set;
	IncidentEdgeList incident_edge_list = vertex->get_incident_edge_list().operator*();
	for (const auto& itr : incident_edge_list) {
		if (itr->get_left_vertex().lock() == vertex) incident_vertices_set.insert(itr->get_right_vertex().lock());
		else incident_vertices_set.insert(itr->get_left_vertex().lock());
	}
	VertexList incident_vertices_list;
	for (const auto& itr : incident_vertices_set) 
		incident_vertices_list.push_back(itr);
	return incident_vertices_list;
}

template<typename VT, typename ET>
inline std::vector<VT> AdjListGraph<VT, ET>::BfsFunction(const VertexPtr start_vertex) const
{
	std::vector<VT> VT_list;
	VertexList temp_list;
	for (auto& itr : vertices_) {
		itr->set_status(STATUS::UNVISITED);
	}
	temp_list.push_back(start_vertex);
	start_vertex->set_status(STATUS::VISITED);
	int level{ 0 };
	BfsUtility(level ,VT_list, temp_list);
	return VT_list;
}

template<typename VT, typename ET>
inline void AdjListGraph<VT, ET>::BfsUtility(int& level, std::vector<VT>& data_list, VertexList current_vertices_list) const
{
	VertexList next_vertex_list;
		for (auto& itr : current_vertices_list) {
			data_list.push_back(**itr);
			VertexList incident_vertices_list = IncidentVerticesOfVertexOrdered(itr);
			for (auto& itr : incident_vertices_list) {
				if (itr->get_status() == STATUS::UNVISITED) {
					next_vertex_list.push_back(itr);
					itr->set_status(STATUS::VISITED);
				}
			}
		}
		if (next_vertex_list.empty()) return;
		BfsUtility(++level, data_list, next_vertex_list);
}

template<typename VT, typename ET>
inline void AdjListGraph<VT, ET>::PrintByDfs(VertexPtr start_vertex) const
{
	std::vector<VT> data_list{ DfsFunction(start_vertex) };
	std::cout << "\n======DFS======\n" << std::endl;
	for (const auto& itr : data_list)
		std::cout << itr << ' ';
	std::cout << "\n\n======DFS======\n" << std::endl;
}

template<typename VT, typename ET>
inline void AdjListGraph<VT, ET>::PrintByBfs(VertexPtr start_vertex) const
{
	std::vector<VT> data_list{ BfsFunction(start_vertex) };
	std::cout << "\n======BFS======\n" << std::endl;
	for (const auto& itr : data_list)
		std::cout << itr << ' ';
	std::cout << "\n\n======BFS======\n" << std::endl;
}

template<typename VT, typename ET>
inline typename AdjListGraph<VT,ET>::IncidentEdgeListPtr AdjListGraph<VT, ET>::IncidentEdgeOfVertex(VertexPtr vertex) const
{
	if (vertex == nullptr) throw std::invalid_argument("There is no such vertex!");
	return vertex->get_incident_edge_list();
}

template<typename VT, typename ET>
inline typename AdjListGraph<VT , ET>::VertexList AdjListGraph<VT, ET>::IncidentVerticesOfVertex(VertexPtr vertex) const
{
	VertexList incident_vertices_list;
	IncidentEdgeList incident_edge_list = vertex->get_incident_edge_list().operator*();
	for (const auto& itr : incident_edge_list) {

		if (itr->get_left_vertex().lock() == vertex) incident_vertices_list.push_back(itr->get_right_vertex().lock());
		else incident_vertices_list.push_back(itr->get_left_vertex().lock());
	}
	return incident_vertices_list;
}

template<typename VT, typename ET>
inline void AdjListGraph<VT, ET>::DfsUtility(std::vector<VT>& data_list, VertexPtr current_vertex) const
{
	data_list.push_back(**current_vertex);
	current_vertex->set_status(STATUS::VISITED);
	VertexList incident_vertices_list = IncidentVerticesOfVertexOrdered(current_vertex);
	for (auto& itr : incident_vertices_list) {
		if (itr->get_status() == STATUS::UNVISITED)
			DfsUtility(data_list, itr);
	}
}

template<typename VT, typename ET>
inline void AdjListGraph<VT, ET>::PrintIncidentEdgeOfVertex(VertexPtr vertex) const
{
	IncidentEdgeListPtr temp{ IncidentEdgeOfVertex(vertex) };
	if (temp->size() == 0) {
		std::cout << "There is no incident edge!" << std::endl;
	}
	else {
		std::cout << "Incident edge of vertex has " << **vertex << std::endl;
		for (const auto& itr : *temp)
			std::cout << **itr << ' ';
		std::cout << std::endl;
	}
}

template<typename VT, typename ET>
inline typename AdjListGraph<VT,ET>::EdgePtr AdjListGraph<VT, ET>::SearchEdge(const ET& data) const
{
	for (const auto& itr : edges_) {
		if (**itr == data) return itr;
	}
	return EdgePtr();
}
#pragma once
#include "DFS.h"

template<typename G>
inline void DFS<G>::Initialize()
{
	VertexList& vertex_list{ graph_.Vertices() };
	EdgeList& edge_list{ graph_.Edges() };
	for (auto& itr : vertex_list) {
		itr->set_status(STATUS::UNVISITED);
	}
	for (auto& itr : edge_list) {
		itr->set_status(STATUS::UNVISITED);
	}
}

template<typename G>
void DFS<G>::DfsTraversal(const VertexPtr vertex)
{
	
}

template<typename G>
inline void DFS<G>::StartVisit(const VertexPtr vertex)
{

}

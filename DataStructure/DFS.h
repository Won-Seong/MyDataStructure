#pragma once
#include "AdjListGraph.h"

template <typename G>
class DFS : std::exception {
protected:
	using VertexPtr = typename G::VertexPtr;
	using VertexList = typename G::VertexList;
	using EdgePtr = typename G::EdgePtr;
	using EdgeList = typename G::EdgeList;
	using IncidentEdgeList = typename G::IncidentEdgeList;
	using IncidentEdgeListPtr = typename G::IncidentEdgeListPtr;
	using STATUS = typename G::STATUS;
protected:
	const G& graph_;
	VertexPtr start_;
protected:
	DFS(const G& graph) :graph_(graph) {};
	void Initialize();//새롭게 DFS 하기 위하여 초기화
	void DfsTraversal(const VertexPtr vertex);//재귀적 DFS 유틸리티
	void StartVisit(const VertexPtr vertex);
	void TraverseDiscovery(const EdgePtr edge, const VertexPtr from_vertex);
	void TraverseBack(const EdgePtr edge, const VertexPtr from_vertex);
	void FinishVisit(const VertexPtr vertex);
	bool IsDone() const { return false; };//끝?
protected:
	void Visit(VertexPtr vertex) { vertex->set_status(STATUS::VISITED); }
	void Visit(EdgePtr edge) { edge->set_status(STATUS::VISITED); }
	void UnVisit(VertexPtr vertex) { vertex->set_status(STATUS::UNVISITED); }
	void UnVisit(EdgePtr edge) { edge->set_status(STATUS::UNVISITED); }
	bool IsVisit(VertexPtr vertex) { return  vertex->get_status() == STATUS::VISITED; }
	bool IsVisit(EdgePtr edge) { return edge->get_status() == STATUS::VISITED; }
};
#include "DFS.hpp"
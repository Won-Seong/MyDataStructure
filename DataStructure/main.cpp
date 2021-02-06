#include <iostream>
#include "AdjListGraph.h"
#include "DFS.h"

int main() {
	AdjListGraph<int, int> graph;

	int alpha, beta, gamma;
	std::cin >> alpha >> beta >> gamma;

	for (int i = 1; i <= alpha; i++)
		graph.InsertVertex(i);

	for (int i = 0; i < beta; i++) {
		int left, right;
		std::cin >> left >> right;
		graph.InsertEdge(graph.SearchVertex(left), graph.SearchVertex(right), int());
	}
	std::vector<int> dfs_list = graph.DfsFunction(graph.SearchVertex(gamma));
	std::vector<int> bfs_list = graph.BfsFunction(graph.SearchVertex(gamma));
	for (const auto& itr : dfs_list)
		std::cout << itr << ' ';
	std::cout << std::endl;
	for (const auto& itr : bfs_list)
		std::cout << itr << ' ';
	std::cout << std::endl;
}
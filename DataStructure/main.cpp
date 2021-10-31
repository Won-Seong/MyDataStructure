#include "AdjListGraph.h"

int main() {
	AdjListGraph<int, int> graph;

	int number;
	std::cin >> number;

	for (int j = 0; j < number; j++) {
		graph.InsertVertex(j + 1);
	}

	std::cin >> number;
	int alpha, beta;

	for (int j = 0; j < 6; j++) {
		std::cin >> alpha >> beta;
		graph.InsertEdge(graph.SearchVertex(alpha), graph.SearchVertex(beta), int());
	}
	std::vector<int> vertices;
	vertices = graph.BfsFunction(graph.SearchVertex(1));

	std::cout << vertices.size() - 1;
}
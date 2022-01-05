#include "IndexTree.h"

int main() {
	std::cin.sync_with_stdio(false);
	std::cin.tie(NULL);

	size_t N, M, K;
	std::cin >> N >> M >> K;

	IndexTree tree(N);
	long temp;
	std::vector<long long> vec;

	for (size_t i = 0; i < N; i++)
	{
		std::cin >> temp;
		vec.push_back(temp);
	}
	
	tree.CreateTree(vec);

	long long A, B, C;

	for (size_t i = 0; i < M + K; i++)
	{
		std::cin >> A >> B >> C;
		if (A == 1) {
			tree.UpdateValue(B,C);
		}
		else {
			std::cout << tree.RangeSum(B, C) << '\n';
		}
	}
}
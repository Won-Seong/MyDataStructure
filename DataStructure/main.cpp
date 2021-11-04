#include "RedBlackTree.h"

int main() {
	RedBlackTree tree;

	int number;
	std::cin >> number;

	char inst{ char() };
	unsigned int id{ unsigned int() };
	std::string name{ std::string() };
	unsigned int capacity{ unsigned int() };
	int price{ int() };
	unsigned int depth{ 0 };

	for (int j = 0; j < number; j++) {
		std::cin >> inst;
		switch (inst)
		{
		case 'I':
			std::cin >> id >> name >> capacity >> price;
			std::cout << tree.InsertNode(Application(id, name, capacity, price)) << std::endl;
			break;
		case 'R':
			std::cin >> id >> name >> capacity >> price;
			tree.UpdateNode(id, name, capacity, price);
			break;
		case 'F':
			std::cin >> id;
			depth = 0;
			tree.SearchNode(id, depth)->Print();
			break;
		case 'D' :
			tree.Debug();
			break;
		default:
			break;
		}
	}
	

	return 0;
}
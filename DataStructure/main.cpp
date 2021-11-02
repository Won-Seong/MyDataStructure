#include "RedBlackTree.h"

int main() {
	
	RedBlackTree tree;
	std::cout << "love ya" << std::endl;
	Application app{ 1,"good",2,3 };

	tree.AddRoot(app);
	std::cout << "love ya" << std::endl;
	tree.SearchNode(100)->Print();


	tree.InsertNode(Application(10, "dd", 4, 5));
	tree.InsertNode(Application(5, "dd", 4, 5));
	tree.InsertNode(Application(134, "dd", 4, 5));
	tree.InsertNode(Application(77, "dd", 4, 5));

	tree.SearchNode(77)->Print();

	return 0;
}
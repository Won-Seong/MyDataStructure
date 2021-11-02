#include "RedBlackTree.h"

int main() {
	
	RedBlackTree tree;
	Application app{ 1,"good",2,3 };
	tree.AddRoot(app);

	tree.SearchNode(1)->Print();
	tree.SearchNode(2)->Print();


	tree.InsertNode(Application(10, "dd", 4, 5));
	tree.InsertNode(Application(5, "dd", 4, 5));
	tree.InsertNode(Application(134, "dd", 4, 5));
	tree.InsertNode(Application(77, "dd", 4, 5));

	tree.SearchNode(77)->Print();
	tree.UpdateNode(77, "whatthe", 100, 200);
	tree.SearchNode(77)->Print();

	return 0;
}
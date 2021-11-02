#include "RedBlackTree.h"

void RedBlackTree::InsertNode(const Application& app)
{
	NodePtr ptr = SearchNode(app.get_id());
	if (!(ptr->IsLeaf())) {
		std::cout << "Already existed!" << std::endl;
		return;
	}
	
	ptr->SetApplication(app);
	size_++;
}

RedBlackTree::NodePtr RedBlackTree::SearchNode(unsigned int id) const
{
	NodePtr ptr = root_;

	while (!(ptr->IsLeaf()) || ptr->get_app().get_id() == id) {
		if (ptr->get_app().get_id() < id)//Should we go down left or right?
			ptr = ptr->right();
		else
			ptr = ptr->left();
	}

	return ptr;
}

void Application::Print() const
{
	std::cout << "ID : " << id_ << std::endl;
	std::cout << "Name : " << id_ << std::endl;
	std::cout << "Capacity : " << id_ << std::endl;
	std::cout << "Price : " << id_ << std::endl;
}

void RedBlackTree::Node::Print() const
{
	app_.Print();
	if (color_ == Color::RED)
		std::cout << "Color : Red!" << std::endl;
	else
		std::cout << "Color : Black!" << std::endl;
}

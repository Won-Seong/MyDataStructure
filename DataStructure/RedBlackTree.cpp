#include "RedBlackTree.h"

void RedBlackTree::AddRoot(const Application& app)
{
	if (size_ > 0) {
		std::cout << "There already is a root!" << std::endl;
		return;
	}
	root_ = std::make_shared<Node>(app); 
	ExpandExternal(root_);
	size_ = 1;
}

void RedBlackTree::InsertNode(const Application& app)
{
	NodePtr ptr = SearchNode(app.get_id());
	if (!(ptr->IsLeaf())) {
		std::cout << "Already existed!" << std::endl;
		return;
	}
	
	ptr->SetApplication(app);
	ExpandExternal(ptr);
	size_++;
}

RedBlackTree::NodePtr RedBlackTree::SearchNode(unsigned int id) const
{
	NodePtr ptr = root_;

	while (!(ptr->IsLeaf()) && ptr->get_app().get_id() != id) {
		if (ptr->get_app().get_id() < id)//Should we go down left or right?
			ptr = ptr->right();
		else
			ptr = ptr->left();
	}
	return ptr;
}

const Application& RedBlackTree::UpdateNode(unsigned int id, std::string name, unsigned int capacity, int price)
{
	NodePtr ptr = SearchNode(id);
	if (ptr->IsLeaf()) {
		std::cout << "There isn't node has the id!" << std::endl;
		return Application();
	}

	ptr->UpdateApplication(name, capacity, price);
	return ptr->get_app();
}

void RedBlackTree::ExpandExternal(NodePtr ptr)
{
	ptr->set_left(std::make_shared<Node>(Application()));
	ptr->set_right(std::make_shared<Node>(Application()));
}

bool Application::Print() const
{
	if (id_ == unsigned int()) {
		std::cout << "This application is empty!\n" << std::endl;
		return false;
	}
	std::cout << "ID : " << id_ << std::endl;
	std::cout << "Name : " << name_ << std::endl;
	std::cout << "Capacity : " << capacity_ << std::endl;
	std::cout << "Price : " << price_ << std::endl;
	return true;
}

void RedBlackTree::Node::Print() const
{
	if (app_.Print()) {
		if (color_ == Color::RED)
			std::cout << "Color : Red\n" << std::endl;
		else
			std::cout << "Color : Black\n" << std::endl;
	}
}

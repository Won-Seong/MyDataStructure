#include "RedBlackTree.h"

void RedBlackTree::AddRoot(const Application& app)
{
	if (size_ > 0) {
		std::cout << "There already is a root!" << std::endl;
		return;
	}
	root_ = std::make_shared<Node>(app);
	root_->ChangeColor();
	ExpandExternal(root_);
	size_ = 1;
}

unsigned int RedBlackTree::InsertNode(const Application& app)
{
	unsigned int depth{ 0 };
	if (size_ == 0) {
		AddRoot(app);
		return depth;
	}
	NodePtr ptr = SearchNode(app.get_id(), depth);

	if (!(ptr->IsLeaf())) {
		std::cout << "Already existed!" << std::endl;
		return depth;
	}

	ptr->SetApplication(app);
	ptr->ChangeColor(Color::RED);
	ExpandExternal(ptr);
	size_++; 
	switch (ptr->IsDoubleRed())
	{
	case 0:
		break;
	case 1:
		while (ptr->IsDoubleRed() == 1) {
			ptr = Recoloring(ptr);
		}
		if (ptr->IsDoubleRed() == 2) {
			Reconstruct(ptr, depth);
		}
		return depth;
		break;
	case 2:
		Reconstruct(ptr, depth);
		break;
	default:
		break;
	}
	return depth;
}

RedBlackTree::NodePtr RedBlackTree::SearchNode(unsigned int id, unsigned int& depth) const
{
	NodePtr ptr = root_;
	
	while (!(ptr->IsLeaf()) && ptr->get_app().get_id() != id) {
		if (ptr->get_app().get_id() < id)//Should we go down left or right?
			ptr = ptr->right();
		else
			ptr = ptr->left();
		
		depth++;
	}
	//std::cout << "DEPTH : " << depth << std::endl;
	return ptr;
}

Application RedBlackTree::UpdateNode(unsigned int id, std::string name, unsigned int capacity, int price)
{
	unsigned int depth{ 0 };
	NodePtr ptr = SearchNode(id, depth);
	if (ptr->IsLeaf()) {
		std::cout << "NULL" << std::endl;
		return Application();
	}

	ptr->UpdateApplication(name, capacity, price);
	std::cout << depth << std::endl;
	return ptr->get_app();
}

void RedBlackTree::Debug() const
{
	NodePtr ptr = root_;
	char temp{ char() };
	ptr->Print();
	while (true) {
		std::cin >> temp;
		switch (temp)
		{
		case 'l':
			ptr = ptr->left();
			break;
		case 'r':
			ptr = ptr->right(); 
			break;
		case 'p':
			ptr = ptr->parent();
			break;
		default:
			continue;
			break;
		}
		ptr->Print();
	}
}

void RedBlackTree::ExpandExternal(NodePtr ptr)
{
	ptr->set_left(std::make_shared<Node>(Application(), ptr));
	ptr->set_right(std::make_shared<Node>(Application(), ptr));
	ptr->left()->ChangeColor(Color::BLACK);
	ptr->right()->ChangeColor(Color::BLACK);
}

RedBlackTree::NodePtr RedBlackTree::Recoloring(NodePtr ptr)
{
	ptr->parent()->ChangeColor();
	ptr->uncle()->ChangeColor();
	if (!(ptr->grandparent()->IsRoot())) ptr->grandparent()->ChangeColor();
	return ptr->grandparent();
}

void RedBlackTree::Reconstruct(NodePtr ptr, unsigned int& depth)
{
	NodePtr old_grandparent = ptr->grandparent();
	NodePtr parent = ptr->parent();
	old_grandparent->ChangeColor();
	//std::cout << "RECON : " << ptr->get_app().get_id() << std::endl;
	if (old_grandparent->left() == parent) {//Case 1, 2: grandparent -> left
		if (parent->left()->get_app().get_id() != ptr->get_app().get_id()) {//Case 1: left -> right
			parent->set_right(ptr->left());
			ptr->left()->set_parent(parent);
			ptr->set_left(parent);
			parent->set_parent(ptr);
			ptr->set_parent(old_grandparent);
			old_grandparent->set_left(ptr);
			ptr = parent;
			depth--;
		}
		ptr->parent()->ChangeColor();
		ptr->parent()->set_parent(old_grandparent->parent());
		if (!(old_grandparent->IsRoot())) {
			if (old_grandparent->parent()->left() == old_grandparent)
				old_grandparent->parent()->set_left(ptr->parent());
			else
				old_grandparent->parent()->set_right(ptr->parent());
		}
		old_grandparent->set_left(ptr->parent()->right());
		ptr->parent()->right()->set_parent(old_grandparent);
		old_grandparent->set_parent(ptr->parent());
		ptr->parent()->set_right(old_grandparent);
	}
	else {//Case 3, 4: grandparent -> right
		if (parent->left()->get_app().get_id() == ptr->get_app().get_id()) {//Case 3: right -> left
			parent->set_left(ptr->right());
			ptr->right()->set_parent(parent);
			ptr->set_right(parent);
			parent->set_parent(ptr);
			ptr->set_parent(old_grandparent);
			old_grandparent->set_right(ptr);
			ptr = parent;
			depth--;
		}
		ptr->parent()->ChangeColor();
		if (!(old_grandparent->IsRoot())) { 
			if(old_grandparent->parent()->left() == old_grandparent)
				old_grandparent->parent()->set_left(ptr->parent()); 
			else
				old_grandparent->parent()->set_right(ptr->parent());
		}
		ptr->parent()->set_parent(old_grandparent->parent());
		old_grandparent->set_right(ptr->parent()->left());
		ptr->parent()->left()->set_parent(old_grandparent);
		old_grandparent->set_parent(ptr->parent());
		ptr->parent()->set_left(old_grandparent);
	}
	if (ptr->parent()->IsRoot()) { 
		ptr->parent()->ChangeColor(Color::BLACK); 
		root_ = ptr->parent();
	}
	depth--;
}

bool Application::Print() const
{
	if (id_ == unsigned int()) {
		std::cout << "NULL" << std::endl;
		return false;
	}
	std::cout << id_ << ' ' << name_ << ' ' << capacity_ << ' ' << price_  << std::endl;
	return true;
}

void RedBlackTree::Node::Print() const
{
	if (!app_.IsEmpty()) {
		app_.Print();
		if (color_ == Color::RED)
			std::cout << "Red\n" << std::endl;
		else
			std::cout << "Black\n" << std::endl;
	}
	else
		std::cout << "There isn't such node\n" << std::endl;
}

int RedBlackTree::Node::IsDoubleRed() const {

	if (color_ == Color::RED && parent_->get_color() == Color::RED)
		return uncle()->get_color() == Color::RED ? 1 : 2;
	else
		return 0;
}

#include "RedBlackTree.h"

void RedBlackTree::AddRoot(const Application& app)
{
	if (size_ > 0) {
		std::cout << "There already is a root!" << std::endl;
		return;
	}//ũ�Ⱑ 0���� ũ�ٸ� �̹� root�� �����Ѵٴ� �̾߱��̹Ƿ�.
	root_ = std::make_shared<Node>(app);
	root_->ChangeColor(Color::BLACK);//root�� ���� ������ Black�̹Ƿ�.
	ExpandExternal(root_);
	size_ = 1;
}

unsigned int RedBlackTree::InsertNode(const Application& app)
{
	unsigned int depth{ 0 };
	if (size_ == 0) {
		AddRoot(app);
		return depth;
	}//��尡 ���ٸ� ��Ʈ�� �����Ѵ�. 
	NodePtr ptr = SearchNode(app.get_id(), depth);//��带 �����ϱ� ���� �� ��带 Ž���Ѵ�.

	if (!(ptr->IsLeaf())) {
		std::cout << "Already existed!" << std::endl;
		return depth;
	}//���� Ž���� ��尡 ������� �ʴٸ� ���� id�� ���� ��尡 �ִٴ� ����̹Ƿ� �ƹ��͵� ���� �ʴ´�.

	ptr->SetApplication(app);//�� ��忡 Application�� �����Ѵ�.
	ptr->ChangeColor(Color::RED);//���Ե� ���� Red�̹Ƿ�.
	ExpandExternal(ptr);
	size_++; 

	switch (ptr->IsDoubleRed())
	{
	case 0://DoubleRed�� ����.
		break;
	case 1://DoubleRed�� �����鼭 Uncle�� Red�̴�.
		while (ptr->IsDoubleRed() == 1) {
			ptr = Recoloring(ptr);
		}//�� �̻� �׷� ��Ȳ�� �߻����� ���� ������ Recoloring.
		if (ptr->IsDoubleRed() == 2) {
			Reconstruct(ptr, depth);
		}//Recoloring �� Reconstruct�� ��Ȳ�� �߻��ϸ� �����Ѵ�. �̴� ���ĵ��� �����Ƿ� 1ȸ�� �����Ѵ�.
		return depth;
		break;
	case 2://DoubleRed�� �����鼭, Uncle�� Black�̴�.
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
		if (ptr->get_app().get_id() < id)//�������� ���� �ϴ°�, �ƴϸ� ���������� ���� �ϴ°�?
			ptr = ptr->right();
		else
			ptr = ptr->left();
		depth++;
	}
	//std::cout << depth << ' ';
	return ptr;
}

Application RedBlackTree::UpdateNode(unsigned int id, std::string name, unsigned int capacity, int price)
{
	unsigned int depth{ 0 };
	NodePtr ptr = SearchNode(id, depth);//������ ��带 Ž���Ѵ�.
	if (ptr->IsLeaf()) {
		std::cout << "NULL" << std::endl;
		return Application();
	}//Ž���� ��尡 ����ٸ� �̴� �������� �ʴ� id��� ���̹Ƿ� �ƹ��͵� ���� �ʰ� �� Application�� ��ȯ�Ѵ�.

	ptr->UpdateApplication(name, capacity, price);//������ ��尡 �����Ѵٸ� id�� ������ ������ �����͸� �����Ѵ�.
	std::cout << depth << std::endl;
	return ptr->get_app();
}

void RedBlackTree::Discount(unsigned int x, unsigned int y, int percent)
{
	NodePtrList list = InOrder();

	for (const auto& itr : list) {
		if (itr->get_app().get_id() < x) continue;
		else if (itr->get_app().get_id() > y) continue;
		itr->DiscountPrice(percent);
	}
}

RedBlackTree::NodePtrList RedBlackTree::InOrder() const
{
	NodePtrList list;
	InOrderUtility(root_, list);
	//for (const auto& itr : list)
	//	std::cout << itr->get_app().get_id() << std::endl;
	return list;
}//InOrder ����, ������������ ���ĵ� �迭�� ��ȯ�Ѵ�.

void RedBlackTree::Debug() const
{
	NodePtr ptr{root_};
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
}//����׸� ���� �Լ�.

unsigned int RedBlackTree::get_depth(NodePtr ptr) const
{
	unsigned int depth{ 0 };
	SearchNode(ptr->get_app().get_id(), depth);
	return depth;
}

void RedBlackTree::ExpandExternal(NodePtr ptr)
{
	//�� �ڽ��� �� Application�� ���� ���� �����.
	ptr->set_left(std::make_shared<Node>(Application(), ptr));
	ptr->set_right(std::make_shared<Node>(Application(), ptr));
	//��� leaf�� Black�̹Ƿ� ���� �ٲ۴�.
	ptr->left()->ChangeColor(Color::BLACK);
	ptr->right()->ChangeColor(Color::BLACK);
}

RedBlackTree::NodePtr RedBlackTree::Recoloring(NodePtr ptr)
{
	//�θ�� ������ ���� �ٲ۴�.
	ptr->parent()->ChangeColor();
	ptr->uncle()->ChangeColor();
	if (!(ptr->grandparent()->IsRoot())) ptr->grandparent()->ChangeColor();//root��� Black�� �����ؾ� �ϹǷ� �ٲ��� �ʴ´�.
	return ptr->grandparent();
}

void RedBlackTree::Reconstruct(NodePtr ptr, unsigned int& depth)
{
	NodePtr old_grandparent = ptr->grandparent();
	NodePtr parent = ptr->parent();
	old_grandparent->ChangeColor();//��� ��Ȳ���� grandaprent�� ���� �ٲ�����Ƿ�.
	//std::cout << "RECON : " << ptr->get_app().get_id() << std::endl;

	if (old_grandparent->left() == parent) {//Case 1, 2: grandparent -> left(grandparent�� ���� �ڽ��� Red)
		if (parent->left()->get_app().get_id() != ptr->get_app().get_id()) {//Case 1: left -> right(���� ���� ���)
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
	else {//Case 3, 4: grandparent -> right(grandparent�� ������ �ڽ��� Red)
		if (parent->left()->get_app().get_id() == ptr->get_app().get_id()) {//Case 3: right -> left(���� ���� ���)
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

void RedBlackTree::InOrderUtility(NodePtr ptr, NodePtrList& list) const
{
	if (!ptr->left()->IsLeaf())
		InOrderUtility(ptr->left(), list);
	list.push_back(ptr);
	if (!ptr->right()->IsLeaf())
		InOrderUtility(ptr->right(), list);
}//InOrder�� ��������� �����ϱ� ���� ���� �Լ�

bool Application::Print() const
{
	if (id_ == unsigned int()) {
		std::cout << "NULL" << std::endl;
		return false;
	}//����ִٸ� NULL�� ����Ѵ�. 
	//std::cout << id_ << ' ';
	std::cout << name_ << ' ' << capacity_ << ' ' << price_  << std::endl;
	return true;
}

void RedBlackTree::Node::Print() const
{
	if (!app_.IsEmpty()) {
		app_.Print();
		/*if (color_ == Color::RED)
			std::cout << "Red\n" << std::endl;
		else
			std::cout << "Black\n" << std::endl;*/
	}
	else
		std::cout << "There isn't such node\n" << std::endl;
}

int RedBlackTree::Node::IsDoubleRed() const {

	if (color_ == Color::RED && parent_->get_color() == Color::RED)//� ���� �� ����� �θ� ���ÿ� Red�ΰ�?
		return uncle()->get_color() == Color::RED ? 1 : 2;//���� �׷��ٸ� ������ ����?
	else
		return 0;
}

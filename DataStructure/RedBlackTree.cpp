#include "RedBlackTree.h"

void RedBlackTree::AddRoot(const Application& app)
{
	if (size_ > 0) {
		std::cout << "There already is a root!" << std::endl;
		return;
	}//크기가 0보다 크다면 이미 root가 존재한다는 이야기이므로.
	root_ = std::make_shared<Node>(app);
	root_->ChangeColor(Color::BLACK);//root의 색은 무조건 Black이므로.
	ExpandExternal(root_);
	size_ = 1;
}

unsigned int RedBlackTree::InsertNode(const Application& app)
{
	unsigned int depth{ 0 };
	if (size_ == 0) {
		AddRoot(app);
		return depth;
	}//노드가 없다면 루트를 생성한다. 
	NodePtr ptr = SearchNode(app.get_id(), depth);//노드를 삽입하기 위해 빈 노드를 탐색한다.

	if (!(ptr->IsLeaf())) {
		std::cout << "Already existed!" << std::endl;
		return depth;
	}//만약 탐색한 노드가 비어있지 않다면 같은 id를 갖는 노드가 있다는 얘기이므로 아무것도 하지 않는다.

	ptr->SetApplication(app);//빈 노드에 Application을 삽입한다.
	ptr->ChangeColor(Color::RED);//삽입된 노드는 Red이므로.
	ExpandExternal(ptr);
	size_++; 

	switch (ptr->IsDoubleRed())
	{
	case 0://DoubleRed가 없다.
		break;
	case 1://DoubleRed가 있으면서 Uncle이 Red이다.
		while (ptr->IsDoubleRed() == 1) {
			ptr = Recoloring(ptr);
		}//더 이상 그런 상황이 발생하지 않을 때까지 Recoloring.
		if (ptr->IsDoubleRed() == 2) {
			Reconstruct(ptr, depth);
		}//Recoloring 후 Reconstruct할 상황이 발생하면 수행한다. 이는 전파되지 않으므로 1회로 종료한다.
		return depth;
		break;
	case 2://DoubleRed가 있으면서, Uncle이 Black이다.
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
		if (ptr->get_app().get_id() < id)//왼쪽으로 가야 하는가, 아니면 오른쪽으로 가야 하는가?
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
	NodePtr ptr = SearchNode(id, depth);//갱신할 노드를 탐색한다.
	if (ptr->IsLeaf()) {
		std::cout << "NULL" << std::endl;
		return Application();
	}//탐색한 노드가 비었다면 이는 존재하지 않는 id라는 뜻이므로 아무것도 하지 않고 빈 Application을 반환한다.

	ptr->UpdateApplication(name, capacity, price);//갱신할 노드가 존재한다면 id를 제외한 나머지 데이터를 갱신한다.
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
}//InOrder 수행, 오름차순으로 정렬된 배열을 반환한다.

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
}//디버그를 위한 함수.

unsigned int RedBlackTree::get_depth(NodePtr ptr) const
{
	unsigned int depth{ 0 };
	SearchNode(ptr->get_app().get_id(), depth);
	return depth;
}

void RedBlackTree::ExpandExternal(NodePtr ptr)
{
	//각 자식을 빈 Application을 가진 노드로 만든다.
	ptr->set_left(std::make_shared<Node>(Application(), ptr));
	ptr->set_right(std::make_shared<Node>(Application(), ptr));
	//모든 leaf는 Black이므로 색을 바꾼다.
	ptr->left()->ChangeColor(Color::BLACK);
	ptr->right()->ChangeColor(Color::BLACK);
}

RedBlackTree::NodePtr RedBlackTree::Recoloring(NodePtr ptr)
{
	//부모와 사촌의 색을 바꾼다.
	ptr->parent()->ChangeColor();
	ptr->uncle()->ChangeColor();
	if (!(ptr->grandparent()->IsRoot())) ptr->grandparent()->ChangeColor();//root라면 Black을 유지해야 하므로 바꾸지 않는다.
	return ptr->grandparent();
}

void RedBlackTree::Reconstruct(NodePtr ptr, unsigned int& depth)
{
	NodePtr old_grandparent = ptr->grandparent();
	NodePtr parent = ptr->parent();
	old_grandparent->ChangeColor();//모든 상황에서 grandaprent의 색은 바뀌어지므로.
	//std::cout << "RECON : " << ptr->get_app().get_id() << std::endl;

	if (old_grandparent->left() == parent) {//Case 1, 2: grandparent -> left(grandparent의 왼쪽 자식이 Red)
		if (parent->left()->get_app().get_id() != ptr->get_app().get_id()) {//Case 1: left -> right(꺾여 있을 경우)
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
	else {//Case 3, 4: grandparent -> right(grandparent의 오른쪽 자식이 Red)
		if (parent->left()->get_app().get_id() == ptr->get_app().get_id()) {//Case 3: right -> left(꺾여 있을 경우)
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
}//InOrder를 재귀적으로 수행하기 위한 내부 함수

bool Application::Print() const
{
	if (id_ == unsigned int()) {
		std::cout << "NULL" << std::endl;
		return false;
	}//비어있다면 NULL을 출력한다. 
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

	if (color_ == Color::RED && parent_->get_color() == Color::RED)//어떤 노드와 그 노드의 부모가 동시에 Red인가?
		return uncle()->get_color() == Color::RED ? 1 : 2;//만약 그렇다면 사촌의 색은?
	else
		return 0;
}

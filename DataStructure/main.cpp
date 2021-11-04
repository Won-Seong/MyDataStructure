//��ǻ�� ���а� 12141508 �輺��
#include <iostream>
#include <string>
#include <vector>

class Application {//Application Ŭ����
private:
	unsigned int id_;//[1000000 , 1999999]
	std::string name_;
	unsigned int capacity_;//[1 , 10000] , Unit : MB
	int price_;//[0, 1000] , Unit : 1000
public://������ �� ���� ������
	Application() :id_(int()), name_(std::string()), capacity_(int()), price_(int()) {};//�� Application ������
	Application(const unsigned int& id, const std::string& name, const unsigned int& capacity, const int& price) :
		id_(id), name_(name), capacity_(capacity), price_(price) {};
	Application(const Application& app) :
		id_(app.id_), name_(app.name_), capacity_(app.capacity_), price_(app.price_) {};
	Application& operator=(Application&& app) noexcept {
		id_ = app.id_;
		name_ = app.name_;
		capacity_ = app.capacity_;
		price_ = app.price_;
		app.id_ = int();
		app.name_ = std::string();
		app.capacity_ = int();
		app.price_ = int();
		return *this;
	}
	Application& operator=(const Application& app) {
		id_ = app.id_;
		name_ = app.name_;
		capacity_ = app.capacity_;
		price_ = app.price_;
		return *this;
	}
	void set_name(const std::string& name) { name_ = name; }
	void set_capacity(const unsigned int& capacity) { capacity_ = capacity; }
	void set_price(const int& price) { price_ = price; }
	const unsigned int& get_id() const { return id_; }
	std::string get_name() const { return name_; }
	unsigned int get_capacity() const { return capacity_; }
	int get_price() const { return price_; }
	bool IsEmpty() const { return id_ == int(); }//id�� unsigned int�� �⺻���̶�� �� Application���� �����Ѵ�.
	bool Print() const;//Application�� ������ ����Ѵ�.
};

class RedBlackTree {
private:
	enum class Color {
		RED, BLACK
	};//����� �� ����
	struct Node;
public:
	using NodePtr = Node*;//Node�� ����Ű�� ������
	using NodePtrList = std::vector<NodePtr>;//NodePtr�� �迭, ��ȸ�� �� �̿�
private:
	struct Node {
	private:
		Application app_;//�� ���� Application�� ����, id�� ��Ҹ� ���Ѵ�.
		NodePtr left_child_;
		NodePtr right_child_;
		NodePtr parent_;
		Color color_;
	public:
		Node(const int& id, const std::string& name, const unsigned int& capacity, const int& price, NodePtr parent = nullptr) :
			app_(id, name, capacity, price), left_child_(nullptr), right_child_(nullptr), parent_(parent), color_(Color::RED) {};
		Node(const Application& app, NodePtr parent = nullptr) :
			app_(app), left_child_(nullptr), right_child_(nullptr), parent_(parent), color_(Color::RED) {};
		//���� �����ڳ� ���� ������ ���� �������� ������ ���� ���� ����
		Node(const Node&) = delete;
		Node(Node&&) = delete;
		Node& operator=(const Node&) = delete;
		Node& operator=(Node&&) = delete;
		~Node() {};
	public:
		const Application& get_app() const { return app_; };
		bool IsLeaf() const { return app_.IsEmpty(); }//Application�� ������� leaf�� ����Ѵ�. 
		bool IsRoot() const { return parent_ == nullptr; }//�θ� ��尡 ���ٸ� root.
		NodePtr left() const { return left_child_; }
		NodePtr right() const { return right_child_; }
		NodePtr parent() const { return parent_; }
		NodePtr grandparent() const { return parent_->parent(); }
		NodePtr uncle() const {
			return grandparent()->left() == parent_ ? grandparent()->right() : grandparent()->left();
		}//Uncle�� Grandparent�� �ڽ� �� �θ� �ƴ� �ڽ�
		void set_left(NodePtr ptr) { left_child_ = ptr; }
		void set_right(NodePtr ptr) { right_child_ = ptr; }
		void set_parent(NodePtr ptr) { parent_ = ptr; }
		Color get_color() const { return color_; }
		void SetApplication(const Application& app) {
			app_ = app;
		}
		void UpdateApplication(std::string name, unsigned int capacity, int price) { app_.set_name(name); app_.set_capacity(capacity); app_.set_price(price); }
		//id�� ������ Application�� ������ �����Ѵ�.
		int DiscountPrice(double percent) {
			double temp{ (100 - percent) / 100 * app_.get_price() };//������ ���
			app_.set_price(temp); 
			return app_.get_price(); 
		}
		void Print() const;//����� ������ ����Ѵ�.
		int IsDoubleRed() const;//�� ����� �θ�� �� ��尡 �� �� Red�ΰ�?
		//return : 0(No double red) | 1(Double red and needs recoloring) | 2(Double red and needs reconstruct)
		Color ChangeColor() { if (color_ == Color::RED) color_ = Color::BLACK; else color_ = Color::RED; return color_; };//Red��� Black, Black�̶�� Red��
		Color ChangeColor(Color color) { color_ = color; return color_; };//���� �����Ͽ� �ٲ۴�.
	};
public:
	RedBlackTree() : size_(0), root_(nullptr) {};
public:
	unsigned int get_size() const { return size_; }
	bool IsEmpty() const { return size_ == 0; };
	void AddRoot(const Application& app);//��Ʈ�� �����ϴ� �Լ�
	unsigned int InsertNode(const Application& app);//��带 �����ϴ� �Լ�
	NodePtr SearchNode(unsigned int id, unsigned int& depth) const;//��带 Ž���ϴ� �Լ�
	Application UpdateNode(unsigned int id, std::string name, unsigned int capacity, int price);//��带 �����ϴ� �Լ�
	void Discount(unsigned int x, unsigned int y, int percent);//id ���� ���� ����� Price�� �����ϴ� �Լ�
	NodePtrList InOrder() const;//InOrder �Լ�
public:
	void Debug() const;//������� ���� ���� �Լ�. 
	unsigned int get_depth(NodePtr ptr) const;//����� ���̸� ��ȯ�Ѵ�.
private://���� �Լ�
	void ExpandExternal(NodePtr ptr);//leaf ����� �ڽĵ��� nullptr�� �ƴ� �� Application�� ��� �ִ� ���� ����� ���� �Լ�.
	NodePtr Recoloring(NodePtr ptr);//Recolor nodes
	void Reconstruct(NodePtr ptr, unsigned int& depth);//Reconstruct a node
	void InOrderUtility(NodePtr ptr, NodePtrList& list) const;//InOrder�� ���� ���� �Լ�. ��������� ȣ��.
private:
	NodePtr root_;
	unsigned int size_;
};

void RedBlackTree::AddRoot(const Application& app)
{
	if (size_ > 0) {
		//std::cout << "There already is a root!" << std::endl;
		return;
	}//ũ�Ⱑ 0���� ũ�ٸ� �̹� root�� �����Ѵٴ� �̾߱��̹Ƿ�.
	root_ = new Node(app);
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
		//std::cout << "Already existed!" << std::endl;
		return depth;
	}//���� Ž���� ��尡 ������� �ʴٸ� ���� id�� ���� ��尡 �ִٴ� ����̹Ƿ� �ƹ��͵� ���� �ʴ´�.

	ptr->SetApplication(app);//�� ��忡 Application�� �����Ѵ�.
	ptr->ChangeColor(Color::RED);//���Ե� ���� Red�̹Ƿ�.
	ExpandExternal(ptr);
	size_++;
	unsigned int temp{ depth };//Recoloring �� Reconstruct�� �Ͼ �� ���� ����� ���� �ӽ� ����.

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
			temp--;//Recoloring �� Reconstruct�� ���̴� 1�� ��������.
		}//Recoloring �� Reconstruct�� ��Ȳ�� �߻��ϸ� �����Ѵ�. �̴� ���ĵ��� �����Ƿ� 1ȸ�� �����Ѵ�.
		return temp;
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
			ptr = ptr->right();//����������
		else
			ptr = ptr->left();//��������
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
	NodePtrList list = InOrder();//���� ��ȸ�Ͽ� ����� ���� �迭. ������������ ���ĵǾ� �ִ�.

	for (const auto& itr : list) {
		if (itr->get_app().get_id() < x) continue;
		else if (itr->get_app().get_id() > y) break;
		itr->DiscountPrice(percent);
	}
}

RedBlackTree::NodePtrList RedBlackTree::InOrder() const
{
	NodePtrList list;
	InOrderUtility(root_, list);
	/*for (const auto& itr : list)
		std::cout << itr->get_app().get_id() << std::endl;*/
	return list;
}//InOrder ����, ������������ ���ĵ� �迭�� ��ȯ�Ѵ�.

void RedBlackTree::Debug() const
{
	NodePtr ptr{ root_ };
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
	ptr->set_left(new Node(Application(), ptr));
	ptr->set_right(new Node(Application(), ptr));
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
	return ptr->grandparent();//���� Recoloring ���� Ȯ���� ���� grandparent ��ȯ
}

void RedBlackTree::Reconstruct(NodePtr ptr, unsigned int& depth)
{
	//Reconstruct �������� ���谡 ���� �ٲ�� ������ �̸� ������ ���´�.
	NodePtr old_grandparent = ptr->grandparent();
	NodePtr parent = ptr->parent();
	old_grandparent->ChangeColor();//��� ��Ȳ���� grandaprent�� ���� �ٲ�����Ƿ� �̸� �ٲپ� ���´�.
	//std::cout << "RECON : " << ptr->get_app().get_id() << std::endl;

	if (old_grandparent->left() == parent) {//Case 1, 2: grandparent -> left(grandparent�� ���� �ڽ��� Red)
		if (parent->left()->get_app().get_id() != ptr->get_app().get_id()) {//Case 1: left -> right(���� ���� ���)
			//�θ�� �ڽ��� ���踦 �ٲ۴�. �� ��, �Ϸķ� ���ĵǵ��� �����.
			parent->set_right(ptr->left());
			ptr->left()->set_parent(parent);
			ptr->set_left(parent);
			parent->set_parent(ptr);
			ptr->set_parent(old_grandparent);
			old_grandparent->set_left(ptr);
			ptr = parent;
			depth--;
		}
		//�Ϸķ� ���ĵǾ��� ���, ���θ� �θ��� �ڽ��� �ǵ��� �����.
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
			//�θ�� �ڽ��� ���踦 �ٲ۴�. �� ��, �Ϸķ� ���ĵǵ��� �����.
			parent->set_left(ptr->right());
			ptr->right()->set_parent(parent);
			ptr->set_right(parent);
			parent->set_parent(ptr);
			ptr->set_parent(old_grandparent);
			old_grandparent->set_right(ptr);
			ptr = parent;
			depth--;
		}
		//�Ϸķ� ���ĵǾ��� ���, ���θ� �θ��� �ڽ��� �ǵ��� �����.
		ptr->parent()->ChangeColor();
		ptr->parent()->set_parent(old_grandparent->parent());
		if (!(old_grandparent->IsRoot())) {
			if (old_grandparent->parent()->left() == old_grandparent)
				old_grandparent->parent()->set_left(ptr->parent());
			else
				old_grandparent->parent()->set_right(ptr->parent());
		}
		old_grandparent->set_right(ptr->parent()->left());
		ptr->parent()->left()->set_parent(old_grandparent);
		old_grandparent->set_parent(ptr->parent());
		ptr->parent()->set_left(old_grandparent);
	}
	if (ptr->parent()->IsRoot()) {//���� ��Ʈ�� �ٲ���� ��� ��Ʈ�� �����Ѵ�.
		ptr->parent()->ChangeColor(Color::BLACK);
		ptr->parent()->set_parent(nullptr);
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
	if (id_ == int()) {
		std::cout << "NULL" << std::endl;
		return false;
	}//����ִٸ� NULL�� ����Ѵ�. 
	//std::cout << id_ << ' ';
	std::cout << name_ << ' ' << capacity_ << ' ' << price_ << std::endl;
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
		std::cout << "NULL" << std::endl;
}

int RedBlackTree::Node::IsDoubleRed() const {

	if (color_ == Color::RED && parent_->get_color() == Color::RED)//� ���� �� ����� �θ� ���ÿ� Red�ΰ�?
		return uncle()->get_color() == Color::RED ? 1 : 2;//���� �׷��ٸ� ������ ����?
	else
		return 0;
}

int main() {
	RedBlackTree tree;

	int number;
	std::cin >> number;

	//��ɾ� �� ���ڵ��� �ްų� ������ ����� �����ϱ� ���� �ӽ� ������
	char inst{ char() };
	unsigned int id{ int() };
	std::string name{ std::string() };
	unsigned int capacity{ int() };
	int price{ int() };
	unsigned int depth{ 0 };
	unsigned int x{ 0 };
	unsigned int y{ 0 };
	int percent{ 0 };
	RedBlackTree::NodePtr ptr;

	for (int j = 0; j < number; j++) {
		std::cin >> inst;
		switch (inst)
		{
		case 'I':
			std::cin >> id >> name >> capacity >> price;
			std::cout << tree.InsertNode(Application(id, name , capacity, price)) << std::endl;
			break;
		case 'R':
			std::cin >> id >> name >> capacity >> price;
			tree.UpdateNode(id, name, capacity, price);
			break;
		case 'F':
			std::cin >> id;
			depth = 0;
			ptr = tree.SearchNode(id, depth);
			if (!ptr->IsLeaf())//leaf��� ã�� ��尡 ���ٴ� ����̹Ƿ� ���̸� ������� �ʴ´�.
				std::cout << depth << ' ';
			ptr->Print();
			break;
		case 'D':
			std::cin >> x >> y >> percent;
			tree.Discount(x, y, percent);
			break;
		/*case 'B' :
			tree.Debug();
			break;
		case 'O':
			tree.InOrder();
			break;*/
		default:
			break;
		}
	}
	return 0;
}
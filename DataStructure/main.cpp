//컴퓨터 공학과 12141508 김성원
#include <iostream>
#include <string>
#include <vector>

class Application {//Application 클래스
private:
	unsigned int id_;//[1000000 , 1999999]
	std::string name_;
	unsigned int capacity_;//[1 , 10000] , Unit : MB
	int price_;//[0, 1000] , Unit : 1000
public://생성자 및 대입 연산자
	Application() :id_(int()), name_(std::string()), capacity_(int()), price_(int()) {};//빈 Application 생성자
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
	bool IsEmpty() const { return id_ == int(); }//id가 unsigned int의 기본값이라면 빈 Application으로 간주한다.
	bool Print() const;//Application의 정보를 출력한다.
};

class RedBlackTree {
private:
	enum class Color {
		RED, BLACK
	};//노드의 색 정보
	struct Node;
public:
	using NodePtr = Node*;//Node를 가리키는 포인터
	using NodePtrList = std::vector<NodePtr>;//NodePtr의 배열, 순회할 때 이용
private:
	struct Node {
	private:
		Application app_;//각 노드는 Application을 갖고, id로 대소를 비교한다.
		NodePtr left_child_;
		NodePtr right_child_;
		NodePtr parent_;
		Color color_;
	public:
		Node(const int& id, const std::string& name, const unsigned int& capacity, const int& price, NodePtr parent = nullptr) :
			app_(id, name, capacity, price), left_child_(nullptr), right_child_(nullptr), parent_(parent), color_(Color::RED) {};
		Node(const Application& app, NodePtr parent = nullptr) :
			app_(app), left_child_(nullptr), right_child_(nullptr), parent_(parent), color_(Color::RED) {};
		//복사 생성자나 대입 연산자 등은 포인터의 꼬임을 막기 위해 삭제
		Node(const Node&) = delete;
		Node(Node&&) = delete;
		Node& operator=(const Node&) = delete;
		Node& operator=(Node&&) = delete;
		~Node() {};
	public:
		const Application& get_app() const { return app_; };
		bool IsLeaf() const { return app_.IsEmpty(); }//Application이 비었으면 leaf로 취급한다. 
		bool IsRoot() const { return parent_ == nullptr; }//부모 노드가 없다면 root.
		NodePtr left() const { return left_child_; }
		NodePtr right() const { return right_child_; }
		NodePtr parent() const { return parent_; }
		NodePtr grandparent() const { return parent_->parent(); }
		NodePtr uncle() const {
			return grandparent()->left() == parent_ ? grandparent()->right() : grandparent()->left();
		}//Uncle은 Grandparent의 자식 중 부모가 아닌 자식
		void set_left(NodePtr ptr) { left_child_ = ptr; }
		void set_right(NodePtr ptr) { right_child_ = ptr; }
		void set_parent(NodePtr ptr) { parent_ = ptr; }
		Color get_color() const { return color_; }
		void SetApplication(const Application& app) {
			app_ = app;
		}
		void UpdateApplication(std::string name, unsigned int capacity, int price) { app_.set_name(name); app_.set_capacity(capacity); app_.set_price(price); }
		//id를 제외한 Application의 정보를 변경한다.
		int DiscountPrice(double percent) {
			double temp{ (100 - percent) / 100 * app_.get_price() };//할인율 계산
			app_.set_price(temp); 
			return app_.get_price(); 
		}
		void Print() const;//노드의 정보를 출력한다.
		int IsDoubleRed() const;//이 노드의 부모와 이 노드가 둘 다 Red인가?
		//return : 0(No double red) | 1(Double red and needs recoloring) | 2(Double red and needs reconstruct)
		Color ChangeColor() { if (color_ == Color::RED) color_ = Color::BLACK; else color_ = Color::RED; return color_; };//Red라면 Black, Black이라면 Red로
		Color ChangeColor(Color color) { color_ = color; return color_; };//색을 지정하여 바꾼다.
	};
public:
	RedBlackTree() : size_(0), root_(nullptr) {};
public:
	unsigned int get_size() const { return size_; }
	bool IsEmpty() const { return size_ == 0; };
	void AddRoot(const Application& app);//루트를 생성하는 함수
	unsigned int InsertNode(const Application& app);//노드를 삽입하는 함수
	NodePtr SearchNode(unsigned int id, unsigned int& depth) const;//노드를 탐색하는 함수
	Application UpdateNode(unsigned int id, std::string name, unsigned int capacity, int price);//노드를 갱신하는 함수
	void Discount(unsigned int x, unsigned int y, int percent);//id 범위 내의 노드의 Price를 할인하는 함수
	NodePtrList InOrder() const;//InOrder 함수
public:
	void Debug() const;//디버깅을 위한 편의 함수. 
	unsigned int get_depth(NodePtr ptr) const;//노드의 깊이를 반환한다.
private://내부 함수
	void ExpandExternal(NodePtr ptr);//leaf 노드의 자식들을 nullptr이 아닌 빈 Application을 담고 있는 노드로 만드는 편의 함수.
	NodePtr Recoloring(NodePtr ptr);//Recolor nodes
	void Reconstruct(NodePtr ptr, unsigned int& depth);//Reconstruct a node
	void InOrderUtility(NodePtr ptr, NodePtrList& list) const;//InOrder를 위한 편의 함수. 재귀적으로 호출.
private:
	NodePtr root_;
	unsigned int size_;
};

void RedBlackTree::AddRoot(const Application& app)
{
	if (size_ > 0) {
		//std::cout << "There already is a root!" << std::endl;
		return;
	}//크기가 0보다 크다면 이미 root가 존재한다는 이야기이므로.
	root_ = new Node(app);
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
		//std::cout << "Already existed!" << std::endl;
		return depth;
	}//만약 탐색한 노드가 비어있지 않다면 같은 id를 갖는 노드가 있다는 얘기이므로 아무것도 하지 않는다.

	ptr->SetApplication(app);//빈 노드에 Application을 삽입한다.
	ptr->ChangeColor(Color::RED);//삽입된 노드는 Red이므로.
	ExpandExternal(ptr);
	size_++;
	unsigned int temp{ depth };//Recoloring 후 Reconstruct가 일어날 시 깊이 계산을 위한 임시 변수.

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
			temp--;//Recoloring 후 Reconstruct시 깊이는 1이 감해진다.
		}//Recoloring 후 Reconstruct할 상황이 발생하면 수행한다. 이는 전파되지 않으므로 1회로 종료한다.
		return temp;
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
			ptr = ptr->right();//오른쪽으로
		else
			ptr = ptr->left();//왼쪽으로
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
	NodePtrList list = InOrder();//중위 순회하여 결과로 나온 배열. 오름차순으로 정렬되어 있다.

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
}//InOrder 수행, 오름차순으로 정렬된 배열을 반환한다.

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
	ptr->set_left(new Node(Application(), ptr));
	ptr->set_right(new Node(Application(), ptr));
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
	return ptr->grandparent();//다음 Recoloring 전파 확인을 위해 grandparent 반환
}

void RedBlackTree::Reconstruct(NodePtr ptr, unsigned int& depth)
{
	//Reconstruct 과정에서 관계가 많이 바뀌기 때문에 미리 저장해 놓는다.
	NodePtr old_grandparent = ptr->grandparent();
	NodePtr parent = ptr->parent();
	old_grandparent->ChangeColor();//모든 상황에서 grandaprent의 색은 바뀌어지므로 미리 바꾸어 놓는다.
	//std::cout << "RECON : " << ptr->get_app().get_id() << std::endl;

	if (old_grandparent->left() == parent) {//Case 1, 2: grandparent -> left(grandparent의 왼쪽 자식이 Red)
		if (parent->left()->get_app().get_id() != ptr->get_app().get_id()) {//Case 1: left -> right(꺾여 있을 경우)
			//부모와 자식의 관계를 바꾼다. 이 때, 일렬로 정렬되도록 만든다.
			parent->set_right(ptr->left());
			ptr->left()->set_parent(parent);
			ptr->set_left(parent);
			parent->set_parent(ptr);
			ptr->set_parent(old_grandparent);
			old_grandparent->set_left(ptr);
			ptr = parent;
			depth--;
		}
		//일렬로 정렬되었을 경우, 조부모가 부모의 자식이 되도록 만든다.
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
			//부모와 자식의 관계를 바꾼다. 이 때, 일렬로 정렬되도록 만든다.
			parent->set_left(ptr->right());
			ptr->right()->set_parent(parent);
			ptr->set_right(parent);
			parent->set_parent(ptr);
			ptr->set_parent(old_grandparent);
			old_grandparent->set_right(ptr);
			ptr = parent;
			depth--;
		}
		//일렬로 정렬되었을 경우, 조부모가 부모의 자식이 되도록 만든다.
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
	if (ptr->parent()->IsRoot()) {//만약 루트가 바뀌었을 경우 루트를 갱신한다.
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
}//InOrder를 재귀적으로 수행하기 위한 내부 함수

bool Application::Print() const
{
	if (id_ == int()) {
		std::cout << "NULL" << std::endl;
		return false;
	}//비어있다면 NULL을 출력한다. 
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

	if (color_ == Color::RED && parent_->get_color() == Color::RED)//어떤 노드와 그 노드의 부모가 동시에 Red인가?
		return uncle()->get_color() == Color::RED ? 1 : 2;//만약 그렇다면 사촌의 색은?
	else
		return 0;
}

int main() {
	RedBlackTree tree;

	int number;
	std::cin >> number;

	//명령어 후 인자들을 받거나 과정의 결과를 저장하기 위한 임시 변수들
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
			if (!ptr->IsLeaf())//leaf라면 찾는 노드가 없다는 얘기이므로 깊이를 출력하지 않는다.
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
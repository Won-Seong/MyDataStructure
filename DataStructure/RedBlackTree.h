#pragma once
#include <iostream>
#include <vector>

class Application {//Application 클래스
private:
	unsigned int id_;//[1000000 , 1999999]
	std::string name_;
	unsigned int capacity_;//[1 , 10000] , Unit : MB
	int price_;//[0, 1000] , Unit : 1000
public:
	Application() :id_(unsigned int()), name_(std::string()), capacity_(unsigned int()), price_(int()) {};//빈 Application 생성자
	Application(const unsigned int& id, std::string_view name, const unsigned int& capacity, const int& price) :
		id_(id), name_(name), capacity_(capacity), price_(price) {};
	Application(const Application& app) :
		id_(app.id_), name_(app.name_), capacity_(app.capacity_), price_(app.price_) {};
	Application& operator=(Application&& app) noexcept {
		id_ = app.id_;
		name_ = app.name_;
		capacity_ = app.capacity_;
		price_ = app.price_;
		app.id_ = unsigned int();
		app.name_ = std::string();
		app.capacity_ = unsigned int();
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
	void set_name(std::string_view name) { name_ = name; }
	void set_capacity(const unsigned int& capacity) { capacity_ = capacity; }
	void set_price(const int& price) { price_ = price; }
	const unsigned int& get_id() const { return id_; }
	std::string get_name() const { return name_; }
	unsigned int get_capacity() const { return capacity_; }
	int get_price() const { return price_; }
	bool IsEmpty() const { return id_ == unsigned int(); }//id가 unsigned int의 기본값이라면 빈 Application으로 간주한다.
	bool Print() const;//Application의 정보를 출력한다.
};

class RedBlackTree {
private:
	enum class Color {
		RED, BLACK
	};//노드의 색 정보
	struct Node;
public:
	using NodePtr = std::shared_ptr<Node>;
	using NodePtrList = std::vector<NodePtr>;
private:
	struct Node {
	private:
		Application app_;//각 노드는 Application을 갖고, id로 대소를 비교한다.
		NodePtr left_child_;
		NodePtr right_child_;
		NodePtr parent_;
		Color color_;
	public:
		Node(const int& id, std::string_view name, const unsigned int& capacity, const int& price, NodePtr parent = nullptr) :
			app_(id, name, capacity, price), left_child_(nullptr), right_child_(nullptr), parent_(parent), color_(Color::RED) {};
		Node(const Application& app, NodePtr parent = nullptr) :
			app_(app), left_child_(nullptr), right_child_(nullptr), parent_(parent), color_(Color::RED) {};
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
		int DiscountPrice(int percent) { app_.set_price(app_.get_price() * (100 - percent) / 100); return app_.get_price(); }
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
	void AddRoot(const Application& app);
	unsigned int InsertNode(const Application& app);
	NodePtr SearchNode(unsigned int id, unsigned int& depth) const;
	Application UpdateNode(unsigned int id, std::string name, unsigned int capacity, int price);
	void Discount(unsigned int x, unsigned int y, int percent);
	NodePtrList InOrder() const;
public:
	void Debug() const;//디버깅을 위한 편의 함수. 
	unsigned int get_depth(NodePtr ptr) const;//노드의 깊이를 반환한다.
private://내부 함수
	void ExpandExternal(NodePtr ptr);//leaf 노드의 자식들을 nullptr이 아닌 빈 Application을 담고 있는 노드로 만드는 편의 함수.
	NodePtr Recoloring(NodePtr ptr);//Recolor nodes
	void Reconstruct(NodePtr ptr, unsigned int& depth);//Reconstruct a node
	void InOrderUtility(NodePtr ptr, NodePtrList& list) const;
private:
	NodePtr root_;
	unsigned int size_;
};
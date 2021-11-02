#pragma once
#include <iostream>
#include <string_view>

class Application {
private:
	unsigned int id_;//[1000000 , 1999999] | Once fixed, It'd not change.
	std::string name_;
	unsigned int capacity_;//[1 , 10000] , Unit : MB
	int price_;//[0, 1000] , Unit : 1000
public:
	Application() :id_(unsigned int()), name_(std::string()), capacity_(unsigned int()), price_(int()) {};//Default
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
	bool IsEmpty() const { return id_ == unsigned int(); }
	void Print() const;
};

class RedBlackTree {
private:
	enum class Color {
		RED, BLACK
	};
	struct Node;
	using NodePtr = std::shared_ptr<Node>;
private:
	struct Node {
	private:
		Application app_;
		NodePtr left_child_;
		NodePtr right_child_;
		NodePtr parent_;
		Color color_;
	public:
		Node(const int& id, std::string_view name, const unsigned int& capacity, const int& price, NodePtr parent = nullptr) :
			app_(id, name, capacity, price), left_child_(std::make_shared<Node>(Application())), right_child_(std::make_shared<Node>(Application())), parent_(parent), color_(Color::RED) {};
		/*Node(const Application& app, NodePtr parent = nullptr) :
			app_(app), left_child_(std::make_shared<Node>(Application())), right_child_(std::make_shared<Node>(Application())), parent_(parent), color_(Color::RED) {
			
		};*/
		Node(const Application& app) {
			app_ = app;
			std::cout << "a" << std::endl;
			system("pause");
			left_child_ = std::make_shared<Node>(Application());
			std::cout << "b" << std::endl;

			right_child_ = std::make_shared<Node>(Application());
			parent_ = nullptr;
			color_ = Color::RED;
		}
		Node(const Node&) = delete;
		Node(Node&&) = delete;
		Node& operator=(const Node&) = delete;
		Node& operator=(Node&&) = delete;
		~Node() {};
	public:
		const Application& get_app() const { return app_; };
		bool IsLeaf() const { return app_.IsEmpty(); }
		bool IsRoot() const { return parent_ == nullptr; }
		NodePtr left() const { return left_child_; }
		NodePtr right() const { return right_child_; }
		NodePtr parent() const { return parent_; }
		Color get_color() const { return color_; }
		void SetApplication(const Application& app) {
			app_ = app;
		}
		void Print() const;
		
	};
public:
	RedBlackTree() : size_(0), root_(nullptr) {};
public:
	unsigned int get_size() const { return size_; }
	bool IsEmpty() const { return size_ == 0; };
	void AddRoot(const Application& app) { root_ = std::make_shared<Node>(app); size_ = 1; }
	void InsertNode(const Application& app);
	NodePtr SearchNode(unsigned int id) const;
private:
	NodePtr root_;
	unsigned int size_;
};
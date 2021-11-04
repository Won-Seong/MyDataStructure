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
	bool Print() const;
};

class RedBlackTree {
private:
	enum class Color {
		RED, BLACK
	};
	struct Node;
public:
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
		bool IsLeaf() const { return app_.IsEmpty(); }
		bool IsRoot() const { return parent_ == nullptr; }
		NodePtr left() const { return left_child_; }
		NodePtr right() const { return right_child_; }
		NodePtr parent() const { return parent_; }
		NodePtr grandparent() const { return parent_->parent(); }
		NodePtr uncle() const { 
			return grandparent()->left() == parent_ ? grandparent()->right() : grandparent()->left();
		}
		void set_left(NodePtr ptr) { left_child_ = ptr; }
		void set_right(NodePtr ptr) { right_child_ = ptr; }
		void set_parent(NodePtr ptr) { parent_ = ptr; }
		Color get_color() const { return color_; }
		void SetApplication(const Application& app) {
			app_ = app;
		}
		void UpdateApplication(std::string name, unsigned int capacity, int price) { app_.set_name(name); app_.set_capacity(capacity); app_.set_price(price); }
		void Print() const;
		int IsDoubleRed() const;//Is the parent red when this is red?
		//return : 0(No double red) | 1(Double red and needs recoloring) | 2(Double red and needs reconstruct)
		Color ChangeColor() { if (color_ == Color::RED) color_ = Color::BLACK; else color_ = Color::RED; return color_; };
		Color ChangeColor(Color color) { color_ = color; return color_; };
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
	void Debug() const;
private://Internal function
	void ExpandExternal(NodePtr ptr);//It changes leave's children from nullptr to empty node
	NodePtr Recoloring(NodePtr ptr);//Recolor nodes
	void Reconstruct(NodePtr ptr, unsigned int& depth);//Reconstruct a node
private:
	NodePtr root_;
	unsigned int size_;
};
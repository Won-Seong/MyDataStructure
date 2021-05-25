#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

constexpr int MAX_DEGREE = 5;
constexpr int BLOCK_SIZE = 8;

class B_tree : std::exception {
protected:
	class Node; //36Byte, 노드의 원형
	class Entry;//8Byte, 엔트리의 원형
	class IndexNode;
	class LeafNode;
protected:
	using NodePtr = std::shared_ptr<Node>;
protected:
	//Entry
	class Entry {
	protected:
		int key_; 
		int value_;// 인덱스 엔트리일 때에는 next_level_bid, 데이터 엔트리일 때에는 value
	public:
		Entry(const int& key, const int& value) : key_(key) , value_(value) {
			
		}
		~Entry(){}
	public:
		const int& get_key() const { return key_; }
		const int& get_value() const { return value_; }
	};
	//Node
	class Node {
	protected:
		int block_id_;
		int next_bid_; //leaf node일 경우 next level bid, 아니면 next bid
		std::vector<Entry> entries;
	public:
		Node(const int& block_id , const int& next_bid = int()) :block_id_(block_id) , next_bid_(next_bid) {
			entries.reserve(MAX_DEGREE);
		}
	public:
		virtual bool IsLeaf() const = 0;
		bool InsertEntry(const int& key, const int& value);
		void PrintEntry() const {
			for (auto& itr : entries)
				std::cout << itr.get_key() << ' ';
			std::cout << std::endl;
		}
	};
	class IndexNode : public Node {
	public:
		IndexNode(const int& block_id, const int& next_bid = int()) : Node(block_id, next_bid) {

		}
	public:
		bool IsLeaf() const override {
			return false;
		}
		
	};
	class LeafNode : public Node {
	public:
		LeafNode(const int& block_id, const int& next_bid = int()) : Node(block_id,next_bid) {

		}
	public:
		bool IsLeaf() const override {
			return true;
		}
	};
public:
	B_tree() : entry_number_(0) , current_block_id_(1) {
		root_ptr_ = std::make_shared<LeafNode>(current_block_id_++);
	}
public:
	bool Insert(const int& key, const int& value);
	void Print() const;
private:
	int current_block_id_; //1부터 시작, 0은 Null Block
	int entry_number_;//How many entries?
	NodePtr root_ptr_;
};




int main() {

	B_tree my_tree;
	std::ofstream ofs;
	ofs.open("good.bin" , std::ios::binary);

	std::string str = "asdf";
	ofs.write(str.c_str(),str.size());
	
	ofs.close();
	
	
}

bool B_tree::Insert(const int& key, const int& value)
{
	if (root_ptr_.operator*().IsLeaf() == true) {
		try
		{
			root_ptr_.operator*().InsertEntry(key, value);
		}
		catch (const std::exception& e)
		{
			std::cout << e.what() << " : (" << key << " , " << value << ")" << std::endl;
		}
	}
	entry_number_++;
	return true;
}

void B_tree::Print() const
{
	root_ptr_.operator*().PrintEntry();
}

bool B_tree::Node::InsertEntry(const int& key, const int& value)
{
	if (entries.empty()) {
		entries.push_back(Entry{ key, value });
		return false;
	}
	else {
		std::vector<Entry>::iterator iter = entries.begin();
		for (; iter != entries.end(); iter++) {
			if (iter.operator*().get_key() == key) throw std::invalid_argument("Existed Entry");
			else if (iter.operator*().get_key() > key) {
				entries.insert(iter, Entry{ key , value });
				break;
			}
		}
		if (iter == entries.end())
			entries.push_back(Entry{ key, value });
	}
	if (entries.size() >= MAX_DEGREE) { //만약 리스트의 크기가 max_degree를 넘었다면?
		return true;
	}
	return false;
}

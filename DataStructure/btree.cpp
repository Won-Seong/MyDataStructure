#include <iostream>
#include <fstream>
#include <vector>

class Entry : std::exception {
protected:
	int key_;
	int value_;//�ε��� ��Ʈ���� ������ next_level_bid, ������ ��Ʈ���� ������ value
public:
	Entry(const int& key, const int& value) : key_(key), value_(value) { }
	~Entry() {}
public:
	const int& get_key() const { return key_; }
	const int& get_value() const { return value_; }
	void Print() const { std::cout << "Key : " << key_ << " | Value : " << value_ << std::endl; }
	void WriteEntry(std::ofstream& out_file_stream) const;
};//�� 8Byte

class B_tree : std::exception {
protected:
	class Node; //(8 * Entry number + 4)Byte, ���� Non-leaf node�� Leaf node�� ���� �ڽ����� ������ �ʰ� Node�� ����
	using BunchOfBlock = std::vector<Node>;
public:
	class Node {
	protected:
		bool leaf_flag_;//flag bit�� leaf�� non-leaf�� ����
		int block_id_;//�� ����� �� ��° ����ΰ�? | ��ϰ� ����� ����
		int next_bid_;
		//Leaf node�� ��� ���������� ���� leaf node�� ID�� ����Ų��
		//Non leaf node�� ��� ���� ������ Block ID�� ����Ų��.
		std::vector<Entry> entries;//Block size�� ���� ��Ʈ���� �󸶳� ���� ���� ���̳İ� ��������.
	public:
		Node(const int& block_id, bool flag, const int& next_bid = int()) :block_id_(block_id), leaf_flag_(flag), next_bid_(next_bid) {

		}
	public:
		bool IsLeaf() const { return leaf_flag_; }
		const Entry& InsertEntry(const int& key, const int& value);
		void PrintNode() const {
			std::cout << "Current Block ID : " << block_id_ << " | Next Block ID : " << next_bid_ << std::endl;
			for (auto& itr : entries)
				itr.Print();
			std::cout << std::endl;
		}
		void WriteBlock(std::ofstream& ofs) const;
		int get_block_id() const { return block_id_; }
		int get_next_bid() const { return next_bid_; }
		void ReadBlockAsLeaf(std::ifstream& ifs, const int& block_size);
		void ReadBlockAsIndex(std::ifstream& ifs, const int& block_size);
		void EntryClear() { entries.clear(); }//For debugging
	};
public:
	B_tree() : current_block_id_(1) {}
public:
	bool Insert(std::ifstream& ifs, std::ofstream& ofs, const int& key, const int& value);
	BunchOfBlock PointSearch(std::ifstream& ifs, const int& key) const;//Point search
	void Print() const;
private:
	void SearchUtility(std::ifstream& ifs, BunchOfBlock& blocks, const int& current_bid, int depth, const int& block_size) const;
private:
	int current_block_id_; //1���� ����, 0�� Null Block
};

void CreateBinaryFile(std::ofstream& ofs, const int& block_size) {
	ofs.write((char*)&block_size, sizeof(int));//block_size
	int temp = 1;
	ofs.write((char*)&temp, sizeof(int));//root_bid
	temp = 0;
	ofs.write((char*)&temp, sizeof(int));//depth
}

int main() {
	std::string file_name;
	file_name = "good.bin";

	std::ofstream ofs;
	ofs.open("good.bin", std::ios::binary);

	B_tree tree;
	B_tree::Node asdf{ 1,true,2 };

	asdf.InsertEntry(1, 2);
	asdf.InsertEntry(7, 4);
	asdf.InsertEntry(4, 6);
	asdf.InsertEntry(2, 5);

	

	asdf.PrintNode();

	asdf.WriteBlock(ofs);

	asdf.EntryClear();

	asdf.PrintNode();

	ofs.close();
	std::ifstream ifs;
	ifs.open("good.bin", std::ios::binary);

	asdf.ReadBlockAsLeaf(ifs,36);

	asdf.PrintNode();
	


}

const Entry& B_tree::Node::InsertEntry(const int& key, const int& value)
{
	Entry temp_entry{ key,value };
	if (entries.empty()) {
		entries.push_back(temp_entry);
	}//���� ��尡 ������� ��쿡�� �׳� �ִ´�
	else {
		std::vector<Entry>::iterator iter = entries.begin();
		for (; iter != entries.end(); iter++) {
			if (iter.operator*().get_key() == key) throw std::invalid_argument("Existed Entry");//�ߺ��Ǵ� ��Ʈ���� ���� �Ұ�
			else if (iter.operator*().get_key() > key) {
				break;
			}
		}
		entries.insert(iter, temp_entry);//�����Ϸ��� ��Ʈ���� Ű�� ���� Ŭ ��� �� �ڿ� �ִ´�.
	}

	return temp_entry;
}

void B_tree::Node::WriteBlock(std::ofstream& out_file_stream) const
{
	if (IsLeaf()) {
		for (auto& itr : entries) {
			itr.WriteEntry(out_file_stream);
		}
		out_file_stream.write((char*)&next_bid_, sizeof(int));
	}
	else {
		out_file_stream.write((char*)&next_bid_, sizeof(int));
		for (auto& itr : entries) {
			itr.WriteEntry(out_file_stream);
		}
	}
}

void B_tree::Node::ReadBlockAsLeaf(std::ifstream& ifs, const int& block_size)
{
	for (int i = 0; i < (block_size - 4) / 8; i++) {
		int temp_key = int();
		int temp_value = int();
		ifs.read((char*)&temp_key, sizeof(int));
		ifs.read((char*)&temp_value, sizeof(int));
		entries.push_back(Entry{ temp_key,temp_value });
	}
	int temp_next_bid = int();
	ifs.read((char*)&temp_next_bid, sizeof(int));
	next_bid_ = temp_next_bid;
}

void B_tree::Node::ReadBlockAsIndex(std::ifstream& ifs, const int& block_size)
{
	int temp_next_bid = int();
	ifs.read((char*)&temp_next_bid, sizeof(int));
	next_bid_ = temp_next_bid;
	for (int i = 0; i < (block_size - 4) / 8; i++) {
		int temp_key = int();
		int temp_value = int();
		ifs.read((char*)&temp_key, sizeof(int));
		ifs.read((char*)&temp_value, sizeof(int));
		entries.push_back(Entry{ temp_key,temp_value });
	}
}

void Entry::WriteEntry(std::ofstream& out_file_stream) const
{
	out_file_stream.write((char*)&key_, sizeof(int));
	out_file_stream.write((char*)&value_, sizeof(int));
}

B_tree::BunchOfBlock B_tree::PointSearch(std::ifstream& ifs, const int& key) const
{
	ifs.seekg(0, std::ios::beg);
	int block_size = int();
	int root_bid = int();
	int depth = int();
	BunchOfBlock blocks;

	ifs.read((char*)&block_size, sizeof(int));
	ifs.read((char*)&root_bid, sizeof(int));
	ifs.read((char*)&depth, sizeof(int));

	SearchUtility(ifs, blocks, root_bid, depth, block_size);
	return blocks;
}

void B_tree::SearchUtility(std::ifstream& ifs, BunchOfBlock& blocks, const int& current_bid, int depth, const int& block_size) const
{
	if (depth == 0) {
		Node temp_node{ current_bid, true };
		blocks.push_back(temp_node);
		return;
	}
	Node temp_node{ current_bid, false };
	temp_node.ReadBlockAsIndex(ifs, block_size);
	blocks.push_back(temp_node);
	SearchUtility(ifs, blocks, blocks.back().get_next_bid(), depth - 1, block_size);
}




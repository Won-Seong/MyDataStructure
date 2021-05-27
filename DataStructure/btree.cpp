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
		void PrintEntry() const {
			for (auto& itr : entries)
				itr.Print();
			std::cout << std::endl;
		}
		void WriteBlock(std::ofstream& ofs) const;
		void ReadBlock(std::ifstream& ifs);
	};
public:
	B_tree() : current_block_id_(1) {
	}
public:
	bool Insert(std::ifstream& ifs,std::ofstream& ofs, const int& key, const int& value);
	int Search(std::ifstream& ifs, const int& key) const;//Point search
	void Print() const;
private:
	const Entry& SearchUtility(std::ifstream& ifs,int current_bid, int depth) const;
private:
	int current_block_id_; //1���� ����, 0�� Null Block
};

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



	asdf.PrintEntry();

	asdf.WriteBlock(ofs);

	ofs.close();
	std::ifstream ifs;
	ifs.open("good.bin", std::ios::binary);
	int aaa;
	ifs.read((char*)&aaa, sizeof(int));
	std::cout << aaa << std::endl;
	int bbb;
	ifs.seekg(0);
	ifs.read((char*)&bbb, sizeof(int));
	std::cout << bbb;

}

//
//bool B_tree::Insert(const int& key, const int& value)
//{
//	if (root_ptr_.operator*().IsLeaf() == true) {
//		try
//		{
//			root_ptr_.operator*().InsertEntry(key, value);
//		}
//		catch (const std::exception& e)
//		{
//			std::cout << e.what() << " : (" << key << " , " << value << ")" << std::endl;
//		}
//	}
//	entry_number_++;
//	return true;
//}
//
//void B_tree::Print() const
//{
//	root_ptr_.operator*().PrintEntry();
//}
//
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
		out_file_stream.write((char*)&next_bid_,sizeof(int));
	}
	else {
		out_file_stream.write((char*)&next_bid_, sizeof(int));
		for (auto& itr : entries) {
			itr.WriteEntry(out_file_stream);
		}
	}
}

void Entry::WriteEntry(std::ofstream& out_file_stream) const
{
	out_file_stream.write((char*)&key_, sizeof(int));
	out_file_stream.write((char*)&value_, sizeof(int));
}

int B_tree::Search(std::ifstream& ifs, const int& key) const
{
	ifs.seekg(0, std::ios::beg);
	int block_size, root_bid, depth;
	ifs.read((char*)&block_size, sizeof(int));
	ifs.read((char*)&root_bid, sizeof(int));
	ifs.read((char*)&depth, sizeof(int));

	return 4;
}

const Entry& B_tree::SearchUtility(std::ifstream& ifs, int current_bid, int depth) const
{
	if (depth == 0) {
		
	}
	else {

	}
}

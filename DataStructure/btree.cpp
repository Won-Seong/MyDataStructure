#include <iostream>
#include <fstream>
#include <vector>

class Entry : std::exception {
protected:
	int key_;
	int value_;//인덱스 엔트리일 때에는 next_level_bid, 데이터 엔트리일 때에는 value
public:
	Entry(const int& key, const int& value) : key_(key), value_(value) { }
	~Entry() {}
public:
	const int& get_key() const { return key_; }
	const int& get_value() const { return value_; }
	void Print() const { std::cout << "Key : " << key_ << " | Value : " << value_ << std::endl; }
	void WriteEntry(std::ofstream& out_file_stream) const;
};//총 8Byte

class B_tree : std::exception {
protected:
	class Node; //(8 * Entry number + 4)Byte, 굳이 Non-leaf node와 Leaf node를 따로 자식으로 나누지 않고 Node로 통합
	using BunchOfBlock = std::vector<Node>;
public:
	class Node {
	protected:
		bool leaf_flag_;//flag bit로 leaf와 non-leaf를 구분
		int block_id_;//이 블록은 몇 번째 블록인가? | 블록과 블록을 구분
		int next_bid_;
		//Leaf node일 경우 선형적으로 다음 leaf node의 ID를 가리킨다
		//Non leaf node일 경우 다음 레벨의 Block ID를 가리킨다.
		std::vector<Entry> entries;//Block size에 따라 엔트리를 얼마나 갖고 있을 것이냐가 정해진다.
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
	int current_block_id_; //1부터 시작, 0은 Null Block
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
	}//만약 노드가 비어있을 경우에는 그냥 넣는다
	else {
		std::vector<Entry>::iterator iter = entries.begin();
		for (; iter != entries.end(); iter++) {
			if (iter.operator*().get_key() == key) throw std::invalid_argument("Existed Entry");//중복되는 엔트리는 삽입 불가
			else if (iter.operator*().get_key() > key) {
				break;
			}
		}
		entries.insert(iter, temp_entry);//삽입하려는 엔트리의 키가 가장 클 경우 맨 뒤에 넣는다.
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

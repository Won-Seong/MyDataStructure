//컴퓨터 공학과 12141508 김성원
#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <algorithm>
#include <string>

class Entry : std::exception {
protected:
	int key_;
	int value_;//인덱스 엔트리일 때에는 next_level_bid, 데이터 엔트리일 때에는 value
public:
	Entry(const int& key = 0, const int& value = 0) : key_(key), value_(value) { }
	Entry operator=(const Entry& entry) { key_ = entry.get_key(); value_ = entry.get_value(); return *this; }
	~Entry() {}
public:
	const int& get_key() const { return key_; }
	const int& get_value() const { return value_; }
	void Print() const { std::cout << "Key : " << key_ << " | Value : " << value_ << std::endl; }
	void WriteEntry(std::ofstream& out_file_stream) const;
public:
	bool operator==(const Entry& entry) const {
		return key_ == entry.key_ && value_ == entry.value_;
	}
};//총 8Byte

class B_tree : std::exception {
protected:
	class Node; //(8 * Entry number + 4)Byte, 굳이 Non-leaf node와 Leaf node를 따로 자식으로 나누지 않고 Node로 통합
	using BunchOfBlock = std::vector<Node>;
	using Entries = std::vector<Entry>;
protected:
	class Node {
	protected:
		bool leaf_flag_;//flag bit로 leaf와 non-leaf를 구분
		int block_id_;//이 블록은 몇 번째 블록인가? | 블록과 블록을 구분
		int next_bid_;
		//Leaf node일 경우 선형적으로 다음 leaf node의 ID를 가리킨다
		//Non leaf node일 경우 다음 레벨의 Block ID를 가리킨다.
		Entries entries;//Block size에 따라 엔트리를 얼마나 갖고 있을 것이냐가 정해진다.
		int block_size_;
	public:
		Node(const int& block_id, const int& block_size , bool flag = false, const int& next_bid = int()) :block_id_(block_id), block_size_(block_size) ,leaf_flag_(flag), next_bid_(next_bid) {}
	public:
		bool IsLeaf() const { return leaf_flag_; }
		void SetLeaf() { leaf_flag_ = true; }
		Entry InsertEntry(const int& key, const int& value);
		Entry InsertEntry(const Entry& entry);
		void PrintNode() const {
			std::cout << "Current Block ID : " << block_id_ << " | Next Block ID : " << next_bid_ << std::endl;
			for (auto& itr : entries)
				itr.Print();
			std::cout << std::endl;
		}
		void PrintForPrint() const {
			for (int i = 0; i < entries.size() - 1; i++) {
				std::cout << entries[i].get_key() << ", ";
			}
			std::cout << entries.back().get_key();
		}
		void GetAllValuesForPrint(std::queue<int>& input_queue) {
			for (auto& itr : entries) {
				input_queue.push(itr.get_value());
			}
		}
		void WriteBlock(std::ofstream& ofs);
		void ResizeBlock(const int& max_degree) { entries.resize(max_degree); }
		int get_block_id() const { return block_id_; }
		int get_next_bid() const { return next_bid_; }
		void ReadBlockAsLeaf(std::ifstream& ifs);
		void ReadBlockAsIndex(std::ifstream& ifs);
		const Entry& SearchEntry(const int& key) const;
		int SearchEntryForNextId(const int& key) const;
		void EntryClear() { entries.clear(); }//For debugging
		Entry CheckAndSplit(const int& input_bid, std::ofstream& ofs);//만약 블록이 정해진 degree보다 커질 경우...
		const Entries& GetEntries() const { return entries; }
	};
public:
	B_tree() : current_block_id_(2) {}
public:
	bool Insert(std::ifstream& ifs, std::ofstream& ofs, const int& key, const int& value);
	Entry PointSearch(std::ifstream& ifs, const int& key) const;
	std::vector<Entry> RangeSearch(std::ifstream& ifs, const int& left_key, const int& right_key) const;
	void Print(std::ifstream& ifs) const;
private:
	void SearchUtility(std::ifstream& ifs, BunchOfBlock& blocks, const int& current_bid, int depth, const int& block_size, const int& key) const;
	BunchOfBlock Search(std::ifstream& ifs, const int& key) const;//Search
	void InsertUtility(BunchOfBlock& blocks,std::ofstream& ofs, const int& block_size, const int& depth);
	void SearchMetaData(int& block_size, int& root_bid, int& depth, std::ifstream& ifs) const;
private:
	int current_block_id_; //1부터 시작, 0은 Null Block
};

void CreateBinaryFile(std::ofstream& ofs, const int& block_size) {
	ofs.write((char*)&block_size, sizeof(int));//block_size
	int temp = 1;
	ofs.write((char*)&temp, sizeof(int));//root_bid
	temp = 0;
	ofs.write((char*)&temp, sizeof(int));//depth
	ofs.flush();
}

Entry B_tree::Node::InsertEntry(const int& key, const int& value)
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

Entry B_tree::Node::InsertEntry(const Entry& entry)
{
	if (entries.empty()) {
		entries.push_back(entry);
	}//만약 노드가 비어있을 경우에는 그냥 넣는다
	else {
		std::vector<Entry>::iterator iter = entries.begin();
		for (; iter != entries.end(); iter++) {
			if (iter.operator*().get_key() == entry.get_key()) throw std::invalid_argument("Existed Entry");//중복되는 엔트리는 삽입 불가
			else if (iter.operator*().get_key() > entry.get_key()) {
				break;
			}
		}
		entries.insert(iter, entry);//삽입하려는 엔트리의 키가 가장 클 경우 맨 뒤에 넣는다.
	}
	return entry;
}

void B_tree::Node::WriteBlock(std::ofstream& out_file_stream) 
{
	ResizeBlock((block_size_ - 4) / 8);
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
	out_file_stream.flush();
}

void B_tree::Node::ReadBlockAsLeaf(std::ifstream& ifs)
{
	for (int i = 0; i < (block_size_ - 4) / 8; i++) {
		int temp_key = int();
		int temp_value = int();
		ifs.read((char*)&temp_key, sizeof(int));
		ifs.read((char*)&temp_value, sizeof(int));
		if (temp_key == int() && temp_value == int()) continue;
		entries.push_back(Entry{ temp_key,temp_value });
	}
	int temp_next_bid = int();
	ifs.read((char*)&temp_next_bid, sizeof(int));
	next_bid_ = temp_next_bid;
}

void B_tree::Node::ReadBlockAsIndex(std::ifstream& ifs)
{
	int temp_next_bid = int();
	ifs.read((char*)&temp_next_bid, sizeof(int));
	next_bid_ = temp_next_bid;
	for (int i = 0; i < (block_size_ - 4) / 8; i++) {
		int temp_key = int();
		int temp_value = int();
		ifs.read((char*)&temp_key, sizeof(int));
		ifs.read((char*)&temp_value, sizeof(int));
		if (temp_key == int() && temp_value == int()) continue;

		entries.push_back(Entry{ temp_key,temp_value });
	}
}

const Entry& B_tree::Node::SearchEntry(const int& key) const
{
	for (auto& itr : entries) {
		if (itr.get_key() == key) return itr;
	}
	throw std::invalid_argument("No existed.");
}

int B_tree::Node::SearchEntryForNextId(const int& key) const
{
	if (entries.front().get_key() > key) return next_bid_;
	std::vector<Entry>::const_iterator c_iter = entries.cbegin();
	for (; c_iter != entries.cend(); c_iter++) {
		if (c_iter.operator*().get_key() > key) return (c_iter - 1).operator*().get_value();
	}
	return entries.back().get_value();
}

Entry B_tree::Node::CheckAndSplit(const int& input_bid, std::ofstream& ofs)
{
	//1. 먼저 크기가 한도보다 커졌는지 검사한다.

	int max_size = ((block_size_ - 4) / 8) + 1;
	if (max_size > entries.size()) return Entry(int(), int());//크기가 크지 않다면 빠져나간다.

	//2. 먼저 분할한다.
	Node next_node{ input_bid ,block_size_};
	std::vector<Entry>::iterator iter = (max_size % 2) == 1 ? entries.begin() + ((max_size - 1) / 2) : entries.begin() + (max_size / 2);//위치 지정
	Entry temp_entry{ iter.operator*().get_key(), input_bid };

	//만약 LeafNode라면
	if (IsLeaf()) {
		next_node.SetLeaf();
		next_node.next_bid_ = next_bid_;
		next_bid_ = input_bid;
		std::copy(iter, entries.end(), std::back_inserter(next_node.entries));
		entries.erase(iter, entries.end());
	}
	else {
		next_node.next_bid_ = iter.operator*().get_value();
		std::copy(iter + 1, entries.end(), std::back_inserter(next_node.entries));
		entries.erase(iter, entries.end());
	}
	//분할 완료

	//3. 분할을 하였으므로 새 블록을 쓴다.

	ofs.seekp(12 + (input_bid - 1) * block_size_);
	next_node.WriteBlock(ofs);
	ofs.flush();

	//4. 다음 노드를 가리키는 엔트리를 생성하고 반환한다. 반환 후 처리는 다른 함수에서.

	return temp_entry;
}

void Entry::WriteEntry(std::ofstream& out_file_stream) const
{
	out_file_stream.write((char*)&key_, sizeof(int));
	out_file_stream.write((char*)&value_, sizeof(int));
	out_file_stream.flush();
}

bool B_tree::Insert(std::ifstream& ifs, std::ofstream& ofs, const int& key, const int& value)
{
	int block_size = int();
	int root_bid = int();
	int depth = int();
	SearchMetaData(block_size, root_bid, depth,ifs);

	//넣을 곳 찾아야하므로 우선 서치
	BunchOfBlock blocks = std::move(Search(ifs, key));
	
	//삽입
	blocks.back().InsertEntry(key, value);
	
	//사이즈가 한도보다 큰가?
	InsertUtility(blocks, ofs, block_size, depth);
	return true;
}

Entry B_tree::PointSearch(std::ifstream& ifs, const int& key) const
{
	BunchOfBlock blocks = std::move(Search(ifs, key));
	
	try
	{
		return blocks.back().SearchEntry(key);
	}
	catch (const std::exception& e)
	{
		throw e;
	}
	return -1;
}

std::vector<Entry> B_tree::RangeSearch(std::ifstream& ifs, const int& left_key, const int& right_key) const
{
	BunchOfBlock blocks = std::move(Search(ifs, left_key));
	Entries entries;
	for (auto& itr : blocks.back().GetEntries()) {
		if (itr.get_key() >= left_key) { entries.push_back(itr); }
	}

	int block_size = int();
	int root_bid = int();
	int depth = int();
	SearchMetaData(block_size, root_bid, depth,ifs);
	int next_bid = blocks.back().get_next_bid();

	std::cout << block_size << ' ' << root_bid << ' ' << depth << std::endl;
	ifs.clear();
	for (bool flag = false; flag == false; ) {
		ifs.seekg(12 + ((next_bid - 1) * block_size) , std::ios::beg);
		Node temp_node{ next_bid , block_size, true };
		temp_node.ReadBlockAsLeaf(ifs);
		for (auto& itr : temp_node.GetEntries()) {
			if (itr.get_key() <= right_key) { entries.push_back(itr); }
			else {
				flag = true;
				break;
			}
		}
		next_bid = temp_node.get_next_bid();
		if (next_bid == 0) break;
	}
	
	return entries;
}

void B_tree::Print(std::ifstream& ifs) const
{
	int block_size = int();
	int root_bid = int();
	int depth = int();
	SearchMetaData(block_size, root_bid, depth,ifs);

	std::queue<int> temp_queue;//ID를 담는 큐
	temp_queue.push(root_bid);
	size_t temp_size = size_t();
	int temp = int();//ID 지정할 때마다 쓰일 임시 변수

	for (int i = 0; i < depth; i++) {

		std::cout << "<" << i << ">" << std::endl;
		temp_size = temp_queue.size();
		for (int j = 0; j < temp_size; j++) {
			temp = temp_queue.front();
			temp_queue.pop();
			ifs.seekg(12 + ((temp - 1) * block_size));

			Node temp_node{ 0, block_size };
			temp_node.ReadBlockAsIndex(ifs);
			temp_node.PrintForPrint();
			std::cout << ", ";
			temp_queue.push(temp_node.get_next_bid());
			temp_node.GetAllValuesForPrint(temp_queue);
		}
		std::cout << std::endl;
	}
}


B_tree::BunchOfBlock B_tree::Search(std::ifstream& ifs, const int& key) const
{
	//우선 메타 데이터를 서치
	int block_size = int();
	int root_bid = int();
	int depth = int();
	SearchMetaData(block_size, root_bid, depth, ifs);
	BunchOfBlock blocks;
	
	//재귀적으로 서치
	SearchUtility(ifs, blocks, root_bid, depth, block_size, key);
	return blocks;
}

void B_tree::SearchUtility(std::ifstream& ifs, BunchOfBlock& blocks, const int& current_bid, int depth, const int& block_size, const int& key) const
{
	ifs.seekg(12 + ((current_bid - 1) * block_size));
	if (depth == 0) {
		Node temp_node{ current_bid, block_size, true };
		temp_node.ReadBlockAsLeaf(ifs);
		blocks.push_back(temp_node);
		return;
	}
	Node temp_node{ current_bid, block_size };
	temp_node.ReadBlockAsIndex(ifs);
	blocks.push_back(temp_node);
	int next_bid = blocks.back().SearchEntryForNextId(key); 
	SearchUtility(ifs, blocks, next_bid, depth - 1, block_size, key);
}

void B_tree::InsertUtility(BunchOfBlock& blocks, std::ofstream& ofs, const int& block_size,  const int& depth)
{
	Entry temp_entry = blocks.back().CheckAndSplit(current_block_id_, ofs);
	ofs.seekp(12 + (blocks.back().get_block_id() - 1) * block_size);
	if (temp_entry == Entry(int(), int())) {
		blocks.back().WriteBlock(ofs);
	}
	else {//만약 크다면 우선 next_node는 write했으므로 현재 노드를 써야 한다.
		current_block_id_++;//분할했으므로
		blocks.back().WriteBlock(ofs);
		std::cout << "hehe" << std::endl;
		if (blocks.size() == 1) {
			std::cout << "WOW!" << std::endl;
			Node root_node{current_block_id_ , block_size ,false, blocks.back().get_block_id()};
			root_node.InsertEntry(temp_entry);
			ofs.seekp(12 + (root_node.get_block_id() - 1) * block_size);
			root_node.WriteBlock(ofs);


			ofs.seekp(4);
			int temp_bid = root_node.get_block_id();
			int temp_depth = depth + 1;
			ofs.write((char*)&(temp_bid), sizeof(int));
			ofs.write((char*)&(temp_depth), sizeof(int));

			current_block_id_++;
			ofs.flush();
			return;
		}//만약 Root가 한도가 넘었다면

		blocks.pop_back();
		blocks.back().InsertEntry(temp_entry);
		InsertUtility(blocks, ofs, block_size, depth);
	}
}

void B_tree::SearchMetaData(int& block_size, int& root_bid, int& depth, std::ifstream& ifs) const
{
	ifs.clear();
	ifs.seekg(0, std::ios::beg);
	ifs.read((char*)&block_size, sizeof(int));
	ifs.read((char*)&root_bid, sizeof(int));
	ifs.read((char*)&depth, sizeof(int));
}

int main(char* argv[]) {
	char command;
	B_tree tree;
	std::string file_name;
	std::string file_name_for_insert;
	std::string file_name_for_out;
	std::ofstream ofs;
	std::ofstream ofs_for_out;
	std::ifstream ifs;
	std::ifstream ifs_for_insert;
	int key_temp = int();
	int value_temp = int();

	int block_size = 1, root_bid = 2, depth = 3;


	std::string str;
	std::vector<Entry> good;
	std::vector<int> temp_int;


	std::cin >> command;

	switch (command)
	{
	case 'c':
		std::cin >> file_name >> block_size;
		ofs.open(file_name, std::ios::binary);
		ifs.open(file_name, std::ios::binary);
		CreateBinaryFile(ofs, block_size);
		break;
	case 'i':
		std::cin >> file_name >> file_name_for_insert;
		std::cout << file_name << std::endl;
		std::cout << file_name_for_insert << std::endl;
		ifs_for_insert.open(file_name_for_insert);
		
		
		while (ifs_for_insert) {
			if (ifs_for_insert.eof()) break;
			try
			{
				std::getline(ifs_for_insert, str, ',');
				key_temp = std::stoi(str);
				std::getline(ifs_for_insert, str, '\n');
				value_temp = std::stoi(str);
			}
			catch (const std::exception& e)
			{
				break;
			}
			std::cout << key_temp << ' ' << value_temp << std::endl;
			good.push_back(Entry{ key_temp, value_temp });
		}


		ifs_for_insert.close();
		ifs.open(file_name, std::ios::binary);
		ofs.open(file_name, std::ios::binary || std::ios::ate);
		

		for (auto& itr : good) {
			try
			{
				tree.Insert(ifs, ofs, itr.get_key(), itr.get_value());
			}
			catch (const std::exception&)
			{
				std::cout << "Error!" << std::endl;
			}
		}
		
		
		break;
	case 's' :
		std::cin >> file_name >> file_name_for_insert >> file_name_for_out;
		ifs_for_insert.open(file_name_for_insert);

		while (ifs_for_insert) {
			try
			{
				std::getline(ifs_for_insert, str);
				temp_int.push_back(std::stoi(str));
			}
			catch (const std::exception&)
			{
				break;
			}
			
		}


		ifs.open(file_name, std::ios::binary);

		tree.Print(ifs);



		ofs.open(file_name_for_out);

		for (auto& itr : temp_int) {
			try
			{
				Entry etr = tree.PointSearch(ifs, itr);
				etr.Print();
				ofs << etr.get_key() << ' ' << etr.get_value() << std::endl;

			}
			catch (const std::exception&)
			{

			}
		}

		break;
	case 'p':
		std::cin >> file_name;
		ifs.open(file_name, std::ios::binary);
		tree.Print(ifs);
		break;
	default:
		break;
	}



}
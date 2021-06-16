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
	int value_;//It has next block ID if it's an index entry, otherwise value.
public:
	Entry(const int& key = int(), const int& value = int()) : key_(key), value_(value) {}
	Entry operator=(const Entry& entry) { key_ = entry.get_key(); value_ = entry.get_value(); return *this; }
	~Entry() {}
public:
	const int& get_key() const { return key_; }
	const int& get_value() const { return value_; }
	void Print() const { std::cout << "Key : " << key_ << " | Value : " << value_ << std::endl; }
	void WriteEntry(std::fstream&) const;
public:
	bool operator==(const Entry& entry) const {
		return key_ == entry.key_ && value_ == entry.value_;
	}
};//8Byte

class B_tree : std::exception {
protected:
	class Node; //(8 * Entry number + 4)Byte
	using BunchOfBlock = std::vector<Node>;
	using Entries = std::vector<Entry>;
protected:
	class Node {
	protected:
		bool leaf_flag_;//It's a leaf node if the flag is true.
		unsigned int block_id_;//Block number.
		unsigned int next_bid_;
		//This points block id of next leaf node linearly if it's a leaf node.
		//Otherwise, block id of next level node.
		Entries entries;//It depends on the block size.
	public:
		Node(const unsigned int& block_id, bool flag = false, const unsigned int& next_bid = unsigned int()) :block_id_(block_id), leaf_flag_(flag), next_bid_(next_bid) {}
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
		void PrintForPrint(std::fstream& fs) const {
			for (int i = 0; i < entries.size() - 1; i++) {
				fs << entries[i].get_key() << ", ";
			}
			fs << entries.back().get_key();
		}
		void GetAllValuesForPrint(std::queue<int>& input_queue) {
			for (auto& itr : entries) {
				input_queue.push(itr.get_value());
			}
		}
		void WriteBlock(std::fstream& fs, const size_t block_size);//Write to file.
		void ResizeBlock(const size_t& block_size) { entries.resize((block_size - 4) / 8); }
		unsigned int get_block_id() const { return block_id_; }
		unsigned int get_next_bid() const { return next_bid_; }
		void ReadBlock(std::fstream& fs, const size_t block_size);
		const Entry& SearchEntry(const int& key) const;
		int SearchEntryForNextId(const int& key) const;
		void EntryClear() { entries.clear(); }//For debugging
		Entry CheckAndSplit(std::fstream& fs, const unsigned int& input_bid, const size_t& block_size);//If an overflow has occurred
		const Entries& GetEntries() const { return entries; }
	};
public:
	B_tree() : current_block_id_(2), block_size_(unsigned int()), root_bid_(unsigned int()), depth_(unsigned int()) {}
public:
	bool Insert(std::fstream& fs, const int& key, const int& value);
	Entry PointSearch(std::fstream& fs, const int& key) const;
	std::vector<Entry> RangeSearch(std::fstream& fs, const int& left_key, const int& right_key) const;
	void Print(std::fstream& fs, std::fstream& out_fs) const;
	void UpdateMetaData(std::fstream& fs);
private:
	void SearchUtility(std::fstream& fs, BunchOfBlock& blocks, const unsigned int& current_bid, unsigned int depth, const int& key) const;
	BunchOfBlock Search(std::fstream& fs, const int& key) const;//Search
	void InsertUtility(BunchOfBlock& blocks, std::fstream& fs);
private://Metadata
	unsigned int current_block_id_;//Starts at one, zero is a null block. 
	size_t block_size_;//Should be (4 + 8k)
	unsigned int root_bid_;
	unsigned int depth_;
};

void CreateBinaryFile(std::fstream& fs, const size_t& block_size) {
	fs.write((char*)&block_size, sizeof(int));//block_size
	int temp = 1;
	fs.write((char*)&temp, sizeof(int));//root_bid
	temp = 0;
	fs.write((char*)&temp, sizeof(int));//depth
	fs.flush();
}

Entry B_tree::Node::InsertEntry(const int& key, const int& value)
{
	Entry temp_entry{ key,value };
	if (entries.empty()) {
		entries.push_back(temp_entry);
	}//If this is an empty node, just input
	else {
		std::vector<Entry>::iterator iter = entries.begin();
		for (; iter != entries.end(); iter++) {
			if (iter.operator*().get_key() == key) throw std::invalid_argument("Existed Entry");//Doesn't allow duplicated value.
			else if (iter.operator*().get_key() > key) {
				break;
			}
		}
		entries.insert(iter, temp_entry);
	}
	return temp_entry;
}

Entry B_tree::Node::InsertEntry(const Entry& entry)
{
	if (entries.empty()) {
		entries.push_back(entry);
	}///If this is an empty node, just input
	else {
		std::vector<Entry>::iterator iter = entries.begin();
		for (; iter != entries.end(); iter++) {
			if (iter.operator*().get_key() == entry.get_key()) throw std::invalid_argument("Existed Entry");//Doesn't allow duplicated value.
			else if (iter.operator*().get_key() > entry.get_key()) {
				break;
			}
		}
		entries.insert(iter, entry);
	}
	return entry;
}

void B_tree::Node::WriteBlock(std::fstream& fs, const size_t block_size)
{
	ResizeBlock(block_size);
	if (IsLeaf()) {
		for (auto& itr : entries)
			itr.WriteEntry(fs);
		fs.write((char*)&next_bid_, sizeof(int));
	}
	else {
		fs.write((char*)&next_bid_, sizeof(int));
		for (auto& itr : entries)
			itr.WriteEntry(fs);
	}
	fs.flush();
}

void B_tree::Node::ReadBlock(std::fstream& fs, const size_t block_size)
{
	if (IsLeaf()) {
		for (int i = 0; i < (block_size - 4) / 8; i++) {
			int temp_key = int();
			int temp_value = int();
			fs.read((char*)&temp_key, sizeof(int));
			fs.read((char*)&temp_value, sizeof(int));
			if (temp_key == int() && temp_value == int()) continue;
			entries.push_back(Entry{ temp_key,temp_value });
		}
		int temp_next_bid = int();
		fs.read((char*)&temp_next_bid, sizeof(int));
		next_bid_ = temp_next_bid;
	}//If it's a leaf node.
	else {
		int temp_next_bid = int();
		fs.read((char*)&temp_next_bid, sizeof(int));
		next_bid_ = temp_next_bid;
		for (int i = 0; i < (block_size - 4) / 8; i++) {
			int temp_key = int();
			int temp_value = int();
			fs.read((char*)&temp_key, sizeof(int));
			fs.read((char*)&temp_value, sizeof(int));
			if (temp_key == int() && temp_value == int()) continue;
			entries.push_back(Entry{ temp_key,temp_value });
		}
	}//Otherwise...
}

const Entry& B_tree::Node::SearchEntry(const int& key) const
{
	for (auto& itr : entries)
		if (itr.get_key() == key) return itr;
	throw std::invalid_argument("No existed!");
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

Entry B_tree::Node::CheckAndSplit(std::fstream& fs, const unsigned int& input_bid, const size_t& block_size)
{
	//First, check overflow.
	size_t max_size = ((block_size - 4) / 8) + 1;
	if (max_size > entries.size()) return Entry(int(), int());//크기가 크지 않다면 빠져나간다.

	//Split.
	Node next_node{ input_bid };
	std::vector<Entry>::iterator iter = (max_size % 2) == 1 ? entries.begin() + ((max_size - 1) / 2) : entries.begin() + (max_size / 2);//위치 지정
	Entry temp_entry{ iter.operator*().get_key(), static_cast<int>(input_bid) };
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
	//Write the new block.
	fs.seekp(12 + (input_bid - 1) * block_size);
	next_node.WriteBlock(fs, block_size);
	//Return the node that points the new block.
	return temp_entry;
}

void Entry::WriteEntry(std::fstream& out_file_stream) const
{
	out_file_stream.write((char*)&key_, sizeof(int));
	out_file_stream.write((char*)&value_, sizeof(int));
	out_file_stream.flush();
}

bool B_tree::Insert(std::fstream& fs, const int& key, const int& value)
{
	//Search first.
	BunchOfBlock blocks = std::move(Search(fs, key));
	//Input.
	blocks.back().InsertEntry(key, value);
	//check overflow.
	InsertUtility(blocks, fs);
	UpdateMetaData(fs);
	return true;
}

Entry B_tree::PointSearch(std::fstream& fs, const int& key) const
{
	BunchOfBlock blocks = std::move(Search(fs, key));
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

std::vector<Entry> B_tree::RangeSearch(std::fstream& fs, const int& left_key, const int& right_key) const
{
	BunchOfBlock blocks = std::move(Search(fs, left_key));
	Entries entries;

	for (auto& itr : blocks.back().GetEntries()) {
		if (itr.get_key() >= left_key) { entries.push_back(itr); }
	}
	fs.clear();
	unsigned int next_bid = blocks.back().get_next_bid();
	fs.clear();

	for (bool flag = false; flag == false; ) {
		fs.seekg(12 + (next_bid - 1) * block_size_, std::ios::beg);
		Node temp_node{ next_bid , true };
		temp_node.ReadBlock(fs, block_size_);
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

void B_tree::Print(std::fstream& fs, std::fstream& out_fs) const
{
	std::queue<int> temp_queue;//Queue for ID
	temp_queue.push(root_bid_);
	size_t temp_size = size_t();
	unsigned int temp = unsigned int();//Temp var for ID

	for (unsigned int i = 0; i < depth_; i++) {

		out_fs << "<" << i << ">" << std::endl;
		temp_size = temp_queue.size();
		for (int j = 0; j < temp_size; j++) {
			if (j != 0) out_fs << ", ";
			temp = temp_queue.front();
			temp_queue.pop();
			fs.seekg(12 + (temp - 1) * block_size_);

			Node temp_node{ int() };
			temp_node.ReadBlock(fs, block_size_);
			temp_node.PrintForPrint(out_fs);
			temp_queue.push(temp_node.get_next_bid());
			temp_node.GetAllValuesForPrint(temp_queue);
		}
		out_fs << std::endl;
	}
}


B_tree::BunchOfBlock B_tree::Search(std::fstream& fs, const int& key) const
{
	fs.clear();
	BunchOfBlock blocks;
	SearchUtility(fs, blocks, root_bid_, depth_, key);
	return blocks;
}

void B_tree::SearchUtility(std::fstream& fs, BunchOfBlock& blocks, const unsigned int& current_bid, unsigned int depth, const int& key) const
{
	fs.seekg(12 + (current_bid - 1) * block_size_);
	if (depth == 0) {
		Node temp_node{ current_bid, true };
		temp_node.ReadBlock(fs, block_size_);
		blocks.push_back(temp_node);
		return;
	}
	Node temp_node{ current_bid };
	temp_node.ReadBlock(fs, block_size_);
	blocks.push_back(temp_node);
	int next_bid = blocks.back().SearchEntryForNextId(key);
	SearchUtility(fs, blocks, next_bid, depth - 1, key);
}

void B_tree::InsertUtility(BunchOfBlock& blocks, std::fstream& fs)
{
	Entry temp_entry = blocks.back().CheckAndSplit(fs, current_block_id_, block_size_);
	fs.clear();
	fs.seekp(12 + (blocks.back().get_block_id() - 1) * block_size_);
	if (temp_entry == Entry(int(), int())) {
		blocks.back().WriteBlock(fs, block_size_);
		return;
	}
	else {//If overflow has occurred, update the old block.
		current_block_id_++;
		blocks.back().WriteBlock(fs, block_size_);
		if (blocks.size() <= 1) {//If the root block has overflowed...
			Node root_node{ current_block_id_ , false, blocks.back().get_block_id() };
			root_node.InsertEntry(temp_entry);
			fs.seekp(12 + (root_node.get_block_id() - 1) * block_size_);
			root_node.WriteBlock(fs, block_size_);
			//Update metadata
			fs.seekp(4);
			int temp_bid = root_node.get_block_id();
			int temp_depth = depth_ + 1;
			fs.write((char*)&(temp_bid), sizeof(int));
			fs.write((char*)&(temp_depth), sizeof(int));
			current_block_id_++;
			return;
		}
		blocks.pop_back();
		blocks.back().InsertEntry(temp_entry);
		InsertUtility(blocks, fs);
	}
}

void B_tree::UpdateMetaData(std::fstream& fs)
{
	fs.clear();
	fs.seekg(0, std::ios::beg);
	fs.read((char*)&block_size_, sizeof(int));
	fs.read((char*)&root_bid_, sizeof(int));
	fs.read((char*)&depth_, sizeof(int));
}

bool InsertFromText(std::fstream& fs, std::fstream& fs_second, B_tree& tree) {
	std::string str;
	int key_temp = int(), value_temp = int();
	std::vector<Entry> entries;
	while (fs_second) {
		if (fs_second.eof()) break;
		try
		{
			std::getline(fs_second, str, ',');
			key_temp = std::stoi(str);
			std::getline(fs_second, str, '\n');
			value_temp = std::stoi(str);
		}
		catch (const std::exception&)
		{
			break;
		}
		entries.push_back(Entry{ key_temp, value_temp });
	}
	for (auto& itr : entries) {
		try
		{
			tree.Insert(fs, itr.get_key(), itr.get_value());
		}
		catch (const std::exception&)
		{
			std::cout << "Error!" << std::endl;
			return false;
		}
	}
	return true;
}

bool PointSearchFromText(std::fstream& fs, std::fstream& fs_second, std::string& file_name_for_out, B_tree& tree) {
	std::string str;
	std::vector<int> bunch_of_key;
	while (fs_second) {
		try
		{
			std::getline(fs_second, str);
			bunch_of_key.push_back(std::stoi(str));
		}
		catch (const std::exception&)
		{
			break;
		}
	}
	fs_second.close();
	fs_second.open(file_name_for_out, std::ios::out);
	for (auto& itr : bunch_of_key) {
		try
		{
			Entry entry = tree.PointSearch(fs, itr);
			fs_second << entry.get_key() << ',' << entry.get_value() << std::endl;
		}
		catch (const std::exception&)
		{
			std::cout << "Key number " << itr << " makes an error!" << std::endl;
		}
	}
	return true;
}

bool RangeSearchFromText(std::fstream& fs, std::fstream& fs_second, std::string& file_name_for_out, B_tree& tree) {
	std::string str;
	std::vector<std::tuple<int, int>> bunch_of_keys;
	int left_key = int(), right_key = int();

	while (fs_second) {
		try
		{
			std::getline(fs_second, str, ',');
			left_key = std::stoi(str);
			std::getline(fs_second, str, '\n');
			right_key = std::stoi(str);
			bunch_of_keys.push_back(std::make_tuple(left_key,right_key));
		}
		catch (const std::exception&)
		{
			break;
		}
	}
	fs_second.close();
	fs_second.open(file_name_for_out, std::ios::out);
	for (auto& itr : bunch_of_keys) {
		try
		{
			std::vector<Entry> entries = tree.RangeSearch(fs, std::get<0>(itr), std::get<1>(itr));
			for (auto& itr_2 : entries)
				fs_second << itr_2.get_key() << ',' << itr_2.get_value() << '\t';
		}
		catch (const std::exception&)
		{
			std::cout << "Key number " << std::get<0>(itr) << "," << std::get<1>(itr) << " makes an error!" << std::endl;
		}
		fs_second << std::endl;
	}
	return true;
}

int main() {
	char command;
	B_tree tree;
	std::string file_name;//For btree.bin
	std::string file_name_for_insert;//For txt
	std::string file_name_for_out;//For txt
	std::fstream fs;//For btree.bin
	std::fstream fs_for_other;//For txt

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
		fs.open(file_name, std::ios::binary | std::ios::out | std::ios::trunc);
		if (!fs.is_open()) {
			std::cout << "Can't open the file!" << std::endl;
			return 1;
		}
		CreateBinaryFile(fs, block_size);
		fs.close();
		break;
	case 'i':
		std::cin >> file_name >> file_name_for_insert;
		fs_for_other.open(file_name_for_insert, std::ios::in);
		fs.open(file_name, std::ios::out | std::ios::in | std::ios::binary);
		if (!fs.is_open() || !fs_for_other.is_open()) {
			std::cout << "Can't open the file!" << std::endl;
			return 1;
		}
		tree.UpdateMetaData(fs);
		InsertFromText(fs, fs_for_other, tree);
		fs.close();
		fs_for_other.close();
		break;
	case 's':
		std::cin >> file_name >> file_name_for_insert >> file_name_for_out;
		fs_for_other.open(file_name_for_insert, std::ios::in);
		fs.open(file_name, std::ios::binary | std::ios::in);
		if (!fs.is_open() || !fs_for_other.is_open()) {
			std::cout << "Can't open the file!" << std::endl;
			return 1;
		}
		tree.UpdateMetaData(fs);
		PointSearchFromText(fs, fs_for_other, file_name_for_out, tree);
		fs.close();
		fs_for_other.close();
		break;
	case 'r':
		std::cin >> file_name >> file_name_for_insert >> file_name_for_out;
		fs_for_other.open(file_name_for_insert, std::ios::in);
		fs.open(file_name, std::ios::in | std::ios::binary);
		if (!fs.is_open() || !fs_for_other.is_open()) {
			std::cout << "Can't open the file!" << std::endl;
			return 1;
		}
		tree.UpdateMetaData(fs);
		RangeSearchFromText(fs, fs_for_other, file_name_for_out, tree);
		fs.close();
		fs_for_other.close();
		break;
	case 'p':
		std::cin >> file_name >> file_name_for_insert;
		fs_for_other.open(file_name_for_insert, std::ios::out);
		fs.open(file_name, std::ios::binary | std::ios::in);
		if (!fs.is_open() || !fs_for_other.is_open()) {
			std::cout << "Can't open the file!" << std::endl;
			return 1;
		}
		tree.UpdateMetaData(fs);
		tree.Print(fs, fs_for_other);
		fs.close();
		fs_for_other.close();
		break;
	default:
		std::cout << "Wrong Command!" << std::endl;
		break;
	}
	return 0;
}
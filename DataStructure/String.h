#pragma once

class String {
private:
	using CharUniquePtr = std::unique_ptr<char[]>;
	CharUniquePtr str_;
	//���ڿ��� ������ �����̴�.
	//String�� ����� ������ �߻������� ���ڿ��� ������ �ϰ�����, �����δ� ���� �ϳ��ϳ��� �� char �迭�� �����Ѵ�. ������� ������ ���ڸ� �־� ���ڿ��� ��ó�� �����Ѵ�.
	size_t len_;//���ڿ��� �����̴�.
	size_t capacity_;//�迭�� ũ���̴�.
public://���� ���� ���
	explicit String(const size_t& capacity = 1) : len_(0), capacity_(capacity)
	{
		str_ = std::make_unique<char[]>(capacity_);
		strcpy_s(str_.get(), capacity_, "");
	}//�Ϲ����� �������̴�. Capacity�� �����Ͽ� ������ �ٷ� ������ �� �ִ�.
	String(const char* input_str) : len_(strlen(input_str)), capacity_(len_ + 1)
	{
		str_ = std::make_unique<char[]>(capacity_);
		strcpy_s(str_.get(), capacity_, input_str);
	}//�����ڿ��� ������ ���ڿ��� �ٷ� ������ �� �ִ�.
	String(const String& input_str) :len_(input_str.len_), capacity_(input_str.capacity_)
	{
		str_ = std::make_unique<char[]>(capacity_);
		strcpy_s(str_.get(), capacity_, input_str.str_.get());
	}//���� ������.
	String(String&& input_str) noexcept :len_(input_str.get_len()), capacity_(input_str.get_capacity()) {
		str_ = std::move(input_str.str_);
		input_str.len_ = 0;
		input_str.capacity_ = 1;
		input_str.str_ = std::make_unique<char[]>(input_str.get_capacity());
	}//�̵� ������.
	String& operator=(const char* input_str) {
		len_ = strlen(input_str);
		capacity_ = len_ + 1;
		str_ = std::make_unique<char[]>(capacity_);
		strcpy_s(str_.get(), capacity_, input_str);
		return *this;
	}
	String& operator=(const String& input_str) {
		len_ = input_str.len_;
		capacity_ = input_str.capacity_ + 1;
		str_ = std::make_unique<char[]>(capacity_);
		strcpy_s(str_.get(), capacity_, input_str.str_.get());
		return *this;
	};//���� ������.
	String& operator=(String&& input_str) noexcept {
		len_ = input_str.len_;
		capacity_ = input_str.capacity_;
		str_ = std::move(input_str.str_);
		input_str.len_ = 0;
		input_str.capacity_ = 1;
		input_str.str_ = std::make_unique<char[]>(input_str.get_capacity());
		return *this;
	};//���� �̵� ������.
	~String() {
		
	}
private:
	void SizeCheck();//�� �Լ��� �迭�� ũ�⸦ �˻��Ͽ� ���ڿ��� ���̿� �迭�� ũ�Ⱑ ���� ��� �迭 ������ �ø���. ũ��� ���� �߻��Ǵ� ������ ������ ���´�.
public:
	void Append(const char& input_char);//�ڿ� ���ο� ���ڸ� �߰��Ѵ�.
	void Append(const String& input_str);//�ڿ� ���ο� ���ڿ��� �߰��Ѵ�.
	int Search(const String& input_str) const;//�ش� ���ڿ��� ���� �ִ��� ã�´�. ã���� ���� ��ġ�� ��ȯ�ϰ� ������ -1�� ��ȯ�Ѵ�
	char At(const int& index) const;
	size_t get_len() const { return len_; };
	size_t get_capacity() const { return capacity_; };
	bool Empty() const { return get_len() == 0; }
public://������ �����ε�
	friend std::ostream& operator<<(std::ostream& os, const String& str);
	String operator+(const String& input_str);
	void operator+=(const String& input_str);
	bool operator==(const String& str_to_compare) const;//�� ���ڿ��� ������?
	char& operator[](int index) const;//�����ڸ� ��ȯ�ϱ� ������ ���� ���� ����
};
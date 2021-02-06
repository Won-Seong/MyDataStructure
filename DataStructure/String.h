#pragma once

class String {
private:
	using CharUniquePtr = std::unique_ptr<char[]>;
	CharUniquePtr str_;
	//문자열을 저장할 공간이다.
	//String을 사용할 때에는 추상적으로 문자열로 생각을 하겠지만, 실제로는 문자 하나하나가 들어갈 char 배열을 생성한다. 만들어진 공간에 문자를 넣어 문자열인 것처럼 생각한다.
	size_t len_;//문자열의 길이이다.
	size_t capacity_;//배열의 크기이다.
public://유지 보수 블록
	explicit String(const size_t& capacity = 1) : len_(0), capacity_(capacity)
	{
		str_ = std::make_unique<char[]>(capacity_);
		strcpy_s(str_.get(), capacity_, "");
	}//일반적인 생성자이다. Capacity를 지정하여 공간을 바로 예약할 수 있다.
	String(const char* input_str) : len_(strlen(input_str)), capacity_(len_ + 1)
	{
		str_ = std::make_unique<char[]>(capacity_);
		strcpy_s(str_.get(), capacity_, input_str);
	}//생성자에서 저장할 문자열을 바로 지정할 수 있다.
	String(const String& input_str) :len_(input_str.len_), capacity_(input_str.capacity_)
	{
		str_ = std::make_unique<char[]>(capacity_);
		strcpy_s(str_.get(), capacity_, input_str.str_.get());
	}//복사 생성자.
	String(String&& input_str) noexcept :len_(input_str.get_len()), capacity_(input_str.get_capacity()) {
		str_ = std::move(input_str.str_);
		input_str.len_ = 0;
		input_str.capacity_ = 1;
		input_str.str_ = std::make_unique<char[]>(input_str.get_capacity());
	}//이동 생성자.
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
	};//대입 연산자.
	String& operator=(String&& input_str) noexcept {
		len_ = input_str.len_;
		capacity_ = input_str.capacity_;
		str_ = std::move(input_str.str_);
		input_str.len_ = 0;
		input_str.capacity_ = 1;
		input_str.str_ = std::make_unique<char[]>(input_str.get_capacity());
		return *this;
	};//대입 이동 연산자.
	~String() {
		
	}
private:
	void SizeCheck();//이 함수는 배열의 크기를 검사하여 문자열의 길이와 배열의 크기가 같을 경우 배열 공간을 늘린다. 크기로 인해 발생되는 문제를 사전에 막는다.
public:
	void Append(const char& input_char);//뒤에 새로운 문자를 추가한다.
	void Append(const String& input_str);//뒤에 새로운 문자열을 추가한다.
	int Search(const String& input_str) const;//해당 문자열을 갖고 있는지 찾는다. 찾으면 시작 위치를 반환하고 없으면 -1을 반환한다
	char At(const int& index) const;
	size_t get_len() const { return len_; };
	size_t get_capacity() const { return capacity_; };
	bool Empty() const { return get_len() == 0; }
public://연산자 오버로딩
	friend std::ostream& operator<<(std::ostream& os, const String& str);
	String operator+(const String& input_str);
	void operator+=(const String& input_str);
	bool operator==(const String& str_to_compare) const;//두 문자열이 같은가?
	char& operator[](int index) const;//참조자를 반환하기 때문에 문자 수정 가능
};
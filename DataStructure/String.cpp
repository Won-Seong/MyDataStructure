#include <iostream>
#include <vector>
#include "String.h"

std::ostream& operator<<(std::ostream& os, const String& str) {
	os << str.str_;
	return os;
}

void String::SizeCheck()
{
	while (len_ >= capacity_) {
		capacity_ *= 2;
		CharUniquePtr temp_str = std::make_unique<char[]>(capacity_);
		strcpy_s(temp_str.get(), capacity_, str_.get());
		str_ = std::move(temp_str);
	}
}

void String::Append(const char& input_char)
{
	SizeCheck();
	str_[len_++] = input_char;
}

void String::Append(const String& input_str)
{
	capacity_ += input_str.capacity_;
	len_ += input_str.len_;
	strcat_s(str_.get(), capacity_, input_str.str_.get());
}

String String::operator+(const String& input_str)
{
	String str = *this;
	str.Append(input_str);
	return str;
}

void String::operator+=(const String& input_str)
{
	Append(input_str);
}

bool String::operator==(const String& str_to_compare) const
{
	if (get_len() != str_to_compare.get_len()) return false;
	for (int i = 0; i < get_len(); i++) {
		if (str_[i] != str_to_compare[i]) return false;
	}
	return true;
}

char& String::operator[](int index) const
{
	if (index < 0 || index > len_)
		throw std::out_of_range("Out of range!");
	return str_[index];
}

int String::Search(const String& input_str) const
{
	for (int i = 0; i < len_; i++) {
		if (str_[i] == input_str.str_[0]) {
			CharUniquePtr temp_str = std::make_unique<char[]>(input_str.len_ + 1);
			for (int j = 0; j < input_str.len_; j++) {
				int temp{ i + j };
				temp_str[j] = str_[temp];
			}
			if (strncmp(temp_str.get(), input_str.str_.get(), input_str.len_) == 0)
				return i;
		}
	}
	return -1;
}

char String::At(const int& index) const
{
	if (index < 0 ||index > len_)
		throw std::out_of_range("Out of range!");
	return str_[index];
}

String operator+(const String& str_1, const String& str_2) {
	String str = str_1;
	str.Append(str_2);
	return str;
}

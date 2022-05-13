#pragma once
#include <iostream>
#include <string>
#include <deque>

using namespace std;

class BigInt {
	string num{};
	deque<short>dec;
	bool negative{false};
public:
	BigInt() = default;
	BigInt(std::string num);
	BigInt(deque<short> dec);
	bool operator>(const BigInt& obj) const;
	bool operator<(const BigInt& obj) const;
	BigInt operator+(BigInt &rhs);
	BigInt operator-(BigInt &rhs);
	BigInt operator*(BigInt &rhs);
	BigInt operator/(BigInt& rhs);

	BigInt add(BigInt num);
	//BigInt mult(BigInt num);
	BigInt mult2(BigInt num);
	BigInt sub(BigInt num);
	BigInt div(BigInt num);
	BigInt div2(BigInt num);

	void print_number();
};

#include <iostream>
#include <string>
#include "BigInt.h"

using namespace std;

void variables(string line);
void switches(string num1, string num2, char sign);

int main() {
	cout << "-----------------------------------------------------------------------" << endl;
	cout << "Enter an equation (spaces between numbers and signs)" << endl;
	string line;
	getline(cin, line);
	variables(line);

	return 0;
}

void variables(string line) {
	string num1;
	string num2;
	char sign{};
	int spaces{ 0 };
	bool done{ false };
	for (size_t i{ 0 }; i < line.length(); i++) {
		if (spaces == 2)
			num2 += line.at(i);
		if (line.at(i) == ' ')
			spaces++;
		if (spaces == 0)
			num1 += line.at(i);
		if (spaces == 2 && done == false) {
			sign = line.at(i - 1);
			done = true;
		}
	}
	switches(num1, num2, sign);
}

void switches(string num1, string num2, char sign) {
	BigInt numb1 = num1;
	BigInt numb2 = num2;
	BigInt num3;

	switch (sign) {
	case '+':
		num3 = numb1 + numb2;
		num3.print_number();
		break;
	case '-':
		num3 = numb1 - numb2;
		num3.print_number();
		break;
	case '*':
		num3 = numb1 * numb2;
		num3.print_number();
		break;
	case '/':
		num3 = numb1 / numb2;
		num3.print_number();
		break;
	}
	std::cout << endl;
	main();
}

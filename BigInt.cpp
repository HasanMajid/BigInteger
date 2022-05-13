#include "BigInt.h"

/*
* Authors: Hasan, Alejandro
* Trademark H & A
*/ 

BigInt::BigInt(string num)
	:num(num) {
	for (auto loop : num) {
		if (isdigit(loop)) {
			loop = (short)loop - 48;
			dec.push_front(loop); // this reverses the string but as shorts into a deque
		}
		else if (loop == '-'){
			negative = true;
		}
	}
} 
BigInt::BigInt(deque<short> dec)
	:dec{ dec } {
}

bool BigInt::operator>(const BigInt &obj) const { 
	if (dec.size() > obj.dec.size()){
		return true;
	}
	else if (dec.size() < obj.dec.size()){
		return false;
	}

	for (int x = dec.size()-1; x >= 0; x--){ //321 23
		if (dec.at(x) > obj.dec.at(x))
			return true;
		if (dec.at(x) < obj.dec.at(x)){
			return false;
		}

	}

	return false;

}

bool BigInt::operator<(const BigInt& obj) const { 
	return (obj > *this);
}

BigInt BigInt::operator+(BigInt &rhs) {
	return add(rhs);
}

BigInt BigInt::operator-(BigInt &rhs) {
	return sub(rhs);
}

BigInt BigInt::operator*(BigInt &rhs) {
	return mult2(rhs);
}

BigInt BigInt::operator/(BigInt& rhs) {
	return div2(rhs);
}

BigInt BigInt::add(BigInt num) {
	if (negative && !num.negative){
		BigInt rhs = *this;
		rhs.negative = false;
		BigInt result = num.sub(rhs);
		return result;
	}
	if (!negative && num.negative){
		BigInt rhs = num;
		rhs.negative = false;
		return sub(rhs);
	}

	int carry = 0;
	short temp = 0;
	deque<short> looper;
	deque<short> bigDec;
	deque<short> smallDec;
	if (dec.size() >= num.dec.size()){
		bigDec = dec;
		looper = num.dec;
	}
	else{
		bigDec = num.dec;
		looper = dec;
	}

	for (int i = 0; i < bigDec.size(); i++) {
		if (i >= looper.size()){
			temp = bigDec.at(i) + carry;
			carry = 0;
			if (temp > 9){
				carry = 1;
				temp -= 10;
				looper.push_back(temp);
			}
			else{
				looper.push_back(temp);
			}
		}
		else{
			temp = looper.at(i) + bigDec.at(i) + carry;
			carry = 0;
			if (temp > 9){
				carry = 1;
				temp -= 10;
				looper.at(i) = temp;
			}
			else{
				looper.at(i) = temp;
			}
		}
	}
	if (carry == 1){ 
		looper.push_back(1);
	}

	BigInt result = BigInt(looper);
	if (negative && num.negative) {
		result.negative = true;
	}
	return result;
}

BigInt BigInt::sub(BigInt num) {
	bool neg = false;
	BigInt result; //-10 - (-10) = -10 - 10
	if ((negative && !num.negative) || (!negative && num.negative) ) { //10 - (-20) = 10 + 20 = 20;; -10 - 
		BigInt lhs = *this;
		lhs.negative = false;
		BigInt result = lhs.add(num);
		if (negative)
			result.negative = true;
		return result;
	}
	int carry = 0;
	short temp = 0;
	deque<short> looper;
	deque<short> bigDec;
	if (*this > num) {
		bigDec = dec;
		looper = num.dec;
	}
	else {
		bigDec = num.dec;
		looper = dec;
		neg = true;
	}
	for (int i = 0; i < bigDec.size(); i++) {
		if (i >= looper.size()) {
			temp = bigDec.at(i) - carry;
			carry = 0;
			if (temp < 0) {
				carry = 1;
				//temp -= 10;
				looper.push_back(9);
			}
			else {
				looper.push_back(temp);
			}
		}
		else {
			if (bigDec.at(i) < looper.at(i)) {
				temp = (bigDec.at(i) + 10) - looper.at(i) - carry;
				carry = 1;
			}
			else {
				temp = bigDec.at(i) - looper.at(i) - carry;
				carry = 0;
			}
			looper.at(i) = temp;
		}
	}
	// REMEMBAH, REMOVE ALL leading ZEROS 
	while (looper.back() == 0) {
		looper.pop_back();
	}

	result = BigInt(looper);
	result.negative = neg;
	return result;
}

//BigInt BigInt::mult(BigInt num) {
//	BigInt result("0");
//	BigInt adder(dec);
//	short digit = 1;
//	for (auto x:num.dec) {
//		for (short n = 1; n <= x*digit; n++ ){
//			result = result.add(adder);
//		}
//		digit *= 10;
//	}
//	if ((negative && !num.negative) || (!negative && num.negative)){
//		result.negative = true;
//	}
//	return result;
//}

//// SECOND VERSION OF MULT; MUCH MORE OPTIMIZED THAN THE OTHER ONE /////////
BigInt BigInt::mult2(BigInt num) {
	short carry = 0;
	short temp = 0;
	deque<short> bigDec;
	deque<short> smallDec;
	BigInt result;
	BigInt numb;
	if (dec.size() >= num.dec.size()){
		smallDec = num.dec;
		bigDec = dec;
	}
	else{
		smallDec = dec;
		bigDec = num.dec;
	}

	// looping through each digit; O(m * n) complexity 
	for (int i = 0; i < smallDec.size(); i++) {
		numb = BigInt();
		carry = 0;
		for (int j = 0; j < bigDec.size(); j++){
			temp = (smallDec.at(i) * bigDec.at(j)) + carry;
			carry = 0;
			if (temp > 9){
				carry = floor(temp / 10);
				temp %= 10;
			}
			numb.dec.push_back(temp);
		}
		// if the carry has not been carried, it pushes it back here before the loop is reset
		if (carry ){
			numb.dec.push_back(carry);
		}
		// Shifting the numbers to the left by adding 0s on the right
		for (int x = 0; x < i; x++){
			numb.dec.push_front(0); // maybe this
		}

		result = result + numb;
		numb = BigInt();
	}

	// Checking to see if the result should be negative or not
	if ((negative && !num.negative) || (!negative && num.negative)){
		result.negative = true;
	}
	return result;
}

BigInt BigInt::div(BigInt num) {
	if (num.dec == BigInt("1").dec){
		return *this;
	} 
	if( *this < num){
		return BigInt("0");
	}
	BigInt num1("1");
	BigInt result("1");
	BigInt temp = num;
	while (temp < *this){
		temp = temp + num;
		result = result + num1;
	}
	
	//Subtract one from result if it goes over dividend
	if (temp > *this) 
		result = result - num1;

	//Check for negatives
	if ((negative && !num.negative) || (!negative && num.negative)){
		result.negative = true;
	}
	return result;
}

BigInt BigInt::div2(BigInt num) {
	if (num.dec == BigInt("1").dec){
		return *this;
	} 
	if (num > *this){
		return BigInt("0");
	}
	BigInt result("1");
	BigInt one("1");

	BigInt resNum = (num * result); 
	BigInt counter("0");
	 
	while(resNum < *this ){ //&& counter.dec < num.dec
		if ((num * resNum) > *this ){
			break;
		}
		resNum = num * resNum;
		result = result * num;
		//counter = counter + one;
	}
	while(resNum < *this){
		resNum = resNum + num;
		result = result + one;
	}

	// For when it goes over
	while(resNum > *this){
		result = result - one;
		resNum = resNum - num;
	}

	return result;
}

void BigInt::print_number() {
	auto it = dec.rbegin();
	if (negative == true) {
		cout << '-';
	}
	while (it != dec.rend()) {
		cout << *it;
		it++;
	}
	cout << endl;
}


#include "InputStream.h"

InputStream::InputStream(std::string s) {
	code = s;
	index = 0;
	getChar();
}

InputStream::InputStream(const InputStream &is) {
	code = is.code;
	index = is.index;
	look = is.look;
}

void InputStream::getChar() {
	while (code[index] == ' ' || code[index] == '	') {
		index++;
	}
	look = code[index];
	index++;
}

void InputStream::error(std::string s) {
	std::cout << s << std::endl;
}

void InputStream::abort(std::string s) {
	error(s);
	std::exit(0);
}

void InputStream::expect(std::string s) {
	std::string expect = "Expected: " + s;
	error(expect);
}

void InputStream::match(char c) {
	if (look == c) {
		getChar();
	} else {
		error("" + c);
	}
}

bool InputStream::isAlpha(char c) {
	return (c >= 'A' && c<= 'Z');
}

bool InputStream::isDigit(char c) {
	return (c >= '0' && c<= '9');
}

bool InputStream::isAddOp() {
	return (look == '+' || look == '-');
}

bool InputStream::isMulOp() {
	return (look == '*' || look == '/');
}
char InputStream::getName() {
	char c = look;
	if (!isAlpha(c)) {
		expect("Name");
	}
	getChar();
	return c;
}

char InputStream::getNum() {
	char c = look;
	if (!isDigit(look)) {
		expect("Digit");
	}
	getChar();
	return c;
}

void InputStream::emit(std::string s) {
	std::cout << s;
}

void InputStream::emitln(std::string s) {
	emit(s);
	std::cout << std::endl;
}
